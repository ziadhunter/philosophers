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
	long time_to_die;
	long time_to_eat;
	long time_to_sleep;
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
	pthread_mutex_t start_thinking;
} t_mutex;

typedef struct philo
{
    int id;
	t_fork *right_f;
	t_fork *left_f;
	long *start;
	pthread_t thread;
	int meal_eaten;
	pthread_mutex_t     meal_mutex;
	long last_time_eat;
	t_input *input;
	bool *simulation_has_ended;
	bool *philo_died;
	t_mutex *mutex;
	long start_thinking;
} t_philo;

typedef struct data
{
	long start;
	t_input *input;
	t_philo *philo;
	t_fork *fork;
	t_mutex	*mutex;
	pthread_t *thread;
	bool philo_died;
	bool stop_simulation;
}  t_data;

int simulation_should_end(t_philo *philo);
void philosophers(t_input *input);
int	 eating_stage(t_philo *philo);
void *check_meals(void *args);
void *check_philo_death(void *args);
long get_current_time_ms();

#endif
