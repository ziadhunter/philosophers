/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:10:40 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/02 18:51:27 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_all(t_data *data, int i)
{
	if (data->fork != NULL)
		free(data->fork);
	if (data->philo != NULL)
		free(data->philo);
	if (data->thread != NULL)
		free(data->thread);
	if (data->mutex != NULL)
		free(data->mutex);
	free(data);
	if (i == 1)
		printf("malloc or initialization of the mutex failed");
	if (i == 2)
		printf("Failed to create thread");
	if (i != 0)
		exit(1);
}

int	create_philosopher_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->input->num_philo)
	{
		if (pthread_create(&data->thread[i], NULL, &philo_routine,
				(void *)&data->philo[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	create_monitor_threads(t_data *data, int *index)
{
	if (pthread_create(&data->thread[*index], NULL, &check_philo_death,
			(void *)data) != 0)
		return (1);
	(*index)++;
	if (data->input->check_meal_eated)
	{
		if (pthread_create(&data->thread[*index], NULL, &check_meals,
				(void *)data) != 0)
			return (1);
		(*index)++;
	}
	return (0);
}

void	join_all_threads(t_data *data, int thread_count)
{
	int	i;

	i = 0;
	while (i < thread_count)
	{
		pthread_join(data->thread[i], NULL);
		i++;
	}
}

void	destroy_all_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->input->num_philo)
	{
		pthread_mutex_destroy(&data->philo[i].meal_mutex);
		pthread_mutex_destroy(&data->fork[i].mutex);
		i++;
	}
}
