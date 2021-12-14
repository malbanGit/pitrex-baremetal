                    ORG   $0000
L0000               ABX                           ; 0000: 3A            
                    ASLA                          ; 0001: 48            
                    ASLB                          ; 0002: 58            
                    ASRA                          ; 0003: 47            
                    ASRB                          ; 0004: 57            
                    CLRA                          ; 0005: 4F            
                    CLRB                          ; 0006: 5F            
                    COMA                          ; 0007: 43            
                    COMB                          ; 0008: 53            
                    DAA                           ; 0009: 19            
                    DECA                          ; 000A: 4A            
                    DECB                          ; 000B: 5A            
                    INCA                          ; 000C: 4C            
                    INCB                          ; 000D: 5C            
                    ASLA                          ; 000E: 48            
                    ASLB                          ; 000F: 58            
                    LSRA                          ; 0010: 44            
                    LSRB                          ; 0011: 54            
L0012               MUL                           ; 0012: 3D            
                    NEGA                          ; 0013: 40            
                    NEGB                          ; 0014: 50            
                    NOP                           ; 0015: 12            
                    ROLA                          ; 0016: 49            
                    ROLB                          ; 0017: 59            
                    RORA                          ; 0018: 46            
                    RORB                          ; 0019: 56            
                    RTI                           ; 001A: 3B            

                    RTS                           ; 001B: 39            

                    SEX                           ; 001C: 1D            
                    SWI                           ; 001D: 3F            
L001E               SWI2                          ; 001E: 10 3F         
L0020               SWI3                          ; 0020: 11 3F         
L0022               SYNC                          ; 0022: 13            
L0023               TSTA                          ; 0023: 4D            
                    TSTB                          ; 0024: 5D            
                    PSHS  B                       ; 0025: 34 04         
                    ADDA  ,S+                     ; 0027: AB E0         
                    ASLB                          ; 0029: 58            
                    ROLA                          ; 002A: 49            
                    ASRA                          ; 002B: 47            
                    RORB                          ; 002C: 56            
                    PSHS  B                       ; 002D: 34 04         
                    CMPA  ,S+                     ; 002F: A1 E0         
                    ANDCC #$FE                    ; 0031: 1C FE         
                    ANDCC #$BF                    ; 0033: 1C BF         
                    ANDCC #$EF                    ; 0035: 1C EF         
                    ANDCC #$AF                    ; 0037: 1C AF         
                    CLRA                          ; 0039: 4F            
                    CLRB                          ; 003A: 5F            
                    ANDCC #$FD                    ; 003B: 1C FD         
                    COMA                          ; 003D: 43            
                    COMB                          ; 003E: 53            
                    LEAS  -1,S                    ; 003F: 32 7F         
                    LEAX  -1,X                    ; 0041: 30 1F         
                    LEAY  -1,Y                    ; 0043: 31 3F         
                    LEAU  -1,U                    ; 0045: 33 5F         
                    LEAS  $01,S                   ; 0047: 32 61         
                    LEAU  $01,U                   ; 0049: 33 41         
                    LEAX  $01,X                   ; 004B: 30 01         
                    LEAY  $01,Y                   ; 004D: 31 21         
                    ASLB                          ; 004F: 58            
                    ROLA                          ; 0050: 49            
                    LSRA                          ; 0051: 44            
                    RORB                          ; 0052: 56            
                    PSHS  A                       ; 0053: 34 02         
                    PSHS  B                       ; 0055: 34 04         
                    PSHS  CC                      ; 0057: 34 01         
                    PSHS  B,A                     ; 0059: 34 06         
                    PSHS  DP                      ; 005B: 34 08         
                    PSHS  PC                      ; 005D: 34 80         
                    PSHS  X                       ; 005F: 34 10         
                    PSHS  Y                       ; 0061: 34 20         
                    PULS  A                       ; 0063: 35 02         
                    PULS  B                       ; 0065: 35 04         
                    PULS  CC                      ; 0067: 35 01         
                    PULS  A,B                     ; 0069: 35 06         
                    PULS  DP                      ; 006B: 35 08         
                    PULS  PC                      ; 006D: 35 80             // (PUL? PC=RTS)

                    PULS  X                       ; 006F: 35 10         
                    PULS  Y                       ; 0071: 35 20         
                    ROLB                          ; 0073: 59            
                    ROLA                          ; 0074: 49            
                    RORA                          ; 0075: 46            
                    RORB                          ; 0076: 56            
                    PSHS  B                       ; 0077: 34 04         
                    SUBA  ,S+                     ; 0079: A0 E0         
                    ORCC  #$01                    ; 007B: 1A 01         
                    ORCC  #$40                    ; 007D: 1A 40         
                    ORCC  #$10                    ; 007F: 1A 10         
                    ORCC  #$50                    ; 0081: 1A 50         
                    ORCC  #$02                    ; 0083: 1A 02         
                    TFR   A,B                     ; 0085: 1F 89         
                    TSTA                          ; 0087: 4D            
                    TFR   A,CC                    ; 0088: 1F 8A         
                    TFR   B,A                     ; 008A: 1F 98         
                    TSTA                          ; 008C: 4D            
                    TFR   CC,A                    ; 008D: 1F A8         
                    TFR   S,X                     ; 008F: 1F 41         
                    TFR   X,S                     ; 0091: 1F 14         
                    CWAI  #$FF                    ; 0093: 3C FF         
