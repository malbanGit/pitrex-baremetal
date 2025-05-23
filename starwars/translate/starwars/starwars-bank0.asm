                    ORG   $6000
; ===========================================================================
; Segment type: Pure code
; =============== S U B R O U T I N E =======================================
L6005               LDX   #$00                    ; 6005: 8E 00 00      
                    LDD   #$F800                  ; 6008: CC F8 00      
                    LDA   #$48                    ; 600B: 86 48         
                    TFR   A,DP                    ; 600D: 1F 8B         
                    LSR   <$3D                    ; 600F: 04 3D         
                    BCC   L6005                   ; 6011: 24 F2         
                    CMPS  #$4FFF                  ; 6013: 11 8C 4F FF   
                    BEQ   L601A                   ; 6017: 27 01         
                    RTS                           ; 6019: 39            

; ---------------------------------------------------------------------------
L601A               LDA   <$28                    ; 601A: 96 28         
                    BNE   L602F                   ; 601C: 26 11         
                    LDA   $4824                   ; 601E: B6 48 24        // Opt0_Shadow
                    ANDA  #$80                    ; 6021: 84 80         
                    BNE   L602F                   ; 6023: 26 0A           // Check DIP switch for freeze mode
                    LDA   <$31                    ; 6025: 96 31         
                    BITA  #$80                    ; 6027: 85 80         
                    BEQ   L6005                   ; 6029: 27 DA         
                    ANDA  #$7F                    ; 602B: 84 7F         
                    STA   <$31                    ; 602D: 97 31         
L602F               JSR   L60BE                   ; 602F: BD 60 BE      
L6032               LDA   <$3F                    ; 6032: 96 3F           // Vector pointer state
                    BMI   L6032                   ; 6034: 2B FC         
L6036               LDA   <$41                    ; 6036: 96 41           // Game mode/screen state
                    CMPA  #$3D                    ; 6038: 81 3D           // Check game state for out of bounds
L603A               BCC   L603A                   ; 603A: 24 FE         
                    ASLA                          ; 603C: 48            
                    LDX   #$6044                  ; 603D: 8E 60 44      
                    JSR   [A,X]                   ; 6040: AD 96         
; End of function sub_6005
; ---------------------------------------------------------------------------
L6042               BRA   L6005                   ; 6042: 20 C1         

; =============== S U B R O U T I N E =======================================
L60BE               INC   <$43                    ; 60BE: 0C 43           // Game over/insert coins timer
                    BNE   L60CA                   ; 60C0: 26 08         
                    INC   <$42                    ; 60C2: 0C 42         
                    BNE   L60CA                   ; 60C4: 26 04         
                    LDA   #$80                    ; 60C6: 86 80         
                    STA   <$42                    ; 60C8: 97 42         
L60CA               LDA   $4814                   ; 60CA: B6 48 14        // Credits
                    BNE   L60D4                   ; 60CD: 26 05         
                    STA   $4B31                   ; 60CF: B7 4B 31      
                    BRA   L60F4                   ; 60D2: 20 20         

; ---------------------------------------------------------------------------
L60D4               LDA   $4B31                   ; 60D4: B6 4B 31      
                    BNE   L60E4                   ; 60D7: 26 0B         
                    JSR   $BD12                   ; 60D9: BD BD 12      
L60DC               LDA   $4814                   ; 60DC: B6 48 14        // Credits
                    STA   $4B31                   ; 60DF: B7 4B 31      
                    BRA   L60F4                   ; 60E2: 20 10         

; ---------------------------------------------------------------------------
L60E4               LDA   $4B31                   ; 60E4: B6 4B 31      
                    CMPA  $4814                   ; 60E7: B1 48 14        // Credits
                    BCC   L60F4                   ; 60EA: 24 08         
                    JSR   $BD03                   ; 60EC: BD BD 03      
; ---------------------------------------------------------------------------
L60EF               LDA   #$FF                    ; 60EF: 86 FF         
                    STA   $4B31                   ; 60F1: B7 4B 31      
L60F4               JSR   L70DB                   ; 60F4: BD 70 DB      
L60F7               LDA   <$AB                    ; 60F7: 96 AB         
                    STA   <$AA                    ; 60F9: 97 AA         
                    LDA   <$21                    ; 60FB: 96 21         
                    ANDA  #$30                    ; 60FD: 84 30         
                    PSHS  A                       ; 60FF: 34 02         
                    LDA   <$1E                    ; 6101: 96 1E         
                    ANDA  #$CF                    ; 6103: 84 CF         
                    ORA   ,S+                     ; 6105: AA E0         
                    ANDA  #$F4                    ; 6107: 84 F4         
                    STA   <$AB                    ; 6109: 97 AB         
                    EORA  <$AA                    ; 610B: 98 AA         
                    ANDA  <$AA                    ; 610D: 94 AA         
                    STA   <$AC                    ; 610F: 97 AC         
; End of function sub_60BE
                    RTS                           ; 6111: 39            

; =============== S U B R O U T I N E =======================================
; Insert vector data for four blue dots in screen corners
L6112               LDA   <$3F                    ; 6112: 96 3F           // Vector pointer state
                    LDB   #$70                    ; 6114: C6 70         
                    TFR   D,Y                     ; 6116: 1F 02           // Set new vector pointer value
                    LDD   #$B99E                  ; 6118: CC B9 9E        // Vector JRSL to draw four blue dots in screen corners
                    STD   ,Y++                    ; 611B: ED A1         
; End of function sub_6112
                    RTS                           ; 611D: 39            

; =============== S U B R O U T I N E =======================================
; Copies Star Wars logo vector data to vector RAM
L611E               LDX   #$CEDE                  ; 611E: 8E CE DE        // Copies vector data. Called at start of attract screen 2
                    LDU   #$2800                  ; 6121: CE 28 00      
L6124               LDD   ,X++                    ; 6124: EC 81         
                    STD   ,U++                    ; 6126: ED C1         
                    CMPU  #$3000                  ; 6128: 11 83 30 00   
                    BCS   L6124                   ; 612C: 25 F6         
; End of function sub_611E
                    RTS                           ; 612E: 39            

; =============== S U B R O U T I N E =======================================
; Vector instructions end
L612F               LDD   #$8040                  ; 612F: CC 80 40      
                    STD   ,Y++                    ; 6132: ED A1         
                    LDD   #$2020                  ; 6134: CC 20 20        // Vector HALT
                    STD   ,Y++                    ; 6137: ED A1         
                    STD   ,Y+                     ; 6139: ED A0         
                    TFR   Y,D                     ; 613B: 1F 20         
                    SUBA  <$3F                    ; 613D: 90 3F           // Vector pointer state
                    SUBA  #$14                    ; 613F: 80 14         
                    BCS   L6155                   ; 6141: 25 12         
                    JSR   L611E                   ; 6143: BD 61 1E        // Copies Star Wars logo vector data to vector RAM
L6146               LDD   #$2020                  ; 6146: CC 20 20      
                    STD   $13FE                   ; 6149: FD 13 FE      
                    STD   $13FC                   ; 614C: FD 13 FC      
                    STD   $27FE                   ; 614F: FD 27 FE      
                    STD   $27FC                   ; 6152: FD 27 FC      
L6155               LDA   #$FF                    ; 6155: 86 FF         
                    STA   <$3F                    ; 6157: 97 3F           // Vector pointer state
; End of function sub_612F
                    RTS                           ; 6159: 39            

; =============== S U B R O U T I N E =======================================
; More stars/ties init stuff
L615A               JSR   L6161                   ; 615A: BD 61 61        // Initialise tie fighters and fireballs
L615D               JSR   L7A48                   ; 615D: BD 7A 48      
; End of function sub_615A
L6160               RTS                           ; 6160: 39            

; =============== S U B R O U T I N E =======================================
; Initialise tie fighters and fireballs
L6161               LDA   $4703                   ; 6161: B6 47 03      
                    SUBA  $4703                   ; 6164: B0 47 03      
                    BNE   L6171                   ; 6167: 26 08         
                    STA   $4685                   ; 6169: B7 46 85      
                    LDA   #$80                    ; 616C: 86 80         
                    STA   $4685                   ; 616E: B7 46 85      
L6171               LDA   #$80                    ; 6171: 86 80         
                    STA   <$83                    ; 6173: 97 83           // Star intensity
                    LDX   #$4900                  ; 6175: 8E 49 00        // 3x Tie fighter data structure ($19 bytes per Tie)
                    LDU   #$50F0                  ; 6178: CE 50 F0        // 3x Tie fighter math data structure ($20 bytes per Tie)
                    LDB   #$1C                    ; 617B: C6 1C         
L617D               STU   ,X                      ; 617D: EF 84         
                    STB   $02,X                   ; 617F: E7 02         
                    LEAU  $20,U                   ; 6181: 33 C8 20      
                    ADDB  #$04                    ; 6184: CB 04         
                    LEAX  $19,X                   ; 6186: 30 88 19      
                    CMPX  #$494B                  ; 6189: 8C 49 4B        // 3x Tie fighter data structure ($19 bytes per Tie)
                    BCS   L617D                   ; 618C: 25 EF         
                    LDX   #$494B                  ; 618E: 8E 49 4B        // 6x Fireball data structure 2 ($6 bytes per fireball)
                    LDU   #$5160                  ; 6191: CE 51 60        // 6x Fireball math data structure 2 ($8 bytes per Tie)
                    LDB   #$2C                    ; 6194: C6 2C         
L6196               STU   ,X                      ; 6196: EF 84         
                    STB   $02,X                   ; 6198: E7 02         
                    CLR   $03,X                   ; 619A: 6F 03         
                    LEAU  $08,U                   ; 619C: 33 48         
                    ADDB  #$01                    ; 619E: CB 01         
                    LEAX  $06,X                   ; 61A0: 30 06         
                    CMPX  #$496F                  ; 61A2: 8C 49 6F        // 6x Fireball data structure 2 ($6 bytes per fireball)
                    BCS   L6196                   ; 61A5: 25 EF         
                    JSR   $8ED6                   ; 61A7: BD 8E D6      
L61AA               LDB   #$04                    ; 61AA: C6 04         
                    JSR   $CCCC                   ; 61AC: BD CC CC        // Copy XYZ data to math RAM
L61AF               LDB   #$07                    ; 61AF: C6 07         
                    JSR   $CCCC                   ; 61B1: BD CC CC        // Copy XYZ data to math RAM
; End of function sub_6161
L61B4               RTS                           ; 61B4: 39            

; =============== S U B R O U T I N E =======================================
; Set up math constants
L61B5               LDD   #$00                    ; 61B5: CC 00 00      
                    STD   $501E                   ; 61B8: FD 50 1E        // Math zero constant
                    LDD   #$4000                  ; 61BB: CC 40 00      
                    STD   $5020                   ; 61BE: FD 50 20        // Math 1.000 constant
                    LDD   #$E000                  ; 61C1: CC E0 00      
                    STD   $5026                   ; 61C4: FD 50 26        // Constant -0.5 ?
                    LDD   #$80                    ; 61C7: CC 00 80      
                    STD   $506A                   ; 61CA: FD 50 6A        // Math Constant $0080
                    LDD   #$40                    ; 61CD: CC 00 40      
                    STD   $5068                   ; 61D0: FD 50 68        // Math Constant $0040
                    LDD   #$021F                  ; 61D3: CC 02 1F      
                    STD   $5022                   ; 61D6: FD 50 22        // Sine for rotation
                    LDD   #$3FF7                  ; 61D9: CC 3F F7      
                    STD   $5024                   ; 61DC: FD 50 24        // Cosine for rotation
                    LDD   #$4000                  ; 61DF: CC 40 00      
                    STD   $506C                   ; 61E2: FD 50 6C        // Math 1.000 constant
                    LDD   #$0200                  ; 61E5: CC 02 00      
                    STD   $4706                   ; 61E8: FD 47 06      
; End of function sub_61B5
                    RTS                           ; 61EB: 39            

; =============== S U B R O U T I N E =======================================
; Init stars math data
L61EC               LDA   #$80                    ; 61EC: 86 80         
                    STA   <$83                    ; 61EE: 97 83           // Star intensity
                    LDX   #$5C60                  ; 61F0: 8E 5C 60      
L61F3               LDA   $4703                   ; 61F3: B6 47 03      
                    LDB   $4703                   ; 61F6: F6 47 03      
                    STD   ,X                      ; 61F9: ED 84         
                    MUL                           ; 61FB: 3D            
                    LDA   $4703                   ; 61FC: B6 47 03      
                    STD   $02,X                   ; 61FF: ED 02         
                    MUL                           ; 6201: 3D            
                    LDA   $4703                   ; 6202: B6 47 03      
                    STD   $04,X                   ; 6205: ED 04         
                    LEAX  $08,X                   ; 6207: 30 08         
                    CMPX  #$5DF0                  ; 6209: 8C 5D F0      
                    BCS   L61F3                   ; 620C: 25 E5         
; End of function sub_61EC
                    RTS                           ; 620E: 39            

; =============== S U B R O U T I N E =======================================
; Init towers surface dots
L620F               LDX   #$5C60                  ; 620F: 8E 5C 60      
L6212               LDA   $4703                   ; 6212: B6 47 03      
                    LDB   $4703                   ; 6215: F6 47 03      
                    STD   ,X                      ; 6218: ED 84         
                    MUL                           ; 621A: 3D            
                    LDA   $4703                   ; 621B: B6 47 03      
                    STD   $02,X                   ; 621E: ED 02         
                    LDD   #$00                    ; 6220: CC 00 00      
                    STD   $04,X                   ; 6223: ED 04         
                    LEAX  $08,X                   ; 6225: 30 08         
                    CMPX  #$5DF0                  ; 6227: 8C 5D F0      
                    BCS   L6212                   ; 622A: 25 E6         
; End of function sub_620F
                    RTS                           ; 622C: 39            

; =============== S U B R O U T I N E =======================================
; Check joystick X to show high scores if moved
L622D               LDB   <$7D                    ; 622D: D6 7D           // Joystick X
                    CMPB  #$A0                    ; 622F: C1 A0         
                    BGT   L623E                   ; 6231: 2E 0B         
                    LDA   #$08                    ; 6233: 86 08         
                    CMPA  <$41                    ; 6235: 91 41           // Game mode/screen state
                    BEQ   L623C                   ; 6237: 27 03         
                    DECA                          ; 6239: 4A            
                    STA   <$41                    ; 623A: 97 41           // Game mode/screen state
L623C               BRA   L624B                   ; 623C: 20 0D         

; ---------------------------------------------------------------------------
L623E               CMPB  #$60                    ; 623E: C1 60         
                    BLT   L624B                   ; 6240: 2D 09         
                    LDA   #$0C                    ; 6242: 86 0C         
                    CMPA  <$41                    ; 6244: 91 41           // Game mode/screen state
                    BEQ   L624B                   ; 6246: 27 03         
                    DECA                          ; 6248: 4A            
                    STA   <$41                    ; 6249: 97 41           // Game mode/screen state
L624B               LDA   $4591                   ; 624B: B6 45 91      
                    ANDA  #$03                    ; 624E: 84 03         
                    BNE   L6257                   ; 6250: 26 05         
                    LDA   #$01                    ; 6252: 86 01         
                    STA   $4814                   ; 6254: B7 48 14        // Credits
L6257               LDA   $4814                   ; 6257: B6 48 14        // Credits
                    BEQ   L6269                   ; 625A: 27 0D         
                    LDA   <$AC                    ; 625C: 96 AC         
                    ANDA  #$F0                    ; 625E: 84 F0         
                    BEQ   L6269                   ; 6260: 27 07         
                    LDA   #$19                    ; 6262: 86 19         
                    STA   <$41                    ; 6264: 97 41           // Game mode/screen state
                    DEC   $4814                   ; 6266: 7A 48 14        // Credits
L6269               LDA   $481E                   ; 6269: B6 48 1E      
                    ANDA  #$10                    ; 626C: 84 10         
                    BNE   locret_6274             ; 626E: 26 04         
                    LDA   #$01                    ; 6270: 86 01         
                    STA   <$41                    ; 6272: 97 41           // Game mode/screen state
; End of function sub_622D
locret_6274         RTS                           ; 6274: 39            

; =============== S U B R O U T I N E =======================================
; Game initialisation
L6275               ORCC  #$10                    ; 6275: 1A 10           // Disable interrupts
                    LDX   #$4534                  ; 6277: 8E 45 34      
                    JSR   $C6D4                   ; 627A: BD C6 D4        // Read NOVRAM
L627D               LDX   #$4AFA                  ; 627D: 8E 4A FA      
                    JSR   L62D5                   ; 6280: BD 62 D5      
L6283               STA   $4866                   ; 6283: B7 48 66        // Joystick pitch data structure
                    LDX   #$4AFB                  ; 6286: 8E 4A FB      
                    JSR   L62D5                   ; 6289: BD 62 D5      
L628C               STA   $4868                   ; 628C: B7 48 68      
                    LDX   #$4AFC                  ; 628F: 8E 4A FC      
                    JSR   L62D5                   ; 6292: BD 62 D5      
L6295               STA   $486F                   ; 6295: B7 48 6F        // Joystick roll data structure
                    LDX   #$4AFD                  ; 6298: 8E 4A FD      
                    JSR   L62D5                   ; 629B: BD 62 D5      
L629E               STA   $4871                   ; 629E: B7 48 71      
                    ANDCC #$EF                    ; 62A1: 1C EF           // n      ; Enable interrupts
                    LDA   #$0B                    ; 62A3: 86 0B         
                    STA   <$41                    ; 62A5: 97 41           // Game mode/screen state
                    LDA   #$FF                    ; 62A7: 86 FF         
                    STA   $4B34                   ; 62A9: B7 4B 34      
                    JSR   L61B5                   ; 62AC: BD 61 B5        // Set up math constants
L62AF               JSR   L615A                   ; 62AF: BD 61 5A        // More stars/ties init stuff
L62B2               JSR   L611E                   ; 62B2: BD 61 1E        // Copies Star Wars logo vector data to vector RAM
L62B5               JSR   L61EC                   ; 62B5: BD 61 EC        // Init stars math data
L62B8               JSR   $D91A                   ; 62B8: BD D9 1A      
L62BB               LDA   #$00                    ; 62BB: 86 00         
                    STA   <$5C                    ; 62BD: 97 5C           // Score millions
                    STA   <$5D                    ; 62BF: 97 5D           // Score hundred thousands
                    STA   <$5E                    ; 62C1: 97 5E           // Score thousands
                    STA   <$5F                    ; 62C3: 97 5F           // Score
                    STA   <$8B                    ; 62C5: 97 8B         
                    STA   <$8C                    ; 62C7: 97 8C           // Sheild being depleted
                    JSR   $CC18                   ; 62C9: BD CC 18      
L62CC               LDA   #$FF                    ; 62CC: 86 FF         
                    STA   $4AEC                   ; 62CE: B7 4A EC      
                    STA   $4818                   ; 62D1: B7 48 18      
; End of function sub_6275
                    RTS                           ; 62D4: 39            

; =============== S U B R O U T I N E =======================================
L62D5               LDA   #$40                    ; 62D5: 86 40         
                    CMPA  ,X                      ; 62D7: A1 84         
                    BLS   locret_62E3             ; 62D9: 23 08         
                    SUBA  ,X                      ; 62DB: A0 84         
                    LSRA                          ; 62DD: 44            
                    LSRA                          ; 62DE: 44            
                    LSRA                          ; 62DF: 44            
                    INCA                          ; 62E0: 4C            
                    ADDA  ,X                      ; 62E1: AB 84         
; End of function sub_62D5
locret_62E3         RTS                           ; 62E3: 39            

; =============== S U B R O U T I N E =======================================
; Called once before attract screen 3
L62E4               LDD   #$00                    ; 62E4: CC 00 00      
                    STD   $4B0C                   ; 62E7: FD 4B 0C        // Attract text position for scrolling
                    LDD   #$0200                  ; 62EA: CC 02 00      
                    STD   $4B0E                   ; 62ED: FD 4B 0E        // Attract screen/game phase  timer
                    LDA   #$0F                    ; 62F0: 86 0F         
                    STA   $4ADB                   ; 62F2: B7 4A DB      
                    LDA   #$1F                    ; 62F5: 86 1F         
                    STA   $4ADC                   ; 62F7: B7 4A DC      
                    LDD   #$6480                  ; 62FA: CC 64 80      
                    STD   $4B10                   ; 62FD: FD 4B 10        // Attract text colour/intensity for fading
                    JSR   $D91A                   ; 6300: BD D9 1A      
L6303               INC   <$41                    ; 6303: 0C 41           // Game mode/screen state
; End of function sub_62E4
                    RTS                           ; 6305: 39            

; =============== S U B R O U T I N E =======================================
; Attract screen 3
L6306               JSR   L6112                   ; 6306: BD 61 12        // Insert vector data for four blue dots in screen corners
L6309               JSR   L761D                   ; 6309: BD 76 1D        // Display score
L630C               JSR   L63D5                   ; 630C: BD 63 D5        // Check credits status
L630F               JSR   L6368                   ; 630F: BD 63 68      
L6312               JSR   $CD80                   ; 6312: BD CD 80        // Starfield
L6315               JSR   L612F                   ; 6315: BD 61 2F        // Vector instructions end
L6318               JSR   L6DB6                   ; 6318: BD 6D B6        // Attract screen 3 stars YT move
L631B               LDD   $4B0E                   ; 631B: FC 4B 0E        // Attract screen/game phase  timer
                    BPL   L6322                   ; 631E: 2A 02         
                    INC   <$41                    ; 6320: 0C 41           // Game mode/screen state
L6322               JSR   L622D                   ; 6322: BD 62 2D        // Check joystick X to show high scores if moved
; End of function sub_6306
L6325               RTS                           ; 6325: 39            

; =============== S U B R O U T I N E =======================================
L6326               LDD   #$03C0                  ; 6326: CC 03 C0        // Called once before attract screen 4
                    STD   $4B0C                   ; 6329: FD 4B 0C        // Attract text position for scrolling
                    LDD   #$0200                  ; 632C: CC 02 00      
                    STD   $4B0E                   ; 632F: FD 4B 0E        // Attract screen/game phase  timer
                    LDA   #$23                    ; 6332: 86 23         
                    STA   $4ADB                   ; 6334: B7 4A DB      
                    LDA   #$2C                    ; 6337: 86 2C         
                    STA   $4ADC                   ; 6339: B7 4A DC      
                    LDD   #$6580                  ; 633C: CC 65 80      
                    STD   $4B10                   ; 633F: FD 4B 10        // Attract text colour/intensity for fading
                    JSR   $D91A                   ; 6342: BD D9 1A      
L6345               INC   <$41                    ; 6345: 0C 41           // Game mode/screen state
; End of function sub_6326
                    RTS                           ; 6347: 39            

; =============== S U B R O U T I N E =======================================
; Attract screen 4
L6348               JSR   L6112                   ; 6348: BD 61 12        // Insert vector data for four blue dots in screen corners
L634B               JSR   L761D                   ; 634B: BD 76 1D        // Display score
L634E               JSR   L63D5                   ; 634E: BD 63 D5        // Check credits status
L6351               JSR   L6368                   ; 6351: BD 63 68      
L6354               JSR   $CD80                   ; 6354: BD CD 80        // Starfield
L6357               JSR   L612F                   ; 6357: BD 61 2F        // Vector instructions end
L635A               JSR   L6DC0                   ; 635A: BD 6D C0        // Attract screen 4 stars ZT move
L635D               LDD   $4B0E                   ; 635D: FC 4B 0E        // Attract screen/game phase  timer
                    BPL   L6364                   ; 6360: 2A 02         
                    INC   <$41                    ; 6362: 0C 41           // Game mode/screen state
L6364               JSR   L622D                   ; 6364: BD 62 2D        // Check joystick X to show high scores if moved
; End of function sub_6348
L6367               RTS                           ; 6367: 39            

; =============== S U B R O U T I N E =======================================
L6368               LDD   $4B0E                   ; 6368: FC 4B 0E        // Attract screen/game phase  timer
                    SUBD  #$01                    ; 636B: 83 00 01      
                    STD   $4B0E                   ; 636E: FD 4B 0E        // Attract screen/game phase  timer
                    ANDB  #$07                    ; 6371: C4 07         
                    BNE   L639D                   ; 6373: 26 28         
                    LDA   $4ADB                   ; 6375: B6 4A DB      
                    BMI   L637D                   ; 6378: 2B 03         
                    JSR   $D8DF                   ; 637A: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
