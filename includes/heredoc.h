/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 13:31:55 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/10 22:20:38 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H
# include "./minishell.h"
# include "./parser.h"

int		input_heredoc_content(char *eof);
void	heredoc_handler(t_node *node);
void	call_heredoc(t_node *node);
bool	is_heredoc_end(char **redirects);

#endif