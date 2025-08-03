/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:33:13 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/03 18:08:03 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	smart_sleep(t_philo *philo, size_t duration)
{
	size_t	start;

	start = get_current_time_ms();
	while (!simulation_should_end(philo) && (get_current_time_ms()
			- start) < duration)
	{
		usleep(100);
	}
	if (simulation_should_end(philo))
		return (1);
	return (0);
}

void	unlock_forks(t_philo *philo)
{
	if (philo->has_right_fork)
	{
		pthread_mutex_unlock(&philo->right_f->mutex);
		philo->has_right_fork = false;
	}
	if (philo->has_left_fork)
	{
		pthread_mutex_unlock(&philo->left_f->mutex);
		philo->has_left_fork = false;
	}
}

int	eating_stage(t_philo *philo)
{
	int	stop;

	lock_forks(philo);
	if (philo->input->num_philo == 1)
		usleep(philo->input->time_to_die * 3000);
	pthread_mutex_lock(&philo->mutex->last_time_eat);
	philo->last_time_eat = get_current_time_ms();
	pthread_mutex_unlock(&philo->mutex->last_time_eat);
	print_msg(philo, 3);
	stop = smart_sleep(philo, philo->input->time_to_eat);
	unlock_forks(philo);
	if (stop)
		return (stop);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meal_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_msg(philo, 4);
	stop = smart_sleep(philo, philo->input->time_to_sleep);
	if (stop)
		return (stop);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		stop;

	stop = 0;
	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->mutex->last_time_eat);
	philo->last_time_eat = get_current_time_ms();
	pthread_mutex_unlock(&philo->mutex->last_time_eat);
	if (philo->id % 2 == 0)
	{
		usleep(200);
	}
	while (!stop)
	{
		stop = eating_stage(philo);
		if (!stop)
			stop = thinking_stage(philo);
	}
	unlock_forks(philo);
	return (NULL);
}

void	philosophers(t_input *input)
{
	t_data	*data;
	int		thread_count;

	data = initialization(input);
	if (!data)
		return ;
	data->start = get_current_time_ms();
	thread_count = data->input->num_philo;
	if (create_philosopher_threads(data) != 0)
	{
		free_all(data, 2);
		return ;
	}
	usleep(100);
	if (create_monitor_threads(data, &thread_count) != 0)
	{
		free_all(data, 2);
		return ;
	}
	join_all_threads(data, thread_count);
	print_dead(data);
	destroy_all_mutexes(data);
	free_all(data, 0);
}

// void	philosophers(t_input *input)
// {
// 	t_data	*data;
// 	int		i;
// 	int		thread_count;

// 	i = 0;
// 	data = initialization(input);
// 	data->start = get_current_time_ms();
// 	while (i < data->input->num_philo)
// 	{
// 		if (pthread_create(&data->thread[i], NULL, &philo_routine,
// 				(void *)&data->philo[i]) != 0)
// 			free_all(data, 2);
// 		i++;
// 	}
// 	usleep(100);
// 	if (pthread_create(&data->thread[i], NULL, &check_philo_death,
// 			(void *)data) != 0)
// 		free_all(data, 2);
// 	i++;
// 	if (data->input->check_meal_eated)
// 	{
// 		if (pthread_create(&data->thread[i], NULL, &check_meals,
// 				(void *)data) != 0)
// 			free_all(data, 2);
// 		i++;
// 	}
// 	thread_count = i;
// 	i = 0;
// 	while (i < thread_count)
// 	{
// 		pthread_join(data->thread[i], NULL);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < data->input->num_philo)
// 	{
// 		pthread_mutex_destroy(&data->philo[i].meal_mutex);
// 		pthread_mutex_destroy(&data->fork[i].mutex);
// 		i++;
// 	}
//     free_all(data, 0);
// }
