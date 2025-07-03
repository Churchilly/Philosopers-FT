/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_cleanup_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 06:00:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/03 05:00:07 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

static void	delete_semaphore(sem_t *sem, const char *name)
{
	sem_close(sem);
	sem_unlink(name);
}

static void	clean_semaphores(t_program *p)
{
	if (p->semaphores)
	{
		if (p->semaphores->write_lock)
		{
			delete_semaphore(p->semaphores->write_lock, "/write_sem");
		}
		if (p->semaphores->forks)
		{
			delete_semaphore(p->semaphores->forks, "/fork_sem");
		}
		if (p->semaphores->finish_lock)
		{
			delete_semaphore(p->semaphores->finish_lock, "/meal_sem");
		}
		if (p->semaphores->term_lock)
		{
			delete_semaphore(p->semaphores->term_lock, "/term_sem");
		}
		if (p->semaphores->start_lock)
		{
			delete_semaphore(p->semaphores->start_lock, "/start_sem");
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
		if (p->semaphores->start_lock)
		{
			sem_close(p->semaphores->start_lock);
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
	if (p->philosophers)
	{
		free(p->philosophers);
		p->philosophers = NULL;
	}
	clean_semaphores(p);
	exit(exit_status);
}
