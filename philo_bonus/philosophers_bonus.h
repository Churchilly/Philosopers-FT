/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:40:21 by yusudemi          #+#    #+#             */
/*   Updated: 2025/07/02 22:20:50 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <semaphore.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_program		t_program;
typedef struct s_data			t_data;
typedef struct s_philosopher	t_philosopher;
typedef struct s_semaphores		t_semaphores;
typedef struct s_collector		t_collector;

struct	s_semaphores
{
	sem_t	*forks;
	sem_t	*write_lock;
	sem_t	*finish_lock;
	sem_t	*term_lock;
};

struct	s_philosopher
{
	int				id;
	int				eaten_meal;
	int				alive;
	suseconds_t		last_meal;
	t_data			*data;
	t_semaphores	*semaphores;
	t_program		*program;
};

struct s_data
{
	int			num_of_philos;
	suseconds_t	time_to_die;
	suseconds_t	time_to_eat;
	suseconds_t	time_to_sleep;
	int			must_eat;
	suseconds_t	start_time;
};

struct s_program
{
	int				philos_done_eating;
	t_data			*data;
	pid_t			*child_ids;
	t_philosopher	*philosophers;
	t_semaphores	*semaphores;
};

void		input_error(char *message);
void		insert_input(int argc, char **argv, t_data *d);
int			ft_atoi(const char *str);
int			is_num(char *str);

suseconds_t	get_current_time(void);
int			start_timer(t_program *p);
suseconds_t	philo_perform(suseconds_t time);
suseconds_t	get_elapsed_time(t_data *d);

void		ft_bzero(void *addr, int size);
void		log_status(t_philosopher *philo, char *message);
void		cleanup_and_exit(t_program *p);

void		create_scene(t_program *p, t_data *d);
void		end_scene(t_program *p);
void		establish_actors(t_program *p);
void		clear_scene(t_program *p, int exit_status);
void		clear_fork(t_program *p, int exit_status);

void		routine(t_philosopher *philo);
void		one_fork_routine(t_philosopher *philo);
void		*death_monitor(void *arg);
void		*kill_monitor(void	*arg);
long		get_last_meal(t_philosopher *philo);
int			eat(t_philosopher *philo);
int			philo_sleep(t_philosopher *philo);
int			think(t_philosopher *philo);

#endif