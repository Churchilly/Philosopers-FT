/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:12:05 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/09 23:36:43 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>
#include <stdlib.h>

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

void	input_test(t_data *data)
{
	printf("%d\n", data->num_of_philos);
	printf("%d\n", data->time_to_die);
	printf("%d\n", data->time_to_eat);
	printf("%d\n", data->time_to_sleep);
	printf("%d\n", data->must_eat);
}

int	clear_scene(t_program *p)
{
	int	i;
	int	status;

	i = -1;
	while (++i < p->data->num_of_philos)
	{
		status = pthread_mutex_destroy(&p->forks[i]);
		if (status != 0)
			return (printf("%d:pthread_mutex_destroy() failed: %d.\n", i,status), 1);
	}
	status = pthread_mutex_destroy(&p->lock);
	if (status != 0)
		return (printf("b:pthread_mutex_destroy() failed: %d.\n", status), 1);
	status = pthread_mutex_destroy(&p->philosophers->lock);
	if (status != 0)
		return (printf("pthread_mutex_destroy() failed: %d.\n", status), 1);
	free(p->forks);
	free(p->philosophers);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data			data;
	t_program		program;
	int				ret;

	ft_bzero(&data, sizeof(t_data));
	if (insert_input(argc, argv, &data))
		return (1);
	program.data = &data;
	program.everyone_ok = 1;
	ret = 0;
	if (create_scene(&program) || establish_actors(&program)
		|| monitoring(&program) || end_scene(&program))
		ret = 1;
	if (clear_scene(&program))
		ret = 1;
	return (ret);
}
