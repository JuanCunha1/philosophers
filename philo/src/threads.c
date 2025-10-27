#include <philosopher.h>

void	destroy_all(t_engine *engine, const char *msg, int exit_code, int free_mem)
{
	int i;

	if (msg)
		fprintf(stderr, "%s", msg);
	if (engine->forks)
	{
		i = 0;
		while (i < engine->philo_count)
		{
			pthread_mutex_destroy(&engine->forks[i]);
			i++;
		}
	}
	pthread_mutex_destroy(&engine->write_lock);
	pthread_mutex_destroy(&engine->meal_lock);
	if (free_mem)
	{
		free(engine->forks);
		free(engine->philos);
		engine->forks = NULL;
		engine->philos = NULL;
	}
	if (exit_code != -1)
		exit(exit_code);
}