/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:12:05 by yusudemi          #+#    #+#             */
/*   Updated: 2025/02/11 19:48:48 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

static void	initialize_data(t_data *data)
{
	data->number_of_philosophers = 0;
	data->time_to_die = 0;
	data->time_to_eat = 0;
	data->time_to_sleep = 0;
	data->number_of_times_each_philosopher_must_eat = 0;
	data->forks = NULL;
	data->philosophers = NULL;
}

static void	*routine(void *arg);

static void	create_scene(t_data *d)
{
	int	i;
	
	d->forks = malloc(d->number_of_philosophers * sizeof(pthread_mutex_t));
	if (!d->forks)
		exit(1); // free and exit needed ofc
	d->philosophers = malloc(d->number_of_philosophers * sizeof(t_philosopher));
	if (!d->philosophers)
		exit(1);
	i = -1;
	while (++i < d->number_of_philosophers)
		if (pthread_mutex_init(&d->forks[i], NULL))
			exit(1);
	i = -1;
	while (++i < d->number_of_philosophers)
	{
		d->philosophers[i].id = i;
		d->philosophers[i].left_fork = &d->forks[i];
		d->philosophers[i].right_fork = &d->forks[(i + 1) % d->number_of_philosophers];
		if (pthread_create(&d->philosophers[i].thread, NULL, routine, (void *)&d->philosophers))
			exit(1);
	}
	i = -1;
	while (++i < d->number_of_philosophers)
		if (pthread_join(d->philosophers[i].thread, NULL))
			exit(1);
}

int main(int argc, char **argv)
{
	t_data	data;
	int	i;
	
	initialize_data(&data);
	insert_input(argc, argv, &data);
	create_scene(&data);
	while (++i < data.number_of_philosophers)
		if (pthread_mutex_destroy(&data.forks[i]))
			exit(1);
	free(data.forks);
	free(data.philosophers);
	return (0);
}
