/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gnicolo <gnicolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 15:04:58 by giuseppenic       #+#    #+#             */
/*   Updated: 2025/11/26 16:49:20 by gnicolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	DEAD,
	HAS_TAKEN_A_FORK
}					t_state;

typedef struct s_data
{
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				number_of_philosophers;
	int				number_of_times_each_philosopher_must_eat;
	int				all_ate;
	int				someone_died;
	unsigned long	start_time;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	all_ate_mutex;
	pthread_mutex_t	*forks;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	unsigned long	last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*forks;
	t_data			*data;

}					t_philo;

void				ft_routine(void *arg);
void				ft_taken_even(void *arg);
void				ft_taken_odd(void *arg);
void				ft_unlock_fork(void *arg);
void				ft_eat_think_sleep(void *arg);
void				set_death_state(t_philo *philo);
void				ft_print_state(t_philo *philo, t_state state);
unsigned long		get_time(void);
int					ft_check_input(char **argv, int argc);
int					check_list_parse(int argc, char **argv, t_data *data);
void				ft_init_philos(t_philo *philos, t_data *data);
void				ft_init_mutex(t_philo *philos);
long				ft_atoi(const char *str);
int					ft_start_threads(t_philo *philos);
int					ft_set_data(t_data *data, char **argv, int argc);
void				set_death_state(t_philo *philo);
int					ft_check_input(char **argv, int argc);
void				ft_one_philo(void *data);
int					get_all_ate(t_philo *philo);
void				ft_print_state(t_philo *philo, t_state state);
void				*ft_death_star(void *arg);

#endif
