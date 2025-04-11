#include "libby.h"
//#include <unistd.h>

void	print(char *str)
{
	int	i = 0;
	while (str && str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

void	print_err(char *func, char *err)
{
	print("error occured in function: ");
	print(func);
	print("\ntype of error: ");
	print(err);
	print("\n");
}


