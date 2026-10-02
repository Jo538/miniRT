/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 13:26:58 by admin             #+#    #+#             */
/*   Updated: 2026/10/02 12:11:57 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	define_local_frame(t_frame *local_frame)
{
	double	helper[3];

	make_vector(rt->first_object->vector, local_frame->up);

	if (fabs(rt->first_object->vector[1]) > 0.999)
		make_vector((double []){1, 0, 0}, helper);
	else
		make_vector((double []){0, 1, 0}, helper);

	cross_product(local_frame->up, helper, local_frame->right);
	normalise_vector(local_frame->right);

	cross_product(local_frame->right, local_frame->up, local_frame->forward);	
}

static void	rotate_cylinder(t_rt *rt, t_hit *hit, t_frame *local_frame)
{
	double	new_ray_origin[3];
	double	new_ray_direction[3];
	
	new_ray_origin[0] = make_dot_product(rt->C->coordinates, local_frame->right);
	new_ray_origin[1] = make_dot_product(rt->C->coordinates, local_frame->up);
	new_ray_origin[2] = make_dot_product(rt->C->coordinates, local_frame->forward);

	scalar_product(1, new_ray_origin, rt->C->coordinates);

	new_ray_direction[0] = make_dot_product(hit->ray_direction, local_frame->right);
	new_ray_direction[1] = make_dot_product(hit->ray_direction, local_frame->up);
	new_ray_direction[2] = make_dot_product(hit->ray_direction, local_frame->forward);

	scalar_product(1, new_ray_direction, hit->ray_direction);
}

void	revert_to_world_frame(t_rt *rt, t_hit *hit, t_frame *local_frame)
{
	double	world_origin[3];
	double	world_direction[3];
	double	component[3];

	scalar_product(rt->C->coordinates[0], local_frame->right, world_origin);
	scalar_product(rt->C->coordinates[1], local_frame->up, component);
	add_vectors(world_origin, component, world_origin);
	scalar_product(rt->C->coordinates[2], local_frame->forward, component);
	add_vectors(world_origin, component, world_origin);
	add_vectors(world_origin, rt->first_object->coordinates, world_origin);
	scalar_product(hit->ray_direction[0], local_frame->right, world_direction);
	scalar_product(hit->ray_direction[1], local_frame->up, component);
	add_vectors(world_direction, component, world_direction);
	scalar_product(hit->ray_direction[2], local_frame->forward, component);
	add_vectors(world_direction, component, world_direction);
	make_vector(world_origin, rt->C->coordinates);
	make_vector(world_direction, hit->ray_direction);
}

void	transform_cylinder(t_hit *hit, t_frame *local_frame)
{
	define_local_frame(local_frame);
	vector_subst(rt->C->coordinates, rt->first_object->coordinates, rt->C->coordinates);
	rotate_cylinder(rt, hit, local_frame);
}
