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
        philo[i].meal_eaten = 0;
        philo[i].input = data->input;
        philo[i].right_f = &data->fork[i];
        philo[i].mutex = data->mutex;
        philo[i].start = &data->start;
        philo[i].simulation_has_ended = &data->stop_simulation;
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

t_mutex *init_mutex()
{
    t_mutex *mutex;
    int i;

    mutex = malloc(sizeof(t_mutex));
    if (pthread_mutex_init(&mutex->stop_simulatoin, NULL) != 0)
        return (NULL);
    if (pthread_mutex_init(&mutex->printf, NULL) != 0)
        return (NULL);
    if (pthread_mutex_init(&mutex->philo_died, NULL) != 0)
        return (NULL);
    if (pthread_mutex_init(&mutex->start, NULL) != 0)
        return (NULL);
    if (pthread_mutex_init(&mutex->last_time_eat, NULL) != 0)
        return (NULL);
    if (pthread_mutex_init(&mutex->meal_eaten, NULL) != 0)
        return (NULL);
    return(mutex);
}

t_data *initialization(t_input *input)
{
    t_data *data;
    
    data =  malloc(sizeof(t_data));
    if (!data)
        return (NULL);
    data->thread = malloc(sizeof(pthread_t) * (input->num_philo + 2));
    data->mutex = init_mutex();
    data->input = input;
    data->philo_died = false;
    data->fork = init_fork(input);
    data->philo = init_philo(data);
    if (data->fork == NULL|| data->philo == NULL || data->thread == NULL || data->mutex == NULL)
        free_failed(data, 1);
    return (data);
}

int simulation_should_end(t_philo *philo)
{
    pthread_mutex_lock(&philo->mutex->stop_simulatoin);
    if (*(philo->simulation_has_ended))
    {
        pthread_mutex_unlock(&philo->mutex->stop_simulatoin);
        return (1);
    }
    pthread_mutex_unlock(&philo->mutex->stop_simulatoin);
    return (0);
}

void print_msg(t_philo *philo, int i)
{
    if (*(philo->simulation_has_ended))
        return;
    pthread_mutex_lock(&philo->mutex->printf);
    if (i == 1)
        printf("%ld %d is thinking\n",get_current_time_ms() - *(philo->start), philo->id);
    else if (i == 2)
        printf("%ld %d has taken a fork\n",get_current_time_ms() - *(philo->start), philo->id);
    else if (i == 3)
        printf("%ld %d is eating\n",get_current_time_ms() - *(philo->start), philo->id);
    else if (i == 4)
        printf("%ld %d is sleeping\n", get_current_time_ms() - *(philo->start), philo->id);
    pthread_mutex_unlock(&philo->mutex->printf);
}


void thinking_stage(t_philo *philo)
{
    pthread_t thread;

    print_msg(philo, 1);
    // pthread_mutex_lock(&philo->mutex->printf);
    // printf("%ld %d is thinking\n",get_current_time_ms() - *(philo->start), philo->id);
    // pthread_mutex_unlock(&philo->mutex->printf);
    if (philo->id % 2 == 1) {
        pthread_mutex_lock(&philo->left_f->mutex);
        print_msg(philo, 2);

        // pthread_mutex_lock(&philo->mutex->printf);
        // printf("%ld %d has taken a fork\n",get_current_time_ms() - *(philo->start), philo->id);
        // pthread_mutex_unlock(&philo->mutex->printf);
        // if (simulation_should_end(philo))
        // {
        //     pthread_mutex_unlock(&philo->left_f->mutex);
        //     return;
        // }
        pthread_mutex_lock(&philo->right_f->mutex);
        print_msg(philo, 2);
        // pthread_mutex_lock(&philo->mutex->printf);
        // printf("%ld %d has taken a fork\n",get_current_time_ms() - *(philo->start), philo->id);
        // pthread_mutex_unlock(&philo->mutex->printf);
    } else {
        pthread_mutex_lock(&philo->right_f->mutex);
        print_msg(philo, 2);
        // pthread_mutex_lock(&philo->mutex->printf);
        // printf("%ld %d has taken a fork\n", get_current_time_ms() - *(philo->start), philo->id);
        // pthread_mutex_unlock(&philo->mutex->printf);
        // if (simulation_should_end(philo))
        // {
        //     pthread_mutex_unlock(&philo->left_f->mutex);
        //     return;
        // }
        pthread_mutex_lock(&philo->left_f->mutex);
        print_msg(philo, 2);
        // pthread_mutex_lock(&philo->mutex->printf);
        // printf("%ld %d has taken a fork\n",get_current_time_ms() - *(philo->start), philo->id);
        // pthread_mutex_unlock(&philo->mutex->printf);
    }
}

void eating_stage(t_philo *philo)
{
    // pthread_mutex_lock(&philo->mutex->printf);
    // printf("%ld %d is eating\n",get_current_time_ms() - *(philo->start), philo->id);
    // pthread_mutex_unlock(&philo->mutex->printf);
    print_msg(philo, 3);
    pthread_mutex_lock(&philo->mutex->last_time_eat);
    philo->last_time_eat = get_current_time_ms();
    pthread_mutex_unlock(&philo->mutex->last_time_eat); 
    usleep(philo->input->time_to_eat * 1000);
    // release fork
    pthread_mutex_unlock(&philo->left_f->mutex);
    pthread_mutex_unlock(&philo->right_f->mutex);
    philo->meal_eaten++;
    print_msg(philo, 3);
    // pthread_mutex_lock(&philo->mutex->printf);
    // printf("%ld %d is sleeping\n", get_current_time_ms() - *(philo->start), philo->id);
    // pthread_mutex_unlock(&philo->mutex->printf);
    usleep(philo->input->time_to_sleep * 1000);
}


void *philo_routine(void *arg)
{
    t_philo *philo;
    philo = (t_philo *)arg;
    pthread_mutex_lock(&philo->mutex->last_time_eat);
    philo->last_time_eat = get_current_time_ms();
    pthread_mutex_unlock(&philo->mutex->last_time_eat);
    // if (philo->id % 2 == 0)
    //     usleep(100);
    while (!simulation_should_end(philo)) {
        thinking_stage(philo);
        eating_stage(philo);
    }
    return (NULL);
}

void philosophers(t_input *input)
{
    t_data *data;
    int i;
    
    i = 0;
    data = initialization(input);
    data->start = get_current_time_ms();
    while (i < data->input->num_philo)
    {
        if (pthread_create(&data->thread[i], NULL, &philo_routine, (void *)&data->philo[i]) != 0)
                free_failed(data, 2);
        i++;
    }
    usleep(100);
    // thread that will check if any philo dies

    if (pthread_create(&data->thread[i], NULL, &check_philo_death, (void *)data) != 0)
        free_failed(data, 2);

    // thread that will check if all philosophers has eaten their meals
    if (data->input->check_meal_eated)
        if (pthread_create(&data->thread[i++], NULL, &check_meals, (void *)data) != 0)
            free_failed(data, 2);

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