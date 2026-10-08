/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/10/08 13:06:01 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"


static int	has_correct_extension(char *file)
{
	int	len;

	len = ft_strlen(file);
	if (ft_strncmp(file + len - 3, ".rt", 3))
	{
		ft_putstr_fd("Error: wrong file extension\n", 2);
		return (0);
	}
	return (1);
}

static int	open_scene(char *file, int *fd)
{
	if (!has_correct_extension(file))
		return (1);
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
	int		fd;
	t_rt	*rt;

	if (argc != 2)
	{
		ft_putstr_fd("Error: wrong number of arguments\n", 2);
		return (1);
	}
	if (open_scene(argv[1], &fd))
		return (1);
	if (rt_init(&rt))
		return (1);
	if (parse(fd, rt))
		return (1);
	close(fd);
	run_engine(rt);
	free_hoa(rt);
	return (0);
}
