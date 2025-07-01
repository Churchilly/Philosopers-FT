/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actors_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:16:07 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 04:47:08 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdlib.h>
#include <unistd.h>

void	establish_actors(t_program *p)
{
	int		i;
	pid_t	pid;

	start_timer(p);
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		pid = fork();
		if (pid < 0)
			exit(1);
		if (pid == 0)
		{
			if (p->data->num_of_philos == 1)
				one_fork_routine(&p->philosophers[i]);
			else
				routine(&p->philosophers[i]);
			exit(0);
		}
		else
			p->child_ids[i] = pid;
	}
}