L0095               BCC   $95                     ; 0095: 24 FE         
                    BCS   $B9                     ; 0097: 25 20         
                    BEQ   $95                     ; 0099: 27 FA         
                    BGE   $B9                     ; 009B: 2C 1C         
                    BGT   $95                     ; 009D: 2E F6         
                    BHI   $B9                     ; 009F: 22 18         
                    BCC   $95                     ; 00A1: 24 F2         
                    BLE   $B9                     ; 00A3: 2F 14         
                    BCS   $95                     ; 00A5: 25 EE         
                    BLS   $B9                     ; 00A7: 23 10         
                    BLT   $95                     ; 00A9: 2D EA         
                    BMI   $B9                     ; 00AB: 2B 0C         
                    BNE   $95                     ; 00AD: 26 E6         
                    BPL   $B9                     ; 00AF: 2A 08         
                    BRA   $95                     ; 00B1: 20 E2         

                    BRN   $B9                     ; 00B3: 21 04         
                    BSR   $95                     ; 00B5: 8D DE         
L00B7               BVC   $B9                     ; 00B7: 28 00         
L00B9               BVS   $95                     ; 00B9: 29 DA         
                    LBCC  $1234                   ; 00BB: 10 24 11 75   
                    LBCS  $1234                   ; 00BF: 10 25 11 71   
                    LBEQ  $1234                   ; 00C3: 10 27 11 6D   
                    LBGE  $1234                   ; 00C7: 10 2C 11 69   
                    LBGT  $1234                   ; 00CB: 10 2E 11 65   
                    LBHI  $1234                   ; 00CF: 10 22 11 61   
                    LBCC  $1234                   ; 00D3: 10 24 11 5D   
                    LBLE  $1234                   ; 00D7: 10 2F 11 59   
                    LBCS  $1234                   ; 00DB: 10 25 11 55   
                    LBLS  $1234                   ; 00DF: 10 23 11 51   
                    LBLT  $1234                   ; 00E3: 10 2D 11 4D   
                    LBMI  $1234                   ; 00E7: 10 2B 11 49   
                    LBNE  $1234                   ; 00EB: 10 26 11 45   
                    LBPL  $1234                   ; 00EF: 10 2A 11 41   
                    LBRA  $1234                   ; 00F3: 16 11 3E      

                    LBRN  $1234                   ; 00F6: 10 21 11 3A   
                    LBSR  $1234                   ; 00FA: 17 11 37      
