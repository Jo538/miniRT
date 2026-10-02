/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 16:30:10 by bribot            #+#    #+#             */
/*   Updated: 2026/10/02 10:57:06 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	solver_main_bis(t_rt *rt, t_object *obj, t_hit *hit)
{
	double	t;

	if (obj->id == PLANE)
	{
		t = solver_pl(hit, obj);
		if (t <= 0)
			return (1);
		hit->t = t;
		find_intersection(rt, hit);
	}
	if (obj->id == SPHERE)
		return (solver_sphere(obj, hit));
	if (obj->id == CYLINDER)
		return (solver_cylinder(rt, hit));
	return (0);
}

int	solver_main(t_rt *rt, t_hit *hit)
{
	t_object	*object_trot;
	t_hit		candidate;

	object_trot = rt->first_object;
	while (object_trot != NULL)
	{
		candidate = *hit;
		candidate.t = INFINITY;
		candidate.closest = NULL;
		if (!solver_main_bis(rt, object_trot, &candidate)
			&& candidate.t < hit->t)
			*hit = candidate;
		object_trot = object_trot->next;
	}
	return (hit->closest == NULL);
}
