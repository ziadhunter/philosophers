/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:23:03 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/02 21:38:00 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
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

typedef struct sema
{
	sem_t			*printf;
	sem_t			*forks;
	sem_t			*meal_eaten;
}					t_sem;

typedef struct philo
{
	int				id;
	size_t			start;
	int				meal_eaten;
	int				yes;
	pthread_mutex_t	time_check;
	long			last_time_eat;
	t_input			*input;
	t_sem			*semaphore;
}					t_philo;

typedef struct data
{
	t_input			*input;
	t_philo			*philo;
	pid_t			*pids;
	t_sem			*semaphore;
	pthread_t		thread;
	pthread_mutex_t	simulation;
	int				simulation_should_stop;
	int				i;
}					t_data;

/* FUNCTIONS */
void				ft_bzero(void *s, size_t n);
void				smart_sleep(size_t mls);
void				smart_usleep(t_philo *philo);
size_t				get_current_time_ms(void);
void				print_msg(t_philo *philo, char *msg);
void				kill_all_processes(pid_t *philo_pids, int num_philos);
void				*death_checker(void *args);
void				*check_meals(void *args);
int					create_monitor(t_data *data);
void				join_monitor(t_data *data);
void				take_forks(t_philo *philo);
void				drop_forks(t_philo *philo);
void				eating_stage(t_philo *philo);
void				philo_routine(t_data *data, int i);
void				create_philo(t_data *data);
void				free_all(char *message, t_data *data, int exit_status,
						int i);
t_sem				*init_semaphores(t_input *input);
t_philo				*init_philo(t_input *input, t_sem *semaphore);
t_data				*initialize_all(t_input *input);
void				philosophers(t_input *input);
int					ft_atoi(char *str);
void				free_exit(int i);
void				check_arg(t_input *info, int ac);
void				parcing(char *av[], int ac, t_input *info);
int					main(int ac, char *av[]);

#endif
