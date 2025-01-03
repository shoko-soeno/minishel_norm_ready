/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 20:58:10 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/04 02:29:35 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H
# include <stdbool.h>
# include <stdlib.h>
# include "map.h"

/* t_map is a dummy head of the linked list of items
which is not counted in the size of the map
item_head is not a pointer so its memory is automatically 
allocated when t_map is created
*/

// structure for builtins
typedef struct s_builtin {
	char	*name;
	int		(*f)(int argc, char *argv[], t_context *ctx);
}	t_builtin;

t_builtin	*lookup_builtin(char *cmd);

// builtin_cd.c
int			builtin_cd(int argc, char *argv[], t_context *ctx);

// builtin_cd_utils.c
bool		consume_path(char **rest, char *path, char *elem);
void		delete_last_elem(char *path);
void		append_path_elem(char *dst, char **rest, char *src);
char		*resolve_pwd(char *pwd_before_chdir, char *path);

// builtin pwd, exit, unset, env, echo
int			builtin_pwd(int argc, char *argv[], t_context *ctx);
int			builtin_exit(int argc, char *argv[], t_context *ctx);
int			builtin_unset(int argc, char *argv[], t_context *ctx);
// int		builtin_env(int argc, char *argv[], t_map *envmap);
int			builtin_env(int argc, char *argv[], t_context *ctx);
int			builtin_echo(int argc, char *argv[], t_context *ctx);

// builtin_export.c
int			builtin_export(int argc, char **argv, t_context *ctx);
// builtin_export_utils.c
void		print_sorted_env(t_map *envmap);

#endif