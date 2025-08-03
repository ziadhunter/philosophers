/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 17:23:06 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/03 17:25:16 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;
	size_t			i;

	p = (unsigned char *)s;
	i = 0;
	while (i < n)
		p[i++] = 0;
}

void	kill_all_processes(pid_t *philo_pids, int num_philos)
{
	int	i;

	i = 0;
	while (i < num_philos && philo_pids[i] > 0)
	{
		kill(philo_pids[i], SIGKILL);
		i++;
	}
}

void	free_philo(t_data *data, int i)
{
	while (i < data->input->num_philo)
	{
		pthread_mutex_destroy(&data->philo[i].stop);
		pthread_mutex_destroy(&data->philo[i++].time_check);
	}
	free(data->philo);
}

void	free_all(char *message, t_data *data, int exit_status, int i)
{
	if (data->semaphore)
	{
		if (data->semaphore->forks != SEM_FAILED)
			sem_close(data->semaphore->forks);
		if (data->semaphore->printf != SEM_FAILED)
			sem_close(data->semaphore->printf);
		if (data->semaphore->meal_eaten != SEM_FAILED)
			sem_close(data->semaphore->meal_eaten);
		if (data->semaphore->death != SEM_FAILED)
			sem_close(data->semaphore->death);
	}
	free(data->semaphore);
	if (data->philo)
		free_philo(data, i);
	pthread_mutex_destroy(&data->simulation);
	if (data->pids)
		free(data->pids);
	free(data);
	if (message)
		printf("%s", message);
	exit(exit_status);
}
