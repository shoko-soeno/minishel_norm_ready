#!/bin/bash

cleanup() {
	rm -f cmp out result
}

error_echo() 
{
	echo -e -n "\e[1;31m$1\e[0m"
}

error_echo_w_linebreak() 
{
	echo -e "\e[1;31m$1\e[0m"
}

assert() {
	printf '%-30s:' "\"$1\""
	# exit status
	echo -n -e "$1" | bash >cmp 2>&-
	expected=$?
	echo -n -e "$1" | ./minishell >out 2>&-
	actual=$?

	diff cmp out >/dev/null && echo -n '  diff OK' | tee -a result || error_echo '  diff NG' | tee -a result

	if [ "$actual" = "$expected" ]; then
		echo '  status OK' | tee -a result
	else
		error_echo_w_linebreak "  status NG, expected $expected but got $actual" | tee -a result
	fi
	echo
}
