#include "libby.h"

void	red_player(t_data *player, int size)
{
	int i = 0;
	int j = 0;

	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			my_mlx_pixel_put(player, i, j, 0x00FF0000);
			j++;
		}
		i++;
	}
}

int	get_img_i(t_str *all)
{
	if (all->player_i + 25 > all->hor)
		return (all->hor - 25);
	return (all->player_i);
}

int	get_img_j(t_str *all)
{
	if (all->player_j + 7 > all->vert)
		return (all->vert - 7);
	return (all->player_j);
}

int	get_pl_i(t_str *all)
{
	if (all->player_i + 25 <= all->hor)
		return (0);
	return (all->player_i - all->hor + 25);
}

int	get_pl_j(t_str *all)
{
	if (all->player_j + 7 <= all->vert)
		return (0);
	return (all->player_j - all->vert + 7);
}

int	move(int key, t_str *all)
{
	if (key == 65363)
	{
		if (!all->map[all->player_j][all->player_i + 1] || all->map[all->player_j][all->player_i + 1] == ' ')
			return (0);
		all->player_i++;
	}
	else if (key == 65364)
	{
		if (!all->map[all->player_j + 1] || all->map[all->player_j + 1][all->player_i] == ' ')
			return (0);
		all->player_j++;
	}
	else if (key == 65361)
	{
		if (all->player_i == 0 || all->map[all->player_j][all->player_i - 1] == ' ')
			return (0);
		all->player_i--;
	}
	else if (key == 65362)
	{
		if (all->player_j == 0 || all->map[all->player_j - 1][all->player_i] == ' ')
			return (0);
		all->player_j--;
	}
	else
		return (0);
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, all->pixel -get_img_i(all) * all->pixel, all->pixel -get_img_j(all) * all->pixel);
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->next->img, all->pixel + get_pl_i(all) * all->pixel, all->pixel + get_pl_j(all) * all->pixel);
	return (0);
}

int	main(void)
{
	t_str *all = malloc(sizeof(t_str));
	if (!all)
		return (0);
	all->mlx = NULL;
	all->win = NULL;
	all->map = NULL;
	all->player_i = 1;
	all->player_j = 1;
	all->current_img = NULL;
	all->pixel = 30;
	all->dir = 0;
	all->mlx = mlx_init();
	if (!all->mlx)
		return (free_all(all));
	int hor = get_map_width("maps/labyrinth.txt");
	if (!hor || !check_map("maps/labyrinth.txt", 25, hor))
		return (0);
	int vert = 25;
	all->win = mlx_new_window(all->mlx, all->pixel * 25, all->pixel * 7, "leaf");
	if (!all->win)
		return (free_all(all));
	all->current_img = make_img(all->mlx, NULL, hor, vert);
	if (!all->current_img)
		return (free_all(all));
	all->current_img->next = make_img(all->mlx, all->current_img, 1, 1);
	if (!all->current_img->next)
		return (free_all(all));
	red_player(all->current_img->next, all->pixel);
	all->map = make_map("maps/labyrinth.txt", vert, hor);
	if (!all->map)
		return (free_all(all));
	map_to_img(all->current_img, all->map, vert, hor);
	all->hor = hor;
	all->vert = vert;
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->img, 0, 0);
	mlx_put_image_to_window(all->mlx, all->win, all->current_img->next->img, all->pixel, all->pixel);
	mlx_key_hook(all->win, &move, all);
	mlx_hook(all->win, 2, 1L << 0, &closer, all);
	mlx_loop(all->mlx);
	return (free_all(all));
}
