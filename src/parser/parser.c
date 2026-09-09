/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:18:53 by benji             #+#    #+#             */
/*   Updated: 2026/09/09 17:03:55 by benji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_object	*parse_camera(char **row)
{
	t_object	*to_return;
	char		**tmp;

	to_return = malloc(sizeof(t_object));
	if (!to_return)
		return (NULL);
	to_return->id = CAMERA;
	tmp = ft_split(row[1], ',');
	if (!tmp)
		return (NULL);

}

t_object *parse_ambient_lighting(char **row)
{
	t_object	*to_return;
	char		**tmp;
	int			i;

	to_return = malloc(sizeof(t_object));
	if (!to_return)
		return (NULL);
	to_return->id = AMBIENT_LIGHTING;
	to_return->ratio = ft_atof(row[1]);
	tmp = ft_split(row[2], ',');
	if (!tmp)
		return (NULL);
	to_return->rgb[0] = ft_atoi(tmp[0]);
	to_return->rgb[1] = ft_atoi(tmp[1]);
	to_return->rgb[2] = ft_atoi(tmp[2]);
	i = -1;
	while (tmp[++i])
		free(tmp[i]);
	free(tmp);
	return (to_return);
}

void	acl_case(t_head_objects *head_of_all, char **row, int type_obj)
{
	if (type_obj == AMBIENT_LIGHTING)
		head_of_all->A = parse_ambient_lighting(row); //faire le cas ou le malloc ou le split echoue
	if (type_obj == CAMERA)
		head_of_all->C = parse_camera(row);
}

void	parser(char **row, t_head_objects *head_of_all)
{
	int type_obj;

	type_obj = recognize_obj_type(row);
	if (type_obj == AMBIENT_LIGHTING || type_obj == CAMERA || type_obj == LIGHT)
		acl_case(head_of_all, row, type_obj);
	printf("l id est : %d", head_of_all->A->rgb[1]);
}
