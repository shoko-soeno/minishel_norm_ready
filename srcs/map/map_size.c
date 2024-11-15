#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

size_t	map_size(t_map *map, bool count_null_value)
{
	size_t	size;
	t_item	*item;

	size = 0;
	item = map->item_head.next;
	while (item)
	{
		if (count_null_value || item->value)
			size++;
		item = item->next;
	}
	return (size);
}
