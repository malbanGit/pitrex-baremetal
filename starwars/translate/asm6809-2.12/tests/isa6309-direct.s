; test direct 6309 instructions

	adcd	<$99
	adde	<$9b
	addf	<$db
	addw	<$9b
	aim	#$02,<$02
	andd	<$94
	aim	#1,<$12
	aim	#-2,$12

	band	cc,6,0,<$30
	biand	cc,6,1,<$31
	beor	cc,6,4,<$34
	bieor	cc,6,5,<$35
	bitd	<$95
	bor	cc,6,2,<$32
	bior	cc,6,3,<$33

	cmpe	<$91
	cmpf	<$d1
	cmpw	<$91

	divd	<$9d
	divq	<$9e

	eim	#$05,<$05
	eord	<$98

	lde	<$96
	ldf	<$d6
	ldq	<$dc
	ldw	<$96
	ldbt	cc,6,6,<$36

	muld	<$9f

	oim	#$01,<$01
	aim	#1,$12
	aim	#-2,<$12
	ord	<$9a

	sbcd	<$92
	ste	<$97
	stf	<$d7
	stq	<$dd
	stw	<$97
	stbt	cc,6,7,<$37
	sube	<$90
	subf	<$d0
	subw	<$90

	tim	#$0b,<$0b
