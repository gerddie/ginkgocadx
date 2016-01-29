// Created:20030511
// By Jeff Connelly

// Example program for Endpoint - chat program

#include <iostream>
#include "endpoint.h"


using namespace std;

Endpoint make_server();
Endpoint make_client(string);

int main(int argc, char** argv)
{
    Endpoint::Initialize();
    EndpointAddrlist::g_default_family = AF_INET;

    if (argc != 2) {
        cout << "usage: " << argv[0] << " next-host" << endl;
        return -1;
    }  

    string strPeer = argv[1];

    if (strPeer[0] == '-') {
        make_server();
        make_client(strPeer.substr(1));
    } else {
        make_client(strPeer);
        make_server();
    }

    return 0;
}

Endpoint make_server()
{
    Endpoint ep(TCP | SERVER, "localhost:8000");
    if (!ep) {
        cout << "problem starting server" << endl;
        exit(-2);
    }
    cout << "server started" << endl;

    return ep;
}

Endpoint make_client(string strPeer)
{ 
    Endpoint ep(TCP | CLIENT, strPeer, "8000");
    if (!ep)
    {
         cout << "problem connecting to peer" << endl;
         exit(-2);
    }
    cout << "connected to peer" << endl;

    return ep;
}
