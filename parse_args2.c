/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_int(char *str, int *out)
{
	int		ok;
	long	val;

	val = ft_atol(str, &ok);
	if (!ok)
		return (1);
	*out = (int)val;
	return (0);
}

static int	parse_long(char *str, long *out)
{
	int		ok;
	long	val;

	val = ft_atol(str, &ok);
	if (!ok)
		return (1);
	*out = val;
	return (0);
}

static int	parse_scheduler(char *str, t_sched *out)
{
	if (ft_strcmp(str, "fifo") == 0)
		*out = CDX_FIFO;
	else if (ft_strcmp(str, "edf") == 0)
		*out = CDX_EDF;
	else
		return (1);
	return (0);
}

int	parse_args2(char **argv, t_config *cfg)
{
	if (parse_long(argv[5], &cfg->time_to_refactor))
		return (1);
	if (parse_int(argv[6], &cfg->compiles_required))
		return (1);
	if (parse_long(argv[7], &cfg->dongle_cooldown))
		return (1);
	return (parse_scheduler(argv[8], &cfg->scheduler));
}
