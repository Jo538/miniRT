/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_create_nodes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:27:32 by benji             #+#    #+#             */
/*   Updated: 2026/09/15 12:52:52 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_head_objects	*create_linked_list_object(void)
{
	t_head_objects *to_return;

	to_return = malloc(sizeof(t_head_objects));
	if (!to_return)
		return (NULL);
	to_return->A = NULL;
	to_return->C = NULL;
	to_return->L = NULL;
	to_return->first_object = NULL;
	to_return->err = 0;
	return (to_return);
}