L637D               LDA   $4ADB                   ; 637D: B6 4A DB      
                    CMPA  #$12                    ; 6380: 81 12         
                    BNE   L638E                   ; 6382: 26 0A         
                    LDA   $4593                   ; 6384: B6 45 93      
                    ANDA  #$03                    ; 6387: 84 03         
                    ADDA  #$1F                    ; 6389: 8B 1F         
                    JSR   $D8DF                   ; 638B: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
L638E               LDA   $4ADB                   ; 638E: B6 4A DB      
                    ADDA  #$01                    ; 6391: 8B 01         
                    CMPA  $4ADC                   ; 6393: B1 4A DC      
                    BCS   L639A                   ; 6396: 25 02         
                    LDA   #$80                    ; 6398: 86 80         
L639A               STA   $4ADB                   ; 639A: B7 4A DB      
L639D               LDD   $4B0C                   ; 639D: FC 4B 0C        // Attract text position for scrolling
                    SUBD  #$08                    ; 63A0: 83 00 08      
                    BPL   L63A8                   ; 63A3: 2A 03         
                    LDD   #$00                    ; 63A5: CC 00 00      
L63A8               STD   $4B0C                   ; 63A8: FD 4B 0C        // Attract text position for scrolling
                    LDD   $4B0E                   ; 63AB: FC 4B 0E        // Attract screen/game phase  timer
                    CMPD  #$C0                    ; 63AE: 10 83 00 C0   
                    BCC   L63C6                   ; 63B2: 24 12         
                    LDB   $4B11                   ; 63B4: F6 4B 11        // Attract text intensity
                    SUBB  #$01                    ; 63B7: C0 01         
                    CMPB  #$10                    ; 63B9: C1 10         
                    BCC   L63C3                   ; 63BB: 24 06         
                    LDD   #$00                    ; 63BD: CC 00 00      
                    STD   $4B0E                   ; 63C0: FD 4B 0E        // Attract screen/game phase  timer
L63C3               STB   $4B11                   ; 63C3: F7 4B 11        // Attract text intensity
L63C6               LDD   $4B0C                   ; 63C6: FC 4B 0C        // Attract text position for scrolling
                    STD   $48AF                   ; 63C9: FD 48 AF      
                    LDD   $4B10                   ; 63CC: FC 4B 10        // Attract text colour/intensity for fading
                    STD   ,Y++                    ; 63CF: ED A1         
                    JSR   $D942                   ; 63D1: BD D9 42        // Called from Attract screen 3 + 4
; End of function sub_6368
L63D4               RTS                           ; 63D4: 39            

; =============== S U B R O U T I N E =======================================
; Check credits status
L63D5               LDA   $4814                   ; 63D5: B6 48 14        // Credits
                    BNE   L63EB                   ; 63D8: 26 11         
                    LDA   <$43                    ; 63DA: 96 43           // Game over/insert coins timer
                    ANDA  #$10                    ; 63DC: 84 10         
                    BNE   L63E4                   ; 63DE: 26 04         
                    LDB   #$06                    ; 63E0: C6 06         
                    BRA   L63E6                   ; 63E2: 20 02         

; ---------------------------------------------------------------------------
L63E4               LDB   #$05                    ; 63E4: C6 05         
L63E6               JSR   $E7C7                   ; 63E6: BD E7 C7        // Print text string from pointer table
L63E9               BRA   L63F0                   ; 63E9: 20 05         

; ---------------------------------------------------------------------------
L63EB               LDB   #$0B                    ; 63EB: C6 0B         
                    JSR   $E7C7                   ; 63ED: BD E7 C7        // Print text string from pointer table
L63F0               LDA   $4814                   ; 63F0: B6 48 14        // Credits
                    BNE   L6410                   ; 63F3: 26 1B         
                    LDA   $4812                   ; 63F5: B6 48 12        // Half credit
                    BEQ   L6404                   ; 63F8: 27 0A         
                    LDA   <$43                    ; 63FA: 96 43           // Game over/insert coins timer
                    ANDA  #$10                    ; 63FC: 84 10         
                    BEQ   L6404                   ; 63FE: 27 04         
                    BRA   L6410                   ; 6400: 20 0E         

; ---------------------------------------------------------------------------
                    BRA   L640E                   ; 6402: 20 0A         

; ---------------------------------------------------------------------------
L6404               LDB   $4591                   ; 6404: F6 45 91      
                    ANDB  #$03                    ; 6407: C4 03         
                    ADDB  #$07                    ; 6409: CB 07         
                    JSR   $E7C7                   ; 640B: BD E7 C7        // Print text string from pointer table
L640E               BRA   locret_6458             ; 640E: 20 48         

; ---------------------------------------------------------------------------
L6410               LDA   $4814                   ; 6410: B6 48 14        // Credits
                    ADDA  $4812                   ; 6413: BB 48 12        // Half credit
                    CMPA  #$01                    ; 6416: 81 01         
                    BNE   L641E                   ; 6418: 26 04         
                    LDB   #$0D                    ; 641A: C6 0D         
                    BRA   L6420                   ; 641C: 20 02         

; ---------------------------------------------------------------------------
L641E               LDB   #$0C                    ; 641E: C6 0C         
L6420               JSR   $E7C7                   ; 6420: BD E7 C7        // Print text string from pointer table
L6423               LDD   #$01B0                  ; 6423: CC 01 B0      
                    ANDA  #$1F                    ; 6426: 84 1F         
                    STD   ,Y++                    ; 6428: ED A1         
                    LDD   #$FF80                  ; 642A: CC FF 80      
                    TST   $4812                   ; 642D: 7D 48 12        // Half credit
                    BEQ   L6435                   ; 6430: 27 03         
                    SUBD  #$18                    ; 6432: 83 00 18      
L6435               ANDA  #$1F                    ; 6435: 84 1F         
                    STD   ,Y++                    ; 6437: ED A1         
                    LDA   $4814                   ; 6439: B6 48 14        // Credits
                    CMPA  #$0A                    ; 643C: 81 0A         
                    BCS   L6442                   ; 643E: 25 02         
                    ADDA  #$06                    ; 6440: 8B 06         
L6442               LDB   #$02                    ; 6442: C6 02         
                    STB   <$AD                    ; 6444: D7 AD         
                    JSR   $E790                   ; 6446: BD E7 90        // Display BCD numbers
L6449               LDA   $4812                   ; 6449: B6 48 12        // Half credit
                    BEQ   L6453                   ; 644C: 27 05         
                    LDD   #$B8F3                  ; 644E: CC B8 F3      
                    STD   ,Y++                    ; 6451: ED A1         
L6453               LDD   #$8040                  ; 6453: CC 80 40      
                    STD   ,Y++                    ; 6456: ED A1         
; End of function sub_63D5
locret_6458         RTS                           ; 6458: 39            

; =============== S U B R O U T I N E =======================================
L6459               LDD   #$00                    ; 6459: CC 00 00        // Called once before attract screen 1
                    STD   $4B0C                   ; 645C: FD 4B 0C        // Attract text position for scrolling
                    STD   $48AF                   ; 645F: FD 48 AF      
                    LDD   #$0100                  ; 6462: CC 01 00      
                    STD   $4B0E                   ; 6465: FD 4B 0E        // Attract screen/game phase  timer
                    LDD   #$6180                  ; 6468: CC 61 80      
                    STD   $4B10                   ; 646B: FD 4B 10        // Attract text colour/intensity for fading
                    JSR   $D91A                   ; 646E: BD D9 1A      
L6471               JSR   L61B5                   ; 6471: BD 61 B5        // Set up math constants
L6474               JSR   L615A                   ; 6474: BD 61 5A        // More stars/ties init stuff
L6477               LDU   #$5038                  ; 6477: CE 50 38        // Matrix 2
                    JSR   $CDC3                   ; 647A: BD CD C3        // Initialise math registers matrix
L647D               JSR   $CC38                   ; 647D: BD CC 38      
L6480               INC   <$41                    ; 6480: 0C 41           // Game mode/screen state
; End of function sub_6459
                    RTS                           ; 6482: 39            

; =============== S U B R O U T I N E =======================================
; Attract screen 1
L6483               JSR   L6112                   ; 6483: BD 61 12        // Insert vector data for four blue dots in screen corners
L6486               JSR   L64CD                   ; 6486: BD 64 CD        // Print Star Wars and copyright text
L6489               JSR   $CD80                   ; 6489: BD CD 80        // Starfield
L648C               JSR   L761D                   ; 648C: BD 76 1D        // Display score
L648F               JSR   L63D5                   ; 648F: BD 63 D5        // Check credits status
L6492               LDD   $4B0E                   ; 6492: FC 4B 0E        // Attract screen/game phase  timer
                    CMPD  #$50                    ; 6495: 10 83 00 50   
                    BCC   L64A9                   ; 6499: 24 0E         
                    LDB   $4B11                   ; 649B: F6 4B 11        // Attract text intensity
                    SUBB  #$01                    ; 649E: C0 01         
                    CMPB  #$F0                    ; 64A0: C1 F0         
                    BCS   L64A6                   ; 64A2: 25 02         
                    LDB   #$00                    ; 64A4: C6 00         
L64A6               STB   $4B11                   ; 64A6: F7 4B 11        // Attract text intensity
L64A9               LDD   $4B10                   ; 64A9: FC 4B 10        // Attract text colour/intensity for fading
                    STD   ,Y++                    ; 64AC: ED A1         
                    JSR   $C7FD                   ; 64AE: BD C7 FD        // Display high scores
L64B1               JSR   $D923                   ; 64B1: BD D9 23        // Called from attract screen 1
L64B4               JSR   L612F                   ; 64B4: BD 61 2F        // Vector instructions end
L64B7               JSR   L6DCA                   ; 64B7: BD 6D CA        // Move stars XT translate position
L64BA               LDD   $4B0E                   ; 64BA: FC 4B 0E        // Attract screen/game phase  timer
                    SUBD  #$01                    ; 64BD: 83 00 01      
                    STD   $4B0E                   ; 64C0: FD 4B 0E        // Attract screen/game phase  timer
                    BPL   L64C9                   ; 64C3: 2A 04         
                    LDA   #$05                    ; 64C5: 86 05         
                    STA   <$41                    ; 64C7: 97 41           // Game mode/screen state
L64C9               JSR   L622D                   ; 64C9: BD 62 2D        // Check joystick X to show high scores if moved
; End of function sub_6483
L64CC               RTS                           ; 64CC: 39            

; =============== S U B R O U T I N E =======================================
; Print Star Wars and copyright text
L64CD               LDB   #$00                    ; 64CD: C6 00         
                    JSR   $E7C7                   ; 64CF: BD E7 C7        // Print text string from pointer table
L64D2               LDB   #$01                    ; 64D2: C6 01         
                    JSR   $E7C7                   ; 64D4: BD E7 C7        // Print text string from pointer table
L64D7               LDB   #$02                    ; 64D7: C6 02         
                    JSR   $E7C7                   ; 64D9: BD E7 C7        // Print text string from pointer table
L64DC               LDB   #$03                    ; 64DC: C6 03         
                    JSR   $E7C7                   ; 64DE: BD E7 C7        // Print text string from pointer table
; End of function sub_64CD
L64E1               RTS                           ; 64E1: 39            

; =============== S U B R O U T I N E =======================================
L64E2               JSR   $D91A                   ; 64E2: BD D9 1A      
L64E5               LDD   #$00                    ; 64E5: CC 00 00      
                    STD   $4B0C                   ; 64E8: FD 4B 0C        // Attract text position for scrolling
                    STD   $48AF                   ; 64EB: FD 48 AF      
                    INC   <$41                    ; 64EE: 0C 41           // Game mode/screen state
; End of function sub_64E2
                    RTS                           ; 64F0: 39            

; =============== S U B R O U T I N E =======================================
L64F1               JSR   L6112                   ; 64F1: BD 61 12        // Accounting time stats
L64F4               JSR   $BE20                   ; 64F4: BD BE 20        // Display accounting screen
L64F7               JSR   $D923                   ; 64F7: BD D9 23        // Called from attract screen 1
L64FA               JSR   L612F                   ; 64FA: BD 61 2F        // Vector instructions end
L64FD               LDA   <$AC                    ; 64FD: 96 AC         
                    ANDA  #$04                    ; 64FF: 84 04         
                    BEQ   L6507                   ; 6501: 27 04         
                    LDA   #$03                    ; 6503: 86 03         
                    STA   <$41                    ; 6505: 97 41           // Game mode/screen state
L6507               LDA   $481E                   ; 6507: B6 48 1E      
                    ANDA  #$10                    ; 650A: 84 10         
                    BEQ   locret_6512             ; 650C: 27 04         
                    LDA   #$05                    ; 650E: 86 05         
                    STA   <$41                    ; 6510: 97 41           // Game mode/screen state
; End of function sub_64F1
locret_6512         RTS                           ; 6512: 39            

; =============== S U B R O U T I N E =======================================
L6513               JSR   $D91A                   ; 6513: BD D9 1A      
L6516               LDA   #$0A                    ; 6516: 86 0A         
                    STA   $4AF6                   ; 6518: B7 4A F6      
                    LDA   #$00                    ; 651B: 86 00         
                    STA   $4598                   ; 651D: B7 45 98      
                    STA   $4AF7                   ; 6520: B7 4A F7      
                    LDA   #$03                    ; 6523: 86 03         
                    JSR   $C2C3                   ; 6525: BD C2 C3      
L6528               BEQ   L652F                   ; 6528: 27 05         
                    LDA   #$03                    ; 652A: 86 03         
                    JSR   $C369                   ; 652C: BD C3 69      
L652F               INC   <$41                    ; 652F: 0C 41           // Game mode/screen state
; End of function sub_6513
                    RTS                           ; 6531: 39            

; =============== S U B R O U T I N E =======================================
; FUNCTION CHUNK AT ROM:F261 SIZE 000000CD BYTES
; FUNCTION CHUNK AT ROM:F36E SIZE 000001AF BYTES
; FUNCTION CHUNK AT ROM:F520 SIZE 000001ED BYTES
; FUNCTION CHUNK AT ROM:F720 SIZE 00000003 BYTES
; FUNCTION CHUNK AT ROM:FD07 SIZE 0000007E BYTES
; FUNCTION CHUNK AT ROM:FF24 SIZE 00000033 BYTES
L6532               JSR   L6112                   ; 6532: BD 61 12        // Game options
L6535               JSR   $C450                   ; 6535: BD C4 50      
L6538               LDD   #L6780                  ; 6538: CC 67 80      
                    STD   ,Y++                    ; 653B: ED A1         
                    LDB   #$66                    ; 653D: C6 66         
                    ADDB  $4AF6                   ; 653F: FB 4A F6      
                    JSR   $E7D3                   ; 6542: BD E7 D3      
L6545               LDA   $4AF6                   ; 6545: B6 4A F6      
                    JSR   $C5A4                   ; 6548: BD C5 A4      
L654B               JSR   $C4EB                   ; 654B: BD C4 EB      
L654E               JSR   $D923                   ; 654E: BD D9 23        // Called from attract screen 1
L6551               JSR   L612F                   ; 6551: BD 61 2F        // Vector instructions end
L6554               LDA   <$AC                    ; 6554: 96 AC         
                    ANDA  #$04                    ; 6556: 84 04         
                    BEQ   L655D                   ; 6558: 27 03         
                    JMP   $F261                   ; 655A: 7E F2 61      

; ---------------------------------------------------------------------------
L655D               LDA   $481E                   ; 655D: B6 48 1E      
                    ANDA  #$10                    ; 6560: 84 10         
                    BEQ   locret_656B             ; 6562: 27 07         
                    JSR   $D91A                   ; 6564: BD D9 1A      
L6567               LDA   #$05                    ; 6567: 86 05         
                    STA   <$41                    ; 6569: 97 41           // Game mode/screen state
; End of function sub_6532
locret_656B         RTS                           ; 656B: 39            

; =============== S U B R O U T I N E =======================================
; Start select screen
L656C               LDD   #$0100                  ; 656C: CC 01 00      
                    STD   $4B0E                   ; 656F: FD 4B 0E        // Attract screen/game phase  timer
                    LDD   #$00                    ; 6572: CC 00 00      
                    STD   $4B0C                   ; 6575: FD 4B 0C        // Attract text position for scrolling
                    STD   $48AF                   ; 6578: FD 48 AF      
                    LDD   #$6480                  ; 657B: CC 64 80      
                    STD   $4B10                   ; 657E: FD 4B 10        // Attract text colour/intensity for fading
                    JSR   $D91A                   ; 6581: BD D9 1A      
L6584               LDB   #$2C                    ; 6584: C6 2C         
L6586               TFR   B,A                     ; 6586: 1F 98         
                    JSR   $D8DF                   ; 6588: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
L658B               INCB                          ; 658B: 5C            
                    CMPB  #$3A                    ; 658C: C1 3A         
                    BCS   L6586                   ; 658E: 25 F6         
                    INC   <$41                    ; 6590: 0C 41           // Game mode/screen state
; End of function sub_656C
; ---------------------------------------------------------------------------
                    RTS                           ; 6592: 39            

; =============== S U B R O U T I N E =======================================
L659F               LDD   $4B0E                   ; 659F: FC 4B 0E        // Attract screen/game phase  timer
                    SUBD  #$01                    ; 65A2: 83 00 01      
                    STD   $4B0E                   ; 65A5: FD 4B 0E        // Attract screen/game phase  timer
                    BPL   L65BD                   ; 65A8: 2A 13         
                    LDA   #$00                    ; 65AA: 86 00         
                    STA   $4B15                   ; 65AC: B7 4B 15      
                    LDA   #$1B                    ; 65AF: 86 1B         
                    STA   <$41                    ; 65B1: 97 41           // Game mode/screen state
                    CLR   $481B                   ; 65B3: 7F 48 1B      
                    CLR   $481A                   ; 65B6: 7F 48 1A      
                    CLR   $4819                   ; 65B9: 7F 48 19      
                    RTS                           ; 65BC: 39            

; ---------------------------------------------------------------------------
L65BD               JSR   L6112                   ; 65BD: BD 61 12        // Insert vector data for four blue dots in screen corners
L65C0               LDX   #word_6593              ; 65C0: 8E 65 93      
L65C3               LDD   ,X++                    ; 65C3: EC 81         
                    ANDA  #$1F                    ; 65C5: 84 1F         
                    STD   ,Y++                    ; 65C7: ED A1         
                    LDD   ,X++                    ; 65C9: EC 81         
                    ANDA  #$1F                    ; 65CB: 84 1F         
                    STD   ,Y++                    ; 65CD: ED A1         
                    LDD   #$7200                  ; 65CF: CC 72 00      
                    STD   ,Y++                    ; 65D2: ED A1         
                    LDD   #$BE50                  ; 65D4: CC BE 50      
                    STD   ,Y++                    ; 65D7: ED A1         
                    CMPX  #L659F                  ; 65D9: 8C 65 9F      
                    BCS   L65C3                   ; 65DC: 25 E5         
                    LDA   <$DD                    ; 65DE: 96 DD         
                    BPL   L65E7                   ; 65E0: 2A 05         
                    LDD   #$6380                  ; 65E2: CC 63 80      
                    BRA   L65EA                   ; 65E5: 20 03         

; ---------------------------------------------------------------------------
L65E7               LDD   #$6680                  ; 65E7: CC 66 80      
L65EA               STD   ,Y++                    ; 65EA: ED A1         
                    JSR   $B6C0                   ; 65EC: BD B6 C0        // Insert vector instructions at joystick position for laser explosion 3
L65EF               JSR   $D923                   ; 65EF: BD D9 23        // Called from attract screen 1
L65F2               LDD   #$C8                    ; 65F2: CC 00 C8      
                    STD   ,Y++                    ; 65F5: ED A1         
                    LDD   #$1FF0                  ; 65F7: CC 1F F0      
                    STD   ,Y++                    ; 65FA: ED A1         
                    LDD   $4B0E                   ; 65FC: FC 4B 0E        // Attract screen/game phase  timer
                    ASLB                          ; 65FF: 58            
                    ROLA                          ; 6600: 49            
                    ASLB                          ; 6601: 58            
                    ROLA                          ; 6602: 49            
                    ASLB                          ; 6603: 58            
                    ROLA                          ; 6604: 49            
                    CMPA  #$0A                    ; 6605: 81 0A         
                    BCS   L660B                   ; 6607: 25 02         
                    ADDA  #$06                    ; 6609: 8B 06         
L660B               JSR   $E790                   ; 660B: BD E7 90        // Display BCD numbers
L660E               JSR   L612F                   ; 660E: BD 61 2F        // Vector instructions end
L6611               LDA   #$FF                    ; 6611: 86 FF         
                    STA   <$DD                    ; 6613: 97 DD         
                    LDX   #word_6593              ; 6615: 8E 65 93      
L6618               LDD   <$7B                    ; 6618: DC 7B         
                    ADDD  #$FF98                  ; 661A: C3 FF 98      
                    SUBD  ,X                      ; 661D: A3 84         
                    TSTA                          ; 661F: 4D            
                    BPL   L6626                   ; 6620: 2A 04         
                    COMA                          ; 6622: 43            
                    NEGB                          ; 6623: 50            
                    SBCA  #$FF                    ; 6624: 82 FF         
L6626               STD   $4AFA                   ; 6626: FD 4A FA      
                    CMPD  #$48                    ; 6629: 10 83 00 48   
                    BCC   L6668                   ; 662D: 24 39         
                    LDD   <$79                    ; 662F: DC 79         
                    SUBD  $02,X                   ; 6631: A3 02         
                    TSTA                          ; 6633: 4D            
                    BPL   L663A                   ; 6634: 2A 04         
                    COMA                          ; 6636: 43            
                    NEGB                          ; 6637: 50            
                    SBCA  #$FF                    ; 6638: 82 FF         
L663A               CMPD  #$34                    ; 663A: 10 83 00 34   
                    BCC   L6668                   ; 663E: 24 28         
                    ADDD  $4AFA                   ; 6640: F3 4A FA      
                    CMPD  #$50                    ; 6643: 10 83 00 50   
                    BCC   L6668                   ; 6647: 24 1F         
                    TFR   X,D                     ; 6649: 1F 10         
                    SUBD  #word_6593              ; 664B: 83 65 93      
                    LSRB                          ; 664E: 54            
                    STB   <$DD                    ; 664F: D7 DD         
                    STB   $4B15                   ; 6651: F7 4B 15      
                    LDA   <$AC                    ; 6654: 96 AC         
                    ANDA  #$F0                    ; 6656: 84 F0         
                    BEQ   locret_6667             ; 6658: 27 0D         
                    LDA   #$1B                    ; 665A: 86 1B         
                    STA   <$41                    ; 665C: 97 41           // Game mode/screen state
                    CLR   $481B                   ; 665E: 7F 48 1B      
                    CLR   $481A                   ; 6661: 7F 48 1A      
                    CLR   $4819                   ; 6664: 7F 48 19      
locret_6667         RTS                           ; 6667: 39            

; ---------------------------------------------------------------------------
L6668               LEAX  $04,X                   ; 6668: 30 04         
                    CMPX  #L659F                  ; 666A: 8C 65 9F      
                    BCS   L6618                   ; 666D: 25 A9         
; End of function sub_659F
                    RTS                           ; 666F: 39            

; =============== S U B R O U T I N E =======================================
L6670               LDD   #$00                    ; 6670: CC 00 00      
                    STD   $4B0E                   ; 6673: FD 4B 0E        // Attract screen/game phase  timer
                    STD   $4B0C                   ; 6676: FD 4B 0C        // Attract text position for scrolling
                    STD   $48AF                   ; 6679: FD 48 AF      
                    LDD   #$6180                  ; 667C: CC 61 80      
                    STD   $4B10                   ; 667F: FD 4B 10        // Attract text colour/intensity for fading
                    JSR   $D91A                   ; 6682: BD D9 1A      
L6685               LDA   #$3A                    ; 6685: 86 3A         
                    JSR   $D8DF                   ; 6687: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
L668A               LDA   #$3B                    ; 668A: 86 3B         
                    JSR   $D8DF                   ; 668C: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
L668F               LDA   #$3C                    ; 668F: 86 3C         
                    JSR   $D8DF                   ; 6691: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
L6694               LDA   #$3D                    ; 6694: 86 3D         
                    JSR   $D8DF                   ; 6696: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
L6699               LDA   #$3E                    ; 6699: 86 3E         
                    JSR   $D8DF                   ; 669B: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
L669E               LDA   #$3D                    ; 669E: 86 3D         
                    JSR   $D8DF                   ; 66A0: BD D8 DF        // Called from select screen, attract screen 1 + 3 when writing text
