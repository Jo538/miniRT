/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:27:32 by benji             #+#    #+#             */
/*   Updated: 2026/10/05 19:22:02 by bribot           ###   ########.fr       */
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
	(*rt)->a = NULL;
	(*rt)->c = NULL;
	(*rt)->l = NULL;
	(*rt)->first_object = NULL;
	(*rt)->viewport = NULL;
	(*rt)->mlx = NULL;
	(*rt)->err = 0;
	return (0);
}
