#!/bin/bash

cleanup() {
	rm -f cmp out
}

assert() {
	printf '%-30s:' "\"$1\""
	# exit status
	echo -n -e "$1" | bash >cmp 2>&-
	expected=$?
	echo -n -e "$1" | ./minishell >out 2>&-
	actual=$?

	diff cmp out >/dev/null && echo -n '  diff OK' || echo -n '  diff NG'

	if [ "$actual" = "$expected" ]; then
		echo -n '  status OK'
	else
		echo -n "  status NG, expected $expected but got $actual"
	fi
	echo
}

# Empty line (EOF)
assert ''

# buitins
assert "env"
assert "export"
assert "export NEW_VAR"
assert "export NEW_VAR=42"
assert "unset"
assert "unset NEW_VAR"
assert "unset NON_EXISTING_VAR"
assert "echo"
assert "echo hello"
assert "echo -n hello"
assert "echo -n -n hello"
assert "echo -nnn hello"
assert "echo -nabc hello"
assert "echo -nnnn -nn -nnnn abc"
assert "pwd"
# assert "cd"
# assert "pwd"
# assert 'exit' 0
# assert 'exit 42' 42
# assert 'exit ""' 0
# assert 'exit hello' 0  # This might differ depending on how invalid arguments are handled
# assert 'exit 42Tokyo' 0  # This might differ depending on handling of non-integer args
# assert 'exit 1 2' 1

cleanup