L66A3               JSR   $BD80                   ; 66A3: BD BD 80      
L66A6               JSR   $CC38                   ; 66A6: BD CC 38      
L66A9               INC   <$41                    ; 66A9: 0C 41           // Game mode/screen state
; End of function sub_6670
                    RTS                           ; 66AB: 39            

; =============== S U B R O U T I N E =======================================
L66AC               JSR   L6112                   ; 66AC: BD 61 12        // Insert vector data for four blue dots in screen corners
L66AF               JSR   L761D                   ; 66AF: BD 76 1D        // Display score
L66B2               JSR   L63D5                   ; 66B2: BD 63 D5        // Check credits status
L66B5               JSR   $C7FD                   ; 66B5: BD C7 FD        // Display high scores
L66B8               JSR   $D923                   ; 66B8: BD D9 23        // Called from attract screen 1
L66BB               JSR   L612F                   ; 66BB: BD 61 2F        // Vector instructions end
L66BE               JSR   $CAF3                   ; 66BE: BD CA F3      
L66C1               LDA   $481E                   ; 66C1: B6 48 1E      
                    ANDA  #$10                    ; 66C4: 84 10         
                    BNE   L66CE                   ; 66C6: 26 06         
                    LDD   #$0300                  ; 66C8: CC 03 00      
                    STD   $4B0E                   ; 66CB: FD 4B 0E        // Attract screen/game phase  timer
L66CE               LDD   $4B0E                   ; 66CE: FC 4B 0E        // Attract screen/game phase  timer
                    ADDD  #$01                    ; 66D1: C3 00 01      
                    STD   $4B0E                   ; 66D4: FD 4B 0E        // Attract screen/game phase  timer
                    CMPD  #$0280                  ; 66D7: 10 83 02 80   
                    BCS   L66E3                   ; 66DB: 25 06         
                    LDD   #$FFFF                  ; 66DD: CC FF FF      
                    STD   $4AEC                   ; 66E0: FD 4A EC      
L66E3               LDD   $4AEC                   ; 66E3: FC 4A EC      
                    BPL   locret_6707             ; 66E6: 2A 1F         
                    LDA   #$0B                    ; 66E8: 86 0B         
                    STA   <$41                    ; 66EA: 97 41           // Game mode/screen state
                    LDU   #$4AB6                  ; 66EC: CE 4A B6      
                    LDX   #$4520                  ; 66EF: 8E 45 20      
                    LDA   #$08                    ; 66F2: 86 08         
                    JSR   $C6F9                   ; 66F4: BD C6 F9      
L66F7               LDU   #$4A8E                  ; 66F7: CE 4A 8E        // High scores RAM
                    LDX   #$4508                  ; 66FA: 8E 45 08      
                    LDA   #$0B                    ; 66FD: 86 0B         
                    JSR   $C6F9                   ; 66FF: BD C6 F9      
L6702               LDA   #$01                    ; 6702: 86 01         
                    JSR   $C2B3                   ; 6704: BD C2 B3      
; End of function sub_66AC
locret_6707         RTS                           ; 6707: 39            

; =============== S U B R O U T I N E =======================================
; Called once before attract screen 2
L6708               INC   <$41                    ; 6708: 0C 41           // Game mode/screen state
                    JSR   L611E                   ; 670A: BD 61 1E        // Copies Star Wars logo vector data to vector RAM
L670D               JSR   $D91A                   ; 670D: BD D9 1A      
L6710               LDD   #$00                    ; 6710: CC 00 00      
                    STD   $4B0C                   ; 6713: FD 4B 0C        // Attract text position for scrolling
                    STD   $48AF                   ; 6716: FD 48 AF      
                    JSR   $D9DC                   ; 6719: BD D9 DC        // Initialise before game start
L671C               JSR   L61B5                   ; 671C: BD 61 B5        // Set up math constants
L671F               JSR   L615A                   ; 671F: BD 61 5A        // More stars/ties init stuff
L6722               LDU   #$5038                  ; 6722: CE 50 38        // Matrix 2
L6725               JSR   $CDC3                   ; 6725: BD CD C3        // Initialise math registers matrix
L6728               LDA   $4B34                   ; 6728: B6 4B 34      
                    CMPA  #$FF                    ; 672B: 81 FF         
                    BNE   L6737                   ; 672D: 26 08         
                    LDA   $4B06                   ; 672F: B6 4B 06      
                    STA   $4B34                   ; 6732: B7 4B 34      
                    BRA   locret_6758             ; 6735: 20 21         

; ---------------------------------------------------------------------------
L6737               LDA   $4B06                   ; 6737: B6 4B 06        // Called once before attract screen 2
                    CMPA  $4B34                   ; 673A: B1 4B 34      
                    BEQ   locret_6758             ; 673D: 27 19         
                    STA   $4B34                   ; 673F: B7 4B 34      
                    JSR   $C20C                   ; 6742: BD C2 0C      
L6745               LDA   $4592                   ; 6745: B6 45 92      
                    ANDA  #$04                    ; 6748: 84 04         
                    BNE   locret_6758             ; 674A: 26 0C         
                    LDX   #off_6759               ; 674C: 8E 67 59      
                    LDA   $4703                   ; 674F: B6 47 03      
                    LDB   #$09                    ; 6752: C6 09         
                    MUL                           ; 6754: 3D            
                    ASLA                          ; 6755: 48            
                    JSR   [A,X]                   ; 6756: AD 96           // Play random attract sound
; End of function sub_6708
; ---------------------------------------------------------------------------
locret_6758         RTS                           ; 6758: 39            

; =============== S U B R O U T I N E =======================================
L676B               JSR   L6112                   ; 676B: BD 61 12        // Attract screen 2
L676E               JSR   $CD80                   ; 676E: BD CD 80        // Starfield
L6771               JSR   $D9FA                   ; 6771: BD D9 FA        // Attract screen 2 large Star Wars logo handling
L6774               JSR   $D985                   ; 6774: BD D9 85      
L6777               JSR   L63D5                   ; 6777: BD 63 D5        // Check credits status
L677A               JSR   L761D                   ; 677A: BD 76 1D        // Display score
L677D               JSR   L612F                   ; 677D: BD 61 2F        // Vector instructions end
L6780               JSR   L6DA5                   ; 6780: BD 6D A5        // Attract screen 2 stars forward and down translate
L6783               JSR   L622D                   ; 6783: BD 62 2D        // Check joystick X to show high scores if moved
; End of function sub_676B
L6786               RTS                           ; 6786: 39            

; =============== S U B R O U T I N E =======================================
; Called once before difficulty select screen
L6787               LDA   #$05                    ; 6787: 86 05         
                    STA   $4B0E                   ; 6789: B7 4B 0E        // Attract screen/game phase  timer
                    LDA   #$FF                    ; 678C: 86 FF         
                    STA   $4B34                   ; 678E: B7 4B 34      
                    JSR   $D91A                   ; 6791: BD D9 1A      
L6794               JSR   $BD44                   ; 6794: BD BD 44      
L6797               INC   <$41                    ; 6797: 0C 41           // Game mode/screen state
; End of function sub_6787
                    RTS                           ; 6799: 39            

; =============== S U B R O U T I N E =======================================
; Called once before difficulty select screen
L679A               LDA   #$0D                    ; 679A: 86 0D         
                    STA   <$41                    ; 679C: 97 41           // Game mode/screen state
                    LDD   #$00                    ; 679E: CC 00 00      
                    STD   <$42                    ; 67A1: DD 42         
                    STA   <$DD                    ; 67A3: 97 DD         
                    LDA   $4593                   ; 67A5: B6 45 93      
                    ANDA  #$03                    ; 67A8: 84 03         
L67AA               ADDA  #$06                    ; 67AA: 8B 06         
                    STA   <$60                    ; 67AC: 97 60           // Shield count
                    STA   <$8E                    ; 67AE: 97 8E         
                    LDA   $4593                   ; 67B0: B6 45 93      
                    LSRA                          ; 67B3: 44            
                    LSRA                          ; 67B4: 44            
                    ANDA  #$03                    ; 67B5: 84 03         
                    STA   $4B18                   ; 67B7: B7 4B 18      
                    LDA   #$00                    ; 67BA: 86 00         
                    STA   $4B17                   ; 67BC: B7 4B 17      
                    STA   <$8B                    ; 67BF: 97 8B         
                    STA   <$8C                    ; 67C1: 97 8C           // Sheild being depleted
                    STA   <$5C                    ; 67C3: 97 5C           // Score millions
                    STA   <$5D                    ; 67C5: 97 5D           // Score hundred thousands
                    STA   <$5E                    ; 67C7: 97 5E           // Score thousands
                    STA   <$5F                    ; 67C9: 97 5F           // Score
                    STA   $4B2D                   ; 67CB: B7 4B 2D      
                    STA   $4B37                   ; 67CE: B7 4B 37      
L67D1               STA   $4B35                   ; 67D1: B7 4B 35      
L67D4               ORCC  #$10                    ; 67D4: 1A 10         
L67D6               INC   $486F                   ; 67D6: 7C 48 6F        // Joystick roll data structure
                    INC   $4866                   ; 67D9: 7C 48 66        // Joystick pitch data structure
                    INC   $4871                   ; 67DC: 7C 48 71      
                    INC   $4868                   ; 67DF: 7C 48 68      
                    ANDCC #$EF                    ; 67E2: 1C EF         
; End of function sub_679A
                    RTS                           ; 67E4: 39            

; =============== S U B R O U T I N E =======================================
L67E5               JSR   L61B5                   ; 67E5: BD 61 B5        // Set up math constants
L67E8               JSR   L615A                   ; 67E8: BD 61 5A        // More stars/ties init stuff
L67EB               JSR   L61EC                   ; 67EB: BD 61 EC        // Init stars math data
L67EE               LDA   #$C0                    ; 67EE: 86 C0         
                    STA   $5080                   ; 67F0: B7 50 80        // Matrix 4
                    STA   $508A                   ; 67F3: B7 50 8A      
                    LDA   #$00                    ; 67F6: 86 00         
                    STA   $4813                   ; 67F8: B7 48 13      
                    INC   <$41                    ; 67FB: 0C 41           // Game mode/screen state
L67FD               LDA   #$1D                    ; 67FD: 86 1D         
                    STA   <$41                    ; 67FF: 97 41           // Game mode/screen state
; End of function sub_67E5
                    RTS                           ; 6801: 39            

; =============== S U B R O U T I N E =======================================
; Next space wave after Death Star explosion
L6802               JSR   L7A5A                   ; 6802: BD 7A 5A      
L6805               LDX   #$08                    ; 6805: 8E 00 08      
                    LDA   $4B15                   ; 6808: B6 4B 15      
                    INCA                          ; 680B: 4C            
                    JSR   L7720                   ; 680C: BD 77 20      
L680F               LDA   $4AD6                   ; 680F: B6 4A D6      
                    STA   $4B16                   ; 6812: B7 4B 16      
                    LDA   $4B15                   ; 6815: B6 4B 15      
L6818               CMPA  #$1F                    ; 6818: 81 1F         
                    BLS   L681E                   ; 681A: 23 02         
                    LDA   #$1F                    ; 681C: 86 1F         
L681E               STA   $4B14                   ; 681E: B7 4B 14      
                    ADDA  $4B18                   ; 6821: BB 4B 18      
                    CMPA  #$0F                    ; 6824: 81 0F         
                    BLS   L682A                   ; 6826: 23 02         
                    LDA   #$0F                    ; 6828: 86 0F         
L682A               STA   $4B19                   ; 682A: B7 4B 19      
                    INC   <$41                    ; 682D: 0C 41           // Game mode/screen state
L682F               LDA   #$1F                    ; 682F: 86 1F         
                    STA   <$41                    ; 6831: 97 41           // Game mode/screen state
                    LDA   #$00                    ; 6833: 86 00         
                    STA   <$DD                    ; 6835: 97 DD         
; End of function sub_6802
                    RTS                           ; 6837: 39            

; =============== S U B R O U T I N E =======================================
; Next space wave after Death Star explosion
; FUNCTION CHUNK AT ROM:6C76 SIZE 0000000E BYTES
L6838               JSR   L6161                   ; 6838: BD 61 61        // Initialise tie fighters and fireballs
L683B               JSR   $B939                   ; 683B: BD B9 39      
L683E               LDD   #$00                    ; 683E: CC 00 00      
                    STD   $4B0E                   ; 6841: FD 4B 0E        // Attract screen/game phase  timer
                    STA   $4B3B                   ; 6844: B7 4B 3B      
                    LDA   #$09                    ; 6847: 86 09         
                    STA   $4B3C                   ; 6849: B7 4B 3C      
                    LDA   $4B2D                   ; 684C: B6 4B 2D      
                    BNE   L6857                   ; 684F: 26 06         
                    LDD   #$27                    ; 6851: CC 00 27      
                    STD   $4B0E                   ; 6854: FD 4B 0E        // Attract screen/game phase  timer
L6857               INC   <$41                    ; 6857: 0C 41           // Game mode/screen state
L6859               JSR   L72C7                   ; 6859: BD 72 C7        // Space wave
L685C               LDA   <$60                    ; 685C: 96 60           // Shield count
                    LBMI  L6C76                   ; 685E: 10 2B 04 14   
L6862               JSR   $A849                   ; 6862: BD A8 49        // Fireball movement
L6865               JSR   $9898                   ; 6865: BD 98 98      
L6868               JSR   $B98B                   ; 6868: BD B9 8B        // Check if tie/bunker/tower been hit
L686B               JSR   $9890                   ; 686B: BD 98 90        // Fireball timer 3
L686E               JSR   $9558                   ; 686E: BD 95 58        // Process shields
L6871               JSR   $8B6D                   ; 6871: BD 8B 6D      
L6874               JSR   L70DB                   ; 6874: BD 70 DB      
L6877               JSR   L6DD2                   ; 6877: BD 6D D2      
L687A               LDA   #$10                    ; 687A: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 687C: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L687F               JSR   L6F5F                   ; 687F: BD 6F 5F      
L6882               LDD   $4B0E                   ; 6882: FC 4B 0E        // Attract screen/game phase  timer
                    ADDD  #$01                    ; 6885: C3 00 01      
                    STD   $4B0E                   ; 6888: FD 4B 0E        // Attract screen/game phase  timer
                    CMPD  #$28                    ; 688B: 10 83 00 28   
                    BNE   L68A6                   ; 688F: 26 15         
                    LDA   $4B15                   ; 6891: B6 4B 15      
                    CMPA  #$03                    ; 6894: 81 03         
                    BLT   L68A1                   ; 6896: 2D 09         
                    ANDA  #$01                    ; 6898: 84 01         
                    BEQ   L68A1                   ; 689A: 27 05         
                    JSR   $BD85                   ; 689C: BD BD 85        // Imperial March
L689F               BRA   L68A4                   ; 689F: 20 03         

; ---------------------------------------------------------------------------
L68A1               JSR   $BDA8                   ; 68A1: BD BD A8        // Space wave 1 music
L68A4               BRA   L68C6                   ; 68A4: 20 20         

; ---------------------------------------------------------------------------
L68A6               CMPD  #$C8                    ; 68A6: 10 83 00 C8   
                    BNE   L68B1                   ; 68AA: 26 05         
                    JSR   $BDAD                   ; 68AC: BD BD AD        // Space Wave 2 music
L68AF               BRA   L68C6                   ; 68AF: 20 15         

; ---------------------------------------------------------------------------
L68B1               CMPD  #$0190                  ; 68B1: 10 83 01 90   
                    BNE   L68BC                   ; 68B5: 26 05         
                    JSR   $BD8A                   ; 68B7: BD BD 8A        // Enter Death Star
L68BA               BRA   L68C6                   ; 68BA: 20 0A         

; ---------------------------------------------------------------------------
L68BC               CMPD  #$01A4                  ; 68BC: 10 83 01 A4   
                    BCS   L68C6                   ; 68C0: 25 04         
                    LDA   #$21                    ; 68C2: 86 21         
                    STA   <$41                    ; 68C4: 97 41           // Game mode/screen state
L68C6               LDA   <$E6                    ; 68C6: 96 E6         
                    CMPA  #$03                    ; 68C8: 81 03         
                    BCC   locret_68CF             ; 68CA: 24 03         
                    JSR   $8F7B                   ; 68CC: BD 8F 7B      
; End of function sub_6838
locret_68CF         RTS                           ; 68CF: 39            

; =============== S U B R O U T I N E =======================================
L68D0               INC   $4B3B                   ; 68D0: 7C 4B 3B      
                    INC   <$41                    ; 68D3: 0C 41           // Game mode/screen state
L68D5               JSR   L72C7                   ; 68D5: BD 72 C7        // Entering Death Star
L68D8               LDA   <$60                    ; 68D8: 96 60           // Shield count
                    LBMI  L6C76                   ; 68DA: 10 2B 03 98   
                    JSR   $A849                   ; 68DE: BD A8 49        // Fireball movement
L68E1               JSR   $9898                   ; 68E1: BD 98 98      
L68E4               JSR   $B98B                   ; 68E4: BD B9 8B        // Check if tie/bunker/tower been hit
L68E7               JSR   $9890                   ; 68E7: BD 98 90        // Fireball timer 3
L68EA               JSR   $9558                   ; 68EA: BD 95 58        // Process shields
L68ED               JSR   $8B86                   ; 68ED: BD 8B 86      
L68F0               JSR   L70DB                   ; 68F0: BD 70 DB      
L68F3               JSR   L6DFA                   ; 68F3: BD 6D FA      
L68F6               LDA   #$10                    ; 68F6: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 68F8: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L68FB               JSR   L6F5F                   ; 68FB: BD 6F 5F      
L68FE               LDX   #$4900                  ; 68FE: 8E 49 00        // 3x Tie fighter data structure ($19 bytes per Tie)
L6901               LDA   $03,X                   ; 6901: A6 03         
                    BNE   locret_6911             ; 6903: 26 0C         
                    LEAX  $19,X                   ; 6905: 30 88 19      
                    CMPX  #$494B                  ; 6908: 8C 49 4B        // 3x Tie fighter data structure ($19 bytes per Tie)
                    BCS   L6901                   ; 690B: 25 F4         
                    LDA   #$23                    ; 690D: 86 23         
                    STA   <$41                    ; 690F: 97 41           // Game mode/screen state
; End of function sub_68D0
locret_6911         RTS                           ; 6911: 39            

; =============== S U B R O U T I N E =======================================
L6912               LDD   #$7780                  ; 6912: CC 77 80      
                    STD   <$56                    ; 6915: DD 56           // Zoom value
                    LDD   #$0100                  ; 6917: CC 01 00      
                    STD   <$58                    ; 691A: DD 58           // Death Star zoom value
                    LDA   $4B2D                   ; 691C: B6 4B 2D      
                    BNE   L692A                   ; 691F: 26 09         
                    LDA   $4B14                   ; 6921: B6 4B 14      
                    CMPA  #$04                    ; 6924: 81 04         
                    BNE   L692A                   ; 6926: 26 02         
                    BRA   L6930                   ; 6928: 20 06         

; ---------------------------------------------------------------------------
L692A               JSR   $BD67                   ; 692A: BD BD 67        // Red 5 Im going in
L692D               JSR   $BDEE                   ; 692D: BD BD EE        // R2 beeps entering Death Star
L6930               INC   <$41                    ; 6930: 0C 41           // Game mode/screen state
; End of function sub_6912
                    RTS                           ; 6932: 39            

; =============== S U B R O U T I N E =======================================
; Entering Death Star 2nd part
L6933               JSR   L733C                   ; 6933: BD 73 3C        // End of space wave? Also Trench catwalks
L6936               JSR   $B98B                   ; 6936: BD B9 8B        // Check if tie/bunker/tower been hit
L6939               JSR   $9890                   ; 6939: BD 98 90        // Fireball timer 3
L693C               JSR   $9558                   ; 693C: BD 95 58        // Process shields
L693F               JSR   L6DD2                   ; 693F: BD 6D D2      
L6942               JSR   L6F67                   ; 6942: BD 6F 67      
L6945               LDD   $5080                   ; 6945: FC 50 80        // Matrix 4
                    CMPD  #$3F00                  ; 6948: 10 83 3F 00   
                    BLT   locret_6952             ; 694C: 2D 04         
                    LDA   #$25                    ; 694E: 86 25         
                    STA   <$41                    ; 6950: 97 41           // Game mode/screen state
; End of function sub_6933
locret_6952         RTS                           ; 6952: 39            

; =============== S U B R O U T I N E =======================================
L6953               LDA   $4B2D                   ; 6953: B6 4B 2D      
                    BNE   L6962                   ; 6956: 26 0A         
                    LDA   $4B14                   ; 6958: B6 4B 14      
                    CMPA  #$04                    ; 695B: 81 04         
                    BNE   L6962                   ; 695D: 26 03         
                    JSR   $BD53                   ; 695F: BD BD 53        // Look at the size of that thing
L6962               JSR   $BE0C                   ; 6962: BD BE 0C      
L6965               INC   <$41                    ; 6965: 0C 41           // Game mode/screen state
; End of function sub_6953
                    RTS                           ; 6967: 39            

; =============== S U B R O U T I N E =======================================
; Entering Death Star zoom in
L6968               JSR   L736F                   ; 6968: BD 73 6F      
L696B               JSR   $9898                   ; 696B: BD 98 98      
L696E               JSR   $9890                   ; 696E: BD 98 90        // Fireball timer 3
L6971               JSR   $9558                   ; 6971: BD 95 58        // Process shields
L6974               JSR   L6DD2                   ; 6974: BD 6D D2      
L6977               JSR   L6F67                   ; 6977: BD 6F 67      
L697A               LDB   <$58                    ; 697A: D6 58           // Death Star zoom value
                    NEGB                          ; 697C: 50            
                    SEX                           ; 697D: 1D            
                    ADDD  <$56                    ; 697E: D3 56           // Zoom value
                    ANDB  #$7F                    ; 6980: C4 7F         
                    STD   <$56                    ; 6982: DD 56           // Zoom value
                    CMPD  #$7310                  ; 6984: 10 83 73 10   
                    BHI   L6997                   ; 6988: 22 0D         
                    LDA   $4B14                   ; 698A: B6 4B 14      
                    BNE   L6993                   ; 698D: 26 04         
                    LDA   #$27                    ; 698F: 86 27         
                    BRA   L6995                   ; 6991: 20 02         

; ---------------------------------------------------------------------------
L6993               LDA   #$29                    ; 6993: 86 29         
L6995               STA   <$41                    ; 6995: 97 41           // Game mode/screen state
L6997               LDD   <$58                    ; 6997: DC 58           // Death Star zoom value
                    ADDD  #$60                    ; 6999: C3 00 60      
                    STD   <$58                    ; 699C: DD 58           // Death Star zoom value
                    LDA   <$83                    ; 699E: 96 83           // Star intensity
                    SUBA  #$02                    ; 69A0: 80 02         
                    BHI   L69A6                   ; 69A2: 22 02         
                    LDA   #$00                    ; 69A4: 86 00         
L69A6               STA   <$83                    ; 69A6: 97 83           // Star intensity
; End of function sub_6968
                    RTS                           ; 69A8: 39            

; =============== S U B R O U T I N E =======================================
; Towers/Bunkers init
L69A9               LDA   $4B15                   ; 69A9: B6 4B 15      
                    DECA                          ; 69AC: 4A            
                    CMPA  #$1F                    ; 69AD: 81 1F         
                    BCS   L69B3                   ; 69AF: 25 02         
                    LDA   #$1F                    ; 69B1: 86 1F         
L69B3               STA   $4B13                   ; 69B3: B7 4B 13      
                    ADDA  $4B18                   ; 69B6: BB 4B 18      
                    CMPA  #$0F                    ; 69B9: 81 0F         
                    BLS   L69BF                   ; 69BB: 23 02         
                    LDA   #$0F                    ; 69BD: 86 0F         
L69BF               STA   $4B19                   ; 69BF: B7 4B 19      
                    JSR   L615A                   ; 69C2: BD 61 5A        // More stars/ties init stuff
L69C5               JSR   L620F                   ; 69C5: BD 62 0F        // Init towers surface dots
L69C8               JSR   $A1CE                   ; 69C8: BD A1 CE      
L69CB               JSR   $B939                   ; 69CB: BD B9 39      
L69CE               LDD   #$0100                  ; 69CE: CC 01 00      
                    STD   $5086                   ; 69D1: FD 50 86      
                    ASRA                          ; 69D4: 47            
                    RORB                          ; 69D5: 56            
                    STD   $5098                   ; 69D6: FD 50 98      
                    LDD   #$2000                  ; 69D9: CC 20 00      
                    STD   $509C                   ; 69DC: FD 50 9C      
                    LDA   #$00                    ; 69DF: 86 00         
                    STA   <$A7                    ; 69E1: 97 A7         
                    STA   $4B35                   ; 69E3: B7 4B 35      
                    STA   $4B3D                   ; 69E6: B7 4B 3D      
                    LDA   #$00                    ; 69E9: 86 00         
                    STA   $4B0E                   ; 69EB: B7 4B 0E        // Attract screen/game phase  timer
                    JSR   $BD94                   ; 69EE: BD BD 94        // Towers 1 music
