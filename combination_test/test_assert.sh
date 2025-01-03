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

	diff cmp out >/dev/null && echo -n '  diff OK' | tee -a result || echo -n '  diff NG' | tee -a result

	if [ "$actual" = "$expected" ]; then
		echo '  status OK' | tee -a result
	else
		echo "  status NG, expected $expected but got $actual" | tee -a result
	fi
	echo
}
