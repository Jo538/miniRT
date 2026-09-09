/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:28:28 by bribot            #+#    #+#             */
/*   Updated: 2026/09/09 18:37:52 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"
#include "libft.h"

float	ft_atof(const char *s)
{
	float	to_return;
	float	tmp;
	int		i;

	i = 0;
	if (!s)
		return (0);
	to_return = (float)ft_atoi(s);
	while (s[0] && s[0] != '.')
		s = s + 1;
	if (s[i + 1])
		s = s + 1;
	tmp = (float)ft_atoi(s);
	while (tmp > 1)
		tmp = tmp / 10;
	to_return = to_return + tmp;
	return (to_return);
}
