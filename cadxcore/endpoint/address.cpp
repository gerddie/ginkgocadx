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
#include <sstream>       // for numeric conversion

#include <string>
#include <cstdlib>

#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#include "protocols.h"
#include "address.h"
#ifdef _WIN32
#include "inet_ntop.h"
#endif


//******************* EndpointAddress ***********************
// A single address


// Return literal IP address
std::string EndpointAddress::IP()
{
        char str[128];
        addrinfo* addr = this;

        if (!m_Inicializada)
                return "???";

        memset(&str, 0, 128);
        // This is nasty, but necessary
        // From, I think, W. Richard Steven's work
#ifdef _WIN32
        if (addr->ai_family == AF_INET) {
                myinet_ntop(AF_INET, &(((sockaddr_in*)addr->ai_addr)->sin_addr),
                            str, sizeof(str));
        } else if (addr->ai_family == AF_INET6) {
                myinet_ntop(AF_INET6, &(((sockaddr_in6*)addr->ai_addr)->sin6_addr),
                            str, sizeof(str));
        } else {
                return "";
        }
#else
        if (addr->ai_family == AF_INET) {
                inet_ntop(AF_INET, &(((sockaddr_in*)addr->ai_addr)->sin_addr),
                          str, sizeof(str));
        } else if (addr->ai_family == AF_INET6) {
                inet_ntop(AF_INET6, &(((sockaddr_in6*)addr->ai_addr)->sin6_addr),
                          str, sizeof(str));
        } else {
                return "";
        }
#endif

        return std::string(str);
}

// Return canonical DNS name, empty string if none
std::string EndpointAddress::DNS()
{
        return ai_canonname ? ai_canonname : "";
}

// Return canonical DNS name if has one, else IP
std::string EndpointAddress::Name()
{
        if (DNS().length())
                return DNS();
        else
                return IP();
}


// Return stringified port number
std::string EndpointAddress::Port()
{
        unsigned short port = PortNumber();
        std::string str;
        std::ostringstream o;

        if (!(o << port))
                return "???";
        return o.str();
}

// Return integer port number
unsigned short EndpointAddress::PortNumber()
{
        unsigned short port;

        if (!m_Inicializada)
                return 0;

        if (ai_family == AF_INET)
                port = ((sockaddr_in*)ai_addr)->sin_port;
        else if (ai_family == AF_INET6)
                port = ((sockaddr_in6*)ai_addr)->sin6_port;
        else
                port = 0;

        return ntohs(port);
}

/*int EndpointAddress::SetPort(int port)
{
    if (ai_family == AF_INET)
        ((sockaddr_in*)ai_addr)->sin_port = htons(port);
    else if (ai_family == AF_INET6)
        ((sockaddr_in6*)ai_addr)->sin6_port = htons(port);
    else
    {
#ifdef _EP_DEBUG
        std::cout << "EndpointAddress::SetPort()-addr family?" << std::endl;
#endif
    }
    return port;
}*/

// Return IP address, colon, port
std::string EndpointAddress::IPPort()
{
        if( this == 0 || !m_Inicializada )
                return "(unconnected)";

        std::string ip = IP();

        if (ip.length() == 0)
                return "(invalid)";

        // If address has colons (as with IPv6), need to put in brackets
        if (ip.find(":") != std::string::npos)
                ip = "[" + ip + "]";

        return ip + ":" + Port();
}

// Return IP address, colon, port
EndpointAddress::operator std::string()
{
        return IPPort();
}

/****************************************************/
// Function Added by John Medema, 05/05.

// ai_canonname is the C Pointer that
// normally holds the DNS Canonical Name.

// dynamicMemLoc is a pointer that holds
// the address for the dynamically
// (heap-allocated) memory.  ai_canonname
// should also be pointing to this, but
// I wanted a guaranteed pointer the
// dynamic memory.

// dynamicMemData is a string that holds
// a copy of the data pointed to by
// dynamicMemLoc.

// Process:  When the EndpointAddress
// is first instantiated, dynamicMemData
// is automatically set to "".  The only
// time dynamicMemData is not "" is when
// there has been dynamically allocated
// memory.  (A String is the smallest
// C/C++ object that is initialized to a
// set value.  No other objects zero out
// their value upon initialization.)
// As long as we need to have something
// in dynamicMemData, we put in a copy of
// the C String data.

// I chose to use a string/pointer combination
// because the Constuctor/Create()/Clone()
// functions can be used manually (not just
// for object initialization).


//******************* EndpointAddrlist ***********************

void EndpointAddrlist::Copy(const EndpointAddrlist & original)
{
        m_error_code = original.m_error_code;
        m_error_str = original.m_error_str;
        m_error_cat = original.m_error_cat;
        m_bool = original.m_bool;
        m_addrlist = original.m_addrlist;
        m_addrs = m_addrlist.begin();
}

