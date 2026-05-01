/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	get_fifo_seq(t_coder *c)
{
	long	seq;

	pthread_mutex_lock(&c->sim->stop_mutex);
	seq = c->sim->seq_counter;
	c->sim->seq_counter++;
	pthread_mutex_unlock(&c->sim->stop_mutex);
	return (seq);
}

static long	get_edf_deadline(t_coder *c)
{
	return (c->last_compile_start + c->sim->config.time_to_burnout);
}

long	get_priority(t_coder *c)
{
	if (c->sim->config.scheduler == CDX_FIFO)
		return (get_fifo_seq(c));
	return (get_edf_deadline(c));
}

int	heap_cmp(t_hnode a, t_hnode b)
{
	if (a.priority < b.priority)
		return (1);
	if (a.priority > b.priority)
		return (0);
	return (a.coder_id < b.coder_id);
}
