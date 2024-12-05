/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mconde-s <mconde-s@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:12:15 by mconde-s          #+#    #+#             */
/*   Updated: 2024/12/05 21:56:37 by mconde-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	len_s;

	len_s = ft_strlen(s);
	if (!s)
		return (NULL);
	if (len > __SIZE_MAX__)
		len = __SIZE_MAX__;
	if (start >= len_s)
		return (ft_strdup(""));
	if (len > len_s - start)
		len = len_s - start;
	sub = malloc((len * sizeof(char)) + 1);
	if (!sub)
		return (NULL);
	ft_strlcpy(sub, (s + start), len + 1);
	return (sub);
}
char	*ft_strjoin(char *s1, char *s2)
{
	size_t	lens1;
	size_t	lens2;
	char	*result;

	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	result = malloc(lens1 + lens2 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, lens1);
	// free(s1);
	ft_memcpy(result + lens1, s2, lens2 + 1);
	// free(s2);
	return (result);
}

char	*ft_read(int fd, char **result)
{
	char	*buffer;
	char	*line;
	char	*tmp;
	ssize_t	nbr;

	if (BUFFER_SIZE < 0)
		return (0);
	nbr = 1;
	buffer = NULL;
	line = ft_strdup("");
	tmp = NULL;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free(line), NULL);
	while (1)
	{
		nbr = read(fd, buffer, BUFFER_SIZE);
		buffer[nbr] = '\0';
		tmp = ft_strjoin(line, buffer);
		if (tmp)
			line = (free(line), tmp);
		if (nbr <= 0)
		{
			free(buffer);
			*result = line;
			return (*result);
		}

		if (ft_strchr(line, '\n'))
			break ;
	}

	*result = (free(buffer), line);
	return (*result);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*tmp;
	char		*buffer;
	static char	*temp = NULL;
	size_t		i;

	i = 0;
	line = ft_strdup("");
	buffer = NULL;
	tmp = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(temp);
		free(line);
		free(tmp);
		free(buffer);
		return (NULL);
	}
	if (!ft_read(fd, &buffer))
	{
		if (temp)
		{
			free(line);
			free(buffer);
			free(temp);
		}
		temp = NULL;
		return (NULL);
	}
	else
	{

		while (buffer[i] != '\0' && buffer[i] != '\n')
			i++;
		tmp = line;
		line = ft_substr(buffer, 0, i);
		free(tmp);
		if (!line)
			return (free(temp), free(buffer), NULL);
		if (ft_strchr(buffer, '\n'))
		{
			free(temp);
			temp = ft_substr(buffer, i, ft_strlen(buffer) - i);
			tmp = ft_strjoin(line, temp);
			line = (free(line), tmp);
		}
		else if (buffer[i] == '\0' && i == 0)
		{
			free(temp);
			free(line);
			free(buffer);
			temp = NULL;
			return (NULL);
		}
		free(buffer);
	}
	return (line);
}

int	main(void)
{
	int fd = open("prueba.txt", O_RDONLY);
	// int fd = 35;//open("get_next_line.c", O_RDONLY);
	if (fd < 0)
		printf("error al abrir el archivo");
	char *line;

	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}