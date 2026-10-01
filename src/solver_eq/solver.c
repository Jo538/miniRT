/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 16:30:10 by bribot            #+#    #+#             */
/*   Updated: 2026/10/01 15:26:18 by benji            ###   ########.fr       */
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
			return (0);
		hit->t = t;
		find_intersection(rt, hit);
	}
	else if (obj->id == SPHERE)
	{
		if (solver_sphere(obj, hit))
			return (0);
	}
	else if (obj->id == CYLINDER)
	{
		if (solver_cylinder(rt, hit))
			return (0);
	}
	else
		return (0);
	hit->closest = obj;
	return (1);
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
		if (solver_main_bis(rt, object_trot, &candidate)
			&& candidate.t < hit->t)
			*hit = candidate;
		object_trot = object_trot->next;
	}
	return (hit->closest != NULL);
}
