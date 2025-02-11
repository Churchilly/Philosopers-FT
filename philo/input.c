/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 02:00:41 by yusudemi          #+#    #+#             */
/*   Updated: 2025/02/09 05:44:19 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include "philosophers.h"

static void	input_error(char *message)
{
	char	*p;

	p = message;
	while (*p)
		p++;
	write(2, "Error: ", 8);
	write(2, message, p - message);
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

static int	check_sign(const char **str)
{
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			input_error("Parameters can't be negative.");
		(*str)++;
		return (1);
	}
	return (0);
}

static int	ft_atoi(const char *str)
{
	long	num;
	int		prefix;

	num = 0;
	prefix = 0;
	while (*str == 9 || *str == 10 || *str == 11
		|| *str == 12 || *str == 13 || *str == 32)
		str++;
	prefix = check_sign(&str);
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		if (num > INT_MAX)
			input_error("Input must be smaller than INT_MAX.");
		str++;
	}
	if (prefix && num == 0)
		input_error("Invalid input.");
	return ((int)(num));
}

static bool	is_num(char *str)
{
	if (*str == 0 || !str)
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (*str > '9' || *str < '0')
			return (false);
		str++;
	}
	return (true);
}

void	insert_input(int argc, char **argv, t_data *d)
{
	int	buffer[4];
	printf("[%d]\n", argc);
	if (argc < 5)
		input_error("Missing parameters\nUsage: ./philo\
 <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep>\
 [number_of_times_each_philosopher_must_eat]");
	if (argc == 6)
	{
		--argc;
		if (!is_num(argv[argc]))
			input_error("All must be number.");
		d->number_of_times_each_philosopher_must_eat = ft_atoi(argv[argc]);
	}
	while (--argc > 0)
	{
		if (!is_num(argv[argc]))
			input_error("All must be number.");
		buffer[argc - 1]= ft_atoi(argv[argc]);
	}
	d->number_of_philosophers = buffer[0];
	d->time_to_die = buffer[1];
	d->time_to_eat = buffer[2];
	d->time_to_sleep = buffer[3];
}
