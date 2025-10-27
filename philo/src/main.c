#include <philosopher.h>
#include <limits.h>

static void	check_args(int ac, char **av, t_engine *en)
{
	if (ac < 5 || ac > 6)
	{
		error_message("incorrect input!\nThe correct one is:\n./philo number_of_philosophers"
			" time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	}
	en->philo_count = ft_atoi(av[1]);
	en->time_to_die = ft_atoi(av[2]);
	en->time_to_eat = ft_atoi(av[3]);
	en->time_to_sleep = ft_atoi(av[4]);
	en->must_eat = -1;
	if (av[5])
		en->must_eat = ft_atoi(av[5]);
	if (en->philo_count < 1)
		error_message("[Argument ERROR] Invalid philosopher count\n", 1);
	if (en->time_to_die < 1 || en->time_to_die > INT_MAX)
		error_message("[Argument ERROR] Invalid time_to_die\n", 1);
	if (en->time_to_eat < 1 || en->time_to_eat > INT_MAX)
		error_message("[Argument ERROR] Invalid time_to_eat\n", 1);
	if (en->time_to_sleep < 1 || en->time_to_sleep > INT_MAX)
		error_message("[Argument ERROR] Invalid time_to_sleep\n", 1);
	if (en->must_eat < -1 || en->must_eat > INT_MAX)
		error_message("[Argument ERROR] Invalid must_eat\n", 1);
}

int	main(int ac, char **av)
{
	t_engine	engine;
	pthread_t	monitor;

	check_args(ac, av, &engine);
	if (init_engine(&engine) != 0)
		return (1);
	if (pthread_create(&monitor, NULL, monitor_routine, &engine) != 0)
	{
		destroy_all(&engine, "[Monitor ERROR]\n", 1, 1);
		return (1);
	}
	start_simulation(&engine);
	pthread_join(monitor, NULL);
	destroy_all(&engine, NULL, 0, 1);
	return (0);
}