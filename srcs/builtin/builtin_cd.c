/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:55:17 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/04 02:31:03 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"
#include "../../includes/map.h"
#include "../../includes/environment.h"

/*
`cd -` simply swaps `PWD` and `OLDPWD` alternately.  
As a result, it repeatedly returns to the previous directory.  
`OLDPWD` always stores the current `PWD` and is used for subsequent `cd -`.
may not be implemented in this project
"cd --" is out of subject.
*/

void	cd_with_no_argument(t_map *envmap, char *path)
{
	const char	*home;

	home = xgetenv(envmap, "HOME");
	if (home == NULL)
	{
		builtin_error("cd", NULL, "HOME not set");
		path[0] = '\0';
		return ;
	}
	ft_strlcpy(path, home, PATH_MAX);
}

int	handle_cd_error(int argc, char *argv[], t_context *ctx)
{
	if (argc > 2)
	{
		ft_putendl_fd("cd: too many argument\n", STDERR_FILENO);
		ctx->last_status = EXIT_FAILURE;
		return (ctx->last_status);
	}
	else if (argc == 2 && argv[1][0] == '-')
	{
		builtin_error("cd", argv[1], "option not supported");
		ctx->last_status = EXIT_FAILURE;
		return (ctx->last_status);
	}
	return (EXIT_SUCCESS);
}

void	update_pwd(t_map *envmap, char *pwd_before_chdir, char *path)
{
	char	*pwd_after_chdir;

	pwd_after_chdir = resolve_pwd(pwd_before_chdir, path);
	if (pwd_after_chdir)
	{
		map_set(envmap, "PWD", pwd_after_chdir);
		free(pwd_after_chdir);
	}
}

int	builtin_cd(int argc, char *argv[], t_context *ctx)
{
	char		*pwd_before_chdir;
	char		path[PATH_MAX];

	if (handle_cd_error(argc, argv, ctx) == EXIT_FAILURE)
		return (ctx->last_status);
	pwd_before_chdir = xgetenv(ctx->env, "PWD");
	map_set(ctx->env, "OLDPWD", pwd_before_chdir);
	if (argc == 1)
		cd_with_no_argument(ctx->env, path);
	else
		ft_strlcpy(path, argv[1], PATH_MAX);
	if (path[0] == '\0' || chdir(path) < 0)
	{
		builtin_error("cd", path, "no such file or directory");
		ctx->last_status = EXIT_FAILURE;
		return (ctx->last_status);
	}
	update_pwd(ctx->env, pwd_before_chdir, path);
	return (EXIT_SUCCESS);
}
/*
if no arguments are given, change to the home directory
chdir only changes the current directory and 
does not update the logical path (PWD), 
so we need to update the PWD manually. 
*/
