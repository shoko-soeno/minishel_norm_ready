#!/bin/bash

source ./combination_test/test_assert.sh

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

assert "cd"
assert "unset HOME"
assert "cd"
assert "pwd"
assert "cd /"
assert "pwd"
assert "cd ."
assert "pwd"
assert "cd .."
assert "pwd"
assert "cd -"
assert "pwd"
assert "cd /home/user/bin"
assert "pwd"
assert "cd /home/user/bin/.."
assert "pwd"
assert "cd ./bin"
assert "pwd"
assert "cd ./bin/../usr"
assert "pwd"
assert "cd /non-existing-dir"
assert "pwd"
assert "cd invalid!path"
assert "pwd"

assert 'exit'
assert 'exit 42'
assert 'exit ""'
assert 'exit hello'
assert 'exit 42Tokyo'
assert 'exit 1 2'
assert 'exit  +1'
assert 'exit  ++1'
assert 'exit -1'
assert 'exit --1'
assert 'exit 99999999999999999999'
assert 'exit 2147483648'
assert 'exit -9223372036854775807'
assert 'exit -9223372036854775808'
assert 'exit -9223372036854775809'
assert 'exit 9223372036854775807'
assert 'exit 9223372036854775808'
