/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:56:56 by bribot            #+#    #+#             */
/*   Updated: 2026/09/18 17:22:02 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
float	make_dot_product(float first[3], float second[3])
{
	float	to_return;

	to_return = first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
	return (to_return);
}

t_solutions	*solver(float x_dir, float y_dir, t_head_objects *head_of_all)
{
	float	CO[3];
	float	D[3];
	float	rayon;
	t_trifloat	eq;

	CO[0] = head_of_all->C->coordinates[0] - head_of_all->first_object->coordinates[0];
	CO[1] = head_of_all->C->coordinates[1] - head_of_all->first_object->coordinates[1];
	CO[2] = head_of_all->C->coordinates[2] - head_of_all->first_object->coordinates[2];
	D[0] = x_dir;
	D[1] = y_dir;
	D[2] = 1;
	rayon = (head_of_all->first_object->diameter / 2) * (head_of_all->first_object->diameter / 2);
	eq.a = make_dot_product(D, D);
	eq.b = make_dot_product(CO, D);
	eq.c = make_dot_product(CO, CO);
	//reste a faire la fonction qui resout le a^2t + bt + c
	return (NULL);
}
