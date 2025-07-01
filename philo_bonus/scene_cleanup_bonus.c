/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_cleanup_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 06:00:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/02 00:36:32 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

static void	clean_philosophers(t_program *p)
{
	if (p->philosophers)
	{
		free(p->philosophers);
		p->philosophers = NULL;
	}
}

static void	clean_semaphores(t_program *p)
{
	if (p->semaphores)
	{
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
		if (p->semaphores->finish_lock)
		{
			sem_close(p->semaphores->finish_lock);
			sem_unlink("/meal_sem");
		}
		if (p->semaphores->term_lock)
		{
			sem_close(p->semaphores->term_lock);
			sem_unlink("/term_sem");
		}
		free(p->semaphores);
		p->semaphores = NULL;
	}
}

static void	close_semaphores(t_program *p)
{
	if (p->semaphores)
	{
		if (p->semaphores->write_lock)
		{
			sem_close(p->semaphores->write_lock);
		}
		if (p->semaphores->forks)
		{
			sem_close(p->semaphores->forks);
		}
		if (p->semaphores->finish_lock)
		{
			sem_close(p->semaphores->finish_lock);
		}
		if (p->semaphores->term_lock)
		{
			sem_close(p->semaphores->term_lock);
		}
		free(p->semaphores);
		p->semaphores = NULL;
	}
}
void	clear_fork(t_program *p, int exit_status)
{
	if (p->child_ids)
	{
		free(p->child_ids);
		p->child_ids = NULL;
	}
	if (p->philosophers)
	{
		free(p->philosophers);
		p->philosophers = NULL;
	}
	close_semaphores(p);
	exit(exit_status);
}

void	clear_scene(t_program *p, int exit_status)
{
	if (p->child_ids)
	{
		free(p->child_ids);
		p->child_ids = NULL;
	}
	clean_philosophers(p);
	clean_semaphores(p);
	(void)exit_status;
	//exit(exit_status);
}
