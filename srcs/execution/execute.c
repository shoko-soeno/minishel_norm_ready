/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 01:57:54 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/04 23:17:39 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/signals.h"
#include "../includes/lexer.h"
#include "../includes/execute.h"
#include "../includes/utils.h"
#include "../includes/builtin.h"
#include "../includes/map.h"
#include "../includes/environment.h"
#include "../includes/expand.h"

int	exec_handler(t_node *ast_node, t_context *ctx)
{
	if (ast_node->kind == ND_CMD)
		return (exec_cmd_handler(ast_node, ctx));
	else if (ast_node->kind == ND_RND_BRACKET)
		return (exec_round_brackets(ast_node, ctx));
	else if (ast_node->kind == ND_PIPE)
		return (exec_pipe(ast_node, ctx));
	else if (ast_node->kind == ND_OR_OP)
		return (exec_or_node(ast_node, ctx));
	else if (ast_node->kind == ND_AND_OP)
		return (exec_and_node(ast_node, ctx));
	//else if (ast_node->kind == ND_REDIRECTS)
	//	return (exec_redirects(ast_node, envp, ctx));
	else
		return (EXIT_FAILURE);
}

int	exec_pipe(t_node *node, t_context *ctx)
{
	int	pfd[2];

	if (pipe(pfd))
		d_throw_error("exec_pipeline", "pipe is failed");
	ctx->is_exec_in_child_ps = true;
	set_pipe_fd(&ctx->in_pipe_fd, &ctx->out_pipe_fd, pfd);
	exec_handler(node->left, ctx);
	ctx->out_pipe_fd = STDOUT_FILENO;
	return (exec_handler(node->right, ctx));
}

int	exec_builtin(char *cmd, char **argv, t_context *ctx)
{
	t_builtin	*builtin;
	int			arr_size;

	builtin = lookup_builtin(cmd);
	arr_size = 0;
	while (argv[arr_size])
		arr_size++;
	if (builtin)
	{
		ctx->last_status = builtin->f(arr_size, argv, ctx);
		if (ctx->last_status != EXIT_SUCCESS)
		{
			ft_putendl_fd("exec_builtin failed\n", STDERR_FILENO);
			return (ctx->last_status);
			// exit (ctx->last_status);
		}
		return (EXIT_SUCCESS);
	}
	else
	{
		ft_putendl_fd("exec_builtin: not recognized builtin\n", STDERR_FILENO);
		ctx->last_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
}

/*

*/
int	exec_cmd(t_node *node, t_context *ctx)
{
	//signal();
	expand_handler(node, ctx);
	//redirect();
	char	*cmd_path;

	if (is_builtin(node->cmds[0]))
		return (exec_builtin(node->cmds[0], node->cmds, ctx));
	else
	{
		cmd_path = resolve_executable_path(node, ctx->env);
		if (!cmd_path)
			d_throw_error("exec_cmd", "unexpected: cmd_path is NULL");
		execve(cmd_path, node->cmds, get_environ(ctx->env));
		perror("execvp");
		free(cmd_path);
		exit(EXIT_FAILURE);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	exec_cmd_handler(t_node *node, t_context *ctx)
{
	printf("DEBUG: exec_cmd_handler\n");
	if (!ctx->is_exec_in_child_ps && is_builtin(node->cmds[0]))
		return (exec_cmd(node, ctx));
	else
	{
		ctx->pids[ctx->cnt] = fork();
		ctx->cnt += 1;
		ignore_sig(SIGINT); // while child runs, ignore SIGINT
		// ignore_sig(SIGQUIT);
		if (ctx->pids[ctx->cnt -1] == 0)
		{
			set_exec_child_handler(); // SIGINT=SIG_DFL, SIGQUIT=SIG_DFL
			setup_child_process_fd(ctx);
			exec_cmd(node, ctx);
			printf("DEBUG: exec_cmd_handler: child: ctx->last_status: %d\n", ctx->last_status);
		}
		else if (ctx->pids[ctx->cnt -1] == -1)
			d_throw_error("exec_cmd_handler", "fork is failed");
		else
		{
			reset_parent_process_fd(ctx);
			printf("DEBUG: exec_cmd_handler: ctx->last_status: %d\n", ctx->last_status);
			set_idle_handler();
		}
	}
	return (EXIT_SUCCESS);
}
