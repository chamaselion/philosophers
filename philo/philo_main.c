/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:59:05 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/12 14:33:16 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	threads_join(t_philoargs *args, pthread_t *thread)
{
	int	i;

	i = 0;
	while (i < args->no_philosophers)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
}

void	threads_free(t_philoargs *args, pthread_t *thread, t_philosopher *philo)
{
	free_forks(args);
	free_philosophers(args, philo);
	pthread_mutex_destroy(&args->print_mutex);
	pthread_mutex_destroy(&args->terminate_mutex);
	if (thread)
		free(thread);
}

int	initialize_philosopher(t_philosopher *philo, t_philoargs *ar, int i)
{
	if (pthread_mutex_init(&ar->forks[i].mutex, NULL) != 0)
		return (1);
	ar->forks[i].available = 1;
	if (pthread_mutex_init(&philo[i].meal_mutex, NULL) != 0)
		return (1);
	philo[i].id = i + 1;
	philo[i].fork = &ar->forks[i];
	philo[i].args = ar;
	philo[i].last_meal_time = get_time_of_day();
	philo[i].times_eaten = 0;
	return (0);
}

int	threads_init(t_philoargs *ar, pthread_t **thread, t_philosopher **philo)
{
	*thread = malloc(sizeof(pthread_t) * ar->no_philosophers);
	if (!*thread)
		return (1);
	*philo = malloc(sizeof(t_philosopher) * ar->no_philosophers);
	if (!*philo)
		return (free(*thread), 1);
	ar->forks = malloc(sizeof(t_fork) * ar->no_philosophers);
	if (!ar->forks)
		return (free(*thread), free(*philo), 1);
	if (initialize_mutexes(ar) != 0)
		return (free(*thread), free(*philo), free(ar->forks), 1);
	if (initialize_philosophers(ar, *philo) != 0)
		return (free(*thread), free(*philo), free(ar->forks), 1);
	if (create_threads(ar, *thread, *philo) != 0)
		return (free(*thread), free(*philo), free(ar->forks), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_philoargs		args;
	pthread_t		*thread;
	t_philosopher	*philo;
	int				parse_error;
	pthread_t		monitor_thread;

	thread = NULL;
	philo = NULL;
	parse_error = user_input_parse(&args, argc, argv);
	if (parse_error)
	{
		return (1);
	}
	args.firstime = get_time_of_day();
	if (threads_init(&args, &thread, &philo) == 1)
		return (1);
	pthread_create(&monitor_thread, NULL, monitor_routine, philo);
	threads_join(&args, thread);
	pthread_join(monitor_thread, NULL);
	threads_free(&args, thread, philo);
	return (0);
}
