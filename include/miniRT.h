/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benji <benji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/11 13:34:38 by benji            ###   ########.fr       */
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
	struct s_object	*next;
} t_object;

typedef struct	s_head_objects
{
	t_object	*A;
	t_object	*L;
	t_object	*C;
	t_object	*first_object;
}				t_head_objects;

char	**extract_line(int fd);
void	extract_file(int fd);


//PARSER
void	parser(char **row, t_head_objects *head_of_all);
t_head_objects	*create_linked_list_object(void);
int	recognize_obj_type(char **info);
int	put_coordinates_i_objects(char *to_split, t_object *obj);
int	put_rgb_i_objects(char *to_split, t_object *obj);
int	put_normalized_vector(char *to_split, t_object *obj);
void	other_case(t_head_objects *head_of_all, char **row, int type_obj);



#endif
