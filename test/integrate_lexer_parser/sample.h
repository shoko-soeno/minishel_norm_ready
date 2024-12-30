#include <stdlib.h>
#include <string.h>

// 先にforward宣言を行い、相互再帰構造を可能にする
typedef struct ASTNode ASTNode;

typedef enum {
    NODE_COMMAND,
    NODE_SIMPLE_COMMAND,
    NODE_COMMAND_TAIL,
    NODE_REDIRECTION,
    NODE_WORDLIST,
    NODE_FILENAME
} NodeType;

// word, numberなどの文字列を保持
typedef struct {
    char *value;
} WordNodeData;

typedef struct {
    char **words;
    size_t word_count;
} WordListData;

typedef struct {
    char *symbol; // redirectionの種類　<, >, >>, <<など
    ASTNode *filename; // filename node
    int number; // NUMBERがある場合はその値を保持（なければ-1などで無効化）
} RedirectionData;

// simple commandは　前後にredirectionがつく場合がある。
// wordlistを持つ場合もある
// redirectionsは複数とれるのでリスト化
typedef struct {
    ASTNode **redirections_before;
    size_t redirection_before_count;
    ASTNode **redirections_after;
    size_t redirection_after_count;
    ASTNode *wordlist;
} SimpleCommandData;

typedef struct {
    ASTNode *simple_command;
    ASTNode *command_tail; // command_tailは再帰的構造になる
} CommandData;

/*
command_tail: 
   "|" cmd_type command_tail 
 | ("&&" | "||") cmd_type command_tail
 | redirection
 |
ここではオペレータ（|, &&, ||）とcmd_type、またはredirectionを格納
空(ε)の場合はNULLになる。
*/
typedef struct {
    char *connector; // &&, ||, |
    ASTNode *cmd_type_or_redir;
    ASTNode *next_tail;
} CommandTailData;

struct ASTNode {
    NodeType type;
    union {
        CommandData command;
        SimpleCommandData simple_command;
        CommandTailData command_tail;
        RedirectionData redirection;
        WordListData wordlist;
        WordNodeData filename; // filenameはwordとほぼ同じ構造だが、リダイレクトの種類が違うため分ける
    } data;
};

/*
ASTNodeにはtypeとdataがあります。
dataはunionなので、typeによって有効なメンバが異なります。
typeがNODE_COMMANDならdata.commandが意味を持ちます。
CommandDataはNODE_COMMANDノードが保持するデータ構造で、
simple_commandとcommand_tailという2つのASTNode*を持っています。

ASTNode (NODE_COMMAND)
+--------------------------------------+
| type = NODE_COMMAND                  |
|                                      |
|  data (union)                        |
|   +----------------------------------+
|   | command (CommandData)            |
|   |   +----------------------------+ |
|   |   | simple_command (ASTNode*)  | |
|   |   | command_tail  (ASTNode*)   | |
|   |   +----------------------------+ |
|   +----------------------------------+
+--------------------------------------+
CommandDataが持つsimple_commandやcommand_tailは、また別のASTNodeオブジェクトへのポインタです。
つまり、NODE_COMMAND型のASTNode（親ノード）は、
CommandDataを通して子ノード（simple_commandとcommand_tail）を参照します。
例えば、simple_commandがNODE_SIMPLE_COMMANDタイプのASTNodeだとしたら、以下のようにツリー状に繋がります。

ASTNode (NODE_COMMAND)
+--------------------------------------+
| type = NODE_COMMAND                  |
| data.command.simple_command ---------+----> ASTNode (NODE_SIMPLE_COMMAND)
| data.command.command_tail   ---------+----> ASTNode (NODE_COMMAND_TAIL)
+--------------------------------------+
*/