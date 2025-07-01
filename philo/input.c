/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 02:00:41 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 03:02:17 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

int	input_error(char *message)
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

static int	check_and_insert(int *p, char *d)
{
	if (!is_num(d))
		return (input_error("All must be number."), 1);
	*p = ft_atoi(d);
	if ((*p) < 0)
		return (1);
	return (0);
}

void	insert_hcf(t_data *d)
{
	int	tmp;
	int	a;
	int	b;

	a = d->time_to_eat;
	b = d->time_to_sleep;
	while (b != 0)
	{
		tmp = b;
		b = a % b;
		a = tmp;
	}
	d->hcf = a;
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
	insert_hcf(d);
	return (0);
}
