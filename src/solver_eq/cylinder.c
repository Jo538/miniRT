/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 20:21:55 by admin             #+#    #+#             */
/*   Updated: 2026/09/29 20:38:09 by admin            ###   ########.fr       */
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
	eq->c = pow(ray_origin[0], 2) + pow(ray_origin[2], 2) - pow(cylinder->diameter / 2, 2);

	delta = pow(eq->b, 2) - 4 * eq->a * eq->c;
	return (delta);
}

static void	positive_delta(double delta, t_tridouble *eq, double *t, t_rt *rt, t_hit *hit, double y_min, double y_max)
{
	double	new_t;

	new_t = (-eq->b - sqrt(delta)) / (2 * eq->a);
	if (pass_height_check(rt, hit, y_min, y_max, new_t))
		*t = new_t;
			
 	new_t = (-eq->b + sqrt(delta)) / (2 * eq->a);

	if (new_t < *t && pass_height_check(rt, hit, y_min, y_max, new_t))
		*t = new_t;
}

static void	zero_delta(double delta, t_tridouble *eq, double *t, t_rt *rt, t_hit *hit, double y_min, double y_max)
{
	double	new_t;

	new_t = -eq->b / (2 * eq->a);
	if (pass_height_check(rt, hit, y_min, y_max, new_t))
		*t = new_t;
}

static int	solve_quadratic(t_rt *rt, t_hit *hit, double *t, double y_min, double y_max)
{
	double	delta;
	t_tridouble	eq;
	
	delta = compute_delta(rt, hit, &eq);
	if (delta < 0)
		return (1);
	if (delta == 0)
		zero_delta(delta, &eq, t, rt, hit, y_min, y_max);
	if (delta > 0)
		positive_delta(delta, &eq, t, rt, hit, y_min, y_max);
	return (0);
}

int	solver_cylinder(t_rt *rt, t_hit *hit)
{
	double	t;
	double	y_min; // eventually create 3 structures for sphere, plane and cylinder and add y_min and y_max at initialisation as never changes
	double	y_max;

	y_min = -rt->first_object->height / 2;
	y_max = rt->first_object->height / 2;

	if (solve_quadratic(rt, hit, &t, y_min, y_max))
		return (1);
	pass_cap_check(rt, hit, y_max);
	pass_cap_check(rt, hit, y_min);
	find_intersection(t, rt, hit);
	hit->surface = SIDE_WALL;
	return (0);
}
