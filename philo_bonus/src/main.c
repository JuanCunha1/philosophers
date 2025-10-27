#include <philosopher.h>
#include <limits.h>

static void	check_args(int ac, char **av, t_info *info)
{
	if (ac < 5 || ac > 6)
	{
		printf("incorrect input!\nThe correct one is:\n./philo number_of_philosophers"
			" time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		exit(1);
	}
	info->philo_count = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]);
	info->time_to_eat = ft_atoi(av[3]);
	info->time_to_sleep = ft_atoi(av[4]);
	info->must_eat = -1;
	if (av[5])
		info->must_eat = ft_atoi(av[5]);
	if (info->philo_count < 1)
		error_message("[Argument ERROR] Invalid philosopher count\n", 1);
	if (info->time_to_die < 1 || info->time_to_die > INT_MAX)
		error_message("[Argument ERROR] Invalid time_to_die\n", 1);
	if (info->time_to_eat < 1 || info->time_to_eat > INT_MAX)
		error_message("[Argument ERROR] Invalid time_to_eat\n", 1);
	if (info->time_to_sleep < 1 || info->time_to_sleep > INT_MAX)
		error_message("[Argument ERROR] Invalid time_to_sleep\n", 1);
	if (info->must_eat < -1 || info->must_eat > INT_MAX)
		error_message("[Argument ERROR] Invalid must_eat\n", 1);
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