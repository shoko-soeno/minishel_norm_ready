#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

void	print_allenv(t_map *envmap)
{
	t_item	*cur;

	cur = envmap->item_head.next;
	while (cur)
	{
		if (cur->value)
			printf("declare -x %s=\"%s\"\n", cur->name, cur->value);
		else
			printf("declare -x %s\n", cur->name);
		cur = cur->next;
	}
}

int	builtin_export(int argc, char **argv, t_map *envmap)
{
	size_t	i;
	int		status;

	(void)argc;
	if (argv[1] == NULL)
	{
		print_allenv(envmap);
		return (EXIT_SUCCESS);
	}
	if (envmap == NULL) {
		builtin_error("builtin_export", NULL, "map is not initialized");
		return (EXIT_FAILURE);
	}
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (map_put(envmap, argv[i], true) < 0)
		{
			builtin_error("export", argv[i], "not a valid identifier");
			status = 1;
		}
		i++;
	}
	return (status);
}


