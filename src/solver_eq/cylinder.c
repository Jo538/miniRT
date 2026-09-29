/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 20:21:55 by admin             #+#    #+#             */
/*   Updated: 2026/09/29 12:21:20 by admin            ###   ########.fr       */
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

static void	find_intersection(double t, t_rt *rt, t_hit *hit)
{
	double	tmp1[3];
	
	scalar_product(t, hit->ray_direction, tmp1);
	add_vectors(rt->C->coordinates, tmp1, hit->intersection);
	return (0);	
}

static void	find_t_positive_delta(t_tridouble *eq, double delta, double *t)
{
	double	tmp_t[2];

	tmp_t[0] = (-eq->b - sqrt(delta)) / (2 * eq->a);
 	tmp_t[1] = (-eq->b + sqrt(delta)) / (2 * eq->a);
	if (tmp_t[0] <= tmp_t[1])
		*t = tmp_t[0];
	else
		*t = tmp_t[1];
}

static int	pass_height_check(t_rt *rt, t_hit *hit, double y_min, double y_max, double t)
{
	double	y;

	y = rt->C->coordinates[1] + t * hit->intersection[1];
	if (y < y_min || y > y_max)
		return (0);
	return (1);
}

static void	find_t(double delta, t_tridouble *eq, double *t)
{
	double	tmp_t[2];

	if (delta < 0)
		*t = -1000;
	if (delta == 0)
		*t = -eq->b / (2 * eq->a);
	if (delta > 0)
		find_t_positive_delta(eq, delta, t);
}

int	solver_cylinder(t_rt *rt, t_hit *hit)
{
	double	t;
	double	delta;
	t_tridouble	eq;
	double	y_min; // eventually create 3 structures for sphere, plane and cylinder and add y_min and y_max at initialisation as never changes
	double	y_max;

	y_min = -rt->first_object->height / 2;
	y_max = rt->first_object->height / 2;

	delta = compute_delta(rt, hit, &eq);
	find_t(delta, &eq, &t);
	if (t == -1000)
		return (1);
	if (!pass_height_check(rt, hit, y_min, y_max, t))
		return (1);
	find_intersection(t, rt, hit);
	hit->surface = SIDE_WALL;
	return (0);
}
