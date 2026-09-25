/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/25 16:52:22 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	compute_eye_vector(t_ray *ray, t_shade *shade)
{
	scalar_product(-1, ray->direction, shade->eye_vector);
}

static void	compute_light_vector(double *intersection, t_rt *rt, t_shade *shade)
{
	t_object	*light_source;

	light_source = rt->L;
	vector_subst(light_source->coordinates, intersection, shade->light_vector);
	normalise_vector(shade->light_vector);
}

static void	compute_normal(double *intersection, t_rt *rt, t_shade *shade)
{
	t_object	*sphere;

	sphere = rt->first_object;
	vector_subst(intersection, sphere->coordinates, shade->normal);
	normalise_vector(shade->normal);
}

static void	compute_reflection_vector(t_shade *shade)
{
	double	tmp_scalar;
	double	tmp_vector[3];

	tmp_scalar = 2 * make_dot_product(shade->normal, shade->light_vector);
	scalar_product(tmp_scalar, shade->normal, tmp_vector);
	vector_subst(tmp_vector, shade->light_vector, shade->reflection_vector);
}

void	compute_shaded_colour(t_rt *rt, double *intersection, t_ray *ray,
		double *shaded_rgb)
{
	t_shade	shade;
	double	n_dot_l;

	compute_normal(intersection, rt, &shade);
	compute_light_vector(intersection, rt, &shade);
	compute_eye_vector(ray, &shade);
	compute_reflection_vector(&shade);
	n_dot_l = make_dot_product(shade.normal, shade.light_vector);
	if (n_dot_l < 0)
		n_dot_l = 0;
	compute_ambient_light(rt, &shade);
	compute_diffuse_light(rt, n_dot_l, &shade);
	if (n_dot_l > 0)
		compute_specular_light(rt, &shade);
	else
		ft_bzero(shade.specular_light, sizeof(shade.specular_light));
	add_vectors(shade.ambient_light, shade.diffuse_light, shaded_rgb);
	add_vectors(shaded_rgb, shade.specular_light, shaded_rgb);
}
