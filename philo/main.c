/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:12:05 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 03:10:20 by yusudemi         ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_data			data;
	t_program		program;
	int				ret;

	ft_bzero(&data, sizeof(t_data));
	ft_bzero(&program, sizeof(t_program));
	if (insert_input(argc, argv, &data))
		return (1);
	program.data = &data;
	program.everyone_ok = 1;
	ret = 0;
	if (create_scene(&program) || establish_actors(&program)
		|| end_scene(&program))
		ret = 1;
	clear_scene(&program);
	return (ret);
}
