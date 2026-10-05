/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:16:49 by admin             #+#    #+#             */
/*   Updated: 2026/10/05 18:29:37 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	find_intersection(t_hit *hit)
{
	double	tmp1[3];

	scalar_product(hit->t, hit->ray_direction, tmp1);
	add_vectors(hit->origin, tmp1, hit->intersection);
}

int	pass_height_check(t_hit *hit, double t)
{
	double	y;
	double	y_min;
	double	y_max;

	y_min = -hit->closest->height / 2;
	y_max = hit->closest->height / 2;
	y = hit->origin[1] + t * hit->ray_direction[1];
	if (y < y_min || y > y_max)
		return (0);
	return (1);
}

int	pass_cap_check(t_hit *hit, double t)
{
	double	x;
	double	z;

	x = hit->origin[0] + t * hit->ray_direction[0];
	z = hit->origin[2] + t * hit->ray_direction[2];
	if ((pow(x, 2) + pow(z, 2)) > pow(hit->closest->diameter / 2, 2))
		return (0);
	return (1);
}

int	is_new_best(double new_t, t_hit *hit, int (*check)(t_hit *, double))
{
	if (new_t <= 0)
		return (0);
	if (new_t >= hit->t)
		return (0);
	if (!check(hit, new_t))
		return (0);
	return (1);
}
