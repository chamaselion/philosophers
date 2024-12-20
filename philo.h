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
# include "philo.h"
# include <sys/time.h>

typedef struct s_philoargs
{
	int				no_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				no_t_philosopher_must_eat;
	int				extra;
	long			firstime;
	pthread_mutex_t	terminate_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
	int				should_terminate;
}	t_philoargs;

typedef struct s_philosopher
{
	int				id;
	pthread_mutex_t	*fork;
	t_philoargs		*args;
	long			last_meal_time;
	int				times_eaten;
	int				is_eating;
	pthread_mutex_t	meal_mutex;
}	t_philosopher;

// philo utilities
int		str_isdigit(char *i);
void	threads_free(t_philoargs *args, pthread_t *thread,
			t_philosopher *philo);
int		ft_atoi(const char *t);
long	get_time_of_day(void);
void	initialize_forks(t_philosopher *philo, pthread_mutex_t **left_fork,
			pthread_mutex_t **right_fork);

//philo routines
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);

//philo routine utis
void	precise_sleep(long duration, t_philoargs *args);
void	print_state(t_philosopher *philo, const char *state, t_philoargs *args);
void	extra_checker(int *all_right, t_philosopher *philos, t_philoargs *args);
int		check_meals_completion(t_philosopher *philos, t_philoargs *args);

// philo input handle
int		user_input_parse(t_philoargs *args, int argc, char **argv);

#endif
