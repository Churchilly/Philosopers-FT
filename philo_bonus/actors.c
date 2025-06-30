/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:16:07 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/30 19:17:36 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <stdlib.h>
#include <unistd.h>
void	establish_actors(t_program *p)
{
	int		i;
	pid_t	pid;

	i = -1;
	start_timer(p);
	while (++i < p->data->num_of_philos)
	{
		pid = fork();
		if (pid < 0)
			exit(1);
		if (pid == 0) // Child process
		{
			// Use the pre-initialized philosopher
			routine(&p->philosophers[i]);
			exit(0);
		}
		else // Parent process
		{
			p->child_ids[i] = pid;
		}
	}
}