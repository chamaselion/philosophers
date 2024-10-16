/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:59:16 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/08 14:39:46 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

# ifndef PHILO
# define PHILO

#include <stdlib.h>
#include <stdio.h>


typedef struct s_philoargs {
    int no_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int no_t_philosopher_must_eat;
	int extra;
} t_philoargs;

// philo utilities
int str_isdigit(char *i);
int ft_atoi(const char *t);

// philo input handle
int user_input_parse(t_philoargs *args, int argc, char **argv);

# endif