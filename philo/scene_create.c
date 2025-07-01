/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 05:40:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 02:56:41 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <stdio.h>

static int	init_mutexes(t_program *p)
{
	int	i;

	if (pthread_mutex_init(&p->finish_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&p->write_lock, NULL) != 0)
		return (1);
	p->forks = malloc(sizeof(pthread_mutex_t) * p->data->num_of_philos);
	if (!p->forks)
		return (1);
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		if (pthread_mutex_init(&p->forks[i], NULL) != 0)
			return (1);
	}
	return (0);
}

static void	assign_forks(t_program *p)
{
	int	i;

	i = 0;
	while (i < p->data->num_of_philos)
	{
		p->philosophers[i].fork_one = &p->forks[i];
		if (p->data->num_of_philos == 1)
			p->philosophers[i].fork_two = NULL;
		else
		{
			p->philosophers[i].fork_two = &p->forks[(i + 1)
				% p->data->num_of_philos];
		}
		i++;
	}
}

static int	init_philosophers(t_program *p)
{
	int	i;

	p->philosophers = malloc(sizeof(t_philosopher) * p->data->num_of_philos);
	if (!p->philosophers)
		return (1);
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		p->philosophers[i].id = i + 1;
		p->philosophers[i].eaten_meal = 0;
		p->philosophers[i].last_meal = 0;
		p->philosophers[i].data = p->data;
		p->philosophers[i].program = p;
		if (pthread_mutex_init(&p->philosophers[i].lock, NULL) != 0)
			return (1);
	}
	assign_forks(p);
	return (0);
}

int	create_scene(t_program *p)
{
	if (init_mutexes(p))
		return (1);
	if (init_philosophers(p))
		return (1);
	p->philos_done_eating = 0;
	return (0);
}
