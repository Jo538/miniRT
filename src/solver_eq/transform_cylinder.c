/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/01 13:26:58 by admin             #+#    #+#             */
/*   Updated: 2026/10/02 11:19:15 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	define_local_frame(t_rt *rt, t_hit *hit, t_frame *local_frame)
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
	t_frame	local_frame;

	save_world_frame(rt, hit, world_ray_origin, world_ray_direction);
	define_local_frame(rt, hit, &local_frame);
	vector_subst(rt->C->coordinates, rt->first_object->coordinates, rt->C->coordinates);
	rotate_cylinder(rt, hit, &local_frame);
}
