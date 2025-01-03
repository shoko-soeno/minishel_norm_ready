/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:49:20 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/02 11:10:26 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

bool	is_no_newline_option(char *str)
{
	int	i;

	i = 1;
	if (!str || str[0] != '-')
		return (false);
	while (str[i] && str[i] == 'n')
	{
		i++;
		if (str[i] == '\0')
			return (true);
	}
	return (false);
}

int	builtin_echo(int argc, char *argv[], t_map *envmap, t_context *ctx)
{
	bool	is_no_newline;
	size_t	i;

	(void)argc;
	(void)envmap;
	i = 1;
	is_no_newline = false;
	while (argv[i] && is_no_newline_option(argv[i]))
	{
		is_no_newline = true;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		i++;
		if (argv[i])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!is_no_newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	ctx->last_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
