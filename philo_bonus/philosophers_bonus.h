/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:40:21 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/20 04:32:45 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <semaphore.h>
# include <sys/time.h>

typedef struct s_program		t_program;
typedef struct s_data			t_data;
typedef struct s_philosopher	t_philosopher;
typedef struct s_semaphores		t_semaphores;
typedef struct s_collector		t_collector;

struct	s_semaphores // counters with atomic ops
{
	sem_t	*die; // if someone dies
	sem_t	*fork; // obvious right
	sem_t	*ready; // for wait all philos to start same time
	sem_t	*done; // *parent process will check this* if all philos done with success it will 
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
struct	s_philosopher
{
	int				id;
	int				eaten_meal;
	suseconds_t		last_meal;
	t_data			*data;
	t_semaphores	*semaphores;
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
	t_data			*data;
	pid_t			*child_ids;
	int				philos_ready;
	t_semaphores	*semaphores;
};


//input
int	input_error(char *message);
int	insert_input(int argc, char **argv, t_data *d);
int	check_sign(const char **str);
int	ft_atoi(const char *str);
int	is_num(char *str);
int	check_argc(int argc);

#endif