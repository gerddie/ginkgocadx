/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "endpoint.h"
#include <api/controllers/icontroladorlog.h>
#ifdef _WIN32
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#endif

//#define _EP_DEBUG
Endpoint::Endpoint()
{
        m_bool = false;
        m_error_cat = EP_ERROR_NONE;
        m_error_code = 0;
        m_error_str = "";
        m_servfd = -1;
        m_sockfd = -1;
        m_family = AF_INET;
        m_bytes_recv = 0;
        m_server = 0;
        m_bytes_sent = 0;
        m_type = 0;
}

Endpoint::Endpoint( int type, std::string hostname, int service, int options ):Endpoint()
{
        // we could be getting an option instead of a service-
        // because of the Endpoint(int, string) constructor below.
        // option constants DOACCEPT and NOACCEPT are set to
        // non-service ints.
        if( service == DOACCEPT || service == NOACCEPT ) {
                options = service;
        }
        std::ostringstream o;
        o << service;
        Create( type, hostname, o.str(), options );
}

Endpoint::Endpoint( int type, std::string remote, std::string local, int options  ):Endpoint()
{
        Create( type, EndpointAddrlist(remote, "", type),
                EndpointAddrlist(local, "", type), options );
}

Endpoint::Endpoint( int type, std::string address ):Endpoint()
{
        Create( type, address );
}

Endpoint::Endpoint( int type, std::string remotehost, std::string remoteservice,
                    std::string localhost, std::string localservice, int options  ):Endpoint()
{
        Create( type, remotehost, remoteservice, localhost, localservice, options );
}

Endpoint::Endpoint( int type, const EndpointAddrlist& address, int options  ):Endpoint()
{
        Create( type, address, options );
}

Endpoint::Endpoint( int type, const EndpointAddrlist& local, const EndpointAddrlist& remote, int options ):Endpoint()
{
        Create( type, local, remote, options );
}

bool Endpoint::Create( int type, std::string remotehost, std::string remoteservice,
                       std::string localhost, std::string localservice, int options )
{
        return Create( type, EndpointAddrlist(remotehost, remoteservice, type),
                       EndpointAddrlist(localhost, localservice, type), options );
}

bool Endpoint::Create( int type, std::string hostname, std::string service, int options )
{
        // If "service" isn't a service but a host:service, then the two
        // arguments are: remote host+port, local host+port
        if (service.find(":") != std::string::npos)
                return Create( type, EndpointAddrlist(hostname, "", type),
                               EndpointAddrlist(service, "", type), options );

        // Otherwise the two arguments are: host, port
        return Create( type, EndpointAddrlist(hostname, service, type), options );
}

bool Endpoint::Create( int type, const EndpointAddrlist& addr, int options )
{
        EndpointAddrlist local, remote;
        m_type = type & EP_SOCK_MASK;
        m_server = type & SERVER;

        m_bool = false;

        if (m_server) {
                local = addr;
        } else {
                remote = addr;
        }
        return Create( type, remote, local, options );
}

