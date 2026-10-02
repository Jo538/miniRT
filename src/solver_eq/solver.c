/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 16:30:10 by bribot            #+#    #+#             */
/*   Updated: 2026/10/02 12:25:00 by jchartie         ###   ########.fr       */
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

static void	init_candidate(t_object *object, t_hit *candidate, t_hit *hit)
{
	candidate->closest = object;
	candidate->t = INFINITY;
	candidate->surface = DEFAULT;
	make_vector(hit->ray_direction, candidate->ray_direction);
	ft_bzero(candidate->intersection, 3 * sizeof(double));
	make_vector(hit->origin, candidate->origin);
}

int	solver_main(t_rt *rt, t_hit *hit)
{
	t_object	*object_trot;
	t_hit		candidate;

	object_trot = rt->first_object;
	while (object_trot)
	{

		candidate.closest = object_trot;
		if (!solver_main_bis(rt, object_trot, &candidate)
			&& candidate.t < hit->t)
			*hit = candidate;
		object_trot = object_trot->next;
	}
	return (hit->closest == NULL);
}
