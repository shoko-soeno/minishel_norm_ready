/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:31:55 by shokosoeno        #+#    #+#             */
/*   Updated: 2025/01/19 15:54:20 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PATH_MAX 4096

# define ERROR -1

# define EXIT_INVALID_INPUT 2
# define EXIT_EXEC_ERROR 126
# define EXIT_CMD_NOT_FOUND 127

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <stdbool.h>
# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>
# include "map.h"

typedef struct s_context {
	pid_t	pids[5000];
	int		in_pipe_fd;
	int		out_pipe_fd;
	int		pre_in_pipe_fd;
	int		cnt;
	bool	is_exec_in_child_ps;
	bool	is_in_round_bracket;
	int		last_status;
	int		stored_stdin;
	int		stored_stdout;
	bool	heredoc_interrupted;
	t_map	*env;
}	t_context;

void		start_exec(char *line, t_context *ctx);
t_context	*init_ctx(void);
void		clear_ctx(t_context *ctx);

#endif