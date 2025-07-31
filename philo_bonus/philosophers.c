/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:33:13 by zfarouk           #+#    #+#             */
/*   Updated: 2025/07/31 23:33:41 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;
	size_t			i;

	p = (unsigned char *)s;
	i = 0;
	while (i < n)
		p[i++] = 0;
}

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
	sema->meal_eaten = sem_open("/meals", O_CREAT | O_TRUNC, 0644, input->num_philo);
	sema->philo_died = sem_open("/philodied", O_CREAT | O_TRUNC, 0644, input->num_philo);
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
		pthread_mutex_init(&philo[i].meal_check, NULL);
		philo[i].last_time_eat = 0;
		pthread_mutex_init(&philo[i].time_check, NULL);
		philo[i].semaphore = semaphore;
		i++;
	}
	return (philo); 
}

t_data *initialize_all(t_input *input)
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
	pthread_mutex_init(&data->should_stop, NULL);
	data->pids = malloc(data->input->num_philo * sizeof(pid_t));
	ft_bzero(data->pids, sizeof(pid_t) * data->input->num_philo);
	data->simulation_should_stop = false;
	data->input = input;
	data->thread = 0;
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
	print_msg(philo, "has taken a fork");
	sem_wait(philo->semaphore->forks);
	print_msg(philo, "has taken a fork");
}

void drop_forks(t_philo *philo)
{
	sem_post(philo->semaphore->forks);
	sem_post(philo->semaphore->forks);
}
void eating_stage(t_philo *philo)
{
	print_msg(philo, "is eating");
	usleep(philo->input->time_to_eat * 1000);
	drop_forks(philo);
	sem_wait(philo->meal_check);
	philo->last_time_eat = get_time();
	philo->meal_eaten++;
	if ()
	sem_post(philo->meal_check);
}

void philo_routine(t_philo *philo)
{
	pthread_t monitor;
	
	sem_wait(philo->semaphore->meal_eaten);
	philo->start = get_current_time_ms();
	if (pthread_create(&monitor, NULL, death_checker, philo) != 0)
		//dir chi haja
	if (philo->id % 2 == 0) 
		usleep(200);
	while (1)
	{
		print_msg(philo, "is thinking");
		take_forks(philo);
		eating_stage(philo);
		print_action(philo, "is sleeping");
		// smart_sleep(philo->data->time_to_sleep);
	}
}

void kill_all_processes(pid_t *philo_pids, int num_philos)
{
    int i;

    i = 0;
    while (i < num_philos)
    {
        kill(philo_pids[i], SIGKILL);
        i++;
    }
    i = 0;
    while (i < num_philos)
    {
        waitpid(philo_pids[i], NULL, 0);
        i++;
    }
}

void *check_meals(void *args)
{
	t_data *data;
	int counter;

	counter = 0;
	data = (t_data *)args;
	while (1)
	{
		sem_wait(data->semaphore->meal_eaten);
		counter++;
		if (counter == data->input->num_philo)
		{
			pthread_mutex_lock(&data->should_stop);
			if (data->simulation_should_stop)
			{
				pthread_mutex_unlock(&data->should_stop);
				break;
			}
			pthread_mutex_unlock(&data->should_stop);
			kill_all_processes(data->pids, data->input->num_philo);
			break;
		}
	}
	return (NULL);
}

int creat_monitors(t_data *data)
{
	if (data->input->check_meal_eated)
	{
		if (pthread_create(&data->thread, NULL, &check_meals,
				(void *)data) != 0)
			return (1);
	}
	return (0);
}


void wait_childs(t_data *data)
{
	int status;
	int i;

	i = 0;
	pid_t pid = waitpid(-1, &status, 0);
	if (WIFEXITED(status))
	{
		kill_all_processes(data->pids, data->input->num_philo);
		printf("%d is died\n", WEXITSTATUS(status));
		pthread_mutex_lock(&data->should_stop);
		data->simulation_should_stop = true;
		pthread_mutex_unlock(&data->should_stop);
		while (i < data->input->num_philo)
		{
			sem_post(data->semaphore->meal_eaten);
			i++;
		}
	}
}

void creat_philo(t_data *data)
{
	int i;
	pthread_t thread[2];

	i = 0;
	while (i < data->input->num_philo)
	{
		data->pids[i] = fork();
		if (data->pids[i] < 0)
		{
			free_exit("fork failed\n", data, true, 1);
			exit(1);
		}
		else if (data->pids[i] == 0)
		{
			philo_routine(&data->philo[i]);
			exit (0);
		}
		i++;
	}
	usleep(1000);
	if(creat_monitors(data) != 0);
		free_exit("fork failed\n", data, true, 1);
	wait_childs(data);
	free_exit("", data, false, 0);
}

void philosophers(t_input *input)
{
	t_data	*data;
	int i;
	
	data = initialize_all(input);
	creat_philo(data);
}
// int	smart_sleep(t_philo *philo, size_t duration)
// {
// 	size_t	start;

// 	start = get_current_time_ms();
// 	while (!simulation_should_end(philo) && (get_current_time_ms()
// 			- start) < duration)
// 	{
// 		usleep(100);
// 	}
// 	if (simulation_should_end(philo))
// 		return (1);
// 	return (0);
// }