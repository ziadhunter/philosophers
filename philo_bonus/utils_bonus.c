/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:19:41 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/03 17:25:10 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	smart_sleep(size_t mls)
{
	size_t	start;

	start = get_current_time_ms();
	while (get_current_time_ms() - start < mls)
		usleep(100);
}

size_t	get_current_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_msg(t_philo *philo, char *msg)
{
	size_t	time;

	time = get_current_time_ms() - philo->start;
	sem_wait(philo->semaphore->printf);
	printf("%ld %d %s\n", time, philo->id, msg);
	sem_post(philo->semaphore->printf);
}

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
