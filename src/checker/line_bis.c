/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_bis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 14:58:35 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/15 12:15:50 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	sphere(char ** row)
{
	if (!has_right_number_of_cells(row, 4))
		return (0);
	if (!has_correct_coordinates_format(row[1]))
	 	return (0);
	if (ft_atof(row[2]) < 0)
	{
		ft_putstr_fd("Error: sphere diameter can't be negative.\n", 2);
		return (0);		
	}
	if (!has_correct_colour_format(row[3]))
		return (0);
	return (1);
}

int	cylinder(char ** row)
{
	if (!has_right_number_of_cells(row, 6))
		return (0);
	if (!has_correct_coordinates_format(row[1]))
	 	return (0);
	if (!has_correct_vector_format(row[2]))
		return (0);
	if (ft_atof(row[3]) < 0 || ft_atof(row[4]) < 0)
	{
		ft_putstr_fd("Error: cylinder diameter and height can't be negative.\n", 2);
		return (0);		
	}
	if (!has_correct_colour_format(row[5]))
		return (0);
	return (1);
}
