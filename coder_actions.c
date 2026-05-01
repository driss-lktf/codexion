/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	do_refactor(t_coder *c)
{
	print_status(c->sim, c->id, "is refactoring");
	precise_sleep(c->sim->config.time_to_refactor, c->sim);
}

static void	get_dongle_order(t_coder *c, int *first, int *second)
{
	int	left;
	int	right;
	int	nb;

	nb = c->sim->config.nb_coders;
	left = (c->id - 1 + nb - 1) % nb;
	right = (c->id - 1) % nb;
	if (left < right)
	{
		*first = left;
		*second = right;
	}
	else
	{
		*first = right;
		*second = left;
	}
}

int	acquire_both_dongles(t_coder *c)
{
	int	first;
	int	second;

	get_dongle_order(c, &first, &second);
	c->request_seq = get_priority(c);
	enqueue_on_one(&c->sim->dongles[first], c);
	enqueue_on_one(&c->sim->dongles[second], c);
	while (!is_stopped(c->sim))
	{
		if (try_take_both(c, first, second))
		{
			print_status(c->sim, c->id, "has taken a dongle");
			print_status(c->sim, c->id, "has taken a dongle");
			return (0);
		}
		usleep(500);
	}
	dequeue_from_one(&c->sim->dongles[first], c->id);
	dequeue_from_one(&c->sim->dongles[second], c->id);
	return (1);
}

void	release_both_dongles(t_coder *c)
{
	int	first;
	int	second;

	get_dongle_order(c, &first, &second);
	release_dongle(&c->sim->dongles[first], c->sim);
	release_dongle(&c->sim->dongles[second], c->sim);
}
