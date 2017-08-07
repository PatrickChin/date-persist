#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define DATE_BUF_SIZE 100

int main(int argc, char *argv[])
{
    time_t timer;
    struct timespec tp;
    char * buf = malloc(DATE_BUF_SIZE);
    const char * format = argc == 2 ? argv[1] : "%c\n";

    for (;;)
    {
	time(&timer);
	timer++; // get the date string for the next second
	size_t len = strftime(buf, DATE_BUF_SIZE, format, localtime(&timer));

	clock_gettime(CLOCK_MONOTONIC, &tp);
	tp.tv_sec = 0;
	tp.tv_nsec = (1000000000-1) - tp.tv_nsec;
	nanosleep(&tp, NULL);

	fwrite(buf, 1, len, stdout);
	fflush(stdout);
    }

    return 0;
}
