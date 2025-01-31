/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 01:13:23 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 11:20:14 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdio.h>
# include <stdbool.h>
# include "./minishell.h"
# define OPE_MAP_SIZE 10
# define ERR_MSG_S_QUOTE "unexpected EOF while looking for matching `''"
# define ERR_MSG_D_QUOTE "unexpected EOF while looking for matching `\"'"
# define ERR_MSG_UNEXPECTED_EOF "syntax error: unexpected end of file"

typedef struct s_token	t_token;

typedef enum e_node_kind{
	ND_REDIRECTS,
	ND_PIPE,
	ND_OR_OP,
	ND_AND_OP,
	ND_L_PARE,
	ND_R_PARE,
	ND_RND_BRACKET,
	ND_CMD,
	ND_FD_NUM,
	ND_EOF
}	t_node_kind;

struct s_token {
	char			*word;
	t_node_kind		kind;
	t_token			*next;
};

typedef struct s_ope_map {
	const char	*op_str;
	t_node_kind	kind;
}	t_ope_map;

t_token	*lexer_handler(char *line, t_syntax_error *syntx_err, t_context *ctx);
t_token	*lexer(char *line, t_syntax_error *syntx_err);
bool	skip_blank(char **rest, char *input);
t_token	*fetch_fst_ope_token(char *input);
t_token	*fetch_fst_word_token(char *input, t_syntax_error *syntx_err);
int		move_to_next_quotation(char *input, int i, t_syntax_error *syntx_err);

bool	is_metachar(int c);
bool	is_word(char *input);
bool	is_blank(char c);

bool	is_s_quote(char input);
bool	is_d_quote(char input);

// lexer_operator_checker.c
void	get_ope_map(t_ope_map *ope_map);
t_token	*fetch_fst_ope_token(char *input);
bool	is_operator(char *input);
size_t	get_char_arry_size(char **str);
t_token	*create_token(char *word, t_node_kind kind);
#endif