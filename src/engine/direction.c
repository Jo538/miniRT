/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   direction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/17 16:25:32 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	find_ray_direction(double pixel_x, double pixel_y, int FOV, t_viewport *viewport)
{
	double	normalised_coord[2];
	double	viewport_coord[2];
	
	parse_viewport(FOV, viewport);
	normalised_coord = normalise_pixel(pixel_x, pixel_y);
	viewport_coord = find_viewport_coordinates(normalised_coord);
	find_D();
}

void	parse_viewport(int FOV, t_viewport *viewport)
{
	double	radian_FOV;
	
	radian_FOV = FOV * M_PI / 180;
	viewport->width = 2 * tan(radian_FOV / 2);
	viewport->height = viewport->width * (Y_MAX / X_MAX);	
}

double	*normalise_pixel(double pixel_x, double pixel_y)
{
	double	normalised_coord[2];

	normalised_coord[0] = (pixel_x + 0.5) / X_MAX;
	normalised_coord[1] = (pixel_y + 0.5) / Y_MAX;
	return (normalised_coord);
}

double	*find_viewport_coordinates(double *normalised_coord)
{
	double	viewport_coord[2];

	viewport_coord[0] = normalised_coord[0] * X_MAX - X_MAX / 2;
	viewport_coord[1] = Y_MAX / 2 - normalised_coord[1] * Y_MAX;

	return (viewport_coord);
}

void	find_D(double *viewport_coord, double viewport_z, t_ray *ray)
{
	double	length;

	length = sqrt(pow(viewport_coord[0], 2) + pow(viewport_coord[1], 2) + pow(viewport_z, 2));
	ray->direction[0] = viewport_coord[0] / length;
	ray->direction[1] = viewport_coord[1] / length;
	ray->direction[2] = viewport_z / length;
}