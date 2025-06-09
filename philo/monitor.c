/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 07:55:15 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/09 23:51:16 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>
#include <unistd.h>
int	wait_everyone_done(t_program *p)
{
	int	ready;

	ready = 0;
	while (!ready)
	{
		if (thread_lock(&p->lock))
			return (1);
		if (p->philos_done_eating == (p->data->num_of_philos))
			ready = 1;
		if (thread_unlock(&p->lock))
			return (1);
		usleep(100);
	}
	return (0);
}
int	kill_everyone(t_program *p)
{
	int status;
	int	i;

	i = -1;
	while (++i < p->data->num_of_philos)
	{
		if (thread_lock(&p->philosophers[i].lock))
			return (1);
		p->philosophers[i].dead = 1;
		if (thread_unlock(&p->philosophers[i].lock))
			return (1);
		status = pthread_detach(p->philosophers[i].thread);
		if (status != 0)
			return (printf("pthread_detach() failed: %d.\n", status), 1);
	}
	if (wait_everyone_done(p))
		return (1);
	while (++i < p->data->num_of_philos)
	{
		status = pthread_detach(p->philosophers[i].thread);
		if (status != 0)
			return (printf("pthread_detach() failed: %d.\n", status), 1);
	}
	return (0);
}

int	end_scene(t_program *p)
{
	int		i;
	int		status;
	void	*retval;

	status = is_everyone_alive(p);
	if (status == -1)
		return (1);
	if (status == 0)
		return kill_everyone(p);
	i = -1;
	while (++i < p->data->num_of_philos)
	{
		status = pthread_join(p->philosophers[i].thread, &retval);
		if (status != 0)
			return (printf("pthread_join() failed: %d.\n", status), 1);
		if ((long)retval == (long)1)
			return (1);
	}
	if ((long)retval == (long)1)
		return (1);
	return (0);
}

int	check_philo_status(t_philosopher *philo)
{
	suseconds_t	time;

	if (thread_lock(&philo->lock))
		return (-1);
	time = get_elapsed_time(philo->program);
	if (time < 0)
		return (-1);
	if (time - philo->last_meal > philo->data->time_to_die
		&& (philo->data->must_eat == -1 || philo->eaten_meal < philo->data->must_eat))
	{
		if (thread_lock(&philo->program->lock))
			return (-1);
		if (philo->program->everyone_ok)
			printf("%ld %d died.\n", time, philo->id);
		philo->program->everyone_ok = 0;
		if (thread_unlock(&philo->program->lock))
			return (-1);
	}
	if (thread_unlock(&philo->lock))
		return (-1);
	return (0);
}

int	monitoring(void *arg)
{
	t_program	*p;
	int			i;
	int			retval;

	p = (t_program *)arg;
	if (start_timer(p) || say_ready(p) || wait_other_actors(p))
		return (1);
	retval = is_program_running(p);
	if (retval == -1)
		return (1);
	while (retval)
	{
		i = -1;
		while (++i < p->data->num_of_philos)
		{
			retval = check_philo_status(&(p->philosophers[i]));
			if (retval == -1)
				return (1);
		}
		retval = is_program_running(p);
		if (retval == -1)
			return (1);
	}
	return (0);
}
