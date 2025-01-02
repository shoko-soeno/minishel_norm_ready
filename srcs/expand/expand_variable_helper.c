/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 03:08:18 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/02 15:11:48 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

size_t	retrieve_val_in_sq(char **ret, char *str, size_t i)
{
	size_t	closing_sq_pos;

	if (!is_s_quote(str[i]) || !ret || !str)
		d_throw_error("retrieve_val_in_sq", "arg is invalid");
	closing_sq_pos = skip_s_quote_block(str, i);
	append_substring(ret, str, i, closing_sq_pos - i + 1);
	if (!is_s_quote(str[closing_sq_pos]))
		d_throw_error("retrieve_val_in_sq", "unexpected error");
	return (closing_sq_pos);
}

size_t expand_inside_dq(char **ret, char *str, size_t start, t_map *envp)
{
	size_t cur;

	cur = start;
	while (str[cur] && !is_d_quote(str[cur]))
	{
		if (is_dollar_symbol(str[cur]))
			cur = retrieve_var(ret, str, cur, envp);
		else
			append_substring(ret, str, cur, 1);
		cur++;
	}
	return (cur);
}

size_t	retrieve_val_in_dq(char **ret, char *str, size_t i, t_map *envp)
{
	size_t	content_end;

	if (!ret || !str)
		d_throw_error("retrieve_val_in_dq", "arg is invalid");
	if (!is_d_quote(str[i]))
		d_throw_error("retrieve_val_in_dq", "expected double quote");
	content_end = expand_inside_dq(ret, str, i + 1, envp);
	if (!str[content_end])
		d_throw_error("retrieve_val_in_dq", "double quote not closed");
	return (content_end);
}

size_t	retrieve_var(char **ret, char *str, size_t i, t_map *envp)
{
	size_t	var_name_len;
	char	*tmp;
	char	*map_get_val;

	if (!str)
		d_throw_error("retrieve_var", "arg is invalid");
	if (!str[i + 1])
		return (append_substring(ret, str, i, 1), i);
	var_name_len = retrieve_var_name_len(str, i + 1);
	if (!var_name_len)
		return (append_substring(ret, str, i, 1), i);
	tmp = ft_substr(str, i + 1, var_name_len);
	if (!tmp)
		d_throw_error("retrieve_var", "substr failed");
	map_get_val = map_get(envp, tmp);
	if (map_get_val)
		append_substring(ret, map_get_val, 0, ft_strlen(map_get_val));
	else
		append_substring(ret, "", 0, 0);
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
	append_substring(ret, str, i, j);
	return (i + j - 1);
}
