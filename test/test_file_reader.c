/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_file_reader.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 15:12:36 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/09 12:12:41 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"



static int	free_all(char **result)
{
	int i = 0;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (0);
}

void test_extract_line(int fd)
{
	int i = 0;

	char **line = extract_line(fd);
	while (line[i])
	{
		printf("%s ", line[i]);
		i++;
	}
	free_all(line);
}





