/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:36:23 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/23 20:10:54 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>

int	log_status(t_philosopher *philo, char *msg)
{
	suseconds_t	time;
	int			id;
	int			write_perm;

	if (thread_lock(&philo->program->lock))
		return (1);
	id = philo->id;
	write_perm = philo->program->everyone_ok;
	if (thread_unlock(&philo->program->lock))
		return (1);
	time = get_elapsed_time(philo->program);
	if (time < 0)
		return (1);
	if (write_perm)
		printf("%ld %d %s.\n", time, id, msg);
	return (0);
}

static int	set_last_meal(t_philosopher *philo)
{
	suseconds_t	time;

	time = get_elapsed_time(philo->program);
	if (time < 0)
		return (1);
	if (thread_lock(&philo->lock))
		return (1);
	philo->last_meal = time;
	philo->eaten_meal += 1;
	if (thread_unlock(&philo->lock))
		return (1);
	log_status(philo, "is eating");
	return (0);
}

int	philo_eat(t_philosopher *philo)
{
	if (!am_i_alive(philo))
		return (0);
	if (thread_lock(philo->fork_one))
		return (1);
	log_status(philo, "has taken a fork");
	if (thread_lock(philo->fork_two))
		return (1);
	log_status(philo, "has taken a fork");
	if (set_last_meal(philo))
		return (1);
	if (philo_perform(philo->data->time_to_eat))
		return (1);
	if (thread_unlock(philo->fork_one))
		return (1);
	if (thread_unlock(philo->fork_two))
		return (1);
	return (0);
}

int	philo_think(t_philosopher *philo)
{
	int	time_to_think;

	if (!am_i_alive(philo))
		return (0);
	log_status(philo, "is thinking");
	if (philo->data->num_of_philos % 2 == 0)
		return (0);
	time_to_think = (philo->data->time_to_eat * 2) - philo->data->time_to_sleep;
	if (time_to_think < 0)
		return (0);
	if (philo_perform(time_to_think * 0.5))
		return (1);
	return (0);
}

int	philo_sleep(t_philosopher *philo)
{
	if (!am_i_alive(philo))
		return (0);
	log_status(philo, "is sleeping");
	if (philo_perform(philo->data->time_to_sleep))
		return (1);
	return (0);
}