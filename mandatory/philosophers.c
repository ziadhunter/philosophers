#include "philosophers.h"

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

void *check_meals(void *args)
{
    int full_count;
    int i;
    t_data *data;

    data = (t_data *)args;
    while (1)
    {
        i = 0;
        full_count = 0;
        while (i < data->input->num_philo)
        {
            pthread_mutex_lock(&data->philo[i].meal_mutex);
            if (data->philo[i].meal_eaten >= data->input->num_eat)
               full_count++;
            pthread_mutex_unlock(&data->philo[i].meal_mutex);
            i++;
        }
        if (full_count == data->input->num_philo)
        {
            pthread_mutex_lock(&data->mutex->stop_simulatoin);
            data->stop_simulation = true;
            pthread_mutex_unlock(&data->mutex->stop_simulatoin);
            break;
        }
        if (simulation_should_end(&data->philo[0]))
			return NULL;
        usleep(1000);
    }
    return (NULL);
}


long get_current_time_ms()
{
    struct timeval time;
    
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}


int is_starving(t_philo *philo, long time_to_die)
{
    long current_time;
    long last_eat;
    int ret;

    current_time = get_current_time_ms();
    pthread_mutex_lock(&philo->mutex->last_time_eat);
    last_eat = philo->last_time_eat;
    ret = (current_time - last_eat) > time_to_die;
    pthread_mutex_unlock(&philo->mutex->last_time_eat);
    return ret;
}



void *check_philo_death(void *args)
{
    t_data *data = (t_data *)args;
    int i;
    
    while (1)
    {
        i = 0;
        while (i < data->input->num_philo)
        {
            if (is_starving(&data->philo[i], data->input->time_to_die))
            {
                pthread_mutex_lock(&data->mutex->printf);
                printf("%ld %d died\n", 
                       get_current_time_ms() - data->start,
                       data->philo[i].id);
                pthread_mutex_unlock(&data->mutex->printf);
                pthread_mutex_lock(&data->mutex->stop_simulatoin);
                data->stop_simulation = true;
                pthread_mutex_unlock(&data->mutex->stop_simulatoin);
                return NULL;
            }
            i++;
        }
        usleep(1000);
        if (simulation_should_end(&data->philo[0]))
            return NULL;
    }
    return NULL;
}

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
        {
            
            return (NULL);
        }
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
        pthread_mutex_init(&philo[i].meal_mutex, NULL);
        philo[i].input = data->input;
        philo[i].right_f = &data->fork[i];
        philo[i].mutex = data->mutex;
        philo[i].start = &data->start;
        philo[i].start_thinking = 0;
        philo[i].philo_died = &data->philo_died;
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
    exit(1);
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
    if (pthread_mutex_init(&mutex->start_thinking, NULL) != 0)
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



void print_msg(t_philo *philo, int i)
{
    pthread_mutex_lock(&philo->mutex->printf);
    pthread_mutex_lock(&philo->mutex->stop_simulatoin);
    if (*(philo->simulation_has_ended))
    {
        pthread_mutex_unlock(&philo->mutex->stop_simulatoin);
        pthread_mutex_unlock(&philo->mutex->printf);
        return;
    }
    if (i == 1)
        printf("%ld %d is thinking\n",get_current_time_ms() - *(philo->start), philo->id);
    else if (i == 2)
        printf("%ld %d has taken a fork\n",get_current_time_ms() - *(philo->start), philo->id);
    else if (i == 3)
        printf("%ld %d is eating\n",get_current_time_ms() - *(philo->start), philo->id);
    else if (i == 4)
        printf("%ld %d is sleeping\n", get_current_time_ms() - *(philo->start), philo->id);
    pthread_mutex_unlock(&philo->mutex->stop_simulatoin);
    pthread_mutex_unlock(&philo->mutex->printf);
}

int lock_fork(t_philo *philo, int which_one)
{
    if (which_one == 1)
    {
        if (!simulation_should_end(philo))
        {
            pthread_mutex_lock(&philo->right_f->mutex);
            print_msg(philo, 2);
        }
        else
            return (1);
    }
    else
    {
        if (!simulation_should_end(philo))
        {
            pthread_mutex_lock(&philo->left_f->mutex);
            print_msg(philo, 2);
        }
        else
            return (1);
    }
    return (0);
}

int thinking_stage(t_philo *philo)
{
    pthread_t thread;
    int stop;

    stop = 0;
    print_msg(philo, 1);
    if (philo->id % 2 == 1) 
    {
        // usleep(philo->input->time_to_eat * 2 / 10);
        stop = lock_fork(philo, 2);
        if (!stop)
            stop = lock_fork(philo, 1);
    } else {
        usleep(500);
        stop = lock_fork(philo, 1);
        if (!stop)
            stop = lock_fork(philo, 2);
    }
    return (stop);
}

int smart_eat_sleep(t_philo *philo)
{
    long start = get_current_time_ms();

    while ((get_current_time_ms() - start) < philo->input->time_to_eat)
    {
        if (simulation_should_end(philo))
            return (1);
        usleep(100);
    }
    return (0);
}

