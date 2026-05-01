/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_burned_out(t_coder *c, long now)
{
	long	limit;

	if (c->compiles_done >= c->sim->config.compiles_required)
		return (0);
	limit = c->sim->config.time_to_burnout;
	if ((now - c->last_compile_start) >= limit)
		return (1);
	return (0);
}

static int	check_all_done(t_sim *sim)
{
	int	i;
	int	req;

	i = 0;
	req = sim->config.compiles_required;
	while (i < sim->config.nb_coders)
	{
		if (sim->coders[i].compiles_done < req)
			return (0);
		i++;
	}
	set_stop(sim);
	wake_all_dongles(sim);
	return (1);
}

static int	check_burnout(t_sim *sim)
{
	int		i;
	long	now;

	i = 0;
	now = get_time_ms();
	while (i < sim->config.nb_coders)
	{
		if (coder_burned_out(&sim->coders[i], now))
		{
			set_stop(sim);
			print_burnout(sim, sim->coders[i].id);
			wake_all_dongles(sim);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_thread(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		if (check_all_done(sim))
			break ;
		if (check_burnout(sim))
			break ;
		usleep(1000);
	}
	return (NULL);
}
