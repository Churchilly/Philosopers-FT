/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 02:00:41 by yusudemi          #+#    #+#             */
/*   Updated: 2025/05/19 00:35:52 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include "philosophers.h"

static int	input_error(char *message)
{
	char	*p;

	p = message;
	while (*p)
		p++;
	write(2, "Error: ", 8);
	write(2, message, p - message);
	write(2, "\n", 1);
	return (0);
}

static int	check_sign(const char **str)
{
	int ret;

	ret = 0;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			return (input_error("Parameters can't be negative."), -1);
		ret = 1;
		(*str)++;
	}
	return (ret);
}

static int	ft_atoi(const char *str)
{
	long	num;
	int		sign;

	num = 0;
	while (*str == 9 || *str == 10 || *str == 11
		|| *str == 12 || *str == 13 || *str == 32)
		str++;
	sign = check_sign(&str);
	if (sign < 0)
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		if (num > INT_MAX)
			return (input_error("Input must be smaller than INT_MAX."), -1);
		str++;
	}
	if (sign && num == 0)
		return (input_error("Invalid input."), -1);
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

int check_argc(int argc)
{
	if (argc < 5)
		return (input_error("Missing parameters\nUsage: ./philo\
 <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep>\
 [number_of_times_each_philosopher_must_eat]"));
	if (argc > 6)
		return (input_error("More parameters than expected\nUsage: ./philo\
 <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep>\
 [number_of_times_each_philosopher_must_eat]"));
	return (1);
}

int	check_and_insert(int *p, char *d)
{
	if (!is_num(d))
		return (input_error("All must be number."), 1);
	*p = ft_atoi(d);
	if ((*p) < 0)
		return (1);
	return (0);
}
int	insert_input(int argc, char **argv, t_data *d)
{
	int	buffer[4];

	if (!check_argc(argc))
		return (1);
	d->must_eat = -1;
	if (argc == 6)
	{
		--argc;
		if (check_and_insert(&(d->must_eat), argv[argc]))
			return (1);
	}
	while (--argc > 0)
	{
		if (check_and_insert(&(buffer[argc - 1]), argv[argc]))
			return (1);
	}
	d->num_of_philos = buffer[0];
	d->time_to_die = buffer[1];
	d->time_to_eat = buffer[2];
	d->time_to_sleep = buffer[3];
	return (0);
}
