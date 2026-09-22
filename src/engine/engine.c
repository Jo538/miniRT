/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:25 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/22 20:16:54 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	run_engine(t_rt *rt)
{
	int		col;
	int		row;
	t_ray	ray;

	row = 0;
	while (row < Y_MAX)
	{
		col = 0;
		while (col < X_MAX)
		{
			find_ray_direction(row, col, rt, &ray);
			//Benji's quadratic solving for sphere
			col++;
		}
		row++;
	}
}