// Main creation function - all Create() calls go here
bool Endpoint::Create( int type, const EndpointAddrlist& _remote, const EndpointAddrlist& _local, int options )
{

        EndpointAddrlist local = _local;
        EndpointAddrlist remote = _remote;
        EndpointAddress try_local, try_remote;
        m_type = type & EP_SOCK_MASK;
        m_server = type & SERVER;
        m_sockfd = m_servfd = -1;

        m_bool = false;
        m_error_cat = EP_ERROR_NONE;
        m_error_code = 0;
        m_error_str = "";

        // Possible combinations of local & remote:
        // LOCAL           REMOTE
        // non-specified   non-specified      invalid!
        // non-specified   specified          ok, common for client connections
        // specified       non-specified      ok, common for server connections
        // specified       specified          ok, rare, used for either

        if (!local && !remote) {
                // Local or remote address failed, find out what the deal is
                // To see if an EndpointAddrlist is valid, don't check m_bool
                // because m_bool=false if error, true if not; instead, check
                // m_addrs to see if its non-null.
                if ( remote.GetAddress() && remote.m_error_cat != EP_ERROR_NONE) {
                        m_error_cat = remote.m_error_cat;
                        m_error_code = remote.m_error_code;
                        m_error_str = remote.m_error_str;
                } else if (local.GetAddress() && local.m_error_code != EP_ERROR_NONE) {
                        m_error_cat = local.m_error_cat;
                        m_error_code = local.m_error_code;
                        m_error_str = local.m_error_str;
                } else {
                        m_error_cat = EP_ERROR_ADDRESS;
                        SetLastError();
                        m_error_str = "Neither remote or local address could be determined.";
                }
                return false;
        }

        if (local) { // Local address specified, bind to ALL local addresses associated with this name
                while ( (try_local = local.GetAddressNext()) ) {
                        m_local = try_local;

                        // If its a raw socket, setup the transport-layer-protocol field
                        if (m_type >= RAW_BASE)
                                try_local.ai_protocol = (m_type >> 1) - (RAW_BASE >> 1);

#ifdef _EP_DEBUG
                        std::cout << "trying to bind " << std::string(try_local) << std::endl;
#endif
                        // If g_raw_sockfd is specified, use it for raw sockets instead of socket()
                        if (m_type >= RAW_BASE && g_raw_sockfd)
                                m_sockfd = g_raw_sockfd;
                        else
                                m_sockfd = socket(try_local.ai_family, try_local.ai_socktype,
                                                  try_local.ai_protocol);

                        if (m_sockfd < 0) {
                                m_error_cat = EP_ERROR_SOCKET;
                                SetLastError();
                                /*
                                std::stringstream ss;
                                ss << "socket error: " << strerror(errno)
                                   << " family=" << try_local.ai_family
                                   << " socktype=" << try_local.ai_socktype
                                   << " protocol=" << try_local.ai_protocol;
                                m_error_str = ss.str();
                                */

                                // don't break; try another
                                continue;
                        }

                        // Set local address, unless its a raw socket
                        if (m_type < RAW_BASE) {
                                int n, yes = 1;
                                n = setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)(&yes), sizeof(int));
#ifdef _EP_DEBUG
                                if( n != 0 ) {
                                        std::cout << n << "--> " << ": " << errno << std::endl;
                                        std::perror("setsockopt");
                                }
#endif
                                n = bind(m_sockfd, try_local.ai_addr, try_local.ai_addrlen);
                                if ( n < 0 ) {
                                        m_error_cat = EP_ERROR_BIND;
                                        SetLastError();
                                        /*
                                        std::stringstream ss;
                                        ss << "bind error " << errno << ": " << strerror(errno) << " " << std::string(m_local);
                                        m_error_str = ss.str();
                                        */
#ifdef _EP_DEBUG
                                        std::cout << "bind error: " << strerror(errno) << " " <<
                                                  std::string(m_local) << std::endl;
#endif
                                } else {
#ifdef _EP_DEBUG
                                        std::cout << "bound!" << std::endl;
#endif
                                        m_bool = true;
                                        break;
                                }
                        }
                }

                // Ok...a local address was given, do a passive open?
                // This is not always the case, a local address can be given and
                // the socket be an active open, although thats rare.
                // - Only TCP servers require listen() and accept()
                if ((type & SERVER) && (type & EP_SOCK_MASK) == TCP && m_bool == true) {
                        // What to do here? Data transfer occurs over newfd,
                        // but the current fd is bound to the local address
                        m_servfd = m_sockfd;
                        m_local_server = m_local;

                        // GW: make the socked invalid, because it is now handled by m_servfd
                        m_sockfd = -1;
                        listen(m_servfd, 10);

                        if( options == DOACCEPT ) {
                                bool returnVal = Accept();
                                m_bool = returnVal;
                        } else {
                                m_bool = true;
                        }

                        //try_remote.Create(sa, SOCK_STREAM);     // TCP
                        //(m_type == UDP ? SOCK_DGRAM : SOCK_STREAM));
                        if (remote) { // Remote address for server. Not used. TODO: Only accept from?
#ifdef _EP_DEBUG
                                std::cout << "GIVEN: " << std::string(remote.GetAddress()) << std::endl;
                                std::cout << "CONNECTED: " << std::string(m_remote) << std::endl;
#endif
                        }
                }
        }

        if (remote) {  // Remote address specified, only connect to ONE
                if ((type & SERVER) == CLIENT) {   // Client = active open to remote
                        // Try to find an address that works, stick with it
                        while( (try_remote = remote.GetAddressNext()) ) {
                                // If its a raw socket, setup the transport-layer-protocol field
                                if (m_type >= RAW_BASE)
                                        try_remote.ai_protocol = (m_type >> 1) - (RAW_BASE >> 1);

#ifdef _EP_DEBUG
                                std::cout << "trying to connect "
                                          << std::string(try_remote) << std::endl;
#endif

                                // Only socket() if doesn't exist yet; it may have been
                                // created and bound in the previous step
                                if (m_sockfd < 0) {
                                        if (m_type >= RAW_BASE && g_raw_sockfd)
                                                m_sockfd = g_raw_sockfd;
                                        else
                                                m_sockfd = socket(try_remote.ai_family, try_remote.ai_socktype,
                                                                  try_remote.ai_protocol);
                                }

                                if (m_sockfd < 0) {
                                        m_error_cat = EP_ERROR_SOCKET;
                                        SetLastError();
                                        /*
                                        std::stringstream ss;
                                        ss << "socket error: " << strerror(errno)
                                           << " family=" << try_remote.ai_family
                                           << " socktype=" << try_remote.ai_socktype
                                           << " protocol=" << try_remote.ai_protocol;
                                        m_error_str = ss.str();
                                        */
                                        continue;
                                }
                                if (m_type < RAW_BASE) { // only if not raw (will be in header)
                                        if (connect(m_sockfd, try_remote.ai_addr, try_remote.ai_addrlen) < 0) {
                                                std::string str("connect(): ");
                                                perror((str + try_remote.IP() + " [" + try_remote.DNS() + "]: " + try_remote.Port()).c_str());
                                                m_error_cat = EP_ERROR_CONNECT;
                                                SetLastError();
                                                /*
                                                std::stringstream ss;
                                                ss << "connect error: " << strerror(errno);
                                                m_error_str = ss.str();
                                                */
                                        } else {
#ifdef _EP_DEBUG
                                                std::cout << "connected!" << std::endl;
#endif
                                                m_bool = true;
                                                m_remote = try_remote;
                                                break;
                                        }
                                } else {
                                        m_bool = true;
                                        m_remote = try_remote; // raw sockets will be first(XXX:IPv4)
                                        break;
                                }
                        }

                        // we were not able to establishe a connection
                        if (m_sockfd < 0) {
                                LOG_ERROR("Net", "Unable to open socket");
                                m_bool = false;
                                return false;
                        }

                        // Get local socket name = local address
                        sockaddr sa;
#ifdef _WIN32
                        int n;
#else
                        unsigned int n;
#endif
                        n = sizeof(sockaddr);
                        if (getsockname(m_sockfd, &sa, &n) == 0) {
                                if (!m_local && (type & EP_SOCK_MASK) == TCP)
                                        m_local = EndpointAddress(&sa, SOCK_STREAM);
                                else if (!m_local && (type & EP_SOCK_MASK) == UDP)
                                        m_local = EndpointAddress(&sa, SOCK_STREAM);
                                else if (!m_local && (type & EP_SOCK_MASK) >= RAW_BASE)
                                        m_local = EndpointAddress(&sa, SOCK_STREAM);
                        } else {
                                LOG_ERROR("Net", "Unable to acquire socket name")
                        }

                        // Don't need to getpeername() because its specified

                } else { // not CLIENT (i.e., SERVER)
#ifdef _EP_DEBUG
                        std::cout << "ignoring remote address for server" << std::endl;
#endif
                }
        }

        // Always set IP_HDRINCL on raw sockets so we can write the IP header
        if (m_bool && (m_type >= RAW_BASE)) {
                int on = 1;
                if (setsockopt(m_sockfd, IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof(on)) < 0) {
                        m_error_cat = EP_ERROR_SETSOCKOPT;
                        SetLastError();
                        //m_error_str = "setsockopt error: " + (std::string)strerror(errno);
                }
        }
        return m_bool;
}


