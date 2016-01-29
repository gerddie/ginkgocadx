#include "init.h"
#include "initwx.h"

#if defined(GINKGO_PRO)
#include <lib_test.h>
#include <cstring>
#endif

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
	
	for( int i = 0; i < argc; i++)
	{
		GlobalFree(argv[i]);
	}	
	GlobalFree(argv);

	if (unitTest) {
		return InitTest();
	}
#endif
	return InitWX(argc, argv);
}
#endif
