/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checkers_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:04:36 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/30 19:08:43 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
void input_error(char *message)
{
	char	*p;

	p = message;
	while (*p)
		p++;
	write(2, "Error: ", 8);
	write(2, message, p - message);
	write(2, "\n", 1);
	exit(1);
}

static int	check_sign(const char **str)
{
	int	ret;

	ret = 0;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			input_error("Parameters can't be negative.");
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
			input_error("Input must be smaller than INT_MAX.");
		str++;
	}
	if (sign && num == 0)
		input_error("Invalid input.");
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