bool Endpoint::Accept()
{
        // Ok...a local address was given, do a passive open?
        // This is not always the case, a local address can be given and
        // the socket be an active open, although thats rare.
        // - Only TCP servers require listen() and accept()
        if( (m_server & SERVER) && (m_type & EP_SOCK_MASK) == TCP && m_bool == true && m_servfd >= 0 ) {
                // Don't need to getsockname() because local endpoint is given
                // Passive open (accept), returned sockaddr will be peer name
                // so don't need getpeername()

                // Clear out the connection socket so we don't abandon an open socket.
                Close();

                sockaddr saAcceptInfo;
                sockaddr* sa = &saAcceptInfo;

#ifdef _WIN32
                int n;
#else
                unsigned int n;
#endif
                n = sizeof(sockaddr);


                int newfd = accept(m_servfd, sa, &n);
                if (newfd >= 0) {
                        m_sockfd = newfd;
#ifdef _EP_DEBUG
                        std::cout << "accepted!" << std::endl;
#endif
                        // Set remote address to who connected.
                        // accept() kindly returns this value (getpeername() doesn't seem to work)
                        m_remote = EndpointAddress(sa, m_type);

                        // fill in our local address info.
                        // cannot do this until 1) the connection is accepted
                        // and 2) until sa* is finished being used (stores
                        // the remote address).
                        if (getsockname( m_sockfd, sa, &n ) != 0) {
                                m_local = EndpointAddress(sa, m_type);
                        } else {
                                LOG_ERROR("Net", "Unable to acquire socket name:" << strerror(errno));
                        }

                        return true;
                } else {
                        // accept wasn't successful
                        m_error_cat = EP_ERROR_ACCEPT;
                        m_bool = false;
                        SetLastError();
                        /*
                        std::stringstream ss;
                        ss << "accept() failed: " << strerror(errno);
                        m_error_str = ss.str();
                        */
                }
        }
        return false;
}


int Endpoint::g_bytes_sent = 0;
int Endpoint::g_bytes_recv = 0;

