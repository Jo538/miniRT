/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:22:21 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/22 15:15:43 by admin            ###   ########.fr       */
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

static int	has_ACL(t_head_objects *head)
{
	if (!head->A || !head->C || !head->L)
	{
		ft_putstr_fd("Error: one of ACL is missing.\n", 2);
		return (0);				
	}
	return (1);
}

int	extract_file(int fd, t_head_objects *head_of_all)
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
			free_hoa(head_of_all);
			free_tab(row);
			return (1);
		}
		parse_line(row, head_of_all);
		free_tab(row);
		if (head_of_all->err)
			return (free_hoa(head_of_all), 1);
	}
	return (0);
}

static void	cross_product(double *vector_1, double *vector_2, double *to_fill)
{
	to_fill[0] = vector_1[1] * vector_2[2] -  vector_1[2] * vector_2[1];
	to_fill[1] = vector_1[2] * vector_2[0] -  vector_1[0] * vector_2[2];
	to_fill[2] = vector_1[0] * vector_2[1] -  vector_1[1] * vector_2[0];
}

static void	find_up_right(t_head_objects *rt)
{
	t_object	*camera;
	t_viewport	*viewport;

	camera = rt->C;
	viewport = rt->viewport;
	
	viewport->forward[0] = camera->vector[0];
	viewport->forward[1] = camera->vector[1];
	viewport->forward[2] = camera->vector[2];
	
	cross_product((double []){0, 1, 0},viewport->forward, viewport->right);
	cross_product(viewport->forward, viewport->right, viewport->up);
}

static int	parse_viewport(t_head_objects *rt)
{
	double	FOV = rt->C->fov;
	double	radian_FOV;
	rt->viewport = malloc(sizeof(t_viewport));
	if (!rt->viewport)
	{
		ft_putstr_fd("Error: dynamic allocation failed.\n", 2);
		return (1);
	}
	radian_FOV = FOV * M_PI / 180;
	rt->viewport->width = 2 * tan(radian_FOV / 2);
	rt->viewport->height = rt->viewport->width * Y_MAX / X_MAX;
	find_up_right(rt);
	return (0);
}

int	parse(int fd, t_head_objects *head_of_all)
{
	if (extract_file(fd, head_of_all))
	{
		close(fd);
		get_next_line(fd);
		return (1);
	}
	if (!has_ACL(head_of_all))
	{
		free_hoa(head_of_all);
		return (1);		
	}
	if (parse_viewport(head_of_all))
	{
		close(fd);
		free_hoa(head_of_all);
		return (1);		
	}
	return (0);
}
