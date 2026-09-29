/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 20:21:55 by admin             #+#    #+#             */
/*   Updated: 2026/09/29 10:41:15 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static double	compute_delta(t_rt *rt, t_hit *hit, t_tridouble *eq)
{
	double	*ray_direction;
	double	*ray_origin;
	t_object	*cylinder;
	double	delta;

	ray_direction = hit->ray_direction;
	ray_origin = rt->C->coordinates;
	cylinder = rt->first_object;
	
	eq->a = pow(ray_direction[0], 2) + pow(ray_direction[2], 2);
	eq->b = 2 * (ray_origin[0] * ray_direction[0] + ray_origin[2] * ray_direction[2]);
	eq->c = pow(ray_origin[0], 2) + pow(ray_origin[2], 2) - cylinder->diameter / 2;

	delta = pow(eq->b, 2) - 4 * eq->a * eq->c;
	return (delta);
}

static void	find_intersection(double t, t_rt *rt, t_hit *hit)
{
	double	tmp1[3];
	
	scalar_product(t, hit->ray_direction, tmp1);
	add_vectors(rt->C->coordinates, tmp1, hit->intersection);
	return (0);	
}

void	solver_cylinder(t_rt *rt, t_hit *hit)
{
	double	t;
	double	delta;
	t_tridouble	eq;

	delta = compute_delta(rt, hit, &eq);
	t = -eq.b / (2 * eq.a);
	find_intersection(t, rt, hit);
}
