/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_bis_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 19:04:09 by bribot            #+#    #+#             */
/*   Updated: 2026/10/05 19:23:59 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	compute_eye_vector(t_hit *hit, t_shade *shade)
{
	scalar_product(-1, hit->ray_direction, shade->eye_vector);
}

void	compute_light_vector(t_rt *rt, t_hit *hit, t_shade *shade)
{
	t_object	*light_source;

	light_source = rt->l;
	vector_subst(light_source->coordinates, hit->intersection,
		shade->light_vector);
	normalise_vector(shade->light_vector);
}

void	compute_normal_cylinder(t_hit *hit, t_shade *shade)
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