int smart_sleep(t_philo *philo)
{
    long start_time = get_current_time_ms();

    while ((get_current_time_ms() - start_time) < philo->input->time_to_sleep)
    {
        if (simulation_should_end(philo))
            return (1);
        usleep(100);
    }
    return (0);
}


// int smart_eat_sleep(t_philo *philo)
// {
//     pthread_mutex_lock(&philo->mutex->last_time_eat);
//     while (get_current_time_ms() - philo->last_time_eat < philo->input->time_to_eat)
//     {
//         pthread_mutex_unlock(&philo->mutex->last_time_eat);
//         usleep(100);
//         pthread_mutex_lock(&philo->mutex->last_time_eat);
//     }
//     pthread_mutex_unlock(&philo->mutex->last_time_eat);
//     if (simulation_should_end(philo))
//         return (1);
//     return (0);
// }

// int smart_sleep(t_philo *philo)
// {
//     long start_time = get_current_time_ms();
//     pthread_mutex_lock(&philo->mutex->last_time_eat);
//     while (get_current_time_ms() - start_time < philo->input->time_to_sleep)
//     {
//         pthread_mutex_unlock(&philo->mutex->last_time_eat);
//         usleep(100);
//         pthread_mutex_lock(&philo->mutex->last_time_eat);
//     }
//     pthread_mutex_unlock(&philo->mutex->last_time_eat);
//     if (simulation_should_end(philo))
//         return (1);
//     return (0);
// }

void unlock_forks(t_philo *philo)
{
    if (philo->id % 2 == 1)
    {
        pthread_mutex_unlock(&philo->left_f->mutex);
        pthread_mutex_unlock(&philo->right_f->mutex);
    }
    else
    {
        pthread_mutex_unlock(&philo->right_f->mutex);
        pthread_mutex_unlock(&philo->left_f->mutex);
    }
}

int eating_stage(t_philo *philo)
{
    int stop;

    pthread_mutex_lock(&philo->mutex->last_time_eat);
    philo->last_time_eat = get_current_time_ms();
    pthread_mutex_unlock(&philo->mutex->last_time_eat); 
    print_msg(philo, 3);
    stop = smart_eat_sleep(philo);
    unlock_forks(philo);
    if (stop)
        return (stop);
    pthread_mutex_lock(&philo->meal_mutex);
    philo->meal_eaten++;
    pthread_mutex_unlock(&philo->meal_mutex);
    print_msg(philo, 4);
    stop = smart_sleep(philo);
    if (stop)
        return (stop);
    return (0);
}

void *philo_routine(void *arg)
{
    t_philo *philo;
    int stop;

    stop = 0;
    philo = (t_philo *)arg;
    pthread_mutex_lock(&philo->mutex->last_time_eat);
    philo->last_time_eat = get_current_time_ms();
    pthread_mutex_unlock(&philo->mutex->last_time_eat);
    while (!stop) {
        stop = thinking_stage(philo);
        if (!stop)
            stop = eating_stage(philo);
    }
    return (NULL);
}


void philosophers(t_input *input)
{
    t_data *data;
    int i;
    int thread_count;
    
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

    if (pthread_create(&data->thread[i], NULL, &check_philo_death, (void *)data) != 0)
        free_failed(data, 2);
    i++;

    if (data->input->check_meal_eated)
    {
        if (pthread_create(&data->thread[i], NULL, &check_meals, (void *)data) != 0)
            free_failed(data, 2);
        i++;
    }

    thread_count = i;

    i = 0;
    while (i < thread_count)
    {
        pthread_join(data->thread[i], NULL);
        i++;
    }

    i = 0;
    while (i < data->input->num_philo)
    {
        pthread_mutex_destroy(&data->philo[i].meal_mutex);
        pthread_mutex_destroy(&data->fork[i].mutex);
        i++;
    }
    free(data->thread);
    free(data->fork);
    free(data->philo);
    free(data);
}








// void philosophers(t_input *input)
// {
//     t_data *data;
//     int i;
    
//     i = 0;
//     data = initialization(input);
//     data->start = get_current_time_ms();
//     while (i < data->input->num_philo)
//     {
//         if (pthread_create(&data->thread[i], NULL, &philo_routine, (void *)&data->philo[i]) != 0)
//                 free_failed(data, 2);
//         i++;
//     }
//     usleep(100);
//     // thread that will check if any philo dies

//     if (pthread_create(&data->thread[i], NULL, &check_philo_death, (void *)data) != 0)
//         free_failed(data, 2);

//     // thread that will check if all philosophers has eaten their meals
//     if (data->input->check_meal_eated)
//         if (pthread_create(&data->thread[++i], NULL, &check_meals, (void *)data) != 0)
//             free_failed(data, 2);

//     i = 0;
//     while (i < data->input->num_philo + 2) {
//         pthread_join(data->thread[i], NULL);
//         i++;
//     }
//     i = 0;
//     while (i < data->input->num_philo) {
//         pthread_mutex_destroy(&data->fork[i].mutex);
//         i++;
//     }
//     free(data->thread);
//     free(data->fork);
//     free(data->philo);
//     free(data);
// }
