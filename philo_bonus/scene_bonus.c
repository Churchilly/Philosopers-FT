/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:45:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 05:00:49 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static t_semaphores	*create_semaphores(t_data *d)
{
	t_semaphores	*sems;

	sems = malloc(sizeof(t_semaphores));
	if (!sems)
		return (NULL);
	sem_unlink("/die_sem");
	sem_unlink("/fork_sem");
	sem_unlink("/write_sem");
	sem_unlink("/meal_sem");
	sems->die_lock = sem_open("/die_sem", O_CREAT, 0644, 1);
	sems->forks = sem_open("/fork_sem", O_CREAT, 0644, d->num_of_philos);
	sems->write_lock = sem_open("/write_sem", O_CREAT, 0644, 1);
	sems->meal_complete = sem_open("/meal_sem", O_CREAT, 0644, 0);
	if (sems->die_lock == SEM_FAILED
		|| sems->forks == SEM_FAILED
		|| sems->write_lock == SEM_FAILED
		|| sems->meal_complete == SEM_FAILED)
	{
		free(sems);
		return (NULL);
	}
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
		philos[i].last_meal = 0;
		philos[i].data = p->data;
		philos[i].semaphores = p->semaphores;
		if (sem_init(&philos[i].last_meal_lock, 0, 1) != 0)
		{
			while (--i >= 0)
				sem_destroy(&philos[i].last_meal_lock);
			free(philos);
			return (NULL);
		}
	}
	return (philos);
}

void	create_scene(t_program *p, t_data *d)
{
	int	i;

	p->data = d;
	p->data->start_time = get_current_time();
	p->child_ids = malloc(sizeof(pid_t) * d->num_of_philos);
	if (!p->child_ids)
		exit(1);
	i = -1;
	while (++i < d->num_of_philos)
		p->child_ids[i] = -1;
	p->semaphores = create_semaphores(d);
	if (!p->semaphores)
	{
		free(p->child_ids);
		exit(1);
	}
	p->philosophers = init_philos(p);
	if (!p->philosophers)
	{
		free(p->child_ids);
		free(p->semaphores);
		exit(1);
	}
}

static int	handle_process_exit(t_program *p, int status)
{
	int	i;

	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
			return (1);
		else
		{
			i = -1;
			while (++i < p->data->num_of_philos)
			{
				if (p->child_ids[i] > 0)
					kill(p->child_ids[i], SIGKILL);
			}
			return (-1);
		}
	}
	return (0);
}

void	end_scene(t_program *p)
{
	int		status;
	int		completed_meals;
	int		result;
	pid_t	finished_pid;

	completed_meals = 0;
	while (completed_meals < p->data->num_of_philos)
	{
		finished_pid = waitpid(-1, &status, 0);
		if (finished_pid == -1)
			break ;
		result = handle_process_exit(p, status);
		if (result == 1)
			completed_meals++;
		else if (result == -1)
			break ;
	}
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}
