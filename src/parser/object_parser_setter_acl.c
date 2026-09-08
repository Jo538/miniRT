/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_parser_setter_acl.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 12:57:35 by benji             #+#    #+#             */
/*   Updated: 2026/09/08 14:15:14 by benji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	put_light(t_object **tab, char **info, int *tab_trot)
{
	if (tab[1]->is_set == 1)
		return (*tab_trot = -1);
	tab[2]->id = LIGHT;
	tab[2]->coordinates[0] = ft_atof(info[1]);
	tab[2]->coordinates[1] = ft_atof(info[2]);
	tab[2]->coordinates[2] = ft_atof(info[3]);
	tab[2]->ratio = ft_atof(info[4]);
	tab[2]->rgb[0] = ft_atoi(info[5]); //peut etre changer en fonction de l entree de info avec les ,
	tab[2]->rgb[1] = ft_atoi(info[6]);
	tab[2]->rgb[2] = ft_atoi(info[7]);
	if (info[8] != NULL) //verifie si c est le bon nombre de choses dans info
		return (*tab_trot = -1);
}

void	put_camera(t_object **tab, char **info, int *tab_trot)
{
	if (tab[1]->is_set == 1)
		return (*tab_trot = -1);
	tab[1]->id = CAMERA;
	tab[1]->coordinates[0] = ft_atof(info[1]);
	tab[1]->coordinates[1] = ft_atof(info[2]);
	tab[1]->coordinates[2] = ft_atof(info[3]);
	tab[1]->vector[0] = ft_atof(info[4]);
	tab[1]->vector[1] = ft_atof(info[5]);
	tab[1]->vector[2] = ft_atof(info[6]);
	tab[1]->fov = ft_atoi(info[7]);
	if (info[8] != NULL) //verifie si c est le bon nombre de choses dans info
		return (*tab_trot = -1);
}

void	put_ambient_lightning(t_object **tab, char **info, int *tab_trot)
{
	if (tab[0]->is_set == 1)
		return (*tab_trot = -1);
	tab[0]->id = AMBIENT_LIGHTING;
	//rajouter un checker de si c est un bon float qui est mis
	tab[0]->ratio = ft_atof(info[1]);
	//rajouter un checker de si c est un bon int qui est mis
	tab[0]->rgb[0] = ft_atoi(info[2]);
	tab[0]->rgb[1] = ft_atoi(info[3]);
	tab[0]->rgb[2] = ft_atoi(info[4]);
	if (info[5] != NULL) //verifie si c est le bon nombre de choses dans info
		return (*tab_trot = -1);
}

//rajouter un compteur de la taille de info en fonction du nombre d arg requis

void	acl_case(t_object **tab, char **info, int *tab_trot, int type_obj)
{
	if (type_obj == AMBIENT_LIGHTING)
		return (put_ambient_lightning(tab, info, tab_trot));
	if (type_obj == CAMERA)
		return (put_camera(tab, info, tab_trot));
	if (type_obj == LIGHT)
		return (put_light(tab, info, tab_trot));
}
