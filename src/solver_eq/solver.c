/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:56:56 by bribot            #+#    #+#             */
/*   Updated: 2026/09/28 17:41:15 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	find_lowest_soluc(double a, double b, double c)
{
	double discriminant;
	double	sol1;
	double	sol2;

	discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0)
		return (-1000);
	discriminant = sqrt(discriminant);
	sol1 = (-b + discriminant) / (2 * a);
	sol2 = (-b - discriminant) / (2 * a);
	if (sol1 > 0.0001 && sol2 > 0.0001)
	{
		if (sol1 <= sol2)
			return (sol1);
		return (sol2);
	}
	if (sol1 > 0.0001)
		return (sol1);
	if (sol2 > 0.0001)
		return (sol2);
	return (-1000);
}

double solver_sp(t_ray *ray, t_object *obj)
{
	double	CO[3];
	double	D[3];
	double	rayon;
	t_tridouble	eq;

	CO[0] = ray->origin[0] - obj->coordinates[0];
	CO[1] = ray->origin[1] - obj->coordinates[1];
	CO[2] = ray->origin[2] - obj->coordinates[2];
	D[0] = ray->direction[0];
	D[1] = ray->direction[1];
	D[2] = ray->direction[2];
	rayon = (obj->diameter / 2) * (obj->diameter / 2);
	eq.a = make_dot_product(D, D);
	eq.b = make_dot_product(CO, D) * 2;
	eq.c = make_dot_product(CO, CO) - rayon;
	return (find_lowest_soluc(eq.a, eq.b, eq.c));
}