L00FD               LBVC  $1234                   ; 00FD: 10 28 11 33   
                    LBVS  $1234                   ; 0101: 10 29 11 2F   
                    ADCA  #$12                    ; 0105: 89 12         
                    ADCA  <$12                    ; 0107: 99 12         
                    ADCA  $1234                   ; 0109: B9 12 34      
                    ADCA  [$1234]                 ; 010C: A9 9F 12 34   
                    ADCB  #$12                    ; 0110: C9 12         
                    ADCB  <$12                    ; 0112: D9 12         
                    ADCB  $1234                   ; 0114: F9 12 34      
                    ADCB  [$1234]                 ; 0117: E9 9F 12 34   
                    ADDA  #$12                    ; 011B: 8B 12         
                    ADDA  <$12                    ; 011D: 9B 12         
                    ADDA  $1234                   ; 011F: BB 12 34      
                    ADDA  [$1234]                 ; 0122: AB 9F 12 34   
                    ADDB  #$12                    ; 0126: CB 12         
                    ADDB  <$12                    ; 0128: DB 12         
                    ADDB  $1234                   ; 012A: FB 12 34      
                    ADDB  [$1234]                 ; 012D: EB 9F 12 34   
                    ADDD  #$1234                  ; 0131: C3 12 34      
                    ADDD  <$12                    ; 0134: D3 12         
                    ADDD  $1234                   ; 0136: F3 12 34      
                    ADDD  [$1234]                 ; 0139: E3 9F 12 34   
                    ANDA  #$12                    ; 013D: 84 12         
                    ANDA  <$12                    ; 013F: 94 12         
                    ANDA  $1234                   ; 0141: B4 12 34      
                    ANDA  [$1234]                 ; 0144: A4 9F 12 34   
                    ANDB  #$12                    ; 0148: C4 12         
                    ANDB  <$12                    ; 014A: D4 12         
                    ANDB  $1234                   ; 014C: F4 12 34      
                    ANDB  [$1234]                 ; 014F: E4 9F 12 34   
                    ANDCC #$12                    ; 0153: 1C 12         
                    ASL   <$12                    ; 0155: 08 12         
                    ASL   $1234                   ; 0157: 78 12 34      
                    ASL   [$1234]                 ; 015A: 68 9F 12 34   
                    ASR   <$12                    ; 015E: 07 12         
                    ASR   $1234                   ; 0160: 77 12 34      
                    ASR   [$1234]                 ; 0163: 67 9F 12 34   
                    BITA  #$12                    ; 0167: 85 12         
                    BITA  <$12                    ; 0169: 95 12         
                    BITA  $1234                   ; 016B: B5 12 34      
                    BITA  [$1234]                 ; 016E: A5 9F 12 34   
                    BITB  #$12                    ; 0172: C5 12         
                    BITB  <$12                    ; 0174: D5 12         
                    BITB  $1234                   ; 0176: F5 12 34      
                    BITB  [$1234]                 ; 0179: E5 9F 12 34   
                    CLR   <$12                    ; 017D: 0F 12         
                    CLR   $1234                   ; 017F: 7F 12 34      
                    CLR   [$1234]                 ; 0182: 6F 9F 12 34   
                    CMPA  #$12                    ; 0186: 81 12         
                    CMPA  <$12                    ; 0188: 91 12         
                    CMPA  $1234                   ; 018A: B1 12 34      
                    CMPA  [$1234]                 ; 018D: A1 9F 12 34   
                    CMPB  #$12                    ; 0191: C1 12         
                    CMPB  <$12                    ; 0193: D1 12         
                    CMPB  $1234                   ; 0195: F1 12 34      
                    CMPB  [$1234]                 ; 0198: E1 9F 12 34   
                    CMPD  #$1234                  ; 019C: 10 83 12 34   
                    CMPD  <$12                    ; 01A0: 10 93 12      
                    CMPD  $1234                   ; 01A3: 10 B3 12 34   
                    CMPD  [$1234]                 ; 01A7: 10 A3 9F 12 34
                    CMPS  #$1234                  ; 01AC: 11 8C 12 34   
                    CMPS  <$12                    ; 01B0: 11 9C 12      
                    CMPS  $1234                   ; 01B3: 11 BC 12 34   
                    CMPS  [$1234]                 ; 01B7: 11 AC 9F 12 34
                    CMPU  #$1234                  ; 01BC: 11 83 12 34   
                    CMPU  <$12                    ; 01C0: 11 93 12      
                    CMPU  $1234                   ; 01C3: 11 B3 12 34   
                    CMPU  [$1234]                 ; 01C7: 11 A3 9F 12 34
                    CMPX  #$1234                  ; 01CC: 8C 12 34      
                    CPX   <$12                    ; 01CF: 9C 12         
                    CPX   $1234                   ; 01D1: BC 12 34      
                    CPX   [$1234]                 ; 01D4: AC 9F 12 34   
                    CMPY  #$1234                  ; 01D8: 10 8C 12 34   
                    CMPY  <$12                    ; 01DC: 10 9C 12      
                    CMPY  $1234                   ; 01DF: 10 BC 12 34   
                    CMPY  [$1234]                 ; 01E3: 10 AC 9F 12 34
                    COM   <$12                    ; 01E8: 03 12         
                    COM   $1234                   ; 01EA: 73 12 34      
                    COM   [$1234]                 ; 01ED: 63 9F 12 34   
                    CWAI  #$FF                    ; 01F1: 3C FF         
