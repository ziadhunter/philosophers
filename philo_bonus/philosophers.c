/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:33:13 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/30 19:45:37 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	get_current_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
void free_exit(char *message, t_data *data, int is_parent, int exit_status)
{
	if (is_parent)
		//kill child process
	if (data->semaphore->forks != SEM_FAILED)
		sem_close(data->semaphore->forks);
	if (data->semaphore->printf != SEM_FAILED)
		sem_close(data->semaphore->printf);
	free(data->semaphore);
	free(data->philo);
	free(data);
	printf(message);
	exit(exit_status);
}

t_sem *init_semaphores(t_input *input)
{
	t_sem *sema;
	
	sema = malloc(sizeof(t_sem));
	if (!sema)
		return (NULL);
	sema->forks = sem_open("/forks", O_CREAT | O_TRUNC, 0644, input->num_philo);
	sema->printf = sem_open("/printf", O_CREAT | O_TRUNC, 0644, 1);
	return (sema);
}

t_philo *init_philo(t_input *input, t_sem *semaphore)
{
	t_philo *philo;
	int i;

	i = 0;
	philo = malloc(sizeof(t_philo) * input->num_philo);
	if (!philo)
		return (NULL);
	while (i < input->num_philo)
	{
		philo[i].id = i + 1;
		philo[i].start = 0;
		philo[i].input = input;
		philo[i].meal_eaten = 0;
		philo[i].meal_check = sem_open("/meals", O_CREAT | O_TRUNC, 0644, 1);
		philo[i].last_time_eat = 0;
		philo[i].semaphore = semaphore;
		i++;
	}
	return (philo); 
}

t_data *initialize_all (t_input *input)
{
	t_data *data;
	
	data = malloc(sizeof(t_data));
	if (!data)
	{
		printf("malloc failed\n");
		exit (1);
	}
	data->semaphore = init_semaphores(input);
	if (!data->semaphore)
		free_exit("error open semaphore\n", data, 0, 1);
	data->philo = init_philo(input, data->semaphore);
	if (!data->philo)
		free_exit("malloc failed\n", data, 0, 1);
	data->input = input;
	return (data);
}


void print_msg(t_philo *philo, char *msg)
{
	size_t	time;

	sem_wait(philo->semaphore->printf);
	time = get_current_time_ms() - philo->start;
	printf("%ld %d %s\n", time, philo->id, msg);
	sem_post(philo->semaphore->printf);
}

void take_forks(t_philo *philo)
{
	sem_wait(philo->semaphore->forks);
	print_action(philo, "has taken a fork");
	sem_wait(philo->semaphore->forks);
	print_action(philo, "has taken a fork");
}

void drop_forks(t_philo *philo)
{
	sem_post(philo->semaphore->forks);
	sem_post(philo->semaphore->forks);
}
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
void eating_stage(t_philo *philo)
{
	print_action(philo, "is eating");
	smart_sleep(philo, philo->input->time_to_eat);
	sem_wait(philo->meal_check);
	philo->last_time_eat = get_time();
	philo->meal_eaten++;
	sem_post(philo->meal_check);
}


void philo_routine(t_philo *philo)
{
	pthread_t monitor;
	
	philo->start = get_current_time_ms;
	if (philo->id % 2 == 0) 
		usleep(200);
	// pthread_create(&monitor, NULL, death_checker, philo);
	while (1)
	{
		print_msg(philo, "is thinking");
		take_forks(philo);
		eating_stage(philo);
		drop_forks(philo);
		print_action(philo, "is sleeping");
		// smart_sleep(philo->data->time_to_sleep);
	}
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

void creat_philo(t_data *data)
{
	int i;
	pid_t *pids;

	pids = malloc(data->input->num_philo * sizeof(pid_t));
	ft_bzero(pids, sizeof(pid_t) * data->input->num_philo);
	i = 0;
	while (i < data->input->num_philo)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			free_exit("fork failed\n", data, true, 1);
			exit(1);
		}
		else if (pids[i] == 0)
		{
			philo_routine(&data->philo[i]);
			exit (0);
		}
		i++;
	}
	i = 0;
	// while (i < data->input->num_philo)
	// 	waitpid(pids[i++], NULL, 0);
	// /free_all();
}

void philosophers(t_input *input)
{
	t_data	*data;
	int i;
	
	data = initialize_all(input);
	creat_philo(data);
}