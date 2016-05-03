#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "clock_elapsed.h"

int
main(int argc, char **argv)
{
    void * start;
    double elapsed;
    int no_pass = 0;
    int nap_time = 2;
    double lower_limit = 1.9;
    double upper_limit = 2.1;

    if (argc >= 2) {
        nap_time = atoi(argv[1]);
        lower_limit = ((double)nap_time) - .1;
        upper_limit = ((double)nap_time) + .1;
    }
    if (argc >= 3) {
        lower_limit = strtod(argv[2], NULL);
        upper_limit = nap_time + .1;
    }
    if (argc >= 4) {
        upper_limit = strtod(argv[3], NULL);
    }

    if (( lower_limit >= nap_time
        || nap_time >= upper_limit)) {
        printf("FAIL: inconsistent testparamters\n"
               "    lower limit:  %5.3f\n"
               "    nap_time:     %d\n"
               "    upper limit:  %5.3f\n",
               lower_limit, nap_time, upper_limit);
        return 1;
    }
    
    start = start_timer();
    /* really bad test, some sort of poll would be better so teset
     * does not fail in the case interrupt or signal. */
    sleep(nap_time);
    elapsed = elapsed_timer_free(&start);
    if (elapsed < lower_limit || elapsed > upper_limit) {
        printf("FAIL: elapsed outside of expected range [%5.3f, %5.3f], was %5.3f\n",
               lower_limit, upper_limit, elapsed);
        no_pass = 1;
    } else {
        printf("PASS: [%5.3f, %5.3f, %5.3f]\n", lower_limit,elapsed, upper_limit);
    }
    return no_pass;
}
