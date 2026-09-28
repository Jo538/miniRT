/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/28 16:56:54 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	parse_image(t_rt *rt)
{
	int		col;
	int		row;
	t_ray	ray;
	t_object	*closest_to_ray;

	row = 0;
	closest_to_ray = NULL;
	while (row < Y_MAX)
	{
		col = 0;
		while (col < X_MAX)
		{
			find_ray_direction(col, row, rt, &ray);
			if (solver_main(&ray, rt, &closest_to_ray) != 0)
				colour_pixel(rt, col, row, closest_to_ray);
			col++;
		}
		row++;
	}
	return (0);
}

void	run_engine(t_rt *rt)
{
	mlx_initialization(rt);
	if (rt->err)
		return ;
	parse_image(rt);
	mlx_run(rt);
}
