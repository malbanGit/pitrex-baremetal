#!/bin/sh

fail=0
tests="isa6809-direct isa6809-extended isa6809-immediate isa6809-indexed isa6809-inherent isa6809-relative"

for t in ${tests}; do
	../src/asm6809${EXEEXT} -S -l ${t}.lis -o ${t}.out ${t}.s
	cmp ${t}.out ${t}.cmp || fail=1
done

exit $fail
