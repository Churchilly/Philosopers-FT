/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:34:19 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/23 20:02:08 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

suseconds_t	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (printf("gettimeofday() failed: %d.\n", errno), -1);
	return (tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
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
	suseconds_t	ret;
	suseconds_t	time;

	time = get_current_time();
	if (time < 0)
		return (-1);
	if (thread_lock(&p->lock))
		return (-1);
	ret = time - p->start_time;
	if (thread_unlock(&p->lock))
		return (-1);
	//ret = ret - (ret % p->data->hcf);
	return (ret);
}

int	start_timer(t_program *p)
{
	suseconds_t	time;

	time = get_current_time();
	if (time < 0)
		return (-1);
	if (thread_lock(&p->lock))
		return (-1);
	p->start_time = time;
	if (thread_unlock(&p->lock))
		return (-1);
	return (0);
}
