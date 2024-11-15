/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tester.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 22:08:43 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/15 23:23:50 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"

typedef struct
{
	const char	*cmd;
	int			argc;
	char		*argv[3];
	const char	*description;
}				TestCase;

void			test_builtin(const char *cmd, int argc, char *argv[], t_map *envmap);
t_map			*test_envmap_operations(void);
void			free_envmap(t_map *envmap);

/*
exitについては未実装部分があり、他の昨日実装後に大幅修正が必要
cdの仕様は相談が必要
*/
TestCase		test_cases[] = {
	{"cd", 1, {"cd", NULL}, "Change to HOME directory when no arguments are provided"},
	// {"pwd", 1, {"pwd", NULL}, "pwd after cd with no arguments(expect HOME)"},
	// {"cd", 2, {"cd", "Downloads", NULL}, "cd Downloads"},
	// {"pwd", 1, {"pwd", NULL}, "pwd after cd Downloads"},
	// {"cd", 2, {"cd", "../../", NULL}, "cd ../../ expect home directory"},
	// {"pwd", 1, {"pwd", NULL}, "pwd after cd ../../"},
    // {"cd", 2, {"cd", "/tmp", NULL}, "Change to the specified directory /tmp"},
	// {"pwd", 1, {"pwd", NULL}, "pwd after cd /tmp"},
	// {"cd", 2, {"cd", "..", NULL}, "Change to parent directory (cd ..), expect root directory"},
	// {"pwd", 1, {"pwd", NULL}, "pwd after cd .."},
    // {"cd", 2, {"cd", ".", NULL}, "Stay in the current directory (cd .), expect /tmp"},
	// {"pwd", 1, {"pwd", NULL}, "pwd after cd ."},
	// {"cd", 2, {"cd", "../bin", NULL}, "cd ../bin"},
	// {"pwd", 1, {"pwd", NULL}, "pwd after cd ../bin"},
    // {"cd", 2, {"cd", "/nonexistentdir", NULL}, "Attempt to change to non-existent directory"},
	// {"cd", 2, {"cd", "../tmp", NULL}, "cd ../tmp"},
	// {"pwd", 1, {"pwd", NULL}, "pwd after cd ../tmp"},
    // {"cd", 2, {"cd", "extra_arg", NULL}, "Too many arguments for cd"},
    // {"cd", 2, {"cd", "/nonexistentdir", NULL}, "Attempt to change to a non-existent directory"},
    // {"cd", 2, {"cd", "extra_arg", NULL}, "Too many arguments for cd"},
	// {"pwd", 1, {"pwd", NULL}, "Check current directory after failed cd(expect /tmp)"},
	// {"pwd", 1, {"pwd", NULL}, "pwd (expect home directory)"},
	// {"cd", 2, {"cd", "/tmp", NULL}, "cd to /tmp"},
	// {"pwd", 1, {"pwd", NULL}, "pwd (expect /tmp)"},
	// {"cd", 2, {"cd", "/non-existent-dir", NULL},"Attempt to change to a non-existent directory"},
	// {"pwd", 1, {"pwd", NULL}, "Check current directory after failed cd"},
	// {"cd", 2, {"cd", "..", NULL}, "Move to the parent directory of /tmp"},
	// {"pwd", 1, {"pwd", NULL}, "cd after moving up a level (expect /)"},
	// {"echo", 1, {"echo", NULL}, "Echo with no arguments (expect newline only)"},
    // {"echo", 2, {"echo", "-n", NULL}, "Echo with -n only (no newline)"},
	// {"echo", 2, {"echo", "Hello, world!", NULL}, "Echo with single argument"},
    // {"echo", 3, {"echo", "-n", "No newline"}, "Echo with -n (no newline)"},
};

