/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:02:06 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/25 15:08:52 by admin            ###   ########.fr       */
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

int	test_unitaire(void);

// DEFINITION DES TAILLES DE L ECRAN CENTRE EN 0

# ifndef X_MAX
#  define X_MAX 1920
# endif

# ifndef Y_MAX
#  define Y_MAX 1080
# endif

// LE KEYCODE D ESC DEPEND DE L OS :
// keysym X11 sous Linux, virtual key code Cocoa sous macOS
# ifdef __APPLE__
#  define KEY_ESC 53
# else
#  define KEY_ESC 65307
# endif

# define SPECULAR_REFLECTIVITY_COEFF 0.5
# define SHININESS_EXPONENT 32

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
typedef struct s_data_mlx
{
	void	*mlx;
	void	*img;
	void	*mlx_win;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data_mlx;

typedef struct s_object
{
	t_id id; //peut etre changer a int si fonctionne pas avec t_id
	double ratio;
	double	rgb[3];
	double coordinates[3];
	double vector[3];
	int fov;
	double diameter;
	double height;
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

typedef struct	s_rt
{
	t_object	*A;
	t_object	*L;
	t_object	*C;
	t_object	*first_object;
	t_data_mlx	*mlx;
	t_viewport	*viewport;
	int			err;
}				t_rt;

typedef struct s_ray
{
	double	origin[3];
	double	direction[3];
}	t_ray;

typedef	struct	s_tridouble
{
	double	a;
	double	b;
	double	c;
}				t_tridouble;

int	extract_file(int fd, t_rt *rt);
int	recognize_obj_type(char **info);
void	normalise_vector(double *vector);
int	parse_viewport(t_rt *rt);
void	find_ray_direction(double pixel_x, double pixel_y, t_rt *rt, t_ray *ray);
void	run_engine(t_rt *rt);
int	solver(t_ray *ray, t_rt *rt, double *intersection);
void	colour_pixel(t_rt *rt, int col, int row, double *intersection, t_ray *ray);
void	cross_product(double *vector_1, double *vector_2, double *to_fill);
void	vector_subst(double *vector_1, double *vector_2, double *to_fill);
void	normalise_color(double *normalised_colour, double *src);
void	component_wise_multiplication(double *vector_1, double *vector_2, double *to_fill);
void	add_vectors(double *vector_1, double *vector_2, double *to_fill);
void	compute_shaded_colour(t_rt *rt, double *intersection, t_ray *ray, double *shaded_rgb);

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



//PARSER
int	parse(int fd, t_rt *rt);
void	parse_line(char **row, t_rt *rt);
int	rt_init(t_rt **rt);
int	recognize_obj_type(char **info);
int	put_coordinates_i_objects(char *to_split, t_object *obj);
int	put_rgb_i_objects(char *to_split, t_object *obj);
int	put_normalized_vector(char *to_split, t_object *obj);
void	other_case(t_rt *rt, char **row, int type_obj);
void	free_hoa(t_rt *head);

//CALCULATOR
double	make_dot_product(double first[3], double second[3]);
void	scalar_product(double scalar, double *vector_2, double *to_fill);

//MLX
void	mlx_initialization(t_rt *rt);
void	mlx_run(t_rt *rt);
void	free_mlx(t_rt *rt);
int		close_window(void *rt_tmp);
int		close_window_key(int keypress, void *rt_tmp);

#endif
