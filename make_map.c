#include "libby.h"

/*#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>*/

//void print(char *str);
//void	print_err(char *func, char *err);


int	in_str(char a, char *str)
{
	int	i = 0;
	while (str && str[i])
	{
		if (str[i] == a)
			return (1);
		i++;
	}
	return (0);
}

char	**free_all_map(int fd, char **map)
{
	int	i;

	if (fd > -1)
		close(fd);
	if (map)
	{
		i = 0;
		while (map[i])
		{
			free(map[i]);
			i++;
		}
		free(map);
	}
	return (NULL);
}

char	*fill_line(char *line, int fd)
{
	int	i;
	char	a;

	i = 0;
	while (read(fd, &a, 1) && a != '\n')
	{
		line[i] = a;
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	**make_map(char *filename, int vert, int hor)
{
	int	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_err("make_map", "open file");
		return (NULL);
	}
	char	**map = malloc(sizeof(char *) * (vert + 1));
	if (!map)
	{
		print_err("make_map", "malloc");
		return (free_all_map(fd, map));
	}
	int	i = 0;
	while (i < vert)
	{
		map[i] = malloc(hor + 1);
		if (!map[i])
		{
			print_err("make_map", "malloc");
			return (free_all_map(fd, map));
		}
		map[i] = fill_line(map[i], fd);
		i++;
	}
	close(fd);
	map[i] = NULL;
	return (map);
}

int	check_map(char *filename, int vert, int hor)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_err("check_map", "open file");
		return (0);
	}
	int	i = 0;
	int	j = 0;
	char	a;
	while (read(fd, &a, 1))
	{
		if (a == '\n')
		{
			if (i == hor)
			{
				i = -1;
				j++;
				if (j == vert)
				{
					close(fd);
					return (1);
				}
			}
			else
			{
				close(fd);
				print_err("check_map", "parsing here");
				printf("stopping at i %d, j %d :: %c\n", i, j, a);
				return (0);
			}
		}
		else if (!in_str(a, "f "))
		{
			close(fd);
			print_err("check_map", "parsing");
			printf("stopping at i %d, j %d :: %c\n", i, j, a);
			return (0);
		}
		i++;
	}
	close(fd);
	return (1);
}

void	read_map(char **map)
{
	int	i = 0;
	while (map && map[i])
	{
		print(map[i]);
		print("\n");
		i++;
	}
}

char	**reverse_map(char **map, int vert, int hor)
{
	int	i;
	int	j;
	char	tmp;

	i = 0;
	while (i < vert)
	{
		j = 0;
		while (j < hor / 2)
		{
			tmp = map[i][j];
			map[i][j] = map[i][hor - 1 - j];
			map[i][hor - 1 - j] = tmp;
			j++;
		}
		i++;
	}
	return (map);
}

int	get_map_width(char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_err("get_map_width", "open file");
		return (0);
	}
	int	i = 0;
	char	buf;
	while (read(fd, &buf, 1) && buf != '\n')
		i++;
	close(fd);
	return (i);
}

/*int	main(void)
{
	int res = check_map("maps/straight.txt", 6, 25);
	res = get_map_width("maps/straight.txt");
	printf("here res :: %d\n", res);
	char **map = make_map("maps/straight.txt", 6, 25);
	if (!map)
		return (0);
	read_map(map);
	map = reverse_map(map, 6, 25);
	read_map(map);
	free_all_map(-1, map);
	return (0);
}*/
