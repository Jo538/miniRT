/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:56:56 by bribot            #+#    #+#             */
/*   Updated: 2026/10/05 18:33:29 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_lowest_soluc(t_quadratic *eq, t_hit *hit)
{
	double	rc;
	double	sol1;
	double	sol2;

	rc = (eq->b * eq->b) - (4 * eq->a * eq->c);
	if (rc < 0)
		return ;
	sol1 = (-eq->b + sqrt(rc)) / (2 * eq->a);
	sol2 = (-eq->b - sqrt(rc)) / (2 * eq->a);
	if ((sol1 >= 0 && sol1 <= sol2) || (sol1 >= 0 && sol2 < 0))
		hit->t = sol1;
	else if ((sol2 >= 0 && sol2 <= sol1) || (sol2 >= 0 && sol1 < 0))
		hit->t = sol2;
}

static void	intersection(t_hit *hit)
{
	double	tmp1[3];

	if (hit->t == INFINITY)
		return ;
	scalar_product(hit->t, hit->ray_direction, tmp1);
	add_vectors(hit->origin, tmp1, hit->intersection);
}

void	solver_sphere(t_hit *hit)
{
	double		co[3];
	double		rayon;
	t_quadratic	eq;

	vector_subst(hit->origin, hit->closest->coordinates, co);
	rayon = (hit->closest->diameter / 2) * (hit->closest->diameter / 2);
	eq.a = make_dot_product(hit->ray_direction, hit->ray_direction);
	eq.b = make_dot_product(co, hit->ray_direction) * 2;
	eq.c = make_dot_product(co, co) - rayon;
	parse_lowest_soluc(&eq, hit);
	if (hit->t != INFINITY)
		intersection(hit);
}
