#include <philosopher.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#include <sys/wait.h>

static void	one_philo(t_info *info, t_philo *philo)
{
	if (info->philo_count == 1)
	{
		print_action(philo, "has taken a fork");
		ft_usleep(info->time_to_die);
		print_action(philo, "died");
		exit(1);
	}
}

static void	take_forks(t_philo *philo)
{
	sem_wait(philo->info->forks);
	print_action(philo, "has taken a fork");
	sem_wait(philo->info->forks);
	print_action(philo, "has taken a fork");
}

static void	eat(t_philo *philo)
{
	t_info *info;

	info = philo->info;
	sem_wait(philo->meal_lock);
	philo->last_meal = get_current_time();
	sem_post(philo->meal_lock);
	print_action(philo, "is eating");
	ft_usleep(info->time_to_eat);
	philo->meals_eaten++;
	sem_post(info->info->forks);
	sem_post(info->forks);
}

static void	sleep_and_think(t_philo *philo)
{
	t_info *info = philo->info;

	print_action(philo, "is sleeping");
	ft_usleep(info->time_to_sleep);
	print_action(philo, "is thinking");
}

void	philo_routine(t_philo *philo)
{
	t_info	*info;
	
	info = philo->info;
	sem_wait(info->ready);
	philo->last_meal = info->start_time;
	pthread_create(&philo->monitor, NULL, &monitor_routine, philo);
	pthread_detach(philo->monitor);
	one_philo(info, philo);
	if (philo->id % 2 == 0)
		ft_usleep(10);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		if (info->must_eat != -1 && philo->meals_eaten >= info->must_eat)
			exit(0);
		sleep_and_think(philo);
	}
}
/*void	philo_routine(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	sem_wait(info->ready);
	philo->last_meal = info->start_time;
	pthread_create(&philo->monitor, NULL, &monitor_routine, philo);
	pthread_detach(philo->monitor);
	one_philo(info, philo);
	if (philo->id % 2 == 0)
		ft_usleep(10);
	while (1)
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
		print_action(philo, "is sleeping");
		ft_usleep(info->time_to_sleep);
		print_action(philo, "is thinking");
	}
}*/

void	kill_all_philos(t_info *info)
{
	int	i;

	i = -1;
	while (++i < info->philo_count)
		kill(info->pids[i], SIGKILL);
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
	t_philo	*philo = (t_philo *)arg;
	t_info	*info = philo->info;
	long	elapsed;

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
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/death");
	i = -1;
	while (++i < info->philo_count)
	{
		join(name, "/meal_lock_", i + 1);
		sem_close(philos[i].meal_lock);
		sem_unlink(name);
	}
}
