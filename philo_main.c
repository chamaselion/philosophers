/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:59:05 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/09 16:17:37 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"

void threads_join(t_philoargs *args, pthread_t *thread)
{
    int i;

    i = 0;
    while (i < args->no_philosophers)
    {
        pthread_join(thread[i], NULL);
        i = i + 1;
    }
}

void threads_free(t_philoargs *args, pthread_t *thread, t_philosopher *philo)
{
    int i;

    i = 0;
    while (i < args->no_philosophers)
    {
        pthread_mutex_destroy(&args->forks[i]);
        pthread_mutex_destroy(&philo[i].meal_mutex);
        i++;
    }
    pthread_mutex_destroy(&args->print_mutex);
    free(args->forks);
    free(thread);
    free(philo);
}

void threads_init(t_philoargs *args, pthread_t **thread, t_philosopher **philo)
{
    int i;

    i = 0;
    *thread = malloc(sizeof(pthread_t) * args->no_philosophers);
    *philo = malloc(sizeof(t_philosopher) * args->no_philosophers);
    args->forks = malloc(sizeof(pthread_mutex_t) * args->no_philosophers);
    pthread_mutex_init(&args->print_mutex, NULL);
    while (i < args->no_philosophers)
    {
        pthread_mutex_init(&args->forks[i], NULL);
        pthread_mutex_init(&(*philo)[i].meal_mutex, NULL);
        (*philo)[i].id = i + 1;
        (*philo)[i].fork = &args->forks[i];
        (*philo)[i].args = args;
        (*philo)[i].last_meal_time = get_time_of_day();
		(*philo)[i].times_eaten = 0;
        i++;
    }
    i = 0;
    while (i < args->no_philosophers)
    {
        pthread_create(&(*thread)[i], NULL, philo_routine, &(*philo)[i]);
        i++;
    }
}

int main(int argc, char **argv)
{
    t_philoargs args;
    pthread_t *thread;
    t_philosopher *philo;
    int parse_error;
    pthread_t monitor_thread;
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
    threads_free(&args, thread, philo);
    return (0);
}
