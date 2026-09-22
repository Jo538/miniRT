/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/22 15:08:06 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	find_up_right(t_head_objects *rt)
{
	t_object	*camera;
	t_viewport	*viewport;

	camera = rt->C;
	viewport = rt->viewport;
	
	viewport->forward[0] = camera->vector[0];
	viewport->forward[1] = camera->vector[1];
	viewport->forward[2] = camera->vector[2];
	
	viewport->right[0] = camera->vector[2];
	viewport->right[1] = camera->vector[1];
	viewport->right[2] = -camera->vector[0];

	viewport->up[0] = 0;
	viewport->up[1] = 1;
	viewport->up[2] = 0;
}

static int	parse_viewport(t_head_objects *rt)
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

static int	open_scene(char *file, int *fd)
{
	*fd = open(file, O_RDONLY);

	if (*fd == -1)
	{
		perror("Error");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int	fd;
	t_head_objects	*head_of_all;

	if (argc != 2)
	{
		ft_putstr_fd("Error: wrong number of arguments\n", 2);
		return (1);
	}
	if (open_scene(argv[1], &fd))
		return (1);
	if (rt_init(head_of_all))
		return (1);
	if (parse(fd, head_of_all))
		return (1);
	// mlx
	free_hoa(head_of_all);
	return (0);
}


