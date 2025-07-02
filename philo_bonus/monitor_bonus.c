/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:19:43 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/02 22:35:13 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

void	*kill_monitor(void	*arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	sem_wait(philo->semaphores->term_lock);
	sem_post(philo->semaphores->term_lock);
	clear_fork(philo->program, 0);
	return (NULL);
}

void	*death_monitor(void	*arg)
{
	t_philosopher	*philo;
	suseconds_t		current_elapsed;
	suseconds_t		last_meal_time;

	philo = (t_philosopher *)arg;
	while (1)
	{
		last_meal_time = get_last_meal(philo);
		current_elapsed = get_elapsed_time(philo->data);
		if (current_elapsed - last_meal_time >= philo->data->time_to_die)
		{
			sem_wait(philo->semaphores->write_lock);
			printf("%ld %d died\n", current_elapsed, philo->id);
			sem_post(philo->semaphores->term_lock);
			break ;
		}
		usleep(100);
	}
	return (NULL);
}
