/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:25:12 by jmarques          #+#    #+#             */
/*   Updated: 2025/10/27 11:25:14 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>
#include "unistd.h"

void	print_action(t_philo *philo, char *msg)
{
	size_t	timestamp;
	int		dead;

	pthread_mutex_lock(&philo->engine->death_lock);
	dead = philo->engine->is_dead;
	pthread_mutex_unlock(&philo->engine->death_lock);
	pthread_mutex_lock(philo->mutexes.write_lock);
	if (!dead || msg[0] == 'd')
	{
		timestamp = get_current_time() - philo->engine->start_time;
		printf("%zu %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(philo->mutexes.write_lock);
}

int	error_message(char *text)
{
	if (text)
		write(2, text, ft_strlen(text) + 1);
	return (-1);
}

static void	destroy_helper(t_engine *engine)
{
	pthread_mutex_destroy(&engine->write_lock);
	pthread_mutex_destroy(&engine->meal_lock);
	pthread_mutex_destroy(&engine->death_lock);
	pthread_mutex_destroy(&engine->finish_lock);
}

void	destroy_all(t_engine *engine, const char *msg, int free_mem)
{
	int	i;

	if (msg)
		printf("%s", msg);
	if (engine->forks)
	{
		i = 0;
		while (i < engine->philo_count)
		{
			pthread_mutex_destroy(&engine->forks[i]);
			i++;
		}
	}
	destroy_helper(engine);
	if (free_mem)
	{
		free(engine->forks);
		free(engine->philos);
		engine->forks = NULL;
		engine->philos = NULL;
	}
}
