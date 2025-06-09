/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 22:22:36 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/09 23:17:04 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static t_routine_func	get_routine(t_data *d)
{
	if (d->must_eat == 0)
		return (NULL);
	else if (d->num_of_philos == 1)
		return (one_fork_routine);
	return (routine);
}

static int	init_philo(t_program *p, int ptr)
{
	int	status;

	p->philosophers[ptr].id = ptr;
	if (ptr % 2 == 1)
	{
		p->philosophers[ptr].fork_one = &(p->forks[ptr]);
		p->philosophers[ptr].fork_two = &(p->forks[(ptr + 1)
				% p->data->num_of_philos]);
	}
	else
	{
		p->philosophers[ptr].fork_one = &(p->forks[(ptr + 1)
				% p->data->num_of_philos]);
		p->philosophers[ptr].fork_two = &(p->forks[ptr]);
	}
	status = pthread_mutex_init(&p->philosophers[ptr].lock, NULL);
	if (status != 0)
		return (printf("pthread_mutex_init() failed: %d.\n", status), 1);
	p->philosophers[ptr].dead = 0;
	p->philosophers[ptr].eaten_meal = 0;
	p->philosophers[ptr].last_meal = 0;
	p->philosophers[ptr].program = p;
	p->philosophers[ptr].data = p->data;
	return (0);
}

int	establish_actors(t_program *p)
{
	int				i;
	int				status;
	t_routine_func	func;

	func = get_routine(p->data);
	if (!func)
		return (2);
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		if (init_philo(p, i))
			return (1);
		status = pthread_create(&p->philosophers[i].thread, NULL,
				func, (void *)&(p->philosophers[i]));
		if (status != 0)
			return (printf("pthread_create() failed: %d.\n", status), 1);
	}
	return (0);
}

int	create_scene(t_program *p)
{
	int	i;
	int	status;

	p->forks = malloc(p->data->num_of_philos * sizeof(pthread_mutex_t));
	if (!p->forks)
		return (printf("malloc() failed: %d.\n", errno), 1);
	p->philosophers = malloc(p->data->num_of_philos * sizeof(t_philosopher));
	if (!p->philosophers)
		return (printf("malloc() failed: %d.\n", errno), 1);
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		status = pthread_mutex_init(&p->forks[i], NULL);
		if (status != 0)
			return (printf("pthread_mutex_init() failed: %d.\n", status), 1);
	}
	status = pthread_mutex_init(&p->lock, NULL);
	if (status != 0)
		return (printf("pthread_mutex_init() failed: %d.\n", status), 1);
	return (0);
}
