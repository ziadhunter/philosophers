/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:20:19 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/02 18:48:48 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_fork	*init_fork(t_input *input)
{
	t_fork	*fork;
	int		i;

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
	return (fork);
}

void	init_philo_fields(t_philo *philo, t_data *data, int i)
{
	philo->id = i + 1;
	philo->meal_eaten = 0;
	pthread_mutex_init(&philo->meal_mutex, NULL);
	philo->last_time_eat = get_current_time_ms();
	philo->has_left_fork = false;
	philo->has_right_fork = false;
	philo->input = data->input;
	philo->right_f = &data->fork[i];
	philo->mutex = data->mutex;
	philo->start = &data->start;
	philo->start_thinking = 0;
	philo->philo_died = &data->philo_died;
	philo->simulation_has_ended = &data->stop_simulation;
	if (data->input->num_philo == 1)
		philo->left_f = NULL;
	else if (i + 1 < data->input->num_philo)
		philo->left_f = &data->fork[i + 1];
	else
		philo->left_f = &data->fork[0];
}

t_philo	*init_philo(t_data *data)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * data->input->num_philo);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < data->input->num_philo)
	{
		init_philo_fields(&philo[i], data, i);
		i++;
	}
	return (philo);
}

t_mutex	*init_mutex(void)
{
	t_mutex	*mutex;

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
	return (mutex);
}

t_data	*initialization(t_input *input)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	memset(data, 0, sizeof(t_data));
	data->thread = malloc(sizeof(pthread_t) * (input->num_philo + 2));
	ft_bzero(data->thread, sizeof(pthread_t) * (input->num_philo + 2));
	data->mutex = init_mutex();
	data->input = input;
	data->death.philo = 0;
	data->death.time = 0;
	data->philo_died = false;
	data->fork = init_fork(input);
	data->philo = init_philo(data);
	if (data->fork == NULL || data->philo == NULL || data->thread == NULL
		|| data->mutex == NULL)
		free_all(data, 1);
	return (data);
}
