/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2025/01/31 11:49:33 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"
#include "../includes/execute.h"
#include "../includes/utils.h"
#include "../includes/map.h"
#include "../includes/environment.h"
#include "../includes/signals.h"
#include "../includes/heredoc.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_context	*ctx;
	int			last_status;

	last_status = EXIT_SUCCESS;
	if (argc >= 2)
		ft_putendl_fd("command line arguments will be ignored", STDERR_FILENO);
	(void)argv;
	ctx = init_ctx(envp);
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
		rl_event_hook = sigint_event_hook;
	main_loop(ctx);
	last_status = ctx->last_status;
	free_ctx(&ctx);
	return (last_status);
}

void	main_loop(t_context *ctx)
{	
	while (1)
	{
		set_idle_sig_handlers();
		if (ctx->heredoc_interrupted == true)
		{
			ctx->heredoc_interrupted = false;
			g_sig = 0;
			ctx->last_status = 130;
			if (isatty(STDIN_FILENO)) // rm this if before submit
			{
				rl_event_hook = sigint_event_hook;
				continue ;
			}
			else
				break ;
		}
		if (!read_command(ctx))
			break ;
	}
}

bool	read_command(t_context *ctx)
{
	char	*line;

	line = readline("minishell$ ");
	if (line == NULL)
	{
		ft_putendl_fd("exit", STDERR_FILENO);
		return (false);
	}
	if (g_sig != 0)
	{
		ctx->last_status = g_sig + 128;
		g_sig = 0;
	}
	if (*line && !is_blanc_line(line))
	{
		add_history(line);
		start_pipeline(line, ctx);
	}
	free(line);
	line = NULL;
	return (true);
}

bool	is_blanc_line(char *line)
{
	size_t	i;

	if (!line)
		return (true);
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
			i++;
		else
			return (false);
	}
	return (true);
}
