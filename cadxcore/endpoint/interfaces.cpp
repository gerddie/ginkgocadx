// Created:20030812
// By Jeff Connelly

// Retreive network interface information

//#include <afxwin.h>

#include <string>
#include <vector>

#include "protocols.h"
#include "address.h"
#include "endpoint.h"

#ifndef _WIN32
# error "UNIX network interface retrieval is currently unimplemented - please read UNP and implemenet it"
#else

// derived from code from tangentsoft.com WinSock example
bool Network_Interfaces(std::vector<Interface>& is)
{
    SOCKET sd;
	// Should use INTERFACE_INFO_EX to allow IPv6 support
	INTERFACE_INFO InterfaceList[20];
    unsigned long nBytesReturned;
	int nNumInterfaces;

	sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == SOCKET_ERROR) {
		//MessageBox(GetDesktopWindow(), "Failed to get a socket. Error ", "", 0);
			//+ WSAGetLastError() <<
            //endl;
		return false;
    }

    if (WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList,
			sizeof(InterfaceList), &nBytesReturned, 0, 0) == SOCKET_ERROR) {
    //    MessageBox(GetDesktopWindow(), "Failed calling WSAIoctl: error ", "", 0);
		//<< WSAGetLastError() <<
		//		endl;
		return false;
    }
	closesocket(sd);

    nNumInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);

	//str.Format("There are %d\n", nNumInterfaces);
	//MessageBox(GetDesktopWindow(), str, "", 0);

	for (int i = 0; i < nNumInterfaces; ++i)
	{
		Interface in;

		memset(&in, 0, sizeof(in));

		// NOTE: This should be moved into application-specific code
		if (InterfaceList[i].iiFlags & IFF_LOOPBACK)    // Skip loopback - uninteresting
			continue;
		if (!(InterfaceList[i].iiFlags & IFF_UP))       // Skip down interfaces
			continue;

        sockaddr_in *pAddress;

        pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
        //str += CString("IP=") + inet_ntoa(pAddress->sin_addr);
		in.address = inet_ntoa(pAddress->sin_addr);

        pAddress = (sockaddr_in *) & (InterfaceList[i].iiBroadcastAddress);
        //str += CString(" BCAST=") + inet_ntoa(pAddress->sin_addr);
		in.broadcast = inet_ntoa(pAddress->sin_addr);

        pAddress = (sockaddr_in *) & (InterfaceList[i].iiNetmask);
        //str += CString(" MASK=") + inet_ntoa(pAddress->sin_addr);
		in.netmask = inet_ntoa(pAddress->sin_addr);

        u_long nFlags = InterfaceList[i].iiFlags;
        if (nFlags & IFF_UP) { //str += " <UP";
			in.up = true; }
        else                 { //str += " <DOWN";
			in.up = false; }
        if (nFlags & IFF_POINTTOPOINT) { //str += " P2P";
			in.p2p = true; }
        if (nFlags & IFF_LOOPBACK)     { //str += " LOOPBACK";
			in.loopback = true; }
        if (nFlags & IFF_BROADCAST)    { //str += " BCAST";
			in.bcast = true; }
        if (nFlags & IFF_MULTICAST)    { //str += "MCAST";
			in.mcast = true; }
        //str += ">\n";
		is.push_back(in);
    }
	//MessageBox(GetDesktopWindow(), str, "", 0);

	return true;
}

#endif
