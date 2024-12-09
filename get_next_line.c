/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mconde-s <mconde-s@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:12:15 by mconde-s          #+#    #+#             */
/*   Updated: 2024/12/09 21:33:02 by mconde-s         ###   ########.fr       */
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
	ft_memcpy(result + lens1, s2, lens2 + 1);
	return (result);
}

char	*ft_line(char *buffer, char *line, char **temp, char *tmp)
{
	size_t	i;

	i = 0;
	while (buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	tmp = line;
	free(tmp);
	line = ft_substr(buffer, 0, i);
	if (!line)
		return (free(*temp), free(buffer), NULL);
	if (ft_strchr(buffer, '\n'))
	{
		free(*temp);
		*temp = ft_substr(buffer, i, ft_strlen(buffer) - i);
		tmp = ft_strjoin(line, *temp);
		line = (free(line), tmp);
	}
	else if (buffer[i] == '\0' && i == 0)
		return (*temp = (free(*temp), free(line), free(buffer), NULL), NULL);
	free(buffer);
	return (line);
}

char	*ft_read(int fd, char **result)
{
	char	*buffer;
	char	*line;
	char	*tmp;
	ssize_t	nbr;

	nbr = 1;
	line = ft_strdup("");
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free(line), NULL);
	buffer[0] = '\0';
	while (1)
	{
		nbr = read(fd, buffer, BUFFER_SIZE);
		if (nbr < 0)
			return (free(buffer), free(line), NULL);
		if (nbr == 0)
			return (*result = (free(buffer), line), *result);
		buffer[nbr] = '\0';
		tmp = ft_strjoin(line, buffer);
		line = (free(line), tmp);
		if (ft_strchr(line, '\n'))
			break ;
	}
	return (*result = (free(buffer), line), *result);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*tmp;
	char		*buffer;
	static char	*temp = NULL;

	line = NULL;
	buffer = NULL;
	tmp = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(line), NULL);
	if (!ft_read(fd, &buffer))
	{
		temp = (free(line), free(buffer), free(temp), NULL);
		return (NULL);
	}
	else
		line = ft_line(buffer, line, &temp, tmp);
	return (line);
}

// int	main(void)
// {
// 	int fd = open("41_with_nl.txt", O_RDONLY);
// 	// int fd = 35;//open("get_next_line.c", O_RDONLY);
// 	if (fd < 0)
// 		printf("error al abrir el archivo");
// 	char *line;

// 	while ((line = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s", line);
// 		free(line);
// 	}
// 	close(fd);
// 	return (0);
// }