L69F1               INC   <$41                    ; 69F1: 0C 41           // Game mode/screen state
; End of function sub_69A9
                    RTS                           ; 69F3: 39            

; =============== S U B R O U T I N E =======================================
; Towers/Bunkers wave
; FUNCTION CHUNK AT ROM:6CB6 SIZE 0000000E BYTES
L69F4               JSR   L7390                   ; 69F4: BD 73 90        // Towers/Bunkers wave
L69F7               LDA   <$60                    ; 69F7: 96 60           // Shield count
                    LBMI  L6CB6                   ; 69F9: 10 2B 02 B9   
                    JSR   $A849                   ; 69FD: BD A8 49        // Fireball movement
L6A00               JSR   $9890                   ; 6A00: BD 98 90        // Fireball timer 3
L6A03               JSR   $9558                   ; 6A03: BD 95 58        // Process shields
L6A06               JSR   $B98B                   ; 6A06: BD B9 8B        // Check if tie/bunker/tower been hit
; End of function sub_69F4
L6A09               JSR   L70DB                   ; 6A09: BD 70 DB      
; =============== S U B R O U T I N E =======================================
L6A0C               JSR   L6E22                   ; 6A0C: BD 6E 22      
L6A0F               LDA   #$10                    ; 6A0F: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 6A11: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L6A14               JSR   L6F6F                   ; 6A14: BD 6F 6F      
L6A17               LDD   $5086                   ; 6A17: FC 50 86      
                    ADDD  #$01                    ; 6A1A: C3 00 01      
                    CMPD  #$0400                  ; 6A1D: 10 83 04 00   
                    BHI   L6A26                   ; 6A21: 22 03         
                    STD   $5086                   ; 6A23: FD 50 86      
L6A26               LDA   <$43                    ; 6A26: 96 43           // Game over/insert coins timer
                    ANDA  #$0F                    ; 6A28: 84 0F         
                    BNE   L6A39                   ; 6A2A: 26 0D         
                    LDA   $4B0E                   ; 6A2C: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$0E                    ; 6A2F: 81 0E         
                    BNE   L6A36                   ; 6A31: 26 03         
                    JSR   $BD99                   ; 6A33: BD BD 99        // Towers 2 music
L6A36               INC   $4B0E                   ; 6A36: 7C 4B 0E        // Attract screen/game phase  timer
L6A39               LDA   <$A7                    ; 6A39: 96 A7         
                    CMPA  #$05                    ; 6A3B: 81 05         
                    BCS   locret_6A4F             ; 6A3D: 25 10         
                    LDA   #$01                    ; 6A3F: 86 01         
                    STA   $4B3D                   ; 6A41: B7 4B 3D      
                    LDA   $5098                   ; 6A44: B6 50 98      
                    CMPA  #$80                    ; 6A47: 81 80         
                    BCC   locret_6A4F             ; 6A49: 24 04         
                    LDA   #$2B                    ; 6A4B: 86 2B         
                    STA   <$41                    ; 6A4D: 97 41           // Game mode/screen state
; End of function sub_6A0C
locret_6A4F         RTS                           ; 6A4F: 39            

; =============== S U B R O U T I N E =======================================
; Entering Death Star zoomed fully in
L6A50               LDA   #$00                    ; 6A50: 86 00         
                    STA   <$98                    ; 6A52: 97 98         
                    STA   $4B36                   ; 6A54: B7 4B 36      
                    LDA   $4B15                   ; 6A57: B6 4B 15      
                    CMPA  #$1F                    ; 6A5A: 81 1F         
                    BLS   L6A60                   ; 6A5C: 23 02         
                    LDA   #$1F                    ; 6A5E: 86 1F         
L6A60               STA   $4B12                   ; 6A60: B7 4B 12      
                    ADDA  $4B18                   ; 6A63: BB 4B 18      
                    CMPA  #$0F                    ; 6A66: 81 0F         
                    BLS   L6A6C                   ; 6A68: 23 02         
                    LDA   #$0F                    ; 6A6A: 86 0F         
L6A6C               STA   $4B19                   ; 6A6C: B7 4B 19      
                    JSR   $83A4                   ; 6A6F: BD 83 A4        // Called when starting trench
L6A72               JSR   $BD71                   ; 6A72: BD BD 71      
L6A75               LDD   #$00                    ; 6A75: CC 00 00      
                    STD   $4B0E                   ; 6A78: FD 4B 0E        // Attract screen/game phase  timer
                    INC   <$41                    ; 6A7B: 0C 41           // Game mode/screen state
; End of function sub_6A50
                    RTS                           ; 6A7D: 39            

; =============== S U B R O U T I N E =======================================
L6A7E               JSR   L615A                   ; 6A7E: BD 61 5A        // More stars/ties init stuff
L6A81               JSR   $8341                   ; 6A81: BD 83 41        // Entering trench
L6A84               LDA   #$2F                    ; 6A84: 86 2F         
                    STA   <$41                    ; 6A86: 97 41           // Game mode/screen state
; End of function sub_6A7E
                    RTS                           ; 6A88: 39            

; =============== S U B R O U T I N E =======================================
L6A89               JSR   L73EA                   ; 6A89: BD 73 EA        // Towers
L6A8C               JSR   $9890                   ; 6A8C: BD 98 90        // Fireball timer 3
L6A8F               JSR   $9558                   ; 6A8F: BD 95 58        // Process shields
L6A92               JSR   $B98B                   ; 6A92: BD B9 8B        // Check if tie/bunker/tower been hit
L6A95               JSR   L6FE0                   ; 6A95: BD 6F E0      
L6A98               LDD   $4B0E                   ; 6A98: FC 4B 0E        // Attract screen/game phase  timer
                    ADDD  #$01                    ; 6A9B: C3 00 01      
L6A9E               STD   $4B0E                   ; 6A9E: FD 4B 0E        // Attract screen/game phase  timer
                    SUBD  #$11                    ; 6AA1: 83 00 11      
                    BCS   locret_6AAA             ; 6AA4: 25 04         
                    LDA   #$2D                    ; 6AA6: 86 2D         
                    STA   <$41                    ; 6AA8: 97 41           // Game mode/screen state
; End of function sub_6A89
locret_6AAA         RTS                           ; 6AAA: 39            

; =============== S U B R O U T I N E =======================================
L6AAB               LDD   #$00                    ; 6AAB: CC 00 00      
                    STD   $5098                   ; 6AAE: FD 50 98      
                    STD   $509A                   ; 6AB1: FD 50 9A      
                    STD   $4B0E                   ; 6AB4: FD 4B 0E        // Attract screen/game phase  timer
                    JSR   L6FF1                   ; 6AB7: BD 6F F1      
L6ABA               JSR   $8341                   ; 6ABA: BD 83 41        // Entering trench
L6ABD               INC   <$41                    ; 6ABD: 0C 41           // Game mode/screen state
L6ABF               JSR   L7413                   ; 6ABF: BD 74 13      
L6AC2               JSR   $9890                   ; 6AC2: BD 98 90        // Fireball timer 3
L6AC5               JSR   $9558                   ; 6AC5: BD 95 58        // Process shields
L6AC8               JSR   $B98B                   ; 6AC8: BD B9 8B        // Check if tie/bunker/tower been hit
L6ACB               JSR   L6FF1                   ; 6ACB: BD 6F F1      
L6ACE               LDD   $4B0E                   ; 6ACE: FC 4B 0E        // Attract screen/game phase  timer
                    ADDD  #$01                    ; 6AD1: C3 00 01      
                    STD   $4B0E                   ; 6AD4: FD 4B 0E        // Attract screen/game phase  timer
                    SUBD  #$11                    ; 6AD7: 83 00 11      
                    BCS   locret_6AFE             ; 6ADA: 25 22         
                    LDX   $5098                   ; 6ADC: BE 50 98      
                    LDU   $509A                   ; 6ADF: FE 50 9A      
                    LDD   $509C                   ; 6AE2: FC 50 9C      
                    PSHS  U,X,B,A                 ; 6AE5: 34 56         
                    JSR   L615A                   ; 6AE7: BD 61 5A        // More stars/ties init stuff
L6AEA               PULS  A,B,X,U                 ; 6AEA: 35 56         
                    STX   $5098                   ; 6AEC: BF 50 98      
                    STU   $509A                   ; 6AEF: FF 50 9A      
                    STD   $509C                   ; 6AF2: FD 50 9C      
                    LDA   #$00                    ; 6AF5: 86 00         
                    STA   $4B36                   ; 6AF7: B7 4B 36      
                    LDA   #$2F                    ; 6AFA: 86 2F         
                    STA   <$41                    ; 6AFC: 97 41           // Game mode/screen state
; End of function sub_6AAB
locret_6AFE         RTS                           ; 6AFE: 39            

; =============== S U B R O U T I N E =======================================
L6AFF               LDA   #$01                    ; 6AFF: 86 01         
                    STA   <$98                    ; 6B01: 97 98         
                    JSR   L615A                   ; 6B03: BD 61 5A        // More stars/ties init stuff
L6B06               JSR   $8341                   ; 6B06: BD 83 41        // Entering trench
L6B09               LDA   #$FF                    ; 6B09: 86 FF         
                    STA   $4B36                   ; 6B0B: B7 4B 36      
                    LDA   $4B19                   ; 6B0E: B6 4B 19      
                    ADDA  $4B17                   ; 6B11: BB 4B 17      
                    CMPA  #$0F                    ; 6B14: 81 0F         
                    BLS   L6B1A                   ; 6B16: 23 02         
                    LDA   #$0F                    ; 6B18: 86 0F         
L6B1A               STA   $4B19                   ; 6B1A: B7 4B 19      
L6B1D               LDA   #$2F                    ; 6B1D: 86 2F         
                    STA   <$41                    ; 6B1F: 97 41           // Game mode/screen state
; End of function sub_6AFF
                    RTS                           ; 6B21: 39            

; =============== S U B R O U T I N E =======================================
; FUNCTION CHUNK AT ROM:6CE1 SIZE 0000000E BYTES
L6B22               LDA   #$00                    ; 6B22: 86 00         
                    STA   $4B3E                   ; 6B24: B7 4B 3E      
                    STA   $4B0E                   ; 6B27: B7 4B 0E        // Attract screen/game phase  timer
                    LDD   #$0300                  ; 6B2A: CC 03 00      
                    STD   $5086                   ; 6B2D: FD 50 86      
                    INC   <$41                    ; 6B30: 0C 41           // Game mode/screen state
L6B32               JSR   L743C                   ; 6B32: BD 74 3C        // Trench
L6B35               LDA   <$60                    ; 6B35: 96 60           // Shield count
                    LBMI  L6CE1                   ; 6B37: 10 2B 01 A6   
                    JSR   $A849                   ; 6B3B: BD A8 49        // Fireball movement
L6B3E               JSR   $AD6C                   ; 6B3E: BD AD 6C      
L6B41               JSR   $8495                   ; 6B41: BD 84 95      
L6B44               JSR   $9886                   ; 6B44: BD 98 86        // Fireball timer 2
L6B47               JSR   $9558                   ; 6B47: BD 95 58        // Process shields
L6B4A               JSR   L70DB                   ; 6B4A: BD 70 DB      
L6B4D               JSR   nullL1                  ; 6B4D: BD 6E A1      
L6B50               LDA   #$10                    ; 6B50: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 6B52: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L6B55               JSR   L703B                   ; 6B55: BD 70 3B        // Trench viewpoint calc
L6B58               LDA   <$43                    ; 6B58: 96 43           // Game over/insert coins timer
                    ANDA  #$0F                    ; 6B5A: 84 0F         
                    BNE   L6B99                   ; 6B5C: 26 3B         
                    LDA   $4B0E                   ; 6B5E: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$02                    ; 6B61: 81 02         
                    BNE   L6B68                   ; 6B63: 26 03         
                    JSR   $BD9E                   ; 6B65: BD BD 9E        // Trench music
L6B68               LDA   $4B12                   ; 6B68: B6 4B 12      
                    LSRA                          ; 6B6B: 44            
                    BCS   L6B83                   ; 6B6C: 25 15         
                    LDA   $4B0E                   ; 6B6E: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$10                    ; 6B71: 81 10         
                    BNE   L6B7A                   ; 6B73: 26 05         
                    JSR   $BD6C                   ; 6B75: BD BD 6C        // Luke trust me
L6B78               BRA   L6B81                   ; 6B78: 20 07         

; ---------------------------------------------------------------------------
L6B7A               CMPA  #$18                    ; 6B7A: 81 18         
                    BNE   L6B81                   ; 6B7C: 26 03         
                    JSR   $BD76                   ; 6B7E: BD BD 76        // Yahoo youre all clear kid
L6B81               BRA   L6B96                   ; 6B81: 20 13         

; ---------------------------------------------------------------------------
L6B83               LDA   $4B0E                   ; 6B83: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$10                    ; 6B86: 81 10         
                    BNE   L6B8F                   ; 6B88: 26 05         
                    JSR   $BD30                   ; 6B8A: BD BD 30      
L6B8D               BRA   L6B96                   ; 6B8D: 20 07         

; ---------------------------------------------------------------------------
L6B8F               CMPA  #$16                    ; 6B8F: 81 16         
                    BNE   L6B96                   ; 6B91: 26 03         
                    JSR   $BD62                   ; 6B93: BD BD 62        // Force is strong in this one
L6B96               INC   $4B0E                   ; 6B96: 7C 4B 0E        // Attract screen/game phase  timer
L6B99               LDA   <$92                    ; 6B99: 96 92         
                    BEQ   locret_6BDA             ; 6B9B: 27 3D         
                    LDD   <$93                    ; 6B9D: DC 93         
                    SUBD  $5098                   ; 6B9F: B3 50 98      
                    SUBD  #$0800                  ; 6BA2: 83 08 00      
                    BHI   locret_6BDA             ; 6BA5: 22 33         
                    LDA   $4845                   ; 6BA7: B6 48 45      
                    BNE   L6BC6                   ; 6BAA: 26 1A         
                    LDA   #$01                    ; 6BAC: 86 01         
                    STA   $4B3E                   ; 6BAE: B7 4B 3E      
                    JSR   $BDB2                   ; 6BB1: BD BD B2        // Explosion
L6BB4               JSR   $9874                   ; 6BB4: BD 98 74      
L6BB7               LDA   <$60                    ; 6BB7: 96 60           // Shield count
                    LBLE  L6CE1                   ; 6BB9: 10 2F 01 24   
                    LDA   #$31                    ; 6BBD: 86 31         
                    STA   <$41                    ; 6BBF: 97 41           // Game mode/screen state
                    JSR   $BD3A                   ; 6BC1: BD BD 3A      
L6BC4               BRA   locret_6BDA             ; 6BC4: 20 14         

; ---------------------------------------------------------------------------
L6BC6               LDA   #$11                    ; 6BC6: 86 11         
                    STA   <$41                    ; 6BC8: 97 41           // Game mode/screen state
                    LDA   $4B15                   ; 6BCA: B6 4B 15      
                    CMPA  #$03                    ; 6BCD: 81 03         
                    BLT   locret_6BDA             ; 6BCF: 2D 09         
                    ANDA  #$01                    ; 6BD1: 84 01         
                    BEQ   locret_6BDA             ; 6BD3: 27 05         
                    JSR   $BD17                   ; 6BD5: BD BD 17      
L6BD8               BRA   locret_6BDA             ; 6BD8: 20 00         

; End of function sub_6B22
locret_6BDA         RTS                           ; 6BDA: 39            

; =============== S U B R O U T I N E =======================================
L6BDB               JSR   L61B5                   ; 6BDB: BD 61 B5        // Set up math constants
L6BDE               JSR   L615A                   ; 6BDE: BD 61 5A        // More stars/ties init stuff
L6BE1               LDA   #$C0                    ; 6BE1: 86 C0         
                    STA   $5080                   ; 6BE3: B7 50 80        // Matrix 4
                    STA   $508A                   ; 6BE6: B7 50 8A      
                    LDA   #$04                    ; 6BE9: 86 04         
                    STA   $4B0E                   ; 6BEB: B7 4B 0E        // Attract screen/game phase  timer
                    INC   <$41                    ; 6BEE: 0C 41           // Game mode/screen state
; End of function sub_6BDB
                    RTS                           ; 6BF0: 39            

; =============== S U B R O U T I N E =======================================
; Death Star explosion complete
L6BF1               LDA   <$43                    ; 6BF1: 96 43           // Game over/insert coins timer
                    ANDA  #$0F                    ; 6BF3: 84 0F         
                    BNE   L6C2C                   ; 6BF5: 26 35         
                    DEC   $4B0E                   ; 6BF7: 7A 4B 0E        // Attract screen/game phase  timer
L6BFA               LDA   $4B0E                   ; 6BFA: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$03                    ; 6BFD: 81 03         
                    BNE   L6C09                   ; 6BFF: 26 08         
                    LDA   $4845                   ; 6C01: B6 48 45      
                    BEQ   L6C09                   ; 6C04: 27 03         
                    JSR   $9806                   ; 6C06: BD 98 06        // Exhaust port score
L6C09               LDA   $4B0E                   ; 6C09: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$02                    ; 6C0C: 81 02         
                    BNE   L6C13                   ; 6C0E: 26 03         
                    JSR   $9775                   ; 6C10: BD 97 75        // Shield bonus score
L6C13               LDA   $4B0E                   ; 6C13: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$01                    ; 6C16: 81 01         
                    BNE   L6C22                   ; 6C18: 26 08         
                    LDA   $4845                   ; 6C1A: B6 48 45      
                    BEQ   L6C22                   ; 6C1D: 27 03         
                    JSR   L953B                   ; 6C1F: BD 95 3B      
L6C22               LDA   $4B0E                   ; 6C22: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$00                    ; 6C25: 81 00         
                    BNE   L6C2C                   ; 6C27: 26 03         
                    JSR   $9722                   ; 6C29: BD 97 22        // Death Star starting wave bonus score
L6C2C               JSR   L7519                   ; 6C2C: BD 75 19      
L6C2F               JSR   $9558                   ; 6C2F: BD 95 58        // Process shields
L6C32               JSR   L6F5F                   ; 6C32: BD 6F 5F      
L6C35               LDA   $4B0E                   ; 6C35: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$FE                    ; 6C38: 81 FE         
                    BNE   locret_6C75             ; 6C3A: 26 39         
                    LDA   $4B15                   ; 6C3C: B6 4B 15      
                    INCA                          ; 6C3F: 4C            
                    CMPA  #$62                    ; 6C40: 81 62         
                    BLS   L6C46                   ; 6C42: 23 02         
                    LDA   #$62                    ; 6C44: 86 62         
L6C46               STA   $4B15                   ; 6C46: B7 4B 15      
                    LDA   $4B15                   ; 6C49: B6 4B 15      
                    CMPA  #$05                    ; 6C4C: 81 05         
                    BCC   L6C5D                   ; 6C4E: 24 0D         
                    LDA   $4B17                   ; 6C50: B6 4B 17      
                    INCA                          ; 6C53: 4C            
                    CMPA  #$04                    ; 6C54: 81 04         
                    BLS   L6C5A                   ; 6C56: 23 02         
                    LDA   #$04                    ; 6C58: 86 04         
L6C5A               STA   $4B17                   ; 6C5A: B7 4B 17      
L6C5D               LDA   $4B18                   ; 6C5D: B6 4B 18      
                    ADDA  $4B17                   ; 6C60: BB 4B 17      
                    CMPA  #$0F                    ; 6C63: 81 0F         
                    BLS   L6C69                   ; 6C65: 23 02         
                    LDA   #$0F                    ; 6C67: 86 0F         
L6C69               STA   $4B18                   ; 6C69: B7 4B 18      
                    LDA   #$FF                    ; 6C6C: 86 FF         
                    STA   $4B2D                   ; 6C6E: B7 4B 2D      
                    LDA   #$1D                    ; 6C71: 86 1D         
                    STA   <$41                    ; 6C73: 97 41           // Game mode/screen state
; End of function sub_6BF1
locret_6C75         RTS                           ; 6C75: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_6838
L6C76               JSR   $BD58                   ; 6C76: BD BD 58        // Stay in attack formation
L6C79               LDA   #$36                    ; 6C79: 86 36         
                    STA   <$41                    ; 6C7B: 97 41           // Game mode/screen state
                    LDD   #$00                    ; 6C7D: CC 00 00      
                    STD   $4B0E                   ; 6C80: FD 4B 0E        // Attract screen/game phase  timer
; END OF FUNCTION CHUNK FOR sub_6838
                    RTS                           ; 6C83: 39            

; =============== S U B R O U T I N E =======================================
; Towers
L6C84               JSR   L7315                   ; 6C84: BD 73 15      
L6C87               JSR   $A849                   ; 6C87: BD A8 49        // Fireball movement
L6C8A               JSR   $987F                   ; 6C8A: BD 98 7F        // Fireball timer
L6C8D               JSR   $9898                   ; 6C8D: BD 98 98      
L6C90               JSR   $B98B                   ; 6C90: BD B9 8B        // Check if tie/bunker/tower been hit
L6C93               LDD   #$FB01                  ; 6C93: CC FB 01      
                    STD   $5022                   ; 6C96: FD 50 22        // Sine for rotation
                    LDD   #$3FCE                  ; 6C99: CC 3F CE      
                    STD   $5024                   ; 6C9C: FD 50 24        // Cosine for rotation
                    JSR   $CE24                   ; 6C9F: BD CE 24        // Run math program $00 Roll on Matrix 4
L6CA2               LDD   $4B0E                   ; 6CA2: FC 4B 0E        // Attract screen/game phase  timer
                    ADDD  #$01                    ; 6CA5: C3 00 01      
                    STD   $4B0E                   ; 6CA8: FD 4B 0E        // Attract screen/game phase  timer
                    CMPD  #$28                    ; 6CAB: 10 83 00 28   
                    BCS   locret_6CB5             ; 6CAF: 25 04         
                    LDA   #$3B                    ; 6CB1: 86 3B         
                    STA   <$41                    ; 6CB3: 97 41           // Game mode/screen state
; End of function sub_6C84
locret_6CB5         RTS                           ; 6CB5: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_69F4
L6CB6               JSR   $BD58                   ; 6CB6: BD BD 58        // Stay in attack formation
L6CB9               LDA   #$38                    ; 6CB9: 86 38         
                    STA   <$41                    ; 6CBB: 97 41           // Game mode/screen state
                    LDD   #$00                    ; 6CBD: CC 00 00      
                    STD   $4B0E                   ; 6CC0: FD 4B 0E        // Attract screen/game phase  timer
; END OF FUNCTION CHUNK FOR sub_69F4
                    RTS                           ; 6CC3: 39            

; =============== S U B R O U T I N E =======================================
L6CC4               JSR   L73C3                   ; 6CC4: BD 73 C3        // Towers
L6CC7               JSR   $A849                   ; 6CC7: BD A8 49        // Fireball movement
L6CCA               JSR   $987F                   ; 6CCA: BD 98 7F        // Fireball timer
L6CCD               LDD   $4B0E                   ; 6CCD: FC 4B 0E        // Attract screen/game phase  timer
                    ADDD  #$01                    ; 6CD0: C3 00 01      
                    STD   $4B0E                   ; 6CD3: FD 4B 0E        // Attract screen/game phase  timer
                    CMPD  #$28                    ; 6CD6: 10 83 00 28   
                    BCS   locret_6CE0             ; 6CDA: 25 04         
                    LDA   #$3B                    ; 6CDC: 86 3B         
                    STA   <$41                    ; 6CDE: 97 41           // Game mode/screen state
; End of function sub_6CC4
locret_6CE0         RTS                           ; 6CE0: 39            

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR sub_6B22
L6CE1               JSR   $BD58                   ; 6CE1: BD BD 58        // Stay in attack formation
L6CE4               LDA   #$3A                    ; 6CE4: 86 3A         
                    STA   <$41                    ; 6CE6: 97 41           // Game mode/screen state
                    LDD   #$00                    ; 6CE8: CC 00 00      
                    STD   $4B0E                   ; 6CEB: FD 4B 0E        // Attract screen/game phase  timer
; END OF FUNCTION CHUNK FOR sub_6B22
                    RTS                           ; 6CEE: 39            

