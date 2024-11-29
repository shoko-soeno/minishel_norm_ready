/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:51:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/27 23:39:42 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

/*
this parser will create AST based on below eBNF.
    start: command
	command: cmd_type command_tail
	command_tail: "|" cmd_type command_tail 
            | ("&&" | "||") cmd_type command_tail
            | redirection
			|
	cmd_type: simple_command | subshell
	subshell: "(" command ")" command_tail*
	simple_command: redirection* wordlist redirection* | redirection+
    wordlist: WORD | wordlist WORD
    redirection: ">" filename
               | "<" filename
               | ">>" filename
               | "<<" filename
               | "<>" filename
               | NUMBER ">" filename
               | NUMBER "<" filename
               | NUMBER ">>" filename
               | NUMBER "<<" filename
               | NUMBER "<>" filename
    filename: WORD
    WORD: /[a-zA-Z0-9_"]+/
    NUMBER: /[0-9]+/
				
*は0回以上
+は1回以上
例えば
< input.txt cat > output.txt
最初の redirection*: < input.txt
wordlist: ["cat"]
後半の redirection*: > output.txt

redirection+
> output.txt
1回のリダイレクトだけで構成。wordlist がない
< input.txt > output.txt
複数のリダイレクトが連続

>> fileに追記
<< here document
<> fileを読み書き両方で開く
NUMBER> はファイルディスクリプタ付きのリダイレクト（例えば 2> error.log）

echo "hello" > output.txt
(simple_command)
  ├── (wordlist)
  │     ├── "echo"
  │     └── "Hello"
  └── (redirection)
        ├── ">"
        └── "output.txt"
*/

/*
今あまり明確になっていないところ
・どんなときならノードを作るのか -> オペレーターもしくはコマンドのときのみ
・親ノードを追加する場合や子ノードを追加するときの切り分け方　-> 実際にほしい木を意識してみるとわかりやすい
・現在の問題点 ((ls)) のような場合にパーサーエラーが発生しない
・match_token を行う場合と単純に判定を行う場合で分ける必要がありそう。基本的には、match は実際のトークン作成時に行うのがベター
・match_token を行う際に実際のトークンも勧めていく形にする
修正ポイント　11/17
・match_token / compare_token の箇所は、t_token_kind に合わせる。一方で、t_token_kind の種類を増やす
*/

t_node	*parse_cmd(t_token **token_list)
{
	t_node	*node;

	node = NULL;
	node = parse_cmd_type(token_list);
	if (!node)
		d_throw_error("parse_cmd", "node is empty");
	return (parse_cmd_tail(node, token_list));
}

t_node	*parse_cmd_type(t_token **token_list)
{
	t_node	*node;

	if (compare_token(ND_L_PARE, token_list))
	{
		node = parse_subshell(token_list);
		return (node);
	}	
	else
		return (simple_cmd(token_list));
}
// 左括弧から始まるときはサブシェル
// それ以外はシンプルコマンド

t_node	*simple_cmd(t_token **token_list)
{
	t_node	*node;
	t_node	*tmp;

	node = create_node(ND_CMD);
	if (compare_token(ND_REDIRECTS, token_list))
		node->left = parse_redirects(token_list);
	node->cmds = parse_words(token_list);
	if (!node->cmds && !node->left)
	{
		free(node);
		return (NULL);
	}
	else if (!node->cmds && node->left)
	{
		tmp = node->left;
		free(node);
		return (tmp);
	}
	else if (compare_token(ND_REDIRECTS, token_list))
		node->right = parse_redirects(token_list);
	return (node);
}
// 先頭にリダイレクトがあるときは左子ノードとして保存
// 引数や実行ファイル名はparse_wordsで取得しnode->cmds に保存
// コマンドやリダイレクトもない場合はNULLを返す
// コマンドやリダイレクトのみの場合、左子ノードを返す
// 両方がある場合、ノードを返す。
// 後続のリダイレクトがあれば右子ノードとして追加

t_node	*parse_subshell(t_token **token_list)
{
	t_node	*node;

	if (!match_token(ND_L_PARE, token_list))
		d_throw_error("parser_subshell", "syntax_error");
	node = create_node(ND_SUB_SHELL);
	node->left = parse_cmd(token_list);
	if (!match_token(ND_R_PARE, token_list))
		d_throw_error("parser_subshell", "syntax_error");
	return (node);
}
/*
左括弧(が存在しなければエラー
サブシェル内のコマンドをparse_cmdで解析し、左子ノードとして保存
右括弧)が存在しなければエラー
*/

t_node	*parse_cmd_tail(t_node *left, t_token **token_list)
{
	t_node	*node;

	node = NULL;
	if (match_token(ND_PIPE, token_list))
	{
		node = create_node(ND_PIPE);
		node->left = left;
		node->right = parse_cmd_type(token_list);
		return (parse_cmd_tail(node, token_list));
	}
	else if (compare_token(ND_AND_OP, token_list) \
	|| compare_token(ND_OR_OP, token_list))
	{
		node = create_logi_node(left, token_list);
		if (compare_token(ND_PIPE, token_list))
		{
			node->right = parse_cmd_tail(node->right, token_list);
			return (node);
		}
		else
			return (parse_cmd_tail(node, token_list));
	}
	else if (compare_token(ND_REDIRECTS, token_list))
		return (parse_redirects(token_list));
	return (left);
}
/*
現在のトークンがパイプ|であれば、パイプノード（ND_PIPE）を作成
左ノードを保存し、右ノードをparse_cmd_typeで解析
再帰的に次のコマンドを解析
論理演算子（&&または||）が続く場合:
ロジックノードを作成し、右側のコマンドを解析
パイプが右側に続く場合、再帰的に処理
リダイレクトが続く場合: parse_redirectsを呼び出して処理
何も該当しなければ左ノードをそのまま返す
*/

