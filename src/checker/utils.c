/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:25:54 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/15 10:47:31 by admin            ###   ########.fr       */
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

int	has_correct_separator_formatting(char *str, char separator)
{
	int	i;
	int	size;

	i = 0;
	size = ft_strlen(str) - 1;
	while (str[i])
	{
		if ((i == 0 && str[i] == separator) 
			|| (i == size && str[i] == separator)
				|| (i != 0 && str[i] == separator && str[i - 1] == separator))
		{
			ft_putstr_fd("Error: separators are not correctly formatted.\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	sign_format(char *str, char sign)
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
