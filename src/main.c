/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/15 13:08:51 by bribot           ###   ########.fr       */
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
	head_of_all = extract_file(fd);
	if (!head_of_all)
		return (1);
	window_orchestrator(head_of_all);
	printf("RGB = %d\n", head_of_all->first_object->rgb[2]);
}
