/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/15 11:16:35 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <stdbool.h>
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

int	extract_file(int fd);
int	recognize_obj_type(char **info);

// CHECKER
int	is_correct(char **row);
int	sphere(char ** row);
int	cylinder(char ** row);
int	has_correct_colour_format(char *str);
int	has_correct_vector_format(char *str);
int	has_correct_float_formatting(char *str);
int	is_in_range(char *str, float min, float max);
int	has_right_number_of_cells(char **row, int expected);
int	has_correct_number_of_items(char *str, int expected);
int	has_correct_separator_formatting(char *str, char separator);
int	has_correct_sign_formatting(char *str, bool check_negative, bool check_positive);
int	has_valid_char(char *str, char *expected);

// UTILS
void	free_tab(char **tab);

#endif
