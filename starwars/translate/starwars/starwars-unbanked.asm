                    ORG   $8000
L8000               NEG   <$00                    ; 8000: 00 00         
                    FCB   $05                     ; 8002:  ' ' 05             Invalid ; 
; =============== S U B R O U T I N E =======================================
; Entering trench
L8341               LDA   #$00                    ; 8341: 86 00         
                    STA   <$44                    ; 8343: 97 44         
                    STA   <$45                    ; 8345: 97 45         
                    LDX   #$4989                  ; 8347: 8E 49 89      
                    LDA   #$00                    ; 834A: 86 00         
L834C               STA   ,X+                     ; 834C: A7 80         
                    CMPX  #$49A9                  ; 834E: 8C 49 A9      
                    BCS   L834C                   ; 8351: 25 F9         
                    LDB   $4B12                   ; 8353: F6 4B 12      
                    ASLB                          ; 8356: 58            
                    LDX   #$7CC0                  ; 8357: 8E 7C C0        // Trench catwalk/turrets data?
                    ABX                           ; 835A: 3A            
                    CMPX  #$7CD6                  ; 835B: 8C 7C D6        // Trench catwalk/turrets data?
                    BCS   L8365                   ; 835E: 25 05         
                    LDU   #$4B3F                  ; 8360: CE 4B 3F      
                    BRA   L8367                   ; 8363: 20 02         

; ---------------------------------------------------------------------------
L8365               LDU   ,X                      ; 8365: EE 84         
L8367               STU   $49A9                   ; 8367: FF 49 A9      
                    STU   $49AB                   ; 836A: FF 49 AB      
                    LDU   ,U                      ; 836D: EE C4         
                    LDD   #$00                    ; 836F: CC 00 00      
                    STA   $49C0                   ; 8372: B7 49 C0      
                    STD   $49B1                   ; 8375: FD 49 B1      
                    STD   $49B5                   ; 8378: FD 49 B5      
                    STA   <$92                    ; 837B: 97 92         
                    STA   <$95                    ; 837D: 97 95         
                    STU   $49AF                   ; 837F: FF 49 AF      
                    STU   $49B3                   ; 8382: FF 49 B3      
                    JSR   L8434                   ; 8385: BD 84 34      
L8388               JSR   L8408                   ; 8388: BD 84 08        // Trench
L838B               JSR   L83CE                   ; 838B: BD 83 CE      
L838E               JSR   L83CE                   ; 838E: BD 83 CE      
L8391               JSR   L83CE                   ; 8391: BD 83 CE      
L8394               JSR   L83CE                   ; 8394: BD 83 CE      
L8397               JSR   L83CE                   ; 8397: BD 83 CE      
L839A               JSR   L83CE                   ; 839A: BD 83 CE      
L839D               JSR   L83CE                   ; 839D: BD 83 CE      
L83A0               JSR   L83CE                   ; 83A0: BD 83 CE      
; End of function sub_8341
L83A3               RTS                           ; 83A3: 39            

; =============== S U B R O U T I N E =======================================
; Called when starting trench
L83A4               LDU   #$4B3F                  ; 83A4: CE 4B 3F      
                    LDX   #$7C7E                  ; 83A7: 8E 7C 7E        // Copy pointers from ROM to RAM starting at word_4B3F
L83AA               LDD   ,X++                    ; 83AA: EC 81         
                    STD   ,U++                    ; 83AC: ED C1         
                    CMPU  #$4B5F                  ; 83AE: 11 83 4B 5F   
                    BCS   L83AA                   ; 83B2: 25 F6         
                    LDU   #$4B43                  ; 83B4: CE 4B 43      
                    LDX   #$7C9E                  ; 83B7: 8E 7C 9E      
L83BA               LDA   #$11                    ; 83BA: 86 11         
                    LDB   $4703                   ; 83BC: F6 47 03      
                    MUL                           ; 83BF: 3D            
                    ASLA                          ; 83C0: 48            
                    LDD   A,X                     ; 83C1: EC 86         
                    STD   ,U                      ; 83C3: ED C4         
                    LEAU  $04,U                   ; 83C5: 33 44         
                    CMPU  #$4B5F                  ; 83C7: 11 83 4B 5F   
                    BCS   L83BA                   ; 83CB: 25 ED         
; End of function sub_83A4
                    RTS                           ; 83CD: 39            

; =============== S U B R O U T I N E =======================================
L83CE               LDA   <$92                    ; 83CE: 96 92         
                    BNE   locret_8407             ; 83D0: 26 35         
                    LDU   $49B3                   ; 83D2: FE 49 B3      
                    LEAU  $03,U                   ; 83D5: 33 43         
                    LDA   ,U                      ; 83D7: A6 C4         
                    CMPA  #$05                    ; 83D9: 81 05         
                    BNE   L83E2                   ; 83DB: 26 05         
                    LDU   $49AB                   ; 83DD: FE 49 AB      
                    LDU   $02,U                   ; 83E0: EE 42         
L83E2               LDB   ,U                      ; 83E2: E6 C4         
                    CMPB  #$01                    ; 83E4: C1 01         
                    BNE   L83ED                   ; 83E6: 26 05         
                    LDD   #$0800                  ; 83E8: CC 08 00      
                    BRA   L83F9                   ; 83EB: 20 0C         

; ---------------------------------------------------------------------------
L83ED               CMPB  #$03                    ; 83ED: C1 03         
                    BNE   L83F6                   ; 83EF: 26 05         
                    LDD   #$00                    ; 83F1: CC 00 00      
                    BRA   L83F9                   ; 83F4: 20 03         

; ---------------------------------------------------------------------------
L83F6               LDD   #$1000                  ; 83F6: CC 10 00      
L83F9               ADDD  $49B7                   ; 83F9: F3 49 B7      
                    SUBD  $49B1                   ; 83FC: B3 49 B1      
                    SUBD  #$6000                  ; 83FF: 83 60 00      
                    BHI   locret_8407             ; 8402: 22 03         
                    JSR   L8408                   ; 8404: BD 84 08        // Trench
; End of function sub_83CE
locret_8407         RTS                           ; 8407: 39            

; =============== S U B R O U T I N E =======================================
; Trench
L8408               LDU   $49B3                   ; 8408: FE 49 B3      
                    LDB   ,U                      ; 840B: E6 C4         
                    CMPB  #$01                    ; 840D: C1 01         
                    BNE   L8416                   ; 840F: 26 05         
                    LDD   #$0800                  ; 8411: CC 08 00      
                    BRA   L8419                   ; 8414: 20 03         

; ---------------------------------------------------------------------------
L8416               LDD   #$1000                  ; 8416: CC 10 00      
L8419               ADDD  $49B5                   ; 8419: F3 49 B5      
                    STD   $49B5                   ; 841C: FD 49 B5      
                    LEAU  $03,U                   ; 841F: 33 43         
                    LDB   ,U                      ; 8421: E6 C4         
                    CMPB  #$05                    ; 8423: C1 05         
L8425               BNE   L8431                   ; 8425: 26 0A         
                    LDU   $49AB                   ; 8427: FE 49 AB      
                    LEAU  $02,U                   ; 842A: 33 42         
                    STU   $49AB                   ; 842C: FF 49 AB      
                    LDU   ,U                      ; 842F: EE C4         
L8431               STU   $49B3                   ; 8431: FF 49 B3      
L8434               LDU   $49B3                   ; 8434: FE 49 B3      
                    LDB   ,U                      ; 8437: E6 C4         
                    CMPB  #$03                    ; 8439: C1 03         
                    BNE   L8447                   ; 843B: 26 0A         
                    LDD   $49B5                   ; 843D: FC 49 B5      
                    STD   <$93                    ; 8440: DD 93         
                    LDA   #$FF                    ; 8442: 86 FF         
                    STA   <$92                    ; 8444: 97 92         
                    RTS                           ; 8446: 39            

; ---------------------------------------------------------------------------
L8447               LDB   ,U                      ; 8447: E6 C4         
                    CMPB  #$04                    ; 8449: C1 04         
                    BNE   L846A                   ; 844B: 26 1D         
                    LDD   $49B5                   ; 844D: FC 49 B5      
                    STD   <$96                    ; 8450: DD 96         
                    LDA   #$FF                    ; 8452: 86 FF         
                    STA   <$95                    ; 8454: 97 95         
                    LDA   #$00                    ; 8456: 86 00         
                    STA   <$44                    ; 8458: 97 44         
                    STA   <$45                    ; 845A: 97 45         
                    LDA   $4B36                   ; 845C: B6 4B 36      
                    BNE   L846A                   ; 845F: 26 09         
                    INC   $4B36                   ; 8461: 7C 4B 36      
                    JSR   L97E3                   ; 8464: BD 97 E3      
L8467               LDU   $49B3                   ; 8467: FE 49 B3      
L846A               LDB   ,U                      ; 846A: E6 C4         
                    CMPB  #$01                    ; 846C: C1 01         
                    BNE   L8475                   ; 846E: 26 05         
                    LDD   #$0800                  ; 8470: CC 08 00      
                    BRA   L8478                   ; 8473: 20 03         

; ---------------------------------------------------------------------------
L8475               LDD   #$1000                  ; 8475: CC 10 00      
L8478               ADDD  $49B5                   ; 8478: F3 49 B5      
                    STD   $49B7                   ; 847B: FD 49 B7      
                    LDB   $49B5                   ; 847E: F6 49 B5      
                    LSRB                          ; 8481: 54            
                    LSRB                          ; 8482: 54            
                    LSRB                          ; 8483: 54            
                    ANDB  #$0F                    ; 8484: C4 0F         
                    LDX   #$4989                  ; 8486: 8E 49 89      
                    LDA   $01,U                   ; 8489: A6 41         
                    STA   B,X                     ; 848B: A7 85         
                    LDX   #$4999                  ; 848D: 8E 49 99      
                    LDA   $02,U                   ; 8490: A6 42         
                    STA   B,X                     ; 8492: A7 85         
; End of function sub_8408
                    RTS                           ; 8494: 39            

; =============== S U B R O U T I N E =======================================
L8495               LDA   <$45                    ; 8495: 96 45         
                    BNE   locret_84B5             ; 8497: 26 1C         
                    LDB   $4B19                   ; 8499: F6 4B 19      
                    CMPB  #$07                    ; 849C: C1 07         
                    BLS   L84A2                   ; 849E: 23 02         
                    LDB   #$07                    ; 84A0: C6 07         
L84A2               ASLB                          ; 84A2: 58            
                    LDU   #byte_84B6              ; 84A3: CE 84 B6      
                    LEAU  B,U                     ; 84A6: 33 C5         
                    LDA   <$43                    ; 84A8: 96 43           // Game over/insert coins timer
                    ANDA  ,U                      ; 84AA: A4 C4         
                    BNE   locret_84B5             ; 84AC: 26 07         
                    LDA   $01,U                   ; 84AE: A6 41         
                    STA   <$9B                    ; 84B0: 97 9B         
                    JSR   L84C6                   ; 84B2: BD 84 C6      
; End of function sub_8495
; ---------------------------------------------------------------------------
locret_84B5         RTS                           ; 84B5: 39            

; =============== S U B R O U T I N E =======================================
L84C6               LDD   #$F800                  ; 84C6: CC F8 00      
                    ANDA  $5098                   ; 84C9: B4 50 98      
                    ADDA  #$24                    ; 84CC: 8B 24         
                    STD   $5078                   ; 84CE: FD 50 78      
L84D1               LDA   $5078                   ; 84D1: B6 50 78      
                    LSRA                          ; 84D4: 44            
                    LSRA                          ; 84D5: 44            
                    LSRA                          ; 84D6: 44            
                    ANDA  #$0F                    ; 84D7: 84 0F         
                    STA   <$99                    ; 84D9: 97 99         
                    LDD   #$FE00                  ; 84DB: CC FE 00      
                    STD   $507C                   ; 84DE: FD 50 7C      
                    LDD   #$FC80                  ; 84E1: CC FC 80      
                    STD   $507A                   ; 84E4: FD 50 7A      
                    LDX   #$4989                  ; 84E7: 8E 49 89      
                    LDB   <$99                    ; 84EA: D6 99         
                    LDA   B,X                     ; 84EC: A6 85         
L84EE               STA   <$9A                    ; 84EE: 97 9A         
                    ANDA  #$C0                    ; 84F0: 84 C0         
                    CMPA  #$C0                    ; 84F2: 81 C0         
                    BNE   L8522                   ; 84F4: 26 2C         
                    LDD   $509C                   ; 84F6: FC 50 9C      
                    SUBD  $507C                   ; 84F9: B3 50 7C      
                    BLT   L8522                   ; 84FC: 2D 24         
L84FE               SUBD  #$0400                  ; 84FE: 83 04 00      
                    BGE   L850F                   ; 8501: 2C 0C         
L8503               LDA   $4703                   ; 8503: B6 47 03      
                    CMPA  <$9B                    ; 8506: 91 9B         
                    BCS   L850D                   ; 8508: 25 03         
                    JSR   LA7F7                   ; 850A: BD A7 F7      
L850D               BRA   L8522                   ; 850D: 20 13         

; ---------------------------------------------------------------------------
L850F               SUBD  #$0400                  ; 850F: 83 04 00      
                    BGE   L8522                   ; 8512: 2C 0E         
                    LDA   $4703                   ; 8514: B6 47 03      
                    LDB   $4703                   ; 8517: F6 47 03      
                    MUL                           ; 851A: 3D            
                    CMPA  <$9B                    ; 851B: 91 9B         
                    BCS   L8522                   ; 851D: 25 03         
                    JSR   LA7F7                   ; 851F: BD A7 F7      
L8522               LDD   $507C                   ; 8522: FC 50 7C      
                    SUBD  #$0400                  ; 8525: 83 04 00      
                    STD   $507C                   ; 8528: FD 50 7C      
                    LDA   <$9A                    ; 852B: 96 9A         
                    ASLA                          ; 852D: 48            
                    ASLA                          ; 852E: 48            
                    BNE   L84EE                   ; 852F: 26 BD         
                    LDD   #$FE00                  ; 8531: CC FE 00      
                    STD   $507C                   ; 8534: FD 50 7C      
                    LDD   #$0380                  ; 8537: CC 03 80      
                    STD   $507A                   ; 853A: FD 50 7A      
                    LDX   #$4999                  ; 853D: 8E 49 99      
                    LDB   <$99                    ; 8540: D6 99         
                    LDA   B,X                     ; 8542: A6 85         
L8544               STA   <$9A                    ; 8544: 97 9A         
                    ANDA  #$C0                    ; 8546: 84 C0         
                    CMPA  #$C0                    ; 8548: 81 C0         
                    BNE   L8578                   ; 854A: 26 2C         
                    LDD   $509C                   ; 854C: FC 50 9C      
                    SUBD  $507C                   ; 854F: B3 50 7C      
                    BLT   L8578                   ; 8552: 2D 24         
                    SUBD  #$0400                  ; 8554: 83 04 00      
                    BGE   L8565                   ; 8557: 2C 0C         
                    LDA   $4703                   ; 8559: B6 47 03      
                    CMPA  <$9B                    ; 855C: 91 9B         
                    BCS   L8563                   ; 855E: 25 03         
                    JSR   LA80B                   ; 8560: BD A8 0B      
L8563               BRA   L8578                   ; 8563: 20 13         

; ---------------------------------------------------------------------------
L8565               SUBD  #$0400                  ; 8565: 83 04 00      
                    BGE   L8578                   ; 8568: 2C 0E         
                    LDA   $4703                   ; 856A: B6 47 03      
                    LDB   $4703                   ; 856D: F6 47 03      
                    MUL                           ; 8570: 3D            
                    CMPA  <$9B                    ; 8571: 91 9B         
                    BCS   L8578                   ; 8573: 25 03         
                    JSR   LA80B                   ; 8575: BD A8 0B      
L8578               LDD   $507C                   ; 8578: FC 50 7C      
                    SUBD  #$0400                  ; 857B: 83 04 00      
                    STD   $507C                   ; 857E: FD 50 7C      
                    LDA   <$9A                    ; 8581: 96 9A         
                    ASLA                          ; 8583: 48            
                    ASLA                          ; 8584: 48            
                    BNE   L8544                   ; 8585: 26 BD         
                    LDD   $5078                   ; 8587: FC 50 78      
                    ADDD  #$0800                  ; 858A: C3 08 00      
                    STD   $5078                   ; 858D: FD 50 78      
                    SUBD  $5098                   ; 8590: B3 50 98      
                    SUBD  #$6000                  ; 8593: 83 60 00      
                    LBCS  L84D1                   ; 8596: 10 25 FF 37   
; End of function sub_84C6
                    RTS                           ; 859A: 39            

; =============== S U B R O U T I N E =======================================
L859B               LDA   $5040                   ; 859B: B6 50 40        // XT2
                    LSRA                          ; 859E: 44            
                    LSRA                          ; 859F: 44            
                    LSRA                          ; 85A0: 44            
                    ANDA  #$0F                    ; 85A1: 84 0F         
                    CMPA  $49BF                   ; 85A3: B1 49 BF      
                    BEQ   L85DE                   ; 85A6: 27 36         
                    LDB   $49BF                   ; 85A8: F6 49 BF      
                    STA   $49BF                   ; 85AB: B7 49 BF      
                    LDX   #$4989                  ; 85AE: 8E 49 89      
                    LDA   B,X                     ; 85B1: A6 85         
L85B3               ASLA                          ; 85B3: 48            
                    BCC   L85BD                   ; 85B4: 24 07         
                    BMI   L85BD                   ; 85B6: 2B 05         
                    INC   $49C0                   ; 85B8: 7C 49 C0      
L85BB               BRA   L85D2                   ; 85BB: 20 15         

; ---------------------------------------------------------------------------
L85BD               ASLA                          ; 85BD: 48            
                    BNE   L85B3                   ; 85BE: 26 F3         
                    LDX   #$4999                  ; 85C0: 8E 49 99      
                    LDA   B,X                     ; 85C3: A6 85         
L85C5               ASLA                          ; 85C5: 48            
                    BCC   L85CF                   ; 85C6: 24 07         
                    BMI   L85CF                   ; 85C8: 2B 05         
                    INC   $49C0                   ; 85CA: 7C 49 C0      
                    BRA   L85D2                   ; 85CD: 20 03         

; ---------------------------------------------------------------------------
L85CF               ASLA                          ; 85CF: 48            
                    BNE   L85C5                   ; 85D0: 26 F3         
L85D2               LDA   #$00                    ; 85D2: 86 00         
                    LDX   #$4989                  ; 85D4: 8E 49 89      
                    STA   B,X                     ; 85D7: A7 85         
                    LDX   #$4999                  ; 85D9: 8E 49 99      
                    STA   B,X                     ; 85DC: A7 85         
L85DE               JSR   LB3E4                   ; 85DE: BD B3 E4      
L85E1               JSR   L85F9                   ; 85E1: BD 85 F9      
L85E4               JSR   L8735                   ; 85E4: BD 87 35      
L85E7               JSR   L86AE                   ; 85E7: BD 86 AE      
L85EA               LDA   <$95                    ; 85EA: 96 95         
                    BEQ   L85F1                   ; 85EC: 27 03         
                    JSR   L889F                   ; 85EE: BD 88 9F      
L85F1               LDA   <$92                    ; 85F1: 96 92         
                    BEQ   locret_85F8             ; 85F3: 27 03         
                    JSR   L88F5                   ; 85F5: BD 88 F5      
; End of function sub_859B
locret_85F8         RTS                           ; 85F8: 39            

; =============== S U B R O U T I N E =======================================
L85F9               LDD   #$00                    ; 85F9: CC 00 00      
                    STD   $5040                   ; 85FC: FD 50 40        // XT2
                    LDD   #$6270                  ; 85FF: CC 62 70      
                    STD   ,Y++                    ; 8602: ED A1         
                    LDU   #word_8696              ; 8604: CE 86 96      
L8607               LDA   <$92                    ; 8607: 96 92         
                    BEQ   L8618                   ; 8609: 27 0D         
                    LDD   <$93                    ; 860B: DC 93         
                    SUBD  $5098                   ; 860D: B3 50 98      
                    CMPD  #$7000                  ; 8610: 10 83 70 00   
                    BHI   L8618                   ; 8614: 22 02         
                    BRA   L861B                   ; 8616: 20 03         

; ---------------------------------------------------------------------------
L8618               LDD   #$7000                  ; 8618: CC 70 00      
L861B               STD   $5078                   ; 861B: FD 50 78      
                    LDD   ,U                      ; 861E: EC C4         
                    STD   $507A                   ; 8620: FD 50 7A      
L8623               LDD   $02,U                   ; 8623: EC 42         
                    STD   $507C                   ; 8625: FD 50 7C      
                    LDD   #$0F                    ; 8628: CC 00 0F        // Point BIC to $5078 MReg3C
                    STD   $4701                   ; 862B: FD 47 01      
                    LDA   #$67                    ; 862E: 86 67         
                    JSR   LCDBA                   ; 8630: BD CD BA        // Do math program run
L8633               LDD   $5000                   ; 8633: FC 50 00        // Math result X
                    STD   $4704                   ; 8636: FD 47 04      
                    JSR   LCCE4                   ; 8639: BD CC E4      
L863C               LDD   #$0200                  ; 863C: CC 02 00      
                    STD   $5078                   ; 863F: FD 50 78      
                    LDD   $507A                   ; 8642: FC 50 7A      
                    SUBD  $509A                   ; 8645: B3 50 9A      
                    BPL   L864E                   ; 8648: 2A 04         
                    COMA                          ; 864A: 43            
                    NEGB                          ; 864B: 50            
                    SBCA  #$FF                    ; 864C: 82 FF         
L864E               CMPD  $5078                   ; 864E: 10 B3 50 78   
                    BLE   L8657                   ; 8652: 2F 03         
                    STD   $5078                   ; 8654: FD 50 78      
L8657               LDD   $507C                   ; 8657: FC 50 7C      
                    SUBD  $509C                   ; 865A: B3 50 9C      
                    BPL   L8663                   ; 865D: 2A 04         
                    COMA                          ; 865F: 43            
                    NEGB                          ; 8660: 50            
                    SBCA  #$FF                    ; 8661: 82 FF         
L8663               CMPD  $5078                   ; 8663: 10 B3 50 78   
                    BLE   L866C                   ; 8667: 2F 03         
                    STD   $5078                   ; 8669: FD 50 78      
L866C               LDD   #$0F                    ; 866C: CC 00 0F        // Point BIC to $5078 MReg3C
                    STD   $4701                   ; 866F: FD 47 01      
                    LDA   #$67                    ; 8672: 86 67         
                    JSR   LCDBA                   ; 8674: BD CD BA        // Do math program run
L8677               LDD   $5000                   ; 8677: FC 50 00        // Math result X
                    STD   $4704                   ; 867A: FD 47 04      
                    JSR   LCCFC                   ; 867D: BD CC FC        // Trench floor lines calcs
L8680               LDD   #$8040                  ; 8680: CC 80 40      
                    STD   ,Y++                    ; 8683: ED A1         
                    LEAU  $04,U                   ; 8685: 33 44         
                    CMPU  #L86AE                  ; 8687: 11 83 86 AE   
                    LBCS  L8607                   ; 868B: 10 25 FF 78   
                    LDD   $5098                   ; 868F: FC 50 98      
                    STD   $5040                   ; 8692: FD 50 40        // XT2
; End of function sub_85F9
; ---------------------------------------------------------------------------
                    RTS                           ; 8695: 39            

; =============== S U B R O U T I N E =======================================
L86AE               LDD   #$00                    ; 86AE: CC 00 00      
                    STD   $5040                   ; 86B1: FD 50 40        // XT2
                    LDD   #$6250                  ; 86B4: CC 62 50      
                    STD   ,Y++                    ; 86B7: ED A1         
                    LDU   #word_8725              ; 86B9: CE 87 25      
                    LDA   <$92                    ; 86BC: 96 92         
                    BEQ   L86CD                   ; 86BE: 27 0D         
                    LDD   <$93                    ; 86C0: DC 93         
                    SUBD  $5098                   ; 86C2: B3 50 98      
                    CMPD  #$7000                  ; 86C5: 10 83 70 00   
                    BHI   L86CD                   ; 86C9: 22 02         
                    BRA   L86D0                   ; 86CB: 20 03         

; ---------------------------------------------------------------------------
L86CD               LDD   #$7000                  ; 86CD: CC 70 00      
L86D0               STD   $5078                   ; 86D0: FD 50 78      
                    LDD   ,U                      ; 86D3: EC C4         
                    STD   $507A                   ; 86D5: FD 50 7A      
                    LDD   $02,U                   ; 86D8: EC 42         
                    STD   $507C                   ; 86DA: FD 50 7C      
                    LDD   #$0F                    ; 86DD: CC 00 0F      
                    STD   $4701                   ; 86E0: FD 47 01        // Point BIC to $5078 MReg3C
                    LDA   #$67                    ; 86E3: 86 67         
                    JSR   LCDBA                   ; 86E5: BD CD BA        // Do math program run
L86E8               LDD   $5000                   ; 86E8: FC 50 00        // Math result X
                    STD   $4704                   ; 86EB: FD 47 04      
                    JSR   LCCE4                   ; 86EE: BD CC E4      
L86F1               BRA   L8711                   ; 86F1: 20 1E         

; ---------------------------------------------------------------------------
L86F3               LDD   ,U                      ; 86F3: EC C4         
                    STD   $507A                   ; 86F5: FD 50 7A      
                    LDD   $02,U                   ; 86F8: EC 42         
                    STD   $507C                   ; 86FA: FD 50 7C      
                    LDD   #$0F                    ; 86FD: CC 00 0F      
                    STD   $4701                   ; 8700: FD 47 01        // Point BIC to $5078 MReg3C
                    LDA   #$67                    ; 8703: 86 67         
                    JSR   LCDBA                   ; 8705: BD CD BA        // Do math program run
L8708               LDD   $5000                   ; 8708: FC 50 00        // Math result X
                    STD   $4704                   ; 870B: FD 47 04      
                    JSR   LCCFC                   ; 870E: BD CC FC        // Trench floor lines calcs
L8711               LEAU  $04,U                   ; 8711: 33 44         
                    CMPU  #L8735                  ; 8713: 11 83 87 35   
                    BCS   L86F3                   ; 8717: 25 DA         
                    LDD   #$8040                  ; 8719: CC 80 40      
                    STD   ,Y++                    ; 871C: ED A1         
                    LDD   $5098                   ; 871E: FC 50 98      
                    STD   $5040                   ; 8721: FD 50 40        // XT2
; End of function sub_86AE
; ---------------------------------------------------------------------------
                    RTS                           ; 8724: 39            

; =============== S U B R O U T I N E =======================================
L8735               LDU   $49AF                   ; 8735: FE 49 AF      
                    LDB   ,U                      ; 8738: E6 C4         
                    CMPB  #$03                    ; 873A: C1 03         
                    BNE   L873F                   ; 873C: 26 01         
                    RTS                           ; 873E: 39            

; ---------------------------------------------------------------------------
L873F               CMPB  #$01                    ; 873F: C1 01         
                    BNE   L8748                   ; 8741: 26 05         
                    LDD   #$0800                  ; 8743: CC 08 00      
                    BRA   L874B                   ; 8746: 20 03         

; ---------------------------------------------------------------------------
L8748               LDD   #$1000                  ; 8748: CC 10 00      
L874B               ADDD  $49B1                   ; 874B: F3 49 B1      
                    SUBD  $5040                   ; 874E: B3 50 40        // XT2
                    BPL   L8786                   ; 8751: 2A 33         
                    LDB   ,U                      ; 8753: E6 C4         
L8755               CMPB  #$01                    ; 8755: C1 01         
                    BNE   L875E                   ; 8757: 26 05         
                    LDD   #$0800                  ; 8759: CC 08 00      
                    BRA   L8761                   ; 875C: 20 03         

; ---------------------------------------------------------------------------
L875E               LDD   #$1000                  ; 875E: CC 10 00      
L8761               ADDD  $49B1                   ; 8761: F3 49 B1      
                    STD   $49B1                   ; 8764: FD 49 B1      
                    LEAU  $03,U                   ; 8767: 33 43         
                    STU   $49AF                   ; 8769: FF 49 AF      
                    LDB   ,U                      ; 876C: E6 C4         
                    CMPB  #$03                    ; 876E: C1 03         
                    BNE   L8773                   ; 8770: 26 01         
                    RTS                           ; 8772: 39            

; ---------------------------------------------------------------------------
L8773               LDB   ,U                      ; 8773: E6 C4         
                    CMPB  #$05                    ; 8775: C1 05         
                    BNE   L8783                   ; 8777: 26 0A         
                    LDU   $49A9                   ; 8779: FE 49 A9      
                    LEAU  $02,U                   ; 877C: 33 42         
                    STU   $49A9                   ; 877E: FF 49 A9      
                    LDU   ,U                      ; 8781: EE C4         
L8783               STU   $49AF                   ; 8783: FF 49 AF      
L8786               LDD   #$6260                  ; 8786: CC 62 60      
                    STD   ,Y++                    ; 8789: ED A1         
                    JSR   L83CE                   ; 878B: BD 83 CE      
L878E               LDD   $49A9                   ; 878E: FC 49 A9      
                    STD   $49AD                   ; 8791: FD 49 AD      
                    LDD   $49AF                   ; 8794: FC 49 AF      
                    STD   $49B9                   ; 8797: FD 49 B9      
                    LDD   $49B1                   ; 879A: FC 49 B1      
                    STD   $49BB                   ; 879D: FD 49 BB      
                    STD   $5078                   ; 87A0: FD 50 78      
                    LDD   #$FC00                  ; 87A3: CC FC 00      
                    STD   $507A                   ; 87A6: FD 50 7A      
L87A9               JSR   L87CB                   ; 87A9: BD 87 CB      
L87AC               LDD   $49A9                   ; 87AC: FC 49 A9      
                    STD   $49AD                   ; 87AF: FD 49 AD      
                    LDD   $49AF                   ; 87B2: FC 49 AF      
                    STD   $49B9                   ; 87B5: FD 49 B9      
                    LDD   $49B1                   ; 87B8: FC 49 B1      
                    STD   $49BB                   ; 87BB: FD 49 BB      
                    STD   $5078                   ; 87BE: FD 50 78      
                    LDD   #$0400                  ; 87C1: CC 04 00      
                    STD   $507A                   ; 87C4: FD 50 7A      
                    JSR   L87CB                   ; 87C7: BD 87 CB      
; End of function sub_8735
L87CA               RTS                           ; 87CA: 39            

; =============== S U B R O U T I N E =======================================
L87CB               LDD   $49BB                   ; 87CB: FC 49 BB      
                    STD   $5078                   ; 87CE: FD 50 78      
                    SUBD  $5040                   ; 87D1: B3 50 40        // XT2
                    LBMI  L8866                   ; 87D4: 10 2B 00 8E   
                    CMPD  #$0800                  ; 87D8: 10 83 08 00   
                    BGE   L87F5                   ; 87DC: 2C 17         
                    ASLB                          ; 87DE: 58            
                    ROLA                          ; 87DF: 49            
                    COMA                          ; 87E0: 43            
                    NEGB                          ; 87E1: 50            
                    SBCA  #$FF                    ; 87E2: 82 FF         
                    ADDD  $5044                   ; 87E4: F3 50 44        // ZT2
                    CMPD  #$F000                  ; 87E7: 10 83 F0 00   
                    BGE   L87F0                   ; 87EB: 2C 03         
                    LDD   #$F000                  ; 87ED: CC F0 00      
L87F0               STD   $507C                   ; 87F0: FD 50 7C      
                    BRA   L8801                   ; 87F3: 20 0C         

; ---------------------------------------------------------------------------
L87F5               LDD   $49BB                   ; 87F5: FC 49 BB      
                    STD   $5078                   ; 87F8: FD 50 78      
                    LDD   #$F000                  ; 87FB: CC F0 00      
                    STD   $507C                   ; 87FE: FD 50 7C      
L8801               LDA   <$92                    ; 8801: 96 92         
                    BEQ   L880E                   ; 8803: 27 09         
                    LDD   $5078                   ; 8805: FC 50 78      
                    SUBD  <$93                    ; 8808: 93 93         
                    LBPL  locret_889E             ; 880A: 10 2A 00 90   
L880E               LDD   $5078                   ; 880E: FC 50 78      
                    SUBD  $5098                   ; 8811: B3 50 98      
                    SUBD  #$7000                  ; 8814: 83 70 00      
                    LBPL  locret_889E             ; 8817: 10 2A 00 83   
                    LDD   #$0F                    ; 881B: CC 00 0F      
                    STD   $4701                   ; 881E: FD 47 01        // Point BIC to $5078 MReg3C
                    LDA   #$67                    ; 8821: 86 67         
                    JSR   LCDBA                   ; 8823: BD CD BA        // Do math program run
L8826               LDD   $5002                   ; 8826: FC 50 02        // Math result Y
                    BPL   L882F                   ; 8829: 2A 04         
                    COMA                          ; 882B: 43            
                    NEGB                          ; 882C: 50            
                    SBCA  #$FF                    ; 882D: 82 FF         
L882F               SUBD  $5000                   ; 882F: B3 50 00        // Math result X
                    BGT   L8866                   ; 8832: 2E 32         
                    LDD   $5000                   ; 8834: FC 50 00        // Math result X
                    STD   $4704                   ; 8837: FD 47 04      
                    LDD   #$00                    ; 883A: CC 00 00      
                    STD   <$D6                    ; 883D: DD D6         
                    LDD   #$68                    ; 883F: CC 00 68      
                    STD   <$D8                    ; 8842: DD D8         
                    JSR   LCD08                   ; 8844: BD CD 08        // Trench side vertical lines calcs
L8847               LDD   #$00                    ; 8847: CC 00 00      
                    STD   $507C                   ; 884A: FD 50 7C      
                    LDD   #$0F                    ; 884D: CC 00 0F      
                    STD   $4701                   ; 8850: FD 47 01        // Point BIC to $5078 MReg3C
                    LDA   #$67                    ; 8853: 86 67         
                    JSR   LCDBA                   ; 8855: BD CD BA        // Do math program run
L8858               LDD   $5000                   ; 8858: FC 50 00        // Math result X
                    STD   $4704                   ; 885B: FD 47 04      
                    JSR   LCCFC                   ; 885E: BD CC FC        // Trench floor lines calcs
L8861               LDD   #$8040                  ; 8861: CC 80 40      
                    STD   ,Y++                    ; 8864: ED A1         
L8866               LDU   $49B9                   ; 8866: FE 49 B9      
                    LDB   ,U                      ; 8869: E6 C4         
                    CMPB  #$01                    ; 886B: C1 01         
                    BNE   L8874                   ; 886D: 26 05         
                    LDD   #$0800                  ; 886F: CC 08 00      
                    BRA   L8877                   ; 8872: 20 03         

; ---------------------------------------------------------------------------
L8874               LDD   #$1000                  ; 8874: CC 10 00      
L8877               ADDD  $49BB                   ; 8877: F3 49 BB      
                    STD   $49BB                   ; 887A: FD 49 BB      
                    SUBD  $5040                   ; 887D: B3 50 40        // XT2
                    BMI   locret_889E             ; 8880: 2B 1C         
                    LEAU  $03,U                   ; 8882: 33 43         
                    LDB   ,U                      ; 8884: E6 C4         
                    CMPB  #$03                    ; 8886: C1 03         
L8888               BEQ   locret_889E             ; 8888: 27 14         
                    CMPB  #$05                    ; 888A: C1 05         
                    BNE   L8898                   ; 888C: 26 0A         
                    LDU   $49AD                   ; 888E: FE 49 AD      
                    LEAU  $02,U                   ; 8891: 33 42         
                    STU   $49AD                   ; 8893: FF 49 AD      
                    LDU   ,U                      ; 8896: EE C4         
L8898               STU   $49B9                   ; 8898: FF 49 B9      
                    JMP   L87F5                   ; 889B: 7E 87 F5      

; ---------------------------------------------------------------------------
; End of function sub_87CB
locret_889E         RTS                           ; 889E: 39            

; =============== S U B R O U T I N E =======================================
L889F               LDB   #$10                    ; 889F: C6 10         
                    STB   <$DC                    ; 88A1: D7 DC         
                    JSR   LCD38                   ; 88A3: BD CD 38        // Trench left side turret calcs
L88A6               LDD   #$F000                  ; 88A6: CC F0 00      
                    STD   $5E04                   ; 88A9: FD 5E 04      
                    LDD   #$00                    ; 88AC: CC 00 00      
                    STD   $5E02                   ; 88AF: FD 5E 02      
                    LDD   <$96                    ; 88B2: DC 96         
                    STD   $5E00                   ; 88B4: FD 5E 00      
                    LDD   #$01C0                  ; 88B7: CC 01 C0        // Point BIC to math $5E00
                    STD   $4701                   ; 88BA: FD 47 01      
                    LDA   #$67                    ; 88BD: 86 67         
                    JSR   LCDBA                   ; 88BF: BD CD BA        // Do math program run
L88C2               LDD   $5000                   ; 88C2: FC 50 00        // Math result X
                    CMPD  #$FE00                  ; 88C5: 10 83 FE 00   
                    BGT   L88D0                   ; 88C9: 2E 05         
                    LDA   #$00                    ; 88CB: 86 00         
                    STA   <$95                    ; 88CD: 97 95         
                    RTS                           ; 88CF: 39            

; ---------------------------------------------------------------------------
L88D0               SUBD  #$7000                  ; 88D0: 83 70 00      
                    BGT   locret_88F4             ; 88D3: 2E 1F         
                    LDD   #$A018                  ; 88D5: CC A0 18      
                    STD   $5E00                   ; 88D8: FD 5E 00      
                    LDD   #$7200                  ; 88DB: CC 72 00      
                    STD   $5E04                   ; 88DE: FD 5E 04      
                    LDD   $5000                   ; 88E1: FC 50 00        // Math result X
                    SUBD  #$1000                  ; 88E4: 83 10 00      
                    BGE   L88EE                   ; 88E7: 2C 05         
                    JSR   LCD5C                   ; 88E9: BD CD 5C        // Trench calcs
L88EC               BRA   L88F1                   ; 88EC: 20 03         

; ---------------------------------------------------------------------------
L88EE               JSR   LCD50                   ; 88EE: BD CD 50      
L88F1               JSR   LCD74                   ; 88F1: BD CD 74        // Function select for an object
; End of function sub_889F
locret_88F4         RTS                           ; 88F4: 39            

; =============== S U B R O U T I N E =======================================
L88F5               LDD   #$6280                  ; 88F5: CC 62 80      
                    STD   ,Y++                    ; 88F8: ED A1         
                    LDA   <$92                    ; 88FA: 96 92         
                    BEQ   locret_8950             ; 88FC: 27 52         
                    LDD   <$93                    ; 88FE: DC 93         
                    STD   $5078                   ; 8900: FD 50 78      
                    SUBD  $5098                   ; 8903: B3 50 98      
                    SUBD  #$7000                  ; 8906: 83 70 00      
                    BMI   L8911                   ; 8909: 2B 06         
                    COMA                          ; 890B: 43            
                    NEGB                          ; 890C: 50            
                    SBCA  #$FF                    ; 890D: 82 FF         
                    BRA   L8914                   ; 890F: 20 03         

; ---------------------------------------------------------------------------
L8911               LDD   #$00                    ; 8911: CC 00 00      
L8914               STD   $507C                   ; 8914: FD 50 7C      
                    LDD   #$FC00                  ; 8917: CC FC 00      
                    STD   $507A                   ; 891A: FD 50 7A      
                    LDD   #$0F                    ; 891D: CC 00 0F      
                    STD   $4701                   ; 8920: FD 47 01        // Point BIC to $5078 MReg3C
                    LDA   #$67                    ; 8923: 86 67         
                    JSR   LCDBA                   ; 8925: BD CD BA        // Do math program run
L8928               LDD   $5000                   ; 8928: FC 50 00        // Math result X
                    STD   $4704                   ; 892B: FD 47 04      
                    JSR   LCCE4                   ; 892E: BD CC E4      
L8931               LDD   #$0400                  ; 8931: CC 04 00      
                    STD   $507A                   ; 8934: FD 50 7A      
                    LDD   #$0F                    ; 8937: CC 00 0F      
                    STD   $4701                   ; 893A: FD 47 01        // Point BIC to $5078 MReg3C
                    LDA   #$67                    ; 893D: 86 67         
                    JSR   LCDBA                   ; 893F: BD CD BA        // Do math program run
L8942               LDD   $5000                   ; 8942: FC 50 00        // Math result X
                    STD   $4704                   ; 8945: FD 47 04      
                    JSR   LCCFC                   ; 8948: BD CC FC        // Trench floor lines calcs
L894B               LDD   #$8040                  ; 894B: CC 80 40      
                    STD   ,Y++                    ; 894E: ED A1         
; End of function sub_88F5
locret_8950         RTS                           ; 8950: 39            

; =============== S U B R O U T I N E =======================================
; Attributes: noreturn
L8951               JSR   L8981                   ; 8951: BD 89 81      
L8954               INC   <$4D                    ; 8954: 0C 4D         
; End of function sub_8951
                    JMP   L89D3                   ; 8956: 7E 89 D3        // Space wave pitch

; =============== S U B R O U T I N E =======================================
; Attributes: noreturn
L8959               JSR   L8993                   ; 8959: BD 89 93      
L895C               DEC   <$4D                    ; 895C: 0A 4D         
; End of function sub_8959
                    JMP   L89D3                   ; 895E: 7E 89 D3        // Space wave pitch

; =============== S U B R O U T I N E =======================================
; Attributes: noreturn
L8961               JSR   L8993                   ; 8961: BD 89 93      
L8964               DEC   <$4E                    ; 8964: 0A 4E         
; End of function sub_8961
                    JMP   L89C8                   ; 8966: 7E 89 C8        // Space wave roll

; =============== S U B R O U T I N E =======================================
; Attributes: noreturn
L8969               JSR   L8981                   ; 8969: BD 89 81      
L896C               INC   <$4E                    ; 896C: 0C 4E         
; End of function sub_8969
                    JMP   L89C8                   ; 896E: 7E 89 C8        // Space wave roll

; =============== S U B R O U T I N E =======================================
; Attributes: noreturn
L8971               JSR   L8981                   ; 8971: BD 89 81      
L8974               DEC   <$4F                    ; 8974: 0A 4F         
; End of function sub_8971
                    JMP   L89DE                   ; 8976: 7E 89 DE        // Space wave yaw

; =============== S U B R O U T I N E =======================================
; Attributes: noreturn
L8979               JSR   L8993                   ; 8979: BD 89 93      
L897C               INC   <$4F                    ; 897C: 0C 4F         
; End of function sub_8979
                    JMP   L89DE                   ; 897E: 7E 89 DE        // Space wave yaw

; =============== S U B R O U T I N E =======================================
L8981               LDA   #$14                    ; 8981: 86 14         
                    LDU   #word_89A8              ; 8983: CE 89 A8      
                    LEAU  A,U                     ; 8986: 33 C6         
                    LDD   ,U                      ; 8988: EC C4         
                    STD   $5022                   ; 898A: FD 50 22        // Sine for rotation
                    LDD   $02,U                   ; 898D: EC 42         
                    STD   $5024                   ; 898F: FD 50 24        // Cosine for rotation
; End of function sub_8981
                    RTS                           ; 8992: 39            

; =============== S U B R O U T I N E =======================================
L8993               LDA   #$14                    ; 8993: 86 14         
                    LDU   #word_89A8              ; 8995: CE 89 A8      
                    LEAU  A,U                     ; 8998: 33 C6         
                    LDD   #$00                    ; 899A: CC 00 00      
                    SUBD  ,U                      ; 899D: A3 C4         
                    STD   $5022                   ; 899F: FD 50 22        // Sine for rotation
                    LDD   $02,U                   ; 89A2: EC 42         
                    STD   $5024                   ; 89A4: FD 50 24        // Cosine for rotation
; End of function sub_8993
; ---------------------------------------------------------------------------
                    RTS                           ; 89A7: 39            

; =============== S U B R O U T I N E =======================================
; Space wave roll
; Attributes: noreturn
L89C8               CLRA                          ; 89C8: 4F            
                    LDB   $02,X                   ; 89C9: E6 02         
                    STD   $4701                   ; 89CB: FD 47 01      
                    LDA   #$00                    ; 89CE: 86 00           // Roll
; End of function sub_89C8
                    JMP   LCDBA                   ; 89D0: 7E CD BA        // Do math program run

; =============== S U B R O U T I N E =======================================
; Space wave pitch
L89D3               CLRA                          ; 89D3: 4F            
                    LDB   $02,X                   ; 89D4: E6 02         
                    STD   $4701                   ; 89D6: FD 47 01      
                    LDA   #$0E                    ; 89D9: 86 0E           // Pitch
; End of function sub_89D3
                    JMP   LCDBA                   ; 89DB: 7E CD BA        // Do math program run

; =============== S U B R O U T I N E =======================================
; Space wave yaw
L89DE               CLRA                          ; 89DE: 4F            
                    LDB   $02,X                   ; 89DF: E6 02         
                    STD   $4701                   ; 89E1: FD 47 01      
                    LDA   #$1C                    ; 89E4: 86 1C           // Yaw
; End of function sub_89DE
                    JMP   LCDBA                   ; 89E6: 7E CD BA        // Do math program run

; =============== S U B R O U T I N E =======================================
L89E9               LDD   -16,U                   ; 89E9: EC 50         
                    JSR   Shift_D_R_5             ; 89EB: BD CD 9E        // Shift D register right
L89EE               ADDD  -10,U                   ; 89EE: E3 56         
                    STD   -10,U                   ; 89F0: ED 56         
                    LDD   -8,U                    ; 89F2: EC 58         
                    JSR   Shift_D_R_5             ; 89F4: BD CD 9E        // Shift D register right
L89F7               ADDD  -2,U                    ; 89F7: E3 5E         
                    STD   -2,U                    ; 89F9: ED 5E         
                    LDD   ,U                      ; 89FB: EC C4         
                    JSR   Shift_D_R_5             ; 89FD: BD CD 9E        // Shift D register right
L8A00               ADDD  $06,U                   ; 8A00: E3 46         
                    STD   $06,U                   ; 8A02: ED 46         
; End of function sub_89E9
                    RTS                           ; 8A04: 39            

; =============== S U B R O U T I N E =======================================
L8A05               LDD   -16,U                   ; 8A05: EC 50         
                    JSR   Shift_D_R_6             ; 8A07: BD CD 9C        // Shift D register right
L8A0A               ADDD  -10,U                   ; 8A0A: E3 56         
                    STD   -10,U                   ; 8A0C: ED 56         
                    LDD   -8,U                    ; 8A0E: EC 58         
                    JSR   Shift_D_R_6             ; 8A10: BD CD 9C        // Shift D register right
L8A13               ADDD  -2,U                    ; 8A13: E3 5E         
                    STD   -2,U                    ; 8A15: ED 5E         
                    LDD   ,U                      ; 8A17: EC C4         
                    JSR   Shift_D_R_6             ; 8A19: BD CD 9C        // Shift D register right
L8A1C               ADDD  $06,U                   ; 8A1C: E3 46         
                    STD   $06,U                   ; 8A1E: ED 46         
; End of function sub_8A05
                    RTS                           ; 8A20: 39            

; =============== S U B R O U T I N E =======================================
L8A21               LDD   -12,U                   ; 8A21: EC 54         
                    JSR   Shift_D_R_6             ; 8A23: BD CD 9C        // Shift D register right
L8A26               ADDD  -10,U                   ; 8A26: E3 56         
                    STD   -10,U                   ; 8A28: ED 56         
                    LDD   -4,U                    ; 8A2A: EC 5C         
                    JSR   Shift_D_R_6             ; 8A2C: BD CD 9C        // Shift D register right
L8A2F               ADDD  -2,U                    ; 8A2F: E3 5E         
                    STD   -2,U                    ; 8A31: ED 5E         
                    LDD   $04,U                   ; 8A33: EC 44         
                    JSR   Shift_D_R_6             ; 8A35: BD CD 9C        // Shift D register right
L8A38               ADDD  $06,U                   ; 8A38: E3 46         
                    STD   $06,U                   ; 8A3A: ED 46         
; End of function sub_8A21
                    RTS                           ; 8A3C: 39            

; =============== S U B R O U T I N E =======================================
L8A3D               LDD   -12,U                   ; 8A3D: EC 54         
                    JSR   Shift_D_R_5             ; 8A3F: BD CD 9E        // Shift D register right
L8A42               ADDD  -10,U                   ; 8A42: E3 56         
                    STD   -10,U                   ; 8A44: ED 56         
                    LDD   -4,U                    ; 8A46: EC 5C         
                    JSR   Shift_D_R_5             ; 8A48: BD CD 9E        // Shift D register right
L8A4B               ADDD  -2,U                    ; 8A4B: E3 5E         
                    STD   -2,U                    ; 8A4D: ED 5E         
                    LDD   $04,U                   ; 8A4F: EC 44         
                    JSR   Shift_D_R_5             ; 8A51: BD CD 9E        // Shift D register right
L8A54               ADDD  $06,U                   ; 8A54: E3 46         
                    STD   $06,U                   ; 8A56: ED 46         
; End of function sub_8A3D
                    RTS                           ; 8A58: 39            

; =============== S U B R O U T I N E =======================================
L8A59               LDD   #$00                    ; 8A59: CC 00 00      
                    SUBD  -12,U                   ; 8A5C: A3 54         
                    JSR   Shift_D_R_6             ; 8A5E: BD CD 9C        // Shift D register right
L8A61               ADDD  -10,U                   ; 8A61: E3 56         
                    STD   -10,U                   ; 8A63: ED 56         
                    LDD   #$00                    ; 8A65: CC 00 00      
                    SUBD  -4,U                    ; 8A68: A3 5C         
                    JSR   Shift_D_R_6             ; 8A6A: BD CD 9C        // Shift D register right
L8A6D               ADDD  -2,U                    ; 8A6D: E3 5E         
                    STD   -2,U                    ; 8A6F: ED 5E         
                    LDD   #$00                    ; 8A71: CC 00 00      
                    SUBD  $04,U                   ; 8A74: A3 44         
                    JSR   Shift_D_R_6             ; 8A76: BD CD 9C        // Shift D register right
L8A79               ADDD  $06,U                   ; 8A79: E3 46         
                    STD   $06,U                   ; 8A7B: ED 46         
; End of function sub_8A59
                    RTS                           ; 8A7D: 39            

; =============== S U B R O U T I N E =======================================
L8A7E               LDD   #$00                    ; 8A7E: CC 00 00      
                    SUBD  -12,U                   ; 8A81: A3 54         
                    JSR   Shift_D_R_5             ; 8A83: BD CD 9E        // Shift D register right
L8A86               ADDD  -10,U                   ; 8A86: E3 56         
                    STD   -10,U                   ; 8A88: ED 56         
                    LDD   #$00                    ; 8A8A: CC 00 00      
                    SUBD  -4,U                    ; 8A8D: A3 5C         
                    JSR   Shift_D_R_5             ; 8A8F: BD CD 9E        // Shift D register right
L8A92               ADDD  -2,U                    ; 8A92: E3 5E         
                    STD   -2,U                    ; 8A94: ED 5E         
                    LDD   #$00                    ; 8A96: CC 00 00      
                    SUBD  $04,U                   ; 8A99: A3 44         
                    JSR   Shift_D_R_5             ; 8A9B: BD CD 9E        // Shift D register right
L8A9E               ADDD  $06,U                   ; 8A9E: E3 46         
                    STD   $06,U                   ; 8AA0: ED 46         
; End of function sub_8A7E
; ---------------------------------------------------------------------------
                    RTS                           ; 8AA2: 39            

; =============== S U B R O U T I N E =======================================
L8AB6               LDD   -10,U                   ; 8AB6: EC 56         
                    ADDD  $08,U                   ; 8AB8: E3 48         
                    BVS   L8ABE                   ; 8ABA: 29 02         
                    STD   $08,U                   ; 8ABC: ED 48         
L8ABE               LDD   -2,U                    ; 8ABE: EC 5E         
                    ADDD  $0A,U                   ; 8AC0: E3 4A         
                    BVS   L8AC6                   ; 8AC2: 29 02         
                    STD   $0A,U                   ; 8AC4: ED 4A         
L8AC6               LDD   $0C,U                   ; 8AC6: EC 4C         
                    ADDD  $06,U                   ; 8AC8: E3 46         
                    BVS   locret_8ACE             ; 8ACA: 29 02         
                    STD   $0C,U                   ; 8ACC: ED 4C         
; End of function sub_8AB6
locret_8ACE         RTS                           ; 8ACE: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_B32B
L8ACF               LDX   <$C2                    ; 8ACF: 9E C2         
                    LDA   $03,X                   ; 8AD1: A6 03         
                    CMPA  #$01                    ; 8AD3: 81 01         
                    BNE   locret_8ADB             ; 8AD5: 26 04         
                    LDA   $06,X                   ; 8AD7: A6 06         
                    BEQ   L8ADC                   ; 8AD9: 27 01         
locret_8ADB         RTS                           ; 8ADB: 39            

; ---------------------------------------------------------------------------
L8ADC               JSR   Gen_Random              ; 8ADC: BD CE 45        // Update random numbers
L8ADF               LDA   $03,X                   ; 8ADF: A6 03         
                    CMPA  #$01                    ; 8AE1: 81 01         
                    BNE   L8AED                   ; 8AE3: 26 08         
                    LDD   $15,X                   ; 8AE5: EC 88 15      
                    ORB   #$01                    ; 8AE8: CA 01         
                    STD   $15,X                   ; 8AEA: ED 88 15      
L8AED               LDX   <$C2                    ; 8AED: 9E C2         
                    INC   $08,X                   ; 8AEF: 6C 08         
                    DEC   $07,X                   ; 8AF1: 6A 07         
                    LBLE  L8B57                   ; 8AF3: 10 2F 00 60   
                    LDA   #$05                    ; 8AF7: 86 05         
                    STA   $07,X                   ; 8AF9: A7 07         
                    JSR   L97ED                   ; 8AFB: BD 97 ED        // Vaders tie score
L8AFE               LDA   #$1F                    ; 8AFE: 86 1F         
                    STA   $09,X                   ; 8B00: A7 09         
                    STA   $06,X                   ; 8B02: A7 06         
                    LDX   #$5090                  ; 8B04: 8E 50 90      
                    LDU   <$C2                    ; 8B07: DE C2         
                    LDU   ,U                      ; 8B09: EE C4         
                    LDD   #$00                    ; 8B0B: CC 00 00      
                    STD   -10,U                   ; 8B0E: ED 56         
                    STD   -2,U                    ; 8B10: ED 5E         
                    STD   $06,U                   ; 8B12: ED 46         
                    LDA   #$02                    ; 8B14: 86 02         
                    STA   <$01                    ; 8B16: 97 01         
L8B18               LDD   #$4000                  ; 8B18: CC 40 00      
                    SUBD  <$C4                    ; 8B1B: 93 C4         
                    LDB   -16,X                   ; 8B1D: E6 10         
                    MUL                           ; 8B1F: 3D            
                    TFR   A,B                     ; 8B20: 1F 89         
                    ASLB                          ; 8B22: 58            
                    ROLA                          ; 8B23: 49            
                    SEX                           ; 8B24: 1D            
                    ADDD  -10,U                   ; 8B25: E3 56         
                    STD   -10,U                   ; 8B27: ED 56         
                    LDA   <$53                    ; 8B29: 96 53         
                    ORA   #$80                    ; 8B2B: 8A 80         
                    LDB   -8,X                    ; 8B2D: E6 18         
                    MUL                           ; 8B2F: 3D            
                    BCS   L8B33                   ; 8B30: 25 01         
                    NEGA                          ; 8B32: 40            
L8B33               TFR   A,B                     ; 8B33: 1F 89         
                    NOP                           ; 8B35: 12            
                    SEX                           ; 8B36: 1D            
                    ADDD  -2,U                    ; 8B37: E3 5E         
                    STD   -2,U                    ; 8B39: ED 5E         
                    LDA   <$54                    ; 8B3B: 96 54         
                    ORA   #$80                    ; 8B3D: 8A 80         
                    LDB   ,X                      ; 8B3F: E6 84         
                    MUL                           ; 8B41: 3D            
                    BCS   L8B45                   ; 8B42: 25 01         
                    NEGA                          ; 8B44: 40            
L8B45               TFR   A,B                     ; 8B45: 1F 89         
L8B47               NOP                           ; 8B47: 12            
                    SEX                           ; 8B48: 1D            
                    ADDD  $06,U                   ; 8B49: E3 46         
                    STD   $06,U                   ; 8B4B: ED 46         
                    LEAX  $02,X                   ; 8B4D: 30 02         
                    DEC   <$01                    ; 8B4F: 0A 01         
                    BPL   L8B18                   ; 8B51: 2A C5         
                    JSR   Sound_35                ; 8B53: BD BD FD      
L8B56               RTS                           ; 8B56: 39            

; ---------------------------------------------------------------------------
L8B57               LDX   <$C2                    ; 8B57: 9E C2         
                    JSR   Sound_35                ; 8B59: BD BD FD      
L8B5C               LDA   $02,X                   ; 8B5C: A6 02         
                    CMPA  $4B38                   ; 8B5E: B1 4B 38      
                    BNE   L8B66                   ; 8B61: 26 03         
                    JSR   Sound_2B                ; 8B63: BD BD CB      
L8B66               JSR   LB739                   ; 8B66: BD B7 39      
L8B69               JSR   L97E8                   ; 8B69: BD 97 E8        // Tie fighter score
; END OF FUNCTION CHUNK FOR sub_B32B
L8B6C               RTS                           ; 8B6C: 39            

; =============== S U B R O U T I N E =======================================
L8B6D               LDX   #$4900                  ; 8B6D: 8E 49 00        // 3x Tie fighter data structure ($19 bytes per Tie)
L8B70               STX   <$5A                    ; 8B70: 9F 5A         
                    LDU   ,X                      ; 8B72: EE 84         
                    LDA   $03,X                   ; 8B74: A6 03         
                    BEQ   L8B7B                   ; 8B76: 27 03         
                    JSR   L8BE1                   ; 8B78: BD 8B E1      
L8B7B               LDX   <$5A                    ; 8B7B: 9E 5A         
                    LEAX  $19,X                   ; 8B7D: 30 88 19      
                    CMPX  #$494B                  ; 8B80: 8C 49 4B        // 3x Tie fighter data structure ($19 bytes per Tie)
                    BCS   L8B70                   ; 8B83: 25 EB         
; End of function sub_8B6D
                    RTS                           ; 8B85: 39            

; =============== S U B R O U T I N E =======================================
L8B86               LDX   #$4900                  ; 8B86: 8E 49 00        // 3x Tie fighter data structure ($19 bytes per Tie)
L8B89               STX   <$5A                    ; 8B89: 9F 5A         
                    LDU   ,X                      ; 8B8B: EE 84         
                    LDA   $03,X                   ; 8B8D: A6 03         
                    CMPA  #$01                    ; 8B8F: 81 01         
                    BNE   L8BD6                   ; 8B91: 26 43         
                    LDA   $0A,U                   ; 8B93: A6 4A         
                    CMPA  #$09                    ; 8B95: 81 09         
                    BLT   L8B9B                   ; 8B97: 2D 02         
                    SUBA  #$02                    ; 8B99: 80 02         
L8B9B               CMPA  #$F7                    ; 8B9B: 81 F7         
                    BGT   L8BA1                   ; 8B9D: 2E 02         
                    ADDA  #$02                    ; 8B9F: 8B 02         
L8BA1               STA   $0A,U                   ; 8BA1: A7 4A         
                    LDA   $0C,U                   ; 8BA3: A6 4C         
                    CMPA  #$09                    ; 8BA5: 81 09         
                    BLT   L8BAB                   ; 8BA7: 2D 02         
                    SUBA  #$03                    ; 8BA9: 80 03         
L8BAB               CMPA  #$F7                    ; 8BAB: 81 F7         
                    BGT   L8BB1                   ; 8BAD: 2E 02         
                    ADDA  #$03                    ; 8BAF: 8B 03         
L8BB1               STA   $0C,U                   ; 8BB1: A7 4C         
                    LDD   $08,U                   ; 8BB3: EC 48         
                    ADDD  #$0400                  ; 8BB5: C3 04 00      
                    BVS   L8BBE                   ; 8BB8: 29 04         
                    STD   $08,U                   ; 8BBA: ED 48         
                    BRA   L8BD6                   ; 8BBC: 20 18         

; ---------------------------------------------------------------------------
L8BBE               LDA   $0A,U                   ; 8BBE: A6 4A         
                    TSTA                          ; 8BC0: 4D            
                    BPL   L8BC4                   ; 8BC1: 2A 01         
                    NEGA                          ; 8BC3: 40            
L8BC4               CMPA  #$08                    ; 8BC4: 81 08         
                    BGT   L8BD6                   ; 8BC6: 2E 0E         
                    LDA   $0C,U                   ; 8BC8: A6 4C         
                    TSTA                          ; 8BCA: 4D            
                    BPL   L8BCE                   ; 8BCB: 2A 01         
                    NEGA                          ; 8BCD: 40            
L8BCE               CMPA  #$08                    ; 8BCE: 81 08         
                    BGT   L8BD6                   ; 8BD0: 2E 04         
                    LDA   #$00                    ; 8BD2: 86 00         
                    STA   $03,X                   ; 8BD4: A7 03         
L8BD6               LDX   <$5A                    ; 8BD6: 9E 5A         
                    LEAX  $19,X                   ; 8BD8: 30 88 19      
                    CMPX  #$494B                  ; 8BDB: 8C 49 4B        // 3x Tie fighter data structure ($19 bytes per Tie)
                    BCS   L8B89                   ; 8BDE: 25 A9         
; End of function sub_8B86
                    RTS                           ; 8BE0: 39            

; =============== S U B R O U T I N E =======================================
L8BE1               JSR   L8E3A                   ; 8BE1: BD 8E 3A      
L8BE4               LDD   #$00                    ; 8BE4: CC 00 00      
                    STA   <$4C                    ; 8BE7: 97 4C         
                    STA   <$4E                    ; 8BE9: 97 4E         
                    STA   <$4D                    ; 8BEB: 97 4D         
                    STA   <$4F                    ; 8BED: 97 4F         
                    LDA   $15,X                   ; 8BEF: A6 88 15      
                    ANDA  #$10                    ; 8BF2: 84 10         
                    LDB   $4703                   ; 8BF4: F6 47 03      
                    ANDB  #$30                    ; 8BF7: C4 30         
                    STD   $15,X                   ; 8BF9: ED 88 15      
                    LDA   $09,X                   ; 8BFC: A6 09         
                    DECA                          ; 8BFE: 4A            
                    BMI   L8C15                   ; 8BFF: 2B 14         
                    STA   $09,X                   ; 8C01: A7 09         
                    LDD   #$1640                  ; 8C03: CC 16 40      
                    STD   $5022                   ; 8C06: FD 50 22        // Sine for rotation
                    LDD   #$3C02                  ; 8C09: CC 3C 02      
                    STD   $5024                   ; 8C0C: FD 50 24        // Cosine for rotation
                    JSR   L89C8                   ; 8C0F: BD 89 C8        // Space wave roll
; ---------------------------------------------------------------------------
L8C12               JMP   L8C44                   ; 8C12: 7E 8C 44      

; ---------------------------------------------------------------------------
L8C15               LDB   $11,X                   ; 8C15: E6 88 11      
                    STB   <$50                    ; 8C18: D7 50         
                    LSR   <$50                    ; 8C1A: 04 50         
                    BCC   L8C21                   ; 8C1C: 24 03         
                    JSR   L8961                   ; 8C1E: BD 89 61      
; ---------------------------------------------------------------------------
L8C21               LSR   <$50                    ; 8C21: 04 50         
                    BCC   L8C28                   ; 8C23: 24 03         
                    JSR   L8969                   ; 8C25: BD 89 69      
; ---------------------------------------------------------------------------
L8C28               LSR   <$50                    ; 8C28: 04 50         
                    BCC   L8C2F                   ; 8C2A: 24 03         
                    JSR   L8951                   ; 8C2C: BD 89 51      
; ---------------------------------------------------------------------------
L8C2F               LSR   <$50                    ; 8C2F: 04 50         
                    BCC   L8C36                   ; 8C31: 24 03         
                    JSR   L8959                   ; 8C33: BD 89 59      
; ---------------------------------------------------------------------------
L8C36               LSR   <$50                    ; 8C36: 04 50         
                    BCC   L8C3D                   ; 8C38: 24 03         
                    JSR   L8979                   ; 8C3A: BD 89 79      
; ---------------------------------------------------------------------------
L8C3D               LSR   <$50                    ; 8C3D: 04 50         
                    BCC   L8C44                   ; 8C3F: 24 03         
                    JSR   L8971                   ; 8C41: BD 89 71      
; ---------------------------------------------------------------------------
L8C44               JSR   L8D9D                   ; 8C44: BD 8D 9D        // Some tie fighters process
L8C47               JSR   L8DE3                   ; 8C47: BD 8D E3      
L8C4A               LDA   $03,X                   ; 8C4A: A6 03         
                    CMPA  #$01                    ; 8C4C: 81 01         
                    LBNE  L8D66                   ; 8C4E: 10 26 01 14   
                    LDA   $02,X                   ; 8C52: A6 02         
                    JSR   LCE0C                   ; 8C54: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L8C57               LDX   <$5A                    ; 8C57: 9E 5A         
                    CLR   $0A,X                   ; 8C59: 6F 0A         
                    CLRA                          ; 8C5B: 4F            
                    LDB   #$13                    ; 8C5C: C6 13         
                    STD   $4701                   ; 8C5E: FD 47 01        // Point BIC to $5098 MReg4C
                    LDA   $11,X                   ; 8C61: A6 88 11      
                    BITA  #$40                    ; 8C64: 85 40         
                    BEQ   L8C81                   ; 8C66: 27 19         
                    LDD   $5098                   ; 8C68: FC 50 98      
                    ADDD  #$1000                  ; 8C6B: C3 10 00      
                    STD   $5098                   ; 8C6E: FD 50 98      
                    LDA   #$67                    ; 8C71: 86 67         
                    JSR   LCDBA                   ; 8C73: BD CD BA        // Do math program run
L8C76               LDD   $5098                   ; 8C76: FC 50 98      
                    SUBD  #$1000                  ; 8C79: 83 10 00      
                    STD   $5098                   ; 8C7C: FD 50 98      
                    BRA   L8C86                   ; 8C7F: 20 05         

; ---------------------------------------------------------------------------
L8C81               LDA   #$67                    ; 8C81: 86 67         
                    JSR   LCDBA                   ; 8C83: BD CD BA        // Do math program run
L8C86               LDD   $5000                   ; 8C86: FC 50 00        // Math result X
                    BMI   L8CAE                   ; 8C89: 2B 23         
                    INC   $0A,X                   ; 8C8B: 6C 0A         
                    SUBD  #$4000                  ; 8C8D: 83 40 00      
                    BGE   L8CAE                   ; 8C90: 2C 1C         
                    LDD   $15,X                   ; 8C92: EC 88 15      
                    ORB   #$08                    ; 8C95: CA 08         
                    STD   $15,X                   ; 8C97: ED 88 15      
                    LDD   $5072                   ; 8C9A: FC 50 72      
                    ADDD  $5074                   ; 8C9D: F3 50 74      
                    CMPD  #$20                    ; 8CA0: 10 83 00 20   
                    BHI   L8CAE                   ; 8CA4: 22 08         
                    LDD   $15,X                   ; 8CA6: EC 88 15      
                    ORB   #$04                    ; 8CA9: CA 04         
                    STD   $15,X                   ; 8CAB: ED 88 15      
L8CAE               LDD   $15,X                   ; 8CAE: EC 88 15      
                    BITA  #$10                    ; 8CB1: 85 10         
                    BEQ   L8D05                   ; 8CB3: 27 50         
                    LDD   $11,X                   ; 8CB5: EC 88 11      
                    BITA  #$40                    ; 8CB8: 85 40         
                    BNE   L8D05                   ; 8CBA: 26 49         
                    LDD   $5000                   ; 8CBC: FC 50 00        // Math result X
                    SUBD  #$0800                  ; 8CBF: 83 08 00      
                    BLE   L8D05                   ; 8CC2: 2F 41         
                    LDA   $06,X                   ; 8CC4: A6 06         
                    BNE   L8D05                   ; 8CC6: 26 3D         
                    LDB   $4B19                   ; 8CC8: F6 4B 19      
                    CMPB  #$0B                    ; 8CCB: C1 0B         
                    BCS   L8CD4                   ; 8CCD: 25 05         
                    LDU   #byte_8D99              ; 8CCF: CE 8D 99      
                    BRA   L8CDB                   ; 8CD2: 20 07         

; ---------------------------------------------------------------------------
L8CD4               ASLB                          ; 8CD4: 58            
                    ASLB                          ; 8CD5: 58            
                    LDU   #byte_8D71              ; 8CD6: CE 8D 71      
                    LEAU  B,U                     ; 8CD9: 33 C5         
L8CDB               LDA   <$43                    ; 8CDB: 96 43           // Game over/insert coins timer
                    ANDA  ,U                      ; 8CDD: A4 C4         
                    BNE   L8D05                   ; 8CDF: 26 24         
                    LDA   $4703                   ; 8CE1: B6 47 03      
                    CMPA  $01,U                   ; 8CE4: A1 41         
                    BLS   L8D05                   ; 8CE6: 23 1D         
                    LDU   $02,U                   ; 8CE8: EE 42         
L8CEA               LDA   $03,U                   ; 8CEA: A6 43         
                    BNE   L8CFD                   ; 8CEC: 26 0F         
                    LDX   <$5A                    ; 8CEE: 9E 5A         
                    LDD   $15,X                   ; 8CF0: EC 88 15      
                    ORB   #$40                    ; 8CF3: CA 40         
                    STD   $15,X                   ; 8CF5: ED 88 15      
                    JSR   LA68B                   ; 8CF8: BD A6 8B        // Emit fireballs from tie fighters
L8CFB               BRA   L8D05                   ; 8CFB: 20 08         

; ---------------------------------------------------------------------------
L8CFD               LEAU  $06,U                   ; 8CFD: 33 46         
                    CMPU  #$496F                  ; 8CFF: 11 83 49 6F     // 6x Fireball data structure 2 ($6 bytes per fireball)
                    BCS   L8CEA                   ; 8D03: 25 E5         
L8D05               LDX   <$5A                    ; 8D05: 9E 5A         
                    LDA   $11,X                   ; 8D07: A6 88 11      
                    BITA  #$80                    ; 8D0A: 85 80         
                    BEQ   L8D66                   ; 8D0C: 27 58         
                    LDA   <$4F                    ; 8D0E: 96 4F         
                    BNE   L8D3A                   ; 8D10: 26 28         
                    LDA   $5002                   ; 8D12: B6 50 02        // Math result Y
                    BMI   L8D1C                   ; 8D15: 2B 05         
                    JSR   L8979                   ; 8D17: BD 89 79      
; ---------------------------------------------------------------------------
L8D1A               BRA   L8D1F                   ; 8D1A: 20 03         

; ---------------------------------------------------------------------------
L8D1C               JSR   L8971                   ; 8D1C: BD 89 71      
; ---------------------------------------------------------------------------
L8D1F               LDA   <$4E                    ; 8D1F: 96 4E         
                    BNE   L8D3A                   ; 8D21: 26 17         
                    LDB   $5004                   ; 8D23: F6 50 04        // Math result Z
                    SEX                           ; 8D26: 1D            
                    ADDB  #$01                    ; 8D27: CB 01         
                    CMPB  #$01                    ; 8D29: C1 01         
                    BLS   L8D3A                   ; 8D2B: 23 0D         
                    EORA  $5002                   ; 8D2D: B8 50 02        // Math result Y
                    BMI   L8D37                   ; 8D30: 2B 05         
                    JSR   L8961                   ; 8D32: BD 89 61      
; ---------------------------------------------------------------------------
L8D35               BRA   L8D3A                   ; 8D35: 20 03         

; ---------------------------------------------------------------------------
L8D37               JSR   L8969                   ; 8D37: BD 89 69      
; ---------------------------------------------------------------------------
L8D3A               LDA   <$4D                    ; 8D3A: 96 4D         
                    BNE   L8D66                   ; 8D3C: 26 28         
                    LDA   $5004                   ; 8D3E: B6 50 04        // Math result Z
                    BMI   L8D48                   ; 8D41: 2B 05         
                    JSR   L8951                   ; 8D43: BD 89 51      
; ---------------------------------------------------------------------------
L8D46               BRA   L8D4B                   ; 8D46: 20 03         

; ---------------------------------------------------------------------------
L8D48               JSR   L8959                   ; 8D48: BD 89 59      
; ---------------------------------------------------------------------------
L8D4B               LDA   <$4E                    ; 8D4B: 96 4E         
                    BNE   L8D66                   ; 8D4D: 26 17         
                    LDB   $5002                   ; 8D4F: F6 50 02        // Math result Y
                    SEX                           ; 8D52: 1D            
                    ADDB  #$01                    ; 8D53: CB 01         
                    CMPB  #$01                    ; 8D55: C1 01         
                    BLS   L8D66                   ; 8D57: 23 0D         
                    EORA  $5004                   ; 8D59: B8 50 04        // Math result Z
                    BMI   L8D63                   ; 8D5C: 2B 05         
                    JSR   L8969                   ; 8D5E: BD 89 69      
; ---------------------------------------------------------------------------
L8D61               BRA   L8D66                   ; 8D61: 20 03         

; ---------------------------------------------------------------------------
L8D63               JSR   L8961                   ; 8D63: BD 89 61      
; ---------------------------------------------------------------------------
L8D66               LDX   <$5A                    ; 8D66: 9E 5A         
                    LDD   $15,X                   ; 8D68: EC 88 15      
                    ANDA  #$EF                    ; 8D6B: 84 EF         
                    STD   $15,X                   ; 8D6D: ED 88 15      
; End of function sub_8BE1
; ---------------------------------------------------------------------------
                    RTS                           ; 8D70: 39            

; =============== S U B R O U T I N E =======================================
; Some tie fighters process
L8D9D               LDX   <$5A                    ; 8D9D: 9E 5A         
                    LDU   ,X                      ; 8D9F: EE 84         
                    LDA   $06,X                   ; 8DA1: A6 06         
                    BNE   L8DDF                   ; 8DA3: 26 3A         
                    LDB   $12,X                   ; 8DA5: E6 88 12      
                    STB   <$50                    ; 8DA8: D7 50         
                    LDU   ,X                      ; 8DAA: EE 84         
                    LDD   #$00                    ; 8DAC: CC 00 00      
                    STD   -10,U                   ; 8DAF: ED 56         
                    STD   -2,U                    ; 8DB1: ED 5E         
                    STD   $06,U                   ; 8DB3: ED 46         
                    LSR   <$50                    ; 8DB5: 04 50         
                    BCC   L8DBC                   ; 8DB7: 24 03         
                    JSR   L8A59                   ; 8DB9: BD 8A 59      
L8DBC               LSR   <$50                    ; 8DBC: 04 50         
                    BCC   L8DC3                   ; 8DBE: 24 03         
                    JSR   L8A7E                   ; 8DC0: BD 8A 7E      
L8DC3               LSR   <$50                    ; 8DC3: 04 50         
                    BCC   L8DCA                   ; 8DC5: 24 03         
                    JSR   L8A21                   ; 8DC7: BD 8A 21      
L8DCA               LSR   <$50                    ; 8DCA: 04 50         
                    BCC   L8DD1                   ; 8DCC: 24 03         
                    JSR   L8A3D                   ; 8DCE: BD 8A 3D      
L8DD1               LSR   <$50                    ; 8DD1: 04 50         
                    BCC   L8DD8                   ; 8DD3: 24 03         
                    JSR   L8A05                   ; 8DD5: BD 8A 05      
L8DD8               LSR   <$50                    ; 8DD8: 04 50         
                    BCC   L8DDF                   ; 8DDA: 24 03         
                    JSR   L89E9                   ; 8DDC: BD 89 E9      
L8DDF               JSR   L8AB6                   ; 8DDF: BD 8A B6      
; End of function sub_8D9D
L8DE2               RTS                           ; 8DE2: 39            

; =============== S U B R O U T I N E =======================================
L8DE3               LDU   ,X                      ; 8DE3: EE 84         
                    LDD   $08,U                   ; 8DE5: EC 48         
                    CMPA  #$7D                    ; 8DE7: 81 7D         
                    BLT   L8DEE                   ; 8DE9: 2D 03         
                    LDD   #$7CFF                  ; 8DEB: CC 7C FF      
L8DEE               CMPA  #$82                    ; 8DEE: 81 82         
                    BGT   L8DF5                   ; 8DF0: 2E 03         
                    LDD   #$8300                  ; 8DF2: CC 83 00      
L8DF5               STD   $08,U                   ; 8DF5: ED 48         
                    LDD   $0A,U                   ; 8DF7: EC 4A         
                    CMPA  #$7D                    ; 8DF9: 81 7D         
                    BLT   L8E00                   ; 8DFB: 2D 03         
                    LDD   #$7CFF                  ; 8DFD: CC 7C FF      
L8E00               CMPA  #$82                    ; 8E00: 81 82         
                    BGT   L8E07                   ; 8E02: 2E 03         
                    LDD   #$8300                  ; 8E04: CC 83 00      
L8E07               STD   $0A,U                   ; 8E07: ED 4A         
                    LDD   $0C,U                   ; 8E09: EC 4C         
                    CMPA  #$7D                    ; 8E0B: 81 7D         
                    BLT   L8E12                   ; 8E0D: 2D 03         
                    LDD   #$7CFF                  ; 8E0F: CC 7C FF      
L8E12               CMPA  #$82                    ; 8E12: 81 82         
                    BGT   L8E19                   ; 8E14: 2E 03         
                    LDD   #$8300                  ; 8E16: CC 83 00      
L8E19               STD   $0C,U                   ; 8E19: ED 4C         
; End of function sub_8DE3
                    RTS                           ; 8E1B: 39            

; =============== S U B R O U T I N E =======================================
L8E1C               DEC   <$E6                    ; 8E1C: 0A E6         
                    BGT   locret_8E22             ; 8E1E: 2E 02         
                    CLR   <$E6                    ; 8E20: 0F E6         
; End of function sub_8E1C
locret_8E22         RTS                           ; 8E22: 39            

; =============== S U B R O U T I N E =======================================
L8E23               LDD   #$00                    ; 8E23: CC 00 00      
                    STD   $11,X                   ; 8E26: ED 88 11      
                    STA   $10,X                   ; 8E29: A7 88 10      
                    STD   $13,X                   ; 8E2C: ED 88 13      
                    STD   $15,X                   ; 8E2F: ED 88 15      
L8E32               LDU   $0D,X                   ; 8E32: EE 0D         
                    LDA   ,U                      ; 8E34: A6 C4         
                    STA   $0F,X                   ; 8E36: A7 0F         
                    BEQ   L8E51                   ; 8E38: 27 17         
L8E3A               LDD   $15,X                   ; 8E3A: EC 88 15      
                    ANDA  $13,X                   ; 8E3D: A4 88 13      
                    BNE   L8E5C                   ; 8E40: 26 1A         
                    ANDB  $14,X                   ; 8E42: E4 88 14      
                    BNE   L8E5C                   ; 8E45: 26 15         
                    LDA   $0F,X                   ; 8E47: A6 0F         
                    ANDA  #$07                    ; 8E49: 84 07         
                    ASLA                          ; 8E4B: 48            
                    LDU   #JumpTable8E68          ; 8E4C: CE 8E 68      
                    JMP   [A,U]                   ; 8E4F: 6E D6         

; ---------------------------------------------------------------------------
L8E51               LDD   $01,U                   ; 8E51: EC 41         
                    STD   $13,X                   ; 8E53: ED 88 13      
                    LEAU  $03,U                   ; 8E56: 33 43         
                    STU   $0D,X                   ; 8E58: EF 0D         
                    BRA   L8E32                   ; 8E5A: 20 D6         

; ---------------------------------------------------------------------------
L8E5C               LDU   $0D,X                   ; 8E5C: EE 0D         
L8E5E               LDA   ,U                      ; 8E5E: A6 C4         
                    BEQ   L8E32                   ; 8E60: 27 D0         
                    LEAU  $03,U                   ; 8E62: 33 43         
                    STU   $0D,X                   ; 8E64: EF 0D         
; End of function sub_8E23
; ---------------------------------------------------------------------------
                    BRA   L8E5E                   ; 8E66: 20 F6         

; =============== S U B R O U T I N E =======================================
; End of function sub_8E78
L8E78               SWI                           ; 8E78: 3F            
; =============== S U B R O U T I N E =======================================
L8E79               LDU   $0D,X                   ; 8E79: EE 0D         
L8E7B               LDD   $01,U                   ; 8E7B: EC 41         
                    BEQ   L8E94                   ; 8E7D: 27 15         
                    ANDA  $15,X                   ; 8E7F: A4 88 15      
                    BNE   L8E94                   ; 8E82: 26 10         
                    ANDB  $16,X                   ; 8E84: E4 88 16      
                    BNE   L8E94                   ; 8E87: 26 0B         
L8E89               LEAU  $03,U                   ; 8E89: 33 43         
                    LDA   ,U                      ; 8E8B: A6 C4         
                    ASLA                          ; 8E8D: 48            
                    BNE   L8E89                   ; 8E8E: 26 F9         
                    BCC   L8E89                   ; 8E90: 24 F7         
                    BRA   L8E7B                   ; 8E92: 20 E7         

; ---------------------------------------------------------------------------
L8E94               LEAU  $03,U                   ; 8E94: 33 43         
                    STU   $0D,X                   ; 8E96: EF 0D         
; End of function sub_8E79
                    JMP   L8E32                   ; 8E98: 7E 8E 32      

; =============== S U B R O U T I N E =======================================
L8E9B               LDU   $0D,X                   ; 8E9B: EE 0D         
                    LDU   $01,U                   ; 8E9D: EE 41         
                    STU   $0D,X                   ; 8E9F: EF 0D         
; End of function sub_8E9B
                    JMP   L8E32                   ; 8EA1: 7E 8E 32      

; =============== S U B R O U T I N E =======================================
L8EA4               LDU   $0D,X                   ; 8EA4: EE 0D         
                    LEAU  $03,U                   ; 8EA6: 33 43         
                    STU   $17,X                   ; 8EA8: EF 88 17      
                    LDU   -2,U                    ; 8EAB: EE 5E         
                    STU   $0D,X                   ; 8EAD: EF 0D         
; End of function sub_8EA4
                    JMP   L8E32                   ; 8EAF: 7E 8E 32      

; =============== S U B R O U T I N E =======================================
L8EB2               LDU   $17,X                   ; 8EB2: EE 88 17      
                    STU   $0D,X                   ; 8EB5: EF 0D         
; End of function sub_8EB2
                    JMP   L8E32                   ; 8EB7: 7E 8E 32      

; =============== S U B R O U T I N E =======================================
L8EBA               LDU   $0D,X                   ; 8EBA: EE 0D         
                    LDB   ,U                      ; 8EBC: E6 C4         
                    LSRB                          ; 8EBE: 54            
                    STB   $10,X                   ; 8EBF: E7 88 10      
                    LDD   $01,U                   ; 8EC2: EC 41         
                    STD   $11,X                   ; 8EC4: ED 88 11      
                    LEAU  $03,U                   ; 8EC7: 33 43         
                    STU   $0D,X                   ; 8EC9: EF 0D         
                    INC   $0F,X                   ; 8ECB: 6C 0F         
; End of function sub_8EBA
                    RTS                           ; 8ECD: 39            

; =============== S U B R O U T I N E =======================================
L8ECE               DEC   $10,X                   ; 8ECE: 6A 88 10      
                    LBMI  L8E32                   ; 8ED1: 10 2B FF 5D   
; End of function sub_8ECE
                    RTS                           ; 8ED5: 39            

; =============== S U B R O U T I N E =======================================
L8ED6               JSR   LCCC0                   ; 8ED6: BD CC C0        // Initialise object?
L8ED9               LDB   $4B14                   ; 8ED9: F6 4B 14      
                    ASLB                          ; 8EDC: 58            
                    LDX   #off_9070               ; 8EDD: 8E 90 70      
                    ABX                           ; 8EE0: 3A            
                    CMPX  #byte_907C              ; 8EE1: 8C 90 7C      
                    BCS   L8EF4                   ; 8EE4: 25 0E         
                    LDA   $4B14                   ; 8EE6: B6 4B 14      
                    LSRA                          ; 8EE9: 44            
                    BCS   L8EF1                   ; 8EEA: 25 05         
                    LDX   #off_9078               ; 8EEC: 8E 90 78      
                    BRA   L8EF4                   ; 8EEF: 20 03         

; ---------------------------------------------------------------------------
L8EF1               LDX   #off_907A               ; 8EF1: 8E 90 7A      
L8EF4               LDX   ,X                      ; 8EF4: AE 84         
                    LDB   <$DD                    ; 8EF6: D6 DD         
                    CMPB  ,X+                     ; 8EF8: E1 80         
                    BLS   L8EFE                   ; 8EFA: 23 02         
                    LDB   -1,X                    ; 8EFC: E6 1F         
L8EFE               ASLB                          ; 8EFE: 58            
                    LDD   B,X                     ; 8EFF: EC 85         
                    STD   <$E4                    ; 8F01: DD E4         
                    CLR   <$E6                    ; 8F03: 0F E6         
                    LDX   #$4900                  ; 8F05: 8E 49 00        // 3x Tie fighter data structure ($19 bytes per Tie)
L8F08               STX   <$5A                    ; 8F08: 9F 5A         
                    CLR   $03,X                   ; 8F0A: 6F 03         
                    LDU   <$E4                    ; 8F0C: DE E4         
                    BEQ   L8F29                   ; 8F0E: 27 19         
                    LDA   ,U                      ; 8F10: A6 C4         
                    BEQ   L8F29                   ; 8F12: 27 15         
                    INC   <$E6                    ; 8F14: 0C E6         
                    LDD   ,U                      ; 8F16: EC C4         
                    STD   <$E0                    ; 8F18: DD E0           // Pointer to 3D object index
                    LDD   $02,U                   ; 8F1A: EC 42         
                    STD   <$DE                    ; 8F1C: DD DE         
                    LDD   $04,U                   ; 8F1E: EC 44         
                    STD   <$E2                    ; 8F20: DD E2         
                    LEAU  $06,U                   ; 8F22: 33 46         
                    STU   <$E4                    ; 8F24: DF E4         
                    JSR   L8F34                   ; 8F26: BD 8F 34      
L8F29               LDX   <$5A                    ; 8F29: 9E 5A         
                    LEAX  $19,X                   ; 8F2B: 30 88 19      
                    CMPX  #$494B                  ; 8F2E: 8C 49 4B        // 3x Tie fighter data structure ($19 bytes per Tie)
                    BCS   L8F08                   ; 8F31: 25 D5         
; End of function sub_8ED6
                    RTS                           ; 8F33: 39            

; =============== S U B R O U T I N E =======================================
L8F34               LDA   #$01                    ; 8F34: 86 01           // Called 3 times at start of attract screen 1 and 2
                    STA   $03,X                   ; 8F36: A7 03         
                    LDU   ,X                      ; 8F38: EE 84         
                    JSR   LCDC3                   ; 8F3A: BD CD C3        // Initialise math registers matrix
L8F3D               LDU   ,X                      ; 8F3D: EE 84         
                    LDA   #$C0                    ; 8F3F: 86 C0           // Matrix -1.000 constant
                    STA   -16,U                   ; 8F41: A7 50         
                    STA   -6,U                    ; 8F43: A7 5A         
                    CLRA                          ; 8F45: 4F            
                    STA   $09,X                   ; 8F46: A7 09         
                    STA   $08,X                   ; 8F48: A7 08         
                    STA   $06,X                   ; 8F4A: A7 06         
                    STA   $05,X                   ; 8F4C: A7 05         
                    STA   $0B,X                   ; 8F4E: A7 0B         
                    LDU   <$E0                    ; 8F50: DE E0           // Pointer to 3D object index
                    LDA   $01,U                   ; 8F52: A6 41         
                    STA   $07,X                   ; 8F54: A7 07         
                    LDU   ,X                      ; 8F56: EE 84         
                    LDX   <$E2                    ; 8F58: 9E E2         
                    LDD   ,X                      ; 8F5A: EC 84         
                    STD   $08,U                   ; 8F5C: ED 48         
                    LDD   $02,X                   ; 8F5E: EC 02         
                    STD   $0A,U                   ; 8F60: ED 4A         
                    LDD   $04,X                   ; 8F62: EC 04         
                    STD   $0C,U                   ; 8F64: ED 4C         
                    LDX   <$5A                    ; 8F66: 9E 5A         
                    LDD   <$DE                    ; 8F68: DC DE         
                    STD   $0D,X                   ; 8F6A: ED 0D         
                    JSR   L8E23                   ; 8F6C: BD 8E 23      
L8F6F               LDX   <$5A                    ; 8F6F: 9E 5A         
                    LDB   [$48E0]                 ; 8F71: E6 9F 48 E0     // Pointer to 3D object index
                    STB   $04,X                   ; 8F75: E7 04         
                    JSR   LCCCC                   ; 8F77: BD CC CC        // Copy XYZ data to math RAM
; End of function sub_8F34
L8F7A               RTS                           ; 8F7A: 39            

; =============== S U B R O U T I N E =======================================
L8F7B               LDU   <$E4                    ; 8F7B: DE E4         
                    BEQ   L8F83                   ; 8F7D: 27 04         
                    LDA   ,U                      ; 8F7F: A6 C4         
                    BNE   L8FB1                   ; 8F81: 26 2E         
L8F83               INC   <$DD                    ; 8F83: 0C DD         
                    LDB   $4B14                   ; 8F85: F6 4B 14      
                    ASLB                          ; 8F88: 58            
                    LDX   #off_9070               ; 8F89: 8E 90 70      
                    ABX                           ; 8F8C: 3A            
                    CMPX  #byte_907C              ; 8F8D: 8C 90 7C      
                    BCS   L8FA0                   ; 8F90: 25 0E         
                    LDA   $4B14                   ; 8F92: B6 4B 14      
                    LSRA                          ; 8F95: 44            
                    BCS   L8F9D                   ; 8F96: 25 05         
                    LDX   #off_9078               ; 8F98: 8E 90 78      
                    BRA   L8FA0                   ; 8F9B: 20 03         

; ---------------------------------------------------------------------------
L8F9D               LDX   #off_907A               ; 8F9D: 8E 90 7A      
L8FA0               LDX   ,X                      ; 8FA0: AE 84         
                    LDB   <$DD                    ; 8FA2: D6 DD         
                    CMPB  ,X+                     ; 8FA4: E1 80         
                    BLS   L8FAA                   ; 8FA6: 23 02         
                    LDB   -1,X                    ; 8FA8: E6 1F         
L8FAA               STB   <$DD                    ; 8FAA: D7 DD         
                    ASLB                          ; 8FAC: 58            
                    LDD   B,X                     ; 8FAD: EC 85         
                    STD   <$E4                    ; 8FAF: DD E4         
L8FB1               LDX   #$4900                  ; 8FB1: 8E 49 00        // 3x Tie fighter data structure ($19 bytes per Tie)
L8FB4               STX   <$5A                    ; 8FB4: 9F 5A         
                    LDA   $03,X                   ; 8FB6: A6 03         
                    BEQ   L8FC6                   ; 8FB8: 27 0C         
                    LDX   <$5A                    ; 8FBA: 9E 5A         
                    LEAX  $19,X                   ; 8FBC: 30 88 19      
                    CMPX  #$494B                  ; 8FBF: 8C 49 4B        // 3x Tie fighter data structure ($19 bytes per Tie)
                    BCS   L8FB4                   ; 8FC2: 25 F0         
                    BRA   locret_9023             ; 8FC4: 20 5D         

; ---------------------------------------------------------------------------
L8FC6               LDU   <$E4                    ; 8FC6: DE E4         
                    BEQ   locret_9023             ; 8FC8: 27 59         
                    LDA   ,U                      ; 8FCA: A6 C4         
                    BEQ   locret_9023             ; 8FCC: 27 55         
                    INC   <$E6                    ; 8FCE: 0C E6         
                    LDD   ,U                      ; 8FD0: EC C4         
                    STD   <$E0                    ; 8FD2: DD E0           // Pointer to 3D object index
                    LDD   $02,U                   ; 8FD4: EC 42         
                    STD   <$DE                    ; 8FD6: DD DE         
                    LDD   $04,U                   ; 8FD8: EC 44         
                    STD   <$E2                    ; 8FDA: DD E2         
                    LEAU  $06,U                   ; 8FDC: 33 46         
                    STU   <$E4                    ; 8FDE: DF E4         
                    LDA   #$01                    ; 8FE0: 86 01         
                    STA   $03,X                   ; 8FE2: A7 03         
                    LDU   ,X                      ; 8FE4: EE 84         
                    JSR   LCDC3                   ; 8FE6: BD CD C3        // Initialise math registers matrix
L8FE9               LDU   ,X                      ; 8FE9: EE 84         
                    LDA   #$C0                    ; 8FEB: 86 C0         
                    STA   -16,U                   ; 8FED: A7 50         
                    STA   -6,U                    ; 8FEF: A7 5A         
                    CLRA                          ; 8FF1: 4F            
                    STA   $09,X                   ; 8FF2: A7 09         
                    STA   $08,X                   ; 8FF4: A7 08         
                    STA   $06,X                   ; 8FF6: A7 06         
                    STA   $05,X                   ; 8FF8: A7 05         
                    STA   $0B,X                   ; 8FFA: A7 0B         
                    LDU   <$E0                    ; 8FFC: DE E0           // Pointer to 3D object index
                    LDA   $01,U                   ; 8FFE: A6 41         
                    STA   $07,X                   ; 9000: A7 07         
                    LDU   ,X                      ; 9002: EE 84         
                    LDX   <$E2                    ; 9004: 9E E2         
                    LDD   ,X                      ; 9006: EC 84         
                    STD   $08,U                   ; 9008: ED 48         
                    LDD   $02,X                   ; 900A: EC 02         
                    STD   $0A,U                   ; 900C: ED 4A         
                    LDD   $04,X                   ; 900E: EC 04         
                    STD   $0C,U                   ; 9010: ED 4C         
                    LDX   <$5A                    ; 9012: 9E 5A         
                    LDD   <$DE                    ; 9014: DC DE         
                    STD   $0D,X                   ; 9016: ED 0D         
                    JSR   L8E23                   ; 9018: BD 8E 23      
L901B               LDX   <$5A                    ; 901B: 9E 5A         
                    LDB   [$48E0]                 ; 901D: E6 9F 48 E0     // Pointer to 3D object index
                    STB   $04,X                   ; 9021: E7 04         
; End of function sub_8F7B
; ---------------------------------------------------------------------------
locret_9023         RTS                           ; 9023: 39            

; =============== S U B R O U T I N E =======================================
L953B               LDA   $4592                   ; 953B: B6 45 92      
                    ANDA  #$03                    ; 953E: 84 03         
                    BEQ   locret_9557             ; 9540: 27 15         
                    ADDA  <$60                    ; 9542: 9B 60           // Shield count
                    STA   <$60                    ; 9544: 97 60           // Shield count
                    LDA   $4593                   ; 9546: B6 45 93      
                    ANDA  #$03                    ; 9549: 84 03         
                    ADDA  #$06                    ; 954B: 8B 06         
                    CMPA  <$60                    ; 954D: 91 60           // Shield count
                    BCC   L9553                   ; 954F: 24 02         
                    STA   <$60                    ; 9551: 97 60           // Shield count
L9553               LDA   #$14                    ; 9553: 86 14         
                    STA   <$61                    ; 9555: 97 61         
; End of function sub_953B
locret_9557         RTS                           ; 9557: 39            

; =============== S U B R O U T I N E =======================================
; Process shields
L9558               LDA   <$8B                    ; 9558: 96 8B         
                    BLE   L95A0                   ; 955A: 2F 44         
                    LDA   <$8C                    ; 955C: 96 8C           // Sheild being depleted
                    BGT   L95A0                   ; 955E: 2E 40         
                    LDA   #$01                    ; 9560: 86 01         
                    STA   <$8C                    ; 9562: 97 8C           // Sheild being depleted
                    LDA   <$60                    ; 9564: 96 60           // Shield count
                    STA   <$8E                    ; 9566: 97 8E         
                    LDA   #$F6                    ; 9568: 86 F6         
                    ADDA  <$60                    ; 956A: 9B 60           // Shield count
                    STA   <$8D                    ; 956C: 97 8D         
L956E               DEC   <$60                    ; 956E: 0A 60           // Shield count
                    BGE   L9578                   ; 9570: 2C 06         
                    LDA   #$FF                    ; 9572: 86 FF         
                    STA   <$60                    ; 9574: 97 60           // Shield count
                    CLR   <$8C                    ; 9576: 0F 8C           // Sheild being depleted
L9578               LDA   <$60                    ; 9578: 96 60           // Shield count
                    CMPA  #$00                    ; 957A: 81 00         
                    BNE   L9586                   ; 957C: 26 08         
                    JSR   Sound_D                 ; 957E: BD BD 35      
L9581               JSR   Sound_28                ; 9581: BD BD BC      
L9584               BRA   L9599                   ; 9584: 20 13         

; ---------------------------------------------------------------------------
L9586               CMPA  #$01                    ; 9586: 81 01         
                    BNE   L958F                   ; 9588: 26 05         
                    JSR   Sound_2F                ; 958A: BD BD DF      
L958D               BRA   L9599                   ; 958D: 20 0A         

; ---------------------------------------------------------------------------
L958F               CMPA  #$02                    ; 958F: 81 02         
                    BNE   L9599                   ; 9591: 26 06         
                    JSR   Sound_F                 ; 9593: BD BD 3F      
L9596               JSR   Sound_30                ; 9596: BD BD E4      
L9599               CLRB                          ; 9599: 5F            
                    STB   <$91                    ; 959A: D7 91         
                    STB   <$90                    ; 959C: D7 90         
                    STB   <$8F                    ; 959E: D7 8F         
L95A0               LDA   <$61                    ; 95A0: 96 61         
                    BEQ   locret_95A6             ; 95A2: 27 02         
                    DEC   <$61                    ; 95A4: 0A 61         
; End of function sub_9558
locret_95A6         RTS                           ; 95A6: 39            

; =============== S U B R O U T I N E =======================================
; Insert vector instructions for shields
L95A7               LDA   <$8C                    ; 95A7: 96 8C           // Sheild being depleted
                    BNE   L95B2                   ; 95A9: 26 07         
                    LDA   <$60                    ; 95AB: 96 60           // Shield count
                    BGT   L95B2                   ; 95AD: 2E 03         
                    JMP   L9604                   ; 95AF: 7E 96 04      

; ---------------------------------------------------------------------------
L95B2               LDB   <$60                    ; 95B2: D6 60           // Shield count
                    ASLB                          ; 95B4: 58            
                    LDX   #word_96B6              ; 95B5: 8E 96 B6        // Shield colour table
                    LDU   B,X                     ; 95B8: EE 85         
                    STU   ,Y++                    ; 95BA: EF A1         
                    LDD   #$BA03                  ; 95BC: CC BA 03      
                    STD   ,Y++                    ; 95BF: ED A1         
                    LDD   #$0228                  ; 95C1: CC 02 28      
                    STD   ,Y++                    ; 95C4: ED A1         
                    LDD   #$00                    ; 95C6: CC 00 00      
                    STD   ,Y++                    ; 95C9: ED A1         
                    LDA   <$8C                    ; 95CB: 96 8C           // Sheild being depleted
                    BEQ   L95D4                   ; 95CD: 27 05         
                    JSR   L962A                   ; 95CF: BD 96 2A      
L95D2               BRA   L95D7                   ; 95D2: 20 03         

; ---------------------------------------------------------------------------
L95D4               JSR   L960F                   ; 95D4: BD 96 0F      
L95D7               LDD   #$1FD0                  ; 95D7: CC 1F D0      
                    STD   ,Y++                    ; 95DA: ED A1         
                    LDD   #$1FF4                  ; 95DC: CC 1F F4      
                    STD   ,Y++                    ; 95DF: ED A1         
                    LDA   <$8C                    ; 95E1: 96 8C           // Sheild being depleted
                    BLE   L95EA                   ; 95E3: 2F 05         
                    LDU   #$A018                  ; 95E5: CE A0 18      
                    BRA   L95F2                   ; 95E8: 20 08         

; ---------------------------------------------------------------------------
L95EA               LDB   <$60                    ; 95EA: D6 60           // Shield count
                    ASLB                          ; 95EC: 58            
                    LDX   #word_96B6              ; 95ED: 8E 96 B6        // Shield colour table
                    LDU   B,X                     ; 95F0: EE 85         
L95F2               STU   ,Y++                    ; 95F2: EF A1         
                    CLR   <$AD                    ; 95F4: 0F AD         
                    LDA   <$60                    ; 95F6: 96 60           // Shield count
                    BGE   L95FB                   ; 95F8: 2C 01         
                    CLRA                          ; 95FA: 4F            
L95FB               JSR   LE7AD                   ; 95FB: BD E7 AD      
L95FE               LDD   #$8040                  ; 95FE: CC 80 40      
                    STD   ,Y++                    ; 9601: ED A1         
                    RTS                           ; 9603: 39            

; ---------------------------------------------------------------------------
L9604               LDD   #$7100                  ; 9604: CC 71 00      
                    STD   ,Y++                    ; 9607: ED A1         
                    LDB   #$0E                    ; 9609: C6 0E         
                    JSR   LE7C7                   ; 960B: BD E7 C7        // Print text string from pointer table
; End of function sub_95A7
L960E               RTS                           ; 960E: 39            

; =============== S U B R O U T I N E =======================================
L960F               LDB   <$60                    ; 960F: D6 60           // Shield count
                    ASLB                          ; 9611: 58            
                    LDX   #word_96B6              ; 9612: 8E 96 B6        // Shield colour table
                    LDD   B,X                     ; 9615: EC 85         
                    TST   <$61                    ; 9617: 0D 61         
                    BEQ   L961D                   ; 9619: 27 02         
                    ORB   #$FF                    ; 961B: CA FF         
L961D               STD   ,Y++                    ; 961D: ED A1         
                    LDB   <$60                    ; 961F: D6 60           // Shield count
                    ASLB                          ; 9621: 58            
                    LDX   #word_96CA              ; 9622: 8E 96 CA        // Shield vector table
                    LDU   B,X                     ; 9625: EE 85         
                    STU   ,Y++                    ; 9627: EF A1         
; End of function sub_960F
                    RTS                           ; 9629: 39            

; =============== S U B R O U T I N E =======================================
L962A               LDA   <$91                    ; 962A: 96 91         
                    BNE   L964B                   ; 962C: 26 1D         
                    LDU   #$A018                  ; 962E: CE A0 18      
                    STU   ,Y++                    ; 9631: EF A1         
                    LDB   <$8E                    ; 9633: D6 8E         
                    ASLB                          ; 9635: 58            
                    LDX   #word_96CA              ; 9636: 8E 96 CA        // Shield vector table
                    LDU   B,X                     ; 9639: EE 85         
                    STU   ,Y++                    ; 963B: EF A1         
                    LDA   <$43                    ; 963D: 96 43           // Game over/insert coins timer
                    ANDA  #$00                    ; 963F: 84 00         
                    BNE   L9649                   ; 9641: 26 06         
L9643               INC   <$8D                    ; 9643: 0C 8D         
                    BLT   L9649                   ; 9645: 2D 02         
                    INC   <$91                    ; 9647: 0C 91         
L9649               BRA   locret_96A0             ; 9649: 20 55         

; ---------------------------------------------------------------------------
L964B               JSR   L96A1                   ; 964B: BD 96 A1      
L964E               LDU   #$A018                  ; 964E: CE A0 18      
                    STU   ,Y++                    ; 9651: EF A1         
                    LDA   <$90                    ; 9653: 96 90         
                    BNE   L9674                   ; 9655: 26 1D         
                    LDB   <$8E                    ; 9657: D6 8E         
                    ASLB                          ; 9659: 58            
                    LDX   #word_96DE              ; 965A: 8E 96 DE        // Another copy of shield vector table??
                    LDU   B,X                     ; 965D: EE 85         
                    STU   ,Y++                    ; 965F: EF A1         
                    LDA   <$43                    ; 9661: 96 43           // Game over/insert coins timer
                    ANDA  #$00                    ; 9663: 84 00         
                    BNE   L9672                   ; 9665: 26 0B         
                    INC   <$90                    ; 9667: 0C 90         
                    LDB   <$60                    ; 9669: D6 60           // Shield count
                    LDX   #byte_9718              ; 966B: 8E 97 18      
                    LDA   B,X                     ; 966E: A6 85         
                    STA   <$8D                    ; 9670: 97 8D         
L9672               BRA   locret_96A0             ; 9672: 20 2C         

; ---------------------------------------------------------------------------
L9674               LDA   <$8F                    ; 9674: 96 8F         
                    BNE   L9690                   ; 9676: 26 18         
                    LDB   <$8D                    ; 9678: D6 8D         
                    ASLB                          ; 967A: 58            
                    LDX   #word_96F2              ; 967B: 8E 96 F2      
                    LDU   B,X                     ; 967E: EE 85         
                    STU   ,Y++                    ; 9680: EF A1         
                    LDA   <$43                    ; 9682: 96 43           // Game over/insert coins timer
                    ANDA  #$00                    ; 9684: 84 00         
                    BNE   L968E                   ; 9686: 26 06         
                    DEC   <$8D                    ; 9688: 0A 8D         
                    BGT   L968E                   ; 968A: 2E 02         
                    INC   <$8F                    ; 968C: 0C 8F         
L968E               BRA   locret_96A0             ; 968E: 20 10         

; ---------------------------------------------------------------------------
L9690               LDA   <$43                    ; 9690: 96 43           // Game over/insert coins timer
                    ANDA  #$00                    ; 9692: 84 00         
                    BNE   locret_96A0             ; 9694: 26 0A         
                    LDA   #$00                    ; 9696: 86 00         
                    STA   <$8C                    ; 9698: 97 8C           // Sheild being depleted
                    STA   <$8B                    ; 969A: 97 8B         
                    LDA   <$60                    ; 969C: 96 60           // Shield count
                    STA   <$8E                    ; 969E: 97 8E         
; End of function sub_962A
locret_96A0         RTS                           ; 96A0: 39            

; =============== S U B R O U T I N E =======================================
L96A1               LDB   <$60                    ; 96A1: D6 60           // Shield count
                    ASLB                          ; 96A3: 58            
                    LDX   #word_96B6              ; 96A4: 8E 96 B6        // Shield colour table
                    LDD   B,X                     ; 96A7: EC 85         
                    STD   ,Y++                    ; 96A9: ED A1         
                    LDB   <$60                    ; 96AB: D6 60           // Shield count
                    ASLB                          ; 96AD: 58            
                    LDX   #word_96CA              ; 96AE: 8E 96 CA        // Shield vector table
                    LDU   B,X                     ; 96B1: EE 85         
                    STU   ,Y++                    ; 96B3: EF A1         
; End of function sub_96A1
; ---------------------------------------------------------------------------
                    RTS                           ; 96B5: 39            

; =============== S U B R O U T I N E =======================================
; Death Star starting wave bonus score
L9722               LDA   $4B2D                   ; 9722: B6 4B 2D      
                    BNE   locret_9739             ; 9725: 26 12         
                    LDB   $4B15                   ; 9727: F6 4B 15      
                    BEQ   locret_9739             ; 972A: 27 0D         
                    ASLB                          ; 972C: 58            
                    ADDB  $4B15                   ; 972D: FB 4B 15      
                    LDX   #byte_9865              ; 9730: 8E 98 65        // Death Star destroyed  incrementing score value, Also has unused starting wave bonus scores of 200,000 and 600,000, for waves 2 and 4
                    ABX                           ; 9733: 3A            
                    TFR   X,U                     ; 9734: 1F 13         
                    JSR   L9810                   ; 9736: BD 98 10        // Add to score total
; End of function sub_9722
locret_9739         RTS                           ; 9739: 39            

; =============== S U B R O U T I N E =======================================
; Towers incrementing score
L973A               LDU   #$4B2E                  ; 973A: CE 4B 2E        // Temporary score adder towers 1
                    JSR   L9810                   ; 973D: BD 98 10        // Add to score total
L9740               LDX   #word_9856              ; 9740: 8E 98 56      
                    LDA   $4B30                   ; 9743: B6 4B 30        // Temporary score adder towers 3
                    ADDA  $02,X                   ; 9746: AB 02         
                    DAA                           ; 9748: 19            
                    STA   $4B30                   ; 9749: B7 4B 30        // Temporary score adder towers 3
                    LDA   $4B2F                   ; 974C: B6 4B 2F        // Temporary score adder towers 2
                    ADCA  $01,X                   ; 974F: A9 01         
                    DAA                           ; 9751: 19            
                    STA   $4B2F                   ; 9752: B7 4B 2F        // Temporary score adder towers 2
                    LDA   $4B2E                   ; 9755: B6 4B 2E        // Temporary score adder towers 1
                    ADCA  ,X                      ; 9758: A9 84         
                    DAA                           ; 975A: 19            
                    STA   $4B2E                   ; 975B: B7 4B 2E        // Temporary score adder towers 1
                    LDA   $4B1A                   ; 975E: B6 4B 1A      
                    BEQ   locret_9774             ; 9761: 27 11         
                    ADDA  #$99                    ; 9763: 8B 99         
                    DAA                           ; 9765: 19            
                    STA   $4B1A                   ; 9766: B7 4B 1A      
                    BNE   locret_9774             ; 9769: 26 09         
                    LDU   #byte_9862              ; 976B: CE 98 62        // Cleared all towers score value
                    JSR   L9810                   ; 976E: BD 98 10        // Add to score total
L9771               INC   $4B35                   ; 9771: 7C 4B 35      
; End of function sub_973A
locret_9774         RTS                           ; 9774: 39            

; =============== S U B R O U T I N E =======================================
; Shield bonus score
L9775               LDB   <$60                    ; 9775: D6 60           // Shield count
                    BEQ   locret_97AB             ; 9777: 27 32         
                    LDU   #byte_9865              ; 9779: CE 98 65        // Death Star destroyed  incrementing score value, Also has unused starting wave bonus scores of 200,000 and 600,000, for waves 2 and 4
                    LDA   #$00                    ; 977C: 86 00         
                    STA   $4B29                   ; 977E: B7 4B 29        // Temporary score adder 1
                    STA   $4B2A                   ; 9781: B7 4B 2A        // Temporary score adder 1
                    STA   $4B2B                   ; 9784: B7 4B 2B        // Temporary score adder 2
L9787               LDA   $02,U                   ; 9787: A6 42         
                    ADDA  $4B2B                   ; 9789: BB 4B 2B        // Temporary score adder 2
                    DAA                           ; 978C: 19            
                    STA   $4B2B                   ; 978D: B7 4B 2B        // Temporary score adder 2
                    LDA   $01,U                   ; 9790: A6 41         
                    ADCA  $4B2A                   ; 9792: B9 4B 2A        // Temporary score adder 1
                    DAA                           ; 9795: 19            
                    STA   $4B2A                   ; 9796: B7 4B 2A        // Temporary score adder 1
                    LDA   ,U                      ; 9799: A6 C4         
                    ADCA  $4B29                   ; 979B: B9 4B 29        // Temporary score adder 1
                    DAA                           ; 979E: 19            
                    STA   $4B29                   ; 979F: B7 4B 29        // Temporary score adder 1
                    DECB                          ; 97A2: 5A            
                    BNE   L9787                   ; 97A3: 26 E2         
                    LDU   #$4B29                  ; 97A5: CE 4B 29        // Temporary score adder 1
                    JSR   L9810                   ; 97A8: BD 98 10        // Add to score total
; End of function sub_9775
locret_97AB         RTS                           ; 97AB: 39            

; =============== S U B R O U T I N E =======================================
; Used the force score
L97AC               LDB   $4B15                   ; 97AC: F6 4B 15      
                    CMPB  #$05                    ; 97AF: C1 05         
                    BCS   L97B8                   ; 97B1: 25 05         
                    LDU   #byte_9847              ; 97B3: CE 98 47        // Using Force score value
                    BRA   locret_97C1             ; 97B6: 20 09         

; ---------------------------------------------------------------------------
L97B8               ASLB                          ; 97B8: 58            
                    ADDB  $4B15                   ; 97B9: FB 4B 15      
                    LDU   #byte_983B              ; 97BC: CE 98 3B      
                    LEAU  B,U                     ; 97BF: 33 C5         
; End of function sub_97AC
locret_97C1         RTS                           ; 97C1: 39            

; =============== S U B R O U T I N E =======================================
L97C2               LDD   #$A01A                  ; 97C2: CC A0 1A      
                    STD   ,Y++                    ; 97C5: ED A1         
                    LDD   #$0180                  ; 97C7: CC 01 80      
                    STD   ,Y++                    ; 97CA: ED A1         
                    LDD   #$1EC0                  ; 97CC: CC 1E C0      
                    STD   ,Y++                    ; 97CF: ED A1         
                    LDA   #$04                    ; 97D1: 86 04         
                    STA   <$AD                    ; 97D3: 97 AD         
                    JSR   L97AC                   ; 97D5: BD 97 AC        // Used the force score
L97D8               LEAX  -1,U                    ; 97D8: 30 5F         
                    JSR   LE772                   ; 97DA: BD E7 72        // Display BCD number text
L97DD               LDB   #$50                    ; 97DD: C6 50           // 'For using the Force' text
                    JSR   LE7C7                   ; 97DF: BD E7 C7        // Print text string from pointer table
; End of function sub_97C2
L97E2               RTS                           ; 97E2: 39            

; =============== S U B R O U T I N E =======================================
L97E3               JSR   L97AC                   ; 97E3: BD 97 AC        // Used the force score
; End of function sub_97E3
L97E6               BRA   L9810                   ; 97E6: 20 28           // Add to score total

; =============== S U B R O U T I N E =======================================
; Tie fighter score
L97E8               LDU   #byte_984A              ; 97E8: CE 98 4A        // Tie fighter score value
; End of function sub_97E8
                    BRA   L9810                   ; 97EB: 20 23           // Add to score total

; =============== S U B R O U T I N E =======================================
; Vaders tie score
L97ED               LDU   #byte_984D              ; 97ED: CE 98 4D        // Vaders tie score value
; End of function sub_97ED
                    BRA   L9810                   ; 97F0: 20 1E           // Add to score total

; =============== S U B R O U T I N E =======================================
; Trench green squares score
L97F2               LDU   #byte_9850              ; 97F2: CE 98 50        // Trench green squares score value
; End of function sub_97F2
                    BRA   L9810                   ; 97F5: 20 19           // Add to score total

; =============== S U B R O U T I N E =======================================
; Laser tower score
L97F7               LDU   #byte_9859              ; 97F7: CE 98 59        // Laser tower score value
; End of function sub_97F7
                    BRA   L9810                   ; 97FA: 20 14           // Add to score total

; =============== S U B R O U T I N E =======================================
; Trench turrets score
L97FC               LDU   #byte_9853              ; 97FC: CE 98 53        // Trench turrets score value
; End of function sub_97FC
                    BRA   L9810                   ; 97FF: 20 0F           // Add to score total

; =============== S U B R O U T I N E =======================================
; Fireball score
L9801               LDU   #byte_985C              ; 9801: CE 98 5C        // Fireball score value
                    BRA   L9810                   ; 9804: 20 0A           // Add to score total

; ---------------------------------------------------------------------------
L9806               LDU   #byte_985F              ; 9806: CE 98 5F        // Exhaust port score
                    BRA   L9810                   ; 9809: 20 05           // Add to score total

; ---------------------------------------------------------------------------
                    FCB   $CE                     ; 980B:  ' ' CE 98 62       LDU   #$9862 ; Cleared all towers score value
                    FCB   $98                     ; 980C:  ' ' 98 62          EORA  <$62 ; 
                    FCB   $62                     ; 980D:  'b' 62             Invalid ; 
                    FCB   $20                     ; 980E:  ' ' 20 00          BRA   $9810 ; 
                    FCB   $00                     ; 980F:  ' ' 00 A6          NEG   <$A6 ; 
L9810               LDA   $02,U                   ; 9810: A6 42           // Add to score total
                    STA   $4B2B                   ; 9812: B7 4B 2B        // Temporary score adder 2
                    ADDA  <$5F                    ; 9815: 9B 5F           // Score
                    DAA                           ; 9817: 19            
                    STA   <$5F                    ; 9818: 97 5F           // Score
                    LDA   $01,U                   ; 981A: A6 41         
                    STA   $4B2A                   ; 981C: B7 4B 2A        // Temporary score adder 1
                    ADCA  <$5E                    ; 981F: 99 5E           // Score thousands
                    DAA                           ; 9821: 19            
                    STA   <$5E                    ; 9822: 97 5E           // Score thousands
                    LDA   ,U                      ; 9824: A6 C4         
                    STA   $4B29                   ; 9826: B7 4B 29        // Temporary score adder 1
                    ADCA  <$5D                    ; 9829: 99 5D           // Score hundred thousands
                    DAA                           ; 982B: 19            
                    STA   <$5D                    ; 982C: 97 5D           // Score hundred thousands
                    LDA   <$5C                    ; 982E: 96 5C           // Score millions
                    ADCA  #$00                    ; 9830: 89 00         
                    DAA                           ; 9832: 19            
                    STA   <$5C                    ; 9833: 97 5C           // Score millions
                    LDA   #$FF                    ; 9835: 86 FF         
                    STA   $4B2C                   ; 9837: B7 4B 2C      
; End of function sub_9801
; ---------------------------------------------------------------------------
                    RTS                           ; 983A: 39            

; =============== S U B R O U T I N E =======================================
L9874               LDA   <$8B                    ; 9874: 96 8B         
                    BNE   locret_987E             ; 9876: 26 06         
                    LDA   #$10                    ; 9878: 86 10         
                    STA   <$62                    ; 987A: 97 62           // Timer for fireball hit?
                    INC   <$8B                    ; 987C: 0C 8B         
; End of function sub_9874
locret_987E         RTS                           ; 987E: 39            

; =============== S U B R O U T I N E =======================================
; Fireball timer
L987F               LDA   <$43                    ; 987F: 96 43           // Game over/insert coins timer
                    ANDA  #$03                    ; 9881: 84 03         
                    STA   <$62                    ; 9883: 97 62           // Timer for fireball hit?
; End of function sub_987F
                    RTS                           ; 9885: 39            

; =============== S U B R O U T I N E =======================================
; Fireball timer 2
L9886               LDB   <$62                    ; 9886: D6 62           // Timer for fireball hit?
                    CMPB  #$08                    ; 9888: C1 08         
                    BLS   L9890                   ; 988A: 23 04           // Fireball timer 3
                    LDB   #$08                    ; 988C: C6 08         
; End of function sub_9886
                    STB   <$62                    ; 988E: D7 62           // Timer for fireball hit?
; =============== S U B R O U T I N E =======================================
; Fireball timer 3
L9890               LDB   <$62                    ; 9890: D6 62           // Timer for fireball hit?
                    BEQ   locret_9897             ; 9892: 27 03         
                    DECB                          ; 9894: 5A            
                    STB   <$62                    ; 9895: D7 62           // Timer for fireball hit?
; End of function sub_9890
locret_9897         RTS                           ; 9897: 39            

; =============== S U B R O U T I N E =======================================
L9898               LDA   <$43                    ; 9898: 96 43           // Game over/insert coins timer
                    ANDA  #$00                    ; 989A: 84 00         
                    BNE   locret_98AF             ; 989C: 26 11         
                    LDX   #$4900                  ; 989E: 8E 49 00        // 3x Tie fighter data structure ($19 bytes per Tie)
L98A1               DEC   $06,X                   ; 98A1: 6A 06         
                    BPL   L98A7                   ; 98A3: 2A 02         
                    CLR   $06,X                   ; 98A5: 6F 06         
L98A7               LEAX  $19,X                   ; 98A7: 30 88 19      
                    CMPX  #$494B                  ; 98AA: 8C 49 4B        // 3x Tie fighter data structure ($19 bytes per Tie)
                    BCS   L98A1                   ; 98AD: 25 F2         
; End of function sub_9898
locret_98AF         RTS                           ; 98AF: 39            

; =============== S U B R O U T I N E =======================================
; Insert vector laser explosion small circle
L98B0               LDB   <$62                    ; 98B0: D6 62           // Timer for fireball hit?
                    ANDB  #$03                    ; 98B2: C4 03         
                    BEQ   locret_98CA             ; 98B4: 27 14         
                    LDD   #$7100                  ; 98B6: CC 71 00      
                    STD   ,Y++                    ; 98B9: ED A1         
                    LDD   #$67FF                  ; 98BB: CC 67 FF      
                    STD   ,Y++                    ; 98BE: ED A1         
                    LDD   #$B9B3                  ; 98C0: CC B9 B3        // Vector JSRL 19B3 Laser explosion small circle
                    STD   ,Y++                    ; 98C3: ED A1         
                    LDD   #$7200                  ; 98C5: CC 72 00      
                    STD   ,Y++                    ; 98C8: ED A1         
; End of function sub_98B0
; ---------------------------------------------------------------------------
locret_98CA         RTS                           ; 98CA: 39            

; =============== S U B R O U T I N E =======================================
LA1CE               LDU   #$49C2                  ; A1CE: CE 49 C2        // Tie fighter data 1
LA1D1               LDA   #$00                    ; A1D1: 86 00         
                    STA   ,U                      ; A1D3: A7 C4         
                    LEAU  $01,U                   ; A1D5: 33 41         
                    CMPU  #$49E2                  ; A1D7: 11 83 49 E2     // Tie fighter data 1
                    BCS   LA1D1                   ; A1DB: 25 F4         
                    LDA   $4B13                   ; A1DD: B6 4B 13      
                    CMPA  #$13                    ; A1E0: 81 13         
                    BCS   LA1EF                   ; A1E2: 25 0B         
                    LDB   #$06                    ; A1E4: C6 06         
                    LDA   $4703                   ; A1E6: B6 47 03      
                    MUL                           ; A1E9: 3D            
                    ADDA  #$0D                    ; A1EA: 8B 0D         
                    STA   $4B13                   ; A1EC: B7 4B 13      
LA1EF               LDB   $4B13                   ; A1EF: F6 4B 13      
                    LDX   #byte_98CB              ; A1F2: 8E 98 CB      
                    ABX                           ; A1F5: 3A            
                    CMPX  #byte_98DE              ; A1F6: 8C 98 DE      
                    BCS   LA1FE                   ; A1F9: 25 03         
                    LDX   #byte_98DD              ; A1FB: 8E 98 DD      
LA1FE               LDA   ,X                      ; A1FE: A6 84         
                    STA   $4B1A                   ; A200: B7 4B 1A      
                    LDD   word_9856               ; A203: FC 98 56      
                    STD   $4B2E                   ; A206: FD 4B 2E        // Temporary score adder towers 1
                    LDA   byte_9858               ; A209: B6 98 58      
                    STA   $4B30                   ; A20C: B7 4B 30        // Temporary score adder towers 3
                    LDA   #$00                    ; A20F: 86 00         
                    STA   <$A7                    ; A211: 97 A7         
; End of function sub_A1CE
                    RTS                           ; A213: 39            

; =============== S U B R O U T I N E =======================================
LA214               LDB   #$07                    ; A214: C6 07         
                    STB   <$DC                    ; A216: D7 DC         
                    JSR   LCD38                   ; A218: BD CD 38        // Trench left side turret calcs
LA21B               LDD   #$00                    ; A21B: CC 00 00      
                    STD   $5040                   ; A21E: FD 50 40        // XT2
                    STD   $5042                   ; A221: FD 50 42        // YT2
                    STD   $5044                   ; A224: FD 50 44        // ZT2
                    JSR   LA40A                   ; A227: BD A4 0A        // Some Towers code
LA22A               LDD   $5098                   ; A22A: FC 50 98      
                    STD   $5040                   ; A22D: FD 50 40        // XT2
                    LDD   $509A                   ; A230: FC 50 9A      
                    STD   $5042                   ; A233: FD 50 42        // YT2
                    LDD   $509C                   ; A236: FC 50 9C      
                    STD   $5044                   ; A239: FD 50 44        // ZT2
                    LDB   $4B13                   ; A23C: F6 4B 13      
                    ASLB                          ; A23F: 58            
                    ASLB                          ; A240: 58            
                    LDX   #off_A182               ; A241: 8E A1 82      
                    ABX                           ; A244: 3A            
                    CMPX  #LA1CE                  ; A245: 8C A1 CE      
                    BCS   LA24D                   ; A248: 25 03         
                    LDX   #off_A1CA               ; A24A: 8E A1 CA      
LA24D               LDX   ,X                      ; A24D: AE 84         
LA24F               STX   <$64                    ; A24F: 9F 64           // Pointer to Tie fighter data
                    LDA   <$A7                    ; A251: 96 A7         
                    CMPA  $05,X                   ; A253: A1 05         
                    LBLT  LA2DA                   ; A255: 10 2D 00 81   
                    LDD   ,X                      ; A259: EC 84         
                    STD   $5E00                   ; A25B: FD 5E 00      
                    LDD   $02,X                   ; A25E: EC 02         
                    STD   $5E02                   ; A260: FD 5E 02      
                    LDD   #$1E00                  ; A263: CC 1E 00      
                    STD   $5E04                   ; A266: FD 5E 04      
                    LDD   #$01C0                  ; A269: CC 01 C0      
                    STD   $4701                   ; A26C: FD 47 01      
                    LDA   #$67                    ; A26F: 86 67         
                    JSR   LCDBA                   ; A271: BD CD BA        // Do math program run
LA274               LDD   $5000                   ; A274: FC 50 00        // Math result X
                    BPL   LA27E                   ; A277: 2A 05         
                    ANDA  #$3F                    ; A279: 84 3F         
                    STD   $5000                   ; A27B: FD 50 00        // Math result X
LA27E               STD   $507C                   ; A27E: FD 50 7C      
                    CMPD  #$0100                  ; A281: 10 83 01 00   
                    BLT   LA2B0                   ; A285: 2D 29         
                    CMPD  #$3C00                  ; A287: 10 83 3C 00   
                    BCC   LA2B0                   ; A28B: 24 23         
                    LDD   $5002                   ; A28D: FC 50 02        // Math result Y
                    BPL   LA296                   ; A290: 2A 04         
                    COMA                          ; A292: 43            
                    NEGB                          ; A293: 50            
                    SBCA  #$FF                    ; A294: 82 FF         
LA296               SUBD  $5000                   ; A296: B3 50 00        // Math result X
                    BCC   LA2B0                   ; A299: 24 15         
                    LDU   #$49C2                  ; A29B: CE 49 C2        // Tie fighter data 1
                    LDB   $06,X                   ; A29E: E6 06         
                    LEAU  B,U                     ; A2A0: 33 C5         
                    LDA   ,U                      ; A2A2: A6 C4         
                    BITA  #$02                    ; A2A4: 85 02         
                    BEQ   LA2AE                   ; A2A6: 27 06         
                    JSR   LA2F8                   ; A2A8: BD A2 F8      
LA2AB               JSR   LA591                   ; A2AB: BD A5 91      
LA2AE               BRA   LA2DA                   ; A2AE: 20 2A         

; ---------------------------------------------------------------------------
LA2B0               LDX   <$64                    ; A2B0: 9E 64           // Pointer to Tie fighter data
                    LDU   #$49C2                  ; A2B2: CE 49 C2        // Tie fighter data 1
                    LDB   $06,X                   ; A2B5: E6 06         
                    LEAU  B,U                     ; A2B7: 33 C5         
                    LDA   $4B3D                   ; A2B9: B6 4B 3D      
                    BEQ   LA2C2                   ; A2BC: 27 04         
                    CLR   ,U                      ; A2BE: 6F C4         
                    BRA   LA2DA                   ; A2C0: 20 18         

; ---------------------------------------------------------------------------
LA2C2               LDA   #$0B                    ; A2C2: 86 0B         
                    LDB   ,U                      ; A2C4: E6 C4         
                    BITB  #$04                    ; A2C6: C5 04         
                    BEQ   LA2D6                   ; A2C8: 27 0C         
                    ORA   #$04                    ; A2CA: 8A 04         
                    LDB   $04,X                   ; A2CC: E6 04         
                    CMPB  #$03                    ; A2CE: C1 03         
                    BNE   LA2D4                   ; A2D0: 26 02         
                    ANDA  #$FD                    ; A2D2: 84 FD         
LA2D4               BRA   LA2D8                   ; A2D4: 20 02         

; ---------------------------------------------------------------------------
LA2D6               ORA   #$10                    ; A2D6: 8A 10         
LA2D8               STA   ,U                      ; A2D8: A7 C4         
LA2DA               LDB   $4B13                   ; A2DA: F6 4B 13      
                    ASLB                          ; A2DD: 58            
                    ASLB                          ; A2DE: 58            
                    LDX   #off_A182               ; A2DF: 8E A1 82      
                    ABX                           ; A2E2: 3A            
                    CMPX  #LA1CE                  ; A2E3: 8C A1 CE      
                    BCS   LA2EB                   ; A2E6: 25 03         
                    LDX   #off_A1CA               ; A2E8: 8E A1 CA      
LA2EB               TFR   X,U                     ; A2EB: 1F 13         
                    LDX   <$64                    ; A2ED: 9E 64           // Pointer to Tie fighter data
                    LEAX  $07,X                   ; A2EF: 30 07         
                    CPX   $02,U                   ; A2F1: AC 42         
                    LBCS  LA24F                   ; A2F3: 10 25 FF 58   
; End of function sub_A214
                    RTS                           ; A2F7: 39            

; =============== S U B R O U T I N E =======================================
LA2F8               LDX   <$64                    ; A2F8: 9E 64           // Pointer to Tie fighter data
                    LDB   $04,X                   ; A2FA: E6 04         
                    CMPB  #$03                    ; A2FC: C1 03         
                    BNE   LA304                   ; A2FE: 26 04         
                    LDB   #$09                    ; A300: C6 09         
                    BRA   LA306                   ; A302: 20 02         

; ---------------------------------------------------------------------------
LA304               LDB   #$08                    ; A304: C6 08         
LA306               STB   <$DC                    ; A306: D7 DC         
                    LDD   #$6680                  ; A308: CC 66 80      
                    STD   $5E02                   ; A30B: FD 5E 02      
                    LDU   #$49C2                  ; A30E: CE 49 C2        // Tie fighter data 1
                    LDB   $06,X                   ; A311: E6 06         
                    LEAU  B,U                     ; A313: 33 C5         
                    LDA   ,U                      ; A315: A6 C4         
                    LDB   $04,X                   ; A317: E6 04         
                    CMPB  #$03                    ; A319: C1 03         
                    BEQ   LA333                   ; A31B: 27 16         
                    BITA  #$04                    ; A31D: 85 04         
                    BNE   LA325                   ; A31F: 26 04         
                    BITA  #$10                    ; A321: 85 10         
                    BNE   LA32E                   ; A323: 26 09         
LA325               LDB   #$0A                    ; A325: C6 0A         
                    STB   <$DC                    ; A327: D7 DC         
                    LDD   #$6080                  ; A329: CC 60 80      
                    BRA   LA331                   ; A32C: 20 03         

; ---------------------------------------------------------------------------
LA32E               LDD   #$6780                  ; A32E: CC 67 80      
LA331               BRA   LA33F                   ; A331: 20 0C         

; ---------------------------------------------------------------------------
LA333               BITA  #$04                    ; A333: 85 04         
                    BNE   LA33C                   ; A335: 26 05         
                    LDD   #$6460                  ; A337: CC 64 60      
                    BRA   LA33F                   ; A33A: 20 03         

; ---------------------------------------------------------------------------
LA33C               LDD   #$6080                  ; A33C: CC 60 80      
LA33F               STD   $5E00                   ; A33F: FD 5E 00      
                    LDA   #$72                    ; A342: 86 72         
                    STA   $5E04                   ; A344: B7 5E 04      
                    LDA   $5000                   ; A347: B6 50 00        // Math result X
                    ASLA                          ; A34A: 48            
                    ASLA                          ; A34B: 48            
                    STA   $5E05                   ; A34C: B7 5E 05      
                    LDB   #$40                    ; A34F: C6 40         
                    COMA                          ; A351: 43            
                    MUL                           ; A352: 3D            
                    ADDA  #$40                    ; A353: 8B 40         
                    STA   $5E03                   ; A355: B7 5E 03      
                    LDD   $5086                   ; A358: FC 50 86      
                    ASLB                          ; A35B: 58            
                    ROLA                          ; A35C: 49            
                    ADDD  #$0400                  ; A35D: C3 04 00      
                    SUBD  $5000                   ; A360: B3 50 00        // Math result X
                    LBLT  LA3F4                   ; A363: 10 2D 00 8D   
                    LDA   $04,X                   ; A367: A6 04         
                    CMPA  #$03                    ; A369: 81 03         
                    BEQ   LA3A7                   ; A36B: 27 3A         
                    LDA   <$60                    ; A36D: 96 60           // Shield count
                    BLT   LA374                   ; A36F: 2D 03         
                    JSR   Sound_2C                ; A371: BD BD D0      
LA374               LDD   $5000                   ; A374: FC 50 00        // Math result X
                    SUBD  #$0200                  ; A377: 83 02 00      
                    SUBD  $5086                   ; A37A: B3 50 86      
                    BGT   LA3A5                   ; A37D: 2E 26         
                    LDD   #$A018                  ; A37F: CC A0 18      
                    STD   $5E02                   ; A382: FD 5E 02      
                    LDD   #$6080                  ; A385: CC 60 80      
                    STD   $5E00                   ; A388: FD 5E 00      
                    LDA   <$60                    ; A38B: 96 60           // Shield count
                    BLT   LA395                   ; A38D: 2D 06         
                    JSR   L9874                   ; A38F: BD 98 74      
LA392               JSR   Sound_26                ; A392: BD BD B2        // Explosion
LA395               LDA   <$63                    ; A395: 96 63         
                    BNE   LA39D                   ; A397: 26 04         
                    LDA   $5002                   ; A399: B6 50 02        // Math result Y
                    NEGA                          ; A39C: 40            
LA39D               LDB   #$20                    ; A39D: C6 20         
                    TSTA                          ; A39F: 4D            
                    BPL   LA3A3                   ; A3A0: 2A 01         
                    NEGB                          ; A3A2: 50            
LA3A3               STB   <$63                    ; A3A3: D7 63         
LA3A5               BRA   LA3F4                   ; A3A5: 20 4D         

; ---------------------------------------------------------------------------
LA3A7               LDD   $5044                   ; A3A7: FC 50 44        // ZT2
                    SUBD  #$07A0                  ; A3AA: 83 07 A0      
                    BGT   LA3F4                   ; A3AD: 2E 45         
                    JSR   Sound_2C                ; A3AF: BD BD D0      
LA3B2               LDX   <$64                    ; A3B2: 9E 64           // Pointer to Tie fighter data
                    LDU   #$49C2                  ; A3B4: CE 49 C2        // Tie fighter data 1
                    LDB   $06,X                   ; A3B7: E6 06         
                    LEAU  B,U                     ; A3B9: 33 C5         
                    LDA   ,U                      ; A3BB: A6 C4         
                    BITA  #$04                    ; A3BD: 85 04         
                    BNE   LA3F4                   ; A3BF: 26 33         
                    LDD   $5044                   ; A3C1: FC 50 44        // ZT2
                    SUBD  #$05A0                  ; A3C4: 83 05 A0      
                    BGE   LA3F4                   ; A3C7: 2C 2B         
                    LDD   $5000                   ; A3C9: FC 50 00        // Math result X
                    SUBD  #$0400                  ; A3CC: 83 04 00      
                    SUBD  $5086                   ; A3CF: B3 50 86      
                    BGT   LA3F4                   ; A3D2: 2E 20         
                    LDD   #$A018                  ; A3D4: CC A0 18      
                    STD   $5E00                   ; A3D7: FD 5E 00      
                    LDA   <$60                    ; A3DA: 96 60           // Shield count
                    BLT   LA3E4                   ; A3DC: 2D 06         
                    JSR   L9874                   ; A3DE: BD 98 74      
LA3E1               JSR   Sound_26                ; A3E1: BD BD B2        // Explosion
LA3E4               LDA   <$63                    ; A3E4: 96 63         
                    BNE   LA3F4                   ; A3E6: 26 0C         
                    LDA   #$13                    ; A3E8: 86 13         
                    LDB   $5002                   ; A3EA: F6 50 02        // Math result Y
                    BMI   LA3F0                   ; A3ED: 2B 01         
                    NEGA                          ; A3EF: 40            
LA3F0               ADDA  <$63                    ; A3F0: 9B 63         
                    STA   <$63                    ; A3F2: 97 63         
LA3F4               LDA   $5000                   ; A3F4: B6 50 00        // Math result X
                    SUBA  #$08                    ; A3F7: 80 08         
                    BGT   LA400                   ; A3F9: 2E 05         
                    JSR   LCD68                   ; A3FB: BD CD 68      
LA3FE               BRA   LA403                   ; A3FE: 20 03         

; ---------------------------------------------------------------------------
LA400               JSR   LCD50                   ; A400: BD CD 50      
LA403               JSR   LCD74                   ; A403: BD CD 74        // Function select for an object
LA406               JSR   LA459                   ; A406: BD A4 59        // Some Towers code
; End of function sub_A2F8
LA409               RTS                           ; A409: 39            

; =============== S U B R O U T I N E =======================================
; Some Towers code
LA40A               JSR   LCDE7                   ; A40A: BD CD E7        // Swap Matrix 2  x, y, z to  x, y, z
LA40D               LDD   #$00                    ; A40D: CC 00 00      
                    STD   $5078                   ; A410: FD 50 78      
                    LDD   <$B3                    ; A413: DC B3         
                    STD   $507A                   ; A415: FD 50 7A      
                    LDD   <$B5                    ; A418: DC B5         
                    STD   $507C                   ; A41A: FD 50 7C      
                    LDD   #$0F                    ; A41D: CC 00 0F      
                    STD   $4701                   ; A420: FD 47 01        // Point BIC to $5078 MReg3C
                    LDA   #$2A                    ; A423: 86 2A           // Reg00 = (([BIC,0] - XT) x Ax2)  +  (([BIC,1] - YT) x Bx2)  +  (([BIC,2] - ZT) x Cx2), Reg01 = (([BIC,0] - XT) x Ay2)  +  (([BIC,1] - YT) x By2)  +  (([BIC,2] - ZT) x Cy2), Reg02 = (([BIC,0] - XT) x Az2)  +  (([BIC,1] - YT) x Bz2)  +  (([BIC,2] - ZT) x Cz2), BIC++
                    JSR   LCDBA                   ; A425: BD CD BA        // Do math program run
LA428               LDD   $5002                   ; A428: FC 50 02        // Math result Y
                    STD   $5078                   ; A42B: FD 50 78      
                    LDD   $5004                   ; A42E: FC 50 04        // Math result Z
                    STD   $507A                   ; A431: FD 50 7A      
                    BPL   LA455                   ; A434: 2A 1F         
                    LDA   <$BC                    ; A436: 96 BC         
                    BEQ   LA455                   ; A438: 27 1B         
                    LDA   #$FF                    ; A43A: 86 FF         
                    STA   <$BD                    ; A43C: 97 BD         
                    LDA   #$03                    ; A43E: 86 03         
                    STA   $4B23                   ; A440: B7 4B 23      
                    LDD   #$6280                  ; A443: CC 62 80      
                    STD   $4B20                   ; A446: FD 4B 20      
                    LDD   $5004                   ; A449: FC 50 04        // Math result Z
                    LSRA                          ; A44C: 44            
                    RORB                          ; A44D: 56            
                    LSRA                          ; A44E: 44            
                    RORB                          ; A44F: 56            
                    ADDB  #$C0                    ; A450: CB C0         
                    STB   $4B22                   ; A452: F7 4B 22      
LA455               JSR   LCDE7                   ; A455: BD CD E7        // Called during towers phase only
; End of function sub_A40A
LA458               RTS                           ; A458: 39            

; =============== S U B R O U T I N E =======================================
; Some Towers code
LA459               LDX   <$64                    ; A459: 9E 64           // Pointer to Tie fighter data
                    LDD   $507C                   ; A45B: FC 50 7C      
                    ASLB                          ; A45E: 58            
                    ROLA                          ; A45F: 49            
                    STD   $4704                   ; A460: FD 47 04      
                    LDD   $02,X                   ; A463: EC 02         
                    SUBD  $5042                   ; A465: B3 50 42        // YT2
                    STD   $5002                   ; A468: FD 50 02        // Math result Y
                    LDD   #$00                    ; A46B: CC 00 00      
                    SUBD  $5044                   ; A46E: B3 50 44        // ZT2
                    STD   $5004                   ; A471: FD 50 04        // Math result Z
                    LDD   $4700                   ; A474: FC 47 00      
                    STD   $5000                   ; A477: FD 50 00        // Math result X
                    LDA   #$86                    ; A47A: 86 86           // MReg02 = MReg02 x MReg00
                    JSR   LCDBA                   ; A47C: BD CD BA        // Do math program run
LA47F               LDD   $5002                   ; A47F: FC 50 02        // Math result Y
                    STD   $501A                   ; A482: FD 50 1A        // YT
                    LDD   $5004                   ; A485: FC 50 04        // Math result Z
                    STD   $501C                   ; A488: FD 50 1C        // ZT
                    LDA   $04,X                   ; A48B: A6 04         
                    CMPA  #$03                    ; A48D: 81 03         
                    BEQ   LA49F                   ; A48F: 27 0E         
                    LDD   #$03C0                  ; A491: CC 03 C0      
                    STD   $5002                   ; A494: FD 50 02        // Math result Y
                    LDD   #$5280                  ; A497: CC 52 80      
                    STD   $5004                   ; A49A: FD 50 04        // Math result Z
                    BRA   LA4BA                   ; A49D: 20 1B         

; ---------------------------------------------------------------------------
LA49F               LDU   #$49C2                  ; A49F: CE 49 C2        // Tie fighter data 1
                    LDB   $06,X                   ; A4A2: E6 06         
                    LEAU  B,U                     ; A4A4: 33 C5         
                    LDA   ,U                      ; A4A6: A6 C4         
                    BITA  #$04                    ; A4A8: 85 04         
                    LBNE  locret_A54A             ; A4AA: 10 26 00 9C   
                    LDD   #$0690                  ; A4AE: CC 06 90      
                    STD   $5002                   ; A4B1: FD 50 02        // Math result Y
                    LDD   #$05A0                  ; A4B4: CC 05 A0      
                    STD   $5004                   ; A4B7: FD 50 04        // Math result Z
LA4BA               LDD   $5000                   ; A4BA: FC 50 00        // Math result X
                    SUBD  #$0100                  ; A4BD: 83 01 00      
                    BPL   LA4C5                   ; A4C0: 2A 03         
                    LDD   #$00                    ; A4C2: CC 00 00      
LA4C5               STD   $5000                   ; A4C5: FD 50 00        // Math result X
                    LDA   #$86                    ; A4C8: 86 86           // MReg02 = MReg02 x MReg00
                    JSR   LCDBA                   ; A4CA: BD CD BA        // Do math program run
LA4CD               LDD   $501A                   ; A4CD: FC 50 1A        // YT
                    ADDD  $5002                   ; A4D0: F3 50 02        // Math result Y
                    ADDD  #$0A                    ; A4D3: C3 00 0A      
                    SUBD  $5078                   ; A4D6: B3 50 78      
                    LBLT  locret_A54A             ; A4D9: 10 2D 00 6D   
                    ASRA                          ; A4DD: 47            
                    RORB                          ; A4DE: 56            
                    SUBD  #$0A                    ; A4DF: 83 00 0A      
                    SUBD  $5002                   ; A4E2: B3 50 02        // Math result Y
                    LBGT  locret_A54A             ; A4E5: 10 2E 00 61   
                    LDD   $507A                   ; A4E9: FC 50 7A      
                    SUBD  $501C                   ; A4EC: B3 50 1C        // ZT
                    BMI   locret_A54A             ; A4EF: 2B 59         
                    SUBD  $5004                   ; A4F1: B3 50 04        // Math result Z
                    BGT   locret_A54A             ; A4F4: 2E 54         
                    LDA   $04,X                   ; A4F6: A6 04         
                    CMPA  #$03                    ; A4F8: 81 03         
                    BNE   LA50A                   ; A4FA: 26 0E         
                    LDD   $507C                   ; A4FC: FC 50 7C      
                    CMPD  <$C8                    ; A4FF: 10 93 C8      
                    BCC   LA508                   ; A502: 24 04         
                    STD   <$C8                    ; A504: DD C8         
                    STX   <$C6                    ; A506: 9F C6         
LA508               BRA   locret_A54A             ; A508: 20 40         

; ---------------------------------------------------------------------------
LA50A               LDD   #$4CE0                  ; A50A: CC 4C E0      
                    STD   $5002                   ; A50D: FD 50 02        // Math result Y
                    LDA   #$86                    ; A510: 86 86           // MReg02 = MReg02 x MReg00
                    JSR   LCDBA                   ; A512: BD CD BA        // Do math program run
LA515               LDD   $507A                   ; A515: FC 50 7A      
                    ADDD  #$0A                    ; A518: C3 00 0A      
                    SUBD  $501C                   ; A51B: B3 50 1C        // ZT
                    SUBD  $5002                   ; A51E: B3 50 02        // Math result Y
                    BLT   LA53E                   ; A521: 2D 1B         
                    LDU   #$49C2                  ; A523: CE 49 C2        // Tie fighter data 1
                    LDB   $06,X                   ; A526: E6 06         
                    LEAU  B,U                     ; A528: 33 C5         
                    LDA   ,U                      ; A52A: A6 C4         
                    BITA  #$04                    ; A52C: 85 04         
                    BNE   locret_A54A             ; A52E: 26 1A         
                    LDD   $507C                   ; A530: FC 50 7C      
                    CMPD  <$C8                    ; A533: 10 93 C8      
                    BCC   LA53C                   ; A536: 24 04         
                    STD   <$C8                    ; A538: DD C8         
                    STX   <$C6                    ; A53A: 9F C6         
LA53C               BRA   locret_A54A             ; A53C: 20 0C         

; ---------------------------------------------------------------------------
LA53E               LDD   $507C                   ; A53E: FC 50 7C      
                    CMPD  <$D0                    ; A541: 10 93 D0      
                    BCC   locret_A54A             ; A544: 24 04         
                    STD   <$D0                    ; A546: DD D0         
                    STX   <$CE                    ; A548: 9F CE         
; End of function sub_A459
locret_A54A         RTS                           ; A54A: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_B2D2
LA54B               LDX   <$C6                    ; A54B: 9E C6         
                    LDD   ,X                      ; A54D: EC 84         
                    SUBD  $5098                   ; A54F: B3 50 98      
                    ANDA  #$7F                    ; A552: 84 7F         
                    ADDD  $5098                   ; A554: F3 50 98      
                    STD   $5018                   ; A557: FD 50 18        // XT
                    LDD   $02,X                   ; A55A: EC 02         
                    STD   $501A                   ; A55C: FD 50 1A        // YT
                    LDU   #$49C2                  ; A55F: CE 49 C2        // Tie fighter data 1
                    LDB   $06,X                   ; A562: E6 06         
                    LEAU  B,U                     ; A564: 33 C5         
                    LDA   ,U                      ; A566: A6 C4         
                    ORA   #$04                    ; A568: 8A 04         
                    STA   ,U                      ; A56A: A7 C4         
                    LDA   $04,X                   ; A56C: A6 04         
                    CMPA  #$03                    ; A56E: 81 03         
                    BNE   LA580                   ; A570: 26 0E         
                    LDD   #$02D0                  ; A572: CC 02 D0      
                    STD   $501C                   ; A575: FD 50 1C        // ZT
                    JSR   LB852                   ; A578: BD B8 52      
LA57B               JSR   L97F7                   ; A57B: BD 97 F7        // Laser tower score
LA57E               BRA   LA58C                   ; A57E: 20 0C         

; ---------------------------------------------------------------------------
LA580               LDD   #$5460                  ; A580: CC 54 60      
                    STD   $501C                   ; A583: FD 50 1C        // ZT
                    JSR   LB85E                   ; A586: BD B8 5E      
LA589               JSR   L973A                   ; A589: BD 97 3A        // Towers incrementing score
LA58C               JSR   Sound_35                ; A58C: BD BD FD      
; END OF FUNCTION CHUNK FOR sub_B2D2
; ---------------------------------------------------------------------------
LA58F               RTS                           ; A58F: 39            

; =============== S U B R O U T I N E =======================================
; FUNCTION CHUNK AT ROM:A728 SIZE 0000008E BYTES
LA591               LDA   <$60                    ; A591: 96 60           // Shield count
                    BLT   locret_A5B2             ; A593: 2D 1D         
                    LDX   <$64                    ; A595: 9E 64           // Pointer to Tie fighter data
                    LDU   #$49C2                  ; A597: CE 49 C2        // Tie fighter data 1
                    LDB   $06,X                   ; A59A: E6 06         
                    LEAU  B,U                     ; A59C: 33 C5         
                    LDA   ,U                      ; A59E: A6 C4         
                    BITA  #$04                    ; A5A0: 85 04         
                    BNE   locret_A5B2             ; A5A2: 26 0E         
                    LDA   $04,X                   ; A5A4: A6 04         
                    CMPA  #$02                    ; A5A6: 81 02         
                    LBEQ  LA608                   ; A5A8: 10 27 00 5C   
                    LBHI  LA655                   ; A5AC: 10 22 00 A5   
                    BRA   LA5B3                   ; A5B0: 20 01         

; ---------------------------------------------------------------------------
locret_A5B2         RTS                           ; A5B2: 39            

; ---------------------------------------------------------------------------
LA5B3               LDD   #$4000                  ; A5B3: CC 40 00      
                    SUBD  $507C                   ; A5B6: B3 50 7C      
                    ASLB                          ; A5B9: 58            
                    ROLA                          ; A5BA: 49            
                    STD   $5000                   ; A5BB: FD 50 00        // Math result X
                    LDD   #$2940                  ; A5BE: CC 29 40      
                    STD   $5004                   ; A5C1: FD 50 04        // Math result Z
                    LDD   #$02D0                  ; A5C4: CC 02 D0      
                    STD   $5002                   ; A5C7: FD 50 02        // Math result Y
                    LDA   #$86                    ; A5CA: 86 86           // MReg02 = MReg02 x MReg00
                    JSR   LCDBA                   ; A5CC: BD CD BA        // Do math program run
LA5CF               LDD   $5004                   ; A5CF: FC 50 04        // Math result Z
                    SUBD  $5044                   ; A5D2: B3 50 44        // ZT2
                    BGE   LA5DE                   ; A5D5: 2C 07         
                    LDA   ,U                      ; A5D7: A6 C4         
                    ORA   #$08                    ; A5D9: 8A 08         
                    STA   ,U                      ; A5DB: A7 C4         
                    RTS                           ; A5DD: 39            

; ---------------------------------------------------------------------------
LA5DE               SUBD  $5002                   ; A5DE: B3 50 02        // Math result Y
                    BLE   LA5E9                   ; A5E1: 2F 06         
                    LDA   ,U                      ; A5E3: A6 C4         
                    BITA  #$08                    ; A5E5: 85 08         
                    BEQ   locret_A607             ; A5E7: 27 1E         
LA5E9               LDA   ,U                      ; A5E9: A6 C4         
                    ANDA  #$F7                    ; A5EB: 84 F7         
                    STA   ,U                      ; A5ED: A7 C4         
                    LDA   $4703                   ; A5EF: B6 47 03      
                    BPL   LA5F7                   ; A5F2: 2A 03         
                    JSR   LA7B6                   ; A5F4: BD A7 B6        // Emit fireballs from towers/bunkers
LA5F7               LDA   $4703                   ; A5F7: B6 47 03      
                    BPL   LA5FF                   ; A5FA: 2A 03         
                    JSR   LA7C8                   ; A5FC: BD A7 C8        // Emit fireballs from towers/bunkers 2
LA5FF               LDA   $4703                   ; A5FF: B6 47 03      
                    BPL   locret_A607             ; A602: 2A 03         
                    JSR   LA7BF                   ; A604: BD A7 BF      
locret_A607         RTS                           ; A607: 39            

; ---------------------------------------------------------------------------
LA608               LDD   #$4000                  ; A608: CC 40 00      
                    SUBD  $507C                   ; A60B: B3 50 7C      
                    ASLB                          ; A60E: 58            
                    ROLA                          ; A60F: 49            
                    STD   $5000                   ; A610: FD 50 00        // Math result X
                    LDD   #$2940                  ; A613: CC 29 40      
                    STD   $5004                   ; A616: FD 50 04        // Math result Z
                    LDD   #$02D0                  ; A619: CC 02 D0      
                    STD   $5002                   ; A61C: FD 50 02        // Math result Y
                    LDA   #$86                    ; A61F: 86 86           // MReg02 = MReg02 x MReg00
                    JSR   LCDBA                   ; A621: BD CD BA        // Do math program run
LA624               LDD   $5004                   ; A624: FC 50 04        // Math result Z
                    SUBD  $5044                   ; A627: B3 50 44        // ZT2
                    BGE   LA633                   ; A62A: 2C 07         
                    LDA   ,U                      ; A62C: A6 C4         
                    ORA   #$08                    ; A62E: 8A 08         
                    STA   ,U                      ; A630: A7 C4         
                    RTS                           ; A632: 39            

; ---------------------------------------------------------------------------
LA633               SUBD  $5002                   ; A633: B3 50 02        // Math result Y
                    BLE   LA63E                   ; A636: 2F 06         
                    LDA   ,U                      ; A638: A6 C4         
                    BITA  #$08                    ; A63A: 85 08         
                    BEQ   locret_A654             ; A63C: 27 16         
LA63E               LDA   ,U                      ; A63E: A6 C4         
                    ANDA  #$F7                    ; A640: 84 F7         
                    STA   ,U                      ; A642: A7 C4         
                    LDA   $4703                   ; A644: B6 47 03      
                    BPL   LA64C                   ; A647: 2A 03         
                    JSR   LA7C8                   ; A649: BD A7 C8        // Emit fireballs from towers/bunkers 2
LA64C               LDA   $4703                   ; A64C: B6 47 03      
                    BPL   locret_A654             ; A64F: 2A 03         
                    JSR   LA7BF                   ; A651: BD A7 BF      
locret_A654         RTS                           ; A654: 39            

; ---------------------------------------------------------------------------
LA655               LDA   #$40                    ; A655: 86 40         
                    SUBA  $507C                   ; A657: B0 50 7C      
                    CMPA  $4703                   ; A65A: B1 47 03      
                    BCS   locret_A674             ; A65D: 25 15         
                    LDA   $4703                   ; A65F: B6 47 03      
                    CMPA  #$50                    ; A662: 81 50         
                    BCC   LA669                   ; A664: 24 03         
                    JMP   LA728                   ; A666: 7E A7 28      

; ---------------------------------------------------------------------------
LA669               LDA   $4703                   ; A669: B6 47 03      
                    BPL   LA671                   ; A66C: 2A 03         
                    JMP   LA728                   ; A66E: 7E A7 28      

; ---------------------------------------------------------------------------
LA671               JMP   LA728                   ; A671: 7E A7 28      

; ---------------------------------------------------------------------------
; End of function sub_A591
; ---------------------------------------------------------------------------
locret_A674         RTS                           ; A674: 39            

; =============== S U B R O U T I N E =======================================
; Emit fireballs from tie fighters
LA68B               STU   <$A8                    ; A68B: DF A8           // Fireball data pointer
                    LDB   #$40                    ; A68D: C6 40         
                    STB   $05,U                   ; A68F: E7 45         
                    LDB   #$01                    ; A691: C6 01         
                    STB   $03,U                   ; A693: E7 43         
                    LDB   #$01                    ; A695: C6 01         
                    STB   $04,U                   ; A697: E7 44         
                    LDA   $4B3C                   ; A699: B6 4B 3C      
                    BLE   LA6AB                   ; A69C: 2F 0D         
                    CPX   $4B32                   ; A69E: BC 4B 32      
                    BNE   LA6AB                   ; A6A1: 26 08         
                    DEC   $4B3C                   ; A6A3: 7A 4B 3C      
                    BNE   LA6AB                   ; A6A6: 26 03         
                    JSR   Sound_12                ; A6A8: BD BD 4E      
LA6AB               LDU   ,U                      ; A6AB: EE C4         
                    LDX   ,X                      ; A6AD: AE 84         
                    LDD   $08,X                   ; A6AF: EC 08         
                    SUBD  $5098                   ; A6B1: B3 50 98      
                    STD   ,U                      ; A6B4: ED C4         
                    LDD   $0A,X                   ; A6B6: EC 0A         
                    SUBD  $509A                   ; A6B8: B3 50 9A      
                    STD   $02,U                   ; A6BB: ED 42         
                    LDD   $0C,X                   ; A6BD: EC 0C         
                    SUBD  $509C                   ; A6BF: B3 50 9C      
                    STD   $04,U                   ; A6C2: ED 44         
                    JSR   Sound_36                ; A6C4: BD BE 02      
; End of function sub_A68B
; ---------------------------------------------------------------------------
LA6C7               RTS                           ; A6C7: 39            

; =============== S U B R O U T I N E =======================================
; Emit fireballs from towers 4
LA6E0               LDB   $4B19                   ; A6E0: F6 4B 19      
                    CMPB  #$0C                    ; A6E3: C1 0C         
                    BCS   LA6EC                   ; A6E5: 25 05         
                    LDX   #off_A6DE               ; A6E7: 8E A6 DE      
                    BRA   LA6F2                   ; A6EA: 20 06         

; ---------------------------------------------------------------------------
LA6EC               ASLB                          ; A6EC: 58            
                    LDX   #off_A6C8               ; A6ED: 8E A6 C8      
                    LDX   B,X                     ; A6F0: AE 85         
; End of function sub_A6E0
LA6F2               BRA   LA708                   ; A6F2: 20 14         

; =============== S U B R O U T I N E =======================================
LA6F4               LDB   $4B19                   ; A6F4: F6 4B 19      
                    CMPB  #$07                    ; A6F7: C1 07         
                    BLS   LA6FD                   ; A6F9: 23 02         
                    LDB   #$07                    ; A6FB: C6 07         
LA6FD               ASLB                          ; A6FD: 58            
                    LDX   #off_A718               ; A6FE: 8E A7 18      
                    LDX   B,X                     ; A701: AE 85         
; End of function sub_A6F4
                    BRA   LA708                   ; A703: 20 03         

; =============== S U B R O U T I N E =======================================
; Emit fireballs from towers 3
LA705               LDX   #$494B                  ; A705: 8E 49 4B        // 6x Fireball data structure 2 ($6 bytes per fireball)
LA708               STX   <$A8                    ; A708: 9F A8           // Fireball data pointer
                    LDA   $03,X                   ; A70A: A6 03         
                    BEQ   locret_A717             ; A70C: 27 09         
                    LEAX  $06,X                   ; A70E: 30 06         
                    CMPX  #$496F                  ; A710: 8C 49 6F        // 6x Fireball data structure 2 ($6 bytes per fireball)
                    BCS   LA708                   ; A713: 25 F3         
                    LEAS  $02,S                   ; A715: 32 62           // Do not return to last function call ?
; End of function sub_A705
; ---------------------------------------------------------------------------
locret_A717         RTS                           ; A717: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_A591
LA728               JSR   LA6E0                   ; A728: BD A6 E0        // Emit fireballs from towers 4
LA72B               LDA   #$70                    ; A72B: 86 70         
                    STA   $05,X                   ; A72D: A7 05         
                    LDA   #$05                    ; A72F: 86 05         
                    STA   $03,X                   ; A731: A7 03         
                    LDU   <$64                    ; A733: DE 64           // Pointer to Tie fighter data
                    STX   <$01                    ; A735: 9F 01         
                    LDD   $02,U                   ; A737: EC 42         
                    SUBD  $509A                   ; A739: B3 50 9A      
                    BMI   LA742                   ; A73C: 2B 04         
                    LDA   #$08                    ; A73E: 86 08         
                    BRA   LA744                   ; A740: 20 02         

; ---------------------------------------------------------------------------
LA742               LDA   #$07                    ; A742: 86 07         
LA744               STA   $04,X                   ; A744: A7 04         
                    LDX   ,X                      ; A746: AE 84         
                    LDA   ,U                      ; A748: A6 C4         
                    SUBA  $5098                   ; A74A: B0 50 98      
                    ANDA  #$80                    ; A74D: 84 80         
                    EORA  ,U                      ; A74F: A8 C4         
                    LDB   $01,U                   ; A751: E6 41         
                    STD   ,X                      ; A753: ED 84         
                    LDD   $02,U                   ; A755: EC 42         
                    STD   $02,X                   ; A757: ED 02         
                    LDD   #$0200                  ; A759: CC 02 00      
                    STD   $04,X                   ; A75C: ED 04         
                    LDD   $02,X                   ; A75E: EC 02         
                    SUBD  $509A                   ; A760: B3 50 9A      
                    STA   $06,X                   ; A763: A7 06         
                    BPL   LA76B                   ; A765: 2A 04         
                    COMA                          ; A767: 43            
                    NEGB                          ; A768: 50            
                    SBCA  #$FF                    ; A769: 82 FF         
LA76B               STD   $4706                   ; A76B: FD 47 06      
                    LDD   ,X                      ; A76E: EC 84         
                    SUBD  $5098                   ; A770: B3 50 98      
                    STD   $4704                   ; A773: FD 47 04      
                    LDD   #$0200                  ; A776: CC 02 00      
                    STD   $4706                   ; A779: FD 47 06      
                    LDA   $4700                   ; A77C: B6 47 00      
                    ASLA                          ; A77F: 48            
                    ASLA                          ; A780: 48            
                    BCC   LA784                   ; A781: 24 01         
                    CLRA                          ; A783: 4F            
LA784               STA   $07,X                   ; A784: A7 07         
                    LDA   <$7D                    ; A786: 96 7D           // Joystick X
                    EORA  $06,X                   ; A788: A8 06         
                    BPL   LA790                   ; A78A: 2A 04         
                    CLR   $06,X                   ; A78C: 6F 06         
                    BRA   LA7B2                   ; A78E: 20 22         

; ---------------------------------------------------------------------------
LA790               LDA   <$7D                    ; A790: 96 7D           // Joystick X
                    BPL   LA795                   ; A792: 2A 01         
                    NEGA                          ; A794: 40            
LA795               ASLA                          ; A795: 48            
                    CMPA  $07,X                   ; A796: A1 07         
                    BHI   LA79E                   ; A798: 22 04         
                    CLR   $06,X                   ; A79A: 6F 06         
                    BRA   LA7B2                   ; A79C: 20 14         

; ---------------------------------------------------------------------------
LA79E               LDA   #$FF                    ; A79E: 86 FF         
                    STA   $06,X                   ; A7A0: A7 06         
                    LDX   <$01                    ; A7A2: 9E 01         
                    LDA   $04,X                   ; A7A4: A6 04         
                    CMPA  #$08                    ; A7A6: 81 08         
                    BNE   LA7AE                   ; A7A8: 26 04         
                    LDA   #$07                    ; A7AA: 86 07         
                    BRA   LA7B0                   ; A7AC: 20 02         

; ---------------------------------------------------------------------------
LA7AE               LDA   #$08                    ; A7AE: 86 08         
LA7B0               STA   $04,X                   ; A7B0: A7 04         
LA7B2               JSR   Sound_39                ; A7B2: BD BE 11      
; END OF FUNCTION CHUNK FOR sub_A591
LA7B5               RTS                           ; A7B5: 39            

; =============== S U B R O U T I N E =======================================
; Emit fireballs from towers/bunkers
; FUNCTION CHUNK AT ROM:A7D1 SIZE 0000000E BYTES
LA7B6               JSR   LA705                   ; A7B6: BD A7 05        // Emit fireballs from towers 3
LA7B9               LDA   #$03                    ; A7B9: 86 03         
                    STA   $04,X                   ; A7BB: A7 04         
; End of function sub_A7B6
                    BRA   LA7D1                   ; A7BD: 20 12         

; =============== S U B R O U T I N E =======================================
LA7BF               JSR   LA705                   ; A7BF: BD A7 05        // Emit fireballs from towers 3
LA7C2               LDA   #$04                    ; A7C2: 86 04         
                    STA   $04,X                   ; A7C4: A7 04         
; End of function sub_A7BF
                    BRA   LA7D1                   ; A7C6: 20 09         

; =============== S U B R O U T I N E =======================================
; Emit fireballs from towers/bunkers 2
LA7C8               JSR   LA705                   ; A7C8: BD A7 05        // Emit fireballs from towers 3
LA7CB               LDA   #$05                    ; A7CB: 86 05         
                    STA   $04,X                   ; A7CD: A7 04         
; End of function sub_A7C8
                    BRA   LA7D1                   ; A7CF: 20 00         

; START OF FUNCTION CHUNK FOR sub_A7B6
LA7D1               LDA   #$70                    ; A7D1: 86 70         
                    STA   $05,X                   ; A7D3: A7 05         
                    LDA   #$05                    ; A7D5: 86 05         
                    STA   $03,X                   ; A7D7: A7 03         
                    LDU   <$64                    ; A7D9: DE 64           // Pointer to Tie fighter data
                    LDX   ,X                      ; A7DB: AE 84         
; END OF FUNCTION CHUNK FOR sub_A7B6
                    LDA   ,U                      ; A7DD: A6 C4         
; =============== S U B R O U T I N E =======================================
LA7DF               SUBA  $5098                   ; A7DF: B0 50 98      
                    ANDA  #$80                    ; A7E2: 84 80         
                    EORA  ,U                      ; A7E4: A8 C4         
                    LDB   $01,U                   ; A7E6: E6 41         
                    STD   ,X                      ; A7E8: ED 84         
                    LDD   $02,U                   ; A7EA: EC 42         
                    STD   $02,X                   ; A7EC: ED 02         
                    LDD   $5044                   ; A7EE: FC 50 44        // ZT2
                    STD   $04,X                   ; A7F1: ED 04         
                    JSR   Sound_39                ; A7F3: BD BE 11      
; End of function sub_A7DF
LA7F6               RTS                           ; A7F6: 39            

; =============== S U B R O U T I N E =======================================
LA7F7               LDA   <$95                    ; A7F7: 96 95         
                    BEQ   LA800                   ; A7F9: 27 05         
                    JSR   LA705                   ; A7FB: BD A7 05        // Emit fireballs from towers 3
LA7FE               BRA   LA803                   ; A7FE: 20 03         

; ---------------------------------------------------------------------------
LA800               JSR   LA6F4                   ; A800: BD A6 F4      
LA803               LDA   #$09                    ; A803: 86 09         
                    STA   $04,X                   ; A805: A7 04         
                    LDU   ,X                      ; A807: EE 84         
; End of function sub_A7F7
                    BRA   LA81F                   ; A809: 20 14         

; =============== S U B R O U T I N E =======================================
LA80B               LDA   <$95                    ; A80B: 96 95         
                    BEQ   LA814                   ; A80D: 27 05         
                    JSR   LA705                   ; A80F: BD A7 05        // Emit fireballs from towers 3
LA812               BRA   LA817                   ; A812: 20 03         

; ---------------------------------------------------------------------------
LA814               JSR   LA6F4                   ; A814: BD A6 F4      
LA817               LDA   #$0A                    ; A817: 86 0A         
                    STA   $04,X                   ; A819: A7 04         
                    LDU   ,X                      ; A81B: EE 84         
                    BRA   LA81F                   ; A81D: 20 00         

LA81F               LDD   $5078                   ; A81F: FC 50 78      
                    STD   ,U                      ; A822: ED C4         
                    LDD   $507A                   ; A824: FC 50 7A      
                    STD   $02,U                   ; A827: ED 42         
                    LDD   $507C                   ; A829: FC 50 7C      
                    STD   $04,U                   ; A82C: ED 44         
                    LDA   #$40                    ; A82E: 86 40         
                    STA   $05,X                   ; A830: A7 05         
                    LDA   #$05                    ; A832: 86 05         
                    STA   $03,X                   ; A834: A7 03         
                    LDA   $4703                   ; A836: B6 47 03      
                    LSRA                          ; A839: 44            
                    LDB   $507C                   ; A83A: F6 50 7C      
                    CMPB  #$FC                    ; A83D: C1 FC         
                    BLT   LA843                   ; A83F: 2D 02         
                    LDA   #$00                    ; A841: 86 00         
LA843               STA   $06,U                   ; A843: A7 46         
                    JSR   Sound_39                ; A845: BD BE 11      
; End of function sub_A80B
LA848               RTS                           ; A848: 39            

; =============== S U B R O U T I N E =======================================
; Fireball movement
LA849               LDX   #$494B                  ; A849: 8E 49 4B        // 6x Fireball data structure 2 ($6 bytes per fireball)
LA84C               STX   <$A8                    ; A84C: 9F A8           // Fireball data pointer
                    LDA   $03,X                   ; A84E: A6 03         
                    BEQ   LA861                   ; A850: 27 0F         
                    LDB   $04,X                   ; A852: E6 04         
                    ASLB                          ; A854: 58            
                    CMPB  #$16                    ; A855: C1 16         
                    BCC   LA860                   ; A857: 24 07         
                    LDU   #JumpTableA675          ; A859: CE A6 75      
                    JSR   [B,U]                   ; A85C: AD D5           // Fireball movement processing
LA85E               BRA   LA861                   ; A85E: 20 01         

; ---------------------------------------------------------------------------
LA860               SWI                           ; A860: 3F            
LA861               LDX   <$A8                    ; A861: 9E A8           // Fireball data pointer
                    LEAX  $06,X                   ; A863: 30 06         
                    CMPX  #$496F                  ; A865: 8C 49 6F        // 6x Fireball data structure 2 ($6 bytes per fireball)
                    BCS   LA84C                   ; A868: 25 E2         
; End of function sub_A849
                    RTS                           ; A86A: 39            

; =============== S U B R O U T I N E =======================================
LA86B               DEC   $05,X                   ; A86B: 6A 05         
                    BGT   locret_A874             ; A86D: 2E 05         
                    CLRB                          ; A86F: 5F            
                    STB   $03,X                   ; A870: E7 03         
                    STB   $05,X                   ; A872: E7 05         
; End of function sub_A86B
locret_A874         RTS                           ; A874: 39            

; =============== S U B R O U T I N E =======================================
LA875               DEC   $05,X                   ; A875: 6A 05         
                    BGT   LA880                   ; A877: 2E 07         
                    CLRB                          ; A879: 5F            
                    STB   $03,X                   ; A87A: E7 03         
                    STB   $05,X                   ; A87C: E7 05         
                    BRA   locret_A8A6             ; A87E: 20 26         

; ---------------------------------------------------------------------------
LA880               LDU   ,X                      ; A880: EE 84         
                    LDD   #$00                    ; A882: CC 00 00      
                    SUBD  ,U                      ; A885: A3 C4         
                    JSR   Shift_D_R_3             ; A887: BD CD A2      
LA88A               ADDD  ,U                      ; A88A: E3 C4         
                    STD   ,U                      ; A88C: ED C4         
                    LDD   #$00                    ; A88E: CC 00 00      
                    SUBD  $02,U                   ; A891: A3 42         
                    JSR   Shift_D_R_3             ; A893: BD CD A2      
LA896               ADDD  $02,U                   ; A896: E3 42         
                    STD   $02,U                   ; A898: ED 42         
                    LDD   #$00                    ; A89A: CC 00 00      
                    SUBD  $04,U                   ; A89D: A3 44         
                    JSR   Shift_D_R_3             ; A89F: BD CD A2      
LA8A2               ADDD  $04,U                   ; A8A2: E3 44         
                    STD   $04,U                   ; A8A4: ED 44         
; End of function sub_A875
locret_A8A6         RTS                           ; A8A6: 39            

; =============== S U B R O U T I N E =======================================
LA8A7               DEC   $05,X                   ; A8A7: 6A 05         
                    BGT   LA8B2                   ; A8A9: 2E 07         
                    CLRB                          ; A8AB: 5F            
                    STB   $03,X                   ; A8AC: E7 03         
                    STB   $05,X                   ; A8AE: E7 05         
                    BRA   locret_A8D5             ; A8B0: 20 23         

; ---------------------------------------------------------------------------
LA8B2               LDU   ,X                      ; A8B2: EE 84         
                    LDD   ,U                      ; A8B4: EC C4         
                    JSR   Shift_D_R_3             ; A8B6: BD CD A2      
LA8B9               ADDD  ,U                      ; A8B9: E3 C4         
                    BVS   LA8D6                   ; A8BB: 29 19         
                    STD   ,U                      ; A8BD: ED C4         
                    LDD   $02,U                   ; A8BF: EC 42         
                    JSR   Shift_D_R_3             ; A8C1: BD CD A2      
LA8C4               ADDD  $02,U                   ; A8C4: E3 42         
                    BVS   LA8D6                   ; A8C6: 29 0E         
                    STD   $02,U                   ; A8C8: ED 42         
                    LDD   $04,U                   ; A8CA: EC 44         
                    JSR   Shift_D_R_3             ; A8CC: BD CD A2      
LA8CF               ADDD  $04,U                   ; A8CF: E3 44         
                    BVS   LA8D6                   ; A8D1: 29 03         
                    STD   $04,U                   ; A8D3: ED 44         
locret_A8D5         RTS                           ; A8D5: 39            

; ---------------------------------------------------------------------------
LA8D6               CLRB                          ; A8D6: 5F            
                    STB   $03,X                   ; A8D7: E7 03         
; End of function sub_A8A7
                    RTS                           ; A8D9: 39            

; =============== S U B R O U T I N E =======================================
; Some towers processing
LA8DA               LDU   ,X                      ; A8DA: EE 84         
                    LDD   #$FF00                  ; A8DC: CC FF 00      
                    ADDD  ,U                      ; A8DF: E3 C4         
                    STD   ,U                      ; A8E1: ED C4         
                    JMP   LAA7D                   ; A8E3: 7E AA 7D      

; ---------------------------------------------------------------------------
LA8E6               LDU   ,X                      ; A8E6: EE 84         
                    LDD   #$FF00                  ; A8E8: CC FF 00      
                    ADDD  ,U                      ; A8EB: E3 C4         
                    STD   ,U                      ; A8ED: ED C4         
                    LDD   #$FF00                  ; A8EF: CC FF 00      
                    ADDD  $02,U                   ; A8F2: E3 42         
                    STD   $02,U                   ; A8F4: ED 42         
                    JMP   LAA7D                   ; A8F6: 7E AA 7D      

; ---------------------------------------------------------------------------
LA8F9               LDU   ,X                      ; A8F9: EE 84         
                    LDD   #$FF00                  ; A8FB: CC FF 00      
                    ADDD  ,U                      ; A8FE: E3 C4         
                    STD   ,U                      ; A900: ED C4         
                    LDD   #$0100                  ; A902: CC 01 00      
                    ADDD  $02,U                   ; A905: E3 42         
                    STD   $02,U                   ; A907: ED 42         
                    JMP   LAA7D                   ; A909: 7E AA 7D      

; ---------------------------------------------------------------------------
LA90C               LDB   ,U                      ; A90C: E6 C4         
                    SUBB  $5098                   ; A90E: F0 50 98      
                    BMI   locret_A91F             ; A911: 2B 0C         
                    CLRA                          ; A913: 4F            
                    JSR   Shift_D_L_2             ; A914: BD CD B5      
LA917               COMA                          ; A917: 43            
                    NEGB                          ; A918: 50            
                    SBCA  #$FF                    ; A919: 82 FF         
                    ADDD  ,U                      ; A91B: E3 C4         
                    STD   ,U                      ; A91D: ED C4         
locret_A91F         RTS                           ; A91F: 39            

; ---------------------------------------------------------------------------
LA920               LDD   #$00                    ; A920: CC 00 00      
                    SUBD  $508E                   ; A923: B3 50 8E      
                    JSR   Shift_D_R_3             ; A926: BD CD A2      
LA929               ADDD  $508E                   ; A929: F3 50 8E      
                    BPL   locret_A932             ; A92C: 2A 04         
                    ADDD  $02,U                   ; A92E: E3 42         
                    STD   $02,U                   ; A930: ED 42         
locret_A932         RTS                           ; A932: 39            

; ---------------------------------------------------------------------------
LA933               LDD   #$00                    ; A933: CC 00 00      
                    SUBD  $508E                   ; A936: B3 50 8E      
                    JSR   Shift_D_R_3             ; A939: BD CD A2      
LA93C               ADDD  $508E                   ; A93C: F3 50 8E      
                    BMI   locret_A945             ; A93F: 2B 04         
                    ADDD  $02,U                   ; A941: E3 42         
                    STD   $02,U                   ; A943: ED 42         
locret_A945         RTS                           ; A945: 39            

; ---------------------------------------------------------------------------
LA946               LDU   ,X                      ; A946: EE 84         
                    JSR   LA933                   ; A948: BD A9 33      
LA94B               JSR   LA90C                   ; A94B: BD A9 0C      
LA94E               LDD   ,U                      ; A94E: EC C4         
                    SUBD  $5098                   ; A950: B3 50 98      
                    LDB   $07,U                   ; A953: E6 47         
                    MUL                           ; A955: 3D            
                    TST   $06,U                   ; A956: 6D 46         
                    BNE   LA95E                   ; A958: 26 04         
                    COMA                          ; A95A: 43            
                    NEGB                          ; A95B: 50            
                    SBCA  #$FF                    ; A95C: 82 FF         
LA95E               ADDD  $509A                   ; A95E: F3 50 9A      
                    SUBD  $02,U                   ; A961: A3 42         
                    ADDD  #$0100                  ; A963: C3 01 00      
                    BMI   LA978                   ; A966: 2B 10         
                    JSR   Shift_D_R_3             ; A968: BD CD A2      
LA96B               CMPD  #$0180                  ; A96B: 10 83 01 80   
                    BLE   LA974                   ; A96F: 2F 03         
                    LDD   #$0180                  ; A971: CC 01 80      
LA974               ADDD  $02,U                   ; A974: E3 42         
                    BRA   LA986                   ; A976: 20 0E         

; ---------------------------------------------------------------------------
LA978               JSR   Shift_D_R_5             ; A978: BD CD 9E        // Shift D register right
LA97B               CMPD  #$FE80                  ; A97B: 10 83 FE 80   
                    BGE   LA984                   ; A97F: 2C 03         
                    LDD   #$FE80                  ; A981: CC FE 80      
LA984               ADDD  $02,U                   ; A984: E3 42         
LA986               STD   $02,U                   ; A986: ED 42         
                    LDD   $509C                   ; A988: FC 50 9C      
                    ADDD  #$0100                  ; A98B: C3 01 00      
                    SUBD  $04,U                   ; A98E: A3 44         
                    BMI   locret_A9A2             ; A990: 2B 10         
                    JSR   Shift_D_R_3             ; A992: BD CD A2      
LA995               CMPD  #$0200                  ; A995: 10 83 02 00   
                    BLE   LA99E                   ; A999: 2F 03         
                    LDD   #$0200                  ; A99B: CC 02 00      
LA99E               ADDD  $04,U                   ; A99E: E3 44         
                    STD   $04,U                   ; A9A0: ED 44         
locret_A9A2         RTS                           ; A9A2: 39            

; ---------------------------------------------------------------------------
LA9A3               LDU   ,X                      ; A9A3: EE 84         
                    JSR   LA920                   ; A9A5: BD A9 20      
LA9A8               JSR   LA90C                   ; A9A8: BD A9 0C      
LA9AB               LDD   ,U                      ; A9AB: EC C4         
                    SUBD  $5098                   ; A9AD: B3 50 98      
                    LDB   $07,U                   ; A9B0: E6 47         
                    MUL                           ; A9B2: 3D            
                    TST   $06,U                   ; A9B3: 6D 46         
                    BEQ   LA9BB                   ; A9B5: 27 04         
                    COMA                          ; A9B7: 43            
                    NEGB                          ; A9B8: 50            
                    SBCA  #$FF                    ; A9B9: 82 FF         
LA9BB               ADDD  $509A                   ; A9BB: F3 50 9A      
                    ADDD  #$FF00                  ; A9BE: C3 FF 00      
                    SUBD  $02,U                   ; A9C1: A3 42         
                    BMI   LA9D5                   ; A9C3: 2B 10         
                    JSR   Shift_D_R_5             ; A9C5: BD CD 9E        // Shift D register right
LA9C8               CMPD  #$0180                  ; A9C8: 10 83 01 80   
                    BLE   LA9D1                   ; A9CC: 2F 03         
                    LDD   #$0180                  ; A9CE: CC 01 80      
LA9D1               ADDD  $02,U                   ; A9D1: E3 42         
                    BRA   LA9E3                   ; A9D3: 20 0E         

; ---------------------------------------------------------------------------
LA9D5               JSR   Shift_D_R_3             ; A9D5: BD CD A2      
LA9D8               CMPD  #$FE80                  ; A9D8: 10 83 FE 80   
                    BGE   LA9E1                   ; A9DC: 2C 03         
                    LDD   #$FE80                  ; A9DE: CC FE 80      
LA9E1               ADDD  $02,U                   ; A9E1: E3 42         
LA9E3               STD   $02,U                   ; A9E3: ED 42         
                    LDD   $509C                   ; A9E5: FC 50 9C      
                    ADDD  #$0100                  ; A9E8: C3 01 00      
                    SUBD  $04,U                   ; A9EB: A3 44         
                    BMI   locret_A9FF             ; A9ED: 2B 10         
                    JSR   Shift_D_R_3             ; A9EF: BD CD A2      
LA9F2               CMPD  #$0200                  ; A9F2: 10 83 02 00   
                    BLE   LA9FB                   ; A9F6: 2F 03         
                    LDD   #$0200                  ; A9F8: CC 02 00      
LA9FB               ADDD  $04,U                   ; A9FB: E3 44         
                    STD   $04,U                   ; A9FD: ED 44         
locret_A9FF         RTS                           ; A9FF: 39            

; ---------------------------------------------------------------------------
LAA00               LDU   ,X                      ; AA00: EE 84         
                    LDD   #$FF00                  ; AA02: CC FF 00      
                    ADDD  ,U                      ; AA05: E3 C4         
                    STD   ,U                      ; AA07: ED C4         
                    LDD   $509C                   ; AA09: FC 50 9C      
                    ADDD  #$80                    ; AA0C: C3 00 80      
                    SUBD  $04,U                   ; AA0F: A3 44         
                    BMI   LAA23                   ; AA11: 2B 10         
                    JSR   Shift_D_R_3             ; AA13: BD CD A2      
LAA16               CMPD  #$0180                  ; AA16: 10 83 01 80   
                    BLE   LAA1F                   ; AA1A: 2F 03         
                    LDD   #$0180                  ; AA1C: CC 01 80      
LAA1F               ADDD  $04,U                   ; AA1F: E3 44         
                    STD   $04,U                   ; AA21: ED 44         
; ---------------------------------------------------------------------------
LAA23               BRA   LAA7D                   ; AA23: 20 58         

; ---------------------------------------------------------------------------
LAA7D               LDA   $03,X                   ; AA7D: A6 03         
                    BITA  #$02                    ; AA7F: 85 02         
                    BEQ   locret_AA85             ; AA81: 27 02         
                    CLR   $03,X                   ; AA83: 6F 03         
; End of function sub_A8DA
locret_AA85         RTS                           ; AA85: 39            

; =============== S U B R O U T I N E =======================================
; Exhaust port processing
LAA86               LDU   ,X                      ; AA86: EE 84         
                    JSR   LA90C                   ; AA88: BD A9 0C      
LAA8B               LDD   $509C                   ; AA8B: FC 50 9C      
                    SUBD  $04,U                   ; AA8E: A3 44         
                    BMI   LAA99                   ; AA90: 2B 07         
                    JSR   Shift_D_R_4             ; AA92: BD CD A0      
LAA95               ADDD  $04,U                   ; AA95: E3 44         
                    STD   $04,U                   ; AA97: ED 44         
LAA99               LDA   $4B19                   ; AA99: B6 4B 19      
                    CMPA  #$01                    ; AA9C: 81 01         
                    BCS   LAAA5                   ; AA9E: 25 05         
                    LDD   $509A                   ; AAA0: FC 50 9A      
                    BRA   LAAA8                   ; AAA3: 20 03         

; ---------------------------------------------------------------------------
LAAA5               LDD   #$FE80                  ; AAA5: CC FE 80      
LAAA8               SUBD  $02,U                   ; AAA8: A3 42         
                    BMI   LAAB3                   ; AAAA: 2B 07         
                    JSR   Shift_D_R_4             ; AAAC: BD CD A0      
LAAAF               ADDD  $02,U                   ; AAAF: E3 42         
                    STD   $02,U                   ; AAB1: ED 42         
; End of function sub_AA86
LAAB3               BRA   LAA7D                   ; AAB3: 20 C8         

; =============== S U B R O U T I N E =======================================
LAAB5               LDU   ,X                      ; AAB5: EE 84         
                    JSR   LA90C                   ; AAB7: BD A9 0C      
LAABA               LDD   $509C                   ; AABA: FC 50 9C      
                    SUBD  $04,U                   ; AABD: A3 44         
                    BMI   LAAC8                   ; AABF: 2B 07         
                    JSR   Shift_D_R_4             ; AAC1: BD CD A0      
LAAC4               ADDD  $04,U                   ; AAC4: E3 44         
                    STD   $04,U                   ; AAC6: ED 44         
LAAC8               LDA   $4B19                   ; AAC8: B6 4B 19      
                    CMPA  #$01                    ; AACB: 81 01         
                    BCS   LAAD4                   ; AACD: 25 05         
                    LDD   $509A                   ; AACF: FC 50 9A      
                    BRA   LAAD7                   ; AAD2: 20 03         

; ---------------------------------------------------------------------------
LAAD4               LDD   #$0180                  ; AAD4: CC 01 80      
LAAD7               SUBD  $02,U                   ; AAD7: A3 42         
                    BPL   LAAE2                   ; AAD9: 2A 07         
                    JSR   Shift_D_R_4             ; AADB: BD CD A0      
LAADE               ADDD  $02,U                   ; AADE: E3 42         
                    STD   $02,U                   ; AAE0: ED 42         
; End of function sub_AAB5
LAAE2               BRA   LAA7D                   ; AAE2: 20 99         

; =============== S U B R O U T I N E =======================================
; Fireball processing
LAAE4               LDX   #$494B                  ; AAE4: 8E 49 4B        // 6x Fireball data structure 2 ($6 bytes per fireball)
LAAE7               STX   <$A8                    ; AAE7: 9F A8           // Fireball data pointer
                    LDA   $03,X                   ; AAE9: A6 03         
                    LBEQ  LAC34                   ; AAEB: 10 27 01 45   
                    BITA  #$10                    ; AAEF: 85 10         
                    BEQ   LAAF9                   ; AAF1: 27 06         
                    JSR   LACB1                   ; AAF3: BD AC B1        // Shield lost by fireball animation
LAAF6               JMP   LAC34                   ; AAF6: 7E AC 34      

; ---------------------------------------------------------------------------
LAAF9               BITA  #$04                    ; AAF9: 85 04         
                    BEQ   LAB11                   ; AAFB: 27 14         
                    LDD   $5098                   ; AAFD: FC 50 98      
                    STD   $5040                   ; AB00: FD 50 40        // XT2
                    LDD   $509A                   ; AB03: FC 50 9A      
                    STD   $5042                   ; AB06: FD 50 42        // YT2
                    LDD   $509C                   ; AB09: FC 50 9C      
                    STD   $5044                   ; AB0C: FD 50 44        // ZT2
                    BRA   LAB1D                   ; AB0F: 20 0C         

; ---------------------------------------------------------------------------
LAB11               LDD   #$00                    ; AB11: CC 00 00      
                    STD   $5040                   ; AB14: FD 50 40        // XT2
                    STD   $5042                   ; AB17: FD 50 42        // YT2
                    STD   $5044                   ; AB1A: FD 50 44        // ZT2
LAB1D               CLRA                          ; AB1D: 4F            
                    LDB   $02,X                   ; AB1E: E6 02         
                    STD   $4701                   ; AB20: FD 47 01      
                    LDA   #$67                    ; AB23: 86 67         
                    JSR   LCDBA                   ; AB25: BD CD BA        // Do math program run
LAB28               LDD   $5000                   ; AB28: FC 50 00        // Math result X
                    CMPD  #$01                    ; AB2B: 10 83 00 01   
                    LBLE  LAC27                   ; AB2F: 10 2F 00 F4   
                    CMPD  #$7F00                  ; AB33: 10 83 7F 00   
                    LBHI  LAC27                   ; AB37: 10 22 00 EC   
                    STD   $4704                   ; AB3B: FD 47 04      
                    STD   $5018                   ; AB3E: FD 50 18        // XT
                    LDD   $5002                   ; AB41: FC 50 02        // Math result Y
                    STD   $501A                   ; AB44: FD 50 1A        // YT
                    BPL   LAB4D                   ; AB47: 2A 04         
                    COMA                          ; AB49: 43            
                    NEGB                          ; AB4A: 50            
                    SBCA  #$FF                    ; AB4B: 82 FF         
LAB4D               SUBD  $5000                   ; AB4D: B3 50 00        // Math result X
                    LBCC  LAC27                   ; AB50: 10 24 00 D3   
                    LDD   $5004                   ; AB54: FC 50 04        // Math result Z
                    STD   $501C                   ; AB57: FD 50 1C        // ZT
                    BPL   LAB60                   ; AB5A: 2A 04         
                    COMA                          ; AB5C: 43            
                    NEGB                          ; AB5D: 50            
                    SBCA  #$FF                    ; AB5E: 82 FF         
LAB60               SUBD  $5000                   ; AB60: B3 50 00        // Math result X
                    LBCC  LAC27                   ; AB63: 10 24 00 C0   
                    JSR   LCCF0                   ; AB67: BD CC F0        // Get divider result and multiply by Math result Z, insert VCTR instruction
LAB6A               LDX   <$A8                    ; AB6A: 9E A8           // Fireball data pointer
                    LDA   $03,X                   ; AB6C: A6 03         
                    BITA  #$03                    ; AB6E: 85 03         
                    BEQ   LABC0                   ; AB70: 27 4E         
                    LDD   #$80                    ; AB72: CC 00 80      
                    STD   $5002                   ; AB75: FD 50 02        // Math result Y
                    LDA   #$86                    ; AB78: 86 86           // Reg02 = Reg02 x Reg00
                    JSR   LCDBA                   ; AB7A: BD CD BA        // Do math program run
LAB7D               LDD   $5002                   ; AB7D: FC 50 02        // Math result Y
                    ADDD  #$0A                    ; AB80: C3 00 0A      
                    STD   <$03                    ; AB83: DD 03         
                    LDD   <$D6                    ; AB85: DC D6         
                    SUBD  <$B3                    ; AB87: 93 B3         
                    BPL   LAB8F                   ; AB89: 2A 04         
                    COMA                          ; AB8B: 43            
                    NEGB                          ; AB8C: 50            
                    SBCA  #$FF                    ; AB8D: 82 FF         
LAB8F               STD   <$01                    ; AB8F: DD 01         
                    SUBD  <$03                    ; AB91: 93 03         
                    BGT   LABC0                   ; AB93: 2E 2B         
                    LDD   <$D8                    ; AB95: DC D8         
                    SUBD  <$B5                    ; AB97: 93 B5         
                    BPL   LAB9F                   ; AB99: 2A 04         
                    COMA                          ; AB9B: 43            
                    NEGB                          ; AB9C: 50            
                    SBCA  #$FF                    ; AB9D: 82 FF         
LAB9F               CMPD  <$03                    ; AB9F: 10 93 03      
                    BGT   LABC0                   ; ABA2: 2E 1C         
                    ADDD  <$01                    ; ABA4: D3 01         
                    STD   <$01                    ; ABA6: DD 01         
                    LDD   <$03                    ; ABA8: DC 03         
                    LSRA                          ; ABAA: 44            
                    RORB                          ; ABAB: 56            
                    ADDD  <$03                    ; ABAC: D3 03         
                    SUBD  <$01                    ; ABAE: 93 01         
                    BLT   LABC0                   ; ABB0: 2D 0E         
                    LDD   $5018                   ; ABB2: FC 50 18        // XT
                    CMPD  <$CC                    ; ABB5: 10 93 CC      
                    BCC   LABC0                   ; ABB8: 24 06         
                    STD   <$CC                    ; ABBA: DD CC         
                    LDX   <$A8                    ; ABBC: 9E A8           // Fireball data pointer
                    STX   <$CA                    ; ABBE: 9F CA         
LABC0               LDA   $03,X                   ; ABC0: A6 03         
                    BITA  #$01                    ; ABC2: 85 01         
                    BEQ   LAC22                   ; ABC4: 27 5C         
                    LDD   $5018                   ; ABC6: FC 50 18        // XT
                    ASLB                          ; ABC9: 58            
                    ROLA                          ; ABCA: 49            
                    BMI   LAC22                   ; ABCB: 2B 55         
                    LDU   $5086                   ; ABCD: FE 50 86      
                    CMPU  #$0200                  ; ABD0: 11 83 02 00   
                    BLT   LABDB                   ; ABD4: 2D 05         
                    SUBD  $5086                   ; ABD6: B3 50 86      
                    BRA   LABDE                   ; ABD9: 20 03         

; ---------------------------------------------------------------------------
LABDB               SUBD  #$0200                  ; ABDB: 83 02 00      
LABDE               BLE   LABE5                   ; ABDE: 2F 05         
                    SUBD  #$0110                  ; ABE0: 83 01 10      
                    BGT   LAC22                   ; ABE3: 2E 3D         
LABE5               LDD   <$D6                    ; ABE5: DC D6         
                    BMI   LABF0                   ; ABE7: 2B 07         
                    SUBD  #$01C0                  ; ABE9: 83 01 C0      
                    BGE   LAC22                   ; ABEC: 2C 34         
                    BRA   LABF5                   ; ABEE: 20 05         

; ---------------------------------------------------------------------------
LABF0               SUBD  #$FE40                  ; ABF0: 83 FE 40      
                    BLE   LAC22                   ; ABF3: 2F 2D         
LABF5               LDD   <$D8                    ; ABF5: DC D8         
                    BMI   LAC00                   ; ABF7: 2B 07         
                    SUBD  #$01E0                  ; ABF9: 83 01 E0      
                    BGE   LAC22                   ; ABFC: 2C 24         
                    BRA   LAC05                   ; ABFE: 20 05         

; ---------------------------------------------------------------------------
LAC00               SUBD  #LFE60                  ; AC00: 83 FE 60      
                    BLE   LAC22                   ; AC03: 2F 1D         
LAC05               CPX   <$CA                    ; AC05: 9C CA         
                    BNE   LAC14                   ; AC07: 26 0B         
                    LDA   <$BC                    ; AC09: 96 BC         
                    BEQ   LAC14                   ; AC0B: 27 07         
                    JSR   LAD20                   ; AC0D: BD AD 20        // Fireball destroyed
LAC10               LDX   <$A8                    ; AC10: 9E A8           // Fireball data pointer
                    BRA   LAC22                   ; AC12: 20 0E         

; ---------------------------------------------------------------------------
LAC14               JSR   LACE0                   ; AC14: BD AC E0        // Shield lost by fireball hit
LAC17               LDD   #$8040                  ; AC17: CC 80 40      
                    STD   ,Y++                    ; AC1A: ED A1         
                    JSR   LACB1                   ; AC1C: BD AC B1        // Shield lost by fireball animation
LAC1F               JMP   LAC34                   ; AC1F: 7E AC 34      

; ---------------------------------------------------------------------------
LAC22               JSR   LAC52                   ; AC22: BD AC 52        // Fireball animation
LAC25               BRA   LAC34                   ; AC25: 20 0D         

; ---------------------------------------------------------------------------
LAC27               LDA   $03,X                   ; AC27: A6 03         
                    BITA  #$01                    ; AC29: 85 01         
                    BEQ   LAC31                   ; AC2B: 27 04         
                    LDA   #$00                    ; AC2D: 86 00         
                    BRA   LAC32                   ; AC2F: 20 01         

; ---------------------------------------------------------------------------
LAC31               CLRA                          ; AC31: 4F            
LAC32               STA   $03,X                   ; AC32: A7 03         
LAC34               LDX   <$A8                    ; AC34: 9E A8           // Fireball data pointer
                    LEAX  $06,X                   ; AC36: 30 06         
                    CMPX  #$496F                  ; AC38: 8C 49 6F        // Check for all 6 fireball slots
                    LBCS  LAAE7                   ; AC3B: 10 25 FE A8   
                    LDD   $5098                   ; AC3F: FC 50 98      
                    STD   $5040                   ; AC42: FD 50 40        // XT2
                    LDD   $509A                   ; AC45: FC 50 9A      
                    STD   $5042                   ; AC48: FD 50 42        // YT2
                    LDD   $509C                   ; AC4B: FC 50 9C      
                    STD   $5044                   ; AC4E: FD 50 44        // ZT2
; End of function sub_AAE4
                    RTS                           ; AC51: 39            

; =============== S U B R O U T I N E =======================================
; Fireball animation
LAC52               LDX   <$A8                    ; AC52: 9E A8           // Fireball data pointer
                    LDA   #$08                    ; AC54: 86 08         
                    STA   <$01                    ; AC56: 97 01         
                    LDD   $5018                   ; AC58: FC 50 18        // XT
LAC5B               DEC   <$01                    ; AC5B: 0A 01         
                    BEQ   LAC65                   ; AC5D: 27 06         
                    ASLB                          ; AC5F: 58            
                    ROLA                          ; AC60: 49            
                    BPL   LAC5B                   ; AC61: 2A F8         
                    ANDA  #$7F                    ; AC63: 84 7F         
LAC65               STA   <$02                    ; AC65: 97 02         
                    LDA   $03,X                   ; AC67: A6 03         
                    ANDA  #$20                    ; AC69: 84 20         
                    BNE   LAC7C                   ; AC6B: 26 0F         
                    LDA   <$01                    ; AC6D: 96 01         
                    ORA   #$70                    ; AC6F: 8A 70         
                    LDB   <$02                    ; AC71: D6 02         
                    STD   ,Y++                    ; AC73: ED A1         
                    LDD   #$A015                  ; AC75: CC A0 15      
                    STD   ,Y++                    ; AC78: ED A1         
                    BRA   LACA6                   ; AC7A: 20 2A         

; ---------------------------------------------------------------------------
LAC7C               LDB   <$02                    ; AC7C: D6 02         
                    LDA   <$01                    ; AC7E: 96 01         
                    INCA                          ; AC80: 4C            
                    CMPA  #$03                    ; AC81: 81 03         
                    BCC   LAC8A                   ; AC83: 24 05         
                    LDA   #$03                    ; AC85: 86 03         
                    CLRB                          ; AC87: 5F            
                    BRA   LAC91                   ; AC88: 20 07         

; ---------------------------------------------------------------------------
LAC8A               CMPA  #$06                    ; AC8A: 81 06         
                    BCS   LAC91                   ; AC8C: 25 03         
                    LDA   #$06                    ; AC8E: 86 06         
                    CLRB                          ; AC90: 5F            
LAC91               ORA   #$70                    ; AC91: 8A 70         
                    STD   ,Y++                    ; AC93: ED A1         
                    LDB   $05,X                   ; AC95: E6 05         
                    ASLB                          ; AC97: 58            
                    ASLB                          ; AC98: 58            
                    ASLB                          ; AC99: 58            
                    ASLB                          ; AC9A: 58            
                    ORB   #$0F                    ; AC9B: CA 0F         
                    LDA   #$65                    ; AC9D: 86 65         
                    STD   ,Y++                    ; AC9F: ED A1         
                    LDD   #$A017                  ; ACA1: CC A0 17      
                    STD   ,Y++                    ; ACA4: ED A1         
LACA6               LDD   #$7200                  ; ACA6: CC 72 00      
                    STD   ,Y++                    ; ACA9: ED A1         
                    LDD   #$8040                  ; ACAB: CC 80 40      
                    STD   ,Y++                    ; ACAE: ED A1         
; End of function sub_AC52
                    RTS                           ; ACB0: 39            

; =============== S U B R O U T I N E =======================================
; Shield lost by fireball animation
LACB1               LDX   <$A8                    ; ACB1: 9E A8           // Fireball data pointer
                    LDU   ,X                      ; ACB3: EE 84         
                    LDD   ,U                      ; ACB5: EC C4         
                    STD   ,Y++                    ; ACB7: ED A1         
                    LDD   $02,U                   ; ACB9: EC 42         
                    STD   ,Y++                    ; ACBB: ED A1         
                    LDA   $05,X                   ; ACBD: A6 05         
                    LDB   #$10                    ; ACBF: C6 10         
                    MUL                           ; ACC1: 3D            
                    LDA   #$70                    ; ACC2: 86 70         
                    STD   ,Y++                    ; ACC4: ED A1         
                    LDB   $05,X                   ; ACC6: E6 05         
                    ASLB                          ; ACC8: 58            
                    ASLB                          ; ACC9: 58            
                    ASLB                          ; ACCA: 58            
                    ASLB                          ; ACCB: 58            
                    LDA   #$67                    ; ACCC: 86 67         
                    STD   ,Y++                    ; ACCE: ED A1         
                    LDD   #$A017                  ; ACD0: CC A0 17      
                    STD   ,Y++                    ; ACD3: ED A1         
                    LDD   #$7200                  ; ACD5: CC 72 00      
                    STD   ,Y++                    ; ACD8: ED A1         
                    LDD   #$8040                  ; ACDA: CC 80 40      
                    STD   ,Y++                    ; ACDD: ED A1         
; End of function sub_ACB1
                    RTS                           ; ACDF: 39            

; =============== S U B R O U T I N E =======================================
; Shield lost by fireball hit
LACE0               LDU   ,X                      ; ACE0: EE 84         
                    LDD   -4,Y                    ; ACE2: EC 3C         
                    STD   ,U                      ; ACE4: ED C4         
                    LDD   -2,Y                    ; ACE6: EC 3E         
                    STD   $02,U                   ; ACE8: ED 42         
                    LDA   #$10                    ; ACEA: 86 10         
                    STA   $03,X                   ; ACEC: A7 03         
                    LDA   #$0F                    ; ACEE: 86 0F         
                    STA   $05,X                   ; ACF0: A7 05         
                    LDA   #$00                    ; ACF2: 86 00         
                    STA   $04,X                   ; ACF4: A7 04         
                    JSR   L9874                   ; ACF6: BD 98 74      
LACF9               LDA   <$63                    ; ACF9: 96 63         
                    BNE   LAD00                   ; ACFB: 26 03         
                    LDA   $4703                   ; ACFD: B6 47 03      
LAD00               LDB   #$20                    ; AD00: C6 20         
                    TSTA                          ; AD02: 4D            
                    BPL   LAD06                   ; AD03: 2A 01         
                    NEGB                          ; AD05: 50            
LAD06               STB   <$63                    ; AD06: D7 63         
                    JSR   Sound_33                ; AD08: BD BD F3      
LAD0B               LDA   $4B37                   ; AD0B: B6 4B 37      
                    BNE   locret_AD1F             ; AD0E: 26 0F         
                    INC   $4B37                   ; AD10: 7C 4B 37      
                    LDA   <$60                    ; AD13: 96 60           // Shield count
                    CMPA  #$03                    ; AD15: 81 03         
                    BLS   locret_AD1F             ; AD17: 23 06         
                    JSR   Sound_9                 ; AD19: BD BD 21      
LAD1C               JSR   Sound_31                ; AD1C: BD BD E9      
; End of function sub_ACE0
locret_AD1F         RTS                           ; AD1F: 39            

; =============== S U B R O U T I N E =======================================
; Fireball destroyed
LAD20               LDU   <$CA                    ; AD20: DE CA         
                    LDA   $03,U                   ; AD22: A6 43         
                    BEQ   locret_AD3D             ; AD24: 27 17         
                    ANDA  #$04                    ; AD26: 84 04         
                    ORA   #$20                    ; AD28: 8A 20         
                    STA   $03,U                   ; AD2A: A7 43         
                    LDA   #$0F                    ; AD2C: 86 0F         
                    STA   $05,U                   ; AD2E: A7 45         
                    LDA   #$00                    ; AD30: 86 00         
                    STA   $04,U                   ; AD32: A7 44         
                    JSR   Sound_37                ; AD34: BD BE 07      
LAD37               JSR   L9801                   ; AD37: BD 98 01        // Fireball score
LAD3A               JSR   LBDF8                   ; AD3A: BD BD F8      
; End of function sub_AD20
locret_AD3D         RTS                           ; AD3D: 39            

; =============== S U B R O U T I N E =======================================
LAD3E               LDA   #$01                    ; AD3E: 86 01         
                    STA   <$45                    ; AD40: 97 45         
                    LDD   $5098                   ; AD42: FC 50 98      
                    ADDD  #$0100                  ; AD45: C3 01 00      
                    STD   <$46                    ; AD48: DD 46         
                    LDD   $509A                   ; AD4A: FC 50 9A      
                    STD   <$48                    ; AD4D: DD 48         
                    LDD   $509C                   ; AD4F: FC 50 9C      
                    STD   <$4A                    ; AD52: DD 4A         
                    LDX   #$494B                  ; AD54: 8E 49 4B        // 6x Fireball data structure 2 ($6 bytes per fireball)
LAD57               STX   <$CA                    ; AD57: 9F CA         
                    JSR   LAD20                   ; AD59: BD AD 20        // Fireball destroyed
LAD5C               LDX   <$CA                    ; AD5C: 9E CA         
                    LEAX  $06,X                   ; AD5E: 30 06         
                    CMPX  #$496F                  ; AD60: 8C 49 6F        // 6x Fireball data structure 2 ($6 bytes per fireball)
                    BCS   LAD57                   ; AD63: 25 F2         
                    JSR   Sound_23                ; AD65: BD BD A3      
LAD68               JSR   LBDD5                   ; AD68: BD BD D5      
; End of function sub_AD3E
LAD6B               RTS                           ; AD6B: 39            

; =============== S U B R O U T I N E =======================================
LAD6C               LDA   <$45                    ; AD6C: 96 45         
                    BEQ   locret_ADAE             ; AD6E: 27 3E         
                    LDD   <$46                    ; AD70: DC 46         
                    ADDD  #$0300                  ; AD72: C3 03 00      
                    ADDD  $5086                   ; AD75: F3 50 86      
                    CMPD  <$96                    ; AD78: 10 93 96      
                    BMI   LAD7F                   ; AD7B: 2B 02         
                    LDD   <$96                    ; AD7D: DC 96         
LAD7F               STD   <$46                    ; AD7F: DD 46         
                    LDD   <$96                    ; AD81: DC 96         
                    SUBD  <$46                    ; AD83: 93 46         
                    SUBD  #$1000                  ; AD85: 83 10 00      
                    CMPD  <$4A                    ; AD88: 10 93 4A      
                    BGE   LAD8F                   ; AD8B: 2C 02         
                    STD   <$4A                    ; AD8D: DD 4A         
LAD8F               LDD   <$96                    ; AD8F: DC 96         
                    SUBD  <$46                    ; AD91: 93 46         
                    JSR   Shift_D_R_4             ; AD93: BD CD A0      
LAD96               TST   <$48                    ; AD96: 0D 48         
                    BMI   LADA3                   ; AD98: 2B 09         
                    CMPD  <$48                    ; AD9A: 10 93 48      
                    BGE   LADA1                   ; AD9D: 2C 02         
                    STD   <$48                    ; AD9F: DD 48         
LADA1               BRA   locret_ADAE             ; ADA1: 20 0B         

; ---------------------------------------------------------------------------
LADA3               COMA                          ; ADA3: 43            
                    NEGB                          ; ADA4: 50            
                    SBCA  #$FF                    ; ADA5: 82 FF         
                    CMPD  <$48                    ; ADA7: 10 93 48      
                    BLE   locret_ADAE             ; ADAA: 2F 02         
                    STD   <$48                    ; ADAC: DD 48         
; End of function sub_AD6C
locret_ADAE         RTS                           ; ADAE: 39            

; =============== S U B R O U T I N E =======================================
LADAF               LDA   <$45                    ; ADAF: 96 45         
                    BEQ   locret_ADD3             ; ADB1: 27 20         
                    LDD   <$46                    ; ADB3: DC 46         
                    STD   $5078                   ; ADB5: FD 50 78      
                    LDD   <$48                    ; ADB8: DC 48         
; End of function sub_ADAF
                    ADDD  #$80                    ; ADBA: C3 00 80      
; =============== S U B R O U T I N E =======================================
LADBD               STD   $507A                   ; ADBD: FD 50 7A      
                    LDD   <$4A                    ; ADC0: DC 4A         
                    STD   $507C                   ; ADC2: FD 50 7C      
                    JSR   LADD4                   ; ADC5: BD AD D4      
LADC8               LDD   <$48                    ; ADC8: DC 48         
                    SUBD  #$80                    ; ADCA: 83 00 80      
                    STD   $507A                   ; ADCD: FD 50 7A      
                    JSR   LADD4                   ; ADD0: BD AD D4      
; End of function sub_ADBD
locret_ADD3         RTS                           ; ADD3: 39            

; =============== S U B R O U T I N E =======================================
LADD4               LDD   #$0F                    ; ADD4: CC 00 0F        // Point BIC to $5078 MReg3C
                    STD   $4701                   ; ADD7: FD 47 01      
                    LDA   #$67                    ; ADDA: 86 67         
                    JSR   LCDBA                   ; ADDC: BD CD BA        // Do math program run
LADDF               LDD   $5000                   ; ADDF: FC 50 00        // Math result X
                    SUBD  #LE000                  ; ADE2: 83 E0 00      
                    BGT   LADEB                   ; ADE5: 2E 04         
                    LDA   #$00                    ; ADE7: 86 00         
                    STA   <$45                    ; ADE9: 97 45         
LADEB               LDD   $5000                   ; ADEB: FC 50 00        // Math result X
                    CMPD  #$01                    ; ADEE: 10 83 00 01   
                    BLT   locret_AE5F             ; ADF2: 2D 6B         
                    STD   $4704                   ; ADF4: FD 47 04      
                    STD   $5018                   ; ADF7: FD 50 18        // XT
                    LDD   $5002                   ; ADFA: FC 50 02        // Math result Y
                    BPL   LAE03                   ; ADFD: 2A 04         
                    COMA                          ; ADFF: 43            
                    NEGB                          ; AE00: 50            
                    SBCA  #$FF                    ; AE01: 82 FF         
LAE03               SUBD  $5000                   ; AE03: B3 50 00        // Math result X
                    BGE   locret_AE5F             ; AE06: 2C 57         
                    LDD   $5004                   ; AE08: FC 50 04        // Math result Z
                    BPL   LAE11                   ; AE0B: 2A 04         
                    COMA                          ; AE0D: 43            
                    NEGB                          ; AE0E: 50            
                    SBCA  #$FF                    ; AE0F: 82 FF         
LAE11               SUBD  $5000                   ; AE11: B3 50 00        // Math result X
                    BGE   locret_AE5F             ; AE14: 2C 49         
                    LDD   $4700                   ; AE16: FC 47 00      
                    STD   $5000                   ; AE19: FD 50 00        // Math result X
                    LDA   #$86                    ; AE1C: 86 86           // Reg02 = Reg02 x Reg00
                    JSR   LCDBA                   ; AE1E: BD CD BA        // Do math program run
LAE21               LDD   $5004                   ; AE21: FC 50 04        // Math result Z
                    ADDD  #$FF98                  ; AE24: C3 FF 98      
                    ANDA  #$1F                    ; AE27: 84 1F         
                    STD   ,Y++                    ; AE29: ED A1         
                    LDD   $5002                   ; AE2B: FC 50 02        // Math result Y
                    ANDA  #$1F                    ; AE2E: 84 1F         
                    STD   ,Y++                    ; AE30: ED A1         
                    LDD   #$63FF                  ; AE32: CC 63 FF      
                    STD   ,Y++                    ; AE35: ED A1         
                    LDA   #$06                    ; AE37: 86 06         
                    STA   <$01                    ; AE39: 97 01         
                    LDD   $5018                   ; AE3B: FC 50 18        // XT
LAE3E               DEC   <$01                    ; AE3E: 0A 01         
                    BEQ   LAE48                   ; AE40: 27 06         
                    ASLB                          ; AE42: 58            
                    ROLA                          ; AE43: 49            
                    BPL   LAE3E                   ; AE44: 2A F8         
                    ANDA  #$7F                    ; AE46: 84 7F         
LAE48               LDB   <$01                    ; AE48: D6 01         
                    ORB   #$70                    ; AE4A: CA 70         
                    STB   ,Y+                     ; AE4C: E7 A0         
                    STA   ,Y+                     ; AE4E: A7 A0         
                    LDD   #$A016                  ; AE50: CC A0 16      
                    STD   ,Y++                    ; AE53: ED A1         
                    LDD   #$7200                  ; AE55: CC 72 00      
                    STD   ,Y++                    ; AE58: ED A1         
                    LDD   #$8040                  ; AE5A: CC 80 40      
                    STD   ,Y++                    ; AE5D: ED A1         
; End of function sub_ADD4
locret_AE5F         RTS                           ; AE5F: 39            

; =============== S U B R O U T I N E =======================================
LAE60               LDA   <$BD                    ; AE60: 96 BD         
                    BLE   LAE6E                   ; AE62: 2F 0A         
                    DEC   <$BD                    ; AE64: 0A BD         
                    LDA   #$00                    ; AE66: 86 00         
                    STA   <$B7                    ; AE68: 97 B7         
                    STA   <$BC                    ; AE6A: 97 BC         
                    BRA   LAE72                   ; AE6C: 20 04         

; ---------------------------------------------------------------------------
LAE6E               LDA   #$00                    ; AE6E: 86 00         
                    STA   <$BD                    ; AE70: 97 BD         
LAE72               LDA   #$00                    ; AE72: 86 00         
                    STA   <$BC                    ; AE74: 97 BC         
                    ORCC  #$10                    ; AE76: 1A 10         
                    LDA   <$31                    ; AE78: 96 31         
                    BEQ   LAE87                   ; AE7A: 27 0B         
                    INC   $4B1B                   ; AE7C: 7C 4B 1B      
                    CLR   <$BD                    ; AE7F: 0F BD         
                    LDB   #$08                    ; AE81: C6 08         
                    STB   <$B7                    ; AE83: D7 B7         
                    CLR   <$31                    ; AE85: 0F 31         
LAE87               LDA   <$B7                    ; AE87: 96 B7         
                    BLE   LAE9F                   ; AE89: 2F 14         
                    DEC   <$B7                    ; AE8B: 0A B7         
                    STA   <$BC                    ; AE8D: 97 BC         
                    LDD   <$74                    ; AE8F: DC 74         
                    STD   <$B8                    ; AE91: DD B8         
                    LDD   <$6B                    ; AE93: DC 6B         
                    STD   <$BA                    ; AE95: DD BA         
                    LDD   <$2D                    ; AE97: DC 2D         
                    STD   <$B3                    ; AE99: DD B3         
                    LDD   <$2F                    ; AE9B: DC 2F         
                    STD   <$B5                    ; AE9D: DD B5         
LAE9F               ANDCC #$EF                    ; AE9F: 1C EF         
                    LDA   <$B7                    ; AEA1: 96 B7         
                    CMPA  #$07                    ; AEA3: 81 07         
                    BNE   LAEB2                   ; AEA5: 26 0B         
                    LDA   $4B36                   ; AEA7: B6 4B 36      
                    BNE   LAEAF                   ; AEAA: 26 03         
                    DEC   $4B36                   ; AEAC: 7A 4B 36      
LAEAF               JSR   LBE16                   ; AEAF: BD BE 16      
LAEB2               LDA   #$FF                    ; AEB2: 86 FF         
                    STA   <$C4                    ; AEB4: 97 C4         
                    STA   <$C8                    ; AEB6: 97 C8         
                    STA   <$CC                    ; AEB8: 97 CC         
                    STA   <$D0                    ; AEBA: 97 D0         
; End of function sub_AE60
                    RTS                           ; AEBC: 39            

; =============== S U B R O U T I N E =======================================
LAEBD               LDA   <$BC                    ; AEBD: 96 BC         
                    ORA   <$BD                    ; AEBF: 9A BD         
                    BNE   LAEC4                   ; AEC1: 26 01         
                    RTS                           ; AEC3: 39            

; ---------------------------------------------------------------------------
LAEC4               LDA   $4B1B                   ; AEC4: B6 4B 1B      
                    ANDA  #$01                    ; AEC7: 84 01         
                    BEQ   LAF25                   ; AEC9: 27 5A         
                    LDD   #$FF98                  ; AECB: CC FF 98      
                    ADDD  $4B1E                   ; AECE: F3 4B 1E      
                    ANDA  #$1F                    ; AED1: 84 1F         
                    STD   ,Y++                    ; AED3: ED A1         
                    LDD   <$B5                    ; AED5: DC B5         
                    SUBD  #$00                    ; AED7: 83 00 00      
                    SUBD  $4B1E                   ; AEDA: B3 4B 1E      
                    STD   <$03                    ; AEDD: DD 03         
                    LDD   #$FE7A                  ; AEDF: CC FE 7A      
                    ADDD  $4B1C                   ; AEE2: F3 4B 1C      
                    ANDA  #$1F                    ; AEE5: 84 1F         
                    STD   ,Y++                    ; AEE7: ED A1         
                    LDD   <$B3                    ; AEE9: DC B3         
                    SUBD  #$FE7A                  ; AEEB: 83 FE 7A      
                    SUBD  $4B1C                   ; AEEE: B3 4B 1C      
                    STD   <$01                    ; AEF1: DD 01         
                    LDB   #$00                    ; AEF3: C6 00         
                    JSR   LAF87                   ; AEF5: BD AF 87      
LAEF8               LDD   #$FDFB                  ; AEF8: CC FD FB      
                    ADDD  $4B1E                   ; AEFB: F3 4B 1E      
                    ANDA  #$1F                    ; AEFE: 84 1F         
                    STD   ,Y++                    ; AF00: ED A1         
                    LDD   <$B5                    ; AF02: DC B5         
                    SUBD  #$FE63                  ; AF04: 83 FE 63      
                    SUBD  $4B1E                   ; AF07: B3 4B 1E      
                    STD   <$03                    ; AF0A: DD 03         
                    LDD   #LFE75                  ; AF0C: CC FE 75      
                    ADDD  $4B1C                   ; AF0F: F3 4B 1C      
                    ANDA  #$1F                    ; AF12: 84 1F         
                    STD   ,Y++                    ; AF14: ED A1         
                    LDD   <$B3                    ; AF16: DC B3         
                    SUBD  #LFE75                  ; AF18: 83 FE 75      
                    SUBD  $4B1C                   ; AF1B: B3 4B 1C      
                    STD   <$01                    ; AF1E: DD 01         
                    LDB   #$08                    ; AF20: C6 08         
                    JSR   LAF87                   ; AF22: BD AF 87      
LAF25               LDA   $4B1B                   ; AF25: B6 4B 1B      
                    ANDA  #$01                    ; AF28: 84 01         
                    BNE   locret_AF86             ; AF2A: 26 5A         
                    LDD   #$FDFB                  ; AF2C: CC FD FB      
                    ADDD  $4B1E                   ; AF2F: F3 4B 1E      
                    ANDA  #$1F                    ; AF32: 84 1F         
                    STD   ,Y++                    ; AF34: ED A1         
                    LDD   <$B5                    ; AF36: DC B5         
                    SUBD  #$FE63                  ; AF38: 83 FE 63      
                    SUBD  $4B1E                   ; AF3B: B3 4B 1E      
                    STD   <$03                    ; AF3E: DD 03         
                    LDD   #$018B                  ; AF40: CC 01 8B      
                    ADDD  $4B1C                   ; AF43: F3 4B 1C      
                    ANDA  #$1F                    ; AF46: 84 1F         
                    STD   ,Y++                    ; AF48: ED A1         
                    LDD   <$B3                    ; AF4A: DC B3         
                    SUBD  #$018B                  ; AF4C: 83 01 8B      
                    SUBD  $4B1C                   ; AF4F: B3 4B 1C      
                    STD   <$01                    ; AF52: DD 01         
                    LDB   #$00                    ; AF54: C6 00         
                    JSR   LAF87                   ; AF56: BD AF 87      
LAF59               LDD   #$FF98                  ; AF59: CC FF 98      
                    ADDD  $4B1E                   ; AF5C: F3 4B 1E      
                    ANDA  #$1F                    ; AF5F: 84 1F         
                    STD   ,Y++                    ; AF61: ED A1         
                    LDD   <$B5                    ; AF63: DC B5         
                    SUBD  #$00                    ; AF65: 83 00 00      
                    SUBD  $4B1E                   ; AF68: B3 4B 1E      
                    STD   <$03                    ; AF6B: DD 03         
                    LDD   #$0186                  ; AF6D: CC 01 86      
                    ADDD  $4B1C                   ; AF70: F3 4B 1C      
                    ANDA  #$1F                    ; AF73: 84 1F         
                    STD   ,Y++                    ; AF75: ED A1         
                    LDD   <$B3                    ; AF77: DC B3         
                    SUBD  #$0186                  ; AF79: 83 01 86      
                    SUBD  $4B1C                   ; AF7C: B3 4B 1C      
                    STD   <$01                    ; AF7F: DD 01         
                    LDB   #$08                    ; AF81: C6 08         
                    JSR   LAF87                   ; AF83: BD AF 87      
; End of function sub_AEBD
locret_AF86         RTS                           ; AF86: 39            

; =============== S U B R O U T I N E =======================================
LAF87               LDU   #tbl0xB04F              ; AF87: CE B0 4F      
                    LEAU  B,U                     ; AF8A: 33 C5         
                    LDA   <$BD                    ; AF8C: 96 BD         
                    BLE   LAF9A                   ; AF8E: 2F 0A         
                    LDB   #$3F                    ; AF90: C6 3F         
                    MUL                           ; AF92: 3D            
                    LDA   #$63                    ; AF93: 86 63         
                    STD   ,Y++                    ; AF95: ED A1         
                    JMP   LAFEF                   ; AF97: 7E AF EF      

; ---------------------------------------------------------------------------
LAF9A               LDX   #$4C00                  ; AF9A: 8E 4C 00      
LAF9D               LDD   ,U++                    ; AF9D: EC C1         
                    STD   ,Y++                    ; AF9F: ED A1         
                    LDD   <$03                    ; AFA1: DC 03         
                    BMI   LAFA8                   ; AFA3: 2B 03         
                    ADDD  #$01                    ; AFA5: C3 00 01      
LAFA8               ASRA                          ; AFA8: 47            
                    RORB                          ; AFA9: 56            
                    STD   ,Y                      ; AFAA: ED A4         
                    COMA                          ; AFAC: 43            
                    NEGB                          ; AFAD: 50            
                    SBCA  #$FF                    ; AFAE: 82 FF         
                    ADDD  <$03                    ; AFB0: D3 03         
                    STD   <$03                    ; AFB2: DD 03         
                    LDD   ,Y                      ; AFB4: EC A4         
                    ANDA  #$1F                    ; AFB6: 84 1F         
                    STD   ,Y++                    ; AFB8: ED A1         
                    LDD   <$01                    ; AFBA: DC 01         
                    BMI   LAFC1                   ; AFBC: 2B 03         
                    ADDD  #$01                    ; AFBE: C3 00 01      
LAFC1               ASRA                          ; AFC1: 47            
                    RORB                          ; AFC2: 56            
                    STD   ,Y                      ; AFC3: ED A4         
                    COMA                          ; AFC5: 43            
                    NEGB                          ; AFC6: 50            
                    SBCA  #$FF                    ; AFC7: 82 FF         
                    ADDD  <$01                    ; AFC9: D3 01         
                    STD   <$01                    ; AFCB: DD 01         
                    LDD   ,Y                      ; AFCD: EC A4         
                    ORA   #$E0                    ; AFCF: 8A E0         
                    STD   ,Y++                    ; AFD1: ED A1         
                    LDD   <$01                    ; AFD3: DC 01         
                    ADDD  #$08                    ; AFD5: C3 00 08      
                    BLT   LAF9D                   ; AFD8: 2D C3         
                    SUBD  #$10                    ; AFDA: 83 00 10      
                    BGT   LAF9D                   ; AFDD: 2E BE         
                    LDD   <$03                    ; AFDF: DC 03         
                    ADDD  #$08                    ; AFE1: C3 00 08      
                    BLT   LAF9D                   ; AFE4: 2D B7         
                    SUBD  #$10                    ; AFE6: 83 00 10      
                    BGT   LAF9D                   ; AFE9: 2E B2         
                    LDD   ,U++                    ; AFEB: EC C1         
                    STD   ,Y++                    ; AFED: ED A1         
LAFEF               LDD   <$03                    ; AFEF: DC 03         
                    ANDA  #$1F                    ; AFF1: 84 1F         
                    STD   ,Y++                    ; AFF3: ED A1         
                    LDD   <$01                    ; AFF5: DC 01         
                    ORA   #$E0                    ; AFF7: 8A E0         
                    STD   ,Y++                    ; AFF9: ED A1         
                    LDA   <$BD                    ; AFFB: 96 BD         
                    BLE   LB006                   ; AFFD: 2F 07         
                    LDD   #$A011                  ; AFFF: CC A0 11      
                    STD   ,Y++                    ; B002: ED A1         
                    BRA   LB044                   ; B004: 20 3E         

; ---------------------------------------------------------------------------
LB006               BGE   LB044                   ; B006: 2C 3C         
                    LDB   $4B22                   ; B008: F6 4B 22      
                    LDA   #$71                    ; B00B: 86 71         
                    STD   ,Y++                    ; B00D: ED A1         
                    LDD   $4B20                   ; B00F: FC 4B 20      
                    STD   ,Y++                    ; B012: ED A1         
                    LDA   $4B23                   ; B014: B6 4B 23      
                    ANDA  #$01                    ; B017: 84 01         
                    BEQ   LB020                   ; B019: 27 05         
                    LDD   #$A01C                  ; B01B: CC A0 1C      
                    STD   ,Y++                    ; B01E: ED A1         
LB020               LDA   $4B23                   ; B020: B6 4B 23      
                    ANDA  #$02                    ; B023: 84 02         
                    BEQ   LB02C                   ; B025: 27 05         
                    LDD   #$A023                  ; B027: CC A0 23      
                    STD   ,Y++                    ; B02A: ED A1         
LB02C               LDA   $4B23                   ; B02C: B6 4B 23      
                    ANDA  #$04                    ; B02F: 84 04         
                    BEQ   LB038                   ; B031: 27 05         
                    LDD   #$A02A                  ; B033: CC A0 2A      
                    STD   ,Y++                    ; B036: ED A1         
LB038               LDA   $4B23                   ; B038: B6 4B 23      
                    ANDA  #$08                    ; B03B: 84 08         
                    BEQ   LB044                   ; B03D: 27 05         
                    LDD   #$A031                  ; B03F: CC A0 31      
                    STD   ,Y++                    ; B042: ED A1         
LB044               LDD   #$7200                  ; B044: CC 72 00      
                    STD   ,Y++                    ; B047: ED A1         
                    LDD   #$8040                  ; B049: CC 80 40      
                    STD   ,Y++                    ; B04C: ED A1         
; End of function sub_AF87
; ---------------------------------------------------------------------------
                    RTS                           ; B04E: 39            

; =============== S U B R O U T I N E =======================================
LB071               LDA   <$BC                    ; B071: 96 BC         
                    BEQ   locret_B094             ; B073: 27 1F         
                    JSR   LB095                   ; B075: BD B0 95      
LB078               LDD   #$0200                  ; B078: CC 02 00      
                    STD   $4706                   ; B07B: FD 47 06      
                    LDD   <$CC                    ; B07E: DC CC         
                    BMI   LB089                   ; B080: 2B 07         
                    LDA   #$04                    ; B082: 86 04         
                    STA   <$BD                    ; B084: 97 BD         
                    JSR   LAD20                   ; B086: BD AD 20        // Fireball destroyed
LB089               LDA   <$44                    ; B089: 96 44         
                    BLE   locret_B094             ; B08B: 2F 07         
                    LDA   #$FF                    ; B08D: 86 FF         
                    STA   <$44                    ; B08F: 97 44         
                    JSR   LAD3E                   ; B091: BD AD 3E      
; End of function sub_B071
locret_B094         RTS                           ; B094: 39            

; =============== S U B R O U T I N E =======================================
LB095               LDD   #$7000                  ; B095: CC 70 00      
                    ADDD  $5040                   ; B098: F3 50 40        // XT2
                    STD   <$01                    ; B09B: DD 01         
                    LDD   #$00                    ; B09D: CC 00 00      
                    SUBD  <$B8                    ; B0A0: 93 B8         
                    JSR   Shift_D_R_3             ; B0A2: BD CD A2      
LB0A5               ADDD  <$B8                    ; B0A5: D3 B8         
                    ADDD  $5042                   ; B0A7: F3 50 42        // YT2
                    STD   <$03                    ; B0AA: DD 03         
                    LDD   #$00                    ; B0AC: CC 00 00      
                    SUBD  <$BA                    ; B0AF: 93 BA         
                    JSR   Shift_D_R_3             ; B0B1: BD CD A2      
LB0B4               ADDD  <$BA                    ; B0B4: D3 BA         
                    ADDD  $5044                   ; B0B6: F3 50 44        // ZT2
                    STD   <$05                    ; B0B9: DD 05         
                    LDD   #$F000                  ; B0BB: CC F0 00      
                    SUBD  <$05                    ; B0BE: 93 05         
                    LBLT  LB158                   ; B0C0: 10 2D 00 94   
                    STD   $4706                   ; B0C4: FD 47 06      
                    LDD   $5044                   ; B0C7: FC 50 44        // ZT2
                    SUBD  <$05                    ; B0CA: 93 05         
                    STD   $4704                   ; B0CC: FD 47 04      
                    LDD   <$03                    ; B0CF: DC 03         
                    SUBD  $5042                   ; B0D1: B3 50 42        // YT2
                    STD   $5002                   ; B0D4: FD 50 02        // Math result Y
                    LDD   <$01                    ; B0D7: DC 01         
                    SUBD  $5040                   ; B0D9: B3 50 40        // XT2
                    STD   $5004                   ; B0DC: FD 50 04        // Math result Z
                    LDD   $4700                   ; B0DF: FC 47 00      
                    STD   $5000                   ; B0E2: FD 50 00        // Math result X
                    LDA   #$86                    ; B0E5: 86 86           // Reg02 = Reg02 x Reg00
                    JSR   LCDBA                   ; B0E7: BD CD BA        // Do math program run
LB0EA               LDD   <$03                    ; B0EA: DC 03         
                    SUBD  $5002                   ; B0EC: B3 50 02        // Math result Y
                    BMI   LB0F9                   ; B0EF: 2B 08         
                    CMPD  #$0400                  ; B0F1: 10 83 04 00   
                    BGT   LB158                   ; B0F5: 2E 61         
                    BRA   LB0FF                   ; B0F7: 20 06         

; ---------------------------------------------------------------------------
LB0F9               CMPD  #$FC00                  ; B0F9: 10 83 FC 00   
                    BLT   LB158                   ; B0FD: 2D 59         
LB0FF               STD   <$03                    ; B0FF: DD 03         
                    LDD   <$01                    ; B101: DC 01         
                    SUBD  $5004                   ; B103: B3 50 04        // Math result Z
                    STD   <$01                    ; B106: DD 01         
                    STD   <$C0                    ; B108: DD C0         
                    LDD   #$F000                  ; B10A: CC F0 00      
                    STD   <$05                    ; B10D: DD 05         
                    LDA   #$FF                    ; B10F: 86 FF         
                    STA   <$BD                    ; B111: 97 BD         
                    LDA   #$03                    ; B113: 86 03         
                    STA   $4B23                   ; B115: B7 4B 23      
                    LDA   <$01                    ; B118: 96 01         
                    SUBA  $5040                   ; B11A: B0 50 40        // XT2
                    LDB   #$03                    ; B11D: C6 03         
                    MUL                           ; B11F: 3D            
                    CMPD  #$E0                    ; B120: 10 83 00 E0   
                    BCS   LB128                   ; B124: 25 02         
                    LDB   #$E0                    ; B126: C6 E0         
LB128               STB   $4B22                   ; B128: F7 4B 22      
                    LDD   #$6280                  ; B12B: CC 62 80      
                    STD   $4B20                   ; B12E: FD 4B 20      
                    LDA   <$95                    ; B131: 96 95         
                    BEQ   locret_B157             ; B133: 27 22         
                    LDA   <$44                    ; B135: 96 44         
                    BNE   locret_B157             ; B137: 26 1E         
                    LDD   <$03                    ; B139: DC 03         
                    ADDD  #$0200                  ; B13B: C3 02 00      
                    BLT   locret_B157             ; B13E: 2D 17         
                    SUBD  #$0400                  ; B140: 83 04 00      
                    BGT   locret_B157             ; B143: 2E 12         
                    LDD   <$01                    ; B145: DC 01         
                    SUBD  <$96                    ; B147: 93 96         
                    ADDD  #$0200                  ; B149: C3 02 00      
                    BLT   locret_B157             ; B14C: 2D 09         
                    SUBD  #$0400                  ; B14E: 83 04 00      
                    BGT   locret_B157             ; B151: 2E 04         
                    LDA   #$01                    ; B153: 86 01         
                    STA   <$44                    ; B155: 97 44         
locret_B157         RTS                           ; B157: 39            

; ---------------------------------------------------------------------------
LB158               LDD   <$03                    ; B158: DC 03         
                    LBPL  LB1E3                   ; B15A: 10 2A 00 85   
                    LDD   #$FC00                  ; B15E: CC FC 00      
                    SUBD  <$03                    ; B161: 93 03         
                    LBLT  locret_B260             ; B163: 10 2D 00 F9   
                    STD   $4706                   ; B167: FD 47 06      
                    LDD   $5042                   ; B16A: FC 50 42        // YT2
                    SUBD  <$03                    ; B16D: 93 03         
                    STD   $4704                   ; B16F: FD 47 04      
                    LDD   <$05                    ; B172: DC 05         
                    SUBD  $5044                   ; B174: B3 50 44        // ZT2
                    STD   $5004                   ; B177: FD 50 04        // Math result Z
                    LDD   <$01                    ; B17A: DC 01         
                    SUBD  $5040                   ; B17C: B3 50 40        // XT2
                    STD   $5002                   ; B17F: FD 50 02        // Math result Y
                    LDD   $4700                   ; B182: FC 47 00      
                    STD   $5000                   ; B185: FD 50 00        // Math result X
                    LDA   #$86                    ; B188: 86 86           // Reg02 = Reg02 x Reg00
                    JSR   LCDBA                   ; B18A: BD CD BA        // Do math program run
LB18D               LDD   <$05                    ; B18D: DC 05         
                    SUBD  $5004                   ; B18F: B3 50 04        // Math result Z
                    BMI   LB19E                   ; B192: 2B 0A         
                    CMPD  #$00                    ; B194: 10 83 00 00   
                    LBGT  locret_B260             ; B198: 10 2E 00 C4   
                    BRA   LB1A6                   ; B19C: 20 08         

; ---------------------------------------------------------------------------
LB19E               CMPD  #$F000                  ; B19E: 10 83 F0 00   
                    LBLT  locret_B260             ; B1A2: 10 2D 00 BA   
LB1A6               STD   <$05                    ; B1A6: DD 05         
                    ADDD  #$1000                  ; B1A8: C3 10 00      
                    STD   <$BE                    ; B1AB: DD BE         
                    LDD   <$01                    ; B1AD: DC 01         
                    SUBD  $5002                   ; B1AF: B3 50 02        // Math result Y
                    STD   <$01                    ; B1B2: DD 01         
                    STD   <$C0                    ; B1B4: DD C0         
                    LDD   #$FC00                  ; B1B6: CC FC 00      
                    STD   <$03                    ; B1B9: DD 03         
                    LDA   #$FF                    ; B1BB: 86 FF         
                    STA   <$BD                    ; B1BD: 97 BD         
                    LDA   #$09                    ; B1BF: 86 09         
                    STA   $4B23                   ; B1C1: B7 4B 23      
                    LDA   <$01                    ; B1C4: 96 01         
                    SUBA  $5040                   ; B1C6: B0 50 40        // XT2
                    LDB   #$03                    ; B1C9: C6 03         
                    MUL                           ; B1CB: 3D            
                    CMPD  #$E0                    ; B1CC: 10 83 00 E0   
                    BCS   LB1D4                   ; B1D0: 25 02         
                    LDB   #$E0                    ; B1D2: C6 E0         
LB1D4               STB   $4B22                   ; B1D4: F7 4B 22      
                    LDD   #$6280                  ; B1D7: CC 62 80      
                    STD   $4B20                   ; B1DA: FD 4B 20      
                    LDX   #$4989                  ; B1DD: 8E 49 89      
                    JMP   LB261                   ; B1E0: 7E B2 61      

; ---------------------------------------------------------------------------
LB1E3               SUBD  #$0400                  ; B1E3: 83 04 00      
                    BLT   locret_B260             ; B1E6: 2D 78         
                    STD   $4706                   ; B1E8: FD 47 06      
                    LDD   <$03                    ; B1EB: DC 03         
                    SUBD  $5042                   ; B1ED: B3 50 42        // YT2
                    STD   $4704                   ; B1F0: FD 47 04      
                    LDD   <$05                    ; B1F3: DC 05         
                    SUBD  $5044                   ; B1F5: B3 50 44        // ZT2
                    STD   $5004                   ; B1F8: FD 50 04        // Math result Z
                    LDD   <$01                    ; B1FB: DC 01         
                    SUBD  $5040                   ; B1FD: B3 50 40        // XT2
                    STD   $5002                   ; B200: FD 50 02        // Math result Y
                    LDD   $4700                   ; B203: FC 47 00      
                    STD   $5000                   ; B206: FD 50 00        // Math result X
                    LDA   #$86                    ; B209: 86 86           // Reg02 = Reg02 x Reg00
                    JSR   LCDBA                   ; B20B: BD CD BA        // Do math program run
LB20E               LDD   <$05                    ; B20E: DC 05         
                    SUBD  $5004                   ; B210: B3 50 04        // Math result Z
                    BMI   LB21D                   ; B213: 2B 08         
                    CMPD  #$00                    ; B215: 10 83 00 00   
                    BGT   locret_B260             ; B219: 2E 45         
                    BRA   LB223                   ; B21B: 20 06         

; ---------------------------------------------------------------------------
LB21D               CMPD  #$F000                  ; B21D: 10 83 F0 00   
                    BLT   locret_B260             ; B221: 2D 3D         
LB223               STD   <$05                    ; B223: DD 05         
                    ADDD  #$1000                  ; B225: C3 10 00      
                    STD   <$BE                    ; B228: DD BE         
                    LDD   <$01                    ; B22A: DC 01         
                    SUBD  $5002                   ; B22C: B3 50 02        // Math result Y
                    STD   <$01                    ; B22F: DD 01         
                    STD   <$C0                    ; B231: DD C0         
                    LDD   #$0400                  ; B233: CC 04 00      
                    STD   <$03                    ; B236: DD 03         
                    LDA   #$FF                    ; B238: 86 FF         
                    STA   <$BD                    ; B23A: 97 BD         
                    LDA   #$06                    ; B23C: 86 06         
                    STA   $4B23                   ; B23E: B7 4B 23      
                    LDA   <$01                    ; B241: 96 01         
                    SUBA  $5040                   ; B243: B0 50 40        // XT2
                    LDB   #$03                    ; B246: C6 03         
                    MUL                           ; B248: 3D            
                    CMPD  #$E0                    ; B249: 10 83 00 E0   
                    BCS   LB251                   ; B24D: 25 02         
                    LDB   #$E0                    ; B24F: C6 E0         
LB251               STB   $4B22                   ; B251: F7 4B 22      
                    LDD   #$6280                  ; B254: CC 62 80      
                    STD   $4B20                   ; B257: FD 4B 20      
                    LDX   #$4999                  ; B25A: 8E 49 99      
                    JMP   LB261                   ; B25D: 7E B2 61      

; ---------------------------------------------------------------------------
locret_B260         RTS                           ; B260: 39            

; ---------------------------------------------------------------------------
LB261               LDA   <$BC                    ; B261: 96 BC         
                    BNE   LB266                   ; B263: 26 01         
                    RTS                           ; B265: 39            

; ---------------------------------------------------------------------------
LB266               LDB   <$C0                    ; B266: D6 C0         
                    LSRB                          ; B268: 54            
                    LSRB                          ; B269: 54            
                    LSRB                          ; B26A: 54            
                    ANDB  #$0F                    ; B26B: C4 0F         
                    ABX                           ; B26D: 3A            
                    LDD   <$C0                    ; B26E: DC C0         
                    ANDA  #$07                    ; B270: 84 07         
                    SUBD  #$01C0                  ; B272: 83 01 C0      
                    BLT   locret_B29B             ; B275: 2D 24         
                    SUBD  #$0480                  ; B277: 83 04 80      
                    BGT   locret_B29B             ; B27A: 2E 1F         
                    LDA   #$03                    ; B27C: 86 03         
                    STA   <$01                    ; B27E: 97 01         
                    LDD   <$BE                    ; B280: DC BE         
                    SUBD  #$40                    ; B282: 83 00 40      
LB285               SUBD  #$0380                  ; B285: 83 03 80      
                    BGT   LB292                   ; B288: 2E 08         
                    SUBD  #$FC80                  ; B28A: 83 FC 80      
                    BLT   locret_B29B             ; B28D: 2D 0C         
                    JMP   LB29C                   ; B28F: 7E B2 9C      

; ---------------------------------------------------------------------------
LB292               SUBD  #$80                    ; B292: 83 00 80      
                    ASL   <$01                    ; B295: 08 01         
                    ASL   <$01                    ; B297: 08 01         
                    BNE   LB285                   ; B299: 26 EA         
locret_B29B         RTS                           ; B29B: 39            

; ---------------------------------------------------------------------------
LB29C               LDA   <$01                    ; B29C: 96 01         
                    ANDA  ,X                      ; B29E: A4 84         
                    BEQ   locret_B2D1             ; B2A0: 27 2F         
                    STA   <$02                    ; B2A2: 97 02         
                    LDA   <$01                    ; B2A4: 96 01         
                    ASLA                          ; B2A6: 48            
                    ANDA  <$01                    ; B2A7: 94 01         
                    CMPA  <$02                    ; B2A9: 91 02         
                    BEQ   locret_B2D1             ; B2AB: 27 24         
                    BLS   LB2C1                   ; B2AD: 23 12         
                    LDA   #$04                    ; B2AF: 86 04         
                    STA   <$BD                    ; B2B1: 97 BD         
                    LDA   ,X                      ; B2B3: A6 84         
                    EORA  <$02                    ; B2B5: 98 02         
                    STA   ,X                      ; B2B7: A7 84         
                    JSR   L97F2                   ; B2B9: BD 97 F2        // Trench green squares score
LB2BC               JSR   Sound_35                ; B2BC: BD BD FD      
LB2BF               BRA   locret_B2D1             ; B2BF: 20 10         

; ---------------------------------------------------------------------------
LB2C1               LDA   #$04                    ; B2C1: 86 04         
                    STA   <$BD                    ; B2C3: 97 BD         
                    LDA   ,X                      ; B2C5: A6 84         
                    EORA  <$02                    ; B2C7: 98 02         
                    STA   ,X                      ; B2C9: A7 84         
                    JSR   L97FC                   ; B2CB: BD 97 FC        // Trench turrets score
LB2CE               JSR   Sound_35                ; B2CE: BD BD FD      
; End of function sub_B095
locret_B2D1         RTS                           ; B2D1: 39            

; =============== S U B R O U T I N E =======================================
; FUNCTION CHUNK AT ROM:A54B SIZE 00000045 BYTES
LB2D2               LDA   <$BC                    ; B2D2: 96 BC         
                    BEQ   locret_B2E3             ; B2D4: 27 0D         
                    LDD   <$CC                    ; B2D6: DC CC         
                    BMI   LB2E1                   ; B2D8: 2B 07         
                    LDA   #$04                    ; B2DA: 86 04         
                    STA   <$BD                    ; B2DC: 97 BD         
                    JSR   LAD20                   ; B2DE: BD AD 20        // Fireball destroyed
LB2E1               BRA   LB2E4                   ; B2E1: 20 01         

; ---------------------------------------------------------------------------
locret_B2E3         RTS                           ; B2E3: 39            

; ---------------------------------------------------------------------------
LB2E4               LDD   <$C4                    ; B2E4: DC C4         
                    BMI   LB2F8                   ; B2E6: 2B 10         
                    CMPD  <$C8                    ; B2E8: 10 93 C8      
                    BHI   LB2F8                   ; B2EB: 22 0B         
                    SUBD  <$D0                    ; B2ED: 93 D0         
                    BHI   LB307                   ; B2EF: 22 16         
                    LDA   #$04                    ; B2F1: 86 04         
                    STA   <$BD                    ; B2F3: 97 BD         
                    JMP   L8ACF                   ; B2F5: 7E 8A CF      

; ---------------------------------------------------------------------------
LB2F8               LDD   <$C8                    ; B2F8: DC C8         
                    BMI   LB307                   ; B2FA: 2B 0B         
                    SUBD  <$D0                    ; B2FC: 93 D0         
                    BHI   LB307                   ; B2FE: 22 07         
                    LDA   #$04                    ; B300: 86 04         
                    STA   <$BD                    ; B302: 97 BD         
                    JMP   LA54B                   ; B304: 7E A5 4B      

; ---------------------------------------------------------------------------
LB307               LDD   <$D0                    ; B307: DC D0         
                    BMI   locret_B32A             ; B309: 2B 1F         
                    LDA   #$FF                    ; B30B: 86 FF         
                    STA   <$BD                    ; B30D: 97 BD         
                    LDA   #$0F                    ; B30F: 86 0F         
                    STA   $4B23                   ; B311: B7 4B 23      
                    LDA   <$D0                    ; B314: 96 D0         
                    LDB   #$03                    ; B316: C6 03         
                    MUL                           ; B318: 3D            
                    CMPD  #$E0                    ; B319: 10 83 00 E0   
                    BCS   LB321                   ; B31D: 25 02         
                    LDB   #$E0                    ; B31F: C6 E0         
LB321               STB   $4B22                   ; B321: F7 4B 22      
                    LDD   #$6680                  ; B324: CC 66 80      
                    STD   $4B20                   ; B327: FD 4B 20      
; End of function sub_B2D2
locret_B32A         RTS                           ; B32A: 39            

; =============== S U B R O U T I N E =======================================
; FUNCTION CHUNK AT ROM:8ACF SIZE 0000009E BYTES
LB32B               LDA   <$BC                    ; B32B: 96 BC         
                    BEQ   locret_B33D             ; B32D: 27 0E         
                    LDD   <$CC                    ; B32F: DC CC         
                    BMI   LB339                   ; B331: 2B 06         
                    SUBD  <$C4                    ; B333: 93 C4         
                    BCS   LB33E                   ; B335: 25 07         
                    BRA   LB345                   ; B337: 20 0C         

; ---------------------------------------------------------------------------
LB339               LDA   <$C4                    ; B339: 96 C4         
                    BGE   LB345                   ; B33B: 2C 08         
locret_B33D         RTS                           ; B33D: 39            

; ---------------------------------------------------------------------------
LB33E               LDA   #$04                    ; B33E: 86 04         
                    STA   <$BD                    ; B340: 97 BD         
                    JMP   LAD20                   ; B342: 7E AD 20        // Fireball destroyed

; ---------------------------------------------------------------------------
LB345               LDA   #$04                    ; B345: 86 04         
                    STA   <$BD                    ; B347: 97 BD         
; End of function sub_B32B
; ---------------------------------------------------------------------------
                    JMP   L8ACF                   ; B349: 7E 8A CF      

                    FCB   $28                     ; B3CB:  '(' 28 41          BVC   $B40E ; 
                    FCB   $41                     ; B3CC:  'A' 41             Invalid ; 
                    FCB   $4C                     ; B3CD:  'L' 4C             INCA  ; 
                    FCB   $4C                     ; B3CE:  'L' 4C             INCA  ; 
                    FCB   $59                     ; B3CF:  'Y' 59             ROLB  ; 
                    FCB   $20                     ; B3D0:  ' ' 20 57          BRA   $B429 ; 
                    FCB   $57                     ; B3D1:  'W' 57             ASRB  ; 
                    FCB   $41                     ; B3D2:  'A' 41             Invalid ; 
                    FCB   $53                     ; B3D3:  'S' 53             COMB  ; 
                    FCB   $20                     ; B3D4:  ' ' 20 54          BRA   $B42A ; 
                    FCB   $54                     ; B3D5:  'T' 54             LSRB  ; 
                    FCB   $48                     ; B3D6:  'H' 48             ASLA  ; 
                    FCB   $45                     ; B3D7:  'E' 45             Invalid ; 
                    FCB   $20                     ; B3D8:  ' ' 20 57          BRA   $B431 ; 
                    FCB   $57                     ; B3D9:  'W' 57             ASRB  ; 
                    FCB   $48                     ; B3DA:  'H' 48             ASLA  ; 
                    FCB   $49                     ; B3DB:  'I' 49             ROLA  ; 
                    FCB   $50                     ; B3DC:  'P' 50             NEGB  ; 
                    FCB   $43                     ; B3DD:  'C' 43             COMA  ; 
                    FCB   $52                     ; B3DE:  'R' 52             Invalid ; 
                    FCB   $41                     ; B3DF:  'A' 41             Invalid ; 
                    FCB   $43                     ; B3E0:  'C' 43             COMA  ; 
                    FCB   $4B                     ; B3E1:  'K' 4B             Invalid ; 
                    FCB   $45                     ; B3E2:  'E' 45             Invalid ; 
                    FCB   $52                     ; B3E3:  'R' 52             Invalid ; 
; =============== S U B R O U T I N E =======================================
LB3E4               LDA   $49C0                   ; B3E4: B6 49 C0      
; End of function sub_B3E4
                    CMPA  #$03                    ; B3E7: 81 03         
; =============== S U B R O U T I N E =======================================
LB3E9               BCS   LB3F0                   ; B3E9: 25 05         
                    LDA   #$00                    ; B3EB: 86 00         
                    STA   $49C0                   ; B3ED: B7 49 C0      
LB3F0               LDA   #$01                    ; B3F0: 86 01         
                    STA   <$9C                    ; B3F2: 97 9C         
                    LDA   #$0B                    ; B3F4: 86 0B         
                    STA   <$DC                    ; B3F6: 97 DC         
                    STA   <$9D                    ; B3F8: 97 9D         
                    JSR   LCD38                   ; B3FA: BD CD 38        // Trench left side turret calcs
LB3FD               JSR   LB43F                   ; B3FD: BD B4 3F      
LB400               LDA   <$9D                    ; B400: 96 9D         
                    STA   <$DC                    ; B402: 97 DC         
                    JSR   LCD44                   ; B404: BD CD 44        // Trench right side turret calcs
LB407               JSR   LB579                   ; B407: BD B5 79      
LB40A               LDA   #$02                    ; B40A: 86 02         
                    STA   <$9C                    ; B40C: 97 9C         
                    LDA   #$0E                    ; B40E: 86 0E         
                    STA   <$DC                    ; B410: 97 DC         
                    STA   <$9D                    ; B412: 97 9D         
                    JSR   LCD38                   ; B414: BD CD 38        // Trench left side turret calcs
LB417               JSR   LB43F                   ; B417: BD B4 3F      
LB41A               LDA   <$9D                    ; B41A: 96 9D         
                    STA   <$DC                    ; B41C: 97 DC         
                    JSR   LCD44                   ; B41E: BD CD 44        // Trench right side turret calcs
LB421               JSR   LB579                   ; B421: BD B5 79      
LB424               LDA   #$03                    ; B424: 86 03         
                    STA   <$9C                    ; B426: 97 9C         
                    LDA   #$0C                    ; B428: 86 0C         
                    STA   <$DC                    ; B42A: 97 DC         
                    STA   <$9D                    ; B42C: 97 9D         
                    JSR   LCD38                   ; B42E: BD CD 38        // Trench left side turret calcs
LB431               JSR   LB43F                   ; B431: BD B4 3F      
LB434               LDA   <$9D                    ; B434: 96 9D         
                    STA   <$DC                    ; B436: 97 DC         
                    JSR   LCD44                   ; B438: BD CD 44        // Trench right side turret calcs
LB43B               JSR   LB579                   ; B43B: BD B5 79      
; End of function sub_B3E9
LB43E               RTS                           ; B43E: 39            

; =============== S U B R O U T I N E =======================================
LB43F               LDA   #$00                    ; B43F: 86 00           // Trench
                    STA   $49BD                   ; B441: B7 49 BD      
                    LDA   $49C0                   ; B444: B6 49 C0      
                    STA   $49BE                   ; B447: B7 49 BE      
                    LDA   #$88                    ; B44A: 86 88         
                    STA   $49C1                   ; B44C: B7 49 C1      
                    LDB   $5040                   ; B44F: F6 50 40        // XT2
                    LSRB                          ; B452: 54            
                    LSRB                          ; B453: 54            
                    LSRB                          ; B454: 54            
                    ANDB  #$0F                    ; B455: C4 0F         
                    LDX   #$4989                  ; B457: 8E 49 89      
                    ABX                           ; B45A: 3A            
                    STX   <$64                    ; B45B: 9F 64           // Pointer to Tie fighter data
                    LDA   $5040                   ; B45D: B6 50 40        // XT2
                    ANDA  #$F8                    ; B460: 84 F8         
                    ADDA  #$04                    ; B462: 8B 04         
                    LDB   #$00                    ; B464: C6 00         
                    STD   $5078                   ; B466: FD 50 78      
                    LDD   #$FC00                  ; B469: CC FC 00      
                    STD   $507A                   ; B46C: FD 50 7A      
LB46F               LDD   #$F200                  ; B46F: CC F2 00      
                    STD   $507C                   ; B472: FD 50 7C      
                    LDX   <$64                    ; B475: 9E 64           // Pointer to Tie fighter data
                    CMPX  #$4999                  ; B477: 8C 49 99      
                    BCS   LB47F                   ; B47A: 25 03         
                    LEAX  -$10,X                  ; B47C: 30 88 F0      
LB47F               LDA   ,X+                     ; B47F: A6 80         
                    STX   <$64                    ; B481: 9F 64           // Pointer to Tie fighter data
LB483               STA   <$9E                    ; B483: 97 9E         
                    ANDA  #$03                    ; B485: 84 03         
                    CMPA  <$9C                    ; B487: 91 9C         
                    LBNE  LB516                   ; B489: 10 26 00 89   
                    LDA   <$9D                    ; B48D: 96 9D         
                    STA   <$DC                    ; B48F: 97 DC         
                    LDD   #$0F                    ; B491: CC 00 0F        // Point BIC to $5078 MReg3C
                    STD   $4701                   ; B494: FD 47 01      
                    LDA   #$67                    ; B497: 86 67         
                    JSR   LCDBA                   ; B499: BD CD BA        // Do math program run
LB49C               LDA   <$9C                    ; B49C: 96 9C         
                    CMPA  #$02                    ; B49E: 81 02         
                    BNE   LB4B5                   ; B4A0: 26 13         
                    INC   $49BD                   ; B4A2: 7C 49 BD      
                    LDB   $49BE                   ; B4A5: F6 49 BE      
                    ASLB                          ; B4A8: 58            
                    LDX   #word_B6B3              ; B4A9: 8E B6 B3      
                    LDD   B,X                     ; B4AC: EC 85         
                    STD   ,Y++                    ; B4AE: ED A1         
                    LDA   $49C1                   ; B4B0: B6 49 C1      
                    STA   -1,Y                    ; B4B3: A7 3F         
LB4B5               LDD   $5078                   ; B4B5: FC 50 78      
                    SUBD  $5040                   ; B4B8: B3 50 40        // XT2
                    CMPD  #$1000                  ; B4BB: 10 83 10 00   
                    BGT   LB510                   ; B4BF: 2E 4F         
                    LDA   <$9D                    ; B4C1: 96 9D         
                    CMPA  #$0E                    ; B4C3: 81 0E         
                    BNE   LB50B                   ; B4C5: 26 44         
                    LDD   $5000                   ; B4C7: FC 50 00        // Math result X
                    SUBD  #$00                    ; B4CA: 83 00 00      
                    BLT   LB516                   ; B4CD: 2D 47         
                    LDD   $5042                   ; B4CF: FC 50 42        // YT2
                    BGT   LB50B                   ; B4D2: 2E 37         
                    LDD   $507C                   ; B4D4: FC 50 7C      
                    ADDD  #$0200                  ; B4D7: C3 02 00      
                    SUBD  $5044                   ; B4DA: B3 50 44        // ZT2
                    BLT   LB50B                   ; B4DD: 2D 2C         
                    SUBD  #$0400                  ; B4DF: 83 04 00      
                    BGT   LB50B                   ; B4E2: 2E 27         
                    LDD   $5078                   ; B4E4: FC 50 78      
                    SUBD  $5040                   ; B4E7: B3 50 40        // XT2
                    SUBD  #$0400                  ; B4EA: 83 04 00      
                    BHI   LB50B                   ; B4ED: 22 1C         
                    LDA   #$0F                    ; B4EF: 86 0F         
                    STA   <$DC                    ; B4F1: 97 DC         
                    LDA   <$60                    ; B4F3: 96 60           // Shield count
                    BLT   LB4FD                   ; B4F5: 2D 06         
                    JSR   L9874                   ; B4F7: BD 98 74      
LB4FA               JSR   Sound_26                ; B4FA: BD BD B2        // Explosion
LB4FD               LDA   <$63                    ; B4FD: 96 63         
                    BNE   LB50B                   ; B4FF: 26 0A         
                    LDA   #$4E                    ; B501: 86 4E         
                    LDB   $4703                   ; B503: F6 47 03      
                    BPL   LB509                   ; B506: 2A 01         
                    NEGA                          ; B508: 40            
LB509               STA   <$63                    ; B509: 97 63         
LB50B               JSR   LCD5C                   ; B50B: BD CD 5C        // Trench calcs
LB50E               BRA   LB513                   ; B50E: 20 03         

; ---------------------------------------------------------------------------
LB510               JSR   LCD50                   ; B510: BD CD 50      
LB513               JSR   LCD74                   ; B513: BD CD 74        // Function select for an object
LB516               LDD   $507C                   ; B516: FC 50 7C      
                    ADDD  #$0400                  ; B519: C3 04 00      
                    STD   $507C                   ; B51C: FD 50 7C      
                    LDA   <$9E                    ; B51F: 96 9E         
                    LSRA                          ; B521: 44            
                    LSRA                          ; B522: 44            
                    LBNE  LB483                   ; B523: 10 26 FF 5C   
                    LDA   <$9C                    ; B527: 96 9C         
                    CMPA  #$02                    ; B529: 81 02         
                    BNE   LB565                   ; B52B: 26 38         
                    LDX   <$64                    ; B52D: 9E 64           // Pointer to Tie fighter data
                    LDA   $000F,X                 ; B52F: A6 89 00 0F   
LB533               ASLA                          ; B533: 48            
                    BCC   LB53D                   ; B534: 24 07         
                    BMI   LB53D                   ; B536: 2B 05         
                    INC   $49BD                   ; B538: 7C 49 BD      
                    BRA   LB540                   ; B53B: 20 03         

; ---------------------------------------------------------------------------
LB53D               ASLA                          ; B53D: 48            
                    BNE   LB533                   ; B53E: 26 F3         
LB540               LDA   $49BD                   ; B540: B6 49 BD      
                    BEQ   LB565                   ; B543: 27 20         
                    LDA   $49BE                   ; B545: B6 49 BE      
                    INCA                          ; B548: 4C            
                    CMPA  #$03                    ; B549: 81 03         
                    BCS   LB54F                   ; B54B: 25 02         
                    LDA   #$00                    ; B54D: 86 00         
LB54F               STA   $49BE                   ; B54F: B7 49 BE      
                    LDA   $49C1                   ; B552: B6 49 C1      
                    SUBA  #$08                    ; B555: 80 08         
                    CMPA  #$40                    ; B557: 81 40         
                    BCC   LB55D                   ; B559: 24 02         
                    LDA   #$40                    ; B55B: 86 40         
LB55D               STA   $49C1                   ; B55D: B7 49 C1      
                    LDA   #$00                    ; B560: 86 00         
                    STA   $49BD                   ; B562: B7 49 BD      
LB565               LDD   $5078                   ; B565: FC 50 78      
                    ADDD  #$0800                  ; B568: C3 08 00      
                    STD   $5078                   ; B56B: FD 50 78      
                    SUBD  $5040                   ; B56E: B3 50 40        // XT2
                    SUBD  #$7000                  ; B571: 83 70 00      
                    LBCS  LB46F                   ; B574: 10 25 FE F7   
; End of function sub_B43F
                    RTS                           ; B578: 39            

; =============== S U B R O U T I N E =======================================
LB579               LDA   #$00                    ; B579: 86 00         
                    STA   $49BD                   ; B57B: B7 49 BD      
                    LDA   $49C0                   ; B57E: B6 49 C0      
                    STA   $49BE                   ; B581: B7 49 BE      
                    LDA   #$88                    ; B584: 86 88         
                    STA   $49C1                   ; B586: B7 49 C1      
                    LDB   $5040                   ; B589: F6 50 40        // XT2
                    LSRB                          ; B58C: 54            
                    LSRB                          ; B58D: 54            
                    LSRB                          ; B58E: 54            
                    ANDB  #$0F                    ; B58F: C4 0F         
                    LDX   #$4999                  ; B591: 8E 49 99      
                    ABX                           ; B594: 3A            
                    STX   <$64                    ; B595: 9F 64           // Pointer to Tie fighter data
                    LDA   $5040                   ; B597: B6 50 40        // XT2
                    ANDA  #$F8                    ; B59A: 84 F8         
                    ADDA  #$04                    ; B59C: 8B 04         
                    LDB   #$00                    ; B59E: C6 00         
                    STD   $5078                   ; B5A0: FD 50 78      
                    LDD   #$0400                  ; B5A3: CC 04 00      
                    STD   $507A                   ; B5A6: FD 50 7A      
LB5A9               LDD   #$F200                  ; B5A9: CC F2 00      
                    STD   $507C                   ; B5AC: FD 50 7C      
                    LDX   <$64                    ; B5AF: 9E 64           // Pointer to Tie fighter data
                    CMPX  #$49A9                  ; B5B1: 8C 49 A9      
                    BCS   LB5B9                   ; B5B4: 25 03         
                    LEAX  -$10,X                  ; B5B6: 30 88 F0      
LB5B9               LDA   ,X+                     ; B5B9: A6 80         
                    STX   <$64                    ; B5BB: 9F 64           // Pointer to Tie fighter data
LB5BD               STA   <$9E                    ; B5BD: 97 9E         
                    ANDA  #$03                    ; B5BF: 84 03         
                    CMPA  <$9C                    ; B5C1: 91 9C         
                    LBNE  LB650                   ; B5C3: 10 26 00 89   
                    LDA   <$9D                    ; B5C7: 96 9D         
                    STA   <$DC                    ; B5C9: 97 DC         
                    LDD   #$0F                    ; B5CB: CC 00 0F      
                    STD   $4701                   ; B5CE: FD 47 01        // Point BIC to $5078 MReg3C
                    LDA   #$67                    ; B5D1: 86 67         
                    JSR   LCDBA                   ; B5D3: BD CD BA        // Do math program run
LB5D6               LDA   <$9C                    ; B5D6: 96 9C         
                    CMPA  #$02                    ; B5D8: 81 02         
                    BNE   LB5EF                   ; B5DA: 26 13         
                    INC   $49BD                   ; B5DC: 7C 49 BD      
                    LDB   $49BE                   ; B5DF: F6 49 BE      
                    ASLB                          ; B5E2: 58            
                    LDX   #word_B6B3              ; B5E3: 8E B6 B3      
                    LDD   B,X                     ; B5E6: EC 85         
                    STD   ,Y++                    ; B5E8: ED A1         
                    LDA   $49C1                   ; B5EA: B6 49 C1      
                    STA   -1,Y                    ; B5ED: A7 3F         
LB5EF               LDD   $5078                   ; B5EF: FC 50 78      
                    SUBD  $5040                   ; B5F2: B3 50 40        // XT2
                    CMPD  #$1000                  ; B5F5: 10 83 10 00   
                    BGT   LB64A                   ; B5F9: 2E 4F         
                    LDA   <$9D                    ; B5FB: 96 9D         
                    CMPA  #$0E                    ; B5FD: 81 0E         
                    BNE   LB645                   ; B5FF: 26 44         
                    LDD   $5000                   ; B601: FC 50 00        // Math result X
                    SUBD  #$00                    ; B604: 83 00 00      
                    BLT   LB650                   ; B607: 2D 47         
                    LDD   $5042                   ; B609: FC 50 42        // YT2
                    BLT   LB645                   ; B60C: 2D 37         
                    LDD   $507C                   ; B60E: FC 50 7C      
                    ADDD  #$0200                  ; B611: C3 02 00      
                    SUBD  $5044                   ; B614: B3 50 44        // ZT2
                    BLT   LB645                   ; B617: 2D 2C         
                    SUBD  #$0400                  ; B619: 83 04 00      
                    BGT   LB645                   ; B61C: 2E 27         
                    LDD   $5078                   ; B61E: FC 50 78      
                    SUBD  $5040                   ; B621: B3 50 40        // XT2
                    SUBD  #$0400                  ; B624: 83 04 00      
                    BHI   LB645                   ; B627: 22 1C         
                    LDA   #$0F                    ; B629: 86 0F         
                    STA   <$DC                    ; B62B: 97 DC         
                    LDA   <$60                    ; B62D: 96 60           // Shield count
                    BLT   LB637                   ; B62F: 2D 06         
                    JSR   L9874                   ; B631: BD 98 74      
LB634               JSR   Sound_26                ; B634: BD BD B2        // Explosion
LB637               LDA   <$63                    ; B637: 96 63         
                    BNE   LB645                   ; B639: 26 0A         
                    LDA   #$4E                    ; B63B: 86 4E         
                    LDB   $4703                   ; B63D: F6 47 03      
                    BPL   LB643                   ; B640: 2A 01         
                    NEGA                          ; B642: 40            
LB643               STA   <$63                    ; B643: 97 63         
LB645               JSR   LCD5C                   ; B645: BD CD 5C        // Trench calcs
LB648               BRA   LB64D                   ; B648: 20 03         

; ---------------------------------------------------------------------------
LB64A               JSR   LCD50                   ; B64A: BD CD 50      
LB64D               JSR   LCD74                   ; B64D: BD CD 74        // Function select for an object
LB650               LDD   $507C                   ; B650: FC 50 7C      
                    ADDD  #$0400                  ; B653: C3 04 00      
                    STD   $507C                   ; B656: FD 50 7C      
                    LDA   <$9E                    ; B659: 96 9E         
                    LSRA                          ; B65B: 44            
                    LSRA                          ; B65C: 44            
                    LBNE  LB5BD                   ; B65D: 10 26 FF 5C   
                    LDA   <$9C                    ; B661: 96 9C         
                    CMPA  #$02                    ; B663: 81 02         
                    BNE   LB69F                   ; B665: 26 38         
                    LDX   <$64                    ; B667: 9E 64           // Pointer to Tie fighter data
                    LDA   -$0011,X                ; B669: A6 89 FF EF   
LB66D               ASLA                          ; B66D: 48            
                    BCC   LB677                   ; B66E: 24 07         
                    BMI   LB677                   ; B670: 2B 05         
                    INC   $49BD                   ; B672: 7C 49 BD      
                    BRA   LB67A                   ; B675: 20 03         

; ---------------------------------------------------------------------------
LB677               ASLA                          ; B677: 48            
                    BNE   LB66D                   ; B678: 26 F3         
LB67A               LDA   $49BD                   ; B67A: B6 49 BD      
                    BEQ   LB69F                   ; B67D: 27 20         
                    LDA   $49BE                   ; B67F: B6 49 BE      
                    INCA                          ; B682: 4C            
                    CMPA  #$03                    ; B683: 81 03         
                    BCS   LB689                   ; B685: 25 02         
                    LDA   #$00                    ; B687: 86 00         
LB689               STA   $49BE                   ; B689: B7 49 BE      
                    LDA   $49C1                   ; B68C: B6 49 C1      
                    SUBA  #$08                    ; B68F: 80 08         
                    CMPA  #$40                    ; B691: 81 40         
                    BCC   LB697                   ; B693: 24 02         
                    LDA   #$40                    ; B695: 86 40         
LB697               STA   $49C1                   ; B697: B7 49 C1      
                    LDA   #$00                    ; B69A: 86 00         
                    STA   $49BD                   ; B69C: B7 49 BD      
LB69F               LDD   $5078                   ; B69F: FC 50 78      
                    ADDD  #$0800                  ; B6A2: C3 08 00      
                    STD   $5078                   ; B6A5: FD 50 78      
                    SUBD  $5040                   ; B6A8: B3 50 40        // XT2
                    SUBD  #$7000                  ; B6AB: 83 70 00      
                    LBCS  LB5A9                   ; B6AE: 10 25 FE F7   
; End of function sub_B579
; ---------------------------------------------------------------------------
                    RTS                           ; B6B2: 39            

; =============== S U B R O U T I N E =======================================
; Insert vector instructions at joystick position for laser explosion 2
LB6B9               JSR   LB6C7                   ; B6B9: BD B6 C7      
LB6BC               JSR   LB6D7                   ; B6BC: BD B6 D7        // Insert vector instructions at joystick position for laser explosion
; End of function sub_B6B9
LB6BF               RTS                           ; B6BF: 39            

; =============== S U B R O U T I N E =======================================
; Insert vector instructions at joystick position for laser explosion 3
LB6C0               JSR   LB6CC                   ; B6C0: BD B6 CC      
LB6C3               JSR   LB6D7                   ; B6C3: BD B6 D7        // Insert vector instructions at joystick position for laser explosion
; End of function sub_B6C0
LB6C6               RTS                           ; B6C6: 39            

; =============== S U B R O U T I N E =======================================
LB6C7               LDD   #$6380                  ; B6C7: CC 63 80      
; End of function sub_B6C7
                    STD   ,Y++                    ; B6CA: ED A1         
; =============== S U B R O U T I N E =======================================
LB6CC               LDD   #$A012                  ; B6CC: CC A0 12      
                    STD   ,Y++                    ; B6CF: ED A1         
                    LDD   $32FE                   ; B6D1: FC 32 FE      
                    STD   ,Y++                    ; B6D4: ED A1         
; End of function sub_B6CC
                    RTS                           ; B6D6: 39            

; =============== S U B R O U T I N E =======================================
; Insert vector instructions at joystick position for laser explosion
LB6D7               LDA   <$7D                    ; B6D7: 96 7D           // Joystick X
                    BPL   LB6DC                   ; B6D9: 2A 01         
                    NEGA                          ; B6DB: 40            
LB6DC               LDB   #$6E                    ; B6DC: C6 6E         
                    MUL                           ; B6DE: 3D            
                    LDB   <$7D                    ; B6DF: D6 7D           // Joystick X
                    BPL   LB6E4                   ; B6E1: 2A 01         
                    NEGA                          ; B6E3: 40            
LB6E4               TFR   A,B                     ; B6E4: 1F 89         
                    SEX                           ; B6E6: 1D            
                    STD   $4B1C                   ; B6E7: FD 4B 1C      
                    ANDA  #$1F                    ; B6EA: 84 1F         
                    STD   $02,Y                   ; B6EC: ED 22         
                    STD   $08,Y                   ; B6EE: ED 28         
                    STD   $0E,Y                   ; B6F0: ED 2E         
                    STD   $14,Y                   ; B6F2: ED A8 14      
                    STD   $1A,Y                   ; B6F5: ED A8 1A      
                    LDA   <$7F                    ; B6F8: 96 7F           // Joystick Y
                    BPL   LB6FD                   ; B6FA: 2A 01         
LB6FC               NEGA                          ; B6FC: 40            
LB6FD               LDB   #$50                    ; B6FD: C6 50         
                    MUL                           ; B6FF: 3D            
                    LDB   <$7F                    ; B700: D6 7F           // Joystick Y
                    BPL   LB705                   ; B702: 2A 01         
                    NEGA                          ; B704: 40            
LB705               TFR   A,B                     ; B705: 1F 89         
                    SEX                           ; B707: 1D            
                    STD   $4B1E                   ; B708: FD 4B 1E      
                    ANDA  #$1F                    ; B70B: 84 1F         
                    STD   ,Y                      ; B70D: ED A4         
                    STD   $06,Y                   ; B70F: ED 26         
                    STD   $0C,Y                   ; B711: ED 2C         
                    STD   $12,Y                   ; B713: ED A8 12      
                    STD   $18,Y                   ; B716: ED A8 18      
                    LDD   #LBAAC                  ; B719: CC BA AC      
                    STD   $04,Y                   ; B71C: ED 24         
                    LDD   #$BA0D                  ; B71E: CC BA 0D      
                    STD   $0A,Y                   ; B721: ED 2A         
                    LDD   #$BB33                  ; B723: CC BB 33      
                    STD   $10,Y                   ; B726: ED A8 10      
                    LDD   #$BAE6                  ; B729: CC BA E6      
                    STD   $16,Y                   ; B72C: ED A8 16      
                    LDD   #$BA5A                  ; B72F: CC BA 5A      
                    STD   $1C,Y                   ; B732: ED A8 1C      
                    LEAY  $1E,Y                   ; B735: 31 A8 1E      
; End of function sub_B6D7
                    RTS                           ; B738: 39            

; =============== S U B R O U T I N E =======================================
LB739               LDA   #$00                    ; B739: 86 00         
                    STA   $03,X                   ; B73B: A7 03         
                    JSR   L8E1C                   ; B73D: BD 8E 1C      
LB740               PSHS  U,X                     ; B740: 34 50         
                    JSR   LB76C                   ; B742: BD B7 6C      
LB745               PULS  X,U                     ; B745: 35 50         
; End of function sub_B739
; ---------------------------------------------------------------------------
                    RTS                           ; B747: 39            

; =============== S U B R O U T I N E =======================================
LB76C               LDU   ,X                      ; B76C: EE 84         
                    JSR   LB948                   ; B76E: BD B9 48      
LB771               LDA   #$06                    ; B771: 86 06         
                    STA   $0C,X                   ; B773: A7 0C         
                    LDA   #$18                    ; B775: 86 18         
                    STA   $0D,X                   ; B777: A7 0D         
                    LDD   #$00                    ; B779: CC 00 00      
                    SUBD  -14,U                   ; B77C: A3 52         
                    JSR   Shift_D_R_6             ; B77E: BD CD 9C        // Shift D register right
LB781               STD   $06,X                   ; B781: ED 06         
                    ADDD  $08,U                   ; B783: E3 48         
                    STD   ,X                      ; B785: ED 84         
                    LDD   #$00                    ; B787: CC 00 00      
                    SUBD  -6,U                    ; B78A: A3 5A         
                    JSR   Shift_D_R_6             ; B78C: BD CD 9C        // Shift D register right
LB78F               STD   $08,X                   ; B78F: ED 08         
                    ADDD  $0A,U                   ; B791: E3 4A         
                    STD   $02,X                   ; B793: ED 02         
                    LDD   #$00                    ; B795: CC 00 00      
                    SUBD  $02,U                   ; B798: A3 42         
                    JSR   Shift_D_R_6             ; B79A: BD CD 9C        // Shift D register right
LB79D               STD   $0A,X                   ; B79D: ED 0A         
                    ADDD  $0C,U                   ; B79F: E3 4C         
                    STD   $04,X                   ; B7A1: ED 04         
                    JSR   LB83F                   ; B7A3: BD B8 3F      
LB7A6               JSR   LB948                   ; B7A6: BD B9 48      
LB7A9               LDA   #$07                    ; B7A9: 86 07         
                    STA   $0C,X                   ; B7AB: A7 0C         
                    LDA   #$18                    ; B7AD: 86 18         
                    STA   $0D,X                   ; B7AF: A7 0D         
                    LDD   -14,U                   ; B7B1: EC 52         
                    JSR   Shift_D_R_6             ; B7B3: BD CD 9C        // Shift D register right
LB7B6               STD   $06,X                   ; B7B6: ED 06         
                    ADDD  $08,U                   ; B7B8: E3 48         
                    STD   ,X                      ; B7BA: ED 84         
                    LDD   -6,U                    ; B7BC: EC 5A         
                    JSR   Shift_D_R_6             ; B7BE: BD CD 9C        // Shift D register right
LB7C1               STD   $08,X                   ; B7C1: ED 08         
                    ADDD  $0A,U                   ; B7C3: E3 4A         
                    STD   $02,X                   ; B7C5: ED 02         
                    LDD   $02,U                   ; B7C7: EC 42         
                    JSR   Shift_D_R_6             ; B7C9: BD CD 9C        // Shift D register right
LB7CC               STD   $0A,X                   ; B7CC: ED 0A         
                    ADDD  $0C,U                   ; B7CE: E3 4C         
                    STD   $04,X                   ; B7D0: ED 04         
                    JSR   LB83F                   ; B7D2: BD B8 3F      
LB7D5               JSR   LB948                   ; B7D5: BD B9 48      
LB7D8               LDA   #$08                    ; B7D8: 86 08         
                    STA   $0C,X                   ; B7DA: A7 0C         
                    LDA   #$10                    ; B7DC: 86 10         
                    STA   $0D,X                   ; B7DE: A7 0D         
                    LDD   $08,U                   ; B7E0: EC 48         
                    STD   ,X                      ; B7E2: ED 84         
                    STD   $06,X                   ; B7E4: ED 06         
                    LDD   $0A,U                   ; B7E6: EC 4A         
                    STD   $02,X                   ; B7E8: ED 02         
                    STD   $08,X                   ; B7EA: ED 08         
                    LDD   $0C,U                   ; B7EC: EC 4C         
                    STD   $04,X                   ; B7EE: ED 04         
                    STD   $0A,X                   ; B7F0: ED 0A         
                    LDA   $06,X                   ; B7F2: A6 06         
                    LDB   #$80                    ; B7F4: C6 80         
LB7F6               ASLB                          ; B7F6: 58            
                    ROLA                          ; B7F7: 49            
                    BVC   LB7FE                   ; B7F8: 28 04         
                    RORA                          ; B7FA: 46            
                    RORB                          ; B7FB: 56            
                    BRA   LB81C                   ; B7FC: 20 1E         

; ---------------------------------------------------------------------------
LB7FE               ASL   $09,X                   ; B7FE: 68 09         
                    ROL   $08,X                   ; B800: 69 08         
                    BVC   LB80C                   ; B802: 28 08         
                    ROR   $08,X                   ; B804: 66 08         
                    ROR   $09,X                   ; B806: 66 09         
                    ASRA                          ; B808: 47            
                    RORB                          ; B809: 56            
                    BRA   LB81C                   ; B80A: 20 10         

; ---------------------------------------------------------------------------
LB80C               ASL   $0B,X                   ; B80C: 68 0B         
                    ROL   $0A,X                   ; B80E: 69 0A         
                    BVC   LB7F6                   ; B810: 28 E4         
                    ROR   $0A,X                   ; B812: 66 0A         
                    ROR   $0B,X                   ; B814: 66 0B         
                    ASR   $08,X                   ; B816: 67 08         
                    ASR   $09,X                   ; B818: 67 09         
                    ASRA                          ; B81A: 47            
                    RORB                          ; B81B: 56            
LB81C               SUBD  ,X                      ; B81C: A3 84         
                    JSR   Shift_D_R_4             ; B81E: BD CD A0      
LB821               LDB   $4703                   ; B821: F6 47 03      
                    STD   $06,X                   ; B824: ED 06         
                    LDD   $08,X                   ; B826: EC 08         
                    SUBD  $02,X                   ; B828: A3 02         
                    JSR   Shift_D_R_4             ; B82A: BD CD A0      
LB82D               LDB   $4703                   ; B82D: F6 47 03      
                    STD   $08,X                   ; B830: ED 08         
                    LDD   $0A,X                   ; B832: EC 0A         
                    SUBD  $04,X                   ; B834: A3 04         
LB836               JSR   Shift_D_R_4             ; B836: BD CD A0      
LB839               LDB   $4703                   ; B839: F6 47 03      
                    STD   $0A,X                   ; B83C: ED 0A         
; End of function sub_B76C
                    RTS                           ; B83E: 39            

; =============== S U B R O U T I N E =======================================
LB83F               LDD   -10,U                   ; B83F: EC 56         
                    ADDD  $06,X                   ; B841: E3 06         
                    STD   $06,X                   ; B843: ED 06         
                    LDD   -2,U                    ; B845: EC 5E         
                    ADDD  $08,X                   ; B847: E3 08         
                    STD   $08,X                   ; B849: ED 08         
LB84B               LDD   $06,U                   ; B84B: EC 46         
                    ADDD  $0A,X                   ; B84D: E3 0A         
                    STD   $0A,X                   ; B84F: ED 0A         
; End of function sub_B83F
                    RTS                           ; B851: 39            

; =============== S U B R O U T I N E =======================================
LB852               LDD   #$01                    ; B852: CC 00 01      
                    STD   <$01                    ; B855: DD 01         
                    LDD   #$0203                  ; B857: CC 02 03      
                    STD   <$03                    ; B85A: DD 03         
; End of function sub_B852
                    BRA   LB868                   ; B85C: 20 0A         

; =============== S U B R O U T I N E =======================================
LB85E               LDD   #$0304                  ; B85E: CC 03 04      
                    STD   <$01                    ; B861: DD 01         
                    LDD   #$0502                  ; B863: CC 05 02      
                    STD   <$03                    ; B866: DD 03         
LB868               JSR   LB948                   ; B868: BD B9 48      
LB86B               LDA   <$01                    ; B86B: 96 01         
                    STA   $0C,X                   ; B86D: A7 0C         
                    LDA   #$20                    ; B86F: 86 20         
                    STA   $0D,X                   ; B871: A7 0D         
                    LDD   $5018                   ; B873: FC 50 18        // XT
LB876               STD   ,X                      ; B876: ED 84         
                    LDD   $501A                   ; B878: FC 50 1A        // YT
                    ADDA  #$FE                    ; B87B: 8B FE         
LB87D               STD   $02,X                   ; B87D: ED 02         
                    LDD   $501C                   ; B87F: FC 50 1C        // ZT
                    STD   $04,X                   ; B882: ED 04         
                    LDD   $5098                   ; B884: FC 50 98      
                    ADDA  #$7F                    ; B887: 8B 7F         
                    SUBD  $5018                   ; B889: B3 50 18        // XT
                    JSR   Shift_D_R_5             ; B88C: BD CD 9E        // Shift D register right
LB88F               LDB   $4703                   ; B88F: F6 47 03      
                    STD   $06,X                   ; B892: ED 06         
                    LDD   $509A                   ; B894: FC 50 9A      
                    ADDA  #$C1                    ; B897: 8B C1         
                    SUBD  $501A                   ; B899: B3 50 1A        // YT
                    JSR   Shift_D_R_5             ; B89C: BD CD 9E        // Shift D register right
LB89F               LDB   $4703                   ; B89F: F6 47 03      
                    STD   $08,X                   ; B8A2: ED 08         
                    LDA   <$04                    ; B8A4: 96 04         
                    LDB   $4703                   ; B8A6: F6 47 03      
                    JSR   Shift_D_L_2             ; B8A9: BD CD B5      
LB8AC               STD   $0A,X                   ; B8AC: ED 0A         
                    JSR   LB948                   ; B8AE: BD B9 48      
LB8B1               LDA   <$02                    ; B8B1: 96 02         
                    STA   $0C,X                   ; B8B3: A7 0C         
                    LDA   #$20                    ; B8B5: 86 20         
                    STA   $0D,X                   ; B8B7: A7 0D         
                    LDD   $5018                   ; B8B9: FC 50 18        // XT
                    ADDA  #$02                    ; B8BC: 8B 02         
                    STD   ,X                      ; B8BE: ED 84         
                    LDD   $501A                   ; B8C0: FC 50 1A        // YT
                    STD   $02,X                   ; B8C3: ED 02         
                    LDD   $501C                   ; B8C5: FC 50 1C        // ZT
                    STD   $04,X                   ; B8C8: ED 04         
                    LDD   $5098                   ; B8CA: FC 50 98      
                    ADDA  #$7F                    ; B8CD: 8B 7F         
                    SUBD  $5018                   ; B8CF: B3 50 18        // XT
                    JSR   Shift_D_R_5             ; B8D2: BD CD 9E        // Shift D register right
LB8D5               LDB   $4703                   ; B8D5: F6 47 03      
                    STD   $06,X                   ; B8D8: ED 06         
                    LDD   $509A                   ; B8DA: FC 50 9A      
                    SUBD  $501A                   ; B8DD: B3 50 1A        // YT
                    JSR   Shift_D_R_5             ; B8E0: BD CD 9E        // Shift D register right
LB8E3               LDB   $4703                   ; B8E3: F6 47 03      
                    STD   $08,X                   ; B8E6: ED 08         
                    LDA   <$04                    ; B8E8: 96 04         
                    LDB   $4703                   ; B8EA: F6 47 03      
                    JSR   Shift_D_L_2             ; B8ED: BD CD B5      
LB8F0               STD   $0A,X                   ; B8F0: ED 0A         
                    JSR   LB948                   ; B8F2: BD B9 48      
LB8F5               LDA   <$03                    ; B8F5: 96 03         
                    STA   $0C,X                   ; B8F7: A7 0C         
                    LDA   #$20                    ; B8F9: 86 20         
                    STA   $0D,X                   ; B8FB: A7 0D         
                    LDD   $5018                   ; B8FD: FC 50 18        // XT
                    STD   ,X                      ; B900: ED 84         
                    LDD   $501A                   ; B902: FC 50 1A        // YT
                    ADDA  #$02                    ; B905: 8B 02         
                    STD   $02,X                   ; B907: ED 02         
                    LDD   $501C                   ; B909: FC 50 1C        // ZT
                    STD   $04,X                   ; B90C: ED 04         
                    LDD   $5098                   ; B90E: FC 50 98      
                    ADDA  #$7F                    ; B911: 8B 7F         
                    SUBD  $5018                   ; B913: B3 50 18        // XT
                    JSR   Shift_D_R_5             ; B916: BD CD 9E        // Shift D register right
LB919               LDB   $4703                   ; B919: F6 47 03      
                    STD   $06,X                   ; B91C: ED 06         
                    LDD   $509A                   ; B91E: FC 50 9A      
                    ADDA  #$3F                    ; B921: 8B 3F         
                    SUBD  $501A                   ; B923: B3 50 1A        // YT
                    JSR   Shift_D_R_5             ; B926: BD CD 9E        // Shift D register right
LB929               LDB   $4703                   ; B929: F6 47 03      
                    STD   $08,X                   ; B92C: ED 08         
                    LDA   <$04                    ; B92E: 96 04           // Game over/insert coins timer
                    LDB   $4703                   ; B930: F6 47 03      
                    JSR   Shift_D_L_2             ; B933: BD CD B5      
LB936               STD   $0A,X                   ; B936: ED 0A         
; End of function sub_B85E
                    RTS                           ; B938: 39            

; =============== S U B R O U T I N E =======================================
LB939               LDX   #$49E2                  ; B939: 8E 49 E2        // 3D Object state data 2. 8 slots of 14 bytes
LB93C               LDA   #$00                    ; B93C: 86 00         
                    STA   $0D,X                   ; B93E: A7 0D           // Free up object state slot
                    LEAX  $0E,X                   ; B940: 30 0E         
                    CMPX  #$4A52                  ; B942: 8C 4A 52        // 3D Object state data 2. 8 slots of 14 bytes
                    BCS   LB93C                   ; B945: 25 F5         
; End of function sub_B939
                    RTS                           ; B947: 39            

; =============== S U B R O U T I N E =======================================
LB948               LDB   <$A2                    ; B948: D6 A2           // 3D Object slot select
                    INCB                          ; B94A: 5C            
                    CMPB  #$08                    ; B94B: C1 08         
                    BCS   LB951                   ; B94D: 25 02         
                    LDB   #$00                    ; B94F: C6 00         
LB951               STB   <$A2                    ; B951: D7 A2           // 3D Object slot select
                    LDA   #$0E                    ; B953: 86 0E         
                    MUL                           ; B955: 3D            
                    LDX   #$49E2                  ; B956: 8E 49 E2        // 3D Object state data 2. 8 slots of 14 bytes
                    LEAX  D,X                     ; B959: 30 8B         
; End of function sub_B948
                    RTS                           ; B95B: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_B98B
LB95C               LDD   #$14BD                  ; B95C: CC 14 BD      
                    STD   $5022                   ; B95F: FD 50 22        // Sine for rotation
                    LDD   #$3C8C                  ; B962: CC 3C 8C      
                    STD   $5024                   ; B965: FD 50 24        // Cosine for rotation
                    LDD   #$18                    ; B968: CC 00 18      
                    STD   $4701                   ; B96B: FD 47 01        // Point BIC to $50C0
                    LDA   #$00                    ; B96E: 86 00           // Roll
                    JSR   LCDBA                   ; B970: BD CD BA        // Do math program run
LB973               LDD   #$0590                  ; B973: CC 05 90      
                    STD   $5022                   ; B976: FD 50 22        // Sine for rotation
                    LDD   #$3FC2                  ; B979: CC 3F C2      
                    STD   $5024                   ; B97C: FD 50 24        // Cosine for rotation
                    LDD   #$18                    ; B97F: CC 00 18      
                    STD   $4701                   ; B982: FD 47 01        // Point BIC to $50C0
                    LDA   #$0E                    ; B985: 86 0E           // Pitch
                    JSR   LCDBA                   ; B987: BD CD BA        // Do math program run
; END OF FUNCTION CHUNK FOR sub_B98B
LB98A               RTS                           ; B98A: 39            

; =============== S U B R O U T I N E =======================================
; Check if tie/bunker/tower been hit
; FUNCTION CHUNK AT ROM:B95C SIZE 0000002F BYTES
LB98B               LDX   #$49E2                  ; B98B: 8E 49 E2        // 3D Object state data 2. 8 slots of 14 bytes
LB98E               LDA   $0D,X                   ; B98E: A6 0D         
                    BEQ   LB9A1                   ; B990: 27 0F           // If object state slot active then
                    DEC   $0D,X                   ; B992: 6A 0D         
                    LDB   $0C,X                   ; B994: E6 0C         
                    CMPB  #$09                    ; B996: C1 09         
                    BCS   LB99B                   ; B998: 25 01         
                    SWI                           ; B99A: 3F            
LB99B               LDU   #jt1                    ; B99B: CE B7 48      
                    ASLB                          ; B99E: 58            
                    JSR   [B,U]                   ; B99F: AD D5         
LB9A1               LEAX  $0E,X                   ; B9A1: 30 0E         
                    CMPX  #$4A52                  ; B9A3: 8C 4A 52        // 3D Object state data 2. 8 slots of 14 bytes
                    BCS   LB98E                   ; B9A6: 25 E6           // Loop until all 3D Object states processed
                    LDX   #$49E2                  ; B9A8: 8E 49 E2        // 3D Object state data 2. 8 slots of 14 bytes
LB9AB               LDA   $0D,X                   ; B9AB: A6 0D         
                    BEQ   LB9B2                   ; B9AD: 27 03         
                    JMP   LB95C                   ; B9AF: 7E B9 5C      

; ---------------------------------------------------------------------------
LB9B2               LEAX  $0E,X                   ; B9B2: 30 0E         
                    CMPX  #$4A52                  ; B9B4: 8C 4A 52        // 3D Object state data 2. 8 slots of 14 bytes
                    BCS   LB9AB                   ; B9B7: 25 F2         
                    LDU   #$50D0                  ; B9B9: CE 50 D0      
                    JSR   LCDC3                   ; B9BC: BD CD C3        // Initialise math registers matrix
; End of function sub_B98B
LB9BF               RTS                           ; B9BF: 39            

; =============== S U B R O U T I N E =======================================
LB9C0               LDD   $06,X                   ; B9C0: EC 06         
                    ADDD  ,X                      ; B9C2: E3 84         
                    STD   ,X                      ; B9C4: ED 84         
                    LDD   #$00                    ; B9C6: CC 00 00      
                    SUBD  $06,X                   ; B9C9: A3 06         
                    JSR   Shift_D_R_5             ; B9CB: BD CD 9E        // Shift D register right
LB9CE               ADDD  $06,X                   ; B9CE: E3 06         
                    STD   $06,X                   ; B9D0: ED 06         
                    LDD   $08,X                   ; B9D2: EC 08         
                    ADDD  $02,X                   ; B9D4: E3 02         
                    STD   $02,X                   ; B9D6: ED 02         
                    LDD   #$00                    ; B9D8: CC 00 00      
                    SUBD  $08,X                   ; B9DB: A3 08         
                    JSR   Shift_D_R_5             ; B9DD: BD CD 9E        // Shift D register right
LB9E0               ADDD  $08,X                   ; B9E0: E3 08         
                    STD   $08,X                   ; B9E2: ED 08         
                    LDD   $04,X                   ; B9E4: EC 04         
                    ADDD  $0A,X                   ; B9E6: E3 0A         
                    BVS   LB9F1                   ; B9E8: 29 07         
                    BGE   LB9EF                   ; B9EA: 2C 03         
                    LDD   #$00                    ; B9EC: CC 00 00      
LB9EF               STD   $04,X                   ; B9EF: ED 04         
LB9F1               LDD   $0A,X                   ; B9F1: EC 0A         
                    SUBD  #$C8                    ; B9F3: 83 00 C8      
                    STD   $0A,X                   ; B9F6: ED 0A         
; End of function sub_B9C0
                    RTS                           ; B9F8: 39            

; =============== S U B R O U T I N E =======================================
LB9F9               LDD   $06,X                   ; B9F9: EC 06         
                    ADDD  ,X                      ; B9FB: E3 84         
                    BVS   LBA01                   ; B9FD: 29 02         
                    STD   ,X                      ; B9FF: ED 84         
LBA01               LDD   $08,X                   ; BA01: EC 08         
                    ADDD  $02,X                   ; BA03: E3 02         
                    BVS   LBA09                   ; BA05: 29 02         
                    STD   $02,X                   ; BA07: ED 02         
LBA09               LDD   $0A,X                   ; BA09: EC 0A         
                    ADDD  $04,X                   ; BA0B: E3 04         
                    BVS   locret_BA11             ; BA0D: 29 02         
                    STD   $04,X                   ; BA0F: ED 04         
; End of function sub_B9F9
locret_BA11         RTS                           ; BA11: 39            

; =============== S U B R O U T I N E =======================================
; Process tie/tower/bunker explosions
LBA12               LDA   #$18                    ; BA12: 86 18           // BIC points to $50C0
                    JSR   LCE18                   ; BA14: BD CE 18        // Run math program $80 Copy [BIC] to Matrix 3
LBA17               LDA   #$40                    ; BA17: 86 40           // @      ; Matrix 1 = Matrix 2 x Matrix 3
                    JSR   LCDBA                   ; BA19: BD CD BA        // Do math program run
LBA1C               LDX   #$49E2                  ; BA1C: 8E 49 E2        // 3D Object state data 2. 8 slots of 14 bytes
LBA1F               STX   <$64                    ; BA1F: 9F 64           // Pointer to Tie fighter data
                    LDA   $0D,X                   ; BA21: A6 0D         
                    BEQ   LBA28                   ; BA23: 27 03         
                    JSR   LBA32                   ; BA25: BD BA 32        // Tie/bunker/tower hit explosion
LBA28               LDX   <$64                    ; BA28: 9E 64           // Pointer to Tie fighter data
                    LEAX  $0E,X                   ; BA2A: 30 0E         
                    CMPX  #$4A52                  ; BA2C: 8C 4A 52        // 3D Object state data 2. 8 slots of 14 bytes
                    BCS   LBA1F                   ; BA2F: 25 EE         
; End of function sub_BA12
                    RTS                           ; BA31: 39            

; =============== S U B R O U T I N E =======================================
LBA32               LDD   ,X                      ; BA32: EC 84         
                    STD   $5078                   ; BA34: FD 50 78      
                    LDD   $02,X                   ; BA37: EC 02         
                    STD   $507A                   ; BA39: FD 50 7A      
                    LDD   $04,X                   ; BA3C: EC 04         
                    STD   $507C                   ; BA3E: FD 50 7C      
                    LDD   #$0F                    ; BA41: CC 00 0F      
                    STD   $4701                   ; BA44: FD 47 01        // Point BIC to $5078 MReg3C
                    LDA   #$67                    ; BA47: 86 67         
                    JSR   LCDBA                   ; BA49: BD CD BA        // Do math program run
LBA4C               LDD   $5000                   ; BA4C: FC 50 00        // Math result X
                    BMI   LBA9D                   ; BA4F: 2B 4C           // If object behind observer then remove
                    STD   $5018                   ; BA51: FD 50 18        // XT
                    STD   $4704                   ; BA54: FD 47 04      
                    LDD   $5002                   ; BA57: FC 50 02        // Math result Y
                    STD   $501A                   ; BA5A: FD 50 1A        // YT
                    BPL   LBA63                   ; BA5D: 2A 04         
                    COMA                          ; BA5F: 43            
                    NEGB                          ; BA60: 50            
                    SBCA  #$FF                    ; BA61: 82 FF         
LBA63               SUBD  $5018                   ; BA63: B3 50 18        // XT
                    BGE   LBA9D                   ; BA66: 2C 35           // Remove object if outside visible limit
                    LDD   $5004                   ; BA68: FC 50 04        // Math result Z
                    STD   $501C                   ; BA6B: FD 50 1C        // ZT
                    BPL   LBA74                   ; BA6E: 2A 04         
                    COMA                          ; BA70: 43            
                    NEGB                          ; BA71: 50            
                    SBCA  #$FF                    ; BA72: 82 FF         
LBA74               LSRA                          ; BA74: 44            
                    RORB                          ; BA75: 56            
                    SUBD  $5018                   ; BA76: B3 50 18        // XT
                    BGE   LBA9D                   ; BA79: 2C 22           // Remove object if outside visible limit
                    LDB   $0C,X                   ; BA7B: E6 0C         
                    CMPB  #$09                    ; BA7D: C1 09         
                    BCS   LBA82                   ; BA7F: 25 01         
                    SWI                           ; BA81: 3F            
LBA82               LDU   #off_B75A               ; BA82: CE B7 5A        // Tie/bunker/tower fragments table
                    ASLB                          ; BA85: 58            
                    JSR   [B,U]                   ; BA86: AD D5           // Work out which colour for tie/bunker/ tower fragments
LBA88               JSR   LCCD8                   ; BA88: BD CC D8        // Copy object 3D data to math ram
LBA8B               JSR   LCD20                   ; BA8B: BD CD 20        // Do 3D object transform using Matrix 1
LBA8E               JSR   LCD2C                   ; BA8E: BD CD 2C        // Format vectors for ties, and tower/bunker explosions
LBA91               LDD   #$7200                  ; BA91: CC 72 00      
                    STD   ,Y++                    ; BA94: ED A1         
                    LDD   #$8040                  ; BA96: CC 80 40        // Insert vector CNTR instruction
                    STD   ,Y++                    ; BA99: ED A1         
                    BRA   locret_BA9F             ; BA9B: 20 02         

; ---------------------------------------------------------------------------
LBA9D               CLR   $0D,X                   ; BA9D: 6F 0D           // Remove/clear object state flag
; End of function sub_BA32
locret_BA9F         RTS                           ; BA9F: 39            

; =============== S U B R O U T I N E =======================================
LBAA0               LDA   #$14                    ; BAA0: 86 14         
                    BRA   LBABE                   ; BAA2: 20 1A         

; ---------------------------------------------------------------------------
LBAA4               LDA   #$15                    ; BAA4: 86 15         
                    BRA   LBABE                   ; BAA6: 20 16         

; ---------------------------------------------------------------------------
LBAA8               LDA   #$16                    ; BAA8: 86 16         
                    BRA   LBABE                   ; BAAA: 20 12         

; ---------------------------------------------------------------------------
LBAAC               LDA   #$11                    ; BAAC: 86 11         
                    BRA   LBAB8                   ; BAAE: 20 08         

; ---------------------------------------------------------------------------
LBAB0               LDA   #$12                    ; BAB0: 86 12         
                    BRA   LBAB8                   ; BAB2: 20 04         

; ---------------------------------------------------------------------------
LBAB4               LDA   #$13                    ; BAB4: 86 13         
                    BRA   LBAB8                   ; BAB6: 20 00         

LBAB8               STA   <$DC                    ; BAB8: 97 DC         
                    LDA   #$67                    ; BABA: 86 67           // g      ; Tower fragments colour
                    BRA   LBAC2                   ; BABC: 20 04         

; ---------------------------------------------------------------------------
LBABE               STA   <$DC                    ; BABE: 97 DC         
                    LDA   #$64                    ; BAC0: 86 64           // d      ; Bunker fragments colour
LBAC2               LDB   $0D,X                   ; BAC2: E6 0D         
                    CMPB  #$07                    ; BAC4: C1 07         
                    BHI   LBACE                   ; BAC6: 22 06         
                    ASLB                          ; BAC8: 58            
                    ASLB                          ; BAC9: 58            
                    ASLB                          ; BACA: 58            
                    ASLB                          ; BACB: 58            
                    BRA   LBAD0                   ; BACC: 20 02         

; ---------------------------------------------------------------------------
LBACE               LDB   #$80                    ; BACE: C6 80         
LBAD0               STD   ,Y++                    ; BAD0: ED A1         
                    LDD   ,X                      ; BAD2: EC 84         
                    STD   $5078                   ; BAD4: FD 50 78      
                    LDD   $02,X                   ; BAD7: EC 02         
                    STD   $507A                   ; BAD9: FD 50 7A      
                    LDD   #$00                    ; BADC: CC 00 00      
                    STD   $507C                   ; BADF: FD 50 7C      
                    LDD   #$0F                    ; BAE2: CC 00 0F      
                    STD   $4701                   ; BAE5: FD 47 01        // Point BIC to $5078 MReg3C
                    LDD   $501C                   ; BAE8: FC 50 1C        // ZT
                    PSHS  U,X,B,A                 ; BAEB: 34 56         
                    LDU   $501A                   ; BAED: FE 50 1A        // YT
                    LDX   $5018                   ; BAF0: BE 50 18        // XT
                    LDA   #$67                    ; BAF3: 86 67         
                    JSR   LCDBA                   ; BAF5: BD CD BA        // Do math program run
LBAF8               STX   $5018                   ; BAF8: BF 50 18        // XT
                    STU   $501A                   ; BAFB: FF 50 1A        // YT
                    PULS  A,B,X,U                 ; BAFE: 35 56         
                    STD   $501C                   ; BB00: FD 50 1C        // ZT
                    LDD   $5000                   ; BB03: FC 50 00        // Math result X
                    STD   $4704                   ; BB06: FD 47 04        // Do division
                    JSR   LCCF0                   ; BB09: BD CC F0        // Get divider result and multiply by Math result Z, insert VCTR instruction
LBB0C               LDA   #$72                    ; BB0C: 86 72           // r      ; Vector SCAL instruction
                    LDB   $5018                   ; BB0E: F6 50 18        // XT
                    ASLB                          ; BB11: 58            
                    ASLB                          ; BB12: 58            
                    STD   ,Y++                    ; BB13: ED A1         
; End of function sub_BAA0
                    RTS                           ; BB15: 39            

; =============== S U B R O U T I N E =======================================
LBB16               LDB   #$03                    ; BB16: C6 03         
; End of function sub_BB16
                    BRA   LBB22                   ; BB18: 20 08         

; =============== S U B R O U T I N E =======================================
LBB1A               LDB   #$01                    ; BB1A: C6 01         
; End of function sub_BB1A
                    BRA   LBB22                   ; BB1C: 20 04         

; =============== S U B R O U T I N E =======================================
LBB1E               LDB   #$02                    ; BB1E: C6 02         
                    BRA   LBB22                   ; BB20: 20 00         

LBB22               STB   <$DC                    ; BB22: D7 DC         
                    LDB   $0D,X                   ; BB24: E6 0D         
                    CMPB  #$1F                    ; BB26: C1 1F         
                    BLS   LBB2F                   ; BB28: 23 05           // Tie fighter hit colour cycle table
                    LDD   #$A018                  ; BB2A: CC A0 18      
; End of function sub_BB1E
                    BRA   LBB35                   ; BB2D: 20 06         

; =============== S U B R O U T I N E =======================================
; Tie fighter hit colour cycle table
LBB2F               LDU   #word_BB3B              ; BB2F: CE BB 3B      
                    ASLB                          ; BB32: 58            
                    LDD   B,U                     ; BB33: EC C5         
LBB35               STD   ,Y++                    ; BB35: ED A1         
                    JSR   LCCF0                   ; BB37: BD CC F0        // Get divider result and multiply by Math result Z, insert VCTR instruction
; End of function sub_BB2F
; ---------------------------------------------------------------------------
LBB3A               RTS                           ; BB3A: 39            

; =============== S U B R O U T I N E =======================================
LBB7B               LDA   #$01                    ; BB7B: 86 01         
                    STA   <$A1                    ; BB7D: 97 A1           // Death Star explosion state
                    LDD   #$01                    ; BB7F: CC 00 01      
                    STD   <$9F                    ; BB82: DD 9F         
; End of function sub_BB7B
                    RTS                           ; BB84: 39            

; =============== S U B R O U T I N E =======================================
; Death Star explosion animation
LBB85               LDA   <$A1                    ; BB85: 96 A1           // Death Star explosion state
                    ASLA                          ; BB87: 48            
                    LDX   #off_BB8E               ; BB88: 8E BB 8E      
                    JSR   [A,X]                   ; BB8B: AD 96         
; End of function sub_BB85
; ---------------------------------------------------------------------------
locret_BB8D         RTS                           ; BB8D: 39            

; =============== S U B R O U T I N E =======================================
LBB98               LDD   #$6480                  ; BB98: CC 64 80      
                    LDU   #$76F0                  ; BB9B: CE 76 F0      
                    LDX   <$9F                    ; BB9E: 9E 9F         
                    JSR   LBCAE                   ; BBA0: BD BC AE      
LBBA3               LDD   <$9F                    ; BBA3: DC 9F         
                    ADDD  #$02                    ; BBA5: C3 00 02      
                    STD   <$9F                    ; BBA8: DD 9F         
                    CMPD  #$3F                    ; BBAA: 10 83 00 3F   
                    BCC   locret_BBBA             ; BBAE: 24 0A         
                    LDD   #$01                    ; BBB0: CC 00 01      
                    STD   <$9F                    ; BBB3: DD 9F         
                    INC   <$A1                    ; BBB5: 0C A1           // Death Star explosion state
                    JSR   Sound_27                ; BBB7: BD BD B7      
; End of function sub_BB98
locret_BBBA         RTS                           ; BBBA: 39            

; =============== S U B R O U T I N E =======================================
LBBBB               LDX   <$9F                    ; BBBB: 9E 9F         
                    LDD   #$61FF                  ; BBBD: CC 61 FF      
                    LDU   #$76F0                  ; BBC0: CE 76 F0      
                    JSR   LBCAE                   ; BBC3: BD BC AE      
LBBC6               LDD   <$9F                    ; BBC6: DC 9F         
                    ADDD  #$02                    ; BBC8: C3 00 02      
                    STD   <$9F                    ; BBCB: DD 9F         
                    CMPD  #$3F                    ; BBCD: 10 83 00 3F   
                    BCC   LBC0B                   ; BBD1: 24 38         
                    LDB   #$3F                    ; BBD3: C6 3F         
                    SUBB  <$A0                    ; BBD5: D0 A0         
                    CLRA                          ; BBD7: 4F            
                    TFR   D,X                     ; BBD8: 1F 01         
                    LDD   #$64FF                  ; BBDA: CC 64 FF      
                    JSR   LBCAE                   ; BBDD: BD BC AE      
LBBE0               LDD   <$9F                    ; BBE0: DC 9F         
                    ASLB                          ; BBE2: 58            
                    ROLA                          ; BBE3: 49            
                    ASLB                          ; BBE4: 58            
                    ROLA                          ; BBE5: 49            
                    ASLB                          ; BBE6: 58            
                    ROLA                          ; BBE7: 49            
                    COMA                          ; BBE8: 43            
                    COMB                          ; BBE9: 53            
                    BMI   LBBEF                   ; BBEA: 2B 03         
                    DECA                          ; BBEC: 4A            
                    ORB   #$80                    ; BBED: CA 80         
LBBEF               ADDD  #$7670                  ; BBEF: C3 76 70      
                    TSTB                          ; BBF2: 5D            
                    BMI   LBBF8                   ; BBF3: 2B 03         
                    DECA                          ; BBF5: 4A            
                    ANDB  #$7F                    ; BBF6: C4 7F         
LBBF8               TFR   D,U                     ; BBF8: 1F 03         
                    LDD   <$9F                    ; BBFA: DC 9F         
                    LSRB                          ; BBFC: 54            
                    LSRB                          ; BBFD: 54            
                    ANDB  #$07                    ; BBFE: C4 07         
                    EORB  #$07                    ; BC00: C8 07         
                    INCB                          ; BC02: 5C            
                    TFR   D,X                     ; BC03: 1F 01         
                    LDD   #$64FF                  ; BC05: CC 64 FF      
                    JSR   LBCC8                   ; BC08: BD BC C8      
LBC0B               LDD   <$9F                    ; BC0B: DC 9F         
                    CMPD  #$3F                    ; BC0D: 10 83 00 3F   
                    BCS   locret_BC1D             ; BC11: 25 0A         
                    LDD   #$01                    ; BC13: CC 00 01      
                    STD   <$9F                    ; BC16: DD 9F         
                    INC   <$A1                    ; BC18: 0C A1           // Death Star explosion state
                    JSR   Sound_27                ; BC1A: BD BD B7      
; End of function sub_BBBB
locret_BC1D         RTS                           ; BC1D: 39            

; =============== S U B R O U T I N E =======================================
LBC1E               LDX   <$9F                    ; BC1E: 9E 9F         
                    LDD   #$67FF                  ; BC20: CC 67 FF      
                    LDU   #$7670                  ; BC23: CE 76 70      
                    JSR   LBCAE                   ; BC26: BD BC AE      
LBC29               LDD   <$9F                    ; BC29: DC 9F         
                    ADDD  #$03                    ; BC2B: C3 00 03      
                    STD   <$9F                    ; BC2E: DD 9F         
                    CMPD  #$3F                    ; BC30: 10 83 00 3F   
                    BCC   LBC43                   ; BC34: 24 0D         
                    LDB   #$3F                    ; BC36: C6 3F         
                    SUBB  <$A0                    ; BC38: D0 A0         
                    CLRA                          ; BC3A: 4F            
                    TFR   D,X                     ; BC3B: 1F 01         
                    LDD   #$61FF                  ; BC3D: CC 61 FF      
                    JSR   LBCAE                   ; BC40: BD BC AE      
LBC43               LDD   <$9F                    ; BC43: DC 9F         
                    CMPD  #$3F                    ; BC45: 10 83 00 3F   
                    BCC   LBC72                   ; BC49: 24 27         
                    ASLB                          ; BC4B: 58            
                    ROLA                          ; BC4C: 49            
                    ASLB                          ; BC4D: 58            
                    ROLA                          ; BC4E: 49            
                    ASLB                          ; BC4F: 58            
                    ROLA                          ; BC50: 49            
                    COMA                          ; BC51: 43            
                    COMB                          ; BC52: 53            
                    BMI   LBC58                   ; BC53: 2B 03         
                    DECA                          ; BC55: 4A            
                    ORB   #$80                    ; BC56: CA 80         
LBC58               ADDD  #$7670                  ; BC58: C3 76 70      
                    TSTB                          ; BC5B: 5D            
                    BMI   LBC61                   ; BC5C: 2B 03         
                    DECA                          ; BC5E: 4A            
                    ANDB  #$7F                    ; BC5F: C4 7F         
LBC61               TFR   D,U                     ; BC61: 1F 03         
                    LDD   <$9F                    ; BC63: DC 9F         
                    ANDB  #$0F                    ; BC65: C4 0F         
                    EORB  #$0F                    ; BC67: C8 0F         
                    INCB                          ; BC69: 5C            
                    TFR   D,X                     ; BC6A: 1F 01         
                    LDD   #$61FF                  ; BC6C: CC 61 FF      
                    JSR   LBCC8                   ; BC6F: BD BC C8      
LBC72               LDD   <$9F                    ; BC72: DC 9F         
                    CMPD  #$50                    ; BC74: 10 83 00 50   
                    BCS   locret_BC84             ; BC78: 25 0A         
                    LDD   #$80                    ; BC7A: CC 00 80      
                    STD   <$9F                    ; BC7D: DD 9F         
                    INC   <$A1                    ; BC7F: 0C A1           // Death Star explosion state
                    JSR   Sound_27                ; BC81: BD BD B7      
; End of function sub_BC1E
locret_BC84         RTS                           ; BC84: 39            

; =============== S U B R O U T I N E =======================================
LBC85               LDD   <$9F                    ; BC85: DC 9F         
                    SUBD  #$04                    ; BC87: 83 00 04      
                    STD   <$9F                    ; BC8A: DD 9F         
                    CMPD  #$08                    ; BC8C: 10 83 00 08   
                    BCS   LBCA9                   ; BC90: 25 17         
                    ADDD  #$7500                  ; BC92: C3 75 00      
                    TFR   D,U                     ; BC95: 1F 03         
                    LDD   <$9F                    ; BC97: DC 9F         
                    LSRA                          ; BC99: 44            
                    RORB                          ; BC9A: 56            
                    ANDB  #$3F                    ; BC9B: C4 3F         
                    EORB  #$3F                    ; BC9D: C8 3F         
                    TFR   D,X                     ; BC9F: 1F 01         
                    LDD   #$67FF                  ; BCA1: CC 67 FF      
                    JSR   LBCC8                   ; BCA4: BD BC C8      
LBCA7               BRA   locret_BCAD             ; BCA7: 20 04         

; ---------------------------------------------------------------------------
LBCA9               LDA   #$00                    ; BCA9: 86 00         
                    STA   <$A1                    ; BCAB: 97 A1           // Death Star explosion state
; End of function sub_BC85
locret_BCAD         RTS                           ; BCAD: 39            

; =============== S U B R O U T I N E =======================================
LBCAE               STD   ,Y++                    ; BCAE: ED A1         
LBCB0               LDD   #$1F98                  ; BCB0: CC 1F 98      
                    STD   ,Y++                    ; BCB3: ED A1         
                    LDD   #$00                    ; BCB5: CC 00 00      
                    STD   ,Y++                    ; BCB8: ED A1         
                    STU   ,Y++                    ; BCBA: EF A1         
                    LDD   #$BD69                  ; BCBC: CC BD 69      
                    STD   ,Y++                    ; BCBF: ED A1         
                    LEAU  -2,U                    ; BCC1: 33 5E         
                    LEAX  -1,X                    ; BCC3: 30 1F         
                    BNE   LBCB0                   ; BCC5: 26 E9         
; End of function sub_BCAE
                    RTS                           ; BCC7: 39            

; =============== S U B R O U T I N E =======================================
LBCC8               STD   ,Y++                    ; BCC8: ED A1         
LBCCA               LDD   #$1F98                  ; BCCA: CC 1F 98      
                    STD   ,Y++                    ; BCCD: ED A1         
                    LDD   #$00                    ; BCCF: CC 00 00      
                    STD   ,Y++                    ; BCD2: ED A1         
                    STU   ,Y++                    ; BCD4: EF A1         
                    LDD   #$BD69                  ; BCD6: CC BD 69      
                    STD   ,Y++                    ; BCD9: ED A1         
                    TFR   U,D                     ; BCDB: 1F 30         
                    SUBD  #$04                    ; BCDD: 83 00 04      
                    ANDB  #$7F                    ; BCE0: C4 7F         
                    TFR   D,U                     ; BCE2: 1F 03         
                    LEAX  -1,X                    ; BCE4: 30 1F         
                    BNE   LBCCA                   ; BCE6: 26 E2         
; End of function sub_BCC8
                    RTS                           ; BCE8: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR Sound_3
Write_Sound         LDB   #$0E                    ; BCE9: C6 0E         
LBCEB               TST   $4401                   ; BCEB: 7D 44 01      
                    BPL   LBCF5                   ; BCEE: 2A 05         
                    DECB                          ; BCF0: 5A            
                    BNE   LBCEB                   ; BCF1: 26 F8         
                    LDA   #$00                    ; BCF3: 86 00         
LBCF5               STA   $4400                   ; BCF5: B7 44 00      
; END OF FUNCTION CHUNK FOR Sound_3
                    RTS                           ; BCF8: 39            

; =============== S U B R O U T I N E =======================================
Sound_1             LDA   #$01                    ; BCF9: 86 01         
; End of function Sound_1
                    JMP   Write_Sound             ; BCFB: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_2             LDA   #$02                    ; BCFE: 86 02         
; End of function Sound_2
                    JMP   Write_Sound             ; BD00: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Attributes: noreturn
; FUNCTION CHUNK AT ROM:BCE9 SIZE 00000010 BYTES
LBD03               LDA   #$03                    ; BD03: 86 03         
; End of function Sound_3
                    JMP   Write_Sound             ; BD05: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_4             LDA   #$04                    ; BD08: 86 04         
; End of function Sound_4
                    JMP   Write_Sound             ; BD0A: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_5             LDA   #$05                    ; BD0D: 86 05         
; End of function Sound_5
                    JMP   Write_Sound             ; BD0F: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_6             LDA   #$06                    ; BD12: 86 06         
; End of function Sound_6
                    JMP   Write_Sound             ; BD14: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_7             LDA   #$07                    ; BD17: 86 07         
; End of function Sound_7
                    JMP   Write_Sound             ; BD19: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_8             LDA   #$08                    ; BD1C: 86 08         
; End of function Sound_8
                    JMP   Write_Sound             ; BD1E: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_9             LDA   #$09                    ; BD21: 86 09         
; End of function Sound_9
                    JMP   Write_Sound             ; BD23: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_A             LDA   #$0A                    ; BD26: 86 0A         
; End of function Sound_A
                    JMP   Write_Sound             ; BD28: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_B             LDA   #$0B                    ; BD2B: 86 0B         
; End of function Sound_B
                    JMP   Write_Sound             ; BD2D: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_C             LDA   #$0C                    ; BD30: 86 0C         
; End of function Sound_C
                    JMP   Write_Sound             ; BD32: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_D             LDA   #$0D                    ; BD35: 86 0D         
; End of function Sound_D
                    JMP   Write_Sound             ; BD37: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_E             LDA   #$0E                    ; BD3A: 86 0E         
; End of function Sound_E
                    JMP   Write_Sound             ; BD3C: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_F             LDA   #$0F                    ; BD3F: 86 0F         
; End of function Sound_F
                    JMP   Write_Sound             ; BD41: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_10            LDA   #$10                    ; BD44: 86 10         
; End of function Sound_10
                    JMP   Write_Sound             ; BD46: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Remember
Sound_11            LDA   #$11                    ; BD49: 86 11         
; End of function Sound_11
                    JMP   Write_Sound             ; BD4B: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_12            LDA   #$12                    ; BD4E: 86 12         
; End of function Sound_12
                    JMP   Write_Sound             ; BD50: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Look at the size of that thing
Sound_13            LDA   #$13                    ; BD53: 86 13         
; End of function Sound_13
                    JMP   Write_Sound             ; BD55: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Stay in attack formation
Sound_14            LDA   #$14                    ; BD58: 86 14         
; End of function Sound_14
                    JMP   Write_Sound             ; BD5A: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_15            LDA   #$15                    ; BD5D: 86 15         
; End of function Sound_15
                    JMP   Write_Sound             ; BD5F: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Force is strong in this one
Sound_16            LDA   #$16                    ; BD62: 86 16         
; End of function Sound_16
                    JMP   Write_Sound             ; BD64: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Red 5 Im going in
Sound_17            LDA   #$17                    ; BD67: 86 17         
; End of function Sound_17
                    JMP   Write_Sound             ; BD69: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Luke trust me
Sound_18            LDA   #$18                    ; BD6C: 86 18         
; End of function Sound_18
                    JMP   Write_Sound             ; BD6E: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_19            LDA   #$19                    ; BD71: 86 19         
; End of function Sound_19
                    JMP   Write_Sound             ; BD73: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Yahoo youre all clear kid
Sound_1A            LDA   #$1A                    ; BD76: 86 1A         
; End of function Sound_1A
                    JMP   Write_Sound             ; BD78: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; High score
Sound_1B            LDA   #$1B                    ; BD7B: 86 1B         
; End of function Sound_1B
                    JMP   Write_Sound             ; BD7D: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_1C            LDA   #$1C                    ; BD80: 86 1C         
; End of function Sound_1C
                    JMP   Write_Sound             ; BD82: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Imperial March
Sound_1D            LDA   #$1D                    ; BD85: 86 1D         
; End of function Sound_1D
                    JMP   Write_Sound             ; BD87: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Enter Death Star
Sound_1E            LDA   #$1E                    ; BD8A: 86 1E         
; End of function Sound_1E
                    JMP   Write_Sound             ; BD8C: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Death Star destroyed
Sound_1F            LDA   #$1F                    ; BD8F: 86 1F         
; End of function Sound_1F
                    JMP   Write_Sound             ; BD91: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Towers 1 music
Sound_20            LDA   #$20                    ; BD94: 86 20         
; End of function Sound_20
                    JMP   Write_Sound             ; BD96: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Towers 2 music
Sound_21            LDA   #$21                    ; BD99: 86 21         
; End of function Sound_21
                    JMP   Write_Sound             ; BD9B: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Trench music
Sound_22            LDA   #$22                    ; BD9E: 86 22         
; End of function Sound_22
                    JMP   Write_Sound             ; BDA0: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_23            LDA   #$23                    ; BDA3: 86 23         
; End of function Sound_23
                    JMP   Write_Sound             ; BDA5: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Space wave 1 music
Sound_24            LDA   #$24                    ; BDA8: 86 24         
; End of function Sound_24
                    JMP   Write_Sound             ; BDAA: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Space Wave 2 music
Sound_25            LDA   #$25                    ; BDAD: 86 25         
; End of function Sound_25
                    JMP   Write_Sound             ; BDAF: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Explosion
Sound_26            LDA   #$26                    ; BDB2: 86 26         
; End of function Sound_26
                    JMP   Write_Sound             ; BDB4: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_27            LDA   #$27                    ; BDB7: 86 27         
; End of function Sound_27
                    JMP   Write_Sound             ; BDB9: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_28            LDA   #$28                    ; BDBC: 86 28         
; End of function Sound_28
                    JMP   Write_Sound             ; BDBE: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_29            LDA   #$29                    ; BDC1: 86 29         
; End of function Sound_29
                    JMP   Write_Sound             ; BDC3: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_2A            LDA   #$2A                    ; BDC6: 86 2A         
; End of function Sound_2A
                    JMP   Write_Sound             ; BDC8: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_2B            LDA   #$2B                    ; BDCB: 86 2B         
; End of function Sound_2B
                    JMP   Write_Sound             ; BDCD: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_2C            LDA   #$2C                    ; BDD0: 86 2C         
; End of function Sound_2C
                    JMP   Write_Sound             ; BDD2: 7E BC E9      

; =============== S U B R O U T I N E =======================================
LBDD5               LDA   #$2D                    ; BDD5: 86 2D         
; End of function Sound_2D
                    JMP   Write_Sound             ; BDD7: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_2E            LDA   #$2E                    ; BDDA: 86 2E         
; End of function Sound_2E
                    JMP   Write_Sound             ; BDDC: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_2F            LDA   #$2F                    ; BDDF: 86 2F         
; End of function Sound_2F
                    JMP   Write_Sound             ; BDE1: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_30            LDA   #$30                    ; BDE4: 86 30         
; End of function Sound_30
                    JMP   Write_Sound             ; BDE6: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_31            LDA   #$31                    ; BDE9: 86 31         
; End of function Sound_31
                    JMP   Write_Sound             ; BDEB: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; R2 beeps entering Death Star
Sound_32            LDA   #$32                    ; BDEE: 86 32         
; End of function Sound_32
                    JMP   Write_Sound             ; BDF0: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_33            LDA   #$33                    ; BDF3: 86 33         
; End of function Sound_33
                    JMP   Write_Sound             ; BDF5: 7E BC E9      

; =============== S U B R O U T I N E =======================================
LBDF8               LDA   #$34                    ; BDF8: 86 34         
; End of function Sound_34
                    JMP   Write_Sound             ; BDFA: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_35            LDA   #$35                    ; BDFD: 86 35         
; End of function Sound_35
                    JMP   Write_Sound             ; BDFF: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_36            LDA   #$36                    ; BE02: 86 36         
; End of function Sound_36
                    JMP   Write_Sound             ; BE04: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_37            LDA   #$37                    ; BE07: 86 37         
; End of function Sound_37
                    JMP   Write_Sound             ; BE09: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_38            LDA   #$38                    ; BE0C: 86 38         
; End of function Sound_38
                    JMP   Write_Sound             ; BE0E: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_39            LDA   #$39                    ; BE11: 86 39         
; End of function Sound_39
                    JMP   Write_Sound             ; BE13: 7E BC E9      

; =============== S U B R O U T I N E =======================================
LBE16               LDA   #$3A                    ; BE16: 86 3A         
; End of function Sound_3A
                    JMP   Write_Sound             ; BE18: 7E BC E9      

; =============== S U B R O U T I N E =======================================
Sound_3B            LDA   #$3B                    ; BE1B: 86 3B         
; End of function Sound_3B
                    JMP   Write_Sound             ; BE1D: 7E BC E9      

; =============== S U B R O U T I N E =======================================
; Display accounting screen
LBE20               LDA   #$59                    ; BE20: 86 59         
LBE22               JSR   LD8DF                   ; BE22: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
LBE25               INCA                          ; BE25: 4C            
                    CMPA  #$65                    ; BE26: 81 65         
                    BCS   LBE22                   ; BE28: 25 F8         
                    LDA   #$D3                    ; BE2A: 86 D3         
                    JSR   LD8DF                   ; BE2C: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
LBE2F               LDD   #$6480                  ; BE2F: CC 64 80      
                    STD   ,Y++                    ; BE32: ED A1         
                    LDX   #$453C                  ; BE34: 8E 45 3C      
                    LDU   #word_C7BB              ; BE37: CE C7 BB      
LBE3A               LDD   ,U++                    ; BE3A: EC C1         
                    STD   ,Y++                    ; BE3C: ED A1         
                    LDD   #$30                    ; BE3E: CC 00 30      
                    STD   ,Y++                    ; BE41: ED A1         
                    STU   $4AFA                   ; BE43: FF 4A FA      
                    LDB   #$05                    ; BE46: C6 05         
                    STB   <$AD                    ; BE48: D7 AD         
                    LDB   #$05                    ; BE4A: C6 05         
LBE4C               LDA   ,X+                     ; BE4C: A6 80         
                    JSR   LE7AD                   ; BE4E: BD E7 AD      
LBE51               DECB                          ; BE51: 5A            
                    BPL   LBE4C                   ; BE52: 2A F8         
                    LDD   #$8040                  ; BE54: CC 80 40      
                    STD   ,Y++                    ; BE57: ED A1         
                    LDU   $4AFA                   ; BE59: FE 4A FA      
                    CMPX  #$4554                  ; BE5C: 8C 45 54      
                    BCS   LBE3A                   ; BE5F: 25 D9         
                    LDD   word_C7C1               ; BE61: FC C7 C1      
                    STD   ,Y++                    ; BE64: ED A1         
                    LDD   #$013C                  ; BE66: CC 01 3C      
                    STD   ,Y++                    ; BE69: ED A1         
                    LDA   #$01                    ; BE6B: 86 01         
                    STA   <$AD                    ; BE6D: 97 AD         
                    LDA   $4588                   ; BE6F: B6 45 88      
                    JSR   LE7AD                   ; BE72: BD E7 AD      
LBE75               LDA   $4589                   ; BE75: B6 45 89      
                    JSR   LE7AD                   ; BE78: BD E7 AD      
LBE7B               LDD   #$8040                  ; BE7B: CC 80 40      
                    STD   ,Y++                    ; BE7E: ED A1         
                    LDX   #$4548                  ; BE80: 8E 45 48      
                    JSR   LC6D4                   ; BE83: BD C6 D4        // Read NOVRAM
LBE86               LDU   #$4AFE                  ; BE86: CE 4A FE      
                    LDX   #$4542                  ; BE89: 8E 45 42      
                    JSR   LC6D7                   ; BE8C: BD C6 D7        // Read NOVRAM
LBE8F               LDA   $4AFC                   ; BE8F: B6 4A FC      
                    ADDA  $4B00                   ; BE92: BB 4B 00      
                    DAA                           ; BE95: 19            
                    STA   $4AFC                   ; BE96: B7 4A FC      
                    LDA   $4AFB                   ; BE99: B6 4A FB      
                    ADCA  $4AFF                   ; BE9C: B9 4A FF      
                    DAA                           ; BE9F: 19            
                    STA   $4AFB                   ; BEA0: B7 4A FB      
                    LDA   $4AFA                   ; BEA3: B6 4A FA      
                    ADCA  $4AFE                   ; BEA6: B9 4A FE      
                    DAA                           ; BEA9: 19            
                    STA   $4AFA                   ; BEAA: B7 4A FA      
                    LDD   word_C7C3               ; BEAD: FC C7 C3      
                    STD   ,Y++                    ; BEB0: ED A1         
                    LDD   #$30                    ; BEB2: CC 00 30      
                    STD   ,Y++                    ; BEB5: ED A1         
                    LDB   #$05                    ; BEB7: C6 05         
                    STB   <$AD                    ; BEB9: D7 AD         
                    LDA   $4AFA                   ; BEBB: B6 4A FA      
                    JSR   Display_Vect_BCD        ; BEBE: BD E7 90        // Display BCD numbers
LBEC1               LDA   $4AFB                   ; BEC1: B6 4A FB      
                    JSR   Display_Vect_BCD        ; BEC4: BD E7 90        // Display BCD numbers
LBEC7               LDA   $4AFC                   ; BEC7: B6 4A FC      
                    JSR   Display_Vect_BCD        ; BECA: BD E7 90        // Display BCD numbers
LBECD               LDD   #$8040                  ; BECD: CC 80 40      
                    STD   ,Y++                    ; BED0: ED A1         
                    LDX   #$4554                  ; BED2: 8E 45 54      
                    JSR   LC6D4                   ; BED5: BD C6 D4        // Read NOVRAM
LBED8               JSR   LC087                   ; BED8: BD C0 87      
LBEDB               LDD   word_C7C9               ; BEDB: FC C7 C9      
                    STD   ,Y++                    ; BEDE: ED A1         
                    LDD   #$30                    ; BEE0: CC 00 30      
                    STD   ,Y++                    ; BEE3: ED A1         
                    LDB   #$07                    ; BEE5: C6 07         
                    STB   <$AD                    ; BEE7: D7 AD         
                    LDA   $4AFA                   ; BEE9: B6 4A FA      
                    JSR   Display_Vect_BCD        ; BEEC: BD E7 90        // Display BCD numbers
LBEEF               LDA   $4AFB                   ; BEEF: B6 4A FB      
                    JSR   Display_Vect_BCD        ; BEF2: BD E7 90        // Display BCD numbers
LBEF5               LDA   $4AFC                   ; BEF5: B6 4A FC      
                    JSR   Display_Vect_BCD        ; BEF8: BD E7 90        // Display BCD numbers
LBEFB               LDA   $4AFD                   ; BEFB: B6 4A FD      
                    JSR   Display_Vect_BCD        ; BEFE: BD E7 90        // Display BCD numbers
LBF01               LDD   #$8040                  ; BF01: CC 80 40      
                    STD   ,Y++                    ; BF04: ED A1         
                    LDX   #$455C                  ; BF06: 8E 45 5C      
                    JSR   LC6D4                   ; BF09: BD C6 D4        // Read NOVRAM
LBF0C               JSR   LC087                   ; BF0C: BD C0 87      
LBF0F               LDD   word_C7CB               ; BF0F: FC C7 CB      
                    STD   ,Y++                    ; BF12: ED A1         
                    LDD   #$30                    ; BF14: CC 00 30      
                    STD   ,Y++                    ; BF17: ED A1         
                    LDB   #$07                    ; BF19: C6 07         
                    STB   <$AD                    ; BF1B: D7 AD         
                    LDA   $4AFA                   ; BF1D: B6 4A FA      
                    JSR   Display_Vect_BCD        ; BF20: BD E7 90        // Display BCD numbers
LBF23               LDA   $4AFB                   ; BF23: B6 4A FB      
                    JSR   Display_Vect_BCD        ; BF26: BD E7 90        // Display BCD numbers
LBF29               LDA   $4AFC                   ; BF29: B6 4A FC      
                    JSR   Display_Vect_BCD        ; BF2C: BD E7 90        // Display BCD numbers
LBF2F               LDA   $4AFD                   ; BF2F: B6 4A FD      
                    JSR   Display_Vect_BCD        ; BF32: BD E7 90        // Display BCD numbers
LBF35               LDD   #$8040                  ; BF35: CC 80 40      
                    STD   ,Y++                    ; BF38: ED A1         
                    LDX   #$454C                  ; BF3A: 8E 45 4C      
                    JSR   LC6D4                   ; BF3D: BD C6 D4        // Read NOVRAM
LBF40               CLR   $4AFA                   ; BF40: 7F 4A FA      
                    LDA   $4AFB                   ; BF43: B6 4A FB      
                    ORA   $4AFC                   ; BF46: BA 4A FC      
                    ORA   $4AFD                   ; BF49: BA 4A FD      
                    BEQ   LBF7C                   ; BF4C: 27 2E         
                    LDX   #$4554                  ; BF4E: 8E 45 54      
                    LDU   #$4AFE                  ; BF51: CE 4A FE      
                    JSR   LC6D7                   ; BF54: BD C6 D7        // Read NOVRAM
LBF57               JSR   LC02F                   ; BF57: BD C0 2F      
LBF5A               LDA   $4AFA                   ; BF5A: B6 4A FA      
                    ANDA  #$0F                    ; BF5D: 84 0F         
                    CMPA  #$0F                    ; BF5F: 81 0F         
                    LDA   $4AFA                   ; BF61: B6 4A FA      
                    BCS   LBF69                   ; BF64: 25 03         
                    INC   $4AFA                   ; BF66: 7C 4A FA      
LBF69               LSRA                          ; BF69: 44            
                    LSRA                          ; BF6A: 44            
                    LSRA                          ; BF6B: 44            
                    LSRA                          ; BF6C: 44            
                    ADDA  $4AFA                   ; BF6D: BB 4A FA      
                    STA   $4AFA                   ; BF70: B7 4A FA      
                    ANDA  #$0F                    ; BF73: 84 0F         
                    CMPA  #$0F                    ; BF75: 81 0F         
                    BCS   LBF7C                   ; BF77: 25 03         
                    INC   $4AFA                   ; BF79: 7C 4A FA      
LBF7C               LDD   word_C7C5               ; BF7C: FC C7 C5      
                    STD   ,Y++                    ; BF7F: ED A1         
                    LDD   #$30                    ; BF81: CC 00 30      
                    STD   ,Y++                    ; BF84: ED A1         
                    LDA   $4AFA                   ; BF86: B6 4A FA      
                    JSR   LC70E                   ; BF89: BD C7 0E      
LBF8C               LDD   #$8040                  ; BF8C: CC 80 40      
                    STD   ,Y++                    ; BF8F: ED A1         
                    LDX   #$455C                  ; BF91: 8E 45 5C      
                    JSR   LC6D4                   ; BF94: BD C6 D4        // Read NOVRAM
LBF97               LDA   $4AFA                   ; BF97: B6 4A FA      
                    ORA   $4AFB                   ; BF9A: BA 4A FB      
                    ORA   $4AFC                   ; BF9D: BA 4A FC      
                    ORA   $4AFD                   ; BFA0: BA 4A FD      
                    BEQ   LBFB4                   ; BFA3: 27 0F         
                    LDX   #$4556                  ; BFA5: 8E 45 56      
                    LDU   #$4AFE                  ; BFA8: CE 4A FE      
                    JSR   LC6D7                   ; BFAB: BD C6 D7        // Read NOVRAM
LBFAE               CLR   $4B01                   ; BFAE: 7F 4B 01      
                    JSR   LC02F                   ; BFB1: BD C0 2F      
LBFB4               LDD   word_C7C7               ; BFB4: FC C7 C7      
                    STD   ,Y++                    ; BFB7: ED A1         
                    LDD   #$30                    ; BFB9: CC 00 30      
                    STD   ,Y++                    ; BFBC: ED A1         
                    CLRA                          ; BFBE: 4F            
                    LDB   $4AFA                   ; BFBF: F6 4A FA      
                    LDX   #$10                    ; BFC2: 8E 00 10      
                    JSR   $7720                   ; BFC5: BD 77 20      
LBFC8               LDB   #$01                    ; BFC8: C6 01         
                    STB   <$AD                    ; BFCA: D7 AD         
                    LDA   $4AD6                   ; BFCC: B6 4A D6      
                    JSR   Display_Vect_BCD        ; BFCF: BD E7 90        // Display BCD numbers
LBFD2               LDD   #$8040                  ; BFD2: CC 80 40      
                    STD   ,Y++                    ; BFD5: ED A1         
                    LDB   #$11                    ; BFD7: C6 11         
                    STB   $4AFA                   ; BFD9: F7 4A FA      
                    LDU   #word_C7CD              ; BFDC: CE C7 CD        // Table for game time history text positions
                    STU   $4AFC                   ; BFDF: FF 4A FC      
LBFE2               LDU   $4AFC                   ; BFE2: FE 4A FC      
                    LDD   ,U++                    ; BFE5: EC C1         
                    STD   ,Y++                    ; BFE7: ED A1         
                    CMPU  #$C7E1                  ; BFE9: 11 83 C7 E1   
                    BCS   LBFF4                   ; BFED: 25 05         
                    LDD   #$1ED4                  ; BFEF: CC 1E D4      
                    BRA   LBFF7                   ; BFF2: 20 03         

; ---------------------------------------------------------------------------
LBFF4               LDD   #$64                    ; BFF4: CC 00 64      
LBFF7               STD   ,Y++                    ; BFF7: ED A1         
                    STU   $4AFC                   ; BFF9: FF 4A FC      
                    LDX   #unk_C7A3               ; BFFC: 8E C7 A3      
                    LDB   $4AFA                   ; BFFF: F6 4A FA      
                    LDA   B,X                     ; C002: A6 85         
                    JSR   LC70E                   ; C004: BD C7 0E      
LC007               LDD   $3002                   ; C007: FC 30 02      
                    STD   ,Y++                    ; C00A: ED A1         
                    STD   ,Y++                    ; C00C: ED A1         
                    LDB   #$00                    ; C00E: C6 00         
                    STB   <$AD                    ; C010: D7 AD         
                    LDX   #$4564                  ; C012: 8E 45 64      
                    LDB   $4AFA                   ; C015: F6 4A FA      
                    ASLB                          ; C018: 58            
                    LDA   B,X                     ; C019: A6 85         
                    JSR   LE7AD                   ; C01B: BD E7 AD      
LC01E               INCB                          ; C01E: 5C            
                    LDA   B,X                     ; C01F: A6 85         
                    JSR   LE7AD                   ; C021: BD E7 AD      
LC024               LDD   #$8040                  ; C024: CC 80 40      
                    STD   ,Y++                    ; C027: ED A1         
                    DEC   $4AFA                   ; C029: 7A 4A FA      
                    BPL   LBFE2                   ; C02C: 2A B4         
; End of function sub_BE20
                    RTS                           ; C02E: 39            

; =============== S U B R O U T I N E =======================================
LC02F               LDX   #$4AFE                  ; C02F: 8E 4A FE      
                    LDA   #$01                    ; C032: 86 01         
                    STA   $4B02                   ; C034: B7 4B 02      
                    LDB   #$03                    ; C037: C6 03         
LC039               LDA   #$99                    ; C039: 86 99         
                    SUBA  ,-X                     ; C03B: A0 82         
                    ADDA  $4B02                   ; C03D: BB 4B 02      
                    DAA                           ; C040: 19            
                    STA   ,X                      ; C041: A7 84         
                    BCS   LC04A                   ; C043: 25 05         
                    CLR   $4B02                   ; C045: 7F 4B 02      
                    BRA   LC04F                   ; C048: 20 05         

; ---------------------------------------------------------------------------
LC04A               LDA   #$01                    ; C04A: 86 01         
                    STA   $4B02                   ; C04C: B7 4B 02      
LC04F               DECB                          ; C04F: 5A            
                    BPL   LC039                   ; C050: 2A E7         
                    LDB   #$FF                    ; C052: C6 FF         
LC054               INCB                          ; C054: 5C            
                    CMPB  #$EF                    ; C055: C1 EF         
                    BEQ   LC083                   ; C057: 27 2A         
                    LDA   $4B01                   ; C059: B6 4B 01      
                    ADDA  $4AFD                   ; C05C: BB 4A FD      
                    DAA                           ; C05F: 19            
                    STA   $4B01                   ; C060: B7 4B 01      
                    LDA   $4B00                   ; C063: B6 4B 00      
                    ADCA  $4AFC                   ; C066: B9 4A FC      
                    DAA                           ; C069: 19            
                    STA   $4B00                   ; C06A: B7 4B 00      
                    LDA   $4AFF                   ; C06D: B6 4A FF      
                    ADCA  $4AFB                   ; C070: B9 4A FB      
                    DAA                           ; C073: 19            
                    STA   $4AFF                   ; C074: B7 4A FF      
                    LDA   $4AFE                   ; C077: B6 4A FE      
                    ADCA  $4AFA                   ; C07A: B9 4A FA      
                    DAA                           ; C07D: 19            
                    STA   $4AFE                   ; C07E: B7 4A FE      
                    BCS   LC054                   ; C081: 25 D1         
LC083               STB   $4AFA                   ; C083: F7 4A FA      
; End of function sub_C02F
                    RTS                           ; C086: 39            

; =============== S U B R O U T I N E =======================================
LC087               BSR   $C089                   ; C087: 8D 00         
LC089               ANDCC #$FE                    ; C089: 1C FE         
                    LDB   #$03                    ; C08B: C6 03         
                    LDX   #$4AFD                  ; C08D: 8E 4A FD      
LC090               LDA   ,X                      ; C090: A6 84         
                    ADCA  ,X                      ; C092: A9 84         
                    DAA                           ; C094: 19            
                    STA   ,X                      ; C095: A7 84         
                    LEAX  -1,X                    ; C097: 30 1F         
                    DECB                          ; C099: 5A            
                    BPL   LC090                   ; C09A: 2A F4         
; End of function sub_C087
                    RTS                           ; C09C: 39            

; =============== S U B R O U T I N E =======================================
LC09D               LDA   #$02                    ; C09D: 86 02         
                    JSR   LC2C3                   ; C09F: BD C2 C3      
LC0A2               BNE   locret_C0FE             ; C0A2: 26 5A         
                    CLRA                          ; C0A4: 4F            
                    LDB   $4815                   ; C0A5: F6 48 15      
                    ASLB                          ; C0A8: 58            
                    ROLA                          ; C0A9: 49            
                    LDB   $4816                   ; C0AA: F6 48 16      
                    ASLB                          ; C0AD: 58            
                    ROLA                          ; C0AE: 49            
                    LDB   $4817                   ; C0AF: F6 48 17      
                    ASLB                          ; C0B2: 58            
                    ROLA                          ; C0B3: 49            
                    TFR   A,B                     ; C0B4: 1F 89         
                    EORB  $4AF4                   ; C0B6: F8 4A F4      
                    ANDB  $4AF4                   ; C0B9: F4 4A F4      
                    STA   $4AF4                   ; C0BC: B7 4A F4      
                    LDX   #$4548                  ; C0BF: 8E 45 48      
LC0C2               LSRB                          ; C0C2: 54            
                    BCC   LC0F7                   ; C0C3: 24 32         
                    LDU   #$4B5F                  ; C0C5: CE 4B 5F      
                    JSR   LC6D7                   ; C0C8: BD C6 D7        // Read NOVRAM
LC0CB               LDA   $4B61                   ; C0CB: B6 4B 61      
                    ADDA  #$01                    ; C0CE: 8B 01         
                    DAA                           ; C0D0: 19            
                    STA   $4B61                   ; C0D1: B7 4B 61      
                    LDA   $4B60                   ; C0D4: B6 4B 60      
                    ADCA  #$00                    ; C0D7: 89 00         
                    DAA                           ; C0D9: 19            
                    STA   $4B60                   ; C0DA: B7 4B 60      
                    LDA   $4B5F                   ; C0DD: B6 4B 5F      
                    ADCA  #$00                    ; C0E0: 89 00         
                    DAA                           ; C0E2: 19            
                    STA   $4B5F                   ; C0E3: B7 4B 5F      
                    LDU   #$4B5F                  ; C0E6: CE 4B 5F      
                    JSR   LC6F7                   ; C0E9: BD C6 F7      
LC0EC               LDA   #$02                    ; C0EC: 86 02         
                    STB   $4B62                   ; C0EE: F7 4B 62      
                    JSR   LC2B3                   ; C0F1: BD C2 B3      
LC0F4               LDB   $4B62                   ; C0F4: F6 4B 62      
LC0F7               LEAX  -6,X                    ; C0F7: 30 1A         
                    CMPX  #$453C                  ; C0F9: 8C 45 3C      
                    BCC   LC0C2                   ; C0FC: 24 C4         
; End of function sub_C09D
locret_C0FE         RTS                           ; C0FE: 39            

; =============== S U B R O U T I N E =======================================
; NVRAM something
LC0FF               LDA   #$02                    ; C0FF: 86 02         
                    JSR   LC413                   ; C101: BD C4 13      
LC104               LDX   #$4554                  ; C104: 8E 45 54      
                    JSR   LC6D4                   ; C107: BD C6 D4        // Read NOVRAM
LC10A               LDA   $4AFD                   ; C10A: B6 4A FD      
                    ADDA  $481A                   ; C10D: BB 48 1A      
                    DAA                           ; C110: 19            
                    STA   $4AFD                   ; C111: B7 4A FD      
                    LDA   $4AFC                   ; C114: B6 4A FC      
                    ADCA  $4819                   ; C117: B9 48 19      
                    DAA                           ; C11A: 19            
                    STA   $4AFC                   ; C11B: B7 4A FC      
                    LDA   $4AFB                   ; C11E: B6 4A FB      
                    ADCA  #$00                    ; C121: 89 00         
                    DAA                           ; C123: 19            
                    STA   $4AFB                   ; C124: B7 4A FB      
                    LDA   $4AFA                   ; C127: B6 4A FA      
                    ADCA  #$00                    ; C12A: 89 00         
                    DAA                           ; C12C: 19            
                    BCS   LC132                   ; C12D: 25 03         
                    STA   $4AFA                   ; C12F: B7 4A FA      
LC132               LDA   #$03                    ; C132: 86 03         
                    STB   $4B02                   ; C134: F7 4B 02      
                    LDU   #$4AFA                  ; C137: CE 4A FA      
                    JSR   LC6F9                   ; C13A: BD C6 F9      
LC13D               LDX   #$454E                  ; C13D: 8E 45 4E      
                    JSR   LC6D4                   ; C140: BD C6 D4        // Read NOVRAM
LC143               LDA   $4AFC                   ; C143: B6 4A FC      
                    ADDA  #$01                    ; C146: 8B 01         
                    DAA                           ; C148: 19            
                    STA   $4AFC                   ; C149: B7 4A FC      
                    LDA   $4AFB                   ; C14C: B6 4A FB      
                    ADCA  #$00                    ; C14F: 89 00         
                    DAA                           ; C151: 19            
                    STA   $4AFB                   ; C152: B7 4A FB      
                    LDA   $4AFA                   ; C155: B6 4A FA      
                    ADCA  #$00                    ; C158: 89 00         
                    DAA                           ; C15A: 19            
                    STA   $4AFA                   ; C15B: B7 4A FA      
                    JSR   LC6F4                   ; C15E: BD C6 F4      
LC161               LDX   #$4588                  ; C161: 8E 45 88      
                    JSR   LC6D4                   ; C164: BD C6 D4        // Read NOVRAM
LC167               LDA   $4B16                   ; C167: B6 4B 16      
                    CMPA  $4AFA                   ; C16A: B1 4A FA      
                    BLS   LC17B                   ; C16D: 23 0C         
                    STA   $4AFA                   ; C16F: B7 4A FA      
                    CLR   $4AFB                   ; C172: 7F 4A FB      
                    CLR   $4AFC                   ; C175: 7F 4A FC      
                    JSR   LC6F4                   ; C178: BD C6 F4      
LC17B               LDX   #$4586                  ; C17B: 8E 45 86      
                    LDA   $4819                   ; C17E: B6 48 19      
                    BNE   LC19E                   ; C181: 26 1B         
                    LDA   $481A                   ; C183: B6 48 1A      
                    LDX   #$4564                  ; C186: 8E 45 64      
                    LDB   #$9A                    ; C189: C6 9A         
                    SUBB  word_C7A4               ; C18B: F0 C7 A4      
                    STB   $4AFA                   ; C18E: F7 4A FA      
LC191               ADDA  $4AFA                   ; C191: BB 4A FA      
                    DAA                           ; C194: 19            
                    BCC   LC19E                   ; C195: 24 07         
                    LEAX  $02,X                   ; C197: 30 02         
                    CMPX  #$4586                  ; C199: 8C 45 86      
                    BCS   LC191                   ; C19C: 25 F3         
LC19E               LDA   $01,X                   ; C19E: A6 01         
                    ANDA  #$0F                    ; C1A0: 84 0F         
                    ADDA  #$01                    ; C1A2: 8B 01         
                    DAA                           ; C1A4: 19            
                    STA   $01,X                   ; C1A5: A7 01         
                    ANDA  #$F0                    ; C1A7: 84 F0         
                    BEQ   LC1E9                   ; C1A9: 27 3E         
                    LDA   ,X                      ; C1AB: A6 84         
                    ANDA  #$0F                    ; C1AD: 84 0F         
                    ADDA  #$01                    ; C1AF: 8B 01         
                    DAA                           ; C1B1: 19            
                    STA   ,X                      ; C1B2: A7 84         
                    ANDA  #$F0                    ; C1B4: 84 F0         
                    BEQ   LC1E9                   ; C1B6: 27 31         
                    LDX   #$4564                  ; C1B8: 8E 45 64      
LC1BB               LDA   ,X                      ; C1BB: A6 84         
                    ASLA                          ; C1BD: 48            
                    ASLA                          ; C1BE: 48            
                    ASLA                          ; C1BF: 48            
                    ASLA                          ; C1C0: 48            
                    STA   $4AFA                   ; C1C1: B7 4A FA      
                    LDA   $01,X                   ; C1C4: A6 01         
                    ANDA  #$0F                    ; C1C6: 84 0F         
                    ADDA  $4AFA                   ; C1C8: BB 4A FA      
                    BITA  #$10                    ; C1CB: 85 10         
                    BEQ   LC1D1                   ; C1CD: 27 02         
                    SUBA  #$06                    ; C1CF: 80 06         
LC1D1               LSRA                          ; C1D1: 44            
                    STA   $01,X                   ; C1D2: A7 01         
                    LSRA                          ; C1D4: 44            
                    LSRA                          ; C1D5: 44            
                    LSRA                          ; C1D6: 44            
                    LSRA                          ; C1D7: 44            
                    STA   ,X                      ; C1D8: A7 84         
                    LEAX  $02,X                   ; C1DA: 30 02         
                    CMPX  #$4588                  ; C1DC: 8C 45 88      
                    BCS   LC1BB                   ; C1DF: 25 DA         
                    LDA   #$05                    ; C1E1: 86 05         
                    STA   ,U                      ; C1E3: A7 C4         
                    LDA   #$00                    ; C1E5: 86 00         
                    STA   $01,U                   ; C1E7: A7 41         
LC1E9               LDA   $4866                   ; C1E9: B6 48 66        // Joystick pitch data structure
                    STA   $4AFA                   ; C1EC: B7 4A FA      
                    LDA   $4868                   ; C1EF: B6 48 68      
                    STA   $4AFB                   ; C1F2: B7 4A FB      
                    LDA   $486F                   ; C1F5: B6 48 6F        // Joystick roll data structure
                    STA   $4AFC                   ; C1F8: B7 4A FC      
                    LDA   $4871                   ; C1FB: B6 48 71      
                    STA   $4AFD                   ; C1FE: B7 4A FD      
                    LDX   #$4534                  ; C201: 8E 45 34      
                    LDU   #$4AFA                  ; C204: CE 4A FA      
                    LDA   #$03                    ; C207: 86 03         
; End of function sub_C0FF
                    JSR   LC6F9                   ; C209: BD C6 F9      
; =============== S U B R O U T I N E =======================================
LC20C               LDX   #$455C                  ; C20C: 8E 45 5C      
                    JSR   LC6D4                   ; C20F: BD C6 D4        // Read NOVRAM
LC212               ORCC  #$10                    ; C212: 1A 10         
                    LDU   $4B06                   ; C214: FE 4B 06      
                    LDD   $4B04                   ; C217: FC 4B 04      
                    ANDCC #$EF                    ; C21A: 1C EF         
                    CMPD  $4AFA                   ; C21C: 10 B3 4A FA   
                    BHI   LC22A                   ; C220: 22 08         
                    BCS   LC23D                   ; C222: 25 19         
                    CMPU  $4AFC                   ; C224: 11 B3 4A FC   
                    BLS   LC23D                   ; C228: 23 13         
LC22A               STD   $4AFA                   ; C22A: FD 4A FA      
                    STU   $4AFC                   ; C22D: FF 4A FC      
                    LDX   #$455C                  ; C230: 8E 45 5C      
                    LDU   #$4AFA                  ; C233: CE 4A FA      
                    LDA   #$03                    ; C236: 86 03         
                    JSR   LC6F9                   ; C238: BD C6 F9      
LC23B               BRA   LC249                   ; C23B: 20 0C         

; ---------------------------------------------------------------------------
LC23D               LDD   $4AFA                   ; C23D: FC 4A FA      
                    STD   $4B04                   ; C240: FD 4B 04      
                    LDD   $4AFC                   ; C243: FC 4A FC      
                    STD   $4B06                   ; C246: FD 4B 06      
LC249               LDA   #$02                    ; C249: 86 02         
; End of function sub_C20C
                    JMP   LC2B3                   ; C24B: 7E C2 B3      

; =============== S U B R O U T I N E =======================================
LC24E               CMPA  #$03                    ; C24E: 81 03         
                    BCC   LC2B3                   ; C250: 24 61         
                    CMPA  #$02                    ; C252: 81 02         
                    BCS   LC27F                   ; C254: 25 29         
                    LDX   #word_C7B7              ; C256: 8E C7 B7      
                    LDB   A,X                     ; C259: E6 86         
                    SUBB  #$02                    ; C25B: C0 02         
                    STB   $4AFB                   ; C25D: F7 4A FB      
                    LDB   #$45                    ; C260: C6 45         
                    STB   $4AFA                   ; C262: F7 4A FA      
                    LDX   #unk_C7B6               ; C265: 8E C7 B6      
                    LDB   A,X                     ; C268: E6 86         
                    LDX   #$C707                  ; C26A: 8E C7 07      
                    ABX                           ; C26D: 3A            
                    TFR   X,U                     ; C26E: 1F 13         
                    LDX   #$4500                  ; C270: 8E 45 00        // NOVRAM
                    ABX                           ; C273: 3A            
LC274               LDB   ,U+                     ; C274: E6 C0         
                    STB   ,X+                     ; C276: E7 80         
                    CPX   $4AFA                   ; C278: BC 4A FA      
                    BCS   LC274                   ; C27B: 25 F7         
                    BRA   LC2B3                   ; C27D: 20 34         

; ---------------------------------------------------------------------------
LC27F               TFR   A,B                     ; C27F: 1F 89         
                    TSTA                          ; C281: 4D            
                    BNE   LC29B                   ; C282: 26 17         
                    LDA   <$27                    ; C284: 96 27         
                    STA   $4AFA                   ; C286: B7 4A FA      
                    LDA   <$24                    ; C289: 96 24         
                    STA   $4AFB                   ; C28B: B7 4A FB      
                    LDA   #$00                    ; C28E: 86 00         
                    STA   $4AFC                   ; C290: B7 4A FC      
                    LDX   #$4500                  ; C293: 8E 45 00        // NOVRAM
                    JSR   LC6F4                   ; C296: BD C6 F4      
LC299               BRA   LC2B1                   ; C299: 20 16         

; ---------------------------------------------------------------------------
LC29B               LDX   #$4508                  ; C29B: 8E 45 08      
                    LDU   #word_CC98              ; C29E: CE CC 98        // High scores init table
                    LDA   #$0B                    ; C2A1: 86 0B         
                    JSR   LC6F9                   ; C2A3: BD C6 F9      
LC2A6               LDX   #$4520                  ; C2A6: 8E 45 20      
                    LDU   #word_CC7A              ; C2A9: CE CC 7A        // High score names
                    LDA   #$08                    ; C2AC: 86 08         
                    JSR   LC6F9                   ; C2AE: BD C6 F9      
; End of function sub_C24E
LC2B1               TFR   B,A                     ; C2B1: 1F 98         
; =============== S U B R O U T I N E =======================================
LC2B3               JSR   LC2C3                   ; C2B3: BD C2 C3      
LC2B6               BEQ   locret_C2C2             ; C2B6: 27 0A         
                    STB   $01,X                   ; C2B8: E7 01         
                    LSRB                          ; C2BA: 54            
                    LSRB                          ; C2BB: 54            
                    LSRB                          ; C2BC: 54            
                    LSRB                          ; C2BD: 54            
                    STB   ,X                      ; C2BE: E7 84         
                    LDB   #$FF                    ; C2C0: C6 FF         
; End of function sub_C2B3
locret_C2C2         RTS                           ; C2C2: 39            

; =============== S U B R O U T I N E =======================================
LC2C3               LEAS  -3,S                    ; C2C3: 32 7D         
                    LDX   #word_C7B7              ; C2C5: 8E C7 B7      
                    LDB   A,X                     ; C2C8: E6 86         
                    LDX   #unk_C7B6               ; C2CA: 8E C7 B6      
                    SUBB  A,X                     ; C2CD: E0 86         
                    SUBB  #$02                    ; C2CF: C0 02         
                    STB   ,S                      ; C2D1: E7 E4         
                    LDB   A,X                     ; C2D3: E6 86         
                    LDX   #$4500                  ; C2D5: 8E 45 00        // NOVRAM
                    ABX                           ; C2D8: 3A            
                    LDB   #$00                    ; C2D9: C6 00         
                    STB   $01,S                   ; C2DB: E7 61         
                    ORCC  #$01                    ; C2DD: 1A 01         
LC2DF               LDB   ,X+                     ; C2DF: E6 80         
                    ANDB  #$0F                    ; C2E1: C4 0F         
                    ADCB  $01,S                   ; C2E3: E9 61         
                    STB   $01,S                   ; C2E5: E7 61         
                    DEC   ,S                      ; C2E7: 6A E4         
                    BNE   LC2DF                   ; C2E9: 26 F4         
                    ADCB  #$00                    ; C2EB: C9 00         
                    STB   $01,S                   ; C2ED: E7 61         
                    LDB   ,X                      ; C2EF: E6 84         
                    ASLB                          ; C2F1: 58            
                    ASLB                          ; C2F2: 58            
                    ASLB                          ; C2F3: 58            
                    ASLB                          ; C2F4: 58            
                    STB   $02,S                   ; C2F5: E7 62         
                    LDB   $01,X                   ; C2F7: E6 01         
                    ANDB  #$0F                    ; C2F9: C4 0F         
                    ADDB  $02,S                   ; C2FB: EB 62         
                    STB   $02,S                   ; C2FD: E7 62         
                    LDB   $01,S                   ; C2FF: E6 61         
                    CMPB  $02,S                   ; C301: E1 62         
                    LEAS  $03,S                   ; C303: 32 63         
; End of function sub_C2C3
                    RTS                           ; C305: 39            

; =============== S U B R O U T I N E =======================================
LC306               LDX   #$4500                  ; C306: 8E 45 00        // NOVRAM
                    LDU   #$4C00                  ; C309: CE 4C 00      
LC30C               LDD   ,X++                    ; C30C: EC 81         
                    STD   ,U++                    ; C30E: ED C1         
                    CMPX  #$4600                  ; C310: 8C 46 00        // NOVRAM
                    BCS   LC30C                   ; C313: 25 F7         
                    LDA   #$FF                    ; C315: 86 FF         
                    STA   $4687                   ; C317: B7 46 87      
                    LDX   #$0100                  ; C31A: 8E 01 00      
LC31D               STA   $4640                   ; C31D: B7 46 40      
                    LEAX  -1,X                    ; C320: 30 1F         
                    BNE   LC31D                   ; C322: 26 F9         
                    LDA   #$00                    ; C324: 86 00         
                    STA   $4687                   ; C326: B7 46 87      
                    LDX   #$A000                  ; C329: 8E A0 00      
LC32C               STA   $4640                   ; C32C: B7 46 40      
                    LEAX  -1,X                    ; C32F: 30 1F         
                    BNE   LC32C                   ; C331: 26 F9         
                    LDX   #$4500                  ; C333: 8E 45 00        // NOVRAM
                    LDY   #$4C00                  ; C336: 10 8E 4C 00   
LC33A               LDU   ,X                      ; C33A: EE 84         
                    LDD   ,Y                      ; C33C: EC A4         
                    STU   ,Y++                    ; C33E: EF A1         
                    STD   ,X++                    ; C340: ED 81         
                    CMPX  #$4600                  ; C342: 8C 46 00        // NOVRAM
                    BCS   LC33A                   ; C345: 25 F3         
                    LDA   #$03                    ; C347: 86 03         
                    JSR   LC2C3                   ; C349: BD C2 C3      
LC34C               BEQ   LC37C                   ; C34C: 27 2E         
                    LDA   #$00                    ; C34E: 86 00         
                    JSR   LC2C3                   ; C350: BD C2 C3      
LC353               BEQ   LC36C                   ; C353: 27 17         
                    JSR   LC3EE                   ; C355: BD C3 EE      
LC358               LDA   #$03                    ; C358: 86 03         
                    JSR   LC3EE                   ; C35A: BD C3 EE      
LC35D               JSR   LC2C3                   ; C35D: BD C2 C3      
LC360               BEQ   LC37C                   ; C360: 27 1A         
                    LDA   #$00                    ; C362: 86 00         
                    JSR   LC2C3                   ; C364: BD C2 C3      
LC367               BEQ   LC36C                   ; C367: 27 03         
LC369               JSR   LC24E                   ; C369: BD C2 4E      
LC36C               LDX   #unk_C7B6               ; C36C: 8E C7 B6      
                    LDB   $0003,X                 ; C36F: E6 89 00 03   
                    LDX   #$4500                  ; C373: 8E 45 00        // NOVRAM
                    ABX                           ; C376: 3A            
                    LDU   #$4500                  ; C377: CE 45 00        // NOVRAM
                    BRA   LC3B7                   ; C37A: 20 3B         

; ---------------------------------------------------------------------------
LC37C               LDA   #$00                    ; C37C: 86 00         
                    JSR   LC2C3                   ; C37E: BD C2 C3      
LC381               BNE   LC3A7                   ; C381: 26 24         
                    LDA   $4596                   ; C383: B6 45 96      
                    ANDA  #$0F                    ; C386: 84 0F         
                    STA   $4AFA                   ; C388: B7 4A FA      
                    LDA   $4506                   ; C38B: B6 45 06      
                    ANDA  #$0F                    ; C38E: 84 0F         
                    CMPA  $4AFA                   ; C390: B1 4A FA      
                    BNE   LC3A5                   ; C393: 26 10         
                    LDA   $4597                   ; C395: B6 45 97      
                    ANDA  #$0F                    ; C398: 84 0F         
                    STA   $4AFA                   ; C39A: B7 4A FA      
                    LDA   $4507                   ; C39D: B6 45 07      
                    ANDA  #$0F                    ; C3A0: 84 0F         
                    CMPA  $4AFA                   ; C3A2: B1 4A FA      
LC3A5               BEQ   LC3C6                   ; C3A5: 27 1F         
LC3A7               LDX   #unk_C7B6               ; C3A7: 8E C7 B6      
                    LDB   $0003,X                 ; C3AA: E6 89 00 03   
                    LDX   #$4500                  ; C3AE: 8E 45 00        // NOVRAM
                    ABX                           ; C3B1: 3A            
                    TFR   X,U                     ; C3B2: 1F 13         
                    LDX   #$4500                  ; C3B4: 8E 45 00        // NOVRAM
LC3B7               LDB   word_C7B7               ; C3B7: F6 C7 B7      
                    STB   $4B02                   ; C3BA: F7 4B 02      
LC3BD               LDB   ,U+                     ; C3BD: E6 C0         
                    STB   ,X+                     ; C3BF: E7 80         
                    DEC   $4B02                   ; C3C1: 7A 4B 02      
                    BNE   LC3BD                   ; C3C4: 26 F7         
LC3C6               LDA   #$02                    ; C3C6: 86 02         
LC3C8               JSR   LC2C3                   ; C3C8: BD C2 C3      
LC3CB               BEQ   LC3D8                   ; C3CB: 27 0B         
                    JSR   LC3EE                   ; C3CD: BD C3 EE      
LC3D0               JSR   LC2C3                   ; C3D0: BD C2 C3      
LC3D3               BEQ   LC3D8                   ; C3D3: 27 03         
                    JSR   LC24E                   ; C3D5: BD C2 4E      
LC3D8               DECA                          ; C3D8: 4A            
                    BNE   LC3C8                   ; C3D9: 26 ED         
                    LDX   #$455C                  ; C3DB: 8E 45 5C      
                    JSR   LC6D4                   ; C3DE: BD C6 D4        // Read NOVRAM
LC3E1               LDD   $4AFA                   ; C3E1: FC 4A FA      
                    STD   $4B04                   ; C3E4: FD 4B 04      
                    LDD   $4AFC                   ; C3E7: FC 4A FC      
                    STD   $4B06                   ; C3EA: FD 4B 06      
; End of function sub_C306
                    RTS                           ; C3ED: 39            

; =============== S U B R O U T I N E =======================================
LC3EE               LDX   #word_C7B7              ; C3EE: 8E C7 B7      
                    LDB   A,X                     ; C3F1: E6 86         
                    LDX   #$4500                  ; C3F3: 8E 45 00        // NOVRAM
                    ABX                           ; C3F6: 3A            
                    STX   $4AFA                   ; C3F7: BF 4A FA      
                    LDX   #unk_C7B6               ; C3FA: 8E C7 B6      
                    LDB   A,X                     ; C3FD: E6 86         
                    LDX   #$4C00                  ; C3FF: 8E 4C 00      
                    ABX                           ; C402: 3A            
                    TFR   X,U                     ; C403: 1F 13         
                    LDX   #$4500                  ; C405: 8E 45 00        // NOVRAM
                    ABX                           ; C408: 3A            
LC409               LDB   ,U+                     ; C409: E6 C0         
                    STB   ,X+                     ; C40B: E7 80         
                    CPX   $4AFA                   ; C40D: BC 4A FA      
                    BCS   LC409                   ; C410: 25 F7         
; End of function sub_C3EE
                    RTS                           ; C412: 39            

; =============== S U B R O U T I N E =======================================
LC413               JSR   LC2C3                   ; C413: BD C2 C3      
LC416               BEQ   locret_C44F             ; C416: 27 37         
                    STA   $4AFA                   ; C418: B7 4A FA      
                    LDX   #$4500                  ; C41B: 8E 45 00        // NOVRAM
                    LDU   #$4C00                  ; C41E: CE 4C 00      
LC421               LDD   ,X++                    ; C421: EC 81         
                    STD   ,U++                    ; C423: ED C1         
                    CMPX  #$4600                  ; C425: 8C 46 00        // NOVRAM
                    BCS   LC421                   ; C428: 25 F7         
                    JSR   LC6B8                   ; C42A: BD C6 B8      
LC42D               LDX   #$4500                  ; C42D: 8E 45 00        // NOVRAM
                    LDY   #$4C00                  ; C430: 10 8E 4C 00   
LC434               LDU   ,X                      ; C434: EE 84         
                    LDD   ,Y                      ; C436: EC A4         
                    STU   ,Y++                    ; C438: EF A1         
                    STD   ,X++                    ; C43A: ED 81         
                    CMPX  #$4600                  ; C43C: 8C 46 00        // NOVRAM
                    BCS   LC434                   ; C43F: 25 F3         
                    LDA   $4AFA                   ; C441: B6 4A FA      
                    JSR   LC3EE                   ; C444: BD C3 EE      
LC447               JSR   LC2C3                   ; C447: BD C2 C3      
LC44A               BEQ   locret_C44F             ; C44A: 27 03         
                    JSR   LC24E                   ; C44C: BD C2 4E      
; End of function sub_C413
locret_C44F         RTS                           ; C44F: 39            

; =============== S U B R O U T I N E =======================================
LC450               LDA   #$65                    ; C450: 86 65         
LC452               JSR   LD8DF                   ; C452: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
LC455               INCA                          ; C455: 4C            
                    CMPA  #$74                    ; C456: 81 74         
                    BCS   LC452                   ; C458: 25 F8         
                    LDA   #$D4                    ; C45A: 86 D4         
                    JSR   LD8DF                   ; C45C: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
LC45F               LDD   #$6280                  ; C45F: CC 62 80      
                    STD   ,Y++                    ; C462: ED A1         
                    LDA   #$0B                    ; C464: 86 0B         
                    STA   $4AFE                   ; C466: B7 4A FE      
LC469               LDA   $4AFE                   ; C469: B6 4A FE      
                    JSR   LC5A4                   ; C46C: BD C5 A4      
LC46F               DEC   $4AFE                   ; C46F: 7A 4A FE      
                    BPL   LC469                   ; C472: 2A F5         
                    JSR   LC690                   ; C474: BD C6 90      
LC477               LDA   $4598                   ; C477: B6 45 98      
                    ANDA  #$0F                    ; C47A: 84 0F         
                    BEQ   locret_C4EA             ; C47C: 27 6C         
                    LDB   #$D5                    ; C47E: C6 D5         
                    JSR   LE7C7                   ; C480: BD E7 C7        // Print text string from pointer table
LC483               LDA   <$AC                    ; C483: 96 AC         
                    ANDA  #$40                    ; C485: 84 40         
                    BEQ   locret_C4EA             ; C487: 27 61         
                    LDA   $4598                   ; C489: B6 45 98      
                    ANDA  #$08                    ; C48C: 84 08         
                    BEQ   LC498                   ; C48E: 27 08         
                    LDA   #$01                    ; C490: 86 01         
                    JSR   LC24E                   ; C492: BD C2 4E      
LC495               JSR   LCC18                   ; C495: BD CC 18      
LC498               LDA   $4598                   ; C498: B6 45 98      
                    ANDA  #$04                    ; C49B: 84 04         
                    BEQ   LC4B1                   ; C49D: 27 12         
                    LDX   #$454E                  ; C49F: 8E 45 4E      
                    LDD   #$00                    ; C4A2: CC 00 00      
LC4A5               STD   ,X++                    ; C4A5: ED 81         
                    CMPX  #$458E                  ; C4A7: 8C 45 8E      
                    BCS   LC4A5                   ; C4AA: 25 F9         
                    LDA   #$02                    ; C4AC: 86 02         
                    JSR   LC2B3                   ; C4AE: BD C2 B3      
LC4B1               LDA   $4598                   ; C4B1: B6 45 98      
                    ANDA  #$02                    ; C4B4: 84 02         
                    BEQ   LC4DA                   ; C4B6: 27 22         
                    LDA   #$00                    ; C4B8: 86 00         
                    JSR   LC24E                   ; C4BA: BD C2 4E      
LC4BD               LDX   #unk_C7B6               ; C4BD: 8E C7 B6      
                    LDB   $0003,X                 ; C4C0: E6 89 00 03   
                    LDX   #$4500                  ; C4C4: 8E 45 00        // NOVRAM
                    ABX                           ; C4C7: 3A            
                    LDU   #$4500                  ; C4C8: CE 45 00        // NOVRAM
                    LDB   word_C7B7               ; C4CB: F6 C7 B7      
                    STB   $4B02                   ; C4CE: F7 4B 02      
LC4D1               LDB   ,U+                     ; C4D1: E6 C0         
                    STB   ,X+                     ; C4D3: E7 80         
                    DEC   $4B02                   ; C4D5: 7A 4B 02      
                    BNE   LC4D1                   ; C4D8: 26 F7         
LC4DA               LDA   $4598                   ; C4DA: B6 45 98      
                    ANDA  #$01                    ; C4DD: 84 01         
                    BEQ   LC4E4                   ; C4DF: 27 03         
                    JSR   LC5F2                   ; C4E1: BD C5 F2      
LC4E4               LDD   #$00                    ; C4E4: CC 00 00      
                    STD   $4598                   ; C4E7: FD 45 98      
; End of function sub_C450
locret_C4EA         RTS                           ; C4EA: 39            

; =============== S U B R O U T I N E =======================================
LC4EB               LDB   <$43                    ; C4EB: D6 43           // Game over/insert coins timer
                    ANDB  #$0F                    ; C4ED: C4 0F         
                    BNE   LC519                   ; C4EF: 26 28         
                    LDB   $487F                   ; C4F1: F6 48 7F      
                    CMPB  #$D0                    ; C4F4: C1 D0         
                    BCC   LC519                   ; C4F6: 24 21         
                    CMPB  #$30                    ; C4F8: C1 30         
                    BLS   LC519                   ; C4FA: 23 1D         
                    TSTB                          ; C4FC: 5D            
                    BMI   LC50C                   ; C4FD: 2B 0D         
                    LDB   $4AF6                   ; C4FF: F6 4A F6      
                    DECB                          ; C502: 5A            
                    BPL   LC507                   ; C503: 2A 02         
                    LDB   #$0B                    ; C505: C6 0B         
LC507               STB   $4AF6                   ; C507: F7 4A F6      
                    BRA   LC519                   ; C50A: 20 0D         

; ---------------------------------------------------------------------------
LC50C               LDB   $4AF6                   ; C50C: F6 4A F6      
                    INCB                          ; C50F: 5C            
                    CMPB  #$0B                    ; C510: C1 0B         
                    BLS   LC516                   ; C512: 23 02         
                    LDB   #$00                    ; C514: C6 00         
LC516               STB   $4AF6                   ; C516: F7 4A F6      
LC519               LDX   #byte_C797              ; C519: 8E C7 97      
                    LDA   $4AF6                   ; C51C: B6 4A F6      
                    LDB   A,X                     ; C51F: E6 86         
                    STB   $4AFC                   ; C521: F7 4A FC      
                    LSRB                          ; C524: 54            
                    LSRB                          ; C525: 54            
                    LSRB                          ; C526: 54            
                    ANDB  #$03                    ; C527: C4 03         
                    LDX   #byte_C737              ; C529: 8E C7 37      
                    LDA   B,X                     ; C52C: A6 85         
                    STA   $4AFA                   ; C52E: B7 4A FA      
                    LDB   <$AC                    ; C531: D6 AC         
                    ANDB  #$80                    ; C533: C4 80         
                    BEQ   locret_C5A3             ; C535: 27 6C         
                    LDB   $4AF5                   ; C537: F6 4A F5      
                    INCB                          ; C53A: 5C            
                    CMPB  $4AFA                   ; C53B: F1 4A FA      
                    BLS   LC542                   ; C53E: 23 02         
                    LDB   #$00                    ; C540: C6 00         
LC542               STA   $4AF5                   ; C542: B7 4A F5      
                    LDA   $4AFC                   ; C545: B6 4A FC      
                    ROLA                          ; C548: 49            
                    ROLA                          ; C549: 49            
                    ROLA                          ; C54A: 49            
                    ROLA                          ; C54B: 49            
                    ANDA  #$07                    ; C54C: 84 07         
LC54E               DECA                          ; C54E: 4A            
                    BMI   LC557                   ; C54F: 2B 06         
                    ASLB                          ; C551: 58            
                    ASL   $4AFA                   ; C552: 78 4A FA      
                    BRA   LC54E                   ; C555: 20 F7         

; ---------------------------------------------------------------------------
LC557               LDA   $4AFC                   ; C557: B6 4A FC      
                    ANDA  #$07                    ; C55A: 84 07         
                    ASLA                          ; C55C: 48            
                    LDX   #$4590                  ; C55D: 8E 45 90      
                    LEAX  A,X                     ; C560: 30 86         
                    LDA   ,X                      ; C562: A6 84         
                    ASLA                          ; C564: 48            
                    ASLA                          ; C565: 48            
                    ASLA                          ; C566: 48            
                    ASLA                          ; C567: 48            
                    STA   $4AFB                   ; C568: B7 4A FB      
                    LDA   $01,X                   ; C56B: A6 01         
                    ANDA  #$0F                    ; C56D: 84 0F         
                    ADDA  $4AFB                   ; C56F: BB 4A FB      
                    STA   $4AFB                   ; C572: B7 4A FB      
                    EORB  $4AFB                   ; C575: F8 4A FB      
                    ANDB  $4AFA                   ; C578: F4 4A FA      
                    EORB  $4AFB                   ; C57B: F8 4A FB      
                    STB   $01,X                   ; C57E: E7 01         
                    LSRB                          ; C580: 54            
                    LSRB                          ; C581: 54            
                    LSRB                          ; C582: 54            
                    LSRB                          ; C583: 54            
                    STB   ,X                      ; C584: E7 84         
                    LDA   #$03                    ; C586: 86 03         
                    JSR   LC2B3                   ; C588: BD C2 B3      
LC58B               LDX   #$4500                  ; C58B: 8E 45 00        // NOVRAM
                    LDU   #$4C00                  ; C58E: CE 4C 00      
LC591               LDD   ,X++                    ; C591: EC 81         
                    STD   ,U++                    ; C593: ED C1         
                    CMPX  #$45FF                  ; C595: 8C 45 FF        // NOVRAM
                    BCS   LC591                   ; C598: 25 F7         
                    JSR   LC3A7                   ; C59A: BD C3 A7      
LC59D               LDA   $4AF6                   ; C59D: B6 4A F6      
                    JSR   LC5A4                   ; C5A0: BD C5 A4      
; End of function sub_C4EB
locret_C5A3         RTS                           ; C5A3: 39            

; =============== S U B R O U T I N E =======================================
LC5A4               LDX   #byte_C797              ; C5A4: 8E C7 97      
                    LDB   A,X                     ; C5A7: E6 86         
                    STB   $4AFA                   ; C5A9: F7 4A FA      
                    ANDB  #$07                    ; C5AC: C4 07         
                    LDX   #$4590                  ; C5AE: 8E 45 90      
                    ASLB                          ; C5B1: 58            
                    ABX                           ; C5B2: 3A            
                    LDB   $4AFA                   ; C5B3: F6 4A FA      
                    LSRB                          ; C5B6: 54            
                    LSRB                          ; C5B7: 54            
                    LSRB                          ; C5B8: 54            
                    STB   $4AFA                   ; C5B9: F7 4A FA      
                    ANDB  #$03                    ; C5BC: C4 03         
                    LDU   #byte_C737              ; C5BE: CE C7 37      
                    LEAU  B,U                     ; C5C1: 33 C5         
                    LDB   $4AFA                   ; C5C3: F6 4A FA      
                    LSRB                          ; C5C6: 54            
                    LSRB                          ; C5C7: 54            
                    STB   $4AFA                   ; C5C8: F7 4A FA      
                    LDB   ,X                      ; C5CB: E6 84         
                    ASLB                          ; C5CD: 58            
                    ASLB                          ; C5CE: 58            
                    ASLB                          ; C5CF: 58            
                    ASLB                          ; C5D0: 58            
                    STB   $4AFB                   ; C5D1: F7 4A FB      
                    LDB   $01,X                   ; C5D4: E6 01         
                    ANDB  #$0F                    ; C5D6: C4 0F         
                    ADDB  $4AFB                   ; C5D8: FB 4A FB      
LC5DB               DEC   $4AFA                   ; C5DB: 7A 4A FA      
                    BMI   LC5E3                   ; C5DE: 2B 03         
                    LSRB                          ; C5E0: 54            
                    BRA   LC5DB                   ; C5E1: 20 F8         

; ---------------------------------------------------------------------------
LC5E3               ANDB  ,U                      ; C5E3: E4 C4         
                    STB   $4AF5                   ; C5E5: F7 4A F5      
                    LDX   #byte_C7F1              ; C5E8: 8E C7 F1      
                    LEAX  A,X                     ; C5EB: 30 86         
                    ADDB  ,X                      ; C5ED: EB 84         
; End of function sub_C5A4
                    JMP   LE7D3                   ; C5EF: 7E E7 D3      

; =============== S U B R O U T I N E =======================================
; FUNCTION CHUNK AT ROM:C65B SIZE 0000001F BYTES
LC5F2               LDX   #$4500                  ; C5F2: 8E 45 00        // NOVRAM
                    LDU   #$4C00                  ; C5F5: CE 4C 00      
LC5F8               LDD   ,X++                    ; C5F8: EC 81         
                    STD   ,U++                    ; C5FA: ED C1         
                    CMPX  #$4600                  ; C5FC: 8C 46 00        // NOVRAM
                    BCS   LC5F8                   ; C5FF: 25 F7         
                    LDX   #$4500                  ; C601: 8E 45 00        // NOVRAM
LC604               LDA   ,X                      ; C604: A6 84         
                    COMA                          ; C606: 43            
                    STA   ,X+                     ; C607: A7 80         
                    CMPX  #$4600                  ; C609: 8C 46 00        // NOVRAM
                    BCS   LC604                   ; C60C: 25 F6         
                    STA   $46A0                   ; C60E: B7 46 A0      
                    JSR   LC688                   ; C611: BD C6 88      
LC614               JSR   LC67A                   ; C614: BD C6 7A      
LC617               JSR   LC6B8                   ; C617: BD C6 B8      
LC61A               LDX   #$4500                  ; C61A: 8E 45 00        // NOVRAM
LC61D               LDA   ,X                      ; C61D: A6 84         
                    COMA                          ; C61F: 43            
                    STA   ,X+                     ; C620: A7 80         
                    CMPX  #$4600                  ; C622: 8C 46 00        // NOVRAM
                    BCS   LC61D                   ; C625: 25 F6         
                    STA   $46A0                   ; C627: B7 46 A0      
                    JSR   LC688                   ; C62A: BD C6 88      
LC62D               JSR   LC641                   ; C62D: BD C6 41      
LC630               BNE   LC65B                   ; C630: 26 29         
                    JSR   LC67A                   ; C632: BD C6 7A      
LC635               JSR   LC6B8                   ; C635: BD C6 B8      
LC638               JSR   LC641                   ; C638: BD C6 41      
LC63B               BEQ   locret_C640             ; C63B: 27 03         
                    JMP   LC65B                   ; C63D: 7E C6 5B      

; ---------------------------------------------------------------------------
; End of function sub_C5F2
locret_C640         RTS                           ; C640: 39            

; =============== S U B R O U T I N E =======================================
LC641               LDX   #$4500                  ; C641: 8E 45 00        // NOVRAM
                    LDU   #$4C00                  ; C644: CE 4C 00      
LC647               LDA   ,X+                     ; C647: A6 80         
                    EORA  ,U+                     ; C649: A8 C0         
                    ANDA  #$0F                    ; C64B: 84 0F         
                    BNE   locret_C65A             ; C64D: 26 0B         
                    CMPX  #$4600                  ; C64F: 8C 46 00        // NOVRAM
                    BCS   LC647                   ; C652: 25 F3         
                    LDA   #$01                    ; C654: 86 01         
                    STA   $4AF7                   ; C656: B7 4A F7      
                    CLRA                          ; C659: 4F            
; End of function sub_C641
locret_C65A         RTS                           ; C65A: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_C5F2
LC65B               LEAX  -1,X                    ; C65B: 30 1F         
                    TFR   X,D                     ; C65D: 1F 10         
                    STD   $4AF8                   ; C65F: FD 4A F8      
                    LDA   #$FF                    ; C662: 86 FF         
                    STA   $4AF7                   ; C664: B7 4A F7      
                    LDX   #$4500                  ; C667: 8E 45 00        // NOVRAM
                    LDU   #$4C00                  ; C66A: CE 4C 00      
LC66D               LDD   ,U++                    ; C66D: EC C1         
                    STD   ,X++                    ; C66F: ED 81         
                    CMPX  #$4600                  ; C671: 8C 46 00        // NOVRAM
                    BCS   LC66D                   ; C674: 25 F7         
                    TST   $4AF7                   ; C676: 7D 4A F7      
; END OF FUNCTION CHUNK FOR sub_C5F2
                    RTS                           ; C679: 39            

; =============== S U B R O U T I N E =======================================
LC67A               LDX   #$4500                  ; C67A: 8E 45 00        // NOVRAM
                    LDD   #$00                    ; C67D: CC 00 00      
LC680               STD   ,X++                    ; C680: ED 81         
                    CMPX  #$4600                  ; C682: 8C 46 00        // NOVRAM
                    BCS   LC680                   ; C685: 25 F9         
; End of function sub_C67A
                    RTS                           ; C687: 39            

; =============== S U B R O U T I N E =======================================
LC688               LDX   #$07D0                  ; C688: 8E 07 D0      
LC68B               LEAX  -1,X                    ; C68B: 30 1F         
                    BNE   LC68B                   ; C68D: 26 FC         
; End of function sub_C688
                    RTS                           ; C68F: 39            

; =============== S U B R O U T I N E =======================================
LC690               LDA   $4AF7                   ; C690: B6 4A F7      
                    BEQ   locret_C6B7             ; C693: 27 22         
                    CMPA  #$01                    ; C695: 81 01         
                    BNE   LC69D                   ; C697: 26 04         
                    LDB   #$9C                    ; C699: C6 9C         
                    BRA   LC6B4                   ; C69B: 20 17         

; ---------------------------------------------------------------------------
LC69D               LDD   #$1F6A                  ; C69D: CC 1F 6A      
                    STD   ,Y++                    ; C6A0: ED A1         
                    LDD   #$01A4                  ; C6A2: CC 01 A4      
                    STD   ,Y++                    ; C6A5: ED A1         
                    LDA   $4AF9                   ; C6A7: B6 4A F9      
                    JSR   Display_Vect_BCD        ; C6AA: BD E7 90        // Display BCD numbers
LC6AD               LDD   #$8040                  ; C6AD: CC 80 40      
                    STD   ,Y++                    ; C6B0: ED A1         
                    LDB   #$9D                    ; C6B2: C6 9D         
LC6B4               JSR   LE7C7                   ; C6B4: BD E7 C7        // Print text string from pointer table
; End of function sub_C690
locret_C6B7         RTS                           ; C6B7: 39            

; =============== S U B R O U T I N E =======================================
LC6B8               LDA   #$FF                    ; C6B8: 86 FF         
                    STA   $4687                   ; C6BA: B7 46 87      
                    JSR   $6005                   ; C6BD: BD 60 05      
LC6C0               LDA   #$00                    ; C6C0: 86 00         
                    STA   $4687                   ; C6C2: B7 46 87      
                    LDU   #$00                    ; C6C5: CE 00 00      
LC6C8               JSR   $6005                   ; C6C8: BD 60 05      
LC6CB               LEAU  $01,U                   ; C6CB: 33 41         
                    CMPU  #$20                    ; C6CD: 11 83 00 20   
                    BCS   LC6C8                   ; C6D1: 25 F5         
; End of function sub_C6B8
                    RTS                           ; C6D3: 39            

; =============== S U B R O U T I N E =======================================
; Read NOVRAM
; End of function sub_C6D4
LC6D4               LDU   #$4AFA                  ; C6D4: CE 4A FA      
; =============== S U B R O U T I N E =======================================
; Read NOVRAM
LC6D7               LDA   #$03                    ; C6D7: 86 03         
LC6D9               STA   $4B02                   ; C6D9: B7 4B 02      
LC6DC               LDA   ,X+                     ; C6DC: A6 80         
                    ASLA                          ; C6DE: 48            
                    ASLA                          ; C6DF: 48            
                    ASLA                          ; C6E0: 48            
                    ASLA                          ; C6E1: 48            
                    STA   ,U                      ; C6E2: A7 C4         
                    LDA   ,X+                     ; C6E4: A6 80         
                    ANDA  #$0F                    ; C6E6: 84 0F         
                    ADDA  ,U                      ; C6E8: AB C4         
                    STA   ,U+                     ; C6EA: A7 C0         
                    DEC   $4B02                   ; C6EC: 7A 4B 02      
                    BPL   LC6DC                   ; C6EF: 2A EB         
                    LEAX  -8,X                    ; C6F1: 30 18         
; End of function sub_C6D7
                    RTS                           ; C6F3: 39            

; =============== S U B R O U T I N E =======================================
; End of function sub_C6F4
LC6F4               LDU   #$4AFA                  ; C6F4: CE 4A FA      
; =============== S U B R O U T I N E =======================================
LC6F7               LDA   #$02                    ; C6F7: 86 02         
LC6F9               STA   $4B02                   ; C6F9: B7 4B 02      
LC6FC               LDA   ,U+                     ; C6FC: A6 C0         
                    STA   $01,X                   ; C6FE: A7 01         
                    LSRA                          ; C700: 44            
                    LSRA                          ; C701: 44            
                    LSRA                          ; C702: 44            
                    LSRA                          ; C703: 44            
                    STA   ,X++                    ; C704: A7 81         
LC706               DEC   $4B02                   ; C706: 7A 4B 02      
                    BPL   LC6FC                   ; C709: 2A F1         
                    LEAX  -6,X                    ; C70B: 30 1A         
; End of function sub_C6F7
                    RTS                           ; C70D: 39            

; =============== S U B R O U T I N E =======================================
LC70E               LDB   #$00                    ; C70E: C6 00         
                    STB   <$AD                    ; C710: D7 AD         
                    TFR   A,B                     ; C712: 1F 89         
                    LSRA                          ; C714: 44            
                    LSRA                          ; C715: 44            
                    LSRA                          ; C716: 44            
                    LSRA                          ; C717: 44            
                    CMPA  #$0A                    ; C718: 81 0A         
                    BCS   LC71E                   ; C71A: 25 02         
                    LDA   #$09                    ; C71C: 86 09         
LC71E               JSR   LE7AD                   ; C71E: BD E7 AD      
LC721               LDA   #$B8                    ; C721: 86 B8         
                    STA   ,Y+                     ; C723: A7 A0         
                    LDA   #$DF                    ; C725: 86 DF         
                    STA   ,Y+                     ; C727: A7 A0         
                    CLRA                          ; C729: 4F            
                    ANDB  #$0F                    ; C72A: C4 0F         
                    BEQ   LC734                   ; C72C: 27 06         
LC72E               ADDA  #$04                    ; C72E: 8B 04         
                    DAA                           ; C730: 19            
                    DECB                          ; C731: 5A            
                    BNE   LC72E                   ; C732: 26 FA         
; End of function sub_C70E
; ---------------------------------------------------------------------------
LC734               JMP   Display_Vect_BCD        ; C734: 7E E7 90        // Display BCD numbers

                    FCB   $90                     ; C7B9:  ' ' 90 98          SUBA  <$98 ; 
                    FCB   $98                     ; C7BA:  ' ' 98 01          EORA  <$01 ; 
; =============== S U B R O U T I N E =======================================
; Display high scores
LC7FD               TST   $4AEC                   ; C7FD: 7D 4A EC      
                    BMI   LC811                   ; C800: 2B 0F         
                    LDA   #$3E                    ; C802: 86 3E         
                    JSR   LD8DF                   ; C804: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
LC807               LDD   #$7200                  ; C807: CC 72 00      
                    STD   ,Y++                    ; C80A: ED A1         
                    LDD   #word_CA64              ; C80C: CC CA 64        // High scores text position table
                    BRA   LC81E                   ; C80F: 20 0D         

; ---------------------------------------------------------------------------
LC811               LDA   #$3F                    ; C811: 86 3F         
                    JSR   LD8DF                   ; C813: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
LC816               LDD   #$7140                  ; C816: CC 71 40        // High scores text size
                    STD   ,Y++                    ; C819: ED A1         
                    LDD   #word_CA78              ; C81B: CC CA 78      
LC81E               STD   $4AF1                   ; C81E: FD 4A F1      
                    LDA   #$00                    ; C821: 86 00         
                    STA   $4AEA                   ; C823: B7 4A EA      
LC826               LDU   $4AF1                   ; C826: FE 4A F1      
                    LDD   ,U                      ; C829: EC C4         
                    STD   ,Y++                    ; C82B: ED A1         
                    LDD   #$1F80                  ; C82D: CC 1F 80      
                    STD   ,Y++                    ; C830: ED A1         
                    LDB   $4AEA                   ; C832: F6 4A EA      
                    ASLB                          ; C835: 58            
                    ADDB  $4AEA                   ; C836: FB 4A EA      
                    LDX   #$4AB6                  ; C839: 8E 4A B6      
                    ABX                           ; C83C: 3A            
                    CPX   $4AEC                   ; C83D: BC 4A EC      
                    BNE   LC847                   ; C840: 26 05         
                    LDD   #$6780                  ; C842: CC 67 80      
                    BRA   LC84A                   ; C845: 20 03         

; ---------------------------------------------------------------------------
LC847               LDD   $4B10                   ; C847: FC 4B 10        // Attract text colour/intensity for fading
LC84A               STD   ,Y++                    ; C84A: ED A1         
                    STD   <$01                    ; C84C: DD 01         
                    LDU   #$3016                  ; C84E: CE 30 16      
                    TST   $4AEC                   ; C851: 7D 4A EC      
                    BMI   LC871                   ; C854: 2B 1B         
                    LDA   $4AEE                   ; C856: B6 4A EE      
                    CMPA  #$00                    ; C859: 81 00         
                    BNE   LC86D                   ; C85B: 26 10         
                    LDA   $4843                   ; C85D: B6 48 43      
                    ANDA  #$01                    ; C860: 84 01         
                    BNE   LC869                   ; C862: 26 05         
                    LDD   $4B10                   ; C864: FC 4B 10        // Attract text colour/intensity for fading
                    BRA   LC86B                   ; C867: 20 02         

; ---------------------------------------------------------------------------
LC869               LDD   <$01                    ; C869: DC 01         
LC86B               BRA   LC86F                   ; C86B: 20 02         

; ---------------------------------------------------------------------------
LC86D               LDD   <$01                    ; C86D: DC 01         
LC86F               STD   ,Y++                    ; C86F: ED A1         
LC871               LDA   ,X+                     ; C871: A6 80         
                    BNE   LC884                   ; C873: 26 0F         
                    TST   $4AEC                   ; C875: 7D 4A EC      
                    BMI   LC87F                   ; C878: 2B 05         
                    LDD   $3054                   ; C87A: FC 30 54      
                    BRA   LC882                   ; C87D: 20 03         

; ---------------------------------------------------------------------------
LC87F               LDD   $3002                   ; C87F: FC 30 02      
LC882               BRA   LC887                   ; C882: 20 03         

; ---------------------------------------------------------------------------
LC884               ASLA                          ; C884: 48            
                    LDD   A,U                     ; C885: EC C6         
LC887               STD   ,Y++                    ; C887: ED A1         
                    TST   $4AEC                   ; C889: 7D 4A EC      
                    BMI   LC8A9                   ; C88C: 2B 1B         
                    LDA   $4AEE                   ; C88E: B6 4A EE      
                    CMPA  #$01                    ; C891: 81 01         
                    BNE   LC8A5                   ; C893: 26 10         
                    LDA   $4843                   ; C895: B6 48 43      
                    ANDA  #$01                    ; C898: 84 01         
                    BNE   LC8A1                   ; C89A: 26 05         
                    LDD   $4B10                   ; C89C: FC 4B 10        // Attract text colour/intensity for fading
                    BRA   LC8A3                   ; C89F: 20 02         

; ---------------------------------------------------------------------------
LC8A1               LDD   <$01                    ; C8A1: DC 01         
LC8A3               BRA   LC8A7                   ; C8A3: 20 02         

; ---------------------------------------------------------------------------
LC8A5               LDD   <$01                    ; C8A5: DC 01         
LC8A7               STD   ,Y++                    ; C8A7: ED A1         
LC8A9               LDA   ,X+                     ; C8A9: A6 80         
                    BNE   LC8BC                   ; C8AB: 26 0F         
                    TST   $4AEC                   ; C8AD: 7D 4A EC      
                    BMI   LC8B7                   ; C8B0: 2B 05         
                    LDD   $3054                   ; C8B2: FC 30 54      
                    BRA   LC8BA                   ; C8B5: 20 03         

; ---------------------------------------------------------------------------
LC8B7               LDD   $3002                   ; C8B7: FC 30 02      
LC8BA               BRA   LC8BF                   ; C8BA: 20 03         

; ---------------------------------------------------------------------------
LC8BC               ASLA                          ; C8BC: 48            
                    LDD   A,U                     ; C8BD: EC C6         
LC8BF               STD   ,Y++                    ; C8BF: ED A1         
                    TST   $4AEC                   ; C8C1: 7D 4A EC      
                    BMI   LC8E1                   ; C8C4: 2B 1B         
                    LDA   $4AEE                   ; C8C6: B6 4A EE      
                    CMPA  #$02                    ; C8C9: 81 02         
                    BNE   LC8DD                   ; C8CB: 26 10         
                    LDA   $4843                   ; C8CD: B6 48 43      
                    ANDA  #$01                    ; C8D0: 84 01         
                    BNE   LC8D9                   ; C8D2: 26 05         
                    LDD   $4B10                   ; C8D4: FC 4B 10        // Attract text colour/intensity for fading
                    BRA   LC8DB                   ; C8D7: 20 02         

; ---------------------------------------------------------------------------
LC8D9               LDD   <$01                    ; C8D9: DC 01         
LC8DB               BRA   LC8DF                   ; C8DB: 20 02         

; ---------------------------------------------------------------------------
LC8DD               LDD   <$01                    ; C8DD: DC 01         
LC8DF               STD   ,Y++                    ; C8DF: ED A1         
LC8E1               LDA   ,X+                     ; C8E1: A6 80         
                    BNE   LC8F4                   ; C8E3: 26 0F         
                    TST   $4AEC                   ; C8E5: 7D 4A EC      
                    BMI   LC8EF                   ; C8E8: 2B 05         
                    LDD   $3054                   ; C8EA: FC 30 54      
                    BRA   LC8F2                   ; C8ED: 20 03         

; ---------------------------------------------------------------------------
LC8EF               LDD   $3002                   ; C8EF: FC 30 02      
LC8F2               BRA   LC8F7                   ; C8F2: 20 03         

; ---------------------------------------------------------------------------
LC8F4               ASLA                          ; C8F4: 48            
                    LDD   A,U                     ; C8F5: EC C6         
LC8F7               STD   ,Y++                    ; C8F7: ED A1         
                    LDD   <$01                    ; C8F9: DC 01         
                    STD   ,Y++                    ; C8FB: ED A1         
                    LDD   #$8040                  ; C8FD: CC 80 40      
                    STD   ,Y++                    ; C900: ED A1         
                    LDU   $4AF1                   ; C902: FE 4A F1      
                    LDD   ,U                      ; C905: EC C4         
                    STD   ,Y++                    ; C907: ED A1         
                    LDD   #$1F38                  ; C909: CC 1F 38      
                    STD   ,Y++                    ; C90C: ED A1         
                    LDA   $4AEA                   ; C90E: B6 4A EA      
                    INCA                          ; C911: 4C            
                    CMPA  #$0A                    ; C912: 81 0A         
                    BCS   LC918                   ; C914: 25 02         
                    LDA   #$10                    ; C916: 86 10         
LC918               LDB   #$01                    ; C918: C6 01         
                    STB   <$AD                    ; C91A: D7 AD         
                    JSR   Display_Vect_BCD        ; C91C: BD E7 90        // Display BCD numbers
LC91F               LDD   #$B8DC                  ; C91F: CC B8 DC      
                    STD   ,Y++                    ; C922: ED A1         
                    LDD   #$8040                  ; C924: CC 80 40      
                    STD   ,Y++                    ; C927: ED A1         
                    LDB   $4AEA                   ; C929: F6 4A EA      
                    ASLB                          ; C92C: 58            
                    ASLB                          ; C92D: 58            
                    LDX   #$4A8E                  ; C92E: 8E 4A 8E        // High scores RAM
                    ABX                           ; C931: 3A            
                    LDU   $4AF1                   ; C932: FE 4A F1      
                    LDD   ,U++                    ; C935: EC C1         
                    STD   ,Y++                    ; C937: ED A1         
                    LDD   #$1FF0                  ; C939: CC 1F F0      
                    STD   ,Y++                    ; C93C: ED A1         
                    STU   $4AF1                   ; C93E: FF 4A F1      
                    LDB   #$06                    ; C941: C6 06         
                    STB   <$AD                    ; C943: D7 AD         
                    JSR   LE764                   ; C945: BD E7 64      
LC948               LEAX  $04,X                   ; C948: 30 04         
                    LDD   #$8040                  ; C94A: CC 80 40      
                    STD   ,Y++                    ; C94D: ED A1         
                    INC   $4AEA                   ; C94F: 7C 4A EA      
                    LDA   $4AEA                   ; C952: B6 4A EA      
                    CMPA  #$0A                    ; C955: 81 0A         
                    LBCS  LC826                   ; C957: 10 25 FE CB   
                    LDD   #$7200                  ; C95B: CC 72 00      
                    STD   ,Y++                    ; C95E: ED A1         
                    LDD   $4AEC                   ; C960: FC 4A EC      
                    BPL   LC966                   ; C963: 2A 01         
                    RTS                           ; C965: 39            

; ---------------------------------------------------------------------------
LC966               LDD   #$6480                  ; C966: CC 64 80      
                    STD   ,Y++                    ; C969: ED A1         
                    LDX   #$CBA8                  ; C96B: 8E CB A8      
                    LDU   #$3018                  ; C96E: CE 30 18      
LC971               LDD   $02,X                   ; C971: EC 02         
                    ANDA  #$1F                    ; C973: 84 1F         
                    STD   ,Y++                    ; C975: ED A1         
                    LDD   ,X                      ; C977: EC 84         
                    ANDA  #$1F                    ; C979: 84 1F         
                    STD   ,Y++                    ; C97B: ED A1         
                    LDD   ,U++                    ; C97D: EC C1         
                    STD   ,Y++                    ; C97F: ED A1         
                    LDD   #$8040                  ; C981: CC 80 40      
                    STD   ,Y++                    ; C984: ED A1         
                    LEAX  $04,X                   ; C986: 30 04         
                    CMPX  #word_CC10              ; C988: 8C CC 10      
                    BCS   LC971                   ; C98B: 25 E4         
                    LDD   word_CBA6               ; C98D: FC CB A6      
                    ANDA  #$1F                    ; C990: 84 1F         
                    STD   ,Y++                    ; C992: ED A1         
                    LDD   word_CBA4               ; C994: FC CB A4      
                    ANDA  #$1F                    ; C997: 84 1F         
                    STD   ,Y++                    ; C999: ED A1         
                    LDD   $3054                   ; C99B: FC 30 54      
                    STD   ,Y++                    ; C99E: ED A1         
                    LDD   #$7200                  ; C9A0: CC 72 00      
                    STD   ,Y++                    ; C9A3: ED A1         
                    LDD   #$8040                  ; C9A5: CC 80 40      
                    STD   ,Y++                    ; C9A8: ED A1         
                    LDB   $4AEF                   ; C9AA: F6 4A EF      
                    CMPB  #$1B                    ; C9AD: C1 1B         
                    BNE   LC9B6                   ; C9AF: 26 05         
                    LDD   #$6750                  ; C9B1: CC 67 50      
                    BRA   LC9B9                   ; C9B4: 20 03         

; ---------------------------------------------------------------------------
LC9B6               LDD   #$6450                  ; C9B6: CC 64 50      
LC9B9               STD   ,Y++                    ; C9B9: ED A1         
                    LDD   $02,X                   ; C9BB: EC 02         
                    ANDA  #$1F                    ; C9BD: 84 1F         
                    STD   ,Y++                    ; C9BF: ED A1         
                    LDD   ,X                      ; C9C1: EC 84         
                    SUBD  #$08                    ; C9C3: 83 00 08      
                    ANDA  #$1F                    ; C9C6: 84 1F         
                    STD   ,Y++                    ; C9C8: ED A1         
                    LDD   #$71C0                  ; C9CA: CC 71 C0      
                    STD   ,Y++                    ; C9CD: ED A1         
                    LDD   $303A                   ; C9CF: FC 30 3A      
                    STD   ,Y++                    ; C9D2: ED A1         
                    LDD   $3040                   ; C9D4: FC 30 40      
                    STD   ,Y++                    ; C9D7: ED A1         
                    LDD   $301A                   ; C9D9: FC 30 1A      
                    STD   ,Y++                    ; C9DC: ED A1         
                    LDD   #$7200                  ; C9DE: CC 72 00      
                    STD   ,Y++                    ; C9E1: ED A1         
                    LDD   #$8040                  ; C9E3: CC 80 40      
                    STD   ,Y++                    ; C9E6: ED A1         
                    LDB   $4AEF                   ; C9E8: F6 4A EF      
                    CMPB  #$1C                    ; C9EB: C1 1C         
                    BNE   LC9F4                   ; C9ED: 26 05         
                    LDD   #$6750                  ; C9EF: CC 67 50      
                    BRA   LC9F7                   ; C9F2: 20 03         

; ---------------------------------------------------------------------------
LC9F4               LDD   #$6450                  ; C9F4: CC 64 50      
LC9F7               STD   ,Y++                    ; C9F7: ED A1         
                    LDD   $06,X                   ; C9F9: EC 06         
                    ANDA  #$1F                    ; C9FB: 84 1F         
                    STD   ,Y++                    ; C9FD: ED A1         
                    LDD   $04,X                   ; C9FF: EC 04         
                    SUBD  #$08                    ; CA01: 83 00 08      
                    ANDA  #$1F                    ; CA04: 84 1F         
                    STD   ,Y++                    ; CA06: ED A1         
                    LDD   #$71C0                  ; CA08: CC 71 C0      
                    STD   ,Y++                    ; CA0B: ED A1         
                    LDD   $3020                   ; CA0D: FC 30 20      
                    STD   ,Y++                    ; CA10: ED A1         
                    LDD   $3032                   ; CA12: FC 30 32      
                    STD   ,Y++                    ; CA15: ED A1         
                    LDD   $301E                   ; CA17: FC 30 1E      
                    STD   ,Y++                    ; CA1A: ED A1         
                    LDD   #$7200                  ; CA1C: CC 72 00      
                    STD   ,Y++                    ; CA1F: ED A1         
                    LDD   #$8040                  ; CA21: CC 80 40      
                    STD   ,Y++                    ; CA24: ED A1         
                    LDD   #$6780                  ; CA26: CC 67 80      
                    STD   ,Y++                    ; CA29: ED A1         
                    LDB   $4AEF                   ; CA2B: F6 4A EF      
                    CMPB  #$1B                    ; CA2E: C1 1B         
                    BCC   LCA5B                   ; CA30: 24 29         
                    ASLB                          ; CA32: 58            
                    ASLB                          ; CA33: 58            
                    LDX   #word_CBA4              ; CA34: 8E CB A4      
                    ABX                           ; CA37: 3A            
                    LDD   $02,X                   ; CA38: EC 02         
                    ANDA  #$1F                    ; CA3A: 84 1F         
                    STD   ,Y++                    ; CA3C: ED A1         
                    LDD   ,X                      ; CA3E: EC 84         
                    ANDA  #$1F                    ; CA40: 84 1F         
                    STD   ,Y++                    ; CA42: ED A1         
                    LDB   $4AEF                   ; CA44: F6 4A EF      
                    BNE   LCA4E                   ; CA47: 26 05         
                    LDD   $3054                   ; CA49: FC 30 54      
                    BRA   LCA54                   ; CA4C: 20 06         

; ---------------------------------------------------------------------------
LCA4E               ASLB                          ; CA4E: 58            
                    LDX   #$3016                  ; CA4F: 8E 30 16      
                    LDD   B,X                     ; CA52: EC 85         
LCA54               STD   ,Y++                    ; CA54: ED A1         
                    LDD   #$8040                  ; CA56: CC 80 40        // Vector CNTR instruction
                    STD   ,Y++                    ; CA59: ED A1         
LCA5B               LDD   #$6680                  ; CA5B: CC 66 80      
                    STD   ,Y++                    ; CA5E: ED A1         
                    JSR   LB6CC                   ; CA60: BD B6 CC      
; End of function sub_C7FD
; ---------------------------------------------------------------------------
LCA63               RTS                           ; CA63: 39            

; =============== S U B R O U T I N E =======================================
; Score
LCA8C               LDX   #$4A8E                  ; CA8C: 8E 4A 8E        // High scores RAM
LCA8F               LDD   <$5C                    ; CA8F: DC 5C           // Score millions
                    SUBD  ,X                      ; CA91: A3 84         
                    BHI   LCAA5                   ; CA93: 22 10         
                    BNE   LCA9D                   ; CA95: 26 06         
                    LDD   <$5E                    ; CA97: DC 5E           // Score thousands
                    SUBD  $02,X                   ; CA99: A3 02         
                    BCC   LCAA5                   ; CA9B: 24 08         
LCA9D               LEAX  $04,X                   ; CA9D: 30 04         
                    CMPX  #$4AB6                  ; CA9F: 8C 4A B6      
                    BCS   LCA8F                   ; CAA2: 25 EB         
                    RTS                           ; CAA4: 39            

; ---------------------------------------------------------------------------
LCAA5               JSR   LCAB7                   ; CAA5: BD CA B7      
LCAA8               LDA   #$00                    ; CAA8: 86 00         
                    STA   $4AEE                   ; CAAA: B7 4A EE      
                    LDD   #$00                    ; CAAD: CC 00 00      
                    STD   $4AEF                   ; CAB0: FD 4A EF      
                    LDX   #$4AB6                  ; CAB3: 8E 4A B6      
; End of function sub_CA8C
                    RTS                           ; CAB6: 39            

; =============== S U B R O U T I N E =======================================
LCAB7               STX   $4AEC                   ; CAB7: BF 4A EC      
                    LDU   #$4AD1                  ; CABA: CE 4A D1      
                    LDX   #$4AB2                  ; CABD: 8E 4A B2      
                    CPX   $4AEC                   ; CAC0: BC 4A EC      
                    BEQ   LCADE                   ; CAC3: 27 19         
LCAC5               LDD   -4,X                    ; CAC5: EC 1C         
                    STD   ,X                      ; CAC7: ED 84         
                    LDD   -2,X                    ; CAC9: EC 1E         
                    STD   $02,X                   ; CACB: ED 02         
                    LDD   -3,U                    ; CACD: EC 5D         
                    STD   ,U                      ; CACF: ED C4         
                    LDA   -1,U                    ; CAD1: A6 5F         
                    STA   $02,U                   ; CAD3: A7 42         
                    LEAU  -3,U                    ; CAD5: 33 5D         
                    LEAX  -4,X                    ; CAD7: 30 1C         
                    CPX   $4AEC                   ; CAD9: BC 4A EC      
                    BHI   LCAC5                   ; CADC: 22 E7         
LCADE               STU   $4AEC                   ; CADE: FF 4A EC      
                    LDA   #$00                    ; CAE1: 86 00         
                    STA   ,U                      ; CAE3: A7 C4         
                    LDD   #$00                    ; CAE5: CC 00 00      
                    STD   $01,U                   ; CAE8: ED 41         
                    LDD   <$5C                    ; CAEA: DC 5C           // Score millions
                    STD   ,X                      ; CAEC: ED 84         
                    LDD   <$5E                    ; CAEE: DC 5E           // Score thousands
                    STD   $02,X                   ; CAF0: ED 02         
; End of function sub_CAB7
                    RTS                           ; CAF2: 39            

; =============== S U B R O U T I N E =======================================
LCAF3               LDX   $4AEC                   ; CAF3: BE 4A EC      
                    LDB   $4AEE                   ; CAF6: F6 4A EE      
                    ABX                           ; CAF9: 3A            
                    TFR   X,U                     ; CAFA: 1F 13         
                    LDA   $4AEE                   ; CAFC: B6 4A EE      
                    CMPA  #$03                    ; CAFF: 81 03         
                    BCS   LCB08                   ; CB01: 25 05         
                    LDX   #word_CC10              ; CB03: 8E CC 10      
                    BRA   LCB0B                   ; CB06: 20 03         

; ---------------------------------------------------------------------------
LCB08               LDX   #word_CBA4              ; CB08: 8E CB A4      
LCB0B               LDD   $4879                   ; CB0B: FC 48 79      
                    SUBD  #$08                    ; CB0E: 83 00 08      
                    SUBD  ,X                      ; CB11: A3 84         
                    TSTA                          ; CB13: 4D            
                    BPL   LCB1A                   ; CB14: 2A 04         
                    COMA                          ; CB16: 43            
                    NEGB                          ; CB17: 50            
                    SBCA  #$FF                    ; CB18: 82 FF         
LCB1A               STD   <$01                    ; CB1A: DD 01         
                    CMPD  #$18                    ; CB1C: 10 83 00 18   
                    BCC   LCB49                   ; CB20: 24 27         
                    LDD   $487B                   ; CB22: FC 48 7B      
                    ADDD  #$FF8C                  ; CB25: C3 FF 8C      
                    SUBD  $02,X                   ; CB28: A3 02         
                    TSTA                          ; CB2A: 4D            
                    BPL   LCB31                   ; CB2B: 2A 04         
                    COMA                          ; CB2D: 43            
                    NEGB                          ; CB2E: 50            
                    SBCA  #$FF                    ; CB2F: 82 FF         
LCB31               CMPD  #$18                    ; CB31: 10 83 00 18   
                    BCC   LCB49                   ; CB35: 24 12         
                    ADDD  <$01                    ; CB37: D3 01         
                    CMPD  #$20                    ; CB39: 10 83 00 20   
                    BCC   LCB49                   ; CB3D: 24 0A         
                    TFR   X,D                     ; CB3F: 1F 10         
                    SUBD  #word_CBA4              ; CB41: 83 CB A4      
                    LSRB                          ; CB44: 54            
                    LSRB                          ; CB45: 54            
                    STB   $4AEF                   ; CB46: F7 4A EF      
LCB49               LEAX  $04,X                   ; CB49: 30 04         
                    CMPX  #LCC18                  ; CB4B: 8C CC 18      
                    BCS   LCB0B                   ; CB4E: 25 BB         
                    LDA   $4AEF                   ; CB50: B6 4A EF      
                    CMPA  #$1B                    ; CB53: 81 1B         
                    BCC   LCB59                   ; CB55: 24 02         
                    STA   ,U                      ; CB57: A7 C4         
LCB59               LDA   <$AC                    ; CB59: 96 AC         
                    ANDA  #$F0                    ; CB5B: 84 F0         
                    BEQ   locret_CBA3             ; CB5D: 27 44         
                    LDA   $4AEF                   ; CB5F: B6 4A EF      
                    CMPA  #$1B                    ; CB62: 81 1B         
                    BNE   LCB82                   ; CB64: 26 1C         
                    LDA   $4AEE                   ; CB66: B6 4A EE      
                    CMPA  #$02                    ; CB69: 81 02         
                    BHI   LCB71                   ; CB6B: 22 04         
                    LDA   #$00                    ; CB6D: 86 00         
                    STA   ,U                      ; CB6F: A7 C4         
LCB71               LDA   $4AEE                   ; CB71: B6 4A EE      
                    BEQ   LCB7D                   ; CB74: 27 07         
                    DEC   $4AEE                   ; CB76: 7A 4A EE      
                    LDA   #$00                    ; CB79: 86 00         
                    STA   -1,U                    ; CB7B: A7 5F         
LCB7D               JSR   LBDF8                   ; CB7D: BD BD F8      
LCB80               BRA   locret_CBA3             ; CB80: 20 21         

; ---------------------------------------------------------------------------
LCB82               CMPA  #$1C                    ; CB82: 81 1C         
                    BNE   LCB91                   ; CB84: 26 0B         
                    LDD   #$FFFF                  ; CB86: CC FF FF      
                    STD   $4AEC                   ; CB89: FD 4A EC      
                    JSR   LBDD5                   ; CB8C: BD BD D5      
LCB8F               BRA   locret_CBA3             ; CB8F: 20 12         

; ---------------------------------------------------------------------------
LCB91               INC   $4AEE                   ; CB91: 7C 4A EE      
                    LDA   $4AEE                   ; CB94: B6 4A EE      
                    CMPA  #$03                    ; CB97: 81 03         
                    BCS   LCBA0                   ; CB99: 25 05         
                    LDA   #$1C                    ; CB9B: 86 1C         
                    STA   $4AEF                   ; CB9D: B7 4A EF      
LCBA0               JSR   LBE16                   ; CBA0: BD BE 16      
; End of function sub_CAF3
; ---------------------------------------------------------------------------
locret_CBA3         RTS                           ; CBA3: 39            

; =============== S U B R O U T I N E =======================================
LCC18               JSR   LCC5B                   ; CC18: BD CC 5B      
LCC1B               LDA   #$01                    ; CC1B: 86 01         
                    JSR   LC2C3                   ; CC1D: BD C2 C3      
LCC20               BNE   LCC38                   ; CC20: 26 16         
                    LDU   #$4AB6                  ; CC22: CE 4A B6      
                    LDX   #$4520                  ; CC25: 8E 45 20      
                    LDA   #$08                    ; CC28: 86 08         
                    JSR   LC6D9                   ; CC2A: BD C6 D9      
LCC2D               LDU   #$4A8E                  ; CC2D: CE 4A 8E        // High scores RAM
                    LDX   #$4508                  ; CC30: 8E 45 08      
                    LDA   #$0B                    ; CC33: 86 0B         
                    JSR   LC6D9                   ; CC35: BD C6 D9      
LCC38               LDX   #$4AB6                  ; CC38: 8E 4A B6      
LCC3B               LDA   ,X+                     ; CC3B: A6 80         
                    CMPA  #$1B                    ; CC3D: 81 1B         
                    BCC   LCC5B                   ; CC3F: 24 1A         
                    CMPX  #$4AD4                  ; CC41: 8C 4A D4      
                    BCS   LCC3B                   ; CC44: 25 F5         
                    LDX   #$4A8E                  ; CC46: 8E 4A 8E        // High scores RAM
LCC49               LDA   ,X+                     ; CC49: A6 80         
                    CMPA  #$A0                    ; CC4B: 81 A0         
                    BCC   LCC5B                   ; CC4D: 24 0C         
                    ANDA  #$0F                    ; CC4F: 84 0F         
                    CMPA  #$0A                    ; CC51: 81 0A         
                    BCC   LCC5B                   ; CC53: 24 06         
                    CMPX  #$4AB6                  ; CC55: 8C 4A B6      
                    BCS   LCC49                   ; CC58: 25 EF         
; End of function sub_CC18
                    RTS                           ; CC5A: 39            

; =============== S U B R O U T I N E =======================================
LCC5B               LDX   #$4AB6                  ; CC5B: 8E 4A B6      
                    LDU   #word_CC7A              ; CC5E: CE CC 7A        // High score names
LCC61               LDD   ,U++                    ; CC61: EC C1         
                    STD   ,X++                    ; CC63: ED 81         
                    CMPX  #$4AD4                  ; CC65: 8C 4A D4      
                    BCS   LCC61                   ; CC68: 25 F7         
                    LDX   #$4A8E                  ; CC6A: 8E 4A 8E        // High scores RAM
                    LDU   #word_CC98              ; CC6D: CE CC 98        // High scores init table
LCC70               LDD   ,U++                    ; CC70: EC C1         
                    STD   ,X++                    ; CC72: ED 81         
                    CMPX  #$4AB6                  ; CC74: 8C 4A B6      
                    BCS   LCC70                   ; CC77: 25 F7         
; End of function sub_CC5B
; ---------------------------------------------------------------------------
                    RTS                           ; CC79: 39            

; =============== S U B R O U T I N E =======================================
; Initialise object?
LCCC0               ORCC  #$01                    ; CCC0: 1A 01         
                    ROR   $4684                   ; CCC2: 76 46 84      
                    JSR   $670D                   ; CCC5: BD 67 0D      
LCCC8               CLR   $4684                   ; CCC8: 7F 46 84      
; End of function sub_CCC0
                    RTS                           ; CCCB: 39            

; =============== S U B R O U T I N E =======================================
; Copy XYZ data to math RAM
LCCCC               ORCC  #$01                    ; CCCC: 1A 01         
                    ROR   $4684                   ; CCCE: 76 46 84      
                    JSR   $6724                   ; CCD1: BD 67 24      
LCCD4               CLR   $4684                   ; CCD4: 7F 46 84      
; End of function sub_CCCC
                    RTS                           ; CCD7: 39            

; =============== S U B R O U T I N E =======================================
; Copy object 3D data to math ram
LCCD8               ORCC  #$01                    ; CCD8: 1A 01         
                    ROR   $4684                   ; CCDA: 76 46 84      
                    JSR   $6726                   ; CCDD: BD 67 26      
LCCE0               CLR   $4684                   ; CCE0: 7F 46 84      
; End of function sub_CCD8
                    RTS                           ; CCE3: 39            

; =============== S U B R O U T I N E =======================================
LCCE4               ORCC  #$01                    ; CCE4: 1A 01         
                    ROR   $4684                   ; CCE6: 76 46 84        // Change memory page
                    JSR   $6761                   ; CCE9: BD 67 61      
LCCEC               CLR   $4684                   ; CCEC: 7F 46 84      
; End of function sub_CCE4
                    RTS                           ; CCEF: 39            

; =============== S U B R O U T I N E =======================================
; Get divider result and multiply by Math result Z, insert VCTR instruction
LCCF0               ORCC  #$01                    ; CCF0: 1A 01         
                    ROR   $4684                   ; CCF2: 76 46 84      
                    JSR   $6761                   ; CCF5: BD 67 61      
LCCF8               CLR   $4684                   ; CCF8: 7F 46 84      
; End of function sub_CCF0
                    RTS                           ; CCFB: 39            

; =============== S U B R O U T I N E =======================================
; Trench floor lines calcs
LCCFC               ORCC  #$01                    ; CCFC: 1A 01         
                    ROR   $4684                   ; CCFE: 76 46 84      
                    JSR   $6782                   ; CD01: BD 67 82      
LCD04               CLR   $4684                   ; CD04: 7F 46 84      
; End of function sub_CCFC
                    RTS                           ; CD07: 39            

; =============== S U B R O U T I N E =======================================
; Trench side vertical lines calcs
LCD08               ORCC  #$01                    ; CD08: 1A 01         
                    ROR   $4684                   ; CD0A: 76 46 84      
                    JSR   $67AA                   ; CD0D: BD 67 AA      
LCD10               CLR   $4684                   ; CD10: 7F 46 84      
; End of function sub_CD08
                    RTS                           ; CD13: 39            

; =============== S U B R O U T I N E =======================================
; Math program 0x50. Matrix Multiply - Transposed
; Then do perspective division?
LCD14               ORCC  #$01                    ; CD14: 1A 01         
                    ROR   $4684                   ; CD16: 76 46 84      
                    JSR   $67D2                   ; CD19: BD 67 D2      
LCD1C               CLR   $4684                   ; CD1C: 7F 46 84      
; End of function sub_CD14
                    RTS                           ; CD1F: 39            

; =============== S U B R O U T I N E =======================================
; Do 3D object transform using Matrix 1
LCD20               ORCC  #$01                    ; CD20: 1A 01         
                    ROR   $4684                   ; CD22: 76 46 84      
                    JSR   $67D4                   ; CD25: BD 67 D4      
LCD28               CLR   $4684                   ; CD28: 7F 46 84      
; End of function sub_CD20
                    RTS                           ; CD2B: 39            

; =============== S U B R O U T I N E =======================================
; Format vectors for ties, and tower/bunker explosions
LCD2C               ORCC  #$01                    ; CD2C: 1A 01         
                    ROR   $4684                   ; CD2E: 76 46 84      
                    JSR   $6819                   ; CD31: BD 68 19      
LCD34               CLR   $4684                   ; CD34: 7F 46 84      
; End of function sub_CD2C
                    RTS                           ; CD37: 39            

; =============== S U B R O U T I N E =======================================
; Trench left side turret calcs
LCD38               ORCC  #$01                    ; CD38: 1A 01         
                    ROR   $4684                   ; CD3A: 76 46 84      
                    JSR   $6864                   ; CD3D: BD 68 64      
LCD40               CLR   $4684                   ; CD40: 7F 46 84      
; End of function sub_CD38
                    RTS                           ; CD43: 39            

; =============== S U B R O U T I N E =======================================
; Trench right side turret calcs
LCD44               ORCC  #$01                    ; CD44: 1A 01         
                    ROR   $4684                   ; CD46: 76 46 84      
                    JSR   $68C7                   ; CD49: BD 68 C7      
LCD4C               CLR   $4684                   ; CD4C: 7F 46 84      
; End of function sub_CD44
                    RTS                           ; CD4F: 39            

; =============== S U B R O U T I N E =======================================
LCD50               ORCC  #$01                    ; CD50: 1A 01           // Called during towers
                    ROR   $4684                   ; CD52: 76 46 84      
                    JSR   $692D                   ; CD55: BD 69 2D      
LCD58               CLR   $4684                   ; CD58: 7F 46 84      
; End of function sub_CD50
                    RTS                           ; CD5B: 39            

; =============== S U B R O U T I N E =======================================
; Trench calcs
LCD5C               ORCC  #$01                    ; CD5C: 1A 01           // Called during trench start
                    ROR   $4684                   ; CD5E: 76 46 84      
                    JSR   $6978                   ; CD61: BD 69 78      
LCD64               CLR   $4684                   ; CD64: 7F 46 84      
; End of function sub_CD5C
                    RTS                           ; CD67: 39            

; =============== S U B R O U T I N E =======================================
LCD68               ORCC  #$01                    ; CD68: 1A 01           // Called during towers
                    ROR   $4684                   ; CD6A: 76 46 84      
                    JSR   $6A0C                   ; CD6D: BD 6A 0C      
LCD70               CLR   $4684                   ; CD70: 7F 46 84      
; End of function sub_CD68
                    RTS                           ; CD73: 39            

; =============== S U B R O U T I N E =======================================
; Function select for an object
LCD74               ORCC  #$01                    ; CD74: 1A 01         
                    ROR   $4684                   ; CD76: 76 46 84      
                    JSR   $6AA0                   ; CD79: BD 6A A0      
LCD7C               CLR   $4684                   ; CD7C: 7F 46 84      
; End of function sub_CD74
                    RTS                           ; CD7F: 39            

; =============== S U B R O U T I N E =======================================
; Starfield
LCD80               ORCC  #$01                    ; CD80: 1A 01           // Starfield attract screen 1 + 3 + 4
                    ROR   $4684                   ; CD82: 76 46 84      
                    JSR   $7D9A                   ; CD85: BD 7D 9A      
LCD88               CLR   $4684                   ; CD88: 7F 46 84      
; End of function sub_CD80
                    RTS                           ; CD8B: 39            

; =============== S U B R O U T I N E =======================================
; Towers surface dots
LCD8C               ORCC  #$01                    ; CD8C: 1A 01         
                    ROR   $4684                   ; CD8E: 76 46 84      
                    JSR   $7EAF                   ; CD91: BD 7E AF      
LCD94               CLR   $4684                   ; CD94: 7F 46 84      
; End of function sub_CD8C
                    RTS                           ; CD97: 39            

; =============== S U B R O U T I N E =======================================
; Unused
LCD98               ASRA                          ; CD98: 47            
                    RORB                          ; CD99: 56            
                    ASRA                          ; CD9A: 47            
; End of function sub_CD98
                    RORB                          ; CD9B: 56            
; =============== S U B R O U T I N E =======================================
; Shift D register right
Shift_D_R_6         ASRA                          ; CD9C: 47            
; End of function Shift_D_R_6
                    RORB                          ; CD9D: 56            
; =============== S U B R O U T I N E =======================================
; Shift D register right
Shift_D_R_5         ASRA                          ; CD9E: 47            
                    RORB                          ; CD9F: 56            
Shift_D_R_4         ASRA                          ; CDA0: 47            
                    RORB                          ; CDA1: 56            
Shift_D_R_3         ASRA                          ; CDA2: 47            
                    RORB                          ; CDA3: 56            
                    ASRA                          ; CDA4: 47            
                    RORB                          ; CDA5: 56            
                    ASRA                          ; CDA6: 47            
                    RORB                          ; CDA7: 56            
; End of function Shift_D_R_5
                    RTS                           ; CDA8: 39            

; =============== S U B R O U T I N E =======================================
; Shift D register left
LCDA9               ASLB                          ; CDA9: 58            
; End of function sub_CDA9
                    ROLA                          ; CDAA: 49            
; =============== S U B R O U T I N E =======================================
Shift_D_L_7         ASLB                          ; CDAB: 58            
                    ROLA                          ; CDAC: 49            
                    ASLB                          ; CDAD: 58            
                    ROLA                          ; CDAE: 49            
                    ASLB                          ; CDAF: 58            
                    ROLA                          ; CDB0: 49            
Shift_D_L_4         ASLB                          ; CDB1: 58            
                    ROLA                          ; CDB2: 49            
                    ASLB                          ; CDB3: 58            
                    ROLA                          ; CDB4: 49            
Shift_D_L_2         ASLB                          ; CDB5: 58            
                    ROLA                          ; CDB6: 49            
                    ASLB                          ; CDB7: 58            
                    ROLA                          ; CDB8: 49            
; End of function Shift_D_L_7
                    RTS                           ; CDB9: 39            

; =============== S U B R O U T I N E =======================================
; Do math program run
LCDBA               STA   $4700                   ; CDBA: B7 47 00      
Math_Wait_Til_Halt        TST   $4320                   ; CDBD: 7D 43 20      
                    BMI   Math_Wait_Til_Halt      ; CDC0: 2B FB         
; End of function Math_Run_Start
                    RTS                           ; CDC2: 39            

; =============== S U B R O U T I N E =======================================
; Initialise math registers matrix
LCDC3               LDD   #$00                    ; CDC3: CC 00 00      
                    STD   -10,U                   ; CDC6: ED 56         
                    STD   -2,U                    ; CDC8: ED 5E         
                    STD   $06,U                   ; CDCA: ED 46         
                    STD   $08,U                   ; CDCC: ED 48         
                    STD   $0A,U                   ; CDCE: ED 4A         
                    STD   $0C,U                   ; CDD0: ED 4C         
                    STD   -14,U                   ; CDD2: ED 52         
                    STD   -12,U                   ; CDD4: ED 54         
                    STD   -8,U                    ; CDD6: ED 58         
                    STD   -4,U                    ; CDD8: ED 5C         
                    STD   ,U                      ; CDDA: ED C4         
                    STD   $02,U                   ; CDDC: ED 42         
                    LDA   #$40                    ; CDDE: 86 40         
                    STD   -16,U                   ; CDE0: ED 50         
                    STD   -6,U                    ; CDE2: ED 5A         
                    STD   $04,U                   ; CDE4: ED 44         
; End of function sub_CDC3
                    RTS                           ; CDE6: 39            

; =============== S U B R O U T I N E =======================================
; Swap Matrix 2  x, y, z to  x, y, z
LCDE7               LDD   $502A                   ; CDE7: FC 50 2A        // Ay2
                    LDU   $5030                   ; CDEA: FE 50 30        // Bx2
                    STD   $5030                   ; CDED: FD 50 30        // Bx2
                    STU   $502A                   ; CDF0: FF 50 2A        // Ay2
                    LDD   $502C                   ; CDF3: FC 50 2C        // Az2
                    LDU   $5038                   ; CDF6: FE 50 38        // Cx2
                    STD   $5038                   ; CDF9: FD 50 38        // Cx2
                    STU   $502C                   ; CDFC: FF 50 2C        // Az2
                    LDD   $5034                   ; CDFF: FC 50 34        // Bz2
                    LDU   $503A                   ; CE02: FE 50 3A        // Cy2
                    STD   $503A                   ; CE05: FD 50 3A        // Cy2
                    STU   $5034                   ; CE08: FF 50 34        // Bz2
; End of function sub_CDE7
                    RTS                           ; CE0B: 39            

; =============== S U B R O U T I N E =======================================
; Copy transform data from [BIC] to matrix 2
LCE0C               STA   $4702                   ; CE0C: B7 47 02      
                    CLR   $4701                   ; CE0F: 7F 47 01      
                    LDA   #$77                    ; CE12: 86 77         
                    JSR   LCDBA                   ; CE14: BD CD BA        // Do math program run
; End of function sub_CE0C
LCE17               RTS                           ; CE17: 39            

; =============== S U B R O U T I N E =======================================
; Run math program $80 Copy [BIC] to Matrix 3
LCE18               STA   $4702                   ; CE18: B7 47 02      
                    CLR   $4701                   ; CE1B: 7F 47 01      
                    LDA   #$80                    ; CE1E: 86 80         
                    JSR   LCDBA                   ; CE20: BD CD BA        // Do math program run
; End of function sub_CE18
LCE23               RTS                           ; CE23: 39            

; =============== S U B R O U T I N E =======================================
; Run math program $00 Roll on Matrix 4
LCE24               LDD   #$10                    ; CE24: CC 00 10      
                    STD   $4701                   ; CE27: FD 47 01        // Point BIC to $5080 MReg40 Matrix 4
                    LDA   #$00                    ; CE2A: 86 00         
; End of function sub_CE24
                    JMP   LCDBA                   ; CE2C: 7E CD BA        // Do math program run

; =============== S U B R O U T I N E =======================================
; Run math program $0E Pitch on Matrix 4
LCE2F               LDD   #$10                    ; CE2F: CC 00 10      
                    STD   $4701                   ; CE32: FD 47 01        // Point BIC to $5080 MReg40 Matrix 4
                    LDA   #$0E                    ; CE35: 86 0E         
; End of function sub_CE2F
                    JMP   LCDBA                   ; CE37: 7E CD BA        // Do math program run

; =============== S U B R O U T I N E =======================================
; Run math program $1C Yaw on Matrix 4
LCE3A               LDD   #$10                    ; CE3A: CC 00 10      
                    STD   $4701                   ; CE3D: FD 47 01        // Point BIC to $5080 MReg40 Matrix 4
                    LDA   #$1C                    ; CE40: 86 1C         
; End of function sub_CE3A
                    JMP   LCDBA                   ; CE42: 7E CD BA        // Do math program run

; =============== S U B R O U T I N E =======================================
; Update random numbers
Gen_Random          LDD   <$53                    ; CE45: DC 53         
                    STD   <$54                    ; CE47: DD 54         
                    LDA   $4703                   ; CE49: B6 47 03      
                    STA   <$53                    ; CE4C: 97 53         
; End of function Gen_Random
; ---------------------------------------------------------------------------
                    RTS                           ; CE4E: 39            

; =============== S U B R O U T I N E =======================================
LD660               JSR   LD68D                   ; D660: BD D6 8D        // Point U to vector RAM $38
LD663               JSR   LD690                   ; D663: BD D6 90        // Animate laser target hit
LD666               LDD   #$C000                  ; D666: CC C0 00        // Vector RTSL instruction
                    STD   ,U++                    ; D669: ED C1         
                    JSR   LD6A0                   ; D66B: BD D6 A0      
LD66E               JSR   LD6A3                   ; D66E: BD D6 A3      
LD671               LDD   #$C000                  ; D671: CC C0 00      
                    STD   ,U++                    ; D674: ED C1         
                    JSR   LD6BC                   ; D676: BD D6 BC      
LD679               JSR   LD6BF                   ; D679: BD D6 BF      
LD67C               LDD   #$C000                  ; D67C: CC C0 00      
                    STD   ,U++                    ; D67F: ED C1         
                    JSR   LD6E7                   ; D681: BD D6 E7      
LD684               JSR   LD6EA                   ; D684: BD D6 EA      
LD687               LDD   #$C000                  ; D687: CC C0 00      
                    STD   ,U++                    ; D68A: ED C1         
; End of function sub_D660
                    RTS                           ; D68C: 39            

; =============== S U B R O U T I N E =======================================
; Point U to vector RAM $38
; End of function sub_D68D
LD68D               LDU   #$38                    ; D68D: CE 00 38      
; =============== S U B R O U T I N E =======================================
; Animate laser target hit
LD690               JSR   LD709                   ; D690: BD D7 09        // Laser target animation
LD693               LDD   ,X++                    ; D693: EC 81         
                    STD   ,U++                    ; D695: ED C1         
                    LDD   ,X++                    ; D697: EC 81         
                    STD   ,U++                    ; D699: ED C1         
                    LDD   ,X                      ; D69B: EC 84         
                    STD   ,U++                    ; D69D: ED C1         
; End of function sub_D690
                    RTS                           ; D69F: 39            

; =============== S U B R O U T I N E =======================================
; End of function sub_D6A0
LD6A0               LDU   #$46                    ; D6A0: CE 00 46      
; =============== S U B R O U T I N E =======================================
LD6A3               JSR   LD709                   ; D6A3: BD D7 09        // Laser target animation
LD6A6               LDD   ,X++                    ; D6A6: EC 81         
                    NEGB                          ; D6A8: 50            
                    ANDB  #$1F                    ; D6A9: C4 1F         
                    STD   ,U++                    ; D6AB: ED C1         
                    LDD   ,X++                    ; D6AD: EC 81         
                    NEGB                          ; D6AF: 50            
                    ORB   #$E0                    ; D6B0: CA E0         
                    STD   ,U++                    ; D6B2: ED C1         
                    LDD   ,X                      ; D6B4: EC 84         
                    NEGB                          ; D6B6: 50            
                    ANDB  #$1F                    ; D6B7: C4 1F         
                    STD   ,U++                    ; D6B9: ED C1         
; End of function sub_D6A3
                    RTS                           ; D6BB: 39            

; =============== S U B R O U T I N E =======================================
; End of function sub_D6BC
LD6BC               LDU   #$54                    ; D6BC: CE 00 54      
; =============== S U B R O U T I N E =======================================
LD6BF               JSR   LD709                   ; D6BF: BD D7 09        // Laser target animation
LD6C2               LDD   ,X++                    ; D6C2: EC 81         
                    NEGA                          ; D6C4: 40            
                    ANDA  #$1F                    ; D6C5: 84 1F         
                    ORA   #$40                    ; D6C7: 8A 40         
                    NEGB                          ; D6C9: 50            
                    ANDB  #$1F                    ; D6CA: C4 1F         
                    STD   ,U++                    ; D6CC: ED C1         
                    LDD   ,X++                    ; D6CE: EC 81         
                    NEGA                          ; D6D0: 40            
                    ANDA  #$1F                    ; D6D1: 84 1F         
                    ORA   #$40                    ; D6D3: 8A 40         
                    NEGB                          ; D6D5: 50            
                    ORB   #$E0                    ; D6D6: CA E0         
                    STD   ,U++                    ; D6D8: ED C1         
                    LDD   ,X                      ; D6DA: EC 84         
                    NEGA                          ; D6DC: 40            
                    ANDA  #$1F                    ; D6DD: 84 1F         
                    ORA   #$40                    ; D6DF: 8A 40         
                    NEGB                          ; D6E1: 50            
                    ANDB  #$1F                    ; D6E2: C4 1F         
                    STD   ,U++                    ; D6E4: ED C1         
; End of function sub_D6BF
                    RTS                           ; D6E6: 39            

; =============== S U B R O U T I N E =======================================
; End of function sub_D6E7
LD6E7               LDU   #$62                    ; D6E7: CE 00 62      
; =============== S U B R O U T I N E =======================================
LD6EA               JSR   LD709                   ; D6EA: BD D7 09        // Laser target animation
LD6ED               LDD   ,X++                    ; D6ED: EC 81         
                    NEGA                          ; D6EF: 40            
                    ANDA  #$1F                    ; D6F0: 84 1F         
                    ORA   #$40                    ; D6F2: 8A 40         
                    STD   ,U++                    ; D6F4: ED C1         
                    LDD   ,X++                    ; D6F6: EC 81         
                    NEGA                          ; D6F8: 40            
                    ANDA  #$1F                    ; D6F9: 84 1F         
                    ORA   #$40                    ; D6FB: 8A 40         
                    STD   ,U++                    ; D6FD: ED C1         
                    LDD   ,X                      ; D6FF: EC 84         
                    NEGA                          ; D701: 40            
                    ANDA  #$1F                    ; D702: 84 1F         
                    ORA   #$40                    ; D704: 8A 40         
                    STD   ,U++                    ; D706: ED C1         
; End of function sub_D6EA
                    RTS                           ; D708: 39            

; =============== S U B R O U T I N E =======================================
; Laser target animation
LD709               LDA   $4703                   ; D709: B6 47 03        // Get random number
                    ANDA  #$0F                    ; D70C: 84 0F           // Mask 0-15
                    LDB   #$06                    ; D70E: C6 06         
                    MUL                           ; D710: 3D              // Multiply by 6 for 0- 90 range
                    LDX   #word_D716              ; D711: 8E D7 16        // Table of vector SVEC instructions for laser hit target animation
                    ABX                           ; D714: 3A              // Point X reg to word from table
; End of function sub_D709
; ---------------------------------------------------------------------------
                    RTS                           ; D715: 39            

; =============== S U B R O U T I N E =======================================
; Check coin inputs
LD776               LDA   $4300                   ; D776: B6 43 00      
                    ANDA  #$0F                    ; D779: 84 0F         
                    CMPA  #$0F                    ; D77B: 81 0F         
                    BEQ   LD783                   ; D77D: 27 04           // Check for coin or slam inputs
                    LDA   #$FF                    ; D77F: 86 FF         
                    STA   <$18                    ; D781: 97 18         
LD783               LDA   <$18                    ; D783: 96 18         
                    BNE   LD788                   ; D785: 26 01         
                    RTS                           ; D787: 39            

; ---------------------------------------------------------------------------
LD788               DEC   <$18                    ; D788: 0A 18         
                    LDA   #$12                    ; D78A: 86 12         
                    CMPA  <$14                    ; D78C: 91 14           // Credits
                    BCC   LD792                   ; D78E: 24 02         
                    STA   <$14                    ; D790: 97 14           // Credits
LD792               LDA   <$17                    ; D792: 96 17         
                    STA   $4681                   ; D794: B7 46 81      
                    LDB   <$16                    ; D797: D6 16         
                    STB   $4680                   ; D799: F7 46 80      
                    ORA   <$16                    ; D79C: 9A 16         
                    ORA   <$15                    ; D79E: 9A 15         
                    BEQ   LD7A6                   ; D7A0: 27 04         
                    LDA   #$FF                    ; D7A2: 86 FF         
                    STA   <$18                    ; D7A4: 97 18         
LD7A6               LDA   $4300                   ; D7A6: B6 43 00      
                    ANDA  #$10                    ; D7A9: 84 10         
                    BNE   LD7B0                   ; D7AB: 26 03         
                    JMP   LD8AE                   ; D7AD: 7E D8 AE      

; ---------------------------------------------------------------------------
LD7B0               LDA   $4590                   ; D7B0: B6 45 90      
                    ASLA                          ; D7B3: 48            
                    ASLA                          ; D7B4: 48            
                    ASLA                          ; D7B5: 48            
                    ASLA                          ; D7B6: 48            
                    STA   <$09                    ; D7B7: 97 09         
                    LDA   $4591                   ; D7B9: B6 45 91      
                    ANDA  #$0F                    ; D7BC: 84 0F         
                    ORA   <$09                    ; D7BE: 9A 09         
                    STA   <$09                    ; D7C0: 97 09         
                    LDA   <$0A                    ; D7C2: 96 0A         
                    ANDA  #$03                    ; D7C4: 84 03         
                    BNE   LD7CB                   ; D7C6: 26 03         
                    JSR   LC09D                   ; D7C8: BD C0 9D      
LD7CB               LDX   #$480E                  ; D7CB: 8E 48 0E      
LD7CE               LDA   $4300                   ; D7CE: B6 43 00      
                    CMPX  #$480D                  ; D7D1: 8C 48 0D      
                    BEQ   LD7D9                   ; D7D4: 27 03         
                    BCC   LD7DA                   ; D7D6: 24 02         
                    LSRA                          ; D7D8: 44            
LD7D9               LSRA                          ; D7D9: 44            
LD7DA               LSRA                          ; D7DA: 44            
                    LDA   ,X                      ; D7DB: A6 84         
                    ANDA  #$1F                    ; D7DD: 84 1F         
                    BCS   LD812                   ; D7DF: 25 31         
                    BEQ   LD7EF                   ; D7E1: 27 0C         
                    CMPA  #$1B                    ; D7E3: 81 1B         
                    BCC   LD7ED                   ; D7E5: 24 06         
                    LDB   <$0A                    ; D7E7: D6 0A         
                    ANDB  #$01                    ; D7E9: C4 01         
                    BNE   LD7EF                   ; D7EB: 26 02         
LD7ED               SUBA  #$01                    ; D7ED: 80 01         
LD7EF               STA   ,X                      ; D7EF: A7 84         
                    LDA   $4300                   ; D7F1: B6 43 00      
                    ANDA  #$08                    ; D7F4: 84 08         
                    BNE   LD7FC                   ; D7F6: 26 04         
                    LDA   #$F0                    ; D7F8: 86 F0         
                    STA   <$0B                    ; D7FA: 97 0B         
LD7FC               LDA   <$0B                    ; D7FC: 96 0B         
                    BEQ   LD808                   ; D7FE: 27 08         
                    DEC   <$0B                    ; D800: 0A 0B         
                    LDA   #$00                    ; D802: 86 00         
                    STA   ,X                      ; D804: A7 84         
                    STA   $03,X                   ; D806: A7 03         
LD808               LDA   $03,X                   ; D808: A6 03         
                    BEQ   LD860                   ; D80A: 27 54         
                    DEC   $03,X                   ; D80C: 6A 03         
                    BEQ   LD82F                   ; D80E: 27 1F         
                    BRA   LD860                   ; D810: 20 4E         

; ---------------------------------------------------------------------------
LD812               CMPA  #$1B                    ; D812: 81 1B         
                    BCC   LD81E                   ; D814: 24 08         
                    LDA   ,X                      ; D816: A6 84         
                    ADDA  #$20                    ; D818: 8B 20         
                    BCC   LD7EF                   ; D81A: 24 D3         
                    BNE   LD822                   ; D81C: 26 04         
LD81E               LDA   #$1F                    ; D81E: 86 1F         
                    BRA   LD7EF                   ; D820: 20 CD         

; ---------------------------------------------------------------------------
LD822               LDA   #$1F                    ; D822: 86 1F         
                    STA   ,X                      ; D824: A7 84         
                    LDB   $03,X                   ; D826: E6 03         
                    LDA   #$78                    ; D828: 86 78         
                    STA   $03,X                   ; D82A: A7 03         
                    TSTB                          ; D82C: 5D            
                    BEQ   LD860                   ; D82D: 27 31         
LD82F               CLRA                          ; D82F: 4F            
                    CMPX  #$480D                  ; D830: 8C 48 0D      
                    BCS   LD853                   ; D833: 25 1E         
                    BEQ   LD84B                   ; D835: 27 14         
                    LDA   <$09                    ; D837: 96 09         
                    ANDA  #$0C                    ; D839: 84 0C         
                    LSRA                          ; D83B: 44            
                    LSRA                          ; D83C: 44            
                    BEQ   LD853                   ; D83D: 27 14         
                    ADDA  #$02                    ; D83F: 8B 02         
; ---------------------------------------------------------------------------
                    BRA   LD853                   ; D841: 20 10         

; ---------------------------------------------------------------------------
LD84B               LDA   <$09                    ; D84B: 96 09         
                    ANDA  #$10                    ; D84D: 84 10         
                    BEQ   LD853                   ; D84F: 27 02         
                    LDA   #$01                    ; D851: 86 01         
LD853               INCA                          ; D853: 4C            
                    TFR   A,B                     ; D854: 1F 89         
                    ADDB  <$13                    ; D856: DB 13         
                    STB   <$13                    ; D858: D7 13         
                    ADDA  <$12                    ; D85A: 9B 12         
                    STA   <$12                    ; D85C: 97 12         
                    INC   $09,X                   ; D85E: 6C 09         
LD860               LEAX  -1,X                    ; D860: 30 1F         
                    CMPX  #$480C                  ; D862: 8C 48 0C      
                    LBGE  LD7CE                   ; D865: 10 2C FF 65   
                    LDA   <$09                    ; D869: 96 09         
                    LSRA                          ; D86B: 44            
                    LSRA                          ; D86C: 44            
                    LSRA                          ; D86D: 44            
                    LSRA                          ; D86E: 44            
                    LSRA                          ; D86F: 44            
                    LDB   <$13                    ; D870: D6 13         
                    ASLB                          ; D872: 58            
                    LDX   #byte_D843              ; D873: 8E D8 43      
                    SUBB  A,X                     ; D876: E0 86         
                    BCS   LD88B                   ; D878: 25 11         
                    ASL   <$13                    ; D87A: 08 13         
                    RORB                          ; D87C: 56            
                    CMPA  #$03                    ; D87D: 81 03         
                    BEQ   LD885                   ; D87F: 27 04         
                    ADDB  #$80                    ; D881: CB 80         
                    BCC   LD889                   ; D883: 24 04         
LD885               INC   <$12                    ; D885: 0C 12         
                    INC   <$12                    ; D887: 0C 12         
LD889               STB   <$13                    ; D889: D7 13         
LD88B               LDA   <$09                    ; D88B: 96 09         
                    ANDA  #$03                    ; D88D: 84 03         
                    BEQ   LD8AC                   ; D88F: 27 1B         
                    TFR   A,B                     ; D891: 1F 89         
                    NEGA                          ; D893: 40            
                    ASRA                          ; D894: 47            
                    ADDA  <$12                    ; D895: 9B 12         
                    BPL   LD8A4                   ; D897: 2A 0B         
                    TST   <$13                    ; D899: 0D 13         
                    BPL   LD8AE                   ; D89B: 2A 11         
                    INCA                          ; D89D: 4C            
                    BMI   LD8AE                   ; D89E: 2B 0E         
                    ASL   <$13                    ; D8A0: 08 13         
                    LSR   <$13                    ; D8A2: 04 13         
LD8A4               CMPB  #$01                    ; D8A4: C1 01         
                    BNE   LD8AA                   ; D8A6: 26 02         
                    INC   <$14                    ; D8A8: 0C 14           // Credits
LD8AA               INC   <$14                    ; D8AA: 0C 14           // Credits
LD8AC               STA   <$12                    ; D8AC: 97 12         
LD8AE               LDB   <$0A                    ; D8AE: D6 0A         
                    ANDB  #$0F                    ; D8B0: C4 0F         
                    BNE   locret_D8DE             ; D8B2: 26 2A         
                    LDX   #$4817                  ; D8B4: 8E 48 17      
LD8B7               LDA   ,X                      ; D8B7: A6 84         
                    BPL   LD8C0                   ; D8B9: 2A 05         
                    ANDA  #$7F                    ; D8BB: 84 7F         
                    INCB                          ; D8BD: 5C            
                    STA   ,X                      ; D8BE: A7 84         
LD8C0               LEAX  -1,X                    ; D8C0: 30 1F         
                    CMPX  #$4815                  ; D8C2: 8C 48 15      
                    BGE   LD8B7                   ; D8C5: 2C F0         
                    TSTB                          ; D8C7: 5D            
                    BNE   locret_D8DE             ; D8C8: 26 14         
                    LDX   #$4817                  ; D8CA: 8E 48 17      
LD8CD               LDA   ,X                      ; D8CD: A6 84         
                    BEQ   LD8D7                   ; D8CF: 27 06         
                    ADDA  #$7F                    ; D8D1: 8B 7F         
                    STA   ,X                      ; D8D3: A7 84         
                    BRA   locret_D8DE             ; D8D5: 20 07         

; ---------------------------------------------------------------------------
LD8D7               LEAX  -1,X                    ; D8D7: 30 1F         
                    CMPX  #$4815                  ; D8D9: 8C 48 15      
                    BGE   LD8CD                   ; D8DC: 2C EF         
; End of function sub_D776
locret_D8DE         RTS                           ; D8DE: 39            

; =============== S U B R O U T I N E =======================================
; Called from select screen, attract screen 1 + 3 when writing text
LD8DF               LDX   #$4A52                  ; D8DF: 8E 4A 52      
                    CPX   $4AD9                   ; D8E2: BC 4A D9      
                    BCC   LD8F1                   ; D8E5: 24 0A         
LD8E7               CMPA  ,X+                     ; D8E7: A1 80         
                    BNE   LD8EC                   ; D8E9: 26 01         
                    RTS                           ; D8EB: 39            

; ---------------------------------------------------------------------------
LD8EC               CPX   $4AD9                   ; D8EC: BC 4A D9      
                    BCS   LD8E7                   ; D8EF: 25 F6         
LD8F1               STA   ,X+                     ; D8F1: A7 80         
                    STX   $4AD9                   ; D8F3: BF 4A D9      
; End of function sub_D8DF
                    RTS                           ; D8F6: 39            

; =============== S U B R O U T I N E =======================================
; Doesnt seem to be used anywhere
LD8F7               LDX   #$4A52                  ; D8F7: 8E 4A 52      
LD8FA               CMPA  ,X                      ; D8FA: A1 84         
                    BNE   LD912                   ; D8FC: 26 14         
                    LDU   $4AD9                   ; D8FE: FE 4A D9      
                    CMPU  #$4A52                  ; D901: 11 83 4A 52   
                    BLS   LD912                   ; D905: 23 0B         
                    LEAU  -1,U                    ; D907: 33 5F         
                    LDA   ,U                      ; D909: A6 C4         
                    STA   ,X                      ; D90B: A7 84         
                    STU   $4AD9                   ; D90D: FF 4A D9      
                    LEAX  ,U                      ; D910: 30 C4         
LD912               LEAX  $01,X                   ; D912: 30 01         
                    CPX   $4AD9                   ; D914: BC 4A D9      
                    BCS   LD8FA                   ; D917: 25 E1         
; End of function sub_D8F7
                    RTS                           ; D919: 39            

; =============== S U B R O U T I N E =======================================
LD91A               LDX   #$4A52                  ; D91A: 8E 4A 52      
                    CLR   ,X                      ; D91D: 6F 84         
                    STX   $4AD9                   ; D91F: BF 4A D9      
; End of function sub_D91A
                    RTS                           ; D922: 39            

; =============== S U B R O U T I N E =======================================
; Called from attract screen 1
LD923               LDU   #$4A52                  ; D923: CE 4A 52      
                    CMPU  $4AD9                   ; D926: 11 B3 4A D9   
                    BCC   locret_D941             ; D92A: 24 15         
LD92C               LDB   ,U+                     ; D92C: E6 C0         
                    CMPB  #$D6                    ; D92E: C1 D6         
                    BCC   LD93B                   ; D930: 24 09         
                    STB   $48AE                   ; D932: F7 48 AE        // Text string index
                    JSR   LE7DD                   ; D935: BD E7 DD        // Insert text colour vector instruction
LD938               JSR   LE7FC                   ; D938: BD E7 FC      
LD93B               CMPU  $4AD9                   ; D93B: 11 B3 4A D9   
                    BCS   LD92C                   ; D93F: 25 EB         
; End of function sub_D923
locret_D941         RTS                           ; D941: 39            

; =============== S U B R O U T I N E =======================================
; Called from Attract screen 3 + 4
LD942               LDU   #$4A52                  ; D942: CE 4A 52      
                    CMPU  $4AD9                   ; D945: 11 B3 4A D9   
                    BCC   locret_D95D             ; D949: 24 12         
LD94B               LDB   ,U+                     ; D94B: E6 C0         
                    CMPB  #$D6                    ; D94D: C1 D6         
                    BCC   LD957                   ; D94F: 24 06         
                    STB   $48AE                   ; D951: F7 48 AE        // Text string index
                    JSR   LE7FC                   ; D954: BD E7 FC      
LD957               CMPU  $4AD9                   ; D957: 11 B3 4A D9   
                    BCS   LD94B                   ; D95B: 25 EE         
; End of function sub_D942
locret_D95D         RTS                           ; D95D: 39            

; =============== S U B R O U T I N E =======================================
; Attract screen 2 text position control
LD95E               LDU   $4ADD                   ; D95E: FE 4A DD      
                    STA   ,U+                     ; D961: A7 C0         
                    LDD   #$00                    ; D963: CC 00 00      
                    STD   ,U++                    ; D966: ED C1         
                    LDD   #$0100                  ; D968: CC 01 00      
                    STD   ,U++                    ; D96B: ED C1         
                    STU   $4ADD                   ; D96D: FF 4A DD      
; End of function sub_D95E
                    RTS                           ; D970: 39            

; =============== S U B R O U T I N E =======================================
LD971               LDU   #$4A66                  ; D971: CE 4A 66      
LD974               CMPA  ,U                      ; D974: A1 C4         
                    BNE   LD97C                   ; D976: 26 04         
                    LDA   #$00                    ; D978: 86 00         
                    STD   ,U                      ; D97A: ED C4         
LD97C               LEAU  $05,U                   ; D97C: 33 45         
                    CMPU  $4ADD                   ; D97E: 11 B3 4A DD   
                    BCS   LD974                   ; D982: 25 F0         
; End of function sub_D971
                    RTS                           ; D984: 39            

; =============== S U B R O U T I N E =======================================
LD985               LDU   #$4A66                  ; D985: CE 4A 66      
                    CMPU  $4ADD                   ; D988: 11 B3 4A DD   
                    BCC   locret_D9DB             ; D98C: 24 4D         
                    LDD   #$7200                  ; D98E: CC 72 00      
                    STD   ,Y++                    ; D991: ED A1         
LD993               LDA   ,U+                     ; D993: A6 C0         
                    BEQ   LD9D3                   ; D995: 27 3C         
                    STA   $48AE                   ; D997: B7 48 AE        // Text string index
                    LDD   #$0198                  ; D99A: CC 01 98      
                    STD   ,Y++                    ; D99D: ED A1         
                    LDD   #$00                    ; D99F: CC 00 00      
                    STD   ,Y++                    ; D9A2: ED A1         
                    LDB   ,U                      ; D9A4: E6 C4         
                    LDA   #$71                    ; D9A6: 86 71         
                    STD   ,Y++                    ; D9A8: ED A1         
                    COMB                          ; D9AA: 53            
                    ADDB  #$10                    ; D9AB: CB 10         
                    LDA   #$62                    ; D9AD: 86 62         
                    STD   ,Y++                    ; D9AF: ED A1         
                    LDX   #off_E99E               ; D9B1: 8E E9 9E      
                    LDB   $48AE                   ; D9B4: F6 48 AE        // Text string index
                    ABX                           ; D9B7: 3A            
                    ABX                           ; D9B8: 3A            
                    LDD   #$1DD0                  ; D9B9: CC 1D D0      
                    STD   ,Y++                    ; D9BC: ED A1         
                    LDD   ,X                      ; D9BE: EC 84         
                    ANDA  #$1F                    ; D9C0: 84 1F         
                    ORA   #$00                    ; D9C2: 8A 00         
                    STD   ,Y++                    ; D9C4: ED A1         
                    JSR   LE821                   ; D9C6: BD E8 21        // Text handling
LD9C9               LDD   #$7200                  ; D9C9: CC 72 00      
                    STD   ,Y++                    ; D9CC: ED A1         
                    LDD   #$8040                  ; D9CE: CC 80 40      
                    STD   ,Y++                    ; D9D1: ED A1         
LD9D3               LEAU  $04,U                   ; D9D3: 33 44         
                    CMPU  $4ADD                   ; D9D5: 11 B3 4A DD   
                    BCS   LD993                   ; D9D9: 25 B8         
; End of function sub_D985
locret_D9DB         RTS                           ; D9DB: 39            

; =============== S U B R O U T I N E =======================================
; Initialise before game start
LD9DC               LDD   #$00                    ; D9DC: CC 00 00      
                    STD   $4AE4                   ; D9DF: FD 4A E4      
                    LDD   #$6018                  ; D9E2: CC 60 18      
                    STD   $4AE6                   ; D9E5: FD 4A E6      
                    LDD   #$4A66                  ; D9E8: CC 4A 66      
                    STD   $4ADD                   ; D9EB: FD 4A DD      
                    LDD   word_DB2F               ; D9EE: FC DB 2F      
                    STD   $4AE2                   ; D9F1: FD 4A E2      
                    LDA   #$51                    ; D9F4: 86 51         
                    STA   $4ADF                   ; D9F6: B7 4A DF      
; End of function sub_D9DC
                    RTS                           ; D9F9: 39            

; =============== S U B R O U T I N E =======================================
; Attract screen 2 large Star Wars logo handling
LD9FA               LDD   $4AE4                   ; D9FA: FC 4A E4      
                    ADDD  #$01                    ; D9FD: C3 00 01      
                    STD   $4AE4                   ; DA00: FD 4A E4      
                    CMPD  #$F8                    ; DA03: 10 83 00 F8   
                    LBCC  LDA94                   ; DA07: 10 24 00 89   
                    CMPD  #$40                    ; DA0B: 10 83 00 40   
                    BCC   LDA1E                   ; DA0F: 24 0D         
                    LDD   $4AE6                   ; DA11: FC 4A E6      
                    ADDB  #$03                    ; DA14: CB 03         
                    STD   $4AE6                   ; DA16: FD 4A E6      
                    LDD   #$40                    ; DA19: CC 00 40      
                    BRA   LDA2A                   ; DA1C: 20 0C         

; ---------------------------------------------------------------------------
LDA1E               LDD   $4AE4                   ; DA1E: FC 4A E4      
                    COMB                          ; DA21: 53            
                    ADDB  #$18                    ; DA22: CB 18         
                    STD   $4AE6                   ; DA24: FD 4A E6      
                    LDD   $4AE4                   ; DA27: FC 4A E4      
LDA2A               ORA   #$73                    ; DA2A: 8A 73         
                    STD   $4AE8                   ; DA2C: FD 4A E8      
                    LDD   $4AE6                   ; DA2F: FC 4A E6      
                    ORA   #$61                    ; DA32: 8A 61         
                    STD   ,Y++                    ; DA34: ED A1         
                    LDD   #$0198                  ; DA36: CC 01 98      
                    STD   ,Y                      ; DA39: ED A4         
                    STD   $08,Y                   ; DA3B: ED 28         
                    STD   $10,Y                   ; DA3D: ED A8 10      
                    STD   $18,Y                   ; DA40: ED A8 18      
                    STD   $20,Y                   ; DA43: ED A8 20      
                    STD   $28,Y                   ; DA46: ED A8 28      
                    LDD   #$00                    ; DA49: CC 00 00      
                    STD   $02,Y                   ; DA4C: ED 22         
                    STD   $0A,Y                   ; DA4E: ED 2A         
                    STD   $12,Y                   ; DA50: ED A8 12      
                    STD   $1A,Y                   ; DA53: ED A8 1A      
                    STD   $22,Y                   ; DA56: ED A8 22      
                    STD   $2A,Y                   ; DA59: ED A8 2A      
                    LDD   $4AE8                   ; DA5C: FC 4A E8      
                    STD   $04,Y                   ; DA5F: ED 24         
                    STD   $0C,Y                   ; DA61: ED 2C         
                    STD   $14,Y                   ; DA63: ED A8 14      
                    STD   $1C,Y                   ; DA66: ED A8 1C      
                    STD   $24,Y                   ; DA69: ED A8 24      
                    STD   $2C,Y                   ; DA6C: ED A8 2C      
                    LDD   #$B400                  ; DA6F: CC B4 00      
                    STD   $06,Y                   ; DA72: ED 26         
                    LDD   #$B434                  ; DA74: CC B4 34      
                    STD   $0E,Y                   ; DA77: ED 2E         
                    LDD   #$B458                  ; DA79: CC B4 58      
                    STD   $16,Y                   ; DA7C: ED A8 16      
                    LDD   #$B488                  ; DA7F: CC B4 88      
                    STD   $1E,Y                   ; DA82: ED A8 1E      
                    LDD   #$B4AE                  ; DA85: CC B4 AE      
                    STD   $26,Y                   ; DA88: ED A8 26      
                    LDD   #$B4D2                  ; DA8B: CC B4 D2      
                    STD   $2E,Y                   ; DA8E: ED A8 2E      
                    LEAY  $30,Y                   ; DA91: 31 A8 30      
LDA94               LDX   #$4A66                  ; DA94: 8E 4A 66      
                    CPX   $4ADD                   ; DA97: BC 4A DD      
                    BCC   LDAF5                   ; DA9A: 24 59         
LDA9C               LDD   $4AE4                   ; DA9C: FC 4A E4      
                    CMPD  #$E0                    ; DA9F: 10 83 00 E0   
                    BCC   LDAB3                   ; DAA3: 24 0E         
                    CMPD  #$40                    ; DAA5: 10 83 00 40   
                    BCS   LDAB1                   ; DAA9: 25 06         
                    LDD   $01,X                   ; DAAB: EC 01         
                    ADDD  $03,X                   ; DAAD: E3 03         
                    STD   $01,X                   ; DAAF: ED 01         
LDAB1               BRA   LDAEE                   ; DAB1: 20 3B         

; ---------------------------------------------------------------------------
LDAB3               CMPD  #$0160                  ; DAB3: 10 83 01 60   
                    BCC   LDAC1                   ; DAB7: 24 08         
                    LDD   #$0400                  ; DAB9: CC 04 00      
                    STD   $4A69                   ; DABC: FD 4A 69      
                    BRA   LDAEE                   ; DABF: 20 2D         

; ---------------------------------------------------------------------------
LDAC1               LDD   $01,X                   ; DAC1: EC 01         
                    ADDD  $03,X                   ; DAC3: E3 03         
                    STD   $01,X                   ; DAC5: ED 01         
                    CMPD  #$F000                  ; DAC7: 10 83 F0 00   
                    BCS   LDAEE                   ; DACB: 25 21         
                    LDA   ,X                      ; DACD: A6 84         
                    INCA                          ; DACF: 4C            
                    LDU   #$4A66                  ; DAD0: CE 4A 66      
LDAD3               CMPA  ,U                      ; DAD3: A1 C4         
                    BNE   LDADF                   ; DAD5: 26 08         
                    LDD   #$0400                  ; DAD7: CC 04 00      
                    STD   $03,U                   ; DADA: ED 43         
                    LDU   $4ADD                   ; DADC: FE 4A DD      
LDADF               LEAU  $05,U                   ; DADF: 33 45         
                    CMPU  $4ADD                   ; DAE1: 11 B3 4A DD   
                    BCS   LDAD3                   ; DAE5: 25 EC         
                    LDA   ,X                      ; DAE7: A6 84         
                    JSR   LD971                   ; DAE9: BD D9 71      
LDAEC               LEAX  -5,X                    ; DAEC: 30 1B         
LDAEE               LEAX  $05,X                   ; DAEE: 30 05         
                    CPX   $4ADD                   ; DAF0: BC 4A DD      
                    BCS   LDA9C                   ; DAF3: 25 A7         
LDAF5               LDD   $4AE4                   ; DAF5: FC 4A E4      
                    CMPD  #$0200                  ; DAF8: 10 83 02 00   
                    BCS   LDB03                   ; DAFC: 25 05         
                    LDA   #$07                    ; DAFE: 86 07         
                    STA   $4841                   ; DB00: B7 48 41      
LDB03               CMPD  $4AE2                   ; DB03: 10 B3 4A E2   
                    BCS   locret_DB2E             ; DB07: 25 25         
                    LDA   $4ADF                   ; DB09: B6 4A DF      
                    JSR   LD95E                   ; DB0C: BD D9 5E        // Attract screen 2 text position control
LDB0F               LDA   $4ADF                   ; DB0F: B6 4A DF      
                    INCA                          ; DB12: 4C            
                    CMPA  #$59                    ; DB13: 81 59         
                    BCS   LDB1F                   ; DB15: 25 08         
                    LDD   #$FFFF                  ; DB17: CC FF FF      
                    STD   $4AE2                   ; DB1A: FD 4A E2      
                    BRA   locret_DB2E             ; DB1D: 20 0F         

; ---------------------------------------------------------------------------
LDB1F               STA   $4ADF                   ; DB1F: B7 4A DF      
                    LDX   #$DA8D                  ; DB22: 8E DA 8D      
                    TFR   A,B                     ; DB25: 1F 89         
                    ABX                           ; DB27: 3A            
                    ABX                           ; DB28: 3A            
                    LDD   ,X                      ; DB29: EC 84         
                    STD   $4AE2                   ; DB2B: FD 4A E2      
; End of function sub_D9FA
; ---------------------------------------------------------------------------
locret_DB2E         RTS                           ; DB2E: 39            

                    FCB   $80                     ; DB38:  ' ' 80 00          SUBA  #$00 ; 
                    FCB   $90                     ; DB3A:  ' ' 90 00          SUBA  <$00 ; 
                    FCB   $A0                     ; DB3C:  ' ' A0 00          SUBA  0,X ; 
                    FCB   $20                     ; DB49:  ' ' 20 31          BRA   $DB7C ; 
                    FCB   $31                     ; DB4A:  '1' 31 39          LEAY  -7,Y ; 
                    FCB   $39                     ; DB4B:  '9' 39             RTS   ; 
                    FCB   $38                     ; DB4C:  '8' 38             Invalid ; 
                    FCB   $33                     ; DB4D:  '3' 33 20          LEAU  0,Y ; 
                    FCB   $20                     ; DB4E:  ' ' 20 4C          BRA   $DB9C ; 
                    FCB   $4C                     ; DB4F:  'L' 4C             INCA  ; 
                    FCB   $55                     ; DB50:  'U' 55             Invalid ; 
                    FCB   $43                     ; DB51:  'C' 43             COMA  ; 
                    FCB   $41                     ; DB52:  'A' 41             Invalid ; 
                    FCB   $53                     ; DB53:  'S' 53             COMB  ; 
                    FCB   $46                     ; DB54:  'F' 46             RORA  ; 
                    FCB   $49                     ; DB55:  'I' 49             ROLA  ; 
                    FCB   $4C                     ; DB56:  'L' 4C             INCA  ; 
                    FCB   $4D                     ; DB57:  'M' 4D             TSTA  ; 
                    FCB   $20                     ; DB58:  ' ' 20 4C          BRA   $DBA6 ; 
                    FCB   $4C                     ; DB59:  'L' 4C             INCA  ; 
                    FCB   $54                     ; DB5A:  'T' 54             LSRB  ; 
                    FCB   $44                     ; DB5B:  'D' 44             LSRA  ; 
                    FCB   $2E                     ; DB5C:  '.' 2E 20          BGT   $DB7E ; 
                    FCB   $20                     ; DB5D:  ' ' 20 41          BRA   $DBA0 ; 
                    FCB   $41                     ; DB5E:  'A' 41             Invalid ; 
                    FCB   $4E                     ; DB5F:  'N' 4E             Invalid ; 
                    FCB   $44                     ; DB60:  'D' 44             LSRA  ; 
                    FCB   $20                     ; DB61:  ' ' 20 41          BRA   $DBA4 ; 
                    FCB   $41                     ; DB62:  'A' 41             Invalid ; 
                    FCB   $54                     ; DB63:  'T' 54             LSRB  ; 
                    FCB   $41                     ; DB64:  'A' 41             Invalid ; 
                    FCB   $52                     ; DB65:  'R' 52             Invalid ; 
                    FCB   $49                     ; DB66:  'I' 49             ROLA  ; 
                    FCB   $2C                     ; DB67:  ',' 2C 49          BGE   $DBB2 ; 
                    FCB   $49                     ; DB68:  'I' 49             ROLA  ; 
                    FCB   $4E                     ; DB69:  'N' 4E             Invalid ; 
                    FCB   $43                     ; DB6A:  'C' 43             COMA  ; 
                    FCB   $AE                     ; DB6B:  ' ' AE 41          LDX   1,U ; 
                    FCB   $41                     ; DB6C:  'A' 41             Invalid ; 
                    FCB   $4C                     ; DB6D:  'L' 4C             INCA  ; 
                    FCB   $4C                     ; DB6E:  'L' 4C             INCA  ; 
                    FCB   $20                     ; DB6F:  ' ' 20 52          BRA   $DBC3 ; 
                    FCB   $52                     ; DB70:  'R' 52             Invalid ; 
                    FCB   $49                     ; DB71:  'I' 49             ROLA  ; 
                    FCB   $47                     ; DB72:  'G' 47             ASRA  ; 
                    FCB   $48                     ; DB73:  'H' 48             ASLA  ; 
                    FCB   $54                     ; DB74:  'T' 54             LSRB  ; 
                    FCB   $53                     ; DB75:  'S' 53             COMB  ; 
                    FCB   $20                     ; DB76:  ' ' 20 52          BRA   $DBCA ; 
                    FCB   $52                     ; DB77:  'R' 52             Invalid ; 
                    FCB   $45                     ; DB78:  'E' 45             Invalid ; 
                    FCB   $53                     ; DB79:  'S' 53             COMB  ; 
                    FCB   $45                     ; DB7A:  'E' 45             Invalid ; 
                    FCB   $52                     ; DB7B:  'R' 52             Invalid ; 
                    FCB   $56                     ; DB7C:  'V' 56             RORB  ; 
                    FCB   $45                     ; DB7D:  'E' 45             Invalid ; 
                    FCB   $44                     ; DB7E:  'D' 44             LSRA  ; 
                    FCB   $AE                     ; DB7F:  ' ' AE 4C          LDX   12,U ; 
                    FCB   $4C                     ; DB80:  'L' 4C             INCA  ; 
                    FCB   $55                     ; DB81:  'U' 55             Invalid ; 
                    FCB   $43                     ; DB82:  'C' 43             COMA  ; 
                    FCB   $41                     ; DB83:  'A' 41             Invalid ; 
                    FCB   $53                     ; DB84:  'S' 53             COMB  ; 
                    FCB   $46                     ; DB85:  'F' 46             RORA  ; 
                    FCB   $49                     ; DB86:  'I' 49             ROLA  ; 
                    FCB   $4C                     ; DB87:  'L' 4C             INCA  ; 
                    FCB   $4D                     ; DB88:  'M' 4D             TSTA  ; 
                    FCB   $20                     ; DB89:  ' ' 20 54          BRA   $DBDF ; 
                    FCB   $54                     ; DB8A:  'T' 54             LSRB  ; 
                    FCB   $52                     ; DB8B:  'R' 52             Invalid ; 
                    FCB   $41                     ; DB8C:  'A' 41             Invalid ; 
                    FCB   $44                     ; DB8D:  'D' 44             LSRA  ; 
                    FCB   $45                     ; DB8E:  'E' 45             Invalid ; 
                    FCB   $4D                     ; DB8F:  'M' 4D             TSTA  ; 
                    FCB   $41                     ; DB90:  'A' 41             Invalid ; 
                    FCB   $52                     ; DB91:  'R' 52             Invalid ; 
                    FCB   $4B                     ; DB92:  'K' 4B             Invalid ; 
                    FCB   $53                     ; DB93:  'S' 53             COMB  ; 
                    FCB   $20                     ; DB94:  ' ' 20 55          BRA   $DBEB ; 
                    FCB   $55                     ; DB95:  'U' 55             Invalid ; 
                    FCB   $53                     ; DB96:  'S' 53             COMB  ; 
                    FCB   $45                     ; DB97:  'E' 45             Invalid ; 
                    FCB   $44                     ; DB98:  'D' 44             LSRA  ; 
                    FCB   $20                     ; DB99:  ' ' 20 55          BRA   $DBF0 ; 
                    FCB   $55                     ; DB9A:  'U' 55             Invalid ; 
                    FCB   $4E                     ; DB9B:  'N' 4E             Invalid ; 
                    FCB   $44                     ; DB9C:  'D' 44             LSRA  ; 
                    FCB   $45                     ; DB9D:  'E' 45             Invalid ; 
                    FCB   $52                     ; DB9E:  'R' 52             Invalid ; 
                    FCB   $20                     ; DB9F:  ' ' 20 4C          BRA   $DBED ; 
                    FCB   $4C                     ; DBA0:  'L' 4C             INCA  ; 
                    FCB   $49                     ; DBA1:  'I' 49             ROLA  ; 
                    FCB   $43                     ; DBA2:  'C' 43             COMA  ; 
                    FCB   $45                     ; DBA3:  'E' 45             Invalid ; 
                    FCB   $4E                     ; DBA4:  'N' 4E             Invalid ; 
                    FCB   $53                     ; DBA5:  'S' 53             COMB  ; 
                    FCB   $45                     ; DBA6:  'E' 45             Invalid ; 
                    FCB   $AE                     ; DBA7:  ' ' AE 47          LDX   7,U ; 
                    FCB   $49                     ; DC7E:  'I' 49             ROLA  ; 
                    FCB   $4C                     ; DC7F:  'L' 4C             INCA  ; 
                    FCB   $4C                     ; DC80:  'L' 4C             INCA  ; 
                    FCB   $20                     ; DC81:  ' ' 20 50          BRA   $DCD3 ; 
                    FCB   $50                     ; DC82:  'P' 50             NEGB  ; 
                    FCB   $52                     ; DC83:  'R' 52             Invalid ; 
                    FCB   $4F                     ; DC84:  'O' 4F             CLRA  ; 
                    FCB   $54                     ; DC85:  'T' 54             LSRB  ; 
                    FCB   $45                     ; DC86:  'E' 45             Invalid ; 
                    FCB   $43                     ; DC87:  'C' 43             COMA  ; 
                    FCB   $54                     ; DC88:  'T' 54             LSRB  ; 
                    FCB   $20                     ; DC89:  ' ' 20 59          BRA   $DCE4 ; 
                    FCB   $59                     ; DC8A:  'Y' 59             ROLB  ; 
                    FCB   $4F                     ; DC8B:  'O' 4F             CLRA  ; 
                    FCB   $55                     ; DC8C:  'U' 55             Invalid ; 
                    FCB   $20                     ; DC8D:  ' ' 20 46          BRA   $DCD5 ; 
                    FCB   $46                     ; DC8E:  'F' 46             RORA  ; 
                    FCB   $4F                     ; DC8F:  'O' 4F             CLRA  ; 
                    FCB   $52                     ; DC90:  'R' 52             Invalid ; 
                    FCB   $20                     ; DC91:  ' ' 20 20          BRA   $DCB3 ; 
                    FCB   $20                     ; DC92:  ' ' 20 20          BRA   $DCB4 ; 
                    FCB   $20                     ; DC93:  ' ' 20 43          BRA   $DCD8 ; 
                    FCB   $43                     ; DC94:  'C' 43             COMA  ; 
                    FCB   $4F                     ; DC95:  'O' 4F             CLRA  ; 
                    FCB   $4C                     ; DC96:  'L' 4C             INCA  ; 
                    FCB   $4C                     ; DC97:  'L' 4C             INCA  ; 
                    FCB   $49                     ; DC98:  'I' 49             ROLA  ; 
                    FCB   $53                     ; DC99:  'S' 53             COMB  ; 
                    FCB   $49                     ; DC9A:  'I' 49             ROLA  ; 
                    FCB   $4F                     ; DC9B:  'O' 4F             CLRA  ; 
                    FCB   $4E                     ; DC9C:  'N' 4E             Invalid ; 
                    FCB   $53                     ; DC9D:  'S' 53             COMB  ; 
                    FCB   $AE                     ; DC9E:  ' ' AE 32          LDX   -14,Y ; 
                    FCB   $52                     ; DD04:  'R' 52             Invalid ; 
                    FCB   $45                     ; DD05:  'E' 45             Invalid ; 
                    FCB   $4E                     ; DD06:  'N' 4E             Invalid ; 
                    FCB   $43                     ; DD07:  'C' 43             COMA  ; 
                    FCB   $48                     ; DD08:  'H' 48             ASLA  ; 
                    FCB   $20                     ; DD09:  ' ' 20 43          BRA   $DD4E ; 
                    FCB   $43                     ; DD0A:  'C' 43             COMA  ; 
                    FCB   $41                     ; DD0B:  'A' 41             Invalid ; 
                    FCB   $54                     ; DD0C:  'T' 54             LSRB  ; 
                    FCB   $57                     ; DD0D:  'W' 57             ASRB  ; 
                    FCB   $41                     ; DD0E:  'A' 41             Invalid ; 
                    FCB   $4C                     ; DD0F:  'L' 4C             INCA  ; 
                    FCB   $4B                     ; DD10:  'K' 4B             Invalid ; 
                    FCB   $AE                     ; DD11:  ' ' AE 33          LDX   -13,Y ; 
                    FCB   $4F                     ; DD57:  'O' 4F             CLRA  ; 
                    FCB   $57                     ; DD58:  'W' 57             ASRB  ; 
                    FCB   $45                     ; DD59:  'E' 45             Invalid ; 
                    FCB   $52                     ; DD5A:  'R' 52             Invalid ; 
                    FCB   $20                     ; DD5B:  ' ' 20 54          BRA   $DDB1 ; 
                    FCB   $54                     ; DD5C:  'T' 54             LSRB  ; 
                    FCB   $4F                     ; DD5D:  'O' 4F             CLRA  ; 
                    FCB   $50                     ; DD5E:  'P' 50             NEGB  ; 
                    FCB   $53                     ; DD5F:  'S' 53             COMB  ; 
                    FCB   $20                     ; DD60:  ' ' 20 41          BRA   $DDA3 ; 
                    FCB   $41                     ; DD61:  'A' 41             Invalid ; 
                    FCB   $4E                     ; DD62:  'N' 4E             Invalid ; 
                    FCB   $44                     ; DD63:  'D' 44             LSRA  ; 
                    FCB   $20                     ; DD64:  ' ' 20 54          BRA   $DDBA ; 
                    FCB   $54                     ; DD65:  'T' 54             LSRB  ; 
                    FCB   $52                     ; DD66:  'R' 52             Invalid ; 
                    FCB   $45                     ; DD67:  'E' 45             Invalid ; 
                    FCB   $4E                     ; DD68:  'N' 4E             Invalid ; 
                    FCB   $43                     ; DD69:  'C' 43             COMA  ; 
                    FCB   $48                     ; DD6A:  'H' 48             ASLA  ; 
                    FCB   $20                     ; DD6B:  ' ' 20 54          BRA   $DDC1 ; 
                    FCB   $54                     ; DD6C:  'T' 54             LSRB  ; 
                    FCB   $55                     ; DD6D:  'U' 55             Invalid ; 
                    FCB   $52                     ; DD6E:  'R' 52             Invalid ; 
                    FCB   $52                     ; DD6F:  'R' 52             Invalid ; 
                    FCB   $45                     ; DD70:  'E' 45             Invalid ; 
                    FCB   $54                     ; DD71:  'T' 54             LSRB  ; 
                    FCB   $53                     ; DD72:  'S' 53             COMB  ; 
                    FCB   $AE                     ; DD73:  ' ' AE 34          LDX   -12,Y ; 
                    FCB   $4D                     ; DD94:  'M' 4D             TSTA  ; 
                    FCB   $50                     ; DD95:  'P' 50             NEGB  ; 
                    FCB   $41                     ; DD96:  'A' 41             Invalid ; 
                    FCB   $43                     ; DD97:  'C' 43             COMA  ; 
                    FCB   $54                     ; DD98:  'T' 54             LSRB  ; 
                    FCB   $20                     ; DD99:  ' ' 20 59          BRA   $DDF4 ; 
                    FCB   $59                     ; DD9A:  'Y' 59             ROLB  ; 
                    FCB   $4F                     ; DD9B:  'O' 4F             CLRA  ; 
                    FCB   $55                     ; DD9C:  'U' 55             Invalid ; 
                    FCB   $52                     ; DD9D:  'R' 52             Invalid ; 
                    FCB   $20                     ; DD9E:  ' ' 20 53          BRA   $DDF3 ; 
                    FCB   $53                     ; DD9F:  'S' 53             COMB  ; 
                    FCB   $48                     ; DDA0:  'H' 48             ASLA  ; 
                    FCB   $49                     ; DDA1:  'I' 49             ROLA  ; 
                    FCB   $45                     ; DDA2:  'E' 45             Invalid ; 
                    FCB   $4C                     ; DDA3:  'L' 4C             INCA  ; 
                    FCB   $44                     ; DDA4:  'D' 44             LSRA  ; 
                    FCB   $AE                     ; DDA5:  ' ' AE 35          LDX   -11,Y ; 
                    FCB   $4F                     ; DDCA:  'O' 4F             CLRA  ; 
                    FCB   $55                     ; DDCB:  'U' 55             Invalid ; 
                    FCB   $20                     ; DDCC:  ' ' 20 54          BRA   $DE22 ; 
                    FCB   $54                     ; DDCD:  'T' 54             LSRB  ; 
                    FCB   $4F                     ; DDCE:  'O' 4F             CLRA  ; 
                    FCB   $20                     ; DDCF:  ' ' 20 53          BRA   $DE24 ; 
                    FCB   $53                     ; DDD0:  'S' 53             COMB  ; 
                    FCB   $54                     ; DDD1:  'T' 54             LSRB  ; 
                    FCB   $4F                     ; DDD2:  'O' 4F             CLRA  ; 
                    FCB   $50                     ; DDD3:  'P' 50             NEGB  ; 
                    FCB   $20                     ; DDD4:  ' ' 20 54          BRA   $DE2A ; 
                    FCB   $54                     ; DDD5:  'T' 54             LSRB  ; 
                    FCB   $48                     ; DDD6:  'H' 48             ASLA  ; 
                    FCB   $45                     ; DDD7:  'E' 45             Invalid ; 
                    FCB   $20                     ; DDD8:  ' ' 20 45          BRA   $DE1F ; 
                    FCB   $45                     ; DDD9:  'E' 45             Invalid ; 
                    FCB   $4D                     ; DDDA:  'M' 4D             TSTA  ; 
                    FCB   $50                     ; DDDB:  'P' 50             NEGB  ; 
                    FCB   $49                     ; DDDC:  'I' 49             ROLA  ; 
                    FCB   $52                     ; DDDD:  'R' 52             Invalid ; 
                    FCB   $45                     ; DDDE:  'E' 45             Invalid ; 
                    FCB   $20                     ; DDDF:  ' ' 20 42          BRA   $DE23 ; 
                    FCB   $42                     ; DDE0:  'B' 42             Invalid ; 
                    FCB   $59                     ; DDE1:  'Y' 59             ROLB  ; 
                    FCB   $20                     ; DDE2:  ' ' 20 42          BRA   $DE26 ; 
                    FCB   $42                     ; DDE3:  'B' 42             Invalid ; 
                    FCB   $4C                     ; DDE4:  'L' 4C             INCA  ; 
                    FCB   $4F                     ; DDE5:  'O' 4F             CLRA  ; 
                    FCB   $57                     ; DDE6:  'W' 57             ASRB  ; 
                    FCB   $49                     ; DDE7:  'I' 49             ROLA  ; 
                    FCB   $4E                     ; DDE8:  'N' 4E             Invalid ; 
                    FCB   $C7                     ; DDE9:  ' ' C7             Invalid ; 
                    FCB   $55                     ; DDEA:  'U' 55             Invalid ; 
                    FCB   $50                     ; DDEB:  'P' 50             NEGB  ; 
                    FCB   $20                     ; DDEC:  ' ' 20 54          BRA   $DE42 ; 
                    FCB   $54                     ; DDED:  'T' 54             LSRB  ; 
                    FCB   $48                     ; DDEE:  'H' 48             ASLA  ; 
                    FCB   $45                     ; DDEF:  'E' 45             Invalid ; 
                    FCB   $20                     ; DDF0:  ' ' 20 44          BRA   $DE36 ; 
                    FCB   $44                     ; DDF1:  'D' 44             LSRA  ; 
                    FCB   $45                     ; DDF2:  'E' 45             Invalid ; 
                    FCB   $41                     ; DDF3:  'A' 41             Invalid ; 
                    FCB   $54                     ; DDF4:  'T' 54             LSRB  ; 
                    FCB   $48                     ; DDF5:  'H' 48             ASLA  ; 
                    FCB   $20                     ; DDF6:  ' ' 20 53          BRA   $DE4B ; 
                    FCB   $53                     ; DDF7:  'S' 53             COMB  ; 
                    FCB   $54                     ; DDF8:  'T' 54             LSRB  ; 
                    FCB   $41                     ; DDF9:  'A' 41             Invalid ; 
                    FCB   $52                     ; DDFA:  'R' 52             Invalid ; 
                    FCB   $AE                     ; DDFB:  ' ' AE B6          LDX   [A,Y] ; 
                    FCB   $B6                     ; DDFC:  ' ' B6 B7 B8       LDA   $B7B8 ; 
                    FCB   $B9                     ; DDFF:  ' ' B9 53 43       ADCA  $5343 ; 
                    FCB   $53                     ; DE00:  'S' 53             COMB  ; 
                    FCB   $43                     ; DE01:  'C' 43             COMA  ; 
                    FCB   $4F                     ; DE02:  'O' 4F             CLRA  ; 
                    FCB   $52                     ; DE03:  'R' 52             Invalid ; 
                    FCB   $49                     ; DE04:  'I' 49             ROLA  ; 
                    FCB   $4E                     ; DE05:  'N' 4E             Invalid ; 
                    FCB   $C7                     ; DE06:  ' ' C7             Invalid ; 
                    FCB   $54                     ; DE07:  'T' 54             LSRB  ; 
                    FCB   $49                     ; DE08:  'I' 49             ROLA  ; 
                    FCB   $45                     ; DE09:  'E' 45             Invalid ; 
                    FCB   $20                     ; DE0A:  ' ' 20 46          BRA   $DE52 ; 
                    FCB   $46                     ; DE0B:  'F' 46             RORA  ; 
                    FCB   $49                     ; DE0C:  'I' 49             ROLA  ; 
                    FCB   $47                     ; DE0D:  'G' 47             ASRA  ; 
                    FCB   $48                     ; DE0E:  'H' 48             ASLA  ; 
                    FCB   $54                     ; DE0F:  'T' 54             LSRB  ; 
                    FCB   $45                     ; DE10:  'E' 45             Invalid ; 
                    FCB   $52                     ; DE11:  'R' 52             Invalid ; 
                    FCB   $53                     ; DE12:  'S' 53             COMB  ; 
                    FCB   $20                     ; DE13:  ' ' 20 20          BRA   $DE35 ; 
                    FCB   $20                     ; DE14:  ' ' 20 20          BRA   $DE36 ; 
                    FCB   $20                     ; DE15:  ' ' 20 20          BRA   $DE37 ; 
                    FCB   $20                     ; DE16:  ' ' 20 20          BRA   $DE38 ; 
                    FCB   $20                     ; DE17:  ' ' 20 20          BRA   $DE39 ; 
                    FCB   $20                     ; DE18:  ' ' 20 20          BRA   $DE3A ; 
                    FCB   $20                     ; DE19:  ' ' 20 20          BRA   $DE3B ; 
                    FCB   $20                     ; DE1A:  ' ' 20 20          BRA   $DE3C ; 
                    FCB   $20                     ; DE1B:  ' ' 20 20          BRA   $DE3D ; 
                    FCB   $20                     ; DE1C:  ' ' 20 20          BRA   $DE3E ; 
                    FCB   $20                     ; DE1D:  ' ' 20 20          BRA   $DE3F ; 
                    FCB   $20                     ; DE1E:  ' ' 20 20          BRA   $DE40 ; 
                    FCB   $20                     ; DE1F:  ' ' 20 20          BRA   $DE41 ; 
                    FCB   $20                     ; DE20:  ' ' 20 20          BRA   $DE42 ; 
                    FCB   $20                     ; DE21:  ' ' 20 20          BRA   $DE43 ; 
                    FCB   $20                     ; DE22:  ' ' 20 20          BRA   $DE44 ; 
                    FCB   $20                     ; DE23:  ' ' 20 31          BRA   $DE56 ; 
                    FCB   $31                     ; DE24:  '1' 31 2C          LEAY  12,Y ; 
                    FCB   $2C                     ; DE25:  ',' 2C 30          BGE   $DE57 ; 
                    FCB   $30                     ; DE26:  '0' 30 30          LEAX  -16,Y ; 
                    FCB   $30                     ; DE27:  '0' 30 B0          LEAX  ??? ; 
                    FCB   $B0                     ; DE28:  ' ' B0 44 41       SUBA  $4441 ; 
                    FCB   $20                     ; DE36:  ' ' 20 53          BRA   $DE8B ; 
                    FCB   $53                     ; DE37:  'S' 53             COMB  ; 
                    FCB   $48                     ; DE38:  'H' 48             ASLA  ; 
                    FCB   $49                     ; DE39:  'I' 49             ROLA  ; 
                    FCB   $50                     ; DE3A:  'P' 50             NEGB  ; 
                    FCB   $20                     ; DE3B:  ' ' 20 20          BRA   $DE5D ; 
                    FCB   $20                     ; DE3C:  ' ' 20 20          BRA   $DE5E ; 
                    FCB   $20                     ; DE3D:  ' ' 20 20          BRA   $DE5F ; 
                    FCB   $20                     ; DE3E:  ' ' 20 20          BRA   $DE60 ; 
                    FCB   $20                     ; DE3F:  ' ' 20 20          BRA   $DE61 ; 
                    FCB   $20                     ; DE40:  ' ' 20 20          BRA   $DE62 ; 
                    FCB   $20                     ; DE41:  ' ' 20 20          BRA   $DE63 ; 
                    FCB   $20                     ; DE42:  ' ' 20 20          BRA   $DE64 ; 
                    FCB   $20                     ; DE43:  ' ' 20 20          BRA   $DE65 ; 
                    FCB   $20                     ; DE44:  ' ' 20 20          BRA   $DE66 ; 
                    FCB   $20                     ; DE45:  ' ' 20 32          BRA   $DE79 ; 
                    FCB   $32                     ; DE46:  '2' 32 2C          LEAS  12,Y ; 
                    FCB   $2C                     ; DE47:  ',' 2C 30          BGE   $DE79 ; 
                    FCB   $30                     ; DE48:  '0' 30 30          LEAX  -16,Y ; 
                    FCB   $30                     ; DE49:  '0' 30 B0          LEAX  ??? ; 
                    FCB   $B0                     ; DE4A:  ' ' B0 4C 41       SUBA  $4C41 ; 
                    FCB   $4C                     ; DE4B:  'L' 4C             INCA  ; 
                    FCB   $41                     ; DE4C:  'A' 41             Invalid ; 
                    FCB   $53                     ; DE4D:  'S' 53             COMB  ; 
                    FCB   $45                     ; DE4E:  'E' 45             Invalid ; 
                    FCB   $52                     ; DE4F:  'R' 52             Invalid ; 
                    FCB   $20                     ; DE50:  ' ' 20 42          BRA   $DE94 ; 
                    FCB   $42                     ; DE51:  'B' 42             Invalid ; 
                    FCB   $55                     ; DE52:  'U' 55             Invalid ; 
                    FCB   $4E                     ; DE53:  'N' 4E             Invalid ; 
                    FCB   $4B                     ; DE54:  'K' 4B             Invalid ; 
                    FCB   $45                     ; DE55:  'E' 45             Invalid ; 
                    FCB   $52                     ; DE56:  'R' 52             Invalid ; 
                    FCB   $53                     ; DE57:  'S' 53             COMB  ; 
                    FCB   $20                     ; DE58:  ' ' 20 20          BRA   $DE7A ; 
                    FCB   $20                     ; DE59:  ' ' 20 20          BRA   $DE7B ; 
                    FCB   $20                     ; DE5A:  ' ' 20 20          BRA   $DE7C ; 
                    FCB   $20                     ; DE5B:  ' ' 20 20          BRA   $DE7D ; 
                    FCB   $20                     ; DE5C:  ' ' 20 20          BRA   $DE7E ; 
                    FCB   $20                     ; DE5D:  ' ' 20 20          BRA   $DE7F ; 
                    FCB   $20                     ; DE5E:  ' ' 20 20          BRA   $DE80 ; 
                    FCB   $20                     ; DE5F:  ' ' 20 20          BRA   $DE81 ; 
                    FCB   $20                     ; DE60:  ' ' 20 20          BRA   $DE82 ; 
                    FCB   $20                     ; DE61:  ' ' 20 20          BRA   $DE83 ; 
                    FCB   $20                     ; DE62:  ' ' 20 20          BRA   $DE84 ; 
                    FCB   $20                     ; DE63:  ' ' 20 20          BRA   $DE85 ; 
                    FCB   $20                     ; DE64:  ' ' 20 20          BRA   $DE86 ; 
                    FCB   $20                     ; DE65:  ' ' 20 20          BRA   $DE87 ; 
                    FCB   $20                     ; DE66:  ' ' 20 20          BRA   $DE88 ; 
                    FCB   $20                     ; DE67:  ' ' 20 20          BRA   $DE89 ; 
                    FCB   $20                     ; DE68:  ' ' 20 20          BRA   $DE8A ; 
                    FCB   $20                     ; DE69:  ' ' 20 32          BRA   $DE9D ; 
                    FCB   $32                     ; DE6A:  '2' 32 30          LEAS  -16,Y ; 
                    FCB   $30                     ; DE6B:  '0' 30 B0          LEAX  ??? ; 
                    FCB   $B0                     ; DE6C:  ' ' B0 4C 41       SUBA  $4C41 ; 
                    FCB   $4C                     ; DE6D:  'L' 4C             INCA  ; 
                    FCB   $41                     ; DE6E:  'A' 41             Invalid ; 
                    FCB   $53                     ; DE6F:  'S' 53             COMB  ; 
                    FCB   $45                     ; DE70:  'E' 45             Invalid ; 
                    FCB   $52                     ; DE71:  'R' 52             Invalid ; 
                    FCB   $20                     ; DE72:  ' ' 20 54          BRA   $DEC8 ; 
                    FCB   $54                     ; DE73:  'T' 54             LSRB  ; 
                    FCB   $4F                     ; DE74:  'O' 4F             CLRA  ; 
                    FCB   $57                     ; DE75:  'W' 57             ASRB  ; 
                    FCB   $45                     ; DE76:  'E' 45             Invalid ; 
                    FCB   $52                     ; DE77:  'R' 52             Invalid ; 
                    FCB   $53                     ; DE78:  'S' 53             COMB  ; 
                    FCB   $20                     ; DE79:  ' ' 20 20          BRA   $DE9B ; 
                    FCB   $20                     ; DE7A:  ' ' 20 20          BRA   $DE9C ; 
                    FCB   $20                     ; DE7B:  ' ' 20 20          BRA   $DE9D ; 
                    FCB   $20                     ; DE7C:  ' ' 20 20          BRA   $DE9E ; 
                    FCB   $20                     ; DE7D:  ' ' 20 20          BRA   $DE9F ; 
                    FCB   $20                     ; DE7E:  ' ' 20 20          BRA   $DEA0 ; 
                    FCB   $20                     ; DE7F:  ' ' 20 20          BRA   $DEA1 ; 
                    FCB   $20                     ; DE80:  ' ' 20 20          BRA   $DEA2 ; 
                    FCB   $20                     ; DE81:  ' ' 20 20          BRA   $DEA3 ; 
                    FCB   $20                     ; DE82:  ' ' 20 20          BRA   $DEA4 ; 
                    FCB   $20                     ; DE83:  ' ' 20 20          BRA   $DEA5 ; 
                    FCB   $20                     ; DE84:  ' ' 20 20          BRA   $DEA6 ; 
                    FCB   $20                     ; DE85:  ' ' 20 20          BRA   $DEA7 ; 
                    FCB   $20                     ; DE86:  ' ' 20 20          BRA   $DEA8 ; 
                    FCB   $20                     ; DE87:  ' ' 20 20          BRA   $DEA9 ; 
                    FCB   $20                     ; DE88:  ' ' 20 20          BRA   $DEAA ; 
                    FCB   $20                     ; DE89:  ' ' 20 20          BRA   $DEAB ; 
                    FCB   $20                     ; DE8A:  ' ' 20 20          BRA   $DEAC ; 
                    FCB   $20                     ; DE8B:  ' ' 20 32          BRA   $DEBF ; 
                    FCB   $32                     ; DE8C:  '2' 32 30          LEAS  -16,Y ; 
                    FCB   $30                     ; DE8D:  '0' 30 B0          LEAX  ??? ; 
                    FCB   $B0                     ; DE8E:  ' ' B0 54 52       SUBA  $5452 ; 
                    FCB   $54                     ; DE8F:  'T' 54             LSRB  ; 
                    FCB   $52                     ; DE90:  'R' 52             Invalid ; 
                    FCB   $45                     ; DE91:  'E' 45             Invalid ; 
                    FCB   $4E                     ; DE92:  'N' 4E             Invalid ; 
                    FCB   $43                     ; DE93:  'C' 43             COMA  ; 
                    FCB   $48                     ; DE94:  'H' 48             ASLA  ; 
                    FCB   $20                     ; DE95:  ' ' 20 54          BRA   $DEEB ; 
                    FCB   $54                     ; DE96:  'T' 54             LSRB  ; 
                    FCB   $55                     ; DE97:  'U' 55             Invalid ; 
                    FCB   $52                     ; DE98:  'R' 52             Invalid ; 
                    FCB   $52                     ; DE99:  'R' 52             Invalid ; 
                    FCB   $45                     ; DE9A:  'E' 45             Invalid ; 
                    FCB   $54                     ; DE9B:  'T' 54             LSRB  ; 
                    FCB   $53                     ; DE9C:  'S' 53             COMB  ; 
                    FCB   $20                     ; DE9D:  ' ' 20 20          BRA   $DEBF ; 
                    FCB   $20                     ; DE9E:  ' ' 20 20          BRA   $DEC0 ; 
                    FCB   $20                     ; DE9F:  ' ' 20 20          BRA   $DEC1 ; 
                    FCB   $20                     ; DEA0:  ' ' 20 20          BRA   $DEC2 ; 
                    FCB   $20                     ; DEA1:  ' ' 20 20          BRA   $DEC3 ; 
                    FCB   $20                     ; DEA2:  ' ' 20 20          BRA   $DEC4 ; 
                    FCB   $20                     ; DEA3:  ' ' 20 20          BRA   $DEC5 ; 
                    FCB   $20                     ; DEA4:  ' ' 20 20          BRA   $DEC6 ; 
                    FCB   $20                     ; DEA5:  ' ' 20 20          BRA   $DEC7 ; 
                    FCB   $20                     ; DEA6:  ' ' 20 20          BRA   $DEC8 ; 
                    FCB   $20                     ; DEA7:  ' ' 20 20          BRA   $DEC9 ; 
                    FCB   $20                     ; DEA8:  ' ' 20 20          BRA   $DECA ; 
                    FCB   $20                     ; DEA9:  ' ' 20 20          BRA   $DECB ; 
                    FCB   $20                     ; DEAA:  ' ' 20 20          BRA   $DECC ; 
                    FCB   $20                     ; DEAB:  ' ' 20 20          BRA   $DECD ; 
                    FCB   $20                     ; DEAC:  ' ' 20 20          BRA   $DECE ; 
                    FCB   $20                     ; DEAD:  ' ' 20 31          BRA   $DEE0 ; 
                    FCB   $31                     ; DEAE:  '1' 31 30          LEAY  -16,Y ; 
                    FCB   $30                     ; DEAF:  '0' 30 B0          LEAX  ??? ; 
                    FCB   $B0                     ; DEB0:  ' ' B0 46 49       SUBA  $4649 ; 
                    FCB   $46                     ; DEB1:  'F' 46             RORA  ; 
                    FCB   $49                     ; DEB2:  'I' 49             ROLA  ; 
                    FCB   $52                     ; DEB3:  'R' 52             Invalid ; 
                    FCB   $45                     ; DEB4:  'E' 45             Invalid ; 
                    FCB   $42                     ; DEB5:  'B' 42             Invalid ; 
                    FCB   $41                     ; DEB6:  'A' 41             Invalid ; 
                    FCB   $4C                     ; DEB7:  'L' 4C             INCA  ; 
                    FCB   $4C                     ; DEB8:  'L' 4C             INCA  ; 
                    FCB   $53                     ; DEB9:  'S' 53             COMB  ; 
                    FCB   $20                     ; DEBA:  ' ' 20 20          BRA   $DEDC ; 
                    FCB   $20                     ; DEBB:  ' ' 20 20          BRA   $DEDD ; 
                    FCB   $20                     ; DEBC:  ' ' 20 20          BRA   $DEDE ; 
                    FCB   $20                     ; DEBD:  ' ' 20 20          BRA   $DEDF ; 
                    FCB   $20                     ; DEBE:  ' ' 20 20          BRA   $DEE0 ; 
                    FCB   $20                     ; DEBF:  ' ' 20 20          BRA   $DEE1 ; 
                    FCB   $20                     ; DEC0:  ' ' 20 20          BRA   $DEE2 ; 
                    FCB   $20                     ; DEC1:  ' ' 20 20          BRA   $DEE3 ; 
                    FCB   $20                     ; DEC2:  ' ' 20 20          BRA   $DEE4 ; 
                    FCB   $20                     ; DEC3:  ' ' 20 20          BRA   $DEE5 ; 
                    FCB   $20                     ; DEC4:  ' ' 20 20          BRA   $DEE6 ; 
                    FCB   $20                     ; DEC5:  ' ' 20 20          BRA   $DEE7 ; 
                    FCB   $20                     ; DEC6:  ' ' 20 20          BRA   $DEE8 ; 
                    FCB   $20                     ; DEC7:  ' ' 20 20          BRA   $DEE9 ; 
                    FCB   $20                     ; DEC8:  ' ' 20 20          BRA   $DEEA ; 
                    FCB   $20                     ; DEC9:  ' ' 20 20          BRA   $DEEB ; 
                    FCB   $20                     ; DECA:  ' ' 20 20          BRA   $DEEC ; 
                    FCB   $20                     ; DECB:  ' ' 20 20          BRA   $DEED ; 
                    FCB   $20                     ; DECC:  ' ' 20 20          BRA   $DEEE ; 
                    FCB   $20                     ; DECD:  ' ' 20 20          BRA   $DEEF ; 
                    FCB   $20                     ; DECE:  ' ' 20 20          BRA   $DEF0 ; 
                    FCB   $20                     ; DECF:  ' ' 20 20          BRA   $DEF1 ; 
                    FCB   $20                     ; DED0:  ' ' 20 33          BRA   $DF05 ; 
                    FCB   $33                     ; DED1:  '3' 33 B3          LEAU  [,--Y] ; 
                    FCB   $B3                     ; DED2:  ' ' B3 45 58       SUBD  $4558 ; 
                    FCB   $45                     ; DED3:  'E' 45             Invalid ; 
                    FCB   $58                     ; DED4:  'X' 58             ASLB  ; 
                    FCB   $48                     ; DED5:  'H' 48             ASLA  ; 
                    FCB   $41                     ; DED6:  'A' 41             Invalid ; 
                    FCB   $55                     ; DED7:  'U' 55             Invalid ; 
                    FCB   $53                     ; DED8:  'S' 53             COMB  ; 
                    FCB   $54                     ; DED9:  'T' 54             LSRB  ; 
                    FCB   $20                     ; DEDA:  ' ' 20 50          BRA   $DF2C ; 
                    FCB   $50                     ; DEDB:  'P' 50             NEGB  ; 
                    FCB   $4F                     ; DEDC:  'O' 4F             CLRA  ; 
                    FCB   $52                     ; DEDD:  'R' 52             Invalid ; 
                    FCB   $54                     ; DEDE:  'T' 54             LSRB  ; 
                    FCB   $20                     ; DEDF:  ' ' 20 20          BRA   $DF01 ; 
                    FCB   $20                     ; DEE0:  ' ' 20 20          BRA   $DF02 ; 
                    FCB   $20                     ; DEE1:  ' ' 20 20          BRA   $DF03 ; 
                    FCB   $20                     ; DEE2:  ' ' 20 20          BRA   $DF04 ; 
                    FCB   $20                     ; DEE3:  ' ' 20 20          BRA   $DF05 ; 
                    FCB   $20                     ; DEE4:  ' ' 20 20          BRA   $DF06 ; 
                    FCB   $20                     ; DEE5:  ' ' 20 20          BRA   $DF07 ; 
                    FCB   $20                     ; DEE6:  ' ' 20 20          BRA   $DF08 ; 
                    FCB   $20                     ; DEE7:  ' ' 20 20          BRA   $DF09 ; 
                    FCB   $20                     ; DEE8:  ' ' 20 20          BRA   $DF0A ; 
                    FCB   $20                     ; DEE9:  ' ' 20 20          BRA   $DF0B ; 
                    FCB   $20                     ; DEEA:  ' ' 20 20          BRA   $DF0C ; 
                    FCB   $20                     ; DEEB:  ' ' 20 20          BRA   $DF0D ; 
                    FCB   $20                     ; DEEC:  ' ' 20 20          BRA   $DF0E ; 
                    FCB   $20                     ; DEED:  ' ' 20 20          BRA   $DF0F ; 
                    FCB   $20                     ; DEEE:  ' ' 20 32          BRA   $DF22 ; 
                    FCB   $32                     ; DEEF:  '2' 32 35          LEAS  -11,Y ; 
                    FCB   $35                     ; DEF0:  '5' 35 2C          PULS  Y,DP,B ; 
                    FCB   $2C                     ; DEF1:  ',' 2C 30          BGE   $DF23 ; 
                    FCB   $30                     ; DEF2:  '0' 30 30          LEAX  -16,Y ; 
                    FCB   $30                     ; DEF3:  '0' 30 B0          LEAX  ??? ; 
                    FCB   $B0                     ; DEF4:  ' ' B0 44 45       SUBA  $4445 ; 
                    FCB   $44                     ; DEF5:  'D' 44             LSRA  ; 
                    FCB   $45                     ; DEF6:  'E' 45             Invalid ; 
                    FCB   $53                     ; DEF7:  'S' 53             COMB  ; 
                    FCB   $54                     ; DEF8:  'T' 54             LSRB  ; 
                    FCB   $52                     ; DEF9:  'R' 52             Invalid ; 
                    FCB   $4F                     ; DEFA:  'O' 4F             CLRA  ; 
                    FCB   $59                     ; DEFB:  'Y' 59             ROLB  ; 
                    FCB   $49                     ; DEFC:  'I' 49             ROLA  ; 
                    FCB   $4E                     ; DEFD:  'N' 4E             Invalid ; 
                    FCB   $47                     ; DEFE:  'G' 47             ASRA  ; 
                    FCB   $20                     ; DEFF:  ' ' 20 41          BRA   $DF42 ; 
                    FCB   $41                     ; DF00:  'A' 41             Invalid ; 
                    FCB   $4C                     ; DF01:  'L' 4C             INCA  ; 
                    FCB   $4C                     ; DF02:  'L' 4C             INCA  ; 
                    FCB   $20                     ; DF03:  ' ' 20 54          BRA   $DF59 ; 
                    FCB   $54                     ; DF04:  'T' 54             LSRB  ; 
                    FCB   $4F                     ; DF05:  'O' 4F             CLRA  ; 
                    FCB   $57                     ; DF06:  'W' 57             ASRB  ; 
                    FCB   $45                     ; DF07:  'E' 45             Invalid ; 
                    FCB   $52                     ; DF08:  'R' 52             Invalid ; 
                    FCB   $20                     ; DF09:  ' ' 20 54          BRA   $DF5F ; 
                    FCB   $54                     ; DF0A:  'T' 54             LSRB  ; 
                    FCB   $4F                     ; DF0B:  'O' 4F             CLRA  ; 
                    FCB   $50                     ; DF0C:  'P' 50             NEGB  ; 
                    FCB   $53                     ; DF0D:  'S' 53             COMB  ; 
                    FCB   $20                     ; DF0E:  ' ' 20 20          BRA   $DF30 ; 
                    FCB   $20                     ; DF0F:  ' ' 20 20          BRA   $DF31 ; 
                    FCB   $20                     ; DF10:  ' ' 20 35          BRA   $DF47 ; 
                    FCB   $35                     ; DF11:  '5' 35 30          PULS  Y,X ; 
                    FCB   $30                     ; DF12:  '0' 30 2C          LEAX  12,Y ; 
                    FCB   $2C                     ; DF13:  ',' 2C 30          BGE   $DF45 ; 
                    FCB   $30                     ; DF14:  '0' 30 30          LEAX  -16,Y ; 
                    FCB   $30                     ; DF15:  '0' 30 B0          LEAX  ??? ; 
                    FCB   $B0                     ; DF16:  ' ' B0 53 45       SUBA  $5345 ; 
                    FCB   $41                     ; DF62:  'A' 41             Invalid ; 
                    FCB   $56                     ; DF63:  'V' 56             RORB  ; 
                    FCB   $45                     ; DF64:  'E' 45             Invalid ; 
                    FCB   $20                     ; DF65:  ' ' 20 B1          BRA   $DF18 ; 
                    FCB   $B1                     ; DF66:  ' ' B1 57 41       CMPA  $5741 ; 
                    FCB   $57                     ; DF67:  'W' 57             ASRB  ; 
                    FCB   $41                     ; DF68:  'A' 41             Invalid ; 
                    FCB   $56                     ; DF69:  'V' 56             RORB  ; 
                    FCB   $45                     ; DF6A:  'E' 45             Invalid ; 
                    FCB   $20                     ; DF6B:  ' ' 20 B3          BRA   $DF20 ; 
                    FCB   $B3                     ; DF6C:  ' ' B3 57 41       SUBD  $5741 ; 
                    FCB   $57                     ; DF6D:  'W' 57             ASRB  ; 
                    FCB   $41                     ; DF6E:  'A' 41             Invalid ; 
                    FCB   $56                     ; DF6F:  'V' 56             RORB  ; 
                    FCB   $45                     ; DF70:  'E' 45             Invalid ; 
                    FCB   $20                     ; DF71:  ' ' 20 B5          BRA   $DF28 ; 
                    FCB   $B5                     ; DF72:  ' ' B5 42 4F       BITA  $424F ; 
                    FCB   $30                     ; DF81:  '0' 30 30          LEAX  -16,Y ; 
                    FCB   $30                     ; DF82:  '0' 30 2C          LEAX  12,Y ; 
                    FCB   $2C                     ; DF83:  ',' 2C 30          BGE   $DFB5 ; 
                    FCB   $30                     ; DF84:  '0' 30 30          LEAX  -16,Y ; 
                    FCB   $30                     ; DF85:  '0' 30 B0          LEAX  ??? ; 
                    FCB   $B0                     ; DF86:  ' ' B0 38 30       SUBA  $3830 ; 
                    FCB   $38                     ; DF87:  '8' 38             Invalid ; 
                    FCB   $30                     ; DF88:  '0' 30 30          LEAX  -16,Y ; 
                    FCB   $30                     ; DF89:  '0' 30 2C          LEAX  12,Y ; 
                    FCB   $2C                     ; DF8A:  ',' 2C 30          BGE   $DFBC ; 
                    FCB   $30                     ; DF8B:  '0' 30 30          LEAX  -16,Y ; 
                    FCB   $30                     ; DF8C:  '0' 30 B0          LEAX  ??? ; 
                    FCB   $B0                     ; DF8D:  ' ' B0 4D 45       SUBA  $4D45 ; 
                    FCB   $52                     ; E164:  'R' 52             Invalid ; 
                    FCB   $45                     ; E165:  'E' 45             Invalid ; 
                    FCB   $53                     ; E166:  'S' 53             COMB  ; 
                    FCB   $45                     ; E167:  'E' 45             Invalid ; 
                    FCB   $4E                     ; E168:  'N' 4E             Invalid ; 
                    FCB   $43                     ; E169:  'C' 43             COMA  ; 
                    FCB   $45                     ; E16A:  'E' 45             Invalid ; 
                    FCB   $20                     ; E16B:  ' ' 20 49          BRA   $E1B6 ; 
                    FCB   $49                     ; E16C:  'I' 49             ROLA  ; 
                    FCB   $53                     ; E16D:  'S' 53             COMB  ; 
                    FCB   $20                     ; E16E:  ' ' 20 46          BRA   $E1B6 ; 
                    FCB   $46                     ; E16F:  'F' 46             RORA  ; 
                    FCB   $45                     ; E170:  'E' 45             Invalid ; 
                    FCB   $4C                     ; E171:  'L' 4C             INCA  ; 
                    FCB   $54                     ; E172:  'T' 54             LSRB  ; 
                    FCB   $20                     ; E173:  ' ' 20 57          BRA   $E1CC ; 
                    FCB   $57                     ; E174:  'W' 57             ASRB  ; 
                    FCB   $49                     ; E175:  'I' 49             ROLA  ; 
                    FCB   $54                     ; E176:  'T' 54             LSRB  ; 
                    FCB   $48                     ; E177:  'H' 48             ASLA  ; 
                    FCB   $49                     ; E178:  'I' 49             ROLA  ; 
                    FCB   $4E                     ; E179:  'N' 4E             Invalid ; 
                    FCB   $20                     ; E17A:  ' ' 20 54          BRA   $E1D0 ; 
                    FCB   $54                     ; E17B:  'T' 54             LSRB  ; 
                    FCB   $48                     ; E17C:  'H' 48             ASLA  ; 
                    FCB   $45                     ; E17D:  'E' 45             Invalid ; 
                    FCB   $20                     ; E17E:  ' ' 20 46          BRA   $E1C6 ; 
                    FCB   $46                     ; E17F:  'F' 46             RORA  ; 
                    FCB   $4F                     ; E180:  'O' 4F             CLRA  ; 
                    FCB   $52                     ; E181:  'R' 52             Invalid ; 
                    FCB   $43                     ; E182:  'C' 43             COMA  ; 
                    FCB   $45                     ; E183:  'E' 45             Invalid ; 
                    FCB   $AE                     ; E184:  ' ' AE 54          LDX   -12,U ; 
                    FCB   $45                     ; E1E9:  'E' 45             Invalid ; 
                    FCB   $42                     ; E1EA:  'B' 42             Invalid ; 
                    FCB   $45                     ; E1EB:  'E' 45             Invalid ; 
                    FCB   $4C                     ; E1EC:  'L' 4C             INCA  ; 
                    FCB   $4C                     ; E1ED:  'L' 4C             INCA  ; 
                    FCB   $49                     ; E1EE:  'I' 49             ROLA  ; 
                    FCB   $4F                     ; E1EF:  'O' 4F             CLRA  ; 
                    FCB   $4E                     ; E1F0:  'N' 4E             Invalid ; 
                    FCB   $20                     ; E1F1:  ' ' 20 54          BRA   $E247 ; 
                    FCB   $54                     ; E1F2:  'T' 54             LSRB  ; 
                    FCB   $4F                     ; E1F3:  'O' 4F             CLRA  ; 
                    FCB   $20                     ; E1F4:  ' ' 20 53          BRA   $E249 ; 
                    FCB   $53                     ; E1F5:  'S' 53             COMB  ; 
                    FCB   $54                     ; E1F6:  'T' 54             LSRB  ; 
                    FCB   $4F                     ; E1F7:  'O' 4F             CLRA  ; 
                    FCB   $50                     ; E1F8:  'P' 50             NEGB  ; 
                    FCB   $20                     ; E1F9:  ' ' 20 54          BRA   $E24F ; 
                    FCB   $54                     ; E1FA:  'T' 54             LSRB  ; 
                    FCB   $48                     ; E1FB:  'H' 48             ASLA  ; 
                    FCB   $45                     ; E1FC:  'E' 45             Invalid ; 
                    FCB   $20                     ; E1FD:  ' ' 20 45          BRA   $E244 ; 
                    FCB   $45                     ; E1FE:  'E' 45             Invalid ; 
                    FCB   $4D                     ; E1FF:  'M' 4D             TSTA  ; 
                    FCB   $50                     ; E200:  'P' 50             NEGB  ; 
                    FCB   $49                     ; E201:  'I' 49             ROLA  ; 
                    FCB   $52                     ; E202:  'R' 52             Invalid ; 
                    FCB   $45                     ; E203:  'E' 45             Invalid ; 
                    FCB   $AE                     ; E204:  ' ' AE 54          LDX   -12,U ; 
                    FCB   $54                     ; E205:  'T' 54             LSRB  ; 
                    FCB   $48                     ; E206:  'H' 48             ASLA  ; 
                    FCB   $45                     ; E207:  'E' 45             Invalid ; 
                    FCB   $20                     ; E208:  ' ' 20 46          BRA   $E250 ; 
                    FCB   $46                     ; E209:  'F' 46             RORA  ; 
                    FCB   $4F                     ; E20A:  'O' 4F             CLRA  ; 
                    FCB   $52                     ; E20B:  'R' 52             Invalid ; 
                    FCB   $43                     ; E20C:  'C' 43             COMA  ; 
                    FCB   $45                     ; E20D:  'E' 45             Invalid ; 
                    FCB   $20                     ; E20E:  ' ' 20 57          BRA   $E267 ; 
                    FCB   $57                     ; E20F:  'W' 57             ASRB  ; 
                    FCB   $49                     ; E210:  'I' 49             ROLA  ; 
                    FCB   $4C                     ; E211:  'L' 4C             INCA  ; 
                    FCB   $4C                     ; E212:  'L' 4C             INCA  ; 
                    FCB   $20                     ; E213:  ' ' 20 42          BRA   $E257 ; 
                    FCB   $42                     ; E214:  'B' 42             Invalid ; 
                    FCB   $45                     ; E215:  'E' 45             Invalid ; 
                    FCB   $20                     ; E216:  ' ' 20 57          BRA   $E26F ; 
                    FCB   $57                     ; E217:  'W' 57             ASRB  ; 
                    FCB   $49                     ; E218:  'I' 49             ROLA  ; 
                    FCB   $54                     ; E219:  'T' 54             LSRB  ; 
                    FCB   $48                     ; E21A:  'H' 48             ASLA  ; 
                    FCB   $20                     ; E21B:  ' ' 20 59          BRA   $E276 ; 
                    FCB   $59                     ; E21C:  'Y' 59             ROLB  ; 
                    FCB   $4F                     ; E21D:  'O' 4F             CLRA  ; 
                    FCB   $55                     ; E21E:  'U' 55             Invalid ; 
                    FCB   $AE                     ; E21F:  ' ' AE 41          LDX   1,U ; 
                    FCB   $55                     ; E3DD:  'U' 55             Invalid ; 
                    FCB   $4C                     ; E3DE:  'L' 4C             INCA  ; 
                    FCB   $4C                     ; E3DF:  'L' 4C             INCA  ; 
                    FCB   $20                     ; E3E0:  ' ' 20 4C          BRA   $E42E ; 
                    FCB   $4C                     ; E3E1:  'L' 4C             INCA  ; 
                    FCB   $45                     ; E3E2:  'E' 45             Invalid ; 
                    FCB   $46                     ; E3E3:  'F' 46             RORA  ; 
                    FCB   $54                     ; E3E4:  'T' 54             LSRB  ; 
                    FCB   $20                     ; E3E5:  ' ' 20 46          BRA   $E42D ; 
                    FCB   $46                     ; E3E6:  'F' 46             RORA  ; 
                    FCB   $49                     ; E3E7:  'I' 49             ROLA  ; 
                    FCB   $52                     ; E3E8:  'R' 52             Invalid ; 
                    FCB   $45                     ; E3E9:  'E' 45             Invalid ; 
                    FCB   $20                     ; E3EA:  ' ' 20 54          BRA   $E440 ; 
                    FCB   $54                     ; E3EB:  'T' 54             LSRB  ; 
                    FCB   $4F                     ; E3EC:  'O' 4F             CLRA  ; 
                    FCB   $20                     ; E3ED:  ' ' 20 43          BRA   $E432 ; 
                    FCB   $43                     ; E3EE:  'C' 43             COMA  ; 
                    FCB   $48                     ; E3EF:  'H' 48             ASLA  ; 
                    FCB   $41                     ; E3F0:  'A' 41             Invalid ; 
                    FCB   $4E                     ; E3F1:  'N' 4E             Invalid ; 
                    FCB   $47                     ; E3F2:  'G' 47             ASRA  ; 
                    FCB   $45                     ; E3F3:  'E' 45             Invalid ; 
                    FCB   $20                     ; E3F4:  ' ' 20 53          BRA   $E449 ; 
                    FCB   $53                     ; E3F5:  'S' 53             COMB  ; 
                    FCB   $45                     ; E3F6:  'E' 45             Invalid ; 
                    FCB   $54                     ; E3F7:  'T' 54             LSRB  ; 
                    FCB   $54                     ; E3F8:  'T' 54             LSRB  ; 
                    FCB   $49                     ; E3F9:  'I' 49             ROLA  ; 
                    FCB   $4E                     ; E3FA:  'N' 4E             Invalid ; 
                    FCB   $C7                     ; E3FB:  ' ' C7             Invalid ; 
                    FCB   $B2                     ; E41F:  ' ' B2 B1 B4       SBCA  $B1B4 ; 
                    FCB   $B1                     ; E420:  ' ' B1 B4 B5       CMPA  $B4B5 ; 
                    FCB   $B4                     ; E421:  ' ' B4 B5 B6       ANDA  $B5B6 ; 
                    FCB   $B5                     ; E422:  ' ' B5 B6 4E       BITA  $B64E ; 
                    FCB   $B6                     ; E423:  ' ' B6 4E 4F       LDA   $4E4F ; 
                    FCB   $20                     ; E429:  ' ' 20 47          BRA   $E472 ; 
                    FCB   $47                     ; E42A:  'G' 47             ASRA  ; 
                    FCB   $49                     ; E42B:  'I' 49             ROLA  ; 
                    FCB   $56                     ; E42C:  'V' 56             RORB  ; 
                    FCB   $45                     ; E42D:  'E' 45             Invalid ; 
                    FCB   $53                     ; E42E:  'S' 53             COMB  ; 
                    FCB   $20                     ; E42F:  ' ' 20 B1          BRA   $E3E2 ; 
                    FCB   $B1                     ; E430:  ' ' B1 34 20       CMPA  $3420 ; 
                    FCB   $34                     ; E431:  '4' 34 20          PSHS  Y ; 
                    FCB   $20                     ; E432:  ' ' 20 47          BRA   $E47B ; 
                    FCB   $47                     ; E433:  'G' 47             ASRA  ; 
                    FCB   $49                     ; E434:  'I' 49             ROLA  ; 
                    FCB   $56                     ; E435:  'V' 56             RORB  ; 
                    FCB   $45                     ; E436:  'E' 45             Invalid ; 
                    FCB   $53                     ; E437:  'S' 53             COMB  ; 
                    FCB   $20                     ; E438:  ' ' 20 B1          BRA   $E3EB ; 
                    FCB   $B1                     ; E439:  ' ' B1 34 20       CMPA  $3420 ; 
                    FCB   $34                     ; E43A:  '4' 34 20          PSHS  Y ; 
                    FCB   $20                     ; E43B:  ' ' 20 47          BRA   $E484 ; 
                    FCB   $47                     ; E43C:  'G' 47             ASRA  ; 
                    FCB   $49                     ; E43D:  'I' 49             ROLA  ; 
                    FCB   $56                     ; E43E:  'V' 56             RORB  ; 
                    FCB   $45                     ; E43F:  'E' 45             Invalid ; 
                    FCB   $53                     ; E440:  'S' 53             COMB  ; 
                    FCB   $20                     ; E441:  ' ' 20 B2          BRA   $E3F5 ; 
                    FCB   $B2                     ; E442:  ' ' B2 35 20       SBCA  $3520 ; 
                    FCB   $35                     ; E443:  '5' 35 20          PULS  Y ; 
                    FCB   $20                     ; E444:  ' ' 20 47          BRA   $E48D ; 
                    FCB   $47                     ; E445:  'G' 47             ASRA  ; 
                    FCB   $49                     ; E446:  'I' 49             ROLA  ; 
                    FCB   $56                     ; E447:  'V' 56             RORB  ; 
                    FCB   $45                     ; E448:  'E' 45             Invalid ; 
                    FCB   $53                     ; E449:  'S' 53             COMB  ; 
                    FCB   $20                     ; E44A:  ' ' 20 B1          BRA   $E3FD ; 
                    FCB   $B1                     ; E44B:  ' ' B1 33 20       CMPA  $3320 ; 
                    FCB   $33                     ; E44C:  '3' 33 20          LEAU  0,Y ; 
                    FCB   $20                     ; E44D:  ' ' 20 47          BRA   $E496 ; 
                    FCB   $47                     ; E44E:  'G' 47             ASRA  ; 
                    FCB   $49                     ; E44F:  'I' 49             ROLA  ; 
                    FCB   $56                     ; E450:  'V' 56             RORB  ; 
                    FCB   $45                     ; E451:  'E' 45             Invalid ; 
                    FCB   $53                     ; E452:  'S' 53             COMB  ; 
                    FCB   $20                     ; E453:  ' ' 20 B1          BRA   $E406 ; 
                    FCB   $B1                     ; E454:  ' ' B1 4E 4F       CMPA  $4E4F ; 
                    FCB   $B9                     ; E45C:  ' ' B9 45 41       ADCA  $4541 ; 
                    FCB   $B1                     ; E475:  ' ' B1 B2 B3       CMPA  $B2B3 ; 
                    FCB   $B2                     ; E476:  ' ' B2 B3 59       SBCA  $B359 ; 
                    FCB   $B3                     ; E477:  ' ' B3 59 45       SUBD  $5945 ; 
                    FCB   $50                     ; E4C1:  'P' 50             NEGB  ; 
                    FCB   $41                     ; E4C2:  'A' 41             Invalid ; 
                    FCB   $52                     ; E4C3:  'R' 52             Invalid ; 
                    FCB   $45                     ; E4C4:  'E' 45             Invalid ; 
                    FCB   $20                     ; E4C5:  ' ' 20 B1          BRA   $E478 ; 
                    FCB   $B1                     ; E4C6:  ' ' B1 53 45       CMPA  $5345 ; 
                    FCB   $49                     ; E4E6:  'I' 49             ROLA  ; 
                    FCB   $47                     ; E4E7:  'G' 47             ASRA  ; 
                    FCB   $48                     ; E4E8:  'H' 48             ASLA  ; 
                    FCB   $54                     ; E4E9:  'T' 54             LSRB  ; 
                    FCB   $20                     ; E4EA:  ' ' 20 43          BRA   $E52F ; 
                    FCB   $43                     ; E4EB:  'C' 43             COMA  ; 
                    FCB   $4F                     ; E4EC:  'O' 4F             CLRA  ; 
                    FCB   $49                     ; E4ED:  'I' 49             ROLA  ; 
                    FCB   $CE                     ; E4EE:  ' ' CE A0 4C       LDU   #$A04C ; 
                    FCB   $A0                     ; E4EF:  ' ' A0 4C          SUBA  12,U ; 
                    FCB   $50                     ; E506:  'P' 50             NEGB  ; 
                    FCB   $41                     ; E507:  'A' 41             Invalid ; 
                    FCB   $52                     ; E508:  'R' 52             Invalid ; 
                    FCB   $45                     ; E509:  'E' 45             Invalid ; 
                    FCB   $20                     ; E50A:  ' ' 20 B2          BRA   $E4BE ; 
                    FCB   $B2                     ; E50B:  ' ' B2 A0 50       SBCA  $A050 ; 
                    FCB   $A0                     ; E50C:  ' ' A0 50          SUBA  -16,U ; 
                    FCB   $41                     ; E549:  'A' 41             Invalid ; 
                    FCB   $54                     ; E54A:  'T' 54             LSRB  ; 
                    FCB   $48                     ; E54B:  'H' 48             ASLA  ; 
                    FCB   $20                     ; E54C:  ' ' 20 52          BRA   $E5A0 ; 
                    FCB   $52                     ; E54D:  'R' 52             Invalid ; 
                    FCB   $41                     ; E54E:  'A' 41             Invalid ; 
                    FCB   $4D                     ; E54F:  'M' 4D             TSTA  ; 
                    FCB   $20                     ; E550:  ' ' 20 41          BRA   $E593 ; 
                    FCB   $41                     ; E551:  'A' 41             Invalid ; 
                    FCB   $54                     ; E552:  'T' 54             LSRB  ; 
                    FCB   $20                     ; E553:  ' ' 20 35          BRA   $E58A ; 
                    FCB   $35                     ; E554:  '5' 35 C6          PULS  PC,U,B,A ; 
                    FCB   $C6                     ; E555:  ' ' C6 4D          LDB   #$4D ; 
                    FCB   $47                     ; E565:  'G' 47             ASRA  ; 
                    FCB   $20                     ; E566:  ' ' 20 52          BRA   $E5BA ; 
                    FCB   $52                     ; E567:  'R' 52             Invalid ; 
                    FCB   $41                     ; E568:  'A' 41             Invalid ; 
                    FCB   $4D                     ; E569:  'M' 4D             TSTA  ; 
                    FCB   $30                     ; E56A:  '0' 30 20          LEAX  0,Y ; 
                    FCB   $20                     ; E56B:  ' ' 20 41          BRA   $E5AE ; 
                    FCB   $41                     ; E56C:  'A' 41             Invalid ; 
                    FCB   $54                     ; E56D:  'T' 54             LSRB  ; 
                    FCB   $20                     ; E56E:  ' ' 20 33          BRA   $E5A3 ; 
                    FCB   $33                     ; E56F:  '3' 33 CC 56       LEAU  $56,PC ; 
                    FCB   $CC                     ; E570:  ' ' CC 56 47       LDD   #$5647 ; 
                    FCB   $47                     ; E58C:  'G' 47             ASRA  ; 
                    FCB   $20                     ; E58D:  ' ' 20 52          BRA   $E5E1 ; 
                    FCB   $52                     ; E58E:  'R' 52             Invalid ; 
                    FCB   $41                     ; E58F:  'A' 41             Invalid ; 
                    FCB   $4D                     ; E590:  'M' 4D             TSTA  ; 
                    FCB   $33                     ; E591:  '3' 33 20          LEAU  0,Y ; 
                    FCB   $20                     ; E592:  ' ' 20 41          BRA   $E5D5 ; 
                    FCB   $41                     ; E593:  'A' 41             Invalid ; 
                    FCB   $54                     ; E594:  'T' 54             LSRB  ; 
                    FCB   $20                     ; E595:  ' ' 20 34          BRA   $E5CB ; 
                    FCB   $34                     ; E596:  '4' 34 CC          PSHS  PC,U,DP,B ; 
                    FCB   $CC                     ; E597:  ' ' CC 56 47       LDD   #$5647 ; 
                    FCB   $52                     ; E5C9:  'R' 52             Invalid ; 
                    FCB   $4F                     ; E5CA:  'O' 4F             CLRA  ; 
                    FCB   $47                     ; E5CB:  'G' 47             ASRA  ; 
                    FCB   $52                     ; E5CC:  'R' 52             Invalid ; 
                    FCB   $41                     ; E5CD:  'A' 41             Invalid ; 
                    FCB   $4D                     ; E5CE:  'M' 4D             TSTA  ; 
                    FCB   $20                     ; E5CF:  ' ' 20 52          BRA   $E623 ; 
                    FCB   $52                     ; E5D0:  'R' 52             Invalid ; 
                    FCB   $4F                     ; E5D1:  'O' 4F             CLRA  ; 
                    FCB   $4D                     ; E5D2:  'M' 4D             TSTA  ; 
                    FCB   $30                     ; E5D3:  '0' 30 20          LEAX  0,Y ; 
                    FCB   $20                     ; E5D4:  ' ' 20 41          BRA   $E617 ; 
                    FCB   $41                     ; E5D5:  'A' 41             Invalid ; 
                    FCB   $54                     ; E5D6:  'T' 54             LSRB  ; 
                    FCB   $20                     ; E5D7:  ' ' 20 31          BRA   $E60A ; 
                    FCB   $31                     ; E5D8:  '1' 31 C6          LEAY  A,U ; 
                    FCB   $C6                     ; E5D9:  ' ' C6 50          LDB   #$50 ; 
                    FCB   $52                     ; E601:  'R' 52             Invalid ; 
                    FCB   $4F                     ; E602:  'O' 4F             CLRA  ; 
                    FCB   $47                     ; E603:  'G' 47             ASRA  ; 
                    FCB   $52                     ; E604:  'R' 52             Invalid ; 
                    FCB   $41                     ; E605:  'A' 41             Invalid ; 
                    FCB   $4D                     ; E606:  'M' 4D             TSTA  ; 
                    FCB   $20                     ; E607:  ' ' 20 52          BRA   $E65B ; 
                    FCB   $52                     ; E608:  'R' 52             Invalid ; 
                    FCB   $4F                     ; E609:  'O' 4F             CLRA  ; 
                    FCB   $4D                     ; E60A:  'M' 4D             TSTA  ; 
                    FCB   $33                     ; E60B:  '3' 33 20          LEAU  0,Y ; 
                    FCB   $20                     ; E60C:  ' ' 20 41          BRA   $E64F ; 
                    FCB   $41                     ; E60D:  'A' 41             Invalid ; 
                    FCB   $54                     ; E60E:  'T' 54             LSRB  ; 
                    FCB   $20                     ; E60F:  ' ' 20 31          BRA   $E642 ; 
                    FCB   $31                     ; E610:  '1' 31 4B          LEAY  11,U ; 
                    FCB   $4B                     ; E611:  'K' 4B             Invalid ; 
                    FCB   $CC                     ; E612:  ' ' CC 50 52       LDD   #$5052 ; 
                    FCB   $47                     ; E626:  'G' 47             ASRA  ; 
                    FCB   $20                     ; E627:  ' ' 20 52          BRA   $E67B ; 
                    FCB   $52                     ; E628:  'R' 52             Invalid ; 
                    FCB   $4F                     ; E629:  'O' 4F             CLRA  ; 
                    FCB   $4D                     ; E62A:  'M' 4D             TSTA  ; 
                    FCB   $20                     ; E62B:  ' ' 20 41          BRA   $E66E ; 
                    FCB   $41                     ; E62C:  'A' 41             Invalid ; 
                    FCB   $54                     ; E62D:  'T' 54             LSRB  ; 
                    FCB   $20                     ; E62E:  ' ' 20 31          BRA   $E661 ; 
                    FCB   $31                     ; E62F:  '1' 31 CC 4D       LEAY  $4D,PC ; 
                    FCB   $CC                     ; E630:  ' ' CC 4D 41       LDD   #$4D41 ; 
                    FCB   $4F                     ; E6E9:  'O' 4F             CLRA  ; 
                    FCB   $46                     ; E6EA:  'F' 46             RORA  ; 
                    FCB   $C6                     ; E6EB:  ' ' C6 31          LDB   #$31 ; 
; =============== S U B R O U T I N E =======================================
LE764               LDA   ,X                      ; E764: A6 84         
                    JSR   Display_Vect_BCD        ; E766: BD E7 90        // Display BCD numbers
LE769               LDA   <$AD                    ; E769: 96 AD         
                    BGT   LE772                   ; E76B: 2E 05           // Display BCD number text
                    LDD   #$B913                  ; E76D: CC B9 13      
; End of function sub_E764
                    STD   ,Y++                    ; E770: ED A1         
; =============== S U B R O U T I N E =======================================
; Display BCD number text
LE772               LDA   $01,X                   ; E772: A6 01         
                    JSR   Display_Vect_BCD        ; E774: BD E7 90        // Display BCD numbers
LE777               LDA   $02,X                   ; E777: A6 02         
                    LSRA                          ; E779: 44            
                    LSRA                          ; E77A: 44            
                    LSRA                          ; E77B: 44            
                    LSRA                          ; E77C: 44            
                    JSR   LE7AD                   ; E77D: BD E7 AD      
LE780               LDA   <$AD                    ; E780: 96 AD         
                    BGT   LE789                   ; E782: 2E 05         
                    LDD   #$B913                  ; E784: CC B9 13      
                    STD   ,Y++                    ; E787: ED A1         
LE789               LDA   $02,X                   ; E789: A6 02         
                    JSR   LE7AD                   ; E78B: BD E7 AD      
; End of function sub_E772
LE78E               LDA   $03,X                   ; E78E: A6 03         
; =============== S U B R O U T I N E =======================================
; Display BCD numbers
Display_Vect_BCD        TFR   A,B                     ; E790: 1F 89         
                    LSRB                          ; E792: 54            
                    LSRB                          ; E793: 54            
                    LSRB                          ; E794: 54            
                    ANDB  #$1E                    ; E795: C4 1E         
                    BNE   LE7A2                   ; E797: 26 09         
                    TST   <$AD                    ; E799: 0D AD         
                    BLE   LE7A2                   ; E79B: 2F 05         
                    LDU   $3002                   ; E79D: FE 30 02      
                    BRA   LE7A9                   ; E7A0: 20 07         

; ---------------------------------------------------------------------------
LE7A2               CLR   <$AD                    ; E7A2: 0F AD         
                    LDU   #$3004                  ; E7A4: CE 30 04      
                    LDU   B,U                     ; E7A7: EE C5         
LE7A9               DEC   <$AD                    ; E7A9: 0A AD         
                    STU   ,Y++                    ; E7AB: EF A1         
LE7AD               ASLA                          ; E7AD: 48            
                    ANDA  #$1E                    ; E7AE: 84 1E         
                    BNE   LE7BB                   ; E7B0: 26 09         
                    TST   <$AD                    ; E7B2: 0D AD         
                    BLE   LE7BB                   ; E7B4: 2F 05         
                    LDU   $3002                   ; E7B6: FE 30 02      
                    BRA   LE7C2                   ; E7B9: 20 07         

; ---------------------------------------------------------------------------
LE7BB               CLR   <$AD                    ; E7BB: 0F AD         
                    LDU   #$3004                  ; E7BD: CE 30 04      
                    LDU   A,U                     ; E7C0: EE C6         
LE7C2               DEC   <$AD                    ; E7C2: 0A AD         
                    STU   ,Y++                    ; E7C4: EF A1         
; End of function Display_Vect_BCD
                    RTS                           ; E7C6: 39            

; =============== S U B R O U T I N E =======================================
; Print text string from pointer table
LE7C7               CMPB  #$D6                    ; E7C7: C1 D6         
                    BCC   locret_E7D2             ; E7C9: 24 07         
                    STB   $48AE                   ; E7CB: F7 48 AE        // Text string index
                    BSR   LE7DD                   ; E7CE: 8D 0D           // Insert text colour vector instruction
LE7D0               BSR   LE7EA                   ; E7D0: 8D 18           // Insert text position vector instruction
; End of function sub_E7C7
locret_E7D2         RTS                           ; E7D2: 39            

; =============== S U B R O U T I N E =======================================
LE7D3               CMPB  #$D6                    ; E7D3: C1 D6         
                    BCC   locret_E7DC             ; E7D5: 24 05         
                    STB   $48AE                   ; E7D7: F7 48 AE        // Text string index
                    BSR   LE7EA                   ; E7DA: 8D 0E           // Insert text position vector instruction
; End of function sub_E7D3
locret_E7DC         RTS                           ; E7DC: 39            

; =============== S U B R O U T I N E =======================================
; Insert text colour vector instruction
LE7DD               LDB   $48AE                   ; E7DD: F6 48 AE        // Text string index
                    LDX   #word_EDA8              ; E7E0: 8E ED A8        // Text string colour
                    ABX                           ; E7E3: 3A            
                    ABX                           ; E7E4: 3A            
                    LDD   ,X                      ; E7E5: EC 84         
                    STD   ,Y++                    ; E7E7: ED A1         
; End of function sub_E7DD
                    RTS                           ; E7E9: 39            

; =============== S U B R O U T I N E =======================================
; Insert text position vector instruction
LE7EA               LDB   $48AE                   ; E7EA: F6 48 AE        // Text string index
                    LDX   #word_EA50              ; E7ED: 8E EA 50        // Text string position
                    ABX                           ; E7F0: 3A            
                    ABX                           ; E7F1: 3A            
                    ABX                           ; E7F2: 3A            
                    ABX                           ; E7F3: 3A            
                    LDD   $02,X                   ; E7F4: EC 02         
                    STD   ,Y++                    ; E7F6: ED A1         
                    LDD   ,X                      ; E7F8: EC 84         
                    BRA   LE811                   ; E7FA: 20 15         

; ---------------------------------------------------------------------------
LE7FC               LDB   $48AE                   ; E7FC: F6 48 AE        // Text string index
                    LDX   #word_EA50              ; E7FF: 8E EA 50        // Text string position
                    ABX                           ; E802: 3A            
                    ABX                           ; E803: 3A            
                    ABX                           ; E804: 3A            
                    ABX                           ; E805: 3A            
                    LDD   $02,X                   ; E806: EC 02         
                    SUBD  $48AF                   ; E808: B3 48 AF      
                    ANDA  #$1F                    ; E80B: 84 1F         
                    STD   ,Y++                    ; E80D: ED A1         
                    LDD   ,X                      ; E80F: EC 84         
LE811               STD   ,Y++                    ; E811: ED A1         
                    JSR   LE821                   ; E813: BD E8 21        // Text handling
LE816               LDD   #$7200                  ; E816: CC 72 00        // Vector SCAL 2,0 instruction
                    STD   ,Y++                    ; E819: ED A1         
                    LDD   #$8040                  ; E81B: CC 80 40        // Vector CNTR instruction
                    STD   ,Y++                    ; E81E: ED A1         
; End of function sub_E7EA
                    RTS                           ; E820: 39            

; =============== S U B R O U T I N E =======================================
; Text handling
LE821               LDX   #ptrText                ; E821: 8E E8 94      
                    LDB   $48AE                   ; E824: F6 48 AE        // Text string index
                    ABX                           ; E827: 3A            
                    ABX                           ; E828: 3A            
                    LDX   ,X                      ; E829: AE 84         
LE82B               LDB   ,X+                     ; E82B: E6 80         
                    STX   $48B1                   ; E82D: BF 48 B1      
                    ASLB                          ; E830: 58            
                    CMPB  #$82                    ; E831: C1 82         
                    BCS   LE83C                   ; E833: 25 07         
                    LDX   #$3016                  ; E835: 8E 30 16      
                    ANDB  #$7F                    ; E838: C4 7F         
                    BRA   LE888                   ; E83A: 20 4C         

; ---------------------------------------------------------------------------
LE83C               CMPB  #$74                    ; E83C: C1 74         
                    BNE   LE845                   ; E83E: 26 05         
                    LDX   #$2FDE                  ; E840: 8E 2F DE      
                    BRA   LE888                   ; E843: 20 43         

; ---------------------------------------------------------------------------
LE845               CMPB  #$80                    ; E845: C1 80         
                    BNE   LE84F                   ; E847: 26 06         
                    LDX   #$3058                  ; E849: 8E 30 58      
                    CLRB                          ; E84C: 5F            
                    BRA   LE888                   ; E84D: 20 39         

; ---------------------------------------------------------------------------
LE84F               CMPB  #$60                    ; E84F: C1 60         
                    BCS   LE858                   ; E851: 25 05         
                    LDX   #$2FA4                  ; E853: 8E 2F A4      
                    BRA   LE888                   ; E856: 20 30         

; ---------------------------------------------------------------------------
LE858               CMPB  #$40                    ; E858: C1 40         
                    BNE   LE861                   ; E85A: 26 05         
                    LDX   #$2FC2                  ; E85C: 8E 2F C2      
                    BRA   LE888                   ; E85F: 20 27         

; ---------------------------------------------------------------------------
LE861               CMPB  #$4E                    ; E861: C1 4E         
                    BNE   LE86A                   ; E863: 26 05         
                    LDX   #$2FFE                  ; E865: 8E 2F FE      
                    BRA   LE888                   ; E868: 20 1E         

; ---------------------------------------------------------------------------
LE86A               CMPB  #$58                    ; E86A: C1 58         
                    BNE   LE873                   ; E86C: 26 05         
                    LDX   #$2FF6                  ; E86E: 8E 2F F6      
                    BRA   LE888                   ; E871: 20 15         

; ---------------------------------------------------------------------------
LE873               CMPB  #$5A                    ; E873: C1 5A         
                    BNE   LE87C                   ; E875: 26 05         
                    LDX   #$3000                  ; E877: 8E 30 00      
                    BRA   LE888                   ; E87A: 20 0C         

; ---------------------------------------------------------------------------
LE87C               CMPB  #$4A                    ; E87C: C1 4A         
                    BNE   LE885                   ; E87E: 26 05         
                    LDX   #$300C                  ; E880: 8E 30 0C      
                    BRA   LE888                   ; E883: 20 03         

; ---------------------------------------------------------------------------
LE885               LDX   #$2FF4                  ; E885: 8E 2F F4      
LE888               LDD   B,X                     ; E888: EC 85         
                    STD   ,Y++                    ; E88A: ED A1         
                    LDX   $48B1                   ; E88C: BE 48 B1      
                    TST   -1,X                    ; E88F: 6D 1F         
                    BPL   LE82B                   ; E891: 2A 98         
; End of function sub_E821
; ---------------------------------------------------------------------------
                    RTS                           ; E893: 39            

; =============== S U B R O U T I N E =======================================
; Attributes: noreturn
; End of function BADIRQ
BADIRQ              BRA   BADIRQ                  ; EF54: 20 FE           // public BADIRQ

; =============== S U B R O U T I N E =======================================
IRQ                 LDX   $0A,S                   ; EF56: AE 6A           // public IRQ
                    CMPX  #$6000                  ; EF58: 8C 60 00        // Bounds check for return address in ROM
                    BCC   LEF5E                   ; EF5B: 24 01         
                    SWI                           ; EF5D: 3F            
LEF5E               CMPX  #BADIRQ                 ; EF5E: 8C EF 54        // Only allow IRQ calls from lower in ROM than BADIRQ function
                    BCS   LEF64                   ; EF61: 25 01         
                    SWI                           ; EF63: 3F            
LEF64               LDA   #$48                    ; EF64: 86 48         
                    TFR   A,DP                    ; EF66: 1F 8B         
                    STA   $4640                   ; EF68: B7 46 40      
                    LDA   <$00                    ; EF6B: 96 00           // Check sanity byte is $3F
                    CMPA  #$3F                    ; EF6D: 81 3F         
                    BEQ   LEF72                   ; EF6F: 27 01         
                    SWI                           ; EF71: 3F            
LEF72               DEC   <$3E                    ; EF72: 0A 3E         
                    BPL   LEF7F                   ; EF74: 2A 09         
                    LDA   #$0B                    ; EF76: 86 0B         
                    STA   <$3E                    ; EF78: 97 3E         
                    INC   <$3D                    ; EF7A: 0C 3D         
                    BVC   LEF7F                   ; EF7C: 28 01         
                    SWI                           ; EF7E: 3F            
LEF7F               INC   <$0A                    ; EF7F: 0C 0A         
                    JSR   LD776                   ; EF81: BD D7 76        // Check coin inputs
LEF84               LDA   <$14                    ; EF84: 96 14           // Credits
                    CMPA  #$24                    ; EF86: 81 24         
                    BCS   LEF8B                   ; EF88: 25 01         
                    SWI                           ; EF8A: 3F            
LEF8B               LDX   #$481C                  ; EF8B: 8E 48 1C      
                    LDA   $4300                   ; EF8E: B6 43 00      
                    JSR   LF133                   ; EF91: BD F1 33        // Inputs debounce
LEF94               LDA   $4320                   ; EF94: B6 43 20      
                    JSR   LF133                   ; EF97: BD F1 33        // Inputs debounce
LEF9A               LDA   $4340                   ; EF9A: B6 43 40      
                    JSR   LF133                   ; EF9D: BD F1 33        // Inputs debounce
LEFA0               LDA   $4360                   ; EFA0: B6 43 60      
                    JSR   LF133                   ; EFA3: BD F1 33        // Inputs debounce
LEFA6               LDA   <$33                    ; EFA6: 96 33         
                    STA   <$34                    ; EFA8: 97 34         
                    LDA   $4821                   ; EFAA: B6 48 21      
                    ANDA  #$30                    ; EFAD: 84 30         
                    STA   -1,S                    ; EFAF: A7 7F         
                    LDA   $481E                   ; EFB1: B6 48 1E      
                    ANDA  #$CF                    ; EFB4: 84 CF         
                    ORA   -1,S                    ; EFB6: AA 7F         
                    STA   <$33                    ; EFB8: 97 33         
                    EORA  <$34                    ; EFBA: 98 34         
                    ANDA  <$34                    ; EFBC: 94 34         
                    STA   <$32                    ; EFBE: 97 32         
                    LDA   <$32                    ; EFC0: 96 32         
                    ANDA  #$F0                    ; EFC2: 84 F0         
                    BEQ   LEFCA                   ; EFC4: 27 04         
                    ORA   <$31                    ; EFC6: 9A 31         
                    STA   <$31                    ; EFC8: 97 31         
LEFCA               JSR   LF146                   ; EFCA: BD F1 46        // Joystick processing
LEFCD               LDA   <$0A                    ; EFCD: 96 0A         
                    BNE   LEFE6                   ; EFCF: 26 15         
                    LDA   $4401                   ; EFD1: B6 44 01      
                    ASLA                          ; EFD4: 48            
                    BPL   LEFDE                   ; EFD5: 2A 07         
                    LDA   $4400                   ; EFD7: B6 44 00      
                    CMPA  #$5A                    ; EFDA: 81 5A         
                    BEQ   LEFE6                   ; EFDC: 27 08         
LEFDE               STA   $46E0                   ; EFDE: B7 46 E0      
                    LDA   #$00                    ; EFE1: 86 00         
                    STA   $4400                   ; EFE3: B7 44 00      
LEFE6               LDA   <$0A                    ; EFE6: 96 0A         
                    ANDA  #$03                    ; EFE8: 84 03         
                    BNE   LF034                   ; EFEA: 26 48         
                    LDA   $481B                   ; EFEC: B6 48 1B      
                    ADDA  #$01                    ; EFEF: 8B 01         
                    CMPA  #$FA                    ; EFF1: 81 FA         
                    BCS   LEFF7                   ; EFF3: 25 02         
                    LDA   #$00                    ; EFF5: 86 00         
LEFF7               STA   $481B                   ; EFF7: B7 48 1B      
                    BNE   LF034                   ; EFFA: 26 38         
                    LDA   $481A                   ; EFFC: B6 48 1A      
                    ADDA  #$01                    ; EFFF: 8B 01         
                    DAA                           ; F001: 19            
                    STA   $481A                   ; F002: B7 48 1A      
                    LDA   $4819                   ; F005: B6 48 19      
                    ADCA  #$00                    ; F008: 89 00         
                    DAA                           ; F00A: 19            
                    BCS   LF010                   ; F00B: 25 03         
                    STA   $4819                   ; F00D: B7 48 19      
LF010               LDA   $4B07                   ; F010: B6 4B 07      
                    ADDA  #$01                    ; F013: 8B 01         
                    DAA                           ; F015: 19            
                    STA   $4B07                   ; F016: B7 4B 07      
                    LDA   $4B06                   ; F019: B6 4B 06      
                    ADCA  #$00                    ; F01C: 89 00         
                    DAA                           ; F01E: 19            
                    STA   $4B06                   ; F01F: B7 4B 06      
                    LDA   $4B05                   ; F022: B6 4B 05      
                    ADCA  #$00                    ; F025: 89 00         
                    DAA                           ; F027: 19            
                    STA   $4B05                   ; F028: B7 4B 05      
                    LDA   $4B04                   ; F02B: B6 4B 04      
                    ADCA  #$00                    ; F02E: 89 00         
                    DAA                           ; F030: 19            
                    STA   $4B04                   ; F031: B7 4B 04      
LF034               DEC   <$40                    ; F034: 0A 40         
                    BVC   LF041                   ; F036: 28 09         
                    STA   $4620                   ; F038: B7 46 20      
                    JSR   $611E                   ; F03B: BD 61 1E        // Copies Star Wars logo vector data to vector RAM
LF03E               JMP   LF12F                   ; F03E: 7E F1 2F      

; ---------------------------------------------------------------------------
LF041               LBPL  LF12F                   ; F041: 10 2A 00 EA   
                    LDA   $4320                   ; F045: B6 43 20      
                    ASLA                          ; F048: 48            
                    LBPL  LF12F                   ; F049: 10 2A 00 E2     // If VGHALT is high, VG is ready
                    LDA   <$3F                    ; F04D: 96 3F           // Vector pointer state
                    BPL   LF06A                   ; F04F: 2A 19         
                    LDA   $00                     ; F051: B6 00 00      
                    ANDA  #$0A                    ; F054: 84 0A         
                    BEQ   LF05A                   ; F056: 27 02         
                    LDA   #$14                    ; F058: 86 14         
LF05A               ORA   #$00                    ; F05A: 8A 00           // What the heck is going on here???
                    STA   <$3F                    ; F05C: 97 3F           // Vector pointer state
                    LDB   #$38                    ; F05E: C6 38         
                    LSRA                          ; F060: 44            
                    ANDA  #$0A                    ; F061: 84 0A         
                    EORA  #$0A                    ; F063: 88 0A         
                    ORA   #$E0                    ; F065: 8A E0         
                    STD   $00                     ; F067: FD 00 00        // Seems to toggle vector address $0000 from $E038 and $EA38 JMPL $038 and $A38
LF06A               DEC   <$28                    ; F06A: 0A 28         
                    BPL   LF073                   ; F06C: 2A 05         
                    CLR   <$28                    ; F06E: 0F 28         
                    JSR   LF18D                   ; F070: BD F1 8D      
LF073               JSR   LF22B                   ; F073: BD F2 2B      
LF076               LDB   <$3A                    ; F076: D6 3A         
                    ADDB  #$02                    ; F078: CB 02         
                    CMPB  #$20                    ; F07A: C1 20         
                    BCS   LF07F                   ; F07C: 25 01         
                    CLRB                          ; F07E: 5F            
LF07F               STB   <$3A                    ; F07F: D7 3A           // Colour cycle blue/cyan for lasers
                    LDX   #$04                    ; F081: 8E 00 04      
                    ABX                           ; F084: 3A              // Points to jump table that jump into vector ROM for laser colour cycle
                    LDU   #word_D620              ; F085: CE D6 20        // Copies vector JMPL instructions into vector RAM
LF088               LDD   ,U++                    ; F088: EC C1         
                    STD   ,--X                    ; F08A: ED 83         
                    CMPX  #$02                    ; F08C: 8C 00 02      
                    BGT   LF088                   ; F08F: 2E F7         
                    LDX   #$22                    ; F091: 8E 00 22      
LF094               CMPU  #word_D640              ; F094: 11 83 D6 40   
                    BCC   LF0A0                   ; F098: 24 06         
                    LDD   ,U++                    ; F09A: EC C1         
                    STD   ,--X                    ; F09C: ED 83         
                    BRA   LF094                   ; F09E: 20 F4         

; ---------------------------------------------------------------------------
LF0A0               LDB   <$3B                    ; F0A0: D6 3B         
                    ADDB  #$02                    ; F0A2: CB 02         
                    CMPB  #$08                    ; F0A4: C1 08         
                    BCS   LF0A9                   ; F0A6: 25 01         
                    CLRB                          ; F0A8: 5F            
LF0A9               STB   <$3B                    ; F0A9: D7 3B         
                    LDU   #word_D640              ; F0AB: CE D6 40      
                    LDD   B,U                     ; F0AE: EC C5         
                    STD   $22                     ; F0B0: FD 00 22      
                    DEC   <$36                    ; F0B3: 0A 36         
                    BGT   LF0D8                   ; F0B5: 2E 21         
                    LDB   #$04                    ; F0B7: C6 04         
                    STB   <$36                    ; F0B9: D7 36         
                    LDB   <$37                    ; F0BB: D6 37         
                    ADDB  #$02                    ; F0BD: CB 02         
                    CMPB  #$08                    ; F0BF: C1 08         
                    BCS   LF0C4                   ; F0C1: 25 01         
                    CLRB                          ; F0C3: 5F            
LF0C4               STB   <$37                    ; F0C4: D7 37         
                    LDU   #word_D648              ; F0C6: CE D6 48      
                    LDD   B,U                     ; F0C9: EC C5         
                    STD   $2A                     ; F0CB: FD 00 2A      
                    LDB   <$37                    ; F0CE: D6 37         
                    LDU   #word_D650              ; F0D0: CE D6 50      
                    LDD   B,U                     ; F0D3: EC C5         
                    STD   $2E                     ; F0D5: FD 00 2E      
LF0D8               DEC   <$38                    ; F0D8: 0A 38         
                    BGT   LF0F3                   ; F0DA: 2E 17         
                    LDB   #$01                    ; F0DC: C6 01         
                    STB   <$38                    ; F0DE: D7 38         
                    LDB   <$39                    ; F0E0: D6 39         
                    ADDB  #$02                    ; F0E2: CB 02         
                    CMPB  #$08                    ; F0E4: C1 08         
                    BCS   LF0E9                   ; F0E6: 25 01         
                    CLRB                          ; F0E8: 5F            
LF0E9               STB   <$39                    ; F0E9: D7 39         
                    LDU   #word_D658              ; F0EB: CE D6 58      
                    LDD   B,U                     ; F0EE: EC C5         
                    STD   $2C                     ; F0F0: FD 00 2C      
LF0F3               LDB   <$35                    ; F0F3: D6 35         
                    ADDB  #$02                    ; F0F5: CB 02         
                    CMPB  #$0E                    ; F0F7: C1 0E         
                    BCS   LF0FC                   ; F0F9: 25 01         
                    CLRB                          ; F0FB: 5F            
LF0FC               STB   <$35                    ; F0FC: D7 35         
                    LDU   #word_D604              ; F0FE: CE D6 04      
                    LDD   B,U                     ; F101: EC C5         
                    STD   $30                     ; F103: FD 00 30      
                    LDD   #$C000                  ; F106: CC C0 00        // Vector RTSL
                    STD   $32                     ; F109: FD 00 32      
                    LDB   <$3C                    ; F10C: D6 3C           // Cycle through 7 colours
                    ADDB  #$02                    ; F10E: CB 02         
                    CMPB  #$0E                    ; F110: C1 0E         
                    BCS   LF115                   ; F112: 25 01         
                    CLRB                          ; F114: 5F            
LF115               STB   <$3C                    ; F115: D7 3C           // Vector colour cycle count
                    LDU   #word_D612              ; F117: CE D6 12      
                    LDD   B,U                     ; F11A: EC C5         
                    STD   $34                     ; F11C: FD 00 34      
                    LDD   #$C000                  ; F11F: CC C0 00        // Vector RTSL
                    STD   $36                     ; F122: FD 00 36      
                    JSR   LD660                   ; F125: BD D6 60        // Update laser target hit vector animations
LF128               STA   $4600                   ; F128: B7 46 00        // Start vector generator run
                    LDA   #$05                    ; F12B: 86 05         
                    STA   <$40                    ; F12D: 97 40         
LF12F               STA   $4660                   ; F12F: B7 46 60      
; End of function IRQ
                    RTI                           ; F132: 3B            

; =============== S U B R O U T I N E =======================================
; Inputs debounce
LF133               LDB   ,X                      ; F133: E6 84           // Inputs debounce
                    STB   $01,X                   ; F135: E7 01         
                    STA   ,X                      ; F137: A7 84         
                    ANDA  $01,X                   ; F139: A4 01         
                    ORA   $02,X                   ; F13B: AA 02         
                    STA   $02,X                   ; F13D: A7 02         
                    ORB   ,X++                    ; F13F: EA 81         
                    ANDB  ,X                      ; F141: E4 84         
                    STB   ,X+                     ; F143: E7 80         
; End of function sub_F133
                    RTS                           ; F145: 39            

; =============== S U B R O U T I N E =======================================
; Joystick processing
LF146               LDA   <$0A                    ; F146: 96 0A         
                    LSRA                          ; F148: 44            
                    LDY   #$4829                  ; F149: 10 8E 48 29   
                    BCS   LF153                   ; F14D: 25 04         
                    LDY   #$482B                  ; F14F: 10 8E 48 2B   
LF153               LDB   $01,Y                   ; F153: E6 21         
                    LDA   $4380                   ; F155: B6 43 80      
                    STA   $01,Y                   ; F158: A7 21         
                    CMPA  ,Y                      ; F15A: A1 A4         
                    BCS   LF16C                   ; F15C: 25 0E         
                    CMPB  ,Y                      ; F15E: E1 A4         
                    BLS   LF16A                   ; F160: 23 08         
                    CMPB  $01,Y                   ; F162: E1 21         
                    BLS   LF168                   ; F164: 23 02         
                    LDB   $01,Y                   ; F166: E6 21         
LF168               STB   ,Y                      ; F168: E7 A4         
LF16A               BRA   LF178                   ; F16A: 20 0C         

; ---------------------------------------------------------------------------
LF16C               CMPB  ,Y                      ; F16C: E1 A4         
                    BCC   LF178                   ; F16E: 24 08         
                    CMPB  $01,Y                   ; F170: E1 21         
                    BCC   LF176                   ; F172: 24 02         
                    LDB   $01,Y                   ; F174: E6 21         
LF176               STB   ,Y                      ; F176: E7 A4         
LF178               CMPY  #$4829                  ; F178: 10 8C 48 29   
                    BNE   LF186                   ; F17C: 26 08         
                    STA   $46C0                   ; F17E: B7 46 C0      
                    STA   $46C0                   ; F181: B7 46 C0      
                    BRA   locret_F18C             ; F184: 20 06         

; ---------------------------------------------------------------------------
LF186               STA   $46C1                   ; F186: B7 46 C1      
                    STA   $46C1                   ; F189: B7 46 C1      
; End of function sub_F146
locret_F18C         RTS                           ; F18C: 39            

; =============== S U B R O U T I N E =======================================
LF18D               LDX   #$4866                  ; F18D: 8E 48 66        // Joystick pitch data structure
                    LDA   <$2B                    ; F190: 96 2B           // Joystick Y
                    NOP                           ; F192: 12            
                    JSR   LF1C6                   ; F193: BD F1 C6      
LF196               LDA   $04,X                   ; F196: A6 04         
                    CMPA  #$78                    ; F198: 81 78         
                    BLE   LF19E                   ; F19A: 2F 02         
                    LDA   #$78                    ; F19C: 86 78         
LF19E               CMPA  #$98                    ; F19E: 81 98         
                    BGE   LF1A4                   ; F1A0: 2C 02         
                    LDA   #$98                    ; F1A2: 86 98         
LF1A4               STA   $04,X                   ; F1A4: A7 04         
                    JSR   LF1FD                   ; F1A6: BD F1 FD      
LF1A9               LDX   #$486F                  ; F1A9: 8E 48 6F        // Joystick roll data structure
                    LDA   <$29                    ; F1AC: 96 29           // Joystick X
                    NOP                           ; F1AE: 12            
                    JSR   LF1C6                   ; F1AF: BD F1 C6      
LF1B2               LDA   $04,X                   ; F1B2: A6 04         
                    CMPA  #$70                    ; F1B4: 81 70         
                    BLE   LF1BA                   ; F1B6: 2F 02         
                    LDA   #$70                    ; F1B8: 86 70         
LF1BA               CMPA  #$90                    ; F1BA: 81 90         
                    BGE   LF1C0                   ; F1BC: 2C 02         
                    LDA   #$90                    ; F1BE: 86 90         
LF1C0               STA   $04,X                   ; F1C0: A7 04         
                    JSR   LF1FD                   ; F1C2: BD F1 FD      
; End of function sub_F18D
LF1C5               RTS                           ; F1C5: 39            

; =============== S U B R O U T I N E =======================================
LF1C6               CMPA  ,X                      ; F1C6: A1 84         
                    BCC   LF1D0                   ; F1C8: 24 06         
                    CMPA  $01,X                   ; F1CA: A1 01         
                    BCS   LF1D0                   ; F1CC: 25 02         
                    DEC   ,X                      ; F1CE: 6A 84         
LF1D0               STA   $01,X                   ; F1D0: A7 01         
                    CMPA  ,X                      ; F1D2: A1 84         
                    BCC   LF1D8                   ; F1D4: 24 02         
                    LDA   ,X                      ; F1D6: A6 84         
LF1D8               SUBA  ,X                      ; F1D8: A0 84         
                    STA   <$51                    ; F1DA: 97 51         
                    LDB   $02,X                   ; F1DC: E6 02         
                    MUL                           ; F1DE: 3D            
                    ADDA  <$51                    ; F1DF: 9B 51         
                    BNE   LF1E5                   ; F1E1: 26 02         
                    LDA   #$01                    ; F1E3: 86 01         
LF1E5               BCS   LF1EB                   ; F1E5: 25 04         
                    CLR   $03,X                   ; F1E7: 6F 03         
                    BRA   LF1F7                   ; F1E9: 20 0C         

; ---------------------------------------------------------------------------
LF1EB               LDA   #$FF                    ; F1EB: 86 FF         
                    INC   $03,X                   ; F1ED: 6C 03         
                    LDB   $03,X                   ; F1EF: E6 03         
                    CMPB  #$02                    ; F1F1: C1 02         
                    BCS   LF1F7                   ; F1F3: 25 02         
                    DEC   $02,X                   ; F1F5: 6A 02         
LF1F7               CLRB                          ; F1F7: 5F            
                    SUBA  #$80                    ; F1F8: 80 80         
                    STA   $04,X                   ; F1FA: A7 04         
; End of function sub_F1C6
                    RTS                           ; F1FC: 39            

; =============== S U B R O U T I N E =======================================
LF1FD               CLR   <$51                    ; F1FD: 0F 51         
                    LDA   $04,X                   ; F1FF: A6 04         
                    LDB   #$80                    ; F201: C6 80         
                    SUBD  $05,X                   ; F203: A3 05         
                    BGE   LF20A                   ; F205: 2C 03         
                    NEGA                          ; F207: 40            
                    DEC   <$51                    ; F208: 0A 51         
LF20A               BLE   LF20F                   ; F20A: 2F 03         
                    ADDD  #$FF                    ; F20C: C3 00 FF      
LF20F               CMPA  #$F8                    ; F20F: 81 F8         
                    BLS   LF215                   ; F211: 23 02         
                    LDA   #$F8                    ; F213: 86 F8         
LF215               LDB   #$60                    ; F215: C6 60         
                    CMPA  #$40                    ; F217: 81 40         
                    BCC   LF21D                   ; F219: 24 02         
                    LDB   #$30                    ; F21B: C6 30         
LF21D               MUL                           ; F21D: 3D            
                    TST   <$51                    ; F21E: 0D 51         
                    BPL   LF226                   ; F220: 2A 04         
                    COMA                          ; F222: 43            
                    NEGB                          ; F223: 50            
                    SBCA  #$FF                    ; F224: 82 FF         
LF226               ADDD  $05,X                   ; F226: E3 05         
                    STD   $05,X                   ; F228: ED 05         
; End of function sub_F1FD
                    RTS                           ; F22A: 39            

; =============== S U B R O U T I N E =======================================
LF22B               LDB   <$6B                    ; F22B: D6 6B         
                    LDA   <$6C                    ; F22D: 96 6C         
                    ANDA  #$C0                    ; F22F: 84 C0         
                    ASLA                          ; F231: 48            
                    ROLB                          ; F232: 59            
                    BCC   LF237                   ; F233: 24 02         
                    ORA   #$3F                    ; F235: 8A 3F         
LF237               ROLA                          ; F237: 49            
                    ROLB                          ; F238: 59            
                    ROLA                          ; F239: 49            
                    STD   <$2F                    ; F23A: DD 2F         
                    ADDD  #$FF98                  ; F23C: C3 FF 98      
                    ANDA  #$1F                    ; F23F: 84 1F         
                    STD   $24                     ; F241: FD 00 24      
                    LDB   <$74                    ; F244: D6 74         
                    LDA   <$75                    ; F246: 96 75         
                    ANDA  #$C0                    ; F248: 84 C0         
                    ASLA                          ; F24A: 48            
                    ROLB                          ; F24B: 59            
                    BCC   LF250                   ; F24C: 24 02         
                    ORA   #$3F                    ; F24E: 8A 3F         
LF250               ROLA                          ; F250: 49            
                    ROLB                          ; F251: 59            
                    ROLA                          ; F252: 49            
                    STD   <$2D                    ; F253: DD 2D         
                    ANDA  #$1F                    ; F255: 84 1F         
                    STD   $26                     ; F257: FD 00 26      
                    LDD   #$C000                  ; F25A: CC C0 00      
                    STD   $28                     ; F25D: FD 00 28      
; End of function sub_F22B
                    RTS                           ; F260: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_6532
LF261               ORCC  #$10                    ; F261: 1A 10           // public Reset
                    CLR   $4684                   ; F263: 7F 46 84      
                    LDA   #$00                    ; F266: 86 00         
                    STA   $4686                   ; F268: B7 46 86      
                    STA   $4683                   ; F26B: B7 46 83      
                    STA   $4682                   ; F26E: B7 46 82      
                    LDA   #$00                    ; F271: 86 00         
                    STA   $4685                   ; F273: B7 46 85      
                    LDA   #$80                    ; F276: 86 80         
                    STA   $4685                   ; F278: B7 46 85      
                    STA   $4620                   ; F27B: B7 46 20      
                    LDU   #$00                    ; F27E: CE 00 00      
LF281               STA   $4640                   ; F281: B7 46 40      
                    LEAU  -1,U                    ; F284: 33 5F         
                    CMPU  #$00                    ; F286: 11 83 00 00   
                    BNE   LF281                   ; F28A: 26 F5         
                    LDS   #$4FFF                  ; F28C: 10 CE 4F FF     // Stack top at $4FFF
                    LDA   #$48                    ; F290: 86 48           // H      ; Direct Page at $4800
                    TFR   A,DP                    ; F292: 1F 8B         
                    STA   $46E0                   ; F294: B7 46 E0      
                    LDA   #$00                    ; F297: 86 00         
                    STA   $4400                   ; F299: B7 44 00      
                    LDX   #$4800                  ; F29C: 8E 48 00      
                    LDD   #$00                    ; F29F: CC 00 00      
LF2A2               STD   ,X++                    ; F2A2: ED 81         
                    CMPX  #$5000                  ; F2A4: 8C 50 00      
                    BCS   LF2A2                   ; F2A7: 25 F9         
                    STA   $4640                   ; F2A9: B7 46 40      
                    LDX   #$5000                  ; F2AC: 8E 50 00      
                    LDD   #$00                    ; F2AF: CC 00 00      
LF2B2               STD   ,X++                    ; F2B2: ED 81         
                    STA   $4640                   ; F2B4: B7 46 40      
                    CMPX  #$6000                  ; F2B7: 8C 60 00      
                    BCS   LF2B2                   ; F2BA: 25 F6         
                    LDX   #$00                    ; F2BC: 8E 00 00      
                    LDD   #$00                    ; F2BF: CC 00 00      
LF2C2               STD   ,X++                    ; F2C2: ED 81         
                    STA   $4640                   ; F2C4: B7 46 40      
                    CMPX  #$3000                  ; F2C7: 8C 30 00      
                    BCS   LF2C2                   ; F2CA: 25 F6         
                    LDA   $4300                   ; F2CC: B6 43 00        // Check self test switch
                    ANDA  #$10                    ; F2CF: 84 10         
                    BNE   LF2D6                   ; F2D1: 26 03         
                    JMP   LF36E                   ; F2D3: 7E F3 6E      

; ---------------------------------------------------------------------------
LF2D6               LDA   $4340                   ; F2D6: B6 43 40        // Read option switches
                    LDB   $4360                   ; F2D9: F6 43 60      
LF2DC               STA   <$24                    ; F2DC: 97 24         
                    STB   <$27                    ; F2DE: D7 27           // Store in shadow RAM
                    LDX   #$0800                  ; F2E0: 8E 08 00      
LF2E3               LEAX  -1,X                    ; F2E3: 30 1F         
                    BNE   LF2E3                   ; F2E5: 26 FC         
                    LDA   $4340                   ; F2E7: B6 43 40      
                    LDB   $4360                   ; F2EA: F6 43 60      
                    CMPA  <$24                    ; F2ED: 91 24         
                    BNE   LF2DC                   ; F2EF: 26 EB         
                    CMPB  <$27                    ; F2F1: D1 27         
                    BNE   LF2DC                   ; F2F3: 26 E7         
                    LDD   #$2020                  ; F2F5: CC 20 20      
                    LDX   #$00                    ; F2F8: 8E 00 00      
LF2FB               STD   ,X++                    ; F2FB: ED 81           // Clear vector RAM to HALT instructions
                    STA   $4640                   ; F2FD: B7 46 40      
                    CMPX  #$2800                  ; F300: 8C 28 00      
                    BCS   LF2FB                   ; F303: 25 F6         
                    LDD   #$E038                  ; F305: CC E0 38        // Set up first vector instruction to JSRL 38
                    STD   $00                     ; F308: FD 00 00      
                    LDA   #$FF                    ; F30B: 86 FF         
                    STA   <$3F                    ; F30D: 97 3F           // Vector pointer state
                    LDA   #$3F                    ; F30F: 86 3F         
                    STA   <$00                    ; F311: 97 00         
                    LDA   #$40                    ; F313: 86 40         
                    STA   <$28                    ; F315: 97 28         
                    JSR   LC306                   ; F317: BD C3 06        // Read NOVRAM
LF31A               STA   $4640                   ; F31A: B7 46 40      
                    LDA   #$FF                    ; F31D: 86 FF         
                    STA   $4686                   ; F31F: B7 46 86      
                    STA   $4683                   ; F322: B7 46 83      
                    STA   $4682                   ; F325: B7 46 82      
                    STA   $4660                   ; F328: B7 46 60      
; END OF FUNCTION CHUNK FOR sub_6532
; ---------------------------------------------------------------------------
                    JMP   $6036                   ; F32B: 7E 60 36        // Jump to main game loop

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_6532
LF36E               LDA   $4320                   ; F36E: B6 43 20        // Self test
                    ANDA  #$04                    ; F371: 84 04         
                    BNE   LF380                   ; F373: 26 0B           // Check Aux coin
                    LDA   $4340                   ; F375: B6 43 40      
                    COMA                          ; F378: 43            
                    ANDA  #$7F                    ; F379: 84 7F         
                    BEQ   LF380                   ; F37B: 27 03         
                    JMP   Check_Test_Diag         ; F37D: 7E FD 07      

; ---------------------------------------------------------------------------
LF380               LDS   #$00                    ; F380: 10 CE 00 00   
                    LDU   #word_F32E              ; F384: CE F3 2E      
LF387               LDX   ,U++                    ; F387: AE C1         
                    LDY   #$0800                  ; F389: 10 8E 08 00   
LF38D               LDA   ,X                      ; F38D: A6 84         
                    BEQ   LF39D                   ; F38F: 27 0C         
                    TFR   S,D                     ; F391: 1F 40         
                    ORA   $1E,U                   ; F393: AA C8 1E      
                    ORB   $1F,U                   ; F396: EA C8 1F      
                    TFR   D,S                     ; F399: 1F 04         
                    BRA   LF3DC                   ; F39B: 20 3F         

; ---------------------------------------------------------------------------
LF39D               LDA   #$80                    ; F39D: 86 80         
                    ASLA                          ; F39F: 48            
LF3A0               ROLA                          ; F3A0: 49            
                    STA   ,X                      ; F3A1: A7 84         
                    TFR   A,B                     ; F3A3: 1F 89         
                    EORB  ,X                      ; F3A5: E8 84         
                    BEQ   LF3B5                   ; F3A7: 27 0C         
                    TFR   S,D                     ; F3A9: 1F 40         
                    ORA   $1E,U                   ; F3AB: AA C8 1E      
                    ORB   $1F,U                   ; F3AE: EA C8 1F      
                    TFR   D,S                     ; F3B1: 1F 04         
                    BRA   LF3DC                   ; F3B3: 20 27         

; ---------------------------------------------------------------------------
LF3B5               BCC   LF3BE                   ; F3B5: 24 07         
                    TSTA                          ; F3B7: 4D            
                    BNE   LF3A0                   ; F3B8: 26 E6         
                    ADDA  #$FF                    ; F3BA: 8B FF         
                    BRA   LF3A0                   ; F3BC: 20 E2         

; ---------------------------------------------------------------------------
LF3BE               TFR   A,B                     ; F3BE: 1F 89         
                    INCB                          ; F3C0: 5C            
                    BNE   LF3A0                   ; F3C1: 26 DD         
                    STA   $4640                   ; F3C3: B7 46 40      
                    CMPU  #word_F332              ; F3C6: 11 83 F3 32   
                    BEQ   LF3D2                   ; F3CA: 27 06         
                    CMPU  #word_F334              ; F3CC: 11 83 F3 34   
                    BNE   LF3D6                   ; F3D0: 26 04         
LF3D2               LEAX  $02,X                   ; F3D2: 30 02         
                    BRA   LF3D8                   ; F3D4: 20 02         

; ---------------------------------------------------------------------------
LF3D6               LEAX  $01,X                   ; F3D6: 30 01         
LF3D8               LEAY  -1,Y                    ; F3D8: 31 3F         
                    BNE   LF38D                   ; F3DA: 26 B1         
LF3DC               CMPU  #off_F340               ; F3DC: 11 83 F3 40   
                    BCS   LF387                   ; F3E0: 25 A5         
                    LDU   #word_F32E              ; F3E2: CE F3 2E      
LF3E5               LDX   ,U++                    ; F3E5: AE C1         
                    LDY   #$0800                  ; F3E7: 10 8E 08 00   
LF3EB               INC   ,X                      ; F3EB: 6C 84         
                    BEQ   LF3FB                   ; F3ED: 27 0C         
                    TFR   S,D                     ; F3EF: 1F 40         
                    ORA   $1E,U                   ; F3F1: AA C8 1E      
                    ORB   $1F,U                   ; F3F4: EA C8 1F      
                    TFR   D,S                     ; F3F7: 1F 04         
                    BRA   LF424                   ; F3F9: 20 29         

; ---------------------------------------------------------------------------
LF3FB               LDA   ,X                      ; F3FB: A6 84         
                    BEQ   LF40B                   ; F3FD: 27 0C         
                    TFR   S,D                     ; F3FF: 1F 40         
                    ORA   $1E,U                   ; F401: AA C8 1E      
                    ORB   $1F,U                   ; F404: EA C8 1F      
                    TFR   D,S                     ; F407: 1F 04         
                    BRA   LF424                   ; F409: 20 19         

; ---------------------------------------------------------------------------
LF40B               STA   $4640                   ; F40B: B7 46 40      
                    CMPU  #word_F332              ; F40E: 11 83 F3 32   
                    BEQ   LF41A                   ; F412: 27 06         
                    CMPU  #word_F334              ; F414: 11 83 F3 34   
                    BNE   LF41E                   ; F418: 26 04         
LF41A               LEAX  $02,X                   ; F41A: 30 02         
                    BRA   LF420                   ; F41C: 20 02         

; ---------------------------------------------------------------------------
LF41E               LEAX  $01,X                   ; F41E: 30 01         
LF420               LEAY  -1,Y                    ; F420: 31 3F         
                    BNE   LF3EB                   ; F422: 26 C7         
LF424               CMPU  #off_F340               ; F424: 11 83 F3 40   
                    BCS   LF3E5                   ; F428: 25 BB         
                    TFR   S,D                     ; F42A: 1F 40         
                    ANDB  #$01                    ; F42C: C4 01         
                    BNE   LF43F                   ; F42E: 26 0F         
                    LDX   #$4500                  ; F430: 8E 45 00        // NOVRAM
                    LDU   #$4C00                  ; F433: CE 4C 00      
LF436               LDD   ,X++                    ; F436: EC 81         
                    STD   ,U++                    ; F438: ED C1         
                    CMPX  #$4600                  ; F43A: 8C 46 00        // NOVRAM
                    BCS   LF436                   ; F43D: 25 F7         
LF43F               LDU   #word_F342              ; F43F: CE F3 42      
                    LDX   #$4500                  ; F442: 8E 45 00        // NOVRAM
                    LDD   #$00                    ; F445: CC 00 00      
LF448               STD   ,X++                    ; F448: ED 81         
                    CMPX  #$4600                  ; F44A: 8C 46 00        // NOVRAM
                    BCS   LF448                   ; F44D: 25 F9         
                    LDX   #$4500                  ; F44F: 8E 45 00        // NOVRAM
LF452               LDA   ,X                      ; F452: A6 84         
                    ANDA  #$0F                    ; F454: 84 0F         
                    BEQ   LF464                   ; F456: 27 0C         
                    TFR   S,D                     ; F458: 1F 40         
                    ORA   $1E,U                   ; F45A: AA C8 1E      
                    ORB   $1F,U                   ; F45D: EA C8 1F      
                    TFR   D,S                     ; F460: 1F 04         
                    BRA   LF4B7                   ; F462: 20 53         

; ---------------------------------------------------------------------------
LF464               LDA   #$80                    ; F464: 86 80         
                    ASLA                          ; F466: 48            
                    LDY   #$04                    ; F467: 10 8E 00 04   
LF46B               ROLA                          ; F46B: 49            
                    STA   ,X                      ; F46C: A7 84         
                    TFR   A,B                     ; F46E: 1F 89         
                    EORB  ,X                      ; F470: E8 84         
                    ANDB  #$0F                    ; F472: C4 0F         
                    BEQ   LF482                   ; F474: 27 0C         
                    TFR   S,D                     ; F476: 1F 40         
                    ORA   $1E,U                   ; F478: AA C8 1E      
                    ORB   $1F,U                   ; F47B: EA C8 1F      
                    TFR   D,S                     ; F47E: 1F 04         
                    BRA   LF4B7                   ; F480: 20 35         

; ---------------------------------------------------------------------------
LF482               LEAY  -1,Y                    ; F482: 31 3F         
                    BNE   LF46B                   ; F484: 26 E5         
                    TSTA                          ; F486: 4D            
                    BMI   LF491                   ; F487: 2B 08         
                    LDA   #$FF                    ; F489: 86 FF         
                    LDY   #$05                    ; F48B: 10 8E 00 05   
                    BRA   LF46B                   ; F48F: 20 DA         

; ---------------------------------------------------------------------------
LF491               STA   $4640                   ; F491: B7 46 40      
                    LEAX  $01,X                   ; F494: 30 01         
                    CMPX  #$4600                  ; F496: 8C 46 00        // NOVRAM
                    BCS   LF452                   ; F499: 25 B7         
                    LDX   #$4500                  ; F49B: 8E 45 00        // NOVRAM
LF49E               INC   ,X                      ; F49E: 6C 84         
                    LDA   ,X+                     ; F4A0: A6 80         
                    ANDA  #$0F                    ; F4A2: 84 0F         
                    BEQ   LF4B2                   ; F4A4: 27 0C         
                    TFR   S,D                     ; F4A6: 1F 40         
                    ORA   $1E,U                   ; F4A8: AA C8 1E      
                    ORB   $1F,U                   ; F4AB: EA C8 1F      
                    TFR   D,S                     ; F4AE: 1F 04         
                    BRA   LF4B7                   ; F4B0: 20 05         

; ---------------------------------------------------------------------------
LF4B2               CMPX  #$4600                  ; F4B2: 8C 46 00        // NOVRAM
                    BCS   LF49E                   ; F4B5: 25 E7         
LF4B7               TFR   S,D                     ; F4B7: 1F 40         
                    ANDB  #$01                    ; F4B9: C4 01         
                    BNE   LF4CE                   ; F4BB: 26 11         
                    LDX   #$4C00                  ; F4BD: 8E 4C 00      
                    LDU   #$4500                  ; F4C0: CE 45 00        // NOVRAM
LF4C3               LDD   ,X++                    ; F4C3: EC 81         
                    STD   ,U++                    ; F4C5: ED C1         
                    CMPX  #$4D00                  ; F4C7: 8C 4D 00      
                    BCS   LF4C3                   ; F4CA: 25 F7         
                    BRA   LF4EC                   ; F4CC: 20 1E         

; ---------------------------------------------------------------------------
LF4CE               LDA   #$FF                    ; F4CE: 86 FF         
                    STA   $4687                   ; F4D0: B7 46 87      
                    LDX   #$0100                  ; F4D3: 8E 01 00      
LF4D6               STA   $4640                   ; F4D6: B7 46 40      
                    LEAX  -1,X                    ; F4D9: 30 1F         
                    BNE   LF4D6                   ; F4DB: 26 F9         
                    LDA   #$00                    ; F4DD: 86 00         
                    STA   $4687                   ; F4DF: B7 46 87      
                    LDX   #$A000                  ; F4E2: 8E A0 00      
LF4E5               STA   $4640                   ; F4E5: B7 46 40      
                    LEAX  -1,X                    ; F4E8: 30 1F         
                    BNE   LF4E5                   ; F4EA: 26 F9         
LF4EC               STA   $4640                   ; F4EC: B7 46 40      
                    LDA   #$00                    ; F4EF: 86 00         
                    STA   $4684                   ; F4F1: B7 46 84      
                    LDX   word_F342               ; F4F4: BE F3 42      
                    LDY   #$2000                  ; F4F7: 10 8E 20 00   
                    TFR   X,D                     ; F4FB: 1F 10         
LF4FD               ADCB  $01,X                   ; F4FD: E9 01         
                    ADCA  ,X++                    ; F4FF: A9 81         
                    STA   $4640                   ; F501: B7 46 40      
                    LEAY  -2,Y                    ; F504: 31 3E         
                    BNE   LF4FD                   ; F506: 26 F5         
                    TFR   D,X                     ; F508: 1F 01         
                    LDA   #$FF                    ; F50A: 86 FF         
                    STA   $4684                   ; F50C: B7 46 84      
                    LDU   #word_F344              ; F50F: CE F3 44        // Point to ROM start address table
                    TFR   X,D                     ; F512: 1F 10         
                    LDY   #$2000                  ; F514: 10 8E 20 00     // Size of each ROM
                    LDX   word_F342               ; F518: BE F3 42      
; END OF FUNCTION CHUNK FOR sub_6532
                    BRA   LF528                   ; F51B: 20 0B         

; =============== S U B R O U T I N E =======================================
; End of function sub_F51D
LF51D               LDU   #word_F344              ; F51D: CE F3 44        // Point to ROM start address table
; START OF FUNCTION CHUNK FOR sub_6532
LF520               LDX   ,U++                    ; F520: AE C1           // Next ROM address start
                    LDY   #$2000                  ; F522: 10 8E 20 00     // Size of ROM
LF526               TFR   X,D                     ; F526: 1F 10         
LF528               ADCB  $01,X                   ; F528: E9 01         
                    ADCA  ,X++                    ; F52A: A9 81         
                    STA   $4640                   ; F52C: B7 46 40      
                    LEAY  -2,Y                    ; F52F: 31 3E         
                    BNE   LF528                   ; F531: 26 F5         
                    STD   $5593,U                 ; F533: ED C9 55 93   
                    BEQ   LF543                   ; F537: 27 0A         
                    TFR   S,D                     ; F539: 1F 40         
                    ORA   $1E,U                   ; F53B: AA C8 1E      
                    ORB   $1F,U                   ; F53E: EA C8 1F      
                    TFR   D,S                     ; F541: 1F 04         
LF543               CMPU  #word_F34C              ; F543: 11 83 F3 4C   
                    BCS   LF520                   ; F547: 25 D7         
                    CMPU  #word_F34E              ; F549: 11 83 F3 4E   
                    BCC   LF557                   ; F54D: 24 08         
                    LDX   ,U++                    ; F54F: AE C1         
                    LDY   #$1000                  ; F551: 10 8E 10 00   
                    BRA   LF526                   ; F555: 20 CF         

; ---------------------------------------------------------------------------
LF557               LDA   #$FF                    ; F557: 86 FF         
                    STA   $4686                   ; F559: B7 46 86      
                    STA   $4683                   ; F55C: B7 46 83      
                    STA   $4682                   ; F55F: B7 46 82      
                    CLR   <$D1                    ; F562: 0F D1         
                    LDA   $4320                   ; F564: B6 43 20      
                    ANDA  #$04                    ; F567: 84 04         
                    BNE   LF56E                   ; F569: 26 03         
                    JMP   Check_Test_Diag         ; F56B: 7E FD 07      

; ---------------------------------------------------------------------------
LF56E               STA   $4640                   ; F56E: B7 46 40      
                    STS   <$D2                    ; F571: 10 DF D2      
                    TFR   S,D                     ; F574: 1F 40         
                    ANDB  #$01                    ; F576: C4 01         
                    BNE   LF5ED                   ; F578: 26 73         
                    LDY   #$00                    ; F57A: 10 8E 00 00   
                    LDD   #$6000                  ; F57E: CC 60 00      
                    STD   ,Y++                    ; F581: ED A1         
                    LDD   #$BFAE                  ; F583: CC BF AE      
                    STD   ,Y++                    ; F586: ED A1         
                    LDD   #$8040                  ; F588: CC 80 40      
                    STD   ,Y++                    ; F58B: ED A1         
                    LDA   #$48                    ; F58D: 86 48         
                    TFR   A,DP                    ; F58F: 1F 8B         
                    STS   <$CC                    ; F591: 10 DF CC      
                    LDS   #$4FFF                  ; F594: 10 CE 4F FF   
                    LDB   #$D3                    ; F598: C6 D3         
                    JSR   LE7C7                   ; F59A: BD E7 C7        // Print text string from pointer table
LF59D               LDB   #$B0                    ; F59D: C6 B0         
                    JSR   LE7C7                   ; F59F: BD E7 C7        // Print text string from pointer table
LF5A2               LDB   #$B2                    ; F5A2: C6 B2         
                    STB   <$CE                    ; F5A4: D7 CE         
                    LDD   <$CC                    ; F5A6: DC CC         
                    BNE   LF5B1                   ; F5A8: 26 07         
                    LDB   #$B1                    ; F5AA: C6 B1         
                    JSR   LE7C7                   ; F5AC: BD E7 C7        // Print text string from pointer table
LF5AF               BRA   LF5EB                   ; F5AF: 20 3A         

; ---------------------------------------------------------------------------
LF5B1               LSRA                          ; F5B1: 44            
                    RORB                          ; F5B2: 56            
                    STD   <$CC                    ; F5B3: DD CC         
                    BCC   LF5E5                   ; F5B5: 24 2E         
                    LDB   <$CE                    ; F5B7: D6 CE         
                    JSR   LE7C7                   ; F5B9: BD E7 C7        // Print text string from pointer table
LF5BC               LDB   <$CE                    ; F5BC: D6 CE         
                    SUBB  #$BC                    ; F5BE: C0 BC         
                    BCS   LF5E5                   ; F5C0: 25 23         
                    ASLB                          ; F5C2: 58            
                    LDX   #$F743                  ; F5C3: 8E F7 43      
                    ABX                           ; F5C6: 3A            
                    LDU   ,X                      ; F5C7: EE 84         
                    STU   ,Y++                    ; F5C9: EF A1         
                    LDU   #$0120                  ; F5CB: CE 01 20      
                    STU   ,Y++                    ; F5CE: EF A1         
                    LDX   #$48D7                  ; F5D0: 8E 48 D7      
                    ABX                           ; F5D3: 3A            
                    LDA   ,X+                     ; F5D4: A6 80         
                    COMA                          ; F5D6: 43            
                    JSR   Display_Vect_BCD        ; F5D7: BD E7 90        // Display BCD numbers
LF5DA               LDA   ,X                      ; F5DA: A6 84         
                    COMA                          ; F5DC: 43            
                    JSR   Display_Vect_BCD        ; F5DD: BD E7 90        // Display BCD numbers
LF5E0               LDD   #$8040                  ; F5E0: CC 80 40      
                    STD   ,Y++                    ; F5E3: ED A1         
LF5E5               INC   <$CE                    ; F5E5: 0C CE         
                    LDD   <$CC                    ; F5E7: DC CC         
                    BNE   LF5B1                   ; F5E9: 26 C6         
LF5EB               BRA   LF643                   ; F5EB: 20 56         

; ---------------------------------------------------------------------------
LF5ED               LDX   #$08                    ; F5ED: 8E 00 08      
                    TFR   S,D                     ; F5F0: 1F 40         
LF5F2               LSRB                          ; F5F2: 54            
                    BCC   LF5F9                   ; F5F3: 24 04         
                    LDA   #$29                    ; F5F5: 86 29         
                    BRA   LF5FB                   ; F5F7: 20 02         

; ---------------------------------------------------------------------------
LF5F9               LDA   #$3B                    ; F5F9: 86 3B         
LF5FB               STA   $4400                   ; F5FB: B7 44 00      
                    LDU   #$00                    ; F5FE: CE 00 00      
LF601               STB   $4640                   ; F601: F7 46 40      
                    LEAU  $01,U                   ; F604: 33 41         
                    CMPU  #$A000                  ; F606: 11 83 A0 00   
                    BCS   LF601                   ; F60A: 25 F5         
                    LEAX  -1,X                    ; F60C: 30 1F         
                    BNE   LF5F2                   ; F60E: 26 E2         
                    LDX   #$08                    ; F610: 8E 00 08      
                    TFR   S,D                     ; F613: 1F 40         
LF615               LSRA                          ; F615: 44            
                    BCC   LF61C                   ; F616: 24 04         
                    LDB   #$29                    ; F618: C6 29         
                    BRA   LF61E                   ; F61A: 20 02         

; ---------------------------------------------------------------------------
LF61C               LDB   #$3B                    ; F61C: C6 3B         
LF61E               STB   $4400                   ; F61E: F7 44 00      
                    LDU   #$00                    ; F621: CE 00 00      
LF624               STA   $4640                   ; F624: B7 46 40      
                    LEAU  $01,U                   ; F627: 33 41         
                    CMPU  #$A000                  ; F629: 11 83 A0 00   
                    BCS   LF624                   ; F62D: 25 F5         
                    LEAX  -1,X                    ; F62F: 30 1F         
                    BNE   LF615                   ; F631: 26 E2         
                    LDU   #$00                    ; F633: CE 00 00      
LF636               STA   $4640                   ; F636: B7 46 40      
                    LEAU  $01,U                   ; F639: 33 41         
                    CMPU  #$FF00                  ; F63B: 11 83 FF 00   
                    BCS   LF636                   ; F63F: 25 F5         
                    BRA   LF5ED                   ; F641: 20 AA         

; ---------------------------------------------------------------------------
LF643               LDD   #$2020                  ; F643: CC 20 20      
                    STD   ,Y++                    ; F646: ED A1         
                    LDU   #off_F723               ; F648: CE F7 23      
                    CLR   <$C0                    ; F64B: 0F C0         
                    LDA   #$03                    ; F64D: 86 03         
                    STA   <$C5                    ; F64F: 97 C5         
                    STA   <$C6                    ; F651: 97 C6         
                    STA   <$C7                    ; F653: 97 C7         
                    STA   <$D6                    ; F655: 97 D6         
                    LDX   #word_F34E              ; F657: 8E F3 4E      
                    STX   <$D4                    ; F65A: 9F D4         
LF65C               LDD   #$01                    ; F65C: CC 00 01      
                    LDX   #$00                    ; F65F: 8E 00 00      
LF662               STA   $4640                   ; F662: B7 46 40      
                    LEAX  D,X                     ; F665: 30 8B         
                    CMPX  #$0708                  ; F667: 8C 07 08      
                    BCS   LF662                   ; F66A: 25 F6         
                    STA   $4620                   ; F66C: B7 46 20      
                    LDA   $4320                   ; F66F: B6 43 20      
                    ANDA  #$04                    ; F672: 84 04         
                    BNE   LF679                   ; F674: 26 03         
                    JMP   LF720                   ; F676: 7E F7 20      

; ---------------------------------------------------------------------------
LF679               LDA   $4300                   ; F679: B6 43 00      
                    ANDA  #$04                    ; F67C: 84 04         
                    BNE   LF69F                   ; F67E: 26 1F         
                    LDA   <$C5                    ; F680: 96 C5         
                    BEQ   LF69D                   ; F682: 27 19         
                    DECA                          ; F684: 4A            
                    BNE   LF69D                   ; F685: 26 16         
                    LEAU  $02,U                   ; F687: 33 42         
                    CMPU  #word_F735              ; F689: 11 83 F7 35   
                    BCS   LF692                   ; F68D: 25 03         
                    LDU   #off_F725               ; F68F: CE F7 25      
LF692               LDD   #$00                    ; F692: CC 00 00      
                    STD   <$C2                    ; F695: DD C2         
                    STA   <$C4                    ; F697: 97 C4         
                    STA   <$C0                    ; F699: 97 C0         
                    LDA   #$80                    ; F69B: 86 80         
LF69D               BRA   LF6A1                   ; F69D: 20 02         

; ---------------------------------------------------------------------------
LF69F               LDA   #$03                    ; F69F: 86 03         
LF6A1               STA   <$C5                    ; F6A1: 97 C5         
                    LDA   $4300                   ; F6A3: B6 43 00      
                    COMA                          ; F6A6: 43            
                    ANDA  #$C0                    ; F6A7: 84 C0         
                    BEQ   LF6C7                   ; F6A9: 27 1C         
                    LDA   <$C6                    ; F6AB: 96 C6         
                    BEQ   LF6C5                   ; F6AD: 27 16         
                    DECA                          ; F6AF: 4A            
                    BNE   LF6C5                   ; F6B0: 26 13         
                    LDB   <$C0                    ; F6B2: D6 C0         
                    ADDB  #$02                    ; F6B4: CB 02         
                    CMPB  #$0E                    ; F6B6: C1 0E         
                    BCS   LF6BB                   ; F6B8: 25 01         
                    CLRB                          ; F6BA: 5F            
LF6BB               STB   <$C0                    ; F6BB: D7 C0         
                    CMPB  #$04                    ; F6BD: C1 04         
                    BNE   LF6C3                   ; F6BF: 26 02         
                    CLR   <$C1                    ; F6C1: 0F C1         
LF6C3               LDA   #$80                    ; F6C3: 86 80         
LF6C5               BRA   LF6C9                   ; F6C5: 20 02         

; ---------------------------------------------------------------------------
LF6C7               LDA   #$03                    ; F6C7: 86 03         
LF6C9               STA   <$C6                    ; F6C9: 97 C6         
                    CMPU  #off_F733               ; F6CB: 11 83 F7 33   
                    BNE   LF6F0                   ; F6CF: 26 1F         
                    LDA   $4320                   ; F6D1: B6 43 20      
                    COMA                          ; F6D4: 43            
                    ANDA  #$30                    ; F6D5: 84 30         
                    BEQ   LF6EA                   ; F6D7: 27 11         
                    LDA   <$C7                    ; F6D9: 96 C7         
                    BEQ   LF6E8                   ; F6DB: 27 0B         
                    DECA                          ; F6DD: 4A            
                    BNE   LF6E8                   ; F6DE: 26 08         
                    LDB   <$C1                    ; F6E0: D6 C1         
                    EORB  #$01                    ; F6E2: C8 01         
                    STB   <$C1                    ; F6E4: D7 C1         
                    LDA   #$80                    ; F6E6: 86 80         
LF6E8               BRA   LF6EC                   ; F6E8: 20 02         

; ---------------------------------------------------------------------------
LF6EA               LDA   #$03                    ; F6EA: 86 03         
LF6EC               STA   <$C7                    ; F6EC: 97 C7         
                    BRA   LF6F2                   ; F6EE: 20 02         

; ---------------------------------------------------------------------------
LF6F0               CLR   <$C1                    ; F6F0: 0F C1         
LF6F2               LDY   #$00                    ; F6F2: 10 8E 00 00   
                    LDB   <$C0                    ; F6F6: D6 C0         
                    LDX   #word_F735              ; F6F8: 8E F7 35      
                    LDD   B,X                     ; F6FB: EC 85         
                    CMPD  #$6780                  ; F6FD: 10 83 67 80   
                    BNE   LF709                   ; F701: 26 06         
                    TST   <$C1                    ; F703: 0D C1         
                    BEQ   LF709                   ; F705: 27 02         
                    LDB   #$20                    ; F707: C6 20         
LF709               STD   ,Y++                    ; F709: ED A1         
; END OF FUNCTION CHUNK FOR sub_6532
                    JMP   [,U]                    ; F70B: 6E D4         

; =============== S U B R O U T I N E =======================================
LF70D               LDD   #$2020                  ; F70D: CC 20 20      
                    STD   ,Y++                    ; F710: ED A1         
                    STD   ,Y++                    ; F712: ED A1         
LF714               STA   $4600                   ; F714: B7 46 00      
                    LDA   $4300                   ; F717: B6 43 00      
                    ANDA  #$10                    ; F71A: 84 10         
; End of function sub_F70D
                    LBEQ  LF65C                   ; F71C: 10 27 FF 3C   
; START OF FUNCTION CHUNK FOR sub_6532
; END OF FUNCTION CHUNK FOR sub_6532
; ---------------------------------------------------------------------------
LF720               JMP   LF720                   ; F720: 7E F7 20      

; =============== S U B R O U T I N E =======================================
LF74F               DEC   <$D6                    ; F74F: 0A D6         
                    BPL   LF77C                   ; F751: 2A 29         
                    LDA   #$10                    ; F753: 86 10         
                    STA   <$D6                    ; F755: 97 D6         
                    LDX   <$D4                    ; F757: 9E D4         
                    LDD   <$D2                    ; F759: DC D2         
                    ANDA  ,X                      ; F75B: A4 84         
                    BNE   LF767                   ; F75D: 26 08         
                    ANDB  $01,X                   ; F75F: E4 01         
                    BNE   LF767                   ; F761: 26 04         
                    LDA   #$3B                    ; F763: 86 3B         
                    BRA   LF769                   ; F765: 20 02         

; ---------------------------------------------------------------------------
LF767               LDA   #$29                    ; F767: 86 29         
LF769               STA   $4400                   ; F769: B7 44 00      
                    LEAX  $02,X                   ; F76C: 30 02         
                    CMPX  #LF36E                  ; F76E: 8C F3 6E      
                    BCS   LF77A                   ; F771: 25 07         
                    LDA   #$20                    ; F773: 86 20         
                    STA   <$D6                    ; F775: 97 D6         
                    LDX   #word_F34E              ; F777: 8E F3 4E      
LF77A               STX   <$D4                    ; F77A: 9F D4         
; End of function sub_F74F
LF77C               JMP   LF714                   ; F77C: 7E F7 14      

; =============== S U B R O U T I N E =======================================
LF77F               STA   $46C0                   ; F77F: B7 46 C0      
                    STA   $46C0                   ; F782: B7 46 C0      
                    LDD   #$BFAE                  ; F785: CC BF AE      
                    STD   ,Y++                    ; F788: ED A1         
                    LDD   #$8040                  ; F78A: CC 80 40      
                    STD   ,Y++                    ; F78D: ED A1         
                    LDS   #$4FFF                  ; F78F: 10 CE 4F FF   
                    LDB   #$9E                    ; F793: C6 9E         
                    JSR   LE7C7                   ; F795: BD E7 C7        // Print text string from pointer table
LF798               LDB   #$D3                    ; F798: C6 D3         
                    JSR   LE7C7                   ; F79A: BD E7 C7        // Print text string from pointer table
LF79D               LDB   #$9F                    ; F79D: C6 9F         
                    STB   <$CE                    ; F79F: D7 CE         
                    LDA   $4300                   ; F7A1: B6 43 00      
                    LDB   $4320                   ; F7A4: F6 43 20      
                    ORB   #$C7                    ; F7A7: CA C7         
                    STD   <$CC                    ; F7A9: DD CC         
LF7AB               ASLB                          ; F7AB: 58            
                    ROLA                          ; F7AC: 49            
                    STD   <$CC                    ; F7AD: DD CC         
                    BCS   LF7B6                   ; F7AF: 25 05         
                    LDB   <$CE                    ; F7B1: D6 CE         
                    JSR   LE7C7                   ; F7B3: BD E7 C7        // Print text string from pointer table
LF7B6               INC   <$CE                    ; F7B6: 0C CE         
                    LDD   <$CC                    ; F7B8: DC CC         
                    BNE   LF7AB                   ; F7BA: 26 EF         
                    LDD   <$C8                    ; F7BC: DC C8         
                    STD   <$CA                    ; F7BE: DD CA         
                    LDA   $4300                   ; F7C0: B6 43 00      
                    ANDA  #$CF                    ; F7C3: 84 CF         
                    LDB   $4320                   ; F7C5: F6 43 20      
                    ANDB  #$3A                    ; F7C8: C4 3A         
                    STD   <$C8                    ; F7CA: DD C8         
                    EORA  <$CA                    ; F7CC: 98 CA         
                    ANDA  <$CA                    ; F7CE: 94 CA         
                    EORB  <$CB                    ; F7D0: D8 CB         
                    ANDB  <$CB                    ; F7D2: D4 CB         
                    CMPD  #$00                    ; F7D4: 10 83 00 00   
                    BEQ   LF7DF                   ; F7D8: 27 05         
                    LDA   #$3B                    ; F7DA: 86 3B         
                    STA   $4400                   ; F7DC: B7 44 00      
LF7DF               LDD   #$1EA2                  ; F7DF: CC 1E A2      
                    STD   ,Y++                    ; F7E2: ED A1         
                    LDD   #$1F74                  ; F7E4: CC 1F 74      
                    STD   ,Y++                    ; F7E7: ED A1         
                    LDA   #$10                    ; F7E9: 86 10         
                    JSR   Display_Vect_BCD        ; F7EB: BD E7 90        // Display BCD numbers
LF7EE               LDD   $301E                   ; F7EE: FC 30 1E      
                    STD   ,Y++                    ; F7F1: ED A1         
                    LDD   $3002                   ; F7F3: FC 30 02      
                    STD   ,Y++                    ; F7F6: ED A1         
                    LDA   $4340                   ; F7F8: B6 43 40      
                    JSR   LF86C                   ; F7FB: BD F8 6C      
LF7FE               LDD   #$1E70                  ; F7FE: CC 1E 70      
                    STD   ,Y++                    ; F801: ED A1         
                    LDD   #$1F5C                  ; F803: CC 1F 5C      
                    STD   ,Y++                    ; F806: ED A1         
                    LDA   #$10                    ; F808: 86 10         
                    JSR   Display_Vect_BCD        ; F80A: BD E7 90        // Display BCD numbers
LF80D               LDD   $3020                   ; F80D: FC 30 20      
                    STD   ,Y++                    ; F810: ED A1         
                    LDD   $3022                   ; F812: FC 30 22      
                    STD   ,Y++                    ; F815: ED A1         
                    LDD   $3002                   ; F817: FC 30 02      
                    STD   ,Y++                    ; F81A: ED A1         
                    LDA   $4360                   ; F81C: B6 43 60      
                    JSR   LF86C                   ; F81F: BD F8 6C      
LF822               LDB   #$AF                    ; F822: C6 AF         
                    JSR   LE7C7                   ; F824: BD E7 C7        // Print text string from pointer table
LF827               LDD   #$BFD5                  ; F827: CC BF D5      
                    STD   ,Y++                    ; F82A: ED A1         
                    LDD   #$00                    ; F82C: CC 00 00      
                    STD   ,Y++                    ; F82F: ED A1         
                    LDD   #$0140                  ; F831: CC 01 40      
                    STD   ,Y++                    ; F834: ED A1         
                    LDB   $4380                   ; F836: F6 43 80      
                    SUBB  #$80                    ; F839: C0 80         
                    SEX                           ; F83B: 1D            
                    TFR   D,U                     ; F83C: 1F 03         
                    ASRA                          ; F83E: 47            
                    RORB                          ; F83F: 56            
                    LEAU  D,U                     ; F840: 33 CB         
                    TFR   U,D                     ; F842: 1F 30         
                    ANDA  #$1F                    ; F844: 84 1F         
                    STD   ,Y++                    ; F846: ED A1         
                    STA   $46C1                   ; F848: B7 46 C1      
                    STA   $46C1                   ; F84B: B7 46 C1      
                    LDX   #$14                    ; F84E: 8E 00 14      
LF851               LEAX  -1,X                    ; F851: 30 1F         
                    BNE   LF851                   ; F853: 26 FC         
                    LDB   $4380                   ; F855: F6 43 80      
                    SUBB  #$80                    ; F858: C0 80         
                    SEX                           ; F85A: 1D            
                    ANDA  #$1F                    ; F85B: 84 1F         
                    ORA   #$E0                    ; F85D: 8A E0         
                    STD   ,Y++                    ; F85F: ED A1         
                    LDD   #$8040                  ; F861: CC 80 40      
                    STD   ,Y++                    ; F864: ED A1         
                    LDU   #off_F725               ; F866: CE F7 25      
; End of function sub_F77F
                    JMP   LF70D                   ; F869: 7E F7 0D      

; =============== S U B R O U T I N E =======================================
LF86C               LDB   #$07                    ; F86C: C6 07         
LF86E               LSRA                          ; F86E: 44            
                    BCC   LF876                   ; F86F: 24 05         
                    LDX   $3022                   ; F871: BE 30 22      
                    BRA   LF879                   ; F874: 20 03         

; ---------------------------------------------------------------------------
LF876               LDX   $3032                   ; F876: BE 30 32      
LF879               STX   ,Y++                    ; F879: AF A1         
                    DECB                          ; F87B: 5A            
                    BPL   LF86E                   ; F87C: 2A F0         
                    LDD   #$8040                  ; F87E: CC 80 40      
                    STD   ,Y++                    ; F881: ED A1         
; End of function sub_F86C
                    RTS                           ; F883: 39            

; =============== S U B R O U T I N E =======================================
LF884               LDD   #$BF20                  ; F884: CC BF 20      
                    STD   ,Y++                    ; F887: ED A1         
; End of function sub_F884
                    JMP   LF70D                   ; F889: 7E F7 0D      

; =============== S U B R O U T I N E =======================================
LF88C               LDD   #$BEFD                  ; F88C: CC BE FD      
                    STD   ,Y++                    ; F88F: ED A1         
; End of function sub_F88C
                    JMP   LF70D                   ; F891: 7E F7 0D      

; =============== S U B R O U T I N E =======================================
LF894               LDD   #$BEA7                  ; F894: CC BE A7      
                    STD   ,Y++                    ; F897: ED A1         
                    LDB   #$CE                    ; F899: C6 CE         
LF89B               STB   <$CE                    ; F89B: D7 CE         
                    JSR   LE7C7                   ; F89D: BD E7 C7        // Print text string from pointer table
LF8A0               LDB   <$CE                    ; F8A0: D6 CE         
                    INCB                          ; F8A2: 5C            
                    CMPB  #$D3                    ; F8A3: C1 D3         
                    BCS   LF89B                   ; F8A5: 25 F4         
; End of function sub_F894
                    JMP   LF70D                   ; F8A7: 7E F7 0D      

; =============== S U B R O U T I N E =======================================
LF8AA               LDD   #$BFAE                  ; F8AA: CC BF AE      
                    STD   ,Y++                    ; F8AD: ED A1         
                    LDD   #$BFB3                  ; F8AF: CC BF B3      
                    STD   ,Y++                    ; F8B2: ED A1         
                    STD   ,Y++                    ; F8B4: ED A1         
                    STD   ,Y++                    ; F8B6: ED A1         
                    STD   ,Y++                    ; F8B8: ED A1         
                    LDD   #$8040                  ; F8BA: CC 80 40      
                    STD   ,Y++                    ; F8BD: ED A1         
                    LDB   #$C9                    ; F8BF: C6 C9         
                    JSR   LE7C7                   ; F8C1: BD E7 C7        // Print text string from pointer table
LF8C4               LDB   #$CA                    ; F8C4: C6 CA         
                    JSR   LE7C7                   ; F8C6: BD E7 C7        // Print text string from pointer table
; End of function sub_F8AA
LF8C9               JMP   LF70D                   ; F8C9: 7E F7 0D      

; =============== S U B R O U T I N E =======================================
LF8CC               LDA   <$C6                    ; F8CC: 96 C6         
                    CMPA  #$01                    ; F8CE: 81 01         
                    BHI   LF8D9                   ; F8D0: 22 07         
                    LDD   #$00                    ; F8D2: CC 00 00      
                    STD   <$C2                    ; F8D5: DD C2         
                    STA   <$C4                    ; F8D7: 97 C4         
LF8D9               LDD   #$6000                  ; F8D9: CC 60 00      
                    STD   ,Y++                    ; F8DC: ED A1         
                    LDD   #$BFAF                  ; F8DE: CC BF AF      
                    STD   ,Y++                    ; F8E1: ED A1         
                    LDD   #$8040                  ; F8E3: CC 80 40      
                    STD   ,Y++                    ; F8E6: ED A1         
                    LDD   <$C2                    ; F8E8: DC C2         
                    ADDD  #$01                    ; F8EA: C3 00 01      
                    TST   <$C4                    ; F8ED: 0D C4         
                    BNE   LF900                   ; F8EF: 26 0F         
                    CMPB  #$FC                    ; F8F1: C1 FC         
                    BCS   LF8FC                   ; F8F3: 25 07         
                    LDA   #$01                    ; F8F5: 86 01         
                    STA   <$C4                    ; F8F7: 97 C4         
                    LDD   #$00                    ; F8F9: CC 00 00      
LF8FC               STD   <$C2                    ; F8FC: DD C2         
                    BRA   LF917                   ; F8FE: 20 17         

; ---------------------------------------------------------------------------
LF900               CMPB  #$B0                    ; F900: C1 B0         
                    BCS   LF906                   ; F902: 25 02         
                    CLRB                          ; F904: 5F            
                    INCA                          ; F905: 4C            
LF906               CMPA  #$08                    ; F906: 81 08         
                    BCS   LF90F                   ; F908: 25 05         
                    CLR   <$C4                    ; F90A: 0F C4         
                    LDD   #$00                    ; F90C: CC 00 00      
LF90F               STD   <$C2                    ; F90F: DD C2         
                    CMPB  #$7F                    ; F911: C1 7F         
                    BCS   LF917                   ; F913: 25 02         
                    LDB   #$7F                    ; F915: C6 7F         
LF917               ORA   #$70                    ; F917: 8A 70         
                    STD   ,Y++                    ; F919: ED A1         
                    LDD   #$6280                  ; F91B: CC 62 80      
                    STD   ,Y++                    ; F91E: ED A1         
                    LDD   #$BFBC                  ; F920: CC BF BC      
                    STD   ,Y++                    ; F923: ED A1         
                    LDD   #$7200                  ; F925: CC 72 00      
                    STD   ,Y++                    ; F928: ED A1         
                    LDB   <$C4                    ; F92A: D6 C4         
                    BNE   LF932                   ; F92C: 26 04         
                    LDB   #$CC                    ; F92E: C6 CC         
                    BRA   LF934                   ; F930: 20 02         

; ---------------------------------------------------------------------------
LF932               LDB   #$CD                    ; F932: C6 CD         
LF934               JSR   LE7C7                   ; F934: BD E7 C7        // Print text string from pointer table
LF937               LDB   #$CB                    ; F937: C6 CB         
                    JSR   LE7C7                   ; F939: BD E7 C7        // Print text string from pointer table
; End of function sub_F8CC
LF93C               JMP   LF70D                   ; F93C: 7E F7 0D      

; =============== S U B R O U T I N E =======================================
LF93F               LDD   #$BFC8                  ; F93F: CC BF C8      
                    STD   ,Y++                    ; F942: ED A1         
                    LDX   #$38                    ; F944: 8E 00 38      
                    LDD   #$BFCC                  ; F947: CC BF CC      
LF94A               STD   ,Y++                    ; F94A: ED A1         
                    LEAX  -1,X                    ; F94C: 30 1F         
                    BNE   LF94A                   ; F94E: 26 FA         
                    LDD   #$8040                  ; F950: CC 80 40      
                    STD   ,Y++                    ; F953: ED A1         
; End of function sub_F93F
                    JMP   LF70D                   ; F955: 7E F7 0D      

; =============== S U B R O U T I N E =======================================
LF958               LDD   #$BFAE                  ; F958: CC BF AE      
                    STD   ,Y++                    ; F95B: ED A1         
                    LDD   #$8040                  ; F95D: CC 80 40      
                    STD   ,Y++                    ; F960: ED A1         
                    LDD   #$7200                  ; F962: CC 72 00      
                    STD   ,Y++                    ; F965: ED A1         
                    LDS   #$4FFF                  ; F967: 10 CE 4F FF   
                    LDB   #$C2                    ; F96B: C6 C2         
                    JSR   LE7C7                   ; F96D: BD E7 C7        // Print text string from pointer table
LF970               LDA   #$5D                    ; F970: 86 5D         
                    STA   $4700                   ; F972: B7 47 00      
                    TST   $4320                   ; F975: 7D 43 20      
                    BPL   LF97F                   ; F978: 2A 05         
                    TST   $4320                   ; F97A: 7D 43 20      
                    BPL   LF98A                   ; F97D: 2A 0B         
LF97F               LDB   #$C3                    ; F97F: C6 C3         
                    JSR   LE7C7                   ; F981: BD E7 C7        // Print text string from pointer table
LF984               LDU   #off_F727               ; F984: CE F7 27      
                    JMP   LF70D                   ; F987: 7E F7 0D      

; ---------------------------------------------------------------------------
LF98A               LDD   #$6280                  ; F98A: CC 62 80      
                    STD   ,Y++                    ; F98D: ED A1         
                    CLR   <$CC                    ; F98F: 0F CC         
                    LDX   #word_FB4B              ; F991: 8E FB 4B      
LF994               LDS   #LF99B                  ; F994: 10 CE F9 9B   
; End of function sub_F958
                    JMP   LFB38                   ; F998: 7E FB 38      

; =============== S U B R O U T I N E =======================================
LF99B               BEQ   LFA19                   ; F99B: 27 7C         
                    STD   <$D2                    ; F99D: DD D2         
                    INC   <$CC                    ; F99F: 0C CC         
                    LDS   #$4FFF                  ; F9A1: 10 CE 4F FF   
                    LDD   $06,X                   ; F9A5: EC 06         
                    STD   ,Y++                    ; F9A7: ED A1         
                    LDD   #$1E98                  ; F9A9: CC 1E 98      
                    STD   ,Y++                    ; F9AC: ED A1         
                    LDD   $3032                   ; F9AE: FC 30 32      
                    STD   ,Y++                    ; F9B1: ED A1         
                    STD   ,Y++                    ; F9B3: ED A1         
                    STD   ,Y++                    ; F9B5: ED A1         
                    STD   ,Y++                    ; F9B7: ED A1         
                    LDD   $3002                   ; F9B9: FC 30 02      
                    STD   ,Y++                    ; F9BC: ED A1         
                    TFR   X,D                     ; F9BE: 1F 10         
                    SUBD  #word_FB4B              ; F9C0: 83 FB 4B      
                    ASLB                          ; F9C3: 58            
                    LDA   #$03                    ; F9C4: 86 03         
LF9C6               ASLB                          ; F9C6: 58            
                    BCS   LF9CE                   ; F9C7: 25 05         
                    LDU   $3022                   ; F9C9: FE 30 22      
                    BRA   LF9D1                   ; F9CC: 20 03         

; ---------------------------------------------------------------------------
LF9CE               LDU   $3032                   ; F9CE: FE 30 32      
LF9D1               STU   ,Y++                    ; F9D1: EF A1         
                    DECA                          ; F9D3: 4A            
                    BPL   LF9C6                   ; F9D4: 2A F0         
                    LDD   $3002                   ; F9D6: FC 30 02      
                    STD   ,Y++                    ; F9D9: ED A1         
                    STD   ,Y++                    ; F9DB: ED A1         
                    LDA   ,X                      ; F9DD: A6 84         
                    JSR   Display_Vect_BCD        ; F9DF: BD E7 90        // Display BCD numbers
LF9E2               LDA   $01,X                   ; F9E2: A6 01         
                    JSR   Display_Vect_BCD        ; F9E4: BD E7 90        // Display BCD numbers
LF9E7               LDD   $3002                   ; F9E7: FC 30 02      
                    STD   ,Y++                    ; F9EA: ED A1         
                    LDA   $02,X                   ; F9EC: A6 02         
                    JSR   Display_Vect_BCD        ; F9EE: BD E7 90        // Display BCD numbers
LF9F1               LDA   $03,X                   ; F9F1: A6 03         
                    JSR   Display_Vect_BCD        ; F9F3: BD E7 90        // Display BCD numbers
LF9F6               LDD   $3002                   ; F9F6: FC 30 02      
                    STD   ,Y++                    ; F9F9: ED A1         
                    LDA   $04,X                   ; F9FB: A6 04         
                    JSR   Display_Vect_BCD        ; F9FD: BD E7 90        // Display BCD numbers
LFA00               LDA   $05,X                   ; FA00: A6 05         
                    JSR   Display_Vect_BCD        ; FA02: BD E7 90        // Display BCD numbers
LFA05               LDD   $3002                   ; FA05: FC 30 02      
                    STD   ,Y++                    ; FA08: ED A1         
                    LDA   <$D2                    ; FA0A: 96 D2         
                    JSR   Display_Vect_BCD        ; FA0C: BD E7 90        // Display BCD numbers
LFA0F               LDA   <$D3                    ; FA0F: 96 D3         
                    JSR   Display_Vect_BCD        ; FA11: BD E7 90        // Display BCD numbers
LFA14               LDD   #$8040                  ; FA14: CC 80 40      
                    STD   ,Y++                    ; FA17: ED A1         
LFA19               LEAX  $08,X                   ; FA19: 30 08         
                    CMPX  #LFB73                  ; FA1B: 8C FB 73      
                    LBCS  LF994                   ; FA1E: 10 25 FF 72   
                    LDS   #$4FFF                  ; FA22: 10 CE 4F FF   
                    TST   <$CC                    ; FA26: 0D CC         
                    BNE   LFA2E                   ; FA28: 26 04         
                    LDB   #$C5                    ; FA2A: C6 C5         
                    BRA   LFA35                   ; FA2C: 20 07         

; ---------------------------------------------------------------------------
LFA2E               LDB   #$C4                    ; FA2E: C6 C4         
                    JSR   LE7C7                   ; FA30: BD E7 C7        // Print text string from pointer table
LFA33               LDB   #$C6                    ; FA33: C6 C6         
LFA35               JSR   LE7C7                   ; FA35: BD E7 C7        // Print text string from pointer table
LFA38               LDX   #off_FAE1               ; FA38: 8E FA E1      
                    CLR   <$CC                    ; FA3B: 0F CC         
                    LDD   #$6480                  ; FA3D: CC 64 80      
                    STD   ,Y++                    ; FA40: ED A1         
LFA42               TFR   X,D                     ; FA42: 1F 10         
                    SUBD  #off_FAE9               ; FA44: 83 FA E9      
                    LSRB                          ; FA47: 54            
                    LDS   #LFA4E                  ; FA48: 10 CE FA 4E   
; End of function sub_F99B
                    JMP   [,X]                    ; FA4C: 6E 94         

; =============== S U B R O U T I N E =======================================
LFA4E               BEQ   LFAC1                   ; FA4E: 27 71         
                    STD   <$D2                    ; FA50: DD D2         
                    INC   <$CC                    ; FA52: 0C CC         
                    CMPX  #off_FAF9               ; FA54: 8C FA F9      
                    BCC   LFA92                   ; FA57: 24 39         
                    LDD   $02,X                   ; FA59: EC 02         
                    STD   ,Y++                    ; FA5B: ED A1         
                    LDD   #$1EA2                  ; FA5D: CC 1E A2      
                    STD   ,Y++                    ; FA60: ED A1         
                    LDD   $3032                   ; FA62: FC 30 32      
                    STD   ,Y++                    ; FA65: ED A1         
                    STD   ,Y++                    ; FA67: ED A1         
                    STD   ,Y++                    ; FA69: ED A1         
                    LDD   $3022                   ; FA6B: FC 30 22      
                    STD   ,Y++                    ; FA6E: ED A1         
                    LDD   $3002                   ; FA70: FC 30 02      
                    STD   ,Y++                    ; FA73: ED A1         
                    TFR   X,D                     ; FA75: 1F 10         
                    SUBD  #off_FAE1               ; FA77: 83 FA E1      
                    ASLB                          ; FA7A: 58            
                    ASLB                          ; FA7B: 58            
                    ADDB  #$50                    ; FA7C: CB 50         
                    LDA   #$03                    ; FA7E: 86 03         
LFA80               ASLB                          ; FA80: 58            
                    BCS   LFA88                   ; FA81: 25 05         
                    LDU   $3022                   ; FA83: FE 30 22      
                    BRA   LFA8B                   ; FA86: 20 03         

; ---------------------------------------------------------------------------
LFA88               LDU   $3032                   ; FA88: FE 30 32      
LFA8B               STU   ,Y++                    ; FA8B: EF A1         
                    DECA                          ; FA8D: 4A            
                    BPL   LFA80                   ; FA8E: 2A F0         
                    BRA   LFAA9                   ; FA90: 20 17         

; ---------------------------------------------------------------------------
LFA92               LDD   $02,X                   ; FA92: EC 02         
                    STD   ,Y++                    ; FA94: ED A1         
                    LDD   #$96                    ; FA96: CC 00 96      
                    STD   ,Y++                    ; FA99: ED A1         
                    TFR   X,D                     ; FA9B: 1F 10         
                    SUBD  #off_FAF9               ; FA9D: 83 FA F9      
                    LSRB                          ; FAA0: 54            
                    LDS   #$3018                  ; FAA1: 10 CE 30 18   
                    LDD   B,S                     ; FAA5: EC E5         
                    STD   ,Y++                    ; FAA7: ED A1         
LFAA9               LDD   $3002                   ; FAA9: FC 30 02      
                    STD   ,Y++                    ; FAAC: ED A1         
                    LDS   #$4FFF                  ; FAAE: 10 CE 4F FF   
                    LDA   <$D2                    ; FAB2: 96 D2         
                    JSR   Display_Vect_BCD        ; FAB4: BD E7 90        // Display BCD numbers
LFAB7               LDA   <$D3                    ; FAB7: 96 D3         
                    JSR   Display_Vect_BCD        ; FAB9: BD E7 90        // Display BCD numbers
LFABC               LDD   #$8040                  ; FABC: CC 80 40      
                    STD   ,Y++                    ; FABF: ED A1         
LFAC1               LEAX  $04,X                   ; FAC1: 30 04         
                    CMPX  #LFB09                  ; FAC3: 8C FB 09      
                    LBCS  LFA42                   ; FAC6: 10 25 FF 78   
                    LDS   #$4FFF                  ; FACA: 10 CE 4F FF   
                    TST   <$CC                    ; FACE: 0D CC         
                    BNE   LFAD6                   ; FAD0: 26 04         
                    LDB   #$C8                    ; FAD2: C6 C8         
                    BRA   LFAD8                   ; FAD4: 20 02         

; ---------------------------------------------------------------------------
LFAD6               LDB   #$C7                    ; FAD6: C6 C7         
LFAD8               JSR   LE7C7                   ; FAD8: BD E7 C7        // Print text string from pointer table
LFADB               LDU   #off_F727               ; FADB: CE F7 27      
; End of function sub_FA4E
; ---------------------------------------------------------------------------
                    JMP   LF70D                   ; FADE: 7E F7 0D      

; =============== S U B R O U T I N E =======================================
; FUNCTION CHUNK AT ROM:FBF6 SIZE 00000026 BYTES
LFB09               LDS   #LFB10                  ; FB09: 10 CE FB 10   
; End of function sub_FB09
                    JMP   LFBF6                   ; FB0D: 7E FB F6      

; =============== S U B R O U T I N E =======================================
LFB10               CMPD  #$01                    ; FB10: 10 83 00 01   
                    BEQ   LFB19                   ; FB14: 27 03         
                    JMP   LFA4E                   ; FB16: 7E FA 4E      

; ---------------------------------------------------------------------------
LFB19               LDU   #$02                    ; FB19: CE 00 02      
LFB1C               LDS   #LFB23                  ; FB1C: 10 CE FB 23   
; End of function sub_FB10
                    JMP   LFC0D                   ; FB20: 7E FC 0D      

; =============== S U B R O U T I N E =======================================
LFB23               CMPD  -$0CB2,U                ; FB23: 10 A3 C9 F3 4E
                    BEQ   LFB2D                   ; FB28: 27 03         
                    JMP   LFA4E                   ; FB2A: 7E FA 4E      

; ---------------------------------------------------------------------------
LFB2D               LEAU  $02,U                   ; FB2D: 33 42         
                    CMPU  #$20                    ; FB2F: 11 83 00 20   
                    BCS   LFB1C                   ; FB33: 25 E7         
; End of function sub_FB23
                    JMP   LFA4E                   ; FB35: 7E FA 4E      

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_FE7C
LFB38               LDD   ,X                      ; FB38: EC 84         
                    STD   $4706                   ; FB3A: FD 47 06      
                    LDD   $02,X                   ; FB3D: EC 02         
                    STD   $4704                   ; FB3F: FD 47 04      
                    MUL                           ; FB42: 3D            
                    LDD   $4700                   ; FB43: FC 47 00      
                    CMPD  $04,X                   ; FB46: 10 A3 04      
; END OF FUNCTION CHUNK FOR sub_FE7C
; ---------------------------------------------------------------------------
                    JMP   ,S                      ; FB49: 6E E4         

LFB73               JMP   ,S                      ; FB73: 6E E4         

; =============== S U B R O U T I N E =======================================
LFB75               LDD   #$5555                  ; FB75: CC 55 55      
                    STD   $501E                   ; FB78: FD 50 1E        // Math zero constant
                    LDA   #$57                    ; FB7B: 86 57         
                    STA   $4700                   ; FB7D: B7 47 00      
; End of function sub_FB75
                    BRA   LFB73                   ; FB80: 20 F1         

; =============== S U B R O U T I N E =======================================
LFB82               LDD   #$AAAA                  ; FB82: CC AA AA      
                    STD   $501E                   ; FB85: FD 50 1E        // Math zero constant
                    LDA   #$58                    ; FB88: 86 58         
                    STA   $4700                   ; FB8A: B7 47 00      
; End of function sub_FB82
                    BRA   LFB73                   ; FB8D: 20 E4         

; =============== S U B R O U T I N E =======================================
LFB8F               LDD   #$5555                  ; FB8F: CC 55 55      
                    STD   $501E                   ; FB92: FD 50 1E        // Math zero constant
                    LDA   #$59                    ; FB95: 86 59         
                    STA   $4700                   ; FB97: B7 47 00      
; End of function sub_FB8F
                    BRA   LFB73                   ; FB9A: 20 D7         

; =============== S U B R O U T I N E =======================================
LFB9C               LDA   #$5A                    ; FB9C: 86 5A         
                    STA   $4700                   ; FB9E: B7 47 00      
; End of function sub_FB9C
                    BRA   LFB73                   ; FBA1: 20 D0         

; =============== S U B R O U T I N E =======================================
LFBA3               LDA   #$5B                    ; FBA3: 86 5B         
                    STA   $4700                   ; FBA5: B7 47 00      
; End of function sub_FBA3
                    BRA   LFB73                   ; FBA8: 20 C9         

; =============== S U B R O U T I N E =======================================
LFBAA               LDD   #$5555                  ; FBAA: CC 55 55      
                    STD   $5000                   ; FBAD: FD 50 00        // Math result X
                    LDA   #$5C                    ; FBB0: 86 5C         
                    STA   $4700                   ; FBB2: B7 47 00      
                    NOP                           ; FBB5: 12            
                    LDD   $5002                   ; FBB6: FC 50 02        // Math result Y
                    CMPD  #$5555                  ; FBB9: 10 83 55 55   
; End of function sub_FBAA
                    BRA   LFB73                   ; FBBD: 20 B4         

; =============== S U B R O U T I N E =======================================
LFBBF               LDD   #$AAAA                  ; FBBF: CC AA AA      
                    STD   $5000                   ; FBC2: FD 50 00        // Math result X
                    LDA   #$5C                    ; FBC5: 86 5C         
                    STA   $4700                   ; FBC7: B7 47 00      
                    NOP                           ; FBCA: 12            
                    LDD   $5002                   ; FBCB: FC 50 02        // Math result Y
                    CMPD  #$AAAA                  ; FBCE: 10 83 AA AA   
; End of function sub_FBBF
                    BRA   LFB73                   ; FBD2: 20 9F         

; =============== S U B R O U T I N E =======================================
LFBD4               LDD   #$2696                  ; FBD4: CC 26 96      
                    STD   $5018                   ; FBD7: FD 50 18        // XT
                    LDD   #$1B2C                  ; FBDA: CC 1B 2C      
                    STD   $501A                   ; FBDD: FD 50 1A        // YT
                    LDD   #$4000                  ; FBE0: CC 40 00      
                    STD   $501C                   ; FBE3: FD 50 1C        // ZT
                    LDA   #$5D                    ; FBE6: 86 5D         
                    STA   $4700                   ; FBE8: B7 47 00      
                    MUL                           ; FBEB: 3D            
                    LDD   $5000                   ; FBEC: FC 50 00        // Math result X
                    CMPD  #$0B6A                  ; FBEF: 10 83 0B 6A   
; End of function sub_FBD4
                    JMP   LFB73                   ; FBF3: 7E FB 73      

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_FB09
LFBF6               LDA   #$5A                    ; FBF6: 86 5A         
                    STA   $4700                   ; FBF8: B7 47 00      
                    LDD   #$00                    ; FBFB: CC 00 00      
                    STD   $501A                   ; FBFE: FD 50 1A        // YT
                    LDD   #$4000                  ; FC01: CC 40 00      
                    STD   $501C                   ; FC04: FD 50 1C        // ZT
                    LDD   #$01                    ; FC07: CC 00 01      
                    STD   $5018                   ; FC0A: FD 50 18        // XT
LFC0D               STD   $5018                   ; FC0D: FD 50 18        // XT
                    LDA   #$5E                    ; FC10: 86 5E         
                    STA   $4700                   ; FC12: B7 47 00      
                    MUL                           ; FC15: 3D            
                    LDD   $5000                   ; FC16: FC 50 00        // Math result X
; END OF FUNCTION CHUNK FOR sub_FB09
                    JMP   LFB73                   ; FC19: 7E FB 73      

; =============== S U B R O U T I N E =======================================
LFC1C               LDU   #$5028                  ; FC1C: CE 50 28      
                    LDD   #$05                    ; FC1F: CC 00 05      
LFC22               STD   ,U                      ; FC22: ED C4         
                    ADDD  #$01                    ; FC24: C3 00 01      
                    LEAU  $08,U                   ; FC27: 33 48         
                    CMPU  #$6000                  ; FC29: 11 83 60 00   
                    BCS   LFC22                   ; FC2D: 25 F3         
                    LDD   #$00                    ; FC2F: CC 00 00      
                    STD   $501E                   ; FC32: FD 50 1E        // Math zero constant
                    LDD   #$4000                  ; FC35: CC 40 00      
                    STD   $5020                   ; FC38: FD 50 20        // Math 1.000 constant
                    LDD   #$04                    ; FC3B: CC 00 04      
                    STD   $4701                   ; FC3E: FD 47 01        // Point BIC to $5020 MReg10
                    LDU   #$08                    ; FC41: CE 00 08      
LFC44               STA   $4640                   ; FC44: B7 46 40      
                    TFR   U,D                     ; FC47: 1F 30         
                    LSRA                          ; FC49: 44            
                    RORB                          ; FC4A: 56            
LFC4B               LDA   #$5B                    ; FC4B: 86 5B         
                    STA   $4700                   ; FC4D: B7 47 00        // Test routine $5B
                    DECB                          ; FC50: 5A            
                    BNE   LFC4B                   ; FC51: 26 F8         
                    LDA   #$5F                    ; FC53: 86 5F         
                    STA   $4700                   ; FC55: B7 47 00      
                    MUL                           ; FC58: 3D            
                    CMPU  $5000                   ; FC59: 11 B3 50 00     // Math result X
                    BEQ   LFC62                   ; FC5D: 27 03         
                    JMP   LFB73                   ; FC5F: 7E FB 73      

; ---------------------------------------------------------------------------
LFC62               TFR   U,D                     ; FC62: 1F 30         
                    LEAU  D,U                     ; FC64: 33 CB         
                    CMPU  #$0200                  ; FC66: 11 83 02 00   
                    BCS   LFC44                   ; FC6A: 25 D8         
                    LDD   #$00                    ; FC6C: CC 00 00      
; End of function sub_FC1C
                    JMP   LFB73                   ; FC6F: 7E FB 73      

; =============== S U B R O U T I N E =======================================
LFC72               LDD   #$1B2C                  ; FC72: CC 1B 2C      
                    STD   $5018                   ; FC75: FD 50 18        // XT
                    LDD   #$00                    ; FC78: CC 00 00      
                    STD   $501A                   ; FC7B: FD 50 1A        // YT
                    LDD   #$4000                  ; FC7E: CC 40 00      
                    STD   $501C                   ; FC81: FD 50 1C        // ZT
                    LDA   #$5D                    ; FC84: 86 5D         
                    STA   $4700                   ; FC86: B7 47 00      
                    MUL                           ; FC89: 3D            
                    LDD   #$196A                  ; FC8A: CC 19 6A      
                    STD   $5018                   ; FC8D: FD 50 18        // XT
                    LDD   #$00                    ; FC90: CC 00 00      
                    STD   $501A                   ; FC93: FD 50 1A        // YT
                    LDD   #$4000                  ; FC96: CC 40 00      
                    STD   $501C                   ; FC99: FD 50 1C        // ZT
                    LDA   #$5E                    ; FC9C: 86 5E         
                    STA   $4700                   ; FC9E: B7 47 00      
                    MUL                           ; FCA1: 3D            
                    LDD   $5000                   ; FCA2: FC 50 00        // Math result X
                    CMPD  #$3496                  ; FCA5: 10 83 34 96   
; End of function sub_FC72
                    JMP   LFB73                   ; FCA9: 7E FB 73      

; =============== S U B R O U T I N E =======================================
LFCAC               ASLB                          ; FCAC: 58            
                    ASLB                          ; FCAD: 58            
                    LDU   #word_FCD1              ; FCAE: CE FC D1      
                    LEAU  B,U                     ; FCB1: 33 C5         
                    LDD   ,U                      ; FCB3: EC C4         
                    STD   $5018                   ; FCB5: FD 50 18        // XT
                    LDD   $02,U                   ; FCB8: EC 42         
                    STD   $501A                   ; FCBA: FD 50 1A        // YT
                    LDD   $04,U                   ; FCBD: EC 44         
                    STD   $501C                   ; FCBF: FD 50 1C        // ZT
                    LDA   #$5D                    ; FCC2: 86 5D         
                    STA   $4700                   ; FCC4: B7 47 00      
                    MUL                           ; FCC7: 3D            
                    LDD   $5000                   ; FCC8: FC 50 00        // Math result X
                    CMPD  $06,U                   ; FCCB: 10 A3 46      
; End of function sub_FCAC
; ---------------------------------------------------------------------------
                    JMP   LFB73                   ; FCCE: 7E FB 73      

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_6532
Check_Test_Diag        LDA   $4300                   ; FD07: B6 43 00      
                    ANDA  #$10                    ; FD0A: 84 10         
                    LDB   $4320                   ; FD0C: F6 43 20      
                    ANDB  #$04                    ; FD0F: C4 04         
                    CMPD  #$00                    ; FD11: 10 83 00 00   
                    BEQ   LFD1A                   ; FD15: 27 03         
                    JMP   LF720                   ; FD17: 7E F7 20      

; ---------------------------------------------------------------------------
LFD1A               STA   $4640                   ; FD1A: B7 46 40        // Self test and Diag selected
                    LDA   #$FF                    ; FD1D: 86 FF         
                    STA   $4686                   ; FD1F: B7 46 86      
                    STA   $4683                   ; FD22: B7 46 83      
                    STA   $4682                   ; FD25: B7 46 82      
                    LDB   $4340                   ; FD28: F6 43 40        // Get diag test option
                    COMB                          ; FD2B: 53            
                    LDU   #Diag_Test_Tablefcb $E0, $10, $E1, $11, ; FD2C: CE FD 9A      
LFD2F               CMPB  ,U+                     ; FD2F: E1 C0         
                    BNE   LFD5A                   ; FD31: 26 27         
                    STA   $4620                   ; FD33: B7 46 20      
                    LDY   #$00                    ; FD36: 10 8E 00 00     // Point to Vector RAM start instruction
                    LDD   #$BFAE                  ; FD3A: CC BF AE        // Vector instruction JRSL 0x1FAE
                    STD   ,Y++                    ; FD3D: ED A1         
                    LDD   #$8040                  ; FD3F: CC 80 40        // Vector instruction CNTR 0x40
                    STD   ,Y++                    ; FD42: ED A1         
                    LDA   ,U                      ; FD44: A6 C4           // Get BCD number of diag test from table
                    LDS   #$4FFF                  ; FD46: 10 CE 4F FF     // Reset stack pointer
                    JSR   Display_Vect_BCD        ; FD4A: BD E7 90        // Display 2 digit BCD number in A
LFD4D               LDD   #$2020                  ; FD4D: CC 20 20        // Vector HALT
                    STD   ,Y++                    ; FD50: ED A1         
                    STD   ,Y++                    ; FD52: ED A1         
                    STA   $4600                   ; FD54: B7 46 00      
                    LDU   #LFDBC                  ; FD57: CE FD BC        // Bounds check
LFD5A               LEAU  $01,U                   ; FD5A: 33 41         
                    CMPU  #LFDBC                  ; FD5C: 11 83 FD BC   
                    BCS   LFD2F                   ; FD60: 25 CD         
                    LDB   $4340                   ; FD62: F6 43 40      
                    COMB                          ; FD65: 53            
                    TFR   B,A                     ; FD66: 1F 98         
                    ANDB  #$0F                    ; FD68: C4 0F         
                    ANDA  #$F8                    ; FD6A: 84 F8         
                    CMPA  #$C0                    ; FD6C: 81 C0         
                    BEQ   LFD72                   ; FD6E: 27 02         
                    CLR   <$D1                    ; FD70: 0F D1         
LFD72               LDX   #JMPTBL_Diag            ; FD72: 8E FD 85      
LFD75               CMPA  ,X+                     ; FD75: A1 80         
                    BNE   LFD7B                   ; FD77: 26 02         
                    JMP   [,X]                    ; FD79: 6E 94         

; ---------------------------------------------------------------------------
LFD7B               LEAX  $02,X                   ; FD7B: 30 02         
                    CMPX  #Diag_Test_Tablefcb $E0, $10, $E1, $11, ; FD7D: 8C FD 9A        // Bounds check
                    BCS   LFD75                   ; FD80: 25 F3         
; END OF FUNCTION CHUNK FOR sub_6532
; ---------------------------------------------------------------------------
                    JMP   LFF24                   ; FD82: 7E FF 24      

                    FCB   $80                     ; FD88:  ' ' 80 FD          SUBA  #$FD ; 
                    FCB   $F8                     ; FD97:  ' ' F8 FE A7       EORB  $FEA7 ; 
                    FCB   $F2                     ; FDB4:  ' ' F2 23 F3       SBCB  $23F3 ; 
                    FCB   $23                     ; FDB5:  '#' 23 F3          BLS   $FDAA ; 
                    FCB   $F3                     ; FDB6:  ' ' F3 24 F4       ADDD  $24F4 ; 
                    FCB   $24                     ; FDB7:  '$' 24 F4          BCC   $FDAD ; 
                    FCB   $F4                     ; FDB8:  ' ' F4 25 F8       ANDB  $25F8 ; 
                    FCB   $25                     ; FDB9:  '%' 25 F8          BCS   $FDB3 ; 
                    FCB   $F8                     ; FDBA:  ' ' F8 26 5D       EORB  $265D ; 
                    FCB   $26                     ; FDBB:  '&' 26 5D          BNE   $FE1A ; 
; =============== S U B R O U T I N E =======================================
LFDBC               TSTB                          ; FDBC: 5D            
                    BEQ   LFDC2                   ; FDBD: 27 03         
                    JMP   LFF24                   ; FDBF: 7E FF 24      

; ---------------------------------------------------------------------------
LFDC2               TFR   S,D                     ; FDC2: 1F 40         
                    ANDA  #$40                    ; FDC4: 84 40         
                    ANDB  #$01                    ; FDC6: C4 01         
                    CMPD  #$00                    ; FDC8: 10 83 00 00   
                    BNE   LFDD4                   ; FDCC: 26 06         
                    LDU   #$4686                  ; FDCE: CE 46 86      
                    JMP   LFEFF                   ; FDD1: 7E FE FF      

; ---------------------------------------------------------------------------
; End of function sub_FDBC
LFDD4               JMP   Check_Test_Diag         ; FDD4: 7E FD 07      

; =============== S U B R O U T I N E =======================================
LFDD7               TSTB                          ; FDD7: 5D            
                    BEQ   LFDDD                   ; FDD8: 27 03         
                    JMP   LFF24                   ; FDDA: 7E FF 24      

; ---------------------------------------------------------------------------
LFDDD               TFR   S,D                     ; FDDD: 1F 40         
                    ANDA  #$81                    ; FDDF: 84 81         
                    ANDB  #$F8                    ; FDE1: C4 F8         
                    CMPD  #$00                    ; FDE3: 10 83 00 00   
                    BNE   LFDEF                   ; FDE7: 26 06         
                    LDU   #$4683                  ; FDE9: CE 46 83      
                    JMP   LFEFF                   ; FDEC: 7E FE FF      

; ---------------------------------------------------------------------------
; End of function sub_FDD7
LFDEF               JMP   Check_Test_Diag         ; FDEF: 7E FD 07      

; =============== S U B R O U T I N E =======================================
LFDF2               CMPB  #$06                    ; FDF2: C1 06         
                    BNE   LFE0E                   ; FDF4: 26 18         
                    TST   <$D1                    ; FDF6: 0D D1         
                    BNE   LFE0C                   ; FDF8: 26 12         
                    LDX   #$0800                  ; FDFA: 8E 08 00      
                    LDA   #$00                    ; FDFD: 86 00         
LFDFF               STA   ,X+                     ; FDFF: A7 80         
                    ADDA  #$05                    ; FE01: 8B 05         
                    CMPX  #$1000                  ; FE03: 8C 10 00      
                    BCS   LFDFF                   ; FE06: 25 F7         
                    LDA   #$FF                    ; FE08: 86 FF         
                    STA   <$D1                    ; FE0A: 97 D1         
LFE0C               BRA   LFE34                   ; FE0C: 20 26         

; ---------------------------------------------------------------------------
LFE0E               CLR   <$D1                    ; FE0E: 0F D1         
                    STA   $4620                   ; FE10: B7 46 20      
                    LDX   #Diag_Vect_Data         ; FE13: 8E FE 37        // Point to diag vector test instructions
                    ASLB                          ; FE16: 58            
                    ASLB                          ; FE17: 58            
                    ABX                           ; FE18: 3A            
                    CMPX  #LFE4F                  ; FE19: 8C FE 4F      
                    BCS   LFE21                   ; FE1C: 25 03         
                    JMP   LFF24                   ; FE1E: 7E FF 24      

; ---------------------------------------------------------------------------
LFE21               LDD   ,X                      ; FE21: EC 84         
                    STD   $00                     ; FE23: FD 00 00      
                    LDD   $02,X                   ; FE26: EC 02         
                    STD   $02                     ; FE28: FD 00 02      
                    LDD   #$2020                  ; FE2B: CC 20 20      
                    STD   $04                     ; FE2E: FD 00 04      
                    STA   $4600                   ; FE31: B7 46 00        // Run diag vector test
; End of function sub_FDF2
; ---------------------------------------------------------------------------
LFE34               JMP   Check_Test_Diag         ; FE34: 7E FD 07      

; =============== S U B R O U T I N E =======================================
LFE4F               LDX   #off_FCF1               ; FE4F: 8E FC F1      
                    ASLB                          ; FE52: 58            
                    ABX                           ; FE53: 3A            
                    LDY   #$00                    ; FE54: 10 8E 00 00   
LFE58               CMPX  #Check_Test_Diag        ; FE58: 8C FD 07      
                    BCS   LFE60                   ; FE5B: 25 03         
                    JMP   LFF24                   ; FE5D: 7E FF 24      

; ---------------------------------------------------------------------------
LFE60               STA   $4640                   ; FE60: B7 46 40      
                    LEAY  $01,Y                   ; FE63: 31 21         
                    CMPY  #$0200                  ; FE65: 10 8C 02 00   
                    BCS   LFE71                   ; FE69: 25 06         
                    LDS   #Check_Test_Diag        ; FE6B: 10 CE FD 07   
                    BRA   LFE75                   ; FE6F: 20 04         

; ---------------------------------------------------------------------------
LFE71               LDS   #LFE58                  ; FE71: 10 CE FE 58   
LFE75               TFR   X,D                     ; FE75: 1F 10         
                    SUBD  #off_FCFF               ; FE77: 83 FC FF      
; End of function sub_FE4F
                    JMP   [,X]                    ; FE7A: 6E 94         

; =============== S U B R O U T I N E =======================================
; FUNCTION CHUNK AT ROM:FB38 SIZE 00000013 BYTES
LFE7C               LDX   #word_FB4B              ; FE7C: 8E FB 4B      
                    ASLB                          ; FE7F: 58            
                    ASLB                          ; FE80: 58            
                    ASLB                          ; FE81: 58            
                    ABX                           ; FE82: 3A            
                    LDY   #$00                    ; FE83: 10 8E 00 00   
LFE87               CMPX  #LFB73                  ; FE87: 8C FB 73      
                    BCS   LFE8F                   ; FE8A: 25 03         
                    JMP   LFF24                   ; FE8C: 7E FF 24      

; ---------------------------------------------------------------------------
LFE8F               STA   $4640                   ; FE8F: B7 46 40      
                    LEAY  $01,Y                   ; FE92: 31 21         
                    CMPY  #$0200                  ; FE94: 10 8C 02 00   
                    BCS   LFEA0                   ; FE98: 25 06         
LFE9A               LDS   #Check_Test_Diag        ; FE9A: 10 CE FD 07   
                    BRA   LFEA4                   ; FE9E: 20 04         

; ---------------------------------------------------------------------------
LFEA0               LDS   #LFE87                  ; FEA0: 10 CE FE 87   
; End of function sub_FE7C
LFEA4               JMP   LFB38                   ; FEA4: 7E FB 38      

; =============== S U B R O U T I N E =======================================
LFEA7               ANDB  #$07                    ; FEA7: C4 07         
                    BEQ   LFEAE                   ; FEA9: 27 03         
                    JMP   LFF24                   ; FEAB: 7E FF 24      

; ---------------------------------------------------------------------------
LFEAE               LDA   #$01                    ; FEAE: 86 01         
LFEB0               STB   $46E0                   ; FEB0: F7 46 E0      
                    TFR   X,U                     ; FEB3: 1F 13         
                    LDB   $4400                   ; FEB5: F6 44 00      
                    LDB   $4401                   ; FEB8: F6 44 01      
                    ANDB  #$40                    ; FEBB: C4 40         
                    BNE   LFF03                   ; FEBD: 26 44         
                    LDB   #$80                    ; FEBF: C6 80         
LFEC1               DECB                          ; FEC1: 5A            
                    BMI   LFF03                   ; FEC2: 2B 3F         
                    STA   $4640                   ; FEC4: B7 46 40      
                    TST   $4401                   ; FEC7: 7D 44 01      
                    BMI   LFEC1                   ; FECA: 2B F5         
                    STA   $4400                   ; FECC: B7 44 00      
                    LDB   $4401                   ; FECF: F6 44 01      
                    BPL   LFF03                   ; FED2: 2A 2F         
                    LDX   #$0100                  ; FED4: 8E 01 00      
LFED7               LEAX  -1,X                    ; FED7: 30 1F         
                    BEQ   LFF03                   ; FED9: 27 28         
                    STA   $4640                   ; FEDB: B7 46 40      
                    LDB   $4401                   ; FEDE: F6 44 01      
                    ANDB  #$40                    ; FEE1: C4 40         
                    BEQ   LFED7                   ; FEE3: 27 F2         
                    TST   $4401                   ; FEE5: 7D 44 01      
                    BMI   LFF03                   ; FEE8: 2B 19         
                    CMPA  $4400                   ; FEEA: B1 44 00      
                    BNE   LFF03                   ; FEED: 26 14         
                    LDB   $4401                   ; FEEF: F6 44 01      
                    ANDB  #$40                    ; FEF2: C4 40         
                    BMI   LFF03                   ; FEF4: 2B 0D         
                    ASLA                          ; FEF6: 48            
                    BCC   LFEB0                   ; FEF7: 24 B7         
                    LDU   #$4682                  ; FEF9: CE 46 82      
                    JMP   LFEFF                   ; FEFC: 7E FE FF      

LFEFF               LDA   #$00                    ; FEFF: 86 00         
                    STA   ,U                      ; FF01: A7 C4         
LFF03               LDX   #$00                    ; FF03: 8E 00 00      
LFF06               STA   $4640                   ; FF06: B7 46 40      
                    LEAX  $01,X                   ; FF09: 30 01         
                    CMPX  #$AC55                  ; FF0B: 8C AC 55      
                    BCS   LFF06                   ; FF0E: 25 F6         
                    LDA   #$FF                    ; FF10: 86 FF         
                    STA   ,U                      ; FF12: A7 C4         
                    LDX   #$00                    ; FF14: 8E 00 00      
LFF17               STA   $4640                   ; FF17: B7 46 40      
                    LEAX  $01,X                   ; FF1A: 30 01         
                    CMPX  #$AC55                  ; FF1C: 8C AC 55      
                    BCS   LFF17                   ; FF1F: 25 F6         
; End of function sub_FEA7
                    JMP   Check_Test_Diag         ; FF21: 7E FD 07      

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_6532
LFF24               LDA   #$00                    ; FF24: 86 00         
                    STA   $4686                   ; FF26: B7 46 86      
                    STA   $4683                   ; FF29: B7 46 83      
                    STA   $4682                   ; FF2C: B7 46 82      
                    LDX   #$00                    ; FF2F: 8E 00 00      
LFF32               STA   $4640                   ; FF32: B7 46 40      
                    LEAX  $01,X                   ; FF35: 30 01         
                    CMPX  #$5600                  ; FF37: 8C 56 00      
                    BCS   LFF32                   ; FF3A: 25 F6         
                    LDA   #$FF                    ; FF3C: 86 FF         
                    STA   $4686                   ; FF3E: B7 46 86      
                    STA   $4683                   ; FF41: B7 46 83      
                    STA   $4682                   ; FF44: B7 46 82      
                    LDX   #$00                    ; FF47: 8E 00 00      
LFF4A               STA   $4640                   ; FF4A: B7 46 40      
                    LEAX  $01,X                   ; FF4D: 30 01         
                    CMPX  #$5600                  ; FF4F: 8C 56 00      
                    BCS   LFF4A                   ; FF52: 25 F6         
; END OF FUNCTION CHUNK FOR sub_6532
; ---------------------------------------------------------------------------
                    JMP   Check_Test_Diag         ; FF54: 7E FD 07      

; end of ROM
