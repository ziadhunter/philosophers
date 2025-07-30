/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:19:41 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/25 16:59:01 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_dead(t_data *data)
{
	if (!data->death.philo)
		return ;
	pthread_mutex_lock(&data->mutex->printf);
	printf("%ld %d died\n", data->death.time, data->death.philo);
	pthread_mutex_unlock(&data->mutex->printf);
}

void	print_msg(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->mutex->stop_simulatoin);
	pthread_mutex_lock(&philo->mutex->printf);
	if (*(philo->simulation_has_ended))
	{
		pthread_mutex_unlock(&philo->mutex->stop_simulatoin);
		pthread_mutex_unlock(&philo->mutex->printf);
		return ;
	}
	if (i == 1)
		printf("%ld %d is thinking\n", get_current_time_ms() - *(philo->start),
			philo->id);
	else if (i == 2)
		printf("%ld %d has taken a fork\n", get_current_time_ms()
			- *(philo->start), philo->id);
	else if (i == 3)
		printf("%ld %d is eating\n", get_current_time_ms() - *(philo->start),
			philo->id);
	else if (i == 4)
		printf("%ld %d is sleeping\n", get_current_time_ms() - *(philo->start),
			philo->id);
	pthread_mutex_unlock(&philo->mutex->stop_simulatoin);
	pthread_mutex_unlock(&philo->mutex->printf);
}

int	lock_fork(t_philo *philo, int which_one)
{
	if (which_one == 1)
	{
		if (!philo->right_f || simulation_should_end(philo))
			return (1);
		pthread_mutex_lock(&philo->right_f->mutex);
		philo->has_right_fork = true;
		print_msg(philo, 2);
	}
	else
	{
		if (!philo->left_f || simulation_should_end(philo))
			return (1);
		pthread_mutex_lock(&philo->left_f->mutex);
		philo->has_left_fork = true;
		print_msg(philo, 2);
	}
	return (0);
}

void	cleanup_partial_forks(t_philo *philo, int stop, int locked_first)
{
	if (!stop)
		return ;
	if (locked_first == 1 && philo->has_right_fork)
	{
		pthread_mutex_unlock(&philo->right_f->mutex);
		philo->has_right_fork = false;
	}
	else if (locked_first == 2 && philo->has_left_fork)
	{
		pthread_mutex_unlock(&philo->left_f->mutex);
		philo->has_left_fork = false;
	}
}

int	thinking_stage(t_philo *philo)
{
	int	stop;
	int	locked_first;

	stop = 0;
	locked_first = 0;
	print_msg(philo, 1);
	usleep(200);
	if (philo->id % 2 == 1)
	{
		stop = lock_fork(philo, 1);
		locked_first = 1;
		if (!stop)
			stop = lock_fork(philo, 2);
	}
	else
	{
		stop = lock_fork(philo, 2);
		locked_first = 2;
		if (!stop)
			stop = lock_fork(philo, 1);
	}
	cleanup_partial_forks(philo, stop, locked_first);
	return (stop);
}
