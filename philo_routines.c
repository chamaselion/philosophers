/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:04:50 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/11 16:12:34 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philosopher_death(t_philosopher *philo, t_philoargs *args)
{
	long	time_since_last_meal;

	if (philo->is_eating == 0)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		time_since_last_meal = get_time_of_day() - philo->last_meal_time;
		if (time_since_last_meal > args->time_to_die)
		{
			pthread_mutex_lock(&args->terminate_mutex);
			args->should_terminate = 1;
			pthread_mutex_unlock(&args->terminate_mutex);
			pthread_mutex_lock(&args->print_mutex);
			printf("%ld %d died\n", (get_time_of_day() - args->firstime),
				philo->id);
			pthread_mutex_unlock(&args->print_mutex);
			pthread_mutex_unlock(&philo->meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&philo->meal_mutex);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philosopher	*philos;
	t_philoargs		*args;
	int				i;

	philos = (t_philosopher *)arg;
	args = philos[0].args;
	while (1)
	{
		if (check_meals_completion(philos, args))
			return (NULL);
		i = 0;
		while (i < args->no_philosophers)
		{
			if (check_philosopher_death(&philos[i], args))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	eating_routine(t_philosopher *philo, pthread_mutex_t *left_fork,
		pthread_mutex_t *right_fork, t_philoargs *philoarg)
{
	pthread_mutex_lock(left_fork);
	print_state(philo, "has taken a fork", philoarg);
	if (philoarg->no_philosophers == 1)
	{
		precise_sleep(philoarg->time_to_die + 1, philoarg);
		pthread_mutex_unlock(left_fork);
		pthread_mutex_lock(&philoarg->terminate_mutex);
		philoarg->should_terminate = 1;
		pthread_mutex_unlock(&philoarg->terminate_mutex);
		return ;
	}
	pthread_mutex_lock(right_fork);
	print_state(philo, "has taken a fork", philoarg);
	philo->is_eating = 1;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time_of_day();
	philo->times_eaten++;
	print_state(philo, "is eating", philoarg);
	precise_sleep(philoarg->time_to_eat, philoarg);
	pthread_mutex_unlock(right_fork);
	pthread_mutex_unlock(left_fork);
	pthread_mutex_unlock(&philo->meal_mutex);
	philo->is_eating = 0;
}

void	sleeping_routine(t_philosopher *philo, t_philoargs *philoarg)
{
	print_state(philo, "is sleeping", philoarg);
	precise_sleep(philoarg->time_to_sleep, philoarg);
}

void	*philo_routine(void *arg)
{
	t_philosopher	*philo;
	t_philoargs		*philoarg;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

	philo = (t_philosopher *)arg;
	philoarg = philo->args;
	initialize_forks(philo, &left_fork, &right_fork);
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&philoarg->terminate_mutex);
		if (philoarg->should_terminate)
		{
			pthread_mutex_unlock(&philoarg->terminate_mutex);
			break ;
		}
		pthread_mutex_unlock(&philoarg->terminate_mutex);
		print_state(philo, "is thinking", philoarg);
		eating_routine(philo, left_fork, right_fork, philoarg);
		sleeping_routine(philo, philoarg);
	}
	return (NULL);
}
