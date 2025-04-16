CC=cc
FLAGS=-Wall -Werror -Wextra
NAME=pixelart
OBJ=make_map.c utils.c pixel_art.c pool.c

all: $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)

clean:
	rm -f $(NAME)

fclean: clean
	
re: clean all

.PHONY: all clean re fclean
