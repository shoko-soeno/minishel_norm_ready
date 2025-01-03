/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 02:46:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/30 21:47:29 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"

typedef struct s_node	t_node;
// AST node type
struct s_node {
	t_node_kind	kind;
	t_node		*left;
	t_node		*right;
	char		**cmds;
	char		*op_val;
	int			fd_num;
	char		**redirects;
};

t_node	*parse_cmd(t_token **token_list);
t_node	*parse_cmd_type(t_token **token_list);
t_node	*simple_cmd(t_token **token_list);
t_node	*parse_subshell(t_token **token_list);
t_node	*parse_cmd_tail(t_node *left, t_token **token_list);

t_node	*create_logi_node(t_node *left, t_token **token_list);
t_node	*create_node(t_node_kind token_kind);
t_node	*create_sequential_pipe_node(t_node *parent, t_token **token_list);
t_node	*create_pipe_node(t_node *left, t_token **token_list);

bool	match_token(t_node_kind expected, t_token **cur_token);
bool	compare_tk(t_node_kind expected, t_token **cur_token);

char	**parse_words(t_token	**token_list);
t_node	*parse_redirects(t_token **token_list);
char	**parse_redirect_arry(t_token	**token_list);
size_t	count_nodes(t_token **token_list, t_node_kind kind);

#endif