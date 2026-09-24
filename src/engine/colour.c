/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colour.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/24 18:14:34 by admin            ###   ########.fr       */
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

static int	rgb_to_int(double *rgb)
{

	int	colour;

	colour = ((int)rgb[0] << 16) | ((int)rgb[1] << 8) | (int)rgb[2];
	return (colour);
}

static int	find_position(t_rt *rt, int col, int row)
{
	t_data_mlx	*mlx;

	mlx = rt->mlx;
	return (row * mlx->line_length + col * (mlx->bits_per_pixel / 8));
}

void	colour_pixel(t_rt *rt, int col, int row, double *intersection, t_ray *ray)
{
	int	colour;
	int position;
	double	shaded_rgb[3];

	position = find_position(rt, col, row);
	compute_shaded_colour(rt, intersection, ray, shaded_rgb);
	scalar_product(255, shaded_rgb, shaded_rgb);
	colour = rgb_to_int(shaded_rgb);
	attrib_color(rt, colour, position);
}
