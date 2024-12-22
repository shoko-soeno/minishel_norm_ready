#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/*
expr    = mul ("+" mul | "-" mul)*
mul     = primary ("*" primary | "/" primary)*
primary = num | "(" expr ")"
*/

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind; // enum, nomally same size as int (4 bytes)
    Node *lhs; // pointer (8 bytes)
    Node *rhs; // pointer (8 bytes)
    int val; // int (4 bytes), totally 24 bytes
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}
/*
node is a pointer on the stack, and it points to the heap memory
calloc(1, sizeof(Node)) allocates 24 bytes on the heap
and returns the address of the first byte of the allocated memory
The type of the address which is returned by calloc is void *
So, the address is casted to Node * and assigned to node
*/

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *mul();
Node *expr();
Node *primary();
int expect_number();
void print_ast(Node *node, int depth);
int consume(char c);
void expect(char c);

/*
expr    = mul ("+" mul | "-" mul)*
*/
Node *expr()
{
    Node *node = mul();
    for (;;)
    {
        if (consume('+'))
            node = new_node(ND_ADD, node, mul());
        else if (consume('-'))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

/*
mul     = primary ("*" primary | "/" primary)*
*/
Node *mul()
{
    Node *node = primary();
    for (;;)
    {
        if (consume('*'))
            node = new_node(ND_MUL, node, primary());
        else if (consume('/'))
            node = new_node(ND_DIV, node, primary());
        else
            return node;
    }
}

/*
primary = num | "(" expr ")"
*/
Node *primary()
{
    if (consume('('))
    {
        Node *node = expr();
        expect(')');
        return node;
    }
    return new_node_num(expect_number());
}

void print_ast(Node *node, int depth)
{
    if (!node)
        return;
    
    for (int i = 0; i < depth; i++)
        printf("  ");

    switch (node->kind)
    {
        case ND_ADD:
            printf("+\n");
            break;
        case ND_SUB:
            printf("-\n");
            break;
        case ND_MUL:
            printf("*\n");
            break;
        case ND_DIV:   
            printf("/\n");
            break;
        case ND_NUM:
            printf("%d\n", node->val);
            return; // ND_NUM has no children
    }
    print_ast(node->lhs, depth + 1);
    print_ast(node->rhs, depth + 1);
}

char *current_input;

int consume(char c)
{
    if (*current_input != c)
        return 0;
    current_input++;
    return 1;
}

void expect(char c)
{
    if (*current_input != c)
    {
        fprintf(stderr, "expected %c but not found\n", c);
        exit(1);
    }
    current_input++;
}

int expect_number()
{
    if (!isdigit(*current_input))
    {
        fprintf(stderr, "expected number\n");
        exit(1);
    }
    int val = 0;
    while (isdigit(*current_input))
    {
        val = val * 10 + (*current_input - '0');
        current_input++;
    }
    return val;
}

void free_ast(Node *node)
{
    if (!node)
        return;
    free_ast(node->lhs);
    free_ast(node->rhs);
    free(node);
}

void test_parser(char *input)
{
    current_input = input;
    Node *ast = expr();

    printf("input: %s\n", input);
    printf("output:\n");
    print_ast(ast, 0);
    free_ast(ast);
}

int main() {
    // テストケースを定義
    test_parser("1+2");
    test_parser("3+4*5");
    test_parser("(1+2)*3");
    test_parser("10/(2+3)");
    test_parser("2*(3+4)-5");

    return 0;
}