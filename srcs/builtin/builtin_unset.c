/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:50:01 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/04 02:37:39 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/builtin.h"

int	builtin_unset(int argc, char *argv[], t_context *ctx)
{
	size_t	i;

	(void)argc;
	i = 1;
	if (ctx->env == NULL)
	{
		builtin_error("builtin_unset", NULL, "map is not initialized");
		ctx->last_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	while (argv[i])
	{
		if (map_unset(ctx->env, argv[i]) < 0)
		{
			builtin_error("unset", argv[i], "not a valid identifier");
			ctx->last_status = 1;
		}
		else
			ctx->last_status = 0;
		i++;
	}
	return (ctx->last_status);
}
