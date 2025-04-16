#include "libby.h"

void	map_to_file(int fd, char **map)
{
	int	i = 0;
	int	j;

	while (map[i])
	{
		j = 0;
		while (map[j])
		{
			write(fd, &map[i][j], 1);
			j++;
		}
		write(fd, "\n", 1);
		i++;
	}
}

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
		if (!map[i])
		{
			free_all_map(-2, map);
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

void	add_palette(t_data *img, int hor, int pixel)
{
	int color = 0x00FF0000;
	pixelate(img, (hor + 1) * pixel, pixel * 2, color, pixel);
	color = 0x000000FF;
	pixelate(img, (hor + 1) * pixel, pixel * 4, color, pixel);
	color = 0x0000FF00;
	pixelate(img, (hor + 1) * pixel, pixel * 6, color, pixel);
	color = 0x00FFFFFF;
	pixelate(img, (hor + 1) * pixel, pixel * 8, color, pixel);
}

int	crash_color(int x, int y, t_str *all)
{
	if (x / 16 != all->hor + 1)
		return (0);
	if (y / 16 == 2 || y / 16 == 8)
		all->color = get_red(all->color, 17);
	if (y / 16 == 4 || y / 16 == 8)
		all->color = get_blue(all->color, 17);
	if (y / 16 == 6 || y / 16 == 8)
		all->color = get_green(all->color, 17);
	if (y / 16 > 9)
		all->color = 0x00000000;
	return (0);                       
}

int	pixel_art(int code, int x, int y, t_str *all)
{
	t_data  *next = NULL;
	int	color = all->color;

	if (code != 1 && code != 3)
		return (0);
	if (x / 16 >= all->hor)
		return (crash_color(x, y, all));
	if (code == 3)
		color = 0x00000000;
	if (all->current_img->next)
		next = all->current_img->next;
	else
		next = all->current_img->prev;
	if (!next)
		return (0);
	all->map[y / all->pixel][x / all->pixel] = 'f';
	pixelate(next, (x / all->pixel) * all->pixel, (y / all->pixel) * all->pixel, color, all->pixel);
	mlx_put_image_to_window(all->mlx, all->win, next->img, 0, 0);
	pixelate(all->current_img, (x / all->pixel) * all->pixel, (y / all->pixel) * all->pixel, color, all->pixel);
	all->current_img = next;
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
	all->pixel = 16;
	all->color = 0x00000000;
	all->mlx = mlx_init();
	if (!all->mlx)
		return (free_all(all));
	all->hor = 32;
	all->vert = 32;
	if (!all->hor || !all->vert)
		return (free_all(all));
	all->win = mlx_new_window(all->mlx, all->pixel * (all->hor + 3), all->pixel * all->vert, "leaf");
	if (!all->win)
		return (free_all(all));
	all->current_img = make_img(all->mlx, NULL, all->hor + 3, all->vert, all->pixel);
	if (!all->current_img)
		return (free_all(all));
	all->current_img->next = make_img(all->mlx, all->current_img, all->hor + 3, all->vert, all->pixel);
	if (!all->current_img->next)
		return (free_all(all));
	add_palette(all->current_img, all->hor, all->pixel);
	add_palette(all->current_img->next, all->hor, all->pixel);
	all->map = empty_map(all->hor, all->vert);
	if (!all->map)
		return (free_all(all));
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, 0, 0);
	mlx_mouse_hook(all->win, &pixel_art, all);
	mlx_hook(all->win, 2, 1L << 0, &closer, all);
	mlx_loop(all->mlx);
	int fd = open(argv[1], O_WRONLY | O_CREAT, 0666);
	map_to_file(fd, all->map);
	close(fd);
	return (free_all(all));	
}
