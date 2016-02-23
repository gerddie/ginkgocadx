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

#ifndef __EP_ENDPOINT_H
#define __EP_ENDPOINT_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#include "protocols.h"
#include "address.h"

#ifdef _WIN32
typedef unsigned char  u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned int   u_int32_t;
#endif

// Used for Endpoint connection options, mainly Accept().
#define NOACCEPT 	4
#define DOACCEPT 	6


#define EP_ERROR_NONE        0
#define EP_ERROR_SOCKET      1
#define EP_ERROR_BIND        2
#define EP_ERROR_ACCEPT      3
#define EP_ERROR_SETSOCKOPT  4
#define EP_ERROR_CONNECT     5
#define EP_ERROR_ADDRESS     6
#define EP_ERROR_GETPEERNAME 7
#define EP_ERROR_SEND        8
#define EP_ERROR_RECV        9
// 100+ defined in address.h


class Endpoint
{
public:

	typedef enum EP_Mode {
		INPUT,
		OUTPUT,
		BOTH
	} EP_Mode;

    Endpoint();

    // Each of these functions have an int appended to the end of
    // their constuctors, for determining if we should automatically
    // connect() or accept().  
    Endpoint( int type, std::string hostname, int service, int options = DOACCEPT );

    // type-hostname-service deprecated in favor of type-remote-local
    //Endpoint(int type, std::string hostname, std::string service = "")
    //    { Create(type, hostname, service); }
    Endpoint( int type, std::string remote, std::string local, int options = DOACCEPT );

    // This constructor does not need the options parameter because it would
    // conflict with the above Endpoint( int, std::string, int, int=DOACCEPT )
    Endpoint( int type, std::string address );
    
    Endpoint( int type, std::string remotehost, std::string remoteservice,
             std::string localhost, std::string localservice, int options = DOACCEPT );
    Endpoint( int type, EndpointAddrlist address, int options = DOACCEPT );
    Endpoint( int type, EndpointAddrlist local, EndpointAddrlist remote, int options = DOACCEPT );
    

    bool Create( int type, EndpointAddrlist address, int options = DOACCEPT );
    bool Create( int type, EndpointAddrlist remote, EndpointAddrlist local, int options = DOACCEPT );
    bool Create( int type, std::string host, std::string service, int options = DOACCEPT );
    bool Create( int type, std::string remotehost, std::string remoteservice,
                 std::string localhost, std::string localservice, int options = DOACCEPT );

    virtual ~Endpoint();
    bool Close();
	void Shutdown(EP_Mode mode);
	static void ShutdownSocket(int socketID, EP_Mode mode);
    bool CloseServer();
    bool Accept();

    operator bool() { return m_bool; }
    operator std::string() { return Connection();}
    std::string Connection() { return m_local.IPPort() + " <--> " + m_remote.IPPort(); }
    std::string Server() { return m_local_server.IPPort(); }

    int Send(const void *msg, int len, int flags);
    int Write(std::string);      // Returns number of bytes written
    int Recv(void *buf, int len, unsigned int flags);
    int Read(int, std::string&);  // Returns number of bytes read
	 int Read(int, char* buffer);  // Returns number of bytes read


    void SetOptOn(int level, int opt)
        { char on = 1; setsockopt(m_sockfd, level, opt, &on, sizeof(on)); }

    void AddBytesRecv(int n) { m_bytes_recv += n; g_bytes_recv += n; }
    void AddBytesSent(int n) { m_bytes_sent += n; g_bytes_sent += n; }
    int GetBytesRecv() { return m_bytes_recv; }
    int GetBytesSent() { return m_bytes_sent; }
    int GetAllBytesRecv() { return g_bytes_recv; }
    int GetAllBytesSent() { return g_bytes_sent; }

    std::string Stats() { std::stringstream ss;
                          ss << GetBytesRecv() << " / " << GetBytesSent()
                             << std::endl 
                             << GetAllBytesRecv() << " / "
                             << GetAllBytesSent() << std::endl;
						  return ss.str();
                 }

    int m_sockfd;
    int m_servfd;

    bool m_bool;
    int m_error_cat;
    int m_error_code;
    std::string m_error_str;

    int m_type;
    int m_server;
    int m_family;

    EndpointAddress m_remote;
    EndpointAddress m_local;

    // Only used with TCP | SERVER
    EndpointAddress m_local_server;

    // Multicast functions
    // No special function needed to send to an mcast addr, ex. 226.1.1.2:2000
    //int join(std::string mcastaddr, std::string interface = "0");
    //int leave(std::string mcastaddr, std::string interface = "0");
    int mcast_options(std::string outgoing_if, int ttl = 1, int loop = 1);

    static void Initialize();

    static unsigned short in_cksum(unsigned short *addr, int len);

    static int set_raw_sockfd(int n) { return g_raw_sockfd = n; }
    static int get_raw_sockfd() { return g_raw_sockfd; }

    // Handles creation of one raw socket & dropping of root
    static void setup_raw(std::string argv0);
private:

    int m_bytes_sent;
    int m_bytes_recv;

    // Static data that applies to all instances of this class
    static int g_bytes_sent;
    static int g_bytes_recv;

    // If this is non-zero, it will be used instead of a call to 
    // socket() for raw sockets. The idea is to re-use one raw
    // socket created when the program started (code to do this in SUMI,
    // TODO: maybe move it here) so root privs can be dropped.
    // XXX: Unfortunately, the pipe breaks after multiple sends. 
    // XXXX: Can't sendto() multiple times?
    static int g_raw_sockfd;          // Socket descriptor to use for raw sockets

    int SetLastError();
};

struct Interface
{
	bool up, bcast, loopback, p2p, mcast;
	std::string address;
	std::string broadcast;
	std::string netmask;
};
bool Network_Interfaces(std::vector<Interface>& is);

#endif

