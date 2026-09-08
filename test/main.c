/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 15:19:59 by jchartie          #+#    #+#             */
/*   Updated: 2026/09/08 16:22:58 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int main(void)
{
	int fd = open("/home/jchartie/42/rank04/miniRT/scenes/plane.rt", O_RDONLY);
	test_extract_line(fd, (char *[]){"A", "0.2", "255,255,255", NULL});
}
