#include "libby.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->ll + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

int	closer(int keycode, t_str *str)
{
	if (keycode == 0xff1b)
		mlx_loop_end(str->mlx);
	return (0);
}

void	pixelate(t_data *img, int x, int y, int color)
{
	int	i = 0;
	int	j;

	while (i < 6)
	{
		j = 0;
		while (j < 6)
		{
			my_mlx_pixel_put(img, x + i, y + j, color);
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
			if (map[i][j] == 'f')
				pixelate(img, j * 6, i * 6, 0X00FFFFFF);
			j++;
		}
		i++;
	}
}

char	*get_file(int level)
{
	if (level < 0)
		level = -level;
	if (level == 0)
		return ("maps/straight.txt");
	if (level == 1)
		return ("maps/left_1.txt");
	if (level == 2)
		return ("maps/left_2.txt");
	return (NULL);
}
	

int	make_leaf(t_data *img, int level)
{
	char	**map;
	char	*file;

	file = get_file(level);
	if (!file)
	{
		printf("error make leaf :: no file, level %d\n", level);
		return (-1);
	}
	if (!check_map(file, 6, 25))
	{
		printf("error make leaf :: bad file\n");
		return (-1);
	}
	map = make_map(file, 6, 25);
	if (!map)
	{
		printf("error make leaf :: no map\n");
		return (-1);
	}
	if (level < 0)
		map = reverse_map(map, 6, 25);
	map_to_img(img, map, 6, 25);
	free_all_map(-2, map);
	return (0);
}

void	free_all_imgs(t_data *img, void *mlx)
{
	while (img->prev)
		img = img->prev;
	while (img->next)
	{
		if (img->img)
			mlx_destroy_image(mlx, img->img);
		img = img->next;
		free(img->prev);
	}
	if (img->img)
		mlx_destroy_image(mlx, img->img);
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
		free(all->mlx);
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

t_data	*make_imgs(void *mlx, int hor, int vert)
{
	int	i;
	t_data	*img;

	img = make_img(mlx, NULL, hor, vert);
	if (!img)
		return (NULL);
	i = 1;
	while (i < 5)
	{
		img->next = make_img(mlx, img, hor, vert);
		if (!img->next)
		{
			free_all_imgs(img, mlx);
			return (NULL);
		}
		i++;
		img = img->next;
	}
	return (img);
}

int	init_imgs(t_data *img)
{
	int	i;

	i = -2;
	while (img->next)
		img = img->next;
	while (img)
	{
		if (make_leaf(img, i) == -1)
			return (-1);
		i++;
		img = img->prev;
	}
	return (0);
}

//next up :: animation
int	wind_blows(t_str *all)
{
	useconds_t sec = 80000;
	static int	stat = 0;

	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, 0, 0);
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, 6 * 25, 0);
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, 5 * 6, 6 * 6);
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, 6 * 20, 6 * 6);
	usleep(sec);
	if ((all->dir == -1 && !all->current_img->prev)
		|| (all->dir == 1 && !all->current_img->next))
	{
		if (stat == 2)
		{
			all->dir = -all->dir;
			stat = 0;
		}
		else
		{
			stat++;
			return (0);
		}
	}
	if (all->dir == -1)
		all->current_img = all->current_img->prev;
	if (all->dir == 1)
		all->current_img = all->current_img->next;
	return (0);
}

/*int main(void)
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
	all->win = mlx_new_window(all->mlx, 6 * 50, 6 * 12, "leaf");
	if (!all->win)
		return (free_all(all));
	all->current_img = make_imgs(all->mlx, 6* 25, 6 * 6);
	if (!all->current_img)
		return (free_all(all));
	if (init_imgs(all->current_img) == -1)
		return (free_all(all));
	wind_blows(all);
	mlx_loop_hook(all->mlx, &wind_blows, all);
	mlx_hook(all->win, 2, 1L << 0, &closer, all);
	mlx_loop(all->mlx);
	return (free_all(all));
}*/
