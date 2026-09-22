/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/22 19:19:42 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
	if (rt_init(&head_of_all))
		return (1);
	if (parse(fd, head_of_all))
		return (1);
	// mlx
	free_hoa(head_of_all);
	return (0);
}


