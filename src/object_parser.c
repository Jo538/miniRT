/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:42:46 by bribot            #+#    #+#             */
/*   Updated: 2026/09/07 17:20:25 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	recognize_obj_type(char **info)
{
	if (ft_strncmp(info[0], "A", 1) && ft_strlen(info[0] == 1))
		return (AMBIENT_LIGHTING);
	if (ft_strncmp(info[0], "C", 1) && ft_strlen(info[0] == 1))
		return (CAMERA);
	if (ft_strncmp(info[0], "L", 1) && ft_strlen(info[0] == 1))
		return (LIGHT);
	if (ft_strncmp(info[0], "sp", 2) && ft_strlen(info[0] == 2))
		return (SPHERE);
	if (ft_strncmp(info[0], "pl", 2) && ft_strlen(info[0] == 2))
		return (PLANE);
	if (ft_strncmp(info[0], "cy", 2) && ft_strlen(info[0] == 2))
		return (CYLINDER);
	return (INVALID);
}

void	set_tab(t_object **tab, char **info, int *tab_trot)
{
	int	type_obj;

	type_obj = recognize_obj_type(info);
	if (type_obj == INVALID)
		*tab_trot = -1;
	else
}


t_object *object_parser(char ***info, int size)
{
	int			tab_trot;
	int			info_trot;
	t_object	*tab_info_obj;

	tab_trot = 3;
	info_trot = 0;
	tab_info_obj = malloc(sizeof(t_object) * size);
	if (!tab_info_obj)
		return (NULL);
	while (info_trot < size && tab_trot != -1)
		set_tab(&tab_info_obj, info[info_trot++], &tab_trot);
	if (tab_trot == -1)
	return (free(tab_info_obj), printf("TEST\n"), NULL);
}
