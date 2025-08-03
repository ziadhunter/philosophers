/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:20:19 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/03 17:17:45 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

t_sem	*init_semaphores(t_input *input)
{
	t_sem	*sema;

	sema = malloc(sizeof(t_sem));
	if (!sema)
		return (NULL);
	sem_unlink("/forks");
	sem_unlink("/printf");
	sem_unlink("/meals");
	sem_unlink("/death");
	sema->forks = sem_open("/forks", O_CREAT, 0644, input->num_philo);
	sema->printf = sem_open("/printf", O_CREAT, 0644, 1);
	sema->meal_eaten = sem_open("/meals", O_CREAT, 0644, 0);
	sema->death = sem_open("/death", O_CREAT, 0644, 0);
	if (sema->forks == SEM_FAILED || sema->meal_eaten == SEM_FAILED
		|| sema->printf == SEM_FAILED || sema->death == SEM_FAILED)
		return (NULL);
	return (sema);
}

t_philo	*init_philo(t_input *input, t_sem *semaphore)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = malloc(sizeof(t_philo) * input->num_philo);
	if (!philo)
		return (NULL);
	while (i < input->num_philo)
	{
		philo[i].id = i + 1;
		philo[i].start = 0;
		philo[i].input = input;
		philo[i].meal_eaten = 0;
		philo[i].last_time_eat = get_current_time_ms();
		pthread_mutex_init(&philo[i].time_check, NULL);
		philo[i].semaphore = semaphore;
		philo[i].yes = 0;
		philo[i].stop_simulation = 0;
		pthread_mutex_init(&philo[i].stop, NULL);
		i++;
	}
	return (philo);
}

t_data	*initialize_all(t_input *input)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
	{
		printf("malloc failed\n");
		exit(1);
	}
	data->semaphore = init_semaphores(input);
	if (!data->semaphore)
		free_all("error open semaphore\n", data, 1, 0);
	data->philo = init_philo(input, data->semaphore);
	if (!data->philo)
		free_all("malloc failed\n", data, 1, 0);
	data->pids = malloc(input->num_philo * sizeof(pid_t));
	if (!data->pids)
		free_all("malloc failed\n", data, 1, 0);
	ft_bzero(data->pids, sizeof(pid_t) * input->num_philo);
	pthread_mutex_init(&data->simulation, NULL);
	data->simulation_should_stop = 0;
	data->input = input;
	data->thread[0] = 0;
	data->thread[1] = 0;
	data->i = 0;
	return (data);
}
