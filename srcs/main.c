/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/11/09 21:59:02 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char	*line;

	// test for builtins
	// printf("Testing builtins...\n");
	// printf("Case 1: cd to /tmp and display the directory\n");
	// char *cd_args1[] = {"cd", "/tmp"}; 
	// test_builtin("cd", 2, cd_args1);
	// char *pwd_args1[] = {"pwd"};
	// test_builtin("pwd", 1, pwd_args1);
	// printf("Case 2: cd to non-existent directory\n");
	// char *cd_args2[] = {"cd", "/non-existent-dir"};
	// test_builtin("cd", 2, cd_args2);
	// char *pwd_args2[] = {"pwd"};
	// test_builtin("pwd", 1, pwd_args2);
	// printf("Case 3: Move to the directory one level above the /tmp directory.\n");
	// char *cd_args3[] = {"cd", ".."};
	// test_builtin("cd", 2, cd_args3);
	// char *pwd_args3[] = {"pwd"};
	// test_builtin("pwd", 1, pwd_args3);
	
	// char *exit_args[] = {"exit"};
	// test_builtin("exit", 1, exit_args);
	// printf("Builtins test done.\n");

	rl_outstream = stderr;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
			// TODO: intepret line as a command
		free(line);
	}
	exit(0);
}
