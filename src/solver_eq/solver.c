/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:56:56 by bribot            #+#    #+#             */
/*   Updated: 2026/09/24 09:18:47 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
double	make_dot_product(double first[3], double second[3])
{
	double	to_return;

	to_return = first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
	return (to_return);
}

double	find_lowest_soluc(double a, double b, double c)
{
	double rc;
	double	sol1;
	double	sol2;

	rc = (b * b) - (4 * a * c);
	if (rc < 0)
		return (-1000); // sert a se rendre compte q il n y a pas de solutions
	sol1 = (-b + rc) / (2 * a);
	sol2 = (-b - rc) / (2 * a);

	if (sol1 <= sol2)
		return (sol1);
	else
		return (sol2);
}

double solver(t_ray *ray, t_rt *rt)
{
	double	CO[3];
	double	D[3];
	double	rayon;
	t_tridouble	eq;

	CO[0] = rt->C->coordinates[0] - rt->first_object->coordinates[0];
	CO[1] = rt->C->coordinates[1] - rt->first_object->coordinates[1];
	CO[2] = rt->C->coordinates[2] - rt->first_object->coordinates[2];
	D[0] = ray->direction[0];
	D[1] = ray->direction[1];
	D[2] = ray->direction[2];
	rayon = (rt->first_object->diameter / 2) * (rt->first_object->diameter / 2);
	eq.a = make_dot_product(D, D);
	eq.b = make_dot_product(CO, D) * 2;
	eq.c = make_dot_product(CO, CO) - rayon;
	return (find_lowest_soluc(eq.a, eq.b, eq.c));
}
