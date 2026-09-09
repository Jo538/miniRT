/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 14:58:35 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/09 18:41:39 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	ambient_light(char ** row)
{
	if (!has_right_number_of_cells(row, 3))
		return (0);
	if (!is_capital(row[0]))
		return (0);
	if (!is_in_range(row[1], 0.0f, 1.0f))
		return (0);
	if (!has_correct_colour_format(row[2]))
		return (0);
	return (1);
}

int	camera(char ** row)
{
	(void)row;
	return (1);
}

int	light(char ** row)
{
	(void)row;
	return (1);
}

int	plane(char ** row)
{
	(void)row;
	return (1);
}

int	sphere(char ** row)
{
	(void)row;
	return (1);
}

int	cylinder(char ** row)
{
	(void)row;
	return (1);
}

int	is_correct(char **row)
{
	int type = recognize_obj_type(row);
	int (*func[6])(char **) = {ambient_light, camera, light, sphere, plane, cylinder};
	int i = 0;

	while (i < 6)
	{
		if (type == i)
			return (func[i](row));
		i++;
	}
	ft_putstr_fd("Error: invalid object in .rt file\n", 2);
	return (0);	
}
