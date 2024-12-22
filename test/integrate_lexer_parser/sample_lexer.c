#include "sample.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*
レキサーの動作：
空白文字をスキップします。
数字はファイルディスクリプタとして認識します（例：2>）。
|, ||, &&, <, >, >>, <<, <> などのリダイレクション記号やコネクタを認識します。
その他の連続する非スペース文字列を単語（WORD）として認識します。
*/

// ヘルパー関数：新しいトークンを作成
static t_token *create_token(TokenType type, const char *value) {
    t_token *token = malloc(sizeof(t_token));
    if (!token) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->next = NULL;
    return token;
}

// レキサー関数
t_token *lexer(const char *input) {
    t_token *head = NULL;
    t_token *current = NULL;
    size_t i = 0;
    size_t len = strlen(input);

    while (i < len) {
        // スペースをスキップ
        if (isspace(input[i])) {
            i++;
            continue;
        }

        // 数字（ファイルディスクリプタ）
        if (isdigit(input[i])) {
            size_t start = i;
            while (i < len && isdigit(input[i])) i++;
            char num_str[32];
            size_t num_len = i - start;
            if (num_len >= sizeof(num_str)) {
                fprintf(stderr, "Number too long\n");
                exit(EXIT_FAILURE);
            }
            strncpy(num_str, &input[start], num_len);
            num_str[num_len] = '\0';
            t_token *token = create_token(TOKEN_NUMBER, num_str);
            if (!head) {
                head = current = token;
            } else {
                current->next = token;
                current = token;
            }
            continue;
        }

        // 特殊記号の処理
        if (input[i] == '|') {
            if (i + 1 < len && input[i + 1] == '|') {
                t_token *token = create_token(TOKEN_OR, "||");
                i += 2;
                if (!head) {
                    head = current = token;
                } else {
                    current->next = token;
                    current = token;
                }
            } else {
                t_token *token = create_token(TOKEN_PIPE, "|");
                i += 1;
                if (!head) {
                    head = current = token;
                } else {
                    current->next = token;
                    current = token;
                }
            }
            continue;
        }

        if (input[i] == '&') {
            if (i + 1 < len && input[i + 1] == '&') {
                t_token *token = create_token(TOKEN_AND, "&&");
                i += 2;
                if (!head) {
                    head = current = token;
                } else {
                    current->next = token;
                    current = token;
                }
            } else {
                // 単一の&は未対応（必要に応じて追加）
                fprintf(stderr, "Unsupported token: &\n");
                exit(EXIT_FAILURE);
            }
            continue;
        }

        if (input[i] == '>') {
            if (i + 1 < len && input[i + 1] == '>') {
                t_token *token = create_token(TOKEN_REDIRECT_APPEND, ">>");
                i += 2;
                if (!head) {
                    head = current = token;
                } else {
                    current->next = token;
                    current = token;
                }
            } else {
                t_token *token = create_token(TOKEN_REDIRECT_OUT, ">");
                i += 1;
                if (!head) {
                    head = current = token;
                } else {
                    current->next = token;
                    current = token;
                }
            }
            continue;
        }

        if (input[i] == '<') {
            if (i + 1 < len && input[i + 1] == '<') {
                t_token *token = create_token(TOKEN_REDIRECT_HEREDOC, "<<");
                i += 2;
                if (!head) {
                    head = current = token;
                } else {
                    current->next = token;
                    current = token;
                }
            } else if (i + 1 < len && input[i + 1] == '>') {
                t_token *token = create_token(TOKEN_REDIRECT_READWRITE, "<>");
                i += 2;
                if (!head) {
                    head = current = token;
                } else {
                    current->next = token;
                    current = token;
                }
            } else {
                t_token *token = create_token(TOKEN_REDIRECT_IN, "<");
                i += 1;
                if (!head) {
                    head = current = token;
                } else {
                    current->next = token;
                    current = token;
                }
            }
            continue;
        }

        // 単語（WORD）の処理
        if (isgraph(input[i])) {
            size_t start = i;
            while (i < len && !isspace(input[i]) && !strchr("|&<>", input[i])) i++;
            size_t word_len = i - start;
            char *word = malloc(word_len + 1);
            if (!word) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            strncpy(word, &input[start], word_len);
            word[word_len] = '\0';
            t_token *token = create_token(TOKEN_WORD, word);
            free(word);
            if (!head) {
                head = current = token;
            } else {
                current->next = token;
                current = token;
            }
            continue;
        }

        // 未知の文字
        fprintf(stderr, "Unknown character: %c\n", input[i]);
        exit(EXIT_FAILURE);
    }

    // EOFトークンを追加
    t_token *eof_token = create_token(TOKEN_EOF, NULL);
    if (!head) {
        head = current = eof_token;
    } else {
        current->next = eof_token;
        current = eof_token;
    }

    return head;
}
