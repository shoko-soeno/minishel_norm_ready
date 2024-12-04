/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:32:59 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/30 17:56:12 by ssoeno           ###   ########.fr       */
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
	char 		*infile;
	char 		*outfile;
	struct cmds	*next;
};

int				invoke_commands(struct cmds *cmdhead);
void			exec_pipeline(struct cmds *cmdhead);
int				wait_pipeline(struct cmds *cmdhead);
struct cmds*	pipeline_tail(struct cmds *cmdhead);
void			close_pipe_fds(int *pfd_pre, int *pfd);
void			handle_child_process(struct cmds *cur_cmd,
		int *pfd_pre, int *pfd);

#endif