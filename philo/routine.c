/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 05:40:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 03:31:23 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

static int	check_if_finished(t_philosopher *philo)
{
	int	everyone_ok;
	int	dinner_complete;

	dinner_complete = 0;
	lock_mutex(&philo->program->finish_lock);
	everyone_ok = philo->program->everyone_ok;
	unlock_mutex(&philo->program->finish_lock);
	if (philo->data->must_eat != -1)
	{
		lock_mutex(&(philo->lock));
		dinner_complete = philo->eaten_meal == philo->data->must_eat;
		unlock_mutex(&(philo->lock));
	}
	return (!everyone_ok || dinner_complete);
}

void	prepare_for_routine(t_philosopher *philo)
{
	lock_mutex(&philo->lock);
	philo->last_meal = get_elapsed_time(philo->program);
	unlock_mutex(&philo->lock);
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

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	prepare_for_routine(philo);
	while (!check_if_finished(philo))
	{
		if (philo_eat(philo) || check_if_finished(philo) || philo_sleep(philo)
			|| check_if_finished(philo) || philo_think(philo))
			break ;
	}
	log_status(philo, "done");
	lock_mutex(&philo->program->finish_lock);
	philo->program->philos_done_eating += 1;
	unlock_mutex(&philo->program->finish_lock);
	return (NULL);
}

void	*one_fork_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	lock_mutex(&philo->lock);
	philo->last_meal = get_elapsed_time(philo->program);
	unlock_mutex(&philo->lock);
	log_status(philo, "has taken a fork");
	philo_perform(philo->data->time_to_die);
	return (NULL);
}
