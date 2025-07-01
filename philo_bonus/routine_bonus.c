/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:45:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 05:00:49 by yusudemi         ###   ########.fr       */
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
			fflush(stdout);
			exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}

static int	check_if_finished(t_philosopher *philo)
{
	int	dinner_complete;

	dinner_complete = 0;
	if (philo->data->must_eat != -1)
	{
		dinner_complete = philo->eaten_meal == philo->data->must_eat;
	}
	return (dinner_complete);
}

void	prepare_for_routine(t_philosopher *philo)
{
	set_last_meal(philo);
	if (philo->id % 2 == 0)
	{
		log_status(philo, "is thinking");
		philo_perform(philo->data->time_to_eat * 0.5);
	}
	if (philo->id == philo->data->num_of_philos
		&& philo->data->num_of_philos % 2)
	{
		log_status(philo, "is thinking");
		philo_perform(philo->data->time_to_eat
			+ (philo->data->time_to_sleep * 0.5));
	}
}

void	routine(t_philosopher *philo)
{
	pthread_t	death_checker;

	log_status(philo, "starts routine");
	pthread_create(&death_checker, NULL, death_monitor, philo);
	pthread_detach(death_checker);
	prepare_for_routine(philo);
	while (!check_if_finished(philo))
	{
		if (eat(philo) || check_if_finished(philo) || philo_sleep(philo)
			|| check_if_finished(philo) || think(philo))
			break ;
	}
	exit(0);
}

void	one_fork_routine(t_philosopher *philo)
{
	pthread_t	death_checker;

	set_last_meal(philo);
	pthread_create(&death_checker, NULL, death_monitor, philo);
	pthread_detach(death_checker);
	sem_wait(philo->semaphores->forks);
	log_status(philo, "has taken a fork");
	while (1)
		usleep(10000);
}
