; test indexed 6309 instructions

	adcd	,x
	adde	15,x
	addf	-128,x
	addw	32767,x
	aim	#$02,a,x
	andd	b,x

	bitd	d,x

	cmpe	,x+
	cmpf	,x++
	cmpw	,-x

	divd	,--x
	divq	[,x]

	eim	#$05,[-16,x]
	eord	[127,x]

	lde	[-32768,x]
	ldf	[a,x]
	ldq	[b,x]
	ldw	[d,x]

	muld	[,x++]

	oim	#$01,[,--x]
	ord	-127,pc

	sbcd	128,pc
	ste	*-127,pcr
	stf	-129,pc
	stq	128,pc
	stw	$f06d,pcr
	sube	[$f00d]
	subf	,y
	subw	15,y

	tim	#$0b,-128,y
