/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	validate_counts(t_config *cfg)
{
	if (cfg->nb_coders < 1)
		return (1);
	if (cfg->compiles_required < 1)
		return (1);
	return (0);
}

static int	validate_times(t_config *cfg)
{
	if (cfg->time_to_burnout <= 0)
		return (1);
	if (cfg->time_to_compile <= 0)
		return (1);
	if (cfg->time_to_debug <= 0)
		return (1);
	if (cfg->time_to_refactor <= 0)
		return (1);
	return (0);
}

static int	validate_cooldown(t_config *cfg)
{
	if (cfg->dongle_cooldown < 0)
		return (1);
	return (0);
}

int	validate_config(t_config *cfg)
{
	if (validate_counts(cfg))
		return (1);
	if (validate_times(cfg))
		return (1);
	if (validate_cooldown(cfg))
		return (1);
	return (0);
}
