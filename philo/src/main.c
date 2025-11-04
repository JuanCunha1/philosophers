/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:24:16 by jmarques          #+#    #+#             */
/*   Updated: 2025/10/27 11:24:17 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>
#include <limits.h>

static int	check_arg_content(char **arg, int ac)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		while ((arg[i][j] >= 9 && arg[i][j] <= 13) || arg[i][j] == ' ')
			++j;
		if (arg[i][j] == '+')
			j++;
		while (arg[i][j] != '\0')
		{
			if ((arg[i][j] < '0' || arg[i][j] > '9'))
				return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

static long	ft_atoi(const char *str)
{
	long	num;
	int		i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		++i;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			return (-1);
	num = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		num = num + (str[i] - '0');
		++i;
		if (num > INT_MAX)
			return (-1);
	}
	return (num);
}

static int	check_args(int ac, char **av, t_engine *en)
{
	if (ac < 5 || ac > 6)
		return (error_message("incorrect input!\nThe correct one is:\n"
				"./philo nbr_of_philos time_to_die time_to_eat time_to_sleep"
				" [nbr_of_times_each_philosopher_must_eat]\n"));
	if (check_arg_content(av, ac) == -1)
		return (error_message("Just number allowed\n"));
	en->philo_count = ft_atoi(av[1]);
	en->time_to_die = ft_atoi(av[2]);
	en->time_to_eat = ft_atoi(av[3]);
	en->time_to_sleep = ft_atoi(av[4]);
	en->must_eat = -1;
	if (av[5])
		en->must_eat = ft_atoi(av[5]);
	if (en->philo_count < 1)
		return (error_message("Invalid philosopher count\n"));
	else if (en->time_to_die < 1)
		return (error_message("Invalid time_to_die\n"));
	else if (en->time_to_eat < 1)
		return (error_message("Invalid time_to_eat\n"));
	else if (en->time_to_sleep < 1)
		return (error_message("Invalid time_to_sleep\n"));
	else if (en->must_eat < -1)
		return (error_message("Invalid must_eat\n"));
	return (1);
}

int	main(int ac, char **av)
{
	t_engine	engine;
	pthread_t	monitor;

	if (check_args(ac, av, &engine) == -1)
		return (1);
	if (init_engine(&engine) != 0)
		return (1);
	if (pthread_create(&monitor, NULL, monitor_routine, &engine) != 0)
	{
		destroy_all(&engine, "Monitor ERROR\n", 1);
		return (1);
	}
	start_simulation(&engine);
	pthread_join(monitor, NULL);
	destroy_all(&engine, NULL, 1);
	return (0);
}
