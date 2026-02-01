/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gnicolo <gnicolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 13:42:44 by gnicolo           #+#    #+#             */
/*   Updated: 2025/11/26 16:53:18 by gnicolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

unsigned long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_one_philo(void *data)
{
	printf("0 1 has taken a fork\n");
	usleep(((t_data *)data)->time_to_die * 1000);
	printf("%ld 1 died\n", ((t_data *)data)->time_to_die);
	pthread_mutex_destroy(&((t_data *)data)->all_ate_mutex);
}

int	ft_check_input(char **argv, int argc)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		if (argv[i][j] == '\0')
			return (0);
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

void	set_death_state(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->number_of_philosophers)
	{
		pthread_mutex_lock(&philo[i].data->all_ate_mutex);
		philo[i].data->all_ate = 1;
		pthread_mutex_unlock(&philo[i].data->all_ate_mutex);
		i++;
	}
}
