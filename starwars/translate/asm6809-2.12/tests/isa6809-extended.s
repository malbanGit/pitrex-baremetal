; test extended instructions

	neg	>$7070
	com	>$7373
	lsr	>$7474
	ror	>$7676
	asr	>$7777
	asl	>$7878
	lsl	>$7878	; test both forms
	rol	>$7979
	dec	>$7a7a
	inc	>$7c7c
	tst	>$7d7d
	jmp	>$7e7e
	clr	>$7f7f

	suba	>$b0b0
	cmpa	>$b1b1
	sbca	>$b2b2
	subd	>$b3b3
	anda	>$b4b4
	bita	>$b5b5
	lda	>$b6b6
	sta	>$b7b7
	eora	>$b8b8
	adca	>$b9b9
	ora	>$baba
	adda	>$bbbb
	cmpx	>$bcbc
	jsr	>$bdbd
	ldx	>$bebe
	stx	>$bfbf

	subb	>$f0f0
	cmpb	>$f1f1
	sbcb	>$f2f2
	addd	>$f3f3
	andb	>$f4f4
	bitb	>$f5f5
	ldb	>$f6f6
	stb	>$f7f7
	eorb	>$f8f8
	adcb	>$f9f9
	orb	>$fafa
	addb	>$fbfb
	ldd	>$fcfc
	std	>$fdfd
	ldu	>$fefe
	stu	>$ffff

	cmpd	>$b3b3
	cmpy	>$bcbc
	ldy	>$bebe
	sty	>$bfbf

	lds	>$fefe
	sts	>$ffff

	cmpu	>$b3b3
	cmps	>$bcbc
