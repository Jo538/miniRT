/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:27:32 by benji             #+#    #+#             */
/*   Updated: 2026/09/23 11:33:55 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	rt_init(t_rt **rt)
{
	*rt = malloc(sizeof(t_rt));
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
	return (0);
}
