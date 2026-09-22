/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   direction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/22 15:35:48 by admin            ###   ########.fr       */
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

static void	dot_product(double *vector_1, double *vector_2, double *to_fill)
{
	to_fill[0] = vector_1[0] * vector_2[0];
	to_fill[1] = vector_1[1] * vector_2[1];
	to_fill[2] = vector_1[2] * vector_2[2];
}

static void	find_D(double *viewport_coord, double viewport_z, t_ray *ray, t_head_objects *rt)
{
	double	length;

	// arrow = forward + u . right + v . up
	double u_right[3];
	dot_product(viewport_coord, rt->viewport->right, u_right);
	printf("u.right: (%f,%f,%f)\n", u_right[0], u_right[1], u_right[2]);
	
	double v_up[3];
	dot_product(viewport_coord, rt->viewport->up, v_up);
	printf("v.up: (%f,%f,%f)\n", v_up[0], v_up[1], v_up[2]);
	
	double temp_d[3];
	temp_d[0] = rt->viewport->forward[0] + u_right[0] + v_up[0]; 
	temp_d[1] = rt->viewport->forward[1] + u_right[1] + v_up[1]; 
	temp_d[2] = rt->viewport->forward[2] + u_right[2] + v_up[2]; 

	length = sqrt(pow(temp_d[0], 2) + pow(temp_d[1], 2) + pow(temp_d[2], 2));
	
	ray->direction[0] = temp_d[0] / length;
	ray->direction[1] = temp_d[1] / length;
	ray->direction[2] = temp_d[2] / length;
}

void	find_ray_direction(double pixel_x, double pixel_y, t_head_objects *rt, t_ray *ray)
{
	double	*normalised_coord;
	double	*viewport_coord;
	
	normalised_coord = normalise_pixel(pixel_x, pixel_y);
	viewport_coord = find_viewport_coordinates(normalised_coord, rt);
	free(normalised_coord);
	find_D(viewport_coord, 1, ray, rt);
	free(viewport_coord);
}

static void	cross_product(double *vector_1, double *vector_2, double *to_fill)
{
	to_fill[0] = vector_1[1] * vector_2[2] -  vector_1[2] * vector_2[1];
	to_fill[1] = vector_1[2] * vector_2[0] -  vector_1[0] * vector_2[2];
	to_fill[2] = vector_1[0] * vector_2[1] -  vector_1[1] * vector_2[0];
}

static void	find_up_right(t_head_objects *rt)
{
	t_object	*camera;
	t_viewport	*viewport;

	camera = rt->C;
	viewport = rt->viewport;
	
	viewport->forward[0] = camera->vector[0];
	viewport->forward[1] = camera->vector[1];
	viewport->forward[2] = camera->vector[2];
	
	cross_product((double []){0, 1, 0},viewport->forward, viewport->right);
	printf("right: (%f,%f,%f)\n", viewport->right[0], viewport->right[1], viewport->right[2]);
	cross_product(viewport->forward, viewport->right, viewport->up);
	printf("up: (%f,%f,%f)\n", viewport->up[0], viewport->up[1], viewport->up[2]);
}

static int	parse_viewport(t_head_objects *rt)
{
	double	FOV = rt->C->fov;
	double	radian_FOV;
	rt->viewport = malloc(sizeof(t_viewport));
	if (!rt->viewport)
	{
		//ft_putstr_fd("Error: dynamic allocation failed.\n", 2);
		return (1);
	}
	radian_FOV = FOV * M_PI / 180;
	rt->viewport->width = 2 * tan(radian_FOV / 2);
	rt->viewport->height = rt->viewport->width * Y_MAX / X_MAX;
	find_up_right(rt);
	return (0);
}

int main(void)
{
	t_head_objects rt;
	t_ray ray;
	rt.C = malloc(sizeof(t_object));
	rt.C->fov = 90;
	
	rt.C->vector[0] = 0.6;
	rt.C->vector[1] = 0;
	rt.C->vector[2] = 0.8;
	parse_viewport(&rt);
	
	double pixel_x = 480;
	double pixel_y = 810;

	find_ray_direction(pixel_x, pixel_y, &rt, &ray);
	printf("ray x: %f\n", ray.direction[0]);
	printf("ray y: %f\n", ray.direction[1]);
	printf("ray z: %f\n", ray.direction[2]);
}