L01F3               DEC   <$12                    ; 01F3: 0A 12         
                    DEC   $1234                   ; 01F5: 7A 12 34      
                    DEC   [$1234]                 ; 01F8: 6A 9F 12 34   
                    EORA  #$12                    ; 01FC: 88 12         
                    EORA  <$12                    ; 01FE: 98 12         
                    EORA  $1234                   ; 0200: B8 12 34      
                    EORA  [$1234]                 ; 0203: A8 9F 12 34   
                    EORB  #$12                    ; 0207: C8 12         
                    EORB  <$12                    ; 0209: D8 12         
                    EORB  $1234                   ; 020B: F8 12 34      
                    EORB  [$1234]                 ; 020E: E8 9F 12 34   
                    INC   <$12                    ; 0212: 0C 12         
                    INC   $1234                   ; 0214: 7C 12 34      
                    INC   [$1234]                 ; 0217: 6C 9F 12 34   
                    JMP   <$12                    ; 021B: 0E 12         

                    JMP   $1234                   ; 021D: 7E 12 34      

                    JMP   [$1234]                 ; 0220: 6E 9F 12 34   

                    JSR   <$12                    ; 0224: 9D 12         
L0226               JSR   $1234                   ; 0226: BD 12 34      
L0229               JSR   [$1234]                 ; 0229: AD 9F 12 34   
L022D               LDA   #$12                    ; 022D: 86 12         
                    LDA   <$12                    ; 022F: 96 12         
                    LDA   $1234                   ; 0231: B6 12 34      
                    LDA   [$1234]                 ; 0234: A6 9F 12 34   
                    LDB   #$12                    ; 0238: C6 12         
                    LDB   <$12                    ; 023A: D6 12         
                    LDB   $1234                   ; 023C: F6 12 34      
                    LDB   [$1234]                 ; 023F: E6 9F 12 34   
                    LDD   #$1234                  ; 0243: CC 12 34      
                    LDD   <$12                    ; 0246: DC 12         
                    LDD   $1234                   ; 0248: FC 12 34      
                    LDD   [$1234]                 ; 024B: EC 9F 12 34   
                    LDS   #$1234                  ; 024F: 10 CE 12 34   
                    LDS   <$12                    ; 0253: 10 DE 12      
                    LDS   $1234                   ; 0256: 10 FE 12 34   
                    LDS   [$1234]                 ; 025A: 10 EE 9F 12 34
                    LDU   #$1234                  ; 025F: CE 12 34      
                    LDU   <$12                    ; 0262: DE 12         
                    LDU   $1234                   ; 0264: FE 12 34      
                    LDU   [$1234]                 ; 0267: EE 9F 12 34   
                    LDX   #$1234                  ; 026B: 8E 12 34      
                    LDX   <$12                    ; 026E: 9E 12         
                    LDX   $1234                   ; 0270: BE 12 34      
                    LDX   [$1234]                 ; 0273: AE 9F 12 34   
                    LDY   #$1234                  ; 0277: 10 8E 12 34   
                    LDY   <$12                    ; 027B: 10 9E 12      
                    LDY   $1234                   ; 027E: 10 BE 12 34   
                    LDY   [$1234]                 ; 0282: 10 AE 9F 12 34
                    LEAS  ,S                      ; 0287: 32 E4         
                    LEAU  $01,U                   ; 0289: 33 41         
                    LEAX  -1,X                    ; 028B: 30 1F         
                    LEAY  $01,Y                   ; 028D: 31 21         
                    ASL   <$12                    ; 028F: 08 12         
                    ASL   $1234                   ; 0291: 78 12 34      
                    ASL   [$1234]                 ; 0294: 68 9F 12 34   
                    LSR   <$12                    ; 0298: 04 12         
                    LSR   $1234                   ; 029A: 74 12 34      
                    LSR   [$1234]                 ; 029D: 64 9F 12 34   
                    NEG   <$12                    ; 02A1: 00 12         
                    NEG   $1234                   ; 02A3: 70 12 34      
                    NEG   [$1234]                 ; 02A6: 60 9F 12 34   
                    ORA   #$12                    ; 02AA: 8A 12         
                    ORA   <$12                    ; 02AC: 9A 12         
                    ORA   $1234                   ; 02AE: BA 12 34      
                    ORA   [$1234]                 ; 02B1: AA 9F 12 34   
                    ORB   #$12                    ; 02B5: CA 12         
                    ORB   <$12                    ; 02B7: DA 12         
                    ORB   $1234                   ; 02B9: FA 12 34      
                    ORB   [$1234]                 ; 02BC: EA 9F 12 34   
                    ORCC  #$12                    ; 02C0: 1A 12         
                    ROL   <$12                    ; 02C2: 09 12         
                    ROL   $1234                   ; 02C4: 79 12 34      
                    ROL   [$1234]                 ; 02C7: 69 9F 12 34   
                    ROR   <$12                    ; 02CB: 06 12         
                    ROR   $1234                   ; 02CD: 76 12 34      
                    ROR   [$1234]                 ; 02D0: 66 9F 12 34   
                    SBCA  #$12                    ; 02D4: 82 12         
                    SBCA  <$12                    ; 02D6: 92 12         
                    SBCA  $1234                   ; 02D8: B2 12 34      
                    SBCA  [$1234]                 ; 02DB: A2 9F 12 34   
                    SBCB  #$12                    ; 02DF: C2 12         
                    SBCB  <$12                    ; 02E1: D2 12         
                    SBCB  $1234                   ; 02E3: F2 12 34      
                    SBCB  [$1234]                 ; 02E6: E2 9F 12 34   
                    STA   <$12                    ; 02EA: 97 12         
                    STA   $1234                   ; 02EC: B7 12 34      
                    STA   [$1234]                 ; 02EF: A7 9F 12 34   
                    STB   <$12                    ; 02F3: D7 12         
                    STB   $1234                   ; 02F5: F7 12 34      
                    STB   [$1234]                 ; 02F8: E7 9F 12 34   
                    STD   <$12                    ; 02FC: DD 12         
                    STD   $1234                   ; 02FE: FD 12 34      
                    STD   [$1234]                 ; 0301: ED 9F 12 34   
                    STS   <$12                    ; 0305: 10 DF 12      
                    STS   $1234                   ; 0308: 10 FF 12 34   
                    STS   [$1234]                 ; 030C: 10 EF 9F 12 34
                    STU   <$12                    ; 0311: DF 12         
                    STU   $1234                   ; 0313: FF 12 34      
                    STU   [$1234]                 ; 0316: EF 9F 12 34   
                    STX   <$12                    ; 031A: 9F 12         
                    STX   $1234                   ; 031C: BF 12 34      
                    STX   [$1234]                 ; 031F: AF 9F 12 34   
                    STY   <$12                    ; 0323: 10 9F 12      
                    STY   $1234                   ; 0326: 10 BF 12 34   
                    STY   [$1234]                 ; 032A: 10 AF 9F 12 34
                    SUBA  #$12                    ; 032F: 80 12         
                    SUBA  <$12                    ; 0331: 90 12         
                    SUBA  $1234                   ; 0333: B0 12 34      
                    SUBA  [$1234]                 ; 0336: A0 9F 12 34   
                    SUBB  #$12                    ; 033A: C0 12         
                    SUBB  <$12                    ; 033C: D0 12         
                    SUBB  $1234                   ; 033E: F0 12 34      
                    SUBB  [$1234]                 ; 0341: E0 9F 12 34   
                    SUBD  #$1234                  ; 0345: 83 12 34      
                    SUBD  <$12                    ; 0348: 93 12         
                    SUBD  $1234                   ; 034A: B3 12 34      
                    SUBD  [$1234]                 ; 034D: A3 9F 12 34   
                    TST   <$12                    ; 0351: 0D 12         
                    TST   $1234                   ; 0353: 7D 12 34      
                    TST   [$1234]                 ; 0356: 6D 9F 12 34   
                    EXG   A,B                     ; 035A: 1E 89         
                    EXG   X,Y                     ; 035C: 1E 12         
                    TFR   A,B                     ; 035E: 1F 89         
                    TFR   S,U                     ; 0360: 1F 43         
                    PSHS  PC,U,Y,X,DP,B,A,CC      ; 0362: 34 FF         
                    PSHU  PC,S,Y,X,DP,B,A,CC      ; 0364: 36 FF         
                    PULS  CC,A,B,DP,X,Y,U,PC      ; 0366: 35 FF             // (PUL? PC=RTS)

                    PULU  CC,A,B,DP,X,Y,S,PC      ; 0368: 37 FF             // (PUL? PC=RTS)

                    LDA   <$12                    ; 036A: 96 12         
                    LDA   $1234                   ; 036C: B6 12 34      
                    LDA   $12                     ; 036F: B6 00 12      
                    LDA   <$34                    ; 0372: 96 34         
                    ADDA  ,X                      ; 0374: AB 84         
                    ADDA  ,X+                     ; 0376: AB 80         
                    ADDA  ,X++                    ; 0378: AB 81         
                    ADDA  ,-X                     ; 037A: AB 82         
                    ADDA  ,--X                    ; 037C: AB 83         
                    ADDA  $09,X                   ; 037E: AB 09         
                    ADDA  -9,X                    ; 0380: AB 17         
                    ADDA  $78,X                   ; 0382: AB 88 78      
                    ADDA  -$78,X                  ; 0385: AB 88 88      
                    ADDA  $1234,X                 ; 0388: AB 89 12 34   
                    ADDA  -$1234,X                ; 038C: AB 89 ED CC   
                    ADDA  ,Y                      ; 0390: AB A4         
                    ADDA  ,Y+                     ; 0392: AB A0         
                    ADDA  ,Y++                    ; 0394: AB A1         
                    ADDA  ,-Y                     ; 0396: AB A2         
                    ADDA  ,--Y                    ; 0398: AB A3         
                    ADDA  $09,Y                   ; 039A: AB 29         
                    ADDA  -9,Y                    ; 039C: AB 37         
                    ADDA  $78,Y                   ; 039E: AB A8 78      
                    ADDA  -$78,Y                  ; 03A1: AB A8 88      
                    ADDA  $1234,Y                 ; 03A4: AB A9 12 34   
                    ADDA  -$1234,Y                ; 03A8: AB A9 ED CC   
                    ADDA  ,U                      ; 03AC: AB C4         
                    ADDA  ,U+                     ; 03AE: AB C0         
                    ADDA  ,U++                    ; 03B0: AB C1         
                    ADDA  ,-U                     ; 03B2: AB C2         
                    ADDA  ,--U                    ; 03B4: AB C3         
                    ADDA  $09,U                   ; 03B6: AB 49         
                    ADDA  -9,U                    ; 03B8: AB 57         
                    ADDA  $78,U                   ; 03BA: AB C8 78      
                    ADDA  -$78,U                  ; 03BD: AB C8 88      
                    ADDA  $1234,U                 ; 03C0: AB C9 12 34   
                    ADDA  -$1234,U                ; 03C4: AB C9 ED CC   
                    ADDA  ,S                      ; 03C8: AB E4         
                    ADDA  ,S+                     ; 03CA: AB E0         
                    ADDA  ,S++                    ; 03CC: AB E1         
                    ADDA  ,-S                     ; 03CE: AB E2         
                    ADDA  ,--S                    ; 03D0: AB E3         
                    ADDA  $09,S                   ; 03D2: AB 69         
                    ADDA  -9,S                    ; 03D4: AB 77         
                    ADDA  $78,S                   ; 03D6: AB E8 78      
                    ADDA  -$78,S                  ; 03D9: AB E8 88      
                    ADDA  $1234,S                 ; 03DC: AB E9 12 34   
                    ADDA  -$1234,S                ; 03E0: AB E9 ED CC   
                    ADDA  [,X]                    ; 03E4: AB 94         
                    ADDA  [,X++]                  ; 03E6: AB 91         
                    ADDA  [,--X]                  ; 03E8: AB 93         
                    ADDA  [$09,X]                 ; 03EA: AB 98 09      
                    ADDA  [-$09,X]                ; 03ED: AB 98 F7      
                    ADDA  [$78,X]                 ; 03F0: AB 98 78      
                    ADDA  [-$78,X]                ; 03F3: AB 98 88      
                    ADDA  [$1234,X]               ; 03F6: AB 99 12 34   
                    ADDA  [-$1234,X]              ; 03FA: AB 99 ED CC   
                    ADDA  [,Y]                    ; 03FE: AB B4         
                    ADDA  [,Y++]                  ; 0400: AB B1         
                    ADDA  [,--Y]                  ; 0402: AB B3         
                    ADDA  [$09,Y]                 ; 0404: AB B8 09      
                    ADDA  [-$09,Y]                ; 0407: AB B8 F7      
                    ADDA  [$78,Y]                 ; 040A: AB B8 78      
                    ADDA  [-$78,Y]                ; 040D: AB B8 88      
                    ADDA  [$1234,Y]               ; 0410: AB B9 12 34   
                    ADDA  [-$1234,Y]              ; 0414: AB B9 ED CC   
                    ADDA  [,U]                    ; 0418: AB D4         
                    ADDA  [,U++]                  ; 041A: AB D1         
                    ADDA  [,--U]                  ; 041C: AB D3         
                    ADDA  [$09,U]                 ; 041E: AB D8 09      
                    ADDA  [-$09,U]                ; 0421: AB D8 F7      
                    ADDA  [$78,U]                 ; 0424: AB D8 78      
                    ADDA  [-$78,U]                ; 0427: AB D8 88      
                    ADDA  [$1234,U]               ; 042A: AB D9 12 34   
                    ADDA  [-$1234,U]              ; 042E: AB D9 ED CC   
                    ADDA  [,S]                    ; 0432: AB F4         
                    ADDA  [,S++]                  ; 0434: AB F1         
                    ADDA  [,--S]                  ; 0436: AB F3         
                    ADDA  [$09,S]                 ; 0438: AB F8 09      
                    ADDA  [-$09,S]                ; 043B: AB F8 F7      
                    ADDA  [$78,S]                 ; 043E: AB F8 78      
                    ADDA  [-$78,S]                ; 0441: AB F8 88      
                    ADDA  [$1234,S]               ; 0444: AB F9 12 34   
                    ADDA  [-$1234,S]              ; 0448: AB F9 ED CC   
                    ADDA  A,X                     ; 044C: AB 86         
                    ADDA  B,Y                     ; 044E: AB A5         
                    ADDA  D,U                     ; 0450: AB CB         
                    ADDA  A,S                     ; 0452: AB E6         
                    ADDA  A,U                     ; 0454: AB C6         
                    ADDA  A,S                     ; 0456: AB E6         
                    ADDA  [A,X]                   ; 0458: AB 96         
                    ADDA  [B,Y]                   ; 045A: AB B5         
                    ADDA  [D,U]                   ; 045C: AB DB         
                    ADDA  [A,S]                   ; 045E: AB F6         
                    ADDA  [A,U]                   ; 0460: AB D6         
                    ADDA  [A,S]                   ; 0462: AB F6         
                    LDA   $1234,PC                ; 0464: A6 8D 12 34       //  ($169C)
                    LDA   $EDCC,PC                ; 0468: A6 8D ED CC       //  ($F238)
                    LDA   $0DC4,PC                ; 046C: A6 8D 0D C4       //  ($1234)
                    LDA   #$78                    ; 0470: 86 78         
                    LDA   #$56                    ; 0472: 86 56         
                    LDA   #$34                    ; 0474: 86 34         
                    LDA   #$12                    ; 0476: 86 12         
                    LDX   #$5678                  ; 0478: 8E 56 78      
                    LDX   #$3456                  ; 047B: 8E 34 56      
                    LDX   #$1234                  ; 047E: 8E 12 34      
                    LDX   #$12                    ; 0481: 8E 00 12      
                    ASL   $5634                   ; 0484: 78 56 34      
                    NOP                           ; 0487: 12            
