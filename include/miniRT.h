/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/09 12:07:48 by jchartie         ###   ########.fr       */
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

char	**extract_line(int fd);
void	extract_file(int fd);

#endif
