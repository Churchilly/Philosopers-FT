/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:15:55 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/23 19:44:57 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int	create_scene(t_program *p)
{
	int	i;
	int	status;
	
	p->philosophers = malloc(p->data->num_of_philos * sizeof(t_philosopher));
	if (!p->philosophers)
		return (printf("malloc() failed: %d.\n", errno), 1);
	p->forks = malloc(p->data->num_of_philos * sizeof(pthread_mutex_t));
	if (!p->forks)
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
	status = pthread_mutex_init(&p->write_lock, NULL);
	if (status != 0)
		return (printf("pthread_mutex_init() failed: %d.\n", status), 1);
	return (0);
}

int	end_scene(t_program *p)
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
	status = pthread_join(p->monitor, retval);
	if (status != 0)
		return (printf("pthread_join() failed: %d.\n", status), 1);
	if ((long)retval == (long)1)
		return (1);
	return (0);
}

int	clear_scene(t_program *p)
{
	int	i;
	int	status;

	i = -1;
	while (++i < p->data->num_of_philos)
	{
		status = pthread_mutex_destroy(&p->forks[i]);
		if (status != 0)
			return (printf("pthread_mutex_destroy() failed: %d.\n", status), 1);
		status = pthread_mutex_destroy(&(p->philosophers[i].lock));
		if (status != 0)
			return (printf("pthread_mutex_destroy() failed: %d.\n", status), 1);
	}
	status = pthread_mutex_destroy(&p->lock);
	if (status != 0)
		return (printf("pthread_mutex_destroy() failed: %d.\n", status), 1);
	status = pthread_mutex_destroy(&p->write_lock);
	if (status != 0)
		return (printf("pthread_mutex_destroy() failed: %d.\n", status), 1);
	if (p->forks)
		free(p->forks);
	if (p->philosophers)
		free(p->philosophers);
	return (0);
}
