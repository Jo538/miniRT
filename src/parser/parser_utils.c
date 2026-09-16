/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 17:04:10 by benji             #+#    #+#             */
/*   Updated: 2026/09/15 16:25:41 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	recognize_obj_type(char **info)
{
	if (ft_strncmp(info[0], "A", 1) == 0 && ft_strlen(info[0]) == 1)
		return (AMBIENT_LIGHTING);
	if (ft_strncmp(info[0], "C", 1) == 0 && ft_strlen(info[0]) == 1)
		return (CAMERA);
	if (ft_strncmp(info[0], "L", 1) == 0 && ft_strlen(info[0]) == 1)
		return (LIGHT);
	if (ft_strncmp(info[0], "sp", 2) == 0 && ft_strlen(info[0]) == 2)
		return (SPHERE);
	if (ft_strncmp(info[0], "pl", 2) == 0 && ft_strlen(info[0]) == 2)
		return (PLANE);
	if (ft_strncmp(info[0], "cy", 2) == 0 && ft_strlen(info[0]) == 2)
		return (CYLINDER);
	return (INVALID);
}

int	put_coordinates_i_objects(char *to_split, t_object *obj)
{
	char	**tmp;
	int		i;

	tmp = ft_split(to_split, ',');
	if (!tmp)
		return (1);
	obj->coordinates[0] = ft_atof(tmp[0]);
	obj->coordinates[1] = ft_atof(tmp[1]);
	obj->coordinates[2] = ft_atof(tmp[2]);
	i = -1;
	while (tmp[++i])
		free(tmp[i]);
	free(tmp);
	return (0);
}

int	put_rgb_i_objects(char *to_split, t_object *obj)
{
	char	**tmp;
	int		i;

	tmp = ft_split(to_split, ',');
	if (!tmp)
		return (1);
	obj->rgb[0] = ft_atoi(tmp[0]);
	obj->rgb[1] = ft_atoi(tmp[1]);
	obj->rgb[2] = ft_atoi(tmp[2]);
	i = -1;
	while (tmp[++i])
		free(tmp[i]);
	free(tmp);
	return (0);
}

int	put_normalized_vector(char *to_split, t_object *obj)
{
	char	**tmp;
	int		i;

	tmp = ft_split(to_split, ',');
	if (!tmp)
		return (1);
	obj->vector[0] = ft_atof(tmp[0]);
	obj->vector[1] = ft_atof(tmp[1]);
	obj->vector[2] = ft_atof(tmp[2]);
	i = -1;
	while (tmp[++i])
		free(tmp[i]);
	free(tmp);
	return (0);
}

void	free_hoa(t_head_objects *head)
{
	t_object	*trot;
	t_object	*tmp;

	trot = head->first_object;
	if (head->A)
		free(head->A);
	if (head->C)
		free(head->C);
	if (head->L)
		free(head->L);
	while (trot)
	{
		tmp = trot->next;
		free(trot);
		trot = tmp; 
	}
	free(head);
}
