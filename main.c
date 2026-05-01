/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	err_msg(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	return (1);
}

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_sim		*sim;

	if (argc != 9)
		return (err_msg("invalid number of arguments"));
	memset(&cfg, 0, sizeof(t_config));
	if (parse_args(argv, &cfg))
		return (err_msg("invalid argument value"));
	if (validate_config(&cfg))
		return (err_msg("argument out of range"));
	sim = init_sim(&cfg);
	if (!sim)
		return (err_msg("initialization failed"));
	if (launch_simulation(sim))
	{
		destroy_sim(sim);
		return (err_msg("simulation failed"));
	}
	destroy_sim(sim);
	return (0);
}
