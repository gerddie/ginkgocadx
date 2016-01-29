/*
 *  
 *  $Id: main.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-12 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <api/init.h>

#if defined(_WIN32)
#include <windows.h>



struct Registry {
	Registry();
};


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
#else
int main(int argc, char** argv)
{
	
	return Init(argc, argv);
}
#endif