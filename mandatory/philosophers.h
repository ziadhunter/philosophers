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
	bool check_meal_eated;
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

typedef struct mutex
{
	pthread_mutex_t printf;
	pthread_mutex_t stop_simulatoin;
	pthread_mutex_t philo_died;
	pthread_mutex_t start;
	pthread_mutex_t last_time_eat;
	pthread_mutex_t meal_eaten;
} t_mutex;

typedef struct philo
{
    int id;
	t_fork *right_f;
	t_fork *left_f;
	long *start;
	pthread_t thread;
	int meal_eaten;
	long last_time_eat;
	t_input *input;
	bool *simulation_has_ended;
	t_mutex *mutex;
} t_philo;

typedef struct data
{
	long start;
	t_input *input;
	t_philo *philo;
	t_fork *fork;
	t_mutex *mutex;
	pthread_t *thread;
	bool philo_died;
	bool stop_simulation;
}  t_data;

void philosophers(t_input *input);
void eating_stage(t_philo *philo);
void *check_meals(void *args);
void *check_philo_death(void *args);
long get_current_time_ms();

#endif