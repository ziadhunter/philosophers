/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:19:00 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/31 20:06:20 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(char *str)
{
	long	i;
	long	num;

	i = 0;
	num = 0;
	if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = (num * 10) + (str[i++] - 48);
		if (num > (long)INT_MAX)
			return (-1);
	}
	if (str[i] != '\0')
		return (-1);
	return (num);
}

void	free_exit(int i)
{
	if (i == 2)
		printf("wrong argument!");
	if (i == 3)
		printf("more than 200 philosophers");
	exit(1);
}

void	check_arg(t_input *info, int ac)
{
	if (info->num_philo <= 0 || info->time_to_die <= 0 || info->time_to_eat <= 0
		|| info->time_to_sleep <= 0)
		free_exit(2);
	if (ac == 6 && info->num_eat <= 0)
		free_exit(2);
	if (info->num_philo > 200)
		free_exit(3);
}

void	parcing(char *av[], int ac, t_input *info)
{
	if (ac != 5 && ac != 6)
		free_exit(2);
	if (ac == 6)
		info->check_meal_eated = true;
	else
		info->check_meal_eated = false;
	info->num_philo = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]);
	info->time_to_eat = ft_atoi(av[3]);
	info->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		info->num_eat = atoi(av[5]);
	else
		info->num_eat = 0;
	check_arg(info, ac);
}

int	main(int ac, char *av[])
{
	t_input	info;

	parcing(av, ac, &info);
	philosophers(&info);
}
