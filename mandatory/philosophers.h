/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:23:03 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/25 16:56:57 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef struct input
{
	bool			check_meal_eated;
	int				num_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_eat;
}					t_input;

typedef struct fork
{
	int				id;
	pthread_mutex_t	mutex;
}					t_fork;

typedef struct mutex
{
	pthread_mutex_t	printf;
	pthread_mutex_t	stop_simulatoin;
	pthread_mutex_t	philo_died;
	pthread_mutex_t	start;
	pthread_mutex_t	last_time_eat;
	pthread_mutex_t	meal_eaten;
	pthread_mutex_t	start_thinking;
}					t_mutex;

typedef struct philo
{
	int				id;
	t_fork			*right_f;
	t_fork			*left_f;
	bool			has_left_fork;
	bool			has_right_fork;
	long			*start;
	pthread_t		thread;
	int				meal_eaten;
	pthread_mutex_t	meal_mutex;
	long			last_time_eat;
	t_input			*input;
	bool			*simulation_has_ended;
	bool			*philo_died;
	t_mutex			*mutex;
	long			start_thinking;
}					t_philo;

typedef struct death
{
	long	time;
	int		philo;
}			t_death;

typedef struct data
{
	long			start;
	t_input			*input;
	t_philo			*philo;
	t_fork			*fork;
	t_mutex			*mutex;
	pthread_t		*thread;
	t_death			death;
	bool			philo_died;
	bool			stop_simulation;
}					t_data;

/* FUNCTIONS */
void				print_dead(t_data *data);
void				print_msg(t_philo *philo, int i);
int					lock_fork(t_philo *philo, int which_one);
void				cleanup_partial_forks(t_philo *philo, int stop,
						int locked_first);
int					thinking_stage(t_philo *philo);
size_t				get_current_time_ms(void);
int					simulation_should_end(t_philo *philo);
int					is_starving(t_philo *philo, long time_to_die);
void				*check_philo_death(void *args);
void				*check_meals(void *args);
void				free_all(t_data *data, int i);
int					create_philosopher_threads(t_data *data);
int					create_monitor_threads(t_data *data, int *index);
void				join_all_threads(t_data *data, int thread_count);
void				destroy_all_mutexes(t_data *data);
t_fork				*init_fork(t_input *input);
void				init_philo_fields(t_philo *philo, t_data *data, int i);
t_philo				*init_philo(t_data *data);
t_mutex				*init_mutex(void);
t_data				*initialization(t_input *input);
int					smart_sleep(t_philo *philo, size_t duration);
void				unlock_forks(t_philo *philo);
int					eating_stage(t_philo *philo);
void				*philo_routine(void *arg);
void				philosophers(t_input *input);
int					ft_atoi(char *str);
void				free_exit(int i);
void				check_arg(t_input *info, int ac);
void				parcing(char *av[], int ac, t_input *info);
int					main(int ac, char *av[]);

#endif
