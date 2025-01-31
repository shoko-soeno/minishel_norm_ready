/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 01:13:23 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 11:20:30 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PATH_MAX 4096

# define ERROR -1

# define EXIT_INVALID_INPUT 2
# define EXIT_EXEC_ERROR 126
# define EXIT_CMD_NOT_FOUND 127

# define MAX_PROCESS_COUNT 1024

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

typedef struct s_node	t_node;

typedef struct s_context {
	pid_t	pids[MAX_PROCESS_COUNT];
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
	t_node	**head_node;
}	t_context;

typedef struct s_syntax_error {
	char	*err_msg;
	bool	is_error;
}	t_syntax_error;

// void			start_exec(char *line, t_context *ctx);
t_context		*init_ctx(char *envp[]);
void			clear_ctx(t_context *ctx);
bool			is_blanc_line(char *line);
t_syntax_error	*init_syntax_error(void);
void			start_pipeline(char *line, t_context *ctx);
t_node			*analyze_pipeline(char *line, t_context *ctx);

void			close_stored_fds(t_context *ctx);
void			main_loop(t_context *ctx);
bool			read_command(t_context *ctx);

#endif