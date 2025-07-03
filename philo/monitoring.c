/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 05:40:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/03 04:44:39 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>
#include <stdio.h>

static int	check_simulation_status(t_program *p)
{
	int	status;

	lock_mutex(&p->program_lock);
	status = ((p->everyone_ok) && !(p->data->must_eat != -1
				&& p->philos_done_eating == p->data->num_of_philos));
	unlock_mutex(&p->program_lock);
	return (status);
}

static int	is_philo_alive(t_philosopher *philo)
{
	suseconds_t	current_time;
	suseconds_t	time_since_last_meal;
	suseconds_t	last_meal_time;

	current_time = get_elapsed_time(philo->program);
	lock_mutex(&philo->lock);
	last_meal_time = philo->last_meal;
	unlock_mutex(&philo->lock);
	time_since_last_meal = current_time - last_meal_time;
	if (time_since_last_meal >= philo->data->time_to_die)
		return (0);
	return (1);
}

void	*monitoring(void *arg)
{
	t_program	*p;
	int			i;

	p = (t_program *)arg;
	wait_actors(p);
	usleep(p->data->time_to_die);
	while (check_simulation_status(p))
	{
		i = 0;
		while (i < p->data->num_of_philos && check_simulation_status(p))
		{
			if (!is_philo_alive(&p->philosophers[i]))
			{
				announce_death(&p->philosophers[i]);
				return (NULL);
			}
			i++;
		}
		usleep(1);
	}
	return (NULL);
}
