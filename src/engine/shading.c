/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/24 17:31:53 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	compute_eye_vector(double *eye_vector, t_ray *ray)
{
	scalar_product(-1, ray->direction, eye_vector);
}

static void	compute_light_vector(double *light_vector, double *intersection, t_rt *rt)
{
	t_object	*light;

	light = rt->L;
	vector_subst(light->coordinates, intersection, light_vector);
}

static void	compute_normal(double *intersection, t_rt *rt, double *normal)
{
	t_object	*sphere;

	sphere = rt->first_object;
	vector_subst(sphere->coordinates, intersection, normal);
	normalise_vector(normal);
}

static void	compute_reflection_vector(t_ray *ray, double *normal, double *reflection_vector)
{
	double	tmp_scalar;
	double	tmp_vector[3];

	tmp_scalar = 2 * make_dot_product(normal, ray->direction);
	scalar_product(tmp_scalar, normal, tmp_vector);
	vector_subst(ray->direction, tmp_vector, reflection_vector);
}

static void	compute_ambient_light(t_rt *rt, double *ambient_light)
{	
	t_object	*light;
	double		normalised_colour[3];

	light = rt->A;
	normalise_color(normalised_colour, light);
	scalar_product(light->ratio, normalised_colour, ambient_light);
}

static void	compute_diffuse_light(t_rt *rt, double *diffuse_light, double *normal, double *light_vector)
{
	t_object	*light;
	t_object	*sphere;
	double		tmp[3];
	double		reflectivity_sphere[3];
	double		tmp2;

	light = rt->L;
	sphere = rt->first_object;
	normalise_color(reflectivity_sphere, sphere);
	component_wise_multiplication(reflectivity_sphere, light, tmp);
	tmp2 = make_dot_product(normal, light_vector);
	scalar_product(tmp2, tmp, diffuse_light);
}

static void	compute_specular_light(double *specular_light, double *eye_vector, double *reflection_vector, t_rt *rt)
{
	t_object	*light_source;
	double	tmp1[3];
	double	tmp2;

	light_source = rt->L;
	scalar_product(SPECULAR_REFLECTIVITY_COEFF, light_source->rgb, tmp1);
	tmp2 = make_dot_product(reflection_vector, eye_vector);
	tmp2 = pow(tmp2, SHININESS_EXPONENT);
	scalar_product(tmp2, tmp1, specular_light);
}

void	compute_shaded_colour(t_rt *rt, double *intersection, t_ray *ray, double *shaded_rgb)
{
	double	ambient_light[3];
	double	diffuse_light[3];
	double	specular_light[3];
	
	double	normal[3];
	double	light_vector[3];
	double	eye_vector[3];
	double	reflection_vector[3];
	
	compute_normal(intersection, rt, normal);
	compute_light_vector(light_vector, intersection, rt);
	compute_eye_vector(eye_vector, ray);
	compute_reflection_vector(ray, normal, reflection_vector);
	
	compute_ambient_light(rt, ambient_light);
	compute_diffuse_light(rt, diffuse_light, normal, light_vector);
	compute_specular_light(specular_light, eye_vector, reflection_vector, rt);
	
	add_vectors(ambient_light, diffuse_light, shaded_rgb);
	add_vectors(shaded_rgb, specular_light, shaded_rgb);
}
