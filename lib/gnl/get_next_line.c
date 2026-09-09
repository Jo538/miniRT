/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchartie <jchartie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 21:42:22 by admin             #+#    #+#             */
/*   Updated: 2026/09/08 14:12:36 by jchartie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_read(int fd, char *stash)
{
	ssize_t		n;
	char		buffer[BUFFER_SIZE + 1];

	n = 1;
	ft_bzero(buffer, BUFFER_SIZE + 1);
	if (fd < 0 || read(fd, 0, 0) < 0)
		return (NULL);
	while (n > 0)
	{
		n = read(fd, buffer, BUFFER_SIZE);
		if (n == 0 && !stash)
			return (NULL);
		if (n == -1)
			return (free(stash), NULL);
		stash = ft_strjoin_gnl(stash, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
		ft_bzero(buffer, BUFFER_SIZE + 1);
	}
	return (stash);
}

char	*ft_write(char	*stash)
{
	int		n;
	int		i;
	char	*line_to_return;

	if (!stash)
		return (NULL);
	n = 0;
	while (stash[n] && stash[n] != '\n')
		n++;
	if (stash[n] == '\n')
		n++;
	line_to_return = (char *)malloc((n + 1) * sizeof(char));
	if (!line_to_return)
		return (free(stash), NULL);
	i = 0;
	while (i < n)
	{
		line_to_return[i] = stash[i];
		i++;
	}
	line_to_return[i] = '\0';
	return (line_to_return);
}

char	*ft_new_buffer(char *buffer)
{
	size_t	i;
	size_t	length;
	char	*next_buffer;

	i = 0;
	next_buffer = NULL;
	if (!buffer)
		return (NULL);
	if (!buffer[i])
		return (free(buffer), (NULL));
	while (buffer[i] && buffer[i] != '\n')
		i++;
	length = ft_strlen(buffer);
	if (length - i <= 1)
		return (free(buffer), NULL);
	next_buffer = malloc(length - i);
	if (!next_buffer)
		return (free(buffer), NULL);
	ft_strlcpy(next_buffer, &buffer[i + 1], length - i);
	return (free(buffer), next_buffer);
}

char	*get_next_line(int fd)
{
	static char	*stash = NULL;
	char		*line_to_return;

	line_to_return = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (read(fd, 0, 0) == -1)
	{
		if (stash)
		{
			free(stash);
			stash = NULL;
		}
		return (NULL);
	}
	stash = ft_read(fd, stash);
	if (!stash)
		return (NULL);
	line_to_return = ft_write(stash);
	if (!line_to_return)
		return ((stash = NULL), NULL);
	stash = ft_new_buffer(stash);
	return (line_to_return);
}