// IP headers for raw sockets
struct ep_iphdr {
        u_int8_t ver_ihl;    // version + internet header length (or vice versa)
        u_int8_t dscp;       // diff. services code point, was type of service (tos)
        u_int16_t tot_len;
        u_int16_t id;
        u_int16_t frag_off;
        u_int8_t ttl;
        u_int8_t protocol;
        u_int16_t check;
        u_int32_t saddr;
        u_int32_t daddr;
        /*The options start here. */
};

// Pseudo-header for checksum purposes
struct ep_pseudohdr {
        u_int32_t       saddr;
        u_int32_t       daddr;
        u_int8_t        zero;
        u_int8_t        protocol;
        u_int16_t       length;
};

// Based on BSD's
struct ep_udphdr {
        u_short sport;               /* source port */
        u_short dport;               /* destination port */
        u_short ulen;                /* udp length */
        u_short sum;                 /* udp checksum */
};

struct ep_icmphdr {
        u_int8_t  type;
        u_int8_t  code;
        u_int16_t sum;
};



int Endpoint::Send(const void *msg, int len, int /*flags*/)
{
        unsigned int bytes_sent;
        bytes_sent = send(m_sockfd, (const char*)msg, len, 0);
        if( bytes_sent == (unsigned int)-1||bytes_sent==(unsigned int)0) { // error/ closed
                m_bool = false;
                m_error_cat = EP_ERROR_SEND;
                SetLastError();
                return bytes_sent;
        }

        AddBytesSent(bytes_sent);
        return bytes_sent;
}

// Write all bytes in data
// Returns number of bytes written
int Endpoint::Write(std::string data)
{
        unsigned int bytes_left, bytes_sent;
        bytes_left = data.length();

        if( !m_bool ) {
                return -1;
        }

        // Do not actually send 0 bytes, doing this will kill connection.
        if( bytes_left == 0 ) {
                return 0;
        }

        // If raw socket, include the IP header, unless its RAW_RAW
        // (Note: raw sockets are IPv4-only, use datalink access for raw IPv6)
        if (m_type >= RAW_BASE && m_type != RAW_RAW) {
                char* buff = new char[8192];   // TODO: MTU
                char* pseudobuff = new char[8192];
                int iphdrsz = 20;              // No IP options, 20 byte IP header
                int layer4sz;                  // Layer 4 (TCP, UDP, etc.) hdr size

                switch (m_type) {
                case RAW_UDP:
                        layer4sz = 8;
                        break;
                case RAW_ICMP:
                        layer4sz = 4;
                        break;
                default:
                        layer4sz = 0;
                        break;
                }

                // Copy data to IP buff
                memcpy(buff + iphdrsz + layer4sz, data.data(), data.length());

                // Build an IP header. Always do this if IP_HDRINCL is set.
                ep_iphdr* iphdr = (struct ep_iphdr*)buff;

#if __BYTE_ORDER == __LITTLE_ENDIAN
                iphdr->ver_ihl = (u_int8_t) (iphdrsz >> 2 | 0x40);
#elif __BYTE_ORDER == __BIG_ENDIAN
                iphdr->ver_ihl = (u_int8_t) ( ((iphdrsz >> 2) << 4) | 0x4 );     // really ihl_ver
#else
# error "Please fix <bits/endian.h>"
#endif

                //iphdr->ihl = iphdrsz >> 2;   //  Header Length(=20 bytes)
                //iphdr->version = 4;   // IPv4

                iphdr->dscp = 8;      // Differentiated Services Field (?)
                //iphdr->tot_len = 20 + (m_type == RAW_UDP ? 8 : 0) + data.length()
                iphdr->tot_len = (u_int16_t) (iphdrsz + layer4sz + data.length());
                iphdr->id = 0;        // IPID, let kernel fill it in (randomize??)
                iphdr->frag_off = 0;  // Fragment offset & flags = not fragmented
                iphdr->ttl = 128;     // Time to Live
                iphdr->protocol = (u_int8_t) ( (m_type >> 1) - (RAW_BASE >> 1) );   // Next protocol
                iphdr->check = 0;     // Checksum, filled in by kernel
                iphdr->saddr = inet_addr(m_local.IP().c_str());      // Source &
                iphdr->daddr = inet_addr(m_remote.IP().c_str());     // dest address

                if (m_type == RAW_UDP) {
                        ep_udphdr* udphdr = (struct ep_udphdr*)(buff + iphdrsz);
                        ep_pseudohdr* pseudohdr = (struct ep_pseudohdr*)pseudobuff;

                        udphdr->sport = htons(m_local.PortNumber());
                        udphdr->dport = htons(m_remote.PortNumber());
                        udphdr->sum = 0;
                        udphdr->ulen = (u_short) htons((u_short)data.length() + 8);

                        // Copy UDP header and data into pseudo header
                        memcpy(pseudobuff + sizeof(struct ep_pseudohdr), buff + iphdrsz,
                               layer4sz + data.length());

                        pseudohdr->saddr = iphdr->saddr;
                        pseudohdr->daddr = iphdr->daddr;
                        pseudohdr->zero  = 0;
                        pseudohdr->protocol = IPPROTO_UDP;
                        pseudohdr->length = udphdr->ulen;

                        // XXX: Incorrect checksum if source port is 0? Inconsistantly.
                        // Or if we specify 0 as src addr so it gets filled in
                        udphdr->sum = Endpoint::in_cksum((u_short*)pseudobuff,
                                                         sizeof(struct ep_pseudohdr) + layer4sz + data.length());

                } else if (m_type == RAW_ICMP) {
                        ep_icmphdr* icmphdr = (struct ep_icmphdr*)(buff + iphdrsz);

                        // Port number, upper 8 bits are type, lower are code XXX:ENDIAN
                        icmphdr->type  = (u_int8_t) (m_remote.PortNumber() >> 8);
                        icmphdr->code = (u_int8_t) (m_remote.PortNumber() & 0xff);
                        icmphdr->sum  = 0;

                        icmphdr->sum = Endpoint::in_cksum((u_short*)icmphdr,
                                                          layer4sz + data.length());
                }

                int err = sendto(m_sockfd, buff, data.length()+iphdrsz+layer4sz, 0,
                                 (sockaddr*)m_remote.ai_addr, m_remote.ai_addrlen);

                if (err < 0) {
#ifdef _EP_DEBUG
                        std::cout << "send error: " << strerror(errno)
                                  << std::endl;
#endif
                        m_bool = false;
                        m_error_cat = EP_ERROR_SEND;
                        SetLastError();
                }

                delete[] buff;
                delete[] pseudobuff;

                return err;
        }

        // Loop until either a) all bytes are sent b) no more bytes could be sent
        do {
                // XXX: shall flags be MSG_WAITALL, if supported?
                bytes_sent = send(m_sockfd, ( data.data() + data.length() - bytes_left ), bytes_left, 0);

                if (bytes_sent == (unsigned)-1||bytes_sent==(unsigned)0) { // error/ closed
                        m_bool = false;
                        m_error_cat = EP_ERROR_SEND;
                        SetLastError();
                        return bytes_sent;
                }

                //m_bytes_sent += bytes_sent;
                //Endpoint::g_bytes_sent += bytes_sent;
                AddBytesSent(bytes_sent);

                bytes_left -= bytes_sent;
        } while(bytes_left > 0 && bytes_sent != 0);

        if (bytes_sent != data.length()) {
#ifdef _EP_DEBUG
                std::cout << "Socket::Write() partial write, " << bytes_sent
                          << " != " <<
                          data.length() << std::endl;
#endif
                return bytes_sent;
        }
        return bytes_sent;
}


