/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logical_operator.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:56:20 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/04 02:38:32 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer.h"
#include "../includes/execute.h"
#include "../includes/utils.h"

int	exec_or_node(t_node *node, t_context *ctx)
{
	int	status;

	ctx->is_exec_in_child_ps = false;
	status = 0;
	status = exec_handler(node->left, ctx);
	if (ctx->cnt)
		wait_children_status(ctx);
	if (!ctx->last_status && !status)
	{
		clear_pid_status(ctx);
		return (EXIT_SUCCESS);
	}
	else
	{
		ctx->is_exec_in_child_ps = false;
		clear_pid_status(ctx);
		return (exec_handler(node->right, ctx));
	}
}

int	exec_and_node(t_node *node, t_context *ctx)
{
	int	status;

	ctx->is_exec_in_child_ps = false;
	status = 0;
	status = exec_handler(node->left, ctx);
	if (ctx->cnt)
		wait_children_status(ctx);
	if (ctx->last_status || status)
	{
		clear_pid_status(ctx);
		return (EXIT_FAILURE);
	}
	else
	{
		ctx->is_exec_in_child_ps = false;
		clear_pid_status(ctx);
		return (exec_handler(node->right, ctx));
	}
}

int	exec_round_brackets(t_node *node, t_context *ctx)
{
	ctx->is_exec_in_child_ps = true;
	ctx->last_status = exec_handler(node->left, ctx);
	if (ctx->cnt)
		wait_children_status(ctx);
	clear_pid_status(ctx);
	return (ctx->last_status);
}
