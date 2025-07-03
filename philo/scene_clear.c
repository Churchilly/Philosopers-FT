/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_clear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 02:23:19 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/03 03:31:35 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>

static void	free_program_locks(t_program *p)
{
	int	i;

	if (p->forks)
	{
		i = -1;
		while (++i < p->data->num_of_philos)
			pthread_mutex_destroy(&p->forks[i]);
		free(p->forks);
		p->forks = NULL;
	}
	pthread_mutex_destroy(&p->program_lock);
	pthread_mutex_destroy(&p->write_lock);
}

static void	free_philos_locks(t_program *p)
{
	int	i;

	if (p->philosophers)
	{
		i = -1;
		while (++i < p->data->num_of_philos)
			pthread_mutex_destroy(&p->philosophers[i].lock);
		free(p->philosophers);
		p->philosophers = NULL;
	}
}

void	clear_scene(t_program *p)
{
	free_philos_locks(p);
	free_program_locks(p);
}
