/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/08 16:06:29 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include "libft.h"
# include "get_next_line.h"

typedef enum e_id
{
	AMBIENT_LIGHTING,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER
} t_id;

typedef struct s_object
{
	t_id id; //peutetre changer a int si fonctionne pas avec t_id
	float ratio;
	int	rgb[3];
	float coordinates[3];
	float vector[3];
	int fov;
	float diameter;
	float height;
} t_object;

char	**extract_line(int fd);

#endif