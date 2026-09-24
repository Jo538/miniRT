/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/24 09:11:47 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	attrib_color(t_rt *rt, int colour, int position)
{
	rt->mlx->addr[position++] = (colour);
	rt->mlx->addr[position++] = (colour >> 8);
	rt->mlx->addr[position++] = (colour >> 16);
	rt->mlx->addr[position++] = (colour >> 24);
}

static int	rgb_to_int(t_rt *rt)
{
	int	*rgb;
	int	colour;

	rgb = rt->first_object->rgb;
	colour = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	return (colour);
}

static int	find_position(t_rt *rt, int col, int row)
{
	t_data_mlx	*mlx;

	mlx = rt->mlx;
	return (row * mlx->line_length + col * (mlx->bits_per_pixel / 8));
}

static void	colour_pixel(t_rt *rt, int col, int row)
{
	int	colour;
	int position;

	position = find_position(rt, col, row);
	colour = rgb_to_int(rt);
	attrib_color(rt, colour, position);
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
	mlx_initialization(rt);
	parse_image(rt);
	mlx_finish(rt);
}