; =============== S U B R O U T I N E =======================================
; Game over
L6CEF               JSR   L74D5                   ; 6CEF: BD 74 D5        // Game over
L6CF2               JSR   $A849                   ; 6CF2: BD A8 49        // Fireball movement
L6CF5               JSR   $987F                   ; 6CF5: BD 98 7F        // Fireball timer
L6CF8               LDD   $4B0E                   ; 6CF8: FC 4B 0E        // Attract screen/game phase  timer
                    ADDD  #$01                    ; 6CFB: C3 00 01      
                    STD   $4B0E                   ; 6CFE: FD 4B 0E        // Attract screen/game phase  timer
                    CMPD  #$28                    ; 6D01: 10 83 00 28   
                    BCS   locret_6D0B             ; 6D05: 25 04         
                    LDA   #$3B                    ; 6D07: 86 3B         
                    STA   <$41                    ; 6D09: 97 41           // Game mode/screen state
; End of function sub_6CEF
locret_6D0B         RTS                           ; 6D0B: 39            

; =============== S U B R O U T I N E =======================================
; Game over init
L6D0C               JSR   $BD49                   ; 6D0C: BD BD 49        // Remember
L6D0F               JSR   $BD0D                   ; 6D0F: BD BD 0D      
L6D12               INC   <$41                    ; 6D12: 0C 41           // Game mode/screen state
; End of function sub_6D0C
                    RTS                           ; 6D14: 39            

; =============== S U B R O U T I N E =======================================
; Game over
L6D15               JSR   L61B5                   ; 6D15: BD 61 B5        // Set up math constants
L6D18               JSR   L615A                   ; 6D18: BD 61 5A        // More stars/ties init stuff
L6D1B               JSR   L61EC                   ; 6D1B: BD 61 EC        // Init stars math data
L6D1E               LDU   #$5038                  ; 6D1E: CE 50 38        // Matrix 2
                    JSR   $CDC3                   ; 6D21: BD CD C3        // Initialise math registers matrix
L6D24               JSR   $C0FF                   ; 6D24: BD C0 FF        // NVRAM something
L6D27               JSR   $CA8C                   ; 6D27: BD CA 8C        // Score
L6D2A               LDA   $4AEC                   ; 6D2A: B6 4A EC      
                    BMI   L6D33                   ; 6D2D: 2B 04         
                    LDA   #$0F                    ; 6D2F: 86 0F         
                    BRA   L6D38                   ; 6D31: 20 05         

; ---------------------------------------------------------------------------
L6D33               JSR   $BD7B                   ; 6D33: BD BD 7B        // High score
L6D36               LDA   #$05                    ; 6D36: 86 05         
L6D38               STA   <$41                    ; 6D38: 97 41           // Game mode/screen state
; End of function sub_6D15
                    RTS                           ; 6D3A: 39            

; =============== S U B R O U T I N E =======================================
L6D3B               LDD   #$7304                  ; 6D3B: CC 73 04        // Exhaust port hit init
                    STD   <$56                    ; 6D3E: DD 56           // Zoom value
                    LDD   #$0AFF                  ; 6D40: CC 0A FF      
                    STD   <$58                    ; 6D43: DD 58           // Death Star zoom value
                    JSR   L61EC                   ; 6D45: BD 61 EC        // Init stars math data
L6D48               LDU   #$5038                  ; 6D48: CE 50 38        // Matrix 2
                    JSR   $CDC3                   ; 6D4B: BD CD C3        // Initialise math registers matrix
L6D4E               INC   <$41                    ; 6D4E: 0C 41           // Game mode/screen state
                    JSR   $BD8F                   ; 6D50: BD BD 8F        // Death Star destroyed
; End of function sub_6D3B
L6D53               RTS                           ; 6D53: 39            

; =============== S U B R O U T I N E =======================================
L6D54               JSR   L75B9                   ; 6D54: BD 75 B9        // Death Star hit zoom out
L6D57               JSR   $9558                   ; 6D57: BD 95 58        // Process shields
L6D5A               JSR   $9890                   ; 6D5A: BD 98 90        // Fireball timer 3
L6D5D               LDB   <$58                    ; 6D5D: D6 58           // Death Star zoom value
                    SEX                           ; 6D5F: 1D            
                    ADDD  <$56                    ; 6D60: D3 56           // Zoom value
                    ADDD  #$80                    ; 6D62: C3 00 80      
                    ANDB  #$7F                    ; 6D65: C4 7F         
                    STD   <$56                    ; 6D67: DD 56           // Zoom value
                    CMPD  #$7680                  ; 6D69: 10 83 76 80   
                    BCS   L6D73                   ; 6D6D: 25 04         
                    LDA   #$13                    ; 6D6F: 86 13         
                    STA   <$41                    ; 6D71: 97 41           // Game mode/screen state
L6D73               LDD   <$58                    ; 6D73: DC 58           // Death Star zoom value
                    SUBD  #$10                    ; 6D75: 83 00 10      
                    BPL   L6D7D                   ; 6D78: 2A 03         
                    LDD   #$00                    ; 6D7A: CC 00 00      
L6D7D               STD   <$58                    ; 6D7D: DD 58           // Death Star zoom value
; End of function sub_6D54
                    RTS                           ; 6D7F: 39            

; =============== S U B R O U T I N E =======================================
L6D80               JSR   $BB7B                   ; 6D80: BD BB 7B        // Death Star explosion init
L6D83               INC   <$41                    ; 6D83: 0C 41           // Game mode/screen state
; End of function sub_6D80
                    RTS                           ; 6D85: 39            

; =============== S U B R O U T I N E =======================================
; Space wave init
L6D86               JSR   L75D9                   ; 6D86: BD 75 D9        // Space wave
L6D89               LDA   $48A1                   ; 6D89: B6 48 A1      
                    CMPA  #$01                    ; 6D8C: 81 01         
                    BCS   locret_6D94             ; 6D8E: 25 04         
                    LDA   #$15                    ; 6D90: 86 15         
                    STA   <$41                    ; 6D92: 97 41           // Game mode/screen state
; End of function sub_6D86
locret_6D94         RTS                           ; 6D94: 39            

; =============== S U B R O U T I N E =======================================
L6D95               INC   <$41                    ; 6D95: 0C 41           // Game mode/screen state
; End of function sub_6D95
                    RTS                           ; 6D97: 39            

; =============== S U B R O U T I N E =======================================
L6D98               JSR   L760A                   ; 6D98: BD 76 0A        // Death Star explosion
L6D9B               LDA   $48A1                   ; 6D9B: B6 48 A1      
                    BNE   locret_6DA4             ; 6D9E: 26 04         
                    LDA   #$33                    ; 6DA0: 86 33         
                    STA   <$41                    ; 6DA2: 97 41           // Game mode/screen state
; End of function sub_6D98
locret_6DA4         RTS                           ; 6DA4: 39            

; =============== S U B R O U T I N E =======================================
; Attract screen 2 stars forward and down translate
L6DA5               LDD   <$89                    ; 6DA5: DC 89         
                    ADDD  #$80                    ; 6DA7: C3 00 80      
                    STD   <$89                    ; 6DAA: DD 89         
                    LDD   $4B26                   ; 6DAC: FC 4B 26      
                    ADDD  #$80                    ; 6DAF: C3 00 80      
                    STD   $4B26                   ; 6DB2: FD 4B 26      
; End of function sub_6DA5
                    RTS                           ; 6DB5: 39            

; =============== S U B R O U T I N E =======================================
; Attract screen 3 stars YT move
L6DB6               LDD   $4B24                   ; 6DB6: FC 4B 24      
                    ADDD  #$FF80                  ; 6DB9: C3 FF 80      
                    STD   $4B24                   ; 6DBC: FD 4B 24      
; End of function sub_6DB6
                    RTS                           ; 6DBF: 39            

; =============== S U B R O U T I N E =======================================
; Attract screen 4 stars ZT move
L6DC0               LDD   $4B26                   ; 6DC0: FC 4B 26      
                    ADDD  #$80                    ; 6DC3: C3 00 80      
                    STD   $4B26                   ; 6DC6: FD 4B 26      
; End of function sub_6DC0
                    RTS                           ; 6DC9: 39            

; =============== S U B R O U T I N E =======================================
; Move stars XT translate position
L6DCA               LDD   <$89                    ; 6DCA: DC 89         
                    ADDD  #$80                    ; 6DCC: C3 00 80      
                    STD   <$89                    ; 6DCF: DD 89         
; End of function sub_6DCA
                    RTS                           ; 6DD1: 39            

; =============== S U B R O U T I N E =======================================
L6DD2               LDA   <$63                    ; 6DD2: 96 63         
                    BEQ   L6DF0                   ; 6DD4: 27 1A         
                    BLE   L6DDF                   ; 6DD6: 2F 07         
                    DEC   <$63                    ; 6DD8: 0A 63         
                    LDD   #$04FF                  ; 6DDA: CC 04 FF      
                    BRA   L6DE4                   ; 6DDD: 20 05         

; ---------------------------------------------------------------------------
L6DDF               INC   <$63                    ; 6DDF: 0C 63         
                    LDD   #$FB01                  ; 6DE1: CC FB 01      
L6DE4               STD   $5022                   ; 6DE4: FD 50 22        // Sine for rotation
                    LDD   #$3FCE                  ; 6DE7: CC 3F CE      
                    STD   $5024                   ; 6DEA: FD 50 24        // Cosine for rotation
                    JSR   $CE24                   ; 6DED: BD CE 24        // Run math program $00 Roll on Matrix 4
L6DF0               JSR   L6EA2                   ; 6DF0: BD 6E A2      
L6DF3               JSR   L70BD                   ; 6DF3: BD 70 BD      
L6DF6               JSR   L70CC                   ; 6DF6: BD 70 CC      
; End of function sub_6DD2
L6DF9               RTS                           ; 6DF9: 39            

; =============== S U B R O U T I N E =======================================
L6DFA               LDA   <$63                    ; 6DFA: 96 63         
                    BEQ   L6E18                   ; 6DFC: 27 1A         
                    BLE   L6E07                   ; 6DFE: 2F 07         
                    DEC   <$63                    ; 6E00: 0A 63         
                    LDD   #$04FF                  ; 6E02: CC 04 FF      
                    BRA   L6E0C                   ; 6E05: 20 05         

; ---------------------------------------------------------------------------
L6E07               INC   <$63                    ; 6E07: 0C 63         
                    LDD   #$FB01                  ; 6E09: CC FB 01      
L6E0C               STD   $5022                   ; 6E0C: FD 50 22        // Sine for rotation
                    LDD   #$3FCE                  ; 6E0F: CC 3F CE      
                    STD   $5024                   ; 6E12: FD 50 24        // Cosine for rotation
                    JSR   $CE24                   ; 6E15: BD CE 24        // Run math program $00 Roll on Matrix 4
L6E18               JSR   L6ECB                   ; 6E18: BD 6E CB      
L6E1B               JSR   L70BD                   ; 6E1B: BD 70 BD      
L6E1E               JSR   L70CC                   ; 6E1E: BD 70 CC      
; End of function sub_6DFA
L6E21               RTS                           ; 6E21: 39            

; =============== S U B R O U T I N E =======================================
L6E22               LDA   <$63                    ; 6E22: 96 63         
                    BEQ   L6E2E                   ; 6E24: 27 08         
                    BLE   L6E2B                   ; 6E26: 2F 03         
                    DECA                          ; 6E28: 4A            
                    BRA   L6E2C                   ; 6E29: 20 01         

; ---------------------------------------------------------------------------
L6E2B               INCA                          ; 6E2B: 4C            
L6E2C               STA   <$63                    ; 6E2C: 97 63         
L6E2E               LDA   <$63                    ; 6E2E: 96 63         
                    BPL   L6E33                   ; 6E30: 2A 01         
                    NEGA                          ; 6E32: 40            
L6E33               LDB   #$20                    ; 6E33: C6 20         
                    MUL                           ; 6E35: 3D            
                    TST   <$63                    ; 6E36: 0D 63         
                    BPL   L6E3E                   ; 6E38: 2A 04         
                    COMA                          ; 6E3A: 43            
                    NEGB                          ; 6E3B: 50            
                    SBCA  #$FF                    ; 6E3C: 82 FF         
L6E3E               STD   <$A5                    ; 6E3E: DD A5         
                    LDA   <$7D                    ; 6E40: 96 7D           // Joystick X
                    BPL   L6E45                   ; 6E42: 2A 01         
                    COMA                          ; 6E44: 43            
L6E45               LDB   #$02                    ; 6E45: C6 02         
                    MUL                           ; 6E47: 3D            
                    TST   <$7D                    ; 6E48: 0D 7D           // Joystick X
                    BPL   L6E50                   ; 6E4A: 2A 04         
                    COMA                          ; 6E4C: 43            
                    NEGB                          ; 6E4D: 50            
                    SBCA  #$FF                    ; 6E4E: 82 FF         
L6E50               ADDD  <$A5                    ; 6E50: D3 A5         
                    TST   <$63                    ; 6E52: 0D 63         
                    BNE   L6E70                   ; 6E54: 26 1A           // Towers collision roll limits
                    SUBD  <$A3                    ; 6E56: 93 A3         
                    BLE   L6E65                   ; 6E58: 2F 0B         
                    CMPD  #$10                    ; 6E5A: 10 83 00 10   
                    BLE   L6E63                   ; 6E5E: 2F 03         
                    LDD   #$10                    ; 6E60: CC 00 10      
L6E63               BRA   L6E6E                   ; 6E63: 20 09         

; ---------------------------------------------------------------------------
L6E65               CMPD  #$FFF0                  ; 6E65: 10 83 FF F0   
                    BGE   L6E6E                   ; 6E69: 2C 03         
                    LDD   #$FFF0                  ; 6E6B: CC FF F0      
; End of function sub_6E22
L6E6E               BRA   L6E88                   ; 6E6E: 20 18         

; =============== S U B R O U T I N E =======================================
; Towers collision roll limits
L6E70               SUBD  <$A3                    ; 6E70: 93 A3         
                    BLE   L6E7F                   ; 6E72: 2F 0B         
                    CMPD  #$32                    ; 6E74: 10 83 00 32   
                    BLE   L6E7D                   ; 6E78: 2F 03         
                    LDD   #$32                    ; 6E7A: CC 00 32      
L6E7D               BRA   L6E88                   ; 6E7D: 20 09         

; ---------------------------------------------------------------------------
L6E7F               CMPD  #$FFCE                  ; 6E7F: 10 83 FF CE   
                    BGE   L6E88                   ; 6E83: 2C 03         
                    LDD   #$FFCE                  ; 6E85: CC FF CE      
L6E88               TFR   B,A                     ; 6E88: 1F 98         
                    ADDA  $4878                   ; 6E8A: BB 48 78      
                    STA   $4878                   ; 6E8D: B7 48 78      
                    SEX                           ; 6E90: 1D            
                    ADDD  <$A3                    ; 6E91: D3 A3         
                    STD   <$A3                    ; 6E93: DD A3         
                    LDX   #$4870                  ; 6E95: 8E 48 70      
                    JSR   L7111                   ; 6E98: BD 71 11        // Some joystick driven matrix rotations for pitch/roll
L6E9B               BEQ   locret_6EA0             ; 6E9B: 27 03         
                    JSR   $CE24                   ; 6E9D: BD CE 24        // Run math program $00 Roll on Matrix 4
; End of function sub_6E70
locret_6EA0         RTS                           ; 6EA0: 39            

; =============== S U B R O U T I N E =======================================
; End of function nullsub_1
nullL1              RTS                           ; 6EA1: 39            

; =============== S U B R O U T I N E =======================================
L6EA2               LDX   $4B32                   ; 6EA2: BE 4B 32      
                    BNE   L6EAA                   ; 6EA5: 26 03         
                    LDX   #$4900                  ; 6EA7: 8E 49 00        // 3x Tie fighter data structure ($19 bytes per Tie)
L6EAA               LDA   $03,X                   ; 6EAA: A6 03         
                    CMPA  #$01                    ; 6EAC: 81 01         
                    BNE   L6EB9                   ; 6EAE: 26 09         
                    LDA   $06,X                   ; 6EB0: A6 06         
                    BNE   L6EB9                   ; 6EB2: 26 05         
                    STX   <$64                    ; 6EB4: 9F 64           // Pointer to Tie fighter data
                    JMP   L6EF7                   ; 6EB6: 7E 6E F7        // Point BIC to 5080

; ---------------------------------------------------------------------------
L6EB9               LDA   $4B3C                   ; 6EB9: B6 4B 3C      
                    BLE   L6EC3                   ; 6EBC: 2F 05         
                    LDA   #$09                    ; 6EBE: 86 09         
                    STA   $4B3C                   ; 6EC0: B7 4B 3C      
L6EC3               LEAX  $19,X                   ; 6EC3: 30 88 19      
                    CMPX  #$494B                  ; 6EC6: 8C 49 4B        // 3x Tie fighter data structure ($19 bytes per Tie)
                    BCS   L6EAA                   ; 6EC9: 25 DF         
L6ECB               LDD   #$00                    ; 6ECB: CC 00 00      
                    STD   $4B32                   ; 6ECE: FD 4B 32      
                    LDA   $5080                   ; 6ED1: B6 50 80        // Matrix 4
                    BMI   L6EDB                   ; 6ED4: 2B 05         
                    LDB   $5084                   ; 6ED6: F6 50 84      
                    BRA   L6EE0                   ; 6ED9: 20 05         

; ---------------------------------------------------------------------------
L6EDB               LDB   #$7F                    ; 6EDB: C6 7F         
                    SUBB  $5084                   ; 6EDD: F0 50 84      
L6EE0               STB   $486D                   ; 6EE0: F7 48 6D      
                    LDA   $5080                   ; 6EE3: B6 50 80        // Matrix 4
                    BMI   L6EED                   ; 6EE6: 2B 05         
                    LDB   $5082                   ; 6EE8: F6 50 82      
                    BRA   L6EF2                   ; 6EEB: 20 05         

; ---------------------------------------------------------------------------
L6EED               LDB   #$7F                    ; 6EED: C6 7F         
                    SUBB  $5082                   ; 6EEF: F0 50 82      
L6EF2               COMB                          ; 6EF2: 53            
                    STB   $4876                   ; 6EF3: F7 48 76      
                    RTS                           ; 6EF6: 39            

; ---------------------------------------------------------------------------
L6EF7               LDA   #$10                    ; 6EF7: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 6EF9: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L6EFC               JSR   L7160                   ; 6EFC: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L6EFF               LDX   <$64                    ; 6EFF: 9E 64           // Pointer to Tie fighter data
                    STX   $4B32                   ; 6F01: BF 4B 32      
                    CLRA                          ; 6F04: 4F            
                    LDB   $02,X                   ; 6F05: E6 02         
                    ADDB  #$03                    ; 6F07: CB 03         
                    STD   $4701                   ; 6F09: FD 47 01      
                    LDA   #$67                    ; 6F0C: 86 67         
                    JSR   $CDBA                   ; 6F0E: BD CD BA        // Do math program run
L6F11               LDA   $5000                   ; 6F11: B6 50 00        // Math result X
                    BLE   L6F39                   ; 6F14: 2F 23         
                    LDD   $5002                   ; 6F16: FC 50 02        // Math result Y
L6F19               ASL   $5001                   ; 6F19: 78 50 01        // Math result X
                    ROL   $5000                   ; 6F1C: 79 50 00        // Math result X
                    BVS   L6F54                   ; 6F1F: 29 33         
                    ASLB                          ; 6F21: 58            
                    ROLA                          ; 6F22: 49            
                    BVC   L6F29                   ; 6F23: 28 04         
                    RORA                          ; 6F25: 46            
                    RORB                          ; 6F26: 56            
                    BRA   L6F54                   ; 6F27: 20 2B         

; ---------------------------------------------------------------------------
L6F29               ASL   $5005                   ; 6F29: 78 50 05        // Math result Z
                    ROL   $5004                   ; 6F2C: 79 50 04        // Math result Z
                    BVC   L6F19                   ; 6F2F: 28 E8         
                    ROR   $5004                   ; 6F31: 76 50 04        // Math result Z
                    ROR   $5005                   ; 6F34: 76 50 05        // Math result Z
                    BRA   L6F54                   ; 6F37: 20 1B         

; ---------------------------------------------------------------------------
L6F39               LDD   $5002                   ; 6F39: FC 50 02        // Math result Y
                    ORB   #$01                    ; 6F3C: CA 01         
L6F3E               ASLB                          ; 6F3E: 58            
                    ROLA                          ; 6F3F: 49            
                    BVC   L6F46                   ; 6F40: 28 04         
                    RORA                          ; 6F42: 46            
                    RORB                          ; 6F43: 56            
                    BRA   L6F54                   ; 6F44: 20 0E         

; ---------------------------------------------------------------------------
L6F46               ASL   $5005                   ; 6F46: 78 50 05        // Math result Z
                    ROL   $5004                   ; 6F49: 79 50 04        // Math result Z
                    BVC   L6F3E                   ; 6F4C: 28 F0         
                    ROR   $5004                   ; 6F4E: 76 50 04        // Math result Z
                    ROR   $5005                   ; 6F51: 76 50 05        // Math result Z
L6F54               COMA                          ; 6F54: 43            
                    STA   $4876                   ; 6F55: B7 48 76      
                    LDB   $5004                   ; 6F58: F6 50 04        // Math result Z
                    STB   $486D                   ; 6F5B: F7 48 6D      
; End of function sub_6EA2
                    RTS                           ; 6F5E: 39            

; =============== S U B R O U T I N E =======================================
L6F5F               LDD   <$42                    ; 6F5F: DC 42         
                    JSR   $CDAB                   ; 6F61: BD CD AB      
L6F64               STD   <$89                    ; 6F64: DD 89         
; End of function sub_6F5F
                    RTS                           ; 6F66: 39            

; =============== S U B R O U T I N E =======================================
L6F67               LDD   <$42                    ; 6F67: DC 42         
                    JSR   $CDA9                   ; 6F69: BD CD A9        // Shift D register left
L6F6C               STD   <$89                    ; 6F6C: DD 89         
; End of function sub_6F67
                    RTS                           ; 6F6E: 39            

; =============== S U B R O U T I N E =======================================
L6F6F               LDD   $5086                   ; 6F6F: FC 50 86      
                    ADDD  $5098                   ; 6F72: F3 50 98      
                    BVC   L6F7D                   ; 6F75: 28 06         
                    INC   <$A7                    ; 6F77: 0C A7         
                    BVC   L6F7D                   ; 6F79: 28 02         
                    DEC   <$A7                    ; 6F7B: 0A A7         
L6F7D               STD   $5098                   ; 6F7D: FD 50 98      
                    STD   $5040                   ; 6F80: FD 50 40        // XT2
                    LDD   $5086                   ; 6F83: FC 50 86      
                    JSR   $CDB1                   ; 6F86: BD CD B1      
L6F89               LDB   <$7D                    ; 6F89: D6 7D           // Joystick X
                    BPL   L6F8E                   ; 6F8B: 2A 01         
                    COMB                          ; 6F8D: 53            
L6F8E               ASLB                          ; 6F8E: 58            
                    MUL                           ; 6F8F: 3D            
                    TST   <$7D                    ; 6F90: 0D 7D           // Joystick X
                    BGE   L6F98                   ; 6F92: 2C 04         
                    COMA                          ; 6F94: 43            
                    NEGB                          ; 6F95: 50            
                    SBCA  #$FF                    ; 6F96: 82 FF         
L6F98               JSR   $CDA0                   ; 6F98: BD CD A0      
L6F9B               STD   $508E                   ; 6F9B: FD 50 8E      
                    ADDD  $509A                   ; 6F9E: F3 50 9A      
                    STD   $509A                   ; 6FA1: FD 50 9A      
                    STD   $5042                   ; 6FA4: FD 50 42        // YT2
                    LDD   $5086                   ; 6FA7: FC 50 86      
                    JSR   $CDB1                   ; 6FAA: BD CD B1      
L6FAD               LDB   <$7F                    ; 6FAD: D6 7F           // Joystick Y
                    BPL   L6FB2                   ; 6FAF: 2A 01         
                    COMB                          ; 6FB1: 53            
L6FB2               NOP                           ; 6FB2: 12            
                    MUL                           ; 6FB3: 3D            
                    TST   <$7F                    ; 6FB4: 0D 7F           // Joystick Y
                    BPL   L6FBC                   ; 6FB6: 2A 04         
                    COMA                          ; 6FB8: 43            
                    NEGB                          ; 6FB9: 50            
                    SBCA  #$FF                    ; 6FBA: 82 FF         
