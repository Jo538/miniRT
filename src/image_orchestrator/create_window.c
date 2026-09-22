/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_window.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 18:28:21 by bribot            #+#    #+#             */
/*   Updated: 2026/09/22 20:23:27 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	close_window_prop(int keypress, void *data_mlx_tmp)
{
	t_data_mlx	*data_mlx;

	data_mlx = data_mlx_tmp;
	if (keypress == 65307)
	{
		mlx_destroy_window(data_mlx->mlx, data_mlx->mlx_win);
		mlx_destroy_image(data_mlx->mlx, data_mlx->img);
		//mlx_destroy_display(data_mlx->mlx);
		free(data_mlx->mlx);
		free(data_mlx);
	}
	return (0);
}

void	mlx_hook_orchestrator(t_data_mlx *data_mlx, t_rt *rt)
{
	(void)rt;
	mlx_key_hook(data_mlx->mlx_win, close_window_prop, data_mlx);
}

void	window_orchestrator(t_rt *rt)
{
	t_data_mlx	*data_mlx;

	data_mlx = malloc (sizeof(t_data_mlx));
	if (!data_mlx)
		return ((void)(rt->err = 1));
	data_mlx->mlx = mlx_init();
	data_mlx->img = mlx_new_image(data_mlx->mlx, X_MAX, Y_MAX);
	data_mlx->mlx_win = mlx_new_window(data_mlx->mlx, X_MAX, Y_MAX, "TEST");
	data_mlx->addr = mlx_get_data_addr(data_mlx->img, &data_mlx->bits_per_pixel,
						&data_mlx->line_lenght, &data_mlx->endian);

	//FONCTION QUI PUT LES PIXELS

	mlx_hook_orchestrator(data_mlx, rt);

	mlx_loop(data_mlx->mlx);
}
