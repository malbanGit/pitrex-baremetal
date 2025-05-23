                    ORG   $6000
; ===========================================================================
; Segment type: Pure code
L6000               JMP   $F261                   ; 6000: 7E F2 61        // segment ROM

                    FCB   $EF                     ; 6003:  ' ' EF 56          STU   -10,U ; 
                    FCB   $56                     ; 6004:  'V' 56             RORB  ; V
; =============== S U B R O U T I N E =======================================
L670D               LDX   #$496F                  ; 670D: 8E 49 6F        // "Bank 1"
                    LDA   #$00                    ; 6710: 86 00         
L6712               STA   ,X+                     ; 6712: A7 80         
                    CMPX  #$4989                  ; 6714: 8C 49 89      
                    BCS   $6712                   ; 6717: 25 F9         
                    LDA   #$60                    ; 6719: 86 60         
                    LSRA                          ; 671B: 44            
                    STA   <$D2                    ; 671C: 97 D2         
                    LDD   #$5300                  ; 671E: CC 53 00      
                    STD   <$D4                    ; 6721: DD D4         
; End of function sub_670D
                    RTS                           ; 6723: 39            

; =============== S U B R O U T I N E =======================================
L6724               STB   <$DC                    ; 6724: D7 DC           // Selected object model
L6726               LDB   <$DC                    ; 6726: D6 DC           // Selected object model
                    LDX   #$496F                  ; 6728: 8E 49 6F      
                    LDA   B,X                     ; 672B: A6 85         
                    BNE   $6760                   ; 672D: 26 31         
                    LDA   <$D2                    ; 672F: 96 D2         
                    STA   B,X                     ; 6731: A7 85         
                    LDX   #$7644                  ; 6733: 8E 76 44        // Array of object sizes?
                    LDA   B,X                     ; 6736: A6 85         
                    STA   <$01                    ; 6738: 97 01           // Number of points in object?
                    LSRA                          ; 673A: 44            
                    ADDA  <$D2                    ; 673B: 9B D2         
                    BCC   $6740                   ; 673D: 24 01         
                    SWI                           ; 673F: 3F            
L6740               STA   <$D2                    ; 6740: 97 D2         
                    LDX   #$765E                  ; 6742: 8E 76 5E        // Table of pointers to object X, Y, Z point data?
                    ASLB                          ; 6745: 58            
                    LDX   B,X                     ; 6746: AE 85         
                    LDU   <$D4                    ; 6748: DE D4         
L674A               LDD   ,X                      ; 674A: EC 84         
                    STD   ,U                      ; 674C: ED C4         
                    LDD   $02,X                   ; 674E: EC 02         
                    STD   $02,U                   ; 6750: ED 42         
                    LDD   $04,X                   ; 6752: EC 04         
                    STD   $04,U                   ; 6754: ED 44         
                    LEAX  $06,X                   ; 6756: 30 06         
                    LEAU  $08,U                   ; 6758: 33 48         
                    DEC   <$01                    ; 675A: 0A 01           // Loop count for object points
                    BGT   $674A                   ; 675C: 2E EC           // Copy XYZ data to math RAM
                    STU   <$D4                    ; 675E: DF D4         
; End of function sub_6724
L6760               RTS                           ; 6760: 39            

; =============== S U B R O U T I N E =======================================
L6761               LDD   $4700                   ; 6761: FC 47 00        // Called during space, towers and trench
                    STD   $5000                   ; 6764: FD 50 00        // Math result X
                    LDA   #$86                    ; 6767: 86 86           // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
                    JSR   $CDBA                   ; 6769: BD CD BA      
L676C               LDD   $5004                   ; 676C: FC 50 04        // Math result Z
                    STD   <$D8                    ; 676F: DD D8         
                    ADDD  #$FF98                  ; 6771: C3 FF 98      
                    ANDA  #$1F                    ; 6774: 84 1F         
                    STD   ,Y++                    ; 6776: ED A1         
                    LDD   $5002                   ; 6778: FC 50 02        // Math result Y
                    STD   <$D6                    ; 677B: DD D6         
                    ANDA  #$1F                    ; 677D: 84 1F         
                    STD   ,Y++                    ; 677F: ED A1         
; End of function sub_6761
                    RTS                           ; 6781: 39            

; =============== S U B R O U T I N E =======================================
L6782               LDD   $4700                   ; 6782: FC 47 00        // Calc trench floor lines
                    STD   $5000                   ; 6785: FD 50 00        // Math result X
                    LDA   #$86                    ; 6788: 86 86           // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
                    JSR   $CDBA                   ; 678A: BD CD BA      
L678D               LDD   $5004                   ; 678D: FC 50 04        // Math result Z
                    SUBD  <$D8                    ; 6790: 93 D8         
                    ANDA  #$1F                    ; 6792: 84 1F         
                    STD   ,Y++                    ; 6794: ED A1         
                    LDD   $5004                   ; 6796: FC 50 04        // Math result Z
                    STD   <$D8                    ; 6799: DD D8         
                    LDD   $5002                   ; 679B: FC 50 02        // Math result Y
                    SUBD  <$D6                    ; 679E: 93 D6         
                    ORA   #$E0                    ; 67A0: 8A E0         
                    STD   ,Y++                    ; 67A2: ED A1         
                    LDD   $5002                   ; 67A4: FC 50 02        // Math result Y
                    STD   <$D6                    ; 67A7: DD D6         
; End of function sub_6782
                    RTS                           ; 67A9: 39            

; =============== S U B R O U T I N E =======================================
L67AA               LDD   $4700                   ; 67AA: FC 47 00        // Calc trench vertical lines on walls
                    STD   $5000                   ; 67AD: FD 50 00        // Math result X
                    LDA   #$86                    ; 67B0: 86 86           // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
                    JSR   $CDBA                   ; 67B2: BD CD BA      
L67B5               LDD   $5004                   ; 67B5: FC 50 04        // Math result Z
                    SUBD  <$D8                    ; 67B8: 93 D8         
                    ANDA  #$1F                    ; 67BA: 84 1F         
                    STD   ,Y++                    ; 67BC: ED A1         
                    LDD   $5004                   ; 67BE: FC 50 04        // Math result Z
                    STD   <$D8                    ; 67C1: DD D8         
                    LDD   $5002                   ; 67C3: FC 50 02        // Math result Y
                    SUBD  <$D6                    ; 67C6: 93 D6         
                    ANDA  #$1F                    ; 67C8: 84 1F         
                    STD   ,Y++                    ; 67CA: ED A1         
                    LDD   $5002                   ; 67CC: FC 50 02        // Math result Y
                    STD   <$D6                    ; 67CF: DD D6         
; End of function sub_67AA
                    RTS                           ; 67D1: 39            

; =============== S U B R O U T I N E =======================================
; End of function sub_67D2
L67D2               STB   <$DC                    ; 67D2: D7 DC           // Called during space
; =============== S U B R O U T I N E =======================================
L67D4               LDB   <$DC                    ; 67D4: D6 DC           // Selected object model
                    LDX   #$7644                  ; 67D6: 8E 76 44      
                    LDA   B,X                     ; 67D9: A6 85         
                    STA   <$01                    ; 67DB: 97 01           // Loop count for object points
                    LDX   #$496F                  ; 67DD: 8E 49 6F      
                    LDA   B,X                     ; 67E0: A6 85         
                    BNE   $67E5                   ; 67E2: 26 01         
                    SWI                           ; 67E4: 3F            
L67E5               ASLA                          ; 67E5: 48            
                    STA   $4702                   ; 67E6: B7 47 02        // Set BIC
                    ROLA                          ; 67E9: 49            
                    STA   $4701                   ; 67EA: B7 47 01      
                    LDX   #$4C00                  ; 67ED: 8E 4C 00      
L67F0               LDA   #$50                    ; 67F0: 86 50           // Math program 0x50. Matrix Multiply - Transposed
                    JSR   $CDBA                   ; 67F2: BD CD BA      
L67F5               LDD   $5000                   ; 67F5: FC 50 00        // Math result X
                    STD   $4704                   ; 67F8: FD 47 04      
                    ORCC  #$00                    ; 67FB: 1A 00         
                    LDD   $4700                   ; 67FD: FC 47 00      
                    STD   $5000                   ; 6800: FD 50 00        // Math result X
                    LDA   #$86                    ; 6803: 86 86           // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
                    JSR   $CDBA                   ; 6805: BD CD BA      
L6808               LDD   $5002                   ; 6808: FC 50 02        // Math result Y
                    STD   ,X                      ; 680B: ED 84         
                    LDD   $5004                   ; 680D: FC 50 04        // Math result Z
                    STD   $02,X                   ; 6810: ED 02         
                    LEAX  $04,X                   ; 6812: 30 04         
                    DEC   <$01                    ; 6814: 0A 01           // Loop until all object points done
                    BGT   $67F0                   ; 6816: 2E D8         
; End of function sub_67D4
                    RTS                           ; 6818: 39            

; =============== S U B R O U T I N E =======================================
; Format vectors for ties, and tower/bunker explosions
L6819               LDX   #$7610                  ; 6819: 8E 76 10        // Called during space and towers
                    LDB   <$DC                    ; 681C: D6 DC           // Selected object model
                    ASLB                          ; 681E: 58            
                    LDU   B,X                     ; 681F: EE 85         
                    LDB   ,U+                     ; 6821: E6 C0         
                    BEQ   $6826                   ; 6823: 27 01         
                    SWI                           ; 6825: 3F            
L6826               LDB   ,U+                     ; 6826: E6 C0         
                    BITB  #$02                    ; 6828: C5 02         
                    BEQ   $6838                   ; 682A: 27 0C         
                    CMPB  #$FF                    ; 682C: C1 FF         
                    BEQ   $6863                   ; 682E: 27 33         
                    LSRB                          ; 6830: 54            
                    LSRB                          ; 6831: 54            
                    CMPB  <$DB                    ; 6832: D1 DB         
                    BHI   $6826                   ; 6834: 22 F0         
                    BRA   $6863                   ; 6836: 20 2B         

; ---------------------------------------------------------------------------
L6838               CLRA                          ; 6838: 4F            
                    BITB  #$03                    ; 6839: C5 03         
                    BNE   $683F                   ; 683B: 26 02         
                    LDA   #$E0                    ; 683D: 86 E0           // Vector is visible
L683F               STA   <$DA                    ; 683F: 97 DA         
                    ANDB  #$FC                    ; 6841: C4 FC         
                    LDX   #$4C00                  ; 6843: 8E 4C 00      
                    ABX                           ; 6846: 3A            
                    LDD   $02,X                   ; 6847: EC 02         
                    SUBD  <$D8                    ; 6849: 93 D8         
                    ANDA  #$1F                    ; 684B: 84 1F         
                    STD   ,Y++                    ; 684D: ED A1         
                    LDD   ,X                      ; 684F: EC 84         
                    SUBD  <$D6                    ; 6851: 93 D6         
                    ANDA  #$1F                    ; 6853: 84 1F         
                    ORA   <$DA                    ; 6855: 9A DA         
                    STD   ,Y++                    ; 6857: ED A1         
                    LDD   ,X                      ; 6859: EC 84         
                    STD   <$D6                    ; 685B: DD D6         
                    LDD   $02,X                   ; 685D: EC 02         
                    STD   <$D8                    ; 685F: DD D8         
                    BRA   $6826                   ; 6861: 20 C3         

; ---------------------------------------------------------------------------
; End of function sub_6819
L6863               RTS                           ; 6863: 39            

; =============== S U B R O U T I N E =======================================
L6864               LDD   #$00                    ; 6864: CC 00 00        // Left hand side of trench turret calculation
                    STD   $5040                   ; 6867: FD 50 40        // XT2
                    STD   $5042                   ; 686A: FD 50 42        // Clear XT, YT and ZT to zero
                    STD   $5044                   ; 686D: FD 50 44        // ZT2
                    LDX   #$7644                  ; 6870: 8E 76 44      
                    LDB   <$DC                    ; 6873: D6 DC           // Selected object model
                    LDA   B,X                     ; 6875: A6 85         
                    STA   <$01                    ; 6877: 97 01           // Loop count for object points
                    LDX   #$765E                  ; 6879: 8E 76 5E      
                    ASLB                          ; 687C: 58            
                    LDU   B,X                     ; 687D: EE 85         
                    LDX   #$5DF0                  ; 687F: 8E 5D F0      
L6882               LDD   ,U++                    ; 6882: EC C1         
                    STD   $5E00                   ; 6884: FD 5E 00      
                    LDD   ,U++                    ; 6887: EC C1         
                    STD   $5E02                   ; 6889: FD 5E 02      
                    LDD   ,U++                    ; 688C: EC C1         
                    STD   $5E04                   ; 688E: FD 5E 04      
                    LDD   #$01C0                  ; 6891: CC 01 C0        // Point BIC to Math RAM $5E00
                    STD   $4701                   ; 6894: FD 47 01      
                    LDA   #$2A                    ; 6897: 86 2A         
                    JSR   $CDBA                   ; 6899: BD CD BA      
L689C               LDD   $5000                   ; 689C: FC 50 00        // Math result X
                    STD   $06,X                   ; 689F: ED 06         
                    LDD   $5002                   ; 68A1: FC 50 02        // Math result Y
                    STD   $18,X                   ; 68A4: ED 88 18      
                    LDD   $5004                   ; 68A7: FC 50 04        // Math result Z
                    STD   $1A,X                   ; 68AA: ED 88 1A      
                    LEAX  $10,X                   ; 68AD: 30 88 10      
                    DEC   <$01                    ; 68B0: 0A 01           // Loop until all object points done
                    BGT   $6882                   ; 68B2: 2E CE         
                    LDD   $5098                   ; 68B4: FC 50 98      
                    STD   $5040                   ; 68B7: FD 50 40        // XT2
                    LDD   $509A                   ; 68BA: FC 50 9A      
                    STD   $5042                   ; 68BD: FD 50 42        // YT2
                    LDD   $509C                   ; 68C0: FC 50 9C      
                    STD   $5044                   ; 68C3: FD 50 44        // ZT2
; End of function sub_6864
                    RTS                           ; 68C6: 39            

; =============== S U B R O U T I N E =======================================
L68C7               LDD   #$00                    ; 68C7: CC 00 00        // Right hand side of trench turret calculation
                    STD   $5040                   ; 68CA: FD 50 40        // XT2
                    STD   $5042                   ; 68CD: FD 50 42        // Clear XT, YT and ZT to zero
                    STD   $5044                   ; 68D0: FD 50 44        // ZT2
                    LDX   #$7644                  ; 68D3: 8E 76 44      
                    LDB   <$DC                    ; 68D6: D6 DC           // Selected object model
                    LDA   B,X                     ; 68D8: A6 85         
                    STA   <$01                    ; 68DA: 97 01           // Loop count for object points
                    LDX   #$765E                  ; 68DC: 8E 76 5E      
                    ASLB                          ; 68DF: 58            
                    LDU   B,X                     ; 68E0: EE 85         
                    LDX   #$5DF0                  ; 68E2: 8E 5D F0      
L68E5               LDD   ,U++                    ; 68E5: EC C1         
                    STD   $5E00                   ; 68E7: FD 5E 00      
                    LDD   #$00                    ; 68EA: CC 00 00      
                    SUBD  ,U++                    ; 68ED: A3 C1           // Negate
                    STD   $5E02                   ; 68EF: FD 5E 02      
                    LDD   ,U++                    ; 68F2: EC C1         
                    STD   $5E04                   ; 68F4: FD 5E 04      
                    LDD   #$01C0                  ; 68F7: CC 01 C0        // Point BIC to Math RAM $5E00
                    STD   $4701                   ; 68FA: FD 47 01      
                    LDA   #$2A                    ; 68FD: 86 2A         
                    JSR   $CDBA                   ; 68FF: BD CD BA      
