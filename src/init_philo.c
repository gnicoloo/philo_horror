/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gnicolo <gnicolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 13:41:45 by gnicolo           #+#    #+#             */
/*   Updated: 2025/11/26 16:54:43 by gnicolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_set_data(t_data *data, char **argv, int argc)
{
	data->number_of_philosophers = (int)ft_atoi(argv[1]);
	if (data->number_of_philosophers < 1 || data->number_of_philosophers > 250)
		return (1);
	data->time_to_die = ft_atoi(argv[2]);
	if (data->time_to_die < 120)
		return (1);
	data->time_to_eat = ft_atoi(argv[3]);
	if (data->time_to_eat < 60)
		return (1);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (data->time_to_sleep < 60)
		return (1);
	data->all_ate = 0;
	if (argc == 6)
		data->number_of_times_each_philosopher_must_eat = (int)ft_atoi(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = -1;
	if (data->number_of_times_each_philosopher_must_eat == 0)
		return (1);
	data->start_time = 0;
	pthread_mutex_init(&data->all_ate_mutex, NULL);
	return (0);
}

void	ft_init_philos(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		philos[i].id = i;
		philos[i].meals_eaten = 0;
		philos[i].last_meal_time = get_time();
		philos[i].data = data;
		pthread_mutex_init(&philos[i].meal_lock, NULL);
		i++;
	}
}

void	ft_init_mutex(t_philo *philos)
{
	int				i;
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t)
			* philos[0].data->number_of_philosophers);
	if (!forks)
		return ;
	i = 0;
	while (i < philos[0].data->number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < philos[0].data->number_of_philosophers)
	{
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1)
			% philos[0].data->number_of_philosophers];
		i++;
	}
	pthread_mutex_init(&philos[0].data->write_mutex, NULL);
	philos->forks = forks;
}

int	check_list_parse(int argc, char **argv, t_data *data)
{
	pthread_t	thread;

	if (argc != 5 && argc != 6)
	{
		printf("Usage:%s number_of_philosophers ", argv[0]);
		printf("time_to_die time_to_eat time_to_sleep");
		return (printf(" [number_of_times_each_philosopher_must_eat]\n"), 1);
	}
	if (ft_check_input(argv, argc))
	{
		printf("Invalid input. Please enter positive integers.\n");
		return (1);
	}
	if (ft_set_data(data, argv, argc))
	{
		printf("Invalid arguments values\n");
		return (1);
	}
	if (data->number_of_philosophers == 1)
	{
		pthread_create(&thread, NULL, (void *)ft_one_philo, data);
		pthread_join(thread, NULL);
		return (1);
	}
	return (0);
}
