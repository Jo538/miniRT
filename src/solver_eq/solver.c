/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:56:56 by bribot            #+#    #+#             */
/*   Updated: 2026/09/25 16:39:17 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	find_lowest_soluc(double a, double b, double c)
{
	double rc;
	double	sol1;
	double	sol2;

	rc = (b * b) - (4 * a * c);
	if (rc < 0)
		return (-1000); // sert a se rendre compte q il n y a pas de solutions
	sol1 = (-b + sqrt(rc)) / (2 * a);
	sol2 = (-b - sqrt(rc)) / (2 * a);

	if (sol1 >= 0 && sol1 <= sol2)
		return (sol1);
	else if (sol2 >= 0 && sol2 <= sol2)
		return (sol2);
	else
		return (-1000);
}

static int	find_intersection(double *intersection, t_rt *rt, t_ray *ray, double t)
{
	double	tmp1[3];
	
	if (t == -1000)
		return (1);
	scalar_product(t, ray->direction, tmp1);
	add_vectors(rt->C->coordinates, tmp1, intersection);
	return (0);
}	

int	solver(t_ray *ray, t_rt *rt, double *intersection)
{
	double	CO[3];
	double	D[3];
	double	rayon;
	t_tridouble	eq;
	double	t;

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
	t = find_lowest_soluc(eq.a, eq.b, eq.c);
	return (find_intersection(intersection, rt, ray, t));
}
