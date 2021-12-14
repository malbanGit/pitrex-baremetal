; Contrived example involving nested ifs with multiple elsifs.

fcbbit	macro
	if *&4
		if *&2
			if *&1
				fcb 0
			else
				fcb 1
			endif
		else
			if *&1
				fcb 2
			else
				fcb 3
			endif
		endif
	elsif *&2
		if *&1
			fcb 4
		else
			fcb 5
		endif
	elsif *&1
		fcb 6
	else
		fcb 7
	endif
	endm

	fcbbit
	fcbbit
	fcbbit
	fcbbit
	fcbbit
	fcbbit
	fcbbit
	fcbbit
