/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:33:29 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/10 15:56:49 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#define ERROR_PREFIX "minishell: "
/*
This function for testing purpose. When you submit this code, this should be
replased
*/

void	d_throw_error(char *func_name, char *error_msg)
{
	printf("Error: function name: %s, Error message: %s", func_name, error_msg);
	exit(EXIT_FAILURE);
}

void	perror_prefix(void)
{
	ft_putstr_fd(ERROR_PREFIX, STDERR_FILENO);
}

void	builtin_error(char *func, char *name, char *err)
{
	perror_prefix();
	ft_putstr_fd(func, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (name)
	{
		ft_putstr_fd("`", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putstr_fd("': ", STDERR_FILENO);
	}
	ft_putstr_fd(err, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
