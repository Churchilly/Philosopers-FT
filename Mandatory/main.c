/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:12:05 by yusudemi          #+#    #+#             */
/*   Updated: 2025/02/09 07:40:57 by yusudemi         ###   ########.fr       */
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

static void	create_scene(t_data *d)
{
	d->forks = malloc(d->number_of_philosophers * sizeof(pthread_mutex_t));
	if (!d->forks)
		exit(1); // free and exit needed ofc
	d->philosophers = malloc(d->number_of_philosophers * sizeof(t_philosophers));
	if (!d->philosophers)
		exit(1);
}

int main(int argc, char **argv)
{
	t_data	data;
	
	initialize_data(&data);
	insert_input(argc, argv, &data);
	create_scene(&data);
	
	return (0);
}
