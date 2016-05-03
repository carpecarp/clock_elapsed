#ifndef __CLOCK_ELAPSED_H__
#define __CLOCK_ELAPSED_H__
void * start_timer();
double elapsed_timer(void *timer_data);
double elapsed_timer_free(void **timer_data_p);
#endif /* __CLOCK_ELAPSED_H__ */
