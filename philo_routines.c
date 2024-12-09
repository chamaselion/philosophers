/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:04:50 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/09 16:07:53 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"

void *monitor_routine(void *arg)
{
    t_philosopher *philos;
    t_philoargs *args;
    int i;
    long time_since_last_meal;
    int all_right;

	philos = (t_philosopher *)arg;
   	args = philos[0].args;
    while (1)
    {
        i = 0;
        if (args->extra > 0)
        {
            extra_checker(&all_right, philos, args);
            if (all_right)
            {
                pthread_mutex_lock(&args->print_mutex);
                printf("All philosophers have eaten enough times\n");
                exit(0);
            }
        }
        while (i < args->no_philosophers)
        {
			if (philos[i].is_eating == 0)
			{
            pthread_mutex_lock(&philos[i].meal_mutex);
            time_since_last_meal = get_time_of_day() - philos[i].last_meal_time;
            if (time_since_last_meal > args->time_to_die)
            {
                pthread_mutex_lock(&args->print_mutex);
                printf("%ld %d died\n", (get_time_of_day() - args->firstime), philos[i].id);
                exit(0);
            }
            pthread_mutex_unlock(&philos[i].meal_mutex);
			}
            i++;
        }
        usleep(100);
    }
    return (NULL);
}

void eating_routine(t_philosopher *philo, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork, t_philoargs *philoarg)
{
    pthread_mutex_lock(left_fork);
    print_state(philo, "has taken a fork", philoarg);
    pthread_mutex_lock(right_fork);
    print_state(philo, "has taken a fork", philoarg);
	philo->is_eating = 1;
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal_time = get_time_of_day();
    philo->times_eaten++;
    print_state(philo, "is eating", philoarg);
    precise_sleep(philoarg->time_to_eat);
    
    pthread_mutex_unlock(right_fork);
    pthread_mutex_unlock(left_fork);
    pthread_mutex_unlock(&philo->meal_mutex);
	philo->is_eating = 0;
}

void sleeping_routine(t_philosopher *philo, t_philoargs *philoarg)
{
    print_state(philo, "is sleeping", philoarg);
    precise_sleep(philoarg->time_to_sleep);
}

void *philo_routine(void *arg)
{
    t_philosopher *philo;
    t_philoargs *philoarg;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;

	philo = (t_philosopher *)arg;
    philoarg = philo->args;
    initialize_forks(philo, &left_fork, &right_fork);
    if (philo->id % 2 == 0)
        precise_sleep(100);
    while (1)
    {
        print_state(philo, "is thinking", philoarg);
        eating_routine(philo, left_fork, right_fork, philoarg);
        sleeping_routine(philo, philoarg);
    }
    return NULL;
}