/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:57:36 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/31 03:58:54 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

void	expand_handler(t_node *node, t_map *envp)
{
	if (!node || !node->cmds)
		d_throw_error("expand_handler", "node or cmds is null");
	expand_variable_handler(node, envp);
	//word_split_handler(node);
	//expand_asterisk_handler(node);
	remove_quote_handler(node);
}

void	expand_variable_handler(t_node *node, t_map *envp)
{
	int		i;
	char	*tmp;

	if (!node || !node->cmds)
		d_throw_error("expand_variable_handler", "node or cmds is null");
	i = 0;
	while (node->cmds[i])
	{
		if (ft_strchr(node->cmds[i], DOLLAR))
		{
			tmp = NULL;
			tmp = expand_variable(node->cmds[i], envp);
			free(node->cmds[i]);
			node->cmds[i] = NULL;
			node->cmds[i] = tmp;
		}
		i++;
	}
}
/*
Remove quote handler should remove quotation 
which is not surrounded by other quotation.
e.g. 
"Hello" -> Hello
"'Hello'" -> 'Hello'
'Hello'"hello" -> Hellohello
*/

void	remove_quote_handler(t_node *node)
{
	int		i;
	char	*tmp;

	if (!node || !node->cmds)
		d_throw_error("remove_quote_handler", "node or cmds is null");
	i = 0;
	while (node->cmds[i])
	{
		if (ft_strchr(node->cmds[i], '\'') || ft_strchr(node->cmds[i], '\"'))
		{
			tmp = NULL;
			tmp = remove_quotes(node->cmds[i]);
			free(node->cmds[i]);
			node->cmds[i] = NULL;
			node->cmds[i] = tmp;
		}
		i++;
	}
}
