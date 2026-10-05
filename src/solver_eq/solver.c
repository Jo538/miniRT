/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 16:30:10 by bribot            #+#    #+#             */
/*   Updated: 2026/10/05 18:30:20 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	object_orchestrator(t_hit *hit)
{
	if (hit->closest->id == PLANE)
		solver_pl(hit);
	if (hit->closest->id == SPHERE)
		solver_sphere(hit);
	if (hit->closest->id == CYLINDER)
		solver_cylinder(hit);
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

static void	parse_hit(t_hit *candidate, t_hit *hit)
{
	hit->closest = candidate->closest;
	hit->t = candidate->t;
	hit->surface = candidate->surface;
	make_vector(candidate->intersection, hit->intersection);
}

int	solver(t_rt *rt, t_hit *hit)
{
	t_object	*object_trot;
	t_hit		candidate;

	object_trot = rt->first_object;
	while (object_trot)
	{
		init_candidate(object_trot, &candidate, hit);
		object_orchestrator(&candidate);
		if (candidate.t < hit->t)
			parse_hit(&candidate, hit);
		object_trot = object_trot->next;
	}
	if (hit->t != INFINITY)
		return (0);
	return (1);
}
