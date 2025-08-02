/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:33:13 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/02 21:37:35 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	take_forks(t_philo *philo)
{
	sem_wait(philo->semaphore->forks);
	print_msg(philo, "has taken a fork");
	sem_wait(philo->semaphore->forks);
	print_msg(philo, "has taken a fork");
}

void	drop_forks(t_philo *philo)
{
	sem_post(philo->semaphore->forks);
	sem_post(philo->semaphore->forks);
}

void	eating_stage(t_philo *philo)
{
	pthread_mutex_lock(&philo->time_check);
	philo->last_time_eat = get_current_time_ms();
	pthread_mutex_unlock(&philo->time_check);
	print_msg(philo, "is eating");
	smart_sleep(philo->input->time_to_eat);
	drop_forks(philo);
	philo->meal_eaten++;
	if (philo->input->num_eat && philo->meal_eaten >= philo->input->num_eat
		&& !philo->yes)
	{
		sem_post(philo->semaphore->meal_eaten);
		philo->yes = 1;
	}
}

void	philo_routine(t_data *data, int i)
{
	pthread_t	monitor;
	t_philo		*philo;

	philo = &data->philo[i];
	data->i = i;
	philo->start = get_current_time_ms();
	philo->last_time_eat = get_current_time_ms();
	pthread_create(&monitor, NULL, death_checker, data);
	pthread_detach(monitor);
	if (philo->id % 2 == 0)
		usleep(200);
	while (1)
	{
		take_forks(philo);
		eating_stage(philo);
		print_msg(philo, "is sleeping");
		smart_sleep(philo->input->time_to_sleep);
		print_msg(philo, "is thinking");
		smart_usleep(philo);
	}
}

void	create_philo(t_data *data)
{
	int	i;
	int	status;

	status = 0;
	i = 0;
	while (i < data->input->num_philo)
	{
		data->pids[i] = fork();
		if (data->pids[i] < 0)
		{
			kill_all_processes(data->pids, data->input->num_philo);
			free_all("fork failed\n", data, 1, 0);
		}
		else if (data->pids[i] == 0)
			philo_routine(data, i);
		i++;
	}
	create_monitor(data);
	waitpid(-1, &status, 0);
	join_monitor(data);
	if (WEXITSTATUS(status) == EXIT_SUCCESS)
		kill_all_processes(data->pids, data->input->num_philo);
	free_all(NULL, data, 0, 0);
}
