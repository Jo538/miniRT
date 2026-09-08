/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_parser_setter_other.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 13:55:55 by benji             #+#    #+#             */
/*   Updated: 2026/09/08 14:29:54 by benji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	put_sphere(t_object **tab, char **info, int *tab_trot)
{
	tab[*tab_trot]->id = SPHERE;
	tab[*tab_trot]->coordinates[0] = ft_atof(info[1]);
	tab[*tab_trot]->coordinates[1] = ft_atof(info[2]);
	tab[*tab_trot]->coordinates[2] = ft_atof(info[3]);
	tab[*tab_trot]->diameter = ft_atof(info[4]);
	tab[*tab_trot]->rgb[0] = ft_atoi(info[5]);
	tab[*tab_trot]->rgb[1] = ft_atoi(info[6]);
	tab[*tab_trot]->rgb[2] = ft_atoi(info[7]);
	if (info[8] == NULL)
		return (*tab_trot = -1);
	*tab_trot++;
}

void	put_plane(t_object **tab, char **info, int *tab_trot)
{
	tab[*tab_trot]->id = PLANE;
	tab[*tab_trot]->coordinates[0] = ft_atof(info[1]);
	tab[*tab_trot]->coordinates[1] = ft_atof(info[2]);
	tab[*tab_trot]->coordinates[2] = ft_atof(info[3]);
	tab[*tab_trot]->vector[0] = ft_atof(info[4]);
	tab[*tab_trot]->vector[1] = ft_atof(info[5]);
	tab[*tab_trot]->vector[2] = ft_atof(info[6]);
	tab[*tab_trot]->rgb[0] = ft_atoi(info[7]);
	tab[*tab_trot]->rgb[1] = ft_atoi(info[8]);
	tab[*tab_trot]->rgb[2] = ft_atoi(info[9]);
	if (info[10] == NULL)
		return (*tab_trot = -1);
	*tab_trot++;
}

void	put_cylinder(t_object **tab, char **info, int *tab_trot)
{
	tab[*tab_trot]->id = CYLINDER;
	tab[*tab_trot]->coordinates[0] = ft_atof(info[1]);
	tab[*tab_trot]->coordinates[1] = ft_atof(info[2]);
	tab[*tab_trot]->coordinates[2] = ft_atof(info[3]);
	tab[*tab_trot]->vector[0] = ft_atof(info[4]);
	tab[*tab_trot]->vector[1] = ft_atof(info[5]);
	tab[*tab_trot]->vector[2] = ft_atof(info[6]);
	tab[*tab_trot]->diameter = ft_atof(info[7]);
	tab[*tab_trot]->height = ft_atof(info[8]);
	tab[*tab_trot]->rgb[0] = ft_atoi(info[9]);
	tab[*tab_trot]->rgb[1] = ft_atoi(info[10]);
	tab[*tab_trot]->rgb[2] = ft_atoi(info[11]);
	if (info[12] == NULL)
		return (*tab_trot = -1);
	*tab_trot++;
}

void	other_case(t_object **tab, char **info, int *tab_trot, int type_obj)
{
	if (type_obj == SPHERE)
		return (put_sphere(tab, info, tab_trot));
	if (type_obj == PLANE)
		return (put_plane(tab, info, tab_trot));
	if (type_obj == CYLINDER)
		return (put_cylinder(tab, info, tab_trot));
}
