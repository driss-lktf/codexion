/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
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

int	parse_args(char **argv, t_config *cfg)
{
	if (parse_int(argv[1], &cfg->nb_coders))
		return (1);
	if (parse_long(argv[2], &cfg->time_to_burnout))
		return (1);
	if (parse_long(argv[3], &cfg->time_to_compile))
		return (1);
	if (parse_long(argv[4], &cfg->time_to_debug))
		return (1);
	return (parse_args2(argv, cfg));
}
