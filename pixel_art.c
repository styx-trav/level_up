#include "libby.h"

void	map_to_file(int fd, char **map)
{
	int	i = 0;
	int	j;

	while (map[i])
	{
		

char	**empty_map(int hor, int vert)
{
	int	i = 0;
	int	j;
	char	**map = malloc(sizeof(char *) * (vert + 1));

	if (!map)
	{
		print_err("empty map", "malloc");
		return (NULL);
	}
	while (i < vert)
	{
		map[i] = malloc(hor + 1);
		j = 0;
		if (map[i])
		{
			free_all(-2, map);
			print_err("empty map", "malloc");
			return (NULL);
		}
		while (j < hor)
		{
			map[i][j] = ' ';
			j++;
		}
		map[i][j] = '\0';
		i++;
	}
	map[i] = NULL;
	return (map);
}

int	pixel_art(int code, int x, int y, t_str *all)
{
	if (button != 1)
		return (0);
	map[y / 16][x / 16] = 'f';
	pixelate(mlx->current_img, (x / 16) * 16, (y / 16) * 16, 0x00FFFFFF);
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, 0, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (0);
	t_str *all = malloc(sizeof(t_str));
	if (!all)
		return (0);
	all->mlx = NULL;
	all->win = NULL;
	all->current_img = NULL;
	all->dir = 0;
	all->mlx = mlx_init();
	if (!all->mlx)
		return (free_all(all));
	int hor = 0;
	int vert = 0;
	all->win = mlx_new_window(all->mlx, 16 * hor, 16 * vert, "leaf");
	if (!all->win)
		return (free_all(all));
	all->current_img = make_img(all->mlx, NULL, hor, vert);
	if (!all->current_img)
		return (free_all(all));
	all->map = empty_map(hor, vert);
	if (!map)
		return (free_all(all));
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, 0, 0);
	mlx_mouse_hook(all->win, &paint_map, all);
	mlx_hook(all->win, 2, 1L << 0, &closer, all);
	mlx_loop(all->mlx);
	int fd = open(argv[1], O_WRONLY | O_CREAT, 0666);
	map_to_file(fd, all->map);
	close(fd);
	return (free_all(all));	
}
