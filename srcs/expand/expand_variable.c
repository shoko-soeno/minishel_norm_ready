/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:58:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/04 02:42:26 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

/*
$? -> 前に終了したステータスの中身を文字列として取り出す
$<変数名> -> 変数名に該当する変数を展開し、中身を文字列として取り出す
*展開 -> カレントディレクト下のファイル・フォルダ名に展開することが出来る。
シングルクォーテーションで囲まれている場合、展開は行わない
ダブルクォーテーションで囲まれている場合、$変数の展開は行う
*/

char	*expand_variable(char *str, t_context *ctx)
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
			i = retrieve_var(&ret, str, i, ctx);
		else if (is_d_quote(str[i]))
			i = retrieve_val_in_dq(&ret, str, i, ctx);
		else
			i = retrieve_normal_val(&ret, str, i);
		i++;
	}
	return (ret);
}
