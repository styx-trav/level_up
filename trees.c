#include <mlx.h>
#include <stdlib.h>

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		ll;
	int		endian;
	struct s_data *next;
	struct s_data *prev;
}			t_data;

typedef struct	s_str
{
	void *mlx;
	void *win;
	t_data *current_img;
	int	dir;
}			t_str;
	

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->ll + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

void	pixelate(t_data *img, int x, int y)
{
	int	i = 0;
	x = x -3;

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

void	leaf_straight(t_data *img, int x, int y, int level)
{
	int	i = 0;
	int	j;

	

void	leaf_right(t_data *img, int x, int y, int level)
{
	int	i = 0;
	int	j;

	if (level == 1)
	{
		

void	make_leaf(t_data *img, int level)
{
	int	x = 200;
	int	y = 400;

	pixelate(img, x, y);
	if (level <= 3)
		leaf_right(img, x, y, level);
	else if (level == 4)
		leaf_straight(img, x, y);
	else
		leaf_left(img, x, y, level);
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

t_data	*make_img(void *mlx, t_data *prev)
{
	t_data	*img;

	img = (t_data *)malloc(sizeof(t_data));
	if (!img)
		return (NULL);
	img->img = mlx_new_image(mlx, 400, 400);
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

	i = 1;
	while (img)
	{
		make_leaf(img, i);
		i++;
		img = img->next;
	}
}

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
	all->win = mlx_new_window(str->mlx, 400, 400, "leaf");
	if (!all->win)
		return (free_all(all));
	all->current_img = make_imgs(all);
	if (!all->current_img)
		return (free_all(all));
	init_imgs(all->current_img);
	mlx_loop_hook(/*animation here, moving left -1 or right 1*/);
	mlx_hook(all->win, 2, 1L << 0, &closer, all);
	mlx_loop(all->mlx);
	return (free_all(all));
}
