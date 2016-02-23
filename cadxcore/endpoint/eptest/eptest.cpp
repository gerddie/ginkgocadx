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



using namespace std;



#include "../endpoint.h"



int main(int argc, char* argv[])

{

	Endpoint::Initialize();



	Endpoint ep(UDP | SERVER, "0.0.0.0:80");

	//Endpoint ep(TCP | CLIENT, EndpointAddrlist("10.0.0.2:8000"));

	if (!ep)

	{

		cout << "Couldn't connect" << endl;

		return -2;

	}



	cout << string(ep) << endl;



	string buf;

	ep.Read(-1024, buf);

	cout << buf << endl;

	//while(int n=ep.Write("hello world\n")){

	//}



	cout << "ok done" << endl;



	cout << ep.GetAllBytesSent() << " / " << ep.GetAllBytesRecv() << endl;



	return 0;





	return 0;

}



