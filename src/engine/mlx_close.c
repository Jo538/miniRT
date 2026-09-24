/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_close.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 18:28:21 by bribot            #+#    #+#             */
/*   Updated: 2026/09/24 09:23:01 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

#ifdef __APPLE__

// La MLX macOS ne fournit pas mlx_destroy_display : rien a liberer ici.
static void	destroy_display(void *mlx_ptr)
{
	(void)mlx_ptr;
}

#else

static void	destroy_display(void *mlx_ptr)
{
	mlx_destroy_display(mlx_ptr);
	free(mlx_ptr);
}

#endif

void	free_mlx(t_rt *rt)
{
	t_data_mlx	*data_mlx;

	data_mlx = rt->mlx;
	if (!data_mlx)
		return ;
	if (data_mlx->img)
		mlx_destroy_image(data_mlx->mlx, data_mlx->img);
	if (data_mlx->mlx_win)
		mlx_destroy_window(data_mlx->mlx, data_mlx->mlx_win);
	destroy_display(data_mlx->mlx);
	free(data_mlx);
	rt->mlx = NULL;
}

// Seule sortie de mlx_loop qui marche partout : on libere puis on exit.
// mlx_loop_end n existe que sous Linux, et sous macOS mlx_loop ne rend
// jamais la main, donc le code place apres mlx_loop ne serait jamais execute.
int	close_window(void *rt_tmp)
{
	t_rt	*rt;

	rt = (t_rt *)rt_tmp;
	free_mlx(rt);
	free_hoa(rt);
	exit(0);
	return (0);
}

int	close_window_key(int keypress, void *rt_tmp)
{
	if (keypress == KEY_ESC)
		return (close_window(rt_tmp));
	return (0);
}
