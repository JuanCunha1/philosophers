#include <philosopher.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	print_action(t_philo *philo, char *msg)
{
	long	timestamp;

	sem_wait(philo->info->print);
	timestamp = get_current_time() - philo->info->start_time;
	if (!ft_strcmp(msg, "died"))
	{
		printf("%ld %d %s\n", timestamp, philo->id, msg);
		return ;
	}
	printf("%ld %d %s\n", timestamp, philo->id, msg);
	sem_post(philo->info->print);
}