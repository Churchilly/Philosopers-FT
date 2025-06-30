/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_func_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:14:34 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/30 19:18:34 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	eat(t_philosopher *philo)
{
	// Take two forks
	sem_wait(philo->semaphores->forks);
	safe_print(philo, "has taken a fork");
	
	sem_wait(philo->semaphores->forks);
	safe_print(philo, "has taken a fork");
	
	// Update last meal time
	set_last_meal(philo);
	
	safe_print(philo, "is eating");
	philo_perform(philo->data->time_to_eat);
	
	// Update eaten meal count (not protected since it's not shared)
	philo->eaten_meal++;
	
	// Release forks
	sem_post(philo->semaphores->forks);
	sem_post(philo->semaphores->forks);
}

void	philo_sleep(t_philosopher *philo)
{
	safe_print(philo, "is sleeping");
	philo_perform(philo->data->time_to_sleep);
}

void	think(t_philosopher *philo)
{
	safe_print(philo, "is thinking");
}