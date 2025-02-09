/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_get_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:36:59 by bszikora          #+#    #+#             */
/*   Updated: 2025/02/09 18:27:21 by bszikora         ###   ########.fr       */
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

int	s_len(const char *str)
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

	result = 0;
	i = 0;
	*error = 0;
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
	return (result);
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
