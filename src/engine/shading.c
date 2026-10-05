/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/10/05 16:26:00 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	compute_eye_vector(t_hit *hit, t_shade *shade)
{
	scalar_product(-1, hit->ray_direction, shade->eye_vector);
}

static void	compute_light_vector(t_rt *rt, t_hit *hit, t_shade *shade)
{
	t_object	*light_source;

	light_source = rt->L;
	vector_subst(light_source->coordinates, hit->intersection, shade->light_vector);
	normalise_vector(shade->light_vector);
}

static void	compute_normal_cylinder(t_hit *hit, t_shade *shade)
{
	double	hit_point[3];
	double	tmp;
	double	tmp1[3];

	if (hit->surface == SIDE_WALL)
	{
		vector_subst(hit->intersection, hit->closest->coordinates, hit_point);
		tmp = make_dot_product(hit_point, hit->closest->vector);
		scalar_product(tmp, hit->closest->vector, tmp1);
		vector_subst(hit_point, tmp1, tmp1);
		scalar_product(1 / (hit->closest->diameter / 2), tmp1, shade->normal);
	}
	if (hit->surface == TOP_CAP)
		scalar_product(1, hit->closest->vector, shade->normal);
	if (hit->surface == BOTTOM_CAP)
		scalar_product(-1, hit->closest->vector, shade->normal);
	normalise_vector(shade->normal);
}

static void	compute_normal(t_hit *hit, t_shade *shade)
{
	t_object	*object;

	object = hit->closest;

	if (object->id == PLANE)
	{
		make_vector(object->vector, shade->normal);
		normalise_vector(shade->normal);
	}
	if (object->id == SPHERE)
	{
		vector_subst(hit->intersection, object->coordinates, shade->normal);
		normalise_vector(shade->normal);
	}
	if (object->id == CYLINDER)
		compute_normal_cylinder(hit, shade);
}

static void	compute_reflection_vector(t_shade *shade)
{
	double	tmp_scalar;
	double	tmp_vector[3];

	tmp_scalar = 2 * make_dot_product(shade->normal, shade->light_vector);
	scalar_product(tmp_scalar, shade->normal, tmp_vector);
	vector_subst(tmp_vector, shade->light_vector, shade->reflection_vector);
}



int	compute_shadow(t_rt *rt,t_hit *hit,t_shade *shade)
{
	double	shadow_origin[3];
	double	shadow_dir[3];
	double	tmp[3];
	double	lenght;
	t_hit	hit_of_shadow;

	multiply_a_vector(shade->normal, EPSILON, tmp);
	add_vectors(hit->intersection, tmp, shadow_origin);
	vector_subst(rt->L->coordinates, hit->intersection, shadow_dir);
	lenght = sqrt(shadow_dir[1] * shadow_dir[1] + shadow_dir[2] * shadow_dir[2] + shadow_dir[0] * shadow_dir[0]);
	normalise_vector(shadow_dir);
	make_vector(shadow_origin, hit_of_shadow.origin);
	make_vector(shadow_dir, hit_of_shadow.ray_direction);
	hit_of_shadow.t = INFINITY;
	if (solver(rt, &hit_of_shadow) == 0 && hit_of_shadow.t < lenght)
		return (1);
	return (0);
}

void	compute_shaded_colour(t_rt *rt, t_hit *hit, double *shaded_rgb)
{
	t_shade	shade;
	double	n_dot_l;

	compute_normal(hit, &shade);
	compute_light_vector(rt, hit, &shade);
	compute_eye_vector(hit, &shade);
	if (make_dot_product(shade.normal, shade.eye_vector) < 0)
		scalar_product(-1, shade.normal, shade.normal);
	compute_reflection_vector(&shade);
	n_dot_l = make_dot_product(shade.normal, shade.light_vector);
	if (n_dot_l < 0 || compute_shadow(rt, hit, &shade))
		n_dot_l = 0;
	compute_ambient_light(rt, hit, &shade);
	compute_diffuse_light(rt, n_dot_l, hit, &shade);
	if (n_dot_l > 0)
		compute_specular_light(rt, &shade);
	else
		ft_bzero(shade.specular_light, sizeof(shade.specular_light));
	add_vectors(shade.ambient_light, shade.diffuse_light, shaded_rgb);
	add_vectors(shaded_rgb, shade.specular_light, shaded_rgb);
}
