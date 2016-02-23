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

#include "aetwildcards.h"
#include <string>
#include <algorithm>
#include <cstring>
#include <cctype>
#include <sstream>
#include <iostream>
#include <iomanip>
#if defined(_WINDOWS)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif


std::string GNC::GCS::AETWildcards::Parse(const std::string& AET)
{
	std::string realAET = AET;
	std::string ip4[4];

	if ( (AET.find("%IP") != std::string::npos) || (AET.find("%HOSTNAME") != std::string::npos) || (AET.find("%hostname") != std::string::npos))  {
		char bufHostName[256];
		char bufIP4[16];

		std::memset(bufHostName, 0, 256 * sizeof(char));

#if defined(_WINDOWS)
		if ( gethostname(bufHostName, 256) != 0)
		{
			unsigned long error = WSAGetLastError();
			std::cout << "gethostname() Error: " << error << std::endl;
		}
#else
		int ret = 0;
		if ( (ret = gethostname(bufHostName, 256)) != 0)
		{
			std::cout << "gethostname() Error: " << ret << std::endl;
		}
#endif
		else
		{
			struct addrinfo* l_addrInfo = NULL;
			struct addrinfo l_addrInfoHints;

			std::memset(&l_addrInfoHints, 0, sizeof(addrinfo));

			l_addrInfoHints.ai_socktype = SOCK_STREAM;
			l_addrInfoHints.ai_family = PF_INET;

#if defined(_WINDOWS)
			if ( getaddrinfo(bufHostName, NULL, &l_addrInfoHints, &l_addrInfo) != 0 )
			{
				unsigned long error = WSAGetLastError();
				std::cout << "getaddrinfo() Error: " << error << std::endl;
			}
#else
			if ( (ret = getaddrinfo(bufHostName, NULL, &l_addrInfoHints, &l_addrInfo)) != 0 )
			{
				std::cout << "getaddrinfo() Error: " << ret << std::endl;
			}
#endif
			else {
				struct sockaddr_in* sockaddr_ipv4 = (struct sockaddr_in *) l_addrInfo->ai_addr;
				const char* ip = inet_ntoa(sockaddr_ipv4->sin_addr);
#if defined(_WINDOWS)
				strcpy_s(bufIP4, ip);
#else
				strcpy(bufIP4, ip);
#endif
				unsigned long* sAddr = (unsigned long*) &sockaddr_ipv4->sin_addr.s_addr;
				{
					std::stringstream ss;
					ss << std::setw(3) << std::setfill('0');
					ss << (int) ( ( (*sAddr) & 0xFF000000) >> 24);
					ip4[3] = ss.str();
				}
				{
					std::stringstream ss;
					ss << std::setw(3) << std::setfill('0');
					ss << (int) ( ( (*sAddr) & 0x00FF0000) >> 16);
					ip4[2] = ss.str();
				}
				{
					std::stringstream ss;
					ss << std::setw(3) << std::setfill('0');
					ss << (int) ( ( (*sAddr) & 0x0000FF00) >> 8);
					ip4[1] = ss.str();
				}
				{
					std::stringstream ss;
					ss << std::setw(3) << std::setfill('0');
					ss << (int) ( ( (*sAddr) & 0x000000FF) );
					ip4[0] = ss.str();
				}
			}
		}

		bool changed = true;

		while(changed) {

			std::string::size_type it0;
			it0 = realAET.find("%IP1");
			if (it0 != std::string::npos) {
				realAET = realAET.replace(it0, 4, ip4[0] );
			}
			else {
				it0 = realAET.find("%IP2");
				if (it0 != std::string::npos) {
					realAET = realAET.replace(it0, 4, ip4[1] );
				}
				else {
					it0 = realAET.find("%IP3");
					if (it0 != std::string::npos) {
						realAET = realAET.replace(it0, 4, ip4[2] );
					}
					else {
						it0 = realAET.find("%IP4");
						if (it0 != std::string::npos) {
							realAET	= realAET.replace(it0, 4, ip4[3] );
						}
						else {
							it0 = realAET.find("%IP");
							if (it0 != std::string::npos) {
								realAET = realAET.replace(it0, 3, bufIP4 );
							}
							else {
								it0 = realAET.find("%HOSTNAME");
								if (it0 != std::string::npos) {
									std::string hostName = bufHostName;
									std::transform(hostName.begin(), hostName.end(), hostName.begin(), ::toupper);
									realAET = realAET.replace(it0, 9, hostName.c_str() );
								}
								else {
									it0 = realAET.find("%hostname");
									if (it0 != std::string::npos) {
										std::string hostName = bufHostName;
										std::transform(hostName.begin(), hostName.end(), hostName.begin(), ::tolower);
										realAET = realAET.replace(it0, 9, hostName.c_str() );
									}
									else {
										changed = false;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return realAET;
}
