*
*  MC6809 CPU Emulation Validation for DAA
*
* Tested on an SGS Thomson EF6809P Processor
*
* W. Schwotzer                     20.07.2003
*


WARMS  EQU   $CD03
PCRLF  EQU   $CD24
PUTCHR EQU   $CD18
PSTRNG EQU   $CD1E
OUTDEC EQU   $CD39
OUTHEX EQU   $CD3C
OUTADR EQU   $CD45
EXTREG EQU   $8000
DIRPAG EQU   $80
DPREG  EQU   $00
EOT    EQU   $4

       ORG   $C100

BEGIN  BRA   START
VERSIO FCB   1

START  CLR   COUNT
       LDX   #T1
       JSR   PSTRNG
       JSR   PCRLF
       LDY   #0
S1     TFR   Y,D
       TFR   B,A
       ANDCC #$9E
       ORCC  #$00
       DAA
       TFR   CC,B
       BSR   OUTA
       LEAY  1,Y
       CMPY  #256
       BNE   S1

       LDX   #T2
       JSR   PSTRNG
       JSR   PCRLF
       LDY   #0
S2     TFR   Y,D
       TFR   B,A
       ANDCC #$9E
       ORCC  #$01
       DAA
       TFR   CC,B
       BSR   OUTA
       LEAY  1,Y
       CMPY  #256
       BNE   S2

       LDX   #T3
       JSR   PSTRNG
       JSR   PCRLF
       LDY   #0
S3     TFR   Y,D
       TFR   B,A
       ANDCC #$FE
       ORCC  #$20
       DAA
       TFR   CC,B
       BSR   OUTA
       LEAY  1,Y
       CMPY  #256
       BNE   S3

       LDX   #T4
       JSR   PSTRNG
       JSR   PCRLF
       LDY   #0
S4     TFR   Y,D
       TFR   B,A
       ANDCC #$FF
       ORCC  #$21
       DAA
       TFR   CC,B
       BSR   OUTA
       LEAY  1,Y
       CMPY  #256
       BNE   S4

       JMP   WARMS

OUTA   PSHS  D,X
       STD   ADDR
       LDX   #ADDR
       JSR   OUTADR
       LDA   #' 
       JSR   PUTCHR
       LDA   COUNT
       INCA
       STA   COUNT
       CMPA  #16
       BNE   OUTA9
       JSR   PCRLF
       CLR   COUNT
OUTA9  PULS  D,X,PC

COUNT  FCB   0
ADDR   FDB   0
T1     FCC   'Carry: 0, Halfcarry: 0',EOT
T2     FCC   'Carry: 1, Halfcarry: 0',EOT
T3     FCC   'Carry: 0, Halfcarry: 1',EOT
T4     FCC   'Carry: 1, Halfcarry: 1',EOT

       IF    *>$C5FF
       ERR   PROGRAM TOO LONG
       ENDIF

       END   BEGIN

