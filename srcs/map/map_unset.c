/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:54:23 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/13 18:09:02 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

int	map_unset(t_map *map, const char *name)
{
	t_item	*cur;
	t_item	*prev;

	if (!map || name == NULL || !is_identifier(name))
		return (ERROR);
	prev = &map->item_head;
	cur = map->item_head.next;
	while (cur)
	{
		if (ft_strcmp_for_map(cur->name, name) == 0)
		{
			prev->next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return (EXIT_SUCCESS);
		}
		prev = cur;
		cur = cur->next;
	}
	return (EXIT_SUCCESS);
}
// return -1 if the name is NULL or not a valid identifier
//  (throw errorにすべき？あとで相談）
// if the name is in the map, remove it and
// free the memory of the item (name, value, item itself)
/*
一致する項目が見つかった場合：
prev->next = cur->next で現在の項目（cur）をリストから除く
cur->name と cur->value を free で解放、続けて cur 自体も解放
一致しない場合：
prev を cur に更新し、cur を cur->next に進める
*/
