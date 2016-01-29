// From _Unix Network Programming_
//#include	"unp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sockio.h>

int
sockatmark(int fd)
{
	int		flag;

	if (ioctl(fd, SIOCATMARK, &flag) < 0)
		return(-1);
	return(flag != 0 ? 1 : 0);
}
