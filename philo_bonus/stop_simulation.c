/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:12:14 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/25 17:06:21 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	simulation_should_end(t_philo *philo)
{
	int	res;

	pthread_mutex_lock(&philo->mutex->stop_simulatoin);
	res = *(philo->simulation_has_ended);
	pthread_mutex_unlock(&philo->mutex->stop_simulatoin);
	return (res);
}

int	is_starving(t_philo *philo, long time_to_die)
{
	long	current_time;
	long	last_eat;
	int		ret;

	current_time = get_current_time_ms();
	pthread_mutex_lock(&philo->mutex->last_time_eat);
	last_eat = philo->last_time_eat;
	ret = (current_time - last_eat) > time_to_die;
	pthread_mutex_unlock(&philo->mutex->last_time_eat);
	return (ret);
}

void	*check_philo_death(void *args)
{
	t_data	*data;
	int		i;

	data = (t_data *)args;
	while (1)
	{
		i = 0;
		while (i < data->input->num_philo)
		{
			if (is_starving(&data->philo[i], data->input->time_to_die))
			{
				data->death.philo = data->philo[i].id;
				data->death.time = get_current_time_ms() - data->start;
				pthread_mutex_lock(&data->mutex->stop_simulatoin);
				data->stop_simulation = true;
				pthread_mutex_unlock(&data->mutex->stop_simulatoin);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
		if (simulation_should_end(&data->philo[0]))
			return (NULL);
	}
	return (NULL);
}

int	check_all_philos_full(t_data *data)
{
	int	full_count;
	int	i;

	full_count = 0;
	i = 0;
	while (i < data->input->num_philo)
	{
		pthread_mutex_lock(&data->philo[i].meal_mutex);
		if (data->philo[i].meal_eaten >= data->input->num_eat)
			full_count++;
		pthread_mutex_unlock(&data->philo[i].meal_mutex);
		i++;
	}
	return (full_count == data->input->num_philo);
}

void	*check_meals(void *args)
{
	t_data	*data;

	data = (t_data *)args;
	while (1)
	{
		if (check_all_philos_full(data))
		{
			pthread_mutex_lock(&data->mutex->stop_simulatoin);
			data->stop_simulation = true;
			pthread_mutex_unlock(&data->mutex->stop_simulatoin);
			break ;
		}
		if (simulation_should_end(&data->philo[0]))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
