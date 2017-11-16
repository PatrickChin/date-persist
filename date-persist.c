// vim: ts=8:sw=4:noexpandtab

#include <stdio.h>
#include <time.h>

#define BUF_SIZE 100

int main(int argc, char *argv[])
{
    char * format = argc == 1 ? "%c\n" : argv[1];

    struct timespec curtp;
    clock_gettime(CLOCK_REALTIME, &curtp);

    char buf[BUF_SIZE] = { '\0' };
    struct timespec sleeptp = {0, 0};
    while (1)
    {
	long nextsec =  curtp.tv_sec + 1; // get the date string for the next second
	struct tm * localtm = localtime(&nextsec);
	size_t len = strftime(buf, sizeof buf, format, localtm);

	// append newline
	{
	    char * c = buf;
	    while ((c-buf) < BUF_SIZE && *c != '\0')
		c++;
	    if (*(c-1) != '\n')
	    {
		*c = '\n';
		*++c = '\0';
		len++;
	    }
	}

	clock_gettime(CLOCK_REALTIME, &curtp);
	sleeptp.tv_nsec = (1000000000-1) - curtp.tv_nsec;
	nanosleep(&sleeptp, NULL);
	curtp.tv_sec += 1;

	fwrite(buf, 1, len, stdout);
	fflush(stdout);
    }

    return 0;
}