int Endpoint::Recv(void *buf, int len, unsigned int flags)
{
        int returnVal;
        returnVal = recv(m_sockfd, (char *)buf, len, flags);

        // Check for conditions that may terminate reading prematurely
        if (returnVal == -1) {      // -1  is error or EWOULDBLOCK
                // Check if caller wanted non-blocking, and we got it
                if (/*!m_blocking && */errno == EAGAIN)
                        return returnVal;

                // Otherwise, this is an error
                m_bool = false;
                m_error_cat = EP_ERROR_RECV;
                SetLastError();
                return -1 * returnVal;   // Error condition
        } else if (returnVal == 0) { // 0 is EOF
                return returnVal;
        }

        AddBytesRecv(returnVal);
        return returnVal;
}

// Read bytes_to_read bytes into data, return FALSE if end-of-file
// bytes_to_read=x can be either:
// x>0 - read AT LEAST x bytes, blocking
// x<0 - read AT MOST -x bytes, blocking
// x=0 - Non-blocking I/O (not supported)
// (This is copied almost verbatim from my own IPv4-only Socket)
int Endpoint::Read(int bytes_to_read, std::string& data)
{
        int bytes_left      = bytes_to_read;
        int bytes_read      = 0;
        int bytes_just_read = 0;
        //bool eof            = false;
        bool read_all       = true;
        //bool ret            = true;

        if (bytes_to_read == 0) {            // Non-blocking I/O (not supported)
#ifdef _EP_DEBUG
                std::cout << "Error, can't read 0 bytes" << std::endl;
#endif
        }

        if (bytes_to_read < 0) {     // Negatives = don't read all, do one call
                bytes_left = -bytes_to_read;
                read_all = false;
        }

        do {
                char* buf = new char[bytes_left];
                if (!buf) {
#ifdef _EP_DEBUG
                        std::cout << "OUT OF MEMORY!" << std::endl;
#endif
                        // force-close connection and return 0
                        m_bool = false;
                        m_error_cat = EP_ERROR_RECV;
                        m_error_code = 0;
                        m_error_str = "";
                        Close();
                        CloseServer();
                        return 0;
                }

                bytes_just_read = recv(m_sockfd, (char*)buf, bytes_left, 0);
                if (bytes_just_read > 0) {
                        bytes_read += bytes_just_read;
                        data += std::string(buf, bytes_just_read);
                }
                delete[] buf;      // TODO: Less inefficient

                // Check for conditions that may terminate reading prematurely
                if (bytes_just_read == -1) {      // -1  is error or EWOULDBLOCK
                        // Check if caller wanted non-blocking, and we got it
                        if (/*!m_blocking && */errno == EAGAIN)
                                return bytes_read;

                        // Otherwise, this is an error
                        m_bool = false;
                        m_error_cat = EP_ERROR_RECV;
                        SetLastError();
                        return -bytes_read;   // Error condition
                        break;
                } else if (bytes_just_read == 0)  // 0 is EOF
                        return bytes_read;

                bytes_left -= bytes_just_read;

                // If read_all is set (the default, for + values), loop again.
                // If not set (for - bytes_to_read values), don't loop anymore
                if (!read_all)
                        break;
        } while(bytes_left != 0);

        //m_bytes_recv += data.length();
        //Endpoint::g_bytes_recv += data.length();
        AddBytesRecv(data.length());

        return bytes_read;
}


