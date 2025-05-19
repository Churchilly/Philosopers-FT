/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 22:22:36 by yusudemi          #+#    #+#             */
/*   Updated: 2025/05/19 07:39:44 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>

void	*monitoring(void *arg);
void	*routine(void *arg);
void	*one_fork_routine(void *arg);
suseconds_t	get_current_time(void);

typedef void	*(*routine_func)(void *d);

int	play_scene(t_program *p)
{
	int		i;
	int		status;
	void	*retval;
	
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		status = pthread_join(p->philosophers[i].thread, &retval);
		if (status != 0)
			return (printf("pthread_join() failed: %d.\n", status), 1);
		if ((long)retval == (long)1)
			return (1);
	}
	p->start_time = get_current_time();
	status = pthread_join(p->monitor, &retval);
	if (status != 0)
		return (printf("pthread_join(), failed: %d.\n", status), 1);
	if ((long)retval == (long)1)
		return (1);
	return (0);
}

routine_func	get_routine(t_data *d)
{
	if (d->must_eat == 0)
		return (NULL);
	else if (d->num_of_philos == 1)
		return (one_fork_routine);
	return (routine);
}

static int	init_philo(t_program *p, int ptr)
{
	int status;
	
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
	p->philosophers[ptr].eaten_meal = 0;
	p->philosophers[ptr].last_meal = 0;
	p->philosophers[ptr].program = p;
	p->philosophers[ptr].data = p->data;
	return (0);
}

int	establish_actors(t_program *p)
{
	int	i;
	int	status;
	routine_func	func;
	
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
	status = pthread_create(&p->monitor, NULL, monitoring, (void *)p);
	if (status != 0)
		return (printf("pthread_create() failed: %d.\n", status), 1);
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
