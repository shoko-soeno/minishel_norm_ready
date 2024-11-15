#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

int	builtin_env(int argc, char *argv[], t_map *envmap)
{
	t_item	*cur;

	(void) argc;
	(void) argv;
	cur = envmap->item_head.next;
	while (cur)
	{
		if (cur->value)
			printf("%s=%s\n", cur->name, cur->value);
		cur = cur->next;
	}
	printf("_=/usr/bin/env\n");
	return (EXIT_SUCCESS);
}
/* print "_=/usr/bin/env" to emulate 
the behavior of the env command.
shell commands are executed with _ set to the path of the command
so that the command can know its own path
echo $_ prints the path of the command
 */
