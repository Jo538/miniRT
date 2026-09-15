/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:25:54 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/15 11:42:30 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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

static int	has_correct_float_formatting_bis(char *str, int i, int size, int count)
{
	if (count > 1)
		return (0);
	if (i == 0 && ft_isdigit(str[i + 1]))
		return (1);
	if (i != 0 && ft_isdigit(str[i - 1]))
		return (1);
	if (i != 0 && i != size && ft_isdigit(str[i - 1]) && ft_isdigit(str[i + 1]))
		return (1);
	return (0);
}
	
int	has_correct_float_formatting(char *str)
{
	int	i;
	int	size;
	int	count;

	i = 0;
	size = ft_strlen(str) - 1;
	count = 0;
	while (str[i])
	{
		if (str[i] == '.')
		{
			if (!has_correct_float_formatting_bis(str, i, size, ++count))
			{
				ft_putstr_fd("Error: incorrect float formatting.\n", 2);
				return (0);					
			}
		}
		i++;
	}
	return (1);
}