/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:57:36 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/29 00:51:38 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/execute.h"
#include "../includes/expand.h"
#include "../includes/utils.h"

int	wait_children_status(t_context *ctx)
{
	int	i;
	int	c_status;

	i = -1;
	c_status = EXIT_SUCCESS;
	while (++i < ctx->cnt)
		waitpid(ctx->pids[i], &c_status, 0);
	// if (ctx->last_status != 0) // 確認　"cat < nosuchfile123" should return 1
	// 	return (ctx->last_status);
	if (WIFSIGNALED(c_status))
		ctx->last_status = WTERMSIG(c_status) + 128;
	else if (WIFEXITED(c_status))
		ctx->last_status = WEXITSTATUS(c_status);
	return (ctx->last_status);
}
/*
SIGINT 2, SIGQUIT 3
*/

void	setup_child_process_fd_flg(t_context *ctx)
{
	if (ctx->pre_in_pipe_fd != -1)
	{
		dup2(ctx->pre_in_pipe_fd, STDIN_FILENO);
		close(ctx->pre_in_pipe_fd);
	}
	if (ctx->in_pipe_fd != -1)
		close(ctx->in_pipe_fd);
	if (ctx->out_pipe_fd != -1 && ctx->out_pipe_fd != STDOUT_FILENO)
	{
		dup2(ctx->out_pipe_fd, STDOUT_FILENO);
		close(ctx->out_pipe_fd);
	}
	ctx->is_exec_in_child_ps = true;
}

bool	is_builtin(char *cmd)
{
	if (!cmd)
		d_throw_error("is_builtin", "command is null");
	if (ft_strcmp(cmd, EXIT) == 0)
		return (true);
	else if (ft_strcmp(cmd, CD) == 0)
		return (true);
	else if (ft_strcmp(cmd, PWD) == 0)
		return (true);
	else if (ft_strcmp(cmd, UNSET) == 0)
		return (true);
	else if (ft_strcmp(cmd, ENV) == 0)
		return (true);
	else if (ft_strcmp(cmd, ECHO) == 0)
		return (true);
	else if (ft_strcmp(cmd, EXPORT) == 0)
		return (true);
	else
		return (false);
}
