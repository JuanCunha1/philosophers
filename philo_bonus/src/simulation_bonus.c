/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:44:18 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/07 12:44:21 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher_bonus.h>
#include <pthread.h>
#include <sys/wait.h>

void	one_philo(t_info *info, t_philo *philo)
{
	if (info->philo_count == 1)
	{
		print_action(philo, "has taken a fork");
		ft_usleep(info->time_to_die);
		print_action(philo, "died");
		exit(1);
	}
}

static void	philo_routine_helper(t_philo *philo, t_info *info)
{
	sem_wait(info->forks);
	print_action(philo, "has taken a fork");
	sem_wait(info->forks);
	print_action(philo, "has taken a fork");
	sem_wait(philo->meal_lock);
	philo->last_meal = get_current_time();
	sem_post(philo->meal_lock);
	print_action(philo, "is eating");
	ft_usleep(info->time_to_eat);
	philo->meals_eaten++;
	sem_post(info->forks);
	sem_post(info->forks);
	if (info->must_eat != -1 && philo->meals_eaten >= info->must_eat)
		exit(0);
}

void	philo_routine(t_philo *philo, t_info *info)
{
	philo->last_meal = info->start_time;
	pthread_create(&philo->monitor, NULL, &monitor_routine, philo);
	pthread_detach(philo->monitor);
	one_philo(info, philo);
	if (philo->id % 2 == 0)
		ft_usleep(10);
	while (1)
	{
		philo_routine_helper(philo, info);
		print_action(philo, "is sleeping");
		ft_usleep(info->time_to_sleep);
		print_action(philo, "is thinking");
	}
}

void	supervisor(t_info *info)
{
	int		status;
	int		i;
	pid_t	pid;

	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			i = -1;
			while (++i < info->philo_count)
				kill(info->pids[i], SIGKILL);
			break ;
		}
	}
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	t_info	*info;
	long	elapsed;

	philo = (t_philo *)arg;
	info = philo->info;
	while (1)
	{
		sem_wait(philo->meal_lock);
		elapsed = get_current_time() - philo->last_meal;
		sem_post(philo->meal_lock);
		if (elapsed > info->time_to_die)
		{
			print_action(philo, "died");
			sem_post(info->stop);
			exit(1);
		}
		ft_usleep(1);
	}
}
