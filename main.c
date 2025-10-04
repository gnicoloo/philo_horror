/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuseppenicolo <giuseppenicolo@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 15:31:19 by giuseppenic       #+#    #+#             */
/*   Updated: 2025/10/04 12:25:27 by giuseppenic      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long ft_atoi(const char *str)
{
    long result = 0;
    int sign = 1;
    int i = 0;

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
    return result * sign;
}

unsigned long get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int ft_set_data(t_data *data, char **argv, int argc)
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

void ft_init_philos(t_philo *philos, t_data *data)
{
    int i = 0;

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

void ft_init_mutex(t_philo *philos)
{
    int i;
    pthread_mutex_t *forks;

    forks = malloc(sizeof(pthread_mutex_t) * philos[0].data->number_of_philosophers);
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
        philos[i].right_fork = &forks[(i + 1) % philos[0].data->number_of_philosophers];
        i++;
    }
}

int ft_one_philo(t_data *data)
{
    printf("0 1 has taken a fork\n");
    usleep(data->time_to_die * 1000);
    printf("%ld 1 died\n", data->time_to_die);
    return (1);
}

int ft_check_input(char **argv, int argc)
{
    int i = 1;
    int j;

    while (i < argc)
    {
        j = 0;
        if (argv[i][j] == '\0')
            return 0;
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

int get_all_ate(t_philo *philo)
{
    int res;

    pthread_mutex_lock(&philo->data->all_ate_mutex);
    res = philo->data->all_ate;
    pthread_mutex_unlock(&philo->data->all_ate_mutex);
    return (res);
}

void ft_print_state(t_philo *philo, t_state state)
{
    unsigned long timestamp;

    timestamp = get_time() - philo->data->start_time;
    if (state == THINKING && !get_all_ate(philo))
        printf("%lu %d is thinking\n", timestamp, philo->id);
    else if (state == EATING && !get_all_ate(philo))
        printf("%lu %d is eating\n", timestamp, philo->id);
    else if (state == SLEEPING && !get_all_ate(philo))
        printf("%lu %d is sleeping\n", timestamp, philo->id);
    else if (state == DEAD && !get_all_ate(philo))
        printf("%lu %d died\n", timestamp, philo->id);
    else if (state == HAS_TAKEN_A_FORK && !get_all_ate(philo))
        printf("%lu %d has taken a fork\n", timestamp, philo->id);
}

void set_death_state(t_philo *philo)
{
    int i = 0;

    while (i < philo->data->number_of_philosophers)
    {
        pthread_mutex_lock(&philo[i].data->all_ate_mutex);
        philo[i].data->all_ate = 1;
        pthread_mutex_unlock(&philo[i].data->all_ate_mutex);
        i++;
    }
}


void    ft_unlock_fork(void *arg)
{
    t_philo *philo;

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

void ft_eat_think_sleep(void *arg)
{
    t_philo *philo;

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

void    ft_taken_even(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    if (philo->data->number_of_philosophers % 2 == 1)
        usleep(1000);
    pthread_mutex_lock(philo->right_fork);
    ft_print_state(philo, HAS_TAKEN_A_FORK);
    pthread_mutex_lock(philo->left_fork);
    ft_print_state(philo, HAS_TAKEN_A_FORK);
}

void    ft_taken_odd(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    pthread_mutex_lock(philo->left_fork);
    ft_print_state(philo, HAS_TAKEN_A_FORK);
    pthread_mutex_lock(philo->right_fork);
    ft_print_state(philo, HAS_TAKEN_A_FORK);
}

void ft_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (philo->meals_eaten != philo->data->number_of_times_each_philosopher_must_eat)
    {
        pthread_mutex_lock(&philo->data->all_ate_mutex);
        if (philo->data->all_ate)
        {
            pthread_mutex_unlock(&philo->data->all_ate_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->data->all_ate_mutex);
        if (philo->id % 2 == 0 && philo->meals_eaten != philo->data->number_of_times_each_philosopher_must_eat)
            ft_taken_even(philo);
        else if (philo->id % 2 == 1 && philo->meals_eaten != philo->data->number_of_times_each_philosopher_must_eat)
            ft_taken_odd(philo);
        else 
            return ;
        ft_eat_think_sleep(philo);
    }
}


void *ft_death_star(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_data *data = philos[0].data;
    int i;

    while (1)
    {
        pthread_mutex_lock(&data->all_ate_mutex);
        if (data->all_ate)
        {
            pthread_mutex_unlock(&data->all_ate_mutex);
            break;
        }
        pthread_mutex_unlock(&data->all_ate_mutex);
        i = 0;
        while (i < data->number_of_philosophers)
        {
            pthread_mutex_lock(&philos[i].meal_lock);
            unsigned long time_since_last_meal = get_time() - philos[i].last_meal_time;
            pthread_mutex_unlock(&philos[i].meal_lock);
            if (time_since_last_meal > (unsigned long)data->time_to_die)
            {
                ft_print_state(&philos[i], DEAD);
                set_death_state(philos);
                return (NULL);
            }
            i++;
        }
        if (data->number_of_times_each_philosopher_must_eat != -1)
        {
            int j = 0;
            int all_ate = 1;
            while (j < data->number_of_philosophers)
            {
                pthread_mutex_lock(&philos[j].meal_lock);
                if (philos[j].meals_eaten < data->number_of_times_each_philosopher_must_eat)
                    all_ate = 0;
                pthread_mutex_unlock(&philos[j].meal_lock);
                j++;
            }
            if (all_ate)
            {
                pthread_mutex_lock(&data->all_ate_mutex);
                data->all_ate = 1;
                pthread_mutex_unlock(&data->all_ate_mutex);
                break;
            }
        }
        usleep(1000);
    }
    return NULL;
}


int ft_start_threads(t_philo *philos)
{
    int i;
    pthread_t monitor;

    philos[0].data->start_time = get_time();
    if (pthread_create(&monitor, NULL, ft_death_star, philos) != 0)
        return (perror("Failed to create monitor thread"), 1);
    i = 0;
    while (i < philos[0].data->number_of_philosophers)
    {
        if (pthread_create(&philos[i].thread, NULL, (void *)ft_routine, &philos[i]) != 0)
            return (perror("Failed to create philosopher thread"), 1);
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

int check_list_parse(int argc, char **argv, t_data *data)
{
   if (argc != 5 && argc != 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return (1);
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
        return (ft_one_philo(data));
    } 
    return (0);
}

int main(int argc, char **argv)
{
    t_philo *philos;
    t_data  data;

    if (check_list_parse(argc, argv, &data))
        return (1);
    philos = calloc(data.number_of_philosophers, sizeof(t_philo));
    if (!philos)
        return 1;
    ft_init_philos(philos, &data);
    ft_init_mutex(philos);
    ft_start_threads(philos);
    free(philos); 
    pthread_mutex_destroy(&data.all_ate_mutex);
    return 0;
}