L6902               LDD   $5000                   ; 6902: FC 50 00        // Math result X
                    STD   $06,X                   ; 6905: ED 06         
                    LDD   $5002                   ; 6907: FC 50 02        // Math result Y
                    STD   $18,X                   ; 690A: ED 88 18      
                    LDD   $5004                   ; 690D: FC 50 04        // Math result Z
                    STD   $1A,X                   ; 6910: ED 88 1A      
                    LEAX  $10,X                   ; 6913: 30 88 10      
                    DEC   <$01                    ; 6916: 0A 01           // Loop count for object points
                    BGT   $68E5                   ; 6918: 2E CB         
                    LDD   $5098                   ; 691A: FC 50 98      
                    STD   $5040                   ; 691D: FD 50 40        // XT2
                    LDD   $509A                   ; 6920: FC 50 9A      
                    STD   $5042                   ; 6923: FD 50 42        // YT2
                    LDD   $509C                   ; 6926: FC 50 9C      
                    STD   $5044                   ; 6929: FD 50 44        // ZT2
; End of function sub_68C7
                    RTS                           ; 692C: 39            

; =============== S U B R O U T I N E =======================================
L692D               LDD   #$00                    ; 692D: CC 00 00        // Trench calculation
                    SUBD  $5000                   ; 6930: B3 50 00        // Math result X
                    STD   $5000                   ; 6933: FD 50 00        // Math result X
                    LDD   #$00                    ; 6936: CC 00 00      
                    SUBD  $5002                   ; 6939: B3 50 02        // Math result Y
                    STD   $5002                   ; 693C: FD 50 02        // Math result Y
                    LDD   #$00                    ; 693F: CC 00 00      
                    SUBD  $5004                   ; 6942: B3 50 04        // Math result Z
                    STD   $5004                   ; 6945: FD 50 04        // Math result Z
                    LDD   #$01C0                  ; 6948: CC 01 C0        // Point BIC to Math RAM $5E00
                    STD   $4701                   ; 694B: FD 47 01      
                    LDD   $5DF6                   ; 694E: FC 5D F6      
                    SUBD  $5000                   ; 6951: B3 50 00        // Math result X
                    STD   $4704                   ; 6954: FD 47 04      
                    LDA   #$B0                    ; 6957: 86 B0           // XT = [BIC,3] - Reg00, BIC++
                    STA   $4700                   ; 6959: B7 47 00      
                    LDX   #$7644                  ; 695C: 8E 76 44      
                    LDB   <$DC                    ; 695F: D6 DC           // Selected object model
                    LDA   B,X                     ; 6961: A6 85         
                    LDB   #$AE                    ; 6963: C6 AE           // [BIC,2] = ([BIC,0] - Reg01) x (YT), [BIC,3] = ([BIC,1] - Reg02) x (YT), BIC++, XT = (BIC,3) - Reg00, BIC++
L6965               LDU   $4700                   ; 6965: FE 47 00      
                    LDX   $5018                   ; 6968: BE 50 18        // XT
                    STU   $501A                   ; 696B: FF 50 1A        // YT
                    STB   $4700                   ; 696E: F7 47 00        // Run math program $AE
                    STX   $4704                   ; 6971: BF 47 04      
                    DECA                          ; 6974: 4A            
                    BGT   $6965                   ; 6975: 2E EE         
; End of function sub_692D
                    RTS                           ; 6977: 39            

; =============== S U B R O U T I N E =======================================
L6978               LDD   #$00                    ; 6978: CC 00 00        // Some trench calculation
                    SUBD  $5000                   ; 697B: B3 50 00        // Math result X
                    STD   $5000                   ; 697E: FD 50 00        // Math result X
                    LDD   #$00                    ; 6981: CC 00 00      
                    SUBD  $5002                   ; 6984: B3 50 02        // Math result Y
                    STD   $5002                   ; 6987: FD 50 02        // Math result Y
                    LDD   #$00                    ; 698A: CC 00 00      
                    SUBD  $5004                   ; 698D: B3 50 04        // Math result Z
                    STD   $5004                   ; 6990: FD 50 04        // Math result Z
                    LDD   #$01C0                  ; 6993: CC 01 C0        // Point BIC to Math RAM $5E00
                    STD   $4701                   ; 6996: FD 47 01      
                    LDD   $5DF6                   ; 6999: FC 5D F6      
                    SUBD  $5000                   ; 699C: B3 50 00        // Math result X
                    STD   $4704                   ; 699F: FD 47 04      
                    LDA   #$B0                    ; 69A2: 86 B0           // XT Reg0C = [BIC,3] - Reg00
                    STA   $4700                   ; 69A4: B7 47 00      
                    LDX   #$7644                  ; 69A7: 8E 76 44      
                    LDB   <$DC                    ; 69AA: D6 DC           // Selected object model
                    LDA   B,X                     ; 69AC: A6 85         
                    LDB   #$AE                    ; 69AE: C6 AE           // [BIC,2] = ([BIC,0] - Reg01) x (YT), [BIC,3] = ([BIC,1] - Reg02) x (YT), BIC++, XT = [BIC,3] - Reg00, BIC++
L69B0               LDU   $4700                   ; 69B0: FE 47 00      
                    CMPU  #$0100                  ; 69B3: 11 83 01 00   
                    BGT   $69BC                   ; 69B7: 2E 03         
                    LDU   #$7FFF                  ; 69B9: CE 7F FF      
L69BC               LDX   $5018                   ; 69BC: BE 50 18        // XT
                    STU   $501A                   ; 69BF: FF 50 1A        // YT
                    STB   $4700                   ; 69C2: F7 47 00      
                    STX   $4704                   ; 69C5: BF 47 04      
                    DECA                          ; 69C8: 4A            
                    BGT   $69B0                   ; 69C9: 2E E5         
                    LDX   #$7644                  ; 69CB: 8E 76 44      
                    LDB   <$DC                    ; 69CE: D6 DC           // Selected object model
                    LDA   B,X                     ; 69D0: A6 85         
                    LDU   #$5E0C                  ; 69D2: CE 5E 0C      
L69D5               LDX   ,U                      ; 69D5: AE C4         
                    CMPX  #$0216                  ; 69D7: 8C 02 16      
                    BLE   $69E3                   ; 69DA: 2F 07         
                    LDX   #$0216                  ; 69DC: 8E 02 16      
                    STX   ,U                      ; 69DF: AF C4         
                    BRA   $69ED                   ; 69E1: 20 0A         

; ---------------------------------------------------------------------------
L69E3               CMPX  #$FDEA                  ; 69E3: 8C FD EA      
                    BGE   $69ED                   ; 69E6: 2C 05         
                    LDX   #$FDEA                  ; 69E8: 8E FD EA      
                    STX   ,U                      ; 69EB: AF C4         
L69ED               LDX   $02,U                   ; 69ED: AE 42         
                    CMPX  #$02D8                  ; 69EF: 8C 02 D8      
                    BLE   $69FB                   ; 69F2: 2F 07         
                    LDX   #$02D8                  ; 69F4: 8E 02 D8      
                    STX   $02,U                   ; 69F7: AF 42         
                    BRA   $6A05                   ; 69F9: 20 0A         

; ---------------------------------------------------------------------------
L69FB               CMPX  #$FDEC                  ; 69FB: 8C FD EC      
                    BGE   $6A05                   ; 69FE: 2C 05         
                    LDX   #$FDEC                  ; 6A00: 8E FD EC      
                    STX   $02,U                   ; 6A03: AF 42         
L6A05               LEAU  $10,U                   ; 6A05: 33 C8 10      
                    DECA                          ; 6A08: 4A            
                    BGT   $69D5                   ; 6A09: 2E CA         
; End of function sub_6978
                    RTS                           ; 6A0B: 39            

; =============== S U B R O U T I N E =======================================
L6A0C               LDD   #$00                    ; 6A0C: CC 00 00      
                    SUBD  $5000                   ; 6A0F: B3 50 00        // Math result X
                    STD   $5000                   ; 6A12: FD 50 00        // Math result X
                    LDD   #$00                    ; 6A15: CC 00 00      
                    SUBD  $5002                   ; 6A18: B3 50 02        // Math result Y
                    STD   $5002                   ; 6A1B: FD 50 02        // Math result Y
                    LDD   #$00                    ; 6A1E: CC 00 00      
                    SUBD  $5004                   ; 6A21: B3 50 04        // Math result Z
                    STD   $5004                   ; 6A24: FD 50 04        // Math result Z
                    LDD   #$01C0                  ; 6A27: CC 01 C0        // Point BIC to Math RAM $5E00
                    STD   $4701                   ; 6A2A: FD 47 01      
                    LDD   $5DF6                   ; 6A2D: FC 5D F6      
                    SUBD  $5000                   ; 6A30: B3 50 00        // Math result X
                    STD   $4704                   ; 6A33: FD 47 04      
                    LDA   #$B0                    ; 6A36: 86 B0           // Reg0C = [BIC,3] - Reg00
                    STA   $4700                   ; 6A38: B7 47 00      
                    LDX   #$7644                  ; 6A3B: 8E 76 44      
                    LDB   <$DC                    ; 6A3E: D6 DC           // Selected object model
                    LDA   B,X                     ; 6A40: A6 85         
                    LDB   #$AE                    ; 6A42: C6 AE           // [BIC,2] = ([BIC,0] - Reg01) x YT, [BIC,3] = ([BIC,1] - Reg02) x YT, BIC++, XT = [BIC,3] - Reg00, BIC++
L6A44               LDU   $4700                   ; 6A44: FE 47 00      
                    CMPU  #$0100                  ; 6A47: 11 83 01 00   
                    BGT   $6A50                   ; 6A4B: 2E 03         
                    LDU   #$7FFF                  ; 6A4D: CE 7F FF      
L6A50               LDX   $5018                   ; 6A50: BE 50 18        // XT
                    STU   $501A                   ; 6A53: FF 50 1A        // YT
                    STB   $4700                   ; 6A56: F7 47 00      
                    STX   $4704                   ; 6A59: BF 47 04      
                    DECA                          ; 6A5C: 4A            
                    BGT   $6A44                   ; 6A5D: 2E E5           // Loop until all object points done
                    LDX   #$7644                  ; 6A5F: 8E 76 44      
                    LDB   <$DC                    ; 6A62: D6 DC           // Selected object model
                    LDA   B,X                     ; 6A64: A6 85         
                    LDU   #$5E0C                  ; 6A66: CE 5E 0C      
L6A69               LDX   ,U                      ; 6A69: AE C4         
                    CMPX  #$03FE                  ; 6A6B: 8C 03 FE      
                    BLE   $6A77                   ; 6A6E: 2F 07         
                    LDX   #$03FE                  ; 6A70: 8E 03 FE      
                    STX   ,U                      ; 6A73: AF C4         
                    BRA   $6A81                   ; 6A75: 20 0A         

; ---------------------------------------------------------------------------
L6A77               CMPX  #$FC02                  ; 6A77: 8C FC 02      
                    BGE   $6A81                   ; 6A7A: 2C 05         
                    LDX   #$FC02                  ; 6A7C: 8E FC 02      
                    STX   ,U                      ; 6A7F: AF C4         
L6A81               LDX   $02,U                   ; 6A81: AE 42         
                    CMPX  #$04C0                  ; 6A83: 8C 04 C0      
                    BLE   $6A8F                   ; 6A86: 2F 07         
                    LDX   #$04C0                  ; 6A88: 8E 04 C0      
                    STX   $02,U                   ; 6A8B: AF 42         
                    BRA   $6A99                   ; 6A8D: 20 0A         

; ---------------------------------------------------------------------------
L6A8F               CMPX  #$FC04                  ; 6A8F: 8C FC 04      
                    BGE   $6A99                   ; 6A92: 2C 05         
                    LDX   #$FC04                  ; 6A94: 8E FC 04      
                    STX   $02,U                   ; 6A97: AF 42         
L6A99               LEAU  $10,U                   ; 6A99: 33 C8 10      
                    DECA                          ; 6A9C: 4A            
                    BGT   $6A69                   ; 6A9D: 2E CA         
; End of function sub_6A0C
                    RTS                           ; 6A9F: 39            

; =============== S U B R O U T I N E =======================================
L6AA0               LDX   #$7610                  ; 6AA0: 8E 76 10      
                    LDB   <$DC                    ; 6AA3: D6 DC           // Get object selected
                    ASLB                          ; 6AA5: 58            
                    LDU   B,X                     ; 6AA6: EE 85         
                    LDB   ,U+                     ; 6AA8: E6 C0         
                    CMPB  #$01                    ; 6AAA: C1 01           // Check that start of function has illegal $01 instruction byte. Why??
                    BEQ   $6AAF                   ; 6AAC: 27 01         
                    SWI                           ; 6AAE: 3F            
; End of function sub_6AA0
; ---------------------------------------------------------------------------
L6AAF               JMP   ,U                      ; 6AAF: 6E C4         

