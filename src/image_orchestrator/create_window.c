/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_window.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 18:28:21 by bribot            #+#    #+#             */
/*   Updated: 2026/09/23 11:29:45 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

char	*attrib_color(t_rt *rt)
{
	int			pixel;
	int			color;
	int			x;
	int			y;

	y = 0;
	x = 0;
	pixel = 0;
				color = 1;

	while (y < Y_MAX)
	{
		while (x < X_MAX)
		{
			color = (x * x) + (y * y); //juste un exemple
			rt->mlx->addr[pixel++] = (color);
			rt->mlx->addr[pixel++] = (color >> 8) & 0xFF;
			rt->mlx->addr[pixel++] = (color >> 16) & 0xFF;
			rt->mlx->addr[pixel++] = (color >> 24);
			x++;
		}
		y++;
		x = 0;
	}
	return (rt->mlx->addr);
}

int	close_window_redcross(void *rt_tmp)
{
	t_rt	*rt;

	rt = (t_rt *)rt_tmp;
	mlx_loop_end(rt->mlx->mlx);
	return (0);
}

int	close_window_prop(int keypress, void *rt_tmp)
{
	t_rt	*rt;

	rt = (t_rt *)rt_tmp;
	if (keypress == 65307)
		mlx_loop_end(rt->mlx->mlx);
	return (0);
}

void	mlx_hook_orchestrator(t_rt *rt)
{
	mlx_key_hook(rt->mlx->mlx_win, close_window_prop, rt);
	// mlx_hook(rt->mlx->mlx_win, 17, 1L << 0, ((int (*)(void *))close_window_redcross), rt); //CA marche pas sfaut trouver un truc

}

void	window_orchestrator(t_rt *rt) //en vrai je vais devoir corriger pour que ce soit plus propre
{
	t_data_mlx	*data_mlx;

	data_mlx = malloc (sizeof(t_data_mlx));
	if (!data_mlx)
		return ((void)(rt->err = 1));
	rt->mlx = data_mlx;
	data_mlx->mlx = mlx_init();
	data_mlx->img = mlx_new_image(data_mlx->mlx, X_MAX, Y_MAX);
	data_mlx->mlx_win = mlx_new_window(data_mlx->mlx, X_MAX, Y_MAX, "TEST");
	data_mlx->addr = mlx_get_data_addr(data_mlx->img, &data_mlx->bits_per_pixel,
						&data_mlx->line_lenght, &data_mlx->endian);

	data_mlx->addr = attrib_color(rt);
	mlx_put_image_to_window(data_mlx->mlx, data_mlx->mlx_win, data_mlx->img, 0, 0);
	mlx_hook_orchestrator(rt);

	mlx_loop(data_mlx->mlx);
	mlx_destroy_window(data_mlx->mlx, data_mlx->mlx_win);
	mlx_destroy_image(data_mlx->mlx, data_mlx->img);
	mlx_destroy_display(data_mlx->mlx);
	free(data_mlx->mlx);
	free(data_mlx);
}
