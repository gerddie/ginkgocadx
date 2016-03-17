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


#include "init.h"
#include "initwx.h"

#if defined(GINKGO_PRO)
#include <lib_test.h>
#include <cstring>
#endif

#include <iostream>

#if defined(_WIN32)
int EXTAPI Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
        return InitWX(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
#else
int EXTAPI Init(int argc, char** argv)
{
#if defined(GINKGO_PRO)
        bool unitTest = false;

        for( int i = 1; i < argc; i++ ) {
                if (std::strcmp(argv[i], "-unittest") == 0) {
                        unitTest = true;
                }
        }

        for( int i = 0; i < argc; i++) {
                GlobalFree(argv[i]);
        }
        GlobalFree(argv);

        if (unitTest) {
                return InitTest();
        }
#endif
        std::cout << "calling InitWX(argc, argv);\n";
        return InitWX(argc, argv);
}
#endif
