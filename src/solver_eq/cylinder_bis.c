/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:16:49 by admin             #+#    #+#             */
/*   Updated: 2026/09/30 21:23:48 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	find_intersection(t_rt *rt, t_hit *hit)
{
	double	tmp1[3];
	
	scalar_product(hit->t, hit->ray_direction, tmp1);
	add_vectors(rt->C->coordinates, tmp1, hit->intersection);
}

int	pass_height_check(t_rt *rt, t_hit *hit, double t)
{
	double	y;
	double	y_min;
	double	y_max;

	y_min = -rt->first_object->height / 2;
	y_max = rt->first_object->height / 2;

	y = rt->C->coordinates[1] + t * hit->ray_direction[1];
	if (y < y_min || y > y_max)
		return (0);
	return (1);
}

int	pass_cap_check(t_rt *rt, t_hit *hit, double t)
{
	double	x;
	double	z;

	x = rt->C->coordinates[0] + t * hit->intersection[0];
	z = rt->C->coordinates[2] + t * hit->intersection[2];
	
	if ((pow(x, 2) + pow(z, 2)) > pow(rt->first_object->diameter / 2, 2))
		return (0);
	return (1);
}

int	is_new_best(double new_t, t_hit *hit, t_rt *rt, int(*check)(t_rt *, t_hit *, double))
{
	if (new_t <= 0)
		return (0);
	if (new_t >= hit->t)
		return (0);
	if (!check(rt, hit, new_t))
		return (0);
	return (1);
}

void	define_local_frame(t_rt *rt, t_hit *hit)
{
	double	helper[3];

	make_vector(rt->first_object->vector, hit->up);

	if (rt->first_object->vector[1] > 0.999)
		make_vector((double []){1, 0, 0}, helper);
	else
		make_vector((double []){0, 1, 0}, helper);

	cross_product(hit->up, helper, hit->right);
	normalise_vector(hit->right);

	cross_product(hit->right, hit->up, hit->forward);	
}

void	shift_cylinder(t_rt *rt)
{
	double	new_ray_origin[3];

	vector_subst(rt->C->coordinates, rt->first_object->coordinates, new_ray_origin);
}

void	rotate_cylinder(t_rt *rt)
{
	double	new_ray_direction[3];
	double	new_ray_origin[3];

	new_ray_origin[0] = make_dot_product(rt->C->coordinates, u);
	new_ray_origin[1] = make_dot_product(rt->C->coordinates, a);
	new_ray_origin[2] = make_dot_product(rt->C->coordinates, w);

	new_ray_direction[0] = make_dot_product(rt->C->vector, u);
	new_ray_direction[1] = make_dot_product(rt->C->vector, a);
	new_ray_direction[2] = make_dot_product(rt->C->vector, w);
}