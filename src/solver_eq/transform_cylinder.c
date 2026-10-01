/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 13:26:58 by admin             #+#    #+#             */
/*   Updated: 2026/10/01 13:27:35 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	define_local_frame(t_rt *rt, t_hit *hit)
{
	double	helper[3];

	make_vector(rt->first_object->vector, hit->up);

	if (fabs(rt->first_object->vector[1]) > 0.999)
		make_vector((double []){1, 0, 0}, helper);
	else
		make_vector((double []){0, 1, 0}, helper);

	cross_product(hit->up, helper, hit->right);
	normalise_vector(hit->right);

	cross_product(hit->right, hit->up, hit->forward);	
}

static void	rotate_cylinder(t_rt *rt, t_hit *hit)
{
	double	new_ray_origin[3];
	double	new_ray_direction[3];
	
	new_ray_origin[0] = make_dot_product(rt->C->coordinates, hit->right);
	new_ray_origin[1] = make_dot_product(rt->C->coordinates, hit->up);
	new_ray_origin[2] = make_dot_product(rt->C->coordinates, hit->forward);

	scalar_product(1, new_ray_origin, rt->C->coordinates);

	new_ray_direction[0] = make_dot_product(hit->ray_direction, hit->right);
	new_ray_direction[1] = make_dot_product(hit->ray_direction, hit->up);
	new_ray_direction[2] = make_dot_product(hit->ray_direction, hit->forward);

	scalar_product(1, new_ray_direction, hit->ray_direction);
}

static void	save_world_frame(t_rt *rt, t_hit *hit, double *ray_origin, double *ray_direction)
{
	scalar_product(1, rt->C->coordinates, ray_origin);
	scalar_product(1, hit->ray_direction, ray_direction);
}

void	revert_to_world_frame(t_rt *rt, t_hit *hit, double *ray_origin, double *ray_direction)
{
	make_vector(ray_origin, rt->C->coordinates);
	make_vector(ray_direction, hit->ray_direction);
}

void	transform_cylinder(t_rt *rt, t_hit *hit, double *world_ray_origin, double *world_ray_direction)
{	
	save_world_frame(rt, hit, world_ray_origin, world_ray_direction);
	define_local_frame(rt, hit);
	vector_subst(rt->C->coordinates, rt->first_object->coordinates, rt->C->coordinates);
	rotate_cylinder(rt, hit);
}
