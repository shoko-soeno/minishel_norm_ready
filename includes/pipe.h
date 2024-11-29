/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:32:59 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/29 22:50:56 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_H
# define PIPE_H

struct cmds {
	int			argc;
	char		**cmd;
	pid_t		pid;
	int			exit_status;
	int			capa;
	struct cmds	*next;
};

int		invoke_commands(struct cmds *cmdhead);
void	exec_pipeline(struct cmds *cmdhead, int cp_fd[2]);
int		wait_pipeline(struct cmds *cmdhead);
struct cmds*	pipeline_tail(struct cmds *cmdhead);
void    handle_parent_process(int *pfd_pre, int *pfd);
void   handle_child_process(struct cmds *cur_cmd,
    int *pfd_pre, int *pfd, int cp_fd[2]);

#endif