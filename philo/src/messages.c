#include <philosopher.h>

void	print_action(t_philo *philo, char *msg)
{
	size_t	timestamp;

	pthread_mutex_lock(philo->mutexes.write_lock);
	if (!philo->engine->is_dead || msg[0] == 'd')
	{
		timestamp = get_current_time() - philo->engine->start_time;
		printf("%zu %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(philo->mutexes.write_lock);
}

void	error_message(char *text, int signal)
{
	if (text)
		write(2, text, ft_strlen(text) + 1);
	exit(signal);
}