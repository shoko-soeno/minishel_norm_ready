/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:15:25 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/22 22:10:50 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_TESTER_H
# define LEXER_TESTER_H

# include "../libft/libft.h"
# include "../includes/lexer.h"
# include "../includes/utils.h"
# include <fcntl.h>
# include "ft_get_next_line/get_next_line_bonus.h"
# include <stdio.h>

# define DEFAULT_TEST "lexer_testcase.txt"
# define EXPECTED_RESULT "lexer_expected.txt"

void	exec_lexer_test(char *test_file_name, char *expected_file);
void	node_printer(t_token *token);
int		main(int argc, char *argv[]);
void	free_token_list(t_token	*token);
bool	match(t_token *token, char *str);
void	free_words(char	**word);

#endif