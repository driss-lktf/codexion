/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_acquire.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	other_free(int cid, int did, t_sim *sim)
{
	int		left;
	int		right;
	int		other;
	long	cd;

	cd = sim->config.dongle_cooldown;
	left = (cid + sim->config.nb_coders - 2) % sim->config.nb_coders;
	right = (cid - 1) % sim->config.nb_coders;
	if (left == did)
		other = right;
	else
		other = left;
	return (dongle_available(&sim->dongles[other], cd));
}

static int	should_yield(t_dongle *d, t_coder *c)
{
	int		i;
	t_hnode	mine;

	mine.coder_id = c->id;
	mine.priority = c->request_seq;
	i = 0;
	while (i < d->wait_queue.size)
	{
		if (d->wait_queue.arr[i].coder_id != c->id
			&& heap_cmp(d->wait_queue.arr[i], mine))
		{
			if (other_free(d->wait_queue.arr[i].coder_id,
					d->id, c->sim))
				return (1);
		}
		i++;
	}
	return (0);
}

void	enqueue_on_one(t_dongle *d, t_coder *c)
{
	t_hnode	node;

	pthread_mutex_lock(&d->mutex);
	node.coder_id = c->id;
	node.priority = c->request_seq;
	heap_push(&d->wait_queue, node);
	pthread_mutex_unlock(&d->mutex);
}

void	dequeue_from_one(t_dongle *d, int coder_id)
{
	pthread_mutex_lock(&d->mutex);
	heap_remove(&d->wait_queue, coder_id);
	pthread_mutex_unlock(&d->mutex);
}

int	try_take_both(t_coder *c, int first, int second)
{
	t_dongle	*d1;
	t_dongle	*d2;
	long		cd;
	int			ok;

	d1 = &c->sim->dongles[first];
	d2 = &c->sim->dongles[second];
	cd = c->sim->config.dongle_cooldown;
	ok = 0;
	pthread_mutex_lock(&d1->mutex);
	pthread_mutex_lock(&d2->mutex);
	if (dongle_available(d1, cd) && dongle_available(d2, cd)
		&& !should_yield(d1, c) && !should_yield(d2, c))
	{
		d1->in_use = 1;
		d2->in_use = 1;
		heap_remove(&d1->wait_queue, c->id);
		heap_remove(&d2->wait_queue, c->id);
		ok = 1;
	}
	pthread_mutex_unlock(&d2->mutex);
	pthread_mutex_unlock(&d1->mutex);
	return (ok);
}
