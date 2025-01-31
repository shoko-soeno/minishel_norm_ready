/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/29 23:31:14 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include "../includes/redirect.h"
#include "../includes/execute.h"

void	redirect_in(char *filename, t_context *ctx);
void	redirect_out(char *filename, t_context *ctx);
void	redirect_append(char *filename, t_context *ctx);
void	redirect_here_doc(t_node *node, t_context *ctx);

void	redirect_here_doc(t_node *node, t_context *ctx)
{
	if (node->fd_num > 2)
	{
		if (dup2(node->fd_num, STDIN_FILENO) < 0)
		{
			perror("dup2");
			ctx->last_status = EXIT_FAILURE;
			if (ctx->is_exec_in_child_ps)
				exit(EXIT_FAILURE);
			return ;
		}
		ctx->last_status = EXIT_SUCCESS;
		close(node->fd_num);
		node->fd_num = -1;
	}
}

void	redirect_in(char *filename, t_context *ctx)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror_prefix();
		perror(filename);
		ctx->last_status = EXIT_FAILURE;
		if (ctx->is_exec_in_child_ps)
			exit(ctx->last_status);
		return ;
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("dup2");
		ctx->last_status = EXIT_FAILURE;
		if (ctx->is_exec_in_child_ps)
			exit(ctx->last_status);
		return ;
	}
	close(fd);
	ctx->last_status = EXIT_SUCCESS;
}

void	redirect_out(char *filename, t_context *ctx)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror_prefix();
		perror(filename);
		ctx->last_status = EXIT_FAILURE;
		if (ctx->is_exec_in_child_ps)
			exit(ctx->last_status);
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("dup2");
		ctx->last_status = EXIT_FAILURE;
		if (ctx->is_exec_in_child_ps)
			exit(ctx->last_status);
		return ;
	}
	close(fd);
	ctx->last_status = EXIT_SUCCESS;
}

void	redirect_append(char *filename, t_context *ctx)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror_prefix();
		perror(filename);
		ctx->last_status = EXIT_FAILURE;
		if (ctx->is_exec_in_child_ps)
			exit(ctx->last_status);
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("dup2");
		ctx->last_status = EXIT_FAILURE;
		if (ctx->is_exec_in_child_ps)
			exit(ctx->last_status);
		return ;
	}
	close(fd);
	ctx->last_status = EXIT_SUCCESS;
}
