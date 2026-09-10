/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:22:21 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/10 14:57:57 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	free_tab(char **tab)
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

char	**extract_line(int fd, int *error)
{
	char	*new_line;
	char	*line;
	char	**row;

	row = NULL;
	line = get_next_line(fd);
	if (!line)
		return (NULL);
	new_line = ft_strtrim(line, "\n");
	free(line);
	if (!new_line)
		return (*error = 1, NULL);
	row = ft_split(new_line, ' ');
	free(new_line);
	if (!row)
		*error = 1;
	return (row);
}

int	extract_file(int fd)
{
	int		error;
	char	**row;

	error = 0;
	row = NULL;
	while (1)
	{
		row = extract_line(fd, &error);
		if (!row && error)
			return (1);
		if (!row)
			return (0);
		if (!*row)
			continue ;
		if (!is_correct(row))
		{
			free_tab(row);
			return (1);
		}
		free_tab(row);
	}
	return (0);
}
