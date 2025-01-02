/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:27:18 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/02 14:03:26 by ssoeno           ###   ########.fr       */
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

void	expand_handler(t_node *node, t_map *envp);
void	expand_variable_handler(t_node *node, t_map *envp);
char	*expand_variable(char *str, t_map *envp);
size_t	retrieve_val_in_sq(char **ret, char *str, size_t i);
size_t	skip_s_quote_block(char *str, size_t i);
bool	is_dollar_symbol(char c);
size_t	retrieve_val_in_dq(char **ret, char *str, size_t i, t_map *envp);
char	*append_substring(char **ret, char *str, size_t i, size_t len);
size_t	retrieve_var(char **ret, char *str, size_t i, t_map *envp);
size_t	retrieve_var_name_len(char *str, size_t i);
size_t	retrieve_normal_val(char **ret, char *str, size_t i);
void	remove_quote_handler(t_node *node);
char	*remove_quotes(char *str);
int		move_to_next_quotation_expnd(char *input, int i);

#endif