bool EndpointAddrlist::Create(std::string hostname, std::string service,
                              int type, int family)
{
        struct addrinfo hints;//, *addr;
        int error, proto;
        memset(&hints, 0, sizeof(hints));
        proto = type & EP_SOCK_MASK;
        if (proto == TCP)
                hints.ai_socktype = SOCK_STREAM;
        else if (proto == UDP)
                hints.ai_socktype = SOCK_DGRAM;
        else if (proto >= RAW_BASE)
                hints.ai_socktype = SOCK_RAW;

        m_bool = false;
        m_error_cat = EP_ERROR_NONE;
        m_error_code = 0;
        m_error_str = "";

        //hints.ai_socktype = ((type & EP_SOCK_MASK) == UDP) ? SOCK_DGRAM : SOCK_STREAM;
        hints.ai_family = family;

        // Should we fetch the canonical name (DNS FQDN)? Probably not.
        //hints.ai_flags = //AI_CANONNAME |

        hints.ai_flags = (type & SERVER) ? AI_PASSIVE : 0;

        // hostname can be one of:
        // - IPv4 address
        // - IPv6 address - has >1 colon
        // - DNS name
        // - One of the above, followed by ":service_name"
        // IPv4 or DNS + port = 1 colon
        // IPv6 = >1 colon
        // IPv6 + port = [address]:port, requires brackets
        // [address] can be any literal, including IPv4

        // One colon, no [] - something besides an IPv6 literal, easy
        if (hostname.find(":") != std::string::npos &&        // has colon
            hostname.find_first_of(":") == hostname.find_last_of(":") &&  // 1 ":"
            hostname.find("]") == std::string::npos) {        // and no ]
                // One colon. Before the colon is host, after is service
                service = hostname.substr(hostname.find(":") + 1);
                hostname = hostname.substr(0, hostname.find(":"));

        }
        // Let hostnames with multiple colons be parsed by getaddrinfo(), because
        // they are IPv6 addresses without ports. Unless they contain [...].

        // A service (port) name can be specified with IPv6 addresses also, but
        // they have to be enclosed in brackets to disambiguate the colons.
        // This seems to be the standard in URLs, for example- http://[::1]:80/
        // IPv4 addresses are also allowed, as in [127.0.0.1]:80, but not DNS names
        // Technical Note: Only the last bracket is actually required
        if (hostname.find("]") != std::string::npos) {
                service = hostname.substr(hostname.find("]") + 1);
                if (service[0] == ':')       // [foo]:bar, extract just bar
                        service = service.substr(1); // but also allow [foo]bar
                hostname = hostname.substr(0, hostname.find("]"));
                if (hostname[0] == '[')      // [foo]:bar, just extract foo
                        hostname = hostname.substr(1); // but also allow foo]:bar

#ifdef _EP_DEBUG
                std::cout<<"service:"<<service<<","<<hostname<<std::endl;
#endif

                hints.ai_flags |= AI_NUMERICHOST;  // force literal, disable DNS
        }

        // With raw services, this may not be an error. Assume 0?
        // For now, don't complain about this error. If it becomes a problem,
        // it will cause errors further down the road.
        //if (service == "")
        //    std::cout << "No service port given!" << std::endl;

        // Allow the two octets of the port number to be specified separately,
        // separated by commas. For example, port "1,2" would be 258. This is
        // mainly used when writing to ICMP raw sockets: type,code is port number
        if (service.find(",") != std::string::npos) {
                int nport;
                std::ostringstream o;

#ifdef _EP_DEBUG
                std::cout << "service has comma!" << std::endl;
#endif

                nport = (atoi(service.substr(0, service.find(",")).c_str()) << 8)
                        + atoi(service.substr(service.find(",") + 1).c_str());

                service = (o << nport) ? o.str() : service;
        }

        // XXX: inet_addr could be used here if hostname doesn't contain a colon
        // (non-IPv6), as it could be either an IPv4 literal or DNS hostname.
        //     if ((remote_ip = inet_addr(remote_addr.c_str()) >= 0xffffffff) {
        // That would allow for all kinds of neat IPv4 literals, like hex,
        // octal, dotless, blah blah blah.
        // XXX: But this is not done. Maybe thats a good thing.

        // Raw sockets don't have service/port numbers
        const char* str_service = service.c_str();
        if (type >= RAW_BASE)
                str_service = NULL;

        // Leaving the hostname blank means NULL (getaddrinfo won't accept blank)
        // NULL means INADDR_ANY if server, INADDR_LOOPBACK if client
        const char* str_hostname = hostname.c_str();
        if (!hostname.length())
                str_hostname = NULL;

        if (type < RAW_BASE) {
                addrinfo* res;
                // Call the standard getaddrinfo(), it does all our work!
                error = getaddrinfo(str_hostname, str_service, &hints, &res);
                if (error) {
                        std::stringstream ss;
                        ss << "Used hostname: "
                           << (str_hostname ? str_hostname : "(null)")
                           << ", service: "
                           << (str_service ? str_service : "(null)") << std::endl;
                        ss << "getaddrinfo: " << strerror(errno) << std::endl;

                        if (hints.ai_flags & AI_NUMERICHOST)
                                ss << "(A literal address, i.e. not a DNS name, is required)" << std::endl;
                        m_error_str = ss.str();

                        m_error_cat = EP_ERROR_GETADDRINFO;
                        m_error_code = error;

                        DeleteList();
                        m_bool = false;
                } else {
                        m_bool = true;
                        CreateList(res);
                        freeaddrinfo(res);
                }
        } else {
                // Raw sockets not supported with Win32 getaddrinfo?
                // Build it ourself, IPv4 only
                EndpointAddress tempEA;
                tempEA.ai_flags = 0;
                tempEA.ai_family = AF_INET;
                tempEA.ai_socktype = SOCK_RAW;
                tempEA.ai_protocol = type;
                tempEA.ai_addrlen = sizeof(sockaddr_in);
                tempEA.ai_next = 0;
#if defined(_WINDOWS)
                ( (sockaddr_in*) tempEA.ai_addr )->sin_family = (ADDRESS_FAMILY)tempEA.ai_family;
#else
                ( (sockaddr_in*) tempEA.ai_addr )->sin_family = tempEA.ai_family;
#endif
                ( (sockaddr_in*) tempEA.ai_addr )->sin_port = 0;
                ( (sockaddr_in*) tempEA.ai_addr )->sin_addr.s_addr = inet_addr(hostname.c_str());
                m_addrlist.push_back(tempEA);
        }

        // User specified a service (port) for a raw socket, let's use it :)
        if (type >= RAW_BASE && service.length() != 0) {
                // TODO: let them specify service *names* as well
                int port = atoi(service.c_str());

                // Note: port of 0 is okay here

                std::list< EndpointAddress >::iterator it;
                for (it = m_addrlist.begin(); it != m_addrlist.end(); ++it) {
                        addrinfo* ai = &(*it);

                        // Warning: this is ugly, and duplicated in EndpointAddress
                        if (ai->ai_family == AF_INET)
                                ((sockaddr_in*)ai->ai_addr)->sin_port = (u_short) htons((u_short)port);
                        else if (ai->ai_family == AF_INET6)
                                ((sockaddr_in6*)ai->ai_addr)->sin6_port = (u_short) htons((u_short)port);
                        else {
#ifdef _EP_DEBUG
                                std::cout << "EndpointAddress::SetPort()-addr family?"
                                          << std::endl;
#endif
                        }
                }
        }
        return true;
}

