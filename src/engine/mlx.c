/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 18:28:21 by bribot            #+#    #+#             */
/*   Updated: 2026/09/24 09:23:01 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	mlx_hook_orchestrator(t_rt *rt)
{
	mlx_key_hook(rt->mlx->mlx_win, close_window_key, rt);
	mlx_hook(rt->mlx->mlx_win, 17, 1L << 0, close_window, rt);
}

void	mlx_initialization(t_rt *rt)
{
	t_data_mlx	*data_mlx;

	data_mlx = malloc(sizeof(t_data_mlx));
	if (!data_mlx)
		return ((void)(rt->err = 1));
	rt->mlx = data_mlx;
	data_mlx->mlx = mlx_init();
	data_mlx->img = mlx_new_image(data_mlx->mlx, X_MAX, Y_MAX);
	data_mlx->mlx_win = mlx_new_window(data_mlx->mlx, X_MAX, Y_MAX, "TEST");
	data_mlx->addr = mlx_get_data_addr(data_mlx->img, &data_mlx->bits_per_pixel,
			&data_mlx->line_length, &data_mlx->endian);
}

void	mlx_run(t_rt *rt)
{
	t_data_mlx	*data_mlx;

	data_mlx = rt->mlx;
	mlx_put_image_to_window(data_mlx->mlx, data_mlx->mlx_win, data_mlx->img,
		0, 0);
	mlx_hook_orchestrator(rt);
	mlx_loop(data_mlx->mlx);
}
