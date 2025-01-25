/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:19:21 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/25 08:14:50 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"
#include "../../includes/utils.h"
#include "../../includes/execute.h"

static bool	is_digit_str(char *str);

int	builtin_exit(int argc, char *argv[], t_context *ctx)
{
	int	ret;

	ctx->last_status = EXIT_SUCCESS;
	if (!ctx->is_exec_in_child_ps)
		ft_putendl_fd(EXIT, STDERR_FILENO);
	if (argc > 2)
		return (builtin_error("exit", NULL, "too many arguments"), \
		ctx->last_status = EXIT_FAILURE, ctx->last_status);
	if (argc == 2)
	{
		if (argv[1] == NULL || !is_digit_str(argv[1]))
		{
			builtin_error("exit", argv[1], "numeric argument required");
			ctx->last_status = EXIT_INVALID_INPUT;
			ret = ctx->last_status;
			if (!ctx->is_exec_in_child_ps)
				free_ctx(&ctx);
			exit(ret);
		}
		ctx->last_status = ft_atoi(argv[1]);
	}
	ret = ctx->last_status;
	if (!ctx->is_exec_in_child_ps)
		free_ctx(&ctx);
	exit(ret);
}

static bool	is_digit_str(char *str)
{
	long	result;
	int		digit;

	result = 0;
	digit = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (ft_strcmp("-9223372036854775808", str) == 0)
		return (true);
	if (*str == '+' || *str == '-')
		str++;
	if (!ft_isdigit(*str))
		return (false);
	while (*str && ft_isdigit(*str))
	{
		digit = *str++ - '0';
		if (result > (LONG_MAX - digit) / 10)
			return (false);
		result = result * 10 + digit;
	}
	return (*str == '\0');
}
// sign = (*str != '-') * 2 - 1;
