/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:41:19 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/20 04:10:18 by yusudemi         ###   ########.fr       */
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
#include <pthread.h>

void	input_test(t_data *data)
{
	printf("%d\n", data->num_of_philos);
	printf("%d\n", data->time_to_die);
	printf("%d\n", data->time_to_eat);
	printf("%d\n", data->time_to_sleep);
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

t_semaphores	*create_semaphores(t_program *p)
{
	
}

void	create_scene(t_program *p, t_data *d)
{
	int	i;
	
	p->data = d;
	p->child_ids = mallloc(sizeof(pid_t) * d->num_of_philos);
	p->semaphores = malloc(sizeof(t_semaphores));
	if (!p->child_ids || !p->semaphores)
		exit(1);
	p->semaphores->die = sem_open("/die_sem", O_CREAT, 0644, 1);
	p->semaphores->fork = sem_open("/fork_sem", O_CREAT, 0644, 1);
	p->semaphores->done = sem_open("/meal_sem", O_CREAT, 0644, 1);
	p->semaphores->ready = sem_open("/ready_sem", O_CREAT, 0644, 1);
	if (p->semaphores->die == SEM_FAILED
		|| p->semaphores->fork == SEM_FAILED
		|| p->semaphores->done == SEM_FAILED
		|| p->semaphores->ready == SEM_FAILED)
		exit(1);
	i = -1;
	while (++i < d->num_of_philos)
		p->child_ids[i] = -1;
}

void	*meal_monitor(void	*arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		if (philo->data->must_eat > 0 && philo->eaten_meal == philo->data->must_eat)
		{
			//say done
			exit(1);
		}
	}
}

void	routine(t_philosopher *philo)
{
	pthread_t	death_checker;
	pthread_t	meal_checker;

	pthread_create(&death_checker, NULL, death_monitor, philo);
	if (philo->data->must_eat >= 0)
		pthread_create(&meal_checker, NULL, meal_monitor, philo);
	//routine with semaphores must recode eat-sleep-think functions .p
}
void	establish_actors(t_program *p)
{
	int		i;
	pid_t	pid;
	t_philosopher	philo;

	i = -1;
	while (++i < p->data->num_of_philos)
	{
		pid = fork();
		if (pid < 0)
			exit(1);
		if (pid == 0)
		{
			p->child_ids[i] = pid;
			philo.id = i + 1;
			philo.eaten_meal = 0;
			philo.data = p->data;
			philo.semaphores = p->semaphores;
			//wait all philos ready with ready semaphore
			//start routine
			exit(0);
		}
	}
}

int	main(int argc, char **argv)
{
	t_data			data;
	t_program		program;
	
	ft_bzero(&data, sizeof(t_data));
	if (insert_input(argc, argv, &data))
		return (1);
	input_test(&data);
	create_scene(&program, &data);
	establish_actors();
	
}

void	at_exit(t_program	*p) // maybe a destructor ?_?
{
	int	i;
	
	// CLEAN FORK
	printf("-FORK CLEANUP START-\n");
	if (p->child_ids)
	{
		i = -1;
		while (++i < p->data->num_of_philos)
		{
			if (p->child_ids > 0)
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
	// CLEAN SEMAPs
	printf("-SEMAPHORE CLEANUP START-\n");
	if (p->semaphores)
	{
		if (p->semaphores->die)
		{
			sem_close(p->semaphores->die);
			sem_unlink("/die_sem");
		}
		if (p->semaphores->write)
		{
			sem_close(p->semaphores->write);
			sem_unlink("/die_sem");
		}
		if (p->semaphores->fork)
		{
			sem_close(p->semaphores->fork);
			sem_unlink("/die_sem");
		}
		if (p->semaphores->meal)
		{
			sem_close(p->semaphores->meal);
			sem_unlink("/die_sem");
		}
		if (p->semaphores->ready)
		{
			sem_close(p->semaphores->ready);
			sem_unlink("/die_sem");
		}
		free(p->semaphores);
		p->semaphores = NULL;
		printf("-SEMAPHORES CLEANED-\n");
	}
}