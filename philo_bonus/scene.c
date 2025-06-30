/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:08:44 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/30 18:18:24 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

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
		free(sems);
		return (NULL);
	return (sems);
}

static t_philosopher	*create_philos(t_program *p)
{
	int	i;
	t_philosopher	*philos;
	philos = malloc(sizeof(t_philosopher) * p->data->num_of_philos);
	if (philos)
		return (NULL);
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		p->philosophers[i].id = i + 1;
		p->philosophers[i].eaten_meal = 0;
		p->philosophers[i].last_meal = 0;
		p->philosophers[i].data = p->data;
		p->philosophers[i].semaphores = p->semaphores;
		if (sem_init(&p->philosophers[i].last_meal_lock, 0, 1) != 0)
			return (NULL);
	}
	return (philos);
}

void	create_scene(t_program *p, t_data *d)
{
	int	i;
	
	p->data = d;
	p->child_ids = malloc(sizeof(pid_t) * d->num_of_philos);
	p->semaphores = create_semaphores(d);
	p->philosophers = create_philos(p);
	if (!p->child_ids || !p->semaphores || !p->philosophers)
		exit(1);
	i = -1;
	while (++i < d->num_of_philos)
		p->child_ids[i] = -1;
}

void	end_scene(t_program *p) 
{
	int		status;
	int		i;
	int		completed_meals;
	pid_t	finished_pid;

	completed_meals = 0;
	while (1)
	{
		finished_pid = waitpid(-1, &status, WNOHANG);
		
		if (finished_pid > 0)
		{
			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) == 0) // Successfully completed meals
				{
					completed_meals++;
					if (p->data->must_eat > 0 && completed_meals == p->data->num_of_philos)
					{
						// All philosophers completed their meals
						cleanup_and_exit(p);
						return;
					}
				}
				else // A philosopher died
				{
					// Kill all remaining processes
					i = -1;
					while (++i < p->data->num_of_philos)
					{
						if (p->child_ids[i] > 0)
							kill(p->child_ids[i], SIGKILL);
					}
					cleanup_and_exit(p);
					return;
				}
			}
		}
		usleep(1000); // Check every 1ms
	}
}