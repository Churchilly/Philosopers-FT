/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:04:25 by yusudemi          #+#    #+#             */
/*   Updated: 2025/05/23 19:37:51 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

suseconds_t	get_elapsed_time(t_program *p);
suseconds_t	philo_perform(suseconds_t time);
int	is_everyone_alive(t_program *p);
int	wait_other_actors(t_program *p);
int	thread_lock(pthread_mutex_t *mutex);
int	thread_unlock(pthread_mutex_t *mutex);
int	say_ready(t_program *p);


int	log_status(t_philosopher *philo, char *msg)
{
	suseconds_t	time;

	time = get_elapsed_time(philo->program);
	if (time < 0)
		return (1);
	if (thread_lock(&philo->program->lock))
		return (1);
	if (philo->program->everyone_ok)
		printf("%ld %d %s.\n", time, philo->id, msg);
	if (thread_unlock(&philo->program->lock))
		return (1);
	return (0);
}

int	philo_eat(t_philosopher *philo)
{
	suseconds_t	time;

	if (thread_lock(philo->fork_one))
		return (1);
	log_status(philo, "has taken a fork");
	if (thread_lock(philo->fork_two))
		return (1);
	log_status(philo, "has taken a fork");
	if (thread_lock(&philo->lock))
		return (1);
	philo->eaten_meal += 1;
	time = get_elapsed_time(philo->program);
	if (time < 0)
		return (1);
	philo->last_meal = time;
	log_status(philo, "is eating");
	if (thread_unlock(&philo->lock))
		return (1);
	if (philo_perform(philo->data->time_to_eat))
		return (1);
	if (thread_unlock(philo->fork_one))
		return(1);
	if (thread_unlock(philo->fork_two))
		return (1);
	return (0);
}

int	philo_think(t_philosopher *philo)
{
	int	time_to_think;
	
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
	log_status(philo, "is sleeping");
	if (philo_perform(philo->data->time_to_sleep))
		return (1);
	return (0);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;
	int				retval;

	philo = (t_philosopher *)arg;
	if (say_ready(philo->program) || wait_other_actors(philo->program))
		return((void *)1);
	retval = is_everyone_alive(philo->program);
	if (retval == -1)
		return ((void *)1);
	while (retval)
	{
		if (philo->eaten_meal == philo->data->must_eat)// doesnt need to lock since monitor or other philos doesnt check current philos eaten meal and datas are const 
			break;
		if (philo_eat(philo) || philo_sleep(philo) || philo_think(philo))
			return ((void *)1);
		retval = is_everyone_alive(philo->program);
		if (retval == -1)
			return ((void *)1);
	}
	if (thread_lock(&philo->program->lock))
		return ((void *)1);
	philo->program->philos_done_eating += 1;
	if (thread_unlock(&philo->program->lock))
		return ((void *)1);
	return (NULL);
}

void	*one_fork_routine(void	*arg)
{
	t_philosopher *philo;
	int retval;
	
	philo = (t_philosopher *)arg;
	if (say_ready(philo->program) || wait_other_actors(philo->program))
		return ((void *)1);
	retval = is_everyone_alive(philo->program);
	if (retval == -1)
		return((void *)1);
	log_status(philo, "has taken a fork");
	while (retval)
	{
		retval = is_everyone_alive(philo->program);
		if (retval == -1)
			return ((void *)1);
	}
	return (NULL);
}