; =============== S U B R O U T I N E =======================================
L6C1C               LDA   #$5E                    ; 6C1C: 86 5E           // Draws bunkers before exploded
                    TFR   A,DP                    ; 6C1E: 1F 8B           // Change zero page to Math RAM
                    LDD   <$0E                    ; 6C20: DC 0E         
                    ADDD  #$FF98                  ; 6C22: C3 FF 98      
                    ANDA  #$1F                    ; 6C25: 84 1F         
                    STD   ,Y                      ; 6C27: ED A4           // Y points to vector RAM
                    LDD   <$0C                    ; 6C29: DC 0C         
                    ANDA  #$1F                    ; 6C2B: 84 1F         
                    STD   $02,Y                   ; 6C2D: ED 22         
                    LDD   <$04                    ; 6C2F: DC 04         
                    STD   $04,Y                   ; 6C31: ED 24         
                    LDD   <$00                    ; 6C33: DC 00         
                    STD   $06,Y                   ; 6C35: ED 26         
                    LDD   <$1E                    ; 6C37: DC 1E         
                    SUBD  <$0E                    ; 6C39: 93 0E         
                    ANDA  #$1F                    ; 6C3B: 84 1F         
                    STD   $08,Y                   ; 6C3D: ED 28         
                    LDD   <$1C                    ; 6C3F: DC 1C         
                    SUBD  <$0C                    ; 6C41: 93 0C         
                    ANDA  #$1F                    ; 6C43: 84 1F         
                    STD   $0A,Y                   ; 6C45: ED 2A         
                    LDD   <$2E                    ; 6C47: DC 2E         
                    SUBD  <$1E                    ; 6C49: 93 1E         
                    ANDA  #$1F                    ; 6C4B: 84 1F         
                    STD   $0C,Y                   ; 6C4D: ED 2C         
                    LDD   <$2C                    ; 6C4F: DC 2C         
                    SUBD  <$1C                    ; 6C51: 93 1C         
                    ORA   #$E0                    ; 6C53: 8A E0         
                    STD   $0E,Y                   ; 6C55: ED 2E         
                    LDD   <$EE                    ; 6C57: DC EE         
                    SUBD  <$2E                    ; 6C59: 93 2E         
                    ANDA  #$1F                    ; 6C5B: 84 1F         
                    STD   $10,Y                   ; 6C5D: ED A8 10      
                    LDD   <$EC                    ; 6C60: DC EC         
                    SUBD  <$2C                    ; 6C62: 93 2C         
                    ORA   #$E0                    ; 6C64: 8A E0         
                    STD   $12,Y                   ; 6C66: ED A8 12      
                    LDD   <$DE                    ; 6C69: DC DE         
                    SUBD  <$EE                    ; 6C6B: 93 EE         
                    ANDA  #$1F                    ; 6C6D: 84 1F         
                    STD   $14,Y                   ; 6C6F: ED A8 14      
                    LDD   <$DC                    ; 6C72: DC DC           // Selected object model
                    SUBD  <$EC                    ; 6C74: 93 EC         
                    ORA   #$E0                    ; 6C76: 8A E0         
                    STD   $16,Y                   ; 6C78: ED A8 16      
                    LDD   <$1E                    ; 6C7B: DC 1E         
                    SUBD  <$DE                    ; 6C7D: 93 DE         
                    ANDA  #$1F                    ; 6C7F: 84 1F         
                    STD   $18,Y                   ; 6C81: ED A8 18      
                    LDD   <$1C                    ; 6C84: DC 1C         
                    SUBD  <$DC                    ; 6C86: 93 DC           // Selected object model
                    ORA   #$E0                    ; 6C88: 8A E0         
                    STD   $1A,Y                   ; 6C8A: ED A8 1A      
                    LDD   <$3E                    ; 6C8D: DC 3E         
                    SUBD  <$1E                    ; 6C8F: 93 1E         
                    ANDA  #$1F                    ; 6C91: 84 1F         
                    STD   $1C,Y                   ; 6C93: ED A8 1C      
                    LDD   <$3C                    ; 6C96: DC 3C         
                    SUBD  <$1C                    ; 6C98: 93 1C         
                    ORA   #$E0                    ; 6C9A: 8A E0         
                    STD   $1E,Y                   ; 6C9C: ED A8 1E      
                    LDD   <$FE                    ; 6C9F: DC FE         
                    SUBD  <$3E                    ; 6CA1: 93 3E         
                    ANDA  #$1F                    ; 6CA3: 84 1F         
                    STD   $20,Y                   ; 6CA5: ED A8 20      
                    LDD   <$FC                    ; 6CA8: DC FC         
                    SUBD  <$3C                    ; 6CAA: 93 3C         
                    ORA   #$E0                    ; 6CAC: 8A E0         
                    STD   $22,Y                   ; 6CAE: ED A8 22      
                    LDD   <$DE                    ; 6CB1: DC DE         
                    SUBD  <$FE                    ; 6CB3: 93 FE         
                    ANDA  #$1F                    ; 6CB5: 84 1F         
                    STD   $24,Y                   ; 6CB7: ED A8 24      
                    LDD   <$DC                    ; 6CBA: DC DC           // Selected object model
                    SUBD  <$FC                    ; 6CBC: 93 FC         
                    ORA   #$E0                    ; 6CBE: 8A E0         
                    STD   $26,Y                   ; 6CC0: ED A8 26      
                    LDD   <$EE                    ; 6CC3: DC EE         
                    SUBD  <$DE                    ; 6CC5: 93 DE         
                    ANDA  #$1F                    ; 6CC7: 84 1F         
                    STD   $28,Y                   ; 6CC9: ED A8 28      
                    LDD   <$EC                    ; 6CCC: DC EC         
                    SUBD  <$DC                    ; 6CCE: 93 DC           // Selected object model
                    ANDA  #$1F                    ; 6CD0: 84 1F         
                    STD   $2A,Y                   ; 6CD2: ED A8 2A      
                    LDD   <$FE                    ; 6CD5: DC FE         
                    SUBD  <$EE                    ; 6CD7: 93 EE         
                    ANDA  #$1F                    ; 6CD9: 84 1F         
                    STD   $2C,Y                   ; 6CDB: ED A8 2C      
                    LDD   <$FC                    ; 6CDE: DC FC         
                    SUBD  <$EC                    ; 6CE0: 93 EC         
                    ORA   #$E0                    ; 6CE2: 8A E0         
                    STD   $2E,Y                   ; 6CE4: ED A8 2E      
                    LDD   #$7200                  ; 6CE7: CC 72 00        // Insert Vector SCAL 2,00 instruction
                    STD   $30,Y                   ; 6CEA: ED A8 30      
                    LDD   #$8040                  ; 6CED: CC 80 40        // Insert Vector CNTR instruction
                    STD   $32,Y                   ; 6CF0: ED A8 32      
                    LEAY  $34,Y                   ; 6CF3: 31 A8 34        // Increment vector RAM pointer by 52 bytes for bunker vector data
                    LDA   #$48                    ; 6CF6: 86 48         
                    TFR   A,DP                    ; 6CF8: 1F 8B         
; End of function sub_6C1C
; ---------------------------------------------------------------------------
                    RTS                           ; 6CFA: 39            

; =============== S U B R O U T I N E =======================================
L6CFC               LDA   #$5E                    ; 6CFC: 86 5E           // Draws towers before top exploded
                    TFR   A,DP                    ; 6CFE: 1F 8B         
                    LDD   <$0E                    ; 6D00: DC 0E         
                    ADDD  #$FF98                  ; 6D02: C3 FF 98      
                    ANDA  #$1F                    ; 6D05: 84 1F         
                    STD   ,Y                      ; 6D07: ED A4         
                    LDD   <$0C                    ; 6D09: DC 0C         
                    ANDA  #$1F                    ; 6D0B: 84 1F         
                    STD   $02,Y                   ; 6D0D: ED 22         
                    LDD   <$04                    ; 6D0F: DC 04         
                    STD   $04,Y                   ; 6D11: ED 24         
                    LDD   <$02                    ; 6D13: DC 02         
                    STD   $06,Y                   ; 6D15: ED 26         
                    LDD   <$1E                    ; 6D17: DC 1E         
                    SUBD  <$0E                    ; 6D19: 93 0E         
                    ANDA  #$1F                    ; 6D1B: 84 1F         
                    STD   $08,Y                   ; 6D1D: ED 28         
                    LDD   <$1C                    ; 6D1F: DC 1C         
                    SUBD  <$0C                    ; 6D21: 93 0C         
                    ANDA  #$1F                    ; 6D23: 84 1F         
                    STD   $0A,Y                   ; 6D25: ED 2A         
                    LDD   <$3E                    ; 6D27: DC 3E         
                    SUBD  <$1E                    ; 6D29: 93 1E         
                    ANDA  #$1F                    ; 6D2B: 84 1F         
                    STD   $0C,Y                   ; 6D2D: ED 2C         
                    LDD   <$3C                    ; 6D2F: DC 3C         
                    SUBD  <$1C                    ; 6D31: 93 1C         
                    ORA   #$E0                    ; 6D33: 8A E0         
                    STD   $0E,Y                   ; 6D35: ED 2E         
                    LDD   <$FE                    ; 6D37: DC FE         
                    SUBD  <$3E                    ; 6D39: 93 3E         
                    ANDA  #$1F                    ; 6D3B: 84 1F         
                    STD   $10,Y                   ; 6D3D: ED A8 10      
                    LDD   <$FC                    ; 6D40: DC FC         
                    SUBD  <$3C                    ; 6D42: 93 3C         
                    ORA   #$E0                    ; 6D44: 8A E0         
                    STD   $12,Y                   ; 6D46: ED A8 12      
                    LDD   <$CE                    ; 6D49: DC CE         
                    SUBD  <$FE                    ; 6D4B: 93 FE         
                    ANDA  #$1F                    ; 6D4D: 84 1F         
                    STD   $14,Y                   ; 6D4F: ED A8 14      
                    LDD   <$CC                    ; 6D52: DC CC         
                    SUBD  <$FC                    ; 6D54: 93 FC         
                    ORA   #$E0                    ; 6D56: 8A E0         
                    STD   $16,Y                   ; 6D58: ED A8 16      
                    LDD   <$9E                    ; 6D5B: DC 9E         
                    SUBD  <$CE                    ; 6D5D: 93 CE         
                    ANDA  #$1F                    ; 6D5F: 84 1F         
                    STD   $18,Y                   ; 6D61: ED A8 18      
                    LDD   <$9C                    ; 6D64: DC 9C         
                    SUBD  <$CC                    ; 6D66: 93 CC         
                    ORA   #$E0                    ; 6D68: 8A E0         
                    STD   $1A,Y                   ; 6D6A: ED A8 1A      
                    LDD   <$00                    ; 6D6D: DC 00         
                    STD   $1C,Y                   ; 6D6F: ED A8 1C      
                    LDD   <$6E                    ; 6D72: DC 6E         
                    SUBD  <$9E                    ; 6D74: 93 9E         
                    ANDA  #$1F                    ; 6D76: 84 1F         
                    STD   $1E,Y                   ; 6D78: ED A8 1E      
                    LDD   <$6C                    ; 6D7B: DC 6C         
                    SUBD  <$9C                    ; 6D7D: 93 9C         
                    ORA   #$E0                    ; 6D7F: 8A E0         
                    STD   $20,Y                   ; 6D81: ED A8 20      
                    LDD   <$4E                    ; 6D84: DC 4E         
                    SUBD  <$6E                    ; 6D86: 93 6E         
                    ANDA  #$1F                    ; 6D88: 84 1F         
                    STD   $22,Y                   ; 6D8A: ED A8 22      
                    LDD   <$4C                    ; 6D8D: DC 4C         
                    SUBD  <$6C                    ; 6D8F: 93 6C         
                    ORA   #$E0                    ; 6D91: 8A E0         
                    STD   $24,Y                   ; 6D93: ED A8 24      
                    LDD   <$7E                    ; 6D96: DC 7E         
                    SUBD  <$4E                    ; 6D98: 93 4E         
                    ANDA  #$1F                    ; 6D9A: 84 1F         
                    STD   $26,Y                   ; 6D9C: ED A8 26      
                    LDD   <$7C                    ; 6D9F: DC 7C         
                    SUBD  <$4C                    ; 6DA1: 93 4C         
                    ORA   #$E0                    ; 6DA3: 8A E0         
                    STD   $28,Y                   ; 6DA5: ED A8 28      
                    LDD   <$02                    ; 6DA8: DC 02         
                    STD   $2A,Y                   ; 6DAA: ED A8 2A      
                    LDD   <$AE                    ; 6DAD: DC AE         
                    SUBD  <$7E                    ; 6DAF: 93 7E         
                    ANDA  #$1F                    ; 6DB1: 84 1F         
                    STD   $2C,Y                   ; 6DB3: ED A8 2C      
                    LDD   <$AC                    ; 6DB6: DC AC         
                    SUBD  <$7C                    ; 6DB8: 93 7C         
                    ORA   #$E0                    ; 6DBA: 8A E0         
                    STD   $2E,Y                   ; 6DBC: ED A8 2E      
                    LDD   <$DE                    ; 6DBF: DC DE         
                    SUBD  <$AE                    ; 6DC1: 93 AE         
                    ANDA  #$1F                    ; 6DC3: 84 1F         
                    STD   $30,Y                   ; 6DC5: ED A8 30      
                    LDD   <$DC                    ; 6DC8: DC DC           // Selected object model
                    SUBD  <$AC                    ; 6DCA: 93 AC         
                    ORA   #$E0                    ; 6DCC: 8A E0         
                    STD   $32,Y                   ; 6DCE: ED A8 32      
                    LDD   <$1E                    ; 6DD1: DC 1E         
                    SUBD  <$DE                    ; 6DD3: 93 DE         
                    ANDA  #$1F                    ; 6DD5: 84 1F         
                    STD   $34,Y                   ; 6DD7: ED A8 34      
                    LDD   <$1C                    ; 6DDA: DC 1C         
                    SUBD  <$DC                    ; 6DDC: 93 DC           // Selected object model
                    ORA   #$E0                    ; 6DDE: 8A E0         
                    STD   $36,Y                   ; 6DE0: ED A8 36      
                    LDD   <$2E                    ; 6DE3: DC 2E         
                    SUBD  <$1E                    ; 6DE5: 93 1E         
                    ANDA  #$1F                    ; 6DE7: 84 1F         
                    STD   $38,Y                   ; 6DE9: ED A8 38      
                    LDD   <$2C                    ; 6DEC: DC 2C         
                    SUBD  <$1C                    ; 6DEE: 93 1C         
                    ORA   #$E0                    ; 6DF0: 8A E0         
                    STD   $3A,Y                   ; 6DF2: ED A8 3A      
                    LDD   <$EE                    ; 6DF5: DC EE         
                    SUBD  <$2E                    ; 6DF7: 93 2E         
                    ANDA  #$1F                    ; 6DF9: 84 1F         
                    STD   $3C,Y                   ; 6DFB: ED A8 3C      
                    LDD   <$EC                    ; 6DFE: DC EC         
                    SUBD  <$2C                    ; 6E00: 93 2C         
                    ORA   #$E0                    ; 6E02: 8A E0         
                    STD   $3E,Y                   ; 6E04: ED A8 3E      
                    LDD   <$BE                    ; 6E07: DC BE         
                    SUBD  <$EE                    ; 6E09: 93 EE         
                    ANDA  #$1F                    ; 6E0B: 84 1F         
                    STD   $40,Y                   ; 6E0D: ED A8 40      
                    LDD   <$BC                    ; 6E10: DC BC         
                    SUBD  <$EC                    ; 6E12: 93 EC         
                    ORA   #$E0                    ; 6E14: 8A E0         
                    STD   $42,Y                   ; 6E16: ED A8 42      
                    LDD   <$8E                    ; 6E19: DC 8E         
                    SUBD  <$BE                    ; 6E1B: 93 BE         
                    ANDA  #$1F                    ; 6E1D: 84 1F         
                    STD   $44,Y                   ; 6E1F: ED A8 44      
                    LDD   <$8C                    ; 6E22: DC 8C         
                    SUBD  <$BC                    ; 6E24: 93 BC         
                    ORA   #$E0                    ; 6E26: 8A E0         
                    STD   $46,Y                   ; 6E28: ED A8 46      
                    LDD   <$00                    ; 6E2B: DC 00         
                    STD   $48,Y                   ; 6E2D: ED A8 48      
                    LDD   <$5E                    ; 6E30: DC 5E         
                    SUBD  <$8E                    ; 6E32: 93 8E         
                    ANDA  #$1F                    ; 6E34: 84 1F         
                    STD   $4A,Y                   ; 6E36: ED A8 4A      
                    LDD   <$5C                    ; 6E39: DC 5C         
                    SUBD  <$8C                    ; 6E3B: 93 8C         
                    ORA   #$E0                    ; 6E3D: 8A E0         
                    STD   $4C,Y                   ; 6E3F: ED A8 4C      
                    LDD   <$4E                    ; 6E42: DC 4E         
                    SUBD  <$5E                    ; 6E44: 93 5E         
                    ANDA  #$1F                    ; 6E46: 84 1F         
                    STD   $4E,Y                   ; 6E48: ED A8 4E      
                    LDD   <$4C                    ; 6E4B: DC 4C         
                    SUBD  <$5C                    ; 6E4D: 93 5C         
                    ORA   #$E0                    ; 6E4F: 8A E0         
                    STD   $50,Y                   ; 6E51: ED A8 50      
                    LDD   <$7E                    ; 6E54: DC 7E         
                    SUBD  <$4E                    ; 6E56: 93 4E         
                    ANDA  #$1F                    ; 6E58: 84 1F         
                    STD   $52,Y                   ; 6E5A: ED A8 52      
                    LDD   <$7C                    ; 6E5D: DC 7C         
                    SUBD  <$4C                    ; 6E5F: 93 4C         
                    ANDA  #$1F                    ; 6E61: 84 1F         
                    STD   $54,Y                   ; 6E63: ED A8 54      
                    LDD   <$9E                    ; 6E66: DC 9E         
                    SUBD  <$7E                    ; 6E68: 93 7E         
                    ANDA  #$1F                    ; 6E6A: 84 1F         
                    STD   $56,Y                   ; 6E6C: ED A8 56      
                    LDD   <$9C                    ; 6E6F: DC 9C         
                    SUBD  <$7C                    ; 6E71: 93 7C         
                    ORA   #$E0                    ; 6E73: 8A E0         
                    STD   $58,Y                   ; 6E75: ED A8 58      
                    LDD   <$7E                    ; 6E78: DC 7E         
                    SUBD  <$9E                    ; 6E7A: 93 9E         
                    ANDA  #$1F                    ; 6E7C: 84 1F         
                    STD   $5A,Y                   ; 6E7E: ED A8 5A      
                    LDD   <$7C                    ; 6E81: DC 7C         
                    SUBD  <$9C                    ; 6E83: 93 9C         
                    ANDA  #$1F                    ; 6E85: 84 1F         
                    STD   $5C,Y                   ; 6E87: ED A8 5C      
                    LDD   <$8E                    ; 6E8A: DC 8E         
                    SUBD  <$7E                    ; 6E8C: 93 7E         
                    ANDA  #$1F                    ; 6E8E: 84 1F         
                    STD   $5E,Y                   ; 6E90: ED A8 5E      
                    LDD   <$8C                    ; 6E93: DC 8C         
                    SUBD  <$7C                    ; 6E95: 93 7C         
                    ORA   #$E0                    ; 6E97: 8A E0         
                    STD   $60,Y                   ; 6E99: ED A8 60      
                    LDD   #$7200                  ; 6E9C: CC 72 00        // Vector SCAL 2,0 instruction
                    STD   $62,Y                   ; 6E9F: ED A8 62      
                    LDD   #$8040                  ; 6EA2: CC 80 40        // Insert Vector CNTR instruction
                    STD   $64,Y                   ; 6EA5: ED A8 64      
                    LEAY  $66,Y                   ; 6EA8: 31 A8 66      
                    LDA   #$48                    ; 6EAB: 86 48         
                    TFR   A,DP                    ; 6EAD: 1F 8B         
