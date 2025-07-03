/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:03:21 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/03 04:32:11 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

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

suseconds_t	get_elapsed_time(t_data *d)
{
	suseconds_t	current;

	current = get_current_time();
	return (current - d->start_time);
}

int	start_timer(t_program *p)
{
	p->data->start_time = get_current_time();
	return (0);
}
