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
            cmdhead->cmd[cmdhead->argc] = p;
            cmdhead->argc++;
        }
        while (*p && IDENT_CHAR_P(*p))
            p++;
    }
    if (cmdhead->capa <= cmdhead->argc)
    {
        cmdhead->capa += 1;
        cmdhead->cmd = xrealloc(cmdhead->cmd, cmdhead->capa);
    }
    cmdhead->cmd[cmdhead->argc] = NULL;
    if (*p == '|' || *p == '>')
    {
        if (cmdhead == NULL || cmdhead->argc == 0)
            d_throw_error("parse_commandline", "syntax error"); if (cmdhead) free_cmd(cmdhead);
        cmdhead->next = parse_commandline(p + 1);
        if (cmdhead->next == NULL || cmdhead->next->argc == 0)
            d_throw_error("parse_commandline", "syntax error"); if (cmdhead) free_cmd(cmdhead);
        if (*p == '>')
        {
            if (cmdhead->next->argc != 1)
            {
                d_throw_error("parse_commandline", "syntax error"); if (cmdhead) free_cmd(cmdhead);
                cmdhead->next->argc = -1;
            }
        }
        *p = '\0';
    }
    return cmdhead;
}

int	main(void)
{
	int exit_status;
	struct cmds *cmdhead;
    char *line;
	
    rl_outstream = stderr;
	while (1)
	{
        line = readline("minishell$ ");
        if (line == NULL)
            break ;
        cmdhead = parse_commandline(line);
        if (cmdhead == NULL)
        {
            fprintf(stderr, "parse error\n");
            return ;
        }
        exit_status = invoke_commands(cmdhead);
        // free after use
        struct cmds *tmp;
        while (cmdhead)
        {
            tmp = cmdhead;
            cmdhead = cmdhead->next;
            int i = 0;
            while (i < tmp->argc)
                free(tmp->cmd[i++]);
            free(tmp->cmd);
            free(tmp);
        }
        free(line);
    }
	exit (0);
}

