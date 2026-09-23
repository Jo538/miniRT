/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:22:21 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/23 11:42:39 by jchartie         ###   ########.fr       */
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

static int	has_ACL(t_rt *head)
{
	if (!head->A || !head->C || !head->L)
	{
		ft_putstr_fd("Error: one of ACL is missing.\n", 2);
		return (0);				
	}
	return (1);
}

int	extract_file(int fd, t_rt *rt)
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
			break ;
		if (!*row)
		{
			free_tab(row);
			continue ;			
		}
		if (!is_correct(row))
		{
			free_hoa(rt);
			free_tab(row);
			return (1);
		}
		parse_line(row, rt);
		free_tab(row);
		if (rt->err)
			return (free_hoa(rt), 1);
	}
	return (0);
}


int	parse(int fd, t_rt *rt)
{
	if (extract_file(fd, rt))
	{
		close(fd);
		get_next_line(fd);
		return (1);
	}
	if (!has_ACL(rt))
	{
		close(fd);
		free_hoa(rt);
		return (1);		
	}
	if (parse_viewport(rt))
	{
		close(fd);
		free_hoa(rt);
		return (1);		
	}
	return (0);
}
