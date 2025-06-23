/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:24:14 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/23 20:10:17 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <errno.h>
#include <stdio.h>

static int	kill_everyone(t_program *p)
{
	int	i;

	i = -1;
	while (++i < p->data->num_of_philos)
	{
		if (thread_lock(&p->philosophers[i].lock))
			return (1);
		p->philosophers[i].dead = 1;
		if (thread_unlock(&p->philosophers[i].lock))
			return (1);
	}
	return (0);
}

static int	is_scene_finished(t_program *p)
{
	int	ret;

	ret = 0;
	if (thread_lock(&p->lock))
		return (-1);
	if (!p->everyone_ok)
		ret = 1;
	if (p->philos_done_eating == p->data->num_of_philos)
		ret = 1;
	if (thread_unlock(&p->lock))
		return (-1);
	return (ret);
}

static int	check_philo_status(t_philosopher *philo)
{
	suseconds_t	time;
	suseconds_t	since_last_meal;
	int			philo_eaten_meal;
	
	time = get_elapsed_time(philo->program);
	if (time < 0)
		return (-1);
	
	if (thread_lock(&philo->lock))
		return (-1);
	since_last_meal = time - philo->last_meal;
	philo_eaten_meal = philo->eaten_meal;
	if (thread_unlock(&philo->lock))
		return (-1);
	if (since_last_meal >= philo->data->time_to_die
		&& (philo->data->must_eat == -1
			|| philo_eaten_meal < philo->data->must_eat))
	{
		if (thread_lock(&philo->program->lock))
			return (-1);
		if (philo->program->everyone_ok)
			printf("%ld %d died.\n", time, philo->id);
		if (kill_everyone(philo->program))
			return (-1);
		philo->program->everyone_ok = 0;
		if (thread_unlock(&philo->program->lock))
			return (-1);
	}
	return (0);
}
void	*monitoring(void *arg)
{
	t_program	*p;
	int			i;
	int			retval;

	p = (t_program *)arg;
	if (start_timer(p) || say_ready(p) || wait_other_actors(p))
		return ((void *)1);
	retval = is_scene_finished(p);
	if (retval == -1)
		return ((void *)1);
	while (!retval)
	{
		i = -1;
		while (++i < p->data->num_of_philos)
		{
			retval = check_philo_status(&(p->philosophers[i]));
			if (retval == -1)
				return ((void *)1);
		}
		retval = is_scene_finished(p);
		if (retval == -1)
			return ((void *)1);
	}
	return ((void *)0);
}