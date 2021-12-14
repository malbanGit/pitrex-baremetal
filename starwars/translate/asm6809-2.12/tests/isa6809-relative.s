; test relative instructions

	lbra	l1
	lbsr	*-6	; wrap around

l0

	bra	*-8	; wrap around
	brn	l0
	bhi	l0
	bls	l0
	bhs	l0
	bcc	l0	; test both forms
	blo	l0
	bcs	l0	; test both forms
	bne	l0
	beq	l0
	bvc	l1
	bvs	l1
	bpl	l1
	bmi	l1
	bge	l1
	blt	l1
	bgt	l1
	ble	l1

l1

	bsr	l0
	bsr	*+$81

l2

	lbrn	l2
	lbhi	l2
	lbls	l2
	lbhs	l2
	lbcc	l2	; test both forms
	lblo	l2
	lbcs	l2	; test both forms
	lbne	l2
	lbeq	l2
	lbvc	l3
	lbvs	l3
	lbpl	l3
	lbmi	l3
	lbge	l3
	lblt	l3
	lbgt	l3
	lble	l3

l3
