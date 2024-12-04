/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:31:50 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/30 18:17:02 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/pipe.h"

static void redirect_stdout(char *path);

int	invoke_commands(struct cmds *cmdhead)
{
	int	exit_status;
	int	original_stdin;
	int	original_stdout;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	exec_pipeline(cmdhead);
	exit_status = wait_pipeline(cmdhead);
	
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
	return (exit_status);
}

void	exec_pipeline(struct cmds *cmdhead)
{
	struct cmds	*cur_cmd;
	int			pfd_pre[2];
	int			pfd[2];

	cur_cmd = cmdhead;
	// ft_memset(pfd_pre, -1, sizeof(pfd_pre));
	// ft_memset(pfd, -1, sizeof(pfd));
	pfd_pre[0] = -1;
	pfd_pre[1] = -1;
	pfd[0] = -1;
	pfd[1] = -1;
	while (cur_cmd && cur_cmd->argc != -1)
	{
		pfd_pre[STDIN_FILENO] = pfd[STDIN_FILENO];
		pfd_pre[STDOUT_FILENO] = pfd[STDOUT_FILENO];
		if (cur_cmd->next != NULL && pipe(pfd) < 0)
			d_throw_error("exec_pipeline", "failed to create pipe");
		cur_cmd->pid = fork();
		if (cur_cmd->pid < 0)
			d_throw_error("exec_pipeline", "failed to fork");
		if (cur_cmd->pid > 0)
		{
			close_pipe_fds(pfd_pre, pfd);
			cur_cmd = cur_cmd->next;
			continue ;
		}
		handle_child_process(cur_cmd, pfd_pre, pfd);
	}
}

/*
child process
	- FIRST command
		- close the read end of the pipe(pfd[0])
		- redirect the write end of the pipe to STDOUT
	- MIDDLE command
		- redirect the read end of the previous pipe to STDIN
		- redirect the write end of the pipe to STDOUT
	- LAST command
		- redirect the read end of the previous pipe to STDIN
		- does not write to the pipe
*/
void	close_pipe_fds(int *pfd_pre, int *pfd)
{
	if (pfd_pre[0] != -1)
		close(pfd_pre[0]);
	if (pfd_pre[1] != -1)
		close(pfd_pre[1]);
	if (pfd[1] != -1)
		close(pfd[1]);
	if (pfd[0] != -1)
		close(pfd[0]);
}

void	handle_child_process(struct cmds *cur_cmd, int *pfd_pre, int *pfd)
{
	if (pfd_pre[1] != -1)
	{
		dup2(pfd_pre[0], STDIN_FILENO);
		close_pipe_fds(pfd_pre, pfd);
	}
	if (cur_cmd->next != NULL)
	{
		dup2(pfd[1], STDOUT_FILENO);
		close_pipe_fds(pfd_pre, pfd);
	}
	if (cur_cmd->next != NULL && cur_cmd->next->argc == -1) // redirect
		redirect_stdout(cur_cmd->next->cmd[0]);
	execvp(cur_cmd->cmd[0], cur_cmd->cmd);
	d_throw_error("exec_pipeline", "failed to execvp");
}

/*
if cmd is a builtin
	call the function and return exit status
if cmd is not a builtin
	wait for the child process to finish
	and return the exit status
*/
int	wait_pipeline(struct cmds *cmdhead)
{
	struct cmds	*cmd;

	cmd = cmdhead;
	while (cmd->next)
	{
		waitpid(cmd->pid, &cmd->exit_status, 0);
		cmd = cmd->next;
	}
	return (pipeline_tail(cmdhead)->exit_status);
}

struct cmds	*pipeline_tail(struct cmds *cmdhead)
{
	struct cmds	*cmd;

	cmd = cmdhead;
	while (cmd->next)
		cmd = cmd->next;
	return (cmd);
}
/*
libftのft_lstlastと同じ機能
*/

static void redirect_stdout(char *path)
{
	int fd;
	
	close(STDOUT_FILENO);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		d_throw_error("redirect_stdout", "failed to open file");
	if (fd != STDOUT_FILENO)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
