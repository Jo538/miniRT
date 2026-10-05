/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_bis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/10/05 14:45:11 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	compute_ambient_light(t_rt *rt, t_hit *hit, t_shade *shade)
{
	t_object	*a_light;
	t_object	*object;
	double		light_colour[3];
	double		reflectivity[3];

	a_light = rt->A;
	object = hit->closest;

	normalise_color(reflectivity, object->rgb);
	normalise_color(light_colour, a_light->rgb);

	scalar_product(a_light->ratio, light_colour, light_colour);

	component_wise_multiplication(reflectivity, light_colour, shade->ambient_light);
}

void	compute_diffuse_light(t_rt *rt, double n_dot_l, t_hit *hit, t_shade *shade)
{
	t_object	*light_source;
	t_object	*object;
	double		tmp[3];
	double		reflectivity[3];
	double		light_colour[3];

	light_source = rt->L;
	object = hit->closest;
	normalise_color(reflectivity, object->rgb);
	normalise_color(light_colour, light_source->rgb);
	scalar_product(light_source->ratio, light_colour, light_colour);
	component_wise_multiplication(reflectivity, light_colour, tmp);
	scalar_product(n_dot_l, tmp, shade->diffuse_light);
}

void	compute_specular_light(t_rt *rt, t_shade *shade)
{
	t_object	*light_source;
	double		light_colour[3];
	double		tmp1[3];
	double		tmp2;

	light_source = rt->L;
	normalise_color(light_colour, light_source->rgb);
	scalar_product(light_source->ratio, light_colour, light_colour);
	scalar_product(SPECULAR_REFLECTIVITY_COEFF, light_colour, tmp1);
	tmp2 = make_dot_product(shade->reflection_vector, shade->eye_vector);
	if (tmp2 < 0)
		tmp2 = 0;
	tmp2 = pow(tmp2, SHININESS_EXPONENT);
	scalar_product(tmp2, tmp1, shade->specular_light);
}
