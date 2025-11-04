/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 12:30:12 by jmarques          #+#    #+#             */
/*   Updated: 2025/10/27 12:30:14 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

typedef pthread_t		t_id;
typedef pthread_mutex_t	t_mutex;
typedef struct timeval	t_timeval;
typedef struct s_engine	t_engine;

typedef struct s_mutexes
{
	t_mutex	*left_fork;
	t_mutex	*right_fork;
	t_mutex	*write_lock;
	t_mutex	*meal_lock;
}	t_mutexes;

typedef struct s_times
{
	size_t	last_meal;
	size_t	born_time;
}	t_times;

typedef struct s_philo
{
	int			id;
	t_times		times;
	t_mutexes	mutexes;
	int			must_eat;
	t_id		thread_id;
	int			meals_eaten;
	int			philo_count;
	t_engine	*engine;
}	t_philo;

typedef struct s_engine
{
	int		philo_count;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		must_eat;
	size_t	start_time;
	t_mutex	*forks;
	t_philo	*philos;
	t_mutex	meal_lock;
	t_mutex	write_lock;
	t_mutex	death_lock;
	t_mutex	finish_lock;
	int		is_dead;
	int		finished_philos;
}	t_engine;

//utils
size_t	ft_strlen(const char *s);
size_t	get_current_time(void);
void	ft_usleep(t_philo *philo, size_t milliseconds);

// init
int		init_engine(t_engine *en);
void	start_simulation(t_engine *en);

// messages
void	destroy_all(t_engine *engine, const char *msg, int free_mem);
void	print_action(t_philo *philo, char *msg);
int		error_message(char *text);

// simulation 
void	*monitor_routine(void *arg);
void	*philo_routine(void *arg);

// simulation_helper
int		pihlo_is_dead(t_philo *philo);
void	philo_eat(t_philo *philo);
int		meal_eaten(t_philo *philo);

#endif