// Get current address
EndpointAddress EndpointAddrlist::GetAddress() const
{
        if(m_addrs != m_addrlist.end()) {
                return *m_addrs;
        } else {
                EndpointAddress ret;
                return ret;
        }
}

// Get current address and go to next, or m_addrlist.end() if at end
EndpointAddress EndpointAddrlist::GetAddressNext()
{
        if( m_addrlist.empty() == true || m_addrs == m_addrlist.end() ) {
                EndpointAddress ret;
                return ret;
        }

        EndpointAddress ret = *m_addrs;
        if( m_addrs != m_addrlist.end() ) {
                m_addrs++;
        }

        return ret;
}

// CreateList assumes the pointer is at the head of the list.
// Only pass m_head through here!!!
void EndpointAddrlist::CreateList( struct addrinfo* list )
{
        int count = 1;
        DeleteList();

        // curList_addr = current node in 'list'
//  EndpointAddress *curList_addr = (EndpointAddress*) list;
        addrinfo *curList_addr = list;
        // curM_addr = current node in our class' vector
        EndpointAddress curM_addr;

        if( !list ) {
                return;
        }

        while( curList_addr != NULL ) {
                count++;
                curM_addr = *curList_addr;

                // explicitly set ai_next = 0 since the vector iterators are used instead.
                curM_addr.ai_next = 0;

                //finalize
                m_addrlist.push_back(curM_addr);
                curList_addr = (EndpointAddress*) curList_addr->ai_next;
        }
        m_addrs = m_addrlist.begin();

        return;
}

// Deletes out the local addrinfo linked list.
void EndpointAddrlist::DeleteList()
{
        m_addrlist.clear();
        m_addrs = m_addrlist.begin();
        return;
}

int EndpointAddrlist::SetLastError()
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
        m_error_str = (const char*)lpMsgBuf;
        LocalFree(lpMsgBuf);
#else
        m_error_code = errno;
        m_error_str =  strerror(errno);
#endif
        return m_error_code;
}

EndpointAddrlist::~EndpointAddrlist()
{
        DeleteList();
}


