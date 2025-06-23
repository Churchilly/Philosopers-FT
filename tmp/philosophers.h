/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 04:53:23 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/23 19:28:42 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <sys/time.h>

typedef struct s_program		t_program;
typedef struct s_data			t_data;
typedef struct s_philosopher	t_philosopher;
typedef void					*(*t_routine_func)(void *d);

struct	s_philosopher
{
	int				id;
	int				eaten_meal;
	int				dead;
	suseconds_t		last_meal;
	pthread_t		thread;
	pthread_mutex_t	lock;
	pthread_mutex_t	*fork_one;
	pthread_mutex_t	*fork_two;
	t_data			*data;
	t_program		*program;
};

struct s_data
{
	int	num_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat;
	int	hcf;
};

struct s_program
{
	pthread_mutex_t	lock;
	pthread_mutex_t	write_lock;
	t_data			*data;
	pthread_t		monitor;
	int				everyone_ok;
	int				running_thread_count;
	int				philos_done_eating;
	suseconds_t		start_time;
	pthread_mutex_t	*forks;
	t_philosopher	*philosophers;
};

int			insert_input(int argc, char **argv, t_data *data);
int			input_error(char *message);
int			check_sign(const char **str);
int			ft_atoi(const char *str);
int			is_num(char *str);
int			check_argc(int argc);

int			create_scene(t_program *p);
int			end_scene(t_program *p);
int			clear_scene(t_program *p);
int			establish_actors(t_program *p);
int			say_ready(t_program *p);
int			wait_other_actors(t_program *p);

int			thread_lock(pthread_mutex_t *mutex);
int			thread_unlock(pthread_mutex_t *mutex);

void		*monitoring(void *arg);

int			am_i_alive(t_philosopher *philo);
void		*routine(void *arg);
void		*one_fork_routine(void	*arg);

int			log_status(t_philosopher *philo, char *msg);
int			philo_eat(t_philosopher *philo);
int			philo_think(t_philosopher *philo);
int			philo_sleep(t_philosopher *philo);

suseconds_t	get_current_time(void);
suseconds_t	philo_perform(suseconds_t time);
suseconds_t	get_elapsed_time(t_program *p);
int			start_timer(t_program *p);

#endif