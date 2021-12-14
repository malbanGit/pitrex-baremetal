#!/bin/sh

fail=0
tests="pseudo-cond pseudo-org-put-setdp pseudo-section"

for t in ${tests}; do
	../src/asm6809${EXEEXT} -S -l ${t}.lis -o ${t}.out ${t}.s
	cmp ${t}.out ${t}.cmp || fail=1
done

exit $fail
