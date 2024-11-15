/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:55:17 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/15 22:32:40 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

bool	consume_path(char **rest, char *path, char *elem)
{
	size_t	elem_len;

	elem_len = ft_strlen(elem);
	if (ft_strncmp(path, elem, elem_len) == 0)
	{
		if (path[elem_len] == '\0' || path[elem_len] == '/')
		{
			*rest = path + elem_len;
			return (true);
		}
	}
	return (false);
}

void	delete_last_elem(char *path)
{
	char	*start;
	char	*last_slash_ptr;
	
	start = path;
	last_slash_ptr = NULL;
	while (*path)
	{
		if (*path == '/')
			last_slash_ptr = path;
		path++;
	}
	if (last_slash_ptr != start)
	{
		*last_slash_ptr = '\0';
	}
}

void	append_path_elem(char *dst, char **rest, char *src)
{
	size_t	elem_len;
	char	temp_elem[PATH_MAX];

	elem_len = 0;
	while (src[elem_len] && src[elem_len] != '/')
		elem_len++;
	if (dst[ft_strlen(dst) - 1] != '/')
		ft_strlcat(dst, "/", PATH_MAX);
	if (elem_len > 0)
	{
		ft_strlcat(temp_elem, src, elem_len + 1);
		ft_strlcat(dst, temp_elem, PATH_MAX);
	}
	*rest = src + elem_len;
}
// not sure strlcat is the best choice here

char	*resolve_pwd(char *oldpwd, char *path)
{
	char 	newpwd[PATH_MAX];
	char	*dup;

	if (oldpwd == NULL)
		return (NULL);
	if (*path == '/')
		ft_strlcpy(newpwd, "/", PATH_MAX);
	else
		ft_strlcpy(newpwd, oldpwd, PATH_MAX);
	while (*path)
	{
		if (*path == '/')
			path++;
		else if (consume_path(&path, path, "."))
			;
		else if (consume_path(&path, path, ".."))
			delete_last_elem(newpwd);
		else
			append_path_elem(newpwd, &path, path);
	}
	dup = ft_strdup(newpwd);
	if (dup == NULL)
		d_throw_error("resolve_pwd", "Memory allocation error");
	return (dup);
}
/*
if the path starts with "/" (absolute path), set newpwd to "/"(root directory)
if the path starts with "." (relative path), copy oldpwd to newpwd
In the loop, 
'.' means the current directory so no need to change the path
if the path starts with ".." (relative path), copy oldpwd to newpwd
*/

int	builtin_cd(int argc, char *argv[], t_map *envmap)
{
	const char	*home;
	char 		*oldpwd;
	char 		path[PATH_MAX];
	char		*newpwd;

	oldpwd = xgetenv(envmap, "PWD");
	map_set(envmap, "OLDPWD", oldpwd);
	if (argc > 2)
	{
		printf("%s: too many argument\n", argv[0]);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
	{
		home = xgetenv(envmap, "HOME");
		if (home == NULL)
		{
			builtin_error("cd", argv[0], "HOME not set");
			return (EXIT_FAILURE);
		}
		ft_strlcpy(path, home, PATH_MAX);
	}
	if (argc == 2 && argv[1][0] == '-')
	{
		ft_strlcpy(path, xgetenv(envmap, "OLDPWD"), PATH_MAX);
	}
	if (chdir(path) < 0)
	{
		perror("chdir");
		builtin_error("cd", NULL, "chdir");
		return (EXIT_FAILURE);
	}
	newpwd = resolve_pwd(oldpwd, path);
	map_set(envmap, "PWD", newpwd);
	free(newpwd);
	return (EXIT_SUCCESS);
}
/*
if no arguments are given, change to the home directory
*/
