/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 02:00:41 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/30 19:07:28 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <unistd.h>

static void	check_and_insert(int *p, char *d)
{
	if (!is_num(d))
		input_error("All must be number.");
	*p = ft_atoi(d);
}

void	insert_input(int argc, char **argv, t_data *d)
{
	int	buffer[4];

	if (argc < 5)
		input_error("Missing parameters\nUsage: ./philo\
 <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep>\
 [number_of_times_each_philosopher_must_eat]");
	if (argc > 6)
		input_error("More parameters than expected\nUsage: ./philo\
 <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep>\
 [number_of_times_each_philosopher_must_eat]");
	d->must_eat = -1;
	if (argc == 6)
	{
		--argc;
		check_and_insert(&(d->must_eat), argv[argc]);
	}
	while (--argc > 0)
		check_and_insert(&(buffer[argc - 1]), argv[argc]);
	d->num_of_philos = buffer[0];
	d->time_to_die = buffer[1];
	d->time_to_eat = buffer[2];
	d->time_to_sleep = buffer[3];
}
