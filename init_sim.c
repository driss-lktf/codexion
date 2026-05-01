/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->config.nb_coders);
	if (!sim->dongles)
		return (1);
	i = 0;
	while (i < sim->config.nb_coders)
	{
		if (dongle_init(&sim->dongles[i], i, sim->config.nb_coders))
			return (1);
		i++;
	}
	return (0);
}

static int	init_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->log_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&sim->stop_mutex, NULL))
		return (1);
	return (0);
}

static void	set_defaults(t_sim *sim, t_config *cfg)
{
	sim->config = *cfg;
	sim->coders = NULL;
	sim->dongles = NULL;
	sim->start_time = 0;
	sim->seq_counter = 0;
	sim->stop = 0;
}

t_sim	*init_sim(t_config *cfg)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (NULL);
	set_defaults(sim, cfg);
	if (init_mutexes(sim))
	{
		free(sim);
		return (NULL);
	}
	if (init_dongles(sim))
	{
		destroy_sim(sim);
		return (NULL);
	}
	if (init_coders(sim))
	{
		destroy_sim(sim);
		return (NULL);
	}
	return (sim);
}
