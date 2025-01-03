/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:56:59 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/04 02:37:05 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

int	builtin_pwd(int argc, char *argv[], t_context *ctx)
{
	char	buf[PATH_MAX];
	int		i;

	(void)argc;
	i = 1;
	if (argv[1] && argv[1][0] == '-')
	{
		printf("%s: option unsupported\n", argv[i]);
		ctx->last_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	if (!getcwd(buf, PATH_MAX))
	{
		printf("%s: cannot get working directory\n", argv[0]);
		ctx->last_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	printf("%s\n", buf);
	ctx->last_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
/*
if multiple arguments are given, ignore them
and print the current working directory
if argument contains "-", print error message
*/
