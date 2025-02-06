/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:36:56 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/06 16:26:07 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *t)
{
	int	r;
	int	c;
	int	s;

	c = 0;
	r = 0;
	s = 1;
	while (t[c] == ' ' || t[c] == '\t' || t[c] == '\r' || t[c] == '\n'
		|| t[c] == '\v' || t[c] == '\f')
	{
		c++;
	}
	if (t[c] == '-' || t[c] == '+')
	{
		if (t[c] == '-')
			s = -1;
		c++;
	}
	while (t[c] >= '0' && t[c] <= '9')
	{
		r = r * 10 + (t[c] - '0');
		c++;
	}
	return (r * s);
}

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