L6FBC               JSR   $CDA0                   ; 6FBC: BD CD A0      
L6FBF               STD   $5096                   ; 6FBF: FD 50 96      
                    ADDD  $509C                   ; 6FC2: F3 50 9C      
                    CMPD  #$1C00                  ; 6FC5: 10 83 1C 00   
                    BLE   L6FD0                   ; 6FC9: 2F 05         
                    LDD   #$1C00                  ; 6FCB: CC 1C 00      
                    BRA   L6FD9                   ; 6FCE: 20 09         

; ---------------------------------------------------------------------------
L6FD0               CMPD  #$0200                  ; 6FD0: 10 83 02 00   
                    BGE   L6FD9                   ; 6FD4: 2C 03         
                    LDD   #$0200                  ; 6FD6: CC 02 00      
L6FD9               STD   $509C                   ; 6FD9: FD 50 9C      
                    STD   $5044                   ; 6FDC: FD 50 44        // ZT2
; End of function sub_6F6F
                    RTS                           ; 6FDF: 39            

; =============== S U B R O U T I N E =======================================
L6FE0               LDD   $509C                   ; 6FE0: FC 50 9C      
                    CMPD  #$0380                  ; 6FE3: 10 83 03 80   
                    BLE   L6FEF                   ; 6FE7: 2F 06         
                    SUBD  #$0180                  ; 6FE9: 83 01 80      
                    STD   $509C                   ; 6FEC: FD 50 9C      
; End of function sub_6FE0
L6FEF               BRA   L7000                   ; 6FEF: 20 0F         

; =============== S U B R O U T I N E =======================================
L6FF1               LDD   $509C                   ; 6FF1: FC 50 9C      
                    CMPD  #$F300                  ; 6FF4: 10 83 F3 00   
                    BLE   L7000                   ; 6FF8: 2F 06         
                    SUBD  #$0100                  ; 6FFA: 83 01 00      
                    STD   $509C                   ; 6FFD: FD 50 9C      
L7000               LDD   $5086                   ; 7000: FC 50 86      
                    ADDD  $5098                   ; 7003: F3 50 98      
                    STD   $5098                   ; 7006: FD 50 98      
                    LDD   #$0300                  ; 7009: CC 03 00      
                    SUBD  $5086                   ; 700C: B3 50 86      
                    JSR   $CDA2                   ; 700F: BD CD A2      
L7012               ADDD  $5086                   ; 7012: F3 50 86      
                    STD   $5086                   ; 7015: FD 50 86      
                    LDA   $4B15                   ; 7018: B6 4B 15      
                    LSRA                          ; 701B: 44            
                    BCC   L7023                   ; 701C: 24 05           // Towers roll calcs
                    LDD   #$0BB8                  ; 701E: CC 0B B8      
                    BRA   L7026                   ; 7021: 20 03         

; ---------------------------------------------------------------------------
L7023               LDD   #$F448                  ; 7023: CC F4 48        // Towers roll calcs
L7026               STD   $5022                   ; 7026: FD 50 22        // Sine for rotation
                    LDD   #$3EEB                  ; 7029: CC 3E EB      
                    STD   $5024                   ; 702C: FD 50 24        // Cosine for rotation
                    JSR   $CE24                   ; 702F: BD CE 24        // Run math program $00 Roll on Matrix 4
L7032               LDD   #$00                    ; 7032: CC 00 00      
                    SUBD  <$A3                    ; 7035: 93 A3         
                    JSR   L6E70                   ; 7037: BD 6E 70        // Towers collision roll limits
; End of function sub_6FF1
L703A               RTS                           ; 703A: 39            

; =============== S U B R O U T I N E =======================================
; Trench viewpoint calc
L703B               LDD   $5086                   ; 703B: FC 50 86      
                    ADDD  $5098                   ; 703E: F3 50 98      
                    STD   $5098                   ; 7041: FD 50 98      
                    STD   $5040                   ; 7044: FD 50 40        // XT2
                    LDD   $5086                   ; 7047: FC 50 86      
                    JSR   $CDB1                   ; 704A: BD CD B1      
L704D               LDB   <$7D                    ; 704D: D6 7D           // Joystick X
                    BPL   L7052                   ; 704F: 2A 01         
                    COMB                          ; 7051: 53            
L7052               MUL                           ; 7052: 3D            
L7053               TST   <$7D                    ; 7053: 0D 7D           // Joystick X
                    BGE   L705B                   ; 7055: 2C 04         
                    COMA                          ; 7057: 43            
                    NEGB                          ; 7058: 50            
                    SBCA  #$FF                    ; 7059: 82 FF         
L705B               JSR   $CDA0                   ; 705B: BD CD A0        // Trench X min/max limits
L705E               STD   $508E                   ; 705E: FD 50 8E      
                    ADDD  $509A                   ; 7061: F3 50 9A      
                    CMPD  #$01FF                  ; 7064: 10 83 01 FF   
                    BLE   L706D                   ; 7068: 2F 03         
                    LDD   #$01FF                  ; 706A: CC 01 FF      
L706D               CMPD  #$FE01                  ; 706D: 10 83 FE 01   
                    BGE   L7076                   ; 7071: 2C 03         
                    LDD   #$FE01                  ; 7073: CC FE 01      
L7076               STD   $509A                   ; 7076: FD 50 9A      
                    STD   $5042                   ; 7079: FD 50 42        // YT2
                    LDD   $5086                   ; 707C: FC 50 86      
                    ASLB                          ; 707F: 58            
                    ROLA                          ; 7080: 49            
                    ASLB                          ; 7081: 58            
                    ROLA                          ; 7082: 49            
                    ASLB                          ; 7083: 58            
                    ROLA                          ; 7084: 49            
                    ASLB                          ; 7085: 58            
                    ROLA                          ; 7086: 49            
                    LDB   <$7F                    ; 7087: D6 7F           // Joystick Y
                    BPL   L708C                   ; 7089: 2A 01         
                    COMB                          ; 708B: 53            
L708C               ASLB                          ; 708C: 58            
                    MUL                           ; 708D: 3D            
                    TST   <$7F                    ; 708E: 0D 7F           // Joystick Y
                    BPL   L7096                   ; 7090: 2A 04         
                    COMA                          ; 7092: 43            
                    NEGB                          ; 7093: 50            
                    SBCA  #$FF                    ; 7094: 82 FF         
L7096               ASRA                          ; 7096: 47            
                    RORB                          ; 7097: 56            
                    ASRA                          ; 7098: 47            
                    RORB                          ; 7099: 56            
                    ASRA                          ; 709A: 47            
                    RORB                          ; 709B: 56            
                    ASRA                          ; 709C: 47            
                    RORB                          ; 709D: 56            
                    STD   $5096                   ; 709E: FD 50 96      
                    ADDD  $509C                   ; 70A1: F3 50 9C      
                    CMPD  #$FEFF                  ; 70A4: 10 83 FE FF     // Trench Y top/bottom limits
                    BLE   L70AD                   ; 70A8: 2F 03         
                    LDD   #$FEFF                  ; 70AA: CC FE FF      
L70AD               CMPD  #$F201                  ; 70AD: 10 83 F2 01   
                    BGE   L70B6                   ; 70B1: 2C 03         
                    LDD   #$F201                  ; 70B3: CC F2 01      
L70B6               STD   $509C                   ; 70B6: FD 50 9C      
                    STD   $5044                   ; 70B9: FD 50 44        // ZT2
; End of function sub_703B
                    RTS                           ; 70BC: 39            

; =============== S U B R O U T I N E =======================================
L70BD               LDX   #$4866                  ; 70BD: 8E 48 66        // Joystick pitch data structure
                    JSR   L70F0                   ; 70C0: BD 70 F0        // Some joystick data calculation ?
L70C3               JSR   L7111                   ; 70C3: BD 71 11        // Some joystick driven matrix rotations for pitch/roll
L70C6               BEQ   locret_70CB             ; 70C6: 27 03         
                    JSR   $CE2F                   ; 70C8: BD CE 2F        // Run math program $0E Pitch on Matrix 4
; End of function sub_70BD
locret_70CB         RTS                           ; 70CB: 39            

; =============== S U B R O U T I N E =======================================
L70CC               LDX   #$486F                  ; 70CC: 8E 48 6F        // Joystick roll data structure
                    JSR   L70F0                   ; 70CF: BD 70 F0        // Some joystick data calculation ?
L70D2               JSR   L7111                   ; 70D2: BD 71 11        // Some joystick driven matrix rotations for pitch/roll
L70D5               BEQ   locret_70DA             ; 70D5: 27 03         
                    JSR   $CE3A                   ; 70D7: BD CE 3A        // Run math program $1C Yaw on Matrix 4
; End of function sub_70CC
locret_70DA         RTS                           ; 70DA: 39            

; =============== S U B R O U T I N E =======================================
L70DB               ORCC  #$10                    ; 70DB: 1A 10           // Disable interrupts
                    LDD   <$6B                    ; 70DD: DC 6B         
                    STD   <$7F                    ; 70DF: DD 7F           // Joystick Y
                    LDD   <$74                    ; 70E1: DC 74         
                    STD   <$7D                    ; 70E3: DD 7D           // Joystick X
                    LDD   <$2F                    ; 70E5: DC 2F         
                    STD   <$7B                    ; 70E7: DD 7B         
                    LDD   <$2D                    ; 70E9: DC 2D         
                    STD   <$79                    ; 70EB: DD 79         
                    ANDCC #$EF                    ; 70ED: 1C EF           // n      ; Enable interrupts
; End of function sub_70DB
                    RTS                           ; 70EF: 39            

; =============== S U B R O U T I N E =======================================
; Some joystick data calculation ?
L70F0               LDA   $07,X                   ; 70F0: A6 07         
                    BPL   L70F5                   ; 70F2: 2A 01         
                    COMA                          ; 70F4: 43            
L70F5               LDB   #$80                    ; 70F5: C6 80         
                    MUL                           ; 70F7: 3D            
                    NOP                           ; 70F8: 12            
                    NOP                           ; 70F9: 12            
                    NOP                           ; 70FA: 12            
                    LDB   $07,X                   ; 70FB: E6 07         
                    BMI   L7107                   ; 70FD: 2B 08         
                    ADDA  $08,X                   ; 70FF: AB 08         
                    BVC   L7105                   ; 7101: 28 02         
                    LDA   #$7F                    ; 7103: 86 7F         
L7105               BRA   L710E                   ; 7105: 20 07         

; ---------------------------------------------------------------------------
L7107               NEGA                          ; 7107: 40            
                    ADDA  $08,X                   ; 7108: AB 08         
                    BVC   L710E                   ; 710A: 28 02         
                    LDA   #$81                    ; 710C: 86 81         
L710E               STA   $08,X                   ; 710E: A7 08         
; End of function sub_70F0
                    RTS                           ; 7110: 39            

; =============== S U B R O U T I N E =======================================
; Some joystick driven matrix rotations for pitch/roll
L7111               LDA   $08,X                   ; 7111: A6 08         
                    BPL   L7116                   ; 7113: 2A 01         
                    NEGA                          ; 7115: 40            
L7116               CMPA  #$4E                    ; 7116: 81 4E         
                    BCS   L713A                   ; 7118: 25 20         
                    LDD   #$3FC2                  ; 711A: CC 3F C2      
                    STD   $5024                   ; 711D: FD 50 24        // Cosine for rotation
                    LDA   $08,X                   ; 7120: A6 08         
                    BPL   L712F                   ; 7122: 2A 0B         
                    ADDA  #$4E                    ; 7124: 8B 4E         
                    STA   $08,X                   ; 7126: A7 08         
                    LDD   #$FA70                  ; 7128: CC FA 70      
                    STD   $5022                   ; 712B: FD 50 22        // Sine for rotation
                    RTS                           ; 712E: 39            

; ---------------------------------------------------------------------------
L712F               SUBA  #$4E                    ; 712F: 80 4E         
                    STA   $08,X                   ; 7131: A7 08         
                    LDD   #$0590                  ; 7133: CC 05 90      
                    STD   $5022                   ; 7136: FD 50 22        // Sine for rotation
                    RTS                           ; 7139: 39            

; ---------------------------------------------------------------------------
L713A               CMPA  #$0E                    ; 713A: 81 0E         
                    BCS   L715E                   ; 713C: 25 20         
                    LDD   #$3FFE                  ; 713E: CC 3F FE      
                    STD   $5024                   ; 7141: FD 50 24        // Cosine for rotation
                    LDA   $08,X                   ; 7144: A6 08         
                    BPL   L7153                   ; 7146: 2A 0B         
                    ADDA  #$0E                    ; 7148: 8B 0E         
                    STA   $08,X                   ; 714A: A7 08         
                    LDD   #$FF00                  ; 714C: CC FF 00      
                    STD   $5022                   ; 714F: FD 50 22        // Sine for rotation
                    RTS                           ; 7152: 39            

; ---------------------------------------------------------------------------
L7153               SUBA  #$0E                    ; 7153: 80 0E         
                    STA   $08,X                   ; 7155: A7 08         
                    LDD   #$0100                  ; 7157: CC 01 00      
                    STD   $5022                   ; 715A: FD 50 22        // Sine for rotation
                    RTS                           ; 715D: 39            

; ---------------------------------------------------------------------------
L715E               CLRB                          ; 715E: 5F            
; End of function sub_7111
                    RTS                           ; 715F: 39            

; =============== S U B R O U T I N E =======================================
; Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L7160               LDA   $486E                   ; 7160: B6 48 6E      
                    BEQ   L7181                   ; 7163: 27 1C         
                    JSR   L71C4                   ; 7165: BD 71 C4      
L7168               LDA   $486E                   ; 7168: B6 48 6E      
                    BPL   L7176                   ; 716B: 2A 09         
                    LDD   #$00                    ; 716D: CC 00 00      
                    SUBD  $5022                   ; 7170: B3 50 22        // Sine for rotation
                    STD   $5022                   ; 7173: FD 50 22        // Sine for rotation
L7176               CLRA                          ; 7176: 4F            
                    LDB   #$05                    ; 7177: C6 05           // Point BIC to[ A,B,C ][x, y, z] 2
                    STD   $4701                   ; 7179: FD 47 01      
                    LDA   #$0E                    ; 717C: 86 0E           // Pitch program
                    JSR   $CDBA                   ; 717E: BD CD BA        // Do math program run
L7181               LDA   $4877                   ; 7181: B6 48 77      
                    BEQ   L71A2                   ; 7184: 27 1C         
                    JSR   L71C4                   ; 7186: BD 71 C4      
L7189               LDA   $4877                   ; 7189: B6 48 77      
                    BPL   L7197                   ; 718C: 2A 09         
                    LDD   #$00                    ; 718E: CC 00 00      
                    SUBD  $5022                   ; 7191: B3 50 22        // Sine for rotation
                    STD   $5022                   ; 7194: FD 50 22        // Sine for rotation
L7197               CLRA                          ; 7197: 4F            
                    LDB   #$05                    ; 7198: C6 05           // Point BIC to[ A,B,C ][x, y, z] 2
                    STD   $4701                   ; 719A: FD 47 01      
                    LDA   #$1C                    ; 719D: 86 1C           // Yaw program
                    JSR   $CDBA                   ; 719F: BD CD BA        // Do math program run
L71A2               LDA   $4878                   ; 71A2: B6 48 78      
                    BEQ   locret_71C3             ; 71A5: 27 1C         
                    JSR   L71C4                   ; 71A7: BD 71 C4      
L71AA               LDA   $4878                   ; 71AA: B6 48 78      
                    BPL   L71B8                   ; 71AD: 2A 09         
                    LDD   #$00                    ; 71AF: CC 00 00      
                    SUBD  $5022                   ; 71B2: B3 50 22        // Sine for rotation
                    STD   $5022                   ; 71B5: FD 50 22        // Sine for rotation
L71B8               CLRA                          ; 71B8: 4F            
                    LDB   #$05                    ; 71B9: C6 05           // Point BIC to[ A,B,C ][x, y, z] 2
                    STD   $4701                   ; 71BB: FD 47 01      
                    LDA   #$00                    ; 71BE: 86 00           // Roll program
                    JSR   $CDBA                   ; 71C0: BD CD BA        // Do math program run
; End of function sub_7160
locret_71C3         RTS                           ; 71C3: 39            

; =============== S U B R O U T I N E =======================================
L71C4               BPL   L71C7                   ; 71C4: 2A 01         
                    NEGA                          ; 71C6: 40            
L71C7               LDB   #$03                    ; 71C7: C6 03         
                    MUL                           ; 71C9: 3D            
                    LDX   #$71DA                  ; 71CA: 8E 71 DA      
                    ABX                           ; 71CD: 3A            
                    LDD   ,X                      ; 71CE: EC 84         
                    STD   $5022                   ; 71D0: FD 50 22        // Sine for rotation
                    LDB   $02,X                   ; 71D3: E6 02         
                    SEX                           ; 71D5: 1D            
                    ADDD  #$4000                  ; 71D6: C3 40 00      
L71D9               STD   $5024                   ; 71D9: FD 50 24        // Cosine for rotation
; End of function sub_71C4
; ---------------------------------------------------------------------------
                    RTS                           ; 71DC: 39            

; =============== S U B R O U T I N E =======================================
; Space wave
L72C7               JSR   L6112                   ; 72C7: BD 61 12        // Insert vector data for four blue dots in screen corners
L72CA               LDA   #$10                    ; 72CA: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 72CC: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L72CF               JSR   L7160                   ; 72CF: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L72D2               JSR   L761D                   ; 72D2: BD 76 1D        // Display score
L72D5               LDA   $4B2D                   ; 72D5: B6 4B 2D      
                    BNE   L72F0                   ; 72D8: 26 16         
                    LDD   $4B0E                   ; 72DA: FC 4B 0E        // Attract screen/game phase  timer
                    CMPD  #$A0                    ; 72DD: 10 83 00 A0   
                    BCC   L72F0                   ; 72E1: 24 0D         
                    ANDB  #$10                    ; 72E3: C4 10         
                    BNE   L72EB                   ; 72E5: 26 04         
                    LDB   #$4C                    ; 72E7: C6 4C         
                    BRA   L72ED                   ; 72E9: 20 02         

; ---------------------------------------------------------------------------
L72EB               LDB   #$4D                    ; 72EB: C6 4D           // "Shoot Tie Fighters text index", Shoot Tie Fighters text index
L72ED               JSR   $E7C7                   ; 72ED: BD E7 C7        // "Print text string from pointer table", Print text string from pointer table
L72F0               JSR   $95A7                   ; 72F0: BD 95 A7        // "Insert vector instructions for shields", Insert vector instructions for shields
L72F3               JSR   L7765                   ; 72F3: BD 77 65        // Space wave sub_7765
L72F6               JSR   $B6B9                   ; 72F6: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L72F9               JSR   $CD80                   ; 72F9: BD CD 80        // Starfield
L72FC               JSR   $BA12                   ; 72FC: BD BA 12        // Process tie/tower/bunker explosions
L72FF               JSR   $AE60                   ; 72FF: BD AE 60      
L7302               JSR   L786A                   ; 7302: BD 78 6A        // Process tie fighters and insert vectors
L7305               JSR   $AAE4                   ; 7305: BD AA E4        // Fireball processing
L7308               JSR   $B32B                   ; 7308: BD B3 2B      
L730B               JSR   $AEBD                   ; 730B: BD AE BD      
L730E               JSR   $98B0                   ; 730E: BD 98 B0        // Insert vector laser explosion small circle
L7311               JSR   L612F                   ; 7311: BD 61 2F        // Vector instructions end
; End of function sub_72C7
L7314               RTS                           ; 7314: 39            

; =============== S U B R O U T I N E =======================================
L7315               JSR   L6112                   ; 7315: BD 61 12        // Insert vector data for four blue dots in screen corners
L7318               LDA   #$10                    ; 7318: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 731A: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L731D               JSR   L7160                   ; 731D: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L7320               JSR   L761D                   ; 7320: BD 76 1D        // Display score
L7323               JSR   $95A7                   ; 7323: BD 95 A7        // Insert vector instructions for shields
L7326               JSR   $CD80                   ; 7326: BD CD 80        // Starfield
L7329               JSR   $BA12                   ; 7329: BD BA 12        // Process tie/tower/bunker explosions
L732C               JSR   L786A                   ; 732C: BD 78 6A        // Process tie fighters and insert vectors
L732F               JSR   $AAE4                   ; 732F: BD AA E4        // Fireball processing
L7332               JSR   L7707                   ; 7332: BD 77 07        // Game Over text handling
L7335               JSR   $98B0                   ; 7335: BD 98 B0        // Insert vector laser explosion small circle
L7338               JSR   L612F                   ; 7338: BD 61 2F        // Vector instructions end
; End of function sub_7315
L733B               RTS                           ; 733B: 39            

; =============== S U B R O U T I N E =======================================
; End of space wave? Also Trench catwalks
L733C               JSR   L6112                   ; 733C: BD 61 12        // Insert vector data for four blue dots in screen corners
L733F               LDA   #$10                    ; 733F: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 7341: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L7344               JSR   L7160                   ; 7344: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L7347               JSR   L761D                   ; 7347: BD 76 1D        // Display score
L734A               JSR   $95A7                   ; 734A: BD 95 A7        // Insert vector instructions for shields
L734D               JSR   $B6B9                   ; 734D: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L7350               JSR   $CD80                   ; 7350: BD CD 80        // Starfield
L7353               JSR   L7765                   ; 7353: BD 77 65        // Space wave sub_7765
L7356               JSR   $BA12                   ; 7356: BD BA 12        // Process tie/tower/bunker explosions
L7359               JSR   $AE60                   ; 7359: BD AE 60      
L735C               JSR   L786A                   ; 735C: BD 78 6A        // Process tie fighters and insert vectors
L735F               JSR   $AAE4                   ; 735F: BD AA E4        // Fireball processing
L7362               JSR   $B32B                   ; 7362: BD B3 2B      
L7365               JSR   $AEBD                   ; 7365: BD AE BD      
L7368               JSR   $98B0                   ; 7368: BD 98 B0        // Insert vector laser explosion small circle
L736B               JSR   L612F                   ; 736B: BD 61 2F        // Vector instructions end
; End of function sub_733C
L736E               RTS                           ; 736E: 39            

; =============== S U B R O U T I N E =======================================
L736F               JSR   L6112                   ; 736F: BD 61 12        // Insert vector data for four blue dots in screen corners
L7372               LDA   #$10                    ; 7372: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 7374: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L7377               JSR   L7160                   ; 7377: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L737A               JSR   L761D                   ; 737A: BD 76 1D        // Display score
L737D               JSR   $95A7                   ; 737D: BD 95 A7        // Insert vector instructions for shields
L7380               JSR   $B6B9                   ; 7380: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L7383               JSR   $CD80                   ; 7383: BD CD 80        // Starfield
L7386               JSR   L77A4                   ; 7386: BD 77 A4      
L7389               JSR   $98B0                   ; 7389: BD 98 B0        // Insert vector laser explosion small circle
L738C               JSR   L612F                   ; 738C: BD 61 2F        // Vector instructions end
; End of function sub_736F
L738F               RTS                           ; 738F: 39            

; =============== S U B R O U T I N E =======================================
; Towers/Bunkers wave
L7390               JSR   L6112                   ; 7390: BD 61 12        // Towers/Bunkers wave
L7393               LDA   #$10                    ; 7393: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 7395: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L7398               JSR   L7160                   ; 7398: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L739B               JSR   L761D                   ; 739B: BD 76 1D        // Display score
L739E               JSR   L768D                   ; 739E: BD 76 8D        // Display tower count and hit score
L73A1               JSR   $95A7                   ; 73A1: BD 95 A7        // Insert vector instructions for shields
L73A4               JSR   $B6B9                   ; 73A4: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L73A7               JSR   $CD8C                   ; 73A7: BD CD 8C        // Towers surface dots
L73AA               JSR   $AE60                   ; 73AA: BD AE 60      
L73AD               JSR   $AAE4                   ; 73AD: BD AA E4        // Fireball processing
L73B0               JSR   $A214                   ; 73B0: BD A2 14      
L73B3               JSR   $B2D2                   ; 73B3: BD B2 D2      
L73B6               JSR   $AEBD                   ; 73B6: BD AE BD      
L73B9               JSR   $BA12                   ; 73B9: BD BA 12        // Process tie/tower/bunker explosions
L73BC               JSR   $98B0                   ; 73BC: BD 98 B0        // Insert vector laser explosion small circle
L73BF               JSR   L612F                   ; 73BF: BD 61 2F        // Vector instructions end
; End of function sub_7390
L73C2               RTS                           ; 73C2: 39            

