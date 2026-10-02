/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 20:21:55 by admin             #+#    #+#             */
/*   Updated: 2026/10/02 12:10:47 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	compute_delta(t_rt *rt, t_hit *hit, t_quadratic *eq)
{
	double	*ray_direction;
	double	*ray_origin;
	t_object	*cylinder;

	ray_direction = hit->ray_direction;
	ray_origin = rt->C->coordinates;
	cylinder = rt->first_object;
	
	eq->a = pow(ray_direction[0], 2) + pow(ray_direction[2], 2);
	eq->b = 2 * (ray_origin[0] * ray_direction[0] + ray_origin[2] * ray_direction[2]);
	eq->c = pow(ray_origin[0], 2) + pow(ray_origin[2], 2) - pow(cylinder->diameter / 2, 2);

	eq->delta = pow(eq->b, 2) - 4 * eq->a * eq->c;
}

static void	keep_nearest_root(t_quadratic *eq, t_rt *rt, t_hit *hit)
{
	double	new_t;

	new_t = (-eq->b - sqrt(eq->delta)) / (2 * eq->a);
	if (is_new_best(new_t, hit, rt, pass_height_check))
	{
		hit->t = new_t;
		hit->surface = SIDE_WALL;
	}
			
 	new_t = (-eq->b + sqrt(eq->delta)) / (2 * eq->a);

	if (is_new_best(new_t, hit, rt, pass_height_check))
	{
		hit->t = new_t;
		hit->surface = SIDE_WALL;
	}
}

static int	solve_quadratic(t_rt *rt, t_hit *hit)
{
	t_quadratic	eq;
	
	compute_delta(rt, hit, &eq);
	if (eq.delta < 0)
		return (1);
	if (eq.delta >= 0)
		keep_nearest_root(&eq, rt, hit);
	return (0);
}

static void	solve_cap(t_rt *rt, t_hit *hit)
{
	double	new_t;
	double	y_min;
	double	y_max;

	y_min = -rt->first_object->height / 2;
	y_max = rt->first_object->height / 2;
	
	new_t = (y_max - rt->C->coordinates[1]) / hit->ray_direction[1];
	if (is_new_best(new_t, hit, rt, pass_cap_check))
	{
		hit->t = new_t;
		hit->surface = TOP_CAP;
	}

	new_t = (y_min - rt->C->coordinates[1]) / hit->ray_direction[1];
	if (is_new_best(new_t, hit, rt, pass_cap_check))
	{
		hit->t = new_t;
		hit->surface = BOTTOM_CAP;
	}
}

int	solver_cylinder(t_hit *hit)
{
	t_frame	local_frame;
	
	transform_cylinder(rt, hit, &local_frame);
	if (solve_quadratic(rt, hit))
	{
		revert_to_world_frame(rt, hit, &local_frame);
		return (1);
	}
	solve_cap(rt, hit);
	if (hit->t == INFINITY)
	{
		revert_to_world_frame(rt, hit, &local_frame);
		return (1);
	}
	revert_to_world_frame(rt, hit, &local_frame);
	find_intersection(rt, hit);
	return (0);
}
