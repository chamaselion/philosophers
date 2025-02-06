/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:04:50 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/06 16:25:57 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		usleep(50);
	}
	return (NULL);
}

void	perform_eating(t_philosopher *philo, pthread_mutex_t *first,
		pthread_mutex_t *second, t_philoargs *philoarg)
{
	pthread_mutex_lock(first);
	print_state(philo, "has taken a fork", philoarg);
	pthread_mutex_lock(second);
	print_state(philo, "has taken a fork", philoarg);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time_of_day();
	philo->times_eaten++;
	print_state(philo, "is eating", philoarg);
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_sleep(philoarg->time_to_eat, philoarg);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

void	eating_routine(t_philosopher *philo, pthread_mutex_t *left_fork,
		pthread_mutex_t *right_fork, t_philoargs *philoarg)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (left_fork < right_fork)
	{
		first = left_fork;
		second = right_fork;
	}
	else
	{
		first = right_fork;
		second = left_fork;
	}
	if (philoarg->no_philosophers == 1)
	{
		pthread_mutex_lock(first);
		print_state(philo, "has taken a fork", philoarg);
		precise_sleep(philoarg->time_to_die, philoarg);
		pthread_mutex_unlock(first);
		pthread_mutex_lock(&philoarg->terminate_mutex);
		philoarg->should_terminate = 1;
		pthread_mutex_unlock(&philoarg->terminate_mutex);
		return ;
	}
	perform_eating(philo, first, second, philoarg);
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
		usleep(8000);
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
