/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/27 15:20:21 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	normalise_color(double *normalised_colour, t_object *light)
{
	int			i;
	// double		normalised_colour[3];

	i = 0;
	while (i < 3)
	{
		normalised_colour[i] = (light->rgb)[i] / 255;
		i++;
	}
}
