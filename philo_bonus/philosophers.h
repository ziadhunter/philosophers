/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:23:03 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/30 19:34:43 by zfarouk          ###   ########.fr       */
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

typedef struct input
{
	bool			check_meal_eated;
	int				num_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_eat;
}					t_input;

typedef struct philo
{
	int				id;
	size_t			start;
	int				meal_eaten;
	sem_t			*meal_check;
	long			last_time_eat;
	t_input			*input;
	t_sem			*semaphore;
}					t_philo;

typedef struct sema
{
	sem_t	*printf;
	sem_t	*forks;
}	t_sem;

typedef struct data
{
	t_input *input;
	t_philo *philo;
	t_sem *semaphore;
} t_data;

/* FUNCTIONS */


#endif
