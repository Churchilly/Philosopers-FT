/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:41:19 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/30 19:16:08 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

void	input_test(t_data *data) // for testing purposes
{
	printf("%d\n", data->num_of_philos);
	printf("%ld\n", data->time_to_die);
	printf("%ld\n", data->time_to_eat);
	printf("%ld\n", data->time_to_sleep);
	printf("%d\n", data->must_eat);
}

void	ft_bzero(void *addr, int size)
{
	char	*bytes;
	int		i;

	bytes = addr;
	i = 0;
	while (i < size)
		bytes[i++] = '\0';
	return ;
}



void	safe_print(t_philosopher *philo, char *message)
{
	suseconds_t	timestamp;

	sem_wait(philo->semaphores->write_lock);
	timestamp = get_current_time() - philo->data->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, message);
	sem_post(philo->semaphores->write_lock);
}


int	main(int argc, char **argv)
{
	t_data			data;
	t_program		program;
	
	ft_bzero(&data, sizeof(t_data));
	insert_input(argc, argv, &data);
	//input_test(&data);
	create_scene(&program, &data);
	establish_actors(&program);
	end_scene(&program);
	return (0);
}

void	clear_scene(t_program	*p)
{
	int	i;
	
	// CLEAN FORK
	printf("-FORK CLEANUP START-\n");
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
	printf("-FORKS CLEANED-\n");
	
	// CLEAN PHILOSOPHERS
	if (p->philosophers)
	{
		i = -1;
		while (++i < p->data->num_of_philos)
			sem_destroy(&p->philosophers[i].last_meal_lock);
		free(p->philosophers);
		p->philosophers = NULL;
	}
	
	// CLEAN SEMAPs
	printf("-SEMAPHORE CLEANUP START-\n");
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
		printf("-SEMAPHORES CLEANED-\n");
	}
}

void	establish_actors(t_program *p)
{
	int		i;
	pid_t	pid;

	i = -1;
	while (++i < p->data->num_of_philos)
	{
		pid = fork();
		if (pid < 0)
			exit(1);
		if (pid == 0) // Child process
		{
			// Use pre-initialized philosopher
			routine(&p->philosophers[i]);
			exit(0);
		}
		else // Parent process
		{
			p->child_ids[i] = pid;
		}
	}
}
