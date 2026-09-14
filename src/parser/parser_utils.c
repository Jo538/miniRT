/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 17:04:10 by benji             #+#    #+#             */
/*   Updated: 2026/09/11 13:05:55 by benji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
