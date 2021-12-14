; test immediate instructions

	orcc	#$1a
	andcc	#$1c
	exg	d,x
	exg	y,u
	exg	s,pc
	exg	a,b
	exg	cc,dp
	tfr	dp,cc
	tfr	b,a
	tfr	pc,s
	tfr	u,y
	tfr	x,d

	pshs
	puls
	pshu
	pulu
	pshs	b,x,y
	pulu	cc,d,x,y
	pshs	cc
	pshs	cc,a,y
	puls	cc,b,u
	puls	cc,d,y,u
	pshu	cc,dp,pc
	pshu	cc,a,dp,y,pc
	pulu	cc,b,dp,s,pc
	pulu	cc,a,b,dp,y,s,pc

	cwai	#$3c
