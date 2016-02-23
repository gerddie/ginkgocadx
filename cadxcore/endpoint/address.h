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

#ifndef __ENDPOINT_ADDRESS_H
#define __ENDPOINT_ADDRESS_H

#define EP_ERROR_NONE         0
#define EP_ERROR_GETADDRINFO  100


#ifdef _WIN32
#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else // !_WIN32
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif // !_WIN32

//******************* EndpointAddress ***********************
#include <iostream>
#include <string>
#include <cstring>
#include <list>

// A single address
class EndpointAddress : public addrinfo
{
	public:
	EndpointAddress()     {
		Reset();
	}

	EndpointAddress(const EndpointAddress &otra) {
		*this = otra;
	}

	EndpointAddress(sockaddr* sa, int type) {
		if (!sa)
		{
			m_Inicializada = false;
			return;
		}

		ai_flags = 0;
		ai_family = sa->sa_family;
		ai_socktype = type;
		ai_protocol = 0;

		ai_addr = &m_sockaddr;
		m_sockaddr = *(sa);
		ai_next = NULL;

		ai_addrlen = sizeof(sockaddr);

		ResetCanonName();
		ai_canonname = m_CanonName;

		m_sockaddr = *sa;

		ai_next = NULL;

		m_Inicializada = true;
	}

	~EndpointAddress()
	{
	}

	std::string IPPort();

	std::string IP();

	std::string DNS();

	std::string Name();

	std::string Port();

	unsigned short PortNumber();

	EndpointAddress & operator=(const EndpointAddress & otra) {

		if( !otra.m_Inicializada )
		{
			Reset();
			return *this;
		}

		ResetCanonName();

		ai_flags =otra.ai_flags;
		ai_family = otra.ai_family;
		ai_socktype = otra.ai_socktype;
		ai_protocol = 0;

		ai_addr = &m_sockaddr;
		m_sockaddr = *(otra.ai_addr);
		ai_next =otra.ai_next;

		ai_addrlen = sizeof(sockaddr);

		CopyCanonName(otra.m_CanonName);
		ai_canonname = m_CanonName;

		m_Inicializada = otra.m_Inicializada;

		return *this;
	}

	EndpointAddress & operator=(const addrinfo & otra) {
		ResetCanonName();

		ai_flags = otra.ai_flags;
		ai_family = otra.ai_family;
		ai_socktype = otra.ai_socktype;
		ai_protocol = otra.ai_protocol;
		ai_protocol = 0;

		ai_addr = &m_sockaddr;
		m_sockaddr = *(otra.ai_addr);
		ai_next = otra.ai_next;

		ai_addrlen = sizeof(sockaddr);
		if (otra.ai_canonname != NULL) {
			CopyCanonName(otra.ai_canonname);
		}
		ai_canonname = m_CanonName;

		m_Inicializada = true;

		return *this;
	}

	EndpointAddress & operator*()
	{
		return *this;
	}

	operator std::string();

	operator bool() {
		return m_Inicializada;
	}

	operator sockaddr()
	{
		if (!m_Inicializada) {
			sockaddr _nil;
			std::memset(&_nil, 0, sizeof(sockaddr));
			return _nil;
		}
		else {
			return m_sockaddr;
		}
	}

    bool m_Inicializada;

protected:

	void Reset()
	{
		ai_flags = 0;
		ai_family = 0;
		ai_socktype = 0;
		ai_protocol = 0;
		ai_addrlen = sizeof(sockaddr);
		ai_protocol = 0;

		ai_addr = &m_sockaddr;
		std::memset(&m_sockaddr, 0, sizeof(sockaddr));
		ai_next = NULL;

		ai_addrlen = sizeof(sockaddr);

		ResetCanonName();
		ai_canonname = m_CanonName;

		m_Inicializada = false;
	}

	void ResetCanonName()
	{
		for (int i = 0; i < 255; i++)
		{
			m_CanonName[i] = 0;
		}
	}

	void CopyCanonName(const char* src)
	{
		for (int i = 0; i < 255 && src[i] != 0; i++)
		{
			m_CanonName[i] = src[i];
		}
	}

    struct sockaddr m_sockaddr;
    char m_CanonName[256];

};


//******************* EndpointAddrlist ***********************
// Encapsulates (possibly multiple) addresses

#include <vector>

class EndpointAddrlist
{
public:
    EndpointAddrlist() : m_error_cat(0), m_error_code(0) { m_bool = false; m_addrs = m_addrlist.begin(); };
    EndpointAddrlist( const EndpointAddrlist & orig ) { Copy(orig); };
    EndpointAddrlist(std::string hostname,
                    std::string service = "",
                    int protocol = TCP | CLIENT,
                    int family = AF_INET)
                    { Create(hostname, service, protocol, family); }
    ~EndpointAddrlist();

    bool Create(std::string hostname, std::string service = "",
                int protocol = TCP | CLIENT, int family = AF_INET);

    EndpointAddrlist & operator=(const EndpointAddrlist & orig) {Copy(orig); return *this;}
    operator bool() { return m_bool; }
    void Copy(const EndpointAddrlist & );

    EndpointAddress GetAddress();
    EndpointAddress GetAddressNext();
    static std::string StrIP(struct addrinfo*);
    int SetLastError();
    std::list< EndpointAddress >  m_addrlist;
    std::list< EndpointAddress >::iterator m_addrs;

    int m_error_cat;
    int m_error_code;
    std::string m_error_str;
    bool m_bool;

private:
    void DeleteList();
    void CreateList( struct addrinfo* );
    char canonname[256];
};
#endif
