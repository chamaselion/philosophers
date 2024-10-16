/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bszikora <bszikora@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:59:05 by bszikora          #+#    #+#             */
/*   Updated: 2024/10/08 14:38:13 by bszikora         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "philo.h"

int main(int argc, char **argv)
{
	t_philoargs args;
	int parse_error;

	parse_error = user_input_parse(&args, argc, argv);
	if (parse_error)
		return (1);

	printf("Extra: %i\n", args.extra);
	printf("Number of Philosophers: %i\n", args.no_philosophers);
	printf("Time to Die: %i\n", args.time_to_die);
	printf("Time to Eat: %i\n", args.time_to_eat);
	printf("Time to Sleep: %i\n", args.time_to_sleep);
	if (args.extra == 1)
	{
		printf("Number of Times Each Philosopher Must Eat: %i\n", args.no_t_philosopher_must_eat);
	}

	return (0);
}