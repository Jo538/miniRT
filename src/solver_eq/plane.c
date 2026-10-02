/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 16:05:48 by bribot            #+#    #+#             */
/*   Updated: 2026/10/02 15:45:02 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	solver_pl(t_hit *hit)
{
	double	CO[3];
	double	numerator;
	double	denominator;
	double	t;

	denominator = make_dot_product(hit->ray_direction, hit->closest->vector);
	if (fabs(denominator) < EPSILON)
		return ;
	vector_subst(hit->origin, hit->closest->coordinates, CO);
	numerator = make_dot_product(CO, hit->closest->vector);
	t = -numerator / denominator;
	if (t < 0.0001f)
		return ;
	hit->t = t;
	find_intersection(hit);
}
