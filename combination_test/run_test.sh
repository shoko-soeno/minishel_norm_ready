#!/bin/bash

test_result() {
	echo "=== Test Summary ==="
	passed=$(grep -c "diff OK.*status OK" result)
	total=$(wc -l < result)
	failed=$((total - passed))
	echo "Tests completed: $total"
	echo "Tests passed: $passed"
	echo "Tests failed: $failed"
	true > result
}

make
if [ $? -ne 0 ]; then
	echo "make is failed"
	exit 1
else 
	source ./combination_test/test_assert.sh || { echo "Failed to source test_assert.sh"; exit 1; }
	echo "expand test"
	./combination_test/expand_test.sh
	test_result
	echo "builtin test"
	./combination_test/builtin_test.sh
	echo "test is completed"
	test_result
fi

trap 'cleanup || { echo "Error: cleanup failed"; exit 1; }' EXIT