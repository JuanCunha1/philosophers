/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:40:48 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/07 12:40:52 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher_bonus.h>
#include <fcntl.h>
#include <signal.h>

static void	start_philos(t_philo *philos, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		info->pids[i] = fork();
		if (info->pids[i] == -1)
		{
			printf("Fork failed\n");
			while (--i >= 0)
				kill(info->pids[i], SIGTERM);
			return ;
		}
		if (info->pids[i] == 0)
		{
			sem_wait(info->ready);
			philo_routine(&philos[i], info);
			exit(1);
		}
		i++;
	}
	i = -1;
	while (++i < info->philo_count)
		sem_post(info->ready);
}

void	init_philos(t_philo *philos, t_info *info)
{
	int		i;
	char	name[32];

	i = -1;
	while (++i < info->philo_count)
	{
		philos[i].id = i + 1;
		philos[i].info = info;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = 0;
		join(name, "/meal_lock_", i + 1);
		sem_unlink(name);
		philos[i].meal_lock = sem_open(name, O_CREAT, 0644, 1);
		if (philos[i].meal_lock == SEM_FAILED)
			error_message("sem_open meal_lock failed\n", 1);
	}
	start_philos(philos, info);
}

int	init_info(t_info *info)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/death");
	sem_unlink("/ready");
	info->ready = sem_open("/ready", O_CREAT, 0644, 0);
	info->forks = sem_open("/forks", O_CREAT, 0644, info->philo_count);
	info->print = sem_open("/print", O_CREAT, 0644, 1);
	info->stop = sem_open("/stop", O_CREAT, 0644, 0);
	info->death = sem_open("/death", O_CREAT, 0644, 1);
	if (info->ready == SEM_FAILED || info->forks == SEM_FAILED
		|| info->print == SEM_FAILED || info->stop == SEM_FAILED
		|| info->death == SEM_FAILED)
	{
		error_message("sem_open failed\n", 1);
	}
	info->pids = malloc(sizeof(pid_t) * info->philo_count);
	if (!info->pids)
		error_message("Malloc failed\n", 1);
	info->start_time = get_current_time();
	return (0);
}
