/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 21:42:49 by admin             #+#    #+#             */
/*   Updated: 2026/09/08 14:13:18 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# define MAX 4096
# if BUFFER_SIZE > MAX
#  undef BUFFER_SIZE
#  define BUFFER_SIZE MAX
# endif

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include "libft.h"

char	*get_next_line(int fd);
char	*ft_read(int fd, char *stash);
char	*ft_strjoin_gnl(char *s1, char *s2);

#endif