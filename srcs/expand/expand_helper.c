/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:14:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/02 13:59:57 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

size_t	skip_s_quote_block(char *str, size_t i)
{
	if (!str)
		d_throw_error("skip_s_quote", "str is null");
	i = i + 1;
	while (str[i] && !is_s_quote(str[i]))
		i++;
	return (i);
}

bool	is_dollar_symbol(char c)
{
	return (c == DOLLAR);
}

char	*append_substring(char **ret, char *str, size_t i, size_t len)
{
	char	*tmp;
	char	*ret_val;

	tmp = ft_substr(str, i, len);
	if (!tmp)
		d_throw_error("str_concat_helper", "substr failed");
	ret_val = ft_strjoin(*ret, tmp);
	if (!ret_val)
		d_throw_error("str_concat_helper", "strjoin failed");
	free(tmp);
	free(*ret);
	*ret = NULL;
	*ret = ret_val;
	return (ret_val);
}

//below code for expand asterisk. freeze for developping.
/*
#include <sys/types.h>
#include <dirent.h>
void	expand_asterisk_handler(t_node *node)
{
	int		i;
	char	**tmp;
	int		size;

	if (!node || !node->cmds)
		d_throw_error("expand_variable_handler", "node or cmds is null");
	i = 0;
	while (node->cmds[i])
	{
		size = 0;
		if (ft_strchr(node->cmds[i], ASTERISK))
		{
			tmp = NULL;
			// ここでリストの長さが変わる可能性があるので、詰めなおす必要がある
			tmp = expand_asterisk(node->cmds[i], &size);
			if (size > 1)++
				recreate_command_list();
			free(node->cmds[i]);
			node->cmds[i] = NULL;
			node->cmds[i] = tmp;
		}
		i++;
	}
}

char	**expand_asterisk(char *str, int *size)
{
	size_t	i;
	bool	is_first_asterisk;	

	i = 0;
	is_first_asterisk = false;
	while (str[i])
	{
		i = find_next_asterisk(str);
		if (str[i] != ASTERISK && is_first_asterisk)
			filtered_files = find_end_match_file(str, i);
		else if (str[i] == ASTERISK && is_first_asterisk)
			filtered_files = find_contain_file(str, i);
		else if (str[i] == ASTERISK && !is_first_asterisk)
		{
			find_start_match_file(str, i);
			filtered_files = is_first_asterisk = true;
		}
	}

}

get_all_files_in_dir()
{
	DIR		*dir;
	dirent	*dir_ent;
	
	dir = opendir(".");
	if (!dir)
		d_throw_error("expand_asterisk", "opendir failed");
	while ()
	{
		dir_ent = readdir(dir);
		dir_ent->dir_name
	}
}

bool	check_expandable_asterisk(char *str, size_t i)
{
	
}

size_t	find_next_asterisk(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (is_s_quote(input[i]) || is_d_quote(input[i]))
			i = move_to_next_quotation(input, i);
		if (str[i] == ASTERISK)
			return (i);
		i++;
	}
	return (i);
}
*/