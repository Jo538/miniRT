/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:56:56 by bribot            #+#    #+#             */
/*   Updated: 2026/09/29 11:24:53 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	solver(t_rt *rt, t_hit *hit)
{
	int			result;
	t_object	*object;

	object = rt->first_object;
	if (object == SPHERE)
		result = solver_sphere(rt, hit);
	if (object == CYLINDER)
		result = solver_cylinder(rt, hit);
	return (result);
}