; End of function sub_6CFC
; ---------------------------------------------------------------------------
                    RTS                           ; 6EAF: 39            

; =============== S U B R O U T I N E =======================================
L6EB1               LDA   #$5E                    ; 6EB1: 86 5E           // Draws tower after top exploded
                    TFR   A,DP                    ; 6EB3: 1F 8B         
                    LDD   <$0E                    ; 6EB5: DC 0E         
                    ADDD  #$FF98                  ; 6EB7: C3 FF 98      
                    ANDA  #$1F                    ; 6EBA: 84 1F         
                    STD   ,Y                      ; 6EBC: ED A4         
                    LDD   <$0C                    ; 6EBE: DC 0C         
                    ANDA  #$1F                    ; 6EC0: 84 1F         
                    STD   $02,Y                   ; 6EC2: ED 22         
                    LDD   <$04                    ; 6EC4: DC 04         
                    STD   $04,Y                   ; 6EC6: ED 24         
                    LDD   <$02                    ; 6EC8: DC 02         
                    STD   $06,Y                   ; 6ECA: ED 26         
                    LDD   <$1E                    ; 6ECC: DC 1E         
                    SUBD  <$0E                    ; 6ECE: 93 0E         
                    ANDA  #$1F                    ; 6ED0: 84 1F         
                    STD   $08,Y                   ; 6ED2: ED 28         
                    LDD   <$1C                    ; 6ED4: DC 1C         
                    SUBD  <$0C                    ; 6ED6: 93 0C         
                    ANDA  #$1F                    ; 6ED8: 84 1F         
                    STD   $0A,Y                   ; 6EDA: ED 2A         
                    LDD   <$3E                    ; 6EDC: DC 3E         
                    SUBD  <$1E                    ; 6EDE: 93 1E         
                    ANDA  #$1F                    ; 6EE0: 84 1F         
                    STD   $0C,Y                   ; 6EE2: ED 2C         
                    LDD   <$3C                    ; 6EE4: DC 3C         
                    SUBD  <$1C                    ; 6EE6: 93 1C         
                    ORA   #$E0                    ; 6EE8: 8A E0         
                    STD   $0E,Y                   ; 6EEA: ED 2E         
                    LDD   <$FE                    ; 6EEC: DC FE         
                    SUBD  <$3E                    ; 6EEE: 93 3E         
                    ANDA  #$1F                    ; 6EF0: 84 1F         
                    STD   $10,Y                   ; 6EF2: ED A8 10      
                    LDD   <$FC                    ; 6EF5: DC FC         
                    SUBD  <$3C                    ; 6EF7: 93 3C         
                    ORA   #$E0                    ; 6EF9: 8A E0         
                    STD   $12,Y                   ; 6EFB: ED A8 12      
                    LDD   <$CE                    ; 6EFE: DC CE         
                    SUBD  <$FE                    ; 6F00: 93 FE         
                    ANDA  #$1F                    ; 6F02: 84 1F         
                    STD   $14,Y                   ; 6F04: ED A8 14      
                    LDD   <$CC                    ; 6F07: DC CC         
                    SUBD  <$FC                    ; 6F09: 93 FC         
                    ORA   #$E0                    ; 6F0B: 8A E0         
                    STD   $16,Y                   ; 6F0D: ED A8 16      
                    LDD   <$9E                    ; 6F10: DC 9E         
                    SUBD  <$CE                    ; 6F12: 93 CE         
                    ANDA  #$1F                    ; 6F14: 84 1F         
                    STD   $18,Y                   ; 6F16: ED A8 18      
                    LDD   <$9C                    ; 6F19: DC 9C         
                    SUBD  <$CC                    ; 6F1B: 93 CC         
                    ORA   #$E0                    ; 6F1D: 8A E0         
                    STD   $1A,Y                   ; 6F1F: ED A8 1A      
                    LDD   <$7E                    ; 6F22: DC 7E         
                    SUBD  <$9E                    ; 6F24: 93 9E         
                    ANDA  #$1F                    ; 6F26: 84 1F         
                    STD   $1C,Y                   ; 6F28: ED A8 1C      
                    LDD   <$7C                    ; 6F2B: DC 7C         
                    SUBD  <$9C                    ; 6F2D: 93 9C         
                    ORA   #$E0                    ; 6F2F: 8A E0         
                    STD   $1E,Y                   ; 6F31: ED A8 1E      
                    LDD   <$AE                    ; 6F34: DC AE         
                    SUBD  <$7E                    ; 6F36: 93 7E         
                    ANDA  #$1F                    ; 6F38: 84 1F         
                    STD   $20,Y                   ; 6F3A: ED A8 20      
                    LDD   <$AC                    ; 6F3D: DC AC         
                    SUBD  <$7C                    ; 6F3F: 93 7C         
                    ORA   #$E0                    ; 6F41: 8A E0         
                    STD   $22,Y                   ; 6F43: ED A8 22      
                    LDD   <$DE                    ; 6F46: DC DE         
                    SUBD  <$AE                    ; 6F48: 93 AE         
                    ANDA  #$1F                    ; 6F4A: 84 1F         
                    STD   $24,Y                   ; 6F4C: ED A8 24      
                    LDD   <$DC                    ; 6F4F: DC DC           // Selected object model
                    SUBD  <$AC                    ; 6F51: 93 AC         
                    ORA   #$E0                    ; 6F53: 8A E0         
                    STD   $26,Y                   ; 6F55: ED A8 26      
                    LDD   <$1E                    ; 6F58: DC 1E         
                    SUBD  <$DE                    ; 6F5A: 93 DE         
                    ANDA  #$1F                    ; 6F5C: 84 1F         
                    STD   $28,Y                   ; 6F5E: ED A8 28      
                    LDD   <$1C                    ; 6F61: DC 1C         
                    SUBD  <$DC                    ; 6F63: 93 DC           // Selected object model
                    ORA   #$E0                    ; 6F65: 8A E0         
                    STD   $2A,Y                   ; 6F67: ED A8 2A      
                    LDD   <$2E                    ; 6F6A: DC 2E         
                    SUBD  <$1E                    ; 6F6C: 93 1E         
                    ANDA  #$1F                    ; 6F6E: 84 1F         
                    STD   $2C,Y                   ; 6F70: ED A8 2C      
                    LDD   <$2C                    ; 6F73: DC 2C         
                    SUBD  <$1C                    ; 6F75: 93 1C         
                    ORA   #$E0                    ; 6F77: 8A E0         
                    STD   $2E,Y                   ; 6F79: ED A8 2E      
                    LDD   <$EE                    ; 6F7C: DC EE         
                    SUBD  <$2E                    ; 6F7E: 93 2E         
                    ANDA  #$1F                    ; 6F80: 84 1F         
                    STD   $30,Y                   ; 6F82: ED A8 30      
                    LDD   <$EC                    ; 6F85: DC EC         
                    SUBD  <$2C                    ; 6F87: 93 2C         
                    ORA   #$E0                    ; 6F89: 8A E0         
                    STD   $32,Y                   ; 6F8B: ED A8 32      
                    LDD   <$BE                    ; 6F8E: DC BE         
                    SUBD  <$EE                    ; 6F90: 93 EE         
                    ANDA  #$1F                    ; 6F92: 84 1F         
                    STD   $34,Y                   ; 6F94: ED A8 34      
                    LDD   <$BC                    ; 6F97: DC BC         
                    SUBD  <$EC                    ; 6F99: 93 EC         
                    ORA   #$E0                    ; 6F9B: 8A E0         
                    STD   $36,Y                   ; 6F9D: ED A8 36      
                    LDD   <$8E                    ; 6FA0: DC 8E         
                    SUBD  <$BE                    ; 6FA2: 93 BE         
                    ANDA  #$1F                    ; 6FA4: 84 1F         
                    STD   $38,Y                   ; 6FA6: ED A8 38      
                    LDD   <$8C                    ; 6FA9: DC 8C         
                    SUBD  <$BC                    ; 6FAB: 93 BC         
                    ORA   #$E0                    ; 6FAD: 8A E0         
                    STD   $3A,Y                   ; 6FAF: ED A8 3A      
                    LDD   <$7E                    ; 6FB2: DC 7E         
                    SUBD  <$8E                    ; 6FB4: 93 8E         
                    ANDA  #$1F                    ; 6FB6: 84 1F         
                    STD   $3C,Y                   ; 6FB8: ED A8 3C      
                    LDD   <$7C                    ; 6FBB: DC 7C         
                    SUBD  <$8C                    ; 6FBD: 93 8C         
                    ORA   #$E0                    ; 6FBF: 8A E0         
                    STD   $3E,Y                   ; 6FC1: ED A8 3E      
                    LDD   #$7200                  ; 6FC4: CC 72 00      
                    STD   $40,Y                   ; 6FC7: ED A8 40      
                    LDD   #$8040                  ; 6FCA: CC 80 40        // Insert Vector CNTR instruction
                    STD   $42,Y                   ; 6FCD: ED A8 42      
                    LEAY  $44,Y                   ; 6FD0: 31 A8 44      
                    LDA   #$48                    ; 6FD3: 86 48         
                    TFR   A,DP                    ; 6FD5: 1F 8B         
; End of function sub_6EB1
; ---------------------------------------------------------------------------
                    RTS                           ; 6FD7: 39            

