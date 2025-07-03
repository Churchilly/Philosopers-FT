/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:45:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/03 04:24:53 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdlib.h>
#include <fcntl.h>

static t_semaphores	*create_semaphores(t_data *d)
{
	t_semaphores	*sems;

	sems = malloc(sizeof(t_semaphores));
	if (!sems)
		return (NULL);
	sem_unlink("/fork_sem");
	sem_unlink("/write_sem");
	sem_unlink("/meal_sem");
	sem_unlink("/term_sem");
	sem_unlink("/start_sem");
	sems->forks = sem_open("/fork_sem", O_CREAT, 0644, d->num_of_philos);
	sems->write_lock = sem_open("/write_sem", O_CREAT, 0644, 1);
	sems->finish_lock = sem_open("/meal_sem", O_CREAT, 0644, 1);
	sems->term_lock = sem_open("/term_sem", O_CREAT, 0644, 0);
	sems->start_lock = sem_open("/start_sem", O_CREAT, 0644, 0);
	if (sems->forks == SEM_FAILED
		|| sems->write_lock == SEM_FAILED
		|| sems->term_lock == SEM_FAILED
		|| sems->start_lock == SEM_FAILED
		|| sems->finish_lock == SEM_FAILED)
		return (NULL);
	return (sems);
}

static t_philosopher	*init_philos(t_program *p)
{
	t_philosopher	*philos;
	int				i;

	philos = malloc(sizeof(t_philosopher) * p->data->num_of_philos);
	if (!philos)
		return (NULL);
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].eaten_meal = 0;
		philos[i].program = p;
		philos[i].alive = 1;
		philos[i].last_meal = 0;
		philos[i].data = p->data;
		philos[i].semaphores = p->semaphores;
	}
	return (philos);
}

void	create_scene(t_program *p, t_data *d)
{
	int	i;

	p->data = d;
	p->philos_done_eating = 0;
	p->child_ids = malloc(sizeof(pid_t) * d->num_of_philos);
	if (!p->child_ids)
		clear_scene(p, 1);
	i = -1;
	while (++i < d->num_of_philos)
		p->child_ids[i] = -1;
	p->semaphores = create_semaphores(d);
	if (!p->semaphores)
		clear_scene(p, 1);
	p->philosophers = init_philos(p);
	if (!p->philosophers)
		clear_scene(p, 1);
}
