#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

int	builtin_echo(int argc, char *argv[], t_map *envmap)
{
	bool	is_first_arg;
	bool	echo_newline;
	size_t	i;

	(void)argc;
	(void)envmap;
	i = 1;
	echo_newline = true;
	if (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		echo_newline = false;
		i++;
	}
	is_first_arg = true;
	while (argv[i])
	{
		if (!is_first_arg)
			write(STDOUT_FILENO, " ", 1);
		is_first_arg = false;
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		i++;
	}
	if (echo_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}
/*
*/
