/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/25 16:09:42 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	compute_eye_vector(double *eye_vector, t_ray *ray)
{
	scalar_product(-1, ray->direction, eye_vector);
}

static void	compute_light_vector(double *light_vector, double *intersection, t_rt *rt)
{
	t_object	*light_source;

	light_source = rt->L;
	vector_subst(light_source->coordinates, intersection, light_vector);
	normalise_vector(light_vector);
}

static void	compute_normal(double *intersection, t_rt *rt, double *normal)
{
	t_object	*sphere;

	sphere = rt->first_object;
	vector_subst(intersection, sphere->coordinates, normal);
	normalise_vector(normal);
}

static void	compute_reflection_vector(double *light_vector, double *normal, double *reflection_vector)
{
	double	tmp_scalar;
	double	tmp_vector[3];

	tmp_scalar = 2 * make_dot_product(normal, light_vector);
	scalar_product(tmp_scalar, normal, tmp_vector);
	vector_subst(tmp_vector, light_vector, reflection_vector);
}

static void	compute_ambient_light(t_rt *rt, double *ambient_light)
{	
	t_object	*a_light;
	t_object	*sphere;
	double		light_colour[3];
	double		reflectivity[3];

	a_light = rt->A;
	sphere = rt->first_object;
	
	normalise_color(reflectivity, sphere->rgb);
	normalise_color(light_colour, a_light->rgb);
	
	scalar_product(a_light->ratio, light_colour, light_colour);
	
	component_wise_multiplication(reflectivity, light_colour, ambient_light);
}

static void	compute_diffuse_light(t_rt *rt, double *diffuse_light,
		double n_dot_l)
{
	t_object	*light_source;
	t_object	*sphere;
	double		tmp[3];
	double		reflectivity[3];
	double		light_colour[3];

	light_source = rt->L;
	sphere = rt->first_object;
	normalise_color(reflectivity, sphere->rgb);
	normalise_color(light_colour, light_source->rgb);
	scalar_product(light_source->ratio, light_colour, light_colour);
	component_wise_multiplication(reflectivity, light_colour, tmp);
	scalar_product(n_dot_l, tmp, diffuse_light);
}

static void	compute_specular_light(double *specular_light, double *eye_vector,
		double *reflection_vector, t_rt *rt)
{
	t_object	*light_source;
	double		light_colour[3];
	double		tmp1[3];
	double		tmp2;

	light_source = rt->L;
	normalise_color(light_colour, light_source->rgb);
	scalar_product(light_source->ratio, light_colour, light_colour);
	scalar_product(SPECULAR_REFLECTIVITY_COEFF, light_colour, tmp1);
	tmp2 = make_dot_product(reflection_vector, eye_vector);
	if (tmp2 < 0)
		tmp2 = 0;
	tmp2 = pow(tmp2, SHININESS_EXPONENT);
	scalar_product(tmp2, tmp1, specular_light);
}

void	compute_shaded_colour(t_rt *rt, double *intersection, t_ray *ray,
		double *shaded_rgb)
{
	double	ambient_light[3];
	double	diffuse_light[3];
	double	specular_light[3];
	double	normal[3];
	double	light_vector[3];
	double	eye_vector[3];
	double	reflection_vector[3];
	double	n_dot_l;

	compute_normal(intersection, rt, normal);
	compute_light_vector(light_vector, intersection, rt);
	compute_eye_vector(eye_vector, ray);
	compute_reflection_vector(light_vector, normal, reflection_vector);
	n_dot_l = make_dot_product(normal, light_vector);
	if (n_dot_l < 0)
		n_dot_l = 0;
	compute_ambient_light(rt, ambient_light);
	compute_diffuse_light(rt, diffuse_light, n_dot_l);
	if (n_dot_l > 0)
		compute_specular_light(specular_light, eye_vector,
			reflection_vector, rt);
	else
		ft_bzero(specular_light, sizeof(specular_light));
	add_vectors(ambient_light, diffuse_light, shaded_rgb);
	add_vectors(shaded_rgb, specular_light, shaded_rgb);
}
