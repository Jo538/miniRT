/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nested_items.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 15:25:54 by jchartie          #+#    #+#             */
/*   Updated: 2026/10/05 18:47:14 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	has_correct_colour_format(char *str)
{
	char	**str_array;
	int		i;

	i = 0;
	if (!has_valid_char(str, "+,0123456789"))
		return (0);
	if (!has_correct_separator_formatting(str, ','))
		return (0);
	if (!has_correct_sign_formatting(str, false, true))
		return (0);
	if (!has_correct_number_of_items(str, 3))
		return (0);
	str_array = ft_split(str, ',');
	if (!str_array)
		return (ft_putstr_fd("Error: heap allocation failed.\n", 2), 0);
	while (str_array[i])
	{
		if (!is_in_range(str_array[i], 0, 255))
			return (free_tab(str_array), 0);
		i++;
	}
	free_tab(str_array);
	return (1);
}

int	has_correct_coordinates_format(char *str)
{
	char	**str_array;
	int		i;

	i = 0;
	if (!has_valid_char(str, "+-.,0123456789"))
		return (0);
	if (!has_correct_separator_formatting(str, ','))
		return (0);
	if (!has_correct_sign_formatting(str, true, true))
		return (0);
	if (!has_correct_number_of_items(str, 3))
		return (0);
	str_array = ft_split(str, ',');
	if (!str_array)
	{
		ft_putstr_fd("Error: heap allocation failed.\n", 2);
		return (0);
	}
	while (str_array[i])
	{
		if (!has_correct_float_formatting(str_array[i]))
			return (free_tab(str_array), 0);
		i++;
	}
	return (free_tab(str_array), 1);
}

int	has_correct_vector_format(char *str)
{
	char	**str_array ;
	int		i;

	i = 0;
	if (!has_valid_char(str, "+-.,0123456789"))
		return (0);
	if (!has_correct_separator_formatting(str, ','))
		return (0);
	if (!has_correct_sign_formatting(str, true, true))
		return (0);
	if (!has_correct_number_of_items(str, 3))
		return (0);
	str_array = ft_split(str, ',');
	if (!str_array)
		return (ft_putstr_fd("Error: heap allocation failed.\n", 2), 0);
	while (str_array[i])
	{
		if (!has_correct_float_formatting(str_array[i])
			| !is_in_range(str_array[i], -1, +1))
			return (free_tab(str_array), 0);
		i++;
	}
	return (free_tab(str_array), 1);
}
