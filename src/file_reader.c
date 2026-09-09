/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:22:21 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/09 16:31:03 by benji            ###   ########.fr       */
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
	t_head_objects	*head_of_all;

	row = NULL;
	head_of_all = create_linked_list_object();
	// printf("skdjghf %p %p %p %p", head_of_all->A, head_of_all->C, head_of_all->L, head_of_all->first_object);
	while (1)
	{
		row = extract_line(fd);
		if (!row)
			return ;
		// benji function to parse row
		// int i = -1;
		// while (row[++i])
		// 	printf ("%s ", row[i]);
		// printf ("\n");
		parser(row, head_of_all);
		free_tab(row);
	}
}