; =============== S U B R O U T I N E =======================================
L6FD9               LDA   #$5E                    ; 6FD9: 86 5E           // Draws trench turrets
                    TFR   A,DP                    ; 6FDB: 1F 8B         
                    LDD   <$4E                    ; 6FDD: DC 4E         
                    ADDD  #$FF98                  ; 6FDF: C3 FF 98      
                    ANDA  #$1F                    ; 6FE2: 84 1F         
                    STD   ,Y                      ; 6FE4: ED A4         
                    LDD   <$4C                    ; 6FE6: DC 4C         
                    ANDA  #$1F                    ; 6FE8: 84 1F         
                    STD   $02,Y                   ; 6FEA: ED 22         
                    LDD   #$6480                  ; 6FEC: CC 64 80      
                    STD   $04,Y                   ; 6FEF: ED 24         
                    LDD   <$3E                    ; 6FF1: DC 3E         
                    SUBD  <$4E                    ; 6FF3: 93 4E         
                    ANDA  #$1F                    ; 6FF5: 84 1F         
                    STD   $06,Y                   ; 6FF7: ED 26         
                    LDD   <$3C                    ; 6FF9: DC 3C         
                    SUBD  <$4C                    ; 6FFB: 93 4C         
                    ORA   #$E0                    ; 6FFD: 8A E0         
                    STD   $08,Y                   ; 6FFF: ED 28         
                    LDD   <$2E                    ; 7001: DC 2E         
                    SUBD  <$3E                    ; 7003: 93 3E         
                    ANDA  #$1F                    ; 7005: 84 1F         
                    STD   $0A,Y                   ; 7007: ED 2A         
                    LDD   <$2C                    ; 7009: DC 2C         
                    SUBD  <$3C                    ; 700B: 93 3C         
                    ORA   #$E0                    ; 700D: 8A E0         
                    STD   $0C,Y                   ; 700F: ED 2C         
                    LDD   <$1E                    ; 7011: DC 1E         
                    SUBD  <$2E                    ; 7013: 93 2E         
                    ANDA  #$1F                    ; 7015: 84 1F         
                    STD   $0E,Y                   ; 7017: ED 2E         
                    LDD   <$1C                    ; 7019: DC 1C         
                    SUBD  <$2C                    ; 701B: 93 2C         
                    ORA   #$E0                    ; 701D: 8A E0         
                    STD   $10,Y                   ; 701F: ED A8 10      
                    LDD   <$4E                    ; 7022: DC 4E         
                    SUBD  <$1E                    ; 7024: 93 1E         
                    ANDA  #$1F                    ; 7026: 84 1F         
                    STD   $12,Y                   ; 7028: ED A8 12      
                    LDD   <$4C                    ; 702B: DC 4C         
                    SUBD  <$1C                    ; 702D: 93 1C         
                    ORA   #$E0                    ; 702F: 8A E0         
                    STD   $14,Y                   ; 7031: ED A8 14      
                    LDD   <$9E                    ; 7034: DC 9E         
                    SUBD  <$4E                    ; 7036: 93 4E         
                    ANDA  #$1F                    ; 7038: 84 1F         
                    STD   $16,Y                   ; 703A: ED A8 16      
                    LDD   <$9C                    ; 703D: DC 9C         
                    SUBD  <$4C                    ; 703F: 93 4C         
                    ORA   #$E0                    ; 7041: 8A E0         
                    STD   $18,Y                   ; 7043: ED A8 18      
                    LDD   <$AE                    ; 7046: DC AE         
                    SUBD  <$9E                    ; 7048: 93 9E         
                    ANDA  #$1F                    ; 704A: 84 1F         
                    STD   $1A,Y                   ; 704C: ED A8 1A      
                    LDD   <$AC                    ; 704F: DC AC         
                    SUBD  <$9C                    ; 7051: 93 9C         
                    ORA   #$E0                    ; 7053: 8A E0         
                    STD   $1C,Y                   ; 7055: ED A8 1C      
                    LDD   <$1E                    ; 7058: DC 1E         
                    SUBD  <$AE                    ; 705A: 93 AE         
                    ANDA  #$1F                    ; 705C: 84 1F         
                    STD   $1E,Y                   ; 705E: ED A8 1E      
                    LDD   <$1C                    ; 7061: DC 1C         
                    SUBD  <$AC                    ; 7063: 93 AC         
                    ORA   #$E0                    ; 7065: 8A E0         
                    STD   $20,Y                   ; 7067: ED A8 20      
                    LDD   <$AE                    ; 706A: DC AE         
                    SUBD  <$1E                    ; 706C: 93 1E         
                    ANDA  #$1F                    ; 706E: 84 1F         
                    STD   $22,Y                   ; 7070: ED A8 22      
                    LDD   <$AC                    ; 7073: DC AC         
                    SUBD  <$1C                    ; 7075: 93 1C         
                    ANDA  #$1F                    ; 7077: 84 1F         
                    STD   $24,Y                   ; 7079: ED A8 24      
                    LDD   <$6E                    ; 707C: DC 6E         
                    SUBD  <$AE                    ; 707E: 93 AE         
                    ANDA  #$1F                    ; 7080: 84 1F         
                    STD   $26,Y                   ; 7082: ED A8 26      
                    LDD   <$6C                    ; 7085: DC 6C         
                    SUBD  <$AC                    ; 7087: 93 AC         
                    ORA   #$E0                    ; 7089: 8A E0         
                    STD   $28,Y                   ; 708B: ED A8 28      
                    LDD   <$2E                    ; 708E: DC 2E         
                    SUBD  <$6E                    ; 7090: 93 6E         
                    ANDA  #$1F                    ; 7092: 84 1F         
                    STD   $2A,Y                   ; 7094: ED A8 2A      
                    LDD   <$2C                    ; 7097: DC 2C         
                    SUBD  <$6C                    ; 7099: 93 6C         
                    ORA   #$E0                    ; 709B: 8A E0         
                    STD   $2C,Y                   ; 709D: ED A8 2C      
                    LDD   <$3E                    ; 70A0: DC 3E         
                    SUBD  <$2E                    ; 70A2: 93 2E         
                    ANDA  #$1F                    ; 70A4: 84 1F         
                    STD   $2E,Y                   ; 70A6: ED A8 2E      
                    LDD   <$3C                    ; 70A9: DC 3C         
                    SUBD  <$2C                    ; 70AB: 93 2C         
                    ANDA  #$1F                    ; 70AD: 84 1F         
                    STD   $30,Y                   ; 70AF: ED A8 30      
                    LDD   <$7E                    ; 70B2: DC 7E         
                    SUBD  <$3E                    ; 70B4: 93 3E         
                    ANDA  #$1F                    ; 70B6: 84 1F         
                    STD   $32,Y                   ; 70B8: ED A8 32      
                    LDD   <$7C                    ; 70BB: DC 7C         
                    SUBD  <$3C                    ; 70BD: 93 3C         
                    ORA   #$E0                    ; 70BF: 8A E0         
                    STD   $34,Y                   ; 70C1: ED A8 34      
                    LDD   <$6E                    ; 70C4: DC 6E         
                    SUBD  <$7E                    ; 70C6: 93 7E         
                    ANDA  #$1F                    ; 70C8: 84 1F         
                    STD   $36,Y                   ; 70CA: ED A8 36      
                    LDD   <$6C                    ; 70CD: DC 6C         
                    SUBD  <$7C                    ; 70CF: 93 7C         
                    ORA   #$E0                    ; 70D1: 8A E0         
                    STD   $38,Y                   ; 70D3: ED A8 38      
                    LDD   <$5E                    ; 70D6: DC 5E         
                    SUBD  <$6E                    ; 70D8: 93 6E         
                    ANDA  #$1F                    ; 70DA: 84 1F         
                    STD   $3A,Y                   ; 70DC: ED A8 3A      
                    LDD   <$5C                    ; 70DF: DC 5C         
                    SUBD  <$6C                    ; 70E1: 93 6C         
                    ORA   #$E0                    ; 70E3: 8A E0         
                    STD   $3C,Y                   ; 70E5: ED A8 3C      
                    LDD   <$8E                    ; 70E8: DC 8E         
                    SUBD  <$5E                    ; 70EA: 93 5E         
                    ANDA  #$1F                    ; 70EC: 84 1F         
                    STD   $3E,Y                   ; 70EE: ED A8 3E      
                    LDD   <$8C                    ; 70F1: DC 8C         
                    SUBD  <$5C                    ; 70F3: 93 5C         
                    ORA   #$E0                    ; 70F5: 8A E0         
                    STD   $40,Y                   ; 70F7: ED A8 40      
                    LDD   <$7E                    ; 70FA: DC 7E         
                    SUBD  <$8E                    ; 70FC: 93 8E         
                    ANDA  #$1F                    ; 70FE: 84 1F         
                    STD   $42,Y                   ; 7100: ED A8 42      
                    LDD   <$7C                    ; 7103: DC 7C         
                    SUBD  <$8C                    ; 7105: 93 8C         
                    ORA   #$E0                    ; 7107: 8A E0         
                    STD   $44,Y                   ; 7109: ED A8 44      
                    LDD   <$9E                    ; 710C: DC 9E         
                    SUBD  <$7E                    ; 710E: 93 7E         
                    ANDA  #$1F                    ; 7110: 84 1F         
                    STD   $46,Y                   ; 7112: ED A8 46      
                    LDD   <$9C                    ; 7115: DC 9C         
                    SUBD  <$7C                    ; 7117: 93 7C         
                    ORA   #$E0                    ; 7119: 8A E0         
                    STD   $48,Y                   ; 711B: ED A8 48      
                    LDD   <$8E                    ; 711E: DC 8E         
                    SUBD  <$9E                    ; 7120: 93 9E         
                    ANDA  #$1F                    ; 7122: 84 1F         
                    STD   $4A,Y                   ; 7124: ED A8 4A      
                    LDD   <$8C                    ; 7127: DC 8C         
                    SUBD  <$9C                    ; 7129: 93 9C         
                    ORA   #$E0                    ; 712B: 8A E0         
                    STD   $4C,Y                   ; 712D: ED A8 4C      
                    LDD   <$DE                    ; 7130: DC DE         
                    SUBD  <$8E                    ; 7132: 93 8E         
                    ANDA  #$1F                    ; 7134: 84 1F         
                    STD   $4E,Y                   ; 7136: ED A8 4E      
                    LDD   <$DC                    ; 7139: DC DC           // Selected object model
                    SUBD  <$8C                    ; 713B: 93 8C         
                    ORA   #$E0                    ; 713D: 8A E0         
                    STD   $50,Y                   ; 713F: ED A8 50      
                    LDD   <$EE                    ; 7142: DC EE         
                    SUBD  <$DE                    ; 7144: 93 DE         
                    ANDA  #$1F                    ; 7146: 84 1F         
                    STD   $52,Y                   ; 7148: ED A8 52      
                    LDD   <$EC                    ; 714B: DC EC         
                    SUBD  <$DC                    ; 714D: 93 DC           // Selected object model
                    ORA   #$E0                    ; 714F: 8A E0         
                    STD   $54,Y                   ; 7151: ED A8 54      
                    LDD   <$9E                    ; 7154: DC 9E         
                    SUBD  <$EE                    ; 7156: 93 EE         
                    ANDA  #$1F                    ; 7158: 84 1F         
                    STD   $56,Y                   ; 715A: ED A8 56      
                    LDD   <$9C                    ; 715D: DC 9C         
                    SUBD  <$EC                    ; 715F: 93 EC         
                    ORA   #$E0                    ; 7161: 8A E0         
                    STD   $58,Y                   ; 7163: ED A8 58      
                    LDD   <$EE                    ; 7166: DC EE         
                    SUBD  <$9E                    ; 7168: 93 9E         
                    ANDA  #$1F                    ; 716A: 84 1F         
                    STD   $5A,Y                   ; 716C: ED A8 5A      
                    LDD   <$EC                    ; 716F: DC EC         
                    SUBD  <$9C                    ; 7171: 93 9C         
                    ANDA  #$1F                    ; 7173: 84 1F         
                    STD   $5C,Y                   ; 7175: ED A8 5C      
                    LDD   <$CE                    ; 7178: DC CE         
                    SUBD  <$EE                    ; 717A: 93 EE         
                    ANDA  #$1F                    ; 717C: 84 1F         
                    STD   $5E,Y                   ; 717E: ED A8 5E      
                    LDD   <$CC                    ; 7181: DC CC         
                    SUBD  <$EC                    ; 7183: 93 EC         
                    ORA   #$E0                    ; 7185: 8A E0         
                    STD   $60,Y                   ; 7187: ED A8 60      
                    LDD   <$AE                    ; 718A: DC AE         
                    SUBD  <$CE                    ; 718C: 93 CE         
                    ANDA  #$1F                    ; 718E: 84 1F         
                    STD   $62,Y                   ; 7190: ED A8 62      
                    LDD   <$AC                    ; 7193: DC AC         
                    SUBD  <$CC                    ; 7195: 93 CC         
                    ORA   #$E0                    ; 7197: 8A E0         
                    STD   $64,Y                   ; 7199: ED A8 64      
                    LDD   <$5E                    ; 719C: DC 5E         
                    SUBD  <$AE                    ; 719E: 93 AE         
                    ANDA  #$1F                    ; 71A0: 84 1F         
                    STD   $66,Y                   ; 71A2: ED A8 66      
                    LDD   <$5C                    ; 71A5: DC 5C         
                    SUBD  <$AC                    ; 71A7: 93 AC         
                    ORA   #$E0                    ; 71A9: 8A E0         
                    STD   $68,Y                   ; 71AB: ED A8 68      
                    LDD   <$BE                    ; 71AE: DC BE         
                    SUBD  <$5E                    ; 71B0: 93 5E         
                    ANDA  #$1F                    ; 71B2: 84 1F         
                    STD   $6A,Y                   ; 71B4: ED A8 6A      
                    LDD   <$BC                    ; 71B7: DC BC         
                    SUBD  <$5C                    ; 71B9: 93 5C         
                    ORA   #$E0                    ; 71BB: 8A E0         
                    STD   $6C,Y                   ; 71BD: ED A8 6C      
                    LDD   <$CE                    ; 71C0: DC CE         
                    SUBD  <$BE                    ; 71C2: 93 BE         
                    ANDA  #$1F                    ; 71C4: 84 1F         
                    STD   $6E,Y                   ; 71C6: ED A8 6E      
                    LDD   <$CC                    ; 71C9: DC CC         
                    SUBD  <$BC                    ; 71CB: 93 BC         
                    ORA   #$E0                    ; 71CD: 8A E0         
                    STD   $70,Y                   ; 71CF: ED A8 70      
                    LDD   <$BE                    ; 71D2: DC BE         
                    SUBD  <$CE                    ; 71D4: 93 CE         
                    ANDA  #$1F                    ; 71D6: 84 1F         
                    STD   $72,Y                   ; 71D8: ED A8 72      
                    LDD   <$BC                    ; 71DB: DC BC         
                    SUBD  <$CC                    ; 71DD: 93 CC         
                    ANDA  #$1F                    ; 71DF: 84 1F         
                    STD   $74,Y                   ; 71E1: ED A8 74      
                    LDD   <$DE                    ; 71E4: DC DE         
                    SUBD  <$BE                    ; 71E6: 93 BE         
                    ANDA  #$1F                    ; 71E8: 84 1F         
                    STD   $76,Y                   ; 71EA: ED A8 76      
                    LDD   <$DC                    ; 71ED: DC DC           // Selected object model
                    SUBD  <$BC                    ; 71EF: 93 BC         
                    ORA   #$E0                    ; 71F1: 8A E0         
                    STD   $78,Y                   ; 71F3: ED A8 78      
                    LDD   #$7200                  ; 71F6: CC 72 00      
                    STD   $7A,Y                   ; 71F9: ED A8 7A      
                    LDD   #$8040                  ; 71FC: CC 80 40        // Insert Vector CNTR instruction
                    STD   $7C,Y                   ; 71FF: ED A8 7C      
                    LEAY  $7E,Y                   ; 7202: 31 A8 7E      
                    LDA   #$48                    ; 7205: 86 48         
                    TFR   A,DP                    ; 7207: 1F 8B         
; End of function sub_6FD9
; ---------------------------------------------------------------------------
                    RTS                           ; 7209: 39            

; =============== S U B R O U T I N E =======================================
L720B               LDA   #$5E                    ; 720B: 86 5E           // Draws trench green squares
                    TFR   A,DP                    ; 720D: 1F 8B         
                    LDD   <$0E                    ; 720F: DC 0E         
                    ADDD  #$FF98                  ; 7211: C3 FF 98        // Y axis offset for squares
                    ANDA  #$1F                    ; 7214: 84 1F         
                    STD   ,Y                      ; 7216: ED A4         
                    LDD   <$0C                    ; 7218: DC 0C         
                    ANDA  #$1F                    ; 721A: 84 1F         
                    STD   $02,Y                   ; 721C: ED 22         
                    LDD   #$6280                  ; 721E: CC 62 80        // Insert Vector STAT 2,80 instruction (Green)
                    STD   $04,Y                   ; 7221: ED 24         
                    LDD   <$1E                    ; 7223: DC 1E         
                    SUBD  <$0E                    ; 7225: 93 0E         
                    ANDA  #$1F                    ; 7227: 84 1F         
                    STD   $06,Y                   ; 7229: ED 26         
                    LDD   <$1C                    ; 722B: DC 1C         
                    SUBD  <$0C                    ; 722D: 93 0C         
                    ORA   #$E0                    ; 722F: 8A E0         
                    STD   $08,Y                   ; 7231: ED 28         
                    LDD   <$2E                    ; 7233: DC 2E         
                    SUBD  <$1E                    ; 7235: 93 1E         
                    ANDA  #$1F                    ; 7237: 84 1F         
                    STD   $0A,Y                   ; 7239: ED 2A         
                    LDD   <$2C                    ; 723B: DC 2C         
                    SUBD  <$1C                    ; 723D: 93 1C         
                    ORA   #$E0                    ; 723F: 8A E0         
                    STD   $0C,Y                   ; 7241: ED 2C         
                    LDD   <$3E                    ; 7243: DC 3E         
                    SUBD  <$2E                    ; 7245: 93 2E         
                    ANDA  #$1F                    ; 7247: 84 1F         
                    STD   $0E,Y                   ; 7249: ED 2E         
                    LDD   <$3C                    ; 724B: DC 3C         
                    SUBD  <$2C                    ; 724D: 93 2C         
                    ORA   #$E0                    ; 724F: 8A E0         
                    STD   $10,Y                   ; 7251: ED A8 10      
                    LDD   <$0E                    ; 7254: DC 0E         
                    SUBD  <$3E                    ; 7256: 93 3E         
                    ANDA  #$1F                    ; 7258: 84 1F         
                    STD   $12,Y                   ; 725A: ED A8 12      
                    LDD   <$0C                    ; 725D: DC 0C         
                    SUBD  <$3C                    ; 725F: 93 3C         
                    ORA   #$E0                    ; 7261: 8A E0         
                    STD   $14,Y                   ; 7263: ED A8 14      
                    LDD   <$4E                    ; 7266: DC 4E         
                    SUBD  <$0E                    ; 7268: 93 0E         
                    ANDA  #$1F                    ; 726A: 84 1F         
                    STD   $16,Y                   ; 726C: ED A8 16      
                    LDD   <$4C                    ; 726F: DC 4C         
                    SUBD  <$0C                    ; 7271: 93 0C         
                    ANDA  #$1F                    ; 7273: 84 1F         
                    STD   $18,Y                   ; 7275: ED A8 18      
                    LDD   <$5E                    ; 7278: DC 5E         
                    SUBD  <$4E                    ; 727A: 93 4E         
                    ANDA  #$1F                    ; 727C: 84 1F         
                    STD   $1A,Y                   ; 727E: ED A8 1A      
                    LDD   <$5C                    ; 7281: DC 5C         
                    SUBD  <$4C                    ; 7283: 93 4C         
                    ORA   #$E0                    ; 7285: 8A E0         
                    STD   $1C,Y                   ; 7287: ED A8 1C      
                    LDD   <$6E                    ; 728A: DC 6E         
                    SUBD  <$5E                    ; 728C: 93 5E         
                    ANDA  #$1F                    ; 728E: 84 1F         
                    STD   $1E,Y                   ; 7290: ED A8 1E      
                    LDD   <$6C                    ; 7293: DC 6C         
                    SUBD  <$5C                    ; 7295: 93 5C         
                    ORA   #$E0                    ; 7297: 8A E0         
                    STD   $20,Y                   ; 7299: ED A8 20      
                    LDD   <$7E                    ; 729C: DC 7E         
                    SUBD  <$6E                    ; 729E: 93 6E         
                    ANDA  #$1F                    ; 72A0: 84 1F         
                    STD   $22,Y                   ; 72A2: ED A8 22      
                    LDD   <$7C                    ; 72A5: DC 7C         
                    SUBD  <$6C                    ; 72A7: 93 6C         
                    ORA   #$E0                    ; 72A9: 8A E0         
                    STD   $24,Y                   ; 72AB: ED A8 24      
                    LDD   <$4E                    ; 72AE: DC 4E         
                    SUBD  <$7E                    ; 72B0: 93 7E         
                    ANDA  #$1F                    ; 72B2: 84 1F         
                    STD   $26,Y                   ; 72B4: ED A8 26      
                    LDD   <$4C                    ; 72B7: DC 4C         
                    SUBD  <$7C                    ; 72B9: 93 7C         
                    ORA   #$E0                    ; 72BB: 8A E0         
                    STD   $28,Y                   ; 72BD: ED A8 28      
                    LDD   #$7200                  ; 72C0: CC 72 00      
                    STD   $2A,Y                   ; 72C3: ED A8 2A      
                    LDD   #$8040                  ; 72C6: CC 80 40        // Insert Vector CNTR instruction
                    STD   $2C,Y                   ; 72C9: ED A8 2C      
                    LEAY  $2E,Y                   ; 72CC: 31 A8 2E      
                    LDA   #$48                    ; 72CF: 86 48         
                    TFR   A,DP                    ; 72D1: 1F 8B         
