/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:22:21 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/09 12:55:42 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**extract_line(int fd)
{
	char	*new_line;
	char	*line;
	char	**row;

	row = NULL;
	line = get_next_line(fd);
	if (!line)
		return (row);
	new_line = ft_strtrim(line, "\n");
	free(line);
	if (!new_line)
		return (NULL);
	row = ft_split(new_line, ' ');
	free(new_line);
	return (row);
}

void	extract_file(int fd)
{
	char	**row;

	row = NULL;
	while (1)
	{
		row = extract_line(fd);
		int i = 0;
		if (!row)
			return ;
		while (row && row[i])
		{
			printf("%s ", row[i]);
			i++;
		}
		// benji function to parse row
		free_tab(row);
	}
}
