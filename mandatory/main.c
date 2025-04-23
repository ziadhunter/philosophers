#include "philosophers.h"

int ft_atoi(char *str)
{
	long i;
	int num;

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

void free_exit(int i)
{
	if (i == 1)
		printf("the arguments should be like: [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat(optional)]");
	if (i == 2)
		printf("wrong argument!");
	exit(1);
}

void check_arg(t_input *info, int ac)
{
	if (info->num_philo <= 0 || info->time_to_die <= 0 || info->time_to_eat <= 0 || info->time_to_sleep <= 0)
		free_exit(2);
	if (ac == 6 && info->num_eat <= 0)
		free_exit(2);
}

void parcing(char *av[], int ac, t_input *info)
{
	if (ac != 5 && ac != 6)
		free_exit(1);
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
	check_arg(info, ac);
}


int main(int ac, char *av[])
{
	t_input info;

	parcing(av, ac, &info);
	philosophers(&info);
	// printf("number_of_philosophers: %d\ntime_to_die: %d\n time_to_eat:%d\n time_to_sleep: %d\n [number_of_times_each_philosopher_must_eat]: %d\n",
	// 		info.num_philo, info.time_to_die, info.time_to_eat, info.time_to_sleep, info.num_eat);
}
