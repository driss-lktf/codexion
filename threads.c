/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_coder_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_thread, &sim->coders[i]))
			return (1);
		i++;
	}
	return (0);
}

static int	create_monitor(t_sim *sim)
{
	if (pthread_create(&sim->monitor, NULL, monitor_thread, sim))
		return (1);
	return (0);
}

static void	join_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.nb_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}

static void	init_start_times(t_sim *sim)
{
	int	i;

	sim->start_time = get_time_ms();
	i = 0;
	while (i < sim->config.nb_coders)
	{
		sim->coders[i].last_compile_start = sim->start_time;
		i++;
	}
}

int	launch_simulation(t_sim *sim)
{
	init_start_times(sim);
	if (create_coder_threads(sim))
		return (1);
	if (create_monitor(sim))
		return (1);
	join_all(sim);
	return (0);
}
