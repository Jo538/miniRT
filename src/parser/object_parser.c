/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 16:42:46 by bribot            #+#    #+#             */
/*   Updated: 2026/09/09 16:38:05 by benji            ###   ########.fr       */
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

// CHOSE encore a faire :
//le checker pour savoir s il y a le bon nombre d arg dans info pour chaque objets
//le setter de tout a NULL
//

// void	set_tab(t_object **tab, char **info, int *tab_trot)
// {
// 	int	type_obj;

// 	type_obj = recognize_obj_type(info);
// 	if (type_obj == INVALID)
// 		return (*tab_trot = -1);
// 	if (type_obj == AMBIENT_LIGHTING || type_obj == CAMERA || type_obj == LIGHT)
// 		return (acl_case(tab, info, tab_trot, type_obj));
// 	return (other_case(tab, info, tab_trot, type_obj));

// }



// t_object *object_parser(char ***info, int size)
// {
// 	int			tab_trot;
// 	int			info_trot;
// 	t_object	*tab_info_obj;

// 	tab_trot = 3;
// 	info_trot = 0;
// 	tab_info_obj = malloc(sizeof(t_object) * size); //peut etre mettre un +3 pour le size
// 	if (!tab_info_obj)
// 		return (NULL);
// 	while (info_trot <= 2) //en vrai le remplacer par une fonction qui me tout a NULL/0 pour eviter les segfaults
// 		tab_info_obj[info_trot++].is_set = 0;
// 	info_trot = 0;
// 	while (info_trot < size && tab_trot != -1)
// 		set_tab(&tab_info_obj, info[info_trot++], &tab_trot);
// 	if (tab_trot == -1)
// 		return (free(tab_info_obj), printf("TEST\n"), NULL);
// 	return (tab_info_obj);
// }
