/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/10/02 12:25:20 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	init_hit(t_rt *rt, t_hit *hit)
{
	hit->closest = NULL;
	hit->t = INFINITY;
	hit->surface = DEFAULT;
	ft_bzero(hit->ray_direction, 3 * sizeof(double));
	ft_bzero(hit->intersection, 3 * sizeof(double));
	make_vector(rt->C->coordinates, hit->origin);
}

static int	parse_image(t_rt *rt)
{
	int		col;
	int		row;
	t_hit	hit;

	row = 0;
	while (row < Y_MAX)
	{
		col = 0;
		while (col < X_MAX)
		{
			init_hit(rt, &hit);
			find_ray_direction(col, row, rt, &hit);
			if (!solver_main(rt, &hit))
				colour_pixel(rt, col, row, &hit);
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
