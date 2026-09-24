/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   viewport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/24 12:44:04 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	cross_product(double *vector_1, double *vector_2, double *to_fill)
{
	to_fill[0] = vector_1[1] * vector_2[2] -  vector_1[2] * vector_2[1];
	to_fill[1] = vector_1[2] * vector_2[0] -  vector_1[0] * vector_2[2];
	to_fill[2] = vector_1[0] * vector_2[1] -  vector_1[1] * vector_2[0];
}

void	normalise(double *vector)
{
	double	length;

	length = sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2));
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}

static int	is_equal(double *vector_1, double *vector_2)
{
	if (vector_1[0] != vector_2[0])
		return (0);
	if (vector_1[1] != vector_2[1])
		return (0);
	if (vector_1[2] != vector_2[1])
		return (0);
	return (1);
}

static void	find_up_right(t_rt *rt)
{
	t_object	*camera;
	t_viewport	*viewport;

	camera = rt->C;
	viewport = rt->viewport;
	
	viewport->forward[0] = camera->vector[0];
	viewport->forward[1] = camera->vector[1];
	viewport->forward[2] = camera->vector[2];
	
	if (is_equal((double []){0, 1, 0}, viewport->forward)
		|| is_equal((double []){0, -1, 0}, viewport->forward))
		cross_product((double []){0, 0, 1},viewport->forward, viewport->right);
	else
		cross_product((double []){0, 1, 0},viewport->forward, viewport->right);
	normalise(viewport->right);
	cross_product(viewport->forward, viewport->right, viewport->up);
}

int	parse_viewport(t_rt *rt)
{
	double	FOV = rt->C->fov;
	double	radian_FOV;
	rt->viewport = malloc(sizeof(t_viewport));
	if (!rt->viewport)
	{
		ft_putstr_fd("Error: dynamic allocation failed.\n", 2);
		return (1);
	}
	radian_FOV = FOV * M_PI / 180;
	rt->viewport->width = 2 * tan(radian_FOV / 2);
	rt->viewport->height = rt->viewport->width * Y_MAX / X_MAX;
	find_up_right(rt);
	return (0);
}