// Read bytes_to_read bytes into data, return FALSE if end-of-file
// bytes_to_read=x can be either:
// x>0 - read AT LEAST x bytes, blocking
// x<0 - read AT MOST -x bytes, blocking
// x=0 - Non-blocking I/O (not supported)
// (This is copied almost verbatim from my own IPv4-only Socket)
int Endpoint::Read(int bytes_to_read, char* buffer)
{
        int bytes_left      = bytes_to_read;
        int bytes_read      = 0;
        int bytes_just_read = 0;
        //bool eof            = false;
        bool read_all       = true;
        //bool ret            = true;

        if (bytes_to_read == 0) {            // Non-blocking I/O (not supported)
#ifdef _EP_DEBUG
                std::cout << "Error, can't read 0 bytes" << std::endl;
#endif
        }

        if (bytes_to_read < 0) {     // Negatives = don't read all, do one call
                bytes_left = -bytes_to_read;
                read_all = false;
        }

        do {
                char* buf = new char[bytes_left];
                if (!buf) {
#ifdef _EP_DEBUG
                        std::cout << "OUT OF MEMORY!" << std::endl;
#endif
                        // force-close connection and return 0
                        m_bool = false;
                        m_error_cat = EP_ERROR_RECV;
                        m_error_code = 0;
                        m_error_str = "";
                        Close();
                        CloseServer();
                        return 0;
                }

                bytes_just_read = recv(m_sockfd, (char*)buf, bytes_left, 0);
                if (bytes_just_read > 0) {
                        bytes_read += bytes_just_read;
                        memcpy(buffer, buf, bytes_just_read);
                }
                delete[] buf;      // TODO: Less inefficient

                // Check for conditions that may terminate reading prematurely
                if (bytes_just_read == -1) {      // -1  is error or EWOULDBLOCK
                        // Check if caller wanted non-blocking, and we got it
                        if (/*!m_blocking && */errno == EAGAIN)
                                return bytes_read;

                        // Otherwise, this is an error
                        m_bool = false;
                        m_error_cat = EP_ERROR_RECV;
                        SetLastError();
                        return -bytes_read;   // Error condition
                        break;
                } else if (bytes_just_read == 0)  // 0 is EOF
                        return bytes_read;

                bytes_left -= bytes_just_read;

                // If read_all is set (the default, for + values), loop again.
                // If not set (for - bytes_to_read values), don't loop anymore
                if (!read_all)
                        break;
        } while(bytes_left != 0);

        //m_bytes_recv += data.length();
        //Endpoint::g_bytes_recv += data.length();
        AddBytesRecv(bytes_read);

        return bytes_read;
}

int Endpoint::g_raw_sockfd = 0;        // create new raw socket fd for each instance by default

