/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:48:19 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 14:07:40 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_ASTERISK_H
# define EXPAND_ASTERISK_H
# include "./expand.h"
# include <sys/types.h>
# include <dirent.h>
# include "./map.h"
# include <sys/types.h>

typedef enum e_cmp_type{
	PREFIX,
	SUFFIX,
	CONTAIN,
	ALL
}	t_cmp_type;

typedef struct s_cmp_str
{
	char			*cmp_str;
	t_cmp_type		cmp_type;
}	t_cmp_str;

bool		is_aster_only(char *line);
t_cmp_str	*create_cmp_str(char *s, size_t st, size_t len, t_cmp_type typ);
bool		is_first_aster(char *str, size_t i);
size_t		skip_aster_sequence(char *line, size_t current);
size_t		find_next_aster(char *line, size_t i);
t_cmp_str	**init_cmp_str_arry(char *line);
t_cmp_str	**create_cmp_str_arry(char *line);
void		analyze_aster_loop(t_cmp_str ***ret, char *line, size_t *x);
size_t		analyze_sb_loop(t_cmp_str ***ret, char *line, size_t i, size_t j);
void		expand_asterisk_handler(t_node *node);
char		**expand_asterisk(char *line);
void		get_all_files_in_dir(t_map *file_map);
void		filter_map(t_map *file_map, t_cmp_str **cmp_arry, bool is_dot);
bool		match_aster_regex(t_item *file, t_cmp_str **cmp_arry, bool is_dot);
char		**extract_file_name(t_map *file_map);
bool		match_prefix(char *f_name, size_t *i, char *prefix);
bool		match_suffix(t_item *f, size_t *i, char *suffix, bool is_dot);
bool		match_contain(char *f_name, size_t *i, char *contain, bool is_dot);
bool		match_suffix_directory(t_item *f, size_t *i, char *suffix);

#endif