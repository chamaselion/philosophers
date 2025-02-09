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

int s_len(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

int	int_isdigit(int i)
{
	if (i >= '0' && i <= '9')
	{
		return (1);
	}
	return (0);
}

long	atol_with_error(const char *s, int *error)
{
	long	result;
	int		i;
	int		len;
	int		sign;

	result = 0;
	i = 0;
	sign = 1;
	*error = 0;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	len = s_len(s + i);
	if (len > MAX_DIGITS)
		return (*error = 1, 0);
	while (s[i])
	{
		if (!int_isdigit(s[i]))
			return (*error = 1, 0);
		if (result > (LONG_MAX - (s[i] - '0')) / 10)
			return (*error = 1, 0);
		result = result * 10 + (s[i] - '0');
		i++;
	}
	result *= sign;
	if ((sign == 1 && result < 0) || (sign == -1 && result > 0))
		return (*error = 1, 0);
	return (result);
}

int	get_args(t_philoargs *args, char **argv)
{
	int		error_flag;
	long	nbr;

	error_flag = 0;
	nbr = atol_with_error(argv[1], &error_flag);
	if (error_flag || nbr <= 0 || nbr > 250)
	{
		write(STDERR_FILENO, "Error, argument is too long or invalid\n", 40);
		return (1);
	}
	args->no_philosophers = (int)nbr;
	args->time_to_die = atol_with_error(argv[2], &error_flag);
	if (error_flag)
	{
		write(STDERR_FILENO, "Error, argument is too long or invalid\n", 40);
		return (1);
	}
	args->time_to_eat = atol_with_error(argv[3], &error_flag);
	if (error_flag)
	{
		write(STDERR_FILENO, "Error, argument is too long or invalid\n", 40);
		return (1);
	}
	args->time_to_sleep = atol_with_error(argv[4], &error_flag);
	if (error_flag)
	{
		write(STDERR_FILENO, "Error, argument is too long or invalid\n", 40);
		return (1);
	}
	if (args->extra == 1)
	{
		args->no_t_philosopher_must_eat = atol_with_error(argv[5], &error_flag);
		if (error_flag)
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
	if (get_args(args, argv) == 1)
		return (1);
	return (0);
}
