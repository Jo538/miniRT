/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 16:05:48 by bribot            #+#    #+#             */
/*   Updated: 2026/10/01 15:08:51 by benji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	solver_pl(t_hit *hit, t_object *obj)
{
	double	D[3];
	double	CO[3];
	double	numerator;
	double	denominator;
	double	t;

	D[0] = hit->ray_direction[0];
	D[1] = hit->ray_direction[1];
	D[2] = hit->ray_direction[2];
	denominator = make_dot_product(D, obj->vector);
	if (fabs(denominator) < 0.0001f)
		return (-1000);
	CO[0] = hit->origin[0] - obj->coordinates[0];
	CO[1] = hit->origin[1] - obj->coordinates[1];
	CO[2] = hit->origin[2] - obj->coordinates[2];
	numerator = make_dot_product(CO, obj->vector);
	t = -numerator / denominator;
	if (t < 0.0001f)
		return (-1000);
	return (t);
}
