// vim: ts=8:sw=4:noexpandtab

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{
    struct timespec curtp, sleeptp = {0, 0};
    char buf[100];
    char * format;

    if (argc != 2) {
	format = "%c\n";
    } else {
	format = argv[1];
	size_t flen = strlen(format);
	if (format[flen-1] != '\n' &&
		(format[flen-2] != '%' || format[flen-1] != 'n')) {
	    format = malloc(flen+2);
	    strncpy(format, argv[1], flen);
	    format[flen] = '\n';
	    format[flen+1] = '\0';
	}
    }

    clock_gettime(CLOCK_REALTIME, &curtp);

    for (;;)
    {
	long nextsec =  curtp.tv_sec + 1; // get the date string for the next second
	struct tm * localtm = localtime(&nextsec);
	size_t len = strftime(buf, sizeof buf, format, localtm);

	// MONOTONIC_RAW and REALTIME seconds may not align
	// REALTIME is affected by NTP
	// TODO Find out what happens if realtime jumps backwards
	clock_gettime(CLOCK_REALTIME, &curtp);
	sleeptp.tv_nsec = (1000000000-1) - curtp.tv_nsec;
	nanosleep(&sleeptp, NULL);
	curtp.tv_sec += 1;

	fwrite(buf, 1, len, stdout);
	fflush(stdout);
    }

    return 0;
}
