#include "apue.h"
#include <sys/stat.h>
#include <fcntl.h>
#define	BUFFSIZE 16384

int main(void)
{
		int		n;
		char	buf[BUFFSIZE];
		int fd=open("/dev/null",O_WRONLY);
		while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
				if (write(fd, buf, n) != n)
						err_sys("write error");
		
		close(fd);
		if (n < 0)
				err_sys("read error");

		exit(0);
}

