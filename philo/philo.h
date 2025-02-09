/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:59:16 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/09 16:55:43 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <limits.h>
# include <sys/time.h>
# include <ctype.h>
# include <string.h>

# define MAX_DIGITS 19

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				available;
}	t_fork;

typedef struct s_philoargs
{
	int							no_philosophers;
	long						time_to_die;
	long						time_to_eat;
	long						time_to_sleep;
	long						no_t_philosopher_must_eat;
	int							extra;
	long						firstime;
	pthread_mutex_t				terminate_mutex;
	pthread_mutex_t				print_mutex;
	t_fork						*forks;
	int							should_terminate;
}	t_philoargs;

typedef struct s_philosopher
{
	int				id;
	t_fork			*fork;
	t_philoargs		*args;
	long			last_meal_time;
	long			times_eaten;
	pthread_mutex_t	meal_mutex;
}	t_philosopher;

// philo utilities
int		str_isdigit(char *i);
void	threads_free(t_philoargs *args, pthread_t *thread,
			t_philosopher *philo);
long	get_time_of_day(void);
void	initialize_forks(t_philosopher *philo, t_fork **left_fork,
			t_fork **right_fork);

//philo routines
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);

//philo routine utis
void	precise_sleep(long duration, t_philoargs *args);
void	print_state(t_philosopher *philo, const char *state, t_philoargs *args);
void	extra_checker(int *all_right, t_philosopher *philos, t_philoargs *args);
int		check_meals_completion(t_philosopher *philos, t_philoargs *args);
int		check_philosopher_death(t_philosopher *philo, t_philoargs *args);
void	free_philosophers(t_philoargs *args, t_philosopher *philo);
void	free_forks(t_philoargs *args);
int		create_threads(t_philoargs *ar, pthread_t *thread, t_philosopher *philo);
int		initialize_philosophers(t_philoargs *ar, t_philosopher *philo);
int		initialize_mutexes(t_philoargs *ar);
int		initialize_philosopher(t_philosopher *philo, t_philoargs *ar, int i);
void	sleeping_routine(t_philosopher *philo, t_philoargs *philoarg);
void	eating_routine(t_philosopher *philo, t_fork *left_fork,
			t_fork *right_fork, t_philoargs *philoarg);
void	philo_lifecycle(t_philosopher *philo, t_fork *left_fork, t_fork *right_fork, t_philoargs *philoarg);

// philo input handle
int		user_input_parse(t_philoargs *args, int argc, char **argv);
int		get_args(t_philoargs *args, char **argv);
long	atol_with_error(const char *s, int *error);

#endif
