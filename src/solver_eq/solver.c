/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:56:56 by bribot            #+#    #+#             */
/*   Updated: 2026/09/29 20:53:42 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	solver(t_rt *rt, t_hit *hit)
{
	int			result;
	t_object	*object;

	object = rt->first_object;
	if (object->id == SPHERE)
		result = solver_sphere(rt, hit);
	if (object->id == CYLINDER)
		result = solver_cylinder(rt, hit);
	return (result);
}
