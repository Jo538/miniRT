/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/08 13:37:28 by benji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include "libft.h"

typedef enum e_id
{
	AMBIENT_LIGHTING,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
	INVALID
} t_id;

//dans le tab objects, ambient lighning prends la place 0, CAM la place 1 et LIGHT la 2

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
	int	is_set;
} t_object;

#endif
