/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:45:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/02 22:34:45 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

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
	if (sem_wait(philo->semaphores->finish_lock))
	{
		printf("sem_wait failed:%d\n", errno);
		clear_fork(philo->program, 1);
	}
	philo->last_meal = get_elapsed_time(philo->data);
	if (sem_post(philo->semaphores->finish_lock))
	{
		printf("sem_post failed:%d\n", errno);
		clear_fork(philo->program, 1);
	}
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
	pthread_t	kill_checker;

	pthread_create(&death_checker, NULL, death_monitor, philo);
	pthread_create(&kill_checker, NULL, kill_monitor, philo);
	pthread_detach(death_checker);
	pthread_detach(kill_checker);
	prepare_for_routine(philo);
	while (!check_if_finished(philo))
	{
		if (eat(philo) || check_if_finished(philo) || philo_sleep(philo)
			|| check_if_finished(philo) || think(philo))
			break ;
	}
}

void	one_fork_routine(t_philosopher *philo)
{
	pthread_t	death_checker;
	pthread_t	kill_checker;

	if (sem_wait(philo->semaphores->finish_lock))
	{
		printf("sem_wait failed:%d\n", errno);
		clear_fork(philo->program, 1);
	}
	philo->last_meal = get_elapsed_time(philo->data);
	if (sem_post(philo->semaphores->finish_lock))
	{
		printf("sem_post failed:%d\n", errno);
		clear_fork(philo->program, 1);
	}
	pthread_create(&death_checker, NULL, death_monitor, philo);
	pthread_create(&kill_checker, NULL, kill_monitor, philo);
	pthread_detach(death_checker);
	pthread_detach(kill_checker);
	sem_wait(philo->semaphores->forks);
	log_status(philo, "has taken a fork");
	philo_perform(philo->data->time_to_die);
	while (!check_if_finished(philo))
	{
		usleep(philo->data->time_to_die);
	}
}
