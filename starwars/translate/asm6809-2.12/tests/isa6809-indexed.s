; test direct instructions

	leax	-16,y
	leay	-15,y
	leas	-14,y
	leau	-13,y

	neg	,x
	com	0,x
	lsr	0,x
	ror	<0,x
	asr	>0,x
	asl	-16,x
	lsl	-17,x	; test both forms
	rol	15,x
	dec	16,x
	inc	-128,x
	tst	-129,x
	jmp	127,x
	clr	128,x

	suba	-32768,x
	cmpa	-32769,x
	sbca	32767,x
	subd	32768,x
	anda	a,x
	bita	b,x
	lda	d,x
	sta	,x+
	eora	,x++
	adca	,-x
	ora	,--x
	adda	,y
	cmpx	15,y
	jsr	-128,y
	ldx	>64,y
	stx	a,y

	subb	b,y
	cmpb	d,y
	sbcb	,y+
	addd	,y++
	andb	,-y
	bitb	,--y
	ldb	,u
	stb	-16,u
	eorb	127,u
	adcb	-32768,u
	orb	a,u
	addb	b,u
	ldd	d,u
	std	,u+
	ldu	,u++
	stu	,-u

	; repeat for more tests
	subb	,--u
	cmpb	,s
	sbcb	15,s
	addd	-128,s
	andb	32767,s
	bitb	a,s
	ldb	b,s
	stb	d,s
	eorb	,s+
	adcb	,s++
	orb	,-s
	addb	,--s
	ldd	[,x]
	std	[-16,y]
	ldu	[127,u]
	stu	[-32768,s]

	; bit more repetition...
	bitb	[a,s]
	ldb	[b,s]
	stb	[d,s]
	adcb	[,s++]
	addb	[,--s]

	cmpd	,--u
	cmpy	-127,pc
	ldy	128,pc
	sty	*-127,pcr

	lds	-129,pc
	sts	128,pc

	cmpu	$f0d9,pcr
	cmps	[$f00d]
