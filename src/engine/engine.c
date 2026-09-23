/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/23 12:22:18 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

char	*attrib_color(t_rt *rt)
{
	int			pixel;
	int			color;
	int			x;
	int			y;

	y = 0;
	x = 0;
	pixel = 0;
	color = 1;
	rt->mlx->addr[pixel++] = (color);
	rt->mlx->addr[pixel++] = (color >> 8) & 0xFF;
	rt->mlx->addr[pixel++] = (color >> 16) & 0xFF;
	rt->mlx->addr[pixel++] = (color >> 24);

	return (rt->mlx->addr);
}

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
			find_ray_direction(row, col, rt, &ray);
			//Benji's quadratic solving for sphere
			
			col++;
		}
		row++;
	}
	return (0);
}

void	run_engine(t_rt *rt) //en vrai je vais devoir corriger pour que ce soit plus propre
{
	mlx_init(rt);
	parse_image(rt);
	mlx_finish(rt);
}
