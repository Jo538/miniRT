/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:18:53 by benji             #+#    #+#             */
/*   Updated: 2026/09/11 13:33:33 by benji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_object	*parse_camera(char **row)
{
	t_object	*to_return;

	to_return = malloc(sizeof(t_object));
	if (!to_return)
		return (NULL);
	to_return->id = CAMERA;
	if (put_coordinates_i_objects(row[1], to_return) == 1)
		return(free(to_return), NULL);
	if (put_normalized_vector(row[2], to_return) == 1)
		return(free(to_return), NULL);
	to_return->fov = ft_atoi(row[3]);
	return (to_return);
}

t_object	*parse_ambient_lighting(char **row)
{
	t_object	*to_return;

	to_return = malloc(sizeof(t_object));
	if (!to_return)
		return (NULL);
	to_return->id = AMBIENT_LIGHTING;
	to_return->ratio = ft_atof(row[1]);
	if (put_rgb_i_objects(row[2], to_return) == 1)
		return(free(to_return), NULL);
	return (to_return);
}

t_object	*parse_light(char **row)
{
	t_object	*to_return;

	to_return = malloc(sizeof(t_object));
	if (!to_return)
		return (NULL);
	to_return->id = LIGHT;
	if (put_coordinates_i_objects(row[1], to_return) == 1)
		return(free(to_return), NULL);
	to_return->ratio = ft_atof(row[2]);
	if (put_rgb_i_objects(row[3], to_return) == 1)
		return(free(to_return), NULL);
	return (to_return);
}

void	acl_case(t_head_objects *head_of_all, char **row, int type_obj)
{
	if (type_obj == AMBIENT_LIGHTING)
		head_of_all->A = parse_ambient_lighting(row); //faire le cas ou le malloc ou le split echoue
	if (type_obj == CAMERA)
		head_of_all->C = parse_camera(row);
	if (type_obj == LIGHT)
		head_of_all->L = parse_light(row);
}

void	parser(char **row, t_head_objects *head_of_all)
{
	int type_obj;

	type_obj = recognize_obj_type(row);
	if (type_obj == AMBIENT_LIGHTING || type_obj == CAMERA || type_obj == LIGHT)
		acl_case(head_of_all, row, type_obj);
	// if (type_obj == CAMERA)
	// 	printf("l id est : %f\n", head_of_all->C->vector[2]);
	// if (type_obj == LIGHT)
	// 	printf("LIGHT = ratio %f\n", head_of_all->L->ratio);
	else
		other_case(head_of_all, row, type_obj);
}
