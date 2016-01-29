#include <string>
#include "protocols.h"

#include "address.h"
#include "endpoint.h"
#include <signal.h>
#include <iostream>

#define IPSZ       sizeof(struct ep_iphdr)
#define UDPSZ      sizeof(struct ep_udphdr)
#define DATASZ     sizeof(STRING) - 1
#define PSEUDOSZ   sizeof(struct pseudohdr)
#define BUFFSZ     100
#define SIZE       (IPSZ + UDPSZ + DATASZ)
#define STRING     "hello" 

#include<stdio.h>
int main(int argc, char**argv)
{
#ifndef _WIN32
signal(SIGPIPE, SIG_IGN);
#endif
/*if (argc != 3) {
   std::cout << "usage: %0 <dst> <src>\n" << std::endl;
   return 0;
}*/

EndpointAddrlist::g_default_family = AF_INET;
Endpoint::Initialize();
Endpoint::setup_raw(argv[0]);

Endpoint ep(RAW_UDP | CLIENT, argv[1], argv[2]);
ep.Write("hi");
// for multicasting, can't connect() or else won't receive from everywhere
//Endpoint ep(UDP | CLIENT, ":0", "226.1.1.2:2000");

//std::cout << "Join: " << ep.join("226.1.1.2") << std::endl;

if (!ep)
{
    std::cout << "Couldn't connect" << std::endl;
    return -2;
}

std::cout << std::string(ep) << std::endl;

std::string buf;
ep.Read(-500, buf);
std::cout << buf;

//ep.Write("hello\r\n");

return 0;

}

