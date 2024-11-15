/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:56:59 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/15 17:24:10 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

int	builtin_pwd(int argc, char *argv[], t_map *envmap)
{
	char	buf[PATH_MAX];
	int		i;

	(void)envmap;
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-')
		{
			printf("%s: option unsupported\n", argv[i]);
			return (EXIT_FAILURE);
		}
		i++;
	}
	if (!getcwd(buf, PATH_MAX))
	{
		printf("%s: cannot get working directory\n", argv[0]);
		return (EXIT_FAILURE);
	}
	printf("%s\n", buf);
	return (EXIT_SUCCESS);
}
/*
if multiple arguments are given, ignore them
and print the current working directory
if argument contains "-", print error message
*/
