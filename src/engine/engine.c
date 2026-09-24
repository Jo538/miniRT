/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/24 17:48:41 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	parse_image(t_rt *rt)
{
	int		col;
	int		row;
	t_ray	ray;

	row = 0;
	while (row < Y_MAX)
	{
		col = 0;
		while (col < X_MAX)
		{
			find_ray_direction(col, row, rt, &ray);
			if (solver(&ray, rt) != -1000)
				colour_pixel(rt, col, row, intersection, &ray);
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
