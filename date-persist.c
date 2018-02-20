// vim: ts=8:sw=4:noexpandtab

#include <stdio.h>
#include <time.h>
#include <string.h>

#define BUF_SIZE 254

int main(int argc, char *argv[])
{
    const char * const format = argc == 1 ? "%c\n" : argv[1];
    const size_t flen = strlen(format);
    const int add_newline = format[flen-1] == '\n';

    struct timespec curtp;
    clock_gettime(CLOCK_REALTIME, &curtp);

    char buf[BUF_SIZE] = { '\0' };
    struct timespec sleeptp = {0, 0};
    while (1)
    {
	const long nextsec =  curtp.tv_sec + 1; // get the date string for the next second
	const struct tm * localtm = localtime(&nextsec);
	const size_t len = strftime(buf, sizeof buf, format, localtm);

	if (len == 0)
	    return 1;

	// append newline
	if (add_newline) {
	    const size_t end = len < BUF_SIZE-1 ? len : BUF_SIZE-1;
	    buf[end-1] = '\n';
	    buf[end] = '\0';
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
