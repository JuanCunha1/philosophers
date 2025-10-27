#include <philosopher.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
/*
static void	philo_eat(t_philo *philo)
{
	if (philo->mutexes.left_fork != philo->mutexes.right_fork)
	{
		pthread_mutex_lock(philo->mutexes.left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->mutexes.right_fork);
		print_action(philo, "has taken a fork");
		print_action(philo, "is eating");
		pthread_mutex_lock(philo->mutexes.meal_lock);
		philo->times.last_meal = get_current_time();
		philo->meals_eaten++;
		pthread_mutex_unlock(philo->mutexes.meal_lock);
		ft_usleep(philo, philo->engine->time_to_eat);
		pthread_mutex_unlock(philo->mutexes.right_fork);
		pthread_mutex_unlock(philo->mutexes.left_fork);
	}
}*/
int take_forks(t_philo *philo)
{
    pthread_mutex_t	*first;
    pthread_mutex_t	*second;

	first = philo->left_fork;
	second = philo->right_fork;
    if (philo->id % 2 == 0)
    {
        first = philo->right_fork;
        second = philo->left_fork;
		ft_usleep(philo, 1);
    }
    while (simulation_running(philo))
    {
        if (pthread_mutex_trylock(first) == 0)
        {
            if (pthread_mutex_trylock(second) == 0)
            {
                return (1);
            }
            pthread_mutex_unlock(first);
        }
        ft_usleep(philo, 1);
        if (someone_died)
            return (0);
    }
	return (0);
}
static void	philo_eat(t_philo *philo)
{
	pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;

	first_fork = philo->mutexes.right_fork;
	second_fork = philo->mutexes.left_fork;
	if (philo->id % 2 == 0)
	{
		first_fork = philo->mutexes.left_fork;
		second_fork = philo->mutexes.right_fork;
	}
	else
	{
		first_fork = philo->mutexes.right_fork;
		second_fork = philo->mutexes.left_fork;
	}
	if (first_fork == second_fork)
	{
		pthread_mutex_lock(first_fork);
		print_action(philo, "has taken a fork");
		ft_usleep(philo, philo->engine->time_to_die + 1);
		pthread_mutex_unlock(first_fork);
		return ;
	}
	pthread_mutex_lock(first_fork);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_action(philo, "has taken a fork");
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->engine->meal_lock);
	philo->times.last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->engine->meal_lock);
	ft_usleep(philo, philo->engine->time_to_eat);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);

}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if (philo->id % 2 == 0)
        ft_usleep(philo, 2);
    while (!philo->engine->is_dead)
    {
        philo_eat(philo);
        if (philo->must_eat != -1 && philo->meals_eaten >= philo->must_eat)
        {
            pthread_mutex_lock(&philo->engine->meal_lock);
            philo->engine->finished_philos++;
            pthread_mutex_unlock(&philo->engine->meal_lock);
            break ;
        }
		print_action(philo, "is sleeping");
        ft_usleep(philo, philo->engine->time_to_sleep);
        if (philo->engine->is_dead)
            break ;
        print_action(philo, "is thinking");
    }
    return (NULL);
}

void	start_simulation(t_engine *en)
{
	int	i;

	i = -1;
	while (++i < en->philo_count)
	{
		if (pthread_create(&en->philos[i].thread_id, NULL,
				&philo_routine, &en->philos[i]) != 0)
				destroy_all(en, "[Fork Mutex ERROR]\n", 1, 1);
	}
	i = -1;
	while (++i < en->philo_count)
		pthread_join(en->philos[i].thread_id, NULL);
}

void *monitor_routine(void *arg)
{
    t_engine	*engine;
    size_t		time_l_m;
    int			i;

	engine  = (t_engine *)arg;
	while (1)
	{
		i = -1;
		while (++i < engine->philo_count)
		{
			pthread_mutex_lock(&engine->meal_lock);
			if (engine->is_dead)
			{
				pthread_mutex_unlock(&engine->meal_lock);
				return (NULL);
			}
			time_l_m = get_current_time() - engine->philos[i].times.last_meal;
			if (time_l_m > (size_t)engine->time_to_die)
			{
				pthread_mutex_lock(&engine->write_lock);
				engine->is_dead = 1;
				print_action(&engine->philos[i], "died");
				pthread_mutex_unlock(&engine->write_lock);
				pthread_mutex_unlock(&engine->meal_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&engine->meal_lock);
		}
		usleep(1000);
	}
	return (NULL);
}