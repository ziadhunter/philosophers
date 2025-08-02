/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simulation_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:12:14 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/02 21:37:39 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

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

void	*death_checker(void *args)
{
	t_philo	*philo;
	t_data	*data;

	data = (t_data *)args;
	philo = &data->philo[data->i];
	while (1)
	{
		pthread_mutex_lock(&philo->time_check);
		if ((long)get_current_time_ms()
			- philo->last_time_eat > philo->input->time_to_die)
		{
			print_msg(philo, "died");
			pthread_mutex_unlock(&philo->time_check);
			sem_wait(philo->semaphore->printf);
			exit(EXIT_SUCCESS);
		}
		pthread_mutex_unlock(&philo->time_check);
		usleep(100);
	}
	return (NULL);
}

void	*check_meals(void *args)
{
	t_data	*data;
	int		counter;

	counter = 0;
	data = (t_data *)args;
	while (1)
	{
		sem_wait(data->semaphore->meal_eaten);
		counter++;
		if (counter == data->input->num_philo)
		{
			pthread_mutex_lock(&data->simulation);
			if (data->simulation_should_stop == 1)
			{
				pthread_mutex_unlock(&data->simulation);
				return (NULL);
			}
			pthread_mutex_unlock(&data->simulation);
			kill_all_processes(data->pids, data->input->num_philo);
			return (NULL);
		}
	}
	return (NULL);
}

int	create_monitor(t_data *data)
{
	if (data->input->check_meal_eated)
	{
		if (pthread_create(&data->thread, NULL, &check_meals,
				(void *)data) != 0)
		{
			kill_all_processes(data->pids, data->input->num_philo);
			free_all("failed to creat thread", data, 1, 0);
		}
	}
	return (0);
}

void	join_monitor(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->simulation);
	data->simulation_should_stop = 1;
	pthread_mutex_unlock(&data->simulation);
	while (i < data->input->num_philo)
	{
		sem_post(data->semaphore->meal_eaten);
		i++;
	}
	pthread_join(data->thread, NULL);
}
