/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver_pl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 16:05:48 by bribot            #+#    #+#             */
/*   Updated: 2026/09/27 19:29:33 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	solver_pl(t_ray *ray, t_rt *rt)
{
	double	D[3];
	double	CO[3];
	double	numerator;
	double	denominator;
	double	t;

	D[0] = ray->direction[0];
	D[1] = ray->direction[1];
	D[2] = ray->direction[2];
	denominator = make_dot_product(D, rt->first_object->coordinates);
	if (denominator < 0.0001f)
		return (0);
	CO[0] = rt->C->coordinates[0] - rt->first_object->coordinates[0];
	CO[1] = rt->C->coordinates[1] - rt->first_object->coordinates[1];
	CO[2] = rt->C->coordinates[2] - rt->first_object->coordinates[2];
	numerator = make_dot_product(CO, rt->first_object->vector);
	t = numerator / denominator;
	if (t < 0.0001f)
		return (-1000);
	return (t);
}
