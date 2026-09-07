/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/07 15:18:20 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	main(int argc, char **argv)
{
	int	fd;

	if (argc != 2)
	{
		ft_putstr_fd("Error: wrong number of arguments\n", 2);
		return (1);
	}
	
	fd = open(argv[1], O_RDONLY);
	
	if (fd == -1)
	{
		perror("Error");
		return (errno);
	}
}
