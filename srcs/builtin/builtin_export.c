/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:49:49 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/04 02:36:38 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"
#include "../../includes/map.h"

static bool	is_valid_env_name(char *str);
int			update_key_value(t_map *map, const char *str, bool allow_empty);

int	builtin_export(int argc, char **argv, t_context *ctx)
{
	size_t	i;

	(void)argc;
	if (ctx->env == NULL)
	{
		builtin_error("builtin_export", NULL, "map is not initialized");
		ctx->last_status = EXIT_INVALID_INPUT;
		return (ctx->last_status);
	}
	if (argv[1] == NULL)
	{
		print_sorted_env(ctx->env);
		ctx->last_status = EXIT_SUCCESS;
		return (ctx->last_status);
	}
	i = 1;
	while (argv[i])
	{
		if (update_key_value(ctx->env, argv[i++], true) < 0)
		{
			builtin_error("export ", argv[i - 1], " not a valid identifier");
			ctx->last_status = EXIT_FAILURE;
		}
	}
	return (ctx->last_status);
}

// similar to map_put, but checks the name for validity
int	update_key_value(t_map *map, const char *str, bool allow_empty)
{
	int		result;
	char	*name;
	char	*value;

	name = NULL;
	value = NULL;
	result = split_name_value(str, allow_empty, &name, &value);
	if (result != 0)
		return (result);
	if (!is_valid_env_name(name))
	{
		free(name);
		free(value);
		return (ERROR);
	}
	result = map_set(map, name, value);
	free(name);
	free(value);
	return (EXIT_SUCCESS);
}

static bool	is_valid_env_name(char *str)
{
	size_t	i;

	i = 1;
	if (!str || str[0] == '\0' || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (false);
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}
