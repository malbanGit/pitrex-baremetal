; Tempest Source Code
; -------------------
; The actual code is Copyright 1981 by Atari.
; Documentation is Copyright 1999 by Arcade Gameshop Corporation.
; All rights reserved. Additional rights granted through OpenContent
; License. See http://arcade.gameshop.com/opl.html
;
; No warranty or fitness for particular purpose expressed or implied.
;
; Updated 09/17/2004 by Josh McCormick
;
; Contributors:
; -------------
; Josh McCormick - Project lead and main documentarian.
;                  (http://www.galstar.com/~jmccorm)
; Clay Cowgill   - EAROM programming example, Vector ROM image list,
;                  MAME programming tips (http://www.multigame.com)
; Ken Lui        - Documentation of data segments on V1 roms.
;                  Provided a much better assembler/disassembler package.
;                  (http://www.ecst.csuchico.edu/~tempest)
; 
;
; Power-on code begins at $D93F. (Obtained from vector at $DFFC.)
; Maskable and non-maskable interupt routine at $D704. (From vector $DFFE.)
; Assembler dump begins at ROM 136002.113 at $9000.
;
; NOTE: ROM locations $C000-DFFF are duplicated via hardware to $E000-FFFF.
; NOTE: Currently not formatted for re-assembly. (Good point, Ken Lui.)
; NOTE: Yes, the format SUCKS. Ken Lui has provided a better disassembler
;       and matching assembler. We *will* migrate to this.
;       Tenative migration volunteer: Joey Oravec.

9000 02:BB      DATA			 ; Intuitive guess: CRC data?
9002 5A:30      DATA			 ; The self-test will give the
9004 50:EE      DATA			 ; data that should be here if
9006 3D:A8      DATA			 ; you modify just one byte. Not
9008 4D         DATA			 ; true for other ROM locations.
;
; Unknown subroutine.
;
9009 20:C5:92   JSR:abs    $92C5
900C 20:34 92   JSR:abs    $9234	 ; Set number of enemies to appear
					 ; and size of the spikes to begin
					 ; with.
900F 20:2B 90   JSR:abs    $902B
9012 20:31 A8   JSR:abs    $A831         ; Clear out the superzapper status.
9015 A9:FA      LDA:imm    #FA
9017 85:5B      STA:zp     Zp RAM 005B
9019 A9:00      LDA:imm    #00
901B 8D:06 01   STA:abs    $0106
901E 85:5F      STA:zp     Zp RAM 005F
9020 A9:00      LDA:imm    #00		 ; Set status to gameplay mode
9022 85:01      STA:zp     Zp RAM 0001   ; (or attract mode).
9024 60:        RTS:imp    Ret from Sub
;
; SUBROUTINE: Game initialization.
;
9025 20:1B 92   JSR:abs    $921B	 ; Set the player's position.
9028 20:C5 92   JSR:abs    $92C5
902B 20:8F 92   JSR:abs    $928F	 ; Remove all bullets from play.
902E 20:6F 92   JSR:abs    $926F	 ; Remove all enemies from tube.
9031 20:46 92   JSR:abs    $9246
9034 20:9F 92   JSR:abs    $929F	 ; Clear out any enemy deaths.
9037 20:AD 92   JSR:abs    $92AD	 ; Zero out the spinner.
903A 20:6E C1   JSR:abs    $C16E
903D A9:FF      LDA:imm    #FF
903F 8D:24 01   STA:abs    $0124
9042 8D:48 01   STA:abs    $0148
9045 A9:00      LDA:imm    #00
9047 8D:23 01   STA:abs    $0123
904A 60:        RTS:imp    Ret from Sub
;
904B A9:10      LDA:imm    #10		 ; Put the player at the top
904D 8D:02 02   STA:abs    $0202	 ; of the tunnel.
9050 A9:00      LDA:imm    #00
9052 85:29      STA:zp     Zp RAM 0029
9054 85:2B      STA:zp     Zp RAM 002B
9056 AD:21 01   LDA:abs    $0121
9059 85:2A      STA:zp     Zp RAM 002A
905B 10:02      BPL:rel    Branch->$905F
905D C6:2B      DEC:zp     Zp RAM 002B
905F A2:01      LDX:imm    #01
9061 A5:2A      LDA:zp     Zp RAM 002A
9063 0A:        ASL:accum  
9064 66:2A      ROR:zp     Zp RAM 002A
9066 66:29      ROR:zp     Zp RAM 0029
9068 CA:        DEX:imp    X=X-1
9069 10:F6      BPL:rel    Branch->$9061
906B A5:29      LDA:zp     Zp RAM 0029
906D 18:        CLC:imp    Clear Carry
906E 6D:22 01   ADC:abs    $0122
9071 8D:22 01   STA:abs    $0122
9074 A5:2A      LDA:zp     Zp RAM 002A
9076 65:68      ADC:zp     Zp RAM 0068
9078 85:68      STA:zp     Zp RAM 0068
907A A5:2B      LDA:zp     Zp RAM 002B
907C 65:69      ADC:zp     Zp RAM 0069
907E 85:69      STA:zp     Zp RAM 0069
9080 A5:5F      LDA:zp     Zp RAM 005F
9082 18:        CLC:imp    Clear Carry
9083 69:18      ADC:imm    #18
9085 85:5F      STA:zp     Zp RAM 005F
9087 A5:5B      LDA:zp     Zp RAM 005B
9089 69:00      ADC:imm    #00
908B 85:5B      STA:zp     Zp RAM 005B
908D C9:FC      CMP:imm    #FC
908F 90:05      BCC:rel    Branch->$9096
9091 A9:01      LDA:imm    #01
9093 8D:15 01   STA:abs    $0115
9096 A5:5F      LDA:zp     Zp RAM 005F
9098 38:        SEC:imp    Set Carry
9099 E5:5D      SBC:zp     Zp RAM 005D
909B A5:5B      LDA:zp     Zp RAM 005B
909D F0:02      BEQ:rel    Branch->$90A1
909F E9:FF      SBC:imm    #FF
90A1 D0:19      BNE:rel    Branch->$90BC
90A3 A5:5D      LDA:zp     Zp RAM 005D
90A5 85:5F      STA:zp     Zp RAM 005F
90A7 A9:FF      LDA:imm    #FF
90A9 85:5B      STA:zp     Zp RAM 005B
90AB A9:04      LDA:imm    #04
90AD 24:05      BIT:zp     Zp RAM 0005
90AF 30:02      BMI:rel    Branch->$90B3
90B1 A9:08      LDA:imm    #08		 ; Set game mode to test screen:
90B3 85:00      STA:zp     Zp RAM 0000	 ; color bars test. HUH?!!?
90B5 A6:3D      LDX:zp     Zp RAM 003D	 ;
90B7 A9:00      LDA:imm    #00		 ; Clear out the level selected
90B9 9D:02 01   STA:abs,x  $0102,X	 ; by the user to start on.
90BC A9:FF      LDA:imm    #FF
90BE 8D:14 01   STA:abs    $0114
90C1 4C:49 97   JMP:abs    $9749

; LEVEL SELECTION CODE BEGINS HERE
90C4 AD:26 01   LDA:abs    $0126	 ; Find out what level the player is
90C7 A2:1C      LDX:imm    #1C		 ; permitted to restart on. Look it
90C9 CA:        DEX:imp    X=X-1	 ; up in a table to see how many
90CA DD:FE 91   CMP:abs,x  $91FE,X	 ; levels they have to choose from.
90CD 90:FA      BCC:rel    Branch->$90C9 ; X = # of levels to present
90CF A0:04      LDY:imm    #04
90D1 AD:6A 01   LDA:abs    $016A
90D4 29:04      AND:imm    #04
90D6 F0:12      BEQ:rel    Branch->$90EA
90D8 AD:1D 07   LDA:abs    $071D
90DB C9:30      CMP:imm    #30		 ; Highest score > 300,000?
90DD 90:01      BCC:rel    Branch->$90E0
90DF C8:        INY:imp    Y=Y+1
90E0 C9:50      CMP:imm    #50		 ; Highest score > 500,000?
90E2 90:01      BCC:rel    Branch->$90E5
90E4 C8:        INY:imp    Y=Y+1
90E5 C9:70      CMP:imm    #70		 ; Highest score > 700,000?
90E7 90:01      BCC:rel    Branch->$90EA
90E9 C8:        INY:imp    Y=Y+1
90EA A5:09      LDA:zp     Zp RAM 0009
90EC 29:43      AND:imm    #43
90EE C9:40      CMP:imm    #40
90F0 D0:02      BNE:rel    Branch->$90F4
90F2 A0:1B      LDY:imm    #1B		 ; Limit the number of
90F4 84:29      STY:zp     Zp RAM 0029	 ; menu items to #1B?
90F6 E4:29      CPX:zp     Zp RAM 0029
90F8 B0:02      BCS:rel    Branch->$90FC
90FA A6:29      LDX:zp     Zp RAM 0029
90FC 8E:27 01   STX:abs    $0127
90FF A5:05      LDA:zp     Zp RAM 0005
9101 10:05      BPL:rel    Branch->$9108
9103 A9:00      LDA:imm    #00
9105 8D:26 01   STA:abs    $0126
9108 A6:3F      LDX:zp     Zp RAM 003F
910A 86:3D      STX:zp     Zp RAM 003D
910C F0:03      BEQ:rel    Branch->$9111
910E 20:B2 92   JSR:abs    $92B2	 ; Swap some game data between the
					 ; active and inactive players.
9111 A9:04      LDA:imm    #04
9113 85:7C      STA:zp     Zp RAM 007C
9115 A9:FF      LDA:imm    #FF
9117 85:5B      STA:zp     Zp RAM 005B
9119 A9:00      LDA:imm    #00
911B 8D:00 02   STA:abs    $0200         ; Start the player on segment 00.
911E 85:51      STA:zp     Zp RAM 0051   ; Start the player at position 00.
9120 85:7B      STA:zp     Zp RAM 007B
9122 8D:05 06   STA:abs    $0605	 ; Zero out the 1/20th sec counter.
9125 A6:05      LDX:zp     Zp RAM 0005
9127 10:1B      BPL:rel    Branch->$9144
9129 A9:14      LDA:imm    #14		 ; A regularly decremented counter,
912B 8D:05 06   STA:abs    $0605	 ; when it reachs 0, 1 second elapsed.
912E A9:FF      LDA:imm    #FF
9130 8D:11 01   STA:abs    $0111
9133 A9:16      LDA:imm    #16		 ; Set game mode to pre-game
9135 85:00      STA:zp     Zp RAM 0000	 ; level selection screen.
9137 A9:08      LDA:imm    #08           ; Set mode to pre-game level
9139 85:01      STA:zp     Zp RAM 0001	 ; selection.
913B A9:00      LDA:imm    #00		 ; Set current level to the
913D 85:9F      STA:zp     Zp RAM 009F   ; first screen.
913F 20:96 C1   JSR:abs    $C196
9142 A9:10      LDA:imm    #10		 ; Give the player ten seconds to
9144 85:04      STA:zp     Zp RAM 0004	 ; select their level.
9146 20:AD 92   JSR:abs    $92AD	 ; Zero out the spinner.
9149 CE:05 06   DEC:abs    $0605	 ; Has the 1 second timer reached
914C 10:1B      BPL:rel    Branch->$9169 ; zero? If not, branch $9169
914E F8:        SED:imp    Set Decimal	 ; Go into decimal mode to adjust
914F A5:04      LDA:zp     Zp RAM 0004	 ; the countdown from ten timer.
9151 38:        SEC:imp    Set Carry	 ; Reduce it by one. 
9152 E9:01      SBC:imm    #01		 ;
9154 85:04      STA:zp     Zp RAM 0004	 ; 
9156 D8:        CLD:imp    Unset Decimal ; Go to normal math mode.
9157 10:04      BPL:rel    Branch->$915D
9159 A9:10      LDA:imm    #10           ; Set the fire button as needing to
915B 85:4E      STA:zp     Zp RAM 004E   ; be processed. Or Test button?
915D C9:03      CMP:imm    #03
915F D0:03      BNE:rel    Branch->$9164
9161 20:FE CC   JSR:abs    $CCFE
9164 A9:14      LDA:imm    #14		 ; Reset the 1/20th second countdown
9166 8D:05 06   STA:abs    $0605	 ; timer to one second.
9169 20:AB B0   JSR:abs    $B0AB
916C A9:18      LDA:imm    #18
916E A4:04      LDY:zp     Zp RAM 0004
9170 C0:08      CPY:imm    #08
9172 B0:02      BCS:rel    Branch->$9176
9174 A9:78      LDA:imm    #78           ; Have any buttons been pressed?
9176 25:4E      AND:zp     Zp RAM 004E
9178 F0:34      BEQ:rel    Branch->$91AE : If not, skip ahead.

917A A9:00      LDA:imm    #00           ; Clear the list of buttons that
917C 85:4E      STA:zp     Zp RAM 004E   ; need to be processed.
917E AD:00 02   LDA:abs    $0200         ; Start the player on segment 00.
9181 A8:        TAY:imp    A-->Y
9182 A6:3D      LDX:zp     Zp RAM 003D	 ; Figure out which player is up.
9184 9D:02 01   STA:abs,x  $0102,X	 ; Store the menu selection
					 ; chosen by the user.
9187 B9:FE 91   LDA:abs,y  $91FE,Y
918A 24:05      BIT:zp     Zp RAM 0005
918C 30:09      BMI:rel    Branch->$9197 ; If this is real gameplay,
					 ; skip down a bit.

918E A0:01      LDY:imm    #01		 ; ATTRACT MODE GAMEPLAY SETUP
9190 84:48      STY:zp     Zp RAM 0048	 ; Set player one lives to 1.
9192 AD:CA 60   LDA:abs    $60CA	 ; Get a random number
9195 29:07      AND:imm    #07		 ; 0-7.

9197 95:46      STA:zp,x   Zp RAM 0046	 ; Store it as the current level
					 ; for player one.
9199 85:9F      STA:zp     Zp RAM 009F   ; Store it in the other current
					 ; level register (which is
					 ; level -1)
919B 20:96 C1   JSR:abs    $C196
919E 20:C5 92   JSR:abs    $92C5
91A1 20:34 92   JSR:abs    $9234	 ; Set the number of enemies to
					 ; appear and the size of the spikes
					 ; that start off in the tunnel.
91A4 20:31 A8   JSR:abs    $A831         ; Clear out the superzapper status.
91A7 A9:02      LDA:imm    #02
91A9 85:00      STA:zp     Zp RAM 0000
91AB 20:AD 92   JSR:abs    $92AD	 ; Zero out the spinner.
91AE A5:4E      LDA:zp     Zp RAM 004E
91B0 29:07      AND:imm    #07
91B2 85:4E      STA:zp     Zp RAM 004E
91B4 60:        RTS:imp    Ret from Sub
;
91B5 0A:        ASL:accum  
91B6 AA:        TAX:imp    A-->X
91B7 A9:00      LDA:imm    #00
91B9 85:29      STA:zp     Zp RAM 0029
91BB BD:C6 91   LDA:abs,x  $91C6,X
91BE 85:2A      STA:zp     Zp RAM 002A
91C0 BD:C7 91   LDA:abs,x  $91C7,X
91C3 85:2B      STA:zp     Zp RAM 002B
91C5 60:        RTS:imp    Ret from Sub
;
; Data segment used by above subroutine per Ken Lui.
;
91C6 00:00      DATA
91C8 60:00      DATA
91CA 60:        DATA
91CB 01:20      DATA
91CD 03:40      DATA
91CF 05:40      DATA
91D1 07:40      DATA
91D3 09:40      DATA
91D5 11:40      DATA
91D7 13:        DATA
91D8 20:15:00   DATA
91DB 17:        DATA
91DC 80:        DATA
91DD 18:        DATA
91DE 80:        DATA
91DF 20:60 22   DATA
91E2 80:        DATA
91E3 24:60      DATA
91E5 26:00      DATA
91E7 30:00      DATA
91E9 34:        DATA
91EA 20:38 50   DATA
91ED 41:90      DATA
91EF 43:        DATA
91F0 20:47 10   DATA
91F3 53:        DATA
91F4 10:58      DATA
91F6 40:        DATA
91F7 62:        DATA
91F8 60:        DATA
91F9 65:60      DATA
91FB 76:80      DATA
91FD 89:        DATA
;
91FE 00:        DATA                    ; Data used by $90C4 and $9187.
91FF 02:        DATA                    ; This table contains a list of
9200 04:        DATA			; the valid levels that a user
9201 06:08      DATA			; is able to start on. The
9203 0A:        DATA			; highest starting level is
9204 0C:        DATA			; decimal 81 (#50 + 1).
9205 0E:10 13   DATA
9208 15:17      DATA
920A 19:1B 1E   DATA
920D 20:23 27   DATA
9210 2B:        DATA
9211 2E:30 33   DATA
9214 37:        DATA
9215 3B:        DATA
9216 3E:40 48   DATA
9219 50:FF      DATA
;
; Subroutine: Set the player's position
;
921B A9:0E      LDA:imm    #0E		 ; Put the player on segment 0F
921D 8D:00 02   STA:abs    $0200         ; (E+1) of the tunnel.
9220 A9:F0      LDA:imm    #F0		 ; 
9222 85:51      STA:zp     Zp RAM 0051   ; Put the player at position F0.
9224 A9:00      LDA:imm    #00
9226 8D:06 01   STA:abs    $0106
9229 A9:0F      LDA:imm    #0F		 ; Put the player on segment F
922B 8D:01 02   STA:abs    $0201	 ; of the tunnel.
922E A9:10      LDA:imm    #10		 ; Put the player at the top of
9230 8D:02 02   STA:abs    $0202	 ; the tunnel.
9233 60:        RTS:imp    Ret from Sub
;
; Subroutine: Upon new tunnel, set # of enemies and size of spikes.
; Only used for *certain* levels.
;
9234 AD:5B 01   LDA:abs    $015B	 ; Find out how many enemies to
9237 8D:AB 03   STA:abs    $03AB	 ; create for this level and set it.
923A AD:5A 01   LDA:abs    $015A	 ; Find out how tall the spikes
923D A2:0F      LDX:imm    #0F		 ; should be to start with and
923F 9D:AC 03   STA:abs,x  $03AC,X	 ; create spikes in all the tunnel
9242 CA:        DEX:imp    X=X-1	 ; segments to be that large.
9243 10:FA      BPL:rel    Branch->$923F ;
9245 60:        RTS:imp    Ret from Sub	 ;
;
9246 A9:00      LDA:imm    #00		 ; Clear out the $40 timers used
9248 A2:3F      LDX:imm    #3F		 ; to say when an enemy can be put
924A 9D:43 02   STA:abs,x  $0243,X	 ; onto the tube.
924D CA:        DEX:imp    X=X-1	 ; "Appearance Timer"
924E 10:FA      BPL:rel    Branch->$924A ;
9250 AE:AB 03   LDX:abs    $03AB	 ; Get the number of enemies to
					 ; make an appearance in this tube.
9253 CA:        DEX:imp    X=X-1	 ; 
9254 AD:CA 60   LDA:abs    $60CA	 ; Get a random number.
9257 29:0F      AND:imm    #0F		 ; 
9259 9D:03 02   STA:abs,x  $0203,X	 ; Basic idea:
925C 8A:        TXA:imp    X-->A	 ; Randomize the movement styles
925D 0A:        ASL:accum  		 ; for the enemies that are going
925E 0A:        ASL:accum  		 ; to be placed in the tube.
925F 0A:        ASL:accum  		 ;
9260 0A:        ASL:accum  		 ;
9261 1D:03 02   ORA:abs,x  $0203,X	 ;
9264 D0:02      BNE:rel    Branch->$9268 ;
9266 A9:0F      LDA:imm    #0F		 ;
9268 9D:43 02   STA:abs,x  $0243,X	 ;
926B CA:        DEX:imp    X=X-1	 ;
926C 10:E6      BPL:rel    Branch->$9254 ;
926E 60:        RTS:imp    Ret from Sub	 ;
;
; Subroutine to remove all enemies from the tube. 
;
926F A2:06      LDX:imm    #06		 ; Clear out the table that holds
9271 A9:00      LDA:imm    #00		 ; the height of the enemies inside
9273 9D:DF 02   STA:abs,x  $02DF,X	 ; the tunnel
9276 CA:        DEX:imp    X=X-1	 ;
9277 10:FA      BPL:rel    Branch->$9273 ;
9279 8D:08 01   STA:abs    $0108	 ; No enemies inside the tube or
927C 8D:09 01   STA:abs    $0109	 ; at the top of the tube.
927F 8D:45 01   STA:abs    $0145	 ; Clear out the addresses which
9282 8D:42 01   STA:abs    $0142	 ; contain how many of each type
9285 8D:44 01   STA:abs    $0144	 ; of enemy are currently in
9288 8D:43 01   STA:abs    $0143	 ; the tubes.
928B 8D:46 01   STA:abs    $0146	 ;
928E 60:        RTS:imp    Ret from Sub
;
; Subroutine to remove all bullets from play.
;
928F A9:00      LDA:imm    #00
9291 A2:0B      LDX:imm    #0B
9293 9D:D3 02   STA:abs,x  $02D3,X 	 ; Get rid of all the bullets,
9296 CA:        DEX:imp    X=X-1	 ; both player and enemy.
9297 10:FA      BPL:rel    Branch->$9293 ;
9299 8D:35 01   STA:abs    $0135	 ; Player bullets in play = 0
929C 85:A6      STA:zp     Zp RAM 00A6	 ; Enemy bullets in play = 0
929E 60:        RTS:imp    Ret from Sub
;
929F A2:07      LDX:imm    #07		 ; Clear out the special images
92A1 A9:00      LDA:imm    #00		 ; to display for an enemy death
92A3 9D:0A 03   STA:abs,x  $030A,X	 ; and set the death-in-progress
92A6 CA:        DEX:imp    X=X-1	 ; counter to ZERO.
92A7 10:FA      BPL:rel    Branch->$92A3 ;
92A9 8D:16 01   STA:abs    $0116	 ; Clear out any enemy death
					 ; sequences in progress.
92AC 60:        RTS:imp    Ret from Sub	 ;
;
; SUBROUTINE: Zero out the spinner since last read.
;
92AD A9:00      LDA:imm    #00           ; Zero out the "spinner position
92AF 85:50      STA:zp     Zp RAM 0050   ; since last read" location.
92B1 60:        RTS:imp    Ret from Sub	 ;
;
; Subroutine: Swap some data between the inactive and active player.
;
92B2 A2:11      LDX:imm    #11		 ; This will swap the following
92B4 BD:AA 03   LDA:abs,x  $03AA,X	 ; variables between the active
92B7 BC:BC 03   LDY:abs,x  $03BC,X	 ; player and the inactive player:
92BA 9D:BC 03   STA:abs,x  $03BC,X	 ; 
92BD 98:        TYA:imp    Y-->A	 ; # of times superzapper used
92BE 9D:AA 03   STA:abs,x  $03AA,X	 ; # of enemies left
92C1 CA:        DEX:imp    X=X-1	 ; Height of the spikes in the
92C2 10:F0      BPL:rel    Branch->$92B4 ; segments.
92C4 60:        RTS:imp    Ret from Sub	 ;
;
92C5 A5:9F      LDA:zp     Zp RAM 009F   ; Are we at the last level?
92C7 C9:62      CMP:imm    #62		 ;
92C9 90:07      BCC:rel    Branch->$92D2
92CB AD:DA 60   LDA:abs    $60DA	 ; Grab a random number,
92CE 29:1F      AND:imm    #1F		 ; 40-5F.
92D0 09:40      ORA:imm    #40		 ;
92D2 85:2B      STA:zp     Zp RAM 002B   ; Store into a scratchpad address
92D4 E6:2B      INC:zp     Zp RAM 002B	 ; and add one.
92D6 A2:6F      LDX:imm    #6F
92D8 86:37      STX:zp     Zp RAM 0037
92DA A6:37      LDX:zp     Zp RAM 0037
92DC BD:07 96   LDA:abs,x  $9607,X
92DF 85:3C      STA:zp     Zp RAM 003C
92E1 BD:06 96   LDA:abs,x  $9606,X
92E4 85:3B      STA:zp     Zp RAM 003B
92E6 BD:05 96   LDA:abs,x  $9605,X
92E9 85:2D      STA:zp     Zp RAM 002D
92EB BD:04 96   LDA:abs,x  $9604,X
92EE 85:2C      STA:zp     Zp RAM 002C
92F0 A9:01      LDA:imm    #01
92F2 85:38      STA:zp     Zp RAM 0038
92F4 A0:00      LDY:imm    #00
92F6 B1:2C      LDA:ind,y  2C
92F8 8D:5E 01   STA:abs    $015E
92FB F0:1C      BEQ:rel    Branch->$9319
92FD A5:2B      LDA:zp     Zp RAM 002B
92FF C8:        INY:imp    Y=Y+1
9300 D1:2C      CMP:ind,y  2C
9302 C8:        INY:imp    Y=Y+1
9303 90:0E      BCC:rel    Branch->$9313
9305 D1:2C      CMP:ind,y  2C
9307 D0:01      BNE:rel    Branch->$930A
9309 18:        CLC:imp    Clear Carry
930A B0:07      BCS:rel    Branch->$9313
930C C8:        INY:imp    Y=Y+1
930D 20:77 96   JSR:abs    $9677
9310 4C:19 93   JMP:abs    $9319
9313 20:83 96   JSR:abs    $9683
9316 18:        CLC:imp    Clear Carry
9317 90:DD      BCC:rel    Branch->$92F6
9319 A0:00      LDY:imm    #00
931B 91:3B      STA:ind,y  3B
931D A5:37      LDA:zp     Zp RAM 0037
931F 38:        SEC:imp    Set Carry
9320 E9:04      SBC:imm    #04
9322 85:37      STA:zp     Zp RAM 0037
9324 C9:FF      CMP:imm    #FF
9326 D0:B2      BNE:rel    Branch->$92DA
9328 AD:6A 01   LDA:abs    $016A
932B 29:03      AND:imm    #03
932D C9:01      CMP:imm    #01
932F D0:1C      BNE:rel    Branch->$934D
9331 CE:1A 01   DEC:abs    $011A
9334 AD:60 01   LDA:abs    $0160
9337 49:FF      EOR:imm    #FF
9339 4A:        LSR:accum  
933A 4A:        LSR:accum  
933B 4A:        LSR:accum  
933C 6D:60 01   ADC:abs    $0160
933F 8D:60 01   STA:abs    $0160
9342 A5:9F      LDA:zp     Zp RAM 009F	 ; Are up to at least the RED 
9344 C9:11      CMP:imm    #11		 ; level? (The level after Green.)
9346 B0:02      BCS:rel    Branch->$934A
9348 C6:B3      DEC:zp     Zp RAM 00B3
934A B8:        CLV:imp    Clear Overflow
934B 50:35      BVC:rel    Branch->$9382
934D C9:02      CMP:imm    #02
934F D0:31      BNE:rel    Branch->$9382
9351 EE:1A 01   INC:abs    $011A
9354 AD:1A 01   LDA:abs    $011A
9357 C9:03      CMP:imm    #03
9359 90:05      BCC:rel    Branch->$9360
935B A9:03      LDA:imm    #03
935D 8D:1A 01   STA:abs    $011A
9360 AD:60 01   LDA:abs    $0160
9363 4A:        LSR:accum  
9364 4A:        LSR:accum  
9365 4A:        LSR:accum  
9366 09:E0      ORA:imm    #E0
9368 6D:60 01   ADC:abs    $0160
936B 8D:60 01   STA:abs    $0160
936E AD:5B 01   LDA:abs    $015B
9371 4A:        LSR:accum  
9372 4A:        LSR:accum  
9373 4A:        LSR:accum  
9374 6D:5B 01   ADC:abs    $015B
9377 8D:5B 01   STA:abs    $015B
937A AD:6D 01   LDA:abs    $016D
937D 09:40      ORA:imm    #40
937F 8D:6D 01   STA:abs    $016D
9382 AD:63 01   LDA:abs    $0163
9385 20:E0 93   JSR:abs    $93E0
9388 8D:63 01   STA:abs    $0163
938B 8C:68 01   STY:abs    $0168
938E 8E:54 01   STX:abs    $0154
9391 AD:20 01   LDA:abs    $0120
9394 20:E0 93   JSR:abs    $93E0
9397 8D:20 01   STA:abs    $0120
939A 8C:18 01   STY:abs    $0118
939D 86:A7      STX:zp     Zp RAM 00A7
939F AD:60 01   LDA:abs    $0160
93A2 20:E0 93   JSR:abs    $93E0
93A5 8D:60 01   STA:abs    $0160
93A8 8D:62 01   STA:abs    $0162
93AB 8C:67 01   STY:abs    $0167
93AE 8C:65 01   STY:abs    $0165
93B1 8E:51 01   STX:abs    $0151
93B4 8E:53 01   STX:abs    $0153
93B7 8E:52 01   STX:abs    $0152
93BA AD:60 01   LDA:abs    $0160
93BD 0A:        ASL:accum  
93BE 8D:64 01   STA:abs    $0164
93C1 AD:65 01   LDA:abs    $0165
93C4 2A:        ROL:accum  
93C5 8D:69 01   STA:abs    $0169
93C8 A9:06      LDA:imm    #06
93CA 8D:55 01   STA:abs    $0155
93CD A9:A0      LDA:imm    #A0
93CF 8D:61 01   STA:abs    $0161
93D2 A9:FE      LDA:imm    #FE
93D4 8D:66 01   STA:abs    $0166
93D7 A9:01      LDA:imm    #01
93D9 8D:4A 01   STA:abs    $014A
93DC 8D:49 01   STA:abs    $0149
93DF 60:        RTS:imp    Ret from Sub
;
93E0 A0:FF      LDY:imm    #FF
93E2 84:29      STY:zp     Zp RAM 0029
93E4 0A:        ASL:accum  
93E5 26:29      ROL:zp     Zp RAM 0029
93E7 0A:        ASL:accum  
93E8 26:29      ROL:zp     Zp RAM 0029
93EA 0A:        ASL:accum  
93EB 26:29      ROL:zp     Zp RAM 0029
93ED A4:29      LDY:zp     Zp RAM 0029
93EF 48:        PHA:imp    Push Accum
93F0 98:        TYA:imp    Y-->A
93F1 49:FF      EOR:imm    #FF
93F3 18:        CLC:imp    Clear Carry
93F4 69:0D      ADC:imm    #0D
93F6 4A:        LSR:accum  
93F7 AA:        TAX:imp    A-->X
93F8 68:        PLA:imp    Pull Accum
93F9 60:        RTS:imp    Ret from Sub
;
93FA 08:        PHP:imp    Push P
93FB 01:14      ORA:ind,x  14
93FD 50:FD      BVC:rel    Branch->$93FC
93FF 02:        Illegal Opcode
9400 15:40      ORA:zp,x   Zp RAM 0040
9402 14:        Illegal Opcode
9403 02:        Illegal Opcode
9404 41:63      EOR:ind,x  63
9406 0A:        ASL:accum  
9407 04:        Illegal Opcode
9408 01:09      ORA:ind,x  09
940A 01:01      ORA:ind,x  01
940C 01:02      ORA:ind,x  02
940E 03:        Illegal Opcode
940F 02:        Illegal Opcode
9410 02:        Illegal Opcode
9411 03:        Illegal Opcode
9412 03:        Illegal Opcode
9413 02:        Illegal Opcode
9414 0A:        ASL:accum  
9415 40:        RTI:imp    Ret from Int
9416 02:        Illegal Opcode
9417 02:        Illegal Opcode
9418 41:63      EOR:ind,x  63
941A 03:        Illegal Opcode
941B 08:        PHP:imp    Push P
941C 01:08      ORA:ind,x  08
941E D4:        Illegal Opcode
941F FB:        Illegal Opcode
9420 04:        Illegal Opcode
9421 09:10      ORA:imm    #10
9423 AF:        Illegal Opcode
9424 AC:AC AC   LDY:abs    $ACAC
9427 A8:        TAY:imp    A-->Y
9428 A4:A0      LDY:zp     Zp RAM 00A0
942A A0:08      LDY:imm    #08
942C 11:19      ORA:ind,y  19
942E AF:        Illegal Opcode
942F FD:08 1A   SBC:abs,x  $1A08,X
9432 20:9D FD   JSR:abs    $FD9D
9435 08:        PHP:imp    Push P
9436 21:27      AND:ind,x  27
9438 94:FD      STY:zp,x   Zp RAM 00FD
943A 08:        PHP:imp    Push P
943B 28:        PLP:imp    Pull P
943C 30:92      BMI:rel    Branch->$93D0
943E FF:        Illegal Opcode
943F 08:        PHP:imp    Push P
9440 31:40      AND:ind,y  40
9442 88:        DEY:imp    Y=Y-1
9443 FF:        Illegal Opcode
9444 0C:        Illegal Opcode
9445 41:63      EOR:ind,x  63
9447 60:        RTS:imp    Ret from Sub
9448 41:0A      EOR:ind,x  0A
944A 01:63      ORA:ind,x  63
944C C0:0A      CPY:imm    #0A
944E 01:14      ORA:ind,x  14
9450 00:        BRK:imp    BREAK
9451 0A:        ASL:accum  
9452 15:20      ORA:zp,x   Zp RAM 0020
9454 D0:0A      BNE:rel    Branch->$9460
9456 21:30      AND:ind,x  30
9458 D8:        CLD:imp    Unset Decimal
9459 0A:        ASL:accum  
945A 31:63      AND:ind,y  63
945C D0:02      BNE:rel    Branch->$9460
945E 01:20      ORA:ind,x  20
9460 A0:02      LDY:imm    #02
9462 21:40      AND:ind,x  40
9464 A0:02      LDY:imm    #02
9466 41:63      EOR:ind,x  63
9468 C0:02      CPY:imm    #02
946A 01:30      ORA:ind,x  30
946C 04:        Illegal Opcode
946D 02:        Illegal Opcode
946E 31:40      AND:ind,y  40
9470 06:02      ASL:zp     Zp RAM 0002
9472 41:63      EOR:ind,x  63
9474 08:        PHP:imp    Push P
9475 02:        Illegal Opcode
9476 01:20      ORA:ind,x  20
9478 01:02      ORA:ind,x  02
947A 21:28      AND:ind,x  28
947C 03:        Illegal Opcode
947D 02:        Illegal Opcode
947E 29:63      AND:imm    #63
9480 02:        Illegal Opcode
9481 02:        Illegal Opcode
9482 01:30      ORA:ind,x  30
9484 01:02      ORA:ind,x  02
9486 31:63      AND:ind,y  63
9488 03:        Illegal Opcode
9489 04:        Illegal Opcode
948A 01:04      ORA:ind,x  04
948C 00:        BRK:imp    BREAK
948D 00:        BRK:imp    BREAK
948E 00:        BRK:imp    BREAK
948F 01:02      ORA:ind,x  02
9491 05:10      ORA:zp     Zp RAM 0010
9493 02:        Illegal Opcode
9494 02:        Illegal Opcode
9495 11:13      ORA:ind,y  13
9497 00:        BRK:imp    BREAK
9498 02:        Illegal Opcode
9499 14:        Illegal Opcode
949A 20:01 02   JSR:abs    $0201
949D 23:        Illegal Opcode
949E 27:        Illegal Opcode
949F 01:02      ORA:ind,x  02
94A1 2C:63 01   BIT:abs    $0163
94A4 00:        BRK:imp    BREAK
94A5 04:        Illegal Opcode
94A6 01:06      ORA:ind,x  06
94A8 00:        BRK:imp    BREAK
94A9 00:        BRK:imp    BREAK
94AA 00:        BRK:imp    BREAK
94AB 02:        Illegal Opcode
94AC 03:        Illegal Opcode
94AD 04:        Illegal Opcode
94AE 02:        Illegal Opcode
94AF 07:        Illegal Opcode
94B0 0A:        ASL:accum  
94B1 04:        Illegal Opcode
94B2 02:        Illegal Opcode
94B3 0B:        Illegal Opcode
94B4 10:03      BPL:rel    Branch->$94B9
94B6 02:        Illegal Opcode
94B7 14:        Illegal Opcode
94B8 19:02 04   ORA:abs,y  $0402,Y
94BB 1A:        Illegal Opcode
94BC 20:01 02   JSR:abs    $0201
94BF 02:        Illegal Opcode
94C0 02:        Illegal Opcode
94C1 01:01      ORA:ind,x  01
94C3 02:        Illegal Opcode
94C4 02:        Illegal Opcode
94C5 35:27      AND:zp,x   Zp RAM 0027
94C7 01:02      ORA:ind,x  02
94C9 2B:        Illegal Opcode
94CA 63:        Illegal Opcode
94CB 01:00      ORA:ind,x  00
94CD 02:        Illegal Opcode
94CE 01:04      ORA:ind,x  04
94D0 01:02      ORA:ind,x  02
94D2 05:63      ORA:zp     Zp RAM 0063
94D4 00:        BRK:imp    BREAK
94D5 00:        BRK:imp    BREAK
94D6 02:        Illegal Opcode
94D7 01:04      ORA:ind,x  04
94D9 04:        Illegal Opcode
94DA 02:        Illegal Opcode
94DB 05:10      ORA:zp     Zp RAM 0010
94DD 05:02      ORA:zp     Zp RAM 0002
94DF 11:13      ORA:ind,y  13
94E1 03:        Illegal Opcode
94E2 02:        Illegal Opcode
94E3 14:        Illegal Opcode
94E4 19:04 02   ORA:abs,y  $0204,Y
94E7 1A:        Illegal Opcode
94E8 63:        Illegal Opcode
94E9 05:00      ORA:zp     Zp RAM 0000
94EB 04:        Illegal Opcode
94EC 01:04      ORA:ind,x  04
94EE 00:        BRK:imp    BREAK
94EF 00:        BRK:imp    BREAK
94F0 01:00      ORA:ind,x  00
94F2 02:        Illegal Opcode
94F3 05:10      ORA:zp     Zp RAM 0010
94F5 01:02      ORA:ind,x  02
94F7 11:20      ORA:ind,y  20
94F9 01:02      ORA:ind,x  02
94FB 21:27      AND:ind,x  27
94FD 01:02      ORA:ind,x  02
94FF 28:        PLP:imp    Pull P
9500 63:        Illegal Opcode
9501 01:00      ORA:ind,x  00
9503 04:        Illegal Opcode
9504 01:05      ORA:ind,x  05
9506 00:        BRK:imp    BREAK
9507 00:        BRK:imp    BREAK
9508 01:00      ORA:ind,x  00
950A 01:02      ORA:ind,x  02
950C 06:10      ASL:zp     Zp RAM 0010
950E 02:        Illegal Opcode
950F 02:        Illegal Opcode
9510 11:1A      ORA:ind,y  1A
9512 01:02      ORA:ind,x  02
9514 1B:        Illegal Opcode
9515 20:01 02   JSR:abs    $0201
9518 21:2C      AND:ind,x  2C
951A 02:        Illegal Opcode
951B 02:        Illegal Opcode
951C 2D:63 03   AND:abs    $0363
951F 00:        BRK:imp    BREAK
9520 02:        Illegal Opcode
9521 11:20      ORA:ind,y  20
9523 02:        Illegal Opcode
9524 02:        Illegal Opcode
9525 21:63      AND:ind,x  63
9527 01:00      ORA:ind,x  00
9529 04:        Illegal Opcode
952A 11:20      ORA:ind,y  20
952C 05:03      ORA:zp     Zp RAM 0003
952E 02:        Illegal Opcode
952F 02:        Illegal Opcode
9530 02:        Illegal Opcode
9531 02:        Illegal Opcode
9532 02:        Illegal Opcode
9533 02:        Illegal Opcode
9534 02:        Illegal Opcode
9535 02:        Illegal Opcode
9536 02:        Illegal Opcode
9537 02:        Illegal Opcode
9538 02:        Illegal Opcode
9539 03:        Illegal Opcode
953A 04:        Illegal Opcode
953B 02:        Illegal Opcode
953C 02:        Illegal Opcode
953D 21:63      AND:ind,x  63
953F 03:        Illegal Opcode
9540 00:        BRK:imp    BREAK
9541 02:        Illegal Opcode
9542 0B:        Illegal Opcode
9543 10:01      BPL:rel    Branch->$9546
9545 02:        Illegal Opcode
9546 16:19      ASL:zp,x   Zp RAM 0019
9548 01:02      ORA:ind,x  02
954A 1B:        Illegal Opcode
954B 63:        Illegal Opcode
954C 01:00      ORA:ind,x  00
954E 02:        Illegal Opcode
954F 0B:        Illegal Opcode
9550 10:01      BPL:rel    Branch->$9553
9552 02:        Illegal Opcode
9553 16:19      ASL:zp,x   Zp RAM 0019
9555 01:02      ORA:ind,x  02
9557 1B:        Illegal Opcode
9558 20:01 02   JSR:abs    $0201
955B 21:27      AND:ind,x  27
955D 04:        Illegal Opcode
955E 02:        Illegal Opcode
955F 28:        PLP:imp    Pull P
9560 63:        Illegal Opcode
9561 03:        Illegal Opcode
9562 00:        BRK:imp    BREAK
9563 04:        Illegal Opcode
9564 11:12      ORA:ind,y  12
9566 28:        PLP:imp    Pull P
9567 14:        Illegal Opcode
9568 0C:        Illegal Opcode
9569 13:        Illegal Opcode
956A 20:14 28   JSR:abs    $2814
956D 08:        PHP:imp    Push P
956E 21:27      AND:ind,x  27
9570 14:        Illegal Opcode
9571 FF:        Illegal Opcode
9572 0C:        Illegal Opcode
9573 28:        PLP:imp    Pull P
9574 63:        Illegal Opcode
9575 14:        Illegal Opcode
9576 0A:        ASL:accum  
9577 00:        BRK:imp    BREAK
9578 0C:        Illegal Opcode
9579 11:20      ORA:ind,y  20
957B 00:        BRK:imp    BREAK
957C 40:        RTI:imp    Ret from Int
957D 0C:        Illegal Opcode
957E 21:30      AND:ind,x  30
9580 40:        RTI:imp    Ret from Int
9581 C0:02      CPY:imm    #02
9583 31:63      AND:ind,y  63
9585 C0:00      CPY:imm    #00
9587 02:        Illegal Opcode
9588 01:10      ORA:ind,x  10
958A DC:        Illegal Opcode
958B 02:        Illegal Opcode
958C 11:27      ORA:ind,y  27
958E C0:08      CPY:imm    #08
9590 28:        PLP:imp    Pull P
9591 40:        RTI:imp    Ret from Int
9592 C0:01      CPY:imm    #01
9594 02:        Illegal Opcode
9595 41:63      EOR:ind,x  63
9597 E6:02      INC:zp     Zp RAM 0002
9599 01:63      ORA:ind,x  63
959B 06:06      ASL:zp     Zp RAM 0006
959D 01:63      ORA:ind,x  63
959F 00:        BRK:imp    BREAK
95A0 00:        BRK:imp    BREAK
95A1 00:        BRK:imp    BREAK
95A2 E0:D8      CPX:imm    #D8
95A4 D4:        Illegal Opcode
95A5 D0:C8      BNE:rel    Branch->$956F
95A7 C0:B8      CPY:imm    #B8
95A9 B0:A8      BCS:rel    Branch->$9553
95AB A0:A0      LDY:imm    #A0
95AD A0:A8      LDY:imm    #A8
95AF A0:9C      LDY:imm    #9C
95B1 9A:        TXS:imp    X-->Stack
95B2 98:        TYA:imp    Y-->A
95B3 04:        Illegal Opcode
95B4 01:10      ORA:ind,x  10
95B6 0A:        ASL:accum  
95B7 0C:        Illegal Opcode
95B8 0F:        Illegal Opcode
95B9 11:14      ORA:ind,y  14
95BB 16:14      ASL:zp,x   Zp RAM 0014
95BD 18:        CLC:imp    Clear Carry
95BE 1B:        Illegal Opcode
95BF 1D:1B 18   ORA:abs,x  $181B,X
95C2 1A:        Illegal Opcode
95C3 1C:        Illegal Opcode
95C4 1E:1B 08   ASL:abs,x  $081B,X
95C7 11:1A      ORA:ind,y  1A
95C9 14:        Illegal Opcode
95CA 01:02      ORA:ind,x  02
95CC 1B:        Illegal Opcode
95CD 27:        Illegal Opcode
95CE 1B:        Illegal Opcode
95CF 08:        PHP:imp    Push P
95D0 28:        PLP:imp    Pull P
95D1 30:1D      BMI:rel    Branch->$95F0
95D3 01:08      ORA:ind,x  08
95D5 31:40      AND:ind,y  40
95D7 1F:        Illegal Opcode
95D8 01:08      ORA:ind,x  08
95DA 41:50      EOR:ind,x  50
95DC 23:        Illegal Opcode
95DD 01:08      ORA:ind,x  08
95DF 51:63      EOR:ind,y  63
95E1 2B:        Illegal Opcode
95E2 01:02      ORA:ind,x  02
95E4 01:14      ORA:ind,x  14
95E6 02:        Illegal Opcode
95E7 02:        Illegal Opcode
95E8 15:20      ORA:zp,x   Zp RAM 0020
95EA 02:        Illegal Opcode
95EB 02:        Illegal Opcode
95EC 21:63      AND:ind,x  63
95EE 03:        Illegal Opcode
95EF 02:        Illegal Opcode
95F0 3C:        Illegal Opcode
95F1 63:        Illegal Opcode
95F2 40:        RTI:imp    Ret from Int
95F3 00:        BRK:imp    BREAK
95F4 06:01      ASL:zp     Zp RAM 0001
95F6 63:        Illegal Opcode
95F7 07:        Illegal Opcode
95F8 0B:        Illegal Opcode
95F9 19:24 53   ORA:abs,y  $5324,Y
95FC 0B:        Illegal Opcode
95FD 24:19      BIT:zp     Zp RAM 0019
95FF 53:        Illegal Opcode
9600 87:        Illegal Opcode
9601 24:19      BIT:zp     Zp RAM 0019
9603 53:        Illegal Opcode
9604 07:        Illegal Opcode
;
; Data segment per Ken Lui.
;
9605 87:        DATA
9606 24:EF      DATA
9608 95:6D      DATA
960A 01:E3      DATA
960C 95:B3      DATA
960E 00:FA      DATA
9610 93:        DATA
9611 19:01 07   DATA
9614 94:1A      DATA
9616 01:CD      DATA
9618 94:29      DATA
961A 01:D6      DATA
961C 94:2E      DATA
961E 01:20      DATA
9620 95:2A      DATA
9622 01:29      DATA
9624 95:2F      DATA
9626 01:EB      DATA
9628 94:2B      DATA
962A 01:03      DATA
962C 95:30      DATA
962E 01:89      DATA
9630 94:2C      DATA
9632 01:A5      DATA
9634 94:31      DATA
9636 01:41      DATA
9638 95:2D      DATA
963A 01:4E      DATA
963C 95:32      DATA
963E 01:5D      DATA
9640 94:57      DATA
9642 01:69      DATA
9644 94:47      DATA
9646 01:75      DATA
9648 94:4B      DATA
964A 01:81      DATA
964C 94:4C      DATA
964E 01:98      DATA
9650 95:1C      DATA
9652 01:B3      DATA
9654 95:5B      DATA
9656 01:9C      DATA
9658 95:5A      DATA
965A 01:63      DATA
965C 95:B2      DATA
965E 00:        DATA
965F F4:        DATA
9660 95:5D      DATA
9662 01:4D      DATA
9664 94:63      DATA
9666 01:49      DATA
9668 94:20      DATA
966A 01:1B      DATA
966C 94:60      DATA
966E 01:78      DATA
9670 95:59      DATA
9672 01:87      DATA
9674 95:5F      DATA
9676 01         DATA
;
9677 AE         Illegal Opcode		 ; HEY....
9678 5E:01 BD   LSR:abs,x  $BD01,X	 ; Re-disassemble. We've got a
967B 90:96      BCC:rel    Branch->$9613 ; JSR to $9677 from $930D.
967D 48:        PHA:imp    Push Accum
967E BD:8F 96   LDA:abs,x  $968F,X
9681 48:        PHA:imp    Push Accum
9682 60:        RTS:imp    Ret from Sub
;
9683 AE:5E 01   LDX:abs    $015E
9686 BD:9E 96   LDA:abs,x  $969E,X
9689 48:        PHA:imp    Push Accum
968A BD:9D 96   LDA:abs,x  $969D,X
968D 48:        PHA:imp    Push Accum
968E 60:        RTS:imp    Ret from Sub
;
; Data segment, per Ken Lui.
;
968F 00:00      DATA
9691 C3:        DATA
9692 96:B6      DATA
9694 96:AA      DATA
9696 96:E1      DATA
9698 96:DA      DATA
969A 96:FF      DATA
969C 96         DATA
;
; Another data segment, per Ken Lui.
;
969D 00:00      DATA
969F C7:        DATA
96A0 96:CA      DATA
96A2 96:CA      DATA
96A4 96:C6      DATA
96A6 96:C7      DATA
96A8 96:C6      DATA
96AA 96         DATA
96AB A5         Illegal Opcode		 ; Chances are we'll have to
96AC 2B:        Illegal Opcode		 ; re disassemble these two lines.
96AD 38:        SEC:imp    Set Carry
96AE E9:01      SBC:imm    #01
96B0 29:0F      AND:imm    #0F
96B2 18:        CLC:imp    Clear Carry
96B3 69:01      ADC:imm    #01
96B5 10:02      BPL:rel    Branch->$96B9
96B7 A5:2B      LDA:zp     Zp RAM 002B
96B9 84:29      STY:zp     Zp RAM 0029
96BB 88:        DEY:imp    Y=Y-1
96BC 88:        DEY:imp    Y=Y-1
96BD 38:        SEC:imp    Set Carry
96BE F1:2C      SBC:ind,y  2C
96C0 18:        CLC:imp    Clear Carry
96C1 65:29      ADC:zp     Zp RAM 0029
96C3 A8:        TAY:imp    A-->Y
96C4 B1:2C      LDA:ind,y  2C
96C6 60:        RTS:imp    Ret from Sub
;
; Apparently unreferenced?
;
96C7 C8:        INY:imp    Y=Y+1	 ; Y=Y+3
96C8 C8:        INY:imp    Y=Y+1	 ;
96C9 C8:        INY:imp    Y=Y+1	 ;
96CA 60:        RTS:imp    Ret from Sub	 ;
;
96CB B1:2C      LDA:ind,y  2C
96CD 88:        DEY:imp    Y=Y-1
96CE 38:        SEC:imp    Set Carry
96CF F1:2C      SBC:ind,y  2C
96D1 85:29      STA:zp     Zp RAM 0029
96D3 98:        TYA:imp    Y-->A
96D4 38:        SEC:imp    Set Carry
96D5 65:29      ADC:zp     Zp RAM 0029
96D7 A8:        TAY:imp    A-->Y
96D8 C8:        INY:imp    Y=Y+1
96D9 C8:        INY:imp    Y=Y+1
96DA 60:        RTS:imp    Ret from Sub
;
; Apparently unreferenced?
;
96DB B1:2C      LDA:ind,y  2C
96DD 18:        CLC:imp    Clear Carry
96DE 6D:60 01   ADC:abs    $0160
96E1 60:        RTS:imp    Ret from Sub
;
96E2 20:F4 96   JSR:abs    $96F4
96E5 AA:        TAX:imp    A-->X
96E6 B1:2C      LDA:ind,y  2C
96E8 C8:        INY:imp    Y=Y+1
96E9 E0:00      CPX:imm    #00
96EB F0:06      BEQ:rel    Branch->$96F3
96ED 18:        CLC:imp    Clear Carry
96EE 71:2C      ADC:ind,y  2C
96F0 CA:        DEX:imp    X=X-1
96F1 D0:FA      BNE:rel    Branch->$96ED
96F3 60:        RTS:imp    Ret from Sub
96F4 A5:2B      LDA:zp     Zp RAM 002B
96F6 84:29      STY:zp     Zp RAM 0029
96F8 88:        DEY:imp    Y=Y-1
96F9 88:        DEY:imp    Y=Y-1
96FA 38:        SEC:imp    Set Carry
96FB F1:2C      SBC:ind,y  2C
96FD C8:        INY:imp    Y=Y+1
96FE C8:        INY:imp    Y=Y+1
96FF 60:        RTS:imp    Ret from Sub
;
9700 20:F4 96   JSR:abs    $96F4
9703 29:01      AND:imm    #01
9705 F0:01      BEQ:rel    Branch->$9708
9707 C8:        INY:imp    Y=Y+1
9708 B1:2C      LDA:ind,y  2C
970A 60:        RTS:imp    Ret from Sub
;
970B 20:49 97   JSR:abs    $9749
970E 20:3F A2   JSR:abs    $A23F
9711 20:3A A8   JSR:abs    $A83A
9714 20:A2 98   JSR:abs    $98A2
9717 20:1E 9B   JSR:abs    $9B1E
971A 20:8F A1   JSR:abs    $A18F
971D 20:A6 A2   JSR:abs    $A2A6
9720 20:54 A4   JSR:abs    $A454
9723 20:16 A4   JSR:abs    $A416
9726 4C:04 A5   JMP:abs    $A504
9729 AD:23 01   LDA:abs    $0123
972C 29:7F      AND:imm    #7F
972E 8D:23 01   STA:abs    $0123
9731 20:49 97   JSR:abs    $9749
9734 20:F8 97   JSR:abs    $97F8
9737 20:16 A4   JSR:abs    $A416
973A 20:3F A2   JSR:abs    $A23F
973D 20:8F A1   JSR:abs    $A18F
9740 AD:01 02   LDA:abs    $0201
9743 10:03      BPL:rel    Branch->$9748
9745 20:04 A5   JSR:abs    $A504
9748 60:        RTS:imp    Ret from Sub
;
9749 AD:01 02   LDA:abs    $0201	 ; What segment is the player on?
974C 10:01      BPL:rel    Branch->$974F ; If the player is dead, then
974E 60:        RTS:imp    Ret from Sub	 ; just return. (segment >= 80)
974F A2:00      LDX:imm    #00
9751 A5:05      LDA:zp     Zp RAM 0005
9753 30:06      BMI:rel    Branch->$975B
9755 20:C5 97   JSR:abs    $97C5
9758 B8:        CLV:imp    Clear Overflow
9759 50:15      BVC:rel    Branch->$9770
975B A5:50      LDA:zp     Zp RAM 0050
975D 10:09      BPL:rel    Branch->$9768
975F C9:E1      CMP:imm    #E1
9761 B0:02      BCS:rel    Branch->$9765
9763 A9:E1      LDA:imm    #E1
9765 B8:        CLV:imp    Clear Overflow
9766 50:06      BVC:rel    Branch->$976E
9768 C9:1F      CMP:imm    #1F
976A 90:02      BCC:rel    Branch->$976E
976C A9:1F      LDA:imm    #1F
976E 86:50      STX:zp     Zp RAM 0050
9770 85:2B      STA:zp     Zp RAM 002B
9772 49:FF      EOR:imm    #FF
9774 38:        SEC:imp    Set Carry
9775 65:51      ADC:zp     Zp RAM 0051
9777 85:2C      STA:zp     Zp RAM 002C
9779 AE:11 01   LDX:abs    $0111
977C F0:1F      BEQ:rel    Branch->$979D
977E C9:F0      CMP:imm    #F0
9780 90:04      BCC:rel    Branch->$9786
9782 A9:EF      LDA:imm    #EF
9784 85:2C      STA:zp     Zp RAM 002C
9786 45:2B      EOR:zp     Zp RAM 002B
9788 10:13      BPL:rel    Branch->$979D
978A A5:2C      LDA:zp     Zp RAM 002C
978C 45:51      EOR:zp     Zp RAM 0051
978E 10:0D      BPL:rel    Branch->$979D
9790 A5:51      LDA:zp     Zp RAM 0051
9792 30:05      BMI:rel    Branch->$9799
9794 A9:00      LDA:imm    #00
9796 B8:        CLV:imp    Clear Overflow
9797 50:02      BVC:rel    Branch->$979B
9799 A9:EF      LDA:imm    #EF
979B 85:2C      STA:zp     Zp RAM 002C
979D A5:2C      LDA:zp     Zp RAM 002C
979F 4A:        LSR:accum  
97A0 4A:        LSR:accum  
97A1 4A:        LSR:accum  
97A2 4A:        LSR:accum  
97A3 85:2A      STA:zp     Zp RAM 002A
97A5 18:        CLC:imp    Clear Carry
97A6 69:01      ADC:imm    #01
97A8 29:0F      AND:imm    #0F
97AA 85:2B      STA:zp     Zp RAM 002B
97AC A5:2A      LDA:zp     Zp RAM 002A
97AE CD:00 02   CMP:abs    $0200         ; Same tunnel segment as the player?
97B1 F0:03      BEQ:rel    Branch->$97B6
97B3 20:B5 CC   JSR:abs    $CCB5
97B6 A5:2A      LDA:zp     Zp RAM 002A
97B8 8D:00 02   STA:abs    $0200
97BB A5:2B      LDA:zp     Zp RAM 002B
97BD 8D:01 02   STA:abs    $0201
97C0 A5:2C      LDA:zp     Zp RAM 002C
97C2 85:51      STA:zp     Zp RAM 0051
97C4 60:        RTS:imp    Ret from Sub
;
97C5 A9:FF      LDA:imm    #FF
97C7 85:29      STA:zp     Zp RAM 0029
97C9 85:2A      STA:zp     Zp RAM 002A
97CB AE:1C 01   LDX:abs    $011C	 ; Start with slot 6 of 0-6
97CE BD:DF 02   LDA:abs,x  $02DF,X	 ; Is there an enemy here?
97D1 F0:08      BEQ:rel    Branch->$97DB ; If not, check for another.
97D3 C5:29      CMP:zp     Zp RAM 0029
97D5 B0:04      BCS:rel    Branch->$97DB
97D7 85:29      STA:zp     Zp RAM 0029
97D9 86:2A      STX:zp     Zp RAM 002A
97DB CA:        DEX:imp    X=X-1
97DC 10:F0      BPL:rel    Branch->$97CE
97DE A6:2A      LDX:zp     Zp RAM 002A
97E0 30:15      BMI:rel    Branch->$97F7
97E2 BD:B9 02   LDA:abs,x  $02B9,X
97E5 AC:00 02   LDY:abs    $0200
97E8 20:A6 A7   JSR:abs    $A7A6
97EB A8:        TAY:imp    A-->Y
97EC F0:09      BEQ:rel    Branch->$97F7
97EE 30:05      BMI:rel    Branch->$97F5
97F0 A9:F7      LDA:imm    #F7
97F2 B8:        CLV:imp    Clear Overflow
97F3 50:02      BVC:rel    Branch->$97F7
97F5 A9:09      LDA:imm    #09
97F7 60:        RTS:imp    Ret from Sub
97F8 AD:01 02   LDA:abs    $0201	 ; Is the player alive?
97FB 10:01      BPL:rel    Branch->$97FE ; If so, return.
97FD 60:        RTS:imp    Ret from Sub
;
; Start of ROM 136002.114 at $9800.
;
97FE AD:06 01   LDA:abs    $0106
9801 30:01      BMI:rel    Branch->$9804
9803 60:        RTS:imp    Ret from Sub
9804 AD:02 02   LDA:abs    $0202	 ; Get the player's Y position.
9807 C9:10      CMP:imm    #10		 ; Are they are the top of the tunnel?
9809 D0:03      BNE:rel    Branch->$980E
980B 20:EE CC   JSR:abs    $CCEE
980E AD:07 01   LDA:abs    $0107
9811 18:        CLC:imp    Clear Carry
9812 6D:04 01   ADC:abs    $0104
9815 8D:07 01   STA:abs    $0107
9818 AD:02 02   LDA:abs    $0202	 ; Get player's Y pos in the tunnel.
981B 6D:05 01   ADC:abs    $0105	 ; Move them downwards.
981E 8D:02 02   STA:abs    $0202	 ; Store player's Y position.
9821 B0:02      BCS:rel    Branch->$9825
9823 C9:F0      CMP:imm    #F0
9825 90:0C      BCC:rel    Branch->$9833
9827 A9:0E      LDA:imm    #0E
9829 85:00      STA:zp     Zp RAM 0000
982B 20:F2 CC   JSR:abs    $CCF2
982E A9:FF      LDA:imm    #FF		 ; Put player at the BOTTOM of tunnel.
9830 8D:02 02   STA:abs    $0202	 ;
;
9833 AD:02 02   LDA:abs    $0202	 ; Is the player above or below the
9836 C9:50      CMP:imm    #50		 ; mid-point of the tunnel?
9838 90:08      BCC:rel    Branch->$9842
983A AD:15 01   LDA:abs    $0115
983D D0:03      BNE:rel    Branch->$9842
983F 20:BD A7   JSR:abs    $A7BD
9842 A5:5C      LDA:zp     Zp RAM 005C
9844 18:        CLC:imp    Clear Carry
9845 6D:04 01   ADC:abs    $0104
9848 85:5C      STA:zp     Zp RAM 005C
984A A5:5F      LDA:zp     Zp RAM 005F
984C 6D:05 01   ADC:abs    $0105
984F 90:02      BCC:rel    Branch->$9853
9851 E6:5B      INC:zp     Zp RAM 005B
9853 C5:5F      CMP:zp     Zp RAM 005F
9855 F0:03      BEQ:rel    Branch->$985A
9857 EE:14 01   INC:abs    $0114
985A 85:5F      STA:zp     Zp RAM 005F
985C A5:9F      LDA:zp     Zp RAM 009F
985E 0A:        ASL:accum  
985F 0A:        ASL:accum  
9860 C9:30      CMP:imm    #30
9862 90:02      BCC:rel    Branch->$9866
9864 A9:30      LDA:imm    #30
9866 18:        CLC:imp    Clear Carry
9867 69:20      ADC:imm    #20
9869 18:        CLC:imp    Clear Carry
986A 6D:04 01   ADC:abs    $0104
986D 8D:04 01   STA:abs    $0104
9870 AD:05 01   LDA:abs    $0105
9873 69:00      ADC:imm    #00
9875 8D:05 01   STA:abs    $0105
;
; Spike/Player collision detection.
;
9878 AD:02 02   LDA:abs    $0202	 ; Is the player at the very
987B C9:F0      CMP:imm    #F0	 	 ; bottom of the tunnel?
987D B0:22      BCS:rel    Branch->$98A1 ; If so, ignore this crap. Return.
987F A2:0F      LDX:imm    #0F		 ; For all tunnel segments...
9881 BD:AC 03   LDA:abs,x  $03AC,X	 ; Load the height of the spike.
9884 F0:18      BEQ:rel    Branch->$989E ; If no spike, check the next one.
9886 EC:00 02   CPX:abs    $0200         ; Player on the same segment?
9889 D0:13      BNE:rel    Branch->$989E ; If not, check next spike.
988B CD:02 02   CMP:abs    $0202	 ; Player ran into the spike?
988E B0:0E      BCS:rel    Branch->$989E ; If not, check next spike.
9890 20:06 CD   JSR:abs    $CD06	 ; PLAYER/SPIKE Collision Detected.
9893 20:47 A3   JSR:abs    $A347
9896 A9:00      LDA:imm    #00
9898 8D:15 01   STA:abs    $0115
989B 20:8F 92   JSR:abs    $928F	 ; Remove all bullets from play.
989E CA:        DEX:imp    X=X-1	 ; Check the next spike
989F 10:E0      BPL:rel    Branch->$9881 ;
98A1 60:        RTS:imp    Ret from Sub  ; All done.
;
98A2 A0:00      LDY:imm    #00
98A4 8C:4F 01   STY:abs    $014F
98A7 AD:08 01   LDA:abs    $0108	 ; Get # of enemies in the tube.
98AA 18:        CLC:imp    Clear Carry	 ; Add with the # of enemies at
98AB 6D:09 01   ADC:abs    $0109	 ; the top of the tube.
98AE CD:1C 01   CMP:abs    $011C
98B1 90:04      BCC:rel    Branch->$98B7
98B3 F0:02      BEQ:rel    Branch->$98B7
98B5 A0:FF      LDY:imm    #FF
98B7 AD:25 01   LDA:abs    $0125
98BA F0:02      BEQ:rel    Branch->$98BE
98BC A0:FF      LDY:imm    #FF
98BE 84:2F      STY:zp     Zp RAM 002F
98C0 A2:3F      LDX:imm    #3F		 ; Go through all of the countdown
98C2 BD:43 02   LDA:abs,x  $0243,X	 ; positions for an enemy to appear.
98C5 F0:52      BEQ:rel    Branch->$9919 ; If no enemy pending in this
					 ; slot, skip to the next slot.
98C7 24:2F      BIT:zp     Zp RAM 002F
98C9 30:23      BMI:rel    Branch->$98EE
98CB 38:        SEC:imp    Set Carry
98CC E9:01      SBC:imm    #01
98CE 9D:43 02   STA:abs,x  $0243,X
98D1 D0:06      BNE:rel    Branch->$98D9
98D3 20:23 99   JSR:abs    $9923
98D6 B8:        CLV:imp    Clear Overflow
98D7 50:15      BVC:rel    Branch->$98EE
98D9 C9:3F      CMP:imm    #3F
98DB D0:11      BNE:rel    Branch->$98EE
98DD BC:03 02   LDY:abs,x  $0203,X
98E0 AD:4F 01   LDA:abs    $014F
98E3 0D:4F 01   ORA:abs    $014F
98E6 39:38 CA   AND:abs,y  $CA38,Y
98E9 F0:03      BEQ:rel    Branch->$98EE
98EB FE:43 02   INC:abs,x  $0243,X
98EE BD:43 02   LDA:abs,x  $0243,X
98F1 C9:40      CMP:imm    #40
98F3 90:14      BCC:rel    Branch->$9909
98F5 A5:03      LDA:zp     Zp RAM 0003
98F7 29:01      AND:imm    #01
98F9 D0:0B      BNE:rel    Branch->$9906
98FB BD:03 02   LDA:abs,x  $0203,X
98FE 18:        CLC:imp    Clear Carry
98FF 69:01      ADC:imm    #01
9901 29:0F      AND:imm    #0F
9903 9D:03 02   STA:abs,x  $0203,X
9906 B8:        CLV:imp    Clear Overflow
9907 50:10      BVC:rel    Branch->$9919
9909 C9:20      CMP:imm    #20
990B 90:0C      BCC:rel    Branch->$9919
990D BC:03 02   LDY:abs,x  $0203,X
9910 B9:38 CA   LDA:abs,y  $CA38,Y
9913 0D:4F 01   ORA:abs    $014F
9916 8D:4F 01   STA:abs    $014F
9919 CA:        DEX:imp    X=X-1
991A 10:A6      BPL:rel    Branch->$98C2
991C AD:4F 01   LDA:abs    $014F
991F 8D:50 01   STA:abs    $0150
9922 60:        RTS:imp    Ret from Sub
;
9923 A9:F0      LDA:imm    #F0
9925 85:29      STA:zp     Zp RAM 0029
9927 BD:03 02   LDA:abs,x  $0203,X
992A 85:2A      STA:zp     Zp RAM 002A
992C 86:35      STX:zp     Zp RAM 0035
992E 20:A5 99   JSR:abs    $99A5
9931 A6:35      LDX:zp     Zp RAM 0035
9933 A5:29      LDA:zp     Zp RAM 0029
9935 F0:0E      BEQ:rel    Branch->$9945
9937 20:4D 99   JSR:abs    $994D
993A F0:09      BEQ:rel    Branch->$9945
993C CE:AB 03   DEC:abs    $03AB         ; One less enemy at the very bottom.
993F A9:00      LDA:imm    #00
9941 9D:43 02   STA:abs,x  $0243,X
9944 60:        RTS:imp    Ret from Sub
;
9945 A9:FF      LDA:imm    #FF
9947 85:2F      STA:zp     Zp RAM 002F
9949 FE:43 02   INC:abs,x  $0243,X
994C 60:        RTS:imp    Ret from Sub
;
994D 84:36      STY:zp     Zp RAM 0036
994F AC:1C 01   LDY:abs    $011C	 ; Start with slot 6 (of 0-6)
9952 B9:DF 02   LDA:abs,y  $02DF,Y	 ; Is there an enemy in this lot?
9955 D0:46      BNE:rel    Branch->$999D ; If not, loop through next slot.
9957 A5:29      LDA:zp     Zp RAM 0029
9959 99:DF 02   STA:abs,y  $02DF,Y
995C A5:2A      LDA:zp     Zp RAM 002A
995E C9:0F      CMP:imm    #0F
9960 D0:0A      BNE:rel    Branch->$996C
9962 2C:11 01   BIT:abs    $0111
9965 10:05      BPL:rel    Branch->$996C
9967 AD:CA 60   LDA:abs    $60CA	 ; Get a random number,
996A 29:0E      AND:imm    #0E		 ; 0-E that is even.
996C 99:B9 02   STA:abs,y  $02B9,Y
996F 18:        CLC:imp    Clear Carry
9970 69:01      ADC:imm    #01
9972 29:0F      AND:imm    #0F
9974 99:CC 02   STA:abs,y  $02CC,Y
9977 A9:00      LDA:imm    #00
9979 99:A6 02   STA:abs,y  $02A6,Y
997C A5:2C      LDA:zp     Zp RAM 002C
997E 99:8A 02   STA:abs,y  $028A,Y
9981 A5:2D      LDA:zp     Zp RAM 002D
9983 99:91 02   STA:abs,y  $0291,Y
9986 EE:08 01   INC:abs    $0108	 ; One more enemy inside the tube.
9989 A5:2B      LDA:zp     Zp RAM 002B   ; Set their movement style based
998B 99:83 02   STA:abs,y  $0283,Y	 ; on the passed parameter.
998E A4:36      LDY:zp     Zp RAM 0036
9990 29:07      AND:imm    #07
9992 86:36      STX:zp     Zp RAM 0036
9994 AA:        TAX:imp    A-->X
9995 FE:42 01   INC:abs,x  $0142,X	 ; There is one more of some
					 ; type of enemy in the tunnel.
9998 A6:36      LDX:zp     Zp RAM 0036
999A A9:10      LDA:imm    #10
999C 60:        RTS:imp    Ret from Sub
999D 88:        DEY:imp    Y=Y-1
999E 10:B2      BPL:rel    Branch->$9952
99A0 A4:36      LDY:zp     Zp RAM 0036
99A2 A9:00      LDA:imm    #00
99A4 60:        RTS:imp    Ret from Sub
99A5 A9:00      LDA:imm    #00
99A7 A2:04      LDX:imm    #04
99A9 9D:3D 01   STA:abs,x  $013D,X
99AC CA:        DEX:imp    X=X-1
99AD 10:FA      BPL:rel    Branch->$99A9
99AF A2:04      LDX:imm    #04		 ; Go through all the enemies.
99B1 BD:2E 01   LDA:abs,x  $012E,X	 ; Find out the max # of this type
					 ; of enemy allowed in the tunnel.
99B4 38:        SEC:imp    Set Carry	 ; There is one less of some type
99B5 FD:42 01   SBC:abs,x  $0142,X	 ; of enemy in the tunnel.
99B8 90:03      BCC:rel    Branch->$99BD
99BA 9D:3D 01   STA:abs,x  $013D,X
99BD CA:        DEX:imp    X=X-1
99BE 10:F1      BPL:rel    Branch->$99B1
99C0 AC:1C 01   LDY:abs    $011C
99C3 B9:DF 02   LDA:abs,y  $02DF,Y
99C6 F0:14      BEQ:rel    Branch->$99DC
99C8 B9:8A 02   LDA:abs,y  $028A,Y
99CB 29:03      AND:imm    #03
99CD F0:0D      BEQ:rel    Branch->$99DC
99CF AA:        TAX:imp    A-->X
99D0 E0:03      CPX:imm    #03
99D2 D0:02      BNE:rel    Branch->$99D6
99D4 A2:05      LDX:imm    #05
99D6 DE:3C 01   DEC:abs,x  $013C,X
99D9 DE:3C 01   DEC:abs,x  $013C,X
99DC 88:        DEY:imp    Y=Y-1
99DD 10:E4      BPL:rel    Branch->$99C3
99DF A2:04      LDX:imm    #04
99E1 AD:1C 01   LDA:abs    $011C
99E4 18:        CLC:imp    Clear Carry
99E5 69:01      ADC:imm    #01
99E7 38:        SEC:imp    Set Carry	 ; There is one less of some type
99E8 FD:42 01   SBC:abs,x  $0142,X	 ; of enemy in the tunnel.
99EB CA:        DEX:imp    X=X-1
99EC 10:F9      BPL:rel    Branch->$99E7
99EE A2:04      LDX:imm    #04
99F0 DD:3D 01   CMP:abs,x  $013D,X
99F3 B0:03      BCS:rel    Branch->$99F8
99F5 9D:3D 01   STA:abs,x  $013D,X
99F8 CA:        DEX:imp    X=X-1
99F9 10:F5      BPL:rel    Branch->$99F0
99FB A2:04      LDX:imm    #04
99FD A0:00      LDY:imm    #00
99FF BD:3D 01   LDA:abs,x  $013D,X
9A02 F0:01      BEQ:rel    Branch->$9A05
9A04 C8:        INY:imp    Y=Y+1
9A05 CA:        DEX:imp    X=X-1
9A06 10:F7      BPL:rel    Branch->$99FF
9A08 98:        TYA:imp    Y-->A
9A09 F0:77      BEQ:rel    Branch->$9A82
9A0B 88:        DEY:imp    Y=Y-1
9A0C D0:18      BNE:rel    Branch->$9A26
9A0E A2:04      LDX:imm    #04
9A10 BD:3D 01   LDA:abs,x  $013D,X
9A13 F0:0B      BEQ:rel    Branch->$9A20
9A15 BD:29 01   LDA:abs,x  $0129,X
9A18 F0:06      BEQ:rel    Branch->$9A20
9A1A 20:87 9A   JSR:abs    $9A87
9A1D F0:01      BEQ:rel    Branch->$9A20
9A1F 60:        RTS:imp    Ret from Sub
9A20 CA:        DEX:imp    X=X-1
9A21 10:ED      BPL:rel    Branch->$9A10
9A23 B8:        CLV:imp    Clear Overflow
9A24 50:5C      BVC:rel    Branch->$9A82
9A26 84:61      STY:zp     Zp RAM 0061
9A28 A2:04      LDX:imm    #04
9A2A BD:3D 01   LDA:abs,x  $013D,X
9A2D F0:0E      BEQ:rel    Branch->$9A3D
9A2F BD:42 01   LDA:abs,x  $0142,X	 ; How many of this type of enemy
					 ; is in the tunnel?
9A32 DD:29 01   CMP:abs,x  $0129,X	 ; Compare it to the ratio of this
					 ; type of enemy that is allowed.
9A35 B0:06      BCS:rel    Branch->$9A3D
9A37 20:87 9A   JSR:abs    $9A87
9A3A F0:01      BEQ:rel    Branch->$9A3D
9A3C 60:        RTS:imp    Ret from Sub
9A3D CA:        DEX:imp    X=X-1
9A3E 10:EA      BPL:rel    Branch->$9A2A
9A40 AD:40 01   LDA:abs    $0140
9A43 F0:1C      BEQ:rel    Branch->$9A61
9A45 AD:3F 01   LDA:abs    $013F
9A48 F0:17      BEQ:rel    Branch->$9A61
9A4A A4:2A      LDY:zp     Zp RAM 002A
9A4C B9:AC 03   LDA:abs,y  $03AC,Y
9A4F D0:02      BNE:rel    Branch->$9A53
9A51 A9:FF      LDA:imm    #FF
9A53 A2:03      LDX:imm    #03
9A55 C9:CC      CMP:imm    #CC
9A57 B0:02      BCS:rel    Branch->$9A5B
9A59 A2:02      LDX:imm    #02
9A5B 20:87 9A   JSR:abs    $9A87
9A5E F0:01      BEQ:rel    Branch->$9A61
9A60 60:        RTS:imp    Ret from Sub
9A61 AD:DA 60   LDA:abs    $60DA	 ; Grab a random number,
9A64 29:03      AND:imm    #03		 ; 0-3.
9A66 AA:        TAX:imp    A-->X
9A67 E8:        INX:imp    X=X+1
9A68 A0:04      LDY:imm    #04
9A6A BD:29 01   LDA:abs,x  $0129,X
9A6D F0:0B      BEQ:rel    Branch->$9A7A
9A6F BD:3D 01   LDA:abs,x  $013D,X
9A72 F0:06      BEQ:rel    Branch->$9A7A
9A74 20:87 9A   JSR:abs    $9A87
9A77 F0:01      BEQ:rel    Branch->$9A7A
9A79 60:        RTS:imp    Ret from Sub
9A7A CA:        DEX:imp    X=X-1
9A7B 10:02      BPL:rel    Branch->$9A7F
9A7D A2:04      LDX:imm    #04
9A7F 88:        DEY:imp    Y=Y-1
9A80 10:E8      BPL:rel    Branch->$9A6A
9A82 A9:00      LDA:imm    #00
9A84 85:29      STA:zp     Zp RAM 0029
9A86 60:        RTS:imp    Ret from Sub
;
9A87 8A:        TXA:imp    X-->A
9A88 0A:        ASL:accum  
9A89 A8:        TAY:imp    A-->Y
9A8A B9:94 9A   LDA:abs,y  $9A94,Y
9A8D 48:        PHA:imp    Push Accum
9A8E B9:93 9A   LDA:abs,y  $9A93,Y
9A91 48:        PHA:imp    Push Accum
9A92 60:        RTS:imp    Ret from Sub
;
; Start of a data segment, per Ken Lui. Length unknown.
; Will not label as data until more is known.
;
9A93 9C:        Illegal Opcode
9A94 9A:        TXS:imp    X-->Stack
9A95 A8:        TAY:imp    A-->Y
9A96 9A:        TXS:imp    X-->Stack
9A97 BA:        TSX:imp    Stack-->X
9A98 9A:        TXS:imp    X-->Stack
9A99 B6:9A      LDX:zp,y   Zp RAM 009A
9A9B B2:        Illegal Opcode
9A9C 9A:        TXS:imp    X-->Stack
9A9D AD:02 9B   LDA:abs    $9B02
9AA0 85:2C      STA:zp     Zp RAM 002C
9AA2 AD:5D 01   LDA:abs    $015D
9AA5 A0:00      LDY:imm    #00
9AA7 F0:4D      BEQ:rel    Branch->$9AF6
9AA9 AD:03 9B   LDA:abs    $9B03
9AAC 0D:6D 01   ORA:abs    $016D
9AAF A0:01      LDY:imm    #01
9AB1 D0:3E      BNE:rel    Branch->$9AF1
9AB3 A0:04      LDY:imm    #04
9AB5 D0:37      BNE:rel    Branch->$9AEE
9AB7 A0:03      LDY:imm    #03
9AB9 D0:33      BNE:rel    Branch->$9AEE
9ABB AD:CA 60   LDA:abs    $60CA	 ; Get a random number,
9ABE 29:03      AND:imm    #03		 ; 0-3.
9AC0 A8:        TAY:imp    A-->Y
9AC1 A9:04      LDA:imm    #04
9AC3 85:2B      STA:zp     Zp RAM 002B
9AC5 86:39      STX:zp     Zp RAM 0039
9AC7 C6:2B      DEC:zp     Zp RAM 002B
9AC9 10:05      BPL:rel    Branch->$9AD0
9ACB A6:39      LDX:zp     Zp RAM 0039
9ACD A9:00      LDA:imm    #00
9ACF 60:        RTS:imp    Ret from Sub
9AD0 88:        DEY:imp    Y=Y-1
9AD1 10:02      BPL:rel    Branch->$9AD5
9AD3 A0:03      LDY:imm    #03
9AD5 BE:49 01   LDX:abs,y  $0149,Y
9AD8 E0:03      CPX:imm    #03
9ADA D0:02      BNE:rel    Branch->$9ADE
9ADC A2:05      LDX:imm    #05
9ADE BD:3C 01   LDA:abs,x  $013C,X
9AE1 F0:E4      BEQ:rel    Branch->$9AC7
9AE3 A6:39      LDX:zp     Zp RAM 0039
9AE5 B9:49 01   LDA:abs,y  $0149,Y
9AE8 09:40      ORA:imm    #40
9AEA A0:02      LDY:imm    #02
9AEC D0:03      BNE:rel    Branch->$9AF1
9AEE B9:02 9B   LDA:abs,y  $9B02,Y
9AF1 85:2C      STA:zp     Zp RAM 002C
9AF3 B9:FD 9A   LDA:abs,y  $9AFD,Y
9AF6 84:2B      STY:zp     Zp RAM 002B
9AF8 85:2D      STA:zp     Zp RAM 002D
9AFA A5:29      LDA:zp     Zp RAM 0029
9AFC 60:        RTS:imp    Ret from Sub
9AFD 07:        Illegal Opcode
9AFE 72:        Illegal Opcode
9AFF 07:        Illegal Opcode
9B00 00:        BRK:imp    BREAK
9B01 61:40      ADC:ind,x  40
9B03 00:        BRK:imp    BREAK
9B04 41:40      EOR:ind,x  40
9B06 00:        BRK:imp    BREAK
9B07 84:36      STY:zp     Zp RAM 0036
9B09 A5:29      LDA:zp     Zp RAM 0029
9B0B C9:20      CMP:imm    #20
9B0D A5:2B      LDA:zp     Zp RAM 002B
9B0F B0:07      BCS:rel    Branch->$9B18
9B11 A8:        TAY:imp    A-->Y
9B12 20:EE 9A   JSR:abs    $9AEE
9B15 B8:        CLV:imp    Clear Overflow
9B16 50:03      BVC:rel    Branch->$9B1B
9B18 20:88 9A   JSR:abs    $9A88
9B1B A4:36      LDY:zp     Zp RAM 0036
9B1D 60:        RTS:imp    Ret from Sub
9B1E AD:01 02   LDA:abs    $0201
9B21 30:33      BMI:rel    Branch->$9B56
9B23 AE:1C 01   LDX:abs    $011C
9B26 86:37      STX:zp     Zp RAM 0037
9B28 A6:37      LDX:zp     Zp RAM 0037
9B2A BD:DF 02   LDA:abs,x  $02DF,X
9B2D F0:23      BEQ:rel    Branch->$9B52
9B2F A9:01      LDA:imm    #01
9B31 8D:0A 01   STA:abs    $010A
9B34 BD:91 02   LDA:abs,x  $0291,X
9B37 8D:0B 01   STA:abs    $010B
9B3A AD:0B 01   LDA:abs    $010B
9B3D A8:        TAY:imp    A-->Y
9B3E B9:F7 A0   LDA:abs,y  $A0F7,Y
9B41 20:98 9B   JSR:abs    $9B98
9B44 EE:0B 01   INC:abs    $010B
9B47 AD:0A 01   LDA:abs    $010A
9B4A D0:EE      BNE:rel    Branch->$9B3A
9B4C AD:0B 01   LDA:abs    $010B
9B4F 9D:91 02   STA:abs,x  $0291,X
9B52 C6:37      DEC:zp     Zp RAM 0037
9B54 10:D2      BPL:rel    Branch->$9B28
9B56 AD:48 01   LDA:abs    $0148
9B59 18:        CLC:imp    Clear Carry
9B5A 6D:47 01   ADC:abs    $0147
9B5D A8:        TAY:imp    A-->Y
9B5E 4D:48 01   EOR:abs    $0148
9B61 8C:48 01   STY:abs    $0148
9B64 10:16      BPL:rel    Branch->$9B7C
9B66 98:        TYA:imp    Y-->A
9B67 10:06      BPL:rel    Branch->$9B6F
9B69 20:06 CD   JSR:abs    $CD06
9B6C B8:        CLV:imp    Clear Overflow
9B6D 50:0D      BVC:rel    Branch->$9B7C
9B6F AD:43 01   LDA:abs    $0143
9B72 F0:08      BEQ:rel    Branch->$9B7C
9B74 AD:01 02   LDA:abs    $0201
9B77 30:03      BMI:rel    Branch->$9B7C
9B79 20:02 CD   JSR:abs    $CD02
9B7C AD:48 01   LDA:abs    $0148
9B7F 30:07      BMI:rel    Branch->$9B88
9B81 C9:0F      CMP:imm    #0F
9B83 B0:07      BCS:rel    Branch->$9B8C
9B85 B8:        CLV:imp    Clear Overflow
9B86 50:0F      BVC:rel    Branch->$9B97
9B88 C9:C1      CMP:imm    #C1
9B8A B0:0B      BCS:rel    Branch->$9B97
9B8C AD:47 01   LDA:abs    $0147
9B8F 49:FF      EOR:imm    #FF
9B91 18:        CLC:imp    Clear Carry
9B92 69:01      ADC:imm    #01
9B94 8D:47 01   STA:abs    $0147
9B97 60:        RTS:imp    Ret from Sub
9B98 A8:        TAY:imp    A-->Y
9B99 B9:A3 9B   LDA:abs,y  $9BA3,Y
9B9C 48:        PHA:imp    Push Accum
9B9D B9:A2 9B   LDA:abs,y  $9BA2,Y
9BA0 48:        PHA:imp    Push Accum
9BA1 60:        RTS:imp    Ret from Sub
;
; Data Segment, per Ken Lui.
;
9BA2 C9:9B      DATA
9BA4 CF:9B      DATA
9BA6 ED:9B 16   DATA
9BA9 9C:0B      DATA
9BAB 9C:CE      DATA
9BAD 9B:57      DATA
9BAF 9C:C3      DATA
9BB1 9F:DC      DATA
9BB3 9B:5B      DATA
9BB5 9E:81      DATA
9BB7 9D:4E      DATA
9BB9 9C:2E      DATA
9BBB 9E:F9      DATA
9BBD 9B:20      DATA
9BBF 9C:F0      DATA
9BC1 9E:47      DATA
9BC3 9E:B5      DATA
9BC5 9C:66      DATA
9BC7 9D:3A:9C   DATA
;
9BCA A9:00      LDA:imm    #00
9BCC 8D:0A 01   STA:abs    $010A
9BCF 60:        RTS:imp    Ret from Sub
9BD0 EE:0B 01   INC:abs    $010B
9BD3 AC:0B 01   LDY:abs    $010B
9BD6 B9:F7 A0   LDA:abs,y  $A0F7,Y
9BD9 9D:98 02   STA:abs,x  $0298,X
9BDC 60:        RTS:imp    Ret from Sub
9BDD EE:0B 01   INC:abs    $010B
9BE0 AC:0B 01   LDY:abs    $010B
9BE3 B9:F7 A0   LDA:abs,y  $A0F7,Y
9BE6 A8:        TAY:imp    A-->Y
9BE7 B9:00 00   LDA:abs,y  $0000,Y
9BEA 9D:98 02   STA:abs,x  $0298,X
9BED 60:        RTS:imp    Ret from Sub
9BEE AD:0C 01   LDA:abs    $010C
9BF1 D0:06      BNE:rel    Branch->$9BF9
9BF3 EE:0B 01   INC:abs    $010B
9BF6 EE:0B 01   INC:abs    $010B
9BF9 60:        RTS:imp    Ret from Sub
9BFA EE:0B 01   INC:abs    $010B
9BFD AD:0C 01   LDA:abs    $010C
9C00 D0:09      BNE:rel    Branch->$9C0B
9C02 AC:0B 01   LDY:abs    $010B
9C05 B9:F7 A0   LDA:abs,y  $A0F7,Y
9C08 8D:0B 01   STA:abs    $010B
9C0B 60:        RTS:imp    Ret from Sub
9C0C DE:98 02   DEC:abs,x  $0298,X
9C0F D0:06      BNE:rel    Branch->$9C17
9C11 EE:0B 01   INC:abs    $010B
9C14 B8:        CLV:imp    Clear Overflow
9C15 50:09      BVC:rel    Branch->$9C20
9C17 AC:0B 01   LDY:abs    $010B
9C1A B9:F8 A0   LDA:abs,y  $A0F8,Y
9C1D 8D:0B 01   STA:abs    $010B
9C20 60:        RTS:imp    Ret from Sub
9C21 BC:B9 02   LDY:abs,x  $02B9,X
9C24 B9:AC 03   LDA:abs,y  $03AC,Y
9C27 D0:02      BNE:rel    Branch->$9C2B
9C29 A9:FF      LDA:imm    #FF
9C2B DD:DF 02   CMP:abs,x  $02DF,X
9C2E B0:05      BCS:rel    Branch->$9C35
9C30 A9:00      LDA:imm    #00
9C32 B8:        CLV:imp    Clear Overflow
9C33 50:02      BVC:rel    Branch->$9C37
9C35 A9:01      LDA:imm    #01
9C37 8D:0C 01   STA:abs    $010C
9C3A 60:        RTS:imp    Ret from Sub
9C3B AD:47 01   LDA:abs    $0147
9C3E 0A:        ASL:accum  
9C3F 0A:        ASL:accum  
9C40 18:        CLC:imp    Clear Carry
9C41 6D:48 01   ADC:abs    $0148
9C44 2D:48 01   AND:abs    $0148
9C47 29:80      AND:imm    #80
9C49 49:80      EOR:imm    #80
9C4B 8D:0C 01   STA:abs    $010C
9C4E 60:        RTS:imp    Ret from Sub
;
9C4F BD:83 02   LDA:abs,x  $0283,X	 ; Get enemy movement style and
9C52 49:40      EOR:imm    #40		 ; set it to ??????
9C54 9D:83 02   STA:abs,x  $0283,X	 ;
9C57 60:        RTS:imp    Ret from Sub  ;
;
9C58 BD:83 02   LDA:abs,x  $0283,X	 ; Get enemy movement style and
9C5B 29:07      AND:imm    #07		 ; set it to upwards movement only.
9C5D A8:        TAY:imp    A-->Y
9C5E BD:8A 02   LDA:abs,x  $028A,X
9C61 30:36      BMI:rel    Branch->$9C99
9C63 BD:9F 02   LDA:abs,x  $029F,X
9C66 18:        CLC:imp    Clear Carry
9C67 79:60 01   ADC:abs,y  $0160,Y
9C6A 9D:9F 02   STA:abs,x  $029F,X
9C6D BD:DF 02   LDA:abs,x  $02DF,X	; Get the position of a tunnel enemy,
9C70 79:65 01   ADC:abs,y  $0165,Y	; and move them *UP* by their
9C73 9D:DF 02   STA:abs,x  $02DF,X	; pretermined movement rate.
9C76 CD:02 02   CMP:abs    $0202	; Enemy at the top of the tunnel?
9C79 F0:02      BEQ:rel    Branch->$9C7D
9C7B B0:06      BCS:rel    Branch->$9C83
9C7D 20:06 9D   JSR:abs    $9D06
9C80 B8:        CLV:imp    Clear Overflow
9C81 50:13      BVC:rel    Branch->$9C96
9C83 C9:20      CMP:imm    #20
9C85 B0:0F      BCS:rel    Branch->$9C96
9C87 BD:8A 02   LDA:abs,x  $028A,X
9C8A 29:03      AND:imm    #03
9C8C F0:08      BEQ:rel    Branch->$9C96
9C8E 8A:        TXA:imp    X-->A
9C8F 48:        PHA:imp    Push Accum
9C90 A8:        TAY:imp    A-->Y
9C91 20:6F A0   JSR:abs    $A06F
9C94 68:        PLA:imp    Pull Accum
9C95 AA:        TAX:imp    A-->X
9C96 B8:        CLV:imp    Clear Overflow
9C97 50:1C      BVC:rel    Branch->$9CB5
9C99 BD:9F 02   LDA:abs,x  $029F,X
9C9C 38:        SEC:imp    Set Carry
9C9D F9:60 01   SBC:abs,y  $0160,Y
9CA0 9D:9F 02   STA:abs,x  $029F,X
9CA3 BD:DF 02   LDA:abs,x  $02DF,X	 ; Get the Y pos of an enemy,
9CA6 F9:65 01   SBC:abs,y  $0165,Y	 ; and move them *UP* by their
9CA9 9D:DF 02   STA:abs,x  $02DF,X	 ; predetermined movement rate.
9CAC C9:F0      CMP:imm    #F0
9CAE 90:05      BCC:rel    Branch->$9CB5
9CB0 A9:F2      LDA:imm    #F2
9CB2 9D:DF 02   STA:abs,x  $02DF,X
9CB5 60:        RTS:imp    Ret from Sub

9CB6 A0:01      LDY:imm    #01
9CB8 BD:8A 02   LDA:abs,x  $028A,X
9CBB 30:10      BMI:rel    Branch->$9CCD
9CBD BD:DF 02   LDA:abs,x  $02DF,X
9CC0 CD:57 01   CMP:abs    $0157
9CC3 90:02      BCC:rel    Branch->$9CC7
9CC5 A0:00      LDY:imm    #00
9CC7 20:63 9C   JSR:abs    $9C63
9CCA B8:        CLV:imp    Clear Overflow
9CCB 50:17      BVC:rel    Branch->$9CE4
9CCD 20:99 9C   JSR:abs    $9C99
9CD0 AC:AB 03   LDY:abs    $03AB         ; Are the enemies waiting at the
9CD3 D0:02      BNE:rel    Branch->$9CD7 : bottom? If so, skip ahead a bit.
9CD5 A9:FF      LDA:imm    #FF
9CD7 CD:57 01   CMP:abs    $0157
9CDA 90:08      BCC:rel    Branch->$9CE4
9CDC BD:8A 02   LDA:abs,x  $028A,X
9CDF 49:80      EOR:imm    #80
9CE1 9D:8A 02   STA:abs,x  $028A,X
9CE4 AD:48 01   LDA:abs    $0148
9CE7 30:1B      BMI:rel    Branch->$9D04
9CE9 BD:DF 02   LDA:abs,x  $02DF,X
9CEC CD:57 01   CMP:abs    $0157
9CEF B0:13      BCS:rel    Branch->$9D04
9CF1 AD:00 02   LDA:abs    $0200
9CF4 DD:B9 02   CMP:abs,x  $02B9,X
9CF7 D0:0B      BNE:rel    Branch->$9D04
9CF9 AD:01 02   LDA:abs    $0201
9CFC DD:CC 02   CMP:abs,x  $02CC,X
9CFF D0:03      BNE:rel    Branch->$9D04
9D01 20:47 A3   JSR:abs    $A347
9D04 60:        RTS:imp    Ret from Sub
;
9D05 16:AD      ASL:zp,x   Zp RAM 00AD
9D07 02:        Illegal Opcode
9D08 02:        Illegal Opcode
9D09 9D:DF 02   STA:abs,x  $02DF,X
9D0C BD:83 02   LDA:abs,x  $0283,X	 ; Look at this enemy. Is it a
9D0F 29:07      AND:imm    #07		 ; Pulsar?
9D11 C9:01      CMP:imm    #01		 ;
9D13 D0:0E      BNE:rel    Branch->$9D23
9D15 AD:AB 03   LDA:abs    $03AB         ; If no more enemies waiting at the
9D18 F0:09      BEQ:rel    Branch->$9D23 : bottom, skip ahead.
9D1A BD:8A 02   LDA:abs,x  $028A,X
9D1D 49:80      EOR:imm    #80
9D1F 9D:8A 02   STA:abs,x  $028A,X
9D22 60:        RTS:imp    Ret from Sub
9D23 BD:83 02   LDA:abs,x  $0283,X
9D26 10:04      BPL:rel    Branch->$9D2C
9D28 FE:DF 02   INC:abs,x  $02DF,X	 ; Move this enemy upwards.
9D2B 60:        RTS:imp    Ret from Sub
9D2C CE:08 01   DEC:abs    $0108	 ; One less enemy INSIDE the tube.
9D2F AD:09 01   LDA:abs    $0109	 ; Are the any enemies at the
9D32 C9:01      CMP:imm    #01		 ; top of the tube?
9D34 F0:06      BEQ:rel    Branch->$9D3C
9D36 20:67 9D   JSR:abs    $9D67
9D39 B8:        CLV:imp    Clear Overflow
9D3A 50:22      BVC:rel    Branch->$9D5E
9D3C A0:06      LDY:imm    #06
9D3E B9:DF 02   LDA:abs,y  $02DF,Y
9D41 F0:0E      BEQ:rel    Branch->$9D51
9D43 84:38      STY:zp     Zp RAM 0038
9D45 E4:38      CPX:zp     Zp RAM 0038
9D47 F0:08      BEQ:rel    Branch->$9D51
9D49 B9:DF 02   LDA:abs,y  $02DF,Y	 ; Get the Y position of an enemy.
9D4C CD:02 02   CMP:abs    $0202	 ; Are they at the top of the tunnel?
9D4F F0:03      BEQ:rel    Branch->$9D54
9D51 88:        DEY:imp    Y=Y-1
9D52 10:EA      BPL:rel    Branch->$9D3E
9D54 B9:83 02   LDA:abs,y  $0283,Y	 ; If they are at the top,
9D57 29:40      AND:imm    #40		 ; change their movement style
9D59 49:40      EOR:imm    #40		 ; to ____.
9D5B 9D:83 02   STA:abs,x  $0283,X	 ;
9D5E A9:41      LDA:imm    #41
9D60 8D:0B 01   STA:abs    $010B
9D63 EE:09 01   INC:abs    $0109	 ; One more enemy at the tube top.
9D66 60:        RTS:imp    Ret from Sub
9D67 BD:B9 02   LDA:abs,x  $02B9,X
9D6A A8:        TAY:imp    A-->Y
9D6B AD:00 02   LDA:abs    $0200
9D6E 20:A6 A7   JSR:abs    $A7A6
9D71 0A:        ASL:accum  
9D72 BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9D75 B0:05      BCS:rel    Branch->$9D7C
9D77 09:40      ORA:imm    #40
9D79 B8:        CLV:imp    Clear Overflow
9D7A 50:02      BVC:rel    Branch->$9D7E
9D7C 29:BF      AND:imm    #BF
9D7E 9D:83 02   STA:abs,x  $0283,X	 ; Save enemy type and movement.
9D81 60:        RTS:imp    Ret from Sub
9D82 BC:CC 02   LDY:abs,x  $02CC,X
9D85 BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9D88 29:40      AND:imm    #40
9D8A D0:04      BNE:rel    Branch->$9D90
9D8C C8:        INY:imp    Y=Y+1
9D8D B8:        CLV:imp    Clear Overflow
9D8E 50:01      BVC:rel    Branch->$9D91
9D90 88:        DEY:imp    Y=Y-1
9D91 98:        TYA:imp    Y-->A
9D92 29:0F      AND:imm    #0F
9D94 09:80      ORA:imm    #80
9D96 9D:CC 02   STA:abs,x  $02CC,X
9D99 BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9D9C 29:07      AND:imm    #07
9D9E C9:04      CMP:imm    #04
9DA0 D0:4C      BNE:rel    Branch->$9DEE
9DA2 BD:CC 02   LDA:abs,x  $02CC,X
9DA5 29:07      AND:imm    #07
9DA7 D0:42      BNE:rel    Branch->$9DEB
9DA9 BD:CC 02   LDA:abs,x  $02CC,X
9DAC 29:08      AND:imm    #08
9DAE F0:0B      BEQ:rel    Branch->$9DBB
9DB0 BD:B9 02   LDA:abs,x  $02B9,X
9DB3 18:        CLC:imp    Clear Carry
9DB4 69:01      ADC:imm    #01
9DB6 29:0F      AND:imm    #0F
9DB8 9D:B9 02   STA:abs,x  $02B9,X
9DBB BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9DBE 29:7F      AND:imm    #7F		 ; Strip away any clockwise movement.
9DC0 9D:83 02   STA:abs,x  $0283,X	 ; Store enemy type and movement.
9DC3 A9:20      LDA:imm    #20
9DC5 9D:CC 02   STA:abs,x  $02CC,X
9DC8 BD:8A 02   LDA:abs,x  $028A,X
9DCB 49:80      EOR:imm    #80
9DCD 9D:8A 02   STA:abs,x  $028A,X
9DD0 AD:AB 03   LDA:abs    $03AB         ; If no more enemies waiting at the
9DD3 D0:16      BNE:rel    Branch->$9DEB ; bottom, skip ahead.
9DD5 BD:DF 02   LDA:abs,x  $02DF,X	 ; Is the curent enemy at the
9DD8 CD:02 02   CMP:abs    $0202	 ; very top?
9DDB D0:06      BNE:rel    Branch->$9DE3
9DDD 20:81 9F   JSR:abs    $9F81
9DE0 B8:        CLV:imp    Clear Overflow
9DE1 50:08      BVC:rel    Branch->$9DEB
9DE3 BD:8A 02   LDA:abs,x  $028A,X	 ; Stop vertical movement for
9DE6 29:80      AND:imm    #80		 ; this particular enemy? (Because
9DE8 9D:8A 02   STA:abs,x  $028A,X	 ; they are at the top.)
9DEB B8:        CLV:imp    Clear Overflow
9DEC 50:38      BVC:rel    Branch->$9E26
9DEE BC:B9 02   LDY:abs,x  $02B9,X
9DF1 BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9DF4 49:40      EOR:imm    #40
9DF6 20:D7 9E   JSR:abs    $9ED7
9DF9 DD:CC 02   CMP:abs,x  $02CC,X
9DFC D0:28      BNE:rel    Branch->$9E26
9DFE BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9E01 29:7F      AND:imm    #7F		 ; Strip away clockwise movement.
9E03 9D:83 02   STA:abs,x  $0283,X	 ; Store enemy type and movement.
9E06 29:40      AND:imm    #40
9E08 D0:11      BNE:rel    Branch->$9E1B
9E0A BD:B9 02   LDA:abs,x  $02B9,X
9E0D 9D:CC 02   STA:abs,x  $02CC,X
9E10 38:        SEC:imp    Set Carry
9E11 E9:01      SBC:imm    #01
9E13 29:0F      AND:imm    #0F
9E15 9D:B9 02   STA:abs,x  $02B9,X
9E18 B8:        CLV:imp    Clear Overflow
9E19 50:0B      BVC:rel    Branch->$9E26
9E1B BD:B9 02   LDA:abs,x  $02B9,X
9E1E 18:        CLC:imp    Clear Carry
9E1F 69:01      ADC:imm    #01
9E21 29:0F      AND:imm    #0F
9E23 9D:CC 02   STA:abs,x  $02CC,X
9E26 BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9E29 29:80      AND:imm    #80	
9E2B 8D:0C 01   STA:abs    $010C
9E2E 60:        RTS:imp    Ret from Sub
9E2F BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9E32 30:13      BMI:rel    Branch->$9E47
9E34 BD:B9 02   LDA:abs,x  $02B9,X
9E37 CD:00 02   CMP:abs    $0200
9E3A D0:0B      BNE:rel    Branch->$9E47
9E3C BD:CC 02   LDA:abs,x  $02CC,X
9E3F CD:01 02   CMP:abs    $0201
9E42 D0:03      BNE:rel    Branch->$9E47
9E44 20:3A A3   JSR:abs    $A33A
9E47 60:        RTS:imp    Ret from Sub
9E48 BD:DF 02   LDA:abs,x  $02DF,X
9E4B CD:02 02   CMP:abs    $0202
9E4E D0:0B      BNE:rel    Branch->$9E5B
9E50 BD:B9 02   LDA:abs,x  $02B9,X
9E53 CD:00 02   CMP:abs    $0200
9E56 D0:03      BNE:rel    Branch->$9E5B
9E58 20:43 A3   JSR:abs    $A343
9E5B 60:        RTS:imp    Ret from Sub
9E5C 20:AB 9E   JSR:abs    $9EAB
9E5F BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9E62 09:80      ORA:imm    #80		 ; Set clockwise movement.
9E64 9D:83 02   STA:abs,x  $0283,X	 ; Store enemy type and movement.
9E67 29:07      AND:imm    #07		 ; Is this enemy a
9E69 C9:04      CMP:imm    #04		 ; fuseball?
9E6B D0:1F      BNE:rel    Branch->$9E8C
9E6D BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9E70 29:40      AND:imm    #40
9E72 D0:05      BNE:rel    Branch->$9E79
9E74 A9:81      LDA:imm    #81
9E76 B8:        CLV:imp    Clear Overflow
9E77 50:0D      BVC:rel    Branch->$9E86
9E79 BD:B9 02   LDA:abs,x  $02B9,X
9E7C 38:        SEC:imp    Set Carry
9E7D E9:01      SBC:imm    #01
9E7F 29:0F      AND:imm    #0F
9E81 9D:B9 02   STA:abs,x  $02B9,X
9E84 A9:87      LDA:imm    #87
9E86 9D:CC 02   STA:abs,x  $02CC,X
9E89 B8:        CLV:imp    Clear Overflow
9E8A 50:1E      BVC:rel    Branch->$9EAA
9E8C BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9E8F 29:40      AND:imm    #40
9E91 F0:0B      BEQ:rel    Branch->$9E9E
9E93 BD:B9 02   LDA:abs,x  $02B9,X
9E96 18:        CLC:imp    Clear Carry
9E97 69:01      ADC:imm    #01
9E99 29:0F      AND:imm    #0F
9E9B 9D:B9 02   STA:abs,x  $02B9,X
9E9E BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9EA1 BC:B9 02   LDY:abs,x  $02B9,X
9EA4 20:D7 9E   JSR:abs    $9ED7
9EA7 9D:CC 02   STA:abs,x  $02CC,X
9EAA 60:        RTS:imp    Ret from Sub
9EAB AD:11 01   LDA:abs    $0111
9EAE F0:26      BEQ:rel    Branch->$9ED6
9EB0 BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9EB3 29:40      AND:imm    #40
9EB5 F0:12      BEQ:rel    Branch->$9EC9
9EB7 BD:B9 02   LDA:abs,x  $02B9,X
9EBA C9:0E      CMP:imm    #0E
9EBC 90:08      BCC:rel    Branch->$9EC6
9EBE BD:83 02   LDA:abs,x  $0283,X	 ; Load enemy type and movement.
9EC1 29:BF      AND:imm    #BF
9EC3 9D:83 02   STA:abs,x  $0283,X	 ; Store enemy type and movement.
9EC6 B8:        CLV:imp    Clear Overflow
9EC7 50:0D      BVC:rel    Branch->$9ED6
9EC9 BD:B9 02   LDA:abs,x  $02B9,X
9ECC D0:08      BNE:rel    Branch->$9ED6
9ECE BD:83 02   LDA:abs,x  $0283,X
9ED1 09:40      ORA:imm    #40
9ED3 9D:83 02   STA:abs,x  $0283,X
9ED6 60:        RTS:imp    Ret from Sub
9ED7 29:40      AND:imm    #40
9ED9 F0:10      BEQ:rel    Branch->$9EEB
9EDB 88:        DEY:imp    Y=Y-1
9EDC 98:        TYA:imp    Y-->A
9EDD 29:0F      AND:imm    #0F
9EDF A8:        TAY:imp    A-->Y
9EE0 B9:EE 03   LDA:abs,y  $03EE,Y
9EE3 18:        CLC:imp    Clear Carry
9EE4 69:08      ADC:imm    #08
9EE6 29:0F      AND:imm    #0F
9EE8 B8:        CLV:imp    Clear Overflow
9EE9 50:03      BVC:rel    Branch->$9EEE
9EEB B9:EE 03   LDA:abs,y  $03EE,Y
9EEE 09:80      ORA:imm    #80
9EF0 60:        RTS:imp    Ret from Sub
9EF1 A0:04      LDY:imm    #04
9EF3 BD:8A 02   LDA:abs,x  $028A,X
9EF6 30:4B      BMI:rel    Branch->$9F43
9EF8 BD:9F 02   LDA:abs,x  $029F,X
9EFB 18:        CLC:imp    Clear Carry
9EFC 6D:64 01   ADC:abs    $0164
9EFF 9D:9F 02   STA:abs,x  $029F,X
9F02 BD:DF 02   LDA:abs,x  $02DF,X
9F05 6D:69 01   ADC:abs    $0169
9F08 9D:DF 02   STA:abs,x  $02DF,X
9F0B CD:02 02   CMP:abs    $0202
9F0E B0:09      BCS:rel    Branch->$9F19
9F10 AD:02 02   LDA:abs    $0202
9F13 9D:DF 02   STA:abs,x  $02DF,X
9F16 B8:        CLV:imp    Clear Overflow
9F17 50:11      BVC:rel    Branch->$9F2A
9F19 AC:AB 03   LDY:abs    $03AB         ; If no more enemies waiting at
9F1C F0:0B      BEQ:rel    Branch->$9F29 ; the bottom, skip ahead.
9F1E A4:9F      LDY:zp     Zp RAM 009F	 ; Are we at or beyond the RED 
9F20 C0:11      CPY:imm    #11           ; LEVEL, just after the green?
9F22 B0:02      BCS:rel    Branch->$9F26
9F24 C9:20      CMP:imm    #20
9F26 B8:        CLV:imp    Clear Overflow
9F27 50:01      BVC:rel    Branch->$9F2A
9F29 60:        RTS:imp    Ret from Sub
9F2A B0:11      BCS:rel    Branch->$9F3D
9F2C AD:59 01   LDA:abs    $0159
9F2F 10:06      BPL:rel    Branch->$9F37
9F31 20:81 9F   JSR:abs    $9F81
9F34 B8:        CLV:imp    Clear Overflow
9F35 50:03      BVC:rel    Branch->$9F3A
9F37 20:8A 9F   JSR:abs    $9F8A
9F3A B8:        CLV:imp    Clear Overflow
9F3B 50:03      BVC:rel    Branch->$9F40
9F3D 20:5F 9F   JSR:abs    $9F5F
9F40 B8:        CLV:imp    Clear Overflow
9F41 50:1B      BVC:rel    Branch->$9F5E
9F43 20:99 9C   JSR:abs    $9C99
9F46 C9:80      CMP:imm    #80
9F48 90:11      BCC:rel    Branch->$9F5B
9F4A 2C:59 01   BIT:abs    $0159
9F4D 50:06      BVC:rel    Branch->$9F55
9F4F 20:81 9F   JSR:abs    $9F81
9F52 B8:        CLV:imp    Clear Overflow
9F53 50:03      BVC:rel    Branch->$9F58
9F55 20:8A 9F   JSR:abs    $9F8A
9F58 B8:        CLV:imp    Clear Overflow
9F59 50:03      BVC:rel    Branch->$9F5E
9F5B 20:5F 9F   JSR:abs    $9F5F
9F5E 60:        RTS:imp    Ret from Sub
9F5F BD:DF 02   LDA:abs,x  $02DF,X
9F62 29:20      AND:imm    #20
9F64 F0:1A      BEQ:rel    Branch->$9F80
9F66 AD:DA 60   LDA:abs    $60DA	 ; Grab a random number
9F69 CD:5F 01   CMP:abs    $015F
9F6C 90:12      BCC:rel    Branch->$9F80
9F6E 2C:59 01   BIT:abs    $0159
9F71 50:0A      BVC:rel    Branch->$9F7D
9F73 8A:        TXA:imp    X-->A
9F74 4A:        LSR:accum  
9F75 90:13      BCC:rel    Branch->$9F8A
9F77 20:81 9F   JSR:abs    $9F81
9F7A B8:        CLV:imp    Clear Overflow
9F7B 50:03      BVC:rel    Branch->$9F80
9F7D 20:8A 9F   JSR:abs    $9F8A
9F80 60:        RTS:imp    Ret from Sub
9F81 20:67 9D   JSR:abs    $9D67
9F84 20:4F 9C   JSR:abs    $9C4F
9F87 4C:99 9F   JMP:abs    $9F99
9F8A BD:83 02   LDA:abs,x  $0283,X
9F8D 29:BF      AND:imm    #BF
9F8F 2C:CA 60   BIT:abs    $60CA
9F92 50:02      BVC:rel    Branch->$9F96
9F94 09:40      ORA:imm    #40
9F96 9D:83 02   STA:abs,x  $0283,X
9F99 AD:11 01   LDA:abs    $0111
9F9C F0:1E      BEQ:rel    Branch->$9FBC
9F9E BD:83 02   LDA:abs,x  $0283,X
9FA1 29:40      AND:imm    #40
9FA3 D0:0A      BNE:rel    Branch->$9FAF
9FA5 BD:B9 02   LDA:abs,x  $02B9,X
9FA8 C9:0F      CMP:imm    #0F
9FAA B0:08      BCS:rel    Branch->$9FB4
9FAC B8:        CLV:imp    Clear Overflow
9FAD 50:0D      BVC:rel    Branch->$9FBC
9FAF BD:B9 02   LDA:abs,x  $02B9,X
9FB2 D0:08      BNE:rel    Branch->$9FBC
9FB4 BD:83 02   LDA:abs,x  $0283,X
9FB7 49:40      EOR:imm    #40
9FB9 9D:83 02   STA:abs,x  $0283,X
9FBC A9:66      LDA:imm    #66
9FBE 8D:0B 01   STA:abs    $010B
9FC1 4C:5F 9E   JMP:abs    $9E5F
9FC4 A9:01      LDA:imm    #01
9FC6 8D:0C 01   STA:abs    $010C
9FC9 BC:B9 02   LDY:abs,x  $02B9,X
9FCC B9:AC 03   LDA:abs,y  $03AC,Y
9FCF D0:05      BNE:rel    Branch->$9FD6
9FD1 A9:F1      LDA:imm    #F1
9FD3 99:AC 03   STA:abs,y  $03AC,Y
9FD6 BD:DF 02   LDA:abs,x  $02DF,X
9FD9 D9:AC 03   CMP:abs,y  $03AC,Y
9FDC B0:08      BCS:rel    Branch->$9FE6
9FDE 99:AC 03   STA:abs,y  $03AC,Y
9FE1 A9:80      LDA:imm    #80
9FE3 99:9A 03   STA:abs,y  $039A,Y
9FE6 BD:DF 02   LDA:abs,x  $02DF,X
9FE9 C9:20      CMP:imm    #20
9FEB B0:10      BCS:rel    Branch->$9FFD
9FED BD:8A 02   LDA:abs,x  $028A,X
9FF0 09:80      ORA:imm    #80
9FF2 9D:8A 02   STA:abs,x  $028A,X
9FF5 A9:20      LDA:imm    #20
9FF7 9D:DF 02   STA:abs,x  $02DF,X
9FFA B8:        CLV:imp    Clear Overflow
9FFB 50:2A      BVC:rel    Branch->$A027
9FFD C9:F2      CMP:imm    #F2
;
; Start of ROM 136002.115 at $A000.
;
9FFF 90:26      BCC:rel    Branch->$A027
A001 20:28 A0   JSR:abs    $A028
A004 A9:F0      LDA:imm    #F0
A006 9D:DF 02   STA:abs,x  $02DF,X
A009 AD:AB 03   LDA:abs    $03AB         ; If enemies waiting at the bottom,
A00C D0:19      BNE:rel    Branch->$A027 ; skip ahead.
A00E BD:8A 02   LDA:abs,x  $028A,X
A011 29:FC      AND:imm    #FC
A013 09:01      ORA:imm    #01
A015 9D:8A 02   STA:abs,x  $028A,X
A018 BD:83 02   LDA:abs,x  $0283,X
A01B 29:F8      AND:imm    #F8
A01D 09:02      ORA:imm    #02
A01F 9D:83 02   STA:abs,x  $0283,X
A022 A9:00      LDA:imm    #00
A024 8D:0C 01   STA:abs    $010C
A027 60:        RTS:imp    Ret from Sub
A028 A9:00      LDA:imm    #00
A02A 85:2D      STA:zp     Zp RAM 002D
A02C A9:0F      LDA:imm    #0F
A02E 8D:40 01   STA:abs    $0140
A031 AD:DA 60   LDA:abs    $60DA	 ; Grab a random number,
A034 29:0F      AND:imm    #0F		 ; 0-F.
A036 A8:        TAY:imp    A-->Y
A037 C0:0F      CPY:imm    #0F
A039 D0:05      BNE:rel    Branch->$A040
A03B AD:11 01   LDA:abs    $0111
A03E D0:0F      BNE:rel    Branch->$A04F
A040 B9:AC 03   LDA:abs,y  $03AC,Y
A043 D0:02      BNE:rel    Branch->$A047
A045 A9:FF      LDA:imm    #FF
A047 C5:2D      CMP:zp     Zp RAM 002D
A049 90:04      BCC:rel    Branch->$A04F
A04B 85:2D      STA:zp     Zp RAM 002D
A04D 84:29      STY:zp     Zp RAM 0029
A04F 88:        DEY:imp    Y=Y-1
A050 10:02      BPL:rel    Branch->$A054
A052 A0:0F      LDY:imm    #0F
A054 CE:40 01   DEC:abs    $0140
A057 10:DE      BPL:rel    Branch->$A037
A059 A5:29      LDA:zp     Zp RAM 0029
A05B 9D:B9 02   STA:abs,x  $02B9,X
A05E 18:        CLC:imp    Clear Carry
A05F 69:01      ADC:imm    #01
A061 29:0F      AND:imm    #0F
A063 9D:CC 02   STA:abs,x  $02CC,X
A066 BD:8A 02   LDA:abs,x  $028A,X
A069 29:7F      AND:imm    #7F
A06B 9D:8A 02   STA:abs,x  $028A,X
A06E 60:        RTS:imp    Ret from Sub
A06F B9:DF 02   LDA:abs,y  $02DF,Y
A072 85:29      STA:zp     Zp RAM 0029
A074 CD:02 02   CMP:abs    $0202
A077 D0:0F      BNE:rel    Branch->$A088
A079 B9:83 02   LDA:abs,y  $0283,Y
A07C 29:07      AND:imm    #07
A07E C9:04      CMP:imm    #04
A080 F0:06      BEQ:rel    Branch->$A088
A082 CE:09 01   DEC:abs    $0109	 ; One less enemy at the tube top.
A085 B8:        CLV:imp    Clear Overflow
A086 50:03      BVC:rel    Branch->$A08B
A088 CE:08 01   DEC:abs    $0108	 ; One less enemy INSIDE the tube.
A08B A9:00      LDA:imm    #00
A08D 99:DF 02   STA:abs,y  $02DF,Y
A090 B9:83 02   LDA:abs,y  $0283,Y
A093 29:07      AND:imm    #07
A095 86:35      STX:zp     Zp RAM 0035
A097 AA:        TAX:imp    A-->X
A098 DE:42 01   DEC:abs,x  $0142,X	 ; One less of this type of enemy
					 ; is in the tunnel.
A09B A6:35      LDX:zp     Zp RAM 0035
A09D B9:8A 02   LDA:abs,y  $028A,Y
A0A0 29:03      AND:imm    #03
A0A2 F0:52      BEQ:rel    Branch->$A0F6
A0A4 38:        SEC:imp    Set Carry
A0A5 E9:01      SBC:imm    #01
A0A7 C9:02      CMP:imm    #02
A0A9 D0:02      BNE:rel    Branch->$A0AD
A0AB A9:04      LDA:imm    #04
A0AD 85:2B      STA:zp     Zp RAM 002B
A0AF B9:B9 02   LDA:abs,y  $02B9,Y
A0B2 38:        SEC:imp    Set Carry
A0B3 E9:01      SBC:imm    #01
A0B5 29:0F      AND:imm    #0F
A0B7 C9:0F      CMP:imm    #0F
A0B9 90:07      BCC:rel    Branch->$A0C2
A0BB 2C:11 01   BIT:abs    $0111
A0BE 10:02      BPL:rel    Branch->$A0C2
A0C0 A9:00      LDA:imm    #00
A0C2 85:2A      STA:zp     Zp RAM 002A
A0C4 20:07 9B   JSR:abs    $9B07
A0C7 A5:2D      LDA:zp     Zp RAM 002D
A0C9 8D:0B 01   STA:abs    $010B
A0CC CE:0B 01   DEC:abs    $010B
A0CF A9:00      LDA:imm    #00
A0D1 8D:0A 01   STA:abs    $010A
A0D4 20:4D 99   JSR:abs    $994D
A0D7 F0:1D      BEQ:rel    Branch->$A0F6
A0D9 A5:2A      LDA:zp     Zp RAM 002A
A0DB 18:        CLC:imp    Clear Carry
A0DC 69:02      ADC:imm    #02
A0DE 29:0F      AND:imm    #0F
A0E0 C9:0F      CMP:imm    #0F
A0E2 D0:07      BNE:rel    Branch->$A0EB
A0E4 2C:11 01   BIT:abs    $0111
A0E7 10:02      BPL:rel    Branch->$A0EB
A0E9 A9:0E      LDA:imm    #0E
A0EB 85:2A      STA:zp     Zp RAM 002A
A0ED A5:2B      LDA:zp     Zp RAM 002B
A0EF 09:40      ORA:imm    #40
A0F1 85:2B      STA:zp     Zp RAM 002B
A0F3 20:4D 99   JSR:abs    $994D
A0F6 60:        RTS:imp    Ret from Sub
A0F7 0C:        Illegal Opcode
A0F8 0E:1A 06   ASL:abs    $061A
A0FB 00:        BRK:imp    BREAK
A0FC 06:FF      ASL:zp     Zp RAM 00FF
A0FE 0C:        Illegal Opcode
A0FF 00:        BRK:imp    BREAK
A100 06:06      ASL:zp     Zp RAM 0006
A102 02:        Illegal Opcode
A103 08:        PHP:imp    Push P
A104 0C:        Illegal Opcode
A105 00:        BRK:imp    BREAK
A106 08:        PHP:imp    Push P
A107 0C:        Illegal Opcode
A108 12:        Illegal Opcode
A109 00:        BRK:imp    BREAK
A10A 14:        Illegal Opcode
A10B 04:        Illegal Opcode
A10C 06:11      ASL:zp     Zp RAM 0011
A10E 06:0A      ASL:zp     Zp RAM 000A
A110 0C:        Illegal Opcode
A111 00:        BRK:imp    BREAK
A112 12:        Illegal Opcode
A113 00:        BRK:imp    BREAK
A114 14:        Illegal Opcode
A115 0C:        Illegal Opcode
A116 04:        Illegal Opcode
A117 06:1B      ASL:zp     Zp RAM 001B
A119 06:18      ASL:zp     Zp RAM 0018
A11B 0C:        Illegal Opcode
A11C 00:        BRK:imp    BREAK
A11D 02:        Illegal Opcode
A11E 02:        Illegal Opcode
A11F 12:        Illegal Opcode
A120 00:        BRK:imp    BREAK
A121 14:        Illegal Opcode
A122 0C:        Illegal Opcode
A123 04:        Illegal Opcode
A124 06:28      ASL:zp     Zp RAM 0028
A126 00:        BRK:imp    BREAK
A127 08:        PHP:imp    Push P
A128 27:        Illegal Opcode
A129 16:02      ASL:zp,x   Zp RAM 0002
A12B 03:        Illegal Opcode
A12C 12:        Illegal Opcode
A12D 00:        BRK:imp    BREAK
A12E 14:        Illegal Opcode
A12F 0C:        Illegal Opcode
A130 04:        Illegal Opcode
A131 06:35      ASL:zp     Zp RAM 0035
A133 00:        BRK:imp    BREAK
A134 08:        PHP:imp    Push P
A135 34:        Illegal Opcode
A136 16:06      ASL:zp,x   Zp RAM 0006
A138 23:        Illegal Opcode
A139 02:        Illegal Opcode
A13A 04:        Illegal Opcode
A13B 18:        CLC:imp    Clear Carry
A13C 00:        BRK:imp    BREAK
A13D 08:        PHP:imp    Push P
A13E 43:        Illegal Opcode
A13F 12:        Illegal Opcode
A140 00:        BRK:imp    BREAK
A141 10:B3      BPL:rel    Branch->$A0F6
A143 14:        Illegal Opcode
A144 1A:        Illegal Opcode
A145 41:08      EOR:ind,x  08
A147 4B:        Illegal Opcode
A148 06:48      ASL:zp     Zp RAM 0048
A14A 00:        BRK:imp    BREAK
A14B 0C:        Illegal Opcode
A14C 1C:        Illegal Opcode
A14D 1A:        Illegal Opcode
A14E 52:        Illegal Opcode
A14F 12:        Illegal Opcode
A150 00:        BRK:imp    BREAK
A151 0C:        Illegal Opcode
A152 14:        Illegal Opcode
A153 1A:        Illegal Opcode
A154 52:        Illegal Opcode
A155 00:        BRK:imp    BREAK
A156 06:5A      ASL:zp     Zp RAM 005A
A158 1E:20 00   ASL:abs,x  $0020,X
A15B 06:60      ASL:zp     Zp RAM 0060
A15D 00:        BRK:imp    BREAK
A15E 02:        Illegal Opcode
A15F 03:        Illegal Opcode
A160 20:00 08   JSR:abs    $0800
A163 68:        PLA:imp    Pull Accum
A164 14:        Illegal Opcode
A165 1A:        Illegal Opcode
A166 60:        RTS:imp    Ret from Sub
A167 06:65      ASL:zp     Zp RAM 0065
A169 10:B2      BPL:rel    Branch->$A11D
A16B 22:        Illegal Opcode
A16C 00:        BRK:imp    BREAK
A16D 08:        PHP:imp    Push P
A16E 73:        Illegal Opcode
A16F 26:1A      ROL:zp     Zp RAM 001A
A171 7E:22 00   ROR:abs,x  $0022,X
A174 06:77      ASL:zp     Zp RAM 0077
A176 24:12      BIT:zp     Zp RAM 0012
A178 00:        BRK:imp    BREAK
A179 14:        Illegal Opcode
A17A 1A:        Illegal Opcode
A17B 71:06      ADC:ind,y  06
A17D 80:        Illegal Opcode
A17E 24:16      BIT:zp     Zp RAM 0016
A180 12:        Illegal Opcode
A181 00:        BRK:imp    BREAK
A182 0C:        Illegal Opcode
A183 14:        Illegal Opcode
A184 04:        Illegal Opcode
A185 06:89      ASL:zp     Zp RAM 0089
A187 02:        Illegal Opcode
A188 04:        Illegal Opcode
A189 00:        BRK:imp    BREAK
A18A 0C:        Illegal Opcode
A18B 08:        PHP:imp    Push P
A18C 91:06      STA:ind,y  06
A18E 86:A2      STX:zp     Zp RAM 00A2
A190 0B:        Illegal Opcode
A191 86:37      STX:zp     Zp RAM 0037
; Beginning of loop
;
; RE-EVALUATE THIS. IT APPEARS TO HANDLE BOTH PLAYER AND ENEMY BULLET LOGIC
;
A193 A6:37      LDX:zp     Zp RAM 0037
A195 BD:D3 02   LDA:abs,x  $02D3,X	 ; Grab the height of a player's
A198 F0:45      BEQ:rel    Branch->$A1DF ; bullet. No bullet? Check the next.
A19A E0:08      CPX:imm    #08
A19C B0:22      BCS:rel    Branch->$A1C0
A19E 69:09      ADC:imm    #09
A1A0 BC:F2 02   LDY:abs,x  $02F2,X
A1A3 F0:03      BEQ:rel    Branch->$A1A8
A1A5 38:        SEC:imp    Set Carry
A1A6 E9:04      SBC:imm    #04
A1A8 9D:D3 02   STA:abs,x  $02D3,X
A1AB 20:FA A1   JSR:abs    $A1FA
A1AE BD:D3 02   LDA:abs,x  $02D3,X
A1B1 C9:F0      CMP:imm    #F0
A1B3 90:08      BCC:rel    Branch->$A1BD
A1B5 CE:35 01   DEC:abs    $0135         ; One less shot by the player
                                         ; is on the screen.
A1B8 A9:00      LDA:imm    #00		 ; Get rid of the player's
A1BA 9D:D3 02   STA:abs,x  $02D3,X	 ; bullet.
A1BD B8:        CLV:imp    Clear Overflow
A1BE 50:1F      BVC:rel    Branch->$A1DF
A1C0 BD:E6 02   LDA:abs,x  $02E6,X
A1C3 18:        CLC:imp    Clear Carry
A1C4 6D:20 01   ADC:abs    $0120
A1C7 9D:E6 02   STA:abs,x  $02E6,X
A1CA BD:D3 02   LDA:abs,x  $02D3,X	 ; Grab the enemy bullet Y position.
A1CD 6D:18 01   ADC:abs    $0118	 ; Move it up by its movement rate.
A1D0 CD:02 02   CMP:abs    $0202	 ; Has it hit the player?
A1D3 B0:07      BCS:rel    Branch->$A1DC ; 
A1D5 C6:A6      DEC:zp     Zp RAM 00A6	 ; One less enemy shot outstanding.
A1D7 20:E4 A1   JSR:abs    $A1E4
A1DA A9:00      LDA:imm    #00		 ; Get rid of this player's bullet.
A1DC 9D:D3 02   STA:abs,x  $02D3,X	 ; Store the bullet position.
A1DF C6:37      DEC:zp     Zp RAM 0037   ; More bullets to check?
A1E1 10:B0      BPL:rel    Branch->$A193 ; Re-run the loop else RTS.
A1E3 60:        RTS:imp    Ret from Sub
;
A1E4 AD:00 02   LDA:abs    $0200	 ; Is this shot going down the
A1E7 DD:AD 02   CMP:abs,x  $02AD,X	 ; same segment the player is on?
A1EA D0:0D      BNE:rel    Branch->$A1F9 ; If so, return.
A1EC AD:01 02   LDA:abs    $0201	 ; If they player is already dead,
A1EF 30:08      BMI:rel    Branch->$A1F9 ; return.
A1F1 20:4B A3   JSR:abs    $A34B
A1F4 A9:81      LDA:imm    #81		 ; Player is dead. Indicate this
A1F6 8D:01 02   STA:abs    $0201	 ; with tunnel position #81.
A1F9 60:        RTS:imp    Ret from Sub
;
A1FA BC:AD 02   LDY:abs,x  $02AD,X
A1FD B9:AC 03   LDA:abs,y  $03AC,Y
A200 F0:3C      BEQ:rel    Branch->$A23E
A202 BD:D3 02   LDA:abs,x  $02D3,X
A205 D9:AC 03   CMP:abs,y  $03AC,Y
A208 90:25      BCC:rel    Branch->$A22F
A20A C9:F0      CMP:imm    #F0
A20C 90:02      BCC:rel    Branch->$A210
A20E A9:00      LDA:imm    #00
A210 99:AC 03   STA:abs,y  $03AC,Y
A213 FE:F2 02   INC:abs,x  $02F2,X
A216 A9:C0      LDA:imm    #C0
A218 99:9A 03   STA:abs,y  $039A,Y
A21B 20:F6 CC   JSR:abs    $CCF6
A21E A2:FF      LDX:imm    #FF
A220 A9:00      LDA:imm    #00
A222 85:2A      STA:zp     Zp RAM 002A
A224 85:2B      STA:zp     Zp RAM 002B
A226 A9:01      LDA:imm    #01
A228 85:29      STA:zp     Zp RAM 0029
A22A 20:6C CA   JSR:abs    $CA6C
A22D A6:37      LDX:zp     Zp RAM 0037
A22F BD:F2 02   LDA:abs,x  $02F2,X
A232 C9:02      CMP:imm    #02
A234 90:08      BCC:rel    Branch->$A23E
A236 A9:00      LDA:imm    #00           ; Player's shot has disappeared.
A238 9D:D3 02   STA:abs,x  $02D3,X       ; Clear it's Y slot and record this
A23B CE:35 01   DEC:abs    $0135         ; in the # of bullets "outstanding".
A23E 60:        RTS:imp    Ret from Sub
A23F AD:01 02   LDA:abs    $0201
A242 30:61      BMI:rel    Branch->$A2A5
A244 A5:05      LDA:zp     Zp RAM 0005
A246 30:28      BMI:rel    Branch->$A270
A248 AD:06 01   LDA:abs    $0106
A24B 85:29      STA:zp     Zp RAM 0029
A24D A2:0A      LDX:imm    #0A
A24F BD:DB 02   LDA:abs,x  $02DB,X
A252 F0:14      BEQ:rel    Branch->$A268
A254 BD:B5 02   LDA:abs,x  $02B5,X
A257 38:        SEC:imp    Set Carry
A258 ED:00 02   SBC:abs    $0200
A25B 10:05      BPL:rel    Branch->$A262
A25D 49:FF      EOR:imm    #FF
A25F 18:        CLC:imp    Clear Carry
A260 69:01      ADC:imm    #01
A262 C9:02      CMP:imm    #02
A264 B0:02      BCS:rel    Branch->$A268
A266 E6:29      INC:zp     Zp RAM 0029
A268 CA:        DEX:imp    X=X-1
A269 10:E4      BPL:rel    Branch->$A24F
A26B A5:29      LDA:zp     Zp RAM 0029
A26D B8:        CLV:imp    Clear Overflow
A26E 50:04      BVC:rel    Branch->$A274
A270 A5:4D      LDA:zp     Zp RAM 004D
A272 29:10      AND:imm    #10
A274 F0:2F      BEQ:rel    Branch->$A2A5
A276 A2:07      LDX:imm    #07           ; Start by checking the last bullet
                                         ; slot.
A278 BD:D3 02   LDA:abs,x  $02D3,X       ; Is this currently being used?
A27B D0:25      BNE:rel    Branch->$A2A2 ; If so, branch to a statement that
                                         ; will check another slot and loop.
A27D EE:35 01   INC:abs    $0135         : Store the increase in the
                                         : bullet count.
A280 AD:02 02   LDA:abs    $0202         ; Get the player's current Y position
A283 9D:D3 02   STA:abs,x  $02D3,X       ; and store it as the bullet's Y pos.
A286 AD:00 02   LDA:abs    $0200         ; Get the player's X-1 and store it as
A289 9D:AD 02   STA:abs,x  $02AD,X       ; the bullet's X-1 position.
A28C AD:01 02   LDA:abs    $0201         ; Get the player's X and store it as
A28F 9D:C0 02   STA:abs,x  $02C0,X       ; the bullet's X position.
A292 A9:00      LDA:imm    #00
A294 9D:F2 02   STA:abs,x  $02F2,X
A297 20:EA CC   JSR:abs    $CCEA
A29A AD:02 02   LDA:abs    $0202
A29D 20:63 A4   JSR:abs    $A463
A2A0 A2:00      LDX:imm    #00
A2A2 CA:        DEX:imp    X=X-1         ; Branch from A27B. If slot is
                                         ; taken, then look at the previous
                                         ; slot to see if we can store a
                                         ; bullet in it.
A2A3 10:D3      BPL:rel    Branch->$A278
A2A5 60:        RTS:imp    Ret from Sub

A2A6 AD:01 02   LDA:abs    $0201
A2A9 30:58      BMI:rel    Branch->$A303
A2AB A2:06      LDX:imm    #06
A2AD BD:DF 02   LDA:abs,x  $02DF,X
A2B0 F0:4E      BEQ:rel    Branch->$A300
A2B2 C9:30      CMP:imm    #30
A2B4 90:4A      BCC:rel    Branch->$A300
A2B6 BD:8A 02   LDA:abs,x  $028A,X
A2B9 29:40      AND:imm    #40
A2BB F0:43      BEQ:rel    Branch->$A300
A2BD DE:A6 02   DEC:abs,x  $02A6,X
A2C0 10:3E      BPL:rel    Branch->$A300
A2C2 FE:A6 02   INC:abs,x  $02A6,X
A2C5 BD:83 02   LDA:abs,x  $0283,X
A2C8 29:80      AND:imm    #80
A2CA D0:34      BNE:rel    Branch->$A300
A2CC AD:CA 60   LDA:abs    $60CA
A2CF A4:A6      LDY:zp     Zp RAM 00A6
A2D1 D9:04 A3   CMP:abs,y  $A304,Y
A2D4 90:2A      BCC:rel    Branch->$A300
A2D6 AC:1A 01   LDY:abs    $011A
A2D9 B9:DB 02   LDA:abs,y  $02DB,Y
A2DC D0:1F      BNE:rel    Branch->$A2FD
A2DE BD:DF 02   LDA:abs,x  $02DF,X
A2E1 99:DB 02   STA:abs,y  $02DB,Y
A2E4 BD:B9 02   LDA:abs,x  $02B9,X
A2E7 99:B5 02   STA:abs,y  $02B5,Y
A2EA BD:CC 02   LDA:abs,x  $02CC,X
A2ED 99:C8 02   STA:abs,y  $02C8,Y
A2F0 AD:19 01   LDA:abs    $0119
A2F3 9D:A6 02   STA:abs,x  $02A6,X
A2F6 20:BD CC   JSR:abs    $CCBD
A2F9 E6:A6      INC:zp     Zp RAM 00A6	 ; One more enemy bullet is in play.
A2FB A0:00      LDY:imm    #00
A2FD 88:        DEY:imp    Y=Y-1
A2FE 10:D9      BPL:rel    Branch->$A2D9
A300 CA:        DEX:imp    X=X-1
A301 10:AA      BPL:rel    Branch->$A2AD
A303 60:        RTS:imp    Ret from Sub
A304 00:        BRK:imp    BREAK
A305 E0:F0      CPX:imm    #F0
A307 FA:        Illegal Opcode
A308 FF:        Illegal Opcode
A309 86:37      STX:zp     Zp RAM 0037
A30B A9:FF      LDA:imm    #FF
A30D 9D:F2 02   STA:abs,x  $02F2,X
A310 98:        TYA:imp    Y-->A
A311 38:        SEC:imp    Set Carry
A312 E9:04      SBC:imm    #04
A314 A8:        TAY:imp    A-->Y
A315 B9:B9 02   LDA:abs,y  $02B9,Y
A318 85:2D      STA:zp     Zp RAM 002D
A31A AD:DA 60   LDA:abs    $60DA	 ; Grab a random number,
A31D 29:07      AND:imm    #07		 ; 0-7.
A31F C9:03      CMP:imm    #03
A321 90:02      BCC:rel    Branch->$A325
A323 A9:00      LDA:imm    #00
A325 48:        PHA:imp    Push Accum
A326 18:        CLC:imp    Clear Carry
A327 69:02      ADC:imm    #02
A329 20:CA A3   JSR:abs    $A3CA
A32C 20:6F A0   JSR:abs    $A06F
A32F 68:        PLA:imp    Pull Accum
A330 18:        CLC:imp    Clear Carry
A331 69:05      ADC:imm    #05
A333 AA:        TAX:imp    A-->X
A334 20:6C CA   JSR:abs    $CA6C
A337 A6:37      LDX:zp     Zp RAM 0037
A339 60:        RTS:imp    Ret from Sub
;
A33A A9:05      LDA:imm    #05
A33C 20:52 A3   JSR:abs    $A352
A33F CE:01 02   DEC:abs    $0201	; Move player one segment over.
A342 60:        RTS:imp    Ret from Sub
;
A343 A9:09      LDA:imm    #09
A345 D0:06      BNE:rel    Branch->$A34D
A347 A9:07      LDA:imm    #07
A349 D0:02      BNE:rel    Branch->$A34D
A34B A9:FF      LDA:imm    #FF
A34D 8D:3B 01   STA:abs    $013B
A350 A9:01      LDA:imm    #01
A352 85:2C      STA:zp     Zp RAM 002C
A354 AD:02 02   LDA:abs    $0202
A357 85:29      STA:zp     Zp RAM 0029
A359 AD:00 02   LDA:abs    $0200
A35C 85:2D      STA:zp     Zp RAM 002D
A35E 20:B0 CC   JSR:abs    $CCB0
A361 20:D6 A3   JSR:abs    $A3D6
A364 A9:81      LDA:imm    #81		 ; Player is dead. Indicate this
A366 8D:01 02   STA:abs    $0201	 ; with tunnel position #81.
A369 A9:01      LDA:imm    #01
A36B 8D:3C 01   STA:abs    $013C
A36E 60:        RTS:imp    Ret from Sub
;
A36F 20:C1 CC   JSR:abs    $CCC1
A372 B9:DB 02   LDA:abs,y  $02DB,Y
A375 85:29      STA:zp     Zp RAM 0029
A377 B9:B5 02   LDA:abs,y  $02B5,Y
A37A 85:2D      STA:zp     Zp RAM 002D
A37C A9:00      LDA:imm    #00
A37E 20:D4 A3   JSR:abs    $A3D4
A381 A9:00      LDA:imm    #00		 ; Get rid of this enemy's
A383 99:DB 02   STA:abs,y  $02DB,Y	 ; bullet and decrease the
A386 C6:A6      DEC:zp     Zp RAM 00A6	 ; bullet-in-play count.
A388 A9:FF      LDA:imm    #FF
A38A 9D:F2 02   STA:abs,x  $02F2,X
A38D 60:        RTS:imp    Ret from Sub
;
A38E A9:FF      LDA:imm    #FF
A390 9D:F2 02   STA:abs,x  $02F2,X
A393 98:        TYA:imp    Y-->A
A394 38:        SEC:imp    Set Carry
A395 E9:04      SBC:imm    #04
A397 A8:        TAY:imp    A-->Y
A398 B9:83 02   LDA:abs,y  $0283,Y
A39B 29:C0      AND:imm    #C0
A39D C9:C0      CMP:imm    #C0
A39F F0:06      BEQ:rel    Branch->$A3A7
A3A1 B9:B9 02   LDA:abs,y  $02B9,Y
A3A4 B8:        CLV:imp    Clear Overflow
A3A5 50:08      BVC:rel    Branch->$A3AF
A3A7 B9:B9 02   LDA:abs,y  $02B9,Y
A3AA 38:        SEC:imp    Set Carry
A3AB E9:01      SBC:imm    #01
A3AD 29:0F      AND:imm    #0F
A3AF 85:2D      STA:zp     Zp RAM 002D
A3B1 A9:00      LDA:imm    #00
A3B3 20:CA A3   JSR:abs    $A3CA
A3B6 20:6F A0   JSR:abs    $A06F
A3B9 B9:83 02   LDA:abs,y  $0283,Y
A3BC 29:07      AND:imm    #07
A3BE A8:        TAY:imp    A-->Y
A3BF BE:C5 A3   LDX:abs,y  $A3C5,Y
A3C2 4C:6C CA   JMP:abs    $CA6C
A3C5 01:02      ORA:ind,x  02
A3C7 03:        Illegal Opcode
A3C8 04:        Illegal Opcode
A3C9 01:48      ORA:ind,x  48
A3CB 20:C1 CC   JSR:abs    $CCC1
A3CE B9:DF 02   LDA:abs,y  $02DF,Y
A3D1 85:29      STA:zp     Zp RAM 0029
A3D3 68:        PLA:imp    Pull Accum
A3D4 85:2C      STA:zp     Zp RAM 002C
A3D6 86:35      STX:zp     Zp RAM 0035
A3D8 84:36      STY:zp     Zp RAM 0036
A3DA A9:00      LDA:imm    #00
A3DC 85:2A      STA:zp     Zp RAM 002A
A3DE 85:2B      STA:zp     Zp RAM 002B
A3E0 A2:07      LDX:imm    #07
A3E2 BD:0A 03   LDA:abs,x  $030A,X
A3E5 F0:13      BEQ:rel    Branch->$A3FA
A3E7 BD:12 03   LDA:abs,x  $0312,X
A3EA C5:2A      CMP:zp     Zp RAM 002A
A3EC 90:04      BCC:rel    Branch->$A3F2
A3EE 85:2A      STA:zp     Zp RAM 002A
A3F0 86:2B      STX:zp     Zp RAM 002B
A3F2 CA:        DEX:imp    X=X-1
A3F3 10:ED      BPL:rel    Branch->$A3E2
A3F5 CE:16 01   DEC:abs    $0116
A3F8 A6:2B      LDX:zp     Zp RAM 002B
A3FA A9:00      LDA:imm    #00
A3FC 9D:12 03   STA:abs,x  $0312,X
A3FF A5:2C      LDA:zp     Zp RAM 002C
A401 9D:02 03   STA:abs,x  $0302,X
A404 A5:29      LDA:zp     Zp RAM 0029
A406 9D:0A 03   STA:abs,x  $030A,X
A409 A5:2D      LDA:zp     Zp RAM 002D
A40B 9D:FA 02   STA:abs,x  $02FA,X
A40E EE:16 01   INC:abs    $0116
A411 A6:35      LDX:zp     Zp RAM 0035
A413 A4:36      LDY:zp     Zp RAM 0036
A415 60:        RTS:imp    Ret from Sub
;
A416 AD:16 01   LDA:abs    $0116	 ; Are there any enemy explosions
					 ; (deaths) to handle?
A419 F0:2C      BEQ:rel    Branch->$A447 ; If not, return.
A41B A9:00      LDA:imm    #00		 ; Clear out all enemy explosions.
A41D 8D:16 01   STA:abs    $0116	 ;
A420 A2:07      LDX:imm    #07
A422 BD:0A 03   LDA:abs,x  $030A,X
A425 F0:1D      BEQ:rel    Branch->$A444
A427 BD:12 03   LDA:abs,x  $0312,X
A42A BC:02 03   LDY:abs,x  $0302,X
A42D 18:        CLC:imp    Clear Carry
A42E 79:4E A4   ADC:abs,y  $A44E,Y
A431 9D:12 03   STA:abs,x  $0312,X
A434 D9:48 A4   CMP:abs,y  $A448,Y
A437 90:08      BCC:rel    Branch->$A441
A439 A9:00      LDA:imm    #00
A43B 9D:0A 03   STA:abs,x  $030A,X
A43E B8:        CLV:imp    Clear Overflow
A43F 50:03      BVC:rel    Branch->$A444
A441 EE:16 01   INC:abs    $0116
A444 CA:        DEX:imp    X=X-1
A445 10:DB      BPL:rel    Branch->$A422
A447 60:        RTS:imp    Ret from Sub
;
; DATA used by previous subroutine
;
A448 10:15      BPL:rel    Branch->$A45F
A44A 20:20 20   JSR:abs    $2020
A44D 10:03      BPL:rel    Branch->$A452
A44F 01:03      ORA:ind,x  03
A451 03:        Illegal Opcode
A452 03:        Illegal Opcode
A453 03:        Illegal Opcode
;
A454 A2:07      LDX:imm    #07
A456 BD:D3 02   LDA:abs,x  $02D3,X
A459 F0:03      BEQ:rel    Branch->$A45E
A45B 20:63 A4   JSR:abs    $A463
A45E CA:        DEX:imp    X=X-1
A45F 10:F5      BPL:rel    Branch->$A456
A461 60:        RTS:imp    Ret from Sub
A462 AB:        Illegal Opcode
A463 85:2E      STA:zp     Zp RAM 002E
A465 A0:0A      LDY:imm    #0A
A467 B9:DB 02   LDA:abs,y  $02DB,Y
A46A F0:7F      BEQ:rel    Branch->$A4EB
A46C C5:2E      CMP:zp     Zp RAM 002E
A46E 90:05      BCC:rel    Branch->$A475
A470 E5:2E      SBC:zp     Zp RAM 002E
A472 B8:        CLV:imp    Clear Overflow
A473 50:06      BVC:rel    Branch->$A47B
A475 A5:2E      LDA:zp     Zp RAM 002E
A477 38:        SEC:imp    Set Carry
A478 F9:DB 02   SBC:abs,y  $02DB,Y
A47B C0:04      CPY:imm    #04
A47D B0:12      BCS:rel    Branch->$A491
A47F C5:A7      CMP:zp     Zp RAM 00A7
A481 B0:0B      BCS:rel    Branch->$A48E
A483 B9:B5 02   LDA:abs,y  $02B5,Y
A486 5D:AD 02   EOR:abs,x  $02AD,X
A489 D0:03      BNE:rel    Branch->$A48E
A48B 20:6F A3   JSR:abs    $A36F
A48E B8:        CLV:imp    Clear Overflow
A48F 50:5A      BVC:rel    Branch->$A4EB
A491 48:        PHA:imp    Push Accum
A492 84:38      STY:zp     Zp RAM 0038
A494 B9:7F 02   LDA:abs,y  $027F,Y
A497 29:07      AND:imm    #07
A499 A8:        TAY:imp    A-->Y
A49A 68:        PLA:imp    Pull Accum
A49B D9:51 01   CMP:abs,y  $0151,Y
A49E B0:49      BCS:rel    Branch->$A4E9
A4A0 C0:04      CPY:imm    #04
A4A2 D0:1D      BNE:rel    Branch->$A4C1
A4A4 A4:38      LDY:zp     Zp RAM 0038
A4A6 B9:DB 02   LDA:abs,y  $02DB,Y
A4A9 CD:02 02   CMP:abs    $0202
A4AC F0:10      BEQ:rel    Branch->$A4BE
A4AE BD:AD 02   LDA:abs,x  $02AD,X	 ; Is the player's bullet on the
A4B1 D9:B5 02   CMP:abs,y  $02B5,Y	 ; same segment as an enemy?
A4B4 D0:08      BNE:rel    Branch->$A4BE
A4B6 B9:C8 02   LDA:abs,y  $02C8,Y
A4B9 10:03      BPL:rel    Branch->$A4BE
A4BB 20:09 A3   JSR:abs    $A309
A4BE B8:        CLV:imp    Clear Overflow
A4BF 50:28      BVC:rel    Branch->$A4E9
A4C1 A4:38      LDY:zp     Zp RAM 0038
A4C3 B9:C8 02   LDA:abs,y  $02C8,Y
A4C6 10:0A      BPL:rel    Branch->$A4D2
A4C8 B9:B5 02   LDA:abs,y  $02B5,Y
A4CB DD:C0 02   CMP:abs,x  $02C0,X
A4CE F0:12      BEQ:rel    Branch->$A4E2
A4D0 D0:08      BNE:rel    Branch->$A4DA
A4D2 B9:DB 02   LDA:abs,y  $02DB,Y
A4D5 CD:02 02   CMP:abs    $0202
A4D8 F0:0F      BEQ:rel    Branch->$A4E9
A4DA B9:B5 02   LDA:abs,y  $02B5,Y
A4DD DD:AD 02   CMP:abs,x  $02AD,X
A4E0 D0:07      BNE:rel    Branch->$A4E9
A4E2 86:37      STX:zp     Zp RAM 0037
A4E4 20:8E A3   JSR:abs    $A38E
A4E7 A6:37      LDX:zp     Zp RAM 0037
A4E9 A4:38      LDY:zp     Zp RAM 0038
A4EB 88:        DEY:imp    Y=Y-1
A4EC 30:03      BMI:rel    Branch->$A4F1
A4EE 4C:67 A4   JMP:abs    $A467
;
A4F1 BD:F2 02   LDA:abs,x  $02F2,X
A4F4 C9:FF      CMP:imm    #FF
A4F6 D0:0B      BNE:rel    Branch->$A503
A4F8 A9:00      LDA:imm    #00
A4FA 9D:D3 02   STA:abs,x  $02D3,X
A4FD CE:35 01   DEC:abs    $0135
A500 9D:F2 02   STA:abs,x  $02F2,X
A503 60:        RTS:imp    Ret from Sub
;
A504 AD:01 02   LDA:abs    $0201
A507 10:78      BPL:rel    Branch->$A581
A509 AD:35 01   LDA:abs    $0135	 ; If no player shots on screen
A50C 05:A6      ORA:zp     Zp RAM 00A6	 ; ??????
A50E 0D:16 01   ORA:abs    $0116	 ; and if no enemy shots on screen
A511 D0:6B      BNE:rel    Branch->$A57E ; then skip a lot of this, else...
A513 AE:1C 01   LDX:abs    $011C	 ; Start with slot 6 (of 0-6)
A516 BD:DF 02   LDA:abs,x  $02DF,X	 ; Is there an enemy in this slot?
A519 F0:0E      BEQ:rel    Branch->$A529 ; If not, check the other slots.
A51B 18:        CLC:imp    Clear Carry
A51C 69:0F      ADC:imm    #0F
A51E B0:02      BCS:rel    Branch->$A522
A520 C9:F0      CMP:imm    #F0
A522 90:02      BCC:rel    Branch->$A526
A524 A9:00      LDA:imm    #00		 ; Kill this enemy. 
A526 9D:DF 02   STA:abs,x  $02DF,X	 ; Null their height.
A529 CA:        DEX:imp    X=X-1	 ; Decrement loop and check
A52A 10:EA      BPL:rel    Branch->$A516 ; the next enemy.
A52C A6:3D      LDX:zp     Zp RAM 003D	 ; Does the current player
A52E B5:48      LDA:zp,x   Zp RAM 0048	 ; have exactly one life left?
A530 C9:01      CMP:imm    #01		 ;
A532 D0:20      BNE:rel    Branch->$A554 ; If so, skip a bit.
A534 A9:00      LDA:imm    #00
A536 8D:0F 01   STA:abs    $010F
A539 A9:01      LDA:imm    #01
A53B 8D:14 01   STA:abs    $0114
A53E A5:5F      LDA:zp     Zp RAM 005F
A540 38:        SEC:imp    Set Carry
A541 E9:20      SBC:imm    #20
A543 85:5F      STA:zp     Zp RAM 005F
A545 A5:5B      LDA:zp     Zp RAM 005B
A547 E9:00      SBC:imm    #00
A549 85:5B      STA:zp     Zp RAM 005B
A54B C9:FA      CMP:imm    #FA
A54D 18:        CLC:imp    Clear Carry
A54E D0:01      BNE:rel    Branch->$A551
A550 38:        SEC:imp    Set Carry
A551 B8:        CLV:imp    Clear Overflow
A552 50:0D      BVC:rel    Branch->$A561
A554 AD:02 02   LDA:abs    $0202
A557 18:        CLC:imp    Clear Carry
A558 69:0F      ADC:imm    #0F
A55A 8D:02 02   STA:abs    $0202
A55D B0:02      BCS:rel    Branch->$A561
A55F C9:F0      CMP:imm    #F0
A561 90:1B      BCC:rel    Branch->$A57E
A563 A9:06      LDA:imm    #06
A565 85:00      STA:zp     Zp RAM 0000
A567 20:8F 92   JSR:abs    $928F	 ; Remove all bullets from play.
A56A AD:08 01   LDA:abs    $0108	 ; How many enemies are inside the
A56D 18:        CLC:imp    Clear Carry	 ; tube? Add with the # at the
A56E 6D:09 01   ADC:abs    $0109	 ; top of the tube. Add with the #
A571 18:        CLC:imp    Clear Carry	 ; of enemies YET to appear.
A572 6D:AB 03   ADC:abs    $03AB	 ; If the total is less than
A575 C9:3F      CMP:imm    #3F		 ; 3F, then make the number of
A577 90:02      BCC:rel    Branch->$A57B ; enemies yet to appear equal
A579 A9:3F      LDA:imm    #3F		 ; to 3F (63 decimal). 
A57B 8D:AB 03   STA:abs    $03AB	 ;
A57E B8:        CLV:imp    Clear Overflow;
A57F 50:49      BVC:rel    Branch->$A5CA ; Return from Subroutine

A581 AD:55 04   LDA:abs    $0455	 ; More copy protection?
A584 0D:1B 01   ORA:abs    $011B
A587 F0:0A      BEQ:rel    Branch->$A593

A589 A9:17      LDA:imm    #17		 ; Compare player one's score to
A58B C5:42      CMP:zp     Zp RAM 0042	 ; 170,000. "famous" score
A58D B0:04      BCS:rel    Branch->$A593 ; bug (copy protection) for
A58F A6:40      LDX:zp     Zp RAM 0040   ; Tempest. Mangle $0000-$0099.
A591 F6:00      INC:zp,x   Zp RAM 0000	 ; Kinda. Uses BCD numbers.

A593 AD:06 01   LDA:abs    $0106
A596 D0:32      BNE:rel    Branch->$A5CA ; Return from Subroutine.
A598 AD:AB 03   LDA:abs    $03AB
A59B 0D:16 01   ORA:abs    $0116
A59E D0:15      BNE:rel    Branch->$A5B5
A5A0 AC:1C 01   LDY:abs    $011C
A5A3 B9:DF 02   LDA:abs,y  $02DF,Y
A5A6 F0:04      BEQ:rel    Branch->$A5AC
A5A8 C9:11      CMP:imm    #11
A5AA B0:09      BCS:rel    Branch->$A5B5
A5AC 88:        DEY:imp    Y=Y-1
A5AD 10:F4      BPL:rel    Branch->$A5A3
A5AF 20:CB A5   JSR:abs    $A5CB
A5B2 20:8F 92   JSR:abs    $928F	 ; Remove all bullets from play.
A5B5 A5:4D      LDA:zp     Zp RAM 004D
A5B7 29:60      AND:imm    #60
A5B9 F0:0F      BEQ:rel    Branch->$A5CA ; Return from Subroutine.
A5BB 24:05      BIT:zp     Zp RAM 0005
A5BD 10:0B      BPL:rel    Branch->$A5CA ; Return from Subroutine.
A5BF A5:09      LDA:zp     Zp RAM 0009
A5C1 29:43      AND:imm    #43
A5C3 C9:40      CMP:imm    #40
A5C5 D0:03      BNE:rel    Branch->$A5CA ; Return from Subroutine.
A5C7 20:CB A5   JSR:abs    $A5CB
A5CA 60:        RTS:imp    Ret from Sub

A5CB A9:20      LDA:imm    #20		 ; Level complete. Set general game
A5CD 85:00      STA:zp     Zp RAM 0000	 ; status = zoom-out of current level.
A5CF AD:06 01   LDA:abs    $0106
A5D2 09:80      ORA:imm    #80
A5D4 8D:06 01   STA:abs    $0106
A5D7 A9:00      LDA:imm    #00
A5D9 8D:04 01   STA:abs    $0104
A5DC 8D:07 01   STA:abs    $0107
A5DF 85:5C      STA:zp     Zp RAM 005C
A5E1 8D:23 01   STA:abs    $0123
A5E4 A9:02      LDA:imm    #02
A5E6 8D:05 01   STA:abs    $0105
A5E9 A2:0F      LDX:imm    #0F
A5EB BD:AC 03   LDA:abs,x  $03AC,X
A5EE F0:03      BEQ:rel    Branch->$A5F3
A5F0 EE:23 01   INC:abs    $0123
A5F3 CA:        DEX:imp    X=X-1
A5F4 10:F5      BPL:rel    Branch->$A5EB
A5F6 AD:23 01   LDA:abs    $0123
A5F9 F0:17      BEQ:rel    Branch->$A612
A5FB A5:9F      LDA:zp     Zp RAM 009F
A5FD C9:07      CMP:imm    #07
A5FF B0:11      BCS:rel    Branch->$A612
A601 A9:1E      LDA:imm    #1E
A603 85:04      STA:zp     Zp RAM 0004
A605 A9:0A      LDA:imm    #0A		 ; Set game status to non-player
A607 85:00      STA:zp     Zp RAM 0000	 ; input mode.
A609 A9:20      LDA:imm    #20
A60B 85:02      STA:zp     Zp RAM 0002
A60D A9:80      LDA:imm    #80
A60F 8D:23 01   STA:abs    $0123
A612 A9:FF      LDA:imm    #FF
A614 8D:25 01   STA:abs    $0125
A617 60:        RTS:imp    Ret from Sub
;
A618 AD:0E 01   LDA:abs    $010E
A61B 8D:0D 01   STA:abs    $010D
A61E A2:0F      LDX:imm    #0F
A620 86:37      STX:zp     Zp RAM 0037
A622 A6:37      LDX:zp     Zp RAM 0037
A624 BD:83 02   LDA:abs,x  $0283,X
A627 D0:0B      BNE:rel    Branch->$A634
A629 AD:0E 01   LDA:abs    $010E
A62C F0:03      BEQ:rel    Branch->$A631
A62E 20:5B A6   JSR:abs    $A65B
A631 B8:        CLV:imp    Clear Overflow
A632 50:0B      BVC:rel    Branch->$A63F
A634 20:A9 A6   JSR:abs    $A6A9
A637 20:21 A7   JSR:abs    $A721
A63A A9:FF      LDA:imm    #FF
A63C 8D:0D 01   STA:abs    $010D
A63F C6:37      DEC:zp     Zp RAM 0037
A641 10:DF      BPL:rel    Branch->$A622
A643 A5:03      LDA:zp     Zp RAM 0003
A645 29:01      AND:imm    #01
A647 D0:08      BNE:rel    Branch->$A651
A649 AD:0E 01   LDA:abs    $010E
A64C F0:03      BEQ:rel    Branch->$A651
A64E CE:0E 01   DEC:abs    $010E
A651 AD:0D 01   LDA:abs    $010D
A654 D0:04      BNE:rel    Branch->$A65A
A656 A9:12      LDA:imm    #12		 ; Game status = entry of high score
A658 85:00      STA:zp     Zp RAM 0000	 ; onto high score list, after game.
A65A 60:        RTS:imp    Ret from Sub
;
A65B A5:03      LDA:zp     Zp RAM 0003
A65D 29:00      AND:imm    #00
A65F D0:39      BNE:rel    Branch->$A69A
A661 A9:80      LDA:imm    #80
A663 9D:63 02   STA:abs,x  $0263,X
A666 9D:83 02   STA:abs,x  $0283,X
A669 9D:A3 02   STA:abs,x  $02A3,X
A66C AD:DA 60   LDA:abs    $60DA
A66F 9D:C3 02   STA:abs,x  $02C3,X
A672 20:9B A6   JSR:abs    $A69B
A675 9D:23 03   STA:abs,x  $0323,X
A678 AD:CA 60   LDA:abs    $60CA
A67B 9D:E3 02   STA:abs,x  $02E3,X
A67E 20:9B A6   JSR:abs    $A69B
A681 30:05      BMI:rel    Branch->$A688
A683 49:FF      EOR:imm    #FF
A685 18:        CLC:imp    Clear Carry
A686 69:01      ADC:imm    #01
A688 9D:43 03   STA:abs,x  $0343,X
A68B AD:CA 60   LDA:abs    $60CA
A68E 9D:03 03   STA:abs,x  $0303,X
A691 20:9B A6   JSR:abs    $A69B
A694 9D:63 03   STA:abs,x  $0363,X
A697 20:C1 CC   JSR:abs    $CCC1
A69A 60:        RTS:imp    Ret from Sub
A69B 4A:        LSR:accum  
A69C AD:DA 60   LDA:abs    $60DA	 ; Grab a random number,
A69F 29:07      AND:imm    #07		 ; 0-7.
A6A1 90:05      BCC:rel    Branch->$A6A8
A6A3 49:FF      EOR:imm    #FF
A6A5 18:        CLC:imp    Clear Carry
A6A6 69:01      ADC:imm    #01
A6A8 60:        RTS:imp    Ret from Sub
A6A9 BD:E3 02   LDA:abs,x  $02E3,X
A6AC 18:        CLC:imp    Clear Carry
A6AD 7D:23 02   ADC:abs,x  $0223,X
A6B0 9D:23 02   STA:abs,x  $0223,X
A6B3 BD:43 03   LDA:abs,x  $0343,X
A6B6 30:0C      BMI:rel    Branch->$A6C4
A6B8 7D:83 02   ADC:abs,x  $0283,X
A6BB C9:F0      CMP:imm    #F0
A6BD 90:02      BCC:rel    Branch->$A6C1
A6BF A9:00      LDA:imm    #00
A6C1 B8:        CLV:imp    Clear Overflow
A6C2 50:09      BVC:rel    Branch->$A6CD
A6C4 7D:83 02   ADC:abs,x  $0283,X
A6C7 C9:10      CMP:imm    #10
A6C9 B0:02      BCS:rel    Branch->$A6CD
A6CB A9:00      LDA:imm    #00
A6CD A8:        TAY:imp    A-->Y
A6CE BD:C3 02   LDA:abs,x  $02C3,X
A6D1 18:        CLC:imp    Clear Carry
A6D2 7D:03 02   ADC:abs,x  $0203,X
A6D5 9D:03 02   STA:abs,x  $0203,X
A6D8 BD:23 03   LDA:abs,x  $0323,X
A6DB 30:0C      BMI:rel    Branch->$A6E9
A6DD 7D:63 02   ADC:abs,x  $0263,X
A6E0 C9:F0      CMP:imm    #F0
A6E2 90:02      BCC:rel    Branch->$A6E6
A6E4 A0:00      LDY:imm    #00
A6E6 B8:        CLV:imp    Clear Overflow
A6E7 50:09      BVC:rel    Branch->$A6F2
A6E9 7D:63 02   ADC:abs,x  $0263,X
A6EC C9:10      CMP:imm    #10
A6EE B0:02      BCS:rel    Branch->$A6F2
A6F0 A0:00      LDY:imm    #00
A6F2 9D:63 02   STA:abs,x  $0263,X
A6F5 BD:03 03   LDA:abs,x  $0303,X
A6F8 18:        CLC:imp    Clear Carry
A6F9 7D:43 02   ADC:abs,x  $0243,X
A6FC 9D:43 02   STA:abs,x  $0243,X
A6FF BD:63 03   LDA:abs,x  $0363,X
A702 30:0C      BMI:rel    Branch->$A710
A704 7D:A3 02   ADC:abs,x  $02A3,X
A707 C9:F0      CMP:imm    #F0
A709 90:02      BCC:rel    Branch->$A70D
A70B A0:00      LDY:imm    #00
A70D B8:        CLV:imp    Clear Overflow
A70E 50:09      BVC:rel    Branch->$A719
A710 7D:A3 02   ADC:abs,x  $02A3,X
A713 C9:10      CMP:imm    #10
A715 B0:02      BCS:rel    Branch->$A719
A717 A0:00      LDY:imm    #00
A719 9D:A3 02   STA:abs,x  $02A3,X
A71C 98:        TYA:imp    Y-->A
A71D 9D:83 02   STA:abs,x  $0283,X
A720 60:        RTS:imp    Ret from Sub
A721 A9:FD      LDA:imm    #FD
A723 85:29      STA:zp     Zp RAM 0029
A725 BD:C3 02   LDA:abs,x  $02C3,X
A728 BC:23 03   LDY:abs,x  $0323,X
A72B 20:5D A7   JSR:abs    $A75D
A72E 9D:C3 02   STA:abs,x  $02C3,X
A731 98:        TYA:imp    Y-->A
A732 9D:23 03   STA:abs,x  $0323,X
A735 BD:E3 02   LDA:abs,x  $02E3,X
A738 BC:43 03   LDY:abs,x  $0343,X
A73B 20:5D A7   JSR:abs    $A75D
A73E 9D:E3 02   STA:abs,x  $02E3,X
A741 98:        TYA:imp    Y-->A
A742 9D:43 03   STA:abs,x  $0343,X
A745 BD:03 03   LDA:abs,x  $0303,X
A748 BC:63 03   LDY:abs,x  $0363,X
A74B 20:5D A7   JSR:abs    $A75D
A74E 9D:03 03   STA:abs,x  $0303,X
A751 98:        TYA:imp    Y-->A
A752 9D:63 03   STA:abs,x  $0363,X
A755 A5:29      LDA:zp     Zp RAM 0029
A757 D0:03      BNE:rel    Branch->$A75C
A759 9D:83 02   STA:abs,x  $0283,X
A75C 60:        RTS:imp    Ret from Sub
A75D 84:2B      STY:zp     Zp RAM 002B
A75F 24:2B      BIT:zp     Zp RAM 002B
A761 30:0F      BMI:rel    Branch->$A772
A763 38:        SEC:imp    Set Carry
A764 ED:88 A7   SBC:abs    $A788
A767 85:2A      STA:zp     Zp RAM 002A
A769 A5:2B      LDA:zp     Zp RAM 002B
A76B E9:00      SBC:imm    #00
A76D 90:0F      BCC:rel    Branch->$A77E
A76F B8:        CLV:imp    Clear Overflow
A770 50:12      BVC:rel    Branch->$A784
A772 18:        CLC:imp    Clear Carry
A773 6D:88 A7   ADC:abs    $A788
A776 85:2A      STA:zp     Zp RAM 002A
A778 A5:2B      LDA:zp     Zp RAM 002B
A77A 69:00      ADC:imm    #00
A77C 90:06      BCC:rel    Branch->$A784
A77E E6:29      INC:zp     Zp RAM 0029
A780 A9:00      LDA:imm    #00
A782 85:2A      STA:zp     Zp RAM 002A
A784 A8:        TAY:imp    A-->Y
A785 A5:2A      LDA:zp     Zp RAM 002A
A787 60:        RTS:imp    Ret from Sub
A788 20:A2 0F   JSR:abs    $0FA2
A78B A9:00      LDA:imm    #00
A78D 9D:83 02   STA:abs,x  $0283,X
A790 CA:        DEX:imp    X=X-1
A791 10:F8      BPL:rel    Branch->$A78B
A793 A9:20      LDA:imm    #20
A795 8D:0E 01   STA:abs    $010E
A798 8D:0D 01   STA:abs    $010D
A79B A9:04      LDA:imm    #04		; Set mode to end-game victory
A79D 85:01      STA:zp     Zp RAM 0001	; explosion for high score.
A79F A9:00      LDA:imm    #00
A7A1 85:68      STA:zp     Zp RAM 0068
A7A3 85:69      STA:zp     Zp RAM 0069
A7A5 60:        RTS:imp    Ret from Sub
A7A6 84:2A      STY:zp     Zp RAM 002A
A7A8 38:        SEC:imp    Set Carry
A7A9 E5:2A      SBC:zp     Zp RAM 002A
A7AB 85:2A      STA:zp     Zp RAM 002A
A7AD 2C:11 01   BIT:abs    $0111
A7B0 30:09      BMI:rel    Branch->$A7BB ; Return from subroutine
A7B2 29:0F      AND:imm    #0F
A7B4 2C:BC A7   BIT:abs    $A7BC
A7B7 F0:02      BEQ:rel    Branch->$A7BB ; Return from subroutine
A7B9 09:F8      ORA:imm    #F8
A7BB 60:        RTS:imp    Ret from Sub
A7BC 08:        PHP:imp    Push P
A7BD A2:07      LDX:imm    #07
A7BF A9:00      LDA:imm    #00
A7C1 9D:FE 03   STA:abs,x  $03FE,X
A7C4 CA:        DEX:imp    X=X-1
A7C5 10:FA      BPL:rel    Branch->$A7C1
A7C7 A9:F0      LDA:imm    #F0
A7C9 8D:05 04   STA:abs    $0405
A7CC A9:FF      LDA:imm    #FF
A7CE 8D:15 01   STA:abs    $0115
A7D1 60:        RTS:imp    Ret from Sub
A7D2 AD:15 01   LDA:abs    $0115
A7D5 F0:59      BEQ:rel    Branch->$A830
A7D7 A9:00      LDA:imm    #00
A7D9 85:29      STA:zp     Zp RAM 0029
A7DB A2:07      LDX:imm    #07
A7DD 86:37      STX:zp     Zp RAM 0037
A7DF A6:37      LDX:zp     Zp RAM 0037
A7E1 BD:FE 03   LDA:abs,x  $03FE,X
A7E4 F0:18      BEQ:rel    Branch->$A7FE
A7E6 38:        SEC:imp    Set Carry
A7E7 E9:07      SBC:imm    #07
A7E9 90:02      BCC:rel    Branch->$A7ED
A7EB C9:10      CMP:imm    #10
A7ED B0:0C      BCS:rel    Branch->$A7FB
A7EF AC:15 01   LDY:abs    $0115
A7F2 10:05      BPL:rel    Branch->$A7F9
A7F4 A9:F0      LDA:imm    #F0
A7F6 B8:        CLV:imp    Clear Overflow
A7F7 50:02      BVC:rel    Branch->$A7FB
A7F9 A9:00      LDA:imm    #00
A7FB B8:        CLV:imp    Clear Overflow
A7FC 50:20      BVC:rel    Branch->$A81E
;
; Start of ROM 136002.316 at $A800.	   NOTE: VERSION 3 ROMS, FOLKS.
;
A7FE AC:15 01   LDY:abs    $0115
A801 10:1B      BPL:rel    Branch->$A81E
A803 8A:        TXA:imp    X-->A
A804 18:        CLC:imp    Clear Carry
A805 69:01      ADC:imm    #01
A807 C9:08      CMP:imm    #08
A809 90:02      BCC:rel    Branch->$A80D
A80B A9:00      LDA:imm    #00
A80D A8:        TAY:imp    A-->Y
A80E B9:FE 03   LDA:abs,y  $03FE,Y
A811 F0:0B      BEQ:rel    Branch->$A81E
A813 C9:D5      CMP:imm    #D5
A815 B0:05      BCS:rel    Branch->$A81C
A817 A9:F0      LDA:imm    #F0
A819 B8:        CLV:imp    Clear Overflow
A81A 50:02      BVC:rel    Branch->$A81E
A81C A9:00      LDA:imm    #00
A81E 9D:FE 03   STA:abs,x  $03FE,X
A821 05:29      ORA:zp     Zp RAM 0029
A823 85:29      STA:zp     Zp RAM 0029
A825 C6:37      DEC:zp     Zp RAM 0037
A827 10:B6      BPL:rel    Branch->$A7DF
A829 A5:29      LDA:zp     Zp RAM 0029
A82B D0:03      BNE:rel    Branch->$A830
A82D 8D:15 01   STA:abs    $0115
A830 60:        RTS:imp    Ret from Sub
;
; Superzapper routines begin here
;
A831 A9:00      LDA:imm    #00		 ; Clear out the number of times
A833 8D:AA 03   STA:abs    $03AA	 ; the zapper has been used and
A836 8D:25 01   STA:abs    $0125	 ; stop any zapper session that
A839 60:        RTS:imp    Ret from Sub	 ; is currently running.
;
; Main superzapper logic
;
A83A A5:05      LDA:zp     Zp RAM 0005
A83C 10:3E      BPL:rel    Branch->$A87C
A83E AD:25 01   LDA:abs    $0125	 ; If the superzapper is running,
A841 D0:23      BNE:rel    Branch->$A866 ; skip to the code which handles
					 ; the duration count incrementing
					 ; and checking.

A843 AD:01 02   LDA:abs    $0201	 ; If the player is dead (highest
A846 30:1B      BMI:rel    Branch->$A863 ; bit set) this skip this.

A848 A5:4E      LDA:zp     Zp RAM 004E	 ; If they have not pressed the
A84A 29:08      AND:imm    #08		 ; zapper button, then there is
A84C F0:15      BEQ:rel    Branch->$A863 ; nothing to do.

A84E AD:AA 03   LDA:abs    $03AA	 ; If the zapper buttons has been
A851 C9:02      CMP:imm    #02		 ; used twice already, clear and
A853 B0:08      BCS:rel    Branch->$A85D ; then ignore the request.

A855 EE:AA 03   INC:abs    $03AA	 ; Increase the counter for the #
A858 A9:01      LDA:imm    #01		 ; of times the zapper has been
A85A 8D:25 01   STA:abs    $0125	 ; used. Start the counter which
					 ; is used to control the duration
					 ; of a superzapper event.

A85D A5:4E      LDA:zp     Zp RAM 004E	 ; Clear the zapper from the list
A85F 29:77      AND:imm    #77		 ; of button events which need
A861 85:4E      STA:zp     Zp RAM 004E	 ; to he handled.
A863 B8:        CLV:imp    Clear Overflow
A864 50:16      BVC:rel    Branch->$A87C ; Onto other things.

A866 EE:25 01   INC:abs    $0125	 ; Increase the counter for the
A869 AE:AA 03   LDX:abs    $03AA	 ; duration the superzapper has been
A86C AD:25 01   LDA:abs    $0125	 ; running. Look up the zapper
A86F DD:83 A8   CMP:abs,x  $A883,X	 ; run-length table to see if it
A872 90:05      BCC:rel    Branch->$A879 ; is time to end the current
					 ; zapper session. (The table at
					 ; A884 and A885 control the length
					 ; of time the zapper runs for the
					 ; first use and the second use.
					 ; Branch if it isn't time to end
					 ; yet.

A874 A9:00      LDA:imm    #00		 ; End the current zapper session by
A876 8D:25 01   STA:abs    $0125	 ; setting the run timer to zero.
A879 20:88 A8   JSR:abs    $A888
A87C A5:4E      LDA:zp     Zp RAM 004E
A87E 29:7F      AND:imm    #7F
A880 85:4E      STA:zp     Zp RAM 004E
A882 60:        RTS:imp    Ret from Sub
;
A883 00:        BRK:imp    BREAK
A884 13:        Illegal Opcode
A885 05:00      ORA:zp     Zp RAM 0000
A887 00:        BRK:imp    BREAK
A888 AD:25 01   LDA:abs    $0125
A88B C9:03      CMP:imm    #03
A88D 90:14      BCC:rel    Branch->$A8A3
A88F 29:01      AND:imm    #01
A891 D0:10      BNE:rel    Branch->$A8A3
A893 AC:1C 01   LDY:abs    $011C
A896 B9:DF 02   LDA:abs,y  $02DF,Y
A899 D0:09      BNE:rel    Branch->$A8A4
A89B 88:        DEY:imp    Y=Y-1
A89C 10:F8      BPL:rel    Branch->$A896
A89E A9:00      LDA:imm    #00
A8A0 8D:25 01   STA:abs    $0125
A8A3 60:        RTS:imp    Ret from Sub
A8A4 B9:8A 02   LDA:abs,y  $028A,Y
A8A7 29:FC      AND:imm    #FC
A8A9 99:8A 02   STA:abs,y  $028A,Y
A8AC 4C:98 A3   JMP:abs    $A398
A8AF E1:24      SBC:ind,x  24
A8B1 26:28      ROL:zp     Zp RAM 0028
A8B3 2A:        ROL:accum  
A8B4 A9:01      LDA:imm    #01
A8B6 85:72      STA:zp     Zp RAM 0072
A8B8 20:6A DF   JSR:abs    $DF6A
A8BB A0:05      LDY:imm    #05
A8BD 20:D1 B0   JSR:abs    $B0D1
A8C0 A5:05      LDA:zp     Zp RAM 0005
A8C2 30:26      BMI:rel    Branch->$A8EA
A8C4 A2:00      LDX:imm    #00
A8C6 A5:03      LDA:zp     Zp RAM 0003
A8C8 29:20      AND:imm    #20
A8CA D0:0C      BNE:rel    Branch->$A8D8
A8CC A2:22      LDX:imm    #22
A8CE A5:06      LDA:zp     Zp RAM 0006	 ; Are there any game credits?
A8D0 F0:06      BEQ:rel    Branch->$A8D8
A8D2 24:A2      BIT:zp     Zp RAM 00A2
A8D4 30:02      BMI:rel    Branch->$A8D8
A8D6 A2:06      LDX:imm    #06
A8D8 20:14 AB   JSR:abs    $AB14
A8DB 20:0D AB   JSR:abs    $AB0D
A8DE AD:E4 31   LDA:abs    $31E4	 ; Read something from Vector
A8E1 8D:A6 2F   STA:abs    $2FA6	 ; ROM and put it in Vector RAM
A8E4 8D:A8 2F   STA:abs    $2FA8
A8E7 20:A8 AA   JSR:abs    $AAA8
A8EA A9:01      LDA:imm    #01
A8EC A0:00      LDY:imm    #00
A8EE 20:7F A9   JSR:abs    $A97F
A8F1 24:05      BIT:zp     Zp RAM 0005
A8F3 30:09      BMI:rel    Branch->$A8FE
A8F5 A5:43      LDA:zp     Zp RAM 0043
A8F7 05:44      ORA:zp     Zp RAM 0044
A8F9 05:45      ORA:zp     Zp RAM 0045
A8FB B8:        CLV:imp    Clear Overflow
A8FC 50:02      BVC:rel    Branch->$A900
A8FE A5:3E      LDA:zp     Zp RAM 003E
A900 F0:06      BEQ:rel    Branch->$A908
A902 A9:01      LDA:imm    #01
A904 A8:        TAY:imp    A-->Y
A905 20:7F A9   JSR:abs    $A97F
A908 A5:00      LDA:zp     Zp RAM 0000	 ; Are we in Self-test screen with
A90A C9:04      CMP:imm    #04		 ; diagional lines and character set?
A90C F0:35      BEQ:rel    Branch->$A943
A90E A9:1D      LDA:imm    #1D
A910 85:3B      STA:zp     Zp RAM 003B
A912 A9:07      LDA:imm    #07
A914 85:3C      STA:zp     Zp RAM 003C
A916 AE:E4 CD   LDX:abs    $CDE4
A919 20:D7 A9   JSR:abs    $A9D7
A91C A0:0A      LDY:imm    #0A
A91E A9:A7      LDA:imm    #A7
A920 59:CE AA   EOR:abs,y  $AACE,Y
A923 88:        DEY:imp    Y=Y-1
A924 10:FA      BPL:rel    Branch->$A920
A926 8D:6C 01   STA:abs    $016C
A929 AE:E5 CD   LDX:abs    $CDE5
A92C A9:02      LDA:imm    #02
A92E 85:38      STA:zp     Zp RAM 0038
A930 A4:38      LDY:zp     Zp RAM 0038
A932 B9:1B 06   LDA:abs,y  $061B,Y
A935 0A:        ASL:accum  
A936 A8:        TAY:imp    A-->Y
A937 B9:FA 31   LDA:abs,y  $31FA,Y
A93A 9D:60 2F   STA:abs,x  $2F60,X
A93D E8:        INX:imp    X=X+1
A93E E8:        INX:imp    X=X+1
A93F C6:38      DEC:zp     Zp RAM 0038
A941 10:ED      BPL:rel    Branch->$A930
A943 A9:2F      LDA:imm    #2F
A945 A2:60      LDX:imm    #60
A947 20:39 DF   JSR:abs    $DF39
A94A AD:23 01   LDA:abs    $0123
A94D 10:05      BPL:rel    Branch->$A954
A94F A2:36      LDX:imm    #36
A951 20:14 AB   JSR:abs    $AB14
A954 A5:00      LDA:zp     Zp RAM 0000
A956 C9:18      CMP:imm    #18
A958 D0:22      BNE:rel    Branch->$A97C
A95A A5:05      LDA:zp     Zp RAM 0005
A95C 10:1E      BPL:rel    Branch->$A97C
A95E A6:3D      LDX:zp     Zp RAM 003D	 ; Did the user choose to start
A960 BD:02 01   LDA:abs,x  $0102,X	 ; out on the first level?
A963 F0:0D      BEQ:rel    Branch->$A972 ; If so, skip a bit.
A965 A2:30      LDX:imm    #30
A967 20:14 AB   JSR:abs    $AB14
A96A A4:3D      LDY:zp     Zp RAM 003D	 ; Load into Y reg the choice #
A96C BE:02 01   LDX:abs,y  $0102,Y	 ; that the player started on.
					 ; NOTE: This is the choice #,
					 ; not the actual level #.
A96F 20:C6 B0   JSR:abs    $B0C6
A972 A2:3A      LDX:imm    #3A
A974 20:14 AB   JSR:abs    $AB14
A977 A2:38      LDX:imm    #38
A979 20:14 AB   JSR:abs    $AB14
A97C 60:        RTS:imp    Ret from Sub
;
A97D 42:        Illegal Opcode
A97E 45:A6      EOR:zp     Zp RAM 00A6
A980 00:        BRK:imp    BREAK
A981 E0:04      CPX:imm    #04
A983 84:2B      STY:zp     Zp RAM 002B
A985 C4:3D      CPY:zp     Zp RAM 003D
A987 D0:06      BNE:rel    Branch->$A98F
A989 24:05      BIT:zp     Zp RAM 0005
A98B 10:02      BPL:rel    Branch->$A98F
A98D A9:00      LDA:imm    #00
A98F 09:70      ORA:imm    #70
A991 BE:DE CD   LDX:abs,y  $CDDE,Y
A994 9D:60 2F   STA:abs,x  $2F60,X
A997 BE:E0 CD   LDX:abs,y  $CDE0,Y
A99A B9:48 00   LDA:abs,y  $0048,Y
A99D 85:38      STA:zp     Zp RAM 0038
A99F F0:06      BEQ:rel    Branch->$A9A7
A9A1 C4:3D      CPY:zp     Zp RAM 003D
A9A3 D0:02      BNE:rel    Branch->$A9A7
A9A5 C6:38      DEC:zp     Zp RAM 0038
A9A7 A0:01      LDY:imm    #01
A9A9 AD:84 32   LDA:abs    $3284
A9AC C4:38      CPY:zp     Zp RAM 0038
A9AE 90:05      BCC:rel    Branch->$A9B5
A9B0 F0:03      BEQ:rel    Branch->$A9B5
A9B2 AD:86 32   LDA:abs    $3286
A9B5 9D:60 2F   STA:abs,x  $2F60,X
A9B8 E8:        INX:imp    X=X+1
A9B9 E8:        INX:imp    X=X+1
A9BA C8:        INY:imp    Y=Y+1
A9BB C0:07      CPY:imm    #07
A9BD 90:EA      BCC:rel    Branch->$A9A9
A9BF A4:2B      LDY:zp     Zp RAM 002B
A9C1 A5:00      LDA:zp     Zp RAM 0000	 ; Are we in self-test screen with
A9C3 C9:04      CMP:imm    #04		 ; diagional lines and character set?
A9C5 D0:04      BNE:rel    Branch->$A9CB
A9C7 C4:3D      CPY:zp     Zp RAM 003D
A9C9 D0:30      BNE:rel    Branch->$A9FB
A9CB BE:E2 CD   LDX:abs,y  $CDE2,Y
A9CE B9:7D A9   LDA:abs,y  $A97D,Y
A9D1 85:3B      STA:zp     Zp RAM 003B
A9D3 A9:00      LDA:imm    #00
A9D5 85:3C      STA:zp     Zp RAM 003C
A9D7 A0:02      LDY:imm    #02
A9D9 84:2A      STY:zp     Zp RAM 002A
A9DB 38:        SEC:imp    Set Carry
A9DC 08:        PHP:imp    Push P
A9DD A0:00      LDY:imm    #00
A9DF B1:3B      LDA:ind,y  3B
A9E1 4A:        LSR:accum  
A9E2 4A:        LSR:accum  
A9E3 4A:        LSR:accum  
A9E4 4A:        LSR:accum  
A9E5 28:        PLP:imp    Pull P
A9E6 20:FC A9   JSR:abs    $A9FC
A9E9 A5:2A      LDA:zp     Zp RAM 002A
A9EB D0:01      BNE:rel    Branch->$A9EE
A9ED 18:        CLC:imp    Clear Carry
A9EE A0:00      LDY:imm    #00
A9F0 B1:3B      LDA:ind,y  3B
A9F2 20:FC A9   JSR:abs    $A9FC
A9F5 C6:3B      DEC:zp     Zp RAM 003B
A9F7 C6:2A      DEC:zp     Zp RAM 002A
A9F9 10:E1      BPL:rel    Branch->$A9DC
A9FB 60:        RTS:imp    Ret from Sub
A9FC 29:0F      AND:imm    #0F
A9FE A8:        TAY:imp    A-->Y
A9FF F0:01      BEQ:rel    Branch->$AA02
AA01 18:        CLC:imp    Clear Carry
AA02 B0:01      BCS:rel    Branch->$AA05
AA04 C8:        INY:imp    Y=Y+1
AA05 08:        PHP:imp    Push P
AA06 98:        TYA:imp    Y-->A
AA07 0A:        ASL:accum  
AA08 A8:        TAY:imp    A-->Y
AA09 B9:E4 31   LDA:abs,y  $31E4,Y
AA0C 9D:60 2F   STA:abs,x  $2F60,X
AA0F E8:        INX:imp    X=X+1
AA10 E8:        INX:imp    X=X+1
AA11 28:        PLP:imp    Pull P
AA12 60:        RTS:imp    Ret from Sub
;
AA13 A6:3E      LDX:zp     Zp RAM 003E
AA15 24:05      BIT:zp     Zp RAM 0005
AA17 30:0A      BMI:rel    Branch->$AA23
AA19 A5:43      LDA:zp     Zp RAM 0043
AA1B 05:44      ORA:zp     Zp RAM 0044
AA1D 05:45      ORA:zp     Zp RAM 0045
AA1F F0:02      BEQ:rel    Branch->$AA23
AA21 A2:01      LDX:imm    #01
AA23 A9:60      LDA:imm    #60
AA25 85:74      STA:zp     Zp RAM 0074
AA27 A9:2F      LDA:imm    #2F
AA29 85:75      STA:zp     Zp RAM 0075
AA2B BD:66 CE   LDA:abs,x  $CE66,X
AA2E A8:        TAY:imp    A-->Y
AA2F 38:        SEC:imp    Set Carry
AA30 65:74      ADC:zp     Zp RAM 0074
AA32 48:        PHA:imp    Push Accum
AA33 B9:E6 CD   LDA:abs,y  $CDE6,Y
AA36 91:74      STA:ind,y  74
AA38 88:        DEY:imp    Y=Y-1
AA39 D0:F8      BNE:rel    Branch->$AA33
AA3B B9:E6 CD   LDA:abs,y  $CDE6,Y
AA3E 91:74      STA:ind,y  74
AA40 A5:05      LDA:zp     Zp RAM 0005
AA42 10:10      BPL:rel    Branch->$AA54
AA44 A9:2F      LDA:imm    #2F
AA46 85:75      STA:zp     Zp RAM 0075
AA48 A9:A6      LDA:imm    #A6
AA4A 85:74      STA:zp     Zp RAM 0074
AA4C A5:9F      LDA:zp     Zp RAM 009F
AA4E 18:        CLC:imp    Clear Carry
AA4F 69:01      ADC:imm    #01
AA51 20:77 AF   JSR:abs    $AF77
AA54 68:        PLA:imp    Pull Accum
AA55 85:74      STA:zp     Zp RAM 0074
AA57 4C:09 DF   JMP:abs    $DF09
AA5A A2:08      LDX:imm    #08
AA5C 20:14 AB   JSR:abs    $AB14
AA5F 4C:69 AA   JMP:abs    $AA69
AA62 A9:30      LDA:imm    #30
AA64 A2:00      LDX:imm    #00
AA66 20:17 AB   JSR:abs    $AB17
AA69 20:92 AA   JSR:abs    $AA92
AA6C 4C:E7 A8   JMP:abs    $A8E7
AA6F 20:B4 A8   JSR:abs    $A8B4
AA72 A9:00      LDA:imm    #00
AA74 A2:06      LDX:imm    #06
AA76 4C:17 AB   JMP:abs    $AB17
AA79 A9:00      LDA:imm    #00
AA7B A2:32      LDX:imm    #32
AA7D 20:17 AB   JSR:abs    $AB17
AA80 A5:03      LDA:zp     Zp RAM 0003
AA82 29:1F      AND:imm    #1F
AA84 C9:10      CMP:imm    #10
AA86 B0:07      BCS:rel    Branch->$AA8F
AA88 A9:E0      LDA:imm    #E0
AA8A A2:22      LDX:imm    #22
AA8C 20:17 AB   JSR:abs    $AB17
AA8F 4C:B4 A8   JMP:abs    $A8B4
AA92 A2:02      LDX:imm    #02
AA94 20:14 AB   JSR:abs    $AB14
AA97 A9:00      LDA:imm    #00
AA99 20:DD B0   JSR:abs    $B0DD
AA9C A6:3D      LDX:zp     Zp RAM 003D
AA9E E8:        INX:imp    X=X+1
AA9F 86:61      STX:zp     Zp RAM 0061
AAA1 A9:61      LDA:imm    #61
AAA3 A0:01      LDY:imm    #01
AAA5 4C:B1 DF   JMP:abs    $DFB1
AAA8 A5:09      LDA:zp     Zp RAM 0009
AAAA 29:03      AND:imm    #03
AAAC AA:        TAX:imp    A-->X
AAAD BD:B0 A8   LDA:abs,x  $A8B0,X
AAB0 AA:        TAX:imp    A-->X
AAB1 20:14 AB   JSR:abs    $AB14
AAB4 CE:6E 01   DEC:abs    $016E
AAB7 A5:0A      LDA:zp     Zp RAM 000A
AAB9 29:01      AND:imm    #01
AABB F0:0E      BEQ:rel    Branch->$AACB
AABD A5:03      LDA:zp     Zp RAM 0003
AABF 29:20      AND:imm    #20
AAC1 D0:08      BNE:rel    Branch->$AACB
AAC3 A2:32      LDX:imm    #32
AAC5 20:14 AB   JSR:abs    $AB14
AAC8 B8:        CLV:imp    Clear Overflow
AAC9 50:03      BVC:rel    Branch->$AACE
AACB 20:CA AE   JSR:abs    $AECA
AACE A2:2C      LDX:imm    #2C
AAD0 20:14 AB   JSR:abs    $AB14
AAD3 A2:2E      LDX:imm    #2E
AAD5 20:14 AB   JSR:abs    $AB14
AAD8 A5:06      LDA:zp     Zp RAM 0006	 ; If the number of game credits
AADA C9:28      CMP:imm    #28		 ; exceed $28 (40), then reduce it
AADC 90:04      BCC:rel    Branch->$AAE2 ; back down to 40.
AADE A9:28      LDA:imm    #28		 ;
AAE0 85:06      STA:zp     Zp RAM 0006	 ;
AAE2 20:77 AF   JSR:abs    $AF77
AAE5 A5:17      LDA:zp     Zp RAM 0017
AAE7 F0:09      BEQ:rel    Branch->$AAF2
AAE9 AD:F4 AA   LDA:abs    $AAF4
AAEC AE:F3 AA   LDX:abs    $AAF3
AAEF 20:39 DF   JSR:abs    $DF39
AAF2 60:        RTS:imp    Ret from Sub
;
AAF3 5C:        Illegal Opcode
AAF4 32:        Illegal Opcode
AAF5 F8:        SED:imp    Set Decimal
AAF6 85:29      STA:zp     Zp RAM 0029
AAF8 A9:00      LDA:imm    #00
AAFA 85:2C      STA:zp     Zp RAM 002C
AAFC A0:07      LDY:imm    #07
AAFE 06:29      ASL:zp     Zp RAM 0029
AB00 A5:2C      LDA:zp     Zp RAM 002C
AB02 65:2C      ADC:zp     Zp RAM 002C
AB04 85:2C      STA:zp     Zp RAM 002C
AB06 88:        DEY:imp    Y=Y-1
AB07 10:F5      BPL:rel    Branch->$AAFE
AB09 D8:        CLD:imp    Unset Decimal
AB0A 85:29      STA:zp     Zp RAM 0029
AB0C 60:        RTS:imp    Ret from Sub
;
AB0D A9:20      LDA:imm    #20
AB0F A2:80      LDX:imm    #80
AB11 4C:57 DF   JMP:abs    $DF57
AB14 BD:22 D1   LDA:abs,x  $D122,X
AB17 86:35      STX:zp     Zp RAM 0035
AB19 85:2B      STA:zp     Zp RAM 002B
AB1B A4:35      LDY:zp     Zp RAM 0035
AB1D B1:AC      LDA:ind,y  AC
AB1F 85:3B      STA:zp     Zp RAM 003B
AB21 C8:        INY:imp    Y=Y+1
AB22 B1:AC      LDA:ind,y  AC
AB24 85:3C      STA:zp     Zp RAM 003C
AB26 E0:2C      CPX:imm    #2C
AB28 D0:08      BNE:rel    Branch->$AB32
AB2A A5:74      LDA:zp     Zp RAM 0074
AB2C 85:B6      STA:zp     Zp RAM 00B6
AB2E A5:75      LDA:zp     Zp RAM 0075
AB30 85:B7      STA:zp     Zp RAM 00B7
AB32 A0:00      LDY:imm    #00
AB34 B1:3B      LDA:ind,y  3B
AB36 85:2A      STA:zp     Zp RAM 002A
AB38 20:0D AB   JSR:abs    $AB0D
AB3B A9:00      LDA:imm    #00
AB3D 85:73      STA:zp     Zp RAM 0073
AB3F A9:01      LDA:imm    #01
AB41 85:72      STA:zp     Zp RAM 0072
AB43 20:6A DF   JSR:abs    $DF6A
AB46 A5:2A      LDA:zp     Zp RAM 002A
AB48 A6:2B      LDX:zp     Zp RAM 002B
AB4A 20:75 DF   JSR:abs    $DF75
AB4D A4:35      LDY:zp     Zp RAM 0035
AB4F B1:AC      LDA:ind,y  AC
AB51 85:3B      STA:zp     Zp RAM 003B
AB53 C8:        INY:imp    Y=Y+1
AB54 B1:AC      LDA:ind,y  AC
AB56 85:3C      STA:zp     Zp RAM 003C
AB58 A6:35      LDX:zp     Zp RAM 0035
AB5A BD:21 D1   LDA:abs,x  $D121,X
AB5D 48:        PHA:imp    Push Accum
AB5E 4A:        LSR:accum  
AB5F 4A:        LSR:accum  
AB60 4A:        LSR:accum  
AB61 4A:        LSR:accum  
AB62 A8:        TAY:imp    A-->Y
AB63 20:D1 B0   JSR:abs    $B0D1
AB66 68:        PLA:imp    Pull Accum
AB67 29:0F      AND:imm    #0F
AB69 20:DD B0   JSR:abs    $B0DD
AB6C A0:01      LDY:imm    #01
AB6E A9:00      LDA:imm    #00
AB70 85:2A      STA:zp     Zp RAM 002A
AB72 B1:3B      LDA:ind,y  3B
AB74 85:2B      STA:zp     Zp RAM 002B
AB76 29:7F      AND:imm    #7F
AB78 C8:        INY:imp    Y=Y+1
AB79 84:2C      STY:zp     Zp RAM 002C
AB7B AA:        TAX:imp    A-->X
AB7C BD:E4 31   LDA:abs,x  $31E4,X
AB7F A4:2A      LDY:zp     Zp RAM 002A
AB81 91:74      STA:ind,y  74
AB83 C8:        INY:imp    Y=Y+1
AB84 BD:E5 31   LDA:abs,x  $31E5,X
AB87 91:74      STA:ind,y  74
AB89 C8:        INY:imp    Y=Y+1
AB8A 84:2A      STY:zp     Zp RAM 002A
AB8C A4:2C      LDY:zp     Zp RAM 002C
AB8E 24:2B      BIT:zp     Zp RAM 002B
AB90 10:E0      BPL:rel    Branch->$AB72
AB92 A4:2A      LDY:zp     Zp RAM 002A
AB94 88:        DEY:imp    Y=Y-1
AB95 4C:5F DF   JMP:abs    $DF5F
AB98 86:35      STX:zp     Zp RAM 0035
AB9A 85:2A      STA:zp     Zp RAM 002A
AB9C A9:00      LDA:imm    #00
AB9E 85:2B      STA:zp     Zp RAM 002B
ABA0 F0:99      BEQ:rel    Branch->$AB3B
ABA2 20:20 AC   JSR:abs    $AC20
ABA5 AD:C9 01   LDA:abs    $01C9
ABA8 29:03      AND:imm    #03
ABAA F0:5B      BEQ:rel    Branch->$AC07
ABAC 20:20 AC   JSR:abs    $AC20
ABAF A9:08      LDA:imm    #08
ABB1 8D:00 01   STA:abs    $0100
ABB4 AD:1B 07   LDA:abs    $071B	 ; Is the highest score 000000?
ABB7 0D:1C 07   ORA:abs    $071C	 ;
ABBA 0D:1D 07   ORA:abs    $071D	 ;
ABBD D0:03      BNE:rel    Branch->$ABC2 ;
ABBF 20:36 AC   JSR:abs    $AC36	 ; If so, JSR to $AC36.
ABC2 A2:17      LDX:imm    #17
ABC4 AD:C9 01   LDA:abs    $01C9
ABC7 29:01      AND:imm    #01
ABC9 D0:02      BNE:rel    Branch->$ABCD
ABCB A2:0E      LDX:imm    #0E		 ; Load from ROM the initials of
ABCD BD:08 AC   LDA:abs,x  $AC08,X	 ; the bottom 5 high score into the
ABD0 9D:06 06   STA:abs,x  $0606,X	 ; high score list.
ABD3 CA:        DEX:imp    X=X-1	 ;
ABD4 10:F7      BPL:rel    Branch->$ABCD ;
ABD6 A2:17      LDX:imm    #17
ABD8 AD:C9 01   LDA:abs    $01C9
ABDB 29:02      AND:imm    #02
ABDD D0:02      BNE:rel    Branch->$ABE1
ABDF A2:0E      LDX:imm    #0E		 ; Set the last five high scores
ABE1 A9:01      LDA:imm    #01		 ; to "10101".
ABE3 9D:06 07   STA:abs,x  $0706,X	 ;
ABE6 CA:        DEX:imp    X=X-1	 ;
ABE7 10:F8      BPL:rel    Branch->$ABE1 ;
ABE9 AD:C9 01   LDA:abs    $01C9
ABEC 29:03      AND:imm    #03
ABEE F0:0F      BEQ:rel    Branch->$ABFF
ABF0 A5:0A      LDA:zp     Zp RAM 000A
ABF2 29:F8      AND:imm    #F8
ABF4 8D:1E 07   STA:abs    $071E
ABF7 AD:6A 01   LDA:abs    $016A
ABFA 29:03      AND:imm    #03
ABFC 8D:1F 07   STA:abs    $071F
ABFF AD:C9 01   LDA:abs    $01C9
AC02 29:FC      AND:imm    #FC
AC04 8D:C9 01   STA:abs    $01C9
AC07 60:        RTS:imp    Ret from Sub
;
AC08 07:04:01   DATA		; BEH	Initials to place in the the
AC0B 0F:09:0C   DATA		; MJP	lowest five slots of the high
AC0E 0B:03:12   DATA		; SDL	high score table. (Always done
AC11 13:05:03   DATA		; DFT	upon power-up.)
AC14 07:0F:0C	DATA		; MPH	0=A 1=B 2=C ... 24=Y 25=Z 26=SPACE
;
AC17 11:11:11   DATA		; RRR	Top three high scores to be used
AC1A 12:04:03   DATA		; DES	when the EAROM's high score list has
AC1D 03:09:04   DATA		; EJD	been zero'd out.
;
AC1E 09:04      ORA:imm    #04  	 ; Fake command or not used?
;
AC20 20:BB D6   JSR:abs    $D6BB
AC23 A5:0A      LDA:zp     Zp RAM 000A
AC25 29:F8      AND:imm    #F8
AC27 CD:1E 07   CMP:abs    $071E
AC2A D0:08      BNE:rel    Branch->$AC34
AC2C AD:6A 01   LDA:abs    $016A
AC2F 29:03      AND:imm    #03
AC31 CD:1F 07   CMP:abs    $071F
AC34 F0:08      BEQ:rel    Branch->$AC3E
;
AC36 AD:C9 01   LDA:abs    $01C9
AC39 09:03      ORA:imm    #03
AC3B 8D:C9 01   STA:abs    $01C9
AC3E 60:        RTS:imp    Ret from Sub
;
AC3F A5:05      LDA:zp     Zp RAM 0005
AC41 29:BF      AND:imm    #BF
AC43 85:05      STA:zp     Zp RAM 0005
AC45 A5:09      LDA:zp     Zp RAM 0009
AC47 29:43      AND:imm    #43
AC49 C9:40      CMP:imm    #40
AC4B D0:03      BNE:rel    Branch->$AC50
AC4D 20:62 CA   JSR:abs    $CA62	 ; Clear out the player 1&2 scores.
AC50 20:FB DD   JSR:abs    $DDFB
AC53 A9:00      LDA:imm    #00
AC55 8D:01 06   STA:abs    $0601
AC58 A6:3E      LDX:zp     Zp RAM 003E
AC5A F0:02      BEQ:rel    Branch->$AC5E
AC5C A2:03      LDX:imm    #03
AC5E B5:42      LDA:zp,x   Zp RAM 0042
AC60 85:2C      STA:zp     Zp RAM 002C
AC62 B5:41      LDA:zp,x   Zp RAM 0041
AC64 85:2D      STA:zp     Zp RAM 002D
AC66 B5:40      LDA:zp,x   Zp RAM 0040
AC68 85:2E      STA:zp     Zp RAM 002E
AC6A 8A:        TXA:imp    X-->A
AC6B 29:01      AND:imm    #01
AC6D 85:36      STA:zp     Zp RAM 0036
AC6F A9:00      LDA:imm    #00
AC71 85:2B      STA:zp     Zp RAM 002B
AC73 A9:1A      LDA:imm    #1A
AC75 85:2A      STA:zp     Zp RAM 002A
AC77 85:29      STA:zp     Zp RAM 0029
AC79 A9:00      LDA:imm    #00
AC7B 8D:05 06   STA:abs    $0605
AC7E A0:FD      LDY:imm    #FD
AC80 B9:20 06   LDA:abs,y  $0620,Y
AC83 C5:2C      CMP:zp     Zp RAM 002C
AC85 D0:14      BNE:rel    Branch->$AC9B
AC87 B9:1F 06   LDA:abs,y  $061F,Y
AC8A C5:2D      CMP:zp     Zp RAM 002D
AC8C D0:0D      BNE:rel    Branch->$AC9B
AC8E C0:52      CPY:imm    #52
AC90 90:08      BCC:rel    Branch->$AC9A
AC92 B9:1E 06   LDA:abs,y  $061E,Y
AC95 C5:2E      CMP:zp     Zp RAM 002E
AC97 B8:        CLV:imp    Clear Overflow
AC98 50:01      BVC:rel    Branch->$AC9B
AC9A 38:        SEC:imp    Set Carry
AC9B B0:4F      BCS:rel    Branch->$ACEC
AC9D C0:E8      CPY:imm    #E8
AC9F 90:1E      BCC:rel    Branch->$ACBF
ACA1 A5:29      LDA:zp     Zp RAM 0029
ACA3 BE:1E 05   LDX:abs,y  $051E,Y	 ; High score sort routine
ACA6 99:1E 05   STA:abs,y  $051E,Y	 ; for scores and initials.
ACA9 86:29      STX:zp     Zp RAM 0029	 ; Starts here and extends down.
ACAB A5:2A      LDA:zp     Zp RAM 002A
ACAD BE:1F 05   LDX:abs,y  $051F,Y
ACB0 99:1F 05   STA:abs,y  $051F,Y
ACB3 86:2A      STX:zp     Zp RAM 002A
ACB5 A5:2B      LDA:zp     Zp RAM 002B
ACB7 BE:20 05   LDX:abs,y  $0520,Y
ACBA 99:20 05   STA:abs,y  $0520,Y
ACBD 86:2B      STX:zp     Zp RAM 002B
ACBF A5:2D      LDA:zp     Zp RAM 002D
ACC1 BE:1F 06   LDX:abs,y  $061F,Y
ACC4 99:1F 06   STA:abs,y  $061F,Y
ACC7 86:2D      STX:zp     Zp RAM 002D
ACC9 A5:2C      LDA:zp     Zp RAM 002C
ACCB BE:20 06   LDX:abs,y  $0620,Y
ACCE 99:20 06   STA:abs,y  $0620,Y
ACD1 86:2C      STX:zp     Zp RAM 002C
ACD3 C0:52      CPY:imm    #52
ACD5 90:0A      BCC:rel    Branch->$ACE1
ACD7 A5:2E      LDA:zp     Zp RAM 002E
ACD9 BE:1E 06   LDX:abs,y  $061E,Y
ACDC 99:1E 06   STA:abs,y  $061E,Y
ACDF 86:2E      STX:zp     Zp RAM 002E
ACE1 C0:55      CPY:imm    #55
ACE3 90:01      BCC:rel    Branch->$ACE6
ACE5 88:        DEY:imp    Y=Y-1
ACE6 88:        DEY:imp    Y=Y-1
ACE7 88:        DEY:imp    Y=Y-1
ACE8 D0:B3      BNE:rel    Branch->$AC9D
ACEA A0:02      LDY:imm    #02
ACEC EE:05 06   INC:abs    $0605
ACEF C0:55      CPY:imm    #55
ACF1 90:01      BCC:rel    Branch->$ACF4
ACF3 88:        DEY:imp    Y=Y-1
ACF4 88:        DEY:imp    Y=Y-1
ACF5 88:        DEY:imp    Y=Y-1
ACF6 D0:88      BNE:rel    Branch->$AC80
ACF8 A6:36      LDX:zp     Zp RAM 0036
ACFA AD:05 06   LDA:abs    $0605
ACFD 9D:00 06   STA:abs,x  $0600,X
AD00 CA:        DEX:imp    X=X-1
AD01 30:03      BMI:rel    Branch->$AD06
AD03 4C:5E AC   JMP:abs    $AC5E
AD06 AD:01 06   LDA:abs    $0601
AD09 CD:00 06   CMP:abs    $0600
AD0C 90:07      BCC:rel    Branch->$AD15
AD0E C9:63      CMP:imm    #63
AD10 B0:03      BCS:rel    Branch->$AD15
AD12 EE:01 06   INC:abs    $0601
AD15 A5:3D      LDA:zp     Zp RAM 003D
AD17 49:01      EOR:imm    #01
AD19 0A:        ASL:accum  
AD1A 0A:        ASL:accum  
AD1B 05:3D      ORA:zp     Zp RAM 003D
AD1D 69:05      ADC:imm    #05
AD1F 8D:03 06   STA:abs    $0603
AD22 A0:14      LDY:imm    #14
AD24 AD:03 06   LDA:abs    $0603
AD27 F0:42      BEQ:rel    Branch->$AD6B
AD29 29:03      AND:imm    #03
AD2B 85:3D      STA:zp     Zp RAM 003D
AD2D C6:3D      DEC:zp     Zp RAM 003D
AD2F 4E:03 06   LSR:abs    $0603
AD32 4E:03 06   LSR:abs    $0603
AD35 A6:3D      LDX:zp     Zp RAM 003D
AD37 BD:00 06   LDA:abs,x  $0600,X
AD3A F0:2C      BEQ:rel    Branch->$AD68
AD3C C9:09      CMP:imm    #09
AD3E B0:28      BCS:rel    Branch->$AD68
AD40 0A:        ASL:accum  
AD41 18:        CLC:imp    Clear Carry
AD42 7D:00 06   ADC:abs,x  $0600,X
AD45 49:FF      EOR:imm    #FF
AD47 38:        SEC:imp    Set Carry
AD48 E9:E5      SBC:imm    #E5
AD4A 8D:02 06   STA:abs    $0602
AD4D 20:48 CA   JSR:abs    $CA48
AD50 A9:60      LDA:imm    #60		 ; Set high score entry timer
AD52 8D:05 06   STA:abs    $0605	 ; to $60 seconds. Yeah. In HEX.
AD55 A9:00      LDA:imm    #00           ; Reset the list of buttons to
AD57 85:4E      STA:zp     Zp RAM 004E   ; process and the relative change
AD59 85:50      STA:zp     Zp RAM 0050   ; in spinner position.
AD5B A9:02      LDA:imm    #02
AD5D 8D:04 06   STA:abs    $0604
AD60 20:89 A7   JSR:abs    $A789
AD63 A0:24      LDY:imm    #24		 ; Set game mode to the post-game
AD65 84:00      STY:zp     Zp RAM 0000	 ; "victory explosion" for high score
AD67 60:        RTS:imp    Ret from Sub
AD68 4C:22 AD   JMP:abs    $AD22
AD6B 84:00      STY:zp     Zp RAM 0000
AD6D 60:        RTS:imp    Ret from Sub
;
AD6E A9:06      LDA:imm    #06		; Set game mode to the high score
AD70 85:01      STA:zp     Zp RAM 0001  ; input screen.
AD72 A5:03      LDA:zp     Zp RAM 0003
AD74 29:1F      AND:imm    #1F
AD76 D0:0A      BNE:rel    Branch->$AD82
AD78 CE:05 06   DEC:abs    $0605	 ; Decrease the timer.
AD7B D0:05      BNE:rel    Branch->$AD82
AD7D A0:14      LDY:imm    #14
AD7F 84:00      STY:zp     Zp RAM 0000
AD81 60:        RTS:imp    Ret from Sub
AD82 AE:02 06   LDX:abs    $0602
AD85 BD:06 06   LDA:abs,x  $0606,X
AD88 20:CE AD   JSR:abs    $ADCE
AD8B A8:        TAY:imp    A-->Y
AD8C 10:05      BPL:rel    Branch->$AD93
AD8E A9:1A      LDA:imm    #1A
AD90 B8:        CLV:imp    Clear Overflow
AD91 50:06      BVC:rel    Branch->$AD99
AD93 C9:1B      CMP:imm    #1B
AD95 90:02      BCC:rel    Branch->$AD99
AD97 A9:00      LDA:imm    #00
AD99 AE:02 06   LDX:abs    $0602
AD9C 9D:06 06   STA:abs,x  $0606,X
AD9F A5:4E      LDA:zp     Zp RAM 004E
ADA1 29:18      AND:imm    #18
ADA3 A8:        TAY:imp    A-->Y
ADA4 A5:4E      LDA:zp     Zp RAM 004E
ADA6 29:67      AND:imm    #67
ADA8 85:4E      STA:zp     Zp RAM 004E
ADAA 98:        TYA:imp    Y-->A
ADAB F0:20      BEQ:rel    Branch->$ADCD
ADAD CE:02 06   DEC:abs    $0602
ADB0 CE:04 06   DEC:abs    $0604
ADB3 10:12      BPL:rel    Branch->$ADC7
ADB5 A6:3D      LDX:zp     Zp RAM 003D
ADB7 BD:00 06   LDA:abs,x  $0600,X
ADBA C9:04      CMP:imm    #04
ADBC B0:03      BCS:rel    Branch->$ADC1
ADBE 20:F7 DD   JSR:abs    $DDF7
ADC1 20:22 AD   JSR:abs    $AD22
ADC4 B8:        CLV:imp    Clear Overflow
ADC5 50:06      BVC:rel    Branch->$ADCD
ADC7 CA:        DEX:imp    X=X-1
ADC8 A9:00      LDA:imm    #00
ADCA 9D:06 06   STA:abs,x  $0606,X
ADCD 60:        RTS:imp    Ret from Sub
ADCE 48:        PHA:imp    Push Accum
ADCF A5:50      LDA:zp     Zp RAM 0050
ADD1 0A:        ASL:accum  
ADD2 0A:        ASL:accum  
ADD3 0A:        ASL:accum  
ADD4 18:        CLC:imp    Clear Carry
ADD5 65:51      ADC:zp     Zp RAM 0051
ADD7 85:51      STA:zp     Zp RAM 0051
ADD9 68:        PLA:imp    Pull Accum
ADDA A4:50      LDY:zp     Zp RAM 0050
ADDC 30:05      BMI:rel    Branch->$ADE3
ADDE 69:00      ADC:imm    #00
ADE0 B8:        CLV:imp    Clear Overflow
ADE1 50:02      BVC:rel    Branch->$ADE5
ADE3 69:FF      ADC:imm    #FF
ADE5 A0:00      LDY:imm    #00
ADE7 84:50      STY:zp     Zp RAM 0050
ADE9 60:        RTS:imp    Ret from Sub
ADEA 20:B4 A8   JSR:abs    $A8B4
ADED A9:C0      LDA:imm    #C0
ADEF A2:02      LDX:imm    #02
ADF1 20:17 AB   JSR:abs    $AB17
ADF4 CE:6E 01   DEC:abs    $016E
ADF7 20:97 AA   JSR:abs    $AA97
ADFA A2:0A      LDX:imm    #0A
ADFC 20:14 AB   JSR:abs    $AB14
ADFF A9:A6      LDA:imm    #A6
AE01 A2:0C      LDX:imm    #0C
AE03 20:17 AB   JSR:abs    $AB17
AE06 A9:9C      LDA:imm    #9C
AE08 A2:0E      LDX:imm    #0E
AE0A 20:17 AB   JSR:abs    $AB17
AE0D A2:2C      LDX:imm    #2C
AE0F 20:14 AB   JSR:abs    $AB14
AE12 AD:02 06   LDA:abs    $0602
AE15 38:        SEC:imp    Set Carry
AE16 ED:04 06   SBC:abs    $0604
AE19 4C:4E AE   JMP:abs    $AE4E
AE1C 20:B4 A8   JSR:abs    $A8B4
AE1F 78:        SEI:imp    Disable IRQ	 ; This is the start of a very
					 ; clever routine for Tempest
					 ; Pokey chip protection. The
					 ; interrupts are disabled to
					 ; preserve timing. From the
					 ; POKEY chip emulator 4.5 --

 ; 4.5: changed the 9/17 bit polynomial formulas such that the
 ; values required for the Tempest Pokey protection will be found.
 ; Tempest expects the upper 4 bits of the RNG to appear in the
 ; lower 4 bits after four cycles, so there has to be a shift
 ; of 1 per cycle (which was not the case before). Bits #6-#13 of
 ; the new RNG give this expected result now, bits #0-7 of the 9 bit
 ; poly.
					

AE20 AD:CA 60   LDA:abs    $60CA	 ; Randomize A register.
AE23 AC:CA 60   LDY:abs    $60CA	 ; Randomize Y register.
AE26 84:29      STY:zp     Zp RAM 0029
AE28 4A:        LSR:accum  
AE29 4A:        LSR:accum  
AE2A 4A:        LSR:accum  
AE2B 4A:        LSR:accum  
AE2C 45:29      EOR:zp     Zp RAM 0029
AE2E 85:29      STA:zp     Zp RAM 0029
AE30 AD:DA 60   LDA:abs    $60DA	 ; Randomize A register.
AE33 AC:DA 60   LDY:abs    $60DA	 ; Randomize Y register.
AE36 58:        CLI:imp    Enable IRQ
AE37 45:29      EOR:zp     Zp RAM 0029
AE39 29:F0      AND:imm    #F0
AE3B 45:29      EOR:zp     Zp RAM 0029
AE3D 85:29      STA:zp     Zp RAM 0029
AE3F 98:        TYA:imp    Y-->A
AE40 0A:        ASL:accum  
AE41 0A:        ASL:accum  
AE42 0A:        ASL:accum  
AE43 0A:        ASL:accum  
AE44 45:29      EOR:zp     Zp RAM 0029
AE46 8D:1F 01   STA:abs    $011F

AE49 20:26 AF   JSR:abs    $AF26
AE4C A9:FF      LDA:imm    #FF
AE4E 85:63      STA:zp     Zp RAM 0063
AE50 A2:10      LDX:imm    #10
AE52 20:14 AB   JSR:abs    $AB14
AE55 A9:01      LDA:imm    #01
AE57 85:61      STA:zp     Zp RAM 0061
AE59 20:DD B0   JSR:abs    $B0DD
AE5C A9:28      LDA:imm    #28
AE5E 85:2C      STA:zp     Zp RAM 002C
AE60 A2:15      LDX:imm    #15
AE62 86:37      STX:zp     Zp RAM 0037
AE64 20:0D AB   JSR:abs    $AB0D
AE67 A9:00      LDA:imm    #00
AE69 85:73      STA:zp     Zp RAM 0073
AE6B A5:2C      LDA:zp     Zp RAM 002C
AE6D AA:        TAX:imp    A-->X
AE6E 38:        SEC:imp    Set Carry
AE6F E9:0A      SBC:imm    #0A
AE71 85:2C      STA:zp     Zp RAM 002C
AE73 A9:D0      LDA:imm    #D0
AE75 20:75 DF   JSR:abs    $DF75
AE78 A0:07      LDY:imm    #07
AE7A A5:63      LDA:zp     Zp RAM 0063
AE7C C5:37      CMP:zp     Zp RAM 0037
AE7E D0:02      BNE:rel    Branch->$AE82
AE80 A0:00      LDY:imm    #00
AE82 20:D1 B0   JSR:abs    $B0D1
AE85 A9:61      LDA:imm    #61
AE87 A0:01      LDY:imm    #01
AE89 20:B1 DF   JSR:abs    $DFB1
AE8C A9:A0      LDA:imm    #A0
AE8E 20:6A B5   JSR:abs    $B56A
AE91 A9:00      LDA:imm    #00
AE93 85:73      STA:zp     Zp RAM 0073
AE95 AA:        TAX:imp    A-->X
AE96 A9:08      LDA:imm    #08
AE98 20:75 DF   JSR:abs    $DF75
AE9B E6:61      INC:zp     Zp RAM 0061
AE9D A5:37      LDA:zp     Zp RAM 0037
AE9F 20:F8 AE   JSR:abs    $AEF8
AEA2 A2:00      LDX:imm    #00
AEA4 A9:08      LDA:imm    #08
AEA6 20:75 DF   JSR:abs    $DF75
AEA9 A6:37      LDX:zp     Zp RAM 0037
AEAB BD:06 07   LDA:abs,x  $0706,X
AEAE 85:56      STA:zp     Zp RAM 0056
AEB0 BD:07 07   LDA:abs,x  $0707,X
AEB3 85:57      STA:zp     Zp RAM 0057
AEB5 BD:08 07   LDA:abs,x  $0708,X
AEB8 85:58      STA:zp     Zp RAM 0058
AEBA A9:56      LDA:imm    #56
AEBC A0:03      LDY:imm    #03
AEBE 20:B1 DF   JSR:abs    $DFB1
AEC1 C6:37      DEC:zp     Zp RAM 0037
AEC3 C6:37      DEC:zp     Zp RAM 0037
AEC5 C6:37      DEC:zp     Zp RAM 0037
AEC7 10:9B      BPL:rel    Branch->$AE64
AEC9 60:        RTS:imp    Ret from Sub
AECA AD:56 01   LDA:abs    $0156
AECD F0:14      BEQ:rel    Branch->$AEE3
AECF 85:58      STA:zp     Zp RAM 0058
AED1 A2:34      LDX:imm    #34
AED3 20:14 AB   JSR:abs    $AB14
AED6 A9:00      LDA:imm    #00
AED8 85:56      STA:zp     Zp RAM 0056
AEDA 85:57      STA:zp     Zp RAM 0057
AEDC A9:56      LDA:imm    #56
AEDE A0:03      LDY:imm    #03
AEE0 20:B1 DF   JSR:abs    $DFB1
AEE3 18:        CLC:imp    Clear Carry
AEE4 A0:10      LDY:imm    #10
AEE6 A9:85      LDA:imm    #85
AEE8 79:75 D5   ADC:abs,y  $D575,Y
AEEB 88:        DEY:imp    Y=Y-1
AEEC 10:FA      BPL:rel    Branch->$AEE8
AEEE 85:B5      STA:zp     Zp RAM 00B5
AEF0 60:        RTS:imp    Ret from Sub
AEF1 AD:02 06   LDA:abs    $0602
AEF4 38:        SEC:imp    Set Carry
AEF5 ED:04 06   SBC:abs    $0604
AEF8 18:        CLC:imp    Clear Carry
AEF9 69:02      ADC:imm    #02
AEFB 85:38      STA:zp     Zp RAM 0038
AEFD A0:00      LDY:imm    #00
AEFF A9:02      LDA:imm    #02
AF01 85:39      STA:zp     Zp RAM 0039
AF03 A6:38      LDX:zp     Zp RAM 0038
AF05 BD:06 06   LDA:abs,x  $0606,X
AF08 C9:1E      CMP:imm    #1E
AF0A 90:02      BCC:rel    Branch->$AF0E
AF0C A9:1A      LDA:imm    #1A
AF0E 0A:        ASL:accum  
AF0F AA:        TAX:imp    A-->X
AF10 BD:FA 31   LDA:abs,x  $31FA,X
AF13 91:74      STA:ind,y  74
AF15 C8:        INY:imp    Y=Y+1
AF16 BD:FB 31   LDA:abs,x  $31FB,X
AF19 91:74      STA:ind,y  74
AF1B C8:        INY:imp    Y=Y+1
AF1C C6:38      DEC:zp     Zp RAM 0038
AF1E C6:39      DEC:zp     Zp RAM 0039
AF20 10:E1      BPL:rel    Branch->$AF03
AF22 88:        DEY:imp    Y=Y-1
AF23 4C:5F DF   JMP:abs    $DF5F
AF26 AD:00 06   LDA:abs    $0600
AF29 0D:01 06   ORA:abs    $0601
AF2C F0:40      BEQ:rel    Branch->$AF6E
AF2E A2:12      LDX:imm    #12
AF30 20:14 AB   JSR:abs    $AB14
AF33 A9:63      LDA:imm    #63
AF35 20:71 AF   JSR:abs    $AF71
AF38 A2:00      LDX:imm    #00
AF3A 20:3F AF   JSR:abs    $AF3F
AF3D A2:01      LDX:imm    #01
AF3F BD:00 06   LDA:abs,x  $0600,X
AF42 F0:2A      BEQ:rel    Branch->$AF6E
AF44 48:        PHA:imp    Push Accum
AF45 86:2E      STX:zp     Zp RAM 002E
AF47 A0:03      LDY:imm    #03
AF49 20:D1 B0   JSR:abs    $B0D1
AF4C 20:0D AB   JSR:abs    $AB0D
AF4F A9:D0      LDA:imm    #D0
AF51 A4:2E      LDY:zp     Zp RAM 002E
AF53 BE:6F AF   LDX:abs,y  $AF6F,Y
AF56 20:75 DF   JSR:abs    $DF75
AF59 68:        PLA:imp    Pull Accum
AF5A 20:71 AF   JSR:abs    $AF71
AF5D A9:A0      LDA:imm    #A0
AF5F 20:6A B5   JSR:abs    $B56A
AF62 A9:10      LDA:imm    #10
AF64 A2:04      LDX:imm    #04
AF66 20:98 AB   JSR:abs    $AB98
AF69 A6:2E      LDX:zp     Zp RAM 002E
AF6B 20:9E AA   JSR:abs    $AA9E
AF6E 60:        RTS:imp    Ret from Sub
AF6F C0:B0      CPY:imm    #B0
AF71 C9:63      CMP:imm    #63
AF73 90:02      BCC:rel    Branch->$AF77
AF75 A9:63      LDA:imm    #63
AF77 20:F5 AA   JSR:abs    $AAF5
AF7A A9:29      LDA:imm    #29
AF7C A0:01      LDY:imm    #01
AF7E 4C:B1 DF   JMP:abs    $DFB1
AF81 20:48 CA   JSR:abs    $CA48
AF84 CE:6E 01   DEC:abs    $016E
AF87 A0:03      LDY:imm    #03
AF89 20:D1 B0   JSR:abs    $B0D1
AF8C A9:01      LDA:imm    #01
AF8E 85:72      STA:zp     Zp RAM 0072
AF90 20:6A DF   JSR:abs    $DF6A
AF93 A2:2C      LDX:imm    #2C
AF95 A9:60      LDA:imm    #60
AF97 20:17 AB   JSR:abs    $AB17
AF9A 20:92 AA   JSR:abs    $AA92
AF9D A2:07      LDX:imm    #07
AF9F 86:37      STX:zp     Zp RAM 0037
AFA1 A4:37      LDY:zp     Zp RAM 0037
AFA3 BE:9B B0   LDX:abs,y  $B09B,Y
AFA6 20:14 AB   JSR:abs    $AB14
AFA9 C6:37      DEC:zp     Zp RAM 0037
AFAB 10:F4      BPL:rel    Branch->$AFA1
AFAD AD:00 02   LDA:abs    $0200
AFB0 38:        SEC:imp    Set Carry
AFB1 E5:7B      SBC:zp     Zp RAM 007B
AFB3 10:07      BPL:rel    Branch->$AFBC
AFB5 C6:7B      DEC:zp     Zp RAM 007B
AFB7 C6:7C      DEC:zp     Zp RAM 007C
AFB9 B8:        CLV:imp    Clear Overflow
AFBA 50:25      BVC:rel    Branch->$AFE1
AFBC D0:0D      BNE:rel    Branch->$AFCB
AFBE C6:7C      DEC:zp     Zp RAM 007C
AFC0 C6:7B      DEC:zp     Zp RAM 007B
AFC2 10:04      BPL:rel    Branch->$AFC8
AFC4 E6:7B      INC:zp     Zp RAM 007B
AFC6 E6:7C      INC:zp     Zp RAM 007C
AFC8 B8:        CLV:imp    Clear Overflow
AFC9 50:16      BVC:rel    Branch->$AFE1
AFCB A5:7C      LDA:zp     Zp RAM 007C
AFCD CD:27 01   CMP:abs    $0127
AFD0 F0:02      BEQ:rel    Branch->$AFD4
AFD2 B0:0D      BCS:rel    Branch->$AFE1
AFD4 38:        SEC:imp    Set Carry
AFD5 ED:00 02   SBC:abs    $0200
AFD8 D0:01      BNE:rel    Branch->$AFDB
AFDA 18:        CLC:imp    Clear Carry
AFDB B0:04      BCS:rel    Branch->$AFE1
AFDD E6:7B      INC:zp     Zp RAM 007B
AFDF E6:7C      INC:zp     Zp RAM 007C
AFE1 A5:7C      LDA:zp     Zp RAM 007C
AFE3 85:3A      STA:zp     Zp RAM 003A
AFE5 A2:04      LDX:imm    #04
AFE7 86:37      STX:zp     Zp RAM 0037
AFE9 A0:05      LDY:imm    #05
AFEB 20:D1 B0   JSR:abs    $B0D1
AFEE A9:00      LDA:imm    #00
AFF0 85:73      STA:zp     Zp RAM 0073
AFF2 20:0D AB   JSR:abs    $AB0D
AFF5 A2:D8      LDX:imm    #D8
AFF7 A4:37      LDY:zp     Zp RAM 0037
AFF9 B9:96 B0   LDA:abs,y  $B096,Y
AFFC 18:        CLC:imp    Clear Carry
AFFD 69:F8      ADC:imm    #F8
;
; Start of ROM 136002.217 at $B000.   	NOTE: VERSION 2 ROMS, FOLKS.
;
AFFF 20:75 DF   JSR:abs    $DF75
B002 A6:3A      LDX:zp     Zp RAM 003A
B004 BC:FE 91   LDY:abs,x  $91FE,X
B007 C0:63      CPY:imm    #63
B009 B0:37      BCS:rel    Branch->$B042
B00B C8:        INY:imp    Y=Y+1
B00C 98:        TYA:imp    Y-->A
B00D 20:77 AF   JSR:abs    $AF77
B010 A0:03      LDY:imm    #03
B012 20:D1 B0   JSR:abs    $B0D1
B015 20:0D AB   JSR:abs    $AB0D
B018 A2:BA      LDX:imm    #BA
B01A A4:37      LDY:zp     Zp RAM 0037
B01C B9:96 B0   LDA:abs,y  $B096,Y
B01F 18:        CLC:imp    Clear Carry
B020 69:EC      ADC:imm    #EC
B022 20:75 DF   JSR:abs    $DF75
B025 A6:3A      LDX:zp     Zp RAM 003A
B027 20:C6 B0   JSR:abs    $B0C6
B02A 20:0D AB   JSR:abs    $AB0D
B02D A2:CC      LDX:imm    #CC
B02F A4:37      LDY:zp     Zp RAM 0037
B031 B9:96 B0   LDA:abs,y  $B096,Y
B034 18:        CLC:imp    Clear Carry
B035 69:00      ADC:imm    #00
B037 20:75 DF   JSR:abs    $DF75
B03A A6:3A      LDX:zp     Zp RAM 003A
B03C BD:FE 91   LDA:abs,x  $91FE,X
B03F 20:E1 C4   JSR:abs    $C4E1
B042 C6:3A      DEC:zp     Zp RAM 003A
B044 C6:37      DEC:zp     Zp RAM 0037
B046 10:A1      BPL:rel    Branch->$AFE9
B048 A9:00      LDA:imm    #00
B04A 85:73      STA:zp     Zp RAM 0073
B04C 20:0D AB   JSR:abs    $AB0D
B04F A2:1C      LDX:imm    #1C
B051 20:14 AB   JSR:abs    $AB14
B054 A9:04      LDA:imm    #04
B056 A0:01      LDY:imm    #01
B058 20:B1 DF   JSR:abs    $DFB1
B05B A0:00      LDY:imm    #00
B05D 20:D1 B0   JSR:abs    $B0D1
B060 20:0D AB   JSR:abs    $AB0D
B063 A2:B8      LDX:imm    #B8
B065 20:AB B0   JSR:abs    $B0AB
B068 38:        SEC:imp    Set Carry
B069 E5:7B      SBC:zp     Zp RAM 007B
B06B A8:        TAY:imp    A-->Y
B06C B9:96 B0   LDA:abs,y  $B096,Y
B06F 38:        SEC:imp    Set Carry
B070 E9:16      SBC:imm    #16
B072 20:75 DF   JSR:abs    $DF75
B075 A9:E0      LDA:imm    #E0
B077 85:73      STA:zp     Zp RAM 0073
B079 A2:00      LDX:imm    #00
B07B 86:38      STX:zp     Zp RAM 0038
B07D A0:03      LDY:imm    #03
B07F 84:37      STY:zp     Zp RAM 0037
B081 A4:38      LDY:zp     Zp RAM 0038
B083 B9:A3 B0   LDA:abs,y  $B0A3,Y
B086 AA:        TAX:imp    A-->X
B087 C8:        INY:imp    Y=Y+1
B088 B9:A3 B0   LDA:abs,y  $B0A3,Y
B08B C8:        INY:imp    Y=Y+1
B08C 84:38      STY:zp     Zp RAM 0038
B08E 20:75 DF   JSR:abs    $DF75
B091 C6:37      DEC:zp     Zp RAM 0037
B093 10:EC      BPL:rel    Branch->$B081
B095 60:        RTS:imp    Ret from Sub
B096 BE:E3 09   LDX:abs,y  $09E3,Y	  ; Start of a DATA segment
B099 30:58      BMI:rel    Branch->$B0F3  ; ref'd by several locations.
B09B 14:        Illegal Opcode
B09C 0C:        Illegal Opcode
B09D 0E:16 18   ASL:abs    $1816
B0A0 1E:20 1A   ASL:abs,x  $1A20,X
B0A3 00:        BRK:imp    BREAK
B0A4 26:28      ROL:zp     Zp RAM 0028
B0A6 00:        BRK:imp    BREAK
B0A7 00:        BRK:imp    BREAK
B0A8 DA:        Illegal Opcode
B0A9 D8:        CLD:imp    Unset Decimal
B0AA 00:        BRK:imp    BREAK
B0AB AD:00 02   LDA:abs    $0200
B0AE 20:CE AD   JSR:abs    $ADCE
B0B1 A8:        TAY:imp    A-->Y
B0B2 10:05      BPL:rel    Branch->$B0B9
B0B4 A9:00      LDA:imm    #00
B0B6 B8:        CLV:imp    Clear Overflow
B0B7 50:08      BVC:rel    Branch->$B0C1
B0B9 CD:27 01   CMP:abs    $0127
B0BC 90:03      BCC:rel    Branch->$B0C1
B0BE AD:27 01   LDA:abs    $0127
B0C1 8D:00 02   STA:abs    $0200
B0C4 A8:        TAY:imp    A-->Y
B0C5 60:        RTS:imp    Ret from Sub
;
B0C6 8A:        TXA:imp    X-->A
B0C7 20:B5 91   JSR:abs    $91B5
B0CA A9:29      LDA:imm    #29
B0CC A0:03      LDY:imm    #03
B0CE 4C:B1 DF   JMP:abs    $DFB1
;
B0D1 C4:9E      CPY:zp     Zp RAM 009E
B0D3 F0:07      BEQ:rel    Branch->$B0DC
B0D5 84:9E      STY:zp     Zp RAM 009E
B0D7 A9:08      LDA:imm    #08
B0D9 4C:4C DF   JMP:abs    $DF4C
B0DC 60:        RTS:imp    Ret from Sub
;
B0DD C5:72      CMP:zp     Zp RAM 0072
B0DF F0:05      BEQ:rel    Branch->$B0E6
B0E1 85:72      STA:zp     Zp RAM 0072
B0E3 4C:6A DF   JMP:abs    $DF6A
B0E6 60:        RTS:imp    Ret from Sub

B0E7 A9:0A      LDA:imm    #0A		 ; Game status = non-player input
B0E9 85:00      STA:zp     Zp RAM 0000	 ; mode.
B0EB A9:00      LDA:imm    #00
B0ED 85:02      STA:zp     Zp RAM 0002
B0EF A9:DF      LDA:imm    #DF
B0F1 85:04      STA:zp     Zp RAM 0004

					; START OF TITLE SCREEN

B0F3 A9:12      LDA:imm    #12          ; Set game status to title start
B0F5 85:01      STA:zp     Zp RAM 0001  ; where a box shrinks to the center.
B0F7 A9:19      LDA:imm    #19
B0F9 8D:4E 01   STA:abs    $014E
B0FC A9:18      LDA:imm    #18
B0FE 8D:4D 01   STA:abs    $014D
B101 60:        RTS:imp    Ret from Sub

B102 A9:34      LDA:imm    #34
B104 A2:AA      LDX:imm    #AA
B106 20:5A B1   JSR:abs    $B15A
B109 AD:4E 01   LDA:abs    $014E
B10C C9:A0      CMP:imm    #A0
B10E B0:05      BCS:rel    Branch->$B115
B110 69:14      ADC:imm    #14
B112 8D:4E 01   STA:abs    $014E
B115 C9:50      CMP:imm    #50
B117 90:17      BCC:rel    Branch->$B130
B119 AD:4D 01   LDA:abs    $014D
B11C 18:        CLC:imp    Clear Carry
B11D 69:08      ADC:imm    #08
B11F 8D:4D 01   STA:abs    $014D
B122 CD:4E 01   CMP:abs    $014E
B125 90:09      BCC:rel    Branch->$B130
B127 A9:A0      LDA:imm    #A0
B129 8D:4D 01   STA:abs    $014D
B12C A9:14      LDA:imm    #14		 ; Set game mode to the part of
B12E 85:01      STA:zp     Zp RAM 0001	 ; the attract mode where the
					 ; Tempest logo grows bigger.
B130 60:        RTS:imp    Ret from Sub
;
B131 A9:3F      LDA:imm    #3F		 ; Code section responsible
B133 A2:4E      LDX:imm    #4E		 ; for zoming IN the tempest
B135 20:5A B1   JSR:abs    $B15A	 ; logo
B138 AD:4D 01   LDA:abs    $014D
B13B C9:30      CMP:imm    #30		 ; How big the logo becomes
B13D 90:05      BCC:rel    Branch->$B144
B13F E9:01      SBC:imm    #01
B141 8D:4D 01   STA:abs    $014D
B144 C9:80      CMP:imm    #80		 ; How close the logo gets
					 ; before the back-end
					 ; starts to catch up.

B146 B0:11      BCS:rel    Branch->$B159

B148 AD:4E 01   LDA:abs    $014E
B14B 38:        SEC:imp    Set Carry
B14C E9:01      SBC:imm    #01
B14E CD:4D 01   CMP:abs    $014D
B151 B0:03      BCS:rel    Branch->$B156

B153 AD:4D 01   LDA:abs    $014D
B156 8D:4E 01   STA:abs    $014E
B159 60:        RTS:imp    Ret from Sub
;
B15A 85:57      STA:zp     Zp RAM 0057
B15C 86:56      STX:zp     Zp RAM 0056
B15E AD:4D 01   LDA:abs    $014D
B161 85:37      STA:zp     Zp RAM 0037
B163 CE:6E 01   DEC:abs    $016E
B166 A5:37      LDA:zp     Zp RAM 0037
B168 0A:        ASL:accum  
B169 0A:        ASL:accum  
B16A 29:7F      AND:imm    #7F
B16C A8:        TAY:imp    A-->Y
B16D A5:37      LDA:zp     Zp RAM 0037
B16F 4A:        LSR:accum  
B170 4A:        LSR:accum  
B171 4A:        LSR:accum  
B172 4A:        LSR:accum  
B173 4A:        LSR:accum  
B174 20:6C DF   JSR:abs    $DF6C
B177 A5:37      LDA:zp     Zp RAM 0037
B179 CD:4D 01   CMP:abs    $014D
B17C D0:05      BNE:rel    Branch->$B183
B17E A9:00      LDA:imm    #00
B180 B8:        CLV:imp    Clear Overflow
B181 50:0C      BVC:rel    Branch->$B18F
B183 4A:        LSR:accum  
B184 4A:        LSR:accum  
B185 4A:        LSR:accum  
B186 EA:        NOP:imp    No Operation
B187 29:07      AND:imm    #07
B189 C9:07      CMP:imm    #07
B18B D0:02      BNE:rel    Branch->$B18F
B18D A9:03      LDA:imm    #03
B18F A8:        TAY:imp    A-->Y
B190 A9:68      LDA:imm    #68
B192 20:4C DF   JSR:abs    $DF4C
B195 A5:57      LDA:zp     Zp RAM 0057
B197 A6:56      LDX:zp     Zp RAM 0056
B199 20:39 DF   JSR:abs    $DF39
B19C A5:37      LDA:zp     Zp RAM 0037
B19E 18:        CLC:imp    Clear Carry
B19F 69:02      ADC:imm    #02
B1A1 85:37      STA:zp     Zp RAM 0037
B1A3 CD:4E 01   CMP:abs    $014E
B1A6 90:BE      BCC:rel    Branch->$B166
B1A8 A2:2C      LDX:imm    #2C
B1AA A9:D0      LDA:imm    #D0
B1AC 20:17 AB   JSR:abs    $AB17
B1AF A9:3F      LDA:imm    #3F
B1B1 A2:F2      LDX:imm    #F2
B1B3 4C:39 DF   JMP:abs    $DF39
B1B6 20:C3 C1   JSR:abs    $C1C3
B1B9 AD:00 20   LDA:abs    $2000         ; Look at the first byte in
B1BC CD:C6 CE   CMP:abs    $CEC6         ; vector RAM.
B1BF D0:06      BNE:rel    Branch->$B1C7
B1C1 AD:33 01   LDA:abs    $0133
B1C4 D0:01      BNE:rel    Branch->$B1C7
B1C6 60:        RTS:imp    Ret from Sub
B1C7 A5:01      LDA:zp     Zp RAM 0001	 ; Are we in attract or gameplay
B1C9 C9:00      CMP:imm    #00		 ; mode?
B1CB F0:3C      BEQ:rel    Branch->$B209
B1CD A9:00      LDA:imm    #00
B1CF 20:BE B2   JSR:abs    $B2BE
B1D2 20:32 B3   JSR:abs    $B332
B1D5 B0:1E      BCS:rel    Branch->$B1F5
B1D7 20:0D B2   JSR:abs    $B20D
B1DA AD:6E 01   LDA:abs    $016E
B1DD F0:16      BEQ:rel    Branch->$B1F5
B1DF A0:27      LDY:imm    #27
B1E1 A9:0E      LDA:imm    #0E
B1E3 38:        SEC:imp    Set Carry
B1E4 F1:B6      SBC:ind,y  B6
B1E6 88:        DEY:imp    Y=Y-1
B1E7 10:FB      BPL:rel    Branch->$B1E4
B1E9 A8:        TAY:imp    A-->Y
B1EA F0:02      BEQ:rel    Branch->$B1EE
B1EC 49:E5      EOR:imm    #E5
B1EE F0:02      BEQ:rel    Branch->$B1F2
B1F0 49:29      EOR:imm    #29
B1F2 8D:55 04   STA:abs    $0455
B1F5 A9:00      LDA:imm    #00
B1F7 20:FE B2   JSR:abs    $B2FE
B1FA AD:C4 CE   LDA:abs    $CEC4
B1FD 8D:00 20   STA:abs    $2000
B200 AD:C5 CE   LDA:abs    $CEC5
B203 8D:01 20   STA:abs    $2001
B206 B8:        CLV:imp    Clear Overflow
B207 50:03      BVC:rel    Branch->$B20C
B209 4C:30 B2   JMP:abs    $B230
B20C 60:        RTS:imp    Ret from Sub
B20D A6:01      LDX:zp     Zp RAM 0001
B20F BD:19 B2   LDA:abs,x  $B219,X
B212 48:        PHA:imp    Push Accum
B213 BD:18 B2   LDA:abs,x  $B218,X
B216 48:        PHA:imp    Push Accum
B217 60:        RTS:imp    Ret from Sub
;
; Data Segment, per Ken Lui
;
B218 2F:B2      DATA
B21A 03:D8      DATA
B21C B9:B8      DATA
BC1E E9:AD      DATA
B220 80:AF      DATA
B222 1B:AE      DATA
B224 61:AA      DATA
B226 59:AA      DATA
B228 6E:AA      DATA
B22A 01:B1      DATA
B22C 30:B1      DATA
B22E 78:AA      DATA
;
B230 A9:07      LDA:imm    #07
B232 20:BE B2   JSR:abs    $B2BE
B235 20:86 B5   JSR:abs    $B586
B238 A9:07      LDA:imm    #07
B23A 20:FE B2   JSR:abs    $B2FE
B23D A9:04      LDA:imm    #04
B23F 20:BE B2   JSR:abs    $B2BE
B242 20:5B B7   JSR:abs    $B75B
B245 A9:04      LDA:imm    #04
B247 20:FE B2   JSR:abs    $B2FE
B24A A9:03      LDA:imm    #03
B24C 20:BE B2   JSR:abs    $B2BE
B24F 20:AD B5   JSR:abs    $B5AD
B252 A9:03      LDA:imm    #03
B254 20:FE B2   JSR:abs    $B2FE
B257 A9:06      LDA:imm    #06
B259 20:BE B2   JSR:abs    $B2BE
B25C 20:9A B7   JSR:abs    $B79A
B25F A9:06      LDA:imm    #06
B261 20:FE B2   JSR:abs    $B2FE
B264 A9:05      LDA:imm    #05
B266 20:BE B2   JSR:abs    $B2BE
B269 20:98 B4   JSR:abs    $B498
B26C A9:05      LDA:imm    #05
B26E 20:FE B2   JSR:abs    $B2FE
B271 A9:00      LDA:imm    #00
B273 20:BE B2   JSR:abs    $B2BE
B276 20:B4 A8   JSR:abs    $A8B4
B279 A5:05      LDA:zp     Zp RAM 0005
B27B 30:0D      BMI:rel    Branch->$B28A
B27D A9:F2      LDA:imm    #F2
B27F 18:        CLC:imp    Clear Carry
B280 A0:27      LDY:imm    #27
B282 71:B6      ADC:ind,y  B6
B284 88:        DEY:imp    Y=Y-1
B285 10:FB      BPL:rel    Branch->$B282
B287 8D:1B 01   STA:abs    $011B	; Copy protection code?
B28A A9:00      LDA:imm    #00
B28C 20:FE B2   JSR:abs    $B2FE
B28F 20:67 B3   JSR:abs    $B367
B292 A9:01      LDA:imm    #01
B294 20:BE B2   JSR:abs    $B2BE
B297 20:C2 C5   JSR:abs    $C5C2
B29A A9:01      LDA:imm    #01
B29C 20:FE B2   JSR:abs    $B2FE
B29F A9:08      LDA:imm    #08
B2A1 20:BE B2   JSR:abs    $B2BE
B2A4 20:4D C5   JSR:abs    $C54D
B2A7 A9:08      LDA:imm    #08
B2A9 20:FE B2   JSR:abs    $B2FE
B2AC A9:00      LDA:imm    #00
B2AE 8D:14 01   STA:abs    $0114
B2B1 AD:C2 CE   LDA:abs    $CEC2
B2B4 8D:00 20   STA:abs    $2000
B2B7 AD:C3 CE   LDA:abs    $CEC3
B2BA 8D:01 20   STA:abs    $2001
B2BD 60:        RTS:imp    Ret from Sub
B2BE AA:        TAX:imp    A-->X
B2BF 0A:        ASL:accum  
B2C0 A8:        TAY:imp    A-->Y
B2C1 BD:15 04   LDA:abs,x  $0415,X
B2C4 D0:09      BNE:rel    Branch->$B2CF
B2C6 BE:7A CE   LDX:abs,y  $CE7A,Y
B2C9 B9:7B CE   LDA:abs,y  $CE7B,Y
B2CC B8:        CLV:imp    Clear Overflow
B2CD 50:06      BVC:rel    Branch->$B2D5
B2CF BE:68 CE   LDX:abs,y  $CE68,Y
B2D2 B9:69 CE   LDA:abs,y  $CE69,Y
B2D5 86:74      STX:zp     Zp RAM 0074
B2D7 85:75      STA:zp     Zp RAM 0075
B2D9 A9:00      LDA:imm    #00
B2DB 85:A9      STA:zp     Zp RAM 00A9
B2DD 60:        RTS:imp    Ret from Sub
B2DE AA:        TAX:imp    A-->X
B2DF 0A:        ASL:accum  
B2E0 A8:        TAY:imp    A-->Y
B2E1 BD:15 04   LDA:abs,x  $0415,X
B2E4 D0:09      BNE:rel    Branch->$B2EF
B2E6 BE:68 CE   LDX:abs,y  $CE68,Y
B2E9 B9:69 CE   LDA:abs,y  $CE69,Y
B2EC B8:        CLV:imp    Clear Overflow
B2ED 50:06      BVC:rel    Branch->$B2F5
B2EF BE:7A CE   LDX:abs,y  $CE7A,Y
B2F2 B9:7B CE   LDA:abs,y  $CE7B,Y
B2F5 86:3B      STX:zp     Zp RAM 003B
B2F7 85:3C      STA:zp     Zp RAM 003C
B2F9 A9:00      LDA:imm    #00
B2FB 85:A9      STA:zp     Zp RAM 00A9
B2FD 60:        RTS:imp    Ret from Sub
B2FE 48:        PHA:imp    Push Accum
B2FF 20:09 DF   JSR:abs    $DF09
B302 68:        PLA:imp    Pull Accum
B303 AA:        TAX:imp    A-->X
B304 0A:        ASL:accum  
B305 A8:        TAY:imp    A-->Y
B306 B9:8C CE   LDA:abs,y  $CE8C,Y
B309 85:3B      STA:zp     Zp RAM 003B
B30B B9:8D CE   LDA:abs,y  $CE8D,Y
B30E 85:3C      STA:zp     Zp RAM 003C
B310 BD:15 04   LDA:abs,x  $0415,X
B313 49:01      EOR:imm    #01
B315 9D:15 04   STA:abs,x  $0415,X
B318 D0:09      BNE:rel    Branch->$B323
B31A B9:9E CE   LDA:abs,y  $CE9E,Y
B31D BE:9F CE   LDX:abs,y  $CE9F,Y
B320 B8:        CLV:imp    Clear Overflow
B321 50:06      BVC:rel    Branch->$B329
B323 B9:B0 CE   LDA:abs,y  $CEB0,Y
B326 BE:B1 CE   LDX:abs,y  $CEB1,Y
B329 A0:00      LDY:imm    #00
B32B 91:3B      STA:ind,y  3B
B32D 8A:        TXA:imp    X-->A
B32E C8:        INY:imp    Y=Y+1
B32F 91:3B      STA:ind,y  3B
B331 60:        RTS:imp    Ret from Sub

B332 AD:C4 CE   LDA:abs    $CEC4
B335 CD:00 20   CMP:abs    $2000
B338 F0:05      BEQ:rel    Branch->$B33F
B33A 8D:00 20   STA:abs    $2000
B33D 38:        SEC:imp    Set Carry
B33E 60:        RTS:imp    Ret from Sub
B33F AD:15 04   LDA:abs    $0415
B342 D0:05      BNE:rel    Branch->$B349
B344 A2:02      LDX:imm    #02
B346 B8:        CLV:imp    Clear Overflow
B347 50:02      BVC:rel    Branch->$B34B
B349 A2:08      LDX:imm    #08
B34B BD:9E CE   LDA:abs,x  $CE9E,X
B34E A0:00      LDY:imm    #00
B350 8C:6E 01   STY:abs    $016E
B353 91:74      STA:ind,y  74
B355 C8:        INY:imp    Y=Y+1
B356 BD:9F CE   LDA:abs,x  $CE9F,X
B359 91:74      STA:ind,y  74
B35B BD:68 CE   LDA:abs,x  $CE68,X
B35E 85:74      STA:zp     Zp RAM 0074
B360 BD:69 CE   LDA:abs,x  $CE69,X
B363 85:75      STA:zp     Zp RAM 0075
B365 18:        CLC:imp    Clear Carry
B366 60:        RTS:imp    Ret from Sub

B367 AD:14 01   LDA:abs    $0114
B36A F0:0D      BEQ:rel    Branch->$B379
B36C A9:02      LDA:imm    #02
B36E 20:BE B2   JSR:abs    $B2BE
B371 20:0D C3   JSR:abs    $C30D
B374 A9:02      LDA:imm    #02
B376 20:FE B2   JSR:abs    $B2FE
B379 A9:02      LDA:imm    #02
B37B 20:DE B2   JSR:abs    $B2DE
B37E A9:00      LDA:imm    #00
B380 A2:0F      LDX:imm    #0F
B382 9D:25 04   STA:abs,x  $0425,X
B385 CA:        DEX:imp    X=X-1
B386 10:FA      BPL:rel    Branch->$B382
B388 AD:06 01   LDA:abs    $0106
B38B 30:49      BMI:rel    Branch->$B3D6
B38D AE:1C 01   LDX:abs    $011C
B390 BD:DF 02   LDA:abs,x  $02DF,X
B393 F0:3E      BEQ:rel    Branch->$B3D3
B395 A0:00      LDY:imm    #00
B397 BD:83 02   LDA:abs,x  $0283,X
B39A 29:07      AND:imm    #07
B39C C9:01      CMP:imm    #01
B39E D0:33      BNE:rel    Branch->$B3D3
B3A0 C8:        INY:imp    Y=Y+1
B3A1 84:29      STY:zp     Zp RAM 0029
B3A3 BD:83 02   LDA:abs,x  $0283,X
B3A6 29:80      AND:imm    #80
B3A8 D0:1C      BNE:rel    Branch->$B3C6
B3AA AD:48 01   LDA:abs    $0148
B3AD 30:0C      BMI:rel    Branch->$B3BB
B3AF BD:DF 02   LDA:abs,x  $02DF,X
B3B2 CD:57 01   CMP:abs    $0157
B3B5 B0:04      BCS:rel    Branch->$B3BB
B3B7 E6:29      INC:zp     Zp RAM 0029
B3B9 E6:29      INC:zp     Zp RAM 0029
B3BB A5:29      LDA:zp     Zp RAM 0029
B3BD BC:CC 02   LDY:abs,x  $02CC,X
B3C0 19:25 04   ORA:abs,y  $0425,Y
B3C3 99:25 04   STA:abs,y  $0425,Y
B3C6 BC:B9 02   LDY:abs,x  $02B9,X
B3C9 A5:29      LDA:zp     Zp RAM 0029
B3CB 09:80      ORA:imm    #80
B3CD 19:25 04   ORA:abs,y  $0425,Y
B3D0 99:25 04   STA:abs,y  $0425,Y
B3D3 CA:        DEX:imp    X=X-1
B3D4 10:BA      BPL:rel    Branch->$B390
B3D6 A9:06      LDA:imm    #06
B3D8 AC:25 01   LDY:abs    $0125
B3DB F0:0C      BEQ:rel    Branch->$B3E9
B3DD 30:0A      BMI:rel    Branch->$B3E9
B3DF A5:03      LDA:zp     Zp RAM 0003
B3E1 29:07      AND:imm    #07
B3E3 C9:07      CMP:imm    #07
B3E5 D0:02      BNE:rel    Branch->$B3E9
B3E7 A9:01      LDA:imm    #01
B3E9 85:29      STA:zp     Zp RAM 0029
B3EB A0:FF      LDY:imm    #FF
B3ED A2:FF      LDX:imm    #FF
B3EF 86:2C      STX:zp     Zp RAM 002C
B3F1 AD:02 02   LDA:abs    $0202
B3F4 F0:0B      BEQ:rel    Branch->$B401
B3F6 AD:01 02   LDA:abs    $0201
B3F9 30:06      BMI:rel    Branch->$B401
B3FB AE:00 02   LDX:abs    $0200
B3FE AC:01 02   LDY:abs    $0201
B401 86:2A      STX:zp     Zp RAM 002A
B403 84:2B      STY:zp     Zp RAM 002B
B405 AD:24 01   LDA:abs    $0124
B408 30:08      BMI:rel    Branch->$B412
B40A 29:0E      AND:imm    #0E
B40C 4A:        LSR:accum  
B40D 85:2C      STA:zp     Zp RAM 002C
B40F CE:24 01   DEC:abs    $0124
B412 A2:0F      LDX:imm    #0F
B414 A0:06      LDY:imm    #06
B416 BD:25 04   LDA:abs,x  $0425,X
B419 F0:0C      BEQ:rel    Branch->$B427
B41B 29:02      AND:imm    #02
B41D F0:05      BEQ:rel    Branch->$B424
B41F A5:03      LDA:zp     Zp RAM 0003
B421 29:01      AND:imm    #01
B423 A8:        TAY:imp    A-->Y
B424 B8:        CLV:imp    Clear Overflow
B425 50:24      BVC:rel    Branch->$B44B
B427 E4:2A      CPX:zp     Zp RAM 002A
B429 F0:02      BEQ:rel    Branch->$B42D
B42B E4:2B      CPX:zp     Zp RAM 002B
B42D D0:05      BNE:rel    Branch->$B434
B42F A0:01      LDY:imm    #01
B431 B8:        CLV:imp    Clear Overflow
B432 50:17      BVC:rel    Branch->$B44B
B434 AD:24 01   LDA:abs    $0124
B437 30:10      BMI:rel    Branch->$B449
B439 8A:        TXA:imp    X-->A
B43A 18:        CLC:imp    Clear Carry
B43B 65:2C      ADC:zp     Zp RAM 002C
B43D 29:07      AND:imm    #07
B43F C9:07      CMP:imm    #07
B441 D0:02      BNE:rel    Branch->$B445
B443 A9:03      LDA:imm    #03
B445 A8:        TAY:imp    A-->Y
B446 B8:        CLV:imp    Clear Overflow
B447 50:02      BVC:rel    Branch->$B44B
B449 A4:29      LDY:zp     Zp RAM 0029
B44B 98:        TYA:imp    Y-->A
B44C BC:76 B4   LDY:abs,x  $B476,X
B44F 91:3B      STA:ind,y  3B
B451 CA:        DEX:imp    X=X-1
B452 10:C0      BPL:rel    Branch->$B414
B454 A2:0F      LDX:imm    #0F
B456 2C:11 01   BIT:abs    $0111
B459 10:01      BPL:rel    Branch->$B45C
B45B CA:        DEX:imp    X=X-1
B45C A0:C0      LDY:imm    #C0
B45E BD:25 04   LDA:abs,x  $0425,X
B461 10:02      BPL:rel    Branch->$B465
B463 A0:00      LDY:imm    #00
B465 84:58      STY:zp     Zp RAM 0058
B467 BC:87 B4   LDY:abs,x  $B487,X
B46A B1:B0      LDA:ind,y  B0
B46C 29:1F      AND:imm    #1F
B46E 05:58      ORA:zp     Zp RAM 0058
B470 91:B0      STA:ind,y  B0
B472 CA:        DEX:imp    X=X-1
B473 10:E7      BPL:rel    Branch->$B45C
B475 60:        RTS:imp    Ret from Sub
B476 A8:        TAY:imp    A-->Y
B477 9C:        Illegal Opcode
B478 92:        Illegal Opcode
B479 86:7C      STX:zp     Zp RAM 007C
B47B 70:66      BVS:rel    Branch->$B4E3
B47D 5A:        Illegal Opcode
B47E 50:44      BVC:rel    Branch->$B4C4
B480 3A:        Illegal Opcode
B481 2E:24 18   ROL:abs    $1824
B484 0E:02 B2   ASL:abs    $B202
B487 3B:        Illegal Opcode
B488 37:        Illegal Opcode
B489 33:        Illegal Opcode
B48A 2F:        Illegal Opcode
B48B 2B:        Illegal Opcode
B48C 27:        Illegal Opcode
B48D 23:        Illegal Opcode
B48E 1F:        Illegal Opcode
B48F 1B:        Illegal Opcode
B490 17:        Illegal Opcode
B491 13:        Illegal Opcode
B492 0F:        Illegal Opcode
B493 0B:        Illegal Opcode
B494 07:        Illegal Opcode
B495 03:        Illegal Opcode
B496 3F:        Illegal Opcode
B497 1D:A0 0C   ORA:abs,x  $0CA0,X
B49A 84:9E      STY:zp     Zp RAM 009E
B49C A9:08      LDA:imm    #08
B49E 20:4C DF   JSR:abs    $DF4C
B4A1 A2:66      LDX:imm    #66
B4A3 20:65 C7   JSR:abs    $C765
B4A6 A9:12      LDA:imm    #12
B4A8 85:56      STA:zp     Zp RAM 0056
B4AA A2:3F      LDX:imm    #3F
B4AC 86:37      STX:zp     Zp RAM 0037
B4AE A0:00      LDY:imm    #00
B4B0 A6:37      LDX:zp     Zp RAM 0037
B4B2 BD:43 02   LDA:abs,x  $0243,X
B4B5 D0:03      BNE:rel    Branch->$B4BA
B4B7 4C:49 B5   JMP:abs    $B549
B4BA C9:50      CMP:imm    #50
B4BC 90:02      BCC:rel    Branch->$B4C0
B4BE C6:37      DEC:zp     Zp RAM 0037
B4C0 48:        PHA:imp    Push Accum
B4C1 29:3F      AND:imm    #3F
B4C3 91:74      STA:ind,y  74
B4C5 68:        PLA:imp    Pull Accum
B4C6 2A:        ROL:accum  
B4C7 2A:        ROL:accum  
B4C8 2A:        ROL:accum  
B4C9 29:03      AND:imm    #03
B4CB 18:        CLC:imp    Clear Carry
B4CC 69:01      ADC:imm    #01
B4CE 09:70      ORA:imm    #70
B4D0 C8:        INY:imp    Y=Y+1
B4D1 91:74      STA:ind,y  74
B4D3 C8:        INY:imp    Y=Y+1
B4D4 BD:03 02   LDA:abs,x  $0203,X
B4D7 AA:        TAX:imp    A-->X
B4D8 BD:8A 03   LDA:abs,x  $038A,X
B4DB 38:        SEC:imp    Set Carry
B4DC E5:68      SBC:zp     Zp RAM 0068
B4DE 85:63      STA:zp     Zp RAM 0063
B4E0 91:74      STA:ind,y  74
B4E2 C8:        INY:imp    Y=Y+1
B4E3 BD:7A 03   LDA:abs,x  $037A,X
B4E6 E5:69      SBC:zp     Zp RAM 0069
B4E8 85:64      STA:zp     Zp RAM 0064
B4EA 29:1F      AND:imm    #1F
B4EC 91:74      STA:ind,y  74
B4EE C8:        INY:imp    Y=Y+1
B4EF BD:6A 03   LDA:abs,x  $036A,X
B4F2 85:61      STA:zp     Zp RAM 0061
B4F4 91:74      STA:ind,y  74
B4F6 C8:        INY:imp    Y=Y+1
B4F7 BD:5A 03   LDA:abs,x  $035A,X
B4FA 85:62      STA:zp     Zp RAM 0062
B4FC 29:1F      AND:imm    #1F
B4FE 91:74      STA:ind,y  74
B500 C8:        INY:imp    Y=Y+1
B501 A9:00      LDA:imm    #00
B503 91:74      STA:ind,y  74
B505 C8:        INY:imp    Y=Y+1
B506 91:74      STA:ind,y  74
B508 C8:        INY:imp    Y=Y+1
B509 91:74      STA:ind,y  74
B50B A9:A0      LDA:imm    #A0
B50D C8:        INY:imp    Y=Y+1
B50E 91:74      STA:ind,y  74
B510 C8:        INY:imp    Y=Y+1
B511 A5:63      LDA:zp     Zp RAM 0063
B513 49:FF      EOR:imm    #FF
B515 18:        CLC:imp    Clear Carry
B516 69:01      ADC:imm    #01
B518 91:74      STA:ind,y  74
B51A C8:        INY:imp    Y=Y+1
B51B A5:64      LDA:zp     Zp RAM 0064
B51D 49:FF      EOR:imm    #FF
B51F 69:00      ADC:imm    #00
B521 29:1F      AND:imm    #1F
B523 91:74      STA:ind,y  74
B525 C8:        INY:imp    Y=Y+1
B526 A5:61      LDA:zp     Zp RAM 0061
B528 49:FF      EOR:imm    #FF
B52A 18:        CLC:imp    Clear Carry
B52B 69:01      ADC:imm    #01
B52D 91:74      STA:ind,y  74
B52F C8:        INY:imp    Y=Y+1
B530 A5:62      LDA:zp     Zp RAM 0062
B532 49:FF      EOR:imm    #FF
B534 69:00      ADC:imm    #00
B536 29:1F      AND:imm    #1F
B538 91:74      STA:ind,y  74
B53A C8:        INY:imp    Y=Y+1
B53B C0:F0      CPY:imm    #F0
B53D 90:06      BCC:rel    Branch->$B545
B53F 88:        DEY:imp    Y=Y-1
B540 20:5F DF   JSR:abs    $DF5F
B543 A0:00      LDY:imm    #00
B545 C6:56      DEC:zp     Zp RAM 0056
B547 30:07      BMI:rel    Branch->$B550
B549 C6:37      DEC:zp     Zp RAM 0037
B54B 30:03      BMI:rel    Branch->$B550
B54D 4C:B0 B4   JMP:abs    $B4B0
B550 98:        TYA:imp    Y-->A
B551 F0:04      BEQ:rel    Branch->$B557
B553 88:        DEY:imp    Y=Y-1
B554 20:5F DF   JSR:abs    $DF5F
B557 A5:B5      LDA:zp     Zp RAM 00B5
B559 F0:0A      BEQ:rel    Branch->$B565
B55B A5:46      LDA:zp     Zp RAM 0046	 ; Load current level for player 1.
B55D C9:0A      CMP:imm    #0A
B55F 90:04      BCC:rel    Branch->$B565
B561 A9:7A      LDA:imm    #7A		 ; Set the 0-9 timer as "7A".
B563 85:53      STA:zp     Zp RAM 0053	 ; NOTE: If it reaches 80, the 
					 ; interrupt code will do a BREAK.
B565 A9:01      LDA:imm    #01
B567 4C:6A DF   JMP:abs    $DF6A
B56A 48:        PHA:imp    Push Accum
B56B A0:00      LDY:imm    #00
B56D 98:        TYA:imp    Y-->A
B56E 91:74      STA:ind,y  74
B570 C8:        INY:imp    Y=Y+1
B571 91:74      STA:ind,y  74
B573 C8:        INY:imp    Y=Y+1
B574 91:74      STA:ind,y  74
B576 C8:        INY:imp    Y=Y+1
B577 68:        PLA:imp    Pull Accum
B578 91:74      STA:ind,y  74
B57A A9:04      LDA:imm    #04
B57C 18:        CLC:imp    Clear Carry
B57D 65:74      ADC:zp     Zp RAM 0074
B57F 85:74      STA:zp     Zp RAM 0074
B581 90:02      BCC:rel    Branch->$B585
B583 E6:75      INC:zp     Zp RAM 0075
B585 60:        RTS:imp    Ret from Sub
B586 A9:01      LDA:imm    #01
B588 85:9E      STA:zp     Zp RAM 009E
B58A AD:02 02   LDA:abs    $0202
B58D F0:1D      BEQ:rel    Branch->$B5AC
B58F C9:F0      CMP:imm    #F0
B591 B0:19      BCS:rel    Branch->$B5AC
B593 85:57      STA:zp     Zp RAM 0057
B595 85:2F      STA:zp     Zp RAM 002F
B597 AD:01 02   LDA:abs    $0201	 ; Is the player dead?
B59A C9:81      CMP:imm    #81
B59C F0:0E      BEQ:rel    Branch->$B5AC
B59E AC:00 02   LDY:abs    $0200
B5A1 A5:51      LDA:zp     Zp RAM 0051   ; Grab the player's tunnel position
B5A3 4A:        LSR:accum  
B5A4 29:07      AND:imm    #07
B5A6 18:        CLC:imp    Clear Carry
B5A7 69:01      ADC:imm    #01
B5A9 20:A0 BD   JSR:abs    $BDA0
B5AC 60:        RTS:imp    Ret from Sub
B5AD AD:06 01   LDA:abs    $0106
B5B0 30:24      BMI:rel    Branch->$B5D6
B5B2 A2:06      LDX:imm    #06
B5B4 86:37      STX:zp     Zp RAM 0037
B5B6 A6:37      LDX:zp     Zp RAM 0037
B5B8 BD:DF 02   LDA:abs,x  $02DF,X
B5BB F0:15      BEQ:rel    Branch->$B5D2
B5BD 85:57      STA:zp     Zp RAM 0057
B5BF BD:83 02   LDA:abs,x  $0283,X
B5C2 29:18      AND:imm    #18
B5C4 4A:        LSR:accum  
B5C5 4A:        LSR:accum  
B5C6 4A:        LSR:accum  
B5C7 85:55      STA:zp     Zp RAM 0055
B5C9 BD:83 02   LDA:abs,x  $0283,X
B5CC 29:07      AND:imm    #07
B5CE 0A:        ASL:accum  
B5CF 20:D7 B5   JSR:abs    $B5D7
B5D2 C6:37      DEC:zp     Zp RAM 0037
B5D4 10:E0      BPL:rel    Branch->$B5B6
B5D6 60:        RTS:imp    Ret from Sub
B5D7 A8:        TAY:imp    A-->Y
B5D8 B9:E2 B5   LDA:abs,y  $B5E2,Y
B5DB 48:        PHA:imp    Push Accum
B5DC B9:E1 B5   LDA:abs,y  $B5E1,Y
B5DF 48:        PHA:imp    Push Accum
B5E0 60:        RTS:imp    Ret from Sub
;
B5E1 EA:B5      DATA
B5E3 1A:B7      DATA
B5E5 0E:B6      DATA
B5E7 21:B6      DATA
B5E9 9A:B6      DATA
;
B5EB A9         LDX:zp,y   Zp RAM 00A9	 ; Re-disassemble.
B5EC 03:        Illegal Opcode		 ; Confused by data segment.
B5ED 85:9E      STA:zp     Zp RAM 009E
B5EF BD:83 02   LDA:abs,x  $0283,X
B5F2 30:0E      BMI:rel    Branch->$B602
B5F4 BC:B9 02   LDY:abs,x  $02B9,X
B5F7 A6:55      LDX:zp     Zp RAM 0055
B5F9 BD:0B B6   LDA:abs,x  $B60B,X
B5FC 20:A0 BD   JSR:abs    $BDA0
B5FF B8:        CLV:imp    Clear Overflow
B600 50:08      BVC:rel    Branch->$B60A
B602 20:34 B6   JSR:abs    $B634
B605 A0:00      LDY:imm    #00
B607 20:CB BD   JSR:abs    $BDCB
B60A 60:        RTS:imp    Ret from Sub
B60B 00:        BRK:imp    BREAK
B60C 00:        BRK:imp    BREAK
B60D 00:        BRK:imp    BREAK
B60E 00:        BRK:imp    BREAK
B60F BD:8A 02   LDA:abs,x  $028A,X
B612 29:03      AND:imm    #03
B614 A8:        TAY:imp    A-->Y
B615 B9:1E B6   LDA:abs,y  $B61E,Y
B618 BC:B9 02   LDY:abs,x  $02B9,X
B61B 4C:FD BC   JMP:abs    $BCFD
B61E 1A:        Illegal Opcode
B61F 1A:        Illegal Opcode
B620 4A:        LSR:accum  
B621 4C:BC B9   JMP:abs    $B9BC
B624 02:        Illegal Opcode
B625 A5:03      LDA:zp     Zp RAM 0003
B627 29:03      AND:imm    #03
B629 0A:        ASL:accum  
B62A 18:        CLC:imp    Clear Carry
B62B 69:12      ADC:imm    #12
B62D 4C:FD BC   JMP:abs    $BCFD
B630 12:        Illegal Opcode
B631 14:        Illegal Opcode
B632 16:18      ASL:zp,x   Zp RAM 0018
B634 A5:57      LDA:zp     Zp RAM 0057
B636 85:2F      STA:zp     Zp RAM 002F
B638 BC:B9 02   LDY:abs,x  $02B9,X
B63B B9:CE 03   LDA:abs,y  $03CE,Y
B63E 85:56      STA:zp     Zp RAM 0056
B640 B9:DE 03   LDA:abs,y  $03DE,Y
B643 85:58      STA:zp     Zp RAM 0058
B645 BD:CC 02   LDA:abs,x  $02CC,X
B648 29:0F      AND:imm    #0F
B64A A8:        TAY:imp    A-->Y
B64B A5:56      LDA:zp     Zp RAM 0056
B64D 49:80      EOR:imm    #80
B64F 18:        CLC:imp    Clear Carry
B650 79:8B B6   ADC:abs,y  $B68B,Y
B653 50:09      BVC:rel    Branch->$B65E
B655 10:05      BPL:rel    Branch->$B65C
B657 A9:7F      LDA:imm    #7F
B659 B8:        CLV:imp    Clear Overflow
B65A 50:02      BVC:rel    Branch->$B65E
B65C A9:80      LDA:imm    #80
B65E 49:80      EOR:imm    #80
B660 85:2E      STA:zp     Zp RAM 002E
B662 A5:58      LDA:zp     Zp RAM 0058
B664 49:80      EOR:imm    #80
B666 18:        CLC:imp    Clear Carry
B667 79:87 B6   ADC:abs,y  $B687,Y
B66A 50:09      BVC:rel    Branch->$B675
B66C 10:05      BPL:rel    Branch->$B673
B66E A9:7F      LDA:imm    #7F
B670 B8:        CLV:imp    Clear Overflow
B671 50:02      BVC:rel    Branch->$B675
B673 A9:80      LDA:imm    #80
B675 49:80      EOR:imm    #80
B677 85:30      STA:zp     Zp RAM 0030
B679 AC:12 01   LDY:abs    $0112
B67C B9:DC BC   LDA:abs,y  $BCDC,Y
B67F 85:59      STA:zp     Zp RAM 0059
B681 B9:EC BC   LDA:abs,y  $BCEC,Y
B684 85:5A      STA:zp     Zp RAM 005A
B686 60:        RTS:imp    Ret from Sub
B687 00:        BRK:imp    BREAK
B688 10:1F      BPL:rel    Branch->$B6A9
B68A 28:        PLP:imp    Pull P
B68B 2C:28 1F   BIT:abs    $1F28	 ; Strange. Nothing at that addr?
B68E 10:00      BPL:rel    Branch->$B690
B690 F0:E1      BEQ:rel    Branch->$B673
B692 D8:        CLD:imp    Unset Decimal
B693 D4:        Illegal Opcode
B694 D8:        CLD:imp    Unset Decimal
B695 E1:F0      SBC:ind,x  F0
B697 00:        BRK:imp    BREAK
B698 10:1F      BPL:rel    Branch->$B6B9
B69A 28:        PLP:imp    Pull P
B69B BD:DF 02   LDA:abs,x  $02DF,X
B69E 85:57      STA:zp     Zp RAM 0057
B6A0 BC:B9 02   LDY:abs,x  $02B9,X
B6A3 B9:CE 03   LDA:abs,y  $03CE,Y
B6A6 85:56      STA:zp     Zp RAM 0056
B6A8 B9:DE 03   LDA:abs,y  $03DE,Y
B6AB 85:58      STA:zp     Zp RAM 0058
B6AD BD:CC 02   LDA:abs,x  $02CC,X
B6B0 10:23      BPL:rel    Branch->$B6D5
B6B2 98:        TYA:imp    Y-->A
B6B3 18:        CLC:imp    Clear Carry
B6B4 69:01      ADC:imm    #01
B6B6 29:0F      AND:imm    #0F
B6B8 A8:        TAY:imp    A-->Y
B6B9 B9:CE 03   LDA:abs,y  $03CE,Y
B6BC 38:        SEC:imp    Set Carry
B6BD E5:56      SBC:zp     Zp RAM 0056
B6BF 20:FA B6   JSR:abs    $B6FA
B6C2 18:        CLC:imp    Clear Carry
B6C3 65:56      ADC:zp     Zp RAM 0056
B6C5 85:56      STA:zp     Zp RAM 0056
B6C7 B9:DE 03   LDA:abs,y  $03DE,Y
B6CA 38:        SEC:imp    Set Carry
B6CB E5:58      SBC:zp     Zp RAM 0058
B6CD 20:FA B6   JSR:abs    $B6FA
B6D0 18:        CLC:imp    Clear Carry
B6D1 65:58      ADC:zp     Zp RAM 0058
B6D3 85:58      STA:zp     Zp RAM 0058
B6D5 20:98 C0   JSR:abs    $C098
B6D8 A2:61      LDX:imm    #61
B6DA 20:65 C7   JSR:abs    $C765
B6DD A9:00      LDA:imm    #00
B6DF 85:A9      STA:zp     Zp RAM 00A9
B6E1 20:3E BD   JSR:abs    $BD3E
B6E4 84:A9      STY:zp     Zp RAM 00A9
B6E6 A5:03      LDA:zp     Zp RAM 0003
B6E8 29:03      AND:imm    #03
B6EA 0A:        ASL:accum  
B6EB 18:        CLC:imp    Clear Carry
B6EC 69:4E      ADC:imm    #4E
B6EE A8:        TAY:imp    A-->Y
B6EF BE:C9 CE   LDX:abs,y  $CEC9,Y
B6F2 B9:C8 CE   LDA:abs,y  $CEC8,Y
B6F5 A4:A9      LDY:zp     Zp RAM 00A9
B6F7 4C:59 DF   JMP:abs    $DF59
B6FA 85:29      STA:zp     Zp RAM 0029
B6FC BD:CC 02   LDA:abs,x  $02CC,X
B6FF 29:07      AND:imm    #07
B701 85:2C      STA:zp     Zp RAM 002C
B703 86:2B      STX:zp     Zp RAM 002B
B705 A2:02      LDX:imm    #02
B707 A9:00      LDA:imm    #00
B709 46:2C      LSR:zp     Zp RAM 002C
B70B 90:03      BCC:rel    Branch->$B710
B70D 18:        CLC:imp    Clear Carry
B70E 65:29      ADC:zp     Zp RAM 0029
B710 0A:        ASL:accum  
B711 08:        PHP:imp    Push P
B712 6A:        ROT:accum  
B713 28:        PLP:imp    Pull P
B714 6A:        ROT:accum  
B715 CA:        DEX:imp    X=X-1
B716 10:F1      BPL:rel    Branch->$B709
B718 A6:2B      LDX:zp     Zp RAM 002B
B71A 60:        RTS:imp    Ret from Sub
B71B A9:04      LDA:imm    #04
B71D AC:48 01   LDY:abs    $0148
B720 30:02      BMI:rel    Branch->$B724
B722 A9:00      LDA:imm    #00
B724 85:9E      STA:zp     Zp RAM 009E
B726 AD:48 01   LDA:abs    $0148
B729 18:        CLC:imp    Clear Carry
B72A 69:40      ADC:imm    #40
B72C 4A:        LSR:accum  
B72D 4A:        LSR:accum  
B72E 4A:        LSR:accum  
B72F 4A:        LSR:accum  
B730 C9:05      CMP:imm    #05
B732 90:02      BCC:rel    Branch->$B736
B734 A9:00      LDA:imm    #00
B736 A8:        TAY:imp    A-->Y
B737 B9:55 B7   LDA:abs,y  $B755,Y
B73A 85:29      STA:zp     Zp RAM 0029
B73C BD:83 02   LDA:abs,x  $0283,X
B73F 30:0B      BMI:rel    Branch->$B74C
B741 BC:B9 02   LDY:abs,x  $02B9,X
B744 A5:29      LDA:zp     Zp RAM 0029
B746 20:A0 BD   JSR:abs    $BDA0
B749 B8:        CLV:imp    Clear Overflow
B74A 50:08      BVC:rel    Branch->$B754
B74C 20:34 B6   JSR:abs    $B634
B74F A4:29      LDY:zp     Zp RAM 0029
B751 20:CB BD   JSR:abs    $BDCB
B754 60:        RTS:imp    Ret from Sub
B755 0D:0C 0B   ORA:abs    $0B0C
B758 0A:        ASL:accum  
B759 09:09      ORA:imm    #09
B75B A2:0B      LDX:imm    #0B
B75D 86:37      STX:zp     Zp RAM 0037
B75F A6:37      LDX:zp     Zp RAM 0037
B761 BD:D3 02   LDA:abs,x  $02D3,X
B764 F0:1B      BEQ:rel    Branch->$B781
B766 85:57      STA:zp     Zp RAM 0057
B768 85:2F      STA:zp     Zp RAM 002F
B76A E0:08      CPX:imm    #08
B76C BC:AD 02   LDY:abs,x  $02AD,X
B76F B0:05      BCS:rel    Branch->$B776
B771 A9:08      LDA:imm    #08
B773 B8:        CLV:imp    Clear Overflow
B774 50:08      BVC:rel    Branch->$B77E
B776 A5:03      LDA:zp     Zp RAM 0003
B778 0A:        ASL:accum  
B779 29:06      AND:imm    #06
B77B 18:        CLC:imp    Clear Carry
B77C 69:20      ADC:imm    #20
B77E 20:FD BC   JSR:abs    $BCFD
B781 C6:37      DEC:zp     Zp RAM 0037
B783 10:DA      BPL:rel    Branch->$B75F
B785 A0:04      LDY:imm    #04
B787 AD:35 01   LDA:abs    $0135
B78A C9:06      CMP:imm    #06
B78C 90:08      BCC:rel    Branch->$B796
B78E A0:0B      LDY:imm    #0B
B790 C9:08      CMP:imm    #08
B792 90:02      BCC:rel    Branch->$B796
B794 A0:0C      LDY:imm    #0C
B796 8C:08 08   STY:abs    $0808
B799 60:        RTS:imp    Ret from Sub
B79A A0:00      LDY:imm    #00
B79C 84:9E      STY:zp     Zp RAM 009E
B79E A2:07      LDX:imm    #07
B7A0 86:37      STX:zp     Zp RAM 0037
B7A2 A6:37      LDX:zp     Zp RAM 0037
B7A4 BD:0A 03   LDA:abs,x  $030A,X
B7A7 F0:29      BEQ:rel    Branch->$B7D2
B7A9 85:57      STA:zp     Zp RAM 0057
B7AB BD:FA 02   LDA:abs,x  $02FA,X
B7AE 85:29      STA:zp     Zp RAM 0029
B7B0 BC:02 03   LDY:abs,x  $0302,X
B7B3 C0:01      CPY:imm    #01
B7B5 D0:06      BNE:rel    Branch->$B7BD
B7B7 20:EB B7   JSR:abs    $B7EB
B7BA B8:        CLV:imp    Clear Overflow
B7BB 50:15      BVC:rel    Branch->$B7D2
B7BD BD:12 03   LDA:abs,x  $0312,X
B7C0 4A:        LSR:accum  
B7C1 29:FE      AND:imm    #FE
B7C3 C0:02      CPY:imm    #02
B7C5 90:02      BCC:rel    Branch->$B7C9
B7C7 A9:00      LDA:imm    #00
B7C9 18:        CLC:imp    Clear Carry
B7CA 79:E5 B7   ADC:abs,y  $B7E5,Y
B7CD A4:29      LDY:zp     Zp RAM 0029
B7CF 20:FD BC   JSR:abs    $BCFD
B7D2 C6:37      DEC:zp     Zp RAM 0037
B7D4 10:CC      BPL:rel    Branch->$B7A2
B7D6 AD:20 07   LDA:abs    $0720
B7D9 F0:09      BEQ:rel    Branch->$B7E4
B7DB A5:9F      LDA:zp     Zp RAM 009F 	 ; Are we ??? [near] level
B7DD C9:0D      CMP:imm    #0D		 ; 14?
B7DF 90:03      BCC:rel    Branch->$B7E4
B7E1 8D:FF 01   STA:abs    $01FF
B7E4 60:        RTS:imp    Ret from Sub
B7E5 00:        BRK:imp    BREAK
B7E6 00:        BRK:imp    BREAK
B7E7 5A:        Illegal Opcode
B7E8 58:        CLI:imp    Enable IRQ
B7E9 56:1C      LSR:zp,x   Zp RAM 001C
B7EB A4:29      LDY:zp     Zp RAM 0029
B7ED B9:35 04   LDA:abs,y  $0435,Y
B7F0 85:56      STA:zp     Zp RAM 0056
B7F2 B9:45 04   LDA:abs,y  $0445,Y
B7F5 85:58      STA:zp     Zp RAM 0058
B7F7 20:98 C0   JSR:abs    $C098
B7FA A2:61      LDX:imm    #61
B7FC 20:65 C7   JSR:abs    $C765
;
; Start of ROM 136002.118 at $B800.
;
B7FF AE:3B 01   LDX:abs    $013B
B802 CE:3C 01   DEC:abs    $013C
B805 D0:0A      BNE:rel    Branch->$B811
B807 E8:        INX:imp    X=X+1
B808 8E:3B 01   STX:abs    $013B
B80B BD:2A B8   LDA:abs,x  $B82A,X
B80E 8D:3C 01   STA:abs    $013C
B811 BC:3D B8   LDY:abs,x  $B83D,X
B814 30:03      BMI:rel    Branch->$B819
B816 20:4E B8   JSR:abs    $B84E
B819 AD:3B 01   LDA:abs    $013B
B81C 0A:        ASL:accum  
B81D 18:        CLC:imp    Clear Carry
B81E 69:28      ADC:imm    #28
B820 A8:        TAY:imp    A-->Y
B821 BE:C9 CE   LDX:abs,y  $CEC9,Y
B824 B9:C8 CE   LDA:abs,y  $CEC8,Y
B827 4C:57 DF   JMP:abs    $DF57
B82A 02:        Illegal Opcode
B82B 02:        Illegal Opcode
B82C 02:        Illegal Opcode
B82D 02:        Illegal Opcode
B82E 02:        Illegal Opcode
B82F 04:        Illegal Opcode
B830 03:        Illegal Opcode
B831 02:        Illegal Opcode
B832 01:20      ORA:ind,x  20
B834 03:        Illegal Opcode
B835 03:        Illegal Opcode
B836 03:        Illegal Opcode
B837 03:        Illegal Opcode
B838 03:        Illegal Opcode
B839 03:        Illegal Opcode
B83A 03:        Illegal Opcode
B83B 3B:        Illegal Opcode
B83C B8:        CLV:imp    Clear Overflow
B83D 00:        BRK:imp    BREAK
B83E 02:        Illegal Opcode
B83F 02:        Illegal Opcode
B840 02:        Illegal Opcode
B841 02:        Illegal Opcode
B842 02:        Illegal Opcode
B843 02:        Illegal Opcode
B844 02:        Illegal Opcode
B845 04:        Illegal Opcode
B846 06:FF      ASL:zp     Zp RAM 00FF
B848 FF:        Illegal Opcode
B849 FF:        Illegal Opcode
B84A FF:        Illegal Opcode
B84B FF:        Illegal Opcode
B84C FF:        Illegal Opcode
B84D FF:        Illegal Opcode
B84E B9:58 B8   LDA:abs,y  $B858,Y
B851 48:        PHA:imp    Push Accum
B852 B9:57 B8   LDA:abs,y  $B857,Y
B855 48:        PHA:imp    Push Accum
B856 60:        RTS:imp    Ret from Sub
;
; Data segment, per Ken Lui.
;
B857 5E:B8      DATA
B859 74:B8      DATA
B85B 87:B8      DATA
B85D 95:B8      DATA
;
B85F A9:0C      LDA:imm    #0C
B861 8D:0B 08   STA:abs    $080B
B864 85:24      STA:zp     Zp RAM 0024
B866 A9:04      LDA:imm    #04
B868 8D:0A 08   STA:abs    $080A
B86B 85:23      STA:zp     Zp RAM 0023
B86D A9:00      LDA:imm    #00
B86F 85:22      STA:zp     Zp RAM 0022
B871 8D:09 08   STA:abs    $0809
B874 60:        RTS:imp    Ret from Sub
B875 A4:22      LDY:zp     Zp RAM 0022
B877 A2:02      LDX:imm    #02
B879 B5:22      LDA:zp,x   Zp RAM 0022
B87B 48:        PHA:imp    Push Accum
B87C 94:22      STY:zp,x   Zp RAM 0022
B87E 98:        TYA:imp    Y-->A
B87F 9D:09 08   STA:abs,x  $0809,X
B882 68:        PLA:imp    Pull Accum
B883 A8:        TAY:imp    A-->Y
B884 CA:        DEX:imp    X=X-1
B885 10:F2      BPL:rel    Branch->$B879
B887 60:        RTS:imp    Ret from Sub
B888 20:96 C1   JSR:abs    $C196
B88B A9:7F      LDA:imm    #7F
B88D 8D:39 01   STA:abs    $0139
B890 A9:04      LDA:imm    #04
B892 8D:3A 01   STA:abs    $013A
B895 60:        RTS:imp    Ret from Sub
B896 AD:39 01   LDA:abs    $0139
B899 8D:FC 2F   STA:abs    $2FFC
B89C AD:3A 01   LDA:abs    $013A
B89F 09:70      ORA:imm    #70
B8A1 8D:FD 2F   STA:abs    $2FFD
B8A4 A9:C0      LDA:imm    #C0
B8A6 8D:FF 2F   STA:abs    $2FFF
B8A9 AD:39 01   LDA:abs    $0139
B8AC 38:        SEC:imp    Set Carry
B8AD E9:20      SBC:imm    #20
B8AF 10:05      BPL:rel    Branch->$B8B6
B8B1 29:7F      AND:imm    #7F
B8B3 CE:3A 01   DEC:abs    $013A
B8B6 8D:39 01   STA:abs    $0139
B8B9 60:        RTS:imp    Ret from Sub
B8BA A9:3F      LDA:imm    #3F
B8BC A2:F2      LDX:imm    #F2
B8BE 20:39 DF   JSR:abs    $DF39
B8C1 A9:00      LDA:imm    #00
B8C3 85:6A      STA:zp     Zp RAM 006A
B8C5 85:6B      STA:zp     Zp RAM 006B
B8C7 85:6C      STA:zp     Zp RAM 006C
B8C9 85:6D      STA:zp     Zp RAM 006D
B8CB 8D:02 02   STA:abs    $0202
B8CE 85:68      STA:zp     Zp RAM 0068
B8D0 85:69      STA:zp     Zp RAM 0069
B8D2 A9:E0      LDA:imm    #E0
B8D4 85:5F      STA:zp     Zp RAM 005F
B8D6 A9:FF      LDA:imm    #FF
B8D8 85:5B      STA:zp     Zp RAM 005B
B8DA 20:67 B9   JSR:abs    $B967
B8DD 85:77      STA:zp     Zp RAM 0077
B8DF 86:76      STX:zp     Zp RAM 0076
B8E1 A2:0F      LDX:imm    #0F
B8E3 86:37      STX:zp     Zp RAM 0037
B8E5 A6:37      LDX:zp     Zp RAM 0037
B8E7 BD:83 02   LDA:abs,x  $0283,X
B8EA F0:49      BEQ:rel    Branch->$B935
B8EC 85:57      STA:zp     Zp RAM 0057
B8EE BD:63 02   LDA:abs,x  $0263,X
B8F1 85:56      STA:zp     Zp RAM 0056
B8F3 BD:A3 02   LDA:abs,x  $02A3,X
B8F6 85:58      STA:zp     Zp RAM 0058
B8F8 20:98 C0   JSR:abs    $C098
B8FB A9:00      LDA:imm    #00
B8FD 85:73      STA:zp     Zp RAM 0073
B8FF 20:44 B9   JSR:abs    $B944
B902 20:BA C3   JSR:abs    $C3BA
B905 A9:A0      LDA:imm    #A0
B907 20:6A B5   JSR:abs    $B56A
B90A 20:44 B9   JSR:abs    $B944
B90D A2:61      LDX:imm    #61
B90F 20:72 C7   JSR:abs    $C772
B912 20:55 B9   JSR:abs    $B955
B915 20:6C DF   JSR:abs    $DF6C
B918 A5:37      LDA:zp     Zp RAM 0037
B91A 29:07      AND:imm    #07
B91C C9:07      CMP:imm    #07
B91E D0:02      BNE:rel    Branch->$B922
B920 A9:00      LDA:imm    #00
B922 A8:        TAY:imp    A-->Y
B923 84:9E      STY:zp     Zp RAM 009E
B925 A9:08      LDA:imm    #08
B927 20:4C DF   JSR:abs    $DF4C
B92A A9:00      LDA:imm    #00
B92C 20:4A DF   JSR:abs    $DF4A
B92F 20:67 B9   JSR:abs    $B967
B932 20:39 DF   JSR:abs    $DF39
B935 C6:37      DEC:zp     Zp RAM 0037
B937 10:AC      BPL:rel    Branch->$B8E5
B939 20:44 B9   JSR:abs    $B944
B93C A9:01      LDA:imm    #01
B93E 20:6A DF   JSR:abs    $DF6A
B941 20:09 DF   JSR:abs    $DF09
B944 A6:74      LDX:zp     Zp RAM 0074
B946 A4:75      LDY:zp     Zp RAM 0075
B948 A5:76      LDA:zp     Zp RAM 0076
B94A 85:74      STA:zp     Zp RAM 0074
B94C 86:76      STX:zp     Zp RAM 0076
B94E A5:77      LDA:zp     Zp RAM 0077
B950 85:75      STA:zp     Zp RAM 0075
B952 84:77      STY:zp     Zp RAM 0077
B954 60:        RTS:imp    Ret from Sub
B955 A5:57      LDA:zp     Zp RAM 0057
B957 4A:        LSR:accum  
B958 4A:        LSR:accum  
B959 4A:        LSR:accum  
B95A 4A:        LSR:accum  
B95B A0:00      LDY:imm    #00
B95D C8:        INY:imp    Y=Y+1
B95E 4A:        LSR:accum  
B95F D0:FC      BNE:rel    Branch->$B95D
B961 18:        CLC:imp    Clear Carry
B962 69:02      ADC:imm    #02
B964 A0:00      LDY:imm    #00
B966 60:        RTS:imp    Ret from Sub
B967 AD:15 04   LDA:abs    $0415
B96A F0:09      BEQ:rel    Branch->$B975
B96C AD:6F CE   LDA:abs    $CE6F
B96F AE:6E CE   LDX:abs    $CE6E
B972 B8:        CLV:imp    Clear Overflow
B973 50:06      BVC:rel    Branch->$B97B
B975 AD:87 CE   LDA:abs    $CE87
B978 AE:86 CE   LDX:abs    $CE86
B97B 60:        RTS:imp    Ret from Sub
B97C F0:E7      BEQ:rel    Branch->$B965
B97E CF:        Illegal Opcode
B97F AA:        TAX:imp    A-->X
B980 80:        Illegal Opcode
B981 56:31      LSR:zp,x   Zp RAM 0031
B983 19:10 19   ORA:abs,y  $1910,Y
B986 31:56      AND:ind,y  56
B988 80:        Illegal Opcode
B989 AA:        TAX:imp    A-->X
B98A CF:        Illegal Opcode
B98B E7:        Illegal Opcode
B98C F0:F0      BEQ:rel    Branch->$B97E
B98E F0:B8      BEQ:rel    Branch->$B948
B990 80:        Illegal Opcode
B991 48:        PHA:imp    Push Accum
B992 10:10      BPL:rel    Branch->$B9A4
B994 10:10      BPL:rel    Branch->$B9A6
B996 10:48      BPL:rel    Branch->$B9E0
B998 80:        Illegal Opcode
B999 B8:        CLV:imp    Clear Overflow
B99A F0:F0      BEQ:rel    Branch->$B98C
B99C F0:F0      BEQ:rel    Branch->$B98E
B99E B8:        CLV:imp    Clear Overflow
B99F B8:        CLV:imp    Clear Overflow
B9A0 80:        Illegal Opcode
B9A1 48:        PHA:imp    Push Accum
B9A2 48:        PHA:imp    Push Accum
B9A3 10:10      BPL:rel    Branch->$B9B5
B9A5 10:48      BPL:rel    Branch->$B9EF
B9A7 48:        PHA:imp    Push Accum
B9A8 80:        Illegal Opcode
B9A9 B8:        CLV:imp    Clear Overflow
B9AA B8:        CLV:imp    Clear Overflow
B9AB F0:EC      BEQ:rel    Branch->$B999
B9AD D5:B1      CMP:zp,x   Zp RAM 00B1
B9AF 90:70      BCC:rel    Branch->$BA21
B9B1 4F:        Illegal Opcode
B9B2 2B:        Illegal Opcode
B9B3 14:        Illegal Opcode
B9B4 14:        Illegal Opcode
B9B5 2B:        Illegal Opcode
B9B6 4F:        Illegal Opcode
B9B7 70:90      BVS:rel    Branch->$B949
B9B9 B1:D5      LDA:ind,y  D5
B9BB EC:F0 C0   CPX:abs    $C0F0
B9BE A0:94      LDY:imm    #94
B9C0 6C:60 40   JMP:ind    $4060
B9C3 10:10      BPL:rel    Branch->$B9D5
B9C5 40:        RTI:imp    Ret from Int
B9C6 60:        RTS:imp    Ret from Sub
B9C7 6C:94 A0   JMP:ind    $A094
B9CA C0:F0      CPY:imm    #F0
B9CC D9:C2 AC   CMP:abs,y  $ACC2,Y
B9CF 97:        Illegal Opcode
B9D0 80:        Illegal Opcode
B9D1 69:52      ADC:imm    #52
B9D3 3C:        Illegal Opcode
B9D4 27:        Illegal Opcode
B9D5 10:35      BPL:rel    Branch->$BA0C
B9D7 5A:        Illegal Opcode
B9D8 80:        Illegal Opcode
B9D9 A6:CA      LDX:zp     Zp RAM 00CA
B9DB F0:EA      BEQ:rel    Branch->$B9C7
B9DD E0:9C      CPX:imm    #9C
B9DF 80:        Illegal Opcode
B9E0 64:        Illegal Opcode
B9E1 20:16 50   JSR:abs    $5016
B9E4 16:20      ASL:zp,x   Zp RAM 0020
B9E6 64:        Illegal Opcode
B9E7 80:        Illegal Opcode
B9E8 9C:        Illegal Opcode
B9E9 E0:EA      CPX:imm    #EA
B9EB B0:10      BCS:rel    Branch->$B9FD
B9ED 1E:2C 3A   ASL:abs,x  $3A2C,X
B9F0 48:        PHA:imp    Push Accum
B9F1 56:64      LSR:zp,x   Zp RAM 0064
B9F3 70:90      BVS:rel    Branch->$B985
B9F5 9E:        Illegal Opcode
B9F6 AC:BA C8   LDY:abs    $C8BA
B9F9 D6:E4      DEC:zp,x   Zp RAM 00E4
B9FB F0:10      BEQ:rel    Branch->$BA0D
B9FD 1E:2D 3C   ASL:abs,x  $3C2D,X
BA00 4B:        Illegal Opcode
BA01 5A:        Illegal Opcode
BA02 69:78      ADC:imm    #78
BA04 87:        Illegal Opcode
BA05 96:A5      STX:zp,y   Zp RAM 00A5
BA07 B4:C3      LDY:zp,x   Zp RAM 00C3
BA09 D2:        Illegal Opcode
BA0A E1:F0      SBC:ind,x  F0
BA0C 10:10      BPL:rel    Branch->$BA1E
BA0E 10:10      BPL:rel    Branch->$BA20
BA10 16:29      ASL:zp,x   Zp RAM 0029
BA12 46:69      LSR:zp     Zp RAM 0069
BA14 97:        Illegal Opcode
BA15 BA:        TSX:imp    Stack-->X
BA16 D7:        Illegal Opcode
BA17 EA:        NOP:imp    No Operation
BA18 F0:F0      BEQ:rel    Branch->$BA0A
BA1A F0:F0      BEQ:rel    Branch->$BA0C
BA1C 10:24      BPL:rel    Branch->$BA42
BA1E 30:36      BMI:rel    Branch->$BA56
BA20 3E:49 5A   ROL:abs,x  $5A49,X
BA23 75:94      ADC:zp,x   Zp RAM 0094
BA25 A4:AC      LDY:zp     Zp RAM 00AC
BA27 BA:        TSX:imp    Stack-->X
BA28 DA:        Illegal Opcode
BA29 E2:        Illegal Opcode
BA2A EA:        NOP:imp    No Operation
BA2B F0:80      BEQ:rel    Branch->$B9AD
BA2D 70:48      BVS:rel    Branch->$BA77
BA2F 20:10 20   JSR:abs    $2010
BA32 48:        PHA:imp    Push Accum
BA33 70:80      BVS:rel    Branch->$B9B5
BA35 90:B8      BCC:rel    Branch->$B9EF
BA37 E0:F0      CPX:imm    #F0
BA39 E0:B8      CPX:imm    #B8
BA3B 90:DA      BCC:rel    Branch->$BA17
BA3D A4:87      LDY:zp     Zp RAM 0087
BA3F 80:        Illegal Opcode
BA40 79:5C 26   ADC:abs,y  $265C,Y
BA43 10:10      BPL:rel    Branch->$BA55
BA45 20:48 80   JSR:abs    $8048
BA48 B8:        CLV:imp    Clear Overflow
BA49 E0:F0      CPX:imm    #F0
BA4B F0:10      BEQ:rel    Branch->$BA5D
BA4D 10:30      BPL:rel    Branch->$BA7F
BA4F 30:50      BMI:rel    Branch->$BAA1
BA51 50:70      BVC:rel    Branch->$BAC3
BA53 70:90      BVS:rel    Branch->$B9E5
BA55 90:B0      BCC:rel    Branch->$BA07
BA57 B0:D0      BCS:rel    Branch->$BA29
BA59 D0:F0      BNE:rel    Branch->$BA4B
BA5B F0:B0      BEQ:rel    Branch->$BA0D
BA5D 80:        Illegal Opcode
BA5E 50:47      BVC:rel    Branch->$BAA7
BA60 18:        CLC:imp    Clear Carry
BA61 30:18      BMI:rel    Branch->$BA7B
BA63 47:        Illegal Opcode
BA64 50:80      BVC:rel    Branch->$B9E6
BA66 B0:B9      BCS:rel    Branch->$BA21
BA68 E8:        INX:imp    X=X+1
BA69 D4:        Illegal Opcode
BA6A E8:        INX:imp    X=X+1
BA6B B9:10 1E   LDA:abs,y  $1E10,Y
BA6E 21:28      AND:ind,x  28
BA70 3C:        Illegal Opcode
BA71 55:66      EOR:zp,x   Zp RAM 0066
BA73 73:        Illegal Opcode
BA74 8D:9A AB   STA:abs    $AB9A
BA77 C4:D8      CPY:zp     Zp RAM 00D8
BA79 DF:        Illegal Opcode
BA7A E2:        Illegal Opcode
BA7B F0:80      BEQ:rel    Branch->$B9FD
BA7D AA:        TAX:imp    A-->X
BA7E CF:        Illegal Opcode
BA7F E7:        Illegal Opcode
BA80 F0:E7      BEQ:rel    Branch->$BA69
BA82 CF:        Illegal Opcode
BA83 AA:        TAX:imp    A-->X
BA84 80:        Illegal Opcode
BA85 56:31      LSR:zp,x   Zp RAM 0031
BA87 19:10 19   ORA:abs,y  $1910,Y
BA8A 31:56      AND:ind,y  56
BA8C 80:        Illegal Opcode
BA8D B8:        CLV:imp    Clear Overflow
BA8E F0:F0      BEQ:rel    Branch->$BA80
BA90 F0:F0      BEQ:rel    Branch->$BA82
BA92 F0:B8      BEQ:rel    Branch->$BA4C
BA94 80:        Illegal Opcode
BA95 48:        PHA:imp    Push Accum
BA96 10:10      BPL:rel    Branch->$BAA8
BA98 10:10      BPL:rel    Branch->$BAAA
BA9A 10:48      BPL:rel    Branch->$BAE4
BA9C 80:        Illegal Opcode
BA9D B8:        CLV:imp    Clear Overflow
BA9E B8:        CLV:imp    Clear Overflow
BA9F F0:F0      BEQ:rel    Branch->$BA91
BAA1 F0:B8      BEQ:rel    Branch->$BA5B
BAA3 B8:        CLV:imp    Clear Overflow
BAA4 80:        Illegal Opcode
BAA5 48:        PHA:imp    Push Accum
BAA6 48:        PHA:imp    Push Accum
BAA7 10:10      BPL:rel    Branch->$BAB9
BAA9 10:48      BPL:rel    Branch->$BAF3
BAAB 48:        PHA:imp    Push Accum
BAAC 94:B0      STY:zp,x   Zp RAM 00B0
BAAE B8:        CLV:imp    Clear Overflow
BAAF A7:        Illegal Opcode
BAB0 A7:        Illegal Opcode
BAB1 B8:        CLV:imp    Clear Overflow
BAB2 B0:94      BCS:rel    Branch->$BA48
BAB4 6C:50 48   JMP:ind    $4850
BAB7 59:59 48   EOR:abs,y  $4859,Y
BABA 50:6C      BVC:rel    Branch->$BB28
BABC 96:A3      STX:zp,y   Zp RAM 00A3
BABE C5:F0      CMP:zp     Zp RAM 00F0
BAC0 F0:C5      BEQ:rel    Branch->$BA87
BAC2 A3:        Illegal Opcode
BAC3 96:6A      STX:zp,y   Zp RAM 006A
BAC5 5D:3B 10   EOR:abs,x  $103B,X
BAC8 10:3B      BPL:rel    Branch->$BB05
BACA 5D:6A 3D   EOR:abs,x  $3D6A,X
BACD 6A:        ROT:accum  
BACE 97:        Illegal Opcode
BACF C4:F0      CPY:zp     Zp RAM 00F0
BAD1 C4:97      CPY:zp     Zp RAM 0097
BAD3 6A:        ROT:accum  
BAD4 3D:10 10   AND:abs,x  $1010,X
BAD7 10:10      BPL:rel    Branch->$BAE9
BAD9 10:10      BPL:rel    Branch->$BAEB
BADB 10:A0      BPL:rel    Branch->$BA7D
BADD E0:EA      CPX:imm    #EA
BADF B0:EA      BCS:rel    Branch->$BACB
BAE1 E0:A0      CPX:imm    #A0
BAE3 80:        Illegal Opcode
BAE4 60:        RTS:imp    Ret from Sub
BAE5 20:16 50   JSR:abs    $5016
BAE8 16:20      ASL:zp,x   Zp RAM 0020
BAEA 60:        RTS:imp    Ret from Sub
BAEB 80:        Illegal Opcode
BAEC F0:D0      BEQ:rel    Branch->$BABE
BAEE B0:90      BCS:rel    Branch->$BA80
BAF0 70:50      BVS:rel    Branch->$BB42
BAF2 30:10      BMI:rel    Branch->$BB04
BAF4 10:30      BPL:rel    Branch->$BB26
BAF6 50:70      BVC:rel    Branch->$BB68
BAF8 90:B0      BCC:rel    Branch->$BAAA
BAFA D0:F0      BNE:rel    Branch->$BAEC
BAFC 40:        RTI:imp    Ret from Int
BAFD 40:        RTI:imp    Ret from Int
BAFE 40:        RTI:imp    Ret from Int
BAFF 40:        RTI:imp    Ret from Int
BB00 40:        RTI:imp    Ret from Int
BB01 40:        RTI:imp    Ret from Int
BB02 40:        RTI:imp    Ret from Int
BB03 40:        RTI:imp    Ret from Int
BB04 40:        RTI:imp    Ret from Int
BB05 40:        RTI:imp    Ret from Int
BB06 40:        RTI:imp    Ret from Int
BB07 40:        RTI:imp    Ret from Int
BB08 40:        RTI:imp    Ret from Int
BB09 40:        RTI:imp    Ret from Int
BB0A 40:        RTI:imp    Ret from Int
BB0B 40:        RTI:imp    Ret from Int
BB0C F0:CB      BEQ:rel    Branch->$BAD9
BB0E A6:80      LDX:zp     Zp RAM 0080
BB10 5C:        Illegal Opcode
BB11 39:20 12   AND:abs,y  $1220,Y
BB14 12:        Illegal Opcode
BB15 20:39 5C   JSR:abs    $5C39
BB18 80:        Illegal Opcode
BB19 A6:CB      LDX:zp     Zp RAM 00CB
BB1B F0:C0      BEQ:rel    Branch->$BADD
BB1D A6:8A      LDX:zp     Zp RAM 008A
BB1F 6A:        ROT:accum  
BB20 4A:        LSR:accum  
BB21 2F:        Illegal Opcode
BB22 14:        Illegal Opcode
BB23 24:20      BIT:zp     Zp RAM 0020
BB25 39:59 75   AND:abs,y  $7559,Y
BB28 72:        Illegal Opcode
BB29 90:B0      BCC:rel    Branch->$BADB
BB2B D0:80      BNE:rel    Branch->$BAAD
BB2D 57:        Illegal Opcode
BB2E 48:        PHA:imp    Push Accum
BB2F 57:        Illegal Opcode
BB30 80:        Illegal Opcode
BB31 A9:BA      LDA:imm    #BA
BB33 A9:80      LDA:imm    #80
BB35 57:        Illegal Opcode
BB36 48:        PHA:imp    Push Accum
BB37 57:        Illegal Opcode
BB38 80:        Illegal Opcode
BB39 A9:BA      LDA:imm    #BA
BB3B A9:E4      LDA:imm    #E4
BB3D E8:        INX:imp    X=X+1
BB3E B7:        Illegal Opcode
BB3F 80:        Illegal Opcode
BB40 B7:        Illegal Opcode
BB41 E8:        INX:imp    X=X+1
BB42 E4:B2      CPX:zp     Zp RAM 00B2
BB44 7A:        Illegal Opcode
BB45 47:        Illegal Opcode
BB46 20:10 20   JSR:abs    $2010
BB49 47:        Illegal Opcode
BB4A 7A:        Illegal Opcode
BB4B B2:        Illegal Opcode
BB4C 90:70      BCC:rel    Branch->$BBBE
BB4E 70:50      BVS:rel    Branch->$BBA0
BB50 50:30      BVC:rel    Branch->$BB82
BB52 30:10      BMI:rel    Branch->$BB64
BB54 10:30      BPL:rel    Branch->$BB86
BB56 30:50      BMI:rel    Branch->$BBA8
BB58 50:70      BVC:rel    Branch->$BBCA
BB5A 70:90      BVS:rel    Branch->$BAEC
BB5C E6:D0      INC:zp     Zp RAM 00D0
BB5E E6:B9      INC:zp     Zp RAM 00B9
BB60 AE:80 52   LDX:abs    $5280
BB63 47:        Illegal Opcode
BB64 14:        Illegal Opcode
BB65 30:14      BMI:rel    Branch->$BB7B
BB67 47:        Illegal Opcode
BB68 52:        Illegal Opcode
BB69 80:        Illegal Opcode
BB6A AE:B9 7E   LDX:abs    $7EB9
BB6D 6A:        ROT:accum  
BB6E 51:3A      EOR:ind,y  3A
BB70 2C:2C 38   BIT:abs    $382C
BB73 4E:4E 38   LSR:abs    $384E
BB76 2C:2C 3A   BIT:abs    $3A2C
BB79 51:6A      EOR:ind,y  6A
BB7B 7E:05 06   ROR:abs,x  $0605,X
BB7E 07:        Illegal Opcode
BB7F 08:        PHP:imp    Push P
BB80 09:0A      ORA:imm    #0A
BB82 0B:        Illegal Opcode
BB83 0C:        Illegal Opcode
BB84 0D:0E 0F   ORA:abs    $0F0E
BB87 00:        BRK:imp    BREAK
BB88 01:02      ORA:ind,x  02
BB8A 03:        Illegal Opcode
BB8B 04:        Illegal Opcode
BB8C 04:        Illegal Opcode
BB8D 04:        Illegal Opcode
BB8E 08:        PHP:imp    Push P
BB8F 08:        PHP:imp    Push P
BB90 08:        PHP:imp    Push P
BB91 08:        PHP:imp    Push P
BB92 0C:        Illegal Opcode
BB93 0C:        Illegal Opcode
BB94 0C:        Illegal Opcode
BB95 0C:        Illegal Opcode
BB96 00:        BRK:imp    BREAK
BB97 00:        BRK:imp    BREAK
BB98 00:        BRK:imp    BREAK
BB99 00:        BRK:imp    BREAK
BB9A 04:        Illegal Opcode
BB9B 04:        Illegal Opcode
BB9C 04:        Illegal Opcode
BB9D 08:        PHP:imp    Push P
BB9E 04:        Illegal Opcode
BB9F 08:        PHP:imp    Push P
BBA0 08:        PHP:imp    Push P
BBA1 0C:        Illegal Opcode
BBA2 08:        PHP:imp    Push P
BBA3 0C:        Illegal Opcode
BBA4 0C:        Illegal Opcode
BBA5 00:        BRK:imp    BREAK
BBA6 0C:        Illegal Opcode
BBA7 00:        BRK:imp    BREAK
BBA8 00:        BRK:imp    BREAK
BBA9 04:        Illegal Opcode
BBAA 00:        BRK:imp    BREAK
BBAB 04:        Illegal Opcode
BBAC 06:07      ASL:zp     Zp RAM 0007
BBAE 09:08      ORA:imm    #08
BBB0 07:        Illegal Opcode
BBB1 09:0A      ORA:imm    #0A
BBB3 0C:        Illegal Opcode
BBB4 0E:0F 01   ASL:abs    $010F
BBB7 00:        BRK:imp    BREAK
BBB8 0F:        Illegal Opcode
BBB9 01:02      ORA:ind,x  02
BBBB 04:        Illegal Opcode
BBBC 07:        Illegal Opcode
BBBD 06:05      ASL:zp     Zp RAM 0005
BBBF 08:        PHP:imp    Push P
BBC0 0B:        Illegal Opcode
BBC1 0A:        ASL:accum  
BBC2 09:0C      ORA:imm    #0C
BBC4 0F:        Illegal Opcode
BBC5 0E:0D 00   ASL:abs    $000D
BBC8 03:        Illegal Opcode
BBC9 02:        Illegal Opcode
BBCA 01:04      ORA:ind,x  04
BBCC 05:05      ORA:zp     Zp RAM 0005
BBCE 05:05      ORA:zp     Zp RAM 0005
BBD0 0B:        Illegal Opcode
BBD1 0B:        Illegal Opcode
BBD2 0B:        Illegal Opcode
BBD3 0B:        Illegal Opcode
BBD4 0B:        Illegal Opcode
BBD5 00:        BRK:imp    BREAK
BBD6 00:        BRK:imp    BREAK
BBD7 00:        BRK:imp    BREAK
BBD8 00:        BRK:imp    BREAK
BBD9 00:        BRK:imp    BREAK
BBDA 00:        BRK:imp    BREAK
BBDB 05:04      ORA:zp     Zp RAM 0004
BBDD 08:        PHP:imp    Push P
BBDE 0B:        Illegal Opcode
BBDF 05:08      ORA:zp     Zp RAM 0008
BBE1 0C:        Illegal Opcode
BBE2 0E:09 0C   ASL:abs    $0C09
BBE5 00:        BRK:imp    BREAK
BBE6 03:        Illegal Opcode
BBE7 0D:00 04   ORA:abs    $0400
BBEA 07:        Illegal Opcode
BBEB 02:        Illegal Opcode
BBEC 0D:0D 0D   ORA:abs    $0D0D
BBEF 0D:0D 0D   ORA:abs    $0D0D
BBF2 0D:00 03   ORA:abs    $0300
BBF5 03:        Illegal Opcode
BBF6 03:        Illegal Opcode
BBF7 03:        Illegal Opcode
BBF8 03:        Illegal Opcode
BBF9 03:        Illegal Opcode
BBFA 03:        Illegal Opcode
BBFB 00:        BRK:imp    BREAK
BBFC 00:        BRK:imp    BREAK
BBFD 00:        BRK:imp    BREAK
BBFE 00:        BRK:imp    BREAK
BBFF 00:        BRK:imp    BREAK
BC00 00:        BRK:imp    BREAK
BC01 00:        BRK:imp    BREAK
BC02 00:        BRK:imp    BREAK
BC03 00:        BRK:imp    BREAK
BC04 00:        BRK:imp    BREAK
BC05 00:        BRK:imp    BREAK
BC06 00:        BRK:imp    BREAK
BC07 00:        BRK:imp    BREAK
BC08 00:        BRK:imp    BREAK
BC09 00:        BRK:imp    BREAK
BC0A 00:        BRK:imp    BREAK
BC0B 00:        BRK:imp    BREAK
BC0C 0C:        Illegal Opcode
BC0D 0C:        Illegal Opcode
BC0E 0C:        Illegal Opcode
BC0F 0D:0E 0F   ORA:abs    $0F0E
BC12 0F:        Illegal Opcode
BC13 00:        BRK:imp    BREAK
BC14 01:01      ORA:ind,x  01
BC16 02:        Illegal Opcode
BC17 03:        Illegal Opcode
BC18 04:        Illegal Opcode
BC19 04:        Illegal Opcode
BC1A 04:        Illegal Opcode
BC1B 00:        BRK:imp    BREAK
BC1C 0E:0D 0C   ASL:abs    $0C0D
BC1F 0D:0D 0D   ORA:abs    $0D0D
BC22 01:0F      ORA:ind,x  0F
BC24 02:        Illegal Opcode
BC25 03:        Illegal Opcode
BC26 03:        Illegal Opcode
BC27 00:        BRK:imp    BREAK
BC28 03:        Illegal Opcode
BC29 03:        Illegal Opcode
BC2A 03:        Illegal Opcode
BC2B 00:        BRK:imp    BREAK
BC2C 0B:        Illegal Opcode
BC2D 09:07      ORA:imm    #07
BC2F 05:03      ORA:zp     Zp RAM 0003
BC31 01:0F      ORA:ind,x  0F
BC33 0D:0D 0F   ORA:abs    $0F0D
BC36 01:03      ORA:ind,x  03
BC38 05:07      ORA:zp     Zp RAM 0007
BC3A 09:0B      ORA:imm    #0B
BC3C 08:        PHP:imp    Push P
BC3D 0B:        Illegal Opcode
BC3E 0C:        Illegal Opcode
BC3F 04:        Illegal Opcode
BC40 05:08      ORA:zp     Zp RAM 0008
BC42 0B:        Illegal Opcode
BC43 0C:        Illegal Opcode
BC44 0D:0E 0F   ORA:abs    $0F0E
BC47 01:02      ORA:ind,x  02
BC49 03:        Illegal Opcode
BC4A 04:        Illegal Opcode
BC4B 05:0C      ORA:zp     Zp RAM 000C
BC4D 00:        BRK:imp    BREAK
BC4E 0C:        Illegal Opcode
BC4F 00:        BRK:imp    BREAK
BC50 0C:        Illegal Opcode
BC51 00:        BRK:imp    BREAK
BC52 0C:        Illegal Opcode
BC53 00:        BRK:imp    BREAK
BC54 04:        Illegal Opcode
BC55 00:        BRK:imp    BREAK
BC56 04:        Illegal Opcode
BC57 00:        BRK:imp    BREAK
BC58 04:        Illegal Opcode
BC59 00:        BRK:imp    BREAK
BC5A 04:        Illegal Opcode
BC5B 00:        BRK:imp    BREAK
BC5C 0A:        ASL:accum  
BC5D 06:0C      ASL:zp     Zp RAM 000C
BC5F 08:        PHP:imp    Push P
BC60 0E:0A 00   ASL:abs    $000A
BC63 0C:        Illegal Opcode
BC64 02:        Illegal Opcode
BC65 0E:04 00   ASL:abs    $0004
BC68 06:02      ASL:zp     Zp RAM 0002
BC6A 08:        PHP:imp    Push P
BC6B 04:        Illegal Opcode
BC6C 0E:0C 0D   ASL:abs    $0D0C
BC6F 0E:00 02   ASL:abs    $0200
BC72 02:        Illegal Opcode
BC73 00:        BRK:imp    BREAK
BC74 0E:0E 00   ASL:abs    $000E
BC77 02:        Illegal Opcode
BC78 03:        Illegal Opcode
BC79 04:        Illegal Opcode
BC7A 02:        Illegal Opcode
BC7B 00:        BRK:imp    BREAK
BC7C 00:        BRK:imp    BREAK
BC7D 01:02      ORA:ind,x  02
BC7F 03:        Illegal Opcode
BC80 04:        Illegal Opcode
BC81 05:06      ORA:zp     Zp RAM 0006
BC83 07:        Illegal Opcode
BC84 0D:09 08   ORA:abs    $0809
BC87 0C:        Illegal Opcode
BC88 0E:0F 0A   ASL:abs    $0A0F
BC8B 0B:        Illegal Opcode
BC8C 18:        CLC:imp    Clear Carry
BC8D 1C:        Illegal Opcode
BC8E 18:        CLC:imp    Clear Carry
BC8F 0F:        Illegal Opcode
BC90 18:        CLC:imp    Clear Carry
BC91 18:        CLC:imp    Clear Carry
BC92 18:        CLC:imp    Clear Carry
BC93 18:        CLC:imp    Clear Carry
BC94 0A:        ASL:accum  
BC95 18:        CLC:imp    Clear Carry
BC96 10:0F      BPL:rel    Branch->$BCA7
BC98 18:        CLC:imp    Clear Carry
BC99 0C:        Illegal Opcode
BC9A 14:        Illegal Opcode
BC9B 0A:        ASL:accum  
BC9C 50:50      BVC:rel    Branch->$BCEE
BC9E 50:68      BVC:rel    Branch->$BD08
BCA0 50:50      BVC:rel    Branch->$BCF2
BCA2 68:        PLA:imp    Pull Accum
BCA3 B0:A0      BCS:rel    Branch->$BC45
BCA5 50:90      BVC:rel    Branch->$BC37
BCA7 80:        Illegal Opcode
BCA8 20:B0 60   JSR:abs    $60B0
BCAB A0:40      LDY:imm    #40
BCAD 20:40 80   JSR:abs    $8040
BCB0 40:        RTI:imp    Ret from Int
BCB1 40:        RTI:imp    Ret from Int
BCB2 70:60      BVS:rel    Branch->$BD14
BCB4 00:        BRK:imp    BREAK
BCB5 20:40 00   JSR:abs    $0040
BCB8 A0:40      LDY:imm    #40
BCBA 40:        RTI:imp    Ret from Int
BCBB 00:        BRK:imp    BREAK
BCBC FF:        Illegal Opcode
BCBD FF:        Illegal Opcode
BCBE FF:        Illegal Opcode
BCBF FF:        Illegal Opcode
BCC0 FF:        Illegal Opcode
BCC1 FF:        Illegal Opcode
BCC2 FF:        Illegal Opcode
BCC3 00:        BRK:imp    BREAK
BCC4 01:FF      ORA:ind,x  FF
BCC6 00:        BRK:imp    BREAK
BCC7 00:        BRK:imp    BREAK
BCC8 FE:01 FF   INC:abs,x  $FF01,X
BCCB 01:00      ORA:ind,x  00
BCCD 00:        BRK:imp    BREAK
BCCE 00:        BRK:imp    BREAK
BCCF 00:        BRK:imp    BREAK
BCD0 00:        BRK:imp    BREAK
BCD1 00:        BRK:imp    BREAK
BCD2 00:        BRK:imp    BREAK
BCD3 FF:        Illegal Opcode
BCD4 FF:        Illegal Opcode
BCD5 FF:        Illegal Opcode
BCD6 FF:        Illegal Opcode
BCD7 00:        BRK:imp    BREAK
BCD8 00:        BRK:imp    BREAK
BCD9 FF:        Illegal Opcode
BCDA 00:        BRK:imp    BREAK
BCDB FF:        Illegal Opcode
BCDC 00:        BRK:imp    BREAK
BCDD 00:        BRK:imp    BREAK
BCDE 60:        RTS:imp    Ret from Sub
BCDF 40:        RTI:imp    Ret from Int
BCE0 00:        BRK:imp    BREAK
BCE1 00:        BRK:imp    BREAK
BCE2 48:        PHA:imp    Push Accum
BCE3 40:        RTI:imp    Ret from Int
BCE4 50:28      BVC:rel    Branch->$BD0E
BCE6 50:00      BVC:rel    Branch->$BCE8
BCE8 00:        BRK:imp    BREAK
BCE9 50:00      BVC:rel    Branch->$BCEB
BCEB 40:        RTI:imp    Ret from Int
BCEC 04:        Illegal Opcode
BCED 04:        Illegal Opcode
BCEE 03:        Illegal Opcode
BCEF 04:        Illegal Opcode
BCF0 04:        Illegal Opcode
BCF1 04:        Illegal Opcode
BCF2 03:        Illegal Opcode
BCF3 04:        Illegal Opcode
BCF4 05:04      ORA:zp     Zp RAM 0004
BCF6 04:        Illegal Opcode
BCF7 04:        Illegal Opcode
BCF8 04:        Illegal Opcode
BCF9 04:        Illegal Opcode
BCFA 04:        Illegal Opcode
BCFB 05:3E      ORA:zp     Zp RAM 003E
BCFD 85:55      STA:zp     Zp RAM 0055
BCFF B9:35 04   LDA:abs,y  $0435,Y
BD02 85:56      STA:zp     Zp RAM 0056
BD04 B9:45 04   LDA:abs,y  $0445,Y
BD07 85:58      STA:zp     Zp RAM 0058
BD09 20:98 C0   JSR:abs    $C098
BD0C A2:61      LDX:imm    #61
BD0E 20:65 C7   JSR:abs    $C765
BD11 A9:00      LDA:imm    #00
BD13 85:A9      STA:zp     Zp RAM 00A9
BD15 20:3E BD   JSR:abs    $BD3E
BD18 A5:78      LDA:zp     Zp RAM 0078
BD1A 49:07      EOR:imm    #07
BD1C 0A:        ASL:accum  
BD1D C9:0A      CMP:imm    #0A
BD1F B0:02      BCS:rel    Branch->$BD23
BD21 A9:0A      LDA:imm    #0A
BD23 0A:        ASL:accum  
BD24 0A:        ASL:accum  
BD25 0A:        ASL:accum  
BD26 0A:        ASL:accum  
BD27 91:74      STA:ind,y  74
BD29 C8:        INY:imp    Y=Y+1
BD2A A9:60      LDA:imm    #60
BD2C 91:74      STA:ind,y  74
BD2E C8:        INY:imp    Y=Y+1
BD2F 84:A9      STY:zp     Zp RAM 00A9
BD31 A4:55      LDY:zp     Zp RAM 0055
BD33 BE:C9 CE   LDX:abs,y  $CEC9,Y
BD36 B9:C8 CE   LDA:abs,y  $CEC8,Y
BD39 A4:A9      LDY:zp     Zp RAM 00A9
BD3B 4C:59 DF   JMP:abs    $DF59
BD3E A5:57      LDA:zp     Zp RAM 0057
BD40 C9:10      CMP:imm    #10
BD42 90:48      BCC:rel    Branch->$BD8C
BD44 38:        SEC:imp    Set Carry
BD45 E5:5F      SBC:zp     Zp RAM 005F
BD47 8D:95 60   STA:abs    $6095
BD4A A9:00      LDA:imm    #00
BD4C E5:5B      SBC:zp     Zp RAM 005B
BD4E 8D:96 60   STA:abs    $6096
BD51 A9:18      LDA:imm    #18
BD53 8D:8C 60   STA:abs    $608C
BD56 A5:A0      LDA:zp     Zp RAM 00A0
BD58 8D:8E 60   STA:abs    $608E
BD5B 8D:94 60   STA:abs    $6094
BD5E 2C:40 60   BIT:abs    $6040	 ; Check mathbox status
BD61 30:FB      BMI:rel    Branch->$BD5E
BD63 AD:60 60   LDA:abs    $6060
BD66 85:79      STA:zp     Zp RAM 0079
BD68 AD:70 60   LDA:abs    $6070
BD6B 85:7A      STA:zp     Zp RAM 007A
BD6D A2:0F      LDX:imm    #0F
BD6F 8E:8C 60   STX:abs    $608C
BD72 38:        SEC:imp    Set Carry
BD73 E9:01      SBC:imm    #01
BD75 D0:02      BNE:rel    Branch->$BD79
BD77 A9:01      LDA:imm    #01
BD79 A2:00      LDX:imm    #00
BD7B E8:        INX:imp    X=X+1
BD7C 06:79      ASL:zp     Zp RAM 0079
BD7E 2A:        ROL:accum  
BD7F 90:FA      BCC:rel    Branch->$BD7B
BD81 4A:        LSR:accum  
BD82 49:7F      EOR:imm    #7F
BD84 18:        CLC:imp    Clear Carry
BD85 69:01      ADC:imm    #01
BD87 A8:        TAY:imp    A-->Y
BD88 8A:        TXA:imp    X-->A
BD89 B8:        CLV:imp    Clear Overflow
BD8A 50:04      BVC:rel    Branch->$BD90
BD8C A9:01      LDA:imm    #01
BD8E A0:00      LDY:imm    #00
BD90 85:78      STA:zp     Zp RAM 0078
BD92 48:        PHA:imp    Push Accum
BD93 98:        TYA:imp    Y-->A
BD94 A4:A9      LDY:zp     Zp RAM 00A9
BD96 91:74      STA:ind,y  74
BD98 C8:        INY:imp    Y=Y+1
BD99 68:        PLA:imp    Pull Accum
BD9A 09:70      ORA:imm    #70
BD9C 91:74      STA:ind,y  74
BD9E C8:        INY:imp    Y=Y+1
BD9F 60:        RTS:imp    Ret from Sub
BDA0 85:36      STA:zp     Zp RAM 0036
BDA2 B9:CE 03   LDA:abs,y  $03CE,Y
BDA5 85:56      STA:zp     Zp RAM 0056
BDA7 B9:DE 03   LDA:abs,y  $03DE,Y
BDAA 85:58      STA:zp     Zp RAM 0058
BDAC A5:57      LDA:zp     Zp RAM 0057
BDAE 85:2F      STA:zp     Zp RAM 002F
BDB0 98:        TYA:imp    Y-->A
BDB1 18:        CLC:imp    Clear Carry
BDB2 69:01      ADC:imm    #01
BDB4 29:0F      AND:imm    #0F
BDB6 AA:        TAX:imp    A-->X
BDB7 BD:CE 03   LDA:abs,x  $03CE,X
BDBA 85:2E      STA:zp     Zp RAM 002E
BDBC BD:DE 03   LDA:abs,x  $03DE,X
BDBF 85:30      STA:zp     Zp RAM 0030
BDC1 A9:00      LDA:imm    #00
BDC3 85:59      STA:zp     Zp RAM 0059
BDC5 A9:04      LDA:imm    #04
BDC7 85:5A      STA:zp     Zp RAM 005A
BDC9 A4:36      LDY:zp     Zp RAM 0036
BDCB A5:5B      LDA:zp     Zp RAM 005B
BDCD 30:07      BMI:rel    Branch->$BDD6
BDCF A5:57      LDA:zp     Zp RAM 0057
BDD1 C5:5F      CMP:zp     Zp RAM 005F
BDD3 B0:01      BCS:rel    Branch->$BDD6
BDD5 60:        RTS:imp    Ret from Sub
BDD6 B9:B6 BF   LDA:abs,y  $BFB6,Y
BDD9 85:99      STA:zp     Zp RAM 0099
BDDB B9:C4 BF   LDA:abs,y  $BFC4,Y
BDDE 85:38      STA:zp     Zp RAM 0038
BDE0 A4:9E      LDY:zp     Zp RAM 009E
BDE2 A9:08      LDA:imm    #08
BDE4 20:4C DF   JSR:abs    $DF4C
BDE7 20:98 C0   JSR:abs    $C098
BDEA A2:61      LDX:imm    #61
BDEC 20:65 C7   JSR:abs    $C765
BDEF A5:2E      LDA:zp     Zp RAM 002E
BDF1 85:56      STA:zp     Zp RAM 0056
BDF3 A5:2F      LDA:zp     Zp RAM 002F
BDF5 85:57      STA:zp     Zp RAM 0057
BDF7 A5:30      LDA:zp     Zp RAM 0030
BDF9 85:58      STA:zp     Zp RAM 0058
BDFB 20:98 C0   JSR:abs    $C098
BDFE A4:59      LDY:zp     Zp RAM 0059
BE00 A5:5A      LDA:zp     Zp RAM 005A
BE02 20:6C DF   JSR:abs    $DF6C
BE05 A5:61      LDA:zp     Zp RAM 0061
BE07 38:        SEC:imp    Set Carry
BE08 E5:6A      SBC:zp     Zp RAM 006A
BE0A 85:79      STA:zp     Zp RAM 0079
BE0C A5:62      LDA:zp     Zp RAM 0062
BE0E E5:6B      SBC:zp     Zp RAM 006B
BE10 85:9B      STA:zp     Zp RAM 009B
BE12 30:09      BMI:rel    Branch->$BE1D
BE14 F0:04      BEQ:rel    Branch->$BE1A
BE16 A9:FF      LDA:imm    #FF
BE18 85:79      STA:zp     Zp RAM 0079
BE1A B8:        CLV:imp    Clear Overflow
BE1B 50:16      BVC:rel    Branch->$BE33
BE1D C9:FF      CMP:imm    #FF
BE1F F0:05      BEQ:rel    Branch->$BE26
BE21 A9:FF      LDA:imm    #FF
BE23 B8:        CLV:imp    Clear Overflow
BE24 50:0B      BVC:rel    Branch->$BE31
BE26 A5:79      LDA:zp     Zp RAM 0079
BE28 49:FF      EOR:imm    #FF
BE2A 18:        CLC:imp    Clear Carry
BE2B 69:01      ADC:imm    #01
BE2D 90:02      BCC:rel    Branch->$BE31
BE2F A9:FF      LDA:imm    #FF
BE31 85:79      STA:zp     Zp RAM 0079
BE33 A5:63      LDA:zp     Zp RAM 0063
BE35 38:        SEC:imp    Set Carry
BE36 E5:6C      SBC:zp     Zp RAM 006C
BE38 85:89      STA:zp     Zp RAM 0089
BE3A A5:64      LDA:zp     Zp RAM 0064
BE3C E5:6D      SBC:zp     Zp RAM 006D
BE3E 85:9D      STA:zp     Zp RAM 009D
BE40 30:09      BMI:rel    Branch->$BE4B
BE42 F0:04      BEQ:rel    Branch->$BE48
BE44 A9:FF      LDA:imm    #FF
BE46 85:89      STA:zp     Zp RAM 0089
BE48 B8:        CLV:imp    Clear Overflow
BE49 50:12      BVC:rel    Branch->$BE5D
BE4B C9:FF      CMP:imm    #FF
BE4D F0:05      BEQ:rel    Branch->$BE54
BE4F A9:FF      LDA:imm    #FF
BE51 B8:        CLV:imp    Clear Overflow
BE52 50:07      BVC:rel    Branch->$BE5B
BE54 A5:89      LDA:zp     Zp RAM 0089
BE56 49:FF      EOR:imm    #FF
BE58 18:        CLC:imp    Clear Carry
BE59 69:01      ADC:imm    #01
BE5B 85:89      STA:zp     Zp RAM 0089
BE5D A9:00      LDA:imm    #00
BE5F 85:82      STA:zp     Zp RAM 0082
BE61 85:92      STA:zp     Zp RAM 0092
BE63 A5:79      LDA:zp     Zp RAM 0079
BE65 0A:        ASL:accum  
BE66 26:82      ROL:zp     Zp RAM 0082
BE68 85:7A      STA:zp     Zp RAM 007A
BE6A 0A:        ASL:accum  
BE6B 85:7C      STA:zp     Zp RAM 007C
BE6D A5:82      LDA:zp     Zp RAM 0082
BE6F 2A:        ROL:accum  
BE70 85:84      STA:zp     Zp RAM 0084
BE72 A5:7C      LDA:zp     Zp RAM 007C
BE74 65:79      ADC:zp     Zp RAM 0079
BE76 85:7D      STA:zp     Zp RAM 007D
BE78 A5:84      LDA:zp     Zp RAM 0084
BE7A 69:00      ADC:imm    #00
BE7C 85:85      STA:zp     Zp RAM 0085
BE7E A5:7A      LDA:zp     Zp RAM 007A
BE80 65:79      ADC:zp     Zp RAM 0079
BE82 85:7B      STA:zp     Zp RAM 007B
BE84 A5:82      LDA:zp     Zp RAM 0082
BE86 69:00      ADC:imm    #00
BE88 85:83      STA:zp     Zp RAM 0083
BE8A 85:86      STA:zp     Zp RAM 0086
BE8C A5:7B      LDA:zp     Zp RAM 007B
BE8E 0A:        ASL:accum  
BE8F 85:7E      STA:zp     Zp RAM 007E
BE91 26:86      ROL:zp     Zp RAM 0086
BE93 65:79      ADC:zp     Zp RAM 0079
BE95 85:7F      STA:zp     Zp RAM 007F
BE97 A5:86      LDA:zp     Zp RAM 0086
BE99 69:00      ADC:imm    #00
BE9B 85:87      STA:zp     Zp RAM 0087
BE9D A5:89      LDA:zp     Zp RAM 0089
BE9F 0A:        ASL:accum  
BEA0 26:92      ROL:zp     Zp RAM 0092
BEA2 85:8A      STA:zp     Zp RAM 008A
BEA4 0A:        ASL:accum  
BEA5 85:8C      STA:zp     Zp RAM 008C
BEA7 A5:92      LDA:zp     Zp RAM 0092
BEA9 2A:        ROL:accum  
BEAA 85:94      STA:zp     Zp RAM 0094
BEAC A5:8C      LDA:zp     Zp RAM 008C
BEAE 65:89      ADC:zp     Zp RAM 0089
BEB0 85:8D      STA:zp     Zp RAM 008D
BEB2 A5:94      LDA:zp     Zp RAM 0094
BEB4 69:00      ADC:imm    #00
BEB6 85:95      STA:zp     Zp RAM 0095
BEB8 A5:8A      LDA:zp     Zp RAM 008A
BEBA 65:89      ADC:zp     Zp RAM 0089
BEBC 85:8B      STA:zp     Zp RAM 008B
BEBE A5:92      LDA:zp     Zp RAM 0092
BEC0 69:00      ADC:imm    #00
BEC2 85:93      STA:zp     Zp RAM 0093
BEC4 85:96      STA:zp     Zp RAM 0096
BEC6 A5:8B      LDA:zp     Zp RAM 008B
BEC8 0A:        ASL:accum  
BEC9 85:8E      STA:zp     Zp RAM 008E
BECB 26:96      ROL:zp     Zp RAM 0096
BECD 65:89      ADC:zp     Zp RAM 0089
BECF 85:8F      STA:zp     Zp RAM 008F
BED1 A5:96      LDA:zp     Zp RAM 0096
BED3 69:00      ADC:imm    #00
BED5 85:97      STA:zp     Zp RAM 0097
BED7 A0:00      LDY:imm    #00
BED9 84:A9      STY:zp     Zp RAM 00A9
BEDB A4:38      LDY:zp     Zp RAM 0038
BEDD B9:D3 BF   LDA:abs,y  $BFD3,Y
BEE0 C9:01      CMP:imm    #01
BEE2 D0:02      BNE:rel    Branch->$BEE6
BEE4 A9:C0      LDA:imm    #C0
BEE6 85:73      STA:zp     Zp RAM 0073
BEE8 B9:D2 BF   LDA:abs,y  $BFD2,Y
BEEB 85:2D      STA:zp     Zp RAM 002D
BEED C8:        INY:imp    Y=Y+1
BEEE C8:        INY:imp    Y=Y+1
BEEF 84:38      STY:zp     Zp RAM 0038
BEF1 AA:        TAX:imp    A-->X
BEF2 29:07      AND:imm    #07
BEF4 A8:        TAY:imp    A-->Y
BEF5 8A:        TXA:imp    X-->A
BEF6 0A:        ASL:accum  
BEF7 85:2B      STA:zp     Zp RAM 002B
BEF9 4A:        LSR:accum  
BEFA 4A:        LSR:accum  
BEFB 4A:        LSR:accum  
BEFC 4A:        LSR:accum  
BEFD 29:07      AND:imm    #07
BEFF AA:        TAX:imp    A-->X
BF00 A5:2B      LDA:zp     Zp RAM 002B
BF02 45:9B      EOR:zp     Zp RAM 009B
BF04 30:0B      BMI:rel    Branch->$BF11
BF06 B9:78 00   LDA:abs,y  $0078,Y
BF09 85:61      STA:zp     Zp RAM 0061
BF0B B9:80 00   LDA:abs,y  $0080,Y
BF0E B8:        CLV:imp    Clear Overflow
BF0F 50:11      BVC:rel    Branch->$BF22
BF11 B9:78 00   LDA:abs,y  $0078,Y
BF14 49:FF      EOR:imm    #FF
BF16 18:        CLC:imp    Clear Carry
BF17 69:01      ADC:imm    #01
BF19 85:61      STA:zp     Zp RAM 0061
BF1B B9:80 00   LDA:abs,y  $0080,Y
BF1E 49:FF      EOR:imm    #FF
BF20 69:00      ADC:imm    #00
BF22 85:62      STA:zp     Zp RAM 0062
BF24 A5:2D      LDA:zp     Zp RAM 002D
BF26 45:9D      EOR:zp     Zp RAM 009D
BF28 10:0E      BPL:rel    Branch->$BF38
BF2A B5:88      LDA:zp,x   Zp RAM 0088
BF2C 18:        CLC:imp    Clear Carry
BF2D 65:61      ADC:zp     Zp RAM 0061
BF2F 85:61      STA:zp     Zp RAM 0061
BF31 B5:90      LDA:zp,x   Zp RAM 0090
BF33 65:62      ADC:zp     Zp RAM 0062
BF35 B8:        CLV:imp    Clear Overflow
BF36 50:0B      BVC:rel    Branch->$BF43
BF38 A5:61      LDA:zp     Zp RAM 0061
BF3A 38:        SEC:imp    Set Carry
BF3B F5:88      SBC:zp,x   Zp RAM 0088
BF3D 85:61      STA:zp     Zp RAM 0061
BF3F A5:62      LDA:zp     Zp RAM 0062
BF41 F5:90      SBC:zp,x   Zp RAM 0090
BF43 85:62      STA:zp     Zp RAM 0062
BF45 A5:2B      LDA:zp     Zp RAM 002B
BF47 45:9D      EOR:zp     Zp RAM 009D
BF49 30:0B      BMI:rel    Branch->$BF56
BF4B B9:88 00   LDA:abs,y  $0088,Y
BF4E 85:63      STA:zp     Zp RAM 0063
BF50 B9:90 00   LDA:abs,y  $0090,Y
BF53 B8:        CLV:imp    Clear Overflow
BF54 50:11      BVC:rel    Branch->$BF67
BF56 B9:88 00   LDA:abs,y  $0088,Y
BF59 49:FF      EOR:imm    #FF
BF5B 18:        CLC:imp    Clear Carry
BF5C 69:01      ADC:imm    #01
BF5E 85:63      STA:zp     Zp RAM 0063
BF60 B9:90 00   LDA:abs,y  $0090,Y
BF63 49:FF      EOR:imm    #FF
BF65 69:00      ADC:imm    #00
BF67 85:64      STA:zp     Zp RAM 0064
BF69 A5:2D      LDA:zp     Zp RAM 002D
BF6B 45:9B      EOR:zp     Zp RAM 009B
BF6D 10:0E      BPL:rel    Branch->$BF7D
BF6F A5:63      LDA:zp     Zp RAM 0063
BF71 38:        SEC:imp    Set Carry
BF72 F5:78      SBC:zp,x   Zp RAM 0078
BF74 85:63      STA:zp     Zp RAM 0063
BF76 A5:64      LDA:zp     Zp RAM 0064
BF78 F5:80      SBC:zp,x   Zp RAM 0080
BF7A B8:        CLV:imp    Clear Overflow
BF7B 50:0B      BVC:rel    Branch->$BF88
BF7D A5:63      LDA:zp     Zp RAM 0063
BF7F 18:        CLC:imp    Clear Carry
BF80 75:78      ADC:zp,x   Zp RAM 0078
BF82 85:63      STA:zp     Zp RAM 0063
BF84 A5:64      LDA:zp     Zp RAM 0064
BF86 75:80      ADC:zp,x   Zp RAM 0080
BF88 85:64      STA:zp     Zp RAM 0064
BF8A A4:A9      LDY:zp     Zp RAM 00A9
BF8C A5:63      LDA:zp     Zp RAM 0063
BF8E 91:74      STA:ind,y  74
BF90 C8:        INY:imp    Y=Y+1
BF91 A5:64      LDA:zp     Zp RAM 0064
BF93 29:1F      AND:imm    #1F
BF95 91:74      STA:ind,y  74
BF97 C8:        INY:imp    Y=Y+1
BF98 A5:61      LDA:zp     Zp RAM 0061
BF9A 91:74      STA:ind,y  74
BF9C C8:        INY:imp    Y=Y+1
BF9D A5:62      LDA:zp     Zp RAM 0062
BF9F 29:1F      AND:imm    #1F
BFA1 05:73      ORA:zp     Zp RAM 0073
BFA3 91:74      STA:ind,y  74
BFA5 C8:        INY:imp    Y=Y+1
BFA6 84:A9      STY:zp     Zp RAM 00A9
BFA8 C6:99      DEC:zp     Zp RAM 0099
BFAA F0:03      BEQ:rel    Branch->$BFAF
BFAC 4C:DB BE   JMP:abs    $BEDB
BFAF A4:A9      LDY:zp     Zp RAM 00A9
BFB1 88:        DEY:imp    Y=Y-1
BFB2 4C:5F DF   JMP:abs    $DF5F
BFB5 08:        PHP:imp    Push P
BFB6 08:        PHP:imp    Push P
BFB7 08:        PHP:imp    Push P
BFB8 08:        PHP:imp    Push P
BFB9 08:        PHP:imp    Push P
BFBA 08:        PHP:imp    Push P
BFBB 08:        PHP:imp    Push P
BFBC 08:        PHP:imp    Push P
BFBD 08:        PHP:imp    Push P
BFBE 09:06      ORA:imm    #06
BFC0 07:        Illegal Opcode
BFC1 07:        Illegal Opcode
BFC2 04:        Illegal Opcode
BFC3 02:        Illegal Opcode
BFC4 00:        BRK:imp    BREAK
BFC5 10:20      BPL:rel    Branch->$BFE7
BFC7 30:40      BMI:rel    Branch->$C009
BFC9 50:60      BVC:rel    Branch->$C02B
BFCB 70:80      BVS:rel    Branch->$BF4D
BFCD 92:        Illegal Opcode
BFCE 9E:        Illegal Opcode
BFCF AC:BA C2   LDY:abs    $C2BA
BFD2 0C:        Illegal Opcode
BFD3 01:8C      ORA:ind,x  8C
BFD5 01:4A      ORA:ind,x  4A
BFD7 01:09      ORA:ind,x  09
BFD9 01:CB      ORA:ind,x  CB
BFDB 01:4B      ORA:ind,x  4B
BFDD 01:89      ORA:ind,x  89
BFDF 01:CA      ORA:ind,x  CA
BFE1 01:90      ORA:ind,x  90
BFE3 01:8A      ORA:ind,x  8A
BFE5 01:23      ORA:ind,x  23
BFE7 01:DB      ORA:ind,x  DB
BFE9 01:41      ORA:ind,x  41
BFEB 01:10      ORA:ind,x  10
BFED 01:0A      ORA:ind,x  0A
BFEF 01:CB      ORA:ind,x  CB
BFF1 01:91      ORA:ind,x  91
BFF3 01:17      ORA:ind,x  17
BFF5 01:4B      ORA:ind,x  4B
BFF7 01:8A      ORA:ind,x  8A
BFF9 01:CE      ORA:ind,x  CE
BFFB 01:08      ORA:ind,x  08
BFFD 01:0A      ORA:ind,x  0A
;
; Start of ROM 136002.119 at $C000.
;
BFFF 01:CB      ORA:ind,x  CB
C001 01:92      ORA:ind,x  92
C003 01:16      ORA:ind,x  16
C005 01:4B      ORA:ind,x  4B
C007 01:8A      ORA:ind,x  8A
C009 01:CD      ORA:ind,x  CD
C00B 01:49      ORA:ind,x  49
C00D 01:0A      ORA:ind,x  0A
C00F 01:CB      ORA:ind,x  CB
C011 01:93      ORA:ind,x  93
C013 01:15      ORA:ind,x  15
C015 01:4B      ORA:ind,x  4B
C017 01:8A      ORA:ind,x  8A
C019 01:CC      ORA:ind,x  CC
C01B 01:4A      ORA:ind,x  4A
C01D 01:0A      ORA:ind,x  0A
C01F 01:CB      ORA:ind,x  CB
C021 01:95      ORA:ind,x  95
C023 01:13      ORA:ind,x  13
C025 01:4B      ORA:ind,x  4B
C027 01:8A      ORA:ind,x  8A
C029 01:CA      ORA:ind,x  CA
C02B 01:4C      ORA:ind,x  4C
C02D 01:0A      ORA:ind,x  0A
C02F 01:CB      ORA:ind,x  CB
C031 01:96      ORA:ind,x  96
C033 01:12      ORA:ind,x  12
C035 01:4B      ORA:ind,x  4B
C037 01:8A      ORA:ind,x  8A
C039 01:C9      ORA:ind,x  C9
C03B 01:4D      ORA:ind,x  4D
C03D 01:0A      ORA:ind,x  0A
C03F 01:CB      ORA:ind,x  CB
C041 01:97      ORA:ind,x  97
C043 01:11      ORA:ind,x  11
C045 01:4B      ORA:ind,x  4B
C047 01:8A      ORA:ind,x  8A
C049 01:88      ORA:ind,x  88
C04B 01:4E      ORA:ind,x  4E
C04D 01:0A      ORA:ind,x  0A
C04F 01:CB      ORA:ind,x  CB
C051 01:0B      ORA:ind,x  0B
C053 00:        BRK:imp    BREAK
C054 A3:        Illegal Opcode
C055 01:0A      ORA:ind,x  0A
C057 01:10      ORA:ind,x  10
C059 01:4B      ORA:ind,x  4B
C05B 01:8A      ORA:ind,x  8A
C05D 01:90      ORA:ind,x  90
C05F 01:41      ORA:ind,x  41
C061 01:5B      ORA:ind,x  5B
C063 01:9A      ORA:ind,x  9A
C065 01:31      ORA:ind,x  31
C067 01:B1      ORA:ind,x  B1
C069 01:31      ORA:ind,x  31
C06B 01:B1      ORA:ind,x  B1
C06D 01:1A      ORA:ind,x  1A
C06F 01:01      ORA:ind,x  01
C071 00:        BRK:imp    BREAK
C072 91:01      STA:ind,y  01
C074 21:01      AND:ind,x  01
C076 A1:01      LDA:ind,x  01
C078 21:01      AND:ind,x  01
C07A A1:01      LDA:ind,x  01
C07C 11:01      ORA:ind,y  01
C07E 01:00      ORA:ind,x  00
C080 89:        Illegal Opcode
C081 01:11      ORA:ind,x  11
C083 01:91      ORA:ind,x  91
C085 01:11      ORA:ind,x  11
C087 01:91      ORA:ind,x  91
C089 01:09      ORA:ind,x  09
C08B 01:01      ORA:ind,x  01
C08D 00:        BRK:imp    BREAK
C08E 8A:        TXA:imp    X-->A
C08F 01:12      ORA:ind,x  12
C091 01:8A      ORA:ind,x  8A
C093 01:01      ORA:ind,x  01
C095 00:        BRK:imp    BREAK
C096 06:01      ASL:zp     Zp RAM 0001
C098 A5:57      LDA:zp     Zp RAM 0057
C09A 38:        SEC:imp    Set Carry
C09B E5:5F      SBC:zp     Zp RAM 005F
C09D 8D:95 60   STA:abs    $6095
C0A0 A9:00      LDA:imm    #00
C0A2 E5:5B      SBC:zp     Zp RAM 005B
C0A4 8D:96 60   STA:abs    $6096
C0A7 10:0A      BPL:rel    Branch->$C0B3
C0A9 A9:00      LDA:imm    #00
C0AB 8D:96 60   STA:abs    $6096
C0AE A9:01      LDA:imm    #01
C0B0 8D:95 60   STA:abs    $6095
C0B3 A5:58      LDA:zp     Zp RAM 0058
C0B5 C5:60      CMP:zp     Zp RAM 0060
C0B7 90:07      BCC:rel    Branch->$C0C0
C0B9 E5:60      SBC:zp     Zp RAM 0060
C0BB A2:00      LDX:imm    #00
C0BD B8:        CLV:imp    Clear Overflow
C0BE 50:07      BVC:rel    Branch->$C0C7
C0C0 A5:60      LDA:zp     Zp RAM 0060
C0C2 38:        SEC:imp    Set Carry
C0C3 E5:58      SBC:zp     Zp RAM 0058
C0C5 A2:FF      LDX:imm    #FF
C0C7 8D:8E 60   STA:abs    $608E
C0CA 8D:94 60   STA:abs    $6094
C0CD 86:33      STX:zp     Zp RAM 0033
C0CF A5:56      LDA:zp     Zp RAM 0056
C0D1 C5:5E      CMP:zp     Zp RAM 005E
C0D3 90:07      BCC:rel    Branch->$C0DC
C0D5 E5:5E      SBC:zp     Zp RAM 005E
C0D7 A2:00      LDX:imm    #00
C0D9 B8:        CLV:imp    Clear Overflow
C0DA 50:07      BVC:rel    Branch->$C0E3
C0DC A5:5E      LDA:zp     Zp RAM 005E
C0DE 38:        SEC:imp    Set Carry
C0DF E5:56      SBC:zp     Zp RAM 0056
C0E1 A2:FF      LDX:imm    #FF
C0E3 85:32      STA:zp     Zp RAM 0032
C0E5 86:34      STX:zp     Zp RAM 0034
C0E7 2C:40 60   BIT:abs    $6040	 ; Check mathbox status
C0EA 30:FB      BMI:rel    Branch->$C0E7
C0EC AD:60 60   LDA:abs    $6060
C0EF 85:63      STA:zp     Zp RAM 0063
C0F1 AD:70 60   LDA:abs    $6070
C0F4 85:64      STA:zp     Zp RAM 0064
C0F6 A5:32      LDA:zp     Zp RAM 0032
C0F8 8D:8E 60   STA:abs    $608E
C0FB 8D:94 60   STA:abs    $6094
C0FE A5:33      LDA:zp     Zp RAM 0033
C100 30:18      BMI:rel    Branch->$C11A
C102 A5:63      LDA:zp     Zp RAM 0063
C104 18:        CLC:imp    Clear Carry
C105 65:68      ADC:zp     Zp RAM 0068
C107 85:63      STA:zp     Zp RAM 0063
C109 A5:64      LDA:zp     Zp RAM 0064
C10B 65:69      ADC:zp     Zp RAM 0069
C10D 50:06      BVC:rel    Branch->$C115
C10F A9:FF      LDA:imm    #FF
C111 85:63      STA:zp     Zp RAM 0063
C113 A9:7F      LDA:imm    #7F
C115 85:64      STA:zp     Zp RAM 0064
C117 B8:        CLV:imp    Clear Overflow
C118 50:15      BVC:rel    Branch->$C12F
C11A A5:68      LDA:zp     Zp RAM 0068
C11C 38:        SEC:imp    Set Carry
C11D E5:63      SBC:zp     Zp RAM 0063
C11F 85:63      STA:zp     Zp RAM 0063
C121 A5:69      LDA:zp     Zp RAM 0069
C123 E5:64      SBC:zp     Zp RAM 0064
C125 50:06      BVC:rel    Branch->$C12D
C127 A9:00      LDA:imm    #00
C129 85:63      STA:zp     Zp RAM 0063
C12B A9:80      LDA:imm    #80
C12D 85:64      STA:zp     Zp RAM 0064
C12F 2C:40 60   BIT:abs    $6040	 ; Check mathbox status
C132 30:FB      BMI:rel    Branch->$C12F
C134 AD:60 60   LDA:abs    $6060
C137 85:61      STA:zp     Zp RAM 0061
C139 AD:70 60   LDA:abs    $6070
C13C 85:62      STA:zp     Zp RAM 0062
C13E A6:34      LDX:zp     Zp RAM 0034
C140 30:16      BMI:rel    Branch->$C158
C142 A5:61      LDA:zp     Zp RAM 0061
C144 18:        CLC:imp    Clear Carry
C145 65:66      ADC:zp     Zp RAM 0066
C147 85:61      STA:zp     Zp RAM 0061
C149 A5:62      LDA:zp     Zp RAM 0062
C14B 65:67      ADC:zp     Zp RAM 0067
C14D 50:06      BVC:rel    Branch->$C155
C14F A9:FF      LDA:imm    #FF
C151 85:61      STA:zp     Zp RAM 0061
C153 A9:7F      LDA:imm    #7F
C155 85:62      STA:zp     Zp RAM 0062
C157 60:        RTS:imp    Ret from Sub
C158 A5:66      LDA:zp     Zp RAM 0066
C15A 38:        SEC:imp    Set Carry
C15B E5:61      SBC:zp     Zp RAM 0061
C15D 85:61      STA:zp     Zp RAM 0061
C15F A5:67      LDA:zp     Zp RAM 0067
C161 E5:62      SBC:zp     Zp RAM 0062
C163 50:06      BVC:rel    Branch->$C16B
C165 A9:00      LDA:imm    #00
C167 85:61      STA:zp     Zp RAM 0061
C169 A9:80      LDA:imm    #80
C16B 85:62      STA:zp     Zp RAM 0062
C16D 60:        RTS:imp    Ret from Sub
;
C16E 20:13 AA   JSR:abs    $AA13
C171 A9:80      LDA:imm    #80
C173 85:5E      STA:zp     Zp RAM 005E
C175 A9:FF      LDA:imm    #FF
C177 8D:14 01   STA:abs    $0114
C17A 20:35 C2   JSR:abs    $C235
C17D AD:33 01   LDA:abs    $0133
C180 D0:03      BNE:rel    Branch->$C185
C182 8D:00 58   STA:abs    $5800	 ; Reset the vector state machine
C185 A9:00      LDA:imm    #00		 ; Clear information counter for
C187 8D:33 01   STA:abs    $0133	 ; frame update rate. (Usually
					 ; done when a new screen is
					 ; presented.)
C18A AD:C6 CE   LDA:abs    $CEC6
C18D 8D:00 20   STA:abs    $2000
C190 AD:C7 CE   LDA:abs    $CEC7
C193 8D:01 20   STA:abs    $2001
C196 A5:9F      LDA:zp     Zp RAM 009F
C198 29:70      AND:imm    #70
C19A C9:5F      CMP:imm    #5F
C19C 90:02      BCC:rel    Branch->$C1A0
C19E A9:5F      LDA:imm    #5F
C1A0 4A:        LSR:accum  
C1A1 09:07      ORA:imm    #07
C1A3 AA:        TAX:imp    A-->X
C1A4 A0:07      LDY:imm    #07
C1A6 BD:FD C1   LDA:abs,x  $C1FD,X       ; Some sort of playing around
C1A9 29:0F      AND:imm    #0F           ; with the color RAM from a table.
C1AB 99:19 00   STA:abs,y  $0019,Y
C1AE 99:00 08   STA:abs,y  $0800,Y
C1B1 BD:FD C1   LDA:abs,x  $C1FD,X
C1B4 4A:        LSR:accum  
C1B5 4A:        LSR:accum  
C1B6 4A:        LSR:accum  
C1B7 4A:        LSR:accum  
C1B8 99:21 00   STA:abs,y  $0021,Y
C1BB 99:08 08   STA:abs,y  $0808,Y
C1BE CA:        DEX:imp    X=X-1
C1BF 88:        DEY:imp    Y=Y-1
C1C0 10:E4      BPL:rel    Branch->$C1A6
C1C2 60:        RTS:imp    Ret from Sub

C1C3 A9:00      LDA:imm    #00
C1C5 85:81      STA:zp     Zp RAM 0081
C1C7 85:91      STA:zp     Zp RAM 0091
C1C9 85:80      STA:zp     Zp RAM 0080
C1CB 85:78      STA:zp     Zp RAM 0078
C1CD 85:90      STA:zp     Zp RAM 0090
C1CF 85:88      STA:zp     Zp RAM 0088
;
; This subroutine clears out the mathbox.
;
C1D1 A9:00      LDA:imm    #00           ;
C1D3 8D:80 60   STA:abs    $6080	 ;
C1D6 8D:81 60   STA:abs    $6081	 ;
C1D9 8D:84 60   STA:abs    $6084	 ;
C1DC 8D:85 60   STA:abs    $6085	 ;
C1DF 8D:86 60   STA:abs    $6086	 ;
C1E2 8D:87 60   STA:abs    $6087	 ;
C1E5 8D:89 60   STA:abs    $6089	 ;
C1E8 8D:83 60   STA:abs    $6083	 ;
C1EB 8D:8D 60   STA:abs    $608D	 ;
C1EE 8D:8E 60   STA:abs    $608E	 ;
C1F1 8D:8F 60   STA:abs    $608F	 ;
C1F4 8D:90 60   STA:abs    $6090	 ;
C1F7 A9:0F      LDA:imm    #0F	 	 ;
C1F9 8D:8C 60   STA:abs    $608C	 ;
C1FC 60:        RTS:imp    Ret from Sub	 ;
;
; Data table loaded in from $C1B1.
;
C1FD 00:        BRK:imp    BREAK
C1FE 04:        Illegal Opcode
C1FF 08:        PHP:imp    Push P
C200 0C:        Illegal Opcode
C201 C3:        Illegal Opcode
C202 07:        Illegal Opcode
C203 0B:        Illegal Opcode
C204 0B:        Illegal Opcode
C205 00:        BRK:imp    BREAK
C206 07:        Illegal Opcode
C207 0B:        Illegal Opcode
C208 08:        PHP:imp    Push P
C209 44:        Illegal Opcode
C20A 03:        Illegal Opcode
C20B 0C:        Illegal Opcode
C20C 0C:        Illegal Opcode
C20D 00:        BRK:imp    BREAK
C20E 0B:        Illegal Opcode
C20F 03:        Illegal Opcode
C210 07:        Illegal Opcode
C211 C8:        INY:imp    Y=Y+1
C212 0C:        Illegal Opcode
C213 04:        Illegal Opcode
C214 04:        Illegal Opcode
C215 00:        BRK:imp    BREAK
C216 0B:        Illegal Opcode
C217 08:        PHP:imp    Push P
C218 07:        Illegal Opcode
C219 C4:0C      CPY:zp     Zp RAM 000C
C21B 03:        Illegal Opcode
C21C 03:        Illegal Opcode
C21D 00:        BRK:imp    BREAK
C21E 04:        Illegal Opcode
C21F 08:        PHP:imp    Push P
C220 0C:        Illegal Opcode
C221 C3:        Illegal Opcode
C222 07:        Illegal Opcode
C223 0F:        Illegal Opcode
C224 0B:        Illegal Opcode
C225 00:        BRK:imp    BREAK
C226 0C:        Illegal Opcode
C227 08:        PHP:imp    Push P
C228 04:        Illegal Opcode
C229 C3:        Illegal Opcode
C22A 0B:        Illegal Opcode
C22B 07:        Illegal Opcode
C22C 07:        Illegal Opcode
C22D 06:03      ASL:zp     Zp RAM 0003
C22F 01:04      ORA:ind,x  04
C231 00:        BRK:imp    BREAK
C232 05:05      ORA:zp     Zp RAM 0005
C234 05:A6      ORA:zp     Zp RAM 00A6
C236 3D:B5 46   AND:abs,x  $46B5,X
C239 20:E8 C2   JSR:abs    $C2E8
C23C 48:        PHA:imp    Push Accum
C23D AC:12 01   LDY:abs    $0112
C240 B9:8C BC   LDA:abs,y  $BC8C,Y
C243 49:FF      EOR:imm    #FF
C245 18:        CLC:imp    Clear Carry
C246 69:01      ADC:imm    #01
C248 85:5F      STA:zp     Zp RAM 005F
C24A 85:5D      STA:zp     Zp RAM 005D
C24C A9:10      LDA:imm    #10
C24E 38:        SEC:imp    Set Carry
C24F E5:5F      SBC:zp     Zp RAM 005F
C251 85:A0      STA:zp     Zp RAM 00A0
C253 A9:FF      LDA:imm    #FF
C255 85:5B      STA:zp     Zp RAM 005B
C257 B9:9C BC   LDA:abs,y  $BC9C,Y
C25A 85:60      STA:zp     Zp RAM 0060
C25C B9:CC BC   LDA:abs,y  $BCCC,Y
C25F 8D:11 01   STA:abs    $0111
C262 A5:02      LDA:zp     Zp RAM 0002
C264 C9:1E      CMP:imm    #1E
C266 D0:0D      BNE:rel    Branch->$C275
C268 B9:AC BC   LDA:abs,y  $BCAC,Y
C26B 85:68      STA:zp     Zp RAM 0068
C26D B9:BC BC   LDA:abs,y  $BCBC,Y
C270 85:69      STA:zp     Zp RAM 0069
C272 B8:        CLV:imp    Clear Overflow
C273 50:18      BVC:rel    Branch->$C28D
C275 B9:AC BC   LDA:abs,y  $BCAC,Y
C278 38:        SEC:imp    Set Carry
C279 E5:68      SBC:zp     Zp RAM 0068
C27B 8D:21 01   STA:abs    $0121
C27E B9:BC BC   LDA:abs,y  $BCBC,Y
C281 ED:69 00   SBC:abs    $0069
C284 A2:03      LDX:imm    #03
C286 4A:        LSR:accum  
C287 6E:21 01   ROR:abs    $0121
C28A CA:        DEX:imp    X=X-1
C28B 10:F9      BPL:rel    Branch->$C286
C28D A9:00      LDA:imm    #00
C28F 85:66      STA:zp     Zp RAM 0066
C291 85:67      STA:zp     Zp RAM 0067
C293 A9:00      LDA:imm    #00
C295 8D:0F 01   STA:abs    $010F
C298 8D:10 01   STA:abs    $0110
C29B A9:2C      LDA:imm    #2C
C29D 8D:13 01   STA:abs    $0113
C2A0 68:        PLA:imp    Pull Accum
C2A1 A8:        TAY:imp    A-->Y
C2A2 A2:0F      LDX:imm    #0F
C2A4 B9:7C B9   LDA:abs,y  $B97C,Y
C2A7 9D:CE 03   STA:abs,x  $03CE,X
C2AA B9:7C BA   LDA:abs,y  $BA7C,Y
C2AD 9D:DE 03   STA:abs,x  $03DE,X
C2B0 A9:00      LDA:imm    #00
C2B2 9D:1A 03   STA:abs,x  $031A,X
C2B5 9D:3A 03   STA:abs,x  $033A,X
C2B8 9D:9A 03   STA:abs,x  $039A,X
C2BB B9:7C BB   LDA:abs,y  $BB7C,Y
C2BE 9D:EE 03   STA:abs,x  $03EE,X
C2C1 88:        DEY:imp    Y=Y-1
C2C2 CA:        DEX:imp    X=X-1
C2C3 10:DF      BPL:rel    Branch->$C2A4
C2C5 A0:00      LDY:imm    #00
C2C7 A2:0F      LDX:imm    #0F
C2C9 B9:CE 03   LDA:abs,y  $03CE,Y
C2CC 38:        SEC:imp    Set Carry
C2CD 7D:CE 03   ADC:abs,x  $03CE,X
C2D0 6A:        ROT:accum  
C2D1 9D:35 04   STA:abs,x  $0435,X
C2D4 B9:DE 03   LDA:abs,y  $03DE,Y
C2D7 38:        SEC:imp    Set Carry
C2D8 7D:DE 03   ADC:abs,x  $03DE,X
C2DB 6A:        ROT:accum  
C2DC 9D:45 04   STA:abs,x  $0445,X
C2DF 88:        DEY:imp    Y=Y-1
C2E0 10:02      BPL:rel    Branch->$C2E4
C2E2 A0:0F      LDY:imm    #0F
C2E4 CA:        DEX:imp    X=X-1
C2E5 10:E2      BPL:rel    Branch->$C2C9
C2E7 60:        RTS:imp    Ret from Sub
C2E8 A2:00      LDX:imm    #00
C2EA C9:62      CMP:imm    #62
C2EC 90:05      BCC:rel    Branch->$C2F3
C2EE AD:CA 60   LDA:abs    $60CA	 ; Get a random number,
C2F1 29:5F      AND:imm    #5F		 ; 1-5F.
C2F3 C9:10      CMP:imm    #10
C2F5 90:04      BCC:rel    Branch->$C2FB
C2F7 E8:        INX:imp    X=X+1
C2F8 38:        SEC:imp    Set Carry
C2F9 E9:10      SBC:imm    #10
C2FB C9:10      CMP:imm    #10
C2FD B0:F6      BCS:rel    Branch->$C2F5
C2FF A8:        TAY:imp    A-->Y
C300 B9:7C BC   LDA:abs,y  $BC7C,Y
C303 8D:12 01   STA:abs    $0112
C306 0A:        ASL:accum  
C307 0A:        ASL:accum  
C308 0A:        ASL:accum  
C309 0A:        ASL:accum  
C30A 09:0F      ORA:imm    #0F
C30C 60:        RTS:imp    Ret from Sub
C30D AD:10 01   LDA:abs    $0110
C310 D0:27      BNE:rel    Branch->$C339
C312 A9:F0      LDA:imm    #F0
C314 85:57      STA:zp     Zp RAM 0057
C316 A2:4F      LDX:imm    #4F
C318 20:73 C4   JSR:abs    $C473
C31B 8D:10 01   STA:abs    $0110
C31E F0:03      BEQ:rel    Branch->$C323
C320 8D:0F 01   STA:abs    $010F
C323 AD:0F 01   LDA:abs    $010F
C326 D0:11      BNE:rel    Branch->$C339
C328 A9:10      LDA:imm    #10
C32A 85:57      STA:zp     Zp RAM 0057
C32C 20:53 C4   JSR:abs    $C453
C32F A5:57      LDA:zp     Zp RAM 0057
C331 A2:0F      LDX:imm    #0F
C333 20:73 C4   JSR:abs    $C473
C336 8D:0F 01   STA:abs    $010F
C339 A9:01      LDA:imm    #01
C33B 20:6A DF   JSR:abs    $DF6A
C33E A0:06      LDY:imm    #06
C340 84:9E      STY:zp     Zp RAM 009E
C342 AE:10 01   LDX:abs    $0110
C345 F0:01      BEQ:rel    Branch->$C348
C347 60:        RTS:imp    Ret from Sub
C348 AE:13 01   LDX:abs    $0113
C34B D0:01      BNE:rel    Branch->$C34E
C34D 60:        RTS:imp    Ret from Sub
C34E A2:0F      LDX:imm    #0F
C350 A9:C0      LDA:imm    #C0
C352 20:EE C3   JSR:abs    $C3EE
C355 CA:        DEX:imp    X=X-1
C356 10:F8      BPL:rel    Branch->$C350
C358 A0:06      LDY:imm    #06
C35A 84:9E      STY:zp     Zp RAM 009E
C35C A9:08      LDA:imm    #08
C35E 20:4C DF   JSR:abs    $DF4C
C361 A0:4F      LDY:imm    #4F
C363 AD:10 01   LDA:abs    $0110
C366 20:6E C3   JSR:abs    $C36E
C369 A0:0F      LDY:imm    #0F
C36B AD:0F 01   LDA:abs    $010F
C36E D0:49      BNE:rel    Branch->$C3B9
C370 84:37      STY:zp     Zp RAM 0037
C372 B9:2A 03   LDA:abs,y  $032A,Y
C375 85:61      STA:zp     Zp RAM 0061
C377 B9:1A 03   LDA:abs,y  $031A,Y
C37A 85:62      STA:zp     Zp RAM 0062
C37C B9:4A 03   LDA:abs,y  $034A,Y
C37F 85:63      STA:zp     Zp RAM 0063
C381 B9:3A 03   LDA:abs,y  $033A,Y
C384 85:64      STA:zp     Zp RAM 0064
C386 A2:61      LDX:imm    #61
C388 20:72 C7   JSR:abs    $C772
C38B A5:74      LDA:zp     Zp RAM 0074
C38D 85:B0      STA:zp     Zp RAM 00B0
C38F A5:75      LDA:zp     Zp RAM 0075
C391 85:B1      STA:zp     Zp RAM 00B1
C393 A2:0F      LDX:imm    #0F
C395 AD:11 01   LDA:abs    $0111
C398 F0:01      BEQ:rel    Branch->$C39B
C39A CA:        DEX:imp    X=X-1
C39B A9:C0      LDA:imm    #C0
C39D 85:73      STA:zp     Zp RAM 0073
C39F 86:38      STX:zp     Zp RAM 0038
C3A1 C6:37      DEC:zp     Zp RAM 0037
C3A3 A5:37      LDA:zp     Zp RAM 0037
C3A5 29:0F      AND:imm    #0F
C3A7 C9:0F      CMP:imm    #0F
C3A9 D0:07      BNE:rel    Branch->$C3B2
C3AB A5:37      LDA:zp     Zp RAM 0037
C3AD 18:        CLC:imp    Clear Carry
C3AE 69:10      ADC:imm    #10
C3B0 85:37      STA:zp     Zp RAM 0037
C3B2 20:23 C4   JSR:abs    $C423
C3B5 C6:38      DEC:zp     Zp RAM 0038
C3B7 10:E8      BPL:rel    Branch->$C3A1
C3B9 60:        RTS:imp    Ret from Sub
C3BA A5:61      LDA:zp     Zp RAM 0061
C3BC 38:        SEC:imp    Set Carry
C3BD E5:6A      SBC:zp     Zp RAM 006A
C3BF 85:6E      STA:zp     Zp RAM 006E
C3C1 A5:62      LDA:zp     Zp RAM 0062
C3C3 E5:6B      SBC:zp     Zp RAM 006B
C3C5 85:6F      STA:zp     Zp RAM 006F
C3C7 A5:63      LDA:zp     Zp RAM 0063
C3C9 38:        SEC:imp    Set Carry
C3CA E5:6C      SBC:zp     Zp RAM 006C
C3CC 85:70      STA:zp     Zp RAM 0070
C3CE A5:64      LDA:zp     Zp RAM 0064
C3D0 E5:6D      SBC:zp     Zp RAM 006D
C3D2 85:71      STA:zp     Zp RAM 0071
C3D4 A2:6E      LDX:imm    #6E
C3D6 20:92 DF   JSR:abs    $DF92
C3D9 A5:61      LDA:zp     Zp RAM 0061
C3DB 85:6A      STA:zp     Zp RAM 006A
C3DD A5:62      LDA:zp     Zp RAM 0062
C3DF 85:6B      STA:zp     Zp RAM 006B
C3E1 A5:63      LDA:zp     Zp RAM 0063
C3E3 85:6C      STA:zp     Zp RAM 006C
C3E5 A5:64      LDA:zp     Zp RAM 0064
C3E7 85:6D      STA:zp     Zp RAM 006D
C3E9 A9:C0      LDA:imm    #C0
C3EB 85:73      STA:zp     Zp RAM 0073
C3ED 60:        RTS:imp    Ret from Sub
C3EE 86:37      STX:zp     Zp RAM 0037
C3F0 48:        PHA:imp    Push Accum
C3F1 A4:9E      LDY:zp     Zp RAM 009E
C3F3 A9:08      LDA:imm    #08
C3F5 20:4C DF   JSR:abs    $DF4C
C3F8 20:3C C4   JSR:abs    $C43C
C3FB A2:61      LDX:imm    #61
C3FD 20:72 C7   JSR:abs    $C772
C400 68:        PLA:imp    Pull Accum
C401 85:73      STA:zp     Zp RAM 0073
C403 48:        PHA:imp    Push Accum
C404 20:23 C4   JSR:abs    $C423
C407 C6:37      DEC:zp     Zp RAM 0037
C409 A4:9E      LDY:zp     Zp RAM 009E
C40B A9:00      LDA:imm    #00
C40D 85:73      STA:zp     Zp RAM 0073
C40F A9:08      LDA:imm    #08
C411 20:4C DF   JSR:abs    $DF4C
C414 20:23 C4   JSR:abs    $C423
C417 68:        PLA:imp    Pull Accum
C418 85:73      STA:zp     Zp RAM 0073
C41A 20:3C C4   JSR:abs    $C43C
C41D 20:BA C3   JSR:abs    $C3BA
C420 A6:37      LDX:zp     Zp RAM 0037
C422 60:        RTS:imp    Ret from Sub
C423 A6:37      LDX:zp     Zp RAM 0037
C425 BD:2A 03   LDA:abs,x  $032A,X
C428 85:61      STA:zp     Zp RAM 0061
C42A BD:1A 03   LDA:abs,x  $031A,X
C42D 85:62      STA:zp     Zp RAM 0062
C42F BD:4A 03   LDA:abs,x  $034A,X
C432 85:63      STA:zp     Zp RAM 0063
C434 BD:3A 03   LDA:abs,x  $033A,X
C437 85:64      STA:zp     Zp RAM 0064
C439 4C:BA C3   JMP:abs    $C3BA
C43C A6:37      LDX:zp     Zp RAM 0037
C43E BD:6A 03   LDA:abs,x  $036A,X
C441 85:61      STA:zp     Zp RAM 0061
C443 BD:5A 03   LDA:abs,x  $035A,X
C446 85:62      STA:zp     Zp RAM 0062
C448 BD:8A 03   LDA:abs,x  $038A,X
C44B 85:63      STA:zp     Zp RAM 0063
C44D BD:7A 03   LDA:abs,x  $037A,X
C450 85:64      STA:zp     Zp RAM 0064
C452 60:        RTS:imp    Ret from Sub
C453 A5:5B      LDA:zp     Zp RAM 005B
C455 D0:1A      BNE:rel    Branch->$C471
C457 A5:57      LDA:zp     Zp RAM 0057
C459 38:        SEC:imp    Set Carry
C45A E5:5F      SBC:zp     Zp RAM 005F
C45C 90:02      BCC:rel    Branch->$C460
C45E C9:0C      CMP:imm    #0C
C460 B0:0F      BCS:rel    Branch->$C471
C462 A5:5F      LDA:zp     Zp RAM 005F
C464 18:        CLC:imp    Clear Carry
C465 69:0F      ADC:imm    #0F
C467 B0:02      BCS:rel    Branch->$C46B
C469 C9:F0      CMP:imm    #F0
C46B 90:02      BCC:rel    Branch->$C46F
C46D A9:F0      LDA:imm    #F0
C46F 85:57      STA:zp     Zp RAM 0057
C471 60:        RTS:imp    Ret from Sub
C472 DB:        Illegal Opcode
C473 85:57      STA:zp     Zp RAM 0057
C475 86:38      STX:zp     Zp RAM 0038
C477 A9:00      LDA:imm    #00
C479 85:59      STA:zp     Zp RAM 0059
C47B A2:0F      LDX:imm    #0F
C47D 86:37      STX:zp     Zp RAM 0037
C47F A6:37      LDX:zp     Zp RAM 0037
C481 BD:CE 03   LDA:abs,x  $03CE,X
C484 85:56      STA:zp     Zp RAM 0056
C486 BD:DE 03   LDA:abs,x  $03DE,X
C489 85:58      STA:zp     Zp RAM 0058
C48B 20:98 C0   JSR:abs    $C098
C48E A6:38      LDX:zp     Zp RAM 0038
C490 A4:61      LDY:zp     Zp RAM 0061
C492 A5:62      LDA:zp     Zp RAM 0062
C494 30:0D      BMI:rel    Branch->$C4A3
C496 C9:04      CMP:imm    #04
C498 90:06      BCC:rel    Branch->$C4A0
C49A A0:FF      LDY:imm    #FF
C49C A9:03      LDA:imm    #03
C49E E6:59      INC:zp     Zp RAM 0059
C4A0 B8:        CLV:imp    Clear Overflow
C4A1 50:0A      BVC:rel    Branch->$C4AD
C4A3 C9:FC      CMP:imm    #FC
C4A5 B0:06      BCS:rel    Branch->$C4AD
C4A7 A0:01      LDY:imm    #01
C4A9 A9:FC      LDA:imm    #FC
C4AB E6:59      INC:zp     Zp RAM 0059
C4AD 9D:1A 03   STA:abs,x  $031A,X
C4B0 98:        TYA:imp    Y-->A
C4B1 9D:2A 03   STA:abs,x  $032A,X
C4B4 A4:63      LDY:zp     Zp RAM 0063
C4B6 A5:64      LDA:zp     Zp RAM 0064
C4B8 30:0D      BMI:rel    Branch->$C4C7
C4BA C9:04      CMP:imm    #04
C4BC 90:06      BCC:rel    Branch->$C4C4
C4BE A0:FF      LDY:imm    #FF
C4C0 A9:03      LDA:imm    #03
C4C2 E6:59      INC:zp     Zp RAM 0059
C4C4 B8:        CLV:imp    Clear Overflow
C4C5 50:0A      BVC:rel    Branch->$C4D1
C4C7 C9:FC      CMP:imm    #FC
C4C9 B0:06      BCS:rel    Branch->$C4D1
C4CB A9:FC      LDA:imm    #FC
C4CD A0:01      LDY:imm    #01
C4CF E6:59      INC:zp     Zp RAM 0059
C4D1 9D:3A 03   STA:abs,x  $033A,X
C4D4 98:        TYA:imp    Y-->A
C4D5 9D:4A 03   STA:abs,x  $034A,X
C4D8 C6:38      DEC:zp     Zp RAM 0038
C4DA C6:37      DEC:zp     Zp RAM 0037
C4DC 10:A1      BPL:rel    Branch->$C47F
C4DE A5:59      LDA:zp     Zp RAM 0059
C4E0 60:        RTS:imp    Ret from Sub
C4E1 20:E8 C2   JSR:abs    $C2E8
C4E4 85:36      STA:zp     Zp RAM 0036
C4E6 86:35      STX:zp     Zp RAM 0035
C4E8 A9:00      LDA:imm    #00
C4EA 85:73      STA:zp     Zp RAM 0073
C4EC A9:05      LDA:imm    #05
C4EE 20:6A DF   JSR:abs    $DF6A
C4F1 A5:35      LDA:zp     Zp RAM 0035
C4F3 29:07      AND:imm    #07
C4F5 AA:        TAX:imp    A-->X
C4F6 BC:2D C2   LDY:abs,x  $C22D,X
C4F9 84:9E      STY:zp     Zp RAM 009E
C4FB A9:08      LDA:imm    #08
C4FD 20:4C DF   JSR:abs    $DF4C
C500 AE:12 01   LDX:abs    $0112
C503 A5:36      LDA:zp     Zp RAM 0036
C505 BC:CC BC   LDY:abs,x  $BCCC,X
C508 D0:03      BNE:rel    Branch->$C50D
C50A 38:        SEC:imp    Set Carry
C50B E9:0F      SBC:imm    #0F
C50D A8:        TAY:imp    A-->Y
C50E B9:7C BA   LDA:abs,y  $BA7C,Y
C511 85:57      STA:zp     Zp RAM 0057
C513 49:80      EOR:imm    #80
C515 AA:        TAX:imp    A-->X
C516 B9:7C B9   LDA:abs,y  $B97C,Y
C519 85:56      STA:zp     Zp RAM 0056
C51B 49:80      EOR:imm    #80
C51D 20:75 DF   JSR:abs    $DF75
C520 A9:C0      LDA:imm    #C0
C522 85:73      STA:zp     Zp RAM 0073
C524 A2:0F      LDX:imm    #0F
C526 86:38      STX:zp     Zp RAM 0038
C528 A4:36      LDY:zp     Zp RAM 0036
C52A B9:7C B9   LDA:abs,y  $B97C,Y
C52D AA:        TAX:imp    A-->X
C52E 38:        SEC:imp    Set Carry
C52F E5:56      SBC:zp     Zp RAM 0056
C531 48:        PHA:imp    Push Accum
C532 86:56      STX:zp     Zp RAM 0056
C534 B9:7C BA   LDA:abs,y  $BA7C,Y
C537 A8:        TAY:imp    A-->Y
C538 38:        SEC:imp    Set Carry
C539 E5:57      SBC:zp     Zp RAM 0057
C53B AA:        TAX:imp    A-->X
C53C 84:57      STY:zp     Zp RAM 0057
C53E 68:        PLA:imp    Pull Accum
C53F 20:75 DF   JSR:abs    $DF75
C542 C6:36      DEC:zp     Zp RAM 0036
C544 C6:38      DEC:zp     Zp RAM 0038
C546 10:E0      BPL:rel    Branch->$C528
C548 A9:01      LDA:imm    #01
C54A 4C:6A DF   JMP:abs    $DF6A
C54D AD:15 01   LDA:abs    $0115
C550 F0:5F      BEQ:rel    Branch->$C5B1
C552 A5:5F      LDA:zp     Zp RAM 005F
C554 48:        PHA:imp    Push Accum
C555 A5:5B      LDA:zp     Zp RAM 005B
C557 48:        PHA:imp    Push Accum
C558 A5:A0      LDA:zp     Zp RAM 00A0
C55A 48:        PHA:imp    Push Accum
C55B A9:E8      LDA:imm    #E8
C55D 85:5F      STA:zp     Zp RAM 005F
C55F A9:FF      LDA:imm    #FF
C561 85:5B      STA:zp     Zp RAM 005B
C563 A9:28      LDA:imm    #28
C565 85:A0      STA:zp     Zp RAM 00A0
C567 A2:07      LDX:imm    #07
C569 86:37      STX:zp     Zp RAM 0037
C56B A6:37      LDX:zp     Zp RAM 0037
C56D BD:FE 03   LDA:abs,x  $03FE,X
C570 F0:32      BEQ:rel    Branch->$C5A4
C572 85:57      STA:zp     Zp RAM 0057
C574 A9:80      LDA:imm    #80
C576 85:56      STA:zp     Zp RAM 0056
C578 A9:80      LDA:imm    #80
C57A 85:58      STA:zp     Zp RAM 0058
C57C A5:9F      LDA:zp     Zp RAM 009F
C57E C9:05      CMP:imm    #05
C580 B0:05      BCS:rel    Branch->$C587
C582 A9:06      LDA:imm    #06
C584 B8:        CLV:imp    Clear Overflow
C585 50:09      BVC:rel    Branch->$C590
C587 8A:        TXA:imp    X-->A
C588 29:07      AND:imm    #07
C58A C9:07      CMP:imm    #07
C58C D0:02      BNE:rel    Branch->$C590
C58E A9:04      LDA:imm    #04
C590 85:9E      STA:zp     Zp RAM 009E
C592 A8:        TAY:imp    A-->Y
C593 A9:08      LDA:imm    #08
C595 20:4C DF   JSR:abs    $DF4C
C598 A5:37      LDA:zp     Zp RAM 0037
C59A 29:03      AND:imm    #03
C59C 0A:        ASL:accum  
C59D 69:0A      ADC:imm    #0A
C59F 85:55      STA:zp     Zp RAM 0055
C5A1 20:09 BD   JSR:abs    $BD09
C5A4 C6:37      DEC:zp     Zp RAM 0037
C5A6 10:C3      BPL:rel    Branch->$C56B
C5A8 68:        PLA:imp    Pull Accum
C5A9 85:A0      STA:zp     Zp RAM 00A0
C5AB 68:        PLA:imp    Pull Accum
C5AC 85:5B      STA:zp     Zp RAM 005B
C5AE 68:        PLA:imp    Pull Accum
C5AF 85:5F      STA:zp     Zp RAM 005F
					 ; COPY PROTECTION CODE FOR POKEY
					 ; Causes lockups.

C5B1 AD:1F 01   LDA:abs    $011F	 ; Check pokey protection result.
C5B4 F0:0B      BEQ:rel    Branch->$C5C1 ; If zero (good), then return.
C5B6 A6:42      LDX:zp     Zp RAM 0042	 ; Compare player one's score
C5B8 E0:15      CPX:imm    #15	 	 ; to see if it is >= 16,000.
C5BA 90:05      BCC:rel    Branch->$C5C1 ; If not, then return.
C5BC A6:40      LDX:zp     Zp RAM 0040   ; Load in the lower two digits
					 ; current player's score.
C5BE FE:00 02   INC:abs,x  $0200,X	 ; Add that score to $200, and
					 ; increment the value at that
					 ; address in memory.
C5C1 60:        RTS:imp    Ret from Sub
C5C2 AD:10 01   LDA:abs    $0110
C5C5 F0:01      BEQ:rel    Branch->$C5C8
C5C7 60:        RTS:imp    Ret from Sub
C5C8 A5:5B      LDA:zp     Zp RAM 005B
C5CA D0:07      BNE:rel    Branch->$C5D3
C5CC A5:5F      LDA:zp     Zp RAM 005F
C5CE C9:F0      CMP:imm    #F0
C5D0 90:01      BCC:rel    Branch->$C5D3
C5D2 60:        RTS:imp    Ret from Sub
C5D3 A9:01      LDA:imm    #01
C5D5 20:6A DF   JSR:abs    $DF6A
C5D8 A5:74      LDA:zp     Zp RAM 0074
C5DA 48:        PHA:imp    Push Accum
C5DB A5:75      LDA:zp     Zp RAM 0075
C5DD 48:        PHA:imp    Push Accum
C5DE A9:00      LDA:imm    #00
C5E0 85:38      STA:zp     Zp RAM 0038
C5E2 85:A9      STA:zp     Zp RAM 00A9
C5E4 A2:0F      LDX:imm    #0F
C5E6 AD:11 01   LDA:abs    $0111
C5E9 F0:01      BEQ:rel    Branch->$C5EC
C5EB CA:        DEX:imp    X=X-1
C5EC 86:37      STX:zp     Zp RAM 0037
C5EE A2:03      LDX:imm    #03
C5F0 A4:A9      LDY:zp     Zp RAM 00A9
C5F2 BD:69 C6   LDA:abs,x  $C669,X
C5F5 91:74      STA:ind,y  74
C5F7 C8:        INY:imp    Y=Y+1
C5F8 CA:        DEX:imp    X=X-1
C5F9 10:F7      BPL:rel    Branch->$C5F2
C5FB 84:A9      STY:zp     Zp RAM 00A9
C5FD AD:14 01   LDA:abs    $0114
C600 D0:4A      BNE:rel    Branch->$C64C
C602 A6:38      LDX:zp     Zp RAM 0038
C604 BD:9A 03   LDA:abs,x  $039A,X
C607 30:11      BMI:rel    Branch->$C61A
C609 A2:0B      LDX:imm    #0B
C60B A4:A9      LDY:zp     Zp RAM 00A9
C60D B1:AA      LDA:ind,y  AA
C60F 91:74      STA:ind,y  74
C611 C8:        INY:imp    Y=Y+1
C612 CA:        DEX:imp    X=X-1
C613 10:F8      BPL:rel    Branch->$C60D
C615 84:A9      STY:zp     Zp RAM 00A9
C617 B8:        CLV:imp    Clear Overflow
C618 50:2F      BVC:rel    Branch->$C649
C61A A4:A9      LDY:zp     Zp RAM 00A9
C61C B1:AA      LDA:ind,y  AA
C61E 91:74      STA:ind,y  74
C620 85:6C      STA:zp     Zp RAM 006C
C622 C8:        INY:imp    Y=Y+1
C623 B1:AA      LDA:ind,y  AA
C625 91:74      STA:ind,y  74
C627 C9:10      CMP:imm    #10
C629 90:02      BCC:rel    Branch->$C62D
C62B 09:E0      ORA:imm    #E0
C62D 85:6D      STA:zp     Zp RAM 006D
C62F C8:        INY:imp    Y=Y+1
C630 B1:AA      LDA:ind,y  AA
C632 91:74      STA:ind,y  74
C634 85:6A      STA:zp     Zp RAM 006A
C636 C8:        INY:imp    Y=Y+1
C637 B1:AA      LDA:ind,y  AA
C639 91:74      STA:ind,y  74
C63B C9:10      CMP:imm    #10
C63D 90:02      BCC:rel    Branch->$C641
C63F 09:E0      ORA:imm    #E0
C641 85:6B      STA:zp     Zp RAM 006B
C643 C8:        INY:imp    Y=Y+1
C644 84:A9      STY:zp     Zp RAM 00A9
C646 20:C7 C6   JSR:abs    $C6C7
C649 B8:        CLV:imp    Clear Overflow
C64A 50:06      BVC:rel    Branch->$C652
C64C 20:6D C6   JSR:abs    $C66D
C64F 20:C7 C6   JSR:abs    $C6C7
C652 A6:38      LDX:zp     Zp RAM 0038
C654 1E:9A 03   ASL:abs,x  $039A,X
C657 E6:38      INC:zp     Zp RAM 0038
C659 C6:37      DEC:zp     Zp RAM 0037
C65B 10:91      BPL:rel    Branch->$C5EE
C65D 68:        PLA:imp    Pull Accum
C65E 85:AB      STA:zp     Zp RAM 00AB
C660 68:        PLA:imp    Pull Accum
C661 85:AA      STA:zp     Zp RAM 00AA
C663 A4:A9      LDY:zp     Zp RAM 00A9
C665 88:        DEY:imp    Y=Y-1
C666 4C:5F DF   JMP:abs    $DF5F
C669 80:        Illegal Opcode
C66A 40:        RTI:imp    Ret from Int
C66B 68:        PLA:imp    Pull Accum
C66C 05:A5      ORA:zp     Zp RAM 00A5
C66E 38:        SEC:imp    Set Carry
C66F AA:        TAX:imp    A-->X
C670 18:        CLC:imp    Clear Carry
C671 69:01      ADC:imm    #01
C673 29:0F      AND:imm    #0F
C675 A8:        TAY:imp    A-->Y
C676 BD:6A 03   LDA:abs,x  $036A,X
C679 38:        SEC:imp    Set Carry
C67A 79:6A 03   ADC:abs,y  $036A,Y
C67D 85:61      STA:zp     Zp RAM 0061
C67F BD:5A 03   LDA:abs,x  $035A,X
C682 79:5A 03   ADC:abs,y  $035A,Y
C685 85:62      STA:zp     Zp RAM 0062
C687 0A:        ASL:accum  
C688 66:62      ROR:zp     Zp RAM 0062
C68A 66:61      ROR:zp     Zp RAM 0061
C68C BD:8A 03   LDA:abs,x  $038A,X
C68F 38:        SEC:imp    Set Carry
C690 79:8A 03   ADC:abs,y  $038A,Y
C693 85:63      STA:zp     Zp RAM 0063
C695 BD:7A 03   LDA:abs,x  $037A,X
C698 79:7A 03   ADC:abs,y  $037A,Y
C69B 85:64      STA:zp     Zp RAM 0064
C69D 0A:        ASL:accum  
C69E 66:64      ROR:zp     Zp RAM 0064
C6A0 66:63      ROR:zp     Zp RAM 0063
C6A2 A4:A9      LDY:zp     Zp RAM 00A9
C6A4 A5:63      LDA:zp     Zp RAM 0063
C6A6 91:74      STA:ind,y  74
C6A8 C8:        INY:imp    Y=Y+1
C6A9 85:6C      STA:zp     Zp RAM 006C
C6AB A5:64      LDA:zp     Zp RAM 0064
C6AD 85:6D      STA:zp     Zp RAM 006D
C6AF 29:1F      AND:imm    #1F
C6B1 91:74      STA:ind,y  74
C6B3 C8:        INY:imp    Y=Y+1
C6B4 A5:61      LDA:zp     Zp RAM 0061
C6B6 91:74      STA:ind,y  74
C6B8 C8:        INY:imp    Y=Y+1
C6B9 85:6A      STA:zp     Zp RAM 006A
C6BB A5:62      LDA:zp     Zp RAM 0062
C6BD 85:6B      STA:zp     Zp RAM 006B
C6BF 29:1F      AND:imm    #1F
C6C1 91:74      STA:ind,y  74
C6C3 C8:        INY:imp    Y=Y+1
C6C4 84:A9      STY:zp     Zp RAM 00A9
C6C6 60:        RTS:imp    Ret from Sub
C6C7 A6:38      LDX:zp     Zp RAM 0038
C6C9 BD:AC 03   LDA:abs,x  $03AC,X
C6CC D0:16      BNE:rel    Branch->$C6E4
C6CE A4:A9      LDY:zp     Zp RAM 00A9
C6D0 A2:03      LDX:imm    #03
C6D2 A9:00      LDA:imm    #00
C6D4 91:74      STA:ind,y  74
C6D6 C8:        INY:imp    Y=Y+1
C6D7 A9:71      LDA:imm    #71
C6D9 91:74      STA:ind,y  74
C6DB C8:        INY:imp    Y=Y+1
C6DC CA:        DEX:imp    X=X-1
C6DD 10:F3      BPL:rel    Branch->$C6D2
C6DF 84:A9      STY:zp     Zp RAM 00A9
C6E1 B8:        CLV:imp    Clear Overflow
C6E2 50:57      BVC:rel    Branch->$C73B
C6E4 85:57      STA:zp     Zp RAM 0057
C6E6 20:53 C4   JSR:abs    $C453
C6E9 BD:35 04   LDA:abs,x  $0435,X
C6EC 85:56      STA:zp     Zp RAM 0056
C6EE BD:45 04   LDA:abs,x  $0445,X
C6F1 85:58      STA:zp     Zp RAM 0058
C6F3 20:98 C0   JSR:abs    $C098
C6F6 20:3C C7   JSR:abs    $C73C
C6F9 A6:38      LDX:zp     Zp RAM 0038
C6FB BD:9A 03   LDA:abs,x  $039A,X
C6FE 29:40      AND:imm    #40
C700 F0:1F      BEQ:rel    Branch->$C721
C702 20:3E BD   JSR:abs    $BD3E
C705 AD:CA 60   LDA:abs    $60CA
C708 29:02      AND:imm    #02
C70A 18:        CLC:imp    Clear Carry
C70B 69:1C      ADC:imm    #1C
C70D AA:        TAX:imp    A-->X
C70E BD:C9 CE   LDA:abs,x  $CEC9,X
C711 C8:        INY:imp    Y=Y+1
C712 91:74      STA:ind,y  74
C714 88:        DEY:imp    Y=Y-1
C715 BD:C8 CE   LDA:abs,x  $CEC8,X
C718 91:74      STA:ind,y  74
C71A C8:        INY:imp    Y=Y+1
C71B C8:        INY:imp    Y=Y+1
C71C 84:A9      STY:zp     Zp RAM 00A9
C71E B8:        CLV:imp    Clear Overflow
C71F 50:1A      BVC:rel    Branch->$C73B
C721 A4:A9      LDY:zp     Zp RAM 00A9
C723 A9:00      LDA:imm    #00
C725 91:74      STA:ind,y  74
C727 C8:        INY:imp    Y=Y+1
C728 A9:68      LDA:imm    #68
C72A 91:74      STA:ind,y  74
C72C C8:        INY:imp    Y=Y+1
C72D AD:B2 3D   LDA:abs    $3DB2
C730 91:74      STA:ind,y  74
C732 C8:        INY:imp    Y=Y+1
C733 AD:B3 3D   LDA:abs    $3DB3
C736 91:74      STA:ind,y  74
C738 C8:        INY:imp    Y=Y+1
C739 84:A9      STY:zp     Zp RAM 00A9
C73B 60:        RTS:imp    Ret from Sub
C73C A4:A9      LDY:zp     Zp RAM 00A9
C73E A5:63      LDA:zp     Zp RAM 0063
C740 38:        SEC:imp    Set Carry
C741 E5:6C      SBC:zp     Zp RAM 006C
C743 91:74      STA:ind,y  74
C745 C8:        INY:imp    Y=Y+1
C746 A5:64      LDA:zp     Zp RAM 0064
C748 E5:6D      SBC:zp     Zp RAM 006D
C74A 29:1F      AND:imm    #1F
C74C 91:74      STA:ind,y  74
C74E C8:        INY:imp    Y=Y+1
C74F A5:61      LDA:zp     Zp RAM 0061
C751 38:        SEC:imp    Set Carry
C752 E5:6A      SBC:zp     Zp RAM 006A
C754 91:74      STA:ind,y  74
C756 C8:        INY:imp    Y=Y+1
C757 A5:62      LDA:zp     Zp RAM 0062
C759 E5:6B      SBC:zp     Zp RAM 006B
C75B 29:1F      AND:imm    #1F
C75D 09:A0      ORA:imm    #A0
C75F 91:74      STA:ind,y  74
C761 C8:        INY:imp    Y=Y+1
C762 84:A9      STY:zp     Zp RAM 00A9
C764 60:        RTS:imp    Ret from Sub
C765 A0:00      LDY:imm    #00
C767 98:        TYA:imp    Y-->A
C768 91:74      STA:ind,y  74
C76A A9:71      LDA:imm    #71
C76C C8:        INY:imp    Y=Y+1
C76D 91:74      STA:ind,y  74
C76F C8:        INY:imp    Y=Y+1
C770 D0:02      BNE:rel    Branch->$C774
C772 A0:00      LDY:imm    #00
C774 A9:40      LDA:imm    #40
C776 91:74      STA:ind,y  74
C778 A9:80      LDA:imm    #80
C77A C8:        INY:imp    Y=Y+1
C77B 91:74      STA:ind,y  74
C77D C8:        INY:imp    Y=Y+1
C77E B5:02      LDA:zp,x   Zp RAM 0002
C780 85:6C      STA:zp     Zp RAM 006C
C782 91:74      STA:ind,y  74
C784 C8:        INY:imp    Y=Y+1
C785 B5:03      LDA:zp,x   Zp RAM 0003
C787 85:6D      STA:zp     Zp RAM 006D
C789 29:1F      AND:imm    #1F
C78B 91:74      STA:ind,y  74
C78D B5:00      LDA:zp,x   Zp RAM 0000
C78F 85:6A      STA:zp     Zp RAM 006A
C791 C8:        INY:imp    Y=Y+1
C792 91:74      STA:ind,y  74
C794 B5:01      LDA:zp,x   Zp RAM 0001
C796 85:6B      STA:zp     Zp RAM 006B
C798 29:1F      AND:imm    #1F
C79A C8:        INY:imp    Y=Y+1
C79B 91:74      STA:ind,y  74
C79D 4C:5F DF   JMP:abs    $DF5F
;
; Code jumps here after initialization
;
C7A0 20:95 CD   JSR:abs    $CD95	 ; Pokey Initialization Routine
C7A3 A9:00      LDA:imm    #00		 ; Zero out the game status
C7A5 85:00      STA:zp     Zp RAM 0000	 ; indicator.
C7A7 A5:53      LDA:zp     Zp RAM 0053	 ; Is the 0-9 timer at "9" or
C7A9 C9:09      CMP:imm    #09		 ; greater?
C7AB 90:FA      BCC:rel    Branch->$C7A7 ; If not, loop.
C7AD A9:00      LDA:imm    #00		 ; If so...
C7AF 85:53      STA:zp     Zp RAM 0053	 ; zero out the timer.
C7B1 20:BD C7   JSR:abs    $C7BD
C7B4 20:91 C8   JSR:abs    $C891
C7B7 20:B6 B1   JSR:abs    $B1B6
C7BA 18:        CLC:imp    Clear Carry
C7BB 90:EA      BCC:rel    Branch->$C7A7
C7BD AD:00 0D   LDA:abs    $0D00         ; Are we in demonstration freeze
C7C0 29:83      AND:imm    #83           ; mode?
C7C2 C9:82      CMP:imm    #82
C7C4 F0:13      BEQ:rel    Branch->$C7D9 ; If so, skip this code and return.
C7C6 20:D2 A7   JSR:abs    $A7D2
C7C9 A6:00      LDX:zp     Zp RAM 0000
C7CB A5:4E      LDA:zp     Zp RAM 004E
C7CD 09:80      ORA:imm    #80
C7CF 85:4E      STA:zp     Zp RAM 004E
C7D1 BD:DB C7   LDA:abs,x  $C7DB,X
C7D4 48:        PHA:imp    Push Accum
C7D5 BD:DA C7   LDA:abs,x  $C7DA,X
C7D8 48:        PHA:imp    Push Accum
C7D9 60:        RTS:imp    Ret from Sub
;
; Data segment, per Ken Lui.
;
C7DA 0B:C9      DATA
C7DC 3F:C9      DATA
C7DE 0A:97      DATA
C7E0 AE:C9      DATA
C7E2 F0:C9      DATA
C7E4 FF:C7      DATA
C7E6 00:00      DATA
C7E8 8B:C9      DATA
C7EA 3E:AC      DATA
C7EC 6D:AD      DATA
C7EE 17:CA      DATA
C7F0 48:91      DATA
C7F2 4A:90      DATA
C7F4 E6:B0      DATA
C7F6 07:91      DATA
C7F8 7A:C9      DATA
C7FA 28:97      DATA
C7FC E0:D7      DATA
C7FE 17:A6      DATA
;
; Start of ROM 136002.120 at $C800.
;
C800 A5         Illegal Opcode		 ; Re-disassemble.
C801 03:        Illegal Opcode		 ; Confused by data segment.
C802 2D:6B 01   AND:abs    $016B
C805 D0:11      BNE:rel    Branch->$C818
C807 A5:04      LDA:zp     Zp RAM 0004
C809 F0:02      BEQ:rel    Branch->$C80D
C80B C6:04      DEC:zp     Zp RAM 0004
C80D D0:09      BNE:rel    Branch->$C818
C80F A5:02      LDA:zp     Zp RAM 0002
C811 85:00      STA:zp     Zp RAM 0000
C813 A9:00      LDA:imm    #00
C815 8D:6B 01   STA:abs    $016B
C818 4C:49 97   JMP:abs    $9749
C81B A5:06      LDA:zp     Zp RAM 0006
C81D A0:00      LDY:imm    #00
C81F C9:02      CMP:imm    #02
C821 A5:4E      LDA:zp     Zp RAM 004E
C823 29:60      AND:imm    #60
C825 84:4E      STY:zp     Zp RAM 004E
C827 F0:48      BEQ:rel    Branch->$C871
C829 B0:05      BCS:rel    Branch->$C830
C82B 29:20      AND:imm    #20
C82D B8:        CLV:imp    Clear Overflow
C82E 50:05      BVC:rel    Branch->$C835
C830 C8:        INY:imp    Y=Y+1
C831 C6:06      DEC:zp     Zp RAM 0006	 ; Decrease the number of credits.
C833 29:40      AND:imm    #40
C835 F0:03      BEQ:rel    Branch->$C83A
C837 C6:06      DEC:zp     Zp RAM 0006	 ; Decrease the number of credits.
C839 C8:        INY:imp    Y=Y+1
C83A 98:        TYA:imp    Y-->A
C83B 85:3E      STA:zp     Zp RAM 003E
C83D F0:2F      BEQ:rel    Branch->$C86E
C83F A5:05      LDA:zp     Zp RAM 0005
C841 09:C0      ORA:imm    #C0
C843 85:05      STA:zp     Zp RAM 0005
C845 A9:00      LDA:imm    #00
C847 85:16      STA:zp     Zp RAM 0016
C849 85:18      STA:zp     Zp RAM 0018
C84B A9:00      LDA:imm    #00		 ; Reset the game-mode status
C84D 85:00      STA:zp     Zp RAM 0000	 ; indicator.
C84F C6:3E      DEC:zp     Zp RAM 003E
C851 A6:3E      LDX:zp     Zp RAM 003E
C853 F0:02      BEQ:rel    Branch->$C857
C855 A2:03      LDX:imm    #03
C857 FE:0C 04   INC:abs,x  $040C,X
C85A D0:03      BNE:rel    Branch->$C85F
C85C FE:0D 04   INC:abs,x  $040D,X
C85F AD:00 01   LDA:abs    $0100
C862 38:        SEC:imp    Set Carry
C863 65:3E      ADC:zp     Zp RAM 003E
C865 C9:63      CMP:imm    #63
C867 90:02      BCC:rel    Branch->$C86B
C869 A9:63      LDA:imm    #63
C86B 8D:00 01   STA:abs    $0100
C86E B8:        CLV:imp    Clear Overflow
C86F 50:1F      BVC:rel    Branch->$C890
C871 A5:50      LDA:zp     Zp RAM 0050   ; Check the relative spinner
C873 F0:1B      BEQ:rel    Branch->$C890 ; if no change, then skip a bit.
C875 24:05      BIT:zp     Zp RAM 0005
C877 30:17      BMI:rel    Branch->$C890
C879 A9:10      LDA:imm    #10           ; Set game mode to ????
C87B 85:01      STA:zp     Zp RAM 0001
C87D A9:20      LDA:imm    #20
C87F 85:04      STA:zp     Zp RAM 0004
C881 A9:0A      LDA:imm    #0A		 ; Game status = non-player input
C883 85:00      STA:zp     Zp RAM 0000	 ; mode.
C885 A9:14      LDA:imm    #14
C887 85:02      STA:zp     Zp RAM 0002
C889 A9:00      LDA:imm    #00           ; Zero out the relative change in
C88B 85:50      STA:zp     Zp RAM 0050   ; spinner position.
C88D 8D:23 01   STA:abs    $0123
C890 60:        RTS:imp    Ret from Sub
C891 AD:00 0C   LDA:abs    $0C00         ; Check to see if the test swith
C894 29:10      AND:imm    #10           ; is activated. If not, skip a bit.
C896 D0:07      BNE:rel    Branch->$C89F
C898 A9:22      LDA:imm    #22           ; Set game status to "test menu".
C89A 85:00      STA:zp     Zp RAM 0000	 ; (The first one.)
C89C B8:        CLV:imp    Clear Overflow
C89D 50:44      BVC:rel    Branch->$C8E3
C89F 24:05      BIT:zp     Zp RAM 0005
C8A1 70:40      BVS:rel    Branch->$C8E3
C8A3 A5:0A      LDA:zp     Zp RAM 000A
C8A5 29:01      AND:imm    #01
C8A7 F0:29      BEQ:rel    Branch->$C8D2
C8A9 A4:06      LDY:zp     Zp RAM 0006
C8AB D0:04      BNE:rel    Branch->$C8B1
C8AD A9:80      LDA:imm    #80
C8AF 85:A2      STA:zp     Zp RAM 00A2
C8B1 24:A2      BIT:zp     Zp RAM 00A2
C8B3 10:1D      BPL:rel    Branch->$C8D2
C8B5 C0:02      CPY:imm    #02
C8B7 B0:11      BCS:rel    Branch->$C8CA
C8B9 98:        TYA:imp    Y-->A
C8BA F0:08      BEQ:rel    Branch->$C8C4
C8BC A9:16      LDA:imm    #16
C8BE 85:01      STA:zp     Zp RAM 0001
C8C0 A9:0A      LDA:imm    #0A		 ; Game status = non-player input
C8C2 85:00      STA:zp     Zp RAM 0000	 ; mode.
C8C4 4C:D9 C8   JMP:abs    $C8D9
C8C7 B8:        CLV:imp    Clear Overflow
C8C8 50:08      BVC:rel    Branch->$C8D2
C8CA A9:14      LDA:imm    #14
C8CC 85:00      STA:zp     Zp RAM 0000
C8CE A9:00      LDA:imm    #00
C8D0 85:A2      STA:zp     Zp RAM 00A2
C8D2 A5:06      LDA:zp     Zp RAM 0006	 ; If there are no game credits, 
C8D4 F0:03      BEQ:rel    Branch->$C8D9 ; skip a bit.
C8D6 20:1B C8   JSR:abs    $C81B
C8D9 A5:09      LDA:zp     Zp RAM 0009
C8DB 29:03      AND:imm    #03
C8DD D0:04      BNE:rel    Branch->$C8E3
C8DF A9:02      LDA:imm    #02
C8E1 85:06      STA:zp     Zp RAM 0006
C8E3 E6:03      INC:zp     Zp RAM 0003
C8E5 A5:03      LDA:zp     Zp RAM 0003
C8E7 29:01      AND:imm    #01
C8E9 F0:03      BEQ:rel    Branch->$C8EE
C8EB 20:1B DE   JSR:abs    $DE1B
C8EE A5:0C      LDA:zp     Zp RAM 000C
C8F0 F0:03      BEQ:rel    Branch->$C8F5
C8F2 20:FA CC   JSR:abs    $CCFA
C8F5 AD:6C 01   LDA:abs    $016C
C8F8 F0:07      BEQ:rel    Branch->$C901
C8FA A9:13      LDA:imm    #13
C8FC C5:9F      CMP:zp     Zp RAM 009F
C8FE B0:01      BCS:rel    Branch->$C901
C900 F8:        SED:imp    Set Decimal
C901 A5:4E      LDA:zp     Zp RAM 004E
C903 29:80      AND:imm    #80
C905 F0:04      BEQ:rel    Branch->$C90B
C907 A9:00      LDA:imm    #00
C909 85:4E      STA:zp     Zp RAM 004E
C90B 60:        RTS:imp    Ret from Sub
;
C90C 20:A2 AB   JSR:abs    $ABA2
C90F 20:6E C1   JSR:abs    $C16E
C912 A5:05      LDA:zp     Zp RAM 0005
C914 10:03      BPL:rel    Branch->$C919
C916 20:62 CA   JSR:abs    $CA62	 ; Clear out the player 1&2 scores.
C919 A9:00      LDA:imm    #00
C91B 85:49      STA:zp     Zp RAM 0049
C91D A6:3E      LDX:zp     Zp RAM 003E
C91F 86:3D      STX:zp     Zp RAM 003D
C921 A6:3D      LDX:zp     Zp RAM 003D
C923 AD:58 01   LDA:abs    $0158
C926 9D:48 00   STA:abs,x  $0048,X	 ; Set # of lives for the player
C929 A9:FF      LDA:imm    #FF		 ; Set level for player to FF.
C92B 9D:46 00   STA:abs,x  $0046,X	 ;
C92E C6:3D      DEC:zp     Zp RAM 003D	 ; Do this for player one and two.
C930 10:EF      BPL:rel    Branch->$C921
C932 A9:00      LDA:imm    #00
C934 85:3F      STA:zp     Zp RAM 003F
C936 8D:15 01   STA:abs    $0115
C939 A5:3E      LDA:zp     Zp RAM 003E
C93B 85:3D      STA:zp     Zp RAM 003D
C93D 4C:C4 90   JMP:abs    $90C4	 ; Go to level selection menu

C940 A9:00      LDA:imm    #00		 ; Set game play to attract
C942 85:01      STA:zp     Zp RAM 0001	 ; screen or gameplay mode.
C944 A9:1E      LDA:imm    #1E
C946 85:00      STA:zp     Zp RAM 0000
C948 85:02      STA:zp     Zp RAM 0002
C94A A5:3F      LDA:zp     Zp RAM 003F
C94C C5:3D      CMP:zp     Zp RAM 003D
C94E F0:1C      BEQ:rel    Branch->$C96C
C950 85:3D      STA:zp     Zp RAM 003D
C952 A5:05      LDA:zp     Zp RAM 0005
C954 10:16      BPL:rel    Branch->$C96C
C956 A9:0E      LDA:imm    #0E
C958 85:01      STA:zp     Zp RAM 0001
C95A A9:0A      LDA:imm    #0A		 ; Game status = non-player
C95C 85:00      STA:zp     Zp RAM 0000	 ; input mode.
C95E A9:50      LDA:imm    #50
C960 AC:17 01   LDY:abs    $0117	 ; Is this game a cocktail?
C963 F0:02      BEQ:rel    Branch->$C967 ; If so, branch.
C965 A9:28      LDA:imm    #28
C967 85:04      STA:zp     Zp RAM 0004
C969 20:B2 92   JSR:abs    $92B2	 ; Swap some game variables betwen
					 ; the active and inactive players.
C96C 20:48 CA   JSR:abs    $CA48
C96F A6:3D      LDX:zp     Zp RAM 003D	 ; Load the level for the
C971 B5:46      LDA:zp,x   Zp RAM 0046	 ; current player, store in
C973 85:9F      STA:zp     Zp RAM 009F	 ; $009F.
C975 20:25 90   JSR:abs    $9025	 ; Game initialization routine
C978 4C:95 CD   JMP:abs    $CD95	 ; Pokey initialization routine.

C97B A9:04      LDA:imm    #04
C97D 85:02      STA:zp     Zp RAM 0002
C97F A9:00      LDA:imm    #00
C981 85:01      STA:zp     Zp RAM 0001
C983 A9:0A      LDA:imm    #0A		 ; Game status = non-player
C985 85:00      STA:zp     Zp RAM 0000	 ; input mode.
C987 A9:14      LDA:imm    #14
C989 85:04      STA:zp     Zp RAM 0004
C98B 60:        RTS:imp    Ret from Sub
C98C A6:3D      LDX:zp     Zp RAM 003D
C98E B5:46      LDA:zp,x   Zp RAM 0046	 ; Get level for current player.
C990 C9:62      CMP:imm    #62		 ; If we are below level 98,
C992 B0:04      BCS:rel    Branch->$C998 ; then increase the level.
C994 F6:46      INC:zp,x   Zp RAM 0046   ; Otherwise, do nothing
C996 E6:9F      INC:zp     Zp RAM 009F	 ; with it.
C998 A9:18      LDA:imm    #18		 ; Game status = zooming in the
C99A 85:00      STA:zp     Zp RAM 0000	 ; next level to be played.
C99C BD:02 01   LDA:abs,x  $0102,X	 ; Which choice was selected
					 ; by the player on the level
					 ; selection screen? (This is
					 ; the choice #, not level #.)
C99F F0:0B      BEQ:rel    Branch->$C9AC ; Skip some stuff if they
					 ; chose the first level.
C9A1 20:B5 91   JSR:abs    $91B5
C9A4 A2:FF      LDX:imm    #FF
C9A6 20:6C CA   JSR:abs    $CA6C
C9A9 20:B9 CC   JSR:abs    $CCB9
C9AC 4C:09 90   JMP:abs    $9009

C9AF A9:00      LDA:imm    #00
C9B1 85:04      STA:zp     Zp RAM 0004
C9B3 A6:3D      LDX:zp     Zp RAM 003D   ; Load the current player number.
C9B5 D6:48      DEC:zp,x   Zp RAM 0048	 ; Decrease player lives by one.
C9B7 A5:48      LDA:zp     Zp RAM 0048
C9B9 05:49      ORA:zp     Zp RAM 0049
C9BB D0:06      BNE:rel    Branch->$C9C3
C9BD 20:F1 C9   JSR:abs    $C9F1
C9C0 B8:        CLV:imp    Clear Overflow
C9C1 50:2D      BVC:rel    Branch->$C9F0
C9C3 A6:3D      LDX:zp     Zp RAM 003D
C9C5 B5:48      LDA:zp,x   Zp RAM 0048	 ; Player has any lives left?
C9C7 D0:08      BNE:rel    Branch->$C9D1
C9C9 A9:0C      LDA:imm    #0C
C9CB 85:01      STA:zp     Zp RAM 0001
C9CD A9:28      LDA:imm    #28
C9CF 85:04      STA:zp     Zp RAM 0004
C9D1 A5:3E      LDA:zp     Zp RAM 003E
C9D3 F0:06      BEQ:rel    Branch->$C9DB
C9D5 A5:3F      LDA:zp     Zp RAM 003F
C9D7 49:01      EOR:imm    #01
C9D9 85:3F      STA:zp     Zp RAM 003F
C9DB A6:3F      LDX:zp     Zp RAM 003F
C9DD B5:48      LDA:zp,x   Zp RAM 0048	 ; Player have any lives left?
C9DF F0:F0      BEQ:rel    Branch->$C9D1
C9E1 A9:02      LDA:imm    #02
C9E3 B4:46      LDY:zp,x   Zp RAM 0046
C9E5 C8:        INY:imp    Y=Y+1
C9E6 D0:02      BNE:rel    Branch->$C9EA
C9E8 A9:1C      LDA:imm    #1C
C9EA 85:02      STA:zp     Zp RAM 0002
C9EC A9:0A      LDA:imm    #0A		 ; Game status = non-player input
C9EE 85:00      STA:zp     Zp RAM 0000	 ; mode.
C9F0 60:        RTS:imp    Ret from Sub
;
C9F1 A9:00      LDA:imm    #00
C9F3 8D:26 01   STA:abs    $0126
C9F6 A6:3E      LDX:zp     Zp RAM 003E
C9F8 B5:46      LDA:zp,x   Zp RAM 0046
C9FA CD:26 01   CMP:abs    $0126
C9FD 90:03      BCC:rel    Branch->$CA02
C9FF 8D:26 01   STA:abs    $0126
CA02 CA:        DEX:imp    X=X-1
CA03 10:F3      BPL:rel    Branch->$C9F8
CA05 AC:26 01   LDY:abs    $0126
CA08 F0:03      BEQ:rel    Branch->$CA0D
CA0A CE:26 01   DEC:abs    $0126
CA0D A9:14      LDA:imm    #14
CA0F 24:05      BIT:zp     Zp RAM 0005
CA11 10:02      BPL:rel    Branch->$CA15
CA13 A9:10      LDA:imm    #10
CA15 85:00      STA:zp     Zp RAM 0000
CA17 60:        RTS:imp    Ret from Sub
;
CA18 A5:05      LDA:zp     Zp RAM 0005
CA1A 29:3F      AND:imm    #3F
CA1C 85:05      STA:zp     Zp RAM 0005
CA1E A9:00      LDA:imm    #00
CA20 85:3E      STA:zp     Zp RAM 003E
CA22 A9:1A      LDA:imm    #1A
CA24 85:02      STA:zp     Zp RAM 0002
CA26 A9:0A      LDA:imm    #0A		 ; Game status=non-player input
CA28 85:00      STA:zp     Zp RAM 0000	 ; mode.
CA2A A9:A0      LDA:imm    #A0
CA2C 85:04      STA:zp     Zp RAM 0004
CA2E A9:01      LDA:imm    #01
CA30 8D:6B 01   STA:abs    $016B
CA33 A9:0A      LDA:imm    #0A		 ; Set game mode to post high
CA35 85:01      STA:zp     Zp RAM 0001	 ; score initial entry.
CA37 60:        RTS:imp    Ret from Sub
;
; DATA segment of unknown size.
;
CA38 80:        Illegal Opcode
CA39 40:        RTI:imp    Ret from Int
CA3A 20:10 08   JSR:abs    $0810
CA3D 04:        Illegal Opcode
CA3E 02:        Illegal Opcode
CA3F 01:01      ORA:ind,x  01
CA41 02:        Illegal Opcode
CA42 04:        Illegal Opcode
CA43 08:        PHP:imp    Push P
CA44 10:20      BPL:rel    Branch->$CA66
CA46 40:        RTI:imp    Ret from Int
CA47 80:        Illegal Opcode
;
; Subroutine begins here.
;
CA48 A0:10      LDY:imm    #10
CA4A AD:17 01   LDA:abs    $0117	 ; Is this game a cocktail?
CA4D F0:08      BEQ:rel    Branch->$CA57 ; If not, branch.
CA4F A5:3D      LDA:zp     Zp RAM 003D	 ; Is this player one?
CA51 F0:04      BEQ:rel    Branch->$CA57 ; If so, branch.
CA53 A9:04      LDA:imm    #04
CA55 A0:08      LDY:imm    #08		 ; Invert (flip) X the Axis.
CA57 45:A1      EOR:zp     Zp RAM 00A1	 ;
CA59 29:04      AND:imm    #04		 ; Documented as setting the
CA5B 45:A1      EOR:zp     Zp RAM 00A1   ; unused coin cointer.
CA5D 85:A1      STA:zp     Zp RAM 00A1   ; 
CA5F 84:B4      STY:zp     Zp RAM 00B4   ; Store the axis flip state.
CA61 60:        RTS:imp    Ret from Sub
;
CA62 A9:00      LDA:imm    #00		 ; Clear out the current
CA64 A2:05      LDX:imm    #05		 ; score for player one
CA66 95:40      STA:zp,x   Zp RAM 0040	 ; and for player two.
CA68 CA:        DEX:imp    X=X-1	 ;
CA69 10:FB      BPL:rel    Branch->$CA66 ;
CA6B 60:        RTS:imp    Ret from Sub	 ;
;
CA6C F8:        SED:imp    Set Decimal	 ; Decimal math mode used for scores.
CA6D 24:05      BIT:zp     Zp RAM 0005
CA6F 10:7E      BPL:rel    Branch->$CAEF
CA71 A4:3D      LDY:zp     Zp RAM 003D	 ; If we are dealing with player 2,
CA73 F0:02      BEQ:rel    Branch->$CA77 ; offset the memory location of the
CA75 A0:03      LDY:imm    #03	         ; score by three bytes.
CA77 E0:08      CPX:imm    #08
CA79 90:16      BCC:rel    Branch->$CA91

CA7B A5:29      LDA:zp     Zp RAM 0029
CA7D 18:        CLC:imp    Clear Carry
CA7E 79:40 00   ADC:abs,y  $0040,Y	 ; Update the score.
CA81 99:40 00   STA:abs,y  $0040,Y
CA84 A5:2A      LDA:zp     Zp RAM 002A
CA86 79:41 00   ADC:abs,y  $0041,Y
CA89 99:41 00   STA:abs,y  $0041,Y
CA8C A5:2B      LDA:zp     Zp RAM 002B
CA8E B8:        CLV:imp    Clear Overflow
CA8F 50:15      BVC:rel    Branch->$CAA6
CA91 BD:F1 CA   LDA:abs,x  $CAF1,X
CA94 18:        CLC:imp    Clear Carry
CA95 79:40 00   ADC:abs,y  $0040,Y	 ; Update the score.
CA98 99:40 00   STA:abs,y  $0040,Y
CA9B BD:F9 CA   LDA:abs,x  $CAF9,X
CA9E 79:41 00   ADC:abs,y  $0041,Y
CAA1 99:41 00   STA:abs,y  $0041,Y
CAA4 A9:00      LDA:imm    #00
CAA6 08:        PHP:imp    Push P
CAA7 79:42 00   ADC:abs,y  $0042,Y
CAAA 99:42 00   STA:abs,y  $0042,Y
CAAD 28:        PLP:imp    Pull P
CAAE F0:0B      BEQ:rel    Branch->$CABB
CAB0 AE:56 01   LDX:abs    $0156
CAB3 F0:06      BEQ:rel    Branch->$CABB
CAB5 E4:2B      CPX:zp     Zp RAM 002B
CAB7 F0:23      BEQ:rel    Branch->$CADC
CAB9 90:21      BCC:rel    Branch->$CADC
CABB 90:32      BCC:rel    Branch->$CAEF
CABD AE:56 01   LDX:abs    $0156
CAC0 F0:2C      BEQ:rel    Branch->$CAEE
CAC2 E0:03      CPX:imm    #03
CAC4 90:0B      BCC:rel    Branch->$CAD1
CAC6 38:        SEC:imp    Set Carry
CAC7 ED:56 01   SBC:abs    $0156
CACA F0:10      BEQ:rel    Branch->$CADC
CACC B0:F8      BCS:rel    Branch->$CAC6
CACE B8:        CLV:imp    Clear Overflow
CACF 50:1D      BVC:rel    Branch->$CAEE
CAD1 E0:02      CPX:imm    #02
CAD3 D0:07      BNE:rel    Branch->$CADC
CAD5 29:01      AND:imm    #01
CAD7 F0:03      BEQ:rel    Branch->$CADC
CAD9 B8:        CLV:imp    Clear Overflow
CADA 50:12      BVC:rel    Branch->$CAEE
CADC A6:3D      LDX:zp     Zp RAM 003D
CADE B5:48      LDA:zp,x   Zp RAM 0048	 ; Compare the number of player's
CAE0 C9:06      CMP:imm    #06		 ; lives to "6".
CAE2 B0:0A      BCS:rel    Branch->$CAEE
CAE4 F6:48      INC:zp,x   Zp RAM 0048	 ; Increase player's lives by one.
CAE6 20:B9 CC   JSR:abs    $CCB9
CAE9 A9:20      LDA:imm    #20
CAEB 8D:24 01   STA:abs    $0124
CAEE 38:        SEC:imp    Set Carry
CAEF D8:        CLD:imp    Unset Decimal ; Set normal math mode and
CAF0 60:        RTS:imp    Ret from Sub  ; return.
;
; Multiple DATA segments of unknown size
; CAF1-CAF8  CAF9-CB00  CB01-
;
CAF1 00:        BRK:imp    BREAK
CAF2 50:00      BVC:rel    Branch->$CAF4
CAF4 00:        BRK:imp    BREAK
CAF5 50:50      BVC:rel    Branch->$CB47
CAF7 00:        BRK:imp    BREAK
CAF8 50:00      BVC:rel    Branch->$CAFA
CAFA 01:02      ORA:ind,x  02
CAFC 01:00      ORA:ind,x  00
CAFE 02:        Illegal Opcode
CAFF 05:07      ORA:zp     Zp RAM 0007
CB01 00:        BRK:imp    BREAK
CB02 00:        BRK:imp    BREAK
CB03 00:        BRK:imp    BREAK
CB04 00:        BRK:imp    BREAK
CB05 00:        BRK:imp    BREAK
CB06 00:        BRK:imp    BREAK
CB07 00:        BRK:imp    BREAK
CB08 00:        BRK:imp    BREAK
CB09 35:38      AND:zp,x   Zp RAM 0038
CB0B 00:        BRK:imp    BREAK
CB0C 00:        BRK:imp    BREAK
CB0D 00:        BRK:imp    BREAK
CB0E 00:        BRK:imp    BREAK
CB0F 00:        BRK:imp    BREAK
CB10 00:        BRK:imp    BREAK
CB11 00:        BRK:imp    BREAK
CB12 00:        BRK:imp    BREAK
CB13 47:        Illegal Opcode
CB14 4A:        LSR:accum  
CB15 00:        BRK:imp    BREAK
CB16 00:        BRK:imp    BREAK
CB17 00:        BRK:imp    BREAK
CB18 00:        BRK:imp    BREAK
CB19 00:        BRK:imp    BREAK
CB1A 00:        BRK:imp    BREAK
CB1B 00:        BRK:imp    BREAK
CB1C 00:        BRK:imp    BREAK
CB1D 00:        BRK:imp    BREAK
CB1E 00:        BRK:imp    BREAK
CB1F 00:        BRK:imp    BREAK
CB20 00:        BRK:imp    BREAK
CB21 00:        BRK:imp    BREAK
CB22 00:        BRK:imp    BREAK
CB23 00:        BRK:imp    BREAK
CB24 00:        BRK:imp    BREAK
CB25 0D:10 00   ORA:abs    $0010
CB28 00:        BRK:imp    BREAK
CB29 00:        BRK:imp    BREAK
CB2A 00:        BRK:imp    BREAK
CB2B 00:        BRK:imp    BREAK
CB2C 00:        BRK:imp    BREAK
CB2D 00:        BRK:imp    BREAK
CB2E 00:        BRK:imp    BREAK
CB2F 00:        BRK:imp    BREAK
CB30 00:        BRK:imp    BREAK
CB31 00:        BRK:imp    BREAK
CB32 00:        BRK:imp    BREAK
CB33 00:        BRK:imp    BREAK
CB34 00:        BRK:imp    BREAK
CB35 00:        BRK:imp    BREAK
CB36 00:        BRK:imp    BREAK
CB37 00:        BRK:imp    BREAK
CB38 00:        BRK:imp    BREAK
CB39 00:        BRK:imp    BREAK
CB3A 00:        BRK:imp    BREAK
CB3B 65:68      ADC:zp     Zp RAM 0068
CB3D 00:        BRK:imp    BREAK
CB3E 00:        BRK:imp    BREAK
CB3F 00:        BRK:imp    BREAK
CB40 00:        BRK:imp    BREAK
CB41 00:        BRK:imp    BREAK
CB42 00:        BRK:imp    BREAK
CB43 00:        BRK:imp    BREAK
CB44 00:        BRK:imp    BREAK
CB45 00:        BRK:imp    BREAK
CB46 00:        BRK:imp    BREAK
CB47 21:32      AND:ind,x  32
CB49 00:        BRK:imp    BREAK
CB4A 00:        BRK:imp    BREAK
CB4B 00:        BRK:imp    BREAK
CB4C 00:        BRK:imp    BREAK
CB4D 00:        BRK:imp    BREAK
CB4E 00:        BRK:imp    BREAK
CB4F 00:        BRK:imp    BREAK
CB50 00:        BRK:imp    BREAK
CB51 13:        Illegal Opcode
CB52 1A:        Illegal Opcode
CB53 00:        BRK:imp    BREAK
CB54 00:        BRK:imp    BREAK
CB55 00:        BRK:imp    BREAK
CB56 00:        BRK:imp    BREAK
CB57 00:        BRK:imp    BREAK
CB58 00:        BRK:imp    BREAK
CB59 00:        BRK:imp    BREAK
CB5A 00:        BRK:imp    BREAK
CB5B 00:        BRK:imp    BREAK
CB5C 00:        BRK:imp    BREAK
CB5D 00:        BRK:imp    BREAK
CB5E 00:        BRK:imp    BREAK
CB5F 00:        BRK:imp    BREAK
CB60 00:        BRK:imp    BREAK
CB61 00:        BRK:imp    BREAK
CB62 00:        BRK:imp    BREAK
CB63 00:        BRK:imp    BREAK
CB64 00:        BRK:imp    BREAK
CB65 00:        BRK:imp    BREAK
CB66 00:        BRK:imp    BREAK
CB67 00:        BRK:imp    BREAK
CB68 00:        BRK:imp    BREAK
CB69 00:        BRK:imp    BREAK
CB6A 00:        BRK:imp    BREAK
CB6B 53:        Illegal Opcode
CB6C 56:00      LSR:zp,x   Zp RAM 0000
CB6E 00:        BRK:imp    BREAK
CB6F 00:        BRK:imp    BREAK
CB70 00:        BRK:imp    BREAK
CB71 00:        BRK:imp    BREAK
CB72 00:        BRK:imp    BREAK
CB73 00:        BRK:imp    BREAK
CB74 00:        BRK:imp    BREAK
CB75 00:        BRK:imp    BREAK
CB76 00:        BRK:imp    BREAK
CB77 00:        BRK:imp    BREAK
CB78 00:        BRK:imp    BREAK
CB79 00:        BRK:imp    BREAK
CB7A 00:        BRK:imp    BREAK
CB7B 59:5C 00   EOR:abs,y  $005C,Y
CB7E 00:        BRK:imp    BREAK
CB7F 00:        BRK:imp    BREAK
CB80 00:        BRK:imp    BREAK
CB81 00:        BRK:imp    BREAK
CB82 00:        BRK:imp    BREAK
CB83 00:        BRK:imp    BREAK
CB84 00:        BRK:imp    BREAK
CB85 00:        BRK:imp    BREAK
CB86 00:        BRK:imp    BREAK
CB87 00:        BRK:imp    BREAK
CB88 00:        BRK:imp    BREAK
CB89 00:        BRK:imp    BREAK
CB8A 00:        BRK:imp    BREAK
CB8B 00:        BRK:imp    BREAK
CB8C 00:        BRK:imp    BREAK
CB8D 00:        BRK:imp    BREAK
CB8E 00:        BRK:imp    BREAK
CB8F 3B:        Illegal Opcode
CB90 3E:00 00   ROL:abs,x  $0000,X
CB93 00:        BRK:imp    BREAK
CB94 00:        BRK:imp    BREAK
CB95 00:        BRK:imp    BREAK
CB96 00:        BRK:imp    BREAK
CB97 00:        BRK:imp    BREAK
CB98 00:        BRK:imp    BREAK
CB99 00:        BRK:imp    BREAK
CB9A 00:        BRK:imp    BREAK
CB9B 00:        BRK:imp    BREAK
CB9C 00:        BRK:imp    BREAK
CB9D 41:44      EOR:ind,x  44
CB9F 00:        BRK:imp    BREAK
CBA0 00:        BRK:imp    BREAK
CBA1 4D:50 00   EOR:abs    $0050
CBA4 00:        BRK:imp    BREAK
CBA5 00:        BRK:imp    BREAK
CBA6 00:        BRK:imp    BREAK
CBA7 00:        BRK:imp    BREAK
CBA8 00:        BRK:imp    BREAK
CBA9 00:        BRK:imp    BREAK
CBAA 00:        BRK:imp    BREAK
CBAB 00:        BRK:imp    BREAK
CBAC 00:        BRK:imp    BREAK
CBAD 00:        BRK:imp    BREAK
CBAE 00:        BRK:imp    BREAK
CBAF 00:        BRK:imp    BREAK
CBB0 00:        BRK:imp    BREAK
CBB1 5F:        Illegal Opcode
CBB2 62:        Illegal Opcode
CBB3 00:        BRK:imp    BREAK
CBB4 00:        BRK:imp    BREAK
CBB5 00:        BRK:imp    BREAK
CBB6 00:        BRK:imp    BREAK
CBB7 00:        BRK:imp    BREAK
CBB8 00:        BRK:imp    BREAK
CBB9 00:        BRK:imp    BREAK
CBBA 00:        BRK:imp    BREAK
CBBB 00:        BRK:imp    BREAK
CBBC 00:        BRK:imp    BREAK
CBBD 00:        BRK:imp    BREAK
CBBE 00:        BRK:imp    BREAK
CBBF 00:        BRK:imp    BREAK
CBC0 00:        BRK:imp    BREAK
CBC1 00:        BRK:imp    BREAK
CBC2 00:        BRK:imp    BREAK
CBC3 00:        BRK:imp    BREAK
CBC4 00:        BRK:imp    BREAK
CBC5 00:        BRK:imp    BREAK
CBC6 00:        BRK:imp    BREAK
CBC7 00:        BRK:imp    BREAK
CBC8 00:        BRK:imp    BREAK
CBC9 00:        BRK:imp    BREAK
CBCA 00:        BRK:imp    BREAK
CBCB 6D:6D 00   ADC:abs    $006D
CBCE 00:        BRK:imp    BREAK
CBCF 00:        BRK:imp    BREAK
CBD0 00:        BRK:imp    BREAK
CBD1 C0:08      CPY:imm    #08
CBD3 04:        Illegal Opcode
CBD4 10:00      BPL:rel    Branch->$CBD6
CBD6 00:        BRK:imp    BREAK
CBD7 A6:20      LDX:zp     Zp RAM 0020
CBD9 F8:        SED:imp    Set Decimal
CBDA 04:        Illegal Opcode
CBDB 00:        BRK:imp    BREAK
CBDC 00:        BRK:imp    BREAK
CBDD 40:        RTI:imp    Ret from Int
CBDE 08:        PHP:imp    Push P
CBDF 04:        Illegal Opcode
CBE0 10:00      BPL:rel    Branch->$CBE2
CBE2 00:        BRK:imp    BREAK
CBE3 A6:20      LDX:zp     Zp RAM 0020
CBE5 FE:04 00   INC:abs,x  $0004,X
CBE8 00:        BRK:imp    BREAK
CBE9 10:01      BPL:rel    Branch->$CBEC
CBEB 07:        Illegal Opcode
CBEC 20:00 00   JSR:abs    $0000
CBEF A2:01      LDX:imm    #01
CBF1 F8:        SED:imp    Set Decimal
CBF2 20:00 00   JSR:abs    $0000
CBF5 08:        PHP:imp    Push P
CBF6 04:        Illegal Opcode
CBF7 20:0A 08   JSR:abs    $080A
CBFA 04:        Illegal Opcode
CBFB 01:09      ORA:ind,x  09
CBFD 10:0D      BPL:rel    Branch->$CC0C
CBFF 04:        Illegal Opcode
CC00 0C:        Illegal Opcode
CC01 00:        BRK:imp    BREAK
CC02 00:        BRK:imp    BREAK
CC03 08:        PHP:imp    Push P
CC04 04:        Illegal Opcode
CC05 00:        BRK:imp    BREAK
CC06 0A:        ASL:accum  
CC07 68:        PLA:imp    Pull Accum
CC08 04:        Illegal Opcode
CC09 00:        BRK:imp    BREAK
CC0A 09:68      ORA:imm    #68
CC0C 12:        Illegal Opcode
CC0D FF:        Illegal Opcode
CC0E 09:00      ORA:imm    #00
CC10 00:        BRK:imp    BREAK
CC11 40:        RTI:imp    Ret from Int
CC12 01:00      ORA:ind,x  00
CC14 01:40      ORA:ind,x  40
CC16 01:FF      ORA:ind,x  FF
CC18 40:        RTI:imp    Ret from Int
CC19 30:01      BMI:rel    Branch->$CC1C
CC1B FF:        Illegal Opcode
CC1C 30:20      BMI:rel    Branch->$CC3E
CC1E 01:FF      ORA:ind,x  FF
CC20 20:18 01   JSR:abs    $0118
CC23 FF:        Illegal Opcode
CC24 18:        CLC:imp    Clear Carry
CC25 14:        Illegal Opcode
CC26 01:FF      ORA:ind,x  FF
CC28 14:        Illegal Opcode
CC29 12:        Illegal Opcode
CC2A 01:FF      ORA:ind,x  FF
CC2C 12:        Illegal Opcode
CC2D 10:01      BPL:rel    Branch->$CC30
CC2F FF:        Illegal Opcode
CC30 10:00      BPL:rel    Branch->$CC32
CC32 00:        BRK:imp    BREAK
CC33 A8:        TAY:imp    A-->Y
CC34 93:        Illegal Opcode
CC35 00:        BRK:imp    BREAK
CC36 02:        Illegal Opcode
CC37 00:        BRK:imp    BREAK
CC38 00:        BRK:imp    BREAK
CC39 0F:        Illegal Opcode
CC3A 04:        Illegal Opcode
CC3B 00:        BRK:imp    BREAK
CC3C 01:00      ORA:ind,x  00
CC3E 00:        BRK:imp    BREAK
CC3F A2:04      LDX:imm    #04
CC41 40:        RTI:imp    Ret from Int
CC42 01:00      ORA:ind,x  00
CC44 00:        BRK:imp    BREAK
CC45 00:        BRK:imp    BREAK
CC46 03:        Illegal Opcode
CC47 02:        Illegal Opcode
CC48 09:00      ORA:imm    #00
CC4A 00:        BRK:imp    BREAK
CC4B 08:        PHP:imp    Push P
CC4C 03:        Illegal Opcode
CC4D FF:        Illegal Opcode
CC4E 09:00      ORA:imm    #00
CC50 00:        BRK:imp    BREAK
CC51 80:        Illegal Opcode
CC52 01:E8      ORA:ind,x  E8
CC54 05:00      ORA:zp     Zp RAM 0000
CC56 00:        BRK:imp    BREAK
CC57 A1:01      LDA:ind,x  01
CC59 01:05      ORA:ind,x  05
CC5B 00:        BRK:imp    BREAK
CC5C 00:        BRK:imp    BREAK
CC5D 01:08      ORA:ind,x  08
CC5F 02:        Illegal Opcode
CC60 10:00      BPL:rel    Branch->$CC62
CC62 00:        BRK:imp    BREAK
CC63 86:20      STX:zp     Zp RAM 0020
CC65 00:        BRK:imp    BREAK
CC66 04:        Illegal Opcode
CC67 00:        BRK:imp    BREAK
CC68 00:        BRK:imp    BREAK
CC69 18:        CLC:imp    Clear Carry
CC6A 04:        Illegal Opcode
CC6B 00:        BRK:imp    BREAK
CC6C FF:        Illegal Opcode
CC6D 00:        BRK:imp    BREAK
CC6E 00:        BRK:imp    BREAK
CC6F AF:        Illegal Opcode
CC70 04:        Illegal Opcode
CC71 00:        BRK:imp    BREAK
CC72 FF:        Illegal Opcode
CC73 00:        BRK:imp    BREAK
CC74 00:        BRK:imp    BREAK
CC75 C0:02      CPY:imm    #02
CC77 FF:        Illegal Opcode
CC78 FF:        Illegal Opcode
CC79 00:        BRK:imp    BREAK
CC7A 00:        BRK:imp    BREAK
CC7B 28:        PLP:imp    Pull P
CC7C 02:        Illegal Opcode
CC7D 00:        BRK:imp    BREAK
CC7E F0:00      BEQ:rel    Branch->$CC80
CC80 00:        BRK:imp    BREAK
CC81 10:0B      BPL:rel    Branch->$CC8E
CC83 01:40      ORA:ind,x  40
CC85 00:        BRK:imp    BREAK
CC86 00:        BRK:imp    BREAK
CC87 86:40      STX:zp     Zp RAM 0040
CC89 00:        BRK:imp    BREAK
CC8A 0B:        Illegal Opcode
CC8B 00:        BRK:imp    BREAK
CC8C 00:        BRK:imp    BREAK
CC8D 20:80 00   JSR:abs    $0080
CC90 03:        Illegal Opcode
CC91 00:        BRK:imp    BREAK
CC92 00:        BRK:imp    BREAK
CC93 A8:        TAY:imp    A-->Y
CC94 40:        RTI:imp    Ret from Int
CC95 F8:        SED:imp    Set Decimal
CC96 06:00      ASL:zp     Zp RAM 0000
CC98 00:        BRK:imp    BREAK
CC99 B0:02      BCS:rel    Branch->$CC9D
CC9B 00:        BRK:imp    BREAK
CC9C FF:        Illegal Opcode
CC9D 00:        BRK:imp    BREAK
CC9E 00:        BRK:imp    BREAK
CC9F C8:        INY:imp    Y=Y+1
CCA0 01:02      ORA:ind,x  02
CCA2 FF:        Illegal Opcode
CCA3 C8:        INY:imp    Y=Y+1
CCA4 01:02      ORA:ind,x  02
CCA6 FF:        Illegal Opcode
CCA7 00:        BRK:imp    BREAK
CCA8 00:        BRK:imp    BREAK
CCA9 C0:01      CPY:imm    #01
CCAB 00:        BRK:imp    BREAK
CCAC 01:00      ORA:ind,x  00
CCAE 00:        BRK:imp    BREAK
CCAF 00:        BRK:imp    BREAK
CCB0 A9:5F      LDA:imm    #5F
CCB2 4C:C3 CC   JMP:abs    $CCC3
CCB5 A9:0F      LDA:imm    #0F
CCB7 D0:0A      BNE:rel    Branch->$CCC3
CCB9 A9:4F      LDA:imm    #4F
CCBB D0:06      BNE:rel    Branch->$CCC3
CCBD A9:8F      LDA:imm    #8F
CCBF D0:02      BNE:rel    Branch->$CCC3
CCC1 A9:1F      LDA:imm    #1F
CCC3 24:05      BIT:zp     Zp RAM 0005
CCC5 10:22      BPL:rel    Branch->$CCE9
CCC7 86:31      STX:zp     Zp RAM 0031
CCC9 84:32      STY:zp     Zp RAM 0032
CCCB A8:        TAY:imp    A-->Y
CCCC A2:0F      LDX:imm    #0F
CCCE B9:01 CB   LDA:abs,y  $CB01,Y
CCD1 F0:0E      BEQ:rel    Branch->$CCE1
CCD3 86:BF      STX:zp     Zp RAM 00BF
CCD5 95:C0      STA:zp,x   Zp RAM 00C0
CCD7 A9:01      LDA:imm    #01
CCD9 95:E0      STA:zp,x   Zp RAM 00E0
CCDB 95:F0      STA:zp,x   Zp RAM 00F0
CCDD A9:FF      LDA:imm    #FF
CCDF 85:BF      STA:zp     Zp RAM 00BF
CCE1 88:        DEY:imp    Y=Y-1
CCE2 CA:        DEX:imp    X=X-1
CCE3 10:E9      BPL:rel    Branch->$CCCE
CCE5 A6:31      LDX:zp     Zp RAM 0031
CCE7 A4:32      LDY:zp     Zp RAM 0032
CCE9 60:        RTS:imp    Ret from Sub
;
CCEA A9:2F      LDA:imm    #2F
CCEC D0:D5      BNE:rel    Branch->$CCC3
CCEE A9:6F      LDA:imm    #6F
CCF0 D0:D1      BNE:rel    Branch->$CCC3
CCF2 A9:7F      LDA:imm    #7F
CCF4 D0:CD      BNE:rel    Branch->$CCC3
CCF6 A9:9F      LDA:imm    #9F
CCF8 D0:C9      BNE:rel    Branch->$CCC3
CCFA A9:AF      LDA:imm    #AF
CCFC D0:C9      BNE:rel    Branch->$CCC7
CCFE A9:BF      LDA:imm    #BF
CD00 D0:C1      BNE:rel    Branch->$CCC3
CD02 A9:3F      LDA:imm    #3F
CD04 D0:BD      BNE:rel    Branch->$CCC3
CD06 A9:CF      LDA:imm    #CF
CD08 D0:B9      BNE:rel    Branch->$CCC3
CD0A A2:0F      LDX:imm    #0F
CD0C B5:C0      LDA:zp,x   Zp RAM 00C0
CD0E F0:7E      BEQ:rel    Branch->$CD8E
CD10 E4:BF      CPX:zp     Zp RAM 00BF
CD12 F0:7A      BEQ:rel    Branch->$CD8E
CD14 D6:E0      DEC:zp,x   Zp RAM 00E0
CD16 D0:76      BNE:rel    Branch->$CD8E
CD18 D6:F0      DEC:zp,x   Zp RAM 00F0
CD1A D0:38      BNE:rel    Branch->$CD54
CD1C F6:C0      INC:zp,x   Zp RAM 00C0
CD1E F6:C0      INC:zp,x   Zp RAM 00C0
CD20 B5:C0      LDA:zp,x   Zp RAM 00C0
CD22 0A:        ASL:accum  
CD23 A8:        TAY:imp    A-->Y
CD24 B0:10      BCS:rel    Branch->$CD36
CD26 B9:CB CB   LDA:abs,y  $CBCB,Y
CD29 95:D0      STA:zp,x   Zp RAM 00D0
CD2B B9:CE CB   LDA:abs,y  $CBCE,Y
CD2E 95:F0      STA:zp,x   Zp RAM 00F0
CD30 B9:CC CB   LDA:abs,y  $CBCC,Y
CD33 B8:        CLV:imp    Clear Overflow
CD34 50:0D      BVC:rel    Branch->$CD43
CD36 B9:CB CC   LDA:abs,y  $CCCB,Y
CD39 95:D0      STA:zp,x   Zp RAM 00D0
CD3B B9:CE CC   LDA:abs,y  $CCCE,Y
CD3E 95:F0      STA:zp,x   Zp RAM 00F0
CD40 B9:CC CC   LDA:abs,y  $CCCC,Y
CD43 95:E0      STA:zp,x   Zp RAM 00E0
CD45 D0:0A      BNE:rel    Branch->$CD51
CD47 95:C0      STA:zp,x   Zp RAM 00C0
CD49 B5:D0      LDA:zp,x   Zp RAM 00D0
CD4B F0:04      BEQ:rel    Branch->$CD51
CD4D 95:C0      STA:zp,x   Zp RAM 00C0
CD4F D0:CB      BNE:rel    Branch->$CD1C
CD51 B8:        CLV:imp    Clear Overflow
CD52 50:2B      BVC:rel    Branch->$CD7F
CD54 0A:        ASL:accum  
CD55 A8:        TAY:imp    A-->Y
CD56 B0:0B      BCS:rel    Branch->$CD63
CD58 B9:CC CB   LDA:abs,y  $CBCC,Y
CD5B 95:E0      STA:zp,x   Zp RAM 00E0
CD5D B9:CD CB   LDA:abs,y  $CBCD,Y
CD60 B8:        CLV:imp    Clear Overflow
CD61 50:08      BVC:rel    Branch->$CD6B
CD63 B9:CC CC   LDA:abs,y  $CCCC,Y
CD66 95:E0      STA:zp,x   Zp RAM 00E0
CD68 B9:CD CC   LDA:abs,y  $CCCD,Y
CD6B B4:D0      LDY:zp,x   Zp RAM 00D0
CD6D 18:        CLC:imp    Clear Carry
CD6E 75:D0      ADC:zp,x   Zp RAM 00D0
CD70 95:D0      STA:zp,x   Zp RAM 00D0
CD72 8A:        TXA:imp    X-->A
CD73 4A:        LSR:accum  
CD74 90:09      BCC:rel    Branch->$CD7F
CD76 98:        TYA:imp    Y-->A
CD77 55:D0      EOR:zp,x   Zp RAM 00D0
CD79 29:F0      AND:imm    #F0
CD7B 55:D0      EOR:zp,x   Zp RAM 00D0
CD7D 95:D0      STA:zp,x   Zp RAM 00D0
CD7F B5:D0      LDA:zp,x   Zp RAM 00D0
CD81 E0:08      CPX:imm    #08
CD83 90:06      BCC:rel    Branch->$CD8B
CD85 9D:C8 60   STA:abs,x  $60C8,X
CD88 B8:        CLV:imp    Clear Overflow
CD89 50:03      BVC:rel    Branch->$CD8E
CD8B 9D:C0 60   STA:abs,x  $60C0,X
CD8E CA:        DEX:imp    X=X-1
CD8F 30:03      BMI:rel    Branch->$CD94
CD91 4C:0C CD   JMP:abs    $CD0C
CD94 60:        RTS:imp    Ret from Sub
;
; Pokey initialization routine.
;
CD95 A9:00      LDA:imm    #00
CD97 8D:CF 60   STA:abs    $60CF	 ; Zero out Pokey #1 control reg
CD9A 8D:DF 60   STA:abs    $60DF	 ; Zero out Pokey #2 control reg
CD9D 8D:20 07   STA:abs    $0720
CDA0 A2:04      LDX:imm    #04
CDA2 AD:CA 60   LDA:abs    $60CA	 ; Randomize the accumulator.
CDA5 AC:DA 60   LDY:abs    $60DA	 ; Randomize the Y register.
CDA8 CD:CA 60   CMP:abs    $60CA	 ; Compare ACC w/random number.
CDAB D0:03      BNE:rel    Branch->$CDB0 ; 254/255 chance of branching.
CDAD CC:DA 60   CPY:abs    $60DA	 ; Compare w/random number.
CDB0 F0:05      BEQ:rel    Branch->$CDB7 ; 1/255 chance of branching.
CDB2 8D:20 07   STA:abs    $0720
CDB5 A2:00      LDX:imm    #00           
CDB7 CA:        DEX:imp    X=X-1
CDB8 10:EE      BPL:rel    Branch->$CDA8
CDBA A9:07      LDA:imm    #07           ; Enable sound *and* fast
					 ; potentiumeter scanning on
					 ; the Pokey.
CDBC 8D:CF 60   STA:abs    $60CF	 ; Initialize Pokey #1
CDBF 8D:DF 60   STA:abs    $60DF	 ; Initialize Pokey #2
CDC2 A2:07      LDX:imm    #07		 ; Zero out the volume/distortion
CDC4 A9:00      LDA:imm    #00		 ; and pitch for all channels
CDC6 9D:C0 60   STA:abs,x  $60C0,X	 ; on both Pokeys.
CDC9 9D:D0 60   STA:abs,x  $60D0,X	 ;
CDCC 95:C0      STA:zp,x   Zp RAM 00C0	 ;
CDCE 95:D0      STA:zp,x   Zp RAM 00D0	 ;
CDD0 CA:        DEX:imp    X=X-1	 ;
CDD1 10:F3      BPL:rel    Branch->$CDC6 ; End of initialization loop
CDD3 A9:00      LDA:imm    #00		 ; Clear the master control
CDD5 8D:C8 60   STA:abs    $60C8	 ; register (AUDCTL) for Pokey #1
CDD8 A9:00      LDA:imm    #00		 ; Clear the master control
CDDA 8D:D8 60   STA:abs    $60D8	 ; register (AUDCTL) for Pokey #2
CDDD 60:        RTS:imp    Ret from Sub
;
CDDE 0B:        Illegal Opcode
CDDF 5D:22 74   EOR:abs,x  $7422,X
CDE2 0C:        Illegal Opcode
CDE3 5E:34 50   LSR:abs,x  $5034,X
CDE6 00:        BRK:imp    BREAK
CDE7 71:C5      ADC:ind,y  C5
CDE9 68:        PLA:imp    Pull Accum
CDEA 40:        RTI:imp    Ret from Int
CDEB 80:        Illegal Opcode
CDEC 6C:01 40   JMP:ind    $4001
CDEF 1E:00 71   ASL:abs,x  $7100,X
CDF2 B4:A8      LDY:zp,x   Zp RAM 00A8
CDF4 B4:A8      LDY:zp,x   Zp RAM 00A8
CDF6 B4:A8      LDY:zp,x   Zp RAM 00A8
CDF8 B4:A8      LDY:zp,x   Zp RAM 00A8
CDFA B4:A8      LDY:zp,x   Zp RAM 00A8
CDFC 65:A8      ADC:zp     Zp RAM 00A8
CDFE 00:        BRK:imp    BREAK
CDFF 00:        BRK:imp    BREAK
CE00 70:1F      BVS:rel    Branch->$CE21
CE02 00:        BRK:imp    BREAK
CE03 71:00      ADC:ind,y  00
CE05 58:        CLI:imp    Enable IRQ
CE06 C1:68      CMP:ind,x  68
CE08 3F:        Illegal Opcode
CE09 A9:3F      LDA:imm    #3F
CE0B A9:3F      LDA:imm    #3F
CE0D A9:3F      LDA:imm    #3F
CE0F A9:3F      LDA:imm    #3F
CE11 A9:3F      LDA:imm    #3F
CE13 A9:30      LDA:imm    #30
CE15 00:        BRK:imp    BREAK
CE16 D0:1F      BNE:rel    Branch->$CE37
CE18 C5:68      CMP:zp     Zp RAM 0068
CE1A B4:A8      LDY:zp,x   Zp RAM 00A8
CE1C B4:A8      LDY:zp,x   Zp RAM 00A8
CE1E B4:A8      LDY:zp,x   Zp RAM 00A8
CE20 B4:A8      LDY:zp,x   Zp RAM 00A8
CE22 B4:A8      LDY:zp,x   Zp RAM 00A8
CE24 B4:A8      LDY:zp,x   Zp RAM 00A8
CE26 DC:        Illegal Opcode
CE27 1F:        Illegal Opcode
CE28 00:        BRK:imp    BREAK
CE29 00:        BRK:imp    BREAK
CE2A C7:        Illegal Opcode
CE2B 68:        PLA:imp    Pull Accum
CE2C B4:A8      LDY:zp,x   Zp RAM 00A8
CE2E B4:A8      LDY:zp,x   Zp RAM 00A8
CE30 C5:68      CMP:zp     Zp RAM 0068
CE32 24:00      BIT:zp     Zp RAM 0000
CE34 E8:        INX:imp    X=X+1
CE35 1F:        Illegal Opcode
CE36 B4:A8      LDY:zp,x   Zp RAM 00A8
CE38 B4:A8      LDY:zp,x   Zp RAM 00A8
CE3A B4:A8      LDY:zp,x   Zp RAM 00A8
CE3C 00:        BRK:imp    BREAK
CE3D 71:E0      ADC:ind,y  E0
CE3F 1F:        Illegal Opcode
CE40 28:        PLP:imp    Pull P
CE41 00:        BRK:imp    BREAK
CE42 00:        BRK:imp    BREAK
CE43 71:B4      ADC:ind,y  B4
CE45 A8:        TAY:imp    A-->Y
CE46 B4:A8      LDY:zp,x   Zp RAM 00A8
CE48 B4:A8      LDY:zp,x   Zp RAM 00A8
CE4A B4:A8      LDY:zp,x   Zp RAM 00A8
CE4C B4:A8      LDY:zp,x   Zp RAM 00A8
CE4E 65:A8      ADC:zp     Zp RAM 00A8
CE50 00:        BRK:imp    BREAK
CE51 00:        BRK:imp    BREAK
CE52 70:1F      BVS:rel    Branch->$CE73
CE54 00:        BRK:imp    BREAK
CE55 71:00      ADC:ind,y  00
CE57 58:        CLI:imp    Enable IRQ
CE58 C1:68      CMP:ind,x  68
CE5A 3F:        Illegal Opcode
CE5B A9:3F      LDA:imm    #3F
CE5D A9:3F      LDA:imm    #3F
CE5F A9:3F      LDA:imm    #3F
CE61 A9:3F      LDA:imm    #3F
CE63 A9:3F      LDA:imm    #3F
CE65 A9:55      LDA:imm    #55
CE67 7F:        Illegal Opcode
CE68 06:20      ASL:zp     Zp RAM 0020
CE6A 02:        Illegal Opcode
CE6B 22:        Illegal Opcode
CE6C 0C:        Illegal Opcode
CE6D 24:92      BIT:zp     Zp RAM 0092
CE6F 26:00      ROL:zp     Zp RAM 0000
CE71 29:56      AND:imm    #56
CE73 2A:        ROL:accum  
CE74 D8:        CLD:imp    Unset Decimal
CE75 2C:BE 2D   BIT:abs    $2DBE
CE78 24:2E      BIT:zp     Zp RAM 002E
CE7A 04:        Illegal Opcode
CE7B 21:06      AND:ind,x  06
CE7D 23:        Illegal Opcode
CE7E 4E:25 C8   LSR:abs    $C825
CE81 27:        Illegal Opcode
CE82 AA:        TAX:imp    A-->X
CE83 29:96      AND:imm    #96
CE85 2B:        Illegal Opcode
CE86 4A:        LSR:accum  
CE87 2D:F0 2D   AND:abs    $2DF0
CE8A A6:2E      LDX:zp     Zp RAM 002E
CE8C 04:        Illegal Opcode
CE8D 20:00 22   JSR:abs    $2200
CE90 0A:        ASL:accum  
CE91 24:90      BIT:zp     Zp RAM 0090
CE93 26:FE      ROL:zp     Zp RAM 00FE
CE95 28:        PLP:imp    Pull P
CE96 54:        Illegal Opcode
CE97 2A:        ROL:accum  
CE98 D6:2C      DEC:zp,x   Zp RAM 002C
CE9A BC:2D 22   LDY:abs,x  $222D,X
CE9D 2E:03 E0   ROL:abs    $E003
CEA0 01:E1      ORA:ind,x  E1
CEA2 06:E2      ASL:zp     Zp RAM 00E2
CEA4 49:E3      EOR:imm    #E3
CEA6 80:        Illegal Opcode
CEA7 E4:2B      CPX:zp     Zp RAM 002B
CEA9 E5:6C      SBC:zp     Zp RAM 006C
CEAB E6:DF      INC:zp     Zp RAM 00DF
CEAD E6:12      INC:zp     Zp RAM 0012
CEAF E7:        Illegal Opcode
CEB0 82:        Illegal Opcode
CEB1 E0:83      CPX:imm    #83
CEB3 E1:A7      SBC:ind,x  A7
CEB5 E2:        Illegal Opcode
CEB6 E4:E3      CPX:zp     Zp RAM 00E3
CEB8 D5:E4      CMP:zp,x   Zp RAM 00E4
CEBA CB:        Illegal Opcode
CEBB E5:A5      SBC:zp     Zp RAM 00A5
CEBD E6:F8      INC:zp     Zp RAM 00F8
CEBF E6:53      INC:zp     Zp RAM 0053
CEC1 E7:        Illegal Opcode
CEC2 DA:        Illegal Opcode
CEC3 EE:E4 EE   INC:abs    $EEE4         ; Data loaded into Vector RAM?
CEC6 E6:EE      INC:zp     Zp RAM 00EE
CEC8 61:AA      ADC:ind,x  AA
CECA 7C:        Illegal Opcode
CECB AA:        TAX:imp    A-->X
CECC 91:AA      STA:ind,y  AA
CECE AD:AA CA   LDA:abs    $CAAA
CED1 AA:        TAX:imp    A-->X
CED2 14:        Illegal Opcode
CED3 AB:        Illegal Opcode
CED4 6F:        Illegal Opcode
CED5 AB:        Illegal Opcode
CED6 C0:AB      CPY:imm    #AB
CED8 15:AC      ORA:zp,x   Zp RAM 00AC
CEDA 66:AC      ROR:zp     Zp RAM 00AC
CEDC 7D:AC 94   ADC:abs,x  $94AC,X
CEDF AC:AB AC   LDY:abs    $ACAB
CEE2 D8:        CLD:imp    Unset Decimal
CEE3 AC:FA AC   LDY:abs    $ACFA
CEE6 0D:AD 20   ORA:abs    $20AD
CEE9 AD:39 AD   LDA:abs    $AD39
CEEC 51:AD      EOR:ind,y  AD
CEEE 6A:        ROT:accum  
CEEF AD:8C AD   LDA:abs    $AD8C
CEF2 8A:        TXA:imp    X-->A
CEF3 AD:88 AD   LDA:abs    $AD88
CEF6 86:AD      STX:zp     Zp RAM 00AD
CEF8 84:AD      STY:zp     Zp RAM 00AD
CEFA 82:        Illegal Opcode
CEFB AD:86 AD   LDA:abs    $AD86
CEFE 8A:        TXA:imp    X-->A
CEFF AD:8C AD   LDA:abs    $AD8C
CF02 D7:        Illegal Opcode
CF03 AD:C2 AD   LDA:abs    $ADC2
CF06 C5:AD      CMP:zp     Zp RAM 00AD
CF08 C8:        INY:imp    Y=Y+1
CF09 AD:CB AD   LDA:abs    $ADCB
CF0C CE:AD D1   DEC:abs    $D1AD
CF0F AD:D4 AD   LDA:abs    $ADD4
CF12 C2:        Illegal Opcode
CF13 AC:CB AC   LDY:abs    $ACCB
CF16 35:AE      AND:zp,x   Zp RAM 00AE
CF18 59:AE 7E   EOR:abs,y  $7EAE,Y
CF1B AE:A2 AE   LDX:abs    $AEA2
CF1E C5:AE      CMP:zp     Zp RAM 00AE
CF20 CB:        Illegal Opcode
CF21 AE:D2 AE   LDX:abs    $AED2
CF24 A2:02      LDX:imm    #02
CF26 AD:08 00   LDA:abs    $0008
CF29 E0:01      CPX:imm    #01
CF2B F0:03      BEQ:rel    Branch->$CF30
CF2D B0:02      BCS:rel    Branch->$CF31
CF2F 4A:        LSR:accum  
CF30 4A:        LSR:accum  
CF31 4A:        LSR:accum  
CF32 B5:0D      LDA:zp,x   Zp RAM 000D
CF34 29:1F      AND:imm    #1F
CF36 B0:37      BCS:rel    Branch->$CF6F
CF38 F0:10      BEQ:rel    Branch->$CF4A
CF3A C9:1B      CMP:imm    #1B
CF3C B0:0A      BCS:rel    Branch->$CF48
CF3E A8:        TAY:imp    A-->Y
CF3F A5:07      LDA:zp     Zp RAM 0007
CF41 29:07      AND:imm    #07
CF43 C9:07      CMP:imm    #07
CF45 98:        TYA:imp    Y-->A
CF46 90:02      BCC:rel    Branch->$CF4A
CF48 E9:01      SBC:imm    #01
CF4A 95:0D      STA:zp,x   Zp RAM 000D
CF4C AD:08 00   LDA:abs    $0008         ; If the SLAM switch is hit, then
CF4F 29:08      AND:imm    #08           ; skip a little bit.
CF51 D0:04      BNE:rel    Branch->$CF57
CF53 A9:F0      LDA:imm    #F0
CF55 85:0C      STA:zp     Zp RAM 000C
CF57 A5:0C      LDA:zp     Zp RAM 000C
CF59 F0:08      BEQ:rel    Branch->$CF63
CF5B C6:0C      DEC:zp     Zp RAM 000C
CF5D A9:00      LDA:imm    #00
CF5F 95:0D      STA:zp,x   Zp RAM 000D
CF61 95:10      STA:zp,x   Zp RAM 0010
CF63 18:        CLC:imp    Clear Carry
CF64 B5:10      LDA:zp,x   Zp RAM 0010
CF66 F0:23      BEQ:rel    Branch->$CF8B
CF68 D6:10      DEC:zp,x   Zp RAM 0010
CF6A D0:1F      BNE:rel    Branch->$CF8B
CF6C 38:        SEC:imp    Set Carry
CF6D B0:1C      BCS:rel    Branch->$CF8B
CF6F C9:1B      CMP:imm    #1B
CF71 B0:09      BCS:rel    Branch->$CF7C
CF73 B5:0D      LDA:zp,x   Zp RAM 000D
CF75 69:20      ADC:imm    #20
CF77 90:D1      BCC:rel    Branch->$CF4A
CF79 F0:01      BEQ:rel    Branch->$CF7C
CF7B 18:        CLC:imp    Clear Carry
CF7C A9:1F      LDA:imm    #1F
CF7E B0:CA      BCS:rel    Branch->$CF4A
CF80 95:0D      STA:zp,x   Zp RAM 000D
CF82 B5:10      LDA:zp,x   Zp RAM 0010
CF84 F0:01      BEQ:rel    Branch->$CF87
CF86 38:        SEC:imp    Set Carry
CF87 A9:78      LDA:imm    #78
CF89 95:10      STA:zp,x   Zp RAM 0010
CF8B 90:2A      BCC:rel    Branch->$CFB7
CF8D A9:00      LDA:imm    #00
CF8F E0:01      CPX:imm    #01
CF91 90:16      BCC:rel    Branch->$CFA9
CF93 F0:0C      BEQ:rel    Branch->$CFA1
CF95 A5:09      LDA:zp     Zp RAM 0009
CF97 29:0C      AND:imm    #0C
CF99 4A:        LSR:accum  
CF9A 4A:        LSR:accum  
CF9B F0:0C      BEQ:rel    Branch->$CFA9
CF9D 69:02      ADC:imm    #02
CF9F D0:08      BNE:rel    Branch->$CFA9
CFA1 A5:09      LDA:zp     Zp RAM 0009
CFA3 29:10      AND:imm    #10
CFA5 F0:02      BEQ:rel    Branch->$CFA9
CFA7 A9:01      LDA:imm    #01
CFA9 38:        SEC:imp    Set Carry
CFAA 48:        PHA:imp    Push Accum
CFAB 65:16      ADC:zp     Zp RAM 0016
CFAD 85:16      STA:zp     Zp RAM 0016
CFAF 68:        PLA:imp    Pull Accum
CFB0 38:        SEC:imp    Set Carry
CFB1 65:17      ADC:zp     Zp RAM 0017
CFB3 85:17      STA:zp     Zp RAM 0017
CFB5 F6:13      INC:zp,x   Zp RAM 0013
CFB7 CA:        DEX:imp    X=X-1
CFB8 30:03      BMI:rel    Branch->$CFBD
CFBA 4C:26 CF   JMP:abs    $CF26
CFBD A5:09      LDA:zp     Zp RAM 0009
CFBF 4A:        LSR:accum  
CFC0 4A:        LSR:accum  
CFC1 4A:        LSR:accum  
CFC2 4A:        LSR:accum  
CFC3 4A:        LSR:accum  
CFC4 A8:        TAY:imp    A-->Y
CFC5 A5:16      LDA:zp     Zp RAM 0016
CFC7 38:        SEC:imp    Set Carry
CFC8 F9:D9 CF   SBC:abs,y  $CFD9,Y
CFCB 30:14      BMI:rel    Branch->$CFE1
CFCD 85:16      STA:zp     Zp RAM 0016
CFCF E6:18      INC:zp     Zp RAM 0018
CFD1 C0:03      CPY:imm    #03
CFD3 D0:0C      BNE:rel    Branch->$CFE1
CFD5 E6:18      INC:zp     Zp RAM 0018
CFD7 D0:08      BNE:rel    Branch->$CFE1
CFD9 7F:        Illegal Opcode
CFDA 02:        Illegal Opcode
CFDB 04:        Illegal Opcode
CFDC 04:        Illegal Opcode
CFDD 05:03      ORA:zp     Zp RAM 0003
CFDF 7F:        Illegal Opcode
CFE0 7F:        Illegal Opcode
CFE1 A5:09      LDA:zp     Zp RAM 0009
CFE3 29:03      AND:imm    #03
CFE5 A8:        TAY:imp    A-->Y
CFE6 F0:1A      BEQ:rel    Branch->$D002
CFE8 4A:        LSR:accum  
CFE9 69:00      ADC:imm    #00
CFEB 49:FF      EOR:imm    #FF
CFED 38:        SEC:imp    Set Carry
CFEE 65:17      ADC:zp     Zp RAM 0017
CFF0 B0:08      BCS:rel    Branch->$CFFA
CFF2 65:18      ADC:zp     Zp RAM 0018
CFF4 30:0E      BMI:rel    Branch->$D004
CFF6 85:18      STA:zp     Zp RAM 0018
CFF8 A9:00      LDA:imm    #00
CFFA C0:02      CPY:imm    #02
CFFC B0:02      BCS:rel    Branch->$D000
CFFE E6:06      INC:zp     Zp RAM 0006	 ; Increase the number of game
;
; Start of ROM 136002.121 at $D000.
;
D000 E6:06      INC:zp     Zp RAM 0006	 ; credits by two.
D002 85:17      STA:zp     Zp RAM 0017
D004 A5:07      LDA:zp     Zp RAM 0007
D006 4A:        LSR:accum  
D007 B0:27      BCS:rel    Branch->$D030
D009 A0:00      LDY:imm    #00
D00B A2:02      LDX:imm    #02
D00D B5:13      LDA:zp,x   Zp RAM 0013
D00F F0:09      BEQ:rel    Branch->$D01A
D011 C9:10      CMP:imm    #10
D013 90:05      BCC:rel    Branch->$D01A
D015 69:EF      ADC:imm    #EF
D017 C8:        INY:imp    Y=Y+1
D018 95:13      STA:zp,x   Zp RAM 0013
D01A CA:        DEX:imp    X=X-1
D01B 10:F0      BPL:rel    Branch->$D00D
D01D 98:        TYA:imp    Y-->A
D01E D0:10      BNE:rel    Branch->$D030
D020 A2:02      LDX:imm    #02
D022 B5:13      LDA:zp,x   Zp RAM 0013
D024 F0:07      BEQ:rel    Branch->$D02D
D026 18:        CLC:imp    Clear Carry
D027 69:EF      ADC:imm    #EF
D029 95:13      STA:zp,x   Zp RAM 0013
D02B 30:03      BMI:rel    Branch->$D030
D02D CA:        DEX:imp    X=X-1
D02E 10:F2      BPL:rel    Branch->$D022
D030 60:        RTS:imp    Ret from Sub
D031 5D:D1 8F   EOR:abs,x  $8FD1,X
D034 D1:8F      CMP:ind,y  8F
D036 D1:B1      CMP:ind,y  B1
D038 D1:EB      CMP:ind,y  EB
D03A D1:03      CMP:ind,y  03
D03C D2:        Illegal Opcode
D03D 61:D2      ADC:ind,x  D2
D03F CB:        Illegal Opcode
D040 D2:        Illegal Opcode
D041 33:        Illegal Opcode
D042 D3:        Illegal Opcode
D043 66:D3      ROR:zp     Zp RAM 00D3
D045 B0:D3      BCS:rel    Branch->$D01A
D047 E6:D3      INC:zp     Zp RAM 00D3
D049 FF:        Illegal Opcode
D04A D3:        Illegal Opcode
D04B 17:        Illegal Opcode
D04C D4:        Illegal Opcode
D04D 1D:D4 34   ORA:abs,x  $34D4,X
D050 D4:        Illegal Opcode
D051 4C:D4 60   JMP:abs    $60D4
D054 D4:        Illegal Opcode
D055 A1:D4      LDA:ind,x  D4
D057 AB:        Illegal Opcode
D058 D4:        Illegal Opcode
D059 EF:        Illegal Opcode
D05A D4:        Illegal Opcode
D05B 30:D5      BMI:rel    Branch->$D032
D05D 75:D5      ADC:zp,x   Zp RAM 00D5
D05F 85:D5      STA:zp     Zp RAM 00D5
D061 A1:D5      LDA:ind,x  D5
D063 A8:        TAY:imp    A-->Y
D064 D5:E9      CMP:zp,x   Zp RAM 00E9
D066 D5:1C      CMP:zp,x   Zp RAM 001C
D068 D6:62      DEC:zp,x   Zp RAM 0062
D06A D6:7A      DEC:zp,x   Zp RAM 007A
D06C D6:67      DEC:zp,x   Zp RAM 0067
D06E D1:97      CMP:ind,y  97
D070 D1:97      CMP:ind,y  97
D072 D1:BD      CMP:ind,y  BD
D074 D1:F0      CMP:ind,y  F0
D076 D1:17      CMP:ind,y  17
D078 D2:        Illegal Opcode
D079 75:D2      ADC:zp,x   Zp RAM 00D2
D07B E0:D2      CPX:imm    #D2
D07D 3F:        Illegal Opcode
D07E D3:        Illegal Opcode
D07F 79:D3 BE   ADC:abs,y  $BED3,Y
D082 D3:        Illegal Opcode
D083 E6:D3      INC:zp     Zp RAM 00D3
D085 FF:        Illegal Opcode
D086 D3:        Illegal Opcode
D087 17:        Illegal Opcode
D088 D4:        Illegal Opcode
D089 22:        Illegal Opcode
D08A D4:        Illegal Opcode
D08B 3A:        Illegal Opcode
D08C D4:        Illegal Opcode
D08D 51:D4      EOR:ind,y  D4
D08F 6D:D4 A1   ADC:abs    $A1D4
D092 D4:        Illegal Opcode
D093 BA:        TSX:imp    Stack-->X
D094 D4:        Illegal Opcode
D095 FD:D4 3F   SBC:abs,x  $3FD4,X
D098 D5:75      CMP:zp,x   Zp RAM 0075
D09A D5:85      CMP:zp,x   Zp RAM 0085
D09C D5:A1      CMP:zp,x   Zp RAM 00A1
D09E D5:B9      CMP:zp,x   Zp RAM 00B9
D0A0 D5:F6      CMP:zp,x   Zp RAM 00F6
D0A2 D5:29      CMP:zp,x   Zp RAM 0029
D0A4 D6:68      DEC:zp,x   Zp RAM 0068
D0A6 D6:7A      DEC:zp,x   Zp RAM 007A
D0A8 D6:75      DEC:zp,x   Zp RAM 0075
D0AA D1:9F      CMP:ind,y  9F
D0AC D1:9F      CMP:ind,y  9F
D0AE D1:CF      CMP:ind,y  CF
D0B0 D1:F6      CMP:ind,y  F6
D0B2 D1:30      CMP:ind,y  30
D0B4 D2:        Illegal Opcode
D0B5 94:D2      STY:zp,x   Zp RAM 00D2
D0B7 FB:        Illegal Opcode
D0B8 D2:        Illegal Opcode
D0B9 50:D3      BVC:rel    Branch->$D08E
D0BB 8B:        Illegal Opcode
D0BC D3:        Illegal Opcode
D0BD CB:        Illegal Opcode
D0BE D3:        Illegal Opcode
D0BF F5:D3      SBC:zp,x   Zp RAM 00D3
D0C1 0E:D4 17   ASL:abs    $17D4
D0C4 D4:        Illegal Opcode
D0C5 28:        PLP:imp    Pull P
D0C6 D4:        Illegal Opcode
D0C7 41:D4      EOR:ind,x  D4
D0C9 5B:        Illegal Opcode
D0CA D4:        Illegal Opcode
D0CB 83:        Illegal Opcode
D0CC D4:        Illegal Opcode
D0CD A1:D4      LDA:ind,x  D4
D0CF CC:D4 0E   CPY:abs    $0ED4
D0D2 D5:51      CMP:zp,x   Zp RAM 0051
D0D4 D5:75      CMP:zp,x   Zp RAM 0075
D0D6 D5:8E      CMP:zp,x   Zp RAM 008E
D0D8 D5:A1      CMP:zp,x   Zp RAM 00A1
D0DA D5:C8      CMP:zp,x   Zp RAM 00C8
D0DC D5:04      CMP:zp,x   Zp RAM 0004
D0DE D6:3E      DEC:zp,x   Zp RAM 003E
D0E0 D6:6F      DEC:zp,x   Zp RAM 006F
D0E2 D6:8F      DEC:zp,x   Zp RAM 008F
D0E4 D6:7F      DEC:zp,x   Zp RAM 007F
D0E6 D1:A8      CMP:ind,y  A8
D0E8 D1:A8      CMP:ind,y  A8
D0EA D1:DE      CMP:ind,y  DE
D0EC D1:FC      CMP:ind,y  FC
D0EE D1:4D      CMP:ind,y  4D
D0F0 D2:        Illegal Opcode
D0F1 AE:D2 16   LDX:abs    $16D2
D0F4 D3:        Illegal Opcode
D0F5 5E:D3 A0   LSR:abs,x  $A0D3,X
D0F8 D3:        Illegal Opcode
D0F9 DA:        Illegal Opcode
D0FA D3:        Illegal Opcode
D0FB ED:D3 06   SBC:abs    $06D3
D0FE D4:        Illegal Opcode
D0FF 17:        Illegal Opcode
D100 D4:        Illegal Opcode
D101 2D:D4 46   AND:abs    $46D4
D104 D4:        Illegal Opcode
D105 56:D4      LSR:zp,x   Zp RAM 00D4
D107 92:        Illegal Opcode
D108 D4:        Illegal Opcode
D109 A1:D4      LDA:ind,x  D4
D10B DD:D4 1F   CMP:abs,x  $1FD4,X
D10E D5:63      CMP:zp,x   Zp RAM 0063
D110 D5:75      CMP:zp,x   Zp RAM 0075
D112 D5:97      CMP:zp,x   Zp RAM 0097
D114 D5:A1      CMP:zp,x   Zp RAM 00A1
D116 D5:D9      CMP:zp,x   Zp RAM 00D9
D118 D5:10      CMP:zp,x   Zp RAM 0010
D11A D6:51      DEC:zp,x   Zp RAM 0051
D11C D6:74      DEC:zp,x   Zp RAM 0074
D11E D6:A1      DEC:zp,x   Zp RAM 00A1
D120 D6:51      DEC:zp,x   Zp RAM 0051
D122 56:00      LSR:zp,x   Zp RAM 0000
D124 1A:        Illegal Opcode
D125 01:20      ORA:ind,x  20
D127 31:56      AND:ind,y  56
D129 01:38      ORA:ind,x  38
D12B 31:B0      AND:ind,y  B0
D12D 41:00      EOR:ind,x  00
D12F 11:F6      ORA:ind,y  F6
D131 30:38      BMI:rel    Branch->$D16B
D133 31:CE      AND:ind,y  CE
D135 51:0A      EOR:ind,y  0A
D137 31:E2      AND:ind,y  E2
D139 31:E2      AND:ind,y  E2
D13B 51:BA      EOR:ind,y  BA
D13D 51:98      EOR:ind,y  98
D13F 51:D8      EOR:ind,y  D8
D141 51:C9      EOR:ind,y  C9
D143 31:56      AND:ind,y  56
D145 51:80      EOR:ind,y  80
D147 51:80      EOR:ind,y  80
D149 51:80      EOR:ind,y  80
D14B 51:80      EOR:ind,y  80
D14D 71:92      ADC:ind,y  92
D14F 51:80      EOR:ind,y  80
D151 31:B0      AND:ind,y  B0
D153 51:89      EOR:ind,y  89
D155 41:89      EOR:ind,x  89
D157 00:        BRK:imp    BREAK
D158 00:        BRK:imp    BREAK
D159 71:5A      ADC:ind,y  5A
D15B 71:A0      ADC:ind,y  A0
D15D E5:22      SBC:zp     Zp RAM 0022
D15F 16:2E      ASL:zp,x   Zp RAM 002E
D161 1E:00 32   ASL:abs,x  $3200,X
D164 40:        RTI:imp    Ret from Int
D165 1E:B8 D9   ASL:abs,x  $D9B8,X
D168 20:26 30   JSR:abs    $3026
D16B 00:        BRK:imp    BREAK
D16C 1C:        Illegal Opcode
D16D 1E:00 34   ASL:abs,x  $3400,X
D170 16:38      ASL:zp,x   Zp RAM 0038
D172 3C:        Illegal Opcode
D173 26:9E      ROL:zp     Zp RAM 009E
D175 E5:3A      SBC:zp     Zp RAM 003A
D177 34:        Illegal Opcode
D178 26:1E      ROL:zp     Zp RAM 001E
D17A 2C:1E 30   BIT:abs    $301E
D17D 1C:        Illegal Opcode
D17E 9E:        Illegal Opcode
D17F D3:        Illegal Opcode
D180 28:        PLP:imp    Pull P
D181 3E:1E 22   ROL:abs,x  $221E,X
D184 32:        Illegal Opcode
D185 00:        BRK:imp    BREAK
D186 3C:        Illegal Opcode
D187 1E:38 2E   ASL:abs,x  $2E38,X
D18A 26:30      ROL:zp     Zp RAM 0030
D18C 16:1C      ASL:zp,x   Zp RAM 001C
D18E B2:        Illegal Opcode
D18F CD:34 2C   CMP:abs    $2C34
D192 16:46      ASL:zp,x   Zp RAM 0046
D194 1E:38 80   ASL:abs,x  $8038,X
D197 C6:28      DEC:zp     Zp RAM 0028
D199 32:        Illegal Opcode
D19A 3E:1E 3E   ROL:abs,x  $3E1E,X
D19D 38:        SEC:imp    Set Carry
D19E 80:        Illegal Opcode
D19F C6:3A      DEC:zp     Zp RAM 003A
D1A1 34:        Illegal Opcode
D1A2 26:1E      ROL:zp     Zp RAM 001E
D1A4 2C:1E 38   BIT:abs    $381E
D1A7 80:        Illegal Opcode
D1A8 C6:28      DEC:zp     Zp RAM 0028
D1AA 3E:22 16   ROL:abs,x  $1622,X
D1AD 1C:        Illegal Opcode
D1AE 32:        Illegal Opcode
D1AF 38:        SEC:imp    Set Carry
D1B0 80:        Illegal Opcode
D1B1 DF:        Illegal Opcode
D1B2 34:        Illegal Opcode
D1B3 38:        SEC:imp    Set Carry
D1B4 1E:3A 3A   ASL:abs,x  $3A3A,X
D1B7 00:        BRK:imp    BREAK
D1B8 3A:        Illegal Opcode
D1B9 3C:        Illegal Opcode
D1BA 16:38      ASL:zp,x   Zp RAM 0038
D1BC BC:CD 16   LDY:abs,x  $16CD,X
D1BF 34:        Illegal Opcode
D1C0 34:        Illegal Opcode
D1C1 3E:46 1E   ROL:abs,x  $1E46,X
D1C4 48:        PHA:imp    Push Accum
D1C5 00:        BRK:imp    BREAK
D1C6 3A:        Illegal Opcode
D1C7 3E:38 00   ROL:abs,x  $0038,X
D1CA 3A:        Illegal Opcode
D1CB 3C:        Illegal Opcode
D1CC 16:38      ASL:zp,x   Zp RAM 0038
D1CE BC:D6 3A   LDY:abs,x  $3AD6,X
D1D1 3C:        Illegal Opcode
D1D2 16:38      ASL:zp,x   Zp RAM 0038
D1D4 3C:        Illegal Opcode
D1D5 00:        BRK:imp    BREAK
D1D6 1C:        Illegal Opcode
D1D7 38:        SEC:imp    Set Carry
D1D8 3E:1E 1A   ROL:abs,x  $1A1E,X
D1DB 2A:        ROL:accum  
D1DC 1E:B0 DC   ASL:abs,x  $DCB0,X
D1DF 34:        Illegal Opcode
D1E0 3E:2C 3A   ROL:abs,x  $3A2C,X
D1E3 16:38      ASL:zp,x   Zp RAM 0038
D1E5 00:        BRK:imp    BREAK
D1E6 3A:        Illegal Opcode
D1E7 3C:        Illegal Opcode
D1E8 16:38      ASL:zp,x   Zp RAM 0038
D1EA BC:F4 34   LDY:abs,x  $34F4,X
D1ED 2C:16 C6   BIT:abs    $C616
D1F0 F1:28      SBC:ind,y  28
D1F2 32:        Illegal Opcode
D1F3 3E:1E C8   ROL:abs,x  $C81E,X
D1F6 F1:3A      SBC:ind,y  3A
D1F8 34:        Illegal Opcode
D1F9 26:1E      ROL:zp     Zp RAM 001E
D1FB AC:EE 28   LDY:abs    $28EE
D1FE 3E:1E 22   ROL:abs,x  $221E,X
D201 3E:9E C7   ROL:abs,x  $C79E,X
D204 1E:30 3C   ASL:abs,x  $3C30,X
D207 1E:38 00   ASL:abs,x  $0038,X
D20A 46:32      LSR:zp     Zp RAM 0032
D20C 3E:38 00   ROL:abs,x  $0038,X
D20F 26:30      ROL:zp     Zp RAM 0030
D211 26:3C      ROL:zp     Zp RAM 003C
D213 26:16      ROL:zp     Zp RAM 0016
D215 2C:BA B8   BIT:abs    $B8BA
D218 3A:        Illegal Opcode
D219 40:        RTI:imp    Ret from Int
D21A 34:        Illegal Opcode
D21B 00:        BRK:imp    BREAK
D21C 1E:30 3C   ASL:abs,x  $3C30,X
D21F 38:        SEC:imp    Set Carry
D220 1E:48 00   ASL:abs,x  $0048,X
D223 40:        RTI:imp    Ret from Int
D224 32:        Illegal Opcode
D225 3A:        Illegal Opcode
D226 00:        BRK:imp    BREAK
D227 26:30      ROL:zp     Zp RAM 0030
D229 26:3C      ROL:zp     Zp RAM 003C
D22B 26:16      ROL:zp     Zp RAM 0016
D22D 2C:1E BA   BIT:abs    $BA1E
D230 AC:22 1E   LDY:abs    $1E22
D233 18:        CLC:imp    Clear Carry
D234 1E:30 00   ASL:abs,x  $0030,X
D237 3A:        Illegal Opcode
D238 26:1E      ROL:zp     Zp RAM 001E
D23A 00:        BRK:imp    BREAK
D23B 26:24      ROL:zp     Zp RAM 0024
D23D 38:        SEC:imp    Set Carry
D23E 1E:00 26   ASL:abs,x  $2600,X
D241 30:26      BMI:rel    Branch->$D269
D243 3C:        Illegal Opcode
D244 26:16      ROL:zp     Zp RAM 0016
D246 2C:1E 30   BIT:abs    $301E
D249 00:        BRK:imp    BREAK
D24A 1E:26 B0   ASL:abs,x  $B026,X
D24D C7:        Illegal Opcode
D24E 1E:30 3C   ASL:abs,x  $3C30,X
D251 38:        SEC:imp    Set Carry
D252 1E:00 3A   ASL:abs,x  $3A00,X
D255 3E:3A 00   ROL:abs,x  $003A,X
D258 26:30      ROL:zp     Zp RAM 0030
D25A 26:1A      ROL:zp     Zp RAM 001A
D25C 26:16      ROL:zp     Zp RAM 0016
D25E 2C:1E BA   BIT:abs    $BA1E
D261 C7:        Illegal Opcode
D262 3A:        Illegal Opcode
D263 34:        Illegal Opcode
D264 26:30      ROL:zp     Zp RAM 0030
D266 00:        BRK:imp    BREAK
D267 2A:        ROL:accum  
D268 30:32      BMI:rel    Branch->$D29C
D26A 18:        CLC:imp    Clear Carry
D26B 00:        BRK:imp    BREAK
D26C 3C:        Illegal Opcode
D26D 32:        Illegal Opcode
D26E 00:        BRK:imp    BREAK
D26F 1A:        Illegal Opcode
D270 24:16      BIT:zp     Zp RAM 0016
D272 30:22      BMI:rel    Branch->$D296
D274 9E:        Illegal Opcode
D275 A6:3C      LDX:zp     Zp RAM 003C
D277 32:        Illegal Opcode
D278 3E:38 30   ROL:abs,x  $3038,X
D27B 1E:48 00   ASL:abs,x  $0048,X
D27E 2C:1E 00   BIT:abs    $001E
D281 18:        CLC:imp    Clear Carry
D282 32:        Illegal Opcode
D283 3E:3C 32   ROL:abs,x  $323C,X
D286 30:00      BMI:rel    Branch->$D288
D288 34:        Illegal Opcode
D289 32:        Illegal Opcode
D28A 3E:38 00   ROL:abs,x  $0038,X
D28D 1A:        Illegal Opcode
D28E 24:16      BIT:zp     Zp RAM 0016
D290 30:22      BMI:rel    Branch->$D2B4
D292 1E:B8 B5   ASL:abs,x  $B5B8,X
D295 2A:        ROL:accum  
D296 30:32      BMI:rel    Branch->$D2CA
D298 34:        Illegal Opcode
D299 20:00 1C   JSR:abs    $1C00
D29C 38:        SEC:imp    Set Carry
D29D 1E:24 1E   ASL:abs,x  $1E24,X
D2A0 30:00      BMI:rel    Branch->$D2A2
D2A2 48:        PHA:imp    Push Accum
D2A3 3E:2E 00   ROL:abs,x  $002E,X
D2A6 42:        Illegal Opcode
D2A7 1E:1A 24   ASL:abs,x  $241A,X
D2AA 3A:        Illegal Opcode
D2AB 1E:2C B0   ASL:abs,x  $B02C,X
D2AE AC:22 26   LDY:abs    $2622
D2B1 38:        SEC:imp    Set Carry
D2B2 1E:00 2C   ASL:abs,x  $2C00,X
D2B5 16:00      ASL:zp,x   Zp RAM 0000
D2B7 34:        Illegal Opcode
D2B8 1E:38 26   ASL:abs,x  $2638,X
D2BB 2C:2C 16   BIT:abs    $162C
D2BE 00:        BRK:imp    BREAK
D2BF 34:        Illegal Opcode
D2C0 16:38      ASL:zp,x   Zp RAM 0038
D2C2 16:00      ASL:zp,x   Zp RAM 0000
D2C4 1A:        Illegal Opcode
D2C5 16:2E      ASL:zp,x   Zp RAM 002E
D2C7 18:        CLC:imp    Clear Carry
D2C8 26:16      ROL:zp     Zp RAM 0016
D2CA B8:        CLV:imp    Clear Overflow
D2CB C4:34      CPY:zp     Zp RAM 0034
D2CD 38:        SEC:imp    Set Carry
D2CE 1E:3A 3A   ASL:abs,x  $3A3A,X
D2D1 00:        BRK:imp    BREAK
D2D2 20:26 38   JSR:abs    $3826
D2D5 1E:00 3C   ASL:abs,x  $3C00,X
D2D8 32:        Illegal Opcode
D2D9 00:        BRK:imp    BREAK
D2DA 3A:        Illegal Opcode
D2DB 1E:2C 1E   ASL:abs,x  $1E2C,X
D2DE 1A:        Illegal Opcode
D2DF BC:B2 34   LDY:abs,x  $34B2,X
D2E2 32:        Illegal Opcode
D2E3 3E:3A 3A   ROL:abs,x  $3A3A,X
D2E6 1E:48 00   ASL:abs,x  $0048,X
D2E9 20:1E 3E   JSR:abs    $3E1E
D2EC 00:        BRK:imp    BREAK
D2ED 36:3E      ROL:zp,x   Zp RAM 003E
D2EF 16:30      ASL:zp,x   Zp RAM 0030
D2F1 1C:        Illegal Opcode
D2F2 00:        BRK:imp    BREAK
D2F3 1A:        Illegal Opcode
D2F4 32:        Illegal Opcode
D2F5 38:        SEC:imp    Set Carry
D2F6 38:        SEC:imp    Set Carry
D2F7 1E:1A 3C   ASL:abs,x  $3C1A,X
D2FA 9E:        Illegal Opcode
D2FB B2:        Illegal Opcode
D2FC 20:26 38   JSR:abs    $3826
D2FF 1E:00 1C   ASL:abs,x  $1C00,X
D302 38:        SEC:imp    Set Carry
D303 3E:1E 1A   ROL:abs,x  $1A1E,X
D306 2A:        ROL:accum  
D307 1E:30 00   ASL:abs,x  $0030,X
D30A 42:        Illegal Opcode
D30B 1E:30 30   ASL:abs,x  $3030,X
D30E 00:        BRK:imp    BREAK
D30F 38:        SEC:imp    Set Carry
D310 26:1A      ROL:zp     Zp RAM 001A
D312 24:3C      BIT:zp     Zp RAM 003C
D314 26:A2      ROL:zp     Zp RAM 00A2
D316 AC:32 34   LDY:abs    $3432
D319 38:        SEC:imp    Set Carry
D31A 26:2E      ROL:zp     Zp RAM 002E
D31C 16:00      ASL:zp,x   Zp RAM 0000
D31E 20:26 38   JSR:abs    $3826
D321 1E:00 34   ASL:abs,x  $3400,X
D324 16:38      ASL:zp,x   Zp RAM 0038
D326 16:00      ASL:zp,x   Zp RAM 0000
D328 3A:        Illegal Opcode
D329 1E:2C 1E   ASL:abs,x  $1E2C,X
D32C 1A:        Illegal Opcode
D32D 1A:        Illegal Opcode
D32E 26:32      ROL:zp     Zp RAM 0032
D330 30:16      BMI:rel    Branch->$D348
D332 B8:        CLV:imp    Clear Overflow
D333 BC:24 26   LDY:abs,x  $2624,X
D336 22:        Illegal Opcode
D337 24:00      BIT:zp     Zp RAM 0000
D339 3A:        Illegal Opcode
D33A 1A:        Illegal Opcode
D33B 32:        Illegal Opcode
D33C 38:        SEC:imp    Set Carry
D33D 1E:BA 9E   ASL:abs,x  $9EBA,X
D340 2E:1E 26   ROL:abs    $261E
D343 2C:2C 1E   BIT:abs    $1E2C
D346 3E:38 3A   ROL:abs,x  $3A38,X
D349 00:        BRK:imp    BREAK
D34A 3A:        Illegal Opcode
D34B 1A:        Illegal Opcode
D34C 32:        Illegal Opcode
D34D 38:        SEC:imp    Set Carry
D34E 1E:BA B0   ASL:abs,x  $B0BA,X
D351 24:32      BIT:zp     Zp RAM 0032
D353 1E:1A 24   ASL:abs,x  $241A,X
D356 3A:        Illegal Opcode
D357 3C:        Illegal Opcode
D358 48:        PHA:imp    Push Accum
D359 16:24      ASL:zp,x   Zp RAM 0024
D35B 2C:1E B0   BIT:abs    $B01E
D35E D4:        Illegal Opcode
D35F 38:        SEC:imp    Set Carry
D360 1E:1A 32   ASL:abs,x  $321A,X
D363 38:        SEC:imp    Set Carry
D364 1C:        Illegal Opcode
D365 BA:        TSX:imp    Stack-->X
D366 C2:        Illegal Opcode
D367 38:        SEC:imp    Set Carry
D368 16:30      ASL:zp,x   Zp RAM 0030
D36A 2A:        ROL:accum  
D36B 26:30      ROL:zp     Zp RAM 0030
D36D 22:        Illegal Opcode
D36E 00:        BRK:imp    BREAK
D36F 20:38 32   JSR:abs    $3238
D372 2E:00 04   ROL:abs    $0400
D375 00:        BRK:imp    BREAK
D376 3C:        Illegal Opcode
D377 32:        Illegal Opcode
D378 80:        Illegal Opcode
D379 C2:        Illegal Opcode
D37A 34:        Illegal Opcode
D37B 2C:16 1A   BIT:abs    $1A16
D37E 1E:2E 1E   ASL:abs,x  $1E2E,X
D381 30:3C      BMI:rel    Branch->$D3BF
D383 00:        BRK:imp    BREAK
D384 1C:        Illegal Opcode
D385 1E:00 04   ASL:abs,x  $0400,X
D388 00:        BRK:imp    BREAK
D389 16:80      ASL:zp,x   Zp RAM 0080
D38B BC:38 16   LDY:abs,x  $1638,X
D38E 30:22      BMI:rel    Branch->$D3B2
D390 2C:26 3A   BIT:abs    $3A26
D393 3C:        Illegal Opcode
D394 1E:00 40   ASL:abs,x  $4000,X
D397 32:        Illegal Opcode
D398 30:00      BMI:rel    Branch->$D39A
D39A 04:        Illegal Opcode
D39B 00:        BRK:imp    BREAK
D39C 48:        PHA:imp    Push Accum
D39D 3E:2E 80   ROL:abs,x  $802E,X
D3A0 C8:        INY:imp    Y=Y+1
D3A1 38:        SEC:imp    Set Carry
D3A2 16:30      ASL:zp,x   Zp RAM 0030
D3A4 2A:        ROL:accum  
D3A5 26:30      ROL:zp     Zp RAM 0030
D3A7 22:        Illegal Opcode
D3A8 00:        BRK:imp    BREAK
D3A9 1C:        Illegal Opcode
D3AA 1E:00 04   ASL:abs,x  $0400,X
D3AD 00:        BRK:imp    BREAK
D3AE 16:80      ASL:zp,x   Zp RAM 0080
D3B0 D9:38 16   CMP:abs,y  $1638,Y
D3B3 3C:        Illegal Opcode
D3B4 1E:00 46   ASL:abs,x  $4600,X
D3B7 32:        Illegal Opcode
D3B8 3E:38 3A   ROL:abs,x  $3A38,X
D3BB 1E:2C A0   ASL:abs,x  $A02C,X
D3BE DC:        Illegal Opcode
D3BF 1E:40 16   ASL:abs,x  $1640,X
D3C2 2C:3E 1E   BIT:abs    $1E3E
D3C5 48:        PHA:imp    Push Accum
D3C6 4C:40 32   JMP:abs    $3240
D3C9 3E:BA D6   ROL:abs,x  $D6BA,X
D3CC 3A:        Illegal Opcode
D3CD 1E:2C 18   ASL:abs,x  $182C,X
D3D0 3A:        Illegal Opcode
D3D1 3C:        Illegal Opcode
D3D2 00:        BRK:imp    BREAK
D3D3 38:        SEC:imp    Set Carry
D3D4 1E:1A 24   ASL:abs,x  $241A,X
D3D7 30:1E      BMI:rel    Branch->$D3F7
D3D9 B0:DF      BCS:rel    Branch->$D3BA
D3DB 1A:        Illegal Opcode
D3DC 16:2C      ASL:zp,x   Zp RAM 002C
D3DE 26:20      ROL:zp     Zp RAM 0020
D3E0 26:36      ROL:zp     Zp RAM 0036
D3E2 3E:1E 3A   ROL:abs,x  $3A1E,X
D3E5 9E:        Illegal Opcode
D3E6 AA:        TAX:imp    A-->X
D3E7 30:32      BMI:rel    Branch->$D41B
D3E9 40:        RTI:imp    Ret from Int
D3EA 26:1A      ROL:zp     Zp RAM 001A
D3EC 9E:        Illegal Opcode
D3ED AA:        TAX:imp    A-->X
D3EE 30:32      BMI:rel    Branch->$D422
D3F0 40:        RTI:imp    Ret from Int
D3F1 26:1A      ROL:zp     Zp RAM 001A
D3F3 26:B2      ROL:zp     Zp RAM 00B2
D3F5 AA:        TAX:imp    A-->X
D3F6 16:30      ASL:zp,x   Zp RAM 0030
D3F8 20:16 1E   JSR:abs    $1E16
D3FB 30:22      BMI:rel    Branch->$D41F
D3FD 1E:B8 4A   ASL:abs,x  $4AB8,X
D400 1E:44 34   ASL:abs,x  $3444,X
D403 1E:38 BC   ASL:abs,x  $BC38,X
D406 45:1E      EOR:zp     Zp RAM 001E
D408 44:        Illegal Opcode
D409 34:        Illegal Opcode
D40A 1E:38 3C   ASL:abs,x  $3C38,X
D40D B2:        Illegal Opcode
D40E 40:        RTI:imp    Ret from Int
D40F 1E:38 20   ASL:abs,x  $2038,X
D412 16:24      ASL:zp,x   Zp RAM 0024
D414 38:        SEC:imp    Set Carry
D415 1E:B0 8B   ASL:abs,x  $8BB0,X
D418 18:        CLC:imp    Clear Carry
D419 32:        Illegal Opcode
D41A 30:3E      BMI:rel    Branch->$D45A
D41C BA:        TSX:imp    Stack-->X
D41D E8:        INX:imp    X=X+1
D41E 3C:        Illegal Opcode
D41F 26:2E      ROL:zp     Zp RAM 002E
D421 9E:        Illegal Opcode
D422 E0:1C      CPX:imm    #1C
D424 3E:38 1E   ROL:abs,x  $1E38,X
D427 9E:        Illegal Opcode
D428 E8:        INX:imp    X=X+1
D429 48:        PHA:imp    Push Accum
D42A 1E:26 BC   ASL:abs,x  $BC26,X
D42D E4:3C      CPX:zp     Zp RAM 003C
D42F 26:1E      ROL:zp     Zp RAM 001E
D431 2E:34 B2   ROL:abs    $B234
D434 8B:        Illegal Opcode
D435 2C:1E 40   BIT:abs    $401E
D438 1E:AC 8B   ASL:abs,x  $8BAC,X
D43B 30:26      BMI:rel    Branch->$D463
D43D 40:        RTI:imp    Ret from Int
D43E 1E:16 BE   ASL:abs,x  $BE16,X
D441 8B:        Illegal Opcode
D442 22:        Illegal Opcode
D443 38:        SEC:imp    Set Carry
D444 16:9C      ASL:zp,x   Zp RAM 009C
D446 8B:        Illegal Opcode
D447 30:26      BMI:rel    Branch->$D46F
D449 40:        RTI:imp    Ret from Int
D44A 1E:AC 8B   ASL:abs,x  $8BAC,X
D44D 24:32      BIT:zp     Zp RAM 0032
D44F 2C:9E 8B   BIT:abs    $8B9E
D452 3C:        Illegal Opcode
D453 38:        SEC:imp    Set Carry
D454 32:        Illegal Opcode
D455 BE:8B 24   LDX:abs,y  $248B,Y
D458 32:        Illegal Opcode
D459 46:B2      LSR:zp     Zp RAM 00B2
D45B 8B:        Illegal Opcode
D45C 2C:32 1A   BIT:abs    $1A32
D45F A4:DC      LDY:zp     Zp RAM 00DC
D461 26:30      ROL:zp     Zp RAM 0030
D463 3A:        Illegal Opcode
D464 1E:38 3C   ASL:abs,x  $3C38,X
D467 00:        BRK:imp    BREAK
D468 1A:        Illegal Opcode
D469 32:        Illegal Opcode
D46A 26:30      ROL:zp     Zp RAM 0030
D46C BA:        TSX:imp    Stack-->X
D46D C1:26      CMP:ind,x  26
D46F 30:3C      BMI:rel    Branch->$D4AD
D471 38:        SEC:imp    Set Carry
D472 32:        Illegal Opcode
D473 1C:        Illegal Opcode
D474 3E:26 38   ROL:abs,x  $3826,X
D477 1E:00 2C   ASL:abs,x  $2C00,X
D47A 1E:3A 00   ASL:abs,x  $003A,X
D47D 34:        Illegal Opcode
D47E 26:1E      ROL:zp     Zp RAM 001E
D480 1A:        Illegal Opcode
D481 1E:BA D6   ASL:abs,x  $D6BA,X
D484 22:        Illegal Opcode
D485 1E:2C 1C   ASL:abs,x  $1C2C,X
D488 00:        BRK:imp    BREAK
D489 1E:26 30   ASL:abs,x  $3026,X
D48C 42:        Illegal Opcode
D48D 1E:38 20   ASL:abs,x  $2038,X
D490 1E:B0 D6   ASL:abs,x  $D6B0,X
D493 26:30      ROL:zp     Zp RAM 0030
D495 3A:        Illegal Opcode
D496 1E:38 3C   ASL:abs,x  $3C38,X
D499 1E:00 20   ASL:abs,x  $2000,X
D49C 26:1A      ROL:zp     Zp RAM 001A
D49E 24:16      BIT:zp     Zp RAM 0016
D4A0 BA:        TSX:imp    Stack-->X
D4A1 00:        BRK:imp    BREAK
D4A2 20:38 1E   JSR:abs    $1E38
D4A5 1E:00 34   ASL:abs,x  $3400,X
D4A8 2C:16 C6   BIT:abs    $C616
D4AB 0E:04 00   ASL:abs    $0004
D4AE 1A:        Illegal Opcode
D4AF 32:        Illegal Opcode
D4B0 26:30      ROL:zp     Zp RAM 0030
D4B2 00:        BRK:imp    BREAK
D4B3 06:00      ASL:zp     Zp RAM 0000
D4B5 34:        Illegal Opcode
D4B6 2C:16 46   BIT:abs    $4616
D4B9 BA:        TSX:imp    Stack-->X
D4BA FA:        Illegal Opcode
D4BB 04:        Illegal Opcode
D4BC 00:        BRK:imp    BREAK
D4BD 34:        Illegal Opcode
D4BE 26:1E      ROL:zp     Zp RAM 001E
D4C0 1A:        Illegal Opcode
D4C1 1E:00 06   ASL:abs,x  $0600,X
D4C4 00:        BRK:imp    BREAK
D4C5 28:        PLP:imp    Pull P
D4C6 32:        Illegal Opcode
D4C7 3E:1E 3E   ROL:abs,x  $3E1E,X
D4CA 38:        SEC:imp    Set Carry
D4CB BA:        TSX:imp    Stack-->X
D4CC 00:        BRK:imp    BREAK
D4CD 04:        Illegal Opcode
D4CE 00:        BRK:imp    BREAK
D4CF 2E:3E 1E   ROL:abs    $1E3E
D4D2 30:48      BMI:rel    Branch->$D51C
D4D4 00:        BRK:imp    BREAK
D4D5 06:00      ASL:zp     Zp RAM 0000
D4D7 3A:        Illegal Opcode
D4D8 34:        Illegal Opcode
D4D9 26:1E      ROL:zp     Zp RAM 001E
D4DB 2C:9E FA   BIT:abs    $FA9E
D4DE 04:        Illegal Opcode
D4DF 00:        BRK:imp    BREAK
D4E0 2E:32 30   ROL:abs    $3032
D4E3 1E:1C 16   ASL:abs,x  $161C,X
D4E6 00:        BRK:imp    BREAK
D4E7 06:00      ASL:zp     Zp RAM 0000
D4E9 28:        PLP:imp    Pull P
D4EA 3E:1E 22   ROL:abs,x  $221E,X
D4ED 32:        Illegal Opcode
D4EE BA:        TSX:imp    Stack-->X
D4EF 14:        Illegal Opcode
D4F0 04:        Illegal Opcode
D4F1 00:        BRK:imp    BREAK
D4F2 1A:        Illegal Opcode
D4F3 32:        Illegal Opcode
D4F4 26:30      ROL:zp     Zp RAM 0030
D4F6 00:        BRK:imp    BREAK
D4F7 04:        Illegal Opcode
D4F8 00:        BRK:imp    BREAK
D4F9 34:        Illegal Opcode
D4FA 2C:16 C6   BIT:abs    $C616
D4FD 00:        BRK:imp    BREAK
D4FE 04:        Illegal Opcode
D4FF 00:        BRK:imp    BREAK
D500 34:        Illegal Opcode
D501 26:1E      ROL:zp     Zp RAM 001E
D503 1A:        Illegal Opcode
D504 1E:00 04   ASL:abs,x  $0400,X
D507 00:        BRK:imp    BREAK
D508 28:        PLP:imp    Pull P
D509 32:        Illegal Opcode
D50A 3E:1E 3E   ROL:abs,x  $3E1E,X
D50D B8:        CLV:imp    Clear Overflow
D50E 00:        BRK:imp    BREAK
D50F 04:        Illegal Opcode
D510 00:        BRK:imp    BREAK
D511 2E:3E 1E   ROL:abs    $1E3E
D514 30:48      BMI:rel    Branch->$D55E
D516 1E:00 04   ASL:abs,x  $0400,X
D519 00:        BRK:imp    BREAK
D51A 3A:        Illegal Opcode
D51B 34:        Illegal Opcode
D51C 26:1E      ROL:zp     Zp RAM 001E
D51E AC:00 04   LDY:abs    $0400
D521 00:        BRK:imp    BREAK
D522 2E:32 30   ROL:abs    $3032
D525 1E:1C 16   ASL:abs,x  $161C,X
D528 00:        BRK:imp    BREAK
D529 04:        Illegal Opcode
D52A 00:        BRK:imp    BREAK
D52B 28:        PLP:imp    Pull P
D52C 3E:1E 22   ROL:abs,x  $221E,X
D52F B2:        Illegal Opcode
D530 0E:06 00   ASL:abs    $0006
D533 1A:        Illegal Opcode
D534 32:        Illegal Opcode
D535 26:30      ROL:zp     Zp RAM 0030
D537 3A:        Illegal Opcode
D538 00:        BRK:imp    BREAK
D539 04:        Illegal Opcode
D53A 00:        BRK:imp    BREAK
D53B 34:        Illegal Opcode
D53C 2C:16 C6   BIT:abs    $C616
D53F FA:        Illegal Opcode
D540 06:00      ASL:zp     Zp RAM 0000
D542 34:        Illegal Opcode
D543 26:1E      ROL:zp     Zp RAM 001E
D545 1A:        Illegal Opcode
D546 1E:3A 00   ASL:abs,x  $003A,X
D549 04:        Illegal Opcode
D54A 00:        BRK:imp    BREAK
D54B 28:        PLP:imp    Pull P
D54C 32:        Illegal Opcode
D54D 3E:1E 3E   ROL:abs,x  $3E1E,X
D550 B8:        CLV:imp    Clear Overflow
D551 FA:        Illegal Opcode
D552 06:00      ASL:zp     Zp RAM 0000
D554 2E:3E 1E   ROL:abs    $1E3E
D557 30:48      BMI:rel    Branch->$D5A1
D559 1E:30 00   ASL:abs,x  $0030,X
D55C 04:        Illegal Opcode
D55D 00:        BRK:imp    BREAK
D55E 3A:        Illegal Opcode
D55F 34:        Illegal Opcode
D560 26:1E      ROL:zp     Zp RAM 001E
D562 AC:FA 06   LDY:abs    $06FA
D565 00:        BRK:imp    BREAK
D566 2E:32 30   ROL:abs    $3032
D569 1E:1C 16   ASL:abs,x  $161C,X
D56C 3A:        Illegal Opcode
D56D 00:        BRK:imp    BREAK
D56E 04:        Illegal Opcode
D56F 00:        BRK:imp    BREAK
D570 28:        PLP:imp    Pull P
D571 3E:1E 22   ROL:abs,x  $221E,X
D574 B2:        Illegal Opcode
D575 D3:        Illegal Opcode
D576 50:00      BVC:rel    Branch->$D578
D578 2E:1A 2E   ROL:abs    $2E1A
D57B 2C:44 44   BIT:abs    $4444
D57E 44:        Illegal Opcode
D57F 00:        BRK:imp    BREAK
D580 16:3C      ASL:zp,x   Zp RAM 003C
D582 16:38      ASL:zp,x   Zp RAM 0038
D584 A6:A0      LDX:zp     Zp RAM 00A0
D586 1A:        Illegal Opcode
D587 38:        SEC:imp    Set Carry
D588 1E:1C 26   ASL:abs,x  $261C,X
D58B 3C:        Illegal Opcode
D58C 3A:        Illegal Opcode
D58D 80:        Illegal Opcode
D58E A0:2A      LDY:imm    #2A
D590 38:        SEC:imp    Set Carry
D591 1E:1C 26   ASL:abs,x  $261C,X
D594 3C:        Illegal Opcode
D595 1E:80 A0   ASL:abs,x  $A080,X
D598 1A:        Illegal Opcode
D599 38:        SEC:imp    Set Carry
D59A 1E:1C 26   ASL:abs,x  $261C,X
D59D 3C:        Illegal Opcode
D59E 32:        Illegal Opcode
D59F 3A:        Illegal Opcode
D5A0 80:        Illegal Opcode
D5A1 DA:        Illegal Opcode
D5A2 18:        CLC:imp    Clear Carry
D5A3 32:        Illegal Opcode
D5A4 30:3E      BMI:rel    Branch->$D5E4
D5A6 3A:        Illegal Opcode
D5A7 80:        Illegal Opcode
D5A8 D0:06      BNE:rel    Branch->$D5B0
D5AA 00:        BRK:imp    BREAK
D5AB 1A:        Illegal Opcode
D5AC 38:        SEC:imp    Set Carry
D5AD 1E:1C 26   ASL:abs,x  $261C,X
D5B0 3C:        Illegal Opcode
D5B1 00:        BRK:imp    BREAK
D5B2 2E:26 30   ROL:abs    $3026
D5B5 26:2E      ROL:zp     Zp RAM 002E
D5B7 3E:AE D6   ROL:abs,x  $D6AE,X
D5BA 06:00      ASL:zp     Zp RAM 0000
D5BC 28:        PLP:imp    Pull P
D5BD 1E:3E 44   ASL:abs,x  $443E,X
D5C0 00:        BRK:imp    BREAK
D5C1 2E:26 30   ROL:abs    $3026
D5C4 26:2E      ROL:zp     Zp RAM 002E
D5C6 3E:AE D0   ROL:abs,x  $D0AE,X
D5C9 06:00      ASL:zp     Zp RAM 0000
D5CB 3A:        Illegal Opcode
D5CC 34:        Illegal Opcode
D5CD 26:1E      ROL:zp     Zp RAM 001E
D5CF 2C:1E 00   BIT:abs    $001E
D5D2 2E:26 30   ROL:abs    $3026
D5D5 26:2E      ROL:zp     Zp RAM 002E
D5D7 3E:AE D3   ROL:abs,x  $D3AE,X
D5DA 06:00      ASL:zp     Zp RAM 0000
D5DC 28:        PLP:imp    Pull P
D5DD 3E:1E 22   ROL:abs,x  $221E,X
D5E0 32:        Illegal Opcode
D5E1 3A:        Illegal Opcode
D5E2 00:        BRK:imp    BREAK
D5E3 2E:26 30   ROL:abs    $3026
D5E6 26:2E      ROL:zp     Zp RAM 002E
D5E8 B2:        Illegal Opcode
D5E9 C8:        INY:imp    Y=Y+1
D5EA 18:        CLC:imp    Clear Carry
D5EB 32:        Illegal Opcode
D5EC 30:3E      BMI:rel    Branch->$D62C
D5EE 3A:        Illegal Opcode
D5EF 00:        BRK:imp    BREAK
D5F0 1E:40 1E   ASL:abs,x  $1E40,X
D5F3 38:        SEC:imp    Set Carry
D5F4 46:80      LSR:zp     Zp RAM 0080
D5F6 CE:18 32   DEC:abs    $3218
D5F9 30:3E      BMI:rel    Branch->$D639
D5FB 3A:        Illegal Opcode
D5FC 00:        BRK:imp    BREAK
D5FD 1A:        Illegal Opcode
D5FE 24:16      BIT:zp     Zp RAM 0016
D600 36:3E      ROL:zp,x   Zp RAM 003E
D602 1E:80 CE   ASL:abs,x  $CE80,X
D605 18:        CLC:imp    Clear Carry
D606 32:        Illegal Opcode
D607 30:3E      BMI:rel    Branch->$D647
D609 3A:        Illegal Opcode
D60A 00:        BRK:imp    BREAK
D60B 28:        PLP:imp    Pull P
D60C 1E:1C 1E   ASL:abs,x  $1E1C,X
D60F 80:        Illegal Opcode
D610 C8:        INY:imp    Y=Y+1
D611 18:        CLC:imp    Clear Carry
D612 32:        Illegal Opcode
D613 30:3E      BMI:rel    Branch->$D653
D615 3A:        Illegal Opcode
D616 00:        BRK:imp    BREAK
D617 1A:        Illegal Opcode
D618 16:1C      ASL:zp,x   Zp RAM 001C
D61A 16:80      ASL:zp,x   Zp RAM 0080
D61C B8:        CLV:imp    Clear Overflow
D61D 16:40      ASL:zp,x   Zp RAM 0040
D61F 32:        Illegal Opcode
D620 26:1C      ROL:zp     Zp RAM 001C
D622 00:        BRK:imp    BREAK
D623 3A:        Illegal Opcode
D624 34:        Illegal Opcode
D625 26:2A      ROL:zp     Zp RAM 002A
D627 1E:BA 88   ASL:abs,x  $88BA,X
D62A 16:3C      ASL:zp,x   Zp RAM 003C
D62C 3C:        Illegal Opcode
D62D 1E:30 3C   ASL:abs,x  $3C30,X
D630 26:32      ROL:zp     Zp RAM 0032
D632 30:00      BMI:rel    Branch->$D634
D634 16:3E      ASL:zp,x   Zp RAM 003E
D636 44:        Illegal Opcode
D637 00:        BRK:imp    BREAK
D638 2C:16 30   BIT:abs    $3016
D63B 1A:        Illegal Opcode
D63C 1E:BA 96   ASL:abs,x  $96BA,X
D63F 3A:        Illegal Opcode
D640 34:        Illegal Opcode
D641 26:3C      ROL:zp     Zp RAM 003C
D643 48:        PHA:imp    Push Accum
D644 1E:30 00   ASL:abs,x  $0030,X
D647 16:3E      ASL:zp,x   Zp RAM 003E
D649 3A:        Illegal Opcode
D64A 42:        Illegal Opcode
D64B 1E:26 1A   ASL:abs,x  $1A26,X
D64E 24:1E      BIT:zp     Zp RAM 001E
D650 B0:A0      BCS:rel    Branch->$D5F2
D652 1E:40 26   ASL:abs,x  $2640,X
D655 3C:        Illegal Opcode
D656 1E:00 2C   ASL:abs,x  $2C00,X
D659 16:3A      ASL:zp,x   Zp RAM 003A
D65B 00:        BRK:imp    BREAK
D65C 34:        Illegal Opcode
D65D 3E:30 3C   ROL:abs,x  $3C30,X
D660 16:BA      ASL:zp,x   Zp RAM 00BA
D662 E0:2C      CPX:imm    #2C
D664 1E:40 1E   ASL:abs,x  $1E40,X
D667 AC:DA 30   LDY:abs    $30DA
D66A 26:40      ROL:zp     Zp RAM 0040
D66C 1E:16 BE   ASL:abs,x  $BE16,X
D66F E2:        Illegal Opcode
D670 22:        Illegal Opcode
D671 38:        SEC:imp    Set Carry
D672 16:9C      ASL:zp,x   Zp RAM 009C
D674 E0:30      CPX:imm    #30
D676 26:40      ROL:zp     Zp RAM 0040
D678 1E:AC C4   ASL:abs,x  $C4AC,X
D67B 3A:        Illegal Opcode
D67C 3E:34 1E   ROL:abs,x  $1E34,X
D67F 38:        SEC:imp    Set Carry
D680 48:        PHA:imp    Push Accum
D681 16:34      ASL:zp,x   Zp RAM 0034
D683 34:        Illegal Opcode
D684 1E:38 00   ASL:abs,x  $0038,X
D687 38:        SEC:imp    Set Carry
D688 1E:1A 24   ASL:abs,x  $241A,X
D68B 16:38      ASL:zp,x   Zp RAM 0038
D68D 22:        Illegal Opcode
D68E 9E:        Illegal Opcode
D68F CD:30 1E   CMP:abs    $1E30
D692 3E:1E 38   ROL:abs,x  $381E,X
D695 00:        BRK:imp    BREAK
D696 3A:        Illegal Opcode
D697 3E:34 1E   ROL:abs,x  $1E34,X
D69A 38:        SEC:imp    Set Carry
D69B 48:        PHA:imp    Push Accum
D69C 16:34      ASL:zp,x   Zp RAM 0034
D69E 34:        Illegal Opcode
D69F 1E:B8 CD   ASL:abs,x  $CDB8,X
D6A2 30:3E      BMI:rel    Branch->$D6E2
D6A4 1E:40 32   ASL:abs,x  $3240,X
D6A7 00:        BRK:imp    BREAK
D6A8 3A:        Illegal Opcode
D6A9 3E:34 1E   ROL:abs,x  $1E34,X
D6AC 38:        SEC:imp    Set Carry
D6AD 48:        PHA:imp    Push Accum
D6AE 16:34      ASL:zp,x   Zp RAM 0034
D6B0 34:        Illegal Opcode
D6B1 1E:B8 31   ASL:abs,x  $31B8,X
D6B4 D0:6D      BNE:rel    Branch->$D723
D6B6 D0:A9      BNE:rel    Branch->$D661
D6B8 D0:E5      BNE:rel    Branch->$D69F
D6BA D0:AD      BNE:rel    Branch->$D669
D6BC 00:        BRK:imp    BREAK
D6BD 0E:85 0A   ASL:abs    $0A85
D6C0 29:38      AND:imm    #38
D6C2 4A:        LSR:accum  
D6C3 4A:        LSR:accum  
D6C4 4A:        LSR:accum  
D6C5 AA:        TAX:imp    A-->X
D6C6 BD:F7 D6   LDA:abs,x  $D6F7,X
D6C9 8D:56 01   STA:abs    $0156
D6CC AD:00 0D   LDA:abs    $0D00         ; Get the contents of DIP N13,
D6CF 49:02      EOR:imm    #02           ; EOR with 02 (reason unknown) and
D6D1 85:09      STA:zp     Zp RAM 0009   ; store into 0009.
D6D3 A5:0A      LDA:zp     Zp RAM 000A
D6D5 2A:        ROL:accum  
D6D6 2A:        ROL:accum  
D6D7 2A:        ROL:accum  
D6D8 29:03      AND:imm    #03
D6DA AA:        TAX:imp    A-->X
D6DB BD:FF D6   LDA:abs,x  $D6FF,X
D6DE 8D:58 01   STA:abs    $0158
D6E1 A5:0A      LDA:zp     Zp RAM 000A
D6E3 29:06      AND:imm    #06
D6E5 A8:        TAY:imp    A-->Y
D6E6 B9:B3 D6   LDA:abs,y  $D6B3,Y
D6E9 85:AC      STA:zp     Zp RAM 00AC
D6EB B9:B4 D6   LDA:abs,y  $D6B4,Y
D6EE 85:AD      STA:zp     Zp RAM 00AD
D6F0 20:E0 DB   JSR:abs    $DBE0	 ; Read D/E2 switch (special format)
					 ; and store into $0037.
D6F3 8D:6A 01   STA:abs    $016A
D6F6 60:        RTS:imp    Ret from Sub
D6F7 02:        Illegal Opcode
D6F8 01:03      ORA:ind,x  03
D6FA 04:        Illegal Opcode
D6FB 05:06      ORA:zp     Zp RAM 0006
D6FD 07:        Illegal Opcode
D6FE 00:        BRK:imp    BREAK
;
; Table believed to store the number of lives to start with
;
D6FF 03:        Illegal Opcode
D700 04:        Illegal Opcode
D701 05:02      ORA:zp     Zp RAM 0002
D703 7C:        Illegal Opcode
;
; Interrupt handler. This handles both the maskable and the non-maskable
; interrupts that are produced by the hardware.
;
D704 48:        PHA:imp    Push Accum	 ; Transfer the accumulator, X,
D705 8A:        TXA:imp    X-->A	 ; and Y registers to the stack.
D706 48:        PHA:imp    Push Accum	 ; Necessary to preserve these
D707 98:        TYA:imp    Y-->A	 ; registers upon return.
D708 48:        PHA:imp    Push Accum	 ;
D709 D8:        CLD:imp    Unset Decimal ; Ensure we're in normal math mode.
D70A BA:        TSX:imp    Stack-->X	 ; Look at the stack pointer. If it
D70B E0:D0      CPX:imm    #D0		 ; is less than D0, then BREAK.
D70D 90:04      BCC:rel    Branch->$D713 ;
D70F A5:53      LDA:zp     Zp RAM 0053   ; If the 0-9 timer is > #7f,
D711 10:04      BPL:rel    Branch->$D717 ; then BREAK!!!
D713 00:        BRK:imp    BREAK	 ;
D714 4C:3F D9   JMP:abs    $D93F	 ; Perform a full reset of the
					 ; game, as done on power-up.
D717 8D:00 50   STA:abs    $5000         ; Clear the watchdog timer.
;
; Interrupt level handling of inputs on Pokey #1.
;
D71A 8D:CB 60   STA:abs    $60CB	 ; Rescan the inputs on Pokey #1.
D71D AD:C8 60   LDA:abs    $60C8	 ; Grab the inputs for Pokey #1
					 ; BITS 0-3: Encoder Wheel
					 ; BIT  4  : Cocktail detection
					 ; BIT  5  : Switch #1 at D/E2
					 ; BITS 6-7: Unused.
					 ;
D720 49:0F      EOR:imm    #0F	  	 ; Invert just the encoder bits.
D722 A8:        TAY:imp    A-->Y	 ; 
D723 29:10      AND:imm    #10		 ; Store in $0117 a 10 if the game
D725 8D:17 01   STA:abs    $0117	 ; is a cocktail table. 00 if not.
D728 98:        TYA:imp    Y-->A	 ; 
D729 38:        SEC:imp    Set Carry     ; Store the raw spinner position
D72A E5:52      SBC:zp     Zp RAM 0052	 ; (0-F) $0052.
D72C 29:0F      AND:imm    #0F		 ;
D72E C9:08      CMP:imm    #08		 ;
D730 90:02      BCC:rel    Branch->$D734 ;
D732 09:F0      ORA:imm    #F0		 ;
D734 18:        CLC:imp    Clear Carry   ;
D735 65:50      ADC:zp     Zp RAM 0050	 ; Store the relative spinner
D737 85:50      STA:zp     Zp RAM 0050	 ; position (00-FF) since last read
D739 84:52      STY:zp     Zp RAM 0052	 ; by the software into $0050.
;
; Interrupt level handling of input on Pokey #2 and hardware register $0C00.
;
D73B 8D:DB 60   STA:abs    $60DB	 ; Rescan the inputs on Pokey #2.
D73E AC:D8 60   LDY:abs    $60D8	 ; Grab the inputs of Pokey #2.
D741 AD:00 0C   LDA:abs    $0C00         ; Load the 0C00 register.
D744 85:08      STA:zp     Zp RAM 0008   ; Store in shadow location $0008.
					 ; BIT 0: Right Coin
					 ; BIT 1: Center Coin
					 ; BIT 2: Left Coin
					 ; BIT 3: Slam Swith
					 ; BIT 4: Test Switch
					 ; BIT 5: Diagnoistic Step Pad
					 ; BIT 6: HALT (from vector machine)
					 ; BIT 7: 3kHz square wave
;
D746 A5:4C      LDA:zp     Zp RAM 004C   ; Make a copy of $004C.
D748 84:4C      STY:zp     Zp RAM 004C	 ; Store Pokey #2 inputs in $004C.
					 ; BIT 0: D/E2 switch #2
					 ; BIT 1: D/E2 switch #3
					 ; BIT 2: D/E2 switch #4
					 ; BIT 3: Fire Button
					 ; BIT 4: Zapper Button
					 ; BIT 5: Start Player 1 Button
					 ; BIT 6: Start Player 2 Button
					 ; BIT 7: Unused.
;
D74A A8:        TAY:imp    A-->Y	 ; WHAT A MESS! Here's the jist...
D74B 25:4C      AND:zp     Zp RAM 004C   ; 
D74D 05:4D      ORA:zp     Zp RAM 004D   ; Put into $004D and $004F the
D74F 85:4D      STA:zp     Zp RAM 004D   ; buttons with just a bit of
D751 98:        TYA:imp    Y-->A         ; debounce checking.
D752 05:4C      ORA:zp     Zp RAM 004C   ;
D754 25:4D      AND:zp     Zp RAM 004D   ; *AND* the buttons into $004E so
D756 85:4D      STA:zp     Zp RAM 004D   ; you can maintain a list of
D758 A8:        TAY:imp    A-->Y         ; button events to process.
D759 45:4F      EOR:zp     Zp RAM 004F   ;
D75B 25:4D      AND:zp     Zp RAM 004D	 ; Oh. Almost forgot. $004C will
D75D 05:4E      ORA:zp     Zp RAM 004E	 ; have the raw button status with
D75F 85:4E      STA:zp     Zp RAM 004E	 ; no debounce checking whatsoever.
D761 84:4F      STY:zp     Zp RAM 004F	 ;
;
; Interrupt level handling of LEDS, coin counters, and XY inversion register.
;
D763 A5:B4      LDA:zp     Zp RAM 00B4	 ; Load the X/Y axis flip state.
D765 A4:13      LDY:zp     Zp RAM 0013	 ; Increment coin counter "A"?
D767 10:02      BPL:rel    Branch->$D76B ; If so, set the appropriate bit.
D769 09:04      ORA:imm    #04
D76B A4:14      LDY:zp     Zp RAM 0014	 ; Increment coin counter "B"?
D76D 10:02      BPL:rel    Branch->$D771 ; If so, set the appropriate bit.
D76F 09:02      ORA:imm    #02
D771 A4:15      LDY:zp     Zp RAM 0015	 ; Increment coin cointer "C"?
D773 10:02      BPL:rel    Branch->$D777 ; If so, set the appropriate bit.
D775 09:01      ORA:imm    #01
D777 8D:00 40   STA:abs    $4000         ; Implement coin counter / video
					 ; inversion (XY flip) register.
D77A A6:3E      LDX:zp     Zp RAM 003E
D77C E8:        INX:imp    X=X+1
D77D A4:05      LDY:zp     Zp RAM 0005
D77F D0:10      BNE:rel    Branch->$D791
D781 A2:00      LDX:imm    #00		 ; Haven't gone through this
D783 A4:07      LDY:zp     Zp RAM 0007	 ; extensively, but it appears to
D785 C0:40      CPY:imm    #40		 ; be a handler for the LEDs.
D787 90:08      BCC:rel    Branch->$D791
D789 A6:06      LDX:zp     Zp RAM 0006	 ; Are there two or + game credits
D78B E0:02      CPX:imm    #02		 ; pending?
D78D 90:02      BCC:rel    Branch->$D791
D78F A2:03      LDX:imm    #03
D791 BD:DD D7   LDA:abs,x  $D7DD,X
D794 45:A1      EOR:zp     Zp RAM 00A1
D796 29:03      AND:imm    #03
D798 45:A1      EOR:zp     Zp RAM 00A1
D79A 85:A1      STA:zp     Zp RAM 00A1
D79C 8D:E0 60   STA:abs    $60E0	 ; Set the LED state.
D79F 20:24 CF   JSR:abs    $CF24
D7A2 20:0A CD   JSR:abs    $CD0A
D7A5 E6:53      INC:zp     Zp RAM 0053   ; Update the 0-9 timer
D7A7 E6:07      INC:zp     Zp RAM 0007   ; Update the 00-FF timer
D7A9 D0:1E      BNE:rel    Branch->$D7C9 ; Has a second elapsed?
D7AB EE:06 04   INC:abs    $0406         ; Increase the counter which keeps
D7AE D0:08      BNE:rel    Branch->$D7B8 ; track of the number of seconds
D7B0 EE:07 04   INC:abs    $0407         ; that the machine has been on.
D7B3 D0:03      BNE:rel    Branch->$D7B8 ;
D7B5 EE:08 04   INC:abs    $0408	 ;
D7B8 24:05      BIT:zp     Zp RAM 0005
D7BA 50:0D      BVC:rel    Branch->$D7C9
D7BC EE:09 04   INC:abs    $0409         ; Increase the counter which keeps
D7BF D0:08      BNE:rel    Branch->$D7C9 ; track of the number of seconds
D7C1 EE:0A 04   INC:abs    $040A         ; that the machine has been played.
D7C4 D0:03      BNE:rel    Branch->$D7C9 ;
D7C6 EE:0B 04   INC:abs    $040B	 ;
;
D7C9 2C:00 0C   BIT:abs    $0C00         ; Is the vector machine in HALT state?
D7CC 50:09      BVC:rel    Branch->$D7D7 ; If not, exit the interrupt handler.
;
; Interrupt level handling of the Vector State Machine
;
D7CE EE:33 01   INC:abs    $0133	 ; Update counter that is tied to
					 ; vector refreshes. (informational)
D7D1 8D:00 58   STA:abs    $5800         ; Vector machine RESET
D7D4 8D:00 48   STA:abs    $4800         ; Vector machine GO
;
D7D7 68:        PLA:imp    Pull Accum	 ; Restore the state of the
D7D8 A8:        TAY:imp    A-->Y	 ; Y register, X register, and the
D7D9 68:        PLA:imp    Pull Accum	 ; accumulator. The "RTI" command
D7DA AA:        TAX:imp    A-->X	 ; will also restore the process
D7DB 68:        PLA:imp    Pull Accum	 ; status register, which has the
D7DC 40:        RTI:imp    Ret from Int	 ; proper math mode.
;
; End of interrupt handler code.
;
D7DD FF:        DATA			 ; Table referenced by $D791.
D7DE FD:FE FC   DATA			 ; Something about LED blinking.
D7E1 A9:00      LDA:imm    #00
D7E3 85:05      STA:zp     Zp RAM 0005
D7E5 A9:02      LDA:imm    #02
D7E7 85:01      STA:zp     Zp RAM 0001
D7E9 AD:CA 01   LDA:abs    $01CA
D7EC D0:15      BNE:rel    Branch->$D803
D7EE AD:00 0C   LDA:abs    $0C00         ; Check the TEST switch.
D7F1 29:10      AND:imm    #10
D7F3 F0:0E      BEQ:rel    Branch->$D803 ; If not in TEST, skip ahead.
D7F5 A9:00      LDA:imm    #00
D7F7 85:00      STA:zp     Zp RAM 0000
D7F9 AD:C9 01   LDA:abs    $01C9
D7FC 29:03      AND:imm    #03
D7FE F0:03      BEQ:rel    Branch->$D803
;
; Start of ROM 136002.222 at $D800.	 NOTE: VERSION 2+ ROMS, FOLKS.
;
D800 20:AC AB   JSR:abs    $ABAC
D803 60:        RTS:imp    Ret from Sub
;
D804 20:BB D6   JSR:abs    $D6BB
D807 20:A8 AA   JSR:abs    $AAA8
D80A 20:0D DD   JSR:abs    $DD0D
D80D 20:41 DD   JSR:abs    $DD41
D810 AD:58 01   LDA:abs    $0158
D813 85:37      STA:zp     Zp RAM 0037
D815 20:53 DF   JSR:abs    $DF53
D818 A9:E8      LDA:imm    #E8
D81A A2:C0      LDX:imm    #C0
D81C 20:75 DF   JSR:abs    $DF75
D81F A9:32      LDA:imm    #32
D821 A2:6C      LDX:imm    #6C
D823 20:39 DF   JSR:abs    $DF39
D826 C6:37      DEC:zp     Zp RAM 0037
D828 D0:F5      BNE:rel    Branch->$D81F
D82A AD:6A 01   LDA:abs    $016A
D82D 29:03      AND:imm    #03
D82F 0A:        ASL:accum  
D830 A8:        TAY:imp    A-->Y
D831 B9:1F 3F   LDA:abs,y  $3F1F,Y
D834 BE:1E 3F   LDX:abs,y  $3F1E,Y
D837 20:39 DF   JSR:abs    $DF39
D83A AD:00 02   LDA:abs    $0200
D83D 20:CE AD   JSR:abs    $ADCE
D840 8D:00 02   STA:abs    $0200
D843 29:06      AND:imm    #06
D845 48:        PHA:imp    Push Accum
D846 A8:        TAY:imp    A-->Y
D847 B9:17 3F   LDA:abs,y  $3F17,Y
D84A BE:16 3F   LDX:abs,y  $3F16,Y
D84D 20:39 DF   JSR:abs    $DF39
D850 68:        PLA:imp    Pull Accum
D851 4A:        LSR:accum  
D852 AA:        TAX:imp    A-->X
D853 A5:4D      LDA:zp     Zp RAM 004D
D855 3D:B6 D8   AND:abs,x  $D8B6,X
D858 DD:B6 D8   CMP:abs,x  $D8B6,X
D85B D0:1A      BNE:rel    Branch->$D877
D85D CA:        DEX:imp    X=X-1
D85E CA:        DEX:imp    X=X-1
D85F 10:03      BPL:rel    Branch->$D864
D861 4C:3F D9   JMP:abs    $D93F	 ; Perform a full reset of the
					 ; game, as done on power-up.
D864 D0:06      BNE:rel    Branch->$D86C
D866 20:E9 DD   JSR:abs    $DDE9
D869 B8:        CLV:imp    Clear Overflow
D86A 50:0B      BVC:rel    Branch->$D877
D86C 20:ED DD   JSR:abs    $DDED
D86F AD:C9 01   LDA:abs    $01C9
D872 09:03      ORA:imm    #03
D874 8D:C9 01   STA:abs    $01C9
D877 AD:CA 01   LDA:abs    $01CA
D87A 2D:C6 01   AND:abs    $01C6
D87D F0:07      BEQ:rel    Branch->$D886
D87F A9:34      LDA:imm    #34
D881 A2:6E      LDX:imm    #6E
D883 20:39 DF   JSR:abs    $DF39
D886 20:53 DF   JSR:abs    $DF53
D889 A5:09      LDA:zp     Zp RAM 0009
D88B 29:1C      AND:imm    #1C
D88D 4A:        LSR:accum  
D88E 4A:        LSR:accum  
D88F AA:        TAX:imp    A-->X
D890 BD:BA D8   LDA:abs,x  $D8BA,X
D893 A0:EE      LDY:imm    #EE
D895 A2:1B      LDX:imm    #1B
D897 20:A9 D8   JSR:abs    $D8A9
D89A A5:09      LDA:zp     Zp RAM 0009
D89C 4A:        LSR:accum  
D89D 4A:        LSR:accum  
D89E 4A:        LSR:accum  
D89F 4A:        LSR:accum  
D8A0 4A:        LSR:accum  
D8A1 AA:        TAX:imp    A-->X
D8A2 BD:C2 D8   LDA:abs,x  $D8C2,X
D8A5 A0:32      LDY:imm    #32
D8A7 A2:F8      LDX:imm    #F8
D8A9 85:29      STA:zp     Zp RAM 0029
D8AB 98:        TYA:imp    Y-->A
D8AC 20:75 DF   JSR:abs    $DF75
D8AF A9:29      LDA:imm    #29
D8B1 A0:01      LDY:imm    #01
D8B3 4C:B1 DF   JMP:abs    $DFB1
D8B6 18:        CLC:imp    Clear Carry
D8B7 18:        CLC:imp    Clear Carry
D8B8 30:50      BMI:rel    Branch->$D90A
D8BA 11:14      ORA:ind,y  14
D8BC 15:16      ORA:zp,x   Zp RAM 0016
D8BE 21:24      AND:ind,x  24
D8C0 25:26      AND:zp     Zp RAM 0026
D8C2 00:        BRK:imp    BREAK
D8C3 12:        Illegal Opcode
D8C4 14:        Illegal Opcode
D8C5 24:15      BIT:zp     Zp RAM 0015
D8C7 13:        Illegal Opcode
D8C8 00:        BRK:imp    BREAK
D8C9 00:        BRK:imp    BREAK
D8CA A8:        TAY:imp    A-->Y
D8CB A9:00      LDA:imm    #00
D8CD 84:79      STY:zp     Zp RAM 0079
D8CF 4A:        LSR:accum  
D8D0 4A:        LSR:accum  
D8D1 0A:        ASL:accum  
D8D2 AA:        TAX:imp    A-->X
D8D3 98:        TYA:imp    Y-->A
D8D4 29:0F      AND:imm    #0F
D8D6 D0:01      BNE:rel    Branch->$D8D9
D8D8 E8:        INX:imp    X=X+1
D8D9 9A:        TXS:imp    X-->Stack
D8DA A9:A2      LDA:imm    #A2
D8DC 8D:C1 60   STA:abs    $60C1
D8DF BA:        TSX:imp    Stack-->X
D8E0 D0:07      BNE:rel    Branch->$D8E9
D8E2 A9:60      LDA:imm    #60
D8E4 A0:09      LDY:imm    #09
D8E6 B8:        CLV:imp    Clear Overflow
D8E7 50:04      BVC:rel    Branch->$D8ED
D8E9 A9:C0      LDA:imm    #C0
D8EB A0:01      LDY:imm    #01
D8ED 8D:C0 60   STA:abs    $60C0
D8F0 A9:03      LDA:imm    #03           ; Turn on player 1 and player 2 LED.
D8F2 8D:E0 60   STA:abs    $60E0
D8F5 A2:00      LDX:imm    #00
D8F7 2C:00 0C   BIT:abs    $0C00         ; Loop until the signal from the
D8FA 30:FB      BMI:rel    Branch->$D8F7 ; 3kHz square wave is negative.
D8FC 2C:00 0C   BIT:abs    $0C00         ; Loop until the signal from the
D8FF 10:FB      BPL:rel    Branch->$D8FC ; 3kHz square wave is positive.
D901 8D:00 50   STA:abs    $5000         ; Clear the watchdog timer.
D904 CA:        DEX:imp    X=X-1
D905 D0:F0      BNE:rel    Branch->$D8F7
D907 88:        DEY:imp    Y=Y-1
D908 D0:ED      BNE:rel    Branch->$D8F7
D90A 8E:C1 60   STX:abs    $60C1
D90D A9:00      LDA:imm    #00
D90F 8D:E0 60   STA:abs    $60E0         ; Turn off player 1 and player 2 LED.
D912 A0:09      LDY:imm    #09
D914 2C:00 0C   BIT:abs    $0C00         ; Loop until the signal from the
D917 30:FB      BMI:rel    Branch->$D914 ; 3khz square wave is negative.
D919 2C:00 0C   BIT:abs    $0C00         ; Loop until the signal from the
D91C 10:FB      BPL:rel    Branch->$D919 ; 3kHz square wave is positive.
D91E 8D:00 50   STA:abs    $5000         ; Clear the watchdog timer.
D921 CA:        DEX:imp    X=X-1
D922 D0:F0      BNE:rel    Branch->$D914
D924 88:        DEY:imp    Y=Y-1
D925 D0:ED      BNE:rel    Branch->$D914
D927 BA:        TSX:imp    Stack-->X
D928 CA:        DEX:imp    X=X-1
D929 9A:        TXS:imp    X-->Stack
D92A 10:AE      BPL:rel    Branch->$D8DA
D92C 4C:0A DA   JMP:abs    $DA0A
D92F 51:00      EOR:ind,y  00
D931 A8:        TAY:imp    A-->Y
D932 A5:01      LDA:zp     Zp RAM 0001
D934 C9:20      CMP:imm    #20
D936 90:02      BCC:rel    Branch->$D93A
D938 E9:18      SBC:imm    #18
D93A 29:1F      AND:imm    #1F
D93C 4C:CD D8   JMP:abs    $D8CD
;
; Boot code. Executed as the very first instructions.
; Aka "reset vector".
;
D93F 78:        SEI:imp    Disable IRQ	 ; Disable interrupts and clear
D940 8D:00 50   STA:abs    $5000	 ; the watchdog timer.
D943 8D:00 58   STA:abs    $5800         ; Reset the vector state machine.
D946 A2:FF      LDX:imm    #FF           ; Set the stack pointer to 01FF.
D948 9A:        TXS:imp    X-->Stack
D949 D8:        CLD:imp    Unset Decimal ; Set normal math mode.
D94A E8:        INX:imp    X=X+1         ;
D94B 8A:        TXA:imp    X-->A         ;
D94C A8:        TAY:imp    A-->Y         ;
D94D 84:00      STY:zp     Zp RAM 0000
D94F 86:01      STX:zp     Zp RAM 0001
D951 A0:00      LDY:imm    #00           ; Clear out a page of RAM.
D953 91:00      STA:ind,y  00		 ;
D955 C8:        INY:imp    Y=Y+1	 ;
D956 D0:FB      BNE:rel    Branch->$D953 ;
D958 E8:        INX:imp    X=X+1
D959 E0:08      CPX:imm    #08
D95B D0:02      BNE:rel    Branch->$D95F
D95D A2:20      LDX:imm    #20
D95F E0:30      CPX:imm    #30
D961 8D:00 50   STA:abs    $5000         ; Reset the watchdog timer.
D964 90:E7      BCC:rel    Branch->$D94D
D966 85:01      STA:zp     Zp RAM 0001
D968 8D:E0 60   STA:abs    $60E0         ; Reset the LED/control state.
D96B 8D:CF 60   STA:abs    $60CF         ; Initialize the POKEYs?
D96E 8D:DF 60   STA:abs    $60DF
D971 A2:07      LDX:imm    #07
D973 8E:CF 60   STX:abs    $60CF	 ; Ready Pokey #1 for sound.
D976 8E:DF 60   STX:abs    $60DF	 ; Ready Pokey #2 for sound.
D979 E8:        INX:imp    X=X+1	 ; Clear out the volume/distortion
D97A 9D:C0 60   STA:abs,x  $60C0,X       ; and frequency for all sound
D97D 9D:D0 60   STA:abs,x  $60D0,X	 ; channels on both Pokeys.
D980 CA:        DEX:imp    X=X-1
D981 10:F7      BPL:rel    Branch->$D97A
D983 AD:00 0C   LDA:abs    $0C00         ; Check the TEST switch.
D986 29:10      AND:imm    #10
D988 F0:1F      BEQ:rel    Branch->$D9A9 ; If TEST, skip down a bit.
D98A 8D:00 50   STA:abs    $5000         ; Reset the watchdog timer.
D98D CE:00 01   DEC:abs    $0100
D990 D0:F8      BNE:rel    Branch->$D98A
D992 CE:01 01   DEC:abs    $0101
D995 D0:F3      BNE:rel    Branch->$D98A ; Delay loop.
D997 A9:10      LDA:imm    #10		 ; Set the "Y" axis as being
D999 85:B4      STA:zp     Zp RAM 00B4	 ; flipped.
D99B 20:11 DE   JSR:abs    $DE11
D99E 20:AC AB   JSR:abs    $ABAC
D9A1 20:6E C1   JSR:abs    $C16E
D9A4 58:        CLI:imp    Enable IRQ    ; Enable interrupt processing.
D9A5 4C:A0 C7   JMP:abs    $C7A0
;
D9A8 A0:A2      LDY:imm    #A2           ; WRONG -- RE-DISASSEMBLE
D9AA 11:9A      ORA:ind,y  9A            ; $D9A9 has first command.
D9AC A0:00      LDY:imm    #00
D9AE BA:        TSX:imp    Stack-->X
D9AF 96:00      STX:zp,y   Zp RAM 0000
D9B1 A2:01      LDX:imm    #01
D9B3 C8:        INY:imp    Y=Y+1
D9B4 B9:00 00   LDA:abs,y  $0000,Y
D9B7 F0:03      BEQ:rel    Branch->$D9BC
D9B9 4C:CA D8   JMP:abs    $D8CA
D9BC E8:        INX:imp    X=X+1
D9BD D0:F4      BNE:rel    Branch->$D9B3
D9BF BA:        TSX:imp    Stack-->X
D9C0 8A:        TXA:imp    X-->A
D9C1 8D:00 50   STA:abs    $5000	 ; Clear the watchdog timer.
D9C4 C8:        INY:imp    Y=Y+1
D9C5 59:00 00   EOR:abs,y  $0000,Y
D9C8 D0:EF      BNE:rel    Branch->$D9B9
D9CA 99:00 00   STA:abs,y  $0000,Y
D9CD C8:        INY:imp    Y=Y+1
D9CE D0:DE      BNE:rel    Branch->$D9AE
D9D0 BA:        TSX:imp    Stack-->X
D9D1 8A:        TXA:imp    X-->A
D9D2 0A:        ASL:accum  
D9D3 AA:        TAX:imp    A-->X
D9D4 90:D5      BCC:rel    Branch->$D9AB
D9D6 A0:00      LDY:imm    #00
D9D8 A2:01      LDX:imm    #01
D9DA 84:00      STY:zp     Zp RAM 0000
D9DC 86:01      STX:zp     Zp RAM 0001
D9DE A0:00      LDY:imm    #00
D9E0 B1:00      LDA:ind,y  00
D9E2 F0:03      BEQ:rel    Branch->$D9E7
D9E4 4C:31 D9   JMP:abs    $D931
D9E7 A9:11      LDA:imm    #11
D9E9 91:00      STA:ind,y  00
D9EB D1:00      CMP:ind,y  00
D9ED F0:03      BEQ:rel    Branch->$D9F2
D9EF 4C:2F D9   JMP:abs    $D92F
D9F2 0A:        ASL:accum  
D9F3 90:F4      BCC:rel    Branch->$D9E9
D9F5 A9:00      LDA:imm    #00
D9F7 91:00      STA:ind,y  00
D9F9 C8:        INY:imp    Y=Y+1
D9FA D0:E4      BNE:rel    Branch->$D9E0
D9FC 8D:00 50   STA:abs    $5000	 ; Clear the watchdog timer.
D9FF E8:        INX:imp    X=X+1
DA00 E0:08      CPX:imm    #08
DA02 D0:02      BNE:rel    Branch->$DA06
DA04 A2:20      LDX:imm    #20
DA06 E0:30      CPX:imm    #30
DA08 90:D0      BCC:rel    Branch->$D9DA
DA0A A9:00      LDA:imm    #00
DA0C A8:        TAY:imp    A-->Y
DA0D AA:        TAX:imp    A-->X
DA0E 85:3B      STA:zp     Zp RAM 003B
DA10 A9:30      LDA:imm    #30
DA12 85:3C      STA:zp     Zp RAM 003C
DA14 A9:08      LDA:imm    #08
DA16 85:38      STA:zp     Zp RAM 0038
DA18 8A:        TXA:imp    X-->A
DA19 51:3B      EOR:ind,y  3B
DA1B C8:        INY:imp    Y=Y+1
DA1C D0:FB      BNE:rel    Branch->$DA19
DA1E E6:3C      INC:zp     Zp RAM 003C
DA20 8D:00 50   STA:abs    $5000	 ; Clear the watchdog timer.
DA23 C6:38      DEC:zp     Zp RAM 0038
DA25 D0:F2      BNE:rel    Branch->$DA19
DA27 95:7D      STA:zp,x   Zp RAM 007D
DA29 E8:        INX:imp    X=X+1
DA2A E0:02      CPX:imm    #02
DA2C D0:04      BNE:rel    Branch->$DA32
DA2E A9:90      LDA:imm    #90
DA30 85:3C      STA:zp     Zp RAM 003C
DA32 E0:0C      CPX:imm    #0C
DA34 90:DE      BCC:rel    Branch->$DA14
DA36 A5:7D      LDA:zp     Zp RAM 007D
DA38 F0:0A      BEQ:rel    Branch->$DA44
DA3A A9:40      LDA:imm    #40
DA3C A2:A4      LDX:imm    #A4
DA3E 8D:C4 60   STA:abs    $60C4
DA41 8E:C5 60   STX:abs    $60C5
DA44 A2:05      LDX:imm    #05
DA46 AD:CA 60   LDA:abs    $60CA	 ; Grab a random number 0-FF
DA49 CD:CA 60   CMP:abs    $60CA	 ; Comapre to random 0-FF
DA4C D0:05      BNE:rel    Branch->$DA53 ; 254/255 chance of skipping
DA4E CA:        DEX:imp    X=X-1
DA4F 10:F8      BPL:rel    Branch->$DA49
DA51 85:7A      STA:zp     Zp RAM 007A
DA53 A2:05      LDX:imm    #05
DA55 AD:DA 60   LDA:abs    $60DA	 ; Randomize the accumulator
DA58 CD:DA 60   CMP:abs    $60DA	 ; Compare w/random number.
DA5B D0:05      BNE:rel    Branch->$DA62 ; 254/255 chance of skipping
DA5D CA:        DEX:imp    X=X-1
DA5E 10:F8      BPL:rel    Branch->$DA58
DA60 85:7B      STA:zp     Zp RAM 007B
DA62 20:11 DE   JSR:abs    $DE11
DA65 A0:02      LDY:imm    #02
DA67 AD:C9 01   LDA:abs    $01C9
DA6A F0:0A      BEQ:rel    Branch->$DA76
DA6C 85:7C      STA:zp     Zp RAM 007C
DA6E 20:F1 DD   JSR:abs    $DDF1
DA71 A0:00      LDY:imm    #00
DA73 8C:C9 01   STY:abs    $01C9
DA76 84:00      STY:zp     Zp RAM 0000
DA78 A2:07      LDX:imm    #07		 ;
DA7A BD:F9 DA   LDA:abs,x  $DAF9,X       ; Load the Color RAM from a table.
DA7D 9D:00 08   STA:abs,x  $0800,X       ; (8 bytes)
DA80 CA:        DEX:imp    X=X-1	 ;
DA81 10:F7      BPL:rel    Branch->$DA7A ;
DA83 A9:00      LDA:imm    #00
DA85 8D:E0 60   STA:abs    $60E0         ; Turn off player 1 and player 2 LED.
DA88 A9:10      LDA:imm    #10
DA8A 8D:00 40   STA:abs    $4000         ; Invert video Y axis.
DA8D A0:04      LDY:imm    #04
DA8F A2:14      LDX:imm    #14
DA91 2C:00 0C   BIT:abs    $0C00         ; Loop until the 3kHz square
DA94 10:FB      BPL:rel    Branch->$DA91 ; wave signal is positive.
DA96 2C:00 0C   BIT:abs    $0C00         ; Loop until the 3kHz square
DA99 30:FB      BMI:rel    Branch->$DA96 ; wave signal is negative.
DA9B CA:        DEX:imp    X=X-1
DA9C 10:F3      BPL:rel    Branch->$DA91
DA9E 88:        DEY:imp    Y=Y-1
DA9F 30:08      BMI:rel    Branch->$DAA9
DAA1 8D:00 50   STA:abs    $5000         ; Reset the watchdog timer.
DAA4 2C:00 0C   BIT:abs    $0C00         : If the vector machine is in
DAA7 50:E6      BVC:rel    Branch->$DA8F : HALT mode, then loop.
DAA9 8D:00 58   STA:abs    $5800         : Reset the vector state machine.
DAAC A9:00      LDA:imm    #00
DAAE 85:74      STA:zp     Zp RAM 0074
DAB0 A9:20      LDA:imm    #20
DAB2 85:75      STA:zp     Zp RAM 0075
DAB4 8D:CB 60   STA:abs    $60CB	 ; Rescan Pokey #1's pots.
DAB7 AD:C8 60   LDA:abs    $60C8	 ; Grab the binary pot values.
DABA 85:52      STA:zp     Zp RAM 0052	 ; Store in $0052.
DABC 29:0F      AND:imm    #0F		 ; Put the encoder wheel contents
DABE 85:50      STA:zp     Zp RAM 0050	 ; into $0050.
DAC0 AD:00 0C   LDA:abs    $0C00         ; Invert the 0C00 register and
DAC3 49:FF      EOR:imm    #FF           ; remove the HALT and 3kHz square.
DAC5 29:2F      AND:imm    #2F           ; Store in 004E.
DAC7 85:4E      STA:zp     Zp RAM 004E   ; 
DAC9 29:28      AND:imm    #28
DACB F0:0B      BEQ:rel    Branch->$DAD8
DACD 06:4C      ASL:zp     Zp RAM 004C
DACF 90:04      BCC:rel    Branch->$DAD5
DAD1 E6:00      INC:zp     Zp RAM 0000
DAD3 E6:00      INC:zp     Zp RAM 0000
DAD5 B8:        CLV:imp    Clear Overflow
DAD6 50:04      BVC:rel    Branch->$DADC
DAD8 A9:20      LDA:imm    #20
DADA 85:4C      STA:zp     Zp RAM 004C
DADC 20:0F DB   JSR:abs    $DB0F
DADF 20:0D DF   JSR:abs    $DF0D
DAE2 8D:00 48   STA:abs    $4800         ; Vector machine GO
DAE5 E6:03      INC:zp     Zp RAM 0003
DAE7 A5:03      LDA:zp     Zp RAM 0003
DAE9 29:03      AND:imm    #03
DAEB D0:03      BNE:rel    Branch->$DAF0
DAED 20:1B DE   JSR:abs    $DE1B
DAF0 AD:00 0C   LDA:abs    $0C00	 ; Grab the TEST SWITCH status.
DAF3 29:10      AND:imm    #10		 ; Is the game in test mode?
DAF5 F0:96      BEQ:rel    Branch->$DA8D ; If not, $DA8D.
DAF7 D0:FE      BNE:rel    Branch->$DAF7 ; If so, Infinite loop! YOW!
;
; Data used to populate Color RAM (code at $DA7A)
;
DAF9 00:04      DATA
DAFB 08:0C      DATA
DAFD 03:07      DATA
DAFF 0B:0B      DATA
; 
; Data to put onto stack via subroutine at $DB0F.
;
DB01 59:DB      DATA
DB03 F6:DB      DATA
DB05 83:DB      DATA
DB07 99:DB      DATA
DB09 7D:DB      DATA
DB0B 6E:DB      DATA
DB0D 21:DB      DATA
;
DB0F A6:00      LDX:zp     Zp RAM 0000
DB11 E0:0E      CPX:imm    #0E
DB13 90:04      BCC:rel    Branch->$DB19
DB15 A2:02      LDX:imm    #02		 ; Set game mode to ROM TEST
DB17 86:00      STX:zp     Zp RAM 0000	 ; screen.
DB19 BD:02 DB   LDA:abs,x  $DB02,X
DB1C 48:        PHA:imp    Push Accum
DB1D BD:01 DB   LDA:abs,x  $DB01,X
DB20 48:        PHA:imp    Push Accum
DB21 60:        RTS:imp    Ret from Sub
;
; Unknown initialization routine.
;
DB22 A9:00      LDA:imm    #00
DB24 8D:E0 60   STA:abs    $60E0         ; Reset the player 1 and 2 LEDs.
DB27 8D:80 60   STA:abs    $6080
DB2A 8D:C0 60   STA:abs    $60C0
DB2D 8D:D0 60   STA:abs    $60D0
DB30 8D:00 60   STA:abs    $6000	; Latch the EEPROM onto the bus?
DB33 8D:40 60   STA:abs    $6040	; Huh? Write to mathbox status reg?
DB36 AD:40 60   LDA:abs    $6040	; Read mathbox status.
DB39 AD:60 60   LDA:abs    $6060
DB3C AD:70 60   LDA:abs    $6070
DB3F AD:50 60   LDA:abs    $6050
DB42 A9:08      LDA:imm    #08
DB44 8D:E0 60   STA:abs    $60E0
DB47 A9:01      LDA:imm    #01
DB49 A2:1F      LDX:imm    #1F
DB4B 18:        CLC:imp    Clear Carry
DB4C 9D:80 60   STA:abs,x  $6080,X
DB4F 2A:        ROL:accum  
DB50 CA:        DEX:imp    X=X-1
DB51 10:F9      BPL:rel    Branch->$DB4C
DB53 A9:34      LDA:imm    #34
DB55 A2:A6      LDX:imm    #A6
DB57 4C:39 DF   JMP:abs    $DF39
DB5A AD:CA 01   LDA:abs    $01CA
DB5D 0D:C7 01   ORA:abs    $01C7
DB60 D0:0C      BNE:rel    Branch->$DB6E
DB62 20:11 DE   JSR:abs    $DE11
DB65 AD:C9 01   LDA:abs    $01C9
DB68 85:7C      STA:zp     Zp RAM 007C
DB6A A9:02      LDA:imm    #02		 ; Set game mode to ROM TEST
DB6C 85:00      STA:zp     Zp RAM 0000	 ; screen.
DB6E 60:        RTS:imp    Ret from Sub
DB6F A5:50      LDA:zp     Zp RAM 0050
DB71 4A:        LSR:accum  
DB72 A8:        TAY:imp    A-->Y
DB73 A9:68      LDA:imm    #68
DB75 20:4C DF   JSR:abs    $DF4C
DB78 A2:4E      LDX:imm    #4E
DB7A A9:33      LDA:imm    #33
DB7C D0:0A      BNE:rel    Branch->$DB88
DB7E A2:B6      LDX:imm    #B6
DB80 A9:32      LDA:imm    #32
DB82 D0:04      BNE:rel    Branch->$DB88
DB84 A9:33      LDA:imm    #33
DB86 A2:0A      LDX:imm    #0A
DB88 20:39 DF   JSR:abs    $DF39
DB8B A2:06      LDX:imm    #06
DB8D A9:00      LDA:imm    #00
DB8F 9D:C1 60   STA:abs,x  $60C1,X
DB92 9D:D1 60   STA:abs,x  $60D1,X
DB95 CA:        DEX:imp    X=X-1
DB96 CA:        DEX:imp    X=X-1
DB97 10:F6      BPL:rel    Branch->$DB8F
DB99 60:        RTS:imp    Ret from Sub
;
DB9A A5:03      LDA:zp     Zp RAM 0003
DB9C 29:3F      AND:imm    #3F
DB9E D0:02      BNE:rel    Branch->$DBA2
DBA0 E6:39      INC:zp     Zp RAM 0039
DBA2 A5:39      LDA:zp     Zp RAM 0039
DBA4 29:07      AND:imm    #07
DBA6 AA:        TAX:imp    A-->X
DBA7 BC:D5 DB   LDY:abs,x  $DBD5,X
DBAA A9:00      LDA:imm    #00
DBAC 99:C1 60   STA:abs,y  $60C1,Y
DBAF BC:D6 DB   LDY:abs,x  $DBD6,X
DBB2 BD:DC DF   LDA:abs,x  $DFDC,X
DBB5 99:C0 60   STA:abs,y  $60C0,Y
DBB8 A9:A8      LDA:imm    #A8
DBBA 99:C1 60   STA:abs,y  $60C1,Y
DBBD A9:34      LDA:imm    #34
DBBF A2:56      LDX:imm    #56
DBC1 20:39 DF   JSR:abs    $DF39
DBC4 A5:03      LDA:zp     Zp RAM 0003
DBC6 29:7F      AND:imm    #7F
DBC8 A8:        TAY:imp    A-->Y
DBC9 A9:01      LDA:imm    #01
DBCB 20:6C DF   JSR:abs    $DF6C
DBCE A9:34      LDA:imm    #34
DBD0 A2:AA      LDX:imm    #AA
DBD2 4C:39 DF   JMP:abs    $DF39
;
; DATA Table
;
DBD5 16:00      ASL:zp,x   Zp RAM 0000
DBD7 10:02      BPL:rel    Branch->$DBDB
DBD9 12:        Illegal Opcode
DBDA 04:        Illegal Opcode
DBDB 14:        Illegal Opcode
DBDC 06:16      ASL:zp     Zp RAM 0016
DBDE 00:        BRK:imp    BREAK
DBDF EA:        NOP:imp    No Operation
;
; Obtain the settings for D/E2 and store into scratchpad address $0037.
; Bit 0=Switch 2   Bit 1=Switch 2  Bit 2=Switch 3  Bit 3=Switch 1
;
DBE0 8D:DB 60   STA:abs    $60DB	 ; POTGO - Rescan Pokey #2's POTs
DBE3 AD:D8 60   LDA:abs    $60D8	 ; ALLPOT - Read Pokey #2''s POTs
DBE6 29:07      AND:imm    #07		 ; Just grab switches 2-4 at 
					 ; switch D/E2 on the AUX board.
DBE8 85:37      STA:zp     Zp RAM 0037	 ; Store into a memory address.
DBEA 8D:CB 60   STA:abs    $60CB	 ; POTGO - Rescan Pokey #1's POTs
DBED AD:C8 60   LDA:abs    $60C8	 ; ALLPOT - Read Pokey #1's POTs.
DBF0 29:20      AND:imm    #20		 ; Isolate switch #1 and shift
DBF2 4A:        LSR:accum  		 ; to the right so it has the
DBF3 4A:        LSR:accum  		 ; value of 8 when set.
DBF4 05:37      ORA:zp     Zp RAM 0037	 ; OR is into #0037,
DBF6 60:        RTS:imp    Ret from Sub  ; Return.
;
DBF7 A5:2E      LDA:zp     Zp RAM 002E
DBF9 F0:1E      BEQ:rel    Branch->$DC19
DBFB 8D:95 60   STA:abs    $6095
DBFE 8D:8D 60   STA:abs    $608D
DC01 A5:2F      LDA:zp     Zp RAM 002F
DC03 8D:96 60   STA:abs    $6096
DC06 A2:00      LDX:imm    #00
DC08 20:E6 DC   JSR:abs    $DCE6
DC0B C9:01      CMP:imm    #01
DC0D D0:06      BNE:rel    Branch->$DC15
DC0F 98:        TYA:imp    Y-->A
DC10 D0:03      BNE:rel    Branch->$DC15
DC12 8A:        TXA:imp    X-->A
DC13 10:04      BPL:rel    Branch->$DC19
DC15 A9:FF      LDA:imm    #FF
DC17 85:78      STA:zp     Zp RAM 0078
DC19 A2:00      LDX:imm    #00
DC1B 86:73      STX:zp     Zp RAM 0073
DC1D E6:2E      INC:zp     Zp RAM 002E
DC1F D0:06      BNE:rel    Branch->$DC27
DC21 E6:2F      INC:zp     Zp RAM 002F
DC23 10:02      BPL:rel    Branch->$DC27
DC25 86:2F      STX:zp     Zp RAM 002F
DC27 8D:DB 60   STA:abs    $60DB
DC2A AD:D8 60   LDA:abs    $60D8
DC2D 29:78      AND:imm    #78
DC2F 85:4D      STA:zp     Zp RAM 004D
DC31 F0:05      BEQ:rel    Branch->$DC38
DC33 8D:C0 60   STA:abs    $60C0
DC36 A2:A4      LDX:imm    #A4
DC38 8E:C1 60   STX:abs    $60C1
DC3B A2:00      LDX:imm    #00
DC3D A5:4E      LDA:zp     Zp RAM 004E
DC3F F0:06      BEQ:rel    Branch->$DC47
DC41 0A:        ASL:accum  
DC42 8D:C2 60   STA:abs    $60C2
DC45 A2:A4      LDX:imm    #A4
DC47 8E:C3 60   STX:abs    $60C3
DC4A 20:0D DD   JSR:abs    $DD0D
DC4D A4:4D      LDY:zp     Zp RAM 004D
DC4F A9:D0      LDA:imm    #D0
DC51 A2:F0      LDX:imm    #F0
DC53 20:2B DD   JSR:abs    $DD2B
DC56 A4:4E      LDY:zp     Zp RAM 004E
DC58 20:27 DD   JSR:abs    $DD27
DC5B A5:52      LDA:zp     Zp RAM 0052
DC5D 29:10      AND:imm    #10
DC5F F0:1D      BEQ:rel    Branch->$DC7E
DC61 A9:34      LDA:imm    #34
DC63 A2:82      LDX:imm    #82
DC65 20:39 DF   JSR:abs    $DF39
DC68 A0:10      LDY:imm    #10
DC6A A5:4D      LDA:zp     Zp RAM 004D
DC6C 29:60      AND:imm    #60
DC6E F0:0E      BEQ:rel    Branch->$DC7E
DC70 49:20      EOR:imm    #20
DC72 F0:04      BEQ:rel    Branch->$DC78
DC74 A9:04      LDA:imm    #04
DC76 A0:08      LDY:imm    #08
DC78 8D:E0 60   STA:abs    $60E0
DC7B 8C:00 40   STY:abs    $4000         ; Invert video X axis.
DC7E A9:34      LDA:imm    #34
DC80 A2:92      LDX:imm    #92
DC82 20:39 DF   JSR:abs    $DF39
DC85 A2:0B      LDX:imm    #0B
DC87 B5:7D      LDA:zp,x   Zp RAM 007D
DC89 F0:19      BEQ:rel    Branch->$DCA4
DC8B 85:35      STA:zp     Zp RAM 0035
DC8D 86:38      STX:zp     Zp RAM 0038
DC8F 8A:        TXA:imp    X-->A
DC90 20:1F DF   JSR:abs    $DF1F
DC93 A0:F4      LDY:imm    #F4
DC95 A2:F4      LDX:imm    #F4
DC97 A5:35      LDA:zp     Zp RAM 0035
DC99 20:A9 D8   JSR:abs    $D8A9
DC9C A9:0C      LDA:imm    #0C
DC9E AA:        TAX:imp    A-->X
DC9F 20:75 DF   JSR:abs    $DF75
DCA2 A6:38      LDX:zp     Zp RAM 0038
DCA4 CA:        DEX:imp    X=X-1
DCA5 10:E0      BPL:rel    Branch->$DC87
DCA7 20:53 DF   JSR:abs    $DF53
DCAA A9:00      LDA:imm    #00
DCAC A2:16      LDX:imm    #16
DCAE 20:75 DF   JSR:abs    $DF75
DCB1 A2:04      LDX:imm    #04
DCB3 86:37      STX:zp     Zp RAM 0037
DCB5 A6:37      LDX:zp     Zp RAM 0037
DCB7 A0:00      LDY:imm    #00
DCB9 B5:78      LDA:zp,x   Zp RAM 0078
DCBB F0:03      BEQ:rel    Branch->$DCC0
DCBD BC:E1 DC   LDY:abs,x  $DCE1,X
DCC0 B9:E4 31   LDA:abs,y  $31E4,Y
DCC3 BE:E5 31   LDX:abs,y  $31E5,Y
DCC6 20:57 DF   JSR:abs    $DF57
DCC9 C6:37      DEC:zp     Zp RAM 0037
DCCB 10:E8      BPL:rel    Branch->$DCB5
DCCD A2:AC      LDX:imm    #AC
DCCF A9:30      LDA:imm    #30
DCD1 20:75 DF   JSR:abs    $DF75
DCD4 A4:50      LDY:zp     Zp RAM 0050
DCD6 B9:E8 DF   LDA:abs,y  $DFE8,Y
DCD9 BE:E4 DF   LDX:abs,y  $DFE4,Y
DCDC A0:C0      LDY:imm    #C0
DCDE 4C:73 DF   JMP:abs    $DF73
DCE1 2E:38 34   ROL:abs    $3438
DCE4 36:1E      ROL:zp,x   Zp RAM 001E
DCE6 A0:00      LDY:imm    #00
DCE8 84:73      STY:zp     Zp RAM 0073
DCEA 8C:14 04   STY:abs    $0414
DCED 8D:8E 60   STA:abs    $608E         ; Mathbox stuff.
DCF0 8E:8F 60   STX:abs    $608F
DCF3 8C:90 60   STY:abs    $6090
DCF6 A2:10      LDX:imm    #10
DCF8 8E:8C 60   STX:abs    $608C
DCFB 8E:94 60   STX:abs    $6094
DCFE CA:        DEX:imp    X=X-1
DCFF 30:0B      BMI:rel    Branch->$DD0C
DD01 AD:40 60   LDA:abs    $6040	 ; Read mathbox status reg
DD04 30:F8      BMI:rel    Branch->$DCFE
DD06 AD:60 60   LDA:abs    $6060
DD09 AC:70 60   LDY:abs    $6070
DD0C 60:        RTS:imp    Ret from Sub
;
DD0D 20:53 DF   JSR:abs    $DF53
DD10 A9:00      LDA:imm    #00
DD12 20:6A DF   JSR:abs    $DF6A
DD15 A9:E8      LDA:imm    #E8
DD17 AC:00 0D   LDY:abs    $0D00
DD1A 20:29 DD   JSR:abs    $DD29
DD1D AC:00 0E   LDY:abs    $0E00
DD20 20:27 DD   JSR:abs    $DD27
DD23 20:E0 DB   JSR:abs    $DBE0	 ; Read switch D/E2 (special format)
					 ; and store into $0037.
DD26 A8:        TAY:imp    A-->Y
DD27 A9:D0      LDA:imm    #D0
DD29 A2:F8      LDX:imm    #F8
DD2B 84:35      STY:zp     Zp RAM 0035
DD2D 20:75 DF   JSR:abs    $DF75
DD30 A2:07      LDX:imm    #07
DD32 86:37      STX:zp     Zp RAM 0037
DD34 06:35      ASL:zp     Zp RAM 0035
DD36 A9:00      LDA:imm    #00
DD38 2A:        ROL:accum  
DD39 20:1F DF   JSR:abs    $DF1F
DD3C C6:37      DEC:zp     Zp RAM 0037
DD3E 10:F4      BPL:rel    Branch->$DD34
DD40 60:        RTS:imp    Ret from Sub
DD41 AD:0F 04   LDA:abs    $040F
DD44 0A:        ASL:accum  
DD45 85:29      STA:zp     Zp RAM 0029
DD47 AD:10 04   LDA:abs    $0410
DD4A 2A:        ROL:accum  
DD4B 85:2A      STA:zp     Zp RAM 002A
DD4D AD:0C 04   LDA:abs    $040C
DD50 18:        CLC:imp    Clear Carry
DD51 65:29      ADC:zp     Zp RAM 0029
DD53 8D:95 60   STA:abs    $6095
DD56 85:29      STA:zp     Zp RAM 0029
DD58 AD:0D 04   LDA:abs    $040D
DD5B 65:2A      ADC:zp     Zp RAM 002A
DD5D 8D:96 60   STA:abs    $6096
DD60 05:29      ORA:zp     Zp RAM 0029
DD62 D0:05      BNE:rel    Branch->$DD69
DD64 A9:01      LDA:imm    #01
DD66 8D:95 60   STA:abs    $6095         ; More mathbox stuff.
DD69 AD:09 04   LDA:abs    $0409
DD6C 8D:8D 60   STA:abs    $608D
DD6F AD:0A 04   LDA:abs    $040A
DD72 AE:0B 04   LDX:abs    $040B
DD75 20:E6 DC   JSR:abs    $DCE6
DD78 8D:12 04   STA:abs    $0412
DD7B 8C:13 04   STY:abs    $0413
DD7E A9:3D      LDA:imm    #3D
DD80 A2:CE      LDX:imm    #CE
DD82 20:39 DF   JSR:abs    $DF39
DD85 A9:06      LDA:imm    #06
DD87 85:3B      STA:zp     Zp RAM 003B
DD89 A9:04      LDA:imm    #04
DD8B 85:3C      STA:zp     Zp RAM 003C
DD8D 85:37      STA:zp     Zp RAM 0037
DD8F A0:00      LDY:imm    #00
DD91 84:31      STY:zp     Zp RAM 0031
DD93 84:32      STY:zp     Zp RAM 0032
DD95 84:33      STY:zp     Zp RAM 0033
DD97 84:34      STY:zp     Zp RAM 0034
DD99 B1:3B      LDA:ind,y  3B
DD9B 85:56      STA:zp     Zp RAM 0056
DD9D E6:3B      INC:zp     Zp RAM 003B
DD9F B1:3B      LDA:ind,y  3B
DDA1 85:57      STA:zp     Zp RAM 0057
DDA3 E6:3B      INC:zp     Zp RAM 003B
DDA5 B1:3B      LDA:ind,y  3B
DDA7 85:58      STA:zp     Zp RAM 0058
DDA9 E6:3B      INC:zp     Zp RAM 003B
DDAB F8:        SED:imp    Set Decimal
DDAC A0:17      LDY:imm    #17
DDAE 84:38      STY:zp     Zp RAM 0038
DDB0 26:56      ROL:zp     Zp RAM 0056
DDB2 26:57      ROL:zp     Zp RAM 0057
DDB4 26:58      ROL:zp     Zp RAM 0058
DDB6 A0:03      LDY:imm    #03
DDB8 A2:00      LDX:imm    #00
DDBA B5:31      LDA:zp,x   Zp RAM 0031
DDBC 75:31      ADC:zp,x   Zp RAM 0031
DDBE 95:31      STA:zp,x   Zp RAM 0031
DDC0 E8:        INX:imp    X=X+1
DDC1 88:        DEY:imp    Y=Y-1
DDC2 10:F6      BPL:rel    Branch->$DDBA
DDC4 C6:38      DEC:zp     Zp RAM 0038
DDC6 10:E8      BPL:rel    Branch->$DDB0
DDC8 D8:        CLD:imp    Unset Decimal
DDC9 A9:31      LDA:imm    #31
DDCB A0:04      LDY:imm    #04
DDCD 20:B1 DF   JSR:abs    $DFB1
DDD0 A9:D0      LDA:imm    #D0
DDD2 A2:F8      LDX:imm    #F8
DDD4 20:75 DF   JSR:abs    $DF75
DDD7 C6:37      DEC:zp     Zp RAM 0037
DDD9 10:B4      BPL:rel    Branch->$DD8F
DDDB 60:        RTS:imp    Ret from Sub
DDDC 73:        Illegal Opcode
DDDD 00:        BRK:imp    BREAK
DDDE 09:0A      ORA:imm    #0A
DDE0 15:16      ORA:zp,x   Zp RAM 0016
DDE2 22:        Illegal Opcode
DDE3 15:06      ORA:zp,x   Zp RAM 0006
DDE5 15:07      ORA:zp,x   Zp RAM 0007
DDE7 06:04      ASL:zp     Zp RAM 0004
DDE9 A9:04      LDA:imm    #04
DDEB D0:06      BNE:rel    Branch->$DDF3
DDED A9:03      LDA:imm    #03
DDEF D0:02      BNE:rel    Branch->$DDF3
DDF1 A9:07      LDA:imm    #07
DDF3 A0:FF      LDY:imm    #FF
DDF5 D0:08      BNE:rel    Branch->$DDFF
DDF7 A9:03      LDA:imm    #03
DDF9 D0:02      BNE:rel    Branch->$DDFD
DDFB A9:04      LDA:imm    #04
DDFD A0:00      LDY:imm    #00
DDFF 8C:C6 01   STY:abs    $01C6
DE02 48:        PHA:imp    Push Accum
DE03 0D:C7 01   ORA:abs    $01C7
DE06 8D:C7 01   STA:abs    $01C7
DE09 68:        PLA:imp    Pull Accum
DE0A 0D:C8 01   ORA:abs    $01C8
DE0D 8D:C8 01   STA:abs    $01C8
DE10 60:        RTS:imp    Ret from Sub
DE11 A9:07      LDA:imm    #07
DE13 8D:C7 01   STA:abs    $01C7
DE16 A9:00      LDA:imm    #00
DE18 8D:C8 01   STA:abs    $01C8
DE1B AD:CA 01   LDA:abs    $01CA
DE1E D0:4B      BNE:rel    Branch->$DE6B
DE20 AD:C7 01   LDA:abs    $01C7
DE23 F0:46      BEQ:rel    Branch->$DE6B
DE25 A2:00      LDX:imm    #00
DE27 8E:CB 01   STX:abs    $01CB
DE2A 8E:CF 01   STX:abs    $01CF
DE2D 8E:CE 01   STX:abs    $01CE
DE30 A2:08      LDX:imm    #08
DE32 38:        SEC:imp    Set Carry
DE33 6E:CE 01   ROR:abs    $01CE
DE36 0A:        ASL:accum  
DE37 CA:        DEX:imp    X=X-1
DE38 90:F9      BCC:rel    Branch->$DE33
DE3A A0:80      LDY:imm    #80
DE3C AD:CE 01   LDA:abs    $01CE
DE3F 2D:C8 01   AND:abs    $01C8
DE42 D0:02      BNE:rel    Branch->$DE46
DE44 A0:20      LDY:imm    #20
DE46 8C:CA 01   STY:abs    $01CA
DE49 AD:CE 01   LDA:abs    $01CE
DE4C 4D:C7 01   EOR:abs    $01C7
DE4F 8D:C7 01   STA:abs    $01C7
DE52 8A:        TXA:imp    X-->A
DE53 0A:        ASL:accum  
DE54 AA:        TAX:imp    A-->X
DE55 BD:DD DD   LDA:abs,x  $DDDD,X
DE58 8D:CC 01   STA:abs    $01CC
DE5B BD:DE DD   LDA:abs,x  $DDDE,X
DE5E 8D:CD 01   STA:abs    $01CD
DE61 BD:E3 DD   LDA:abs,x  $DDE3,X
DE64 85:BD      STA:zp     Zp RAM 00BD
DE66 BD:E4 DD   LDA:abs,x  $DDE4,X
DE69 85:BE      STA:zp     Zp RAM 00BE
DE6B A0:00      LDY:imm    #00
DE6D 8C:40 60   STY:abs    $6040
DE70 AD:CA 01   LDA:abs    $01CA
DE73 D0:01      BNE:rel    Branch->$DE76
DE75 60:        RTS:imp    Ret from Sub
DE76 AC:CB 01   LDY:abs    $01CB
DE79 AE:CC 01   LDX:abs    $01CC
DE7C 0A:        ASL:accum  
DE7D 90:0D      BCC:rel    Branch->$DE8C
DE7F 9D:00 60   STA:abs,x  $6000,X	 ; Store something on the EEPROM?
DE82 A9:40      LDA:imm    #40
DE84 8D:CA 01   STA:abs    $01CA
DE87 A0:0E      LDY:imm    #0E
DE89 B8:        CLV:imp    Clear Overflow
DE8A 50:73      BVC:rel    Branch->$DEFF
DE8C 10:25      BPL:rel    Branch->$DEB3
DE8E A9:80      LDA:imm    #80
DE90 8D:CA 01   STA:abs    $01CA
DE93 AD:C6 01   LDA:abs    $01C6
DE96 F0:04      BEQ:rel    Branch->$DE9C
DE98 A9:00      LDA:imm    #00
DE9A 91:BD      STA:ind,y  BD
DE9C B1:BD      LDA:ind,y  BD
DE9E EC:CD 01   CPX:abs    $01CD
DEA1 90:08      BCC:rel    Branch->$DEAB
DEA3 A9:00      LDA:imm    #00
DEA5 8D:CA 01   STA:abs    $01CA
DEA8 AD:CF 01   LDA:abs    $01CF
DEAB 9D:00 60   STA:abs,x  $6000,X	 ; Load something on the EEPROM?
DEAE A0:0C      LDY:imm    #0C
DEB0 B8:        CLV:imp    Clear Overflow
DEB1 50:3F      BVC:rel    Branch->$DEF2
DEB3 A9:08      LDA:imm    #08
DEB5 8D:40 60   STA:abs    $6040	 ; Work the EEPROM control
DEB8 9D:00 60   STA:abs,x  $6000,X	 ; Load something on the EEPROM?
DEBB A9:09      LDA:imm    #09 		 ; Work the eeprom control.
DEBD 8D:40 60   STA:abs    $6040	 ;
DEC0 EA:        NOP:imp    No Operation  ; Wait.
DEC1 A9:08      LDA:imm    #08           ; Work the EEPROM control.
DEC3 8D:40 60   STA:abs    $6040	 ;
DEC6 EC:CD 01   CPX:abs    $01CD	 ;
DEC9 AD:50 60   LDA:abs    $6050	 ; Read the EEPROM read control.
DECC 90:20      BCC:rel    Branch->$DEEE
DECE 4D:CF 01   EOR:abs    $01CF
DED1 F0:13      BEQ:rel    Branch->$DEE6
DED3 A9:00      LDA:imm    #00
DED5 AC:CB 01   LDY:abs    $01CB
DED8 91:BD      STA:ind,y  BD
DEDA 88:        DEY:imp    Y=Y-1
DEDB 10:FB      BPL:rel    Branch->$DED8
DEDD AD:CE 01   LDA:abs    $01CE
DEE0 0D:C9 01   ORA:abs    $01C9
DEE3 8D:C9 01   STA:abs    $01C9
DEE6 A9:00      LDA:imm    #00
DEE8 8D:CA 01   STA:abs    $01CA
DEEB B8:        CLV:imp    Clear Overflow
DEEC 50:02      BVC:rel    Branch->$DEF0
DEEE 91:BD      STA:ind,y  BD
DEF0 A0:00      LDY:imm    #00
DEF2 18:        CLC:imp    Clear Carry
DEF3 6D:CF 01   ADC:abs    $01CF
DEF6 8D:CF 01   STA:abs    $01CF
DEF9 EE:CB 01   INC:abs    $01CB
DEFC EE:CC 01   INC:abs    $01CC
DEFF 8C:40 60   STY:abs    $6040
DF02 98:        TYA:imp    Y-->A
DF03 D0:03      BNE:rel    Branch->$DF08
DF05 4C:1B DE   JMP:abs    $DE1B

DF08 60:        RTS:imp    Ret from Sub
;
DF09 A9:C0      LDA:imm    #C0
DF0B D0:05      BNE:rel    Branch->$DF12
DF0D 20:53 DF   JSR:abs    $DF53
DF10 A9:20      LDA:imm    #20
DF12 A0:00      LDY:imm    #00
DF14 91:74      STA:ind,y  74
DF16 4C:AC DF   JMP:abs    $DFAC
DF19 90:04      BCC:rel    Branch->$DF1F
DF1B 29:0F      AND:imm    #0F
DF1D F0:05      BEQ:rel    Branch->$DF24
DF1F 29:0F      AND:imm    #0F
DF21 18:        CLC:imp    Clear Carry
DF22 69:01      ADC:imm    #01
DF24 08:        PHP:imp    Push P
DF25 0A:        ASL:accum  
DF26 A0:00      LDY:imm    #00
DF28 AA:        TAX:imp    A-->X
DF29 BD:E4 31   LDA:abs,x  $31E4,X
DF2C 91:74      STA:ind,y  74
DF2E BD:E5 31   LDA:abs,x  $31E5,X
DF31 C8:        INY:imp    Y=Y+1
DF32 91:74      STA:ind,y  74
DF34 20:5F DF   JSR:abs    $DF5F
DF37 28:        PLP:imp    Pull P
DF38 60:        RTS:imp    Ret from Sub
DF39 4A:        LSR:accum  
DF3A 29:0F      AND:imm    #0F
DF3C 09:A0      ORA:imm    #A0
DF3E A0:01      LDY:imm    #01
DF40 91:74      STA:ind,y  74
DF42 88:        DEY:imp    Y=Y-1
DF43 8A:        TXA:imp    X-->A
DF44 6A:        ROT:accum  
DF45 91:74      STA:ind,y  74
DF47 C8:        INY:imp    Y=Y+1
DF48 D0:15      BNE:rel    Branch->$DF5F
DF4A A4:73      LDY:zp     Zp RAM 0073
DF4C 09:60      ORA:imm    #60
DF4E AA:        TAX:imp    A-->X
DF4F 98:        TYA:imp    Y-->A
DF50 4C:57 DF   JMP:abs    $DF57
DF53 A9:40      LDA:imm    #40
DF55 A2:80      LDX:imm    #80
DF57 A0:00      LDY:imm    #00
DF59 91:74      STA:ind,y  74
DF5B C8:        INY:imp    Y=Y+1
DF5C 8A:        TXA:imp    X-->A
DF5D 91:74      STA:ind,y  74
DF5F 98:        TYA:imp    Y-->A
DF60 38:        SEC:imp    Set Carry
DF61 65:74      ADC:zp     Zp RAM 0074
DF63 85:74      STA:zp     Zp RAM 0074
DF65 90:02      BCC:rel    Branch->$DF69
DF67 E6:75      INC:zp     Zp RAM 0075
DF69 60:        RTS:imp    Ret from Sub
DF6A A0:00      LDY:imm    #00
DF6C 09:70      ORA:imm    #70
DF6E AA:        TAX:imp    A-->X
DF6F 98:        TYA:imp    Y-->A
DF70 4C:57 DF   JMP:abs    $DF57
DF73 84:73      STY:zp     Zp RAM 0073
DF75 A0:00      LDY:imm    #00
DF77 0A:        ASL:accum  
DF78 90:01      BCC:rel    Branch->$DF7B
DF7A 88:        DEY:imp    Y=Y-1
DF7B 84:6F      STY:zp     Zp RAM 006F
DF7D 0A:        ASL:accum  
DF7E 26:6F      ROL:zp     Zp RAM 006F
DF80 85:6E      STA:zp     Zp RAM 006E
DF82 8A:        TXA:imp    X-->A
DF83 0A:        ASL:accum  
DF84 A0:00      LDY:imm    #00
DF86 90:01      BCC:rel    Branch->$DF89
DF88 88:        DEY:imp    Y=Y-1
DF89 84:71      STY:zp     Zp RAM 0071
DF8B 0A:        ASL:accum  
DF8C 26:71      ROL:zp     Zp RAM 0071
DF8E 85:70      STA:zp     Zp RAM 0070
DF90 A2:6E      LDX:imm    #6E
DF92 A0:00      LDY:imm    #00
DF94 B5:02      LDA:zp,x   Zp RAM 0002
DF96 91:74      STA:ind,y  74
DF98 B5:03      LDA:zp,x   Zp RAM 0003
DF9A 29:1F      AND:imm    #1F
DF9C C8:        INY:imp    Y=Y+1
DF9D 91:74      STA:ind,y  74
DF9F B5:00      LDA:zp,x   Zp RAM 0000
DFA1 C8:        INY:imp    Y=Y+1
DFA2 91:74      STA:ind,y  74
DFA4 B5:01      LDA:zp,x   Zp RAM 0001
DFA6 45:73      EOR:zp     Zp RAM 0073
DFA8 29:1F      AND:imm    #1F
DFAA 45:73      EOR:zp     Zp RAM 0073
DFAC C8:        INY:imp    Y=Y+1
DFAD 91:74      STA:ind,y  74
DFAF D0:AE      BNE:rel    Branch->$DF5F
DFB1 38:        SEC:imp    Set Carry
DFB2 08:        PHP:imp    Push P
DFB3 88:        DEY:imp    Y=Y-1
DFB4 84:AE      STY:zp     Zp RAM 00AE
DFB6 18:        CLC:imp    Clear Carry
DFB7 65:AE      ADC:zp     Zp RAM 00AE
DFB9 28:        PLP:imp    Pull P
DFBA AA:        TAX:imp    A-->X
DFBB 08:        PHP:imp    Push P
DFBC 86:AF      STX:zp     Zp RAM 00AF
DFBE B5:00      LDA:zp,x   Zp RAM 0000
DFC0 4A:        LSR:accum  
DFC1 4A:        LSR:accum  
DFC2 4A:        LSR:accum  
DFC3 4A:        LSR:accum  
DFC4 28:        PLP:imp    Pull P
DFC5 20:19 DF   JSR:abs    $DF19
DFC8 A5:AE      LDA:zp     Zp RAM 00AE
DFCA D0:01      BNE:rel    Branch->$DFCD
DFCC 18:        CLC:imp    Clear Carry
DFCD A6:AF      LDX:zp     Zp RAM 00AF
DFCF B5:00      LDA:zp,x   Zp RAM 0000
DFD1 20:19 DF   JSR:abs    $DF19
DFD4 A6:AF      LDX:zp     Zp RAM 00AF
DFD6 CA:        DEX:imp    X=X-1
DFD7 C6:AE      DEC:zp     Zp RAM 00AE
DFD9 10:E0      BPL:rel    Branch->$DFBB
DFDB 60:        RTS:imp    Ret from Sub
;
DFDC 10:10      BPL:rel    Branch->$DFEE
DFDE 40:        RTI:imp    Ret from Int
DFDF 40:        RTI:imp    Ret from Int
DFE0 90:90      BCC:rel    Branch->$DF72
DFE2 FF:        Illegal Opcode
DFE3 FF:        Illegal Opcode
DFE4 00:        BRK:imp    BREAK
DFE5 0C:        Illegal Opcode
DFE6 16:1E      ASL:zp,x   Zp RAM 001E
DFE8 20:1E 16   JSR:abs    $161E
DFEB 0C:        Illegal Opcode
DFEC 00:        BRK:imp    BREAK
DFED F4:        Illegal Opcode
DFEE EA:        NOP:imp    No Operation
DFEF E2:        Illegal Opcode
DFF0 E0:E2      CPX:imm    #E2
DFF2 EA:        NOP:imp    No Operation
DFF3 F4:        Illegal Opcode
DFF4 00:        BRK:imp    BREAK
DFF5 0C:        Illegal Opcode
DFF6 16:1E      ASL:zp,x   Zp RAM 001E
DFF8 00:        BRK:imp    BREAK
DFF9 00:        BRK:imp    BREAK
DFFA 04:D7      DATA			 ; This is the non-maskable
					 ; interrupt vector. $D704.

DFFC 3F:D9      DATA			 ; This is the reset vector. When
					 ; the CPU powers up, it goes to
					 ; this address to find the
					 ; location of the first instruction
					 ; to execute, at $D93F.

DFFE 04 D7   	DATA			 ; This is the maskable interrupt
					 ; vector. $D704.
