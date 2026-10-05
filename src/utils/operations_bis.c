/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/10/05 16:18:55 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	normalise_color(double *normalised_colour, double *src)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		normalised_colour[i] = src[i] / 255;
		i++;
	}
}

void	multiply_a_vector(double *vector1, double mul, double *to_fill)
{
	to_fill[0] = vector1[0] * mul;
	to_fill[1] = vector1[1] * mul;
	to_fill[2] = vector1[2] * mul;
}

void	component_wise_multiplication(double *vector_1, double *vector_2, double *to_fill)
{
	to_fill[0] = vector_1[0] * vector_2[0];
	to_fill[1] = vector_1[1] * vector_2[1];
	to_fill[2] = vector_1[2] * vector_2[2];
}

void	add_vectors(double *vector_1, double *vector_2, double *to_fill)
{
	to_fill[0] = vector_1[0] + vector_2[0];
	to_fill[1] = vector_1[1] + vector_2[1];
	to_fill[2] = vector_1[2] + vector_2[2];
}

void	make_vector(double *vector, double *to_fill)
{
	to_fill[0] = vector[0];
	to_fill[1] = vector[1];
	to_fill[2] = vector[2];
}
