/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gnicolo <gnicolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 13:51:10 by gnicolo           #+#    #+#             */
/*   Updated: 2025/11/26 16:48:00 by gnicolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_start_threads(t_philo *philos)
{
	int			i;
	pthread_t	monitor;

	philos[0].data->start_time = get_time();
	if (pthread_create(&monitor, NULL, ft_death_star, philos) != 0)
		return (printf("Failed to create monitor thread"), 1);
	i = 0;
	while (i < philos[0].data->number_of_philosophers)
	{
		if (pthread_create(&philos[i].thread, NULL, (void *)ft_routine,
				&philos[i]) != 0)
			return (printf("Failed to create philosopher thread"), 1);
		usleep(100);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < philos[0].data->number_of_philosophers)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	return (0);
}

void	ft_destroy_mutex(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_destroy(&philos[i].meal_lock);
		pthread_mutex_destroy(&philos[i].data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&philos[0].data->write_mutex);
	pthread_mutex_destroy(&data->all_ate_mutex);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_data	data;

	if (check_list_parse(argc, argv, &data))
		return (1);
	philos = malloc(data.number_of_philosophers * sizeof(t_philo));
	if (!philos)
		return (1);
	ft_init_philos(philos, &data);
	ft_init_mutex(philos);
	ft_start_threads(philos);
	free(philos->forks);
	free(philos);
	return (0);
}
