/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 06:00:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 05:00:49 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

static void	clean_child_processes(t_program *p)
{
	int	i;

	if (p->child_ids)
	{
		i = -1;
		while (++i < p->data->num_of_philos)
		{
			if (p->child_ids[i] > 0)
				kill(p->child_ids[i], SIGKILL);
		}
		i = -1;
		while (++i < p->data->num_of_philos)
		{
			if (p->child_ids[i] > 0)
				waitpid(p->child_ids[i], NULL, 0);
		}
		free(p->child_ids);
		p->child_ids = NULL;
	}
}

static void	clean_philosophers(t_program *p)
{
	int	i;

	if (p->philosophers)
	{
		i = -1;
		while (++i < p->data->num_of_philos)
			sem_destroy(&p->philosophers[i].last_meal_lock);
		free(p->philosophers);
		p->philosophers = NULL;
	}
}

static void	clean_semaphores(t_program *p)
{
	if (p->semaphores)
	{
		if (p->semaphores->die_lock)
		{
			sem_close(p->semaphores->die_lock);
			sem_unlink("/die_sem");
		}
		if (p->semaphores->write_lock)
		{
			sem_close(p->semaphores->write_lock);
			sem_unlink("/write_sem");
		}
		if (p->semaphores->forks)
		{
			sem_close(p->semaphores->forks);
			sem_unlink("/fork_sem");
		}
		if (p->semaphores->meal_complete)
		{
			sem_close(p->semaphores->meal_complete);
			sem_unlink("/meal_sem");
		}
		free(p->semaphores);
		p->semaphores = NULL;
	}
}

void	clear_scene(t_program *p)
{
	clean_child_processes(p);
	clean_philosophers(p);
	clean_semaphores(p);
}
