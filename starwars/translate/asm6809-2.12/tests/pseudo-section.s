; Ensure sections follow each other properly

	org $4000
	section "a"
	lda #1
	section "b"
	lda #3
	section "c"
	org $4006
	lda #4
	section "a"
	lda #2
