/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 05:40:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/02 23:04:58 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>
#include <string.h>

static int	should_continue_logging(t_philosopher *philo)
{
	int	continue_log;

	lock_mutex(&philo->program->finish_lock);
	continue_log = philo->program->everyone_ok;
	unlock_mutex(&philo->program->finish_lock);
	return (continue_log);
}

int	log_status(t_philosopher *philo, char *msg)
{
	suseconds_t	timestamp;

	lock_mutex(&philo->program->write_lock);
	if (!should_continue_logging(philo))
		return (unlock_mutex(&philo->program->write_lock), 0);
	timestamp = get_elapsed_time(philo->program);
	printf("%ld %d %s\n", timestamp, philo->id, msg);
	unlock_mutex(&philo->program->write_lock);
	return (0);
}

int	announce_death(t_philosopher *philo)
{
	suseconds_t	timestamp;

	lock_mutex(&philo->program->finish_lock);
	philo->program->everyone_ok = 0;
	unlock_mutex(&philo->program->finish_lock);
	lock_mutex(&philo->program->write_lock);
	timestamp = get_elapsed_time(philo->program);
	if (timestamp == -1)
		return (1);
	printf("%ld %d %s\n", timestamp, philo->id, "died");
	unlock_mutex(&philo->program->write_lock);
	return (0);
}
