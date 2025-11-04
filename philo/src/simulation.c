/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:31:14 by jmarques          #+#    #+#             */
/*   Updated: 2025/10/27 11:31:15 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>
#include <unistd.h>

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (pihlo_is_dead(philo))
			break ;
		philo_eat(philo);
		if (meal_eaten(philo))
			break ;
		print_action(philo, "is sleeping");
		ft_usleep(philo, philo->engine->time_to_sleep);
		if (pihlo_is_dead(philo))
			break ;
		print_action(philo, "is thinking");
	}
	if (meal_eaten(philo))
	{
		pthread_mutex_lock(&philo->engine->finish_lock);
		philo->engine->finished_philos++;
		pthread_mutex_unlock(&philo->engine->finish_lock);
	}
	return (NULL);
}

static int	routine_helper(t_engine	*engine)
{
	int		dead;

	pthread_mutex_lock(&engine->death_lock);
	dead = engine->is_dead;
	pthread_mutex_unlock(&engine->death_lock);
	if (dead)
		return (1);
	pthread_mutex_lock(&engine->finish_lock);
	if (engine->must_eat != -1
		&& engine->finished_philos >= engine->philo_count)
	{
		pthread_mutex_lock(&engine->death_lock);
		engine->is_dead = 1;
		pthread_mutex_unlock(&engine->death_lock);
		pthread_mutex_unlock(&engine->finish_lock);
		return (1);
	}
	pthread_mutex_unlock(&engine->finish_lock);
	return (0);
}

static int	meal_check(t_engine *engine, int i)
{
	size_t	time_l_m;

	pthread_mutex_lock(&engine->meal_lock);
	time_l_m = get_current_time() - engine->philos[i].times.last_meal;
	pthread_mutex_unlock(&engine->meal_lock);
	if (time_l_m > (size_t)engine->time_to_die)
	{
		pthread_mutex_lock(&engine->death_lock);
		engine->is_dead = 1;
		pthread_mutex_unlock(&engine->death_lock);
		print_action(&engine->philos[i], "died");
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_engine	*engine;
	int			i;

	engine = (t_engine *)arg;
	while (1)
	{
		i = -1;
		while (++i < engine->philo_count)
		{
			if (routine_helper(engine))
				return (NULL);
			if (meal_check(engine, i))
				return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
