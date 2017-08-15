#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{
    time_t timer;
    struct timespec tp;
    char buf[100];
    char * format = argc == 2 ? argv[1] : "%c\n";

    // TODO `format` needs to have enough space to store the extra character
    size_t flen = strlen(format);
    if (format[flen-1] != '\n' &&
	    (format[flen-2] != '%' || format[flen-1] != 'n'))
	strncat(format, "\n", 1);

    for (;;)
    {
	time(&timer);
	timer++; // get the date string for the next second
	size_t len = strftime(buf, sizeof buf, format, localtime(&timer));

	clock_gettime(CLOCK_MONOTONIC, &tp);
	tp.tv_sec = 0;
	tp.tv_nsec = (1000000000-1) - tp.tv_nsec;
	nanosleep(&tp, NULL);

	fwrite(buf, 1, len, stdout);
	fflush(stdout);
    }

    return 0;
}
