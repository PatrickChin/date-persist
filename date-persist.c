#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define DATE_BUF_SIZE 100

int main()
{
    time_t timer;
    struct timespec tp, rqtp = {0, 0};
    char * buf = malloc(DATE_BUF_SIZE);

    for (;;)
    {
	time(&timer);
	timer++; // get the date string for the next second
	ctime_r(&timer, buf);

	clock_gettime(CLOCK_MONOTONIC, &tp);
	rqtp.tv_nsec = (1000000000-1) - tp.tv_nsec;
	nanosleep(&rqtp, &rqtp);

	fputs(buf, stdout);
	fflush(stdout);
    }

    return 0;
}
