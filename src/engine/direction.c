/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   direction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/22 19:25:43 by admin            ###   ########.fr       */
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

static void	dot_product(double scalar, double *vector_2, double *to_fill)
{
	to_fill[0] = scalar * vector_2[0];
	to_fill[1] = scalar * vector_2[1];
	to_fill[2] = scalar * vector_2[2];
}

static void	find_D(double *viewport_coord, t_ray *ray, t_head_objects *rt)
{
	double u_right[3];
	double v_up[3];
	
	dot_product(viewport_coord[0], rt->viewport->right, u_right);
	dot_product(viewport_coord[1], rt->viewport->up, v_up);
	
	ray->direction[0] = rt->viewport->forward[0] + u_right[0] + v_up[0]; 
	ray->direction[1] = rt->viewport->forward[1] + u_right[1] + v_up[1]; 
	ray->direction[2] = rt->viewport->forward[2] + u_right[2] + v_up[2]; 
	normalise(ray->direction);
}

void	find_ray_direction(double pixel_x, double pixel_y, t_head_objects *rt, t_ray *ray)
{
	double	*normalised_coord;
	double	*viewport_coord;
	
	normalised_coord = normalise_pixel(pixel_x, pixel_y);
	viewport_coord = find_viewport_coordinates(normalised_coord, rt);
	free(normalised_coord);
	find_D(viewport_coord, ray, rt);
	free(viewport_coord);
}
