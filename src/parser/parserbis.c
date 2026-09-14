/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserbis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 13:29:51 by benji             #+#    #+#             */
/*   Updated: 2026/09/11 13:57:29 by benji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_object	*parse_sphere(char **row)
{
	t_object	*to_return;

	to_return = malloc(sizeof(t_object));
	if (!to_return)
		return (NULL);
	to_return->id = SPHERE;
	if (put_coordinates_i_objects(row[1], to_return) == 1)
		return(free(to_return), NULL);
	to_return->diameter = ft_atof(row[2]);
	if (put_rgb_i_objects(row[3], to_return) == 1)
		return(free(to_return), NULL);
	return (to_return);
}

t_object	*parse_plane(char **row)
{
	t_object	*to_return;

	to_return = malloc(sizeof(t_object));
	if (!to_return)
		return (NULL);
	to_return->id = PLANE;
	if (put_coordinates_i_objects(row[1], to_return) == 1)
		return(free(to_return), NULL);
	if (put_normalized_vector(row[2], to_return) == 1)
		return(free(to_return), NULL);
	if (put_rgb_i_objects(row[3], to_return) == 1)
		return(free(to_return), NULL);
	return (to_return);
}

t_object	*parse_cylinder(char **row)
{
	t_object	*to_return;

	to_return = malloc(sizeof(t_object));
	if (!to_return)
		return (NULL);
	to_return->id = CYLINDER;
	if (put_coordinates_i_objects(row[1], to_return) == 1)
		return(free(to_return), NULL);
	if (put_normalized_vector(row[2], to_return) == 1)
		return(free(to_return), NULL);
	to_return->diameter = ft_atof(row[3]);
	to_return->height = ft_atof(row[4]);
	if (put_rgb_i_objects(row[5], to_return) == 1)
		return(free(to_return), NULL);
	return (to_return);
}

t_object	*create_init_objects(char **row, int type_obj)
{
	t_object	*to_return;

	if (type_obj == SPHERE)
		to_return = parse_sphere(row);
	if (type_obj == PLANE)
		to_return = parse_plane(row);
	if (type_obj == CYLINDER)
		to_return = parse_cylinder(row);
	return (to_return);
}

void	other_case(t_head_objects *head_of_all, char **row, int type_obj)
{
	t_object *trot;

	if (head_of_all->first_object == NULL)
	{
		head_of_all->first_object = create_init_objects(row, type_obj);
		return ;
	}
	trot = head_of_all->first_object;
	while (trot->next)
			trot = trot->next;
	trot->next = create_init_objects(row, type_obj);
	// if (!trot->next)
	// fonction qui free la liste chainee ou alors peut etre une variable d erreur dans head_of_all

}
