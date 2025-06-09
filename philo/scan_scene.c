/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:36:45 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/09 22:08:09 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

int	wait_other_actors(t_program *p)
{
	int	ready;

	ready = 0;
	while (!ready)
	{
		if (thread_lock(&p->lock))
			return (1);
		if (p->running_thread_count == (p->data->num_of_philos + 1))
			ready = 1;
		if (thread_unlock(&p->lock))
			return (1);
		usleep(100);
	}
	return (0);
}

int	say_ready(t_program *p)
{
	if (thread_lock(&p->lock))
		return (1);
	p->running_thread_count += 1;
	if (thread_unlock(&p->lock))
		return (1);
	return (0);
}

int	is_everyone_alive(t_program *p)
{
	int	ret;

	ret = 1;
	if (thread_lock(&p->lock))
		return (-1);
	if (!p->everyone_ok)
		ret = 0;
	if (thread_unlock(&p->lock))
		return (-1);
	return (ret);
}

int	is_program_running(t_program *p)
{
	int	ret;

	ret = 1;
	if (thread_lock(&p->lock))
		return (-1);
	if (!p->everyone_ok)
		ret = 0;
	if (p->philos_done_eating == p->data->num_of_philos)
		ret = 0;
	if (thread_unlock(&p->lock))
		return (-1);
	return (ret);
}
