/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gnicolo <gnicolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:13:28 by gnicolo           #+#    #+#             */
/*   Updated: 2025/11/24 14:59:23 by gnicolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	must_eat(t_data *data, t_philo *philos, int j, int all_ate)
{
	j = 0;
	all_ate = 1;
	while (j < data->number_of_philosophers)
	{
		pthread_mutex_lock(&philos[j].meal_lock);
		if (philos[j].meals_eaten
			< data->number_of_times_each_philosopher_must_eat)
			all_ate = 0;
		pthread_mutex_unlock(&philos[j].meal_lock);
		j++;
	}
	if (all_ate)
	{
		pthread_mutex_lock(&data->all_ate_mutex);
		data->all_ate = 1;
		pthread_mutex_unlock(&data->all_ate_mutex);
		return (1);
	}
	return (0);
}

unsigned long	get_timeh(t_philo *philos, int i)
{
	unsigned long	time_since_last_meal;

	pthread_mutex_lock(&philos[i].meal_lock);
	time_since_last_meal = get_time() - philos[i].last_meal_time;
	pthread_mutex_unlock(&philos[i].meal_lock);
	return (time_since_last_meal);
}

void	*ft_death_star(void *arg)
{
	t_data	*data;
	int		i;
	t_philo	*philos;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (1)
	{
		pthread_mutex_lock(&data->all_ate_mutex);
		if (data->all_ate)
			return (pthread_mutex_unlock(&data->all_ate_mutex), NULL);
		pthread_mutex_unlock(&data->all_ate_mutex);
		i = -1;
		while (++i < data->number_of_philosophers)
		{
			if (get_timeh(philos, i) > (unsigned long)data->time_to_die)
				return (ft_print_state(&philos[i], DEAD),
					set_death_state(philos), NULL);
		}
		if (data->number_of_times_each_philosopher_must_eat != -1)
			if (must_eat(data, philos, 0, 0))
				return (NULL);
		usleep(1000);
	}
	return (NULL);
}
