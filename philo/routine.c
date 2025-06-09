/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:04:25 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/10 00:31:38 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	am_i_alive(t_philosopher *philo)
{
	int	ret;

	ret = 1;
	if (thread_lock(&philo->lock))
		return (-1);
	if (philo->dead == 1)
		ret = 0;
	if (thread_unlock(&philo->lock))
		return (-1);
	return (ret);
}

static int	say_done(t_philosopher *philo)
{
	if (thread_lock(&philo->program->lock))
		return (1);
	philo->program->philos_done_eating += 1;
	if (thread_unlock(&philo->program->lock))
		return (1);
	return (0);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;
	int				status;

	philo = (t_philosopher *)arg;
	if (say_ready(philo->program) || wait_other_actors(philo->program))
		return ((void *)1);
	status = am_i_alive(philo);
	if (status == -1)
		return ((void *)1);
	while (status)
	{
		if (philo->data->must_eat != -1)
			if (philo->eaten_meal == philo->data->must_eat)
				break ;
		if (philo_eat(philo) || philo_sleep(philo) || philo_think(philo))
			return ((void *)1);
		status = am_i_alive(philo);
		if (status == -1)
			return ((void *)1);
	}
	if (say_done(philo))
		return ((void *)1);
	return (NULL);
}

void	*one_fork_routine(void	*arg)
{
	t_philosopher	*philo;
	int				status;

	philo = (t_philosopher *)arg;
	if (say_ready(philo->program) || wait_other_actors(philo->program))
		return ((void *)1);
	log_status(philo, "has taken a fork");
	status = am_i_alive(philo);
	if (status == -1)
		return ((void *)1);
	while (status)
	{
		status = am_i_alive(philo);
		if (status == -1)
			return ((void *)1);
	}
	if (say_done(philo))
		return ((void *)1);
	return (NULL);
}