void Endpoint::setup_raw(std::string argv0)
{
        Initialize();   // static method, so not normally called

        // The "launch" program passes a raw socket in this environment variable,
        // so only *it* (launch) is setuid root, thus increasing safety
#if defined (_WINDOWS)
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif
        if (getenv("RAWSOCKFD")) {
                Endpoint::set_raw_sockfd(std::atoi(std::getenv("RAWSOCKFD")));
#ifdef _EP_DEBUG
                std::cout << "Using raw #" << std::atoi(std::getenv("RAWSOCKFD")) << std::endl;
#endif
                return;
        }
#if defined (_WINDOWS)
#pragma warning ( pop)
#endif

        int rawfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

        // Don't bother getuid() on Win32
#ifdef _WIN32
#define getuid() -1
#endif

        if (rawfd < 0) {
#ifdef _EP_DEBUG
                std::cout << "Couldn't create raw socket." << rawfd << std::endl;
#endif
#ifndef _WIN32
                if (getuid() != 0) {
                        std::cout << "You are not running as root. Please set the setuid "
                                  "bit by typing:\nchmod 4755 " << argv0 << std::endl
                                  << "and try again. Root priviledges will be dropped "
                                  "after the raw socket is created." << std::endl;
                } else {
                        std::cout << "You ARE running as root, but creation of the raw "
                                  "socket failed anyways.\n\nMaybe check your TCP/IP "
                                  "stack?" << std::endl;
                }
#else
                // TODO: Use winpcap's pcap_send, works on all platforms
                std::cout << "You are running the Win32 build.\n" << std::endl
                          << "Please note that only Windows XP/2000 are supported."
                          << std::endl << std::endl
                          << "Windows NT3/Me/98/95 do not have the required "
                          << "capabilities to use raw sockets." << std::endl
                          << "Also, you must have administrator priviledges. Under NT4, you can run\n"
                          << "ntadmraw.reg to enable non-admin raw sockets. Under XP Home Edition, \n"
                          << "all users are Admin by default and should have no problems. Test if \n"
                          << "you have raw socket capabilities by going here: " << std::endl
                          << "http://news.grc.com/files/sockettome.exe - make sure this works"<<std::endl
                          << "(Note: do not install GRC's SocketLock; this program will not work)."
                          << std::endl;
#endif
                //exit(-1);
                return;
        }

        // Comment this out when debugging, then you can do: sudo gdb sumiserv
#if !defined(_WINDOWS)
        if (getuid() == 0) {
                std::cout << "Please run with effective user root, and a real non-root user." << std::endl
                          << "This is required for security purposes.\n" << std::endl
                          << "To do this, set the setuid bit on this file: chmod 4755 "
                          << argv0 << std::endl
                          << "Also, make sure the owner is root: chown root " << argv0
                          << std::endl;
                //exit(-2);
                close(rawfd);
                return;
        }
#endif

        // Set to real user ID so any further bugs won't compromise root
#ifndef _WIN32
        if (setuid(getuid()) < 0) {
                LOG_ERROR("Net", "Error dropping privileges:" << strerror(errno));
        }

#endif

        Endpoint::set_raw_sockfd(rawfd);

        std::cout << "Now running as user " << getuid() << " with socket "
                  << Endpoint::get_raw_sockfd() << std::endl;
}

// Initialize system-wide socket library if necessary
void Endpoint::Initialize()
{

}

bool Endpoint::Close()
{
        EndpointAddress blank;
        m_remote = blank;
        if( m_sockfd >= 0 ) {
#ifdef _WIN32
                closesocket ( m_sockfd );
#else
                close ( m_sockfd );
#endif
        }
        m_sockfd = -1;
        return true;
}

void Endpoint::Shutdown(EP_Mode mode)
{
        switch(mode) {
        case INPUT:
                shutdown(m_sockfd, 0);
                break;
        case OUTPUT:
                shutdown(m_sockfd, 1);
                break;
        case BOTH:
                shutdown(m_sockfd, 2);
                break;
        }
}

void Endpoint::ShutdownSocket(int socketID, EP_Mode mode)
{
        switch(mode) {
        case INPUT:
                shutdown(socketID, 0);
                break;
        case OUTPUT:
                shutdown(socketID, 1);
                break;
        case BOTH:
                shutdown(socketID, 2);
                break;
        }
}

bool Endpoint::CloseServer()
{
        if( m_servfd >= 0 ) {
#ifdef _WIN32
                closesocket ( m_servfd );
#else
                close ( m_servfd );
#endif
        }
        m_servfd = -1;
        return true;
}

#ifdef _WIN32

std::string convertUTF16toUTF8(const wchar_t* source, unsigned sourceLength)
{
#if defined(WIN32) && !defined(__CYGWIN__)
        if (sourceLength == 0) {
                return std::string();
        }

        int destLen = WideCharToMultiByte(CP_UTF8, 0, source, sourceLength, 0, 0, 0, 0);
        if (destLen <= 0) {
                std::cerr << "Cannot convert UTF-16 string to UTF-8." << std::endl;
                return std::string();
        }

        std::string sDest(destLen, '\0');
        destLen = WideCharToMultiByte(CP_UTF8, 0, source, sourceLength, &sDest[0], destLen, 0, 0);

        if (destLen <= 0) {
                std::cerr << "Cannot convert UTF-16 string to UTF-8." << std::endl;
                return std::string();
        }

        return sDest;
#else
        //TODO: Implement for other platforms
        osg::notify(osg::WARN) << "ConvertUTF16toUTF8 not implemented." << std::endl;
        return std::string();
#endif
}


#endif


Endpoint::~Endpoint()
{
        Close();
        CloseServer();
}

