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
            pthread_mutex_lock(&data->mutex->meal_eaten);
            if (data->philo[i].meal_eaten >= data->input->num_eat)
                full_count++;
            pthread_mutex_unlock(&data->mutex->meal_eaten);
            i++;
        }
        if (full_count == data->input->num_philo)
        {
            pthread_mutex_lock(&data->mutex->stop_simulatoin);
            data->stop_simulation = true;
            pthread_mutex_unlock(&data->mutex->stop_simulatoin);
            break;
        }
        if (simulation_should_end(&data->philo[1]))
			return NULL;
        usleep(1000);
    }
    return (NULL);
}


long get_current_time_ms()
{
    struct timeval time;
    
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
        if (simulation_should_end(&data->philo[1]))
            return NULL;
    }
    return NULL;
}
