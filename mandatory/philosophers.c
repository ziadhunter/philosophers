#include "philosophers.h"

t_fork *init_fork(t_input *input)
{
    t_fork *fork;
    int i;

    fork = malloc(sizeof(t_fork) * input->num_philo);
    if (!fork)
        return (NULL);
    i = 0;
    while (i < input->num_philo)
    {
        fork[i].id = i + 1;
        if (pthread_mutex_init(&fork[i].mutex, NULL) != 0)
            return (NULL);
        i++; 
    }
    return(fork);
}

t_philo *init_philo(t_data *data)
{
    t_philo *philo;
    int i;
    
    philo = malloc(sizeof(t_philo) * data->input->num_philo);
    if (!philo)
        return (NULL);
    i = 0;
    while (i < data->input->num_philo)
    {
        philo[i].id = i + 1;
        philo[i].meal_eating = 0;
        philo[i].input = data->input;
        philo[i].right_f = &data->fork[i];
        if (i + 1 < data->input->num_philo)
            philo[i].left_f = &data->fork[i + 1];
        else
            philo[i].left_f = &data->fork[0];
        i++;             
    }
    return (philo);
}

void free_failed(t_data *data, int i)
{
    if (data->fork != NULL)
        free (data->fork);
    if (data->philo != NULL)
        free (data->philo);
    if (data->thread != NULL)
        free (data->thread);
    free(data);
    if (i == 1)
        printf("malloc or initialization of the mutex failed");
    if (i == 2)
        printf("Failed to create thread");
    exit(3);
}

t_data *initialization(t_input *input)
{
    t_data *data;
    
    data =  malloc(sizeof(t_data));
    if (!data)
        return (NULL);
    data->thread = malloc(sizeof(pthread_t) * input->num_philo);
    data->input = input;
    data->fork = init_fork(input);
    data->philo = init_philo(data);
    if (data->fork == NULL|| data->philo == NULL || data->thread == NULL)
        free_failed(data, 1);
    return (data);
}

void is_starving(void *arg)
{
    
}

void thinking_stage(t_philo *philo)
{
    pthread_t thread;

    printf(" %d is thinking\n", philo->id);
    if (philo->id % 2 == 1) {
        pthread_mutex_lock(&philo->left_f->mutex);
        printf("%d has taken a fork\n", philo->id);
        pthread_mutex_lock(&philo->right_f->mutex);
        printf("%d has taken a fork\n", philo->id);
    } else {
        pthread_mutex_lock(&philo->right_f->mutex);
        printf("%d has taken a fork\n", philo->id);
        pthread_mutex_lock(&philo->left_f->mutex);
        printf("%d has taken a fork\n", philo->id);
    }
}

void eating_stage(t_philo *philo)
{
    printf(" %d is eating\n", philo->id);
    gettimeofday(&philo->last_time_eat, NULL);
    usleep(philo->input->time_to_eat);
    // Release fork
    pthread_mutex_unlock(&philo->left_f->mutex);
    pthread_mutex_unlock(&philo->right_f->mutex);
    
    printf(" %d is sleeping\n", philo->id);
    usleep(philo->input->time_to_sleep);
}

void *philo_routine(void *arg)
{
    t_philo *philo;
    philo = (t_philo *)arg;
    gettimeofday(&philo->start, NULL);
    gettimeofday(&philo->last_time_eat, NULL);
    thinking_stage(philo);
}

void philosophers(t_input *input)
{
    t_data *data;
    int i;
    pthread_t *thread;
    
    i = 0;
    data = initialization(input);
    while (i < data->input->num_philo)
    {
        if (pthread_create(&data->thread[i], NULL, &philo_routine, (void *)&data->philo[i]) != 0)
                free_failed(data, 2);
        i++;
    }
    i = 0;
    while (i < data->input->num_philo) {
        pthread_join(data->thread[i], NULL);
        i++;
    }
    i = 0;
    while (i < data->input->num_philo) {
        pthread_mutex_destroy(&data->fork[i].mutex);
        i++;
    }
    free(data->thread);
    free(data->fork);
    free(data->philo);
    free(data);
}
