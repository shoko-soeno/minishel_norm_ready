#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

/*
this parser will create AST based on below eBNF.
    start: command
	command: simple_command command_tail
	command_tail: "|" simple_command command_tail 
            | ("&&" | "||") simple_command command_tail
            | redirection
			|
	simple_command: redirection* wordlist redirection* | redirection+
    wordlist: WORD | wordlist WORD
    redirection: ">" filename
               | "<" filename
               | ">>" filename
               | "<<" filename
               | "<>" filename
    filename: WORD
    WORD: /[a-zA-Z0-9_"]+/
*/

// 既存の定義...

typedef enum {
    NODE_COMMAND,       // シンプルコマンド
    NODE_PIPE,          // パイプ（|）
    NODE_AND,           // 論理AND（&&）
    NODE_OR,            // 論理OR（||）
    NODE_REDIRECTION,   // リダイレクション
    NODE_WORDLIST,      // ワードリスト
    NODE_EOF_NODE       // End of File ノード
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        // シンプルコマンド
        struct {
            struct ASTNode *wordlist;
            struct ASTNode *redirection; // シンプルコマンドに1つのリダイレクションを関連付け
        } command;

        // パイプラインや論理演算子（二分木）
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
        } binary;

        // リダイレクション
        struct {
            char *symbol;
            struct ASTNode *filename;
        } redirection;

        // ワードリスト
        struct {
            char **words;
            size_t count;
        } wordlist;
    } data;
} ASTNode;

// トークン関連の定義（既存のものを維持）
typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,             // |
    TOKEN_AND,              // &&
    TOKEN_OR,               // ||
    TOKEN_REDIRECT_IN,      // <
    TOKEN_REDIRECT_OUT,     // >
    TOKEN_REDIRECT_APPEND,  // >>
    TOKEN_REDIRECT_HEREDOC, // <<
    TOKEN_REDIRECT_READWRITE, // <>
    TOKEN_NUMBER,           // ファイルディスクリプタの番号
    TOKEN_EOF
} TokenType;

typedef struct Token {
    TokenType type;
    char *value;
    struct Token *next;
} t_token;

// レキサー関数のプロトタイプ
t_token *lexer(const char *input);

// ASTノード作成関数
ASTNode* create_command_node(ASTNode *wordlist, ASTNode *redirection);
ASTNode* create_binary_node(NodeType type, ASTNode *left, ASTNode *right);
ASTNode* create_redirection_node(const char *symbol, ASTNode *filename);
ASTNode* create_wordlist_node(char **words, size_t count);

// パース関数
ASTNode *parse_start(t_token *token_list);

// AST表示関数
void print_ast(ASTNode *node, int depth);

// メモリ解放関数
void free_ast(ASTNode *node);
void free_token_list(t_token *token);
