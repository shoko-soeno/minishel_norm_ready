#include <stdio.h>
#include <stdbool.h>
#include "sample.h"

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
    node->data.wordlist.words = malloc(sizeof(char*) * word_count);
    for (size_t i = 0; i < word_count; i++) {
        node->data.wordlist.words[i] = strdup(words[i]);
    }
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
        node->data.command_tail.connector = strdup(connector);
        if (!node->data.command_tail.connector) {
            free(node);
            return NULL;
        }
    }
    node->data.command_tail.cmd_type_or_redir = cmd_type_or_redir;
    node->data.command_tail.next_tail = next_tail;
    return node;
}


/*------------------ Print Functions ------------------*/

static void print_ast_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

void print_ast(ASTNode **node, int depth);

/*------------------ Main Function ------------------*/

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

