/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:25:54 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/10 17:13:22 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	is_in_range(char *str, float min, float max)
{
	float	num;

	num = ft_atof(str);
	if (num < min || num > max)
	{
		ft_putstr_fd("Error: number is not in range.\n", 2);
		return (0);
	}
	return (1);
}

int	has_right_number_of_cells(char **row, int expected)
{
	int	i;

	i = 0;
	while (row[i])
		i++;
	if (i != expected)
	{
		ft_putstr_fd("Error: incorrect number of cells.\n", 2);
		return (0);
	}
	return (1);
}

int	has_correct_number_of_items(char *str, int expected)
{
	int		count;
	char	*ptr;

	count = 0;
	ptr = str;
	while (ptr)
	{
		ptr = ft_strchr(str, ',');
		count++;
		str = ptr + 1;
	}

	if (count != expected)
	{
		ft_putstr_fd("Error: incorrect number of items in string.\n", 2);
		return (0);		
	}
	return (1);
}

int	has_correct_separator_formatting(char *str, char separator)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (i != 0 && str[i] == separator && str[i - 1] == separator)
		{
			ft_putstr_fd("Error: 2 separators follow each other.\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	sign_format(char *str, char sign)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if (i != 0 && str[i] == sign && str[i - 1] != ',' 
				&& !ft_isdigit(str[i] - '0'))
		{
			ft_putstr_fd("Error: incorrect positioning of sign.\n", 2);
			return (0);
		}
		i++;
	}
	if (str[i] == sign)
	{
		ft_putstr_fd("Error: incorrect positioning of sign.\n", 2);
		return (0);
	}
	return (1);
}

int	has_correct_sign_formatting(char *str, bool check_negative, bool check_positive)
{
	if (check_negative && !sign_format(str, '-'))
		return (0);
	if (check_positive && !sign_format(str, '+'))
		return (0);
	return (1);	
}

int	has_valid_char(char *str, char *expected)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_strchr(expected, str[i]))	
		{
			ft_putstr_fd("Error: invalid character.\n", 2);
			return (0);			
		}
		i++;
	}
	return (1);
}

int	has_correct_colour_format(char *str)
{
	if (!has_valid_char(str, "+,0123456789"))
		return (0);
	if (!has_correct_separator_formatting(str, ','))	
		return (0);	
	if (!has_correct_sign_formatting(str, false, true))	
		return (0);		
	if (!has_correct_number_of_items(str, 3))
		return (0);
	char	**str_array = ft_split(str, ',');
	if (!str_array)
	{
		ft_putstr_fd("Error: heap allocation failed.\n", 2);
		return (0);
	}
	int	i = 0;
	while (str_array[i])
	{
		if (!is_in_range(str, 0, 255))
		{
			free_tab(str_array);
			return (0);
		}	
		i++;
	}
	free_tab(str_array);
	return (1);
}

int	has_correct_vector_format(char *str)
{
	if (!has_valid_char(str, "+-.,0123456789"))
		return (0);
	if (!has_correct_separator_formatting(str, ','))	
	 	return (0);	
	if (!has_correct_sign_formatting(str, true, true))	
	 	return (0);		
	// if (!has_correct_number_of_items(str, 3))
	// 	return (0);
	// char	**str_array = ft_split(str, ',');
	// if (!str_array)
	// {
	// 	ft_putstr_fd("Error: heap allocation failed.\n", 2);
	// 	return (0);
	// }
	// int	i = 0;
	// while (str_array[i])
	// {
	// 	if (!is_in_range(str, 0, 255))
	// 	{
	// 		free_tab(str_array);
	// 		return (0);
	// 	}	
	// 	i++;
	// }
	// free_tab(str_array);
	return (1);
}
