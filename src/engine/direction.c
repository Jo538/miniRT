/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   direction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/17 18:03:52 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static double	*normalise_pixel(double pixel_x, double pixel_y)
{
	double	*normalised_coord;

	normalised_coord = malloc(2 * sizeof(double));
	if (!normalised_coord)
		return (NULL);
	normalised_coord[0] = (pixel_x + 0.5) / X_MAX;
	normalised_coord[1] = (pixel_y + 0.5) / Y_MAX;
	return (normalised_coord);
}

static double	*find_viewport_coordinates(double *normalised_coord, t_head_objects *rt)
{
	double	*viewport_coord;
	t_viewport *viewport = rt->viewport;

	viewport_coord = malloc(2 * sizeof(double));
	if (!viewport_coord)
		return (NULL);
	viewport_coord[0] = normalised_coord[0] * viewport->width - viewport->width/ 2;
	viewport_coord[1] = viewport->height / 2 - normalised_coord[1] * viewport->height;
	return (viewport_coord);
}

static void	find_D(double *viewport_coord, double viewport_z, t_ray *ray)
{
	double	length;

	length = sqrt(pow(viewport_coord[0], 2) + pow(viewport_coord[1], 2) + pow(viewport_z, 2));
	ray->direction[0] = viewport_coord[0] / length;
	ray->direction[1] = viewport_coord[1] / length;
	ray->direction[2] = viewport_z / length;
}

void	find_ray_direction(double pixel_x, double pixel_y, t_head_objects *rt, t_ray *ray)
{
	double	*normalised_coord;
	double	*viewport_coord;
	
	normalised_coord = normalise_pixel(pixel_x, pixel_y);
	viewport_coord = find_viewport_coordinates(normalised_coord, rt);
	free(normalised_coord);
	find_D(viewport_coord, 1, ray);
	free(viewport_coord);
}

int main(void)
{
	t_head_objects rt;
	t_ray ray;
	rt.C = malloc(sizeof(t_object));
	rt.C->fov = 90;
	rt.viewport = malloc(sizeof(t_viewport));
	rt.viewport->width = 2;
	rt.viewport->height = 1.125;
	double pixel_x = 480;
	double pixel_y = 810;

	find_ray_direction(pixel_x, pixel_y, &rt, &ray);
	printf("ray x: %f\n", ray.direction[0]);
	printf("ray y: %f\n", ray.direction[1]);
}