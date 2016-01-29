#include <string>
#include "doxmainpage.h"
#include <api/api.h>

#if defined(_WINDOWS)
#define _WINSOCKAPI_
#include <windows.h>
int EXTAPI Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#else
int EXTAPI Init(int argc, char** argv);
#endif

