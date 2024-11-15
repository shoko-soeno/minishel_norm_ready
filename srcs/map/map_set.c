#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

bool	is_identifier(const char *s)
{
	if (!ft_isalpha(*s) && *s != '_')
		return (false);
	s++;
	while (*s)
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (false);
		s++;
	}
	return (true);
}
// to check if the string follows the rules of an identifier 
// (starting with an alphabet or underscore, 
// followed by alphabet, digits, or underscores)

t_item	*map_find_item(t_map *map, const char *name)
{
	t_item	*cur;

	if (map == NULL || name == NULL || name[0] == '\0')
		return (NULL);
	cur = map->item_head.next;
	while (cur)
	{
		if (cur->name && cur->name[0] != '\0')
		{
			if (ft_strcmp_for_map(cur->name, name) == 0)
			{
				return (cur);
			}
		}
		cur = cur->next;
	}
	return (NULL);
}

void	map_update_item(t_item *item, const char *value)
{
	free(item->value);
	if (value == NULL)
		item->value = NULL;
	else
	{
		item->value = ft_strdup(value);
		if (item->value == NULL)
			d_throw_error("map_update_item", "Memory allocation error");
	}
}

void	map_add_item(t_map *map, const char *name, const char *value)
{
	t_item	*new_item;
	char	*name_copy;
	char	*value_copy;

	name_copy = ft_strdup(name);
	if (value == NULL)
		value_copy = NULL;
	else
		value_copy = ft_strdup(value);
	if (!name_copy || (value && !value_copy))
	{
		free(name_copy);
		free(value_copy);
		d_throw_error("map_add_item", "Memory allocation error");
	}
	new_item = item_new(name_copy, value_copy);
	new_item->next = map->item_head.next;
	map->item_head.next = new_item;
}

// if the name is in the map, update the value
int	map_set(t_map *map, const char *name, const char *value)
{
	t_item	*item;

	if (map == NULL || name == NULL || !is_identifier(name))
		return (-1);
	item = map_find_item(map, name);
	if (item)
		map_update_item(item, value);
	else
		map_add_item(map, name, value);
	return (EXIT_SUCCESS);
}
