/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 03:12:31 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/02 15:06:16 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

char	*remove_quotes(char *str)
{
	char	*ret;
	size_t	i;
	size_t	j;
	size_t	quote_end;

	if (!str)
		d_throw_error("remove_quotes", "unexpected error. str is null");
	ret = (char *)xmalloc(sizeof(char) * (ft_strlen(str) + 1));
	i = 0;
	j = 0;
	while(str[i])
	{
		if (is_s_quote(str[i]) || is_d_quote(str[i]))
		{
			quote_end = move_to_next_quotation_expnd(str, i);
			i++;
			while (i < quote_end)
				ret[j++] = str[i++];
			i++;
		}
		else
			ret[j++] = str[i++];
	}
	ret[j] = '\0';
	return (ret);
}

int	move_to_next_quotation_expnd(char *input, int i)
{
	if (is_s_quote(input[i]))
	{
		i++;
		while (input[i] && !is_s_quote(input[i]))
			i++;
		if (!input[i])
			d_throw_error("move_to_next_quotation", "squote not closed");
	}
	else if (is_d_quote(input[i]))
	{
		i++;
		while (input[i] && !is_d_quote(input[i]))
			i++;
		if (!input[i])
			d_throw_error("move_to_next_quotation", "dquote not closed");
	}
	return (i);
}
