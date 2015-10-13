#include "../include/timer.h"

struct timer
{
	int start_time;
	int current_time;
	int reference_time;
	int is_init;
	int pause;
};

struct timer* timer_new()
{
	struct timer* timer = malloc(sizeof(*timer));
	assert(timer);


	timer->start_time=0;
	timer->current_time=0;
	timer->reference_time=0;
	timer->is_init=0;
	timer->pause = 0;

	return timer;
}

void timer_free (struct timer* timer)
{
	assert(timer);
	free(timer);
}

void timer_init(struct timer* timer, int current_time, int reference_time)
{
	assert(timer);
	timer->start_time = current_time;
	timer->current_time = 1;
	timer->reference_time = reference_time;
	timer->is_init = 1;
}

int timer_is_init(struct timer* timer)
{
	assert(timer);
	return timer->is_init;
}

int timer_get_tick(struct timer* timer, int time)
{
	assert(timer);

	if ( timer->current_time * timer->reference_time <= time )
	{
		timer->current_time ++ ;
		if(timer->pause == 1)
		{
			return 0;
			printf("pause\n");
		}
		return 1;
	}
	else
		return 0;
}

void timer_pause_set(struct timer* timer)
{
	assert(timer);
	timer->pause=1;
}

void timer_pause_unset(struct timer* timer)
{
	assert(timer);
	timer->pause=0;
}

int timer_pause_get(struct timer* timer)
{
	assert(timer);
	return timer->pause;
}