; =============== S U B R O U T I N E =======================================
; Towers
L73C3               JSR   L6112                   ; 73C3: BD 61 12        // Insert vector data for four blue dots in screen corners
L73C6               LDA   #$10                    ; 73C6: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 73C8: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L73CB               JSR   L7160                   ; 73CB: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L73CE               JSR   L761D                   ; 73CE: BD 76 1D        // Display score
L73D1               JSR   L768D                   ; 73D1: BD 76 8D        // Display tower count and hit score
L73D4               JSR   $95A7                   ; 73D4: BD 95 A7        // Insert vector instructions for shields
L73D7               JSR   $CD8C                   ; 73D7: BD CD 8C        // Towers surface dots
L73DA               JSR   $AAE4                   ; 73DA: BD AA E4        // Fireball processing
L73DD               JSR   $A214                   ; 73DD: BD A2 14      
L73E0               JSR   L7707                   ; 73E0: BD 77 07        // Game Over text handling
L73E3               JSR   $98B0                   ; 73E3: BD 98 B0        // Insert vector laser explosion small circle
L73E6               JSR   L612F                   ; 73E6: BD 61 2F        // Vector instructions end
; End of function sub_73C3
L73E9               RTS                           ; 73E9: 39            

; =============== S U B R O U T I N E =======================================
; Towers
L73EA               JSR   L6112                   ; 73EA: BD 61 12        // Insert vector data for four blue dots in screen corners
L73ED               LDA   #$10                    ; 73ED: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 73EF: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L73F2               JSR   L7160                   ; 73F2: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L73F5               JSR   L761D                   ; 73F5: BD 76 1D        // Display score
L73F8               JSR   L76D3                   ; 73F8: BD 76 D3        // Towers left to shoot count
L73FB               LDB   #$4F                    ; 73FB: C6 4F         
                    JSR   $E7C7                   ; 73FD: BD E7 C7        // Print text string from pointer table
L7400               JSR   $95A7                   ; 7400: BD 95 A7        // Insert vector instructions for shields
L7403               JSR   $B6B9                   ; 7403: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L7406               JSR   $CD8C                   ; 7406: BD CD 8C        // Towers surface dots
L7409               JSR   $BA12                   ; 7409: BD BA 12        // Process tie/tower/bunker explosions
L740C               JSR   $98B0                   ; 740C: BD 98 B0        // Insert vector laser explosion small circle
L740F               JSR   L612F                   ; 740F: BD 61 2F        // Vector instructions end
; End of function sub_73EA
L7412               RTS                           ; 7412: 39            

; =============== S U B R O U T I N E =======================================
L7413               JSR   L6112                   ; 7413: BD 61 12        // Insert vector data for four blue dots in screen corners
L7416               LDA   #$10                    ; 7416: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 7418: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L741B               JSR   L7160                   ; 741B: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L741E               JSR   L761D                   ; 741E: BD 76 1D        // Display score
L7421               JSR   L76D3                   ; 7421: BD 76 D3        // Towers left to shoot count
L7424               LDB   #$4F                    ; 7424: C6 4F         
                    JSR   $E7C7                   ; 7426: BD E7 C7        // Print text string from pointer table
L7429               JSR   $95A7                   ; 7429: BD 95 A7        // Insert vector instructions for shields
L742C               JSR   $B6B9                   ; 742C: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L742F               JSR   $859B                   ; 742F: BD 85 9B      
L7432               JSR   $BA12                   ; 7432: BD BA 12        // Process tie/tower/bunker explosions
L7435               JSR   $98B0                   ; 7435: BD 98 B0        // Insert vector laser explosion small circle
L7438               JSR   L612F                   ; 7438: BD 61 2F        // Vector instructions end
; End of function sub_7413
L743B               RTS                           ; 743B: 39            

; =============== S U B R O U T I N E =======================================
L743C               JSR   L6112                   ; 743C: BD 61 12        // Insert vector data for four blue dots in screen corners
L743F               LDA   #$10                    ; 743F: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 7441: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L7444               JSR   L7160                   ; 7444: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L7447               JSR   L761D                   ; 7447: BD 76 1D        // Display score
L744A               LDA   $4B0E                   ; 744A: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$04                    ; 744D: 81 04         
                    BHI   L7464                   ; 744F: 22 13         
                    LDA   <$98                    ; 7451: 96 98         
                    BNE   L745A                   ; 7453: 26 05         
                    JSR   L76D3                   ; 7455: BD 76 D3        // Towers left to shoot count
L7458               BRA   L7464                   ; 7458: 20 0A         

; ---------------------------------------------------------------------------
L745A               LDD   #$7100                  ; 745A: CC 71 00      
                    STD   ,Y++                    ; 745D: ED A1         
                    LDB   #$46                    ; 745F: C6 46         
                    JSR   $E7C7                   ; 7461: BD E7 C7        // Print text string from pointer table
L7464               LDA   $4B2D                   ; 7464: B6 4B 2D      
                    BNE   L7491                   ; 7467: 26 28         
                    LDA   $4B0E                   ; 7469: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$08                    ; 746C: 81 08         
                    BHI   L7491                   ; 746E: 22 21         
                    LDA   <$98                    ; 7470: 96 98         
                    BNE   L7491                   ; 7472: 26 1D         
                    LDA   $4B35                   ; 7474: B6 4B 35      
                    BNE   L7491                   ; 7477: 26 18         
                    LDA   $4B12                   ; 7479: B6 4B 12      
                    BNE   L7482                   ; 747C: 26 04         
                    LDB   #$4C                    ; 747E: C6 4C         
                    BRA   L748E                   ; 7480: 20 0C         

; ---------------------------------------------------------------------------
L7482               LDA   <$43                    ; 7482: 96 43           // "Game over/insert coins timer", Game over/insert coins timer
                    ANDA  #$10                    ; 7484: 84 10         
L7486               BNE   L748C                   ; 7486: 26 04         
                    LDB   #$4C                    ; 7488: C6 4C         
L748A               BRA   L748E                   ; 748A: 20 02         

; ---------------------------------------------------------------------------
L748C               LDB   #$4E                    ; 748C: C6 4E         
L748E               JSR   $E7C7                   ; 748E: BD E7 C7        // "Print text string from pointer table", Print text string from pointer table
L7491               LDA   $4B2D                   ; 7491: B6 4B 2D      
                    BNE   L74A5                   ; 7494: 26 0F         
                    LDA   $4895                   ; 7496: B6 48 95      
                    BEQ   L74A5                   ; 7499: 27 0A         
                    LDD   #$7100                  ; 749B: CC 71 00      
                    STD   ,Y++                    ; 749E: ED A1         
                    LDB   #$44                    ; 74A0: C6 44         
                    JSR   $E7C7                   ; 74A2: BD E7 C7        // Print text string from pointer table
L74A5               LDA   $4B36                   ; 74A5: B6 4B 36      
                    BLT   L74B6                   ; 74A8: 2D 0C         
                    BNE   L74B3                   ; 74AA: 26 07         
                    LDB   #$4F                    ; 74AC: C6 4F         
                    JSR   $E7C7                   ; 74AE: BD E7 C7        // Print text string from pointer table
L74B1               BRA   L74B6                   ; 74B1: 20 03         

; ---------------------------------------------------------------------------
L74B3               JSR   $97C2                   ; 74B3: BD 97 C2      
L74B6               JSR   $95A7                   ; 74B6: BD 95 A7        // "Insert vector instructions for shields", Insert vector instructions for shields
L74B9               JSR   $B6B9                   ; 74B9: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L74BC               JSR   $859B                   ; 74BC: BD 85 9B      
L74BF               JSR   $AE60                   ; 74BF: BD AE 60      
L74C2               JSR   $AAE4                   ; 74C2: BD AA E4        // Fireball processing
L74C5               JSR   $B071                   ; 74C5: BD B0 71      
L74C8               JSR   $AEBD                   ; 74C8: BD AE BD      
L74CB               JSR   $ADAF                   ; 74CB: BD AD AF      
L74CE               JSR   $98B0                   ; 74CE: BD 98 B0        // Insert vector laser explosion small circle
L74D1               JSR   L612F                   ; 74D1: BD 61 2F        // Vector instructions end
; End of function sub_743C
L74D4               RTS                           ; 74D4: 39            

; =============== S U B R O U T I N E =======================================
; Game over
L74D5               JSR   L6112                   ; 74D5: BD 61 12        // Insert vector data for four blue dots in screen corners
L74D8               LDA   #$10                    ; 74D8: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 74DA: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L74DD               JSR   L7160                   ; 74DD: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L74E0               JSR   L761D                   ; 74E0: BD 76 1D        // Display score
L74E3               LDA   $4B3E                   ; 74E3: B6 4B 3E      
                    BEQ   L74F2                   ; 74E6: 27 0A         
                    LDD   #$7100                  ; 74E8: CC 71 00      
                    STD   ,Y++                    ; 74EB: ED A1         
                    LDB   #$46                    ; 74ED: C6 46         
                    JSR   $E7C7                   ; 74EF: BD E7 C7        // Print text string from pointer table
L74F2               JSR   $95A7                   ; 74F2: BD 95 A7        // "Insert vector instructions for shields", Insert vector instructions for shields
L74F5               JSR   $859B                   ; 74F5: BD 85 9B      
L74F8               JSR   $AAE4                   ; 74F8: BD AA E4        // Fireball processing
L74FB               LDA   $4B2D                   ; 74FB: B6 4B 2D      
                    BNE   L750F                   ; 74FE: 26 0F         
                    LDA   $4895                   ; 7500: B6 48 95      
                    BEQ   L750F                   ; 7503: 27 0A         
                    LDD   #$7100                  ; 7505: CC 71 00      
                    STD   ,Y++                    ; 7508: ED A1         
                    LDB   #$44                    ; 750A: C6 44         
                    JSR   $E7C7                   ; 750C: BD E7 C7        // Print text string from pointer table
L750F               JSR   L7707                   ; 750F: BD 77 07        // "Game Over text handling", Game Over text handling
L7512               JSR   $98B0                   ; 7512: BD 98 B0        // Insert vector laser explosion small circle
L7515               JSR   L612F                   ; 7515: BD 61 2F        // Vector instructions end
; End of function sub_74D5
L7518               RTS                           ; 7518: 39            

; =============== S U B R O U T I N E =======================================
L7519               JSR   L6112                   ; 7519: BD 61 12        // Insert vector data for four blue dots in screen corners
L751C               LDA   #$10                    ; 751C: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 751E: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L7521               JSR   L7160                   ; 7521: BD 71 60        // Update Roll, Pitch and Yaw for [A, B, C][x, y, z] 2
L7524               JSR   L761D                   ; 7524: BD 76 1D        // Display score
L7527               JSR   $95A7                   ; 7527: BD 95 A7        // Insert vector instructions for shields
L752A               JSR   $B6B9                   ; 752A: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L752D               JSR   $CD80                   ; 752D: BD CD 80        // Starfield
L7530               LDB   #$45                    ; 7530: C6 45         
                    JSR   $E7C7                   ; 7532: BD E7 C7        // Print text string from pointer table
L7535               LDA   $4B0E                   ; 7535: B6 4B 0E        // Attract screen/game phase  timer
                    CMPA  #$02                    ; 7538: 81 02         
                    BGT   L755A                   ; 753A: 2E 1E         
                    LDB   #$47                    ; 753C: C6 47         
                    JSR   $E7C7                   ; 753E: BD E7 C7        // Print text string from pointer table
L7541               LDB   #$48                    ; 7541: C6 48         
                    JSR   $E7C7                   ; 7543: BD E7 C7        // Print text string from pointer table
L7546               LDD   #$90                    ; 7546: CC 00 90      
                    STD   ,Y++                    ; 7549: ED A1         
                    LDD   #$70                    ; 754B: CC 00 70      
                    STD   ,Y++                    ; 754E: ED A1         
                    LDA   <$8E                    ; 7550: 96 8E         
                    JSR   $E7AD                   ; 7552: BD E7 AD      
L7555               LDD   #$8040                  ; 7555: CC 80 40      
                    STD   ,Y++                    ; 7558: ED A1         
L755A               LDA   $4B0E                   ; 755A: B6 4B 0E        // "Attract screen/game phase  timer", Attract screen/game phase  timer
                    CMPA  #$01                    ; 755D: 81 01         
                    BGT   L759C                   ; 755F: 2E 3B         
                    LDA   $4845                   ; 7561: B6 48 45      
                    BEQ   L759C                   ; 7564: 27 36         
                    LDA   $4592                   ; 7566: B6 45 92      
                    ANDA  #$03                    ; 7569: 84 03         
                    BEQ   L759C                   ; 756B: 27 2F         
                    LDB   $4593                   ; 756D: F6 45 93      
                    ANDB  #$03                    ; 7570: C4 03         
                    ADDB  #$06                    ; 7572: CB 06         
                    CMPB  <$60                    ; 7574: D1 60           // Shield count
                    BHI   L757C                   ; 7576: 22 04         
                    LDB   #$4A                    ; 7578: C6 4A         
                    BRA   L7599                   ; 757A: 20 1D         

; ---------------------------------------------------------------------------
L757C               LDU   #$A01A                  ; 757C: CE A0 1A      
                    STU   ,Y++                    ; 757F: EF A1         
                    LDU   #$48                    ; 7581: CE 00 48      
                    STU   ,Y++                    ; 7584: EF A1         
                    LDU   #$1EC0                  ; 7586: CE 1E C0      
                    STU   ,Y++                    ; 7589: EF A1         
                    LDB   #$01                    ; 758B: C6 01         
                    STB   <$AD                    ; 758D: D7 AD         
                    JSR   $E7AD                   ; 758F: BD E7 AD      
L7592               LDD   #$8040                  ; 7592: CC 80 40      
                    STD   ,Y++                    ; 7595: ED A1         
                    LDB   #$49                    ; 7597: C6 49         
L7599               JSR   $E7C7                   ; 7599: BD E7 C7        // "Print text string from pointer table", Print text string from pointer table
L759C               LDA   $4B0E                   ; 759C: B6 4B 0E        // "Attract screen/game phase  timer", Attract screen/game phase  timer
                    CMPA  #$00                    ; 759F: 81 00         
                    BGT   L75B2                   ; 75A1: 2E 0F         
                    LDA   $4B2D                   ; 75A3: B6 4B 2D      
                    BNE   L75B2                   ; 75A6: 26 0A         
                    LDB   $4B15                   ; 75A8: F6 4B 15      
                    BEQ   L75B2                   ; 75AB: 27 05         
                    LDB   #$4B                    ; 75AD: C6 4B         
                    JSR   $E7C7                   ; 75AF: BD E7 C7        // Print text string from pointer table
L75B2               JSR   $98B0                   ; 75B2: BD 98 B0        // "Insert vector laser explosion small circle", Insert vector laser explosion small circle
L75B5               JSR   L612F                   ; 75B5: BD 61 2F        // Vector instructions end
; End of function sub_7519
L75B8               RTS                           ; 75B8: 39            

; =============== S U B R O U T I N E =======================================
L75B9               JSR   L6112                   ; 75B9: BD 61 12        // Insert vector data for four blue dots in screen corners
L75BC               LDA   #$10                    ; 75BC: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 75BE: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L75C1               JSR   L761D                   ; 75C1: BD 76 1D        // Display score
L75C4               LDA   $4B36                   ; 75C4: B6 4B 36      
                    BLE   L75CC                   ; 75C7: 2F 03         
                    JSR   $97C2                   ; 75C9: BD 97 C2      
L75CC               JSR   $95A7                   ; 75CC: BD 95 A7        // "Insert vector instructions for shields", Insert vector instructions for shields
L75CF               JSR   $B6B9                   ; 75CF: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L75D2               JSR   L77A4                   ; 75D2: BD 77 A4      
L75D5               JSR   L612F                   ; 75D5: BD 61 2F        // Vector instructions end
; End of function sub_75B9
L75D8               RTS                           ; 75D8: 39            

; =============== S U B R O U T I N E =======================================
; Space wave
L75D9               JSR   L6112                   ; 75D9: BD 61 12        // Insert vector data for four blue dots in screen corners
L75DC               LDA   #$10                    ; 75DC: 86 10           // BIC points to Matrix 4
                    JSR   $CE0C                   ; 75DE: BD CE 0C        // Copy transform data from [BIC] to matrix 2
L75E1               JSR   L761D                   ; 75E1: BD 76 1D        // Display score
L75E4               JSR   $95A7                   ; 75E4: BD 95 A7        // Insert vector instructions for shields
L75E7               JSR   $B6B9                   ; 75E7: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L75EA               LDD   #$1F98                  ; 75EA: CC 1F 98      
                    STD   ,Y++                    ; 75ED: ED A1         
                    LDD   #$00                    ; 75EF: CC 00 00      
                    STD   ,Y++                    ; 75F2: ED A1         
                    LDD   #$7200                  ; 75F4: CC 72 00      
                    STD   ,Y++                    ; 75F7: ED A1         
                    LDD   #$BE50                  ; 75F9: CC BE 50      
                    STD   ,Y++                    ; 75FC: ED A1         
                    LDD   #$7200                  ; 75FE: CC 72 00      
                    STD   ,Y++                    ; 7601: ED A1         
                    JSR   $BB85                   ; 7603: BD BB 85        // Death Star explosion animation
L7606               JSR   L612F                   ; 7606: BD 61 2F        // Vector instructions end
; End of function sub_75D9
L7609               RTS                           ; 7609: 39            

; =============== S U B R O U T I N E =======================================
; Death Star explosion
L760A               JSR   L6112                   ; 760A: BD 61 12        // Insert vector data for four blue dots in screen corners
L760D               JSR   L761D                   ; 760D: BD 76 1D        // Display score
L7610               JSR   $95A7                   ; 7610: BD 95 A7        // Insert vector instructions for shields
L7613               JSR   $B6B9                   ; 7613: BD B6 B9        // Insert vector instructions at joystick position for laser explosion 2
L7616               JSR   $BB85                   ; 7616: BD BB 85        // Death Star explosion animation
L7619               JSR   L612F                   ; 7619: BD 61 2F        // Vector instructions end
; End of function sub_760A
L761C               RTS                           ; 761C: 39            

; =============== S U B R O U T I N E =======================================
; Display score
L761D               LDD   #$B9F2                  ; 761D: CC B9 F2      
                    STD   ,Y++                    ; 7620: ED A1         
                    LDD   #$6280                  ; 7622: CC 62 80      
                    STD   ,Y++                    ; 7625: ED A1         
                    LDD   #$01E0                  ; 7627: CC 01 E0      
L762A               STD   ,Y++                    ; 762A: ED A1         
L762C               LDD   #$1E20                  ; 762C: CC 1E 20      
                    STD   ,Y++                    ; 762F: ED A1         
                    LDA   #$06                    ; 7631: 86 06         
                    STA   <$AD                    ; 7633: 97 AD         
                    LDX   #$485C                  ; 7635: 8E 48 5C      
                    JSR   $E764                   ; 7638: BD E7 64      
L763B               LDD   #$8040                  ; 763B: CC 80 40      
                    STD   ,Y++                    ; 763E: ED A1         
                    LDB   $4B2C                   ; 7640: F6 4B 2C      
                    BEQ   L766E                   ; 7643: 27 29         
                    SUBB  #$08                    ; 7645: C0 08         
                    CMPB  #$20                    ; 7647: C1 20         
                    BCC   L764D                   ; 7649: 24 02         
                    LDB   #$00                    ; 764B: C6 00         
L764D               STB   $4B2C                   ; 764D: F7 4B 2C      
                    LSRB                          ; 7650: 54            
L7651               LDA   #$66                    ; 7651: 86 66         
                    STD   ,Y++                    ; 7653: ED A1         
                    LDD   #$01B0                  ; 7655: CC 01 B0      
                    STD   ,Y++                    ; 7658: ED A1         
                    LDD   #$1E50                  ; 765A: CC 1E 50      
                    STD   ,Y++                    ; 765D: ED A1         
                    LDA   #$05                    ; 765F: 86 05         
                    STA   <$AD                    ; 7661: 97 AD         
                    LDX   #$4B28                  ; 7663: 8E 4B 28      
                    JSR   $E772                   ; 7666: BD E7 72        // Display BCD number text
L7669               LDD   #$8040                  ; 7669: CC 80 40      
                    STD   ,Y++                    ; 766C: ED A1         
L766E               LDD   #$6280                  ; 766E: CC 62 80      
                    STD   ,Y++                    ; 7671: ED A1         
                    LDD   #$0210                  ; 7673: CC 02 10      
                    STD   ,Y++                    ; 7676: ED A1         
                    LDD   #$0138                  ; 7678: CC 01 38      
                    STD   ,Y++                    ; 767B: ED A1         
                    LDA   #$01                    ; 767D: 86 01         
                    STA   <$AD                    ; 767F: 97 AD         
                    LDA   $4B16                   ; 7681: B6 4B 16      
                    JSR   $E790                   ; 7684: BD E7 90        // Display BCD numbers
L7687               LDD   #$8040                  ; 7687: CC 80 40      
                    STD   ,Y++                    ; 768A: ED A1         
; End of function sub_761D
                    RTS                           ; 768C: 39            

; =============== S U B R O U T I N E =======================================
; Display tower count and hit score
L768D               LDA   $4B13                   ; 768D: B6 4B 13        // Display tower hit score
                    BLE   L76FC                   ; 7690: 2F 6A         
                    LDA   $4B1A                   ; 7692: B6 4B 1A      
                    BEQ   L76D3                   ; 7695: 27 3C           // Towers left to shoot count
                    LDA   <$43                    ; 7697: 96 43           // Game over/insert coins timer
                    ANDA  #$30                    ; 7699: 84 30         
                    BEQ   L76CE                   ; 769B: 27 31         
                    LDB   #$40                    ; 769D: C6 40         
                    JSR   $E7C7                   ; 769F: BD E7 C7        // Print text string from pointer table
L76A2               LDD   #$6280                  ; 76A2: CC 62 80        // Vector STAT 2,80 instruction
                    STD   ,Y++                    ; 76A5: ED A1         
                    LDD   #$0180                  ; 76A7: CC 01 80        // Vector draw to position
                    STD   ,Y++                    ; 76AA: ED A1         
                    LDD   #$1ED0                  ; 76AC: CC 1E D0      
                    STD   ,Y++                    ; 76AF: ED A1         
                    LDA   #$04                    ; 76B1: 86 04         
                    STA   <$AD                    ; 76B3: 97 AD         
                    LDA   $4B2E                   ; 76B5: B6 4B 2E        // Temporary score adder towers 1
                    JSR   $E790                   ; 76B8: BD E7 90        // Display BCD numbers
L76BB               LDA   $4B2F                   ; 76BB: B6 4B 2F        // Temporary score adder towers 2
                    JSR   $E790                   ; 76BE: BD E7 90        // Display BCD numbers
L76C1               LDA   $4B30                   ; 76C1: B6 4B 30        // Temporary score adder towers 3
                    JSR   $E790                   ; 76C4: BD E7 90        // Display BCD numbers
L76C7               LDD   #$8040                  ; 76C7: CC 80 40        // Vector CNTR instruction
                    STD   ,Y++                    ; 76CA: ED A1         
                    BRA   L76D3                   ; 76CC: 20 05           // Towers left to shoot count

; ---------------------------------------------------------------------------
L76CE               LDB   #$43                    ; 76CE: C6 43         
; End of function sub_768D
                    JSR   $E7C7                   ; 76D0: BD E7 C7        // Print text string from pointer table
; =============== S U B R O U T I N E =======================================
; Towers left to shoot count
L76D3               LDA   $4B13                   ; 76D3: B6 4B 13        // Display towers left to shoot count
                    BLE   L76FC                   ; 76D6: 2F 24         
                    LDD   #$41                    ; 76D8: CC 00 41      
                    JSR   $E7C7                   ; 76DB: BD E7 C7        // Print text string from pointer table
L76DE               LDD   #$6280                  ; 76DE: CC 62 80        // Vector STAT 2,80 instruction
                    STD   ,Y++                    ; 76E1: ED A1         
                    LDD   #$0198                  ; 76E3: CC 01 98      
                    STD   ,Y++                    ; 76E6: ED A1         
                    LDD   #$0168                  ; 76E8: CC 01 68      
                    STD   ,Y++                    ; 76EB: ED A1         
                    LDA   #$01                    ; 76ED: 86 01         
                    STA   <$AD                    ; 76EF: 97 AD         
                    LDA   $4B1A                   ; 76F1: B6 4B 1A      
                    JSR   $E790                   ; 76F4: BD E7 90        // Display BCD numbers
L76F7               LDD   #$8040                  ; 76F7: CC 80 40        // Vector CNTR instruction
                    STD   ,Y++                    ; 76FA: ED A1         
