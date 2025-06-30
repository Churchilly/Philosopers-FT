/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:45:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/30 19:14:14 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void	*death_monitor(void	*arg)
{
	t_philosopher	*philo;
	suseconds_t		current_time;
	suseconds_t		last_meal_time;

	philo = (t_philosopher *)arg;
	while (1)
	{
		last_meal_time = get_last_meal(philo);
		current_time = get_current_time();
		
		if (current_time - last_meal_time >= philo->data->time_to_die)
		{
			sem_wait(philo->semaphores->write_lock);
			printf("%ld %d died\n", current_time - philo->data->start_time, philo->id);
			sem_post(philo->semaphores->die_lock);
			exit(1); // Exit with error code to signal death
		}
		usleep(1000); // Check every 1ms
	}
	return (NULL);
}

void	routine(t_philosopher *philo)
{
	pthread_t	death_checker;

	// Initialize philosopher's last meal time
	set_last_meal(philo);
	
	// Create death monitor thread
	pthread_create(&death_checker, NULL, death_monitor, philo);
	pthread_detach(death_checker);
	
	// Offset odd philosophers to prevent deadlock
	if (philo->id % 2 == 0)
		usleep(1000);
	
	while (1)
	{
		// Check if philosopher has eaten enough
		if (philo->data->must_eat > 0 && philo->eaten_meal >= philo->data->must_eat)
		{
			sem_post(philo->semaphores->meal_complete);
			exit(0); // Exit successfully when done eating
		}
		
		eat(philo);
		philo_sleep(philo);
		think(philo);
	}
}
