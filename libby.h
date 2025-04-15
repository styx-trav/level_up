#ifndef LIBBY_H
# define LIBBY_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <mlx.h>

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
	char	**map;
	int	player_i;
	int	player_j;
	int	hor;
	int	vert;
	int	pixel;
}			t_str;

//from utils.c

void	print(char *str);
void	print_err(char *func, char *err);

//from make_map.c

int	in_str(char a, char *str);
char	**free_all_map(int fd, char **map);
char	**make_map(char *filename, int vert, int hor);
int	check_map(char *filename, int vert, int hor);
char	**reverse_map(char **map, int vert, int hor);
void	read_map(char **map);
int	get_map_width(char *filename);


//from trees.c
int	make_leaf(t_data *img, int level);
t_data	*make_imgs(void *mlx, int hor, int vert);
int	init_imgs(t_data *img);

//from pool.c
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
int	closer(int keycode, t_str *str);
void	pixelate(t_data *img, int x, int y, int color);
void	map_to_img(t_data *img, char **map, int vert, int hor);
void	free_all_imgs(t_data *img, void *mlx);
int	free_all(t_str *all);
t_data	*make_img(void *mlx, t_data *prev, int hor, int vert);

#endif
