/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 05:40:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/03 03:35:22 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

static int	check_if_finished(t_philosopher *philo)
{
	int	everyone_ok;
	int	dinner_complete;

	dinner_complete = 0;
	lock_mutex(&philo->program->program_lock);
	everyone_ok = philo->program->everyone_ok;
	unlock_mutex(&philo->program->program_lock);
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
	lock_mutex(&philo->lock);
	philo->last_meal = get_elapsed_time(philo->program);
	unlock_mutex(&philo->lock);
}

void	wait_actors(t_program *p)
{
	lock_mutex(&(p->program_lock));
	while (p->wait_actors)
	{
		unlock_mutex(&(p->program_lock));
		usleep(10);
		lock_mutex(&(p->program_lock));
	}
	unlock_mutex(&(p->program_lock));
}

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	wait_actors(philo->program);
	prepare_for_routine(philo);
	while (!check_if_finished(philo))
	{
		if (philo_eat(philo) || check_if_finished(philo) || philo_sleep(philo)
			|| check_if_finished(philo) || philo_think(philo))
			break ;
	}
	lock_mutex(&philo->program->program_lock);
	philo->program->philos_done_eating += 1;
	unlock_mutex(&philo->program->program_lock);
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
	while (!check_if_finished(philo))
		philo_perform(philo->data->time_to_die);
	return (NULL);
}
