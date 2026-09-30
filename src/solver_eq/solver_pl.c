/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver_pl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 16:05:48 by bribot            #+#    #+#             */
/*   Updated: 2026/09/28 17:09:44 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	solver_pl(t_ray *ray, t_object *obj)
{
	double	D[3];
	double	CO[3];
	double	numerator;
	double	denominator;
	double	t;

	D[0] = ray->direction[0];
	D[1] = ray->direction[1];
	D[2] = ray->direction[2];
	denominator = make_dot_product(D, obj->vector);
	if (fabs(denominator) < 0.0001f)
		return (-1000);
	CO[0] = ray->origin[0] - obj->coordinates[0];
	CO[1] = ray->origin[1] - obj->coordinates[1];
	CO[2] = ray->origin[2] - obj->coordinates[2];
	numerator = make_dot_product(CO, obj->vector);
	t = -numerator / denominator;
	if (t < 0.0001f)
		return (-1000);
	return (t);
}
