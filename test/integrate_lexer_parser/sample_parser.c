#include "sample.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// ヘルパー関数
static void *xmalloc(size_t size) {
    void *p = malloc(size);
    if (!p) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return p;
}

// xstrdupの実装
char *xstrdup(const char *s) {
    if (s == NULL) return NULL;
    char *dup = strdup(s);
    if (!dup) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    return dup;
}

/*------------------ ASTNode Creation Functions------------------*/

ASTNode* create_command_node(ASTNode *wordlist, ASTNode *redirection) {
    ASTNode *node = xmalloc(sizeof(ASTNode));
    node->type = NODE_COMMAND;
    node->data.command.wordlist = wordlist;
    node->data.command.redirection = redirection;
    return node;
}

ASTNode* create_binary_node(NodeType type, ASTNode *left, ASTNode *right) {
    if (type != NODE_PIPE && type != NODE_AND && type != NODE_OR) {
        fprintf(stderr, "Invalid binary node type\n");
        exit(EXIT_FAILURE);
    }
    ASTNode *node = xmalloc(sizeof(ASTNode));
    node->type = type;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

ASTNode* create_redirection_node(const char *symbol, ASTNode *filename) {
    ASTNode *node = xmalloc(sizeof(ASTNode));
    node->type = NODE_REDIRECTION;
    node->data.redirection.symbol = xstrdup(symbol);
    node->data.redirection.filename = filename;
    return node;
}

ASTNode* create_wordlist_node(char **words, size_t count) {
    ASTNode *node = xmalloc(sizeof(ASTNode));
    node->type = NODE_WORDLIST;
    node->data.wordlist.words = xmalloc(sizeof(char*) * count);
    for (size_t i = 0; i < count; i++) {
        node->data.wordlist.words[i] = xstrdup(words[i]);
    }
    node->data.wordlist.count = count;
    return node;
}

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
static bool expect_token(TokenType type) {
    return current_token->type == type;
}

// トークンの値を比較し、マッチすればトークンを進める
static bool match_token(TokenType type, const char *value) {
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
    if (expect_token(TOKEN_WORD)) {
        ASTNode *filename_node = create_wordlist_node(&current_token->value, 1); // 簡略化
        advance();
        return filename_node;
    }
    parse_error("Expected filename (WORD)");
    return NULL; // 実際には到達しない
}

// parse_redirection
ASTNode *parse_redirection() {
    // オプションでNUMBER
    char *fd = -1;
    if (expect_token(TOKEN_NUMBER)) {
        fd = xstrdup(current_token->value);
        advance();
    }

    // リダイレクションの種類
    char *symbol = NULL;
    switch (current_token->type) {
        case TOKEN_REDIRECT_IN:
            symbol = xstrdup("<");
            break;
        case TOKEN_REDIRECT_OUT:
            symbol = xstrdup(">");
            break;
        case TOKEN_REDIRECT_APPEND:
            symbol = xstrdup(">>");
            break;
        case TOKEN_REDIRECT_HEREDOC:
            symbol = xstrdup("<<");
            break;
        case TOKEN_REDIRECT_READWRITE:
            symbol = xstrdup("<>");
            break;
        default:
            parse_error("Expected redirection operator");
    }

    advance(); // リダイレクション記号を進める

    // ファイル名を解析
    ASTNode *filename = parse_filename();

    // 作成
    ASTNode *redir = create_redirection_node(symbol, filename);
    free(symbol);

    // ファイルディスクリプタが指定されていれば設定
    if (fd != -1) {
        // リダイレクションノードにfdを追加する場合、構造を拡張する必要があります。
        // 今回は簡略化のため、fdを無視します。
        free(fd);
    }

    return redir;
}

// parse_wordlist: WORD | wordlist WORD
ASTNode *parse_wordlist() {
    if (!expect_token(TOKEN_WORD)) {
        parse_error("Expected WORD in wordlist");
    }

    // 単語を収集
    size_t capacity = 4;
    size_t count = 0;
    char **words = xmalloc(sizeof(char*) * capacity);

    while (expect_token(TOKEN_WORD)) {
        if (count >= capacity) {
            capacity *= 2;
            words = realloc(words, sizeof(char*) * capacity);
            if (!words) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        words[count++] = xstrdup(current_token->value);
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
    // リダイレクションの前置き
    ASTNode *redirection_left = NULL;
    if (current_token->type == TOKEN_REDIRECT_IN ||
        current_token->type == TOKEN_REDIRECT_OUT ||
        current_token->type == TOKEN_REDIRECT_APPEND ||
        current_token->type == TOKEN_REDIRECT_HEREDOC ||
        current_token->type == TOKEN_REDIRECT_READWRITE ||
        current_token->type == TOKEN_NUMBER) {
        redirection_left = parse_redirection();
    }

    // wordlist
    ASTNode *wordlist = parse_wordlist();

    // リダイレクションの後置き
    ASTNode *redirection_right = NULL;
    if (current_token->type == TOKEN_REDIRECT_IN ||
        current_token->type == TOKEN_REDIRECT_OUT ||
        current_token->type == TOKEN_REDIRECT_APPEND ||
        current_token->type == TOKEN_REDIRECT_HEREDOC ||
        current_token->type == TOKEN_REDIRECT_READWRITE ||
        current_token->type == TOKEN_NUMBER) {
        redirection_right = parse_redirection();
    }

    // シンプルコマンドノードを作成
    ASTNode *simple_cmd = create_command_node(wordlist, NULL);

    // リダイレクションがある場合、シンプルコマンドに関連付ける
    if (redirection_left) {
        // リダイレクションを左に、シンプルコマンドを右に配置
        simple_cmd = create_binary_node(NODE_REDIRECTION, redirection_left, simple_cmd);
    }

    if (redirection_right) {
        // リダイレクションを右に、シンプルコマンドを左に配置
        simple_cmd = create_binary_node(NODE_REDIRECTION, simple_cmd, redirection_right);
    }

    return simple_cmd;
}


/*
 * parse_pipe:
 * パイプ演算子 | を処理する
 * パイプはシンプルコマンドの連結を表す
 */
ASTNode *parse_pipe() {
    ASTNode *left = parse_simple_command();

    while (current_token->type == TOKEN_PIPE) {
        advance(); // パイプを進める
        ASTNode *right = parse_simple_command();
        left = create_binary_node(NODE_PIPE, left, right);
    }

    return left;
}

/*
 * parse_logical:
 * 論理演算子 && と || を処理する
 * 論理演算子はパイプよりも低い優先順位を持つ
 */
ASTNode *parse_logical() {
    ASTNode *left = parse_pipe();

    while (current_token->type == TOKEN_AND || current_token->type == TOKEN_OR) {
        NodeType op_type;
        if (current_token->type == TOKEN_AND) {
            op_type = NODE_AND;
        } else {
            op_type = NODE_OR;
        }

        advance(); // 演算子を進める

        ASTNode *right = parse_pipe();
        left = create_binary_node(op_type, left, right);
    }

    return left;
}
/*
 * parse_start:
 * パースのエントリーポイント
 */
ASTNode *parse_start(t_token *token_list) {
    current_token = token_list;
    ASTNode *command = parse_logical();
    if (current_token->type != TOKEN_EOF) {
        parse_error("Unexpected tokens after command");
    }
    return command;
}

/*------------------ Print Functions ------------------*/

#include <stdio.h>

// ヘルパー関数：インデントを印字
static void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

// print_ast 関数の実装
void print_ast(ASTNode *node, int depth) {
    if (node == NULL) {
        print_indent(depth);
        printf("NULL\n");
        return;
    }

    print_indent(depth);
    switch (node->type) {
        case NODE_COMMAND:
            printf("Command\n");
            print_ast(node->data.command.wordlist, depth + 1);
            if (node->data.command.redirection) {
                print_ast(node->data.command.redirection, depth + 1);
            }
            break;
        case NODE_PIPE:
            printf("Pipe |\n");
            print_ast(node->data.binary.left, depth + 1);
            print_ast(node->data.binary.right, depth + 1);
            break;
        case NODE_AND:
            printf("Logical AND &&\n");
            print_ast(node->data.binary.left, depth + 1);
            print_ast(node->data.binary.right, depth + 1);
            break;
        case NODE_OR:
            printf("Logical OR ||\n");
            print_ast(node->data.binary.left, depth + 1);
            print_ast(node->data.binary.right, depth + 1);
            break;
        case NODE_REDIRECTION:
            printf("Redirection %s\n", node->data.redirection.symbol);
            print_ast(node->data.redirection.filename, depth + 1);
            break;
        case NODE_WORDLIST:
            printf("WordList (%zu)\n", node->data.wordlist.count);
            for (size_t i = 0; i < node->data.wordlist.count; i++) {
                print_indent(depth + 1);
                printf("WORD: %s\n", node->data.wordlist.words[i]);
            }
            break;
        case NODE_EOF_NODE:
            printf("EOF\n");
            break;
        default:
            printf("Unknown node type\n");
    }
}

/*------------------ Memory Management Functions ------------------*/

#include <stdlib.h>

// トークンリストの解放
void free_token_list(t_token *token) {
    while (token) {
        t_token *next = token->next;
        if (token->value) free(token->value);
        free(token);
        token = next;
    }
}

// ASTの解放関数
void free_ast(ASTNode *node) {
    if (!node) return;
    switch (node->type) {
        case NODE_COMMAND:
            free_ast(node->data.command.wordlist);
            if (node->data.command.redirection) {
                free_ast(node->data.command.redirection);
            }
            break;
        case NODE_PIPE:
        case NODE_AND:
        case NODE_OR:
            free_ast(node->data.binary.left);
            free_ast(node->data.binary.right);
            break;
        case NODE_REDIRECTION:
            if (node->data.redirection.symbol) free(node->data.redirection.symbol);
            free_ast(node->data.redirection.filename);
            break;
        case NODE_WORDLIST:
            for (size_t i = 0; i < node->data.wordlist.count; i++) {
                free(node->data.wordlist.words[i]);
            }
            free(node->data.wordlist.words);
            break;
        case NODE_EOF_NODE:
            // 特別な処理は不要
            break;
        default:
            break;
    }
    free(node);
}
