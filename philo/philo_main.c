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
		i = i + 1;
	}
}

void	threads_free(t_philoargs *args, pthread_t *thread, t_philosopher *philo)
{
	int	i;

	i = 0;
	while (i < args->no_philosophers)
	{
		pthread_mutex_destroy(&args->forks[i]);
		pthread_mutex_destroy(&philo[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&args->print_mutex);
	pthread_mutex_destroy(&args->terminate_mutex);
	free(args->forks);
	free(thread);
	free(philo);
}

void	initialize_philosopher(t_philosopher *philo, t_philoargs *ar, int i)
{
	pthread_mutex_init(&ar->forks[i], NULL);
	pthread_mutex_init(&philo[i].meal_mutex, NULL);
	philo[i].id = i + 1;
	philo[i].fork = &ar->forks[i];
	philo[i].args = ar;
	philo[i].last_meal_time = get_time_of_day();
	philo[i].times_eaten = 0;
}

void	threads_init(t_philoargs *ar, pthread_t **thread, t_philosopher **philo)
{
	int	i;

	i = 0;
	*thread = malloc(sizeof(pthread_t) * ar->no_philosophers);
	*philo = malloc(sizeof(t_philosopher) * ar->no_philosophers);
	ar->forks = malloc(sizeof(pthread_mutex_t) * ar->no_philosophers);
	pthread_mutex_init(&ar->print_mutex, NULL);
	pthread_mutex_init(&ar->terminate_mutex, NULL);
	ar->should_terminate = 0;
	while (i < ar->no_philosophers)
	{
		initialize_philosopher(*philo, ar, i);
		i++;
	}
	i = 0;
	while (i < ar->no_philosophers)
	{
		pthread_create(&(*thread)[i], NULL, philo_routine, &(*philo)[i]);
		i++;
	}
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
	threads_init(&args, &thread, &philo);
	pthread_create(&monitor_thread, NULL, monitor_routine, philo);
	threads_join(&args, thread);
	pthread_join(monitor_thread, NULL);
	threads_free(&args, thread, philo);
	return (0);
}