; End of function sub_720B
; ---------------------------------------------------------------------------
                    RTS                           ; 72D3: 39            

; =============== S U B R O U T I N E =======================================
L72D5               LDA   #$5E                    ; 72D5: 86 5E           // Draws trench catwalks
                    TFR   A,DP                    ; 72D7: 1F 8B         
                    LDD   <$1E                    ; 72D9: DC 1E         
                    ADDD  #$FF98                  ; 72DB: C3 FF 98      
                    ANDA  #$1F                    ; 72DE: 84 1F         
                    STD   ,Y                      ; 72E0: ED A4         
                    LDD   <$1C                    ; 72E2: DC 1C         
                    ANDA  #$1F                    ; 72E4: 84 1F         
                    STD   $02,Y                   ; 72E6: ED 22         
                    LDD   <$0E                    ; 72E8: DC 0E         
                    SUBD  <$1E                    ; 72EA: 93 1E         
                    ANDA  #$1F                    ; 72EC: 84 1F         
                    STD   $04,Y                   ; 72EE: ED 24         
                    LDD   <$0C                    ; 72F0: DC 0C         
                    SUBD  <$1C                    ; 72F2: 93 1C         
                    ORA   #$E0                    ; 72F4: 8A E0         
                    STD   $06,Y                   ; 72F6: ED 26         
                    LDD   <$2E                    ; 72F8: DC 2E         
                    SUBD  <$0E                    ; 72FA: 93 0E         
                    ANDA  #$1F                    ; 72FC: 84 1F         
                    STD   $08,Y                   ; 72FE: ED 28         
                    LDD   <$2C                    ; 7300: DC 2C         
                    SUBD  <$0C                    ; 7302: 93 0C         
                    ORA   #$E0                    ; 7304: 8A E0         
                    STD   $0A,Y                   ; 7306: ED 2A         
                    LDD   <$3E                    ; 7308: DC 3E         
                    SUBD  <$2E                    ; 730A: 93 2E         
                    ANDA  #$1F                    ; 730C: 84 1F         
                    STD   $0C,Y                   ; 730E: ED 2C         
                    LDD   <$3C                    ; 7310: DC 3C         
                    SUBD  <$2C                    ; 7312: 93 2C         
                    ORA   #$E0                    ; 7314: 8A E0         
                    STD   $0E,Y                   ; 7316: ED 2E         
                    LDD   <$1E                    ; 7318: DC 1E         
                    SUBD  <$3E                    ; 731A: 93 3E         
                    ANDA  #$1F                    ; 731C: 84 1F         
                    STD   $10,Y                   ; 731E: ED A8 10      
                    LDD   <$1C                    ; 7321: DC 1C         
                    SUBD  <$3C                    ; 7323: 93 3C         
                    ORA   #$E0                    ; 7325: 8A E0         
                    STD   $12,Y                   ; 7327: ED A8 12      
                    LDD   <$5E                    ; 732A: DC 5E         
                    SUBD  <$1E                    ; 732C: 93 1E         
                    ANDA  #$1F                    ; 732E: 84 1F         
                    STD   $14,Y                   ; 7330: ED A8 14      
                    LDD   <$5C                    ; 7333: DC 5C         
                    SUBD  <$1C                    ; 7335: 93 1C         
                    ORA   #$E0                    ; 7337: 8A E0         
                    STD   $16,Y                   ; 7339: ED A8 16      
                    LDD   <$4E                    ; 733C: DC 4E         
                    SUBD  <$5E                    ; 733E: 93 5E         
                    ANDA  #$1F                    ; 7340: 84 1F         
                    STD   $18,Y                   ; 7342: ED A8 18      
                    LDD   <$4C                    ; 7345: DC 4C         
                    SUBD  <$5C                    ; 7347: 93 5C         
                    ORA   #$E0                    ; 7349: 8A E0         
                    STD   $1A,Y                   ; 734B: ED A8 1A      
                    LDD   <$0E                    ; 734E: DC 0E         
                    SUBD  <$4E                    ; 7350: 93 4E         
                    ANDA  #$1F                    ; 7352: 84 1F         
                    STD   $1C,Y                   ; 7354: ED A8 1C      
                    LDD   <$0C                    ; 7357: DC 0C         
                    SUBD  <$4C                    ; 7359: 93 4C         
                    ORA   #$E0                    ; 735B: 8A E0         
                    STD   $1E,Y                   ; 735D: ED A8 1E      
                    LDD   #$7200                  ; 7360: CC 72 00      
                    STD   $20,Y                   ; 7363: ED A8 20      
                    LDD   #$8040                  ; 7366: CC 80 40        // Insert Vector CNTR instruction
                    STD   $22,Y                   ; 7369: ED A8 22      
                    LEAY  $24,Y                   ; 736C: 31 A8 24      
                    LDA   #$48                    ; 736F: 86 48         
                    TFR   A,DP                    ; 7371: 1F 8B         
; End of function sub_72D5
; ---------------------------------------------------------------------------
                    RTS                           ; 7373: 39            

; =============== S U B R O U T I N E =======================================
L7375               LDA   #$5E                    ; 7375: 86 5E           // Tower top explode: Copy math points to vector RAM?
                    TFR   A,DP                    ; 7377: 1F 8B         
                    LDD   <$1E                    ; 7379: DC 1E         
                    ADDD  #$FF98                  ; 737B: C3 FF 98      
                    ANDA  #$1F                    ; 737E: 84 1F         
                    STD   ,Y                      ; 7380: ED A4         
                    LDD   <$1C                    ; 7382: DC 1C         
                    ANDA  #$1F                    ; 7384: 84 1F         
                    STD   $02,Y                   ; 7386: ED 22         
                    LDD   #$A018                  ; 7388: CC A0 18        // Insert Vector JSRL 0018 instruction (STAT for colour fade?)
                    STD   $04,Y                   ; 738B: ED 24         
                    LDD   <$0E                    ; 738D: DC 0E         
                    SUBD  <$1E                    ; 738F: 93 1E         
                    ANDA  #$1F                    ; 7391: 84 1F         
                    STD   $06,Y                   ; 7393: ED 26         
                    LDD   <$0C                    ; 7395: DC 0C         
                    SUBD  <$1C                    ; 7397: 93 1C         
                    ORA   #$E0                    ; 7399: 8A E0         
                    STD   $08,Y                   ; 739B: ED 28         
                    LDD   <$2E                    ; 739D: DC 2E         
                    SUBD  <$0E                    ; 739F: 93 0E         
                    ANDA  #$1F                    ; 73A1: 84 1F         
                    STD   $0A,Y                   ; 73A3: ED 2A         
                    LDD   <$2C                    ; 73A5: DC 2C         
                    SUBD  <$0C                    ; 73A7: 93 0C         
                    ORA   #$E0                    ; 73A9: 8A E0         
                    STD   $0C,Y                   ; 73AB: ED 2C         
                    LDD   <$3E                    ; 73AD: DC 3E         
                    SUBD  <$2E                    ; 73AF: 93 2E         
                    ANDA  #$1F                    ; 73B1: 84 1F         
                    STD   $0E,Y                   ; 73B3: ED 2E         
                    LDD   <$3C                    ; 73B5: DC 3C         
                    SUBD  <$2C                    ; 73B7: 93 2C         
                    ORA   #$E0                    ; 73B9: 8A E0         
                    STD   $10,Y                   ; 73BB: ED A8 10      
                    LDD   <$1E                    ; 73BE: DC 1E         
                    SUBD  <$3E                    ; 73C0: 93 3E         
                    ANDA  #$1F                    ; 73C2: 84 1F         
                    STD   $12,Y                   ; 73C4: ED A8 12      
                    LDD   <$1C                    ; 73C7: DC 1C         
                    SUBD  <$3C                    ; 73C9: 93 3C         
                    ORA   #$E0                    ; 73CB: 8A E0         
                    STD   $14,Y                   ; 73CD: ED A8 14      
                    LDD   <$5E                    ; 73D0: DC 5E         
                    SUBD  <$1E                    ; 73D2: 93 1E         
                    ANDA  #$1F                    ; 73D4: 84 1F         
                    STD   $16,Y                   ; 73D6: ED A8 16      
                    LDD   <$5C                    ; 73D9: DC 5C         
                    SUBD  <$1C                    ; 73DB: 93 1C         
                    ORA   #$E0                    ; 73DD: 8A E0         
                    STD   $18,Y                   ; 73DF: ED A8 18      
                    LDD   <$6E                    ; 73E2: DC 6E         
                    SUBD  <$5E                    ; 73E4: 93 5E         
                    ANDA  #$1F                    ; 73E6: 84 1F         
                    STD   $1A,Y                   ; 73E8: ED A8 1A      
                    LDD   <$6C                    ; 73EB: DC 6C         
                    SUBD  <$5C                    ; 73ED: 93 5C         
                    ORA   #$E0                    ; 73EF: 8A E0         
                    STD   $1C,Y                   ; 73F1: ED A8 1C      
                    LDD   <$3E                    ; 73F4: DC 3E         
                    SUBD  <$6E                    ; 73F6: 93 6E         
                    ANDA  #$1F                    ; 73F8: 84 1F         
                    STD   $1E,Y                   ; 73FA: ED A8 1E      
                    LDD   <$3C                    ; 73FD: DC 3C         
                    SUBD  <$6C                    ; 73FF: 93 6C         
                    ORA   #$E0                    ; 7401: 8A E0         
                    STD   $20,Y                   ; 7403: ED A8 20      
                    LDD   <$5E                    ; 7406: DC 5E         
                    SUBD  <$3E                    ; 7408: 93 3E         
                    ANDA  #$1F                    ; 740A: 84 1F         
                    STD   $22,Y                   ; 740C: ED A8 22      
                    LDD   <$5C                    ; 740F: DC 5C         
                    SUBD  <$3C                    ; 7411: 93 3C         
                    ANDA  #$1F                    ; 7413: 84 1F         
                    STD   $24,Y                   ; 7415: ED A8 24      
                    LDD   <$4E                    ; 7418: DC 4E         
                    SUBD  <$5E                    ; 741A: 93 5E         
                    ANDA  #$1F                    ; 741C: 84 1F         
                    STD   $26,Y                   ; 741E: ED A8 26      
                    LDD   <$4C                    ; 7421: DC 4C         
                    SUBD  <$5C                    ; 7423: 93 5C         
                    ORA   #$E0                    ; 7425: 8A E0         
                    STD   $28,Y                   ; 7427: ED A8 28      
                    LDD   <$0E                    ; 742A: DC 0E         
                    SUBD  <$4E                    ; 742C: 93 4E         
                    ANDA  #$1F                    ; 742E: 84 1F         
                    STD   $2A,Y                   ; 7430: ED A8 2A      
                    LDD   <$0C                    ; 7433: DC 0C         
                    SUBD  <$4C                    ; 7435: 93 4C         
                    ORA   #$E0                    ; 7437: 8A E0         
                    STD   $2C,Y                   ; 7439: ED A8 2C      
                    LDD   #$7200                  ; 743C: CC 72 00      
                    STD   $2E,Y                   ; 743F: ED A8 2E      
                    LDD   #$8040                  ; 7442: CC 80 40        // Insert Vector CNTR instruction
                    STD   $30,Y                   ; 7445: ED A8 30      
                    LEAY  $32,Y                   ; 7448: 31 A8 32      
                    LDA   #$48                    ; 744B: 86 48         
                    TFR   A,DP                    ; 744D: 1F 8B         
; End of function sub_7375
; ---------------------------------------------------------------------------
                    RTS                           ; 744F: 39            

