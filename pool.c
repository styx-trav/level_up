#include "libby.h"

int	get_blue(int color, int shade)
{
	unsigned char	maker[4];

	if (((unsigned char *)&color)[0] == 255)
		shade = 0;
	maker[0] = ((unsigned char *)&color)[0] + shade;
	maker[1] = ((unsigned char *)&color)[1];
	maker[2] = ((unsigned char *)&color)[2];
	maker[3] = ((unsigned char *)&color)[3];
	return ((*(int *)maker));
}

int	get_green(int color, int shade)
{
	unsigned char	maker[4];

	maker[0] = ((unsigned char *)&color)[0];
	if (((unsigned char *)&color)[1] == 255)
		shade = 0;
	maker[1] = ((unsigned char *)&color)[1] + shade;
	maker[2] = ((unsigned char *)&color)[2];
	maker[3] = ((unsigned char *)&color)[3];
	return ((*(int *)maker));
}

int	get_red(int color, int shade)
{
	unsigned char	maker[4];

	maker[0] = ((unsigned char *)&color)[0];
	maker[1] = ((unsigned char *)&color)[1];
	if (((unsigned char *)&color)[2] == 255)
		shade = 0;
	maker[2] = ((unsigned char *)&color)[2] + shade;
	maker[3] = ((unsigned char *)&color)[3];
	return ((*(int *)maker));
}

t_data	*make_img(void *mlx, t_data *prev, int hor, int vert, int pixel)
{
	t_data	*img;

	img = (t_data *)malloc(sizeof(t_data));
	if (!img)
		return (NULL);
	img->img = mlx_new_image(mlx, pixel * hor, pixel * vert);
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
	if (all->map)
		free_all_map(-2, all->map);
	free(all);
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
				pixelate(img, j * 30, i * 30, 0X00FFFFFF, 30);
			j++;
		}
		i++;
	}
}

void	pixelate(t_data *img, int x, int y, int color, int size)
{
	int	i = 0;
	int	j;

	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			my_mlx_pixel_put(img, x + i, y + j, color);
			j++;
		}
		i++;
	}
}

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
