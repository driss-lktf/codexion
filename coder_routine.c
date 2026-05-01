/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	has_finished(t_coder *c)
{
	return (c->compiles_done >= c->sim->config.compiles_required);
}

static void	do_compile(t_coder *c)
{
	c->last_compile_start = get_time_ms();
	print_status(c->sim, c->id, "is compiling");
	precise_sleep(c->sim->config.time_to_compile, c->sim);
	c->compiles_done++;
}

static void	do_debug(t_coder *c)
{
	print_status(c->sim, c->id, "is debugging");
	precise_sleep(c->sim->config.time_to_debug, c->sim);
}

static void	handle_single_coder(t_coder *c)
{
	while (!is_stopped(c->sim) && !has_finished(c))
	{
		precise_sleep(1, c->sim);
	}
}

void	*coder_thread(void *arg)
{
	t_coder	*c;

	c = (t_coder *)arg;
	if (c->sim->config.nb_coders == 1)
	{
		handle_single_coder(c);
		return (NULL);
	}
	while (!is_stopped(c->sim) && !has_finished(c))
	{
		if (acquire_both_dongles(c))
			break ;
		do_compile(c);
		release_both_dongles(c);
		if (is_stopped(c->sim) || has_finished(c))
			break ;
		do_debug(c);
		if (is_stopped(c->sim))
			break ;
		do_refactor(c);
	}
	return (NULL);
}
