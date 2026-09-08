/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_file_reader.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 15:12:36 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/08 16:26:56 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void test_extract_line(int fd, char **expected)
{
	int i = 0;

	char **line = extract_line(fd);
	while (line[i])
	{
		if (strcmp(line[i], expected[i]))
		{
			printf("FAILURE: %s, %s\n", line[i], expected[i]);
			return ;	
		}
		i++;
	}
	printf("SUCCESS\n");
}