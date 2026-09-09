/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:25:54 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/09 16:37:28 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	is_capital(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < 'A' && str[i] > 'Z')
			return (0);
		i++;
	}
	return (1);
}

int	is_in_range(char *str, float min, float max)
{
	float	num;

	num = ft_atof(str);
	if (num < min || num > max)
		return (0);
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

int	count_virgule(char *str, int count)
{
	char	*ptr;

	while (1)
	{
		ptr = ft_strchr(str, ',');
		if (!ptr)
			break ;
		count++;		
	}
	return (count);
}

int	has_correct_colour_format(char *str)
{
	if (count_virgule(str, 0) != 3)
	{
		ft_putstr_fd("Error: too many virgules in RGB.\n", 2);		
		return (0);		
	}
	
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
			ft_putstr_fd("Error: RGB is not in range.\n", 2);
			return (0);
		}	
		i++;
	}
	free_tab(str_array);
	return (1);
}
