// Created:20030726
// By Jeff Connelly

// IRC

#include <iostream>

#include "endpoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int main()
{
    Endpoint ep(TCP | CLIENT, "[127.0.0.1]:6667");
    string str;

    ep.Write("NICK jeff2\015\012");
    ep.Write("USER blah blah blah :blahs\015\012");

    while(ep.Read(-200, str)) 
    {
        std::cout << str;
        if (str.find("376") != string::npos)    // End of /MOTD
        {
            ep.Write("MODE jeff2 +ix\015\012");
            ep.Write("PRIVMSG jeff :hello world\015\012");
        }
        str = "";
    }

    return 0;
}
