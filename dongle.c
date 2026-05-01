/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_init(t_dongle *d, int id, int nb_coders)
{
	d->id = id;
	d->in_use = 0;
	d->release_time = 0;
	d->granted_to = -1;
	if (pthread_mutex_init(&d->mutex, NULL))
		return (1);
	if (pthread_cond_init(&d->cond, NULL))
		return (1);
	if (heap_init(&d->wait_queue, nb_coders))
		return (1);
	return (0);
}

void	dongle_destroy(t_dongle *d)
{
	pthread_mutex_destroy(&d->mutex);
	pthread_cond_destroy(&d->cond);
	heap_destroy(&d->wait_queue);
}

int	dongle_available(t_dongle *d, long cooldown)
{
	long	now;

	if (d->in_use)
		return (0);
	if (d->release_time == 0)
		return (1);
	now = get_time_ms();
	return ((now - d->release_time) >= cooldown);
}
