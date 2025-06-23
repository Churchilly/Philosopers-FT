/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:20:31 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/23 19:47:30 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

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

static t_routine_func	get_routine(t_data *d)
{
	if (d->must_eat == 0)
		return (NULL);
	else if (d->num_of_philos == 1)
		return (one_fork_routine);
	return (routine);
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
	status = pthread_create(&p->monitor, NULL,
		monitoring, (void *)p);
	if (status != 0)
		return (printf("pthread_create() failed: %d.\n", status), 1);
	return (0);
}

int	wait_other_actors(t_program *p) // no need ig
{
	int	ready;

	ready = 0;
	while (!ready)
	{
		if (thread_lock(&p->lock))
			return (1);
		if (p->running_thread_count == (p->data->num_of_philos + 1))
			ready = 1;
		if (thread_unlock(&p->lock))
			return (1);
		usleep(100);
	}
	return (0);
}

int	say_ready(t_program *p) // no need either
{
	if (thread_lock(&p->lock))
		return (1);
	p->running_thread_count += 1;
	if (thread_unlock(&p->lock))
		return (1);
	return (0);
}
