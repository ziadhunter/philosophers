/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:19:41 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/02 21:37:44 by zfarouk          ###   ########.fr       */
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

void	smart_sleep(size_t mls)
{
	size_t	start;

	start = get_current_time_ms();
	while (get_current_time_ms() - start < mls)
		usleep(100);
}

void	smart_usleep(t_philo *philo)
{
	size_t	i;
	int		die;

	die = philo->input->time_to_die;
	pthread_mutex_lock(&philo->time_check);
	i = (size_t)((die - (get_current_time_ms() - philo->last_time_eat)) * 0.7);
	pthread_mutex_unlock(&philo->time_check);
	smart_sleep(i);
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