; =============== S U B R O U T I N E =======================================
L7451               LDA   #$5E                    ; 7451: 86 5E           // Draws trench exhaust port
                    TFR   A,DP                    ; 7453: 1F 8B         
                    LDD   <$5E                    ; 7455: DC 5E         
                    ADDD  #$FF98                  ; 7457: C3 FF 98      
                    ANDA  #$1F                    ; 745A: 84 1F         
                    STD   ,Y                      ; 745C: ED A4         
                    LDD   <$5C                    ; 745E: DC 5C         
                    ANDA  #$1F                    ; 7460: 84 1F         
                    STD   $02,Y                   ; 7462: ED 22         
                    LDD   #$6280                  ; 7464: CC 62 80        // Insert Vector STAT 2,80 instruction (Green)
                    STD   $04,Y                   ; 7467: ED 24         
                    LDD   <$9E                    ; 7469: DC 9E         
                    SUBD  <$5E                    ; 746B: 93 5E         
                    ANDA  #$1F                    ; 746D: 84 1F         
                    STD   $06,Y                   ; 746F: ED 26         
                    LDD   <$9C                    ; 7471: DC 9C         
                    SUBD  <$5C                    ; 7473: 93 5C         
                    ORA   #$E0                    ; 7475: 8A E0         
                    STD   $08,Y                   ; 7477: ED 28         
                    LDD   <$8E                    ; 7479: DC 8E         
                    SUBD  <$9E                    ; 747B: 93 9E         
                    ANDA  #$1F                    ; 747D: 84 1F         
                    STD   $0A,Y                   ; 747F: ED 2A         
                    LDD   <$8C                    ; 7481: DC 8C         
                    SUBD  <$9C                    ; 7483: 93 9C         
                    ORA   #$E0                    ; 7485: 8A E0         
                    STD   $0C,Y                   ; 7487: ED 2C         
                    LDD   <$4E                    ; 7489: DC 4E         
                    SUBD  <$8E                    ; 748B: 93 8E         
                    ANDA  #$1F                    ; 748D: 84 1F         
                    STD   $0E,Y                   ; 748F: ED 2E         
                    LDD   <$4C                    ; 7491: DC 4C         
                    SUBD  <$8C                    ; 7493: 93 8C         
                    ORA   #$E0                    ; 7495: 8A E0         
                    STD   $10,Y                   ; 7497: ED A8 10      
                    LDD   <$6E                    ; 749A: DC 6E         
                    SUBD  <$4E                    ; 749C: 93 4E         
                    ANDA  #$1F                    ; 749E: 84 1F         
                    STD   $12,Y                   ; 74A0: ED A8 12      
                    LDD   <$6C                    ; 74A3: DC 6C         
                    SUBD  <$4C                    ; 74A5: 93 4C         
                    ANDA  #$1F                    ; 74A7: 84 1F         
                    STD   $14,Y                   ; 74A9: ED A8 14      
                    LDD   <$AE                    ; 74AC: DC AE         
                    SUBD  <$6E                    ; 74AE: 93 6E         
                    ANDA  #$1F                    ; 74B0: 84 1F         
                    STD   $16,Y                   ; 74B2: ED A8 16      
                    LDD   <$AC                    ; 74B5: DC AC         
                    SUBD  <$6C                    ; 74B7: 93 6C         
                    ORA   #$E0                    ; 74B9: 8A E0         
                    STD   $18,Y                   ; 74BB: ED A8 18      
                    LDD   <$BE                    ; 74BE: DC BE         
                    SUBD  <$AE                    ; 74C0: 93 AE         
                    ANDA  #$1F                    ; 74C2: 84 1F         
                    STD   $1A,Y                   ; 74C4: ED A8 1A      
                    LDD   <$BC                    ; 74C7: DC BC         
                    SUBD  <$AC                    ; 74C9: 93 AC         
                    ORA   #$E0                    ; 74CB: 8A E0         
                    STD   $1C,Y                   ; 74CD: ED A8 1C      
                    LDD   <$7E                    ; 74D0: DC 7E         
                    SUBD  <$BE                    ; 74D2: 93 BE         
                    ANDA  #$1F                    ; 74D4: 84 1F         
                    STD   $1E,Y                   ; 74D6: ED A8 1E      
                    LDD   <$7C                    ; 74D9: DC 7C         
                    SUBD  <$BC                    ; 74DB: 93 BC         
                    ORA   #$E0                    ; 74DD: 8A E0         
                    STD   $20,Y                   ; 74DF: ED A8 20      
                    LDD   #$6380                  ; 74E2: CC 63 80        // Insert Vector STAT 3,80 instruction (Red)
                    STD   $22,Y                   ; 74E5: ED A8 22      
                    LDD   <$6E                    ; 74E8: DC 6E         
                    SUBD  <$7E                    ; 74EA: 93 7E         
                    ANDA  #$1F                    ; 74EC: 84 1F         
                    STD   $24,Y                   ; 74EE: ED A8 24      
                    LDD   <$6C                    ; 74F1: DC 6C         
                    SUBD  <$7C                    ; 74F3: 93 7C         
                    ORA   #$E0                    ; 74F5: 8A E0         
                    STD   $26,Y                   ; 74F7: ED A8 26      
                    LDD   <$2E                    ; 74FA: DC 2E         
                    SUBD  <$6E                    ; 74FC: 93 6E         
                    ANDA  #$1F                    ; 74FE: 84 1F         
                    STD   $28,Y                   ; 7500: ED A8 28      
                    LDD   <$2C                    ; 7503: DC 2C         
                    SUBD  <$6C                    ; 7505: 93 6C         
                    ORA   #$E0                    ; 7507: 8A E0         
                    STD   $2A,Y                   ; 7509: ED A8 2A      
                    LDD   <$6E                    ; 750C: DC 6E         
                    SUBD  <$2E                    ; 750E: 93 2E         
                    ANDA  #$1F                    ; 7510: 84 1F         
                    STD   $2C,Y                   ; 7512: ED A8 2C      
                    LDD   <$6C                    ; 7515: DC 6C         
                    SUBD  <$2C                    ; 7517: 93 2C         
                    ANDA  #$1F                    ; 7519: 84 1F         
                    STD   $2E,Y                   ; 751B: ED A8 2E      
                    LDD   <$4E                    ; 751E: DC 4E         
                    SUBD  <$6E                    ; 7520: 93 6E         
                    ANDA  #$1F                    ; 7522: 84 1F         
                    STD   $30,Y                   ; 7524: ED A8 30      
                    LDD   <$4C                    ; 7527: DC 4C         
                    SUBD  <$6C                    ; 7529: 93 6C         
                    ORA   #$E0                    ; 752B: 8A E0         
                    STD   $32,Y                   ; 752D: ED A8 32      
                    LDD   <$0E                    ; 7530: DC 0E         
                    SUBD  <$4E                    ; 7532: 93 4E         
                    ANDA  #$1F                    ; 7534: 84 1F         
                    STD   $34,Y                   ; 7536: ED A8 34      
                    LDD   <$0C                    ; 7539: DC 0C         
                    SUBD  <$4C                    ; 753B: 93 4C         
                    ORA   #$E0                    ; 753D: 8A E0         
                    STD   $36,Y                   ; 753F: ED A8 36      
                    LDD   <$4E                    ; 7542: DC 4E         
                    SUBD  <$0E                    ; 7544: 93 0E         
                    ANDA  #$1F                    ; 7546: 84 1F         
                    STD   $38,Y                   ; 7548: ED A8 38      
                    LDD   <$4C                    ; 754B: DC 4C         
                    SUBD  <$0C                    ; 754D: 93 0C         
                    ANDA  #$1F                    ; 754F: 84 1F         
                    STD   $3A,Y                   ; 7551: ED A8 3A      
                    LDD   <$5E                    ; 7554: DC 5E         
                    SUBD  <$4E                    ; 7556: 93 4E         
                    ANDA  #$1F                    ; 7558: 84 1F         
                    STD   $3C,Y                   ; 755A: ED A8 3C      
                    LDD   <$5C                    ; 755D: DC 5C         
                    SUBD  <$4C                    ; 755F: 93 4C         
                    ORA   #$E0                    ; 7561: 8A E0         
                    STD   $3E,Y                   ; 7563: ED A8 3E      
                    LDD   <$1E                    ; 7566: DC 1E         
                    SUBD  <$5E                    ; 7568: 93 5E         
                    ANDA  #$1F                    ; 756A: 84 1F         
                    STD   $40,Y                   ; 756C: ED A8 40      
                    LDD   <$1C                    ; 756F: DC 1C         
                    SUBD  <$5C                    ; 7571: 93 5C         
                    ORA   #$E0                    ; 7573: 8A E0         
                    STD   $42,Y                   ; 7575: ED A8 42      
                    LDD   <$5E                    ; 7578: DC 5E         
                    SUBD  <$1E                    ; 757A: 93 1E         
                    ANDA  #$1F                    ; 757C: 84 1F         
                    STD   $44,Y                   ; 757E: ED A8 44      
                    LDD   <$5C                    ; 7581: DC 5C         
                    SUBD  <$1C                    ; 7583: 93 1C         
                    ANDA  #$1F                    ; 7585: 84 1F         
                    STD   $46,Y                   ; 7587: ED A8 46      
                    LDD   <$7E                    ; 758A: DC 7E         
                    SUBD  <$5E                    ; 758C: 93 5E         
                    ANDA  #$1F                    ; 758E: 84 1F         
                    STD   $48,Y                   ; 7590: ED A8 48      
                    LDD   <$7C                    ; 7593: DC 7C         
                    SUBD  <$5C                    ; 7595: 93 5C         
                    ORA   #$E0                    ; 7597: 8A E0         
                    STD   $4A,Y                   ; 7599: ED A8 4A      
                    LDD   <$3E                    ; 759C: DC 3E         
                    SUBD  <$7E                    ; 759E: 93 7E         
                    ANDA  #$1F                    ; 75A0: 84 1F         
                    STD   $4C,Y                   ; 75A2: ED A8 4C      
                    LDD   <$3C                    ; 75A5: DC 3C         
                    SUBD  <$7C                    ; 75A7: 93 7C         
                    ORA   #$E0                    ; 75A9: 8A E0         
                    STD   $4E,Y                   ; 75AB: ED A8 4E      
                    LDD   #$64FF                  ; 75AE: CC 64 FF        // Insert Vector STAT 4,FF instruction ()
                    STD   $50,Y                   ; 75B1: ED A8 50      
                    LDD   <$2E                    ; 75B4: DC 2E         
                    SUBD  <$3E                    ; 75B6: 93 3E         
                    ANDA  #$1F                    ; 75B8: 84 1F         
                    STD   $52,Y                   ; 75BA: ED A8 52      
                    LDD   <$2C                    ; 75BD: DC 2C         
                    SUBD  <$3C                    ; 75BF: 93 3C         
                    ORA   #$E0                    ; 75C1: 8A E0         
                    STD   $54,Y                   ; 75C3: ED A8 54      
                    LDD   <$0E                    ; 75C6: DC 0E         
                    SUBD  <$2E                    ; 75C8: 93 2E         
                    ANDA  #$1F                    ; 75CA: 84 1F         
                    STD   $56,Y                   ; 75CC: ED A8 56      
                    LDD   <$0C                    ; 75CF: DC 0C         
                    SUBD  <$2C                    ; 75D1: 93 2C         
                    ORA   #$E0                    ; 75D3: 8A E0         
                    STD   $58,Y                   ; 75D5: ED A8 58      
                    LDD   <$1E                    ; 75D8: DC 1E         
                    SUBD  <$0E                    ; 75DA: 93 0E         
                    ANDA  #$1F                    ; 75DC: 84 1F         
                    STD   $5A,Y                   ; 75DE: ED A8 5A      
                    LDD   <$1C                    ; 75E1: DC 1C         
                    SUBD  <$0C                    ; 75E3: 93 0C         
                    ORA   #$E0                    ; 75E5: 8A E0         
                    STD   $5C,Y                   ; 75E7: ED A8 5C      
                    LDD   <$3E                    ; 75EA: DC 3E         
                    SUBD  <$1E                    ; 75EC: 93 1E         
                    ANDA  #$1F                    ; 75EE: 84 1F         
                    STD   $5E,Y                   ; 75F0: ED A8 5E      
                    LDD   <$3C                    ; 75F3: DC 3C         
                    SUBD  <$1C                    ; 75F5: 93 1C         
                    ORA   #$E0                    ; 75F7: 8A E0         
                    STD   $60,Y                   ; 75F9: ED A8 60      
                    LDD   #$7200                  ; 75FC: CC 72 00        // Insert Vector SCAL 2,00 instruction
                    STD   $62,Y                   ; 75FF: ED A8 62      
                    LDD   #$8040                  ; 7602: CC 80 40        // Insert Vector CNTR instruction
                    STD   $64,Y                   ; 7605: ED A8 64      
                    LEAY  $66,Y                   ; 7608: 31 A8 66      
                    LDA   #$48                    ; 760B: 86 48         
                    TFR   A,DP                    ; 760D: 1F 8B         
; End of function sub_7451
; ---------------------------------------------------------------------------
                    RTS                           ; 760F: 39            

; =============== S U B R O U T I N E =======================================
L7D9A               LDD   <$89                    ; 7D9A: DC 89           // Get X 'forward' transform position
                    STD   $5040                   ; 7D9C: FD 50 40        // XT2
                    LDD   $4B24                   ; 7D9F: FC 4B 24        // Get Y 'horizontal' transform
                    STD   $5042                   ; 7DA2: FD 50 42        // YT2
                    LDD   $4B26                   ; 7DA5: FC 4B 26        // Get Z 'vertical' transform
                    STD   $5044                   ; 7DA8: FD 50 44        // ZT2
                    LDX   #$4C00                  ; 7DAB: 8E 4C 00      
                    LDD   #$00                    ; 7DAE: CC 00 00      
L7DB1               STD   ,X++                    ; 7DB1: ED 81         
                    CMPX  #$4C80                  ; 7DB3: 8C 4C 80        // Clear star dots RAM
                    BCS   $7DB1                   ; 7DB6: 25 F9         
                    STA   <$88                    ; 7DB8: 97 88           // Clear DPbyte_88
                    LDA   #$67                    ; 7DBA: 86 67           // Insert Vector STAT 7 for white stars
                    LDB   <$83                    ; 7DBC: D6 83           // Star intensity
                    STD   ,Y++                    ; 7DBE: ED A1           // Y points to vector RAM
                    LDD   #$018C                  ; 7DC0: CC 01 8C        // Point BIC to Math RAM $5C60
                    STD   $4701                   ; 7DC3: FD 47 01      
                    STB   <$82                    ; 7DC6: D7 82           // Store $8C to DPbyte_82
                    NEGB                          ; 7DC8: 50              // Negate to $74
                    ADDB  #$BE                    ; 7DC9: CB BE           // Add $BE to get $32
                    STB   <$81                    ; 7DCB: D7 81           // Store $32 in DPbyte_81 (why not just load B with $32 ?)
                    LDU   #$4C80                  ; 7DCD: CE 4C 80      
                    LDX   #$5C60                  ; 7DD0: 8E 5C 60      
L7DD3               STX   <$84                    ; 7DD3: 9F 84           // DPbyte_84 holds running star MRAM pointer
                    LDA   #$67                    ; 7DD5: 86 67         
                    JSR   $CDBA                   ; 7DD7: BD CD BA      
L7DDA               LDD   $5000                   ; 7DDA: FC 50 00        // Math result X
                    CMPD  #$0100                  ; 7DDD: 10 83 01 00   
                    LBLE  $7E5D                   ; 7DE1: 10 2F 00 78   
                    CMPD  #$0FFF                  ; 7DE5: 10 83 0F FF   
                    LBHI  $7E5D                   ; 7DE9: 10 22 00 70     // Check perspective division value?
                    STD   $4704                   ; 7DED: FD 47 04        // Start division
                    LDD   $5072                   ; 7DF0: FC 50 72        // Reg39 = Reg01 x Reg01
                    SUBD  $5070                   ; 7DF3: B3 50 70        // Reg38 = Reg00 x Reg00
                    LBCC  $7E5D                   ; 7DF6: 10 24 00 63   
                    LDD   $5074                   ; 7DFA: FC 50 74        // Reg3A = Reg02 x Reg02
                    SUBD  $5070                   ; 7DFD: B3 50 70      
                    LBCC  $7E5D                   ; 7E00: 10 24 00 59   
                    LDD   $4700                   ; 7E04: FC 47 00        // Get division result
                    STD   $5000                   ; 7E07: FD 50 00        // Math result X
                    LDA   #$86                    ; 7E0A: 86 86           // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
                    JSR   $CDBA                   ; 7E0C: BD CD BA      
