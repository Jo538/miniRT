/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:28:28 by bribot            #+#    #+#             */
/*   Updated: 2026/09/10 15:43:25 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static float	parse_fraction(const char *s, int *i)
{
	float	frac;
	float	divisor;

	if (s[*i] != '.')
		return (0.0f);
	(*i)++;
	frac = 0.0f;
	divisor = 1.0f;
	while (s[*i] >= '0' && s[*i] <= '9')
	{
		frac = frac * 10.0f + (s[*i] - '0');
		divisor *= 10.0f;
		(*i)++;
	}
	return (frac / divisor);
}

float	ft_atof(const char *s)
{
	int		i;
	float	sign;
	float	num;

	i = 0;
	sign = 1.0f;
	num = 0.0f;
	while (s[i] == ' ' || (s[i] >= '\t' && s[i] <= '\r'))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign = -1.0f;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		num = num * 10.0f + (s[i] - '0');
		i++;
	}
	num += parse_fraction(s, &i);
	return (sign * num);
}
