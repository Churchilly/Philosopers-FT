/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_actors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 02:32:03 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/03 03:36:05 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *arg);

int	establish_actors(t_program *p)
{
	int		i;
	void	*routine_func;

	if (p->data->num_of_philos == 1)
		routine_func = one_fork_routine;
	else
		routine_func = routine;
	p->wait_actors = 1;
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		if (pthread_create(&p->philosophers[i].thread, NULL,
				routine_func, &p->philosophers[i]) != 0)
			return (1);
	}
	lock_mutex(&(p->program_lock));
	p->wait_actors = 0;
	start_timer(p);
	unlock_mutex(&(p->program_lock));
	if (pthread_create(&p->monitor, NULL, monitoring, p) != 0)
		return (1);
	return (0);
}

int	end_scene(t_program *p)
{
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	while (i < p->data->num_of_philos)
	{
		if (pthread_join(p->philosophers[i].thread, NULL) != 0)
			ret = 1;
		i++;
	}
	if (pthread_join(p->monitor, NULL) != 0)
		ret = 1;
	return (ret);
}
