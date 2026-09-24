/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/24 09:14:00 by admin            ###   ########.fr       */
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
	t_rt	*rt;

	// return (test_unitaire());

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
	run_engine(rt); // + close fd + free_hoa
	free_hoa(rt);
	return (0);
}
