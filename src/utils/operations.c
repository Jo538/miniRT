/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/24 16:35:52 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	scalar_product(double scalar, double *vector_2, double *to_fill)
{
	to_fill[0] = scalar * vector_2[0];
	to_fill[1] = scalar * vector_2[1];
	to_fill[2] = scalar * vector_2[2];
}

void	cross_product(double *vector_1, double *vector_2, double *to_fill)
{
	to_fill[0] = vector_1[1] * vector_2[2] -  vector_1[2] * vector_2[1];
	to_fill[1] = vector_1[2] * vector_2[0] -  vector_1[0] * vector_2[2];
	to_fill[2] = vector_1[0] * vector_2[1] -  vector_1[1] * vector_2[0];
}

void	vector_subst(double *vector_1, double *vector_2, double *to_fill)
{
	to_fill[0] = vector_1[0] - vector_2[0];
	to_fill[1] = vector_1[1] - vector_2[1];
	to_fill[2] = vector_1[2] - vector_2[2];
}

double	make_dot_product(double first[3], double second[3])
{
	double	to_return;

	to_return = first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
	return (to_return);
}

void	normalise_vector(double *vector)
{
	double	length;

	length = sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2));
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}