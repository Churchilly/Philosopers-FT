/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 05:40:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/03 04:46:47 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	take_forks(t_philosopher *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->fork_one < philo->fork_two)
	{
		first_fork = philo->fork_one;
		second_fork = philo->fork_two;
	}
	else
	{
		first_fork = philo->fork_two;
		second_fork = philo->fork_one;
	}
	lock_mutex(first_fork);
	log_status(philo, "has taken a fork");
	lock_mutex(second_fork);
	log_status(philo, "has taken a fork");
	return (0);
}

int	philo_eat(t_philosopher *philo)
{
	if (philo->data->must_eat != -1
		&& philo->eaten_meal >= philo->data->must_eat)
		return (1);
	if (take_forks(philo))
		return (1);
	if (log_status(philo, "is eating"))
		return (unlock_mutex(philo->fork_one),
			unlock_mutex(philo->fork_two), 0);
	lock_mutex(&philo->lock);
	philo->last_meal = get_elapsed_time(philo->program);
	unlock_mutex(&philo->lock);
	philo_perform(philo->data->time_to_eat);
	lock_mutex(&philo->lock);
	philo->eaten_meal++;
	unlock_mutex(&philo->lock);
	return (unlock_mutex(philo->fork_one),
		unlock_mutex(philo->fork_two), 0);
}

int	philo_sleep(t_philosopher *philo)
{
	if (log_status(philo, "is sleeping"))
		return (1);
	return (philo_perform(philo->data->time_to_sleep));
}

int	philo_think(t_philosopher *philo)
{
	int	time_to_think;

	if (log_status(philo, "is thinking"))
		return (1);
	if (philo->data->num_of_philos % 2 == 0)
		return (0);
	time_to_think = (philo->data->time_to_eat * 2) - philo->data->time_to_sleep;
	if (time_to_think <= 0)
		return (0);
	return (philo_perform(time_to_think * 0.4));
}
