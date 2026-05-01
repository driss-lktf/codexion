/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_one_coder(t_coder *coder, int id, t_sim *sim)
{
	coder->id = id;
	coder->compiles_done = 0;
	coder->last_compile_start = 0;
	coder->request_seq = 0;
	coder->sim = sim;
}

int	init_coders(t_sim *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->config.nb_coders);
	if (!sim->coders)
		return (1);
	i = 0;
	while (i < sim->config.nb_coders)
	{
		init_one_coder(&sim->coders[i], i + 1, sim);
		i++;
	}
	return (0);
}
