/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:14:34 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/02 22:37:19 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static void	set_last_meal(t_philosopher *philo)
{
	sem_wait(philo->semaphores->finish_lock);
	philo->last_meal = get_elapsed_time(philo->data) + philo->data->time_to_eat;
	sem_post(philo->semaphores->finish_lock);
}

long	get_last_meal(t_philosopher *philo)
{
	long	value;

	sem_wait(philo->semaphores->finish_lock);
	value = (long)philo->last_meal;
	sem_post(philo->semaphores->finish_lock);
	return (value);
}

int	eat(t_philosopher *philo)
{
	if (philo->data->must_eat != -1
		&& philo->eaten_meal == philo->data->must_eat)
		return (1);
	sem_wait(philo->semaphores->forks);
	log_status(philo, "has taken a fork");
	sem_wait(philo->semaphores->forks);
	log_status(philo, "has taken a fork");
	set_last_meal(philo);
	log_status(philo, "is eating");
	philo_perform(philo->data->time_to_eat);
	philo->eaten_meal++;
	sem_post(philo->semaphores->forks);
	sem_post(philo->semaphores->forks);
	return (0);
}

int	philo_sleep(t_philosopher *philo)
{
	log_status(philo, "is sleeping");
	return (philo_perform(philo->data->time_to_sleep));
}

int	think(t_philosopher *philo)
{
	int	time_to_think;

	log_status(philo, "is thinking");
	if (philo->data->num_of_philos % 2 == 0)
		return (0);
	time_to_think = (philo->data->time_to_eat * 2) - philo->data->time_to_sleep;
	if (time_to_think <= 0)
		return (0);
	return (philo_perform(time_to_think * 0.4));
}
