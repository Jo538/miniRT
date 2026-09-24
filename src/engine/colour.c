/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colour.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/24 10:44:18 by admin            ###   ########.fr       */
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

void	colour_pixel(t_rt *rt, int col, int row)
{
	int	colour;
	int position;

	position = find_position(rt, col, row);
	colour = rgb_to_int(rt);
	attrib_color(rt, colour, position);
}
