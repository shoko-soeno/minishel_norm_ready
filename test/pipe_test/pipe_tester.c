#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../includes/pipe.h"

#define INIT_ARGV 8
#define IDENT_CHAR_P(c) (!isspace((int)c) && ((c) != '|') && ((c) != '>'))

static void *xmalloc(size_t sz)
{
    void *p;

    p = calloc(1, sz);
    if (!p)
        exit(3);
    return p;
}

static void *xrealloc(void *ptr, size_t sz)
{
    void *p;

    if (!ptr) return xmalloc(sz);
    p = realloc(ptr, sz);
    if (!p)
        exit(3);
    return p;
}

static void free_cmd(struct cmds *cmdhead)
{
    if (cmdhead->next != NULL)
        free_cmd(cmdhead->next);
    free(cmdhead->cmd);
    free(cmdhead);
}

struct cmds *parse_commandline(char *p)
{
    struct cmds *cmdhead;
    cmdhead = xmalloc(sizeof(struct cmds));
    cmdhead->argc = 0;
    cmdhead->cmd = xmalloc(sizeof(char *) * INIT_ARGV);
    cmdhead->capa = INIT_ARGV;
    cmdhead->next = NULL;
    while (*p)
    {
        while (*p && isspace((int)*p))
            *p++ = '\0';
        if (! IDENT_CHAR_P(*p))
            break ;
        if (*p && IDENT_CHAR_P(*p))
        {
            if (cmdhead->capa <= cmdhead->argc)
            {
                cmdhead->capa *= 2;
                cmdhead->cmd = xrealloc(cmdhead->cmd, cmdhead->capa);
            }
            printf("DEBUG p %s\n", p);
            cmdhead->cmd[cmdhead->argc] = p;
            cmdhead->argc++;
        }
        while (*p && IDENT_CHAR_P(*p))
            p++;
    }
    printf("DEBUG cmdhead->argc: %d\n", cmdhead->argc);
    if (cmdhead->capa <= cmdhead->argc)
    {
        cmdhead->capa += 1;
        cmdhead->cmd = xrealloc(cmdhead->cmd, cmdhead->capa);
    }
    cmdhead->cmd[cmdhead->argc] = NULL; // コマンドリストの最後をNULLにする
    if (*p == '|' || *p == '>')
    {
        if (cmdhead == NULL || cmdhead->argc == 0){
            d_throw_error("parse_commandline", "syntax error");
            if (!cmdhead)
                return (free_cmd(cmdhead), NULL);
        }
        cmdhead->next = parse_commandline(p + 1);
        if (cmdhead->next == NULL || cmdhead->next->argc == 0)
        {
            d_throw_error("parse_commandline", "syntax error");
            if (!cmdhead)
                return (free_cmd(cmdhead), NULL);
        }
        if (*p == '>')
        {
            if (cmdhead->next->argc != 1)
            {
                d_throw_error("parse_commandline", "syntax error");
                if (!cmdhead)
                    return (free_cmd(cmdhead), NULL);
            }
            cmdhead->next->argc = -1;
        }
        *p = '\0';
    }
    return cmdhead;
}

void print_cmd_list(struct cmds *cmdhead)
{
    struct cmds *cmd;
    int i = 0;

    cmd = cmdhead;
    while (cmd)
    {
        printf("Node %d:\n", i++);
        printf("    argc: %d\n", cmd->argc);
        printf("    capa: %d\n", cmd->capa);
        printf("    argv: ");
        if (cmd->cmd != NULL)
        {
            for (int j = 0; cmd->cmd[j]; j++)
                printf("%s ", cmd->cmd[j]);
        }
        printf("\n");
        if (cmd->argc == -1)
            printf("    This node represents a redirect\n");
        cmd = cmd->next;
    }
}

int	main(void)
{
	int exit_status = 0;
	struct cmds *cmdhead;
    char *line = malloc(100);
	ft_strlcpy(line, "echo hello > outfile", 100);
    cmdhead = parse_commandline(line);
    print_cmd_list(cmdhead);
    if (cmdhead == NULL)
    {
        fprintf(stderr, "parse error\n");
        free(line);
        return (1);
    }
    if (cmdhead->argc > 0)
        exit_status = invoke_commands(cmdhead);
    printf("DEBUG exit_status: %d\n", exit_status);
    // free after use
    free_cmd(cmdhead);
    free(line);
	exit (0);
}
