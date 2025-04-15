#include "libby.h"

int	move(int key, t_str *all)
{
	int	mov = 0;

	if (key == 65363)
	{
		if (50 - (all->dir / 6) != 100)
		{
			while (mov < 10)
			{
				mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, all->dir - mov, 0);
				mov++;
			}
			all->dir = all->dir - mov;
		}
		return (0);
	}
	if (key == 65361)
	{
		if (all->dir != 0)
		{
			while (mov < 10)
			{
				mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, all->dir + mov, 0);
				mov++;
			}
			all->dir = all->dir + mov;
		}
	}
	else
		printf("here code %d\n", key);
	return (0);	
}

int	main(void)
{
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
	int hor = get_map_width("maps/labyrinth.txt");
	if (!hor || !check_map("maps/labyrinth.txt", 7, hor))
		return (0);
	int vert = 7;
	all->win = mlx_new_window(all->mlx, 6 * 50, 6 * vert, "leaf");
	if (!all->win)
		return (free_all(all));
	all->current_img = make_img(all->mlx, NULL, hor, vert);
	if (!all->current_img)
		return (free_all(all));
	char **map = make_map("maps/labyrinth.txt", vert, hor);
	if (!map)
		return (free_all(all));
	map_to_img(all->current_img, map, vert, hor);
	free_all_map(-2, map);
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, 0, 0);
	mlx_key_hook(all->win, &move, all);
	mlx_hook(all->win, 2, 1L << 0, &closer, all);
	mlx_loop(all->mlx);
	return (free_all(all));
}
