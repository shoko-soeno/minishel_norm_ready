/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:34:45 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/01 17:00:51 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "minishell.h"
# include "parser.h"

// error.c
void	d_throw_error(char *func_name, char *error_msg);
void	builtin_error(char *func, char *name, char *err);

// free_functions.c
void	free_node(t_node *node);
void	free_wordlist(char **wordlist);
void	free_ast(t_node **node);
void	free_token_list(t_token *token_list);
void    free_map(t_map *map);

// xmalloc.c
void	*xmalloc(size_t size);

// ft_strcmp.c
int     ft_strcmp(const char *s1, const char *s2);

#endif