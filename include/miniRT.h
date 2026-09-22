/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/22 19:18:13 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <stdbool.h>
# include <math.h>
# include "libft.h"
# include "get_next_line.h"
# include "mlx.h"

// DEFINITION DES TAILLES DE L ECRAN CENTRE EN 0

# ifndef X_MAX
#  define X_MAX 1920
# endif

# ifndef Y_MAX
#  define Y_MAX 1080
# endif


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

typedef struct s_viewport
{
	double	width;
	double	height;
	double	forward[3];
	double	right[3];
	double	up[3];
}	t_viewport;

typedef struct	s_head_objects
{
	t_object	*A;
	t_object	*L;
	t_object	*C;
	t_object	*first_object;
	t_viewport	*viewport;
	int			err;
}				t_head_objects;

typedef struct s_ray
{
	double	origin[3];
	double	direction[3];
}	t_ray;


int	extract_file(int fd, t_head_objects *head_of_all);
int	recognize_obj_type(char **info);
void	normalise(double *vector);

// CHECKER
int	is_correct(char **row);
int	sphere(char ** row);
int	cylinder(char ** row);
int	has_correct_colour_format(char *str);
int	has_correct_vector_format(char *str);
int	has_correct_float_formatting(char *str);
int	has_correct_coordinates_format(char *str);
int	is_in_range(char *str, float min, float max);
int	has_right_number_of_cells(char **row, int expected);
int	has_correct_number_of_items(char *str, int expected);
int	has_correct_separator_formatting(char *str, char separator);
int	has_correct_sign_formatting(char *str, bool check_negative, bool check_positive);
int	has_valid_char(char *str, char *expected);

// UTILS
void	free_tab(char **tab);
typedef struct s_data_mlx
{
	void	*mlx;
	void	*img;
	void	*mlx_win;
	char	*addr;
	int		bits_per_pixel;
	int		line_lenght;
	int		endian;
}				t_data_mlx;


//PARSER
int	parse(int fd, t_head_objects *head_of_all);
void	parse_line(char **row, t_head_objects *head_of_all);
int	rt_init(t_head_objects **rt);
int	recognize_obj_type(char **info);
int	put_coordinates_i_objects(char *to_split, t_object *obj);
int	put_rgb_i_objects(char *to_split, t_object *obj);
int	put_normalized_vector(char *to_split, t_object *obj);
void	other_case(t_head_objects *head_of_all, char **row, int type_obj);
void	free_hoa(t_head_objects *head);

//MLX
void	window_orchestrator(t_head_objects *head_of_all);


#endif
