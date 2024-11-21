/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 02:46:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/22 04:43:28 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"

//typedef enum e_node_kind{
//	ND_REDIRECTS,
//	ND_IN_RD,
//	ND_OUT_RD,
//	ND_APPEND_RD,
//	ND_HERE_DOC,
//	ND_PIPE,
//	ND_OR_OP,
//	ND_AND_OP,
//	ND_L_PARE,
//	ND_R_PARE,
//	ND_SUB_SHELL,
//	ND_CMD,
//	ND_FD_NUM,
//	ND_FD_WORD
//}	t_node_kind;

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

bool	match_token(t_node_kind expected, t_token **cur_token);
bool	compare_token(t_node_kind expected, t_token **cur_token);

char	**parse_words(t_token	**token_list);
t_node	*parse_redirects(t_token **token_list);
size_t	count_word_node(t_token	**token_list);
char	**parse_redirect_arry(t_token	**token_list);
size_t	count_rd_node(t_token	**token_list);
#endif