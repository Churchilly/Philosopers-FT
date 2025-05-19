/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 04:53:23 by yusudemi          #+#    #+#             */
/*   Updated: 2025/05/19 07:35:53 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

typedef struct s_program t_program;
typedef struct s_data t_data;
typedef struct s_philosopher t_philosopher;
typedef enum e_states t_states;


struct	s_philosopher
{
	pthread_mutex_t	lock;
	int 			id;
	pthread_t 		thread;
	pthread_mutex_t *fork_one;
	pthread_mutex_t *fork_two;
	int				eaten_meal;
	suseconds_t		last_meal;
	t_program		*program;
	t_data			*data;
};

struct s_data
{
	int	num_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat;
};

struct s_program
{
	pthread_mutex_t	lock;
	t_data			*data;
	pthread_t		monitor;
	int				everyone_ok;
	int				running_thread_count;
	int				philos_done_eating;
	suseconds_t		start_time;
	pthread_mutex_t	*forks;
	t_philosopher	*philosophers;
};

int	insert_input(int argc, char **argv, t_data *data);

#endif