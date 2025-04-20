#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct input
{
	int num_philo;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int num_eat;
} t_input;

typedef struct fork
{
	int id;
	pthread_mutex_t mutex;
} t_fork;

typedef struct philo
{
    int id;
	t_fork *right_f;
	t_fork *left_f;
	pthread_t thread;
	int meal_eating;
	struct timeval start;
	struct timeval last_time_eat;
	t_input *input;
} t_philo;


typedef struct data
{
	t_input *input;
	t_philo *philo;
	t_fork *fork;
	pthread_t *thread;
	bool philo_died;
}  t_data;

void philosophers(t_input *input);
void eating_stage(t_philo *philo);

#endif