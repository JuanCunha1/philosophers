/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:41:56 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/07 12:42:01 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher_bonus.h>
#include <limits.h>

void	close_semaphores(t_info *info, t_philo *philos)
{
	int		i;
	char	name[32];

	if (info->forks)
		sem_close(info->forks);
	if (info->print)
		sem_close(info->print);
	if (info->stop)
		sem_close(info->stop);
	if (info->death)
		sem_close(info->death);
	if (info->ready)
		sem_close(info->ready);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/death");
	sem_unlink("/ready");
	i = -1;
	while (++i < info->philo_count)
	{
		join(name, "/meal_lock_", i + 1);
		sem_close(philos[i].meal_lock);
		sem_unlink(name);
	}
}

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
		if (arg[i][j] == '\0')
			return (-1);
		while (arg[i][j] != '\0')
		{
			if ((arg[i][j] < '0' || arg[i][j] > '9'))
				return (-1);
			++j;
		}
		++i;
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
	if (str[i] == '-' || str[i] == '\0')
		return (-1);
	if (str[i] == '+')
		i++;
	num = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		if (num > INT_MAX)
			return (-1);
		++i;
	}
	if (str[i] != '\0')
		return (-1);
	return (num);
}

static void	check_args(int ac, char **av, t_info *info)
{
	if (ac < 5 || ac > 6)
		error_message("incorrect input!\nThe correct one is:\n"
			"./philo nbr_of_philos time_to_die time_to_eat time_to_sleep"
			" [nbr_of_times_each_philosopher_must_eat]\n", 1);
	if (check_arg_content(av, ac) == -1)
		error_message("invalid argument\n", 1);
	info->philo_count = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]);
	info->time_to_eat = ft_atoi(av[3]);
	info->time_to_sleep = ft_atoi(av[4]);
	info->must_eat = -1;
	if (ac == 6)
		info->must_eat = ft_atoi(av[5]);
	if (info->philo_count < 1)
		error_message("Invalid philosopher count\n", 1);
	if (info->time_to_die < 1)
		error_message("Invalid time_to_die\n", 1);
	if (info->time_to_eat < 1)
		error_message("Invalid time_to_eat\n", 1);
	if (info->time_to_sleep < 1)
		error_message("Invalid time_to_sleep\n", 1);
	if (ac == 6 && info->must_eat < 1)
		error_message("Invalid must_eat\n", 1);
}

int	main(int ac, char **av)
{
	t_info	info;
	t_philo	*philos;

	check_args(ac, av, &info);
	if (init_info(&info) != 0)
		return (1);
	philos = malloc(sizeof(t_philo) * info.philo_count);
	if (!philos)
		error_message("Malloc failed\n", 1);
	init_philos(philos, &info);
	supervisor(&info);
	close_semaphores(&info, philos);
	free(info.pids);
	free(philos);
	return (0);
}
