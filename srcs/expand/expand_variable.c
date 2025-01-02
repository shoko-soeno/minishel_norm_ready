/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:58:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/02 16:43:27 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

/*
Asterisk -> not implemented yet
$? -> not implemented yet?
Single quotes (suppress expansion): Detected using is_s_quote. 
	calls retrieve_val_in_sq which skips the single quotes and 
	returns the string inside the quotes.
Double quotes (allow expansion): Detected using is_d_quote. 
	calls retrieve_val_in_dq
	which expands the variables inside the quotes.
Dollar symbol (expand variables): Detected using is_dollar_symbol. 
	calls retrieve_var which expands the variables
	by looking up its value in envp. 
	If not found, returns an empty string.
Normal value: Handled by retrieve_normal_val
	append literal characters to ret until a special character (', ", $)
*/

char	*expand_variable(char *str, t_map *envp)
{
	size_t	i;
	char	*ret;

	i = 0;
	ret = NULL;
	while (str[i])
	{
		if (is_s_quote(str[i]))
			i = retrieve_val_in_sq(&ret, str, i);
		else if (is_dollar_symbol(str[i]))
			i = retrieve_var(&ret, str, i, envp);
		else if (is_d_quote(str[i]))
			i = retrieve_val_in_dq(&ret, str, i, envp);
		else
			i = retrieve_normal_val(&ret, str, i);
		i++;
	}
	return (ret);
}
