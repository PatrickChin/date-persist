// vim: ts=8

#include <stdio.h>
#include <time.h>
#include <unistd.h>

static char const ab_weekday_name[][4] =
{
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static char const ab_month_name[][4] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/**
 * Print the current time in the default `date' format to the buffer buf of a
 * given size.
 */
void snprinttimer(char * buf, int size, time_t * timer)
{
    struct tm *t = localtime(timer);

    int am = t->tm_hour < 12;
    t->tm_hour = t->tm_hour == 12 ? 12 : t->tm_hour % 12;

    snprintf(buf, size,
	    "%s %d %s %02d:%02d:%02d %s %s %d\n",
	    ab_weekday_name[t->tm_wday],	/* Day of week.  [0-6] */
	    t->tm_mday,				/* Day.    [1-31] */
	    ab_month_name[t->tm_mon],		/* Month.  [0-11] */
	    t->tm_hour,				/* Hours.  [0-23] */
	    t->tm_min,				/* Minutes.  [0-59] */
	    t->tm_sec,				/* Seconds.  [0-60] (1 leap second) */
	    am ? "a.m." : "p.m.",
	    t->tm_zone,				/* Timezone abbreviation.  */
	    1900+t->tm_year			/* Year  - 1900.  */
	    );
}

#define DATE_BUF_SIZE 100

int main()
{
    struct timespec tp, rqtp = {0, 0};
    char buf[DATE_BUF_SIZE] = {'\0'};
    int errno = 0;

    time_t timer;
    time(&timer);

    clock_gettime(CLOCK_MONOTONIC, &tp);
    // if over a millisecond left before the next second
    if (((long) 1e9) - tp.tv_nsec < ((long) 1e6))
    {
	snprinttimer(buf, DATE_BUF_SIZE, &timer);
	fputs(buf, stdout);
	fflush(stdout);
    }

    for (;;)
    {
	// should the timer be updated every minute or every loop?
	if (errno || (long) timer % 60 == 0)
	    time(&timer);
	timer++; // get the date string for the next second
	snprinttimer(buf, DATE_BUF_SIZE, &timer);

	clock_gettime(CLOCK_MONOTONIC, &tp);
	rqtp.tv_nsec = (1000000000-1) - tp.tv_nsec;
	errno = nanosleep(&rqtp, NULL);

	// print the date as soon as sleep finished
	fputs(buf, stdout);
	fflush(stdout);
    }

    return 0;
}

