/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:12:05 by yusudemi          #+#    #+#             */
/*   Updated: 2025/05/19 02:50:05 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int	create_scene(t_program *p);
int	establish_actors(t_program *p);
int	play_scene(t_program *p);

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

int main(int argc, char **argv)
{
	t_data			data;
	t_program		program;
	
	ft_bzero(&data, sizeof(t_data));
	if (insert_input(argc, argv, &data))
		return (1);
	//input_test(&data);
	program.data = &data;
	program.everyone_ok = 1;
	if (create_scene(&program) || establish_actors(&program)
		|| play_scene(&program))
		return (1); // ofc need to free	
	free(program.forks);
	free(program.philosophers);
	return (0);
}
