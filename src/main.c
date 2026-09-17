/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/17 18:07:12 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
	head_of_all = create_linked_list_object(); // maybe put everything below this line inside rt_init() orchestrating function
	if (!head_of_all || extract_file(fd, head_of_all))
	{
		close(fd);
		get_next_line(fd);
		return (1);
	}
	if (parse_viewport(head_of_all))
	{
		close(fd);
		free_hoa(head_of_all);
		return (1);		
	}
	// mlx
	free_hoa(head_of_all);
	return (0);
}


