/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/25 15:08:33 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	normalise_color(double *normalised_colour, double *src)
{
	int			i;

	i = 0;
	while (i < 3)
	{
		normalised_colour[i] = src[i] / 255;
		i++;
	}	
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
