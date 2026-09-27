/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 20:21:55 by admin             #+#    #+#             */
/*   Updated: 2026/09/27 21:26:28 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// solve equation with y = 0. Make it 2D. Intersection with sphere in the plane
// Then solve intersection with disk (in the plane)

// equation ray, P(t) = O + tD
// equation sphere in the plane, x^2 + z^2 = r2
// substitute, a = Dx^2 + Dz^2,  

static	void	save_y_values(t_rt *rt, t_hit *hit, double *y_value)
{
	y_value[0] = rt->C->coordinates[1];	
	y_value[1] = rt->first_object->coordinates[1];	
	y_value[2] = hit->ray_direction[1];	

}

static	void	modify_y_values(t_rt *rt, t_hit *hit)
{
	rt->C->coordinates[1] = 0;	
	rt->first_object->coordinates[1] = 0;	
	hit->ray_direction[1] = 0;	
}

static int	is_in_finite_cylinder(t_rt *rt, t_hit *hit, double y_min, double y_max)
{
	if (hit->intersection[1] < y_min || hit->intersection[1] > y_max)
		return (0);
	return (1);
}

static void	find_y_limits(t_rt *rt,	double *y_min, double *y_max)
{
	*y_min = rt->first_object->coordinates[1] - rt->first_object->height / 2;
	*y_max = rt->first_object->coordinates[1] + rt->first_object->height / 2;	
}

void	solver_cylinder(t_rt *rt, t_hit *hit)
{
	double	y_min;
	double	y_max;
	double	y_value[3];

	save_y_values(rt, hit, y_value);
	modify_y_values(rt, hit);
	
	if (!solver_sphere(rt, hit) && is_in_finite_cylinder(rt, hit, y_min, y_max))
		return (0);
	// solve equation disk in the plane
}