int Endpoint::SetLastError()
{
#ifdef _WIN32
        m_error_code = WSAGetLastError();

        LPVOID lpMsgBuf;
        FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                m_error_code,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf,
                0,
                NULL );

        std::wstring wstr = (WCHAR*)lpMsgBuf;

        m_error_str = convertUTF16toUTF8(wstr.c_str(), wstr.size()); //.assign((const char*)lpMsgBuf);
        LocalFree(lpMsgBuf);
#else
        m_error_code = errno;
        m_error_str =  strerror(errno);
#endif
        return m_error_code;
}


/* Obtained from source code from _Unix Network Programming_ by *
 * W. Richard Stevens. www.kohala.com/start/unpv12e.html for download. */

unsigned short
Endpoint::in_cksum(unsigned short *addr, int len)
{
        int				nleft = len;
        int				sum = 0;
        unsigned short	*w = addr;
        unsigned short	answer = 0;

        /*
         * Our algorithm is simple, using a 32 bit accumulator (sum), we add
         * sequential 16 bit words to it, and at the end, fold back all the
         * carry bits from the top 16 bits into the lower 16 bits.
         */
        while (nleft > 1)  {
                sum += *w++;
                nleft -= 2;
        }

        /* 4mop up an odd byte, if necessary */
        if (nleft == 1) {
                *(unsigned char *)(&answer) = *(unsigned char *)w ;
                sum += answer;
        }

        /* 4add back carry outs from top 16 bits to low 16 bits */
        sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
        sum += (sum >> 16);			/* add carry */
        answer = (unsigned short)~sum;				/* truncate to 16 bits */
        return(answer);
}

/*
    int join(std::string mcastaddr, std::string interface = "0");
    int leave(std::string mcastaddr, std::string interface = "0");
    int mcast_options(std::string outgoing_if, int ttl = 1, int loop = 1);
*/

#if 0
#include <net/if.h>        // for ifreq
#include <sys/sockio.h>    // for SIOCGIFADDR

int Endpoint::join(std::string mcastaddr, std::string ifname)
{
        int sockfd = m_sockfd;
        EndpointAddrlist addrlist(mcastaddr + ":0");
        sockaddr sa_referent = addrlist.GetAddress();
        sockaddr* sa = &sa_referent;
        int n;

// from UNP, slightly modified

        switch (sa->sa_family) {
        case AF_INET: {
                struct ip_mreq          mreq;
                struct ifreq            ifreq;

                memcpy(&mreq.imr_multiaddr,
                       &((struct sockaddr_in *) sa)->sin_addr,
                       sizeof(struct in_addr));

                /* if (ifname.length()) {
                     // modified
                     strncpy(ifreq.ifr_name, ifname.c_str(), IFNAMSIZ);
                     if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
                     {
                #ifdef _EP_DEBUG
                            std::cout << "ioctl: SIOCGIFADDR failed"
                                      << std::endl;
                #endif
                            return(-1);
                     }
                     memcpy(&mreq.imr_interface,
                           &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr,
                           sizeof(struct in_addr));
                } else */
                mreq.imr_interface.s_addr = htonl(INADDR_ANY);

                n = (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                                &mreq, sizeof(mreq)));
                if (n != 0) {
#ifdef _EP_DEBUG
                        std::cout << "setsockopt join: " <<
                                  strerror(errno) << std::endl;
#endif
                        return n;
                }
                return 0;
        }
                /* end mcast_join1 */

                /* include mcast_join2 */
#ifdef  IPV6
        case AF_INET6: {
                struct ipv6_mreq        mreq6;

                memcpy(&mreq6.ipv6mr_multiaddr,
                       &((struct sockaddr_in6 *) sa)->sin6_addr,
                       sizeof(struct in6_addr));

                if (ifindex > 0)
                        mreq6.ipv6mr_interface = ifindex;
                else if (ifname != NULL)
                        if ( (mreq6.ipv6mr_interface = if_nametoindex(ifname)) == 0) {
                                errno = ENXIO;  /* if name not found */
#ifdef _EP_DEBUG
                                std::cout << "ENXIO ifname not found"
                                          << std::endl;
#endif
                                return(-1);
                        } else
                                mreq6.ipv6mr_interface = 0;

                // XXX: IPV6_ADD_MEMBERSHIP -> IPV6_JOIN_GROUP
                n = setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP,
                               &mreq6, sizeof(mreq6));
                if (n != 0) {
#ifdef _EP_DEBUG
                        std::cout << "setsockopt failure: " << strerror(errno)
                                  << std::endl;
#endif
                }
#ifdef _EP_DEBUG
                std::cout << strerror(errno) << std::endl;
#endif
                return n;
        }
#endif

        default:
                errno = EPROTONOSUPPORT;
#ifdef _EP_DEBUG
                std::cout << "join: protocol not supported" << std::endl;
#endif
                return(-1);
        }
}



*/
#endif
