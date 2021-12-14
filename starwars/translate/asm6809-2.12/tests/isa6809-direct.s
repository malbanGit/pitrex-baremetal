; test direct instructions

	neg	<$00
	com	<$03
	lsr	<$04
	ror	<$06
	asr	<$07
	asl	<$08
	lsl	<$08	; test both forms
	rol	<$09
	dec	<$0a
	inc	<$0c
	tst	<$0d
	jmp	<$0e
	clr	<$0f

	suba	<$90
	cmpa	<$91
	sbca	<$92
	subd	<$93
	anda	<$94
	bita	<$95
	lda	<$96
	sta	<$97
	eora	<$98
	adca	<$99
	ora	<$9a
	adda	<$9b
	cmpx	<$9c
	jsr	<$9d
	ldx	<$9e
	stx	<$9f

	subb	<$d0
	cmpb	<$d1
	sbcb	<$d2
	addd	<$d3
	andb	<$d4
	bitb	<$d5
	ldb	<$d6
	stb	<$d7
	eorb	<$d8
	adcb	<$d9
	orb	<$da
	addb	<$db
	ldd	<$dc
	std	<$dd
	ldu	<$de
	stu	<$df

	cmpd	<$93
	cmpy	<$9c
	ldy	<$9e
	sty	<$9f

	lds	<$de
	sts	<$df

	cmpu	<$93
	cmps	<$9c
