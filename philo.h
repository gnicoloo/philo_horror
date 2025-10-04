/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuseppenicolo <giuseppenicolo@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 15:04:58 by giuseppenic       #+#    #+#             */
/*   Updated: 2025/10/04 00:51:38 by giuseppenic      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

typedef enum e_state
{
    THINKING,
    EATING,
    SLEEPING,
    DEAD,
    HAS_TAKEN_A_FORK
}               t_state;

typedef struct s_data
{
    long            time_to_die;
    long            time_to_eat;
    long            time_to_sleep;
    int             number_of_philosophers;
    int             number_of_times_each_philosopher_must_eat;
    int             all_ate;
    unsigned long   start_time;
    pthread_mutex_t all_ate_mutex;
}               t_data;

typedef struct s_philo
{
    int             id;
    int             meals_eaten;
    unsigned long   last_meal_time;
    pthread_mutex_t meal_lock;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_data         *data;
}               t_philo;

#endif