int	main(void)
{
	t_map	*envmap;

	printf("Testing envmap operations...\n");
	envmap = test_envmap_operations();
	printf("builtin test...\n");
	for (size_t i = 0; i < sizeof(test_cases) / sizeof(TestCase); i++)
	{
		printf("Case %zu: %s\n", i + 1, test_cases[i].description);
		test_builtin(test_cases[i].cmd, test_cases[i].argc, test_cases[i].argv, envmap);
	}
	printf("builtins test done.\n");
	free_envmap(envmap);
	exit(0);
}

void	test_builtin(const char *cmd, int argc, char *argv[], t_map *envmap)
{
	t_builtin	*builtin;

	builtin = lookup_builtin((char *)cmd);
	if (builtin)
	{
		int status = builtin->f(argc, argv, envmap);
		if (status != 0)
			printf("%s execution failed with status %d.\n", cmd, status);
	}
	else
	{
		printf("%s is not a builtin command.\n", cmd);
	}
}

t_map *test_envmap_operations(void)
{
	char		*envp[] = {"HOME=/home/ssoeno", "PATH=/usr/bin", "SHELL=/bin/bash", NULL};
	t_map		*envmap = init_env(envp);

	if (!envmap)
	{
		printf("Failed to initialize environment map.\n");
		return (NULL);
	}

	printf("Environment map initialized.\n");
	const char *initial_vars[] = {"PATH", "HOME", "SHELL"};
	for (size_t i = 0; i < sizeof(initial_vars) / sizeof(initial_vars[0]); i++)
	{
		const char *value = map_get(envmap, initial_vars[i]);
		if (value)
			printf("%s=%s\n", initial_vars[i], value);
		else
			printf("Failed to get %s from envmap.\n", initial_vars[i]);
	}

	printf("Testing env with no options\n");
	TestCase env_case = {"env", 1, {"env", NULL}, "Print all environment variables"};
	test_builtin(env_case.cmd, env_case.argc, env_case.argv, envmap);
	
	printf("Testing export (add NEW_VAR=new_value)\n");
	TestCase export_case1 = {"export", 2, {"export", "NEW_VAR=new_value", NULL}, "Export NEW_VAR with value"};
	test_builtin(export_case1.cmd, export_case1.argc, export_case1.argv, envmap);
	
	printf("Checking if NEW_VAR is added...\n");
	const char *new_var_value = map_get(envmap, "NEW_VAR");
	if (new_var_value) {
		printf("NEW_VAR=%s\n", new_var_value);
	} else {
		printf("Failed to get NEW_VAR from envmap.\n");
	}
	
	printf("Testing export (update NEW_VAR=updated_value)\n");
	TestCase export_case2 = {"export", 2, {"export", "NEW_VAR=updated_value", NULL}, "Update NEW_VAR"};
    test_builtin(export_case2.cmd, export_case2.argc, export_case2.argv, envmap);

	printf("Checking if NEW_VAR is updated...\n");
    new_var_value = map_get(envmap, "NEW_VAR");
    if (new_var_value) {
		printf("NEW_VAR=%s\n", new_var_value);
	} else {
		printf("Failed to get NEW_VAR from envmap.\n");
	}
	printf("Testing unset (unset NEW_VAR)...\n");
	TestCase unset_case = {"unset", 2, {"unset", "NEW_VAR", NULL}, "Unset NEW_VAR"};
	test_builtin(unset_case.cmd, unset_case.argc, unset_case.argv, envmap);
	printf("Checking if NEW_VAR is unset...\n");
	new_var_value = map_get(envmap, "NEW_VAR");
	if (new_var_value == NULL) {
		printf("NEW_VAR was successfully unset.\n");
	} else {
		printf("Failed to unset NEW_VAR, still exists with value: %s\n", new_var_value);
	}
	printf("Testing env with no options...\n");
	test_builtin(env_case.cmd, env_case.argc, env_case.argv, envmap);
	return (envmap);
}

void free_envmap(t_map *map)
{
	t_item	*cur;
	t_item	*next;

	cur = map->item_head.next;
	while (cur)
	{
		next = cur->next;
		free(cur->name);
		free(cur->value);
		free(cur);
		cur = next;
	}
	free(map);
}
