// eptest.cpp : Defines the entry point for the console application.



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



