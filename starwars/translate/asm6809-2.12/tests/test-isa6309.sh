#!/bin/sh

fail=0
tests="isa6309-immediate isa6309-direct isa6309-indexed isa6309-extended isa6309-inherent"

for t in ${tests}; do
	../src/asm6809${EXEEXT} -3 -S -l ${t}.lis -o ${t}.out ${t}.s
	cmp ${t}.out ${t}.cmp || fail=1
done

exit $fail
