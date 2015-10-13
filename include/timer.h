#ifndef TIMER_H_
#define TIMER_H_

// Library
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../include/constant.h"



// Function
extern struct timer* 	timer_new();
extern void 	timer_init(struct timer* timer, int time, int reference_time);
extern int		timer_is_init(struct timer* timer);
extern void 	timer_free(struct timer* timer);
extern int 		timer_get_tick(struct timer* timer, int time);

// Pause management
extern void		timer_pause_set(struct timer* timer);
extern void		timer_pause_unset(struct timer* timer);
extern int		timer_pause_get(struct timer* timer);

#endif /* TIMER_H_ */
