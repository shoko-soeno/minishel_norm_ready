#!/bin/bash

# Function to run and capture the output and exit code of the `builtin_exit` command
run_builtin_exit() {
  ./minishell "$@"
  echo $?  # Output the exit code
}

# Assert function to compare the expected output with actual output
assert() {
  local input="$1"
  local expected_output="$2"
  local actual_output
  local command_output

  # Run the command and capture its output and exit code
  command_output=$(run_builtin_exit $input 2>&1)
  actual_output=$?

  # Check if the actual output matches the expected output
  if [ "$actual_output" -eq "$expected_output" ]; then
    echo "PASS: exit $input"
  else
    echo "FAIL: exit $input (expected: $expected_output, got: $actual_output)"
    echo "Output: $command_output"
  fi
}

# Tests
assert 'exit' 0
assert 'exit 42' 42
assert 'exit ""' 0
assert 'exit hello' 0  # This might differ depending on how invalid arguments are handled
assert 'exit 42Tokyo' 0  # This might differ depending on handling of non-integer args
assert 'exit 1 2' 1

