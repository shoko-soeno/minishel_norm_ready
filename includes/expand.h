/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:27:18 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/04 01:57:53 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H
# define DOLLAR '$'
# define ASTERISK '*'
# define UNDER_SCORE '_'
# include "./parser.h"
# include "./lexer.h"
# include "./map.h"
# include "./minishell.h"

void	expand_handler(t_node *node, t_context *ctx);
void	expand_variable_handler(t_node *node, t_context *ctx);
char	*expand_variable(char *str, t_context *ctx);
size_t	retrieve_val_in_sq(char **ret, char *str, size_t i);
size_t	skip_s_quote(char *str, size_t i);
bool	is_dollar_symbol(char c);
size_t	retrieve_val_in_dq(char **ret, char *str, size_t i, t_context *ctx);
char	*str_concat_helper(char **ret, char *str, size_t i, size_t len);
size_t	retrieve_var(char **ret, char *str, size_t i, t_context *ctx);
size_t	retrieve_var_name_len(char *str, size_t i);
size_t	retrieve_normal_val(char **ret, char *str, size_t i);
void	remove_quote_handler(t_node *node);
char	*remove_quotes(char *str);
int		move_to_next_quotation_expnd(char *input, int i);

#endif
