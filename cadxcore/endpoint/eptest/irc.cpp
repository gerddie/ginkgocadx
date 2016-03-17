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

        while(ep.Read(-200, str)) {
                std::cout << str;
                if (str.find("376") != string::npos) {  // End of /MOTD
                        ep.Write("MODE jeff2 +ix\015\012");
                        ep.Write("PRIVMSG jeff :hello world\015\012");
                }
                str = "";
        }

        return 0;
}
