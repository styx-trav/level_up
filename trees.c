#include "libby.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->ll + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

void	pixelate(t_data *img, int x, int y)
{
	int	i = 0;
	int	j;

	while (i < 6)
	{
		j = 0;
		while (j < 6)
		{
			my_mlx_pixel_put(img, x + i, y + j, 0x00FFFFFF);
			j++;
		}
		i++;
	}
}

void	map_to_img(t_data *img, char **map, int vert, int hor)
{
	int	i = 0;
	int	j;

	while (i < vert)
	{
		j = 0;
		while (j < hor)
		{
			if (map[vert - i][j] == 'f')
				pixelate(img, j * 6, i * 6);
			j++;
		}
		i++;
	}
}

void	make_leaf(t_data *img, int level)
{
	char	**map;
	char	*file;

	file = get_file(level);
	if (!file)
		return ;
	if (!check_map(file, 6, 25))
		return ;
	map = make_map(file, 6, 25);
	if (!map)
		return ;
	if (level < 0)
		map = reverse_map(map, 6, 25);
	map_to_img(img, map, 6, 25);
	free_all_map(-2, map);
}

void	free_all_imgs(t_data *img, void *mlx)
{
	while (img->previous)
		img = img->previous;
	while (img->next)
	{
		if (img->img)
			mlx_destroy_image(mlx, img->img);
		img = img->next;
		free(img->prev);
	}
	free(img);
}

int	free_all(t_str *all)
{
	if (all->current_img)
		free_all_imgs(all->current_img, all->mlx);
	if (all->win)
		mlx_destroy_window(all->mlx, all->win);
	if (all->mlx)
	{
		mlx_destroy_display(all->mlx);
		free(mlx);
	}
	free(all);
	return (0);
}

t_data	*make_img(void *mlx, t_data *prev, int hor, int vert)
{
	t_data	*img;

	img = (t_data *)malloc(sizeof(t_data));
	if (!img)
		return (NULL);
	img->img = mlx_new_image(mlx, 6 * hor, 6 * vert);
	if (!img->img)
	{
		free(img);
		return (NULL);
	}
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->ll, &img->endian);
	img->prev = prev;
	img->next = NULL;
	return (img);
}

t_data	*make_imgs(void *mlx)
{
	int	i;
	t_data	*img;

	img = make_img(mlx, NULL);
	if (!img)
		return (NULL);
	i = 1;
	while (i <= 7)
	{
		img->next = make_img(mlx, img);
		if (!img->next)
		{
			free_all_imgs(img);
			return (NULL);
		}
		i++;
		img = img->next;
	}
	return (img);
}

void	init_imgs(t_data *img)
{
	int	i;

	i = -3;
	while (img)
	{
		make_leaf(img, i);
		i++;
		img = img->next;
	}
}

//next up :: animation
int	wind_blows(t_str *all);

int main(void)
{
	t_str *all = malloc(sizeof(t_str));
	if (!all)
		return (0);
	all->mlx = NULL;
	all->win = NULL;
	all->current_img = NULL;
	all->dir = -1;
	all->mlx = mlx_init();
	if (!all->mlx)
		return (free_all(all));
	all->win = mlx_new_window(str->mlx, 6 * 25, 6 * 6, "leaf");
	if (!all->win)
		return (free_all(all));
	all->current_img = make_imgs(all);
	if (!all->current_img)
		return (free_all(all));
	init_imgs(all->current_img);
	mlx_loop_hook(all->mlx, &wind_blows, all);
	mlx_hook(all->win, 2, 1L << 0, &closer, all);
	mlx_loop(all->mlx);
	return (free_all(all));
}
