/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admin <admin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:28:28 by bribot            #+#    #+#             */
/*   Updated: 2026/09/10 11:07:59 by admin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"
#include "libft.h"

float	ft_atof(const char *s)
{
	float	to_return;
	float	tmp;
	int		i;
	float	sign;

	i = 0;
	sign = 1;
	if (!s)
		return (0);
	to_return = (float)ft_atoi(s);
	if (to_return < 0)
	{
		sign = -1;
		to_return = sign * to_return;
	}
	while (s[0] && s[0] != '.')
		s = s + 1;
	if (s[i + 1])
		s = s + 1;
	tmp = (float)ft_atoi(s);
	while (tmp > 1)
		tmp = tmp / 10;
	to_return = to_return + tmp;
	return (sign * to_return);
}
