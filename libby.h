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
}			t_str;

//from make_map.c

void	print(char *str);
void	print_err(char *func, char *err);
int	in_str(char a, char *str);
char	**free_all_map(int fd, char **map);
char	**make_map(char *filename, int vert, int hor);
int	check_map(char *filename, int vert, int hor);
char	**reverse_map(char **map, int vert, int hor);


//from trees.c
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	make_leaf(t_data *img, int level);
void	free_all_imgs(t_data *img, void *mlx);
int	free_all(t_str *all);
t_data	*make_img(void *mlx, t_data *prev);
t_data	*make_imgs(void *mlx);
void	init_imgs(t_data *img);
