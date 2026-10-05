/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/27 16:05:48 by bribot            #+#    #+#             */
/*   Updated: 2026/10/05 18:32:08 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	solver_pl(t_hit *hit)
{
	double	co[3];
	double	numerator;
	double	denominator;
	double	t;

	denominator = make_dot_product(hit->ray_direction, hit->closest->vector);
	if (fabs(denominator) < EPSILON)
		return ;
	vector_subst(hit->origin, hit->closest->coordinates, co);
	numerator = make_dot_product(co, hit->closest->vector);
	t = -numerator / denominator;
	if (t < 0.0001f)
		return ;
	hit->t = t;
	find_intersection(hit);
}
