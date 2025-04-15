#include "libby.h"

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
