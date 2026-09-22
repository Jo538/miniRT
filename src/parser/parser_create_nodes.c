/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_create_nodes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:27:32 by benji             #+#    #+#             */
/*   Updated: 2026/09/22 14:57:13 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	rt_init(t_head_objects **rt)
{
	*rt = malloc(sizeof(t_head_objects));
	if (!(*rt))
	{
		ft_putstr_fd("Error: dynamic allocation failed.\n", 2);
		return (1);		
	}
	(*rt)->A = NULL;
	(*rt)->C = NULL;
	(*rt)->L = NULL;
	(*rt)->first_object = NULL;
	(*rt)->viewport = NULL;
	(*rt)->err = 0;
	return (1);
}
