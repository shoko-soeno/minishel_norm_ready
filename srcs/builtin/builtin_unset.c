#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

int	builtin_unset(int argc, char *argv[], t_map *envmap)
{
	int		status;
	size_t	i;

	(void)argc;
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (map_unset(envmap, argv[i]) < 0)
		{
			builtin_error("unset", argv[i], "not a valid identifier");
			status = 1;
		}
		else
			status = 0;
		i++;
	}
	return (status);
}
