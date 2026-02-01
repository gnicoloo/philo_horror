/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gnicolo <gnicolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 13:46:06 by gnicolo           #+#    #+#             */
/*   Updated: 2025/11/26 16:27:59 by gnicolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_all_ate(t_philo *philo)
{
	int	res;

	pthread_mutex_lock(&philo->data->all_ate_mutex);
	res = philo->data->all_ate;
	pthread_mutex_unlock(&philo->data->all_ate_mutex);
	return (res);
}

void	ft_print_state(t_philo *philo, t_state state)
{
	unsigned long	timestamp;

	timestamp = get_time() - philo->data->start_time;
	pthread_mutex_lock(&philo[0].data->write_mutex);
	if (state == THINKING && !get_all_ate(philo))
		printf("%lu %d is thinking\n", timestamp, philo->id);
	else if (state == EATING && !get_all_ate(philo))
		printf("%lu %d is eating\n", timestamp, philo->id);
	else if (state == SLEEPING && !get_all_ate(philo))
		printf("%lu %d is sleeping\n", timestamp, philo->id);
	else if (state == DEAD)
		printf("%lu %d died\n", timestamp, philo->id);
	else if (state == HAS_TAKEN_A_FORK && !get_all_ate(philo))
		printf("%lu %d has taken a fork\n", timestamp, philo->id);
	pthread_mutex_unlock(&philo[0].data->write_mutex);
}
