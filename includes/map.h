/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 21:18:32 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 11:19:02 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H
# include <stdio.h>

// map
typedef struct s_map		t_map;
typedef struct s_item		t_item;

struct s_item {
	char			*name;
	char			*value;
	struct s_item	*next;
};

struct s_map {
	t_item	item_head;
};

// item_utils.c
t_item	*item_new(char *name, char *value);
char	*item_get_string(t_item *item);
int		ft_strcmp_for_map(const char *s1, const char *s2);

// map_get.c
char	*map_get(t_map *map, const char *name);

// map_new.c
t_map	*map_new(void);

// map_put.c
int		map_put(t_map *map, const char *string, bool allow_empty_value);
int		split_name_value(const char *string, bool allow_empty_value,
			char **name, char **value);

// map_set.c
int		map_set(t_map *map, const char *name, const char *value);
bool	is_identifier(const char *s);
void	map_add_item(t_map *map, const char *name, const char *value);

// map_size.c
size_t	map_size(t_map *map, bool count_null_value);

// map_unset.c
int		map_unset(t_map *map, const char *name);

#endif