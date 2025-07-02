/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_actors_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:16:07 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/02 22:36:39 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

void	establish_actors(t_program *p)
{
	int		i;
	pid_t	pid;

	if (p->data->must_eat == 0)
		clear_scene(p, 0);
	start_timer(p);
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		pid = fork();
		if (pid < 0)
			clear_scene(p, 1);
		if (pid == 0)
		{
			if (p->data->num_of_philos == 1)
				one_fork_routine(&p->philosophers[i]);
			else
				routine(&p->philosophers[i]);
			clear_scene(p, 0);
		}
		else
			p->child_ids[i] = pid;
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
			clear_scene(p, 1);
		}
	}
	return (0);
}

void	increase_philos_done(t_program *p)
{
	sem_wait(p->semaphores->finish_lock);
	p->philos_done_eating += 1;
	sem_post(p->semaphores->finish_lock);
}

int	check_philos_done(t_program *p)
{
	int	ret;

	sem_wait(p->semaphores->finish_lock);
	ret = p->philos_done_eating == p->data->num_of_philos;
	sem_post(p->semaphores->finish_lock);
	return (ret);
}

void	end_scene(t_program *p)
{
	int		status;
	int		result;
	pid_t	finished_pid;

	while (!check_philos_done(p))
	{
		finished_pid = waitpid(-1, &status, 0);
		if (finished_pid == -1)
			clear_scene(p, 1);
		result = handle_process_exit(p, status);
		if (result)
			increase_philos_done(p);
	}
	while (waitpid(-1, NULL, WNOHANG) > 0)
		usleep(100);
}
