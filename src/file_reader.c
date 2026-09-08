/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:22:21 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/08 14:39:42 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	count_cells(char *line)
{
	int	index_now;
	int	index_prev;
	int	cell_count;
	
	index_now = 0;
	index_prev = 0;
	cell_count = 0;
	while (line)
	{
		if (ft_isalpha(line[index_now]) && line[index_prev] == " "
			|| ft_isalpha(line[index_now]) && index_now == 0)
			cell_count++;
	}
	return (cell_count);
}

void	extract_cell(char *line)
{
	char	*cell;
	
	cell = NULL;
	int size = count_cells(line);
}

void	extract_line(int fd)
{
	char	*line;
	char	*row;

	row = NULL;
	line = get_next_line(fd);
	
}