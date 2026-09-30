/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 16:30:10 by bribot            #+#    #+#             */
/*   Updated: 2026/09/28 17:33:45 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	solver_main_bis(t_ray *ray, t_rt *rt, t_object *obj)
{
	(void)rt;
	if (obj->id == PLANE)
		return (solver_pl(ray, obj));
	if (obj->id == SPHERE)
		return (solver_sp(ray, obj));
	// if (obj->id == CYLINDER)
		// return celui de CY
}

int	solver_main(t_ray *ray, t_rt *rt, t_object **closest)
{
	t_object	*object_trot;
	double			smallest;
	double			t;

	object_trot = rt->first_object;
	smallest = 1500000;
	while (object_trot != NULL)
	{
		t = solver_main_bis(ray, rt, object_trot);
		if (t > 0.0001 && t < smallest)
		{
			smallest = t;
			*closest = object_trot;
		}
		object_trot = object_trot->next;
	}
	if (smallest == 1500000)
		return (0);
	return (1);
}
