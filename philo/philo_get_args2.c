/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_get_args2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:31:37 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/09 17:31:37 by bszikora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_args_part1(t_philoargs *args, char **argv, int *error_flag)
{
	long	nbr;

	*error_flag = 0;
	nbr = atol_with_error(argv[1], error_flag);
	if (*error_flag || nbr <= 0 || nbr > 250)
		return (write(STDERR_FILENO, "Error, argument is too long or invalid\n", 40), 1);
	args->no_philosophers = (int)nbr;
	args->time_to_die = atol_with_error(argv[2], error_flag);
	if (*error_flag)
		return (write(STDERR_FILENO, "Error, argument is too long or invalid\n", 40), 1);
	args->time_to_eat = atol_with_error(argv[3], error_flag);
	if (*error_flag)
		return (write(STDERR_FILENO, "Error, argument is too long or invalid\n", 40), 1);
	args->time_to_sleep = atol_with_error(argv[4], error_flag);
	if (*error_flag)
		return (write(STDERR_FILENO, "Error, argument is too long or invalid\n", 40), 1);
	return (0);
}

int	get_args_part2(t_philoargs *args, char **argv, int *error_flag)
{
	if (args->extra == 1)
	{
		args->no_t_philosopher_must_eat = atol_with_error(argv[5], error_flag);
		if (*error_flag)
		{
			write(STDERR_FILENO, "Error, argument is too long or invalid\n", 40);
			return (1);
		}
	}
	else
	{
		args->no_t_philosopher_must_eat = -1;
	}
	return (0);
}

int	get_args(t_philoargs *args, char **argv)
{
	int	error_flag;

	if (get_args_part1(args, argv, &error_flag) == 1)
		return (1);
	if (get_args_part2(args, argv, &error_flag) == 1)
		return (1);
	return (0);
}
