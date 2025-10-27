#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>
# include <limits.h>

typedef struct s_info	t_info;
typedef struct s_philo	t_philo;

struct s_info
{
	int		philo_count;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	int		must_eat;
	long	start_time;
	pid_t	*pids;
	sem_t	*forks;
	sem_t	*print;
	sem_t	*death;
	sem_t	*stop;
	sem_t	*ready;
};

struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal;
	pid_t			pid;
	pthread_t		monitor;
	sem_t			*meal_lock;
	t_info			*info;
};

//utils
long	ft_atoi(const char *str);
void	error_message(char *text, int signal);
size_t	ft_strlen(const char *s);
long	get_current_time(void);
void	ft_usleep(size_t milliseconds);


// destroy
void	print_action(t_philo *philo, char *msg);

void	philo_routine(t_philo *philo);
void	*monitor_routine(void *arg);
void	supervisor(t_info *info);
void	kill_all_philos(t_info *info);

void	one_philo(t_info *info, t_philo *philo);
void	close_semaphores(t_info *info, t_philo *philos);
void	init_philos(t_philo *philos, t_info *info);
int		init_info(t_info *info);
#endif