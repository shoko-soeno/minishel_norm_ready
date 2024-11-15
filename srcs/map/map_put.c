#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

int	split_name_value(const char *string, bool allow_empty_value, char **name,
		char **value)
{
	char	*name_end;

	name_end = ft_strchr(string, '=');
	if (name_end == NULL)
	{
		if (!allow_empty_value)
			return (-1);
		*name = ft_strdup(string);
		*value = NULL;
	}
	else
	{
		*name = ft_substr(string, 0, name_end - string);
		*value = ft_strdup(name_end + 1);
	}
	return (EXIT_SUCCESS);
}

int	map_put(t_map *map, const char *string, bool allow_empty_value)
{
	int		result;
	char	*name;
	char	*value;

	name = NULL;
	value = NULL;
	result = split_name_value(string, allow_empty_value, &name, &value);
	if (result != 0)
		return (result);
	result = map_set(map, name, value);
	free(name);
	free(value);
	return (result);
}
