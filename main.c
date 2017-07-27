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

void snprintdate(char * buf, int size)
{
  time_t timer;
  time(&timer);
  struct tm *t = localtime(&timer);
  // printf("%s %d %s %d:%d:%d %s %d\n",
  snprintf(buf, size,
    "%s %d %s %02d:%02d:%02d %s %d",
    ab_weekday_name[t->tm_wday],	/* Day of week.  [0-6] */
    t->tm_mday,				/* Day.    [1-31] */
    ab_month_name[t->tm_mon],		/* Month.  [0-11] */
    t->tm_hour,				/* Hours.  [0-23] */
    t->tm_min,				/* Minutes.  [0-59] */
    t->tm_sec,				/* Seconds.  [0-60] (1 leap second) */
    t->tm_zone,				/* Timezone abbreviation.  */
    1900+t->tm_year			/* Year  - 1900.  */
  );
}

#define DATE_BUF_SIZE 100

int main()
{
  int errno;
  struct timespec tp, rqtp = {0,0}, rmtp = {0,0};
  char buf[DATE_BUF_SIZE] = { '\0' };

  snprintdate(buf, DATE_BUF_SIZE);
  fputs((const char *) buf, stdout);
  fflush(stdout);

  for (;;)
  {
    clock_gettime(CLOCK_MONOTONIC, &tp);
    rqtp.tv_nsec = 1000000000l - tp.tv_nsec;
    errno = nanosleep(&rqtp, &rmtp);

    fputs((const char *) buf, stdout);
    fflush(stdout);

    snprintdate(buf, DATE_BUF_SIZE);
  }

  return 0;
}
