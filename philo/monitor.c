/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 07:55:15 by yusudemi          #+#    #+#             */
/*   Updated: 2025/05/23 19:33:01 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>

int	thread_lock(pthread_mutex_t *mutex);
int	thread_unlock(pthread_mutex_t *mutex);
suseconds_t	get_elapsed_time(t_program *p);
int	wait_other_actors(t_program *p);
int	is_everyone_alive(t_program *p);
int	start_timer(t_program *p);
int	say_ready(t_program *p);

int	check_philo_status(t_philosopher *philo)
{
	suseconds_t	time;
	
	if (thread_lock(&philo->lock))
		return (-1);
	time = get_elapsed_time(philo->program);
	if (time < 0)
		return (-1);
	if (time - philo->last_meal > philo->data->time_to_die)
	{
		if (thread_lock(&philo->program->lock))
			return (-1);
		if (philo->program->everyone_ok)
			printf("%ld %d died.\n", time, philo->id);
		philo->program->everyone_ok = 0;
		if (thread_unlock(&philo->program->lock))
			return (-1);
	}
	if (thread_unlock(&philo->lock))
		return (-1);
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
	retval = is_everyone_alive(p);
	if (retval == -1)
		return ((void *)1);
	while (retval)
	{
		i = -1;
		while (++i < p->data->num_of_philos)
		{
			retval = check_philo_status(&(p->philosophers[i]));
			if (retval == -1)
				return ((void *)1);
		}
		retval = is_everyone_alive(p);
		if (retval == -1)
			return ((void *)1);
	}
	return (NULL);
}
