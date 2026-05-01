/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_sim *sim, int id, char *msg)
{
	pthread_mutex_lock(&sim->log_mutex);
	if (!sim->stop)
		printf("%ld %d %s\n", elapsed_ms(sim), id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}

void	print_burnout(t_sim *sim, int id)
{
	pthread_mutex_lock(&sim->log_mutex);
	printf("%ld %d burned out\n", elapsed_ms(sim), id);
	pthread_mutex_unlock(&sim->log_mutex);
}

int	is_stopped(t_sim *sim)
{
	int	val;

	pthread_mutex_lock(&sim->stop_mutex);
	val = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (val);
}

void	set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}
