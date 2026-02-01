/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gnicolo <gnicolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 13:43:25 by gnicolo           #+#    #+#             */
/*   Updated: 2025/11/24 14:55:33 by gnicolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_unlock_fork(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}

void	ft_eat_think_sleep(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->meal_lock);
	ft_print_state(philo, EATING);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	usleep(philo->data->time_to_eat * 1000);
	ft_unlock_fork(philo);
	ft_print_state(philo, SLEEPING);
	usleep(philo->data->time_to_sleep * 1000);
	ft_print_state(philo, THINKING);
}

void	ft_taken_even(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->number_of_philosophers % 2 == 1)
		usleep(1000);
	pthread_mutex_lock(philo->right_fork);
	ft_print_state(philo, HAS_TAKEN_A_FORK);
	pthread_mutex_lock(philo->left_fork);
	ft_print_state(philo, HAS_TAKEN_A_FORK);
}

void	ft_taken_odd(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->left_fork);
	ft_print_state(philo, HAS_TAKEN_A_FORK);
	pthread_mutex_lock(philo->right_fork);
	ft_print_state(philo, HAS_TAKEN_A_FORK);
}

void	ft_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->meals_eaten
		!= philo->data->number_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_lock(&philo->data->all_ate_mutex);
		if (philo->data->all_ate)
		{
			pthread_mutex_unlock(&philo->data->all_ate_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->all_ate_mutex);
		if (philo->id % 2 == 0
			&& philo->meals_eaten
			!= philo->data->number_of_times_each_philosopher_must_eat)
			ft_taken_even(philo);
		else if (philo->id % 2 == 1
			&& philo->meals_eaten
			!= philo->data->number_of_times_each_philosopher_must_eat)
			ft_taken_odd(philo);
		else
			return ;
		ft_eat_think_sleep(philo);
	}
}
