#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#ifdef __MACH__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

void *
start_timer()
{
#ifndef __MACH__
    struct timespec *tp = (struct timespec *)malloc(sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, tp);
    return (void *)tp;
#else /* def __MACH__ */
    uint64_t *start = (uint64_t *)malloc(sizeof(uint64_t));
    *start = mach_absolute_time();
    return (void *)start;
#endif /* def __MACH__ */
}

double
elapsed_timer(void *timer_data)
{
    if (timer_data == NULL) {
        return 0.0;
    }

    double elapsed;
#ifndef __MACH__
    struct timespec start = *(struct timespec *)timer_data;
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    if (now.tv_nsec < start.tv_nsec) {
        now.tv_nsec += 1 * 1000 * 1000 * 1000;
        now.tv_sec--;
    }

    elapsed = (double)(now.tv_nsec - start.tv_nsec);
    elapsed /= (double)(1 * 1000 * 1000 * 1000);
    elapsed += (double)(now.tv_sec - start.tv_sec);

#else /* def __MACH__ */
    static mach_timebase_info_data_t sTimebaseInfo;
    uint64_t start = *(uint64_t *)timer_data;
    uint64_t now = mach_absolute_time();

    elapsed = (double)(now - start);
    elapsed /= (double)(1 * 1000 * 1000 * 1000);

    if ( sTimebaseInfo.denom == 0 ) {
        (void) mach_timebase_info(&sTimebaseInfo);
    }

    elapsed *= ((double)sTimebaseInfo.numer) / ((double)sTimebaseInfo.denom);
#endif /* def __MACH__ */

    return elapsed;
}

double
elapsed_timer_free(void ** timer_data_p)
{
    double elapsed = 0;
    
    if (timer_data_p == NULL) {
        return elapsed;
    }

    elapsed = elapsed_timer(*timer_data_p);

    free(*timer_data_p);
    *timer_data_p = NULL;

    return elapsed;
}
