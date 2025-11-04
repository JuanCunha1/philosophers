/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:25:06 by jmarques          #+#    #+#             */
/*   Updated: 2025/10/27 11:25:08 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>
#include <sys/time.h>
#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

void	ft_usleep(t_philo *philo, size_t milliseconds)
{
	size_t	start;
	size_t	now;
	int		dead;

	start = get_current_time();
	while (1)
	{
		now = get_current_time();
		if ((now - start) >= milliseconds)
			break ;
		pthread_mutex_lock(&philo->engine->death_lock);
		dead = philo->engine->is_dead;
		pthread_mutex_unlock(&philo->engine->death_lock);
		if (dead)
			break ;
		usleep(100);
	}
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
