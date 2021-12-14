	org $4000
	put $2000

start

	setdp $20
	lda data

	setdp $40
	lda data

data	rmb 1

	end start
