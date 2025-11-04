/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:26:00 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/04 12:26:04 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher.h>
#include <unistd.h>

static int	one_philo(t_philo *philo, pthread_mutex_t *first_fork)
{
	pthread_mutex_lock(first_fork);
	print_action(philo, "has taken a fork");
	if (philo->engine->philo_count == 1)
	{
		ft_usleep(philo, philo->engine->time_to_die + 1);
		pthread_mutex_unlock(first_fork);
		return (1);
	}
	return (0);
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	first_fork = philo->mutexes.left_fork;
	second_fork = philo->mutexes.right_fork;
	if (one_philo(philo, first_fork))
		return ;
	pthread_mutex_lock(second_fork);
	print_action(philo, "has taken a fork");
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->engine->meal_lock);
	philo->times.last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->engine->meal_lock);
	if (!meal_eaten(philo))
		ft_usleep(philo, philo->engine->time_to_eat);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

int	pihlo_is_dead(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(&philo->engine->death_lock);
	dead = philo->engine->is_dead;
	pthread_mutex_unlock(&philo->engine->death_lock);
	return (dead);
}

int	meal_eaten(t_philo *philo)
{
	int	meals;

	pthread_mutex_lock(&philo->engine->meal_lock);
	meals = philo->meals_eaten;
	pthread_mutex_unlock(&philo->engine->meal_lock);
	if (philo->must_eat != -1 && meals >= philo->must_eat)
		return (1);
	return (0);
}
