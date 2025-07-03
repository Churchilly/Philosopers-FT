/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 05:40:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/03 04:28:44 by yusudemi         ###   ########.fr       */
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

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

suseconds_t	philo_perform(suseconds_t time)
{
	suseconds_t	start;
	suseconds_t	end_time;

	if (time <= 0)
		return (0);
	start = get_current_time();
	end_time = time + start;
	while (get_current_time() < end_time)
		usleep(100);
	return (0);
}

suseconds_t	get_elapsed_time(t_program *p)
{
	suseconds_t	current;

	current = get_current_time();
	return (current - p->data->start_time);
}

int	start_timer(t_program *p)
{
	p->data->start_time = get_current_time();
	return (0);
}
