/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	ft_atol(const char *str, int *ok)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	*ok = 0;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		result = result * 10 + (*str - '0');
		str++;
	}
	*ok = 1;
	return (result * sign);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)(tv.tv_sec * 1000) + (long)(tv.tv_usec / 1000));
}

long	elapsed_ms(t_sim *sim)
{
	return (get_time_ms() - sim->start_time);
}

void	precise_sleep(long ms, t_sim *sim)
{
	long	target;

	target = get_time_ms() + ms;
	while (get_time_ms() < target)
	{
		if (is_stopped(sim))
			return ;
		usleep(200);
	}
}
