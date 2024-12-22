#include "sample.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------------ Parse Functions------------------*/

// グローバルなトークンポインタ
static t_token *current_token;

// エラーハンドリング関数
static void parse_error(const char *msg) {
    fprintf(stderr, "Parse error: %s\n", msg);
    exit(EXIT_FAILURE);
}

// トークンを進める関数
static void advance() {
    if (current_token->type != TOKEN_EOF) {
        current_token = current_token->next;
    }
}

// 現在のトークンが期待するタイプか確認
static bool expect(TokenType type) {
    return current_token->type == type;
}

// トークンの値を比較し、マッチすればトークンを進める
static bool match(TokenType type, const char *value) {
    if (current_token->type == type) {
        if (value == NULL || (current_token->value && strcmp(current_token->value, value) == 0)) {
            advance();
            return true;
        }
    }
    return false;
}

// parse_filename: WORD
ASTNode *parse_filename() {
    if (expect(TOKEN_WORD)) {
        ASTNode *filename = create_word_node(current_token->value);
        advance();
        return filename;
    }
    parse_error("Expected filename (WORD)");
    return NULL; // 実際には到達しない
}

// parse_redirection
ASTNode *parse_redirection() {
    // オプションでNUMBER
    int fd = -1;
    if (expect(TOKEN_NUMBER)) {
        fd = atoi(current_token->value);
        advance();
    }

    // リダイレクションの種類
    char *symbol = NULL;
    switch (current_token->type) {
        case TOKEN_REDIRECT_IN:
            symbol = "<";
            break;
        case TOKEN_REDIRECT_OUT:
            symbol = ">";
            break;
        case TOKEN_REDIRECT_APPEND:
            symbol = ">>";
            break;
        case TOKEN_REDIRECT_HEREDOC:
            symbol = "<<";
            break;
        case TOKEN_REDIRECT_READWRITE:
            symbol = "<>";
            break;
        default:
            parse_error("Expected redirection operator");
    }

    // リダイレクション記号を取得
    symbol = strdup(current_token->value);
    advance();

    // ファイル名を解析
    ASTNode *filename = parse_filename();

    // 作成
    ASTNode *redir = create_redirection_node(symbol, filename);
    free(symbol);

    return redir;
}

// parse_wordlist: WORD | wordlist WORD
ASTNode *parse_wordlist() {
    if (!expect(TOKEN_WORD)) {
        parse_error("Expected WORD in wordlist");
    }

    // 単語を収集
    size_t capacity = 4;
    size_t count = 0;
    char **words = malloc(sizeof(char*) * capacity);
    if (!words) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (expect(TOKEN_WORD)) {
        if (count >= capacity) {
            capacity *= 2;
            words = realloc(words, sizeof(char*) * capacity);
            if (!words) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        words[count++] = strdup(current_token->value);
        advance();
    }

    ASTNode *wordlist = create_wordlist_node(words, count);

    // メモリ解放
    for (size_t i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);

    return wordlist;
}

// parse_simple_command
ASTNode *parse_simple_command() {
    ASTNode **redirs_before = NULL;
    size_t redir_before_count = 0;

    // リダイレクションの前置き
    while (current_token->type == TOKEN_NUMBER || current_token->type == TOKEN_REDIRECT_IN ||
           current_token->type == TOKEN_REDIRECT_OUT ||
           current_token->type == TOKEN_REDIRECT_APPEND ||
           current_token->type == TOKEN_REDIRECT_HEREDOC ||
           current_token->type == TOKEN_REDIRECT_READWRITE) {
        ASTNode *redir = parse_redirection();
        redirs_before = realloc(redirs_before, sizeof(ASTNode*) * (redir_before_count + 1));
        redirs_before[redir_before_count++] = redir;
    }

    // wordlist
    ASTNode *wordlist = parse_wordlist();

    // リダイレクションの後置き
    ASTNode **redirs_after = NULL;
    size_t redir_after_count = 0;
    while (current_token->type == TOKEN_NUMBER || current_token->type == TOKEN_REDIRECT_IN ||
           current_token->type == TOKEN_REDIRECT_OUT ||
           current_token->type == TOKEN_REDIRECT_APPEND ||
           current_token->type == TOKEN_REDIRECT_HEREDOC ||
           current_token->type == TOKEN_REDIRECT_READWRITE) {
        ASTNode *redir = parse_redirection();
        redirs_after = realloc(redirs_after, sizeof(ASTNode*) * (redir_after_count + 1));
        redirs_after[redir_after_count++] = redir;
    }

    return create_simple_command_node(redirs_before, redir_before_count, redirs_after, redir_after_count, wordlist);
}

// parse_command_tail
ASTNode *parse_command_tail() {
    if (expect(TOKEN_PIPE)) {
        char *connector = strdup("|");
        advance();
        ASTNode *cmd = parse_simple_command();
        ASTNode *tail = parse_command_tail();
        return create_command_tail_node(connector, cmd, tail);
    }

    if (expect(TOKEN_AND)) {
        char *connector = strdup("&&");
        advance();
        ASTNode *cmd = parse_simple_command();
        ASTNode *tail = parse_command_tail();
        return create_command_tail_node(connector, cmd, tail);
    }

    if (expect(TOKEN_OR)) {
        char *connector = strdup("||");
        advance();
        ASTNode *cmd = parse_simple_command();
        ASTNode *tail = parse_command_tail();
        return create_command_tail_node(connector, cmd, tail);
    }

    // リダイレクションのみの場合
    if (current_token->type == TOKEN_REDIRECT_IN ||
        current_token->type == TOKEN_REDIRECT_OUT ||
        current_token->type == TOKEN_REDIRECT_APPEND ||
        current_token->type == TOKEN_REDIRECT_HEREDOC ||
        current_token->type == TOKEN_REDIRECT_READWRITE ||
        current_token->type == TOKEN_NUMBER) {
        ASTNode *redir = parse_redirection();
        return create_command_tail_node(NULL, redir, NULL);
    }

    // 空の場合
    return NULL;
}

// parse_command
ASTNode *parse_command() {
    ASTNode *simple_cmd = parse_simple_command();
    ASTNode *cmd_tail = parse_command_tail();
    return create_command_node(simple_cmd, cmd_tail);
}

// parse_start
ASTNode *parse_start(t_token *token_list) {
    current_token = token_list;
    ASTNode *command = parse_command();
    if (!expect(TOKEN_EOF)) {
        parse_error("Unexpected tokens after command");
    }
    return command;
}
