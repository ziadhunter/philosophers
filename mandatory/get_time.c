/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:05:35 by zfarouk           #+#    #+#             */
/*   Updated: 2025/08/02 18:36:09 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	get_current_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	lock_forks(t_philo *philo)
{
	int	stop;
	int	locked_first;

	stop = 0;
	locked_first = 0;
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

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;
	size_t			i;

	p = (unsigned char *)s;
	i = 0;
	while (i < n)
		p[i++] = 0;
}
