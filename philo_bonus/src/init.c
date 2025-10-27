#include <philosopher.h>
#include <fcntl.h> 

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
		philos[i].last_meal = info->start_time;
		sprintf(name, "/meal_lock_%d", i + 1);
		sem_unlink(name);
		philos[i].meal_lock = sem_open(name, O_CREAT, 0644, 1);
		if (philos[i].meal_lock == SEM_FAILED)
			error_message("sem_open meal_lock failed\n", 1);
		info->start_time = get_current_time(); // set once right before release
		for (int i = 0; i < info->philo_count; i++)
    		sem_post(info->ready);
		info->pids[i] = fork();
		if (info->pids[i] == 0)
		{
			philo_routine(&philos[i]);
			exit(0);
		}
		if (info->pids[i] == -1)
			error_message("Fork failed\n", 1);
	}
}

int	init_info(t_info *info)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/death");
	sem_unlink("/ready");
	info->ready = sem_open("/ready", O_CREAT, 0644, 0);
	if (info->ready == SEM_FAILED)
    	error_message("sem_open ready failed\n", 1);
	info->forks = sem_open("/forks", O_CREAT, 0644, info->philo_count);
	info->print = sem_open("/print", O_CREAT, 0644, 1);
	info->stop = sem_open("/stop", O_CREAT, 0644, 0);
	info->death = sem_open("/death", O_CREAT, 0644, 1);
	if (info->forks == SEM_FAILED || info->print == SEM_FAILED
		|| info->stop == SEM_FAILED || info->death == SEM_FAILED)
	{
		perror("sem_open");
		return (1);
	}
	info->pids = malloc(sizeof(pid_t) * info->philo_count);
	if (!info->pids)
		error_message("Malloc failed\n", 1);
	info->start_time = get_current_time();
	return (0);
}
