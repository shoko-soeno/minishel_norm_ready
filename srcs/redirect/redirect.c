/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:34:47 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/29 21:53:26 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include "../includes/redirect.h"
#include "../includes/execute.h"

int	apply_redirects(t_node *node, t_context *ctx);

int	set_redirect_fds(t_node *node, t_context *ctx)
{
	if (!node)
		return (ERROR);
	if (!node->redirects)
		return (ERROR);
	return (apply_redirects(node, ctx));
}

int	apply_redirects(t_node *node, t_context *ctx)
{
	int		i;
	char	*op;
	char	*filename;

	i = 0;
	while (node->redirects[i])
	{
		op = node->redirects[i];
		filename = node->redirects[i + 1];
		if (!filename)
			d_throw_error("apply_redirect_fds", "filename is NULL");
		if (ft_strcmp(op, ">") == 0)
			redirect_out(filename, ctx);
		else if (ft_strcmp(op, ">>") == 0)
			redirect_append(filename, ctx);
		else if (ft_strcmp(op, "<") == 0)
			redirect_in(filename, ctx);
		else if (ft_strcmp(op, "<<") == 0)
			redirect_here_doc(node, ctx);
		else
			d_throw_error("apply_redirect_fds", "unexpected operator");
		if (ctx->last_status != EXIT_SUCCESS)
			break ;
		i += 2;
	}
	// return (EXIT_SUCCESS);
	return (ctx->last_status);
}
