/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_get_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:36:59 by bszikora          #+#    #+#             */
/*   Updated: 2024/12/11 14:18:40 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	pre_check(int argc, char **argv, int *extra)
{
	int	c;

	c = 1;
	if (argc == 5 || argc == 6)
	{
		*extra = 0;
		if (argc == 6)
		{
			*extra = 1;
		}
		while (c < argc)
		{
			if (str_isdigit(argv[c]) == 0)
				return (-2);
			c++;
		}
		return (0);
	}
	else
		return (-1);
}

void	get_args(t_philoargs *args, char **argv)
{
	args->no_philosophers = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (args->extra == 1)
		args->no_t_philosopher_must_eat = ft_atoi(argv[5]);
}

int	user_input_parse(t_philoargs *args, int argc, char **argv)
{
	int	err;

	err = pre_check(argc, argv, &args->extra);
	if (err == -1)
	{
		write(STDERR_FILENO, "Error, number of arguments is incorrect.\n", 41);
		return (1);
	}
	if (err == -2)
	{
		write(STDERR_FILENO, "Error, non-digit found in the argument\n", 39);
		return (1);
	}
	else
		get_args(args, argv);
	if (args->no_philosophers == 0)
	{
		write(STDERR_FILENO, "Error, simulation can not start with 0 philos\n", 46);
		return (1);
	}
	return (0);
}
