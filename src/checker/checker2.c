/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:25:54 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/10 14:48:51 by admin            ###   ########.fr       */
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

int	has_correct_virgule_formatting(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (i != 0 && str[i] == ',' && str[i - 1] == ',')
		{
			ft_putstr_fd("Error: 2 virgules follow each other.\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	has_correct_sign_formatting(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if (i != 0 && str[i] == '+' && str[i - 1] != ',' 
				&& !ft_isdigit(str[i] - '0'))
		{
			ft_putstr_fd("Error: incorrect positioning of + sign.\n", 2);
			return (0);
		}
		i++;
	}
	if (str[i] == '+')
	{
		ft_putstr_fd("Error: incorrect positioning of + sign.\n", 2);
		return (0);
	}		
	return (1);
}

int	has_valid_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ',' && str[i] != '+' && (str[i] < '0' || str[i] > '9'))	
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
	if (!has_valid_char(str))
		return (0);
	if (!has_correct_virgule_formatting(str))	
		return (0);	
	if (!has_correct_sign_formatting(str))	
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