L76FC               LDA   $4B35                   ; 76FC: B6 4B 35      
                    BEQ   locret_7706             ; 76FF: 27 05         
                    LDB   #$42                    ; 7701: C6 42         
                    JSR   $E7C7                   ; 7703: BD E7 C7        // Print text string from pointer table
; End of function sub_76D3
locret_7706         RTS                           ; 7706: 39            

; =============== S U B R O U T I N E =======================================
; Game Over text handling
L7707               LDA   $4B0F                   ; 7707: B6 4B 0F        // Attract screen/game phase  timer
                    CMPA  #$20                    ; 770A: 81 20         
                    BLS   L7710                   ; 770C: 23 02         
                    LDA   #$20                    ; 770E: 86 20         
L7710               LDB   #$06                    ; 7710: C6 06         
                    MUL                           ; 7712: 3D            
                    NEGB                          ; 7713: 50            
                    ADDB  #$C0                    ; 7714: CB C0         
                    LDA   #$70                    ; 7716: 86 70         
                    STD   ,Y++                    ; 7718: ED A1         
                    LDB   #$04                    ; 771A: C6 04         
                    JSR   $E7C7                   ; 771C: BD E7 C7        // Print text string from pointer table
; End of function sub_7707
L771F               RTS                           ; 771F: 39            

; =============== S U B R O U T I N E =======================================
L7720               STD   $4AD7                   ; 7720: FD 4A D7      
                    LDD   #$00                    ; 7723: CC 00 00      
                    STD   $4AD4                   ; 7726: FD 4A D4      
                    STA   $4AD6                   ; 7729: B7 4A D6      
L772C               ASL   $4AD8                   ; 772C: 78 4A D8      
                    ROL   $4AD7                   ; 772F: 79 4A D7      
                    LDA   $4AD6                   ; 7732: B6 4A D6      
                    ADCA  $4AD6                   ; 7735: B9 4A D6      
                    DAA                           ; 7738: 19            
                    STA   $4AD6                   ; 7739: B7 4A D6      
                    LDA   $4AD5                   ; 773C: B6 4A D5      
                    ADCA  $4AD5                   ; 773F: B9 4A D5      
                    DAA                           ; 7742: 19            
                    STA   $4AD5                   ; 7743: B7 4A D5      
                    ROL   $4AD4                   ; 7746: 79 4A D4      
                    LEAX  -1,X                    ; 7749: 30 1F         
                    BNE   L772C                   ; 774B: 26 DF         
; End of function sub_7720
                    RTS                           ; 774D: 39            

; =============== S U B R O U T I N E =======================================
L774E               STA   $4AD7                   ; 774E: B7 4A D7      
                    ANDA  #$F0                    ; 7751: 84 F0         
                    LDB   #$A0                    ; 7753: C6 A0         
                    MUL                           ; 7755: 3D            
                    LDB   $4AD7                   ; 7756: F6 4A D7      
                    ANDB  #$0F                    ; 7759: C4 0F         
                    STB   $4AD7                   ; 775B: F7 4A D7      
                    ADDA  $4AD7                   ; 775E: BB 4A D7      
                    STA   $4AD7                   ; 7761: B7 4A D7      
; End of function sub_774E
                    RTS                           ; 7764: 39            

; =============== S U B R O U T I N E =======================================
; Space wave sub_7765
L7765               LDD   $5028                   ; 7765: FC 50 28        // Ax2
                    BLE   locret_77A3             ; 7768: 2F 39         
                    STD   $4704                   ; 776A: FD 47 04      
                    LDD   $502A                   ; 776D: FC 50 2A        // Ay2
                    STD   $5002                   ; 7770: FD 50 02        // Math result Y
                    TSTA                          ; 7773: 4D            
                    BPL   L777A                   ; 7774: 2A 04         
                    COMA                          ; 7776: 43            
                    NEGB                          ; 7777: 50            
                    SBCA  #$FF                    ; 7778: 82 FF         
L777A               SUBD  $5028                   ; 777A: B3 50 28        // "Ax2", Ax2
                    BGE   locret_77A3             ; 777D: 2C 24         
                    LDD   $502C                   ; 777F: FC 50 2C        // Az2
                    STD   $5004                   ; 7782: FD 50 04        // Math result Z
                    TSTA                          ; 7785: 4D            
                    BPL   L778C                   ; 7786: 2A 04         
                    COMA                          ; 7788: 43            
                    NEGB                          ; 7789: 50            
                    SBCA  #$FF                    ; 778A: 82 FF         
L778C               SUBD  $5028                   ; 778C: B3 50 28        // "Ax2", Ax2
L778F               BGE   locret_77A3             ; 778F: 2C 12         
                    JSR   $CCF0                   ; 7791: BD CC F0        // Get divider result and multiply by Math result Z, insert VCTR instruction
L7794               LDD   #$7300                  ; 7794: CC 73 00      
                    STD   ,Y++                    ; 7797: ED A1         
                    LDD   #$BE50                  ; 7799: CC BE 50        // Vector JRSL Small Death Star
                    STD   ,Y++                    ; 779C: ED A1         
                    LDD   #$7200                  ; 779E: CC 72 00      
                    STD   ,Y++                    ; 77A1: ED A1         
; End of function sub_7765
locret_77A3         RTS                           ; 77A3: 39            

; =============== S U B R O U T I N E =======================================
L77A4               LDD   $5028                   ; 77A4: FC 50 28        // Ax2
                    BLE   locret_77D3             ; 77A7: 2F 2A         
                    STD   $4704                   ; 77A9: FD 47 04      
                    LDD   $502A                   ; 77AC: FC 50 2A        // Ay2
                    STD   $5002                   ; 77AF: FD 50 02        // Math result Y
                    TSTA                          ; 77B2: 4D            
                    BPL   L77B9                   ; 77B3: 2A 04         
                    COMA                          ; 77B5: 43            
                    NEGB                          ; 77B6: 50            
                    SBCA  #$FF                    ; 77B7: 82 FF         
L77B9               SUBD  $5028                   ; 77B9: B3 50 28        // "Ax2", Ax2
                    BGE   locret_77D3             ; 77BC: 2C 15         
                    LDD   $502C                   ; 77BE: FC 50 2C        // Az2
                    STD   $5004                   ; 77C1: FD 50 04        // Math result Z
                    TSTA                          ; 77C4: 4D            
                    BPL   L77CB                   ; 77C5: 2A 04         
                    COMA                          ; 77C7: 43            
                    NEGB                          ; 77C8: 50            
                    SBCA  #$FF                    ; 77C9: 82 FF         
L77CB               SUBD  $5028                   ; 77CB: B3 50 28        // "Ax2", Ax2
                    BGE   locret_77D3             ; 77CE: 2C 03         
                    JSR   L77D4                   ; 77D0: BD 77 D4      
; End of function sub_77A4
locret_77D3         RTS                           ; 77D3: 39            

; =============== S U B R O U T I N E =======================================
L77D4               JSR   $CCF0                   ; 77D4: BD CC F0        // Get divider result and multiply by Math result Z, insert VCTR instruction
L77D7               LDD   -4,Y                    ; 77D7: EC 3C         
                    STD   <$05                    ; 77D9: DD 05           // Zoom value
                    LDD   -2,Y                    ; 77DB: EC 3E         
                    STD   <$03                    ; 77DD: DD 03         
                    LDD   <$56                    ; 77DF: DC 56           // Zoom value
                    STD   <$01                    ; 77E1: DD 01         
                    LDU   #$BD68                  ; 77E3: CE BD 68      
                    JSR   L7863                   ; 77E6: BD 78 63      
L77E9               LDU   #$BDA6                  ; 77E9: CE BD A6      
                    JSR   L785B                   ; 77EC: BD 78 5B      
L77EF               LDU   #$BDB2                  ; 77EF: CE BD B2      
                    JSR   L785B                   ; 77F2: BD 78 5B      
L77F5               LDU   #$BDDA                  ; 77F5: CE BD DA      
                    JSR   L785B                   ; 77F8: BD 78 5B      
L77FB               LDU   #$BE06                  ; 77FB: CE BE 06      
                    JSR   L785B                   ; 77FE: BD 78 5B      
L7801               LDD   <$56                    ; 7801: DC 56           // Zoom value
                    SUBA  #$03                    ; 7803: 80 03         
                    CMPD  #L7000                  ; 7805: 10 83 70 00   
                    BCC   L780E                   ; 7809: 24 03         
                    LDD   #L7000                  ; 780B: CC 70 00      
L780E               STD   <$01                    ; 780E: DD 01         
                    CMPA  #$70                    ; 7810: 81 70         
                    BNE   L7819                   ; 7812: 26 05         
                    LDD   #$6660                  ; 7814: CC 66 60      
                    BRA   L781C                   ; 7817: 20 03         

; ---------------------------------------------------------------------------
L7819               LDD   #$6630                  ; 7819: CC 66 30      
L781C               STD   ,Y++                    ; 781C: ED A1         
                    LDA   $4B14                   ; 781E: B6 4B 14      
                    BITA  #$01                    ; 7821: 85 01         
                    BNE   L782A                   ; 7823: 26 05         
                    LDU   #$B728                  ; 7825: CE B7 28      
                    BRA   L7839                   ; 7828: 20 0F         

; ---------------------------------------------------------------------------
L782A               LDU   #$B73C                  ; 782A: CE B7 3C      
                    JSR   L785B                   ; 782D: BD 78 5B      
L7830               LDU   #$B749                  ; 7830: CE B7 49      
                    JSR   L785B                   ; 7833: BD 78 5B      
L7836               LDU   #$B754                  ; 7836: CE B7 54      
L7839               JSR   L785B                   ; 7839: BD 78 5B      
L783C               LDA   $4B14                   ; 783C: B6 4B 14      
                    BITA  #$01                    ; 783F: 85 01         
                    BNE   L7848                   ; 7841: 26 05         
                    LDU   #$B75E                  ; 7843: CE B7 5E      
                    BRA   L7857                   ; 7846: 20 0F         

; ---------------------------------------------------------------------------
L7848               LDU   #$B770                  ; 7848: CE B7 70      
                    JSR   L785B                   ; 784B: BD 78 5B      
L784E               LDU   #$B77C                  ; 784E: CE B7 7C      
                    JSR   L785B                   ; 7851: BD 78 5B      
L7854               LDU   #$B788                  ; 7854: CE B7 88      
L7857               JSR   L785B                   ; 7857: BD 78 5B      
; End of function sub_77D4
L785A               RTS                           ; 785A: 39            

; =============== S U B R O U T I N E =======================================
L785B               LDD   <$05                    ; 785B: DC 05         
                    STD   ,Y++                    ; 785D: ED A1         
                    LDD   <$03                    ; 785F: DC 03         
; End of function sub_785B
                    STD   ,Y++                    ; 7861: ED A1         
; =============== S U B R O U T I N E =======================================
L7863               LDD   <$01                    ; 7863: DC 01         
                    STD   ,Y++                    ; 7865: ED A1         
                    STU   ,Y++                    ; 7867: EF A1         
; End of function sub_7863
                    RTS                           ; 7869: 39            

; =============== S U B R O U T I N E =======================================
; Process tie fighters and insert vectors
L786A               LDX   #$4900                  ; 786A: 8E 49 00        // 3x Tie fighter data structure ($19 bytes per Tie)
L786D               STX   <$64                    ; 786D: 9F 64           // "Pointer to Tie fighter data", Pointer to Tie fighter data
                    LDA   $03,X                   ; 786F: A6 03         
                    BEQ   L7876                   ; 7871: 27 03         
                    JSR   L7881                   ; 7873: BD 78 81      
L7876               LDX   <$64                    ; 7876: 9E 64           // "Pointer to Tie fighter data", Pointer to Tie fighter data
                    LEAX  $19,X                   ; 7878: 30 88 19      
                    CMPX  #$494B                  ; 787B: 8C 49 4B        // 3x Tie fighter data structure ($19 bytes per Tie)
                    BCS   L786D                   ; 787E: 25 ED         
; End of function sub_786A
locret_7880         RTS                           ; 7880: 39            

; =============== S U B R O U T I N E =======================================
L7881               LDX   <$64                    ; 7881: 9E 64           // Pointer to Tie fighter data
                    CLRA                          ; 7883: 4F            
                    LDB   $02,X                   ; 7884: E6 02         
                    ADDB  #$03                    ; 7886: CB 03         
                    STD   $4701                   ; 7888: FD 47 01      
                    LDA   #$67                    ; 788B: 86 67         
                    JSR   $CDBA                   ; 788D: BD CD BA        // Do math program run
L7890               LDA   $5000                   ; 7890: B6 50 00        // Math result X
                    ASLA                          ; 7893: 48            
                    BVS   L7909                   ; 7894: 29 73         
                    LDA   $5002                   ; 7896: B6 50 02        // Math result Y
                    ASLA                          ; 7899: 48            
                    BVS   L7909                   ; 789A: 29 6D         
                    LDA   $5004                   ; 789C: B6 50 04        // Math result Z
                    ASLA                          ; 789F: 48            
                    BVS   L7909                   ; 78A0: 29 67         
                    LDD   $5070                   ; 78A2: FC 50 70      
                    ADDD  $5072                   ; 78A5: F3 50 72      
                    ADDD  $5074                   ; 78A8: F3 50 74      
                    TFR   D,U                     ; 78AB: 1F 03         
                    CMPU  #$0900                  ; 78AD: 11 83 09 00   
                    BHI   L78BB                   ; 78B1: 22 08         
                    LDD   $15,X                   ; 78B3: EC 88 15      
                    ORA   #$20                    ; 78B6: 8A 20         
                    STD   $15,X                   ; 78B8: ED 88 15      
L78BB               CMPU  #$0100                  ; 78BB: 11 83 01 00   
                    BHI   L78C9                   ; 78BF: 22 08         
                    LDD   $15,X                   ; 78C1: EC 88 15      
                    ORA   #$04                    ; 78C4: 8A 04         
                    STD   $15,X                   ; 78C6: ED 88 15      
L78C9               CMPU  #$A0                    ; 78C9: 11 83 00 A0   
                    BHI   L78FF                   ; 78CD: 22 30         
                    LDA   $4B38                   ; 78CF: B6 4B 38      
                    BNE   L78E4                   ; 78D2: 26 10         
                    LDA   $02,X                   ; 78D4: A6 02         
                    STA   $4B38                   ; 78D6: B7 4B 38      
                    JSR   $BD08                   ; 78D9: BD BD 08      
L78DC               JSR   $BDC6                   ; 78DC: BD BD C6      
L78DF               STU   $4B39                   ; 78DF: FF 4B 39      
                    BRA   L78FD                   ; 78E2: 20 19         

; ---------------------------------------------------------------------------
L78E4               CMPA  $02,X                   ; 78E4: A1 02         
                    BNE   L78FD                   ; 78E6: 26 15         
                    CMPU  $4B39                   ; 78E8: 11 B3 4B 39   
                    BGT   L78F3                   ; 78EC: 2E 05         
                    STU   $4B39                   ; 78EE: FF 4B 39      
                    BRA   L78FD                   ; 78F1: 20 0A         

; ---------------------------------------------------------------------------
L78F3               BCS   L78FD                   ; 78F3: 25 08         
                    LDA   #$FF                    ; 78F5: 86 FF         
                    STA   $4B39                   ; 78F7: B7 4B 39      
                    JSR   $BDDA                   ; 78FA: BD BD DA      
L78FD               BRA   L7909                   ; 78FD: 20 0A         

; ---------------------------------------------------------------------------
L78FF               LDA   $02,X                   ; 78FF: A6 02         
                    CMPA  $4B38                   ; 7901: B1 4B 38      
                    BNE   L7909                   ; 7904: 26 03         
                    CLR   $4B38                   ; 7906: 7F 4B 38      
L7909               LDD   $5000                   ; 7909: FC 50 00        // "Math result X", Math result X
                    CMPD  #$10                    ; 790C: 10 83 00 10   
                    LBLE  locret_7880             ; 7910: 10 2F FF 6C   
                    CMPD  #$7F00                  ; 7914: 10 83 7F 00   
                    LBHI  locret_7880             ; 7918: 10 22 FF 64   
                    STD   $4704                   ; 791C: FD 47 04      
                    STD   $5018                   ; 791F: FD 50 18        // XT
                    LDD   $5002                   ; 7922: FC 50 02        // Math result Y
                    STD   $501A                   ; 7925: FD 50 1A        // YT
                    LDD   $5072                   ; 7928: FC 50 72      
                    SUBD  $5070                   ; 792B: B3 50 70      
                    LBCC  locret_7880             ; 792E: 10 24 FF 4E   
                    LDD   $5004                   ; 7932: FC 50 04        // Math result Z
                    STD   $501C                   ; 7935: FD 50 1C        // ZT
                    LDD   $5074                   ; 7938: FC 50 74      
                    SUBD  $5070                   ; 793B: B3 50 70      
                    LBCC  locret_7880             ; 793E: 10 24 FF 3E   
                    LDX   <$64                    ; 7942: 9E 64           // Pointer to Tie fighter data
                    LDD   $15,X                   ; 7944: EC 88 15      
                    ORA   #$10                    ; 7947: 8A 10         
                    STD   $15,X                   ; 7949: ED 88 15      
                    LDA   $4B3B                   ; 794C: B6 4B 3B      
                    BNE   L7972                   ; 794F: 26 21         
                    LDA   $04,X                   ; 7951: A6 04         
                    CMPA  #$04                    ; 7953: 81 04         
                    BNE   L7972                   ; 7955: 26 1B         
                    INC   $4B3B                   ; 7957: 7C 4B 3B      
                    LDA   $4B14                   ; 795A: B6 4B 14      
                    LSRA                          ; 795D: 44            
                    BCS   L796F                   ; 795E: 25 0F         
                    LDA   $4703                   ; 7960: B6 47 03      
                    BPL   L796A                   ; 7963: 2A 05         
                    JSR   $BD5D                   ; 7965: BD BD 5D      
L7968               BRA   L796D                   ; 7968: 20 03         

; ---------------------------------------------------------------------------
L796A               JSR   $BD2B                   ; 796A: BD BD 2B      
L796D               BRA   L7972                   ; 796D: 20 03         

; ---------------------------------------------------------------------------
L796F               JSR   $BD1C                   ; 796F: BD BD 1C      
L7972               LDA   $02,X                   ; 7972: A6 02         
                    JSR   $CE18                   ; 7974: BD CE 18        // Run math program $80 Copy [BIC] to Matrix 3
L7977               JSR   $CCF0                   ; 7977: BD CC F0        // Get divider result and multiply by Math result Z, insert VCTR instruction
L797A               LDD   #$50                    ; 797A: CC 00 50      
                    STD   $5002                   ; 797D: FD 50 02        // Math result Y
                    LDA   #$86                    ; 7980: 86 86           // Reg02 = Reg02 x Reg00
                    JSR   $CDBA                   ; 7982: BD CD BA        // Do math program run
L7985               LDD   $5002                   ; 7985: FC 50 02        // Math result Y
                    ADDD  #$0A                    ; 7988: C3 00 0A      
                    STD   <$01                    ; 798B: DD 01         
                    LDD   <$D6                    ; 798D: DC D6         
                    SUBD  <$B3                    ; 798F: 93 B3         
                    BPL   L7997                   ; 7991: 2A 04         
                    COMA                          ; 7993: 43            
                    NEGB                          ; 7994: 50            
                    SBCA  #$FF                    ; 7995: 82 FF         
L7997               STD   <$05                    ; 7997: DD 05         
                    STD   <$03                    ; 7999: DD 03         
                    LDD   <$D8                    ; 799B: DC D8         
                    SUBD  <$B5                    ; 799D: 93 B5         
                    BPL   L79A5                   ; 799F: 2A 04         
                    COMA                          ; 79A1: 43            
                    NEGB                          ; 79A2: 50            
                    SBCA  #$FF                    ; 79A3: 82 FF         
L79A5               STD   <$07                    ; 79A5: DD 07         
                    ADDD  <$03                    ; 79A7: D3 03         
                    STD   <$03                    ; 79A9: DD 03         
                    LDD   <$05                    ; 79AB: DC 05         
                    SUBD  <$01                    ; 79AD: 93 01         
                    BGT   L79CF                   ; 79AF: 2E 1E         
                    LDD   <$07                    ; 79B1: DC 07         
                    SUBD  <$01                    ; 79B3: 93 01         
                    BGT   L79CF                   ; 79B5: 2E 18         
                    LDD   <$01                    ; 79B7: DC 01         
                    LSRA                          ; 79B9: 44            
                    RORB                          ; 79BA: 56            
                    ADDD  <$01                    ; 79BB: D3 01         
                    SUBD  <$03                    ; 79BD: 93 03         
                    BLT   L79CF                   ; 79BF: 2D 0E         
                    LDD   $5018                   ; 79C1: FC 50 18        // XT
                    CMPD  <$C4                    ; 79C4: 10 93 C4      
                    BCC   L79CF                   ; 79C7: 24 06         
                    STD   <$C4                    ; 79C9: DD C4         
                    LDX   <$64                    ; 79CB: 9E 64           // Pointer to Tie fighter data
                    STX   <$C2                    ; 79CD: 9F C2         
L79CF               LDD   <$01                    ; 79CF: DC 01         
                    ADDD  <$01                    ; 79D1: D3 01         
                    ADDD  <$01                    ; 79D3: D3 01         
                    SUBD  <$03                    ; 79D5: 93 03         
                    BCS   L79E9                   ; 79D7: 25 10         
                    LDX   <$64                    ; 79D9: 9E 64           // Pointer to Tie fighter data
                    LDA   $03,X                   ; 79DB: A6 03         
                    CMPA  #$01                    ; 79DD: 81 01         
                    BNE   L79E9                   ; 79DF: 26 08         
                    LDD   $15,X                   ; 79E1: EC 88 15      
                    ORA   #$08                    ; 79E4: 8A 08         
                    STD   $15,X                   ; 79E6: ED 88 15      
L79E9               LDX   <$64                    ; 79E9: 9E 64           // "Pointer to Tie fighter data", Pointer to Tie fighter data
                    LDB   $06,X                   ; 79EB: E6 06         
                    LDU   #word_7A08              ; 79ED: CE 7A 08        // Tie fighter colour table
                    ASLB                          ; 79F0: 58            
                    LDD   B,U                     ; 79F1: EC C5         
                    STD   ,Y++                    ; 79F3: ED A1         
                    LDA   #$40                    ; 79F5: 86 40           // Matrix 1 = Matrix 2 x Matrix 3
                    JSR   $CDBA                   ; 79F7: BD CD BA        // Do math program run
L79FA               LDB   $04,X                   ; 79FA: E6 04         
                    JSR   $CD14                   ; 79FC: BD CD 14        // Math program 0x50. Matrix Multiply - Transposed, Then do perspective division?
L79FF               JSR   $CD2C                   ; 79FF: BD CD 2C        // Format vectors for ties, and tower/bunker explosions
L7A02               LDD   #$8040                  ; 7A02: CC 80 40        // Insert vector CNTR instruction
                    STD   ,Y++                    ; 7A05: ED A1         
; End of function sub_7881
; ---------------------------------------------------------------------------
                    RTS                           ; 7A07: 39            

; =============== S U B R O U T I N E =======================================
L7A48               LDU   #$5090                  ; 7A48: CE 50 90      
                    JSR   $CDC3                   ; 7A4B: BD CD C3        // Initialise math registers matrix
L7A4E               LDD   #$00                    ; 7A4E: CC 00 00      
                    STD   $5098                   ; 7A51: FD 50 98      
                    STD   $509A                   ; 7A54: FD 50 9A      
                    STD   $509C                   ; 7A57: FD 50 9C      
L7A5A               LDD   #$00                    ; 7A5A: CC 00 00      
                    STA   <$62                    ; 7A5D: 97 62           // Timer for fireball hit?
                    STA   <$63                    ; 7A5F: 97 63         
                    STA   <$31                    ; 7A61: 97 31         
                    STA   <$BC                    ; 7A63: 97 BC         
                    STA   <$B7                    ; 7A65: 97 B7         
                    STA   <$BD                    ; 7A67: 97 BD         
                    STD   <$A3                    ; 7A69: DD A3         
                    STA   $4878                   ; 7A6B: B7 48 78      
                    STA   $486E                   ; 7A6E: B7 48 6E      
                    STA   $4877                   ; 7A71: B7 48 77      
                    STD   $4874                   ; 7A74: FD 48 74      
                    STD   $486B                   ; 7A77: FD 48 6B      
; End of function sub_7A48
; ---------------------------------------------------------------------------
                    RTS                           ; 7A7A: 39            

