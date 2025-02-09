/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 04:53:23 by yusudemi          #+#    #+#             */
/*   Updated: 2025/02/09 07:30:38 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>

typedef struct s_philosophers
{
	int 			id;
	pthread_t 		thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
} t_philosophers;

typedef struct s_data
{
	unsigned int	number_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*forks;
	t_philosophers	*philosophers;
}		t_data;

void	insert_input(int argc, char **argv, t_data *data);

#endif