L7E0F               CLRB                          ; 7E0F: 5F            
                    LDA   $5004                   ; 7E10: B6 50 04        // Math result Z
                    LSRA                          ; 7E13: 44            
                    ROLB                          ; 7E14: 59            
                    LSRA                          ; 7E15: 44            
                    ROLB                          ; 7E16: 59            
                    LDA   $5002                   ; 7E17: B6 50 02        // Math result Y
                    LSRA                          ; 7E1A: 44            
                    ROLB                          ; 7E1B: 59            
                    LSRA                          ; 7E1C: 44            
                    ROLB                          ; 7E1D: 59            
                    LDA   #$08                    ; 7E1E: 86 08         
                    MUL                           ; 7E20: 3D            
                    LDX   #$4C00                  ; 7E21: 8E 4C 00      
                    ABX                           ; 7E24: 3A            
                    LDD   $02,X                   ; 7E25: EC 02         
                    BNE   L7E2D                   ; 7E27: 26 04         
                    STU   $02,X                   ; 7E29: EF 02         
                    BRA   $7E2F                   ; 7E2B: 20 02         

; ---------------------------------------------------------------------------
L7E2D               STU   [,X]                    ; 7E2D: EF 94         
L7E2F               STU   ,X                      ; 7E2F: EF 84         
                    LDD   #$00                    ; 7E31: CC 00 00      
                    STD   ,U                      ; 7E34: ED C4         
                    LDD   $5004                   ; 7E36: FC 50 04        // Math result Z
                    SUBD  $06,X                   ; 7E39: A3 06         
                    ANDA  #$1F                    ; 7E3B: 84 1F         
                    STD   $04,U                   ; 7E3D: ED 44         
                    LDD   $5002                   ; 7E3F: FC 50 02        // Math result Y
                    SUBD  $04,X                   ; 7E42: A3 04         
                    ANDA  #$1F                    ; 7E44: 84 1F         
                    CMPU  $02,X                   ; 7E46: 11 A3 02      
                    BEQ   $7E4D                   ; 7E49: 27 02           // These 3 lines surely do nothing as ORA #0 does nothing ?
                    ORA   #$00                    ; 7E4B: 8A 00         
L7E4D               STD   $02,U                   ; 7E4D: ED 42         
                    LDD   $5004                   ; 7E4F: FC 50 04        // Math result Z
                    STD   $06,X                   ; 7E52: ED 06         
                    LDD   $5002                   ; 7E54: FC 50 02        // Math result Y
                    STD   $04,X                   ; 7E57: ED 04         
                    LEAU  $06,U                   ; 7E59: 33 46         
                    BRA   $7E62                   ; 7E5B: 20 05         

; ---------------------------------------------------------------------------
L7E5D               LDX   <$84                    ; 7E5D: 9E 84         
                    JSR   L7F9A                   ; 7E5F: BD 7F 9A      
L7E62               INC   <$82                    ; 7E62: 0C 82         
                    LDX   <$84                    ; 7E64: 9E 84         
                    LEAX  $08,X                   ; 7E66: 30 08         
                    DEC   <$81                    ; 7E68: 0A 81         
                    LBNE  $7DD3                   ; 7E6A: 10 26 FF 65   
                    LDX   #$4C00                  ; 7E6E: 8E 4C 00      
L7E71               LDU   $02,X                   ; 7E71: EE 02         
                    BEQ   $7E95                   ; 7E73: 27 20         
                    LDD   #$1F98                  ; 7E75: CC 1F 98        // Starfield Y offset
                    STD   ,Y++                    ; 7E78: ED A1         
                    LDD   #$00                    ; 7E7A: CC 00 00      
                    STD   ,Y++                    ; 7E7D: ED A1         
L7E7F               LDD   $04,U                   ; 7E7F: EC 44         
                    STD   ,Y++                    ; 7E81: ED A1         
                    LDD   $02,U                   ; 7E83: EC 42         
                    STD   ,Y++                    ; 7E85: ED A1         
                    LDD   $33DC                   ; 7E87: FC 33 DC        // JSRL to Vector ROM subroutine to draw dot
                    STD   ,Y++                    ; 7E8A: ED A1         
                    LDU   ,U                      ; 7E8C: EE C4         
                    BNE   $7E7F                   ; 7E8E: 26 EF         
                    LDD   #$8040                  ; 7E90: CC 80 40        // Insert Vector CNTR
                    STD   ,Y++                    ; 7E93: ED A1         
L7E95               LEAX  $08,X                   ; 7E95: 30 08         
                    CMPX  #$4C80                  ; 7E97: 8C 4C 80      
                    BCS   $7E71                   ; 7E9A: 25 D5         
                    LDD   $5098                   ; 7E9C: FC 50 98      
                    STD   $5040                   ; 7E9F: FD 50 40        // XT2
                    LDD   $509A                   ; 7EA2: FC 50 9A      
                    STD   $5042                   ; 7EA5: FD 50 42        // YT2
                    LDD   $509C                   ; 7EA8: FC 50 9C      
                    STD   $5044                   ; 7EAB: FD 50 44        // ZT2
; End of function sub_7D9A
                    RTS                           ; 7EAE: 39            

; =============== S U B R O U T I N E =======================================
L7EAF               LDX   #$4C00                  ; 7EAF: 8E 4C 00        // Towers surface dots
                    LDD   #$00                    ; 7EB2: CC 00 00      
L7EB5               STD   ,X++                    ; 7EB5: ED 81         
                    CMPX  #$4C80                  ; 7EB7: 8C 4C 80      
                    BCS   $7EB5                   ; 7EBA: 25 F9         
                    STA   <$88                    ; 7EBC: 97 88         
                    LDD   #$6280                  ; 7EBE: CC 62 80        // Insert Vector STAT 2, 80 for green dots
                    STD   ,Y++                    ; 7EC1: ED A1         
                    LDD   #$018C                  ; 7EC3: CC 01 8C        // Point BIC to Math RAM $5C60
                    STD   $4701                   ; 7EC6: FD 47 01      
                    STB   <$82                    ; 7EC9: D7 82         
                    NEGB                          ; 7ECB: 50            
                    ADDB  #$BE                    ; 7ECC: CB BE         
                    STB   <$81                    ; 7ECE: D7 81         
                    LDU   #$4C80                  ; 7ED0: CE 4C 80      
                    LDX   #$5C60                  ; 7ED3: 8E 5C 60      
L7ED6               STX   <$84                    ; 7ED6: 9F 84         
                    LDA   #$67                    ; 7ED8: 86 67         
                    JSR   $CDBA                   ; 7EDA: BD CD BA      
L7EDD               LDD   $5000                   ; 7EDD: FC 50 00        // Math result X
                    CMPD  #$0100                  ; 7EE0: 10 83 01 00   
                    LBLE  L7F58                   ; 7EE4: 10 2F 00 70   
                    STD   $4704                   ; 7EE8: FD 47 04      
                    LDD   $5072                   ; 7EEB: FC 50 72      
                    SUBD  $5070                   ; 7EEE: B3 50 70      
                    LBCC  L7F58                   ; 7EF1: 10 24 00 63   
                    LDD   $5074                   ; 7EF5: FC 50 74      
                    SUBD  $5070                   ; 7EF8: B3 50 70      
                    LBCC  L7F58                   ; 7EFB: 10 24 00 59   
                    LDD   $4700                   ; 7EFF: FC 47 00      
                    STD   $5000                   ; 7F02: FD 50 00        // Math result X
                    LDA   #$86                    ; 7F05: 86 86           // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
                    JSR   $CDBA                   ; 7F07: BD CD BA      
L7F0A               CLRB                          ; 7F0A: 5F            
                    LDA   $5004                   ; 7F0B: B6 50 04        // Math result Z
                    LSRA                          ; 7F0E: 44            
                    ROLB                          ; 7F0F: 59            
                    LSRA                          ; 7F10: 44            
                    ROLB                          ; 7F11: 59            
                    LDA   $5002                   ; 7F12: B6 50 02        // Math result Y
                    LSRA                          ; 7F15: 44            
                    ROLB                          ; 7F16: 59            
                    LSRA                          ; 7F17: 44            
                    ROLB                          ; 7F18: 59            
                    LDA   #$08                    ; 7F19: 86 08         
                    MUL                           ; 7F1B: 3D            
                    LDX   #$4C00                  ; 7F1C: 8E 4C 00      
                    ABX                           ; 7F1F: 3A            
                    LDD   $02,X                   ; 7F20: EC 02         
                    BNE   $7F28                   ; 7F22: 26 04         
                    STU   $02,X                   ; 7F24: EF 02         
                    BRA   $7F2A                   ; 7F26: 20 02         

; ---------------------------------------------------------------------------
L7F28               STU   [,X]                    ; 7F28: EF 94         
L7F2A               STU   ,X                      ; 7F2A: EF 84         
                    LDD   #$00                    ; 7F2C: CC 00 00      
                    STD   ,U                      ; 7F2F: ED C4         
                    LDD   $5004                   ; 7F31: FC 50 04        // Math result Z
                    SUBD  $06,X                   ; 7F34: A3 06         
                    ANDA  #$1F                    ; 7F36: 84 1F         
                    STD   $04,U                   ; 7F38: ED 44         
                    LDD   $5002                   ; 7F3A: FC 50 02        // Math result Y
                    SUBD  $04,X                   ; 7F3D: A3 04         
                    ANDA  #$1F                    ; 7F3F: 84 1F         
                    CMPU  $02,X                   ; 7F41: 11 A3 02      
                    BEQ   $7F48                   ; 7F44: 27 02         
                    ORA   #$00                    ; 7F46: 8A 00         
L7F48               STD   $02,U                   ; 7F48: ED 42         
                    LDD   $5004                   ; 7F4A: FC 50 04        // Math result Z
                    STD   $06,X                   ; 7F4D: ED 06         
                    LDD   $5002                   ; 7F4F: FC 50 02        // Math result Y
                    STD   $04,X                   ; 7F52: ED 04         
                    LEAU  $06,U                   ; 7F54: 33 46         
                    BRA   $7F5D                   ; 7F56: 20 05         

; ---------------------------------------------------------------------------
L7F58               LDX   <$84                    ; 7F58: 9E 84           // Towers surface dots
                    JSR   L7FDB                   ; 7F5A: BD 7F DB      
L7F5D               INC   <$82                    ; 7F5D: 0C 82         
                    LDX   <$84                    ; 7F5F: 9E 84         
                    LEAX  $08,X                   ; 7F61: 30 08         
                    DEC   <$81                    ; 7F63: 0A 81         
                    LBNE  $7ED6                   ; 7F65: 10 26 FF 6D   
                    LDX   #$4C00                  ; 7F69: 8E 4C 00      
L7F6C               LDU   $02,X                   ; 7F6C: EE 02         
                    BEQ   $7F92                   ; 7F6E: 27 22         
                    LDD   #$FF98                  ; 7F70: CC FF 98      
                    ANDA  #$1F                    ; 7F73: 84 1F         
                    STD   ,Y++                    ; 7F75: ED A1         
                    LDD   #$00                    ; 7F77: CC 00 00      
                    STD   ,Y++                    ; 7F7A: ED A1         
L7F7C               LDD   $04,U                   ; 7F7C: EC 44         
                    STD   ,Y++                    ; 7F7E: ED A1         
                    LDD   $02,U                   ; 7F80: EC 42         
                    STD   ,Y++                    ; 7F82: ED A1         
                    LDD   $33DC                   ; 7F84: FC 33 DC        // JSRL to Vector ROM subroutine to draw dot
                    STD   ,Y++                    ; 7F87: ED A1         
                    LDU   ,U                      ; 7F89: EE C4         
                    BNE   $7F7C                   ; 7F8B: 26 EF         
                    LDD   #$8040                  ; 7F8D: CC 80 40        // Insert Vector CNTR
                    STD   ,Y++                    ; 7F90: ED A1         
L7F92               LEAX  $08,X                   ; 7F92: 30 08         
                    CMPX  #$4C80                  ; 7F94: 8C 4C 80      
                    BCS   $7F6C                   ; 7F97: 25 D3         
; End of function sub_7EAF
                    RTS                           ; 7F99: 39            

; =============== S U B R O U T I N E =======================================
L7F9A               JSR   $CE45                   ; 7F9A: BD CE 45      
L7F9D               LDA   <$53                    ; 7F9D: 96 53         
                    ANDA  #$1F                    ; 7F9F: 84 1F         
                    LDB   <$54                    ; 7FA1: D6 54         
                    STD   ,X                      ; 7FA3: ED 84         
                    MUL                           ; 7FA5: 3D            
                    TST   $5002                   ; 7FA6: 7D 50 02        // Math result Y
                    BMI   $7FAC                   ; 7FA9: 2B 01         
                    NEGA                          ; 7FAB: 40            
L7FAC               STD   $02,X                   ; 7FAC: ED 02         
                    LDA   <$53                    ; 7FAE: 96 53         
                    ANDA  #$1F                    ; 7FB0: 84 1F         
                    LDB   <$55                    ; 7FB2: D6 55         
                    MUL                           ; 7FB4: 3D            
                    TST   $5004                   ; 7FB5: 7D 50 04        // Math result Z
                    BMI   $7FBB                   ; 7FB8: 2B 01         
                    NEGA                          ; 7FBA: 40            
L7FBB               STD   $04,X                   ; 7FBB: ED 04         
                    LDA   <$82                    ; 7FBD: 96 82         
                    STA   $4702                   ; 7FBF: B7 47 02      
                    LDA   #$60                    ; 7FC2: 86 60         
                    JSR   $CDBA                   ; 7FC4: BD CD BA      
L7FC7               LDD   $5000                   ; 7FC7: FC 50 00        // Math result X
                    STD   ,X                      ; 7FCA: ED 84         
                    LDD   $5002                   ; 7FCC: FC 50 02        // Math result Y
                    STD   $02,X                   ; 7FCF: ED 02         
                    LDD   $5004                   ; 7FD1: FC 50 04        // Math result Z
                    ANDA  #$FF                    ; 7FD4: 84 FF           // These 2 lines do nothing ?
                    ANDB  #$FF                    ; 7FD6: C4 FF         
                    STD   $04,X                   ; 7FD8: ED 04         
; End of function sub_7F9A
                    RTS                           ; 7FDA: 39            

; =============== S U B R O U T I N E =======================================
L7FDB               JSR   $CE45                   ; 7FDB: BD CE 45        // Random number update
L7FDE               LDA   <$53                    ; 7FDE: 96 53         
                    ANDA  #$7F                    ; 7FE0: 84 7F         
                    ORA   #$70                    ; 7FE2: 8A 70         
                    LDB   <$54                    ; 7FE4: D6 54         
                    ADDD  $5040                   ; 7FE6: F3 50 40        // XT2
                    STD   ,X                      ; 7FE9: ED 84         
                    LDA   <$55                    ; 7FEB: 96 55         
                    ANDA  #$7F                    ; 7FED: 84 7F         
                    TST   $5002                   ; 7FEF: 7D 50 02        // Math result Y
                    BMI   $7FF5                   ; 7FF2: 2B 01         
                    NEGA                          ; 7FF4: 40            
L7FF5               ADDD  $5042                   ; 7FF5: F3 50 42        // YT2
                    STD   $02,X                   ; 7FF8: ED 02         
                    LDD   #$00                    ; 7FFA: CC 00 00      
                    STD   $04,X                   ; 7FFD: ED 04         
; End of function sub_7FDB
; end of 'ROM'
                    RTS                           ; 7FFF: 39            

