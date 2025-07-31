/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:23:03 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/31 22:03:07 by zfarouk          ###   ########.fr       */
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
# include <semaphore.h> 
# include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

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
	sem_t	*printf;
	sem_t	*forks;
	sem_t	*philo_died;
	sem_t	*meal_eaten;
}	t_sem;

typedef struct philo
{
	int					id;
	size_t				start;
	int					meal_eaten;
	pthread_mutex_t		*meal_check;
	long				last_time_eat;
	pthread_mutex_t		time_check;
	t_input				*input;
	t_sem				*semaphore;
}					t_philo;

typedef struct data
{
	t_input *input;
	t_philo *philo;
	pid_t *pids;
	t_sem *semaphore;
	pthread_t thread;
	bool	simulation_should_stop;
	pthread_mutex_t should_stop;
} t_data;

/* FUNCTIONS */


#endif
