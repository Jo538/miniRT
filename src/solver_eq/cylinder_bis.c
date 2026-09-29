/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:16:49 by admin             #+#    #+#             */
/*   Updated: 2026/09/29 20:32:50 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	find_intersection(double t, t_rt *rt, t_hit *hit)
{
	double	tmp1[3];
	
	scalar_product(t, hit->ray_direction, tmp1);
	add_vectors(rt->C->coordinates, tmp1, hit->intersection);
}

int	pass_height_check(t_rt *rt, t_hit *hit, double y_min, double y_max, double t)
{
	double	y;

	y = rt->C->coordinates[1] + t * hit->intersection[1];
	if (y < y_min || y > y_max)
		return (0);
	return (1);
}

int	pass_cap_check(t_rt *rt, t_hit *hit, double y)
{
	double	t;
	double	x;
	double	z;

	t = (y - rt->C->coordinates[1]) / hit->ray_direction[1];
	x = rt->C->coordinates[0] + t * hit->intersection[0];
	z = rt->C->coordinates[2] + t * hit->intersection[2];
	
	if ((pow(x, 2) + pow(z, 2)) > pow(rt->first_object->diameter / 2, 2))
		return (0);
	return (1);
}