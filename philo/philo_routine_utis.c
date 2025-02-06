/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:03:00 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/11 16:22:25 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	precise_sleep(long duration, t_philoargs *args)
{
	long	start_time;

	start_time = get_time_of_day();
	while ((get_time_of_day() - start_time) < duration)
	{
		pthread_mutex_lock(&args->terminate_mutex);
		if (args->should_terminate)
		{
			pthread_mutex_unlock(&args->terminate_mutex);
			break ;
		}
		pthread_mutex_unlock(&args->terminate_mutex);
		usleep(50);
	}
}

void	print_state(t_philosopher *philo, const char *state, t_philoargs *args)
{
	long	time;

	pthread_mutex_lock(&args->terminate_mutex);
	pthread_mutex_lock(&args->print_mutex);
	if (args->should_terminate)
	{
		pthread_mutex_unlock(&args->print_mutex);
		pthread_mutex_unlock(&args->terminate_mutex);
		return ;
	}
	time = get_time_of_day() - args->firstime;
	printf("%ld %d %s\n", time, philo->id, state);
	pthread_mutex_unlock(&args->print_mutex);
	pthread_mutex_unlock(&args->terminate_mutex);
}

void	extra_checker(int *all_right, t_philosopher *philos, t_philoargs *args)
{
	int	i;

	i = 0;
	*all_right = 1;
	while (i < args->no_philosophers)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (philos[i].times_eaten < args->no_t_philosopher_must_eat)
		{
			*all_right = 0;
			pthread_mutex_unlock(&philos[i].meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
}

void	initialize_forks(t_philosopher *philo, pthread_mutex_t **left_fork,
		pthread_mutex_t **right_fork)
{
	if (philo->id % 2 == 0)
	{
		*left_fork = philo->fork;
		*right_fork = &philo->args->forks[philo->id
			% philo->args->no_philosophers];
	}
	else
	{
		*right_fork = philo->fork;
		*left_fork = &philo->args->forks[philo->id
			% philo->args->no_philosophers];
	}
}

int	check_meals_completion(t_philosopher *philos, t_philoargs *args)
{
	int	all_right;

	if (args->extra > 0)
	{
		extra_checker(&all_right, philos, args);
		if (all_right)
		{
			pthread_mutex_lock(&args->terminate_mutex);
			args->should_terminate = 1;
			pthread_mutex_unlock(&args->terminate_mutex);
			pthread_mutex_lock(&args->print_mutex);
			printf("All philosophers have eaten enough times\n");
			pthread_mutex_unlock(&args->print_mutex);
			return (1);
		}
	}
	return (0);
}
