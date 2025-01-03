/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 03:08:18 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/04 02:42:04 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

size_t	retrieve_val_in_sq(char **ret, char *str, size_t i)
{
	size_t	next_sq_pos;

	if (!is_s_quote(str[i]) || !ret || !str)
		d_throw_error("retrieve_val_in_sq", "arg is invalid");
	next_sq_pos = skip_s_quote(str, i);
	str_concat_helper(ret, str, i, next_sq_pos - i + 1);
	if (!is_s_quote(str[next_sq_pos]))
		d_throw_error("retrieve_val_in_sq", "unexpected error");
	return (next_sq_pos);
}

size_t	retrieve_val_in_dq(char **ret, char *str, size_t i, t_context *ctx)
{
	size_t	j;

	if (!is_d_quote(str[i]) || !ret || !str)
		d_throw_error("retrieve_val_in_dq", "arg is invalid");
	j = i + 1;
	while (str[j] && !is_d_quote(str[j]))
	{
		if (is_dollar_symbol(str[j]))
		{
			str_concat_helper(ret, str, i, j - i);
			j = retrieve_var(ret, str, j, ctx);
			i = j + 1;
		}
		j++;
	}
	str_concat_helper(ret, str, i, j - i + 1);
	if (!is_d_quote(str[j]))
		d_throw_error("retrieve_val_in_dq", "unexpected error");
	return (j);
}

size_t	retrieve_var(char **ret, char *str, size_t i, t_context *ctx)
{
	size_t	var_name_len;
	char	*tmp;
	char	*map_get_val;

	if (!str)
		d_throw_error("retrieve_var", "arg is invalid");
	if (!str[i + 1])
		return (str_concat_helper(ret, str, i, 1), i);
	var_name_len = retrieve_var_name_len(str, i + 1);
	if (!var_name_len)
		return (str_concat_helper(ret, str, i, 1), i);
	tmp = ft_substr(str, i + 1, var_name_len);
	if (!tmp)
		d_throw_error("retrieve_var", "substr failed");
	map_get_val = map_get(ctx->env, tmp);
	if (map_get_val)
		str_concat_helper(ret, map_get_val, 0, ft_strlen(map_get_val));
	else
		str_concat_helper(ret, "", 0, 0);
	free(tmp);
	return (var_name_len + i);
}

size_t	retrieve_var_name_len(char *str, size_t i)
{
	size_t	len;

	len = 0;
	if (!str)
		d_throw_error("retrieve_var_name_end", "arg is invalid");
	if (is_d_quote(str[i]) || is_s_quote(str[i]))
		return (0);
	if (!ft_isalpha(str[i]) && str[i] != UNDER_SCORE)
		return (1);
	while (ft_isalnum(str[i + len]) || str[i + len] == UNDER_SCORE)
		len++;
	return (len);
}

size_t	retrieve_normal_val(char **ret, char *str, size_t i)
{
	size_t	j;

	if (!str)
		d_throw_error("retrieve_normal_val", "arg is invalid");
	j = 0;
	while (str[i + j] && !is_d_quote(str[i + j]) && \
	!is_s_quote(str[i + j]) && !is_dollar_symbol(str[i + j]))
	{
		j++;
	}
	str_concat_helper(ret, str, i, j);
	return (i + j - 1);
}
