/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/24 13:22:32 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	eye_vector(double *eye_vector, t_ray *ray)
{
	scalar_product(-1, ray->direction, eye_vector);
}

static void	vector_subst(double *vector_1, double *vector_2, double *to_fill)
{
	to_fill[0] = vector_1[0] - vector_2[0];
	to_fill[1] = vector_1[1] - vector_2[1];
	to_fill[2] = vector_1[2] - vector_2[2];
}

static void	light_vector(double *light_vector, double *intersection, t_rt *rt)
{
	t_object	*light;

	light = rt->L;
	vector_subst(light->coordinates, intersection, light_vector);
}

static void	normal(double *intersection, t_rt *rt, double *normal)
{
	t_object	*sphere;

	sphere = rt->first_object;
	vector_subst(sphere->coordinates, intersection, normal);
	normalise(normal);
}

static void	reflection_vector(t_ray *ray, double *normal, double *reflection_vector)
{
	double	tmp_scalar;
	double	tmp_vector[3];

	tmp_scalar = 2 * make_dot_product(normal, ray->direction);
	scalar_product(tmp_scalar, normal, tmp_vector);
	vector_subst(ray->direction, tmp_vector, reflection_vector);
}

static void	ambient_light(t_rt *rt, double *ambient_light)
{	
	int			i;
	t_object	*light;
	double		normalised_colour[3];

	i = 0;
	light = rt->A;
	while (i < 3)
	{
		normalised_colour[i] = (light->rgb)[i] / 255;
		i++;
	}
	scalar_product(light->ratio, normalised_colour, ambient_light);
}
