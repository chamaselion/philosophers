/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:59:05 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/31 12:29:30 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"

long get_time_of_day()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

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

void print_state(t_philosopher *philo, char *state, t_philoargs *philoarg)
{
    long timestamp;

    pthread_mutex_lock(&philoarg->print_mutex);
    timestamp = get_time_of_day();
    printf("%ld %d %s\n", timestamp, philo->id, state);
    pthread_mutex_unlock(&philoarg->print_mutex);
}

void threads_free(t_philoargs *args, pthread_t *thread, t_philosopher *philo)
{
    int i;

    i = 0;
    while (i < args->no_philosophers)
    {
        pthread_mutex_destroy(&args->forks[i]);
        pthread_mutex_destroy(&philo[i].meal_mutex);
        i = i + 1;
    }
    pthread_mutex_destroy(&args->print_mutex);

    free(args->forks);
    free(thread);
    free(philo);
}

void *monitor_routine(void *arg)
{
    t_philosopher *philos;
    t_philoargs *args;
    int i;
    long time_since_last_meal;

    philos = (t_philosopher *)arg;
    args = philos[0].args;

    while (1)
    {
        i = 0;
        while (i < args->no_philosophers)
        {
            pthread_mutex_lock(&philos[i].meal_mutex);
            time_since_last_meal = get_time_of_day() - philos[i].last_meal_time;
            pthread_mutex_unlock(&philos[i].meal_mutex);

            if (time_since_last_meal > args->time_to_die)
            {
                pthread_mutex_lock(&args->print_mutex);
                printf("%ld %d died\n", get_time_of_day(), philos[i].id);
                exit(0);
            }
            i = i + 1;
        }
        usleep(1000);
    }
}


void *philo_routine(void *arg)
{
    t_philosopher *philo;
    t_philoargs *philoarg;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;

    philo = (t_philosopher *)arg;
    philoarg = philo->args;
    left_fork = philo->fork;
    right_fork = &philoarg->forks[(philo->id + 1) % philoarg->no_philosophers];

    if (philo->id % 2 == 0)
    {
        usleep(1000);
    }

    while (1)
    {
        print_state(philo, "is thinking", philoarg);

        pthread_mutex_lock(left_fork);
        print_state(philo, "has taken a fork", philoarg);

        pthread_mutex_lock(right_fork);
        print_state(philo, "has taken a fork", philoarg);

        pthread_mutex_lock(&philo->meal_mutex);
        philo->last_meal_time = get_time_of_day();
        pthread_mutex_unlock(&philo->meal_mutex);

        print_state(philo, "is eating", philoarg);
        usleep(philoarg->time_to_eat * 1000);

        pthread_mutex_unlock(right_fork);
        pthread_mutex_unlock(left_fork);

        print_state(philo, "is sleeping", philoarg);
        usleep(philoarg->time_to_sleep * 1000);
    }
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
        (*philo)[i].id = i;
        (*philo)[i].fork = &args->forks[i];
        (*philo)[i].args = args;
        (*philo)[i].last_meal_time = get_time_of_day();
        i = i + 1;
    }

    i = 0;
    while (i < args->no_philosophers)
    {
        pthread_create(&(*thread)[i], NULL, philo_routine, &(*philo)[i]);
        i = i + 1;
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

    threads_init(&args, &thread, &philo);
    pthread_create(&monitor_thread, NULL, monitor_routine, philo);
    threads_join(&args, thread);
    threads_free(&args, thread, philo);
    return (0);
}