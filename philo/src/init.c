/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 11:24:23 by jmarques          #+#    #+#             */
/*   Updated: 2025/10/27 11:24:39 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>

static void	init_forks(t_philo *philo, t_mutex *forks, int i, int philo_count)
{
	if (i % 2 == 0)
	{
		philo->mutexes.left_fork = &forks[i];
		philo->mutexes.right_fork = &forks[(i + 1) % philo_count];
	}
	else
	{
		philo->mutexes.left_fork = &forks[(i + 1) % philo_count];
		philo->mutexes.right_fork = &forks[i];
	}
}

static void	init_philos(t_engine *en, t_philo *philos, t_mutex *forks)
{
	int		i;

	i = -1;
	while (++i < en->philo_count)
	{
		philos[i].id = i + 1;
		philos[i].times.last_meal = en->start_time;
		philos[i].times.born_time = en->start_time;
		philos[i].must_eat = en->must_eat;
		philos[i].meals_eaten = 0;
		philos[i].philo_count = en->philo_count;
		philos[i].mutexes.write_lock = &en->write_lock;
		philos[i].mutexes.meal_lock = &en->meal_lock;
		philos[i].engine = en;
		init_forks(&philos[i], forks, i, en->philo_count);
	}
}

void	start_simulation(t_engine *en)
{
	int	i;

	i = -1;
	while (++i < en->philo_count)
	{
		if (pthread_create(&en->philos[i].thread_id, NULL,
				&philo_routine, &en->philos[i]) != 0)
		{
			destroy_all(en, "Fork Mutex ERROR\n", 1);
			return ;		
		}
	}
	i = -1;
	while (++i < en->philo_count)
		pthread_join(en->philos[i].thread_id, NULL);
}

static int	init_single_mutex(t_engine *en)
{
	if (pthread_mutex_init(&en->death_lock, NULL) != 0
		|| pthread_mutex_init(&en->finish_lock, NULL) != 0
		|| pthread_mutex_init(&en->write_lock, NULL) != 0
		|| pthread_mutex_init(&en->meal_lock, NULL) != 0)
		return (1);
	return (0);
}

int	init_engine(t_engine *en)
{
	int	i;

	if (init_single_mutex(en))
		return (destroy_all(en, "Mutex Init ERROR\n", 1), -1);
	en->forks = malloc(sizeof(t_mutex) * en->philo_count);
	en->philos = malloc(sizeof(t_philo) * en->philo_count);
	if (!en->forks || !en->philos)
		return (destroy_all(en, "Malloc ERROR\n", 1), -1);
	en->is_dead = 0;
	en->finished_philos = 0;
	en->start_time = get_current_time();
	i = -1;
	while (++i < en->philo_count)
	{
		if (pthread_mutex_init(&en->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&en->forks[i]);
			return (destroy_all(en, "Fork Mutex ERROR\n", 1), -1);
		}
	}
	init_philos(en, en->philos, en->forks);
	return (0);
}
