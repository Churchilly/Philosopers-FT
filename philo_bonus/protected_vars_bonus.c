/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protected_vars_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 19:00:00 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/01 04:47:08 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void	set_last_meal(t_philosopher *philo)
{
	if (sem_wait(&philo->last_meal_lock))
	{
		printf("sem_wait failed:%d\n", errno);
		exit(1);
	}
	philo->last_meal = get_elapsed_time(philo->data);
	if (sem_post(&philo->last_meal_lock))
	{
		printf("sem_post failed:%d\n", errno);
		exit(1);
	}
}

long	get_last_meal(t_philosopher *philo)
{
	long	value;

	if (sem_wait(&philo->last_meal_lock))
	{
		printf("sem_wait failed:%d\n", errno);
		exit(1);
	}
	value = (long)philo->last_meal;
	if (sem_post(&philo->last_meal_lock))
	{
		printf("sem_post failed:%d\n", errno);
		exit(1);
	}
	return (value);
}
