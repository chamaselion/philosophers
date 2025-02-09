/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:36:56 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/09 18:27:50 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	str_isdigit(char *i)
{
	int	c;
	int	rv;

	rv = 1;
	c = 0;
	while (i[c] != '\0')
	{
		if (i[c] < '0' || i[c] > '9')
		{
			rv = 0;
			break ;
		}
		c++;
	}
	return (rv);
}

long	get_time_of_day(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	check_philosopher_death(t_philosopher *philo, t_philoargs *args)
{
	long	time_since_last_meal;

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
	return (0);
}

void	free_forks(t_philoargs *args)
{
	int	i;

	i = 0;
	if (args->forks)
	{
		while (i < args->no_philosophers)
		{
			pthread_mutex_destroy(&args->forks[i].mutex);
			i++;
		}
		free(args->forks);
	}
}

void	free_philosophers(t_philoargs *args, t_philosopher *philo)
{
	int	i;

	if (philo)
	{
		i = 0;
		while (i < args->no_philosophers)
		{
			pthread_mutex_destroy(&philo[i].meal_mutex);
			i++;
		}
		free(philo);
	}
}
