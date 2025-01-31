/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 01:13:23 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 11:21:12 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "minishell.h"
# include "parser.h"
# define ERR_MSG_SYNTAX_S_QUOTE "unexpected EOF while looking for matching `''"
# define ERR_MSG_SYNTAX_D_QUOTE "unexpected EOF while looking for matching `\"'"
# define ERR_MSG_SUBSHELL "syntax error: unexpected end of file"
# define ERR_MSG_RD "syntax error unexpected token near redirect"
# define ERR_MSG_PIPE "syntax error unexpected token near pipe"
# define ERR_MSG_AND_OP "syntax error near unexpected token `&&'"
# define ERR_MSG_OR_OP "syntax error near unexpected token `||'"
# define ERR_MSG_SYNTAX "syntax error : unexpected token"
# define ERR_MSG_L_PARE "syntax error near unexpected token `('"
# define ERR_MSG_R_PARE "syntax error near unexpected token `)'"
# define EXIT_SYNTAX_ERROR 2
# define ERROR_PREFIX "minishell: "
# define ERR_MSG_PROCESS_LIMIT "Exceed number of process limit"

// error.c
void	d_throw_error(char *func_name, char *error_msg);
void	builtin_error(char *func, char *name, char *err);
void	throw_unexpected_error(char *func_name, char *err_msg);
void	throw_syntax_error(char *err_msg, char *err_msg2);
void	perror_prefix(void);
void	throw_system_error(char *err_msg, char *err_msg2);

// free_functions.c
void	free_ctx(t_context **ctx);
void	free_wordlist(char ***wordlist);
void	free_ast(t_node **node);
void	free_token_list(t_token *token_list);
void	free_map(t_map **map);

// xmalloc.c
void	*xmalloc(size_t size);

// ft_strcmp.c
int		ft_strcmp(const char *s1, const char *s2);

char	*x_strjoin(char const *s1, char const *s2);
char	*x_substr(char const *s, unsigned int start, size_t len);
char	*x_strdup(const char *s1);

char	**x_split(char const *s, char c);
char	*x_itoa(int n);

#endif