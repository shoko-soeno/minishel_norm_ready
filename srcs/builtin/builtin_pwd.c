/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:56:59 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/01 16:10:32 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

int	builtin_pwd(int argc, char *argv[], t_map *envmap, t_context *ctx)
{
	char	buf[PATH_MAX];
	int		i;

	(void)argc;
	(void)envmap;
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
