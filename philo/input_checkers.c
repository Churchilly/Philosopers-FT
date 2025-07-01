/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checkers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:04:36 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 03:06:00 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <limits.h>

int	check_sign(const char **str)
{
	int	ret;

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

int	ft_atoi(const char *str)
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

int	is_num(char *str)
{
	if (*str == 0 || !str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (*str > '9' || *str < '0')
			return (0);
		str++;
	}
	return (1);
}

int	check_argc(int argc)
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
