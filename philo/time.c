/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 05:40:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 02:43:35 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

suseconds_t	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static int	pass_time(long time, long start, long *time_passed)
{
	suseconds_t	current_time;

	current_time = get_current_time();
	if (current_time < 0)
		return (1);
	(*time_passed) = current_time - start;
	if (time - (*time_passed) > 1e3)
	{
		if (usleep((*time_passed) / 2))
			return (printf("usleep() failed: %d.\n", errno), 1);
	}
	else
	{
		while ((*time_passed) < time)
		{
			current_time = get_current_time();
			if (current_time < 0)
				return (1);
			(*time_passed) = get_current_time() - start;
		}
	}
	return (0);
}

suseconds_t	philo_perform(suseconds_t time)
{
	suseconds_t	start;
	suseconds_t	current_time;
	suseconds_t	time_passed;

	start = get_current_time();
	if (start == -1)
		return (-1);
	time_passed = 0;
	while (time_passed < time)
	{
		if (pass_time(time, start, &time_passed))
			return (-1);
		current_time = get_current_time();
		if (current_time < 0)
			return (-1);
		time_passed = current_time - start;
	}
	return (0);
}

suseconds_t	get_elapsed_time(t_program *p)
{
	suseconds_t	current;

	current = get_current_time();
	if (current == -1)
		return (-1);
	return (current - p->data->start_time);
}

int	start_timer(t_program *p)
{
	p->data->start_time = get_current_time();
	if (p->data->start_time < 0)
		return (1);
	return (0);
}
