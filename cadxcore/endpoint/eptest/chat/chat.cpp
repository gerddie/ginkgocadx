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
