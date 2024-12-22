#include <stdio.h>
#include <stdbool.h>
#include "sample.h"

// print_ast 関数の実装
void print_ast(ASTNode **node, int depth) {
    if (*node == NULL) {
        print_ast_indent(depth);
        printf("NULL\n");
        return;
    }

    print_ast_indent(depth);
    switch ((*node)->type) {
        case NODE_COMMAND:
            printf("Command\n");
            print_ast(&((*node)->data.command.simple_command), depth + 1);
            print_ast(&((*node)->data.command.command_tail), depth + 1);
            break;
        case NODE_SIMPLE_COMMAND:
            printf("SimpleCommand\n");
            // リダイレクション前
            print_ast_indent(depth + 1);
            printf("Redirections Before (%zu):\n", (*node)->data.simple_command.redirection_before_count);
            for (size_t i = 0; i < (*node)->data.simple_command.redirection_before_count; i++) {
                print_ast(&((*node)->data.simple_command.redirections_before[i]), depth + 2);
            }
            // ワードリスト
            print_ast(&((*node)->data.simple_command.wordlist), depth + 1);
            // リダイレクション後
            print_ast_indent(depth + 1);
            printf("Redirections After (%zu):\n", (*node)->data.simple_command.redirection_after_count);
            for (size_t i = 0; i < (*node)->data.simple_command.redirection_after_count; i++) {
                print_ast(&((*node)->data.simple_command.redirections_after[i]), depth + 2);
            }
            break;
        case NODE_COMMAND_TAIL:
            printf("CommandTail\n");
            if ((*node)->data.command_tail.connector) {
                print_ast_indent(depth + 1);
                printf("Connector: %s\n", (*node)->data.command_tail.connector);
            }
            print_ast(&((*node)->data.command_tail.cmd_type_or_redir), depth + 1);
            print_ast(&((*node)->data.command_tail.next_tail), depth + 1);
            break;
        case NODE_REDIRECTION:
            printf("Redirection\n");
            print_ast_indent(depth + 1);
            printf("Symbol: %s\n", (*node)->data.redirection.symbol);
            print_ast(&((*node)->data.redirection.filename), depth + 1);
            printf("Number: %d\n", (*node)->data.redirection.number);
            break;
        case NODE_WORDLIST:
            printf("WordList (%zu)\n", (*node)->data.wordlist.word_count);
            for (size_t i = 0; i < (*node)->data.wordlist.word_count; i++) {
                print_ast_indent(depth + 1);
                printf("WORD: %s\n", (*node)->data.wordlist.words[i]);
            }
            break;
        case NODE_FILENAME:
            printf("Filename: %s\n", (*node)->data.filename.value);
            break;
        default:
            printf("Unknown node type\n");
    }
}
