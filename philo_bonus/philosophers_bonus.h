/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:40:21 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/30 19:19:16 by yusudemi         ###   ########.fr       */
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


struct	s_semaphores // counters with atomic ops. global semaphores
{
	sem_t	*forks; // obvious right
	sem_t	*die_lock; // if someone dies this sem unlocks and monitor of philos kills its philo.
	sem_t	*write_lock; // when someone want to print need to lock write sem
	sem_t	*meal_complete; // to track when philosophers complete their meals
};
// after fuc with semaphores i think i need to check process exit codes for 
// cant use done sem cause i cant use trywait or getvalue funcs.
// so new design required for s_semaphores
//if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
//{
//    successful_exits++;
//    printf("Process %d/%d completed successfully\n", 
//           successful_exits, total_processes);
//}
// philos who finish their meal must exit and i need to catch thir exit status
// maybe ready semaphore is just a overkill. try without it.
// fork is OK
// die must start from 1. it will stop all philos and post when 0
// parent process must wait wait for a philo to die to kill everyone.


// die monitor and eat monitor have possible datarace
// SO since philos have monitor threads inside them they also need locks
struct	s_philosopher // one philo
{
	int				id;
	int				eaten_meal; // not a shared variable
	sem_t			last_meal_lock;
	suseconds_t		last_meal; // shared var between philo and its monitor locked with last_meal_lock
	pthread_t		death_monitor; // if philo dies post die sem to unlock all monitors to kill its philo use detach on this
	t_data			*data;
	t_semaphores	*semaphores;
};

struct s_data // global data
{
	int			num_of_philos;
	suseconds_t	time_to_die;
	suseconds_t	time_to_eat;
	suseconds_t	time_to_sleep;
	int			must_eat;
	suseconds_t	start_time;
};

struct s_program // global program access everything
{
	t_data			*data;
	pid_t			*child_ids;
	t_philosopher	*philosophers;
	t_semaphores	*semaphores;
};


//input
void	input_error(char *message);
void	insert_input(int argc, char **argv, t_data *d);
int		ft_atoi(const char *str);
int		is_num(char *str);

//time
suseconds_t	get_current_time(void);
int			start_timer(t_program *p);
suseconds_t	philo_perform(suseconds_t time);

//utils
void		ft_bzero(void *addr, int size);
void		safe_print(t_philosopher *philo, char *message);
void		cleanup_and_exit(t_program *p);

//protected variables
void		set_last_meal(t_philosopher *philo);
long		get_last_meal(t_philosopher *philo);

// scene
void	create_scene(t_program *p, t_data *d);
void	end_scene(t_program *p);
void	clear_scene(t_program *p);
void	establish_actors(t_program *p);

// routine
void	routine(t_philosopher *philo);
void	*death_monitor(void *arg);
void	eat(t_philosopher *philo);
void	philo_sleep(t_philosopher *philo);
void	think(t_philosopher *philo);

#endif