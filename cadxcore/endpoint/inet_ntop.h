// An implementation of inet_ntop, if the system doesn't provide it (Win32)

#ifdef _WIN32
extern "C" const char * __cdecl myinet_ntop(int af, const void *src, char *dst, size_t size);
#endif
