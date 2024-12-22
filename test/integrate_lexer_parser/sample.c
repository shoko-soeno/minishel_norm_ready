#include "../../includes/minishell.h"
#include "../lexer_test/lexer_tester.h"
#include "../parser_test/parser_tester.h"
#include <stdio.h>
#include <stdbool.h>
#include "sample.h"

static t_token *current_token = NULL;

/*------------------ Utility functions------------------ */

static void advance_token() {
    if (current_token)
        current_token = current_token->next;
}

static t_node_kind peek_kind()
{
    if (current_token == NULL)
        return ND_EOF;
    return (current_token->kind);
}

static char *current_word(void) {
    if (current_token && current_token->word)
        return current_token->word;
    return ("");
}

static void *xmalloc(size_t size)
{
    void *p = malloc(size);
    if (!p) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return p;
}

/*------------------ ASTNode Creation Functions------------------*/

static ASTNode *alloc_node(NodeType type)
{
    ASTNode *node = xmalloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

ASTNode *create_word_node(const char *word) {
    ASTNode *node = alloc_node(NODE_FILENAME);
    node->data.filename.value = strdup(word);
    if (!node->data.filename.value) {
        free(node);
        return NULL;
    }
    return node;
}

ASTNode *create_wordlist_node(char **words, size_t word_count) {
    ASTNode *node = alloc_node(NODE_WORDLIST);
    node->data.wordlist.words = words;
    node->data.wordlist.word_count = word_count;
    return node;
}

ASTNode *create_redirection_node(const char *symbol, ASTNode *filename) {
    ASTNode *node = alloc_node(NODE_REDIRECTION);
    node->data.redirection.symbol = xstrdup(symbol);
    node->data.redirection.filename = filename;
    return node;
}

ASTNode *create_simple_command_node(ASTNode **redir_before, size_t redir_before_count, ASTNode **redir_after, size_t redir_after_count, ASTNode *wordlist) {
    ASTNode *node = alloc_node(NODE_SIMPLE_COMMAND);
    node->data.simple_command.redirections_before = redir_before;
    node->data.simple_command.redirection_before_count = redir_before_count;
    node->data.simple_command.redirections_after = redir_after;
    node->data.simple_command.redirection_after_count = redir_after_count;
    node->data.simple_command.wordlist = wordlist;
    return node;
}

ASTNode *create_command_node(ASTNode *simple_command, ASTNode *command_tail) {
    ASTNode *node = alloc_node(NODE_COMMAND);
    node->data.command.simple_command = simple_command;
    node->data.command.command_tail = command_tail;
    return node;
}

ASTNode *create_command_tail_node(const char *connector, ASTNode *cmd_type_or_redir, ASTNode *next_tail)
{
    ASTNode *node = alloc_node(NODE_COMMAND_TAIL);
    if (connector) {
        node->data.command_tail.connector = xstrdup(connector);
        if (!node->data.command_tail.connector) {
            free(node);
            return NULL;
        }
    }
    node->data.command_tail.cmd_type_or_redir = cmd_type_or_redir;
    node->data.command_tail.next_tail = next_tail;
    return node;
}

/*------------------ Parse Functions------------------*/

void free_ast(ASTNode **node);

/*
simple_command: redirection* wordlist redirection*
              | redirection+
*/
static bool is_redirection_start() {
    // ND_REDIRECTSやND_FD_NUM + ND_REDIRECTSパターンを考慮
    t_node_kind k = peek_kind();
    if (k == ND_REDIRECTS)
        return true;
    if (k == ND_FD_NUM)
    {
        t_token *next = current_token->next;
        if (next && next->kind == ND_REDIRECTS) return true;
    }
    return false;
}

/*
filename: WORD
*/
ASTNode *parse_filename()
{
    if (peek_kind() != NODE_COMMAND)
    {
        return NULL;
    }
    char *filename = current_word();
    advance_token();
    return (create_filename_node(filename));
}

/*
redirection:
    ">" filename
  | "<" filename
  | ">>" filename
  | "<<" filename
  | "<>" filename
  | NUMBER ">" filename
  | NUMBER "<" filename
  | NUMBER ">>" filename
  | NUMBER "<<" filename
  | NUMBER "<>" filename
*/
ASTNode *parse_redirection()
{
    int num = -1;
    if (peek_kind() == ND_FD_NUM) {
        num = atoi(current_word());
        advance_token();
    }
    if (peek_kind() != ND_REDIRECTS) {
        return NULL; // error
    }
    char *symbol = current_word();
    advance_token();
    ASTNode *filename = parse_filename();
    if (filename == NULL) {
        return NULL;
    }
    ASTNode *redir_node = create_redirection_node(symbol, filename, num);
    if (!redir_node) {
        // create_redirection_nodeが失敗した場合はsymbolとfilenameを解放
        free_ast(&filename);
        return NULL;
    }
    return (redir_node);
}

/*
wordlist: WORD | wordlist WORD
*/
ASTNode *parse_wordlist()
{
    if (peek_kind() != ND_CMD) {
        return NULL; // error: word is a mandatory
    }
    size_t word_count = 0;
    char **words = NULL;
    while (peek_kind() == ND_CMD) {
        char *w = xstrdup(current_word());
        if (!w) {
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }
        char **new_words = realloc(words, sizeof(char *) * (word_count + 1));
        if (!new_words)
        {
            free(w);
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }
        words = new_words;
        words[word_count++] = w;
        advance_token();
    }
    ASTNode *node = create_wordlist_node(words, word_count);
    if (!node) {
        for (size_t i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
    }
    return (node);
}

ASTNode *parse_simple_command() {
    // 前部redirection読み込み
    ASTNode **redir_before = NULL;
    size_t redir_before_count = 0;
    while (is_redirection_start()) {
        ASTNode *r = parse_redirection();
        if (!r) goto error;
        ASTNode **tmp = realloc(redir_before, sizeof(ASTNode*) * (redir_before_count+1));
        if (!tmp) {
            free_ast(&r);
            goto error;
        }
        redir_before = tmp;
        redir_before[redir_before_count++] = r;
    }
    // wordlistチェック
    ASTNode *wordlist = NULL;
    if (peek_kind() == ND_CMD) {
        wordlist = parse_wordlist();
        if (!wordlist) goto error;
    }
    // 後部redirection読み込み
    ASTNode **redir_after = NULL;
    size_t redir_after_count = 0;
    while (is_redirection_start()) {
        ASTNode *r = parse_redirection();
        if (!r) goto error;
        ASTNode **tmp = realloc(redir_after, sizeof(ASTNode*) * (redir_after_count+1));
        if (!tmp) {
            free_ast(&r);
            goto error;
        }
        redir_after = tmp;
        redir_after[redir_after_count++] = r;
    }
    // wordlistも前後redirectionもない場合はエラー
    if (!wordlist && redir_before_count == 0 && redir_after_count == 0) {
        // 空コマンド
        goto error;
    }
    ASTNode *node = create_simple_command_node(redir_before, redir_before_count, redir_after, redir_after_count, wordlist);
    if (!node) {
        // ノード生成失敗時は確保済み解放
        for (size_t i = 0; i < redir_before_count; i++) free_ast(&redir_before[i]);
        free(redir_before);
        free_ast(&wordlist);
        for (size_t i = 0; i < redir_after_count; i++) free_ast(&redir_after[i]);
        free(redir_after);
    }
    return node;

    error:
        if (redir_before) {
            for (size_t i = 0; i < redir_before_count; i++) free_ast(&redir_before[i]);
            free(redir_before);
        }
        if (redir_after) {
            for (size_t i = 0; i < redir_after_count; i++) free_ast(&redir_after[i]);
            free(redir_after);
        }
        if (wordlist) free_ast(&wordlist);
    return NULL;
}


/*
command_tail:
    "|" cmd_type command_tail
  | ("&&" | "||") cmd_type command_tail
  | redirection
  |
*/
ASTNode *parse_command_tail()
{
    t_node_kind k = peek_kind();

    if (k == ND_PIPE || k == ND_OR_OP || k == ND_AND_OP)
    {
        const char *conn = current_word();
        advance_token();
        ASTNode *simple_cmd_node = parse_simple_command();
        if (!simple_cmd_node) return NULL;
        ASTNode *next_tail = parse_command_tail();
        ASTNode *node = create_command_tail_node(conn, simple_cmd_node, next_tail);
        if (!node) {
            free_ast(&simple_cmd_node);
            free_ast(&next_tail);
        }
        return node;
    }
    if (k == ND_FD_NUM || k == ND_REDIRECTS) {
        ASTNode *redir = parse_redirection();
        if (!redir) return NULL;
        ASTNode *node = create_command_tail_node(NULL, redir, NULL);
        if (!node) free_ast(&redir);
        return node;
    }
    // 空
    return NULL;
}

ASTNode *parse_command() {
    ASTNode *simple_cmd_node = parse_simple_command();
    if (!simple_cmd_node) return NULL;
    ASTNode *tail_node = parse_command_tail();
    ASTNode *node = create_command_node(simple_cmd_node, tail_node);
    if (!node) {
        free_ast(&simple_cmd_node);
        free_ast(&tail_node);
    }
    return (node);
}

ASTNode *parse_start(t_token *token_list) {
    current_token = token_list;
    ASTNode *root = parse_command();
    // 最後にND_EOFを期待してもいい(任意)
    return root;
}

/*------------------ Free Functions ------------------*/

void free_ast(ASTNode **node)
{
    if (!node || !*node)
        return;

    ASTNode *n = *node;

    switch (n->type) {
        case NODE_FILENAME:
            if (n->data.filename.value) {
                free(n->data.filename.value);
            }
            break;
        
        case NODE_WORDLIST:
            if (n->data.wordlist.words) {
                for (size_t i = 0; i < n->data.wordlist.word_count; i++) {
                    free(n->data.wordlist.words[i]);
                }
                free(n->data.wordlist.words);
            }
            break;
        
        case NODE_REDIRECTION:
            if (n->data.redirection.symbol) {
                free(n->data.redirection.symbol);
            }
            // filenameは子ノード
            free_ast(&n->data.redirection.filename);
            break;
        
        case NODE_SIMPLE_COMMAND:
            if (n->data.simple_command.redirections_before) {
                for (size_t i = 0; i < n->data.simple_command.redirection_before_count; i++) {
                    free_ast(&n->data.simple_command.redirections_before[i]);
                }
                free(n->data.simple_command.redirections_before);
            }
            if (n->data.simple_command.redirections_after) {
                for (size_t i = 0; i < n->data.simple_command.redirection_after_count; i++) {
                    free_ast(&n->data.simple_command.redirections_after[i]);
                }
                free(n->data.simple_command.redirections_after);
            }
            free_ast(&n->data.simple_command.wordlist);
            break;
        
        case NODE_COMMAND:
            free_ast(&n->data.command.simple_command);
            free_ast(&n->data.command.command_tail);
            break;
        
        case NODE_COMMAND_TAIL:
            if (n->data.command_tail.connector) {
                free(n->data.command_tail.connector);
            }
            free_ast(&n->data.command_tail.cmd_type_or_redir);
            free_ast(&n->data.command_tail.next_tail);
            break;
    }

    free(n);
    *node = NULL;
}

void free_token_list(t_token *token_list)
{
    t_token *current = token_list;
    t_token *next;

    while (current)
    {
        next = current->next;
        if (current->word)
            free(current->word);
        free(current);
        current = next;
    }
}

/*------------------ Print Functions ------------------*/

static void print_ast_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

void print_ast(ASTNode **node, int depth)
{
    if (!node || !*node)
        return;

    ASTNode *n = *node;

    print_ast_indent(depth);
    switch (n->type) {
        case NODE_FILENAME:
            printf("NODE_FILENAME: %s\n", n->data.filename.value);
            break;
        
        case NODE_WORDLIST:
            printf("NODE_WORDLIST: ");
            for (size_t i = 0; i < n->data.wordlist.word_count; i++) {
                printf("%s ", n->data.wordlist.words[i]);
            }
            printf("\n");
            break;
        
        case NODE_REDIRECTION:
            printf("NODE_REDIRECTION: symbol=%s, number=%d\n", 
                n->data.redirection.symbol,
                n->data.redirection.number);
            // filenameノードを再帰呼び出し
            print_ast(&n->data.redirection.filename, depth + 1);
            break;
        
        case NODE_SIMPLE_COMMAND:
            printf("NODE_SIMPLE_COMMAND:\n");
            // redirections_before
            for (size_t i = 0; i < n->data.simple_command.redirection_before_count; i++) {
                print_ast_indent(depth+1);
                printf("redir_before[%zu]:\n", i);
                print_ast(&n->data.simple_command.redirections_before[i], depth + 2);
            }
            // wordlist
            if (n->data.simple_command.wordlist) {
                print_ast_indent(depth+1);
                printf("wordlist:\n");
                print_ast(&n->data.simple_command.wordlist, depth + 2);
            }
            // redirections_after
            for (size_t i = 0; i < n->data.simple_command.redirection_after_count; i++) {
                print_ast_indent(depth+1);
                printf("redir_after[%zu]:\n", i);
                print_ast(&n->data.simple_command.redirections_after[i], depth + 2);
            }
            break;
        
        case NODE_COMMAND:
            printf("NODE_COMMAND:\n");
            print_ast_indent(depth+1);
            printf("simple_command:\n");
            print_ast(&n->data.command.simple_command, depth+2);
            if (n->data.command.command_tail) {
                print_ast_indent(depth+1);
                printf("command_tail:\n");
                print_ast(&n->data.command.command_tail, depth+2);
            }
            break;
        
        case NODE_COMMAND_TAIL:
            printf("NODE_COMMAND_TAIL: connector=%s\n", n->data.command_tail.connector ? n->data.command_tail.connector : "NULL");
            if (n->data.command_tail.cmd_type_or_redir) {
                print_ast_indent(depth+1);
                printf("cmd_type_or_redir:\n");
                print_ast(&n->data.command_tail.cmd_type_or_redir, depth+2);
            }
            if (n->data.command_tail.next_tail) {
                print_ast_indent(depth+1);
                printf("next_tail:\n");
                print_ast(&n->data.command_tail.next_tail, depth+2);
            }
            break;
    }
}

int main() {
    char *input = "echo hello | grep h && cat < input.txt";
    char *input2 = "cat < outfile1 > outfile2";
    t_token *tokens = lexer(input);
    ASTNode *ast = parse_start(tokens);

    // 生成されたastを処理する ...
    // astを解放する処理も必要
    print_ast(&ast, 0);
    free_ast(&ast);
    free_token_list(tokens);
    return 0;
}