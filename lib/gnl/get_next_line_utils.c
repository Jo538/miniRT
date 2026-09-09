/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 21:42:41 by admin             #+#    #+#             */
/*   Updated: 2026/09/08 14:13:45 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin_gnl(char *s1, char*s2)
{
	size_t	len1;
	size_t	len2;
	size_t	i;
	char	*strjoin;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	strjoin = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (strjoin == NULL)
		return (free(s1), NULL);
	ft_bzero(strjoin, len1 + len2 + 1);
	i = 0;
	while (i < len1)
	{
		strjoin[i] = s1[i];
		i++;
	}
	while (i < len1 + len2)
	{
		strjoin[i] = s2[i - len1];
		i++;
	}
	strjoin[i] = '\0';
	free(s1);
	return (strjoin);
}
