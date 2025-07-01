/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:41:19 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/02 00:05:34 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

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

int	main(int argc, char **argv)
{
	t_data		data;
	t_program	program;

	ft_bzero(&data, sizeof(t_data));
	ft_bzero(&data, sizeof(t_program));
	insert_input(argc, argv, &data);
	create_scene(&program, &data);
	establish_actors(&program);
	end_scene(&program);
	clear_scene(&program, 0);
	return (0);
}
