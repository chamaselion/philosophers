/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread_init.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 18:06:56 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/09 18:06:56 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_mutexes(t_philoargs *ar)
{
	if (pthread_mutex_init(&ar->print_mutex, NULL) != 0
		|| pthread_mutex_init(&ar->terminate_mutex, NULL) != 0)
		return (1);
	ar->should_terminate = 0;
	return (0);
}

int	initialize_philosophers(t_philoargs *ar, t_philosopher *philo)
{
	int	i;

	i = 0;
	while (i < ar->no_philosophers)
	{
		if (initialize_philosopher(philo, ar, i) == 1)
			return (1);
		i++;
	}
	return (0);
}

int	create_threads(t_philoargs *ar, pthread_t *thread, t_philosopher *philo)
{
	int	i;

	i = 0;
	while (i < ar->no_philosophers)
	{
		if (pthread_create(&thread[i], NULL, philo_routine, &philo[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	philo_lifecycle(t_philosopher *philo, t_fork *left_fork, t_fork *right_fork, t_philoargs *philoarg)
{
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
}
