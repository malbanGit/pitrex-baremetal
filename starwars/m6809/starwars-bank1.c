                   // ORG   $6000
    // ===========================================================================
    // Segment type: Pure code
  case 0x6000:
L6000_:
                                                     #ifdef DEBUG
                                      mon("L6000_   JMP   $F261                   ; 6000: 7E F2 61      \n");
                                      #endif // segment ROM
    PC = 0x6000; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    PC = 0xf261; 
    JUMP;
    
                   // FCB   $EF                     ; 6003:  ' ' EF 56          STU   -10,U ; 
                   // FCB   $56                     ; 6004:  'V' 56             RORB  ; V
    // =============== S U B R O U T I N E =======================================
  case 0x670D:
L670D_:
                                                     #ifdef DEBUG
                                      mon("L670D    LDX   #$496F                  ; 670D: 8E 49 6F      \n");
                                      #endif // "Bank 1"
    PC = 0x670d; 
    INSTRUCTION_START
    clockticks = clockticks + 3; 
    X = 0x496f; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$00                    ; 6710: 86 00         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x00; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
  case 0x6712:
L6712_:
                                                     #ifdef DEBUG
                                      mon("L6712_   STA   ,X+                     ; 6712: A7 80         \n");
                                      #endif
    PC = 0x6712; 
    INSTRUCTION_START
    clockticks = clockticks + 6; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = X; 
    wr_mem(ea, val); 
    X = X + 1; 
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$4989                  ; 6714: 8C 49 89      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0x4989; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BCS   $6712                   ; 6717: 25 F9         \n");
                                      #endif
    // temp hack to force a flush
    if (C) {
    clockticks = clockticks + 3; 
    goto L6712_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDA   #$60                    ; 6719: 86 60         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x60; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 671B: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         STA   <$D2                    ; 671C: 97 D2         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0xd2; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$5300                  ; 671E: CC 53 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x53; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   <$D4                    ; 6721: DD D4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    Z = A | B; 
    N = A; 
    V = 0; 
    ea = DP|0xd4; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    // End of function sub_670D
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 6723: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6724:
L6724_:
                                                     #ifdef DEBUG
                                      mon("L6724    STB   <$DC                    ; 6724: D7 DC         \n");
                                      #endif // Selected object model
    PC = 0x6724; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = B; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0xdc; 
    wr_mem(ea, val); 
  case 0x6726:
L6726_:
                                                     #ifdef DEBUG
                                      mon("L6726    LDB   <$DC                    ; 6726: D6 DC         \n");
                                      #endif // Selected object model
    PC = 0x6726; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$496F                  ; 6728: 8E 49 6F      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x496f; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 672B: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BNE   $6760                   ; 672D: 26 31         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    clockticks = clockticks + 3; 
    goto L6760_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDA   <$D2                    ; 672F: 96 D2         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xd2); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STA   B,X                     ; 6731: A7 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = (UINT16)(X + (SINT8)B); 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7644                  ; 6733: 8E 76 44      \n");
                                      #endif // Array of object sizes?
    clockticks = clockticks + 3; 
    X = 0x7644; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 6736: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STA   <$01                    ; 6738: 97 01         \n");
                                      #endif // Number of points in object?
    clockticks = clockticks + 4; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x01; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 673A: 44            \n");
                                      #endif
    val = (UINT8)A; 
 // C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ADDA  <$D2                    ; 673B: 9B D2         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)A; 
    arg = rd_mem(DP|0xd2); 
    res = val + (UINT8)arg; 
    C = (res >> 1) & 0x80; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    H = (UINT8)(val ^ arg ^ res ^ C); 
    V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         BCC   $6740                   ; 673D: 24 01         \n");
                                      #endif
    // temp hack to force a flush
    if (!C) {
    clockticks = clockticks + 3; 
    goto L6740_;
    }
                                                     #ifdef DEBUG
                                      mon("         SWI                           ; 673F: 3F            \n");
                                      #endif
    clockticks = clockticks + 19; 
    --S;
    wr_mem(S, 0x40); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x67); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, U); 
    --S;
    wr_mem(S, U >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, Y); 
    --S;
    wr_mem(S, Y >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, (DP >> 8)); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, A); 
    clockticks = clockticks + 1; 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    clockticks = clockticks + 1; 
    PC = (rd_mem(0xfffa)<<8)|rd_mem(0xfffb); 
    JUMP;
  case 0x6740:
L6740_:
                                                     #ifdef DEBUG
                                      mon("L6740_   STA   <$D2                    ; 6740: 97 D2         \n");
                                      #endif
    PC = 0x6740; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0xd2; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$765E                  ; 6742: 8E 76 5E      \n");
                                      #endif // Table of pointers to object X, Y, Z point data?
    clockticks = clockticks + 3; 
    X = 0x765e; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ASLB                          ; 6745: 58            \n");
                                      #endif
    val = (UINT8)B; 
    res = val << 1; 
    C = res & 0x100; 
    res = (UINT8)res; 
 // Z = res; 
 // N = res; 
 // V = val ^ res; 
    B = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDX   B,X                     ; 6746: AE 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + (SINT8)B))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + (SINT8)B))+1)); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDU   <$D4                    ; 6748: DE D4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xd4)<<8; 
    ea = ea | rd_mem(DP|0xd5); 
    U = ea; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
  case 0x674A:
L674A_:
                                                     #ifdef DEBUG
                                      mon("L674A_   LDD   ,X                      ; 674A: EC 84         \n");
                                      #endif
    PC = 0x674a; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,U                      ; 674C: ED C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = U; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $02,X                   ; 674E: EC 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
    ea = (UINT16)(X + 0x0002); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,U                   ; 6750: ED 42         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(U + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $04,X                   ; 6752: EC 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    ea = (UINT16)(X + 0x0004); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,U                   ; 6754: ED 44         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(U + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAX  $06,X                   ; 6756: 30 06         \n");
                                      #endif
    clockticks = clockticks + 5; 
    X = (X + 0x0006); 
 // Z = X; 
                                                     #ifdef DEBUG
                                      mon("         LEAU  $08,U                   ; 6758: 33 48         \n");
                                      #endif
    clockticks = clockticks + 5; 
    U = (U + 0x0008); 
                                                     #ifdef DEBUG
                                      mon("         DEC   <$01                    ; 675A: 0A 01         \n");
                                      #endif // Loop count for object points
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x01); 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    wr_mem(DP|0x01, res); 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $674A                   ; 675C: 2E EC         \n");
                                      #endif // Copy XYZ data to math RAM
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L674A_;
    }
                                                     #ifdef DEBUG
                                      mon("         STU   <$D4                    ; 675E: DF D4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = U; 
    Z = val; 
    N = (UINT8)(val >> 8); 
    V = 0; 
    ea = DP|0xd4; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
    // End of function sub_6724
  case 0x6760:
L6760_:
                                                     #ifdef DEBUG
                                      mon("L6760_   RTS                           ; 6760: 39            \n");
                                      #endif
    PC = 0x6760; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6761:
L6761_:
                                                     #ifdef DEBUG
                                      mon("L6761    LDD   $4700                   ; 6761: FC 47 00      \n");
                                      #endif // Called during space, towers and trench
    PC = 0x6761; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x4700); // Care needed with I/O space and word fetches
    B = rd_mem(0x4701); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5000                   ; 6764: FD 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5000, A); // Care needed with I/O space and word fetches
    wr_mem(0x5001, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$86                    ; 6767: 86 86         \n");
                                      #endif // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
    clockticks = clockticks + 2; 
    val = (UINT8)0x86; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 6769: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x6c); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x67); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x676C:
L676C_:
                                                     #ifdef DEBUG
                                      mon("L676C_   LDD   $5004                   ; 676C: FC 50 04      \n");
                                      #endif // Math result Z
    PC = 0x676c; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   <$D8                    ; 676F: DD D8         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = DP|0xd8; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$FF98                  ; 6771: C3 FF 98      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0xff98; 
    arg = (A << 8) | B; 
    res = arg + val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6774: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 6776: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 6778: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   <$D6                    ; 677B: DD D6         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = DP|0xd6; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 677D: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 677F: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    Z = A | B; 
    N = A; 
    V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
    // End of function sub_6761
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 6781: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6782:
L6782_:
                                                     #ifdef DEBUG
                                      mon("L6782    LDD   $4700                   ; 6782: FC 47 00      \n");
                                      #endif // Calc trench floor lines
    PC = 0x6782; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x4700); // Care needed with I/O space and word fetches
    B = rd_mem(0x4701); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5000                   ; 6785: FD 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5000, A); // Care needed with I/O space and word fetches
    wr_mem(0x5001, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$86                    ; 6788: 86 86         \n");
                                      #endif // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
    clockticks = clockticks + 2; 
    val = (UINT8)0x86; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 678A: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x8d); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x67); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x678D:
L678D_:
                                                     #ifdef DEBUG
                                      mon("L678D_   LDD   $5004                   ; 678D: FC 50 04      \n");
                                      #endif // Math result Z
    PC = 0x678d; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$D8                    ; 6790: 93 D8         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xd8)<<8; 
    ea = ea | rd_mem(DP|0xd9); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6792: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 6794: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 6796: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   <$D8                    ; 6799: DD D8         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = DP|0xd8; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 679B: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$D6                    ; 679E: 93 D6         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xd6)<<8; 
    ea = ea | rd_mem(DP|0xd7); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 67A0: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 67A2: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 67A4: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   <$D6                    ; 67A7: DD D6         \n");
                                      #endif
    clockticks = clockticks + 4; 
    Z = A | B; 
    N = A; 
    V = 0; 
    ea = DP|0xd6; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    // End of function sub_6782
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 67A9: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x67AA:
L67AA_:
                                                     #ifdef DEBUG
                                      mon("L67AA    LDD   $4700                   ; 67AA: FC 47 00      \n");
                                      #endif // Calc trench vertical lines on walls
    PC = 0x67aa; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x4700); // Care needed with I/O space and word fetches
    B = rd_mem(0x4701); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5000                   ; 67AD: FD 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5000, A); // Care needed with I/O space and word fetches
    wr_mem(0x5001, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$86                    ; 67B0: 86 86         \n");
                                      #endif // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
    clockticks = clockticks + 2; 
    val = (UINT8)0x86; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 67B2: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0xb5); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x67); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x67B5:
L67B5_:
                                                     #ifdef DEBUG
                                      mon("L67B5_   LDD   $5004                   ; 67B5: FC 50 04      \n");
                                      #endif // Math result Z
    PC = 0x67b5; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$D8                    ; 67B8: 93 D8         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xd8)<<8; 
    ea = ea | rd_mem(DP|0xd9); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 67BA: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 67BC: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 67BE: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   <$D8                    ; 67C1: DD D8         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = DP|0xd8; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 67C3: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$D6                    ; 67C6: 93 D6         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xd6)<<8; 
    ea = ea | rd_mem(DP|0xd7); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 67C8: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 67CA: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 67CC: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   <$D6                    ; 67CF: DD D6         \n");
                                      #endif
    clockticks = clockticks + 4; 
    Z = A | B; 
    N = A; 
    V = 0; 
    ea = DP|0xd6; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    // End of function sub_67AA
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 67D1: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
    // End of function sub_67D2
  case 0x67D2:
L67D2_:
                                                     #ifdef DEBUG
                                      mon("L67D2    STB   <$DC                    ; 67D2: D7 DC         \n");
                                      #endif // Called during space
    PC = 0x67d2; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = B; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0xdc; 
    wr_mem(ea, val); 
    // =============== S U B R O U T I N E =======================================
  case 0x67D4:
L67D4_:
                                                     #ifdef DEBUG
                                      mon("L67D4    LDB   <$DC                    ; 67D4: D6 DC         \n");
                                      #endif // Selected object model
    PC = 0x67d4; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7644                  ; 67D6: 8E 76 44      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x7644; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 67D9: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STA   <$01                    ; 67DB: 97 01         \n");
                                      #endif // Loop count for object points
    clockticks = clockticks + 4; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x01; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$496F                  ; 67DD: 8E 49 6F      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x496f; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 67E0: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BNE   $67E5                   ; 67E2: 26 01         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    clockticks = clockticks + 3; 
    goto L67E5_;
    }
                                                     #ifdef DEBUG
                                      mon("         SWI                           ; 67E4: 3F            \n");
                                      #endif
    clockticks = clockticks + 19; 
    --S;
    wr_mem(S, 0xe5); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x67); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, U); 
    --S;
    wr_mem(S, U >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, Y); 
    --S;
    wr_mem(S, Y >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, (DP >> 8)); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, A); 
    clockticks = clockticks + 1; 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    clockticks = clockticks + 1; 
    PC = (rd_mem(0xfffa)<<8)|rd_mem(0xfffb); 
    JUMP;
  case 0x67E5:
L67E5_:
                                                     #ifdef DEBUG
                                      mon("L67E5_   ASLA                          ; 67E5: 48            \n");
                                      #endif
    PC = 0x67e5; 
    INSTRUCTION_START
    val = (UINT8)A; 
    res = val << 1; 
    C = res & 0x100; 
    res = (UINT8)res; 
 // Z = res; 
 // N = res; 
 // V = val ^ res; 
    A = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         STA   $4702                   ; 67E6: B7 47 02      \n");
                                      #endif // Set BIC
    clockticks = clockticks + 5; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    wr_mem(0x4702, val); 
                                                     #ifdef DEBUG
                                      mon("         ROLA                          ; 67E9: 49            \n");
                                      #endif
    val = (UINT8)A; 
    res = (val << 1) + (C != 0 ?1:0); 
    C = res & 0x100; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
 // V = (UINT8)res ^ val; 
    A = (UINT8)res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         STA   $4701                   ; 67EA: B7 47 01      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    wr_mem(0x4701, val); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$4C00                  ; 67ED: 8E 4C 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x4c00; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
  case 0x67F0:
L67F0_:
                                                     #ifdef DEBUG
                                      mon("L67F0_   LDA   #$50                    ; 67F0: 86 50         \n");
                                      #endif // Math program 0x50. Matrix Multiply - Transposed
    PC = 0x67f0; 
    INSTRUCTION_START
    clockticks = clockticks + 2; 
    val = (UINT8)0x50; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 67F2: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0xf5); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x67); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x67F5:
L67F5_:
                                                     #ifdef DEBUG
                                      mon("L67F5_   LDD   $5000                   ; 67F5: FC 50 00      \n");
                                      #endif // Math result X
    PC = 0x67f5; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5000); // Care needed with I/O space and word fetches
    B = rd_mem(0x5001); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4704                   ; 67F8: FD 47 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4704, A); // Care needed with I/O space and word fetches
    wr_mem(0x4705, B); 
                                                     #ifdef DEBUG
                                      mon("         ORCC  #$00                    ; 67FB: 1A 00         \n");
                                      #endif
    clockticks = clockticks + 3; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $4700                   ; 67FD: FC 47 00      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x4700); // Care needed with I/O space and word fetches
    B = rd_mem(0x4701); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5000                   ; 6800: FD 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5000, A); // Care needed with I/O space and word fetches
    wr_mem(0x5001, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$86                    ; 6803: 86 86         \n");
                                      #endif // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
    clockticks = clockticks + 2; 
    val = (UINT8)0x86; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 6805: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x08); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x68); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x6808:
L6808_:
                                                     #ifdef DEBUG
                                      mon("L6808_   LDD   $5002                   ; 6808: FC 50 02      \n");
                                      #endif // Math result Y
    PC = 0x6808; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,X                      ; 680B: ED 84         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = X; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 680D: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,X                   ; 6810: ED 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAX  $04,X                   ; 6812: 30 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    X = (X + 0x0004); 
 // Z = X; 
                                                     #ifdef DEBUG
                                      mon("         DEC   <$01                    ; 6814: 0A 01         \n");
                                      #endif // Loop until all object points done
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x01); 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    wr_mem(DP|0x01, res); 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $67F0                   ; 6816: 2E D8         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L67F0_;
    }
    // End of function sub_67D4
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 6818: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
    // Format vectors for ties, and tower/bunker explosions
  case 0x6819:
L6819_:
                                                     #ifdef DEBUG
                                      mon("L6819    LDX   #$7610                  ; 6819: 8E 76 10      \n");
                                      #endif // Called during space and towers
    PC = 0x6819; 
    INSTRUCTION_START
    clockticks = clockticks + 3; 
    X = 0x7610; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$DC                    ; 681C: D6 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ASLB                          ; 681E: 58            \n");
                                      #endif
    val = (UINT8)B; 
    res = val << 1; 
    C = res & 0x100; 
    res = (UINT8)res; 
 // Z = res; 
 // N = res; 
 // V = val ^ res; 
    B = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDU   B,X                     ; 681F: EE 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + (SINT8)B))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + (SINT8)B))+1)); 
    U = ea; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   ,U+                     ; 6821: E6 C0         \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = (UINT8)rd_mem(U); 
    B = val; 
    N = val; 
    Z = val; 
    V = 0; 
    U = U + 1; 
                                                     #ifdef DEBUG
                                      mon("         BEQ   $6826                   ; 6823: 27 01         \n");
                                      #endif
    // temp hack to force a flush
    if (!Z) {
    clockticks = clockticks + 3; 
    goto L6826_;
    }
                                                     #ifdef DEBUG
                                      mon("         SWI                           ; 6825: 3F            \n");
                                      #endif
    clockticks = clockticks + 19; 
    --S;
    wr_mem(S, 0x26); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x68); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, U); 
    --S;
    wr_mem(S, U >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, Y); 
    --S;
    wr_mem(S, Y >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, (DP >> 8)); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, A); 
    clockticks = clockticks + 1; 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    clockticks = clockticks + 1; 
    PC = (rd_mem(0xfffa)<<8)|rd_mem(0xfffb); 
    JUMP;
  case 0x6826:
L6826_:
                                                     #ifdef DEBUG
                                      mon("L6826_   LDB   ,U+                     ; 6826: E6 C0         \n");
                                      #endif
    PC = 0x6826; 
    INSTRUCTION_START
    clockticks = clockticks + 6; 
    val = (UINT8)rd_mem(U); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
    U = U + 1; 
                                                     #ifdef DEBUG
                                      mon("         BITB  #$02                    ; 6828: C5 02         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = B & (SINT8)0x02; 
    Z = res; 
    N = res; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BEQ   $6838                   ; 682A: 27 0C         \n");
                                      #endif
    // temp hack to force a flush
    if (!Z) {
    clockticks = clockticks + 3; 
    goto L6838_;
    }
                                                     #ifdef DEBUG
                                      mon("         CMPB  #$FF                    ; 682C: C1 FF         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    arg = 0xff; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
                                                     #ifdef DEBUG
                                      mon("         BEQ   $6863                   ; 682E: 27 33         \n");
                                      #endif
    // temp hack to force a flush
    if (!Z) {
    clockticks = clockticks + 3; 
    goto L6863_;
    }
                                                     #ifdef DEBUG
                                      mon("         LSRB                          ; 6830: 54            \n");
                                      #endif
    val = (UINT8)B; 
 // C = val & 1; 
    val = val >> 1; 
    B = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LSRB                          ; 6831: 54            \n");
                                      #endif
    val = (UINT8)B; 
 // C = val & 1; 
    val = val >> 1; 
    B = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         CMPB  <$DB                    ; 6832: D1 DB         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)B; 
    arg = rd_mem(DP|0xdb); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
                                                     #ifdef DEBUG
                                      mon("         BHI   $6826                   ; 6834: 22 F0         \n");
                                      #endif
    // temp hack to force a flush
    if (Z && (!C)) {
    clockticks = clockticks + 3; 
    goto L6826_;
    }
                                                     #ifdef DEBUG
                                      mon("         BRA   $6863                   ; 6836: 20 2B         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L6863_;
    
    // ---------------------------------------------------------------------------
  case 0x6838:
L6838_:
                                                     #ifdef DEBUG
                                      mon("L6838_   CLRA                          ; 6838: 4F            \n");
                                      #endif
    PC = 0x6838; 
    INSTRUCTION_START
    A = 0; // SEARCH_ME
 // V = 0; 
 // Z = 0; 
 // N = 0; 
    C = 0; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BITB  #$03                    ; 6839: C5 03         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = B & (SINT8)0x03; 
    Z = res; 
    N = res; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BNE   $683F                   ; 683B: 26 02         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    clockticks = clockticks + 3; 
    goto L683F_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDA   #$E0                    ; 683D: 86 E0         \n");
                                      #endif // Vector is visible
    clockticks = clockticks + 2; 
    val = (UINT8)0xe0; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
  case 0x683F:
L683F_:
                                                     #ifdef DEBUG
                                      mon("L683F_   STA   <$DA                    ; 683F: 97 DA         \n");
                                      #endif
    PC = 0x683f; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0xda; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$FC                    ; 6841: C4 FC         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    res = val & (SINT8)0xfc; 
    B = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$4C00                  ; 6843: 8E 4C 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x4c00; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ABX                           ; 6846: 3A            \n");
                                      #endif
    X = X + B; 
    clockticks = clockticks + 3; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $02,X                   ; 6847: EC 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
    ea = (UINT16)(X + 0x0002); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$D8                    ; 6849: 93 D8         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xd8)<<8; 
    ea = ea | rd_mem(DP|0xd9); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 684B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 684D: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   ,X                      ; 684F: EC 84         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$D6                    ; 6851: 93 D6         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xd6)<<8; 
    ea = ea | rd_mem(DP|0xd7); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6853: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ORA   <$DA                    ; 6855: 9A DA         \n");
                                      #endif
    clockticks = clockticks + 4; 
    res = A | (UINT8)rd_mem(DP|0xda); 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 6857: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   ,X                      ; 6859: EC 84         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   <$D6                    ; 685B: DD D6         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = DP|0xd6; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $02,X                   ; 685D: EC 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
    ea = (UINT16)(X + 0x0002); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   <$D8                    ; 685F: DD D8         \n");
                                      #endif
    clockticks = clockticks + 4; 
    Z = A | B; 
    N = A; 
    V = 0; 
    ea = DP|0xd8; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         BRA   $6826                   ; 6861: 20 C3         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L6826_;
    
    // ---------------------------------------------------------------------------
    // End of function sub_6819
  case 0x6863:
L6863_:
                                                     #ifdef DEBUG
                                      mon("L6863_   RTS                           ; 6863: 39            \n");
                                      #endif
    PC = 0x6863; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6864:
L6864_:
                                                     #ifdef DEBUG
                                      mon("L6864    LDD   #$00                    ; 6864: CC 00 00      \n");
                                      #endif // Left hand side of trench turret calculation
    PC = 0x6864; 
    INSTRUCTION_START
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5040                   ; 6867: FD 50 40      \n");
                                      #endif // XT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5040, A); // Care needed with I/O space and word fetches
    wr_mem(0x5041, B); 
                                                     #ifdef DEBUG
                                      mon("         STD   $5042                   ; 686A: FD 50 42      \n");
                                      #endif // Clear XT, YT and ZT to zero
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5042, A); // Care needed with I/O space and word fetches
    wr_mem(0x5043, B); 
                                                     #ifdef DEBUG
                                      mon("         STD   $5044                   ; 686D: FD 50 44      \n");
                                      #endif // ZT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5044, A); // Care needed with I/O space and word fetches
    wr_mem(0x5045, B); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7644                  ; 6870: 8E 76 44      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x7644; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$DC                    ; 6873: D6 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 6875: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STA   <$01                    ; 6877: 97 01         \n");
                                      #endif // Loop count for object points
    clockticks = clockticks + 4; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x01; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$765E                  ; 6879: 8E 76 5E      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x765e; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ASLB                          ; 687C: 58            \n");
                                      #endif
    val = (UINT8)B; 
    res = val << 1; 
    C = res & 0x100; 
    res = (UINT8)res; 
 // Z = res; 
 // N = res; 
 // V = val ^ res; 
    B = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDU   B,X                     ; 687D: EE 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + (SINT8)B))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + (SINT8)B))+1)); 
    U = ea; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$5DF0                  ; 687F: 8E 5D F0      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x5df0; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
  case 0x6882:
L6882_:
                                                     #ifdef DEBUG
                                      mon("L6882_   LDD   ,U++                    ; 6882: EC C1         \n");
                                      #endif
    PC = 0x6882; 
    INSTRUCTION_START
    clockticks = clockticks + 7; 
    A = rd_mem(U); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(U+1)); 
    U = U + 2; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5E00                   ; 6884: FD 5E 00      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5e00, A); // Care needed with I/O space and word fetches
    wr_mem(0x5e01, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   ,U++                    ; 6887: EC C1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    A = rd_mem(U); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(U+1)); 
    U = U + 2; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5E02                   ; 6889: FD 5E 02      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5e02, A); // Care needed with I/O space and word fetches
    wr_mem(0x5e03, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   ,U++                    ; 688C: EC C1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    A = rd_mem(U); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(U+1)); 
    U = U + 2; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5E04                   ; 688E: FD 5E 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5e04, A); // Care needed with I/O space and word fetches
    wr_mem(0x5e05, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$01C0                  ; 6891: CC 01 C0      \n");
                                      #endif // Point BIC to Math RAM $5E00
    clockticks = clockticks + 3; 
    A = 0x01; 
    B = 0xc0; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4701                   ; 6894: FD 47 01      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4701, A); // Care needed with I/O space and word fetches
    wr_mem(0x4702, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$2A                    ; 6897: 86 2A         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x2a; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 6899: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x9c); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x68); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x689C:
L689C_:
                                                     #ifdef DEBUG
                                      mon("L689C_   LDD   $5000                   ; 689C: FC 50 00      \n");
                                      #endif // Math result X
    PC = 0x689c; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5000); // Care needed with I/O space and word fetches
    B = rd_mem(0x5001); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,X                   ; 689F: ED 06         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 68A1: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,X                   ; 68A4: ED 88 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 68A7: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,X                   ; 68AA: ED 88 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAX  $10,X                   ; 68AD: 30 88 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
    X = (X + (SINT8)0x10); 
 // Z = X; 
                                                     #ifdef DEBUG
                                      mon("         DEC   <$01                    ; 68B0: 0A 01         \n");
                                      #endif // Loop until all object points done
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x01); 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    wr_mem(DP|0x01, res); 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $6882                   ; 68B2: 2E CE         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L6882_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   $5098                   ; 68B4: FC 50 98      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x5098); // Care needed with I/O space and word fetches
    B = rd_mem(0x5099); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5040                   ; 68B7: FD 50 40      \n");
                                      #endif // XT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5040, A); // Care needed with I/O space and word fetches
    wr_mem(0x5041, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $509A                   ; 68BA: FC 50 9A      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x509a); // Care needed with I/O space and word fetches
    B = rd_mem(0x509b); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5042                   ; 68BD: FD 50 42      \n");
                                      #endif // YT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5042, A); // Care needed with I/O space and word fetches
    wr_mem(0x5043, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $509C                   ; 68C0: FC 50 9C      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x509c); // Care needed with I/O space and word fetches
    B = rd_mem(0x509d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5044                   ; 68C3: FD 50 44      \n");
                                      #endif // ZT2
    clockticks = clockticks + 5; 
    Z = A | B; 
    N = A; 
    V = 0; 
    wr_mem(0x5044, A); // Care needed with I/O space and word fetches
    wr_mem(0x5045, B); 
    // End of function sub_6864
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 68C6: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x68C7:
L68C7_:
                                                     #ifdef DEBUG
                                      mon("L68C7    LDD   #$00                    ; 68C7: CC 00 00      \n");
                                      #endif // Right hand side of trench turret calculation
    PC = 0x68c7; 
    INSTRUCTION_START
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5040                   ; 68CA: FD 50 40      \n");
                                      #endif // XT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5040, A); // Care needed with I/O space and word fetches
    wr_mem(0x5041, B); 
                                                     #ifdef DEBUG
                                      mon("         STD   $5042                   ; 68CD: FD 50 42      \n");
                                      #endif // Clear XT, YT and ZT to zero
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5042, A); // Care needed with I/O space and word fetches
    wr_mem(0x5043, B); 
                                                     #ifdef DEBUG
                                      mon("         STD   $5044                   ; 68D0: FD 50 44      \n");
                                      #endif // ZT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5044, A); // Care needed with I/O space and word fetches
    wr_mem(0x5045, B); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7644                  ; 68D3: 8E 76 44      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x7644; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$DC                    ; 68D6: D6 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 68D8: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STA   <$01                    ; 68DA: 97 01         \n");
                                      #endif // Loop count for object points
    clockticks = clockticks + 4; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x01; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$765E                  ; 68DC: 8E 76 5E      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x765e; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ASLB                          ; 68DF: 58            \n");
                                      #endif
    val = (UINT8)B; 
    res = val << 1; 
 // C = res & 0x100; 
    res = (UINT8)res; 
 // Z = res; 
 // N = res; 
 // V = val ^ res; 
    B = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDU   B,X                     ; 68E0: EE 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + (SINT8)B))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + (SINT8)B))+1)); 
    U = ea; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$5DF0                  ; 68E2: 8E 5D F0      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x5df0; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
  case 0x68E5:
L68E5_:
                                                     #ifdef DEBUG
                                      mon("L68E5_   LDD   ,U++                    ; 68E5: EC C1         \n");
                                      #endif
    PC = 0x68e5; 
    INSTRUCTION_START
    clockticks = clockticks + 7; 
    A = rd_mem(U); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(U+1)); 
    U = U + 2; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5E00                   ; 68E7: FD 5E 00      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5e00, A); // Care needed with I/O space and word fetches
    wr_mem(0x5e01, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 68EA: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  ,U++                    ; 68ED: A3 C1         \n");
                                      #endif // Negate
    clockticks = clockticks + 1; 
    clockticks = clockticks + 7; 
    clockticks = clockticks + 1; 
    ea = rd_mem(U)<<8; 
    ea = ea | rd_mem((UINT16)((U)+1)); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
    U = U + 2; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5E02                   ; 68EF: FD 5E 02      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5e02, A); // Care needed with I/O space and word fetches
    wr_mem(0x5e03, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   ,U++                    ; 68F2: EC C1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    A = rd_mem(U); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(U+1)); 
    U = U + 2; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5E04                   ; 68F4: FD 5E 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5e04, A); // Care needed with I/O space and word fetches
    wr_mem(0x5e05, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$01C0                  ; 68F7: CC 01 C0      \n");
                                      #endif // Point BIC to Math RAM $5E00
    clockticks = clockticks + 3; 
    A = 0x01; 
    B = 0xc0; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4701                   ; 68FA: FD 47 01      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4701, A); // Care needed with I/O space and word fetches
    wr_mem(0x4702, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$2A                    ; 68FD: 86 2A         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x2a; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 68FF: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x02); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x69); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x6902:
L6902_:
                                                     #ifdef DEBUG
                                      mon("L6902_   LDD   $5000                   ; 6902: FC 50 00      \n");
                                      #endif // Math result X
    PC = 0x6902; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5000); // Care needed with I/O space and word fetches
    B = rd_mem(0x5001); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,X                   ; 6905: ED 06         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 6907: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,X                   ; 690A: ED 88 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 690D: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,X                   ; 6910: ED 88 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAX  $10,X                   ; 6913: 30 88 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
    X = (X + (SINT8)0x10); 
 // Z = X; 
                                                     #ifdef DEBUG
                                      mon("         DEC   <$01                    ; 6916: 0A 01         \n");
                                      #endif // Loop count for object points
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x01); 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    wr_mem(DP|0x01, res); 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $68E5                   ; 6918: 2E CB         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L68E5_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   $5098                   ; 691A: FC 50 98      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x5098); // Care needed with I/O space and word fetches
    B = rd_mem(0x5099); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5040                   ; 691D: FD 50 40      \n");
                                      #endif // XT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5040, A); // Care needed with I/O space and word fetches
    wr_mem(0x5041, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $509A                   ; 6920: FC 50 9A      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x509a); // Care needed with I/O space and word fetches
    B = rd_mem(0x509b); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5042                   ; 6923: FD 50 42      \n");
                                      #endif // YT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5042, A); // Care needed with I/O space and word fetches
    wr_mem(0x5043, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $509C                   ; 6926: FC 50 9C      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x509c); // Care needed with I/O space and word fetches
    B = rd_mem(0x509d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5044                   ; 6929: FD 50 44      \n");
                                      #endif // ZT2
    clockticks = clockticks + 5; 
    Z = A | B; 
    N = A; 
    V = 0; 
    wr_mem(0x5044, A); // Care needed with I/O space and word fetches
    wr_mem(0x5045, B); 
    // End of function sub_68C7
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 692C: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x692D:
L692D_:
                                                     #ifdef DEBUG
                                      mon("L692D    LDD   #$00                    ; 692D: CC 00 00      \n");
                                      #endif // Trench calculation
    PC = 0x692d; 
    INSTRUCTION_START
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5000                   ; 6930: B3 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5000)<<8; 
    ea = ea | rd_mem(0x5001); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5000                   ; 6933: FD 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5000, A); // Care needed with I/O space and word fetches
    wr_mem(0x5001, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 6936: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5002                   ; 6939: B3 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5002)<<8; 
    ea = ea | rd_mem(0x5003); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5002                   ; 693C: FD 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5002, A); // Care needed with I/O space and word fetches
    wr_mem(0x5003, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 693F: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5004                   ; 6942: B3 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5004)<<8; 
    ea = ea | rd_mem(0x5005); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5004                   ; 6945: FD 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5004, A); // Care needed with I/O space and word fetches
    wr_mem(0x5005, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$01C0                  ; 6948: CC 01 C0      \n");
                                      #endif // Point BIC to Math RAM $5E00
    clockticks = clockticks + 3; 
    A = 0x01; 
    B = 0xc0; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4701                   ; 694B: FD 47 01      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4701, A); // Care needed with I/O space and word fetches
    wr_mem(0x4702, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5DF6                   ; 694E: FC 5D F6      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x5df6); // Care needed with I/O space and word fetches
    B = rd_mem(0x5df7); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5000                   ; 6951: B3 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5000)<<8; 
    ea = ea | rd_mem(0x5001); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4704                   ; 6954: FD 47 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4704, A); // Care needed with I/O space and word fetches
    wr_mem(0x4705, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$B0                    ; 6957: 86 B0         \n");
                                      #endif // XT = [BIC,3] - Reg00, BIC++
    clockticks = clockticks + 2; 
    val = (UINT8)0xb0; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STA   $4700                   ; 6959: B7 47 00      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    wr_mem(0x4700, val); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7644                  ; 695C: 8E 76 44      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x7644; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$DC                    ; 695F: D6 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 6961: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   #$AE                    ; 6963: C6 AE         \n");
                                      #endif // [BIC,2] = ([BIC,0] - Reg01) x (YT), [BIC,3] = ([BIC,1] - Reg02) x (YT), BIC++, XT = (BIC,3) - Reg00, BIC++
    clockticks = clockticks + 2; 
    val = (UINT8)0xae; 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
  case 0x6965:
L6965_:
                                                     #ifdef DEBUG
                                      mon("L6965_   LDU   $4700                   ; 6965: FE 47 00      \n");
                                      #endif
    PC = 0x6965; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x4700)<<8; 
    ea = ea | rd_mem(0x4701); 
    U = ea; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDX   $5018                   ; 6968: BE 50 18      \n");
                                      #endif // XT
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5018)<<8; 
    ea = ea | rd_mem(0x5019); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STU   $501A                   ; 696B: FF 50 1A      \n");
                                      #endif // YT
    clockticks = clockticks + 5; 
    val = U; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    wr_mem(0x501a, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x501b, (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         STB   $4700                   ; 696E: F7 47 00      \n");
                                      #endif // Run math program $AE
    clockticks = clockticks + 5; 
    val = B; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    wr_mem(0x4700, val); 
                                                     #ifdef DEBUG
                                      mon("         STX   $4704                   ; 6971: BF 47 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = X; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    wr_mem(0x4704, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x4705, (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         DECA                          ; 6974: 4A            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    A = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $6965                   ; 6975: 2E EE         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L6965_;
    }
    // End of function sub_692D
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 6977: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6978:
L6978_:
                                                     #ifdef DEBUG
                                      mon("L6978    LDD   #$00                    ; 6978: CC 00 00      \n");
                                      #endif // Some trench calculation
    PC = 0x6978; 
    INSTRUCTION_START
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5000                   ; 697B: B3 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5000)<<8; 
    ea = ea | rd_mem(0x5001); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5000                   ; 697E: FD 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5000, A); // Care needed with I/O space and word fetches
    wr_mem(0x5001, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 6981: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5002                   ; 6984: B3 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5002)<<8; 
    ea = ea | rd_mem(0x5003); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5002                   ; 6987: FD 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5002, A); // Care needed with I/O space and word fetches
    wr_mem(0x5003, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 698A: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5004                   ; 698D: B3 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5004)<<8; 
    ea = ea | rd_mem(0x5005); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5004                   ; 6990: FD 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5004, A); // Care needed with I/O space and word fetches
    wr_mem(0x5005, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$01C0                  ; 6993: CC 01 C0      \n");
                                      #endif // Point BIC to Math RAM $5E00
    clockticks = clockticks + 3; 
    A = 0x01; 
    B = 0xc0; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4701                   ; 6996: FD 47 01      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4701, A); // Care needed with I/O space and word fetches
    wr_mem(0x4702, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5DF6                   ; 6999: FC 5D F6      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x5df6); // Care needed with I/O space and word fetches
    B = rd_mem(0x5df7); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5000                   ; 699C: B3 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5000)<<8; 
    ea = ea | rd_mem(0x5001); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4704                   ; 699F: FD 47 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4704, A); // Care needed with I/O space and word fetches
    wr_mem(0x4705, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$B0                    ; 69A2: 86 B0         \n");
                                      #endif // XT Reg0C = [BIC,3] - Reg00
    clockticks = clockticks + 2; 
    val = (UINT8)0xb0; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STA   $4700                   ; 69A4: B7 47 00      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    wr_mem(0x4700, val); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7644                  ; 69A7: 8E 76 44      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x7644; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$DC                    ; 69AA: D6 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 69AC: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   #$AE                    ; 69AE: C6 AE         \n");
                                      #endif // [BIC,2] = ([BIC,0] - Reg01) x (YT), [BIC,3] = ([BIC,1] - Reg02) x (YT), BIC++, XT = [BIC,3] - Reg00, BIC++
    clockticks = clockticks + 2; 
    val = (UINT8)0xae; 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
  case 0x69B0:
L69B0_:
                                                     #ifdef DEBUG
                                      mon("L69B0_   LDU   $4700                   ; 69B0: FE 47 00      \n");
                                      #endif
    PC = 0x69b0; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x4700)<<8; 
    ea = ea | rd_mem(0x4701); 
    U = ea; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPU  #$0100                  ; 69B3: 11 83 01 00   \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = U; 
    arg = 0x0100; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $69BC                   ; 69B7: 2E 03         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L69BC_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDU   #$7FFF                  ; 69B9: CE 7F FF      \n");
                                      #endif
    clockticks = clockticks + 3; 
    U = 0x7fff; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
  case 0x69BC:
L69BC_:
                                                     #ifdef DEBUG
                                      mon("L69BC_   LDX   $5018                   ; 69BC: BE 50 18      \n");
                                      #endif // XT
    PC = 0x69bc; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5018)<<8; 
    ea = ea | rd_mem(0x5019); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STU   $501A                   ; 69BF: FF 50 1A      \n");
                                      #endif // YT
    clockticks = clockticks + 5; 
    val = U; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    wr_mem(0x501a, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x501b, (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         STB   $4700                   ; 69C2: F7 47 00      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = B; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    wr_mem(0x4700, val); 
                                                     #ifdef DEBUG
                                      mon("         STX   $4704                   ; 69C5: BF 47 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = X; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    wr_mem(0x4704, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x4705, (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         DECA                          ; 69C8: 4A            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    A = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $69B0                   ; 69C9: 2E E5         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L69B0_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7644                  ; 69CB: 8E 76 44      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x7644; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$DC                    ; 69CE: D6 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 69D0: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDU   #$5E0C                  ; 69D2: CE 5E 0C      \n");
                                      #endif
    clockticks = clockticks + 3; 
    U = 0x5e0c; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
  case 0x69D5:
L69D5_:
                                                     #ifdef DEBUG
                                      mon("L69D5_   LDX   ,U                      ; 69D5: AE C4         \n");
                                      #endif
    PC = 0x69d5; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(U)<<8; 
    ea = ea | rd_mem((UINT16)((U)+1)); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$0216                  ; 69D7: 8C 02 16      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0x0216; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BLE   $69E3                   ; 69DA: 2F 07         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) <  0) || (!Z)) {
    clockticks = clockticks + 3; 
    goto L69E3_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$0216                  ; 69DC: 8E 02 16      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x0216; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STX   ,U                      ; 69DF: AF C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    Z = val; 
    N = (UINT8)(val >> 8); 
    V = 0; 
    ea = U; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         BRA   $69ED                   ; 69E1: 20 0A         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L69ED_;
    
    // ---------------------------------------------------------------------------
  case 0x69E3:
L69E3_:
                                                     #ifdef DEBUG
                                      mon("L69E3_   CMPX  #$FDEA                  ; 69E3: 8C FD EA      \n");
                                      #endif
    PC = 0x69e3; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0xfdea; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BGE   $69ED                   ; 69E6: 2C 05         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)(N^V) >= 0) {
    clockticks = clockticks + 3; 
    goto L69ED_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$FDEA                  ; 69E8: 8E FD EA      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0xfdea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STX   ,U                      ; 69EB: AF C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = U; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
  case 0x69ED:
L69ED_:
                                                     #ifdef DEBUG
                                      mon("L69ED_   LDX   $02,U                   ; 69ED: AE 42         \n");
                                      #endif
    PC = 0x69ed; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(U + 0x0002))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(U + 0x0002))+1)); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$02D8                  ; 69EF: 8C 02 D8      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0x02d8; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BLE   $69FB                   ; 69F2: 2F 07         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) <  0) || (!Z)) {
    clockticks = clockticks + 3; 
    goto L69FB_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$02D8                  ; 69F4: 8E 02 D8      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x02d8; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STX   $02,U                   ; 69F7: AF 42         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = X; 
    Z = val; 
    N = (UINT8)(val >> 8); 
    V = 0; 
    ea = (U + 0x0002); 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         BRA   $6A05                   ; 69F9: 20 0A         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L6A05_;
    
    // ---------------------------------------------------------------------------
  case 0x69FB:
L69FB_:
                                                     #ifdef DEBUG
                                      mon("L69FB_   CMPX  #$FDEC                  ; 69FB: 8C FD EC      \n");
                                      #endif
    PC = 0x69fb; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0xfdec; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BGE   $6A05                   ; 69FE: 2C 05         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)(N^V) >= 0) {
    clockticks = clockticks + 3; 
    goto L6A05_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$FDEC                  ; 6A00: 8E FD EC      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0xfdec; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STX   $02,U                   ; 6A03: AF 42         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = X; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = (U + 0x0002); 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
  case 0x6A05:
L6A05_:
                                                     #ifdef DEBUG
                                      mon("L6A05_   LEAU  $10,U                   ; 6A05: 33 C8 10      \n");
                                      #endif
    PC = 0x6a05; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    U = (U + (SINT8)0x10); 
                                                     #ifdef DEBUG
                                      mon("         DECA                          ; 6A08: 4A            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    A = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $69D5                   ; 6A09: 2E CA         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L69D5_;
    }
    // End of function sub_6978
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 6A0B: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6A0C:
L6A0C_:
                                                     #ifdef DEBUG
                                      mon("L6A0C    LDD   #$00                    ; 6A0C: CC 00 00      \n");
                                      #endif
    PC = 0x6a0c; 
    INSTRUCTION_START
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5000                   ; 6A0F: B3 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5000)<<8; 
    ea = ea | rd_mem(0x5001); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5000                   ; 6A12: FD 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5000, A); // Care needed with I/O space and word fetches
    wr_mem(0x5001, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 6A15: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5002                   ; 6A18: B3 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5002)<<8; 
    ea = ea | rd_mem(0x5003); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5002                   ; 6A1B: FD 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5002, A); // Care needed with I/O space and word fetches
    wr_mem(0x5003, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 6A1E: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5004                   ; 6A21: B3 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5004)<<8; 
    ea = ea | rd_mem(0x5005); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5004                   ; 6A24: FD 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5004, A); // Care needed with I/O space and word fetches
    wr_mem(0x5005, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$01C0                  ; 6A27: CC 01 C0      \n");
                                      #endif // Point BIC to Math RAM $5E00
    clockticks = clockticks + 3; 
    A = 0x01; 
    B = 0xc0; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4701                   ; 6A2A: FD 47 01      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4701, A); // Care needed with I/O space and word fetches
    wr_mem(0x4702, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5DF6                   ; 6A2D: FC 5D F6      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x5df6); // Care needed with I/O space and word fetches
    B = rd_mem(0x5df7); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5000                   ; 6A30: B3 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5000)<<8; 
    ea = ea | rd_mem(0x5001); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4704                   ; 6A33: FD 47 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4704, A); // Care needed with I/O space and word fetches
    wr_mem(0x4705, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$B0                    ; 6A36: 86 B0         \n");
                                      #endif // Reg0C = [BIC,3] - Reg00
    clockticks = clockticks + 2; 
    val = (UINT8)0xb0; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STA   $4700                   ; 6A38: B7 47 00      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    wr_mem(0x4700, val); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7644                  ; 6A3B: 8E 76 44      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x7644; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$DC                    ; 6A3E: D6 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 6A40: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   #$AE                    ; 6A42: C6 AE         \n");
                                      #endif // [BIC,2] = ([BIC,0] - Reg01) x YT, [BIC,3] = ([BIC,1] - Reg02) x YT, BIC++, XT = [BIC,3] - Reg00, BIC++
    clockticks = clockticks + 2; 
    val = (UINT8)0xae; 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
  case 0x6A44:
L6A44_:
                                                     #ifdef DEBUG
                                      mon("L6A44_   LDU   $4700                   ; 6A44: FE 47 00      \n");
                                      #endif
    PC = 0x6a44; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x4700)<<8; 
    ea = ea | rd_mem(0x4701); 
    U = ea; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPU  #$0100                  ; 6A47: 11 83 01 00   \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = U; 
    arg = 0x0100; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $6A50                   ; 6A4B: 2E 03         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L6A50_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDU   #$7FFF                  ; 6A4D: CE 7F FF      \n");
                                      #endif
    clockticks = clockticks + 3; 
    U = 0x7fff; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
  case 0x6A50:
L6A50_:
                                                     #ifdef DEBUG
                                      mon("L6A50_   LDX   $5018                   ; 6A50: BE 50 18      \n");
                                      #endif // XT
    PC = 0x6a50; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5018)<<8; 
    ea = ea | rd_mem(0x5019); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STU   $501A                   ; 6A53: FF 50 1A      \n");
                                      #endif // YT
    clockticks = clockticks + 5; 
    val = U; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    wr_mem(0x501a, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x501b, (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         STB   $4700                   ; 6A56: F7 47 00      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = B; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    wr_mem(0x4700, val); 
                                                     #ifdef DEBUG
                                      mon("         STX   $4704                   ; 6A59: BF 47 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = X; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    wr_mem(0x4704, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x4705, (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         DECA                          ; 6A5C: 4A            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    A = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $6A44                   ; 6A5D: 2E E5         \n");
                                      #endif // Loop until all object points done
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L6A44_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7644                  ; 6A5F: 8E 76 44      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x7644; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$DC                    ; 6A62: D6 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDA   B,X                     ; 6A64: A6 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)B)); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDU   #$5E0C                  ; 6A66: CE 5E 0C      \n");
                                      #endif
    clockticks = clockticks + 3; 
    U = 0x5e0c; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
  case 0x6A69:
L6A69_:
                                                     #ifdef DEBUG
                                      mon("L6A69_   LDX   ,U                      ; 6A69: AE C4         \n");
                                      #endif
    PC = 0x6a69; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(U)<<8; 
    ea = ea | rd_mem((UINT16)((U)+1)); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$03FE                  ; 6A6B: 8C 03 FE      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0x03fe; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BLE   $6A77                   ; 6A6E: 2F 07         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) <  0) || (!Z)) {
    clockticks = clockticks + 3; 
    goto L6A77_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$03FE                  ; 6A70: 8E 03 FE      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x03fe; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STX   ,U                      ; 6A73: AF C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    Z = val; 
    N = (UINT8)(val >> 8); 
    V = 0; 
    ea = U; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         BRA   $6A81                   ; 6A75: 20 0A         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L6A81_;
    
    // ---------------------------------------------------------------------------
  case 0x6A77:
L6A77_:
                                                     #ifdef DEBUG
                                      mon("L6A77_   CMPX  #$FC02                  ; 6A77: 8C FC 02      \n");
                                      #endif
    PC = 0x6a77; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0xfc02; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BGE   $6A81                   ; 6A7A: 2C 05         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)(N^V) >= 0) {
    clockticks = clockticks + 3; 
    goto L6A81_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$FC02                  ; 6A7C: 8E FC 02      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0xfc02; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STX   ,U                      ; 6A7F: AF C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = U; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
  case 0x6A81:
L6A81_:
                                                     #ifdef DEBUG
                                      mon("L6A81_   LDX   $02,U                   ; 6A81: AE 42         \n");
                                      #endif
    PC = 0x6a81; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(U + 0x0002))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(U + 0x0002))+1)); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$04C0                  ; 6A83: 8C 04 C0      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0x04c0; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BLE   $6A8F                   ; 6A86: 2F 07         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) <  0) || (!Z)) {
    clockticks = clockticks + 3; 
    goto L6A8F_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$04C0                  ; 6A88: 8E 04 C0      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x04c0; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STX   $02,U                   ; 6A8B: AF 42         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = X; 
    Z = val; 
    N = (UINT8)(val >> 8); 
    V = 0; 
    ea = (U + 0x0002); 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         BRA   $6A99                   ; 6A8D: 20 0A         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L6A99_;
    
    // ---------------------------------------------------------------------------
  case 0x6A8F:
L6A8F_:
                                                     #ifdef DEBUG
                                      mon("L6A8F_   CMPX  #$FC04                  ; 6A8F: 8C FC 04      \n");
                                      #endif
    PC = 0x6a8f; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0xfc04; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BGE   $6A99                   ; 6A92: 2C 05         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)(N^V) >= 0) {
    clockticks = clockticks + 3; 
    goto L6A99_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$FC04                  ; 6A94: 8E FC 04      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0xfc04; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STX   $02,U                   ; 6A97: AF 42         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = X; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = (U + 0x0002); 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
  case 0x6A99:
L6A99_:
                                                     #ifdef DEBUG
                                      mon("L6A99_   LEAU  $10,U                   ; 6A99: 33 C8 10      \n");
                                      #endif
    PC = 0x6a99; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    U = (U + (SINT8)0x10); 
                                                     #ifdef DEBUG
                                      mon("         DECA                          ; 6A9C: 4A            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    A = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BGT   $6A69                   ; 6A9D: 2E CA         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    goto L6A69_;
    }
    // End of function sub_6A0C
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 6A9F: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6AA0:
L6AA0_:
                                                     #ifdef DEBUG
                                      mon("L6AA0    LDX   #$7610                  ; 6AA0: 8E 76 10      \n");
                                      #endif
    PC = 0x6aa0; 
    INSTRUCTION_START
    clockticks = clockticks + 3; 
    X = 0x7610; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$DC                    ; 6AA3: D6 DC         \n");
                                      #endif // Get object selected
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0xdc); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ASLB                          ; 6AA5: 58            \n");
                                      #endif
    val = (UINT8)B; 
    res = val << 1; 
 // C = res & 0x100; 
    res = (UINT8)res; 
 // Z = res; 
 // N = res; 
 // V = val ^ res; 
    B = res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDU   B,X                     ; 6AA6: EE 85         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + (SINT8)B))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + (SINT8)B))+1)); 
    U = ea; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   ,U+                     ; 6AA8: E6 C0         \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = (UINT8)rd_mem(U); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
    U = U + 1; 
                                                     #ifdef DEBUG
                                      mon("         CMPB  #$01                    ; 6AAA: C1 01         \n");
                                      #endif // Check that start of function has illegal $01 instruction byte. Why??
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    arg = 0x01; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
                                                     #ifdef DEBUG
                                      mon("         BEQ   $6AAF                   ; 6AAC: 27 01         \n");
                                      #endif
    // temp hack to force a flush
    if (!Z) {
    clockticks = clockticks + 3; 
    goto L6AAF_;
    }
                                                     #ifdef DEBUG
                                      mon("         SWI                           ; 6AAE: 3F            \n");
                                      #endif
    clockticks = clockticks + 19; 
    --S;
    wr_mem(S, 0xaf); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x6a); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, U); 
    --S;
    wr_mem(S, U >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, Y); 
    --S;
    wr_mem(S, Y >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, (DP >> 8)); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, A); 
    clockticks = clockticks + 1; 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    clockticks = clockticks + 1; 
    PC = (rd_mem(0xfffa)<<8)|rd_mem(0xfffb); 
    JUMP;
    // End of function sub_6AA0
    // ---------------------------------------------------------------------------
  case 0x6AAF:
L6AAF_:
                                                     #ifdef DEBUG
                                      mon("L6AAF_   JMP   ,U                      ; 6AAF: 6E C4         \n");
                                      #endif
    PC = 0x6aaf; 
    INSTRUCTION_START
    clockticks = clockticks + 1; 
    clockticks = clockticks + 4; 
    PC = U; 
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6C1C:
L6C1C_:
                                                     #ifdef DEBUG
                                      mon("L6C1C    LDA   #$5E                    ; 6C1C: 86 5E         \n");
                                      #endif // Draws bunkers before exploded
    PC = 0x6c1c; 
    INSTRUCTION_START
    clockticks = clockticks + 2; 
    val = (UINT8)0x5e; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 6C1E: 1F 8B         \n");
                                      #endif // Change zero page to Math RAM
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 6C20: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$FF98                  ; 6C22: C3 FF 98      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0xff98; 
    arg = (A << 8) | B; 
    res = arg + val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6C25: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y                      ; 6C27: ED A4         \n");
                                      #endif // Y points to vector RAM
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 6C29: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6C2B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,Y                   ; 6C2D: ED 22         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$04                    ; 6C2F: DC 04         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x04); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x05); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,Y                   ; 6C31: ED 24         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$00                    ; 6C33: DC 00         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x00); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x01); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,Y                   ; 6C35: ED 26         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 6C37: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0E                    ; 6C39: 93 0E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0e)<<8; 
    ea = ea | rd_mem(DP|0x0f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6C3B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $08,Y                   ; 6C3D: ED 28         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0008); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 6C3F: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0C                    ; 6C41: 93 0C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0c)<<8; 
    ea = ea | rd_mem(DP|0x0d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6C43: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0A,Y                   ; 6C45: ED 2A         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 6C47: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 6C49: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6C4B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0C,Y                   ; 6C4D: ED 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 6C4F: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 6C51: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6C53: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0E,Y                   ; 6C55: ED 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EE                    ; 6C57: DC EE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xee); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xef); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 6C59: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6C5B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $10,Y                   ; 6C5D: ED A8 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x10); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EC                    ; 6C60: DC EC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xec); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xed); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 6C62: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6C64: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $12,Y                   ; 6C66: ED A8 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x12); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DE                    ; 6C69: DC DE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xde); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EE                    ; 6C6B: 93 EE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xee)<<8; 
    ea = ea | rd_mem(DP|0xef); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6C6D: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $14,Y                   ; 6C6F: ED A8 14      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x14); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DC                    ; 6C72: DC DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xdc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EC                    ; 6C74: 93 EC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xec)<<8; 
    ea = ea | rd_mem(DP|0xed); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6C76: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $16,Y                   ; 6C78: ED A8 16      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x16); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 6C7B: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DE                    ; 6C7D: 93 DE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xde)<<8; 
    ea = ea | rd_mem(DP|0xdf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6C7F: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,Y                   ; 6C81: ED A8 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 6C84: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DC                    ; 6C86: 93 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xdc)<<8; 
    ea = ea | rd_mem(DP|0xdd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6C88: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,Y                   ; 6C8A: ED A8 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 6C8D: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 6C8F: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6C91: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1C,Y                   ; 6C93: ED A8 1C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 6C96: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 6C98: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6C9A: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1E,Y                   ; 6C9C: ED A8 1E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$FE                    ; 6C9F: DC FE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xfe); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xff); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 6CA1: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6CA3: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $20,Y                   ; 6CA5: ED A8 20      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x20); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$FC                    ; 6CA8: DC FC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xfc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xfd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 6CAA: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6CAC: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $22,Y                   ; 6CAE: ED A8 22      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x22); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DE                    ; 6CB1: DC DE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xde); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$FE                    ; 6CB3: 93 FE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xfe)<<8; 
    ea = ea | rd_mem(DP|0xff); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6CB5: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $24,Y                   ; 6CB7: ED A8 24      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x24); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DC                    ; 6CBA: DC DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xdc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$FC                    ; 6CBC: 93 FC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xfc)<<8; 
    ea = ea | rd_mem(DP|0xfd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6CBE: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $26,Y                   ; 6CC0: ED A8 26      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x26); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EE                    ; 6CC3: DC EE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xee); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xef); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DE                    ; 6CC5: 93 DE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xde)<<8; 
    ea = ea | rd_mem(DP|0xdf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6CC7: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $28,Y                   ; 6CC9: ED A8 28      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x28); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EC                    ; 6CCC: DC EC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xec); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xed); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DC                    ; 6CCE: 93 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xdc)<<8; 
    ea = ea | rd_mem(DP|0xdd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6CD0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2A,Y                   ; 6CD2: ED A8 2A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$FE                    ; 6CD5: DC FE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xfe); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xff); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EE                    ; 6CD7: 93 EE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xee)<<8; 
    ea = ea | rd_mem(DP|0xef); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6CD9: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2C,Y                   ; 6CDB: ED A8 2C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$FC                    ; 6CDE: DC FC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xfc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xfd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EC                    ; 6CE0: 93 EC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xec)<<8; 
    ea = ea | rd_mem(DP|0xed); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6CE2: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2E,Y                   ; 6CE4: ED A8 2E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$7200                  ; 6CE7: CC 72 00      \n");
                                      #endif // Insert Vector SCAL 2,00 instruction
    clockticks = clockticks + 3; 
    A = 0x72; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $30,Y                   ; 6CEA: ED A8 30      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x30); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 6CED: CC 80 40      \n");
                                      #endif // Insert Vector CNTR instruction
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $32,Y                   ; 6CF0: ED A8 32      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x32); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAY  $34,Y                   ; 6CF3: 31 A8 34      \n");
                                      #endif // Increment vector RAM pointer by 52 bytes for bunker vector data
    clockticks = clockticks + 5; 
    Y = (Y + (SINT8)0x34); 
 // Z = Y; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$48                    ; 6CF6: 86 48         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x48; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 6CF8: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
    // End of function sub_6C1C
    // ---------------------------------------------------------------------------
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 6CFA: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6CFC:
L6CFC_:
                                                     #ifdef DEBUG
                                      mon("L6CFC    LDA   #$5E                    ; 6CFC: 86 5E         \n");
                                      #endif // Draws towers before top exploded
    PC = 0x6cfc; 
    INSTRUCTION_START
    clockticks = clockticks + 2; 
    val = (UINT8)0x5e; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 6CFE: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 6D00: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$FF98                  ; 6D02: C3 FF 98      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0xff98; 
    arg = (A << 8) | B; 
    res = arg + val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D05: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y                      ; 6D07: ED A4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 6D09: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D0B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,Y                   ; 6D0D: ED 22         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$04                    ; 6D0F: DC 04         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x04); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x05); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,Y                   ; 6D11: ED 24         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$02                    ; 6D13: DC 02         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x02); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x03); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,Y                   ; 6D15: ED 26         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 6D17: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0E                    ; 6D19: 93 0E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0e)<<8; 
    ea = ea | rd_mem(DP|0x0f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D1B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $08,Y                   ; 6D1D: ED 28         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0008); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 6D1F: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0C                    ; 6D21: 93 0C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0c)<<8; 
    ea = ea | rd_mem(DP|0x0d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D23: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0A,Y                   ; 6D25: ED 2A         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 6D27: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 6D29: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D2B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0C,Y                   ; 6D2D: ED 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 6D2F: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 6D31: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6D33: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0E,Y                   ; 6D35: ED 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$FE                    ; 6D37: DC FE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xfe); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xff); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 6D39: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D3B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $10,Y                   ; 6D3D: ED A8 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x10); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$FC                    ; 6D40: DC FC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xfc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xfd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 6D42: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6D44: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $12,Y                   ; 6D46: ED A8 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x12); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$CE                    ; 6D49: DC CE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xce); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xcf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$FE                    ; 6D4B: 93 FE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xfe)<<8; 
    ea = ea | rd_mem(DP|0xff); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D4D: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $14,Y                   ; 6D4F: ED A8 14      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x14); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$CC                    ; 6D52: DC CC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xcc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xcd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$FC                    ; 6D54: 93 FC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xfc)<<8; 
    ea = ea | rd_mem(DP|0xfd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6D56: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $16,Y                   ; 6D58: ED A8 16      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x16); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9E                    ; 6D5B: DC 9E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$CE                    ; 6D5D: 93 CE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xce)<<8; 
    ea = ea | rd_mem(DP|0xcf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D5F: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,Y                   ; 6D61: ED A8 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9C                    ; 6D64: DC 9C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$CC                    ; 6D66: 93 CC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xcc)<<8; 
    ea = ea | rd_mem(DP|0xcd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6D68: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,Y                   ; 6D6A: ED A8 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$00                    ; 6D6D: DC 00         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x00); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x01); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1C,Y                   ; 6D6F: ED A8 1C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6E                    ; 6D72: DC 6E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9E                    ; 6D74: 93 9E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9e)<<8; 
    ea = ea | rd_mem(DP|0x9f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D76: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1E,Y                   ; 6D78: ED A8 1E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6C                    ; 6D7B: DC 6C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9C                    ; 6D7D: 93 9C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9c)<<8; 
    ea = ea | rd_mem(DP|0x9d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6D7F: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $20,Y                   ; 6D81: ED A8 20      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x20); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 6D84: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6E                    ; 6D86: 93 6E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6e)<<8; 
    ea = ea | rd_mem(DP|0x6f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D88: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $22,Y                   ; 6D8A: ED A8 22      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x22); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 6D8D: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6C                    ; 6D8F: 93 6C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6c)<<8; 
    ea = ea | rd_mem(DP|0x6d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6D91: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $24,Y                   ; 6D93: ED A8 24      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x24); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 6D96: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 6D98: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6D9A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $26,Y                   ; 6D9C: ED A8 26      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x26); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 6D9F: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 6DA1: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6DA3: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $28,Y                   ; 6DA5: ED A8 28      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x28); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$02                    ; 6DA8: DC 02         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x02); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x03); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2A,Y                   ; 6DAA: ED A8 2A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AE                    ; 6DAD: DC AE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xae); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xaf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7E                    ; 6DAF: 93 7E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7e)<<8; 
    ea = ea | rd_mem(DP|0x7f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6DB1: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2C,Y                   ; 6DB3: ED A8 2C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AC                    ; 6DB6: DC AC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xac); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xad); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7C                    ; 6DB8: 93 7C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7c)<<8; 
    ea = ea | rd_mem(DP|0x7d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6DBA: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2E,Y                   ; 6DBC: ED A8 2E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DE                    ; 6DBF: DC DE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xde); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AE                    ; 6DC1: 93 AE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xae)<<8; 
    ea = ea | rd_mem(DP|0xaf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6DC3: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $30,Y                   ; 6DC5: ED A8 30      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x30); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DC                    ; 6DC8: DC DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xdc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AC                    ; 6DCA: 93 AC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xac)<<8; 
    ea = ea | rd_mem(DP|0xad); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6DCC: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $32,Y                   ; 6DCE: ED A8 32      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x32); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 6DD1: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DE                    ; 6DD3: 93 DE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xde)<<8; 
    ea = ea | rd_mem(DP|0xdf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6DD5: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $34,Y                   ; 6DD7: ED A8 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x34); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 6DDA: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DC                    ; 6DDC: 93 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xdc)<<8; 
    ea = ea | rd_mem(DP|0xdd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6DDE: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $36,Y                   ; 6DE0: ED A8 36      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x36); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 6DE3: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 6DE5: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6DE7: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $38,Y                   ; 6DE9: ED A8 38      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x38); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 6DEC: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 6DEE: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6DF0: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3A,Y                   ; 6DF2: ED A8 3A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EE                    ; 6DF5: DC EE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xee); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xef); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 6DF7: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6DF9: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3C,Y                   ; 6DFB: ED A8 3C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EC                    ; 6DFE: DC EC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xec); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xed); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 6E00: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6E02: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3E,Y                   ; 6E04: ED A8 3E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BE                    ; 6E07: DC BE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbe); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EE                    ; 6E09: 93 EE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xee)<<8; 
    ea = ea | rd_mem(DP|0xef); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E0B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $40,Y                   ; 6E0D: ED A8 40      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x40); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BC                    ; 6E10: DC BC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EC                    ; 6E12: 93 EC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xec)<<8; 
    ea = ea | rd_mem(DP|0xed); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6E14: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $42,Y                   ; 6E16: ED A8 42      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x42); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8E                    ; 6E19: DC 8E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BE                    ; 6E1B: 93 BE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbe)<<8; 
    ea = ea | rd_mem(DP|0xbf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E1D: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $44,Y                   ; 6E1F: ED A8 44      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x44); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8C                    ; 6E22: DC 8C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BC                    ; 6E24: 93 BC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbc)<<8; 
    ea = ea | rd_mem(DP|0xbd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6E26: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $46,Y                   ; 6E28: ED A8 46      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x46); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$00                    ; 6E2B: DC 00         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x00); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x01); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $48,Y                   ; 6E2D: ED A8 48      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x48); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 6E30: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8E                    ; 6E32: 93 8E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8e)<<8; 
    ea = ea | rd_mem(DP|0x8f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E34: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4A,Y                   ; 6E36: ED A8 4A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x4a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 6E39: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8C                    ; 6E3B: 93 8C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8c)<<8; 
    ea = ea | rd_mem(DP|0x8d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6E3D: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4C,Y                   ; 6E3F: ED A8 4C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x4c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 6E42: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 6E44: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E46: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4E,Y                   ; 6E48: ED A8 4E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x4e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 6E4B: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 6E4D: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6E4F: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $50,Y                   ; 6E51: ED A8 50      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x50); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 6E54: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 6E56: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E58: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $52,Y                   ; 6E5A: ED A8 52      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x52); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 6E5D: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 6E5F: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E61: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $54,Y                   ; 6E63: ED A8 54      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x54); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9E                    ; 6E66: DC 9E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7E                    ; 6E68: 93 7E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7e)<<8; 
    ea = ea | rd_mem(DP|0x7f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E6A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $56,Y                   ; 6E6C: ED A8 56      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x56); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9C                    ; 6E6F: DC 9C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7C                    ; 6E71: 93 7C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7c)<<8; 
    ea = ea | rd_mem(DP|0x7d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6E73: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $58,Y                   ; 6E75: ED A8 58      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x58); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 6E78: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9E                    ; 6E7A: 93 9E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9e)<<8; 
    ea = ea | rd_mem(DP|0x9f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E7C: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5A,Y                   ; 6E7E: ED A8 5A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x5a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 6E81: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9C                    ; 6E83: 93 9C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9c)<<8; 
    ea = ea | rd_mem(DP|0x9d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E85: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5C,Y                   ; 6E87: ED A8 5C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x5c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8E                    ; 6E8A: DC 8E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7E                    ; 6E8C: 93 7E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7e)<<8; 
    ea = ea | rd_mem(DP|0x7f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6E8E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5E,Y                   ; 6E90: ED A8 5E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x5e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8C                    ; 6E93: DC 8C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7C                    ; 6E95: 93 7C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7c)<<8; 
    ea = ea | rd_mem(DP|0x7d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6E97: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $60,Y                   ; 6E99: ED A8 60      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x60); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$7200                  ; 6E9C: CC 72 00      \n");
                                      #endif // Vector SCAL 2,0 instruction
    clockticks = clockticks + 3; 
    A = 0x72; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $62,Y                   ; 6E9F: ED A8 62      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x62); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 6EA2: CC 80 40      \n");
                                      #endif // Insert Vector CNTR instruction
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $64,Y                   ; 6EA5: ED A8 64      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x64); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAY  $66,Y                   ; 6EA8: 31 A8 66      \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + (SINT8)0x66); 
 // Z = Y; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$48                    ; 6EAB: 86 48         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x48; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 6EAD: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
    // End of function sub_6CFC
    // ---------------------------------------------------------------------------
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 6EAF: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6EB1:
L6EB1_:
                                                     #ifdef DEBUG
                                      mon("L6EB1    LDA   #$5E                    ; 6EB1: 86 5E         \n");
                                      #endif // Draws tower after top exploded
    PC = 0x6eb1; 
    INSTRUCTION_START
    clockticks = clockticks + 2; 
    val = (UINT8)0x5e; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 6EB3: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 6EB5: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$FF98                  ; 6EB7: C3 FF 98      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0xff98; 
    arg = (A << 8) | B; 
    res = arg + val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6EBA: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y                      ; 6EBC: ED A4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 6EBE: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6EC0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,Y                   ; 6EC2: ED 22         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$04                    ; 6EC4: DC 04         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x04); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x05); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,Y                   ; 6EC6: ED 24         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$02                    ; 6EC8: DC 02         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x02); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x03); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,Y                   ; 6ECA: ED 26         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 6ECC: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0E                    ; 6ECE: 93 0E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0e)<<8; 
    ea = ea | rd_mem(DP|0x0f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6ED0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $08,Y                   ; 6ED2: ED 28         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0008); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 6ED4: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0C                    ; 6ED6: 93 0C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0c)<<8; 
    ea = ea | rd_mem(DP|0x0d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6ED8: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0A,Y                   ; 6EDA: ED 2A         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 6EDC: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 6EDE: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6EE0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0C,Y                   ; 6EE2: ED 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 6EE4: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 6EE6: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6EE8: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0E,Y                   ; 6EEA: ED 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$FE                    ; 6EEC: DC FE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xfe); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xff); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 6EEE: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6EF0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $10,Y                   ; 6EF2: ED A8 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x10); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$FC                    ; 6EF5: DC FC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xfc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xfd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 6EF7: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6EF9: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $12,Y                   ; 6EFB: ED A8 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x12); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$CE                    ; 6EFE: DC CE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xce); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xcf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$FE                    ; 6F00: 93 FE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xfe)<<8; 
    ea = ea | rd_mem(DP|0xff); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6F02: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $14,Y                   ; 6F04: ED A8 14      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x14); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$CC                    ; 6F07: DC CC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xcc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xcd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$FC                    ; 6F09: 93 FC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xfc)<<8; 
    ea = ea | rd_mem(DP|0xfd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6F0B: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $16,Y                   ; 6F0D: ED A8 16      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x16); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9E                    ; 6F10: DC 9E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$CE                    ; 6F12: 93 CE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xce)<<8; 
    ea = ea | rd_mem(DP|0xcf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6F14: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,Y                   ; 6F16: ED A8 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9C                    ; 6F19: DC 9C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$CC                    ; 6F1B: 93 CC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xcc)<<8; 
    ea = ea | rd_mem(DP|0xcd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6F1D: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,Y                   ; 6F1F: ED A8 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 6F22: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9E                    ; 6F24: 93 9E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9e)<<8; 
    ea = ea | rd_mem(DP|0x9f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6F26: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1C,Y                   ; 6F28: ED A8 1C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 6F2B: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9C                    ; 6F2D: 93 9C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9c)<<8; 
    ea = ea | rd_mem(DP|0x9d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6F2F: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1E,Y                   ; 6F31: ED A8 1E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AE                    ; 6F34: DC AE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xae); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xaf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7E                    ; 6F36: 93 7E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7e)<<8; 
    ea = ea | rd_mem(DP|0x7f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6F38: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $20,Y                   ; 6F3A: ED A8 20      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x20); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AC                    ; 6F3D: DC AC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xac); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xad); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7C                    ; 6F3F: 93 7C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7c)<<8; 
    ea = ea | rd_mem(DP|0x7d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6F41: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $22,Y                   ; 6F43: ED A8 22      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x22); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DE                    ; 6F46: DC DE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xde); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AE                    ; 6F48: 93 AE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xae)<<8; 
    ea = ea | rd_mem(DP|0xaf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6F4A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $24,Y                   ; 6F4C: ED A8 24      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x24); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DC                    ; 6F4F: DC DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xdc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AC                    ; 6F51: 93 AC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xac)<<8; 
    ea = ea | rd_mem(DP|0xad); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6F53: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $26,Y                   ; 6F55: ED A8 26      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x26); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 6F58: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DE                    ; 6F5A: 93 DE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xde)<<8; 
    ea = ea | rd_mem(DP|0xdf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6F5C: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $28,Y                   ; 6F5E: ED A8 28      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x28); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 6F61: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DC                    ; 6F63: 93 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xdc)<<8; 
    ea = ea | rd_mem(DP|0xdd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6F65: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2A,Y                   ; 6F67: ED A8 2A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 6F6A: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 6F6C: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6F6E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2C,Y                   ; 6F70: ED A8 2C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 6F73: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 6F75: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6F77: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2E,Y                   ; 6F79: ED A8 2E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EE                    ; 6F7C: DC EE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xee); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xef); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 6F7E: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6F80: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $30,Y                   ; 6F82: ED A8 30      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x30); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EC                    ; 6F85: DC EC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xec); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xed); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 6F87: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6F89: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $32,Y                   ; 6F8B: ED A8 32      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x32); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BE                    ; 6F8E: DC BE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbe); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EE                    ; 6F90: 93 EE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xee)<<8; 
    ea = ea | rd_mem(DP|0xef); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6F92: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $34,Y                   ; 6F94: ED A8 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x34); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BC                    ; 6F97: DC BC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EC                    ; 6F99: 93 EC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xec)<<8; 
    ea = ea | rd_mem(DP|0xed); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6F9B: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $36,Y                   ; 6F9D: ED A8 36      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x36); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8E                    ; 6FA0: DC 8E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BE                    ; 6FA2: 93 BE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbe)<<8; 
    ea = ea | rd_mem(DP|0xbf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6FA4: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $38,Y                   ; 6FA6: ED A8 38      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x38); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8C                    ; 6FA9: DC 8C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BC                    ; 6FAB: 93 BC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbc)<<8; 
    ea = ea | rd_mem(DP|0xbd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6FAD: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3A,Y                   ; 6FAF: ED A8 3A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 6FB2: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8E                    ; 6FB4: 93 8E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8e)<<8; 
    ea = ea | rd_mem(DP|0x8f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6FB6: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3C,Y                   ; 6FB8: ED A8 3C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 6FBB: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8C                    ; 6FBD: 93 8C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8c)<<8; 
    ea = ea | rd_mem(DP|0x8d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6FBF: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3E,Y                   ; 6FC1: ED A8 3E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$7200                  ; 6FC4: CC 72 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x72; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $40,Y                   ; 6FC7: ED A8 40      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x40); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 6FCA: CC 80 40      \n");
                                      #endif // Insert Vector CNTR instruction
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $42,Y                   ; 6FCD: ED A8 42      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x42); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAY  $44,Y                   ; 6FD0: 31 A8 44      \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + (SINT8)0x44); 
 // Z = Y; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$48                    ; 6FD3: 86 48         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x48; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 6FD5: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
    // End of function sub_6EB1
    // ---------------------------------------------------------------------------
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 6FD7: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x6FD9:
L6FD9_:
                                                     #ifdef DEBUG
                                      mon("L6FD9    LDA   #$5E                    ; 6FD9: 86 5E         \n");
                                      #endif // Draws trench turrets
    PC = 0x6fd9; 
    INSTRUCTION_START
    clockticks = clockticks + 2; 
    val = (UINT8)0x5e; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 6FDB: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 6FDD: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$FF98                  ; 6FDF: C3 FF 98      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0xff98; 
    arg = (A << 8) | B; 
    res = arg + val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6FE2: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y                      ; 6FE4: ED A4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 6FE6: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6FE8: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,Y                   ; 6FEA: ED 22         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$6480                  ; 6FEC: CC 64 80      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x64; 
    B = 0x80; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,Y                   ; 6FEF: ED 24         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 6FF1: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 6FF3: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 6FF5: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,Y                   ; 6FF7: ED 26         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 6FF9: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 6FFB: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 6FFD: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $08,Y                   ; 6FFF: ED 28         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0008); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 7001: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 7003: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7005: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0A,Y                   ; 7007: ED 2A         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 7009: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 700B: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 700D: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0C,Y                   ; 700F: ED 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 7011: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 7013: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7015: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0E,Y                   ; 7017: ED 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 7019: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 701B: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 701D: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $10,Y                   ; 701F: ED A8 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x10); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 7022: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 7024: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7026: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $12,Y                   ; 7028: ED A8 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x12); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 702B: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 702D: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 702F: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $14,Y                   ; 7031: ED A8 14      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x14); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9E                    ; 7034: DC 9E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 7036: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7038: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $16,Y                   ; 703A: ED A8 16      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x16); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9C                    ; 703D: DC 9C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 703F: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7041: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,Y                   ; 7043: ED A8 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AE                    ; 7046: DC AE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xae); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xaf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9E                    ; 7048: 93 9E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9e)<<8; 
    ea = ea | rd_mem(DP|0x9f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 704A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,Y                   ; 704C: ED A8 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AC                    ; 704F: DC AC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xac); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xad); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9C                    ; 7051: 93 9C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9c)<<8; 
    ea = ea | rd_mem(DP|0x9d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7053: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1C,Y                   ; 7055: ED A8 1C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 7058: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AE                    ; 705A: 93 AE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xae)<<8; 
    ea = ea | rd_mem(DP|0xaf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 705C: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1E,Y                   ; 705E: ED A8 1E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 7061: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AC                    ; 7063: 93 AC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xac)<<8; 
    ea = ea | rd_mem(DP|0xad); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7065: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $20,Y                   ; 7067: ED A8 20      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x20); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AE                    ; 706A: DC AE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xae); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xaf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 706C: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 706E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $22,Y                   ; 7070: ED A8 22      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x22); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AC                    ; 7073: DC AC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xac); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xad); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 7075: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7077: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $24,Y                   ; 7079: ED A8 24      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x24); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6E                    ; 707C: DC 6E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AE                    ; 707E: 93 AE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xae)<<8; 
    ea = ea | rd_mem(DP|0xaf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7080: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $26,Y                   ; 7082: ED A8 26      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x26); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6C                    ; 7085: DC 6C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AC                    ; 7087: 93 AC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xac)<<8; 
    ea = ea | rd_mem(DP|0xad); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7089: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $28,Y                   ; 708B: ED A8 28      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x28); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 708E: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6E                    ; 7090: 93 6E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6e)<<8; 
    ea = ea | rd_mem(DP|0x6f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7092: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2A,Y                   ; 7094: ED A8 2A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 7097: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6C                    ; 7099: 93 6C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6c)<<8; 
    ea = ea | rd_mem(DP|0x6d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 709B: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2C,Y                   ; 709D: ED A8 2C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 70A0: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 70A2: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 70A4: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2E,Y                   ; 70A6: ED A8 2E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 70A9: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 70AB: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 70AD: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $30,Y                   ; 70AF: ED A8 30      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x30); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 70B2: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 70B4: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 70B6: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $32,Y                   ; 70B8: ED A8 32      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x32); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 70BB: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 70BD: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 70BF: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $34,Y                   ; 70C1: ED A8 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x34); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6E                    ; 70C4: DC 6E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7E                    ; 70C6: 93 7E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7e)<<8; 
    ea = ea | rd_mem(DP|0x7f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 70C8: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $36,Y                   ; 70CA: ED A8 36      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x36); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6C                    ; 70CD: DC 6C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7C                    ; 70CF: 93 7C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7c)<<8; 
    ea = ea | rd_mem(DP|0x7d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 70D1: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $38,Y                   ; 70D3: ED A8 38      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x38); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 70D6: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6E                    ; 70D8: 93 6E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6e)<<8; 
    ea = ea | rd_mem(DP|0x6f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 70DA: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3A,Y                   ; 70DC: ED A8 3A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 70DF: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6C                    ; 70E1: 93 6C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6c)<<8; 
    ea = ea | rd_mem(DP|0x6d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 70E3: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3C,Y                   ; 70E5: ED A8 3C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8E                    ; 70E8: DC 8E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 70EA: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 70EC: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3E,Y                   ; 70EE: ED A8 3E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8C                    ; 70F1: DC 8C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 70F3: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 70F5: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $40,Y                   ; 70F7: ED A8 40      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x40); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 70FA: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8E                    ; 70FC: 93 8E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8e)<<8; 
    ea = ea | rd_mem(DP|0x8f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 70FE: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $42,Y                   ; 7100: ED A8 42      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x42); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 7103: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8C                    ; 7105: 93 8C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8c)<<8; 
    ea = ea | rd_mem(DP|0x8d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7107: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $44,Y                   ; 7109: ED A8 44      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x44); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9E                    ; 710C: DC 9E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7E                    ; 710E: 93 7E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7e)<<8; 
    ea = ea | rd_mem(DP|0x7f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7110: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $46,Y                   ; 7112: ED A8 46      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x46); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9C                    ; 7115: DC 9C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7C                    ; 7117: 93 7C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7c)<<8; 
    ea = ea | rd_mem(DP|0x7d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7119: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $48,Y                   ; 711B: ED A8 48      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x48); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8E                    ; 711E: DC 8E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9E                    ; 7120: 93 9E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9e)<<8; 
    ea = ea | rd_mem(DP|0x9f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7122: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4A,Y                   ; 7124: ED A8 4A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x4a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8C                    ; 7127: DC 8C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9C                    ; 7129: 93 9C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9c)<<8; 
    ea = ea | rd_mem(DP|0x9d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 712B: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4C,Y                   ; 712D: ED A8 4C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x4c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DE                    ; 7130: DC DE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xde); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8E                    ; 7132: 93 8E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8e)<<8; 
    ea = ea | rd_mem(DP|0x8f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7134: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4E,Y                   ; 7136: ED A8 4E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x4e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DC                    ; 7139: DC DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xdc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8C                    ; 713B: 93 8C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8c)<<8; 
    ea = ea | rd_mem(DP|0x8d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 713D: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $50,Y                   ; 713F: ED A8 50      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x50); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EE                    ; 7142: DC EE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xee); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xef); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DE                    ; 7144: 93 DE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xde)<<8; 
    ea = ea | rd_mem(DP|0xdf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7146: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $52,Y                   ; 7148: ED A8 52      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x52); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EC                    ; 714B: DC EC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xec); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xed); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$DC                    ; 714D: 93 DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xdc)<<8; 
    ea = ea | rd_mem(DP|0xdd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 714F: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $54,Y                   ; 7151: ED A8 54      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x54); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9E                    ; 7154: DC 9E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EE                    ; 7156: 93 EE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xee)<<8; 
    ea = ea | rd_mem(DP|0xef); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7158: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $56,Y                   ; 715A: ED A8 56      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x56); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9C                    ; 715D: DC 9C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EC                    ; 715F: 93 EC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xec)<<8; 
    ea = ea | rd_mem(DP|0xed); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7161: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $58,Y                   ; 7163: ED A8 58      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x58); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EE                    ; 7166: DC EE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xee); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xef); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9E                    ; 7168: 93 9E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9e)<<8; 
    ea = ea | rd_mem(DP|0x9f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 716A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5A,Y                   ; 716C: ED A8 5A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x5a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$EC                    ; 716F: DC EC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xec); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xed); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9C                    ; 7171: 93 9C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9c)<<8; 
    ea = ea | rd_mem(DP|0x9d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7173: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5C,Y                   ; 7175: ED A8 5C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x5c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$CE                    ; 7178: DC CE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xce); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xcf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EE                    ; 717A: 93 EE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xee)<<8; 
    ea = ea | rd_mem(DP|0xef); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 717C: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5E,Y                   ; 717E: ED A8 5E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x5e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$CC                    ; 7181: DC CC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xcc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xcd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$EC                    ; 7183: 93 EC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xec)<<8; 
    ea = ea | rd_mem(DP|0xed); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7185: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $60,Y                   ; 7187: ED A8 60      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x60); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AE                    ; 718A: DC AE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xae); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xaf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$CE                    ; 718C: 93 CE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xce)<<8; 
    ea = ea | rd_mem(DP|0xcf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 718E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $62,Y                   ; 7190: ED A8 62      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x62); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AC                    ; 7193: DC AC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xac); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xad); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$CC                    ; 7195: 93 CC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xcc)<<8; 
    ea = ea | rd_mem(DP|0xcd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7197: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $64,Y                   ; 7199: ED A8 64      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x64); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 719C: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AE                    ; 719E: 93 AE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xae)<<8; 
    ea = ea | rd_mem(DP|0xaf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 71A0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $66,Y                   ; 71A2: ED A8 66      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x66); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 71A5: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AC                    ; 71A7: 93 AC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xac)<<8; 
    ea = ea | rd_mem(DP|0xad); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 71A9: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $68,Y                   ; 71AB: ED A8 68      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x68); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BE                    ; 71AE: DC BE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbe); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 71B0: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 71B2: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $6A,Y                   ; 71B4: ED A8 6A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x6a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BC                    ; 71B7: DC BC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 71B9: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 71BB: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $6C,Y                   ; 71BD: ED A8 6C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x6c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$CE                    ; 71C0: DC CE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xce); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xcf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BE                    ; 71C2: 93 BE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbe)<<8; 
    ea = ea | rd_mem(DP|0xbf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 71C4: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $6E,Y                   ; 71C6: ED A8 6E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x6e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$CC                    ; 71C9: DC CC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xcc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xcd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BC                    ; 71CB: 93 BC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbc)<<8; 
    ea = ea | rd_mem(DP|0xbd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 71CD: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $70,Y                   ; 71CF: ED A8 70      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x70); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BE                    ; 71D2: DC BE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbe); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$CE                    ; 71D4: 93 CE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xce)<<8; 
    ea = ea | rd_mem(DP|0xcf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 71D6: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $72,Y                   ; 71D8: ED A8 72      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x72); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BC                    ; 71DB: DC BC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$CC                    ; 71DD: 93 CC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xcc)<<8; 
    ea = ea | rd_mem(DP|0xcd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 71DF: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $74,Y                   ; 71E1: ED A8 74      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x74); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DE                    ; 71E4: DC DE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xde); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BE                    ; 71E6: 93 BE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbe)<<8; 
    ea = ea | rd_mem(DP|0xbf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 71E8: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $76,Y                   ; 71EA: ED A8 76      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x76); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$DC                    ; 71ED: DC DC         \n");
                                      #endif // Selected object model
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xdc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xdd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BC                    ; 71EF: 93 BC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbc)<<8; 
    ea = ea | rd_mem(DP|0xbd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 71F1: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $78,Y                   ; 71F3: ED A8 78      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x78); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$7200                  ; 71F6: CC 72 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x72; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $7A,Y                   ; 71F9: ED A8 7A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x7a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 71FC: CC 80 40      \n");
                                      #endif // Insert Vector CNTR instruction
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $7C,Y                   ; 71FF: ED A8 7C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x7c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAY  $7E,Y                   ; 7202: 31 A8 7E      \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + (SINT8)0x7e); 
 // Z = Y; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$48                    ; 7205: 86 48         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x48; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 7207: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
    // End of function sub_6FD9
    // ---------------------------------------------------------------------------
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 7209: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x720B:
L720B_:
                                                     #ifdef DEBUG
                                      mon("L720B    LDA   #$5E                    ; 720B: 86 5E         \n");
                                      #endif // Draws trench green squares
    PC = 0x720b; 
    INSTRUCTION_START
    clockticks = clockticks + 2; 
    val = (UINT8)0x5e; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 720D: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 720F: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$FF98                  ; 7211: C3 FF 98      \n");
                                      #endif // Y axis offset for squares
    clockticks = clockticks + 4; 
    val = 0xff98; 
    arg = (A << 8) | B; 
    res = arg + val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7214: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y                      ; 7216: ED A4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 7218: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 721A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,Y                   ; 721C: ED 22         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$6280                  ; 721E: CC 62 80      \n");
                                      #endif // Insert Vector STAT 2,80 instruction (Green)
    clockticks = clockticks + 3; 
    A = 0x62; 
    B = 0x80; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,Y                   ; 7221: ED 24         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 7223: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0E                    ; 7225: 93 0E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0e)<<8; 
    ea = ea | rd_mem(DP|0x0f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7227: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,Y                   ; 7229: ED 26         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 722B: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0C                    ; 722D: 93 0C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0c)<<8; 
    ea = ea | rd_mem(DP|0x0d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 722F: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $08,Y                   ; 7231: ED 28         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0008); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 7233: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 7235: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7237: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0A,Y                   ; 7239: ED 2A         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 723B: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 723D: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 723F: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0C,Y                   ; 7241: ED 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 7243: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 7245: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7247: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0E,Y                   ; 7249: ED 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 724B: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 724D: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 724F: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $10,Y                   ; 7251: ED A8 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x10); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 7254: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 7256: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7258: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $12,Y                   ; 725A: ED A8 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x12); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 725D: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 725F: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7261: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $14,Y                   ; 7263: ED A8 14      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x14); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 7266: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0E                    ; 7268: 93 0E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0e)<<8; 
    ea = ea | rd_mem(DP|0x0f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 726A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $16,Y                   ; 726C: ED A8 16      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x16); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 726F: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0C                    ; 7271: 93 0C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0c)<<8; 
    ea = ea | rd_mem(DP|0x0d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7273: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,Y                   ; 7275: ED A8 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 7278: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 727A: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 727C: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,Y                   ; 727E: ED A8 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 7281: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 7283: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7285: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1C,Y                   ; 7287: ED A8 1C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6E                    ; 728A: DC 6E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 728C: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 728E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1E,Y                   ; 7290: ED A8 1E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6C                    ; 7293: DC 6C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 7295: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7297: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $20,Y                   ; 7299: ED A8 20      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x20); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 729C: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6E                    ; 729E: 93 6E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6e)<<8; 
    ea = ea | rd_mem(DP|0x6f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 72A0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $22,Y                   ; 72A2: ED A8 22      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x22); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 72A5: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6C                    ; 72A7: 93 6C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6c)<<8; 
    ea = ea | rd_mem(DP|0x6d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 72A9: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $24,Y                   ; 72AB: ED A8 24      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x24); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 72AE: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7E                    ; 72B0: 93 7E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7e)<<8; 
    ea = ea | rd_mem(DP|0x7f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 72B2: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $26,Y                   ; 72B4: ED A8 26      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x26); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 72B7: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7C                    ; 72B9: 93 7C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7c)<<8; 
    ea = ea | rd_mem(DP|0x7d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 72BB: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $28,Y                   ; 72BD: ED A8 28      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x28); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$7200                  ; 72C0: CC 72 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x72; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2A,Y                   ; 72C3: ED A8 2A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 72C6: CC 80 40      \n");
                                      #endif // Insert Vector CNTR instruction
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2C,Y                   ; 72C9: ED A8 2C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAY  $2E,Y                   ; 72CC: 31 A8 2E      \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + (SINT8)0x2e); 
 // Z = Y; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$48                    ; 72CF: 86 48         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x48; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 72D1: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
    // End of function sub_720B
    // ---------------------------------------------------------------------------
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 72D3: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x72D5:
L72D5_:
                                                     #ifdef DEBUG
                                      mon("L72D5    LDA   #$5E                    ; 72D5: 86 5E         \n");
                                      #endif // Draws trench catwalks
    PC = 0x72d5; 
    INSTRUCTION_START
    clockticks = clockticks + 2; 
    val = (UINT8)0x5e; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 72D7: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 72D9: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$FF98                  ; 72DB: C3 FF 98      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0xff98; 
    arg = (A << 8) | B; 
    res = arg + val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 72DE: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y                      ; 72E0: ED A4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 72E2: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 72E4: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,Y                   ; 72E6: ED 22         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 72E8: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 72EA: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 72EC: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,Y                   ; 72EE: ED 24         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 72F0: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 72F2: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 72F4: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,Y                   ; 72F6: ED 26         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 72F8: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0E                    ; 72FA: 93 0E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0e)<<8; 
    ea = ea | rd_mem(DP|0x0f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 72FC: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $08,Y                   ; 72FE: ED 28         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0008); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 7300: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0C                    ; 7302: 93 0C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0c)<<8; 
    ea = ea | rd_mem(DP|0x0d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7304: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0A,Y                   ; 7306: ED 2A         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 7308: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 730A: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 730C: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0C,Y                   ; 730E: ED 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 7310: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 7312: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7314: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0E,Y                   ; 7316: ED 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 7318: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 731A: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 731C: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $10,Y                   ; 731E: ED A8 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x10); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 7321: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 7323: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7325: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $12,Y                   ; 7327: ED A8 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x12); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 732A: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 732C: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 732E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $14,Y                   ; 7330: ED A8 14      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x14); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 7333: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 7335: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7337: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $16,Y                   ; 7339: ED A8 16      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x16); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 733C: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 733E: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7340: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,Y                   ; 7342: ED A8 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 7345: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 7347: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7349: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,Y                   ; 734B: ED A8 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 734E: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 7350: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7352: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1C,Y                   ; 7354: ED A8 1C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 7357: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 7359: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 735B: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1E,Y                   ; 735D: ED A8 1E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$7200                  ; 7360: CC 72 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x72; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $20,Y                   ; 7363: ED A8 20      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x20); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 7366: CC 80 40      \n");
                                      #endif // Insert Vector CNTR instruction
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $22,Y                   ; 7369: ED A8 22      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x22); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAY  $24,Y                   ; 736C: 31 A8 24      \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + (SINT8)0x24); 
 // Z = Y; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$48                    ; 736F: 86 48         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x48; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 7371: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
    // End of function sub_72D5
    // ---------------------------------------------------------------------------
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 7373: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x7375:
L7375_:
                                                     #ifdef DEBUG
                                      mon("L7375    LDA   #$5E                    ; 7375: 86 5E         \n");
                                      #endif // Tower top explode: Copy math points to vector RAM?
    PC = 0x7375; 
    INSTRUCTION_START
    clockticks = clockticks + 2; 
    val = (UINT8)0x5e; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 7377: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 7379: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$FF98                  ; 737B: C3 FF 98      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0xff98; 
    arg = (A << 8) | B; 
    res = arg + val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 737E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y                      ; 7380: ED A4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 7382: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7384: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,Y                   ; 7386: ED 22         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$A018                  ; 7388: CC A0 18      \n");
                                      #endif // Insert Vector JSRL 0018 instruction (STAT for colour fade?)
    clockticks = clockticks + 3; 
    A = 0xa0; 
    B = 0x18; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,Y                   ; 738B: ED 24         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 738D: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 738F: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7391: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,Y                   ; 7393: ED 26         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 7395: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 7397: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7399: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $08,Y                   ; 739B: ED 28         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0008); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 739D: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0E                    ; 739F: 93 0E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0e)<<8; 
    ea = ea | rd_mem(DP|0x0f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 73A1: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0A,Y                   ; 73A3: ED 2A         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 73A5: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0C                    ; 73A7: 93 0C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0c)<<8; 
    ea = ea | rd_mem(DP|0x0d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 73A9: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0C,Y                   ; 73AB: ED 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 73AD: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 73AF: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 73B1: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0E,Y                   ; 73B3: ED 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 73B5: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 73B7: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 73B9: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $10,Y                   ; 73BB: ED A8 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x10); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 73BE: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 73C0: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 73C2: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $12,Y                   ; 73C4: ED A8 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x12); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 73C7: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 73C9: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 73CB: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $14,Y                   ; 73CD: ED A8 14      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x14); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 73D0: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 73D2: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 73D4: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $16,Y                   ; 73D6: ED A8 16      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x16); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 73D9: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 73DB: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 73DD: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,Y                   ; 73DF: ED A8 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6E                    ; 73E2: DC 6E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 73E4: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 73E6: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,Y                   ; 73E8: ED A8 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6C                    ; 73EB: DC 6C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 73ED: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 73EF: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1C,Y                   ; 73F1: ED A8 1C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 73F4: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6E                    ; 73F6: 93 6E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6e)<<8; 
    ea = ea | rd_mem(DP|0x6f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 73F8: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1E,Y                   ; 73FA: ED A8 1E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 73FD: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6C                    ; 73FF: 93 6C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6c)<<8; 
    ea = ea | rd_mem(DP|0x6d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7401: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $20,Y                   ; 7403: ED A8 20      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x20); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 7406: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 7408: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 740A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $22,Y                   ; 740C: ED A8 22      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x22); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 740F: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 7411: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7413: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $24,Y                   ; 7415: ED A8 24      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x24); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 7418: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 741A: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 741C: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $26,Y                   ; 741E: ED A8 26      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x26); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 7421: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 7423: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7425: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $28,Y                   ; 7427: ED A8 28      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x28); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 742A: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 742C: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 742E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2A,Y                   ; 7430: ED A8 2A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 7433: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 7435: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7437: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2C,Y                   ; 7439: ED A8 2C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$7200                  ; 743C: CC 72 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x72; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2E,Y                   ; 743F: ED A8 2E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 7442: CC 80 40      \n");
                                      #endif // Insert Vector CNTR instruction
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $30,Y                   ; 7445: ED A8 30      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x30); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAY  $32,Y                   ; 7448: 31 A8 32      \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + (SINT8)0x32); 
 // Z = Y; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$48                    ; 744B: 86 48         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x48; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 744D: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
    // End of function sub_7375
    // ---------------------------------------------------------------------------
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 744F: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x7451:
L7451_:
                                                     #ifdef DEBUG
                                      mon("L7451    LDA   #$5E                    ; 7451: 86 5E         \n");
                                      #endif // Draws trench exhaust port
    PC = 0x7451; 
    INSTRUCTION_START
    clockticks = clockticks + 2; 
    val = (UINT8)0x5e; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 7453: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 7455: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$FF98                  ; 7457: C3 FF 98      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0xff98; 
    arg = (A << 8) | B; 
    res = arg + val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 745A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y                      ; 745C: ED A4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 745E: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7460: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,Y                   ; 7462: ED 22         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$6280                  ; 7464: CC 62 80      \n");
                                      #endif // Insert Vector STAT 2,80 instruction (Green)
    clockticks = clockticks + 3; 
    A = 0x62; 
    B = 0x80; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,Y                   ; 7467: ED 24         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9E                    ; 7469: DC 9E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 746B: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 746D: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,Y                   ; 746F: ED 26         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$9C                    ; 7471: DC 9C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x9c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x9d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 7473: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7475: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $08,Y                   ; 7477: ED 28         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x0008); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8E                    ; 7479: DC 8E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9E                    ; 747B: 93 9E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9e)<<8; 
    ea = ea | rd_mem(DP|0x9f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 747D: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0A,Y                   ; 747F: ED 2A         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$8C                    ; 7481: DC 8C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x8c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$9C                    ; 7483: 93 9C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x9c)<<8; 
    ea = ea | rd_mem(DP|0x9d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7485: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0C,Y                   ; 7487: ED 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 7489: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8E                    ; 748B: 93 8E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8e)<<8; 
    ea = ea | rd_mem(DP|0x8f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 748D: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $0E,Y                   ; 748F: ED 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + 0x000e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 7491: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$8C                    ; 7493: 93 8C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x8c)<<8; 
    ea = ea | rd_mem(DP|0x8d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7495: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $10,Y                   ; 7497: ED A8 10      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x10); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6E                    ; 749A: DC 6E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 749C: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 749E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $12,Y                   ; 74A0: ED A8 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x12); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6C                    ; 74A3: DC 6C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 74A5: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 74A7: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $14,Y                   ; 74A9: ED A8 14      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x14); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AE                    ; 74AC: DC AE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xae); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xaf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6E                    ; 74AE: 93 6E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6e)<<8; 
    ea = ea | rd_mem(DP|0x6f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 74B0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $16,Y                   ; 74B2: ED A8 16      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x16); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$AC                    ; 74B5: DC AC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xac); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xad); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6C                    ; 74B7: 93 6C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6c)<<8; 
    ea = ea | rd_mem(DP|0x6d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 74B9: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $18,Y                   ; 74BB: ED A8 18      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x18); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BE                    ; 74BE: DC BE         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbe); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbf); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AE                    ; 74C0: 93 AE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xae)<<8; 
    ea = ea | rd_mem(DP|0xaf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 74C2: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1A,Y                   ; 74C4: ED A8 1A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$BC                    ; 74C7: DC BC         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0xbc); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0xbd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$AC                    ; 74C9: 93 AC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xac)<<8; 
    ea = ea | rd_mem(DP|0xad); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 74CB: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1C,Y                   ; 74CD: ED A8 1C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 74D0: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BE                    ; 74D2: 93 BE         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbe)<<8; 
    ea = ea | rd_mem(DP|0xbf); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 74D4: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $1E,Y                   ; 74D6: ED A8 1E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x1e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 74D9: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$BC                    ; 74DB: 93 BC         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0xbc)<<8; 
    ea = ea | rd_mem(DP|0xbd); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 74DD: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $20,Y                   ; 74DF: ED A8 20      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x20); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$6380                  ; 74E2: CC 63 80      \n");
                                      #endif // Insert Vector STAT 3,80 instruction (Red)
    clockticks = clockticks + 3; 
    A = 0x63; 
    B = 0x80; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $22,Y                   ; 74E5: ED A8 22      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x22); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6E                    ; 74E8: DC 6E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7E                    ; 74EA: 93 7E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7e)<<8; 
    ea = ea | rd_mem(DP|0x7f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 74EC: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $24,Y                   ; 74EE: ED A8 24      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x24); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6C                    ; 74F1: DC 6C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7C                    ; 74F3: 93 7C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7c)<<8; 
    ea = ea | rd_mem(DP|0x7d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 74F5: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $26,Y                   ; 74F7: ED A8 26      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x26); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 74FA: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6E                    ; 74FC: 93 6E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6e)<<8; 
    ea = ea | rd_mem(DP|0x6f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 74FE: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $28,Y                   ; 7500: ED A8 28      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x28); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 7503: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6C                    ; 7505: 93 6C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6c)<<8; 
    ea = ea | rd_mem(DP|0x6d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7507: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2A,Y                   ; 7509: ED A8 2A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6E                    ; 750C: DC 6E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 750E: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7510: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2C,Y                   ; 7512: ED A8 2C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$6C                    ; 7515: DC 6C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x6c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x6d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 7517: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7519: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $2E,Y                   ; 751B: ED A8 2E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x2e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 751E: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6E                    ; 7520: 93 6E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6e)<<8; 
    ea = ea | rd_mem(DP|0x6f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7522: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $30,Y                   ; 7524: ED A8 30      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x30); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 7527: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$6C                    ; 7529: 93 6C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x6c)<<8; 
    ea = ea | rd_mem(DP|0x6d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 752B: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $32,Y                   ; 752D: ED A8 32      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x32); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 7530: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 7532: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7534: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $34,Y                   ; 7536: ED A8 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x34); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 7539: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 753B: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 753D: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $36,Y                   ; 753F: ED A8 36      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x36); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4E                    ; 7542: DC 4E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0E                    ; 7544: 93 0E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0e)<<8; 
    ea = ea | rd_mem(DP|0x0f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7546: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $38,Y                   ; 7548: ED A8 38      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x38); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$4C                    ; 754B: DC 4C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x4c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x4d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0C                    ; 754D: 93 0C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0c)<<8; 
    ea = ea | rd_mem(DP|0x0d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 754F: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3A,Y                   ; 7551: ED A8 3A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 7554: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4E                    ; 7556: 93 4E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4e)<<8; 
    ea = ea | rd_mem(DP|0x4f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7558: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3C,Y                   ; 755A: ED A8 3C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 755D: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$4C                    ; 755F: 93 4C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x4c)<<8; 
    ea = ea | rd_mem(DP|0x4d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7561: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $3E,Y                   ; 7563: ED A8 3E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x3e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 7566: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 7568: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 756A: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $40,Y                   ; 756C: ED A8 40      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x40); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 756F: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 7571: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7573: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $42,Y                   ; 7575: ED A8 42      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x42); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5E                    ; 7578: DC 5E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 757A: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 757C: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $44,Y                   ; 757E: ED A8 44      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x44); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$5C                    ; 7581: DC 5C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x5c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x5d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 7583: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7585: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $46,Y                   ; 7587: ED A8 46      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x46); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7E                    ; 758A: DC 7E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5E                    ; 758C: 93 5E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5e)<<8; 
    ea = ea | rd_mem(DP|0x5f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 758E: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $48,Y                   ; 7590: ED A8 48      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x48); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$7C                    ; 7593: DC 7C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x7c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x7d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$5C                    ; 7595: 93 5C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x5c)<<8; 
    ea = ea | rd_mem(DP|0x5d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 7597: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4A,Y                   ; 7599: ED A8 4A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x4a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 759C: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7E                    ; 759E: 93 7E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7e)<<8; 
    ea = ea | rd_mem(DP|0x7f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 75A0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4C,Y                   ; 75A2: ED A8 4C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x4c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 75A5: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$7C                    ; 75A7: 93 7C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x7c)<<8; 
    ea = ea | rd_mem(DP|0x7d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 75A9: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4E,Y                   ; 75AB: ED A8 4E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x4e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$64FF                  ; 75AE: CC 64 FF      \n");
                                      #endif // Insert Vector STAT 4,FF instruction ()
    clockticks = clockticks + 3; 
    A = 0x64; 
    B = 0xff; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $50,Y                   ; 75B1: ED A8 50      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x50); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2E                    ; 75B4: DC 2E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3E                    ; 75B6: 93 3E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3e)<<8; 
    ea = ea | rd_mem(DP|0x3f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 75B8: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $52,Y                   ; 75BA: ED A8 52      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x52); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$2C                    ; 75BD: DC 2C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x2c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x2d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$3C                    ; 75BF: 93 3C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x3c)<<8; 
    ea = ea | rd_mem(DP|0x3d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 75C1: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $54,Y                   ; 75C3: ED A8 54      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x54); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0E                    ; 75C6: DC 0E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2E                    ; 75C8: 93 2E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2e)<<8; 
    ea = ea | rd_mem(DP|0x2f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 75CA: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $56,Y                   ; 75CC: ED A8 56      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x56); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$0C                    ; 75CF: DC 0C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x0c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x0d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$2C                    ; 75D1: 93 2C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x2c)<<8; 
    ea = ea | rd_mem(DP|0x2d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 75D3: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $58,Y                   ; 75D5: ED A8 58      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x58); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1E                    ; 75D8: DC 1E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0E                    ; 75DA: 93 0E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0e)<<8; 
    ea = ea | rd_mem(DP|0x0f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 75DC: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5A,Y                   ; 75DE: ED A8 5A      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x5a); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$1C                    ; 75E1: DC 1C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x1c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x1d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$0C                    ; 75E3: 93 0C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x0c)<<8; 
    ea = ea | rd_mem(DP|0x0d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 75E5: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5C,Y                   ; 75E7: ED A8 5C      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x5c); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3E                    ; 75EA: DC 3E         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3e); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3f); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1E                    ; 75EC: 93 1E         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1e)<<8; 
    ea = ea | rd_mem(DP|0x1f); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 75EE: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5E,Y                   ; 75F0: ED A8 5E      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x5e); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   <$3C                    ; 75F3: DC 3C         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x3c); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x3d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$1C                    ; 75F5: 93 1C         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x1c)<<8; 
    ea = ea | rd_mem(DP|0x1d); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$E0                    ; 75F7: 8A E0         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0xe0; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $60,Y                   ; 75F9: ED A8 60      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x60); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$7200                  ; 75FC: CC 72 00      \n");
                                      #endif // Insert Vector SCAL 2,00 instruction
    clockticks = clockticks + 3; 
    A = 0x72; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $62,Y                   ; 75FF: ED A8 62      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x62); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 7602: CC 80 40      \n");
                                      #endif // Insert Vector CNTR instruction
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $64,Y                   ; 7605: ED A8 64      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(Y + (SINT8)0x64); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAY  $66,Y                   ; 7608: 31 A8 66      \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + (SINT8)0x66); 
 // Z = Y; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$48                    ; 760B: 86 48         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x48; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 760D: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 6; 
    // End of function sub_7451
    // ---------------------------------------------------------------------------
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 760F: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x7D9A:
L7D9A_:
                                                     #ifdef DEBUG
                                      mon("L7D9A    LDD   <$89                    ; 7D9A: DC 89         \n");
                                      #endif // Get X 'forward' transform position
    PC = 0x7d9a; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x89); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x8a); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5040                   ; 7D9C: FD 50 40      \n");
                                      #endif // XT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5040, A); // Care needed with I/O space and word fetches
    wr_mem(0x5041, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $4B24                   ; 7D9F: FC 4B 24      \n");
                                      #endif // Get Y 'horizontal' transform
    clockticks = clockticks + 5; 
    A = rd_mem(0x4b24); // Care needed with I/O space and word fetches
    B = rd_mem(0x4b25); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5042                   ; 7DA2: FD 50 42      \n");
                                      #endif // YT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5042, A); // Care needed with I/O space and word fetches
    wr_mem(0x5043, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $4B26                   ; 7DA5: FC 4B 26      \n");
                                      #endif // Get Z 'vertical' transform
    clockticks = clockticks + 5; 
    A = rd_mem(0x4b26); // Care needed with I/O space and word fetches
    B = rd_mem(0x4b27); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5044                   ; 7DA8: FD 50 44      \n");
                                      #endif // ZT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5044, A); // Care needed with I/O space and word fetches
    wr_mem(0x5045, B); 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$4C00                  ; 7DAB: 8E 4C 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x4c00; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 7DAE: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
  case 0x7DB1:
L7DB1_:
                                                     #ifdef DEBUG
                                      mon("L7DB1_   STD   ,X++                    ; 7DB1: ED 81         \n");
                                      #endif
    PC = 0x7db1; 
    INSTRUCTION_START
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = X; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    X = X + 2; 
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$4C80                  ; 7DB3: 8C 4C 80      \n");
                                      #endif // Clear star dots RAM
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0x4c80; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BCS   $7DB1                   ; 7DB6: 25 F9         \n");
                                      #endif
    // temp hack to force a flush
    if (C) {
    clockticks = clockticks + 3; 
    goto L7DB1_;
    }
                                                     #ifdef DEBUG
                                      mon("         STA   <$88                    ; 7DB8: 97 88         \n");
                                      #endif // Clear DPbyte_88
    clockticks = clockticks + 4; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x88; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$67                    ; 7DBA: 86 67         \n");
                                      #endif // Insert Vector STAT 7 for white stars
    clockticks = clockticks + 2; 
    val = (UINT8)0x67; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$83                    ; 7DBC: D6 83         \n");
                                      #endif // Star intensity
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x83); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7DBE: ED A1         \n");
                                      #endif // Y points to vector RAM
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$018C                  ; 7DC0: CC 01 8C      \n");
                                      #endif // Point BIC to Math RAM $5C60
    clockticks = clockticks + 3; 
    A = 0x01; 
    B = 0x8c; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4701                   ; 7DC3: FD 47 01      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4701, A); // Care needed with I/O space and word fetches
    wr_mem(0x4702, B); 
                                                     #ifdef DEBUG
                                      mon("         STB   <$82                    ; 7DC6: D7 82         \n");
                                      #endif // Store $8C to DPbyte_82
    clockticks = clockticks + 4; 
    val = B; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x82; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         NEGB                          ; 7DC8: 50            \n");
                                      #endif // Negate to $74
    val = (UINT8)B; 
    res = -val; 
 // V = val & (UINT8)res; 
 // C = res & 0x100; 
    val = (UINT8)res; 
    B = val; 
 // Z = val; 
 // N = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ADDB  #$BE                    ; 7DC9: CB BE         \n");
                                      #endif // Add $BE to get $32
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    arg = 0xbe; 
    res = val + (UINT8)arg; 
    C = (res >> 1) & 0x80; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
    H = (UINT8)(val ^ arg ^ res ^ C); 
 // V = (UINT8)(val ^ arg ^ res ^ C); 
    B = (UINT8)res; 
    if (gameRunning)
    {
      if (countOfStarsIngame != 0) B=countOfStarsIngame;    
    }
    else
    {
      if (countOfStars != 0) B=countOfStars;    
    }
    
    
                                                     #ifdef DEBUG
                                      mon("         STB   <$81                    ; 7DCB: D7 81         \n");
                                      #endif // Store $32 in DPbyte_81 (why not just load B with $32 ?)
    clockticks = clockticks + 4; 
    val = B; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x81; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDU   #$4C80                  ; 7DCD: CE 4C 80      \n");
                                      #endif
    clockticks = clockticks + 3; 
    U = 0x4c80; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$5C60                  ; 7DD0: 8E 5C 60      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x5c60; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
  case 0x7DD3:
L7DD3_:
                                                     #ifdef DEBUG
                                      mon("L7DD3_   STX   <$84                    ; 7DD3: 9F 84         \n");
                                      #endif // DPbyte_84 holds running star MRAM pointer
    PC = 0x7dd3; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = X; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = DP|0x84; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$67                    ; 7DD5: 86 67         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x67; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 7DD7: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0xda); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x7d); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x7DDA:
L7DDA_:
                                                     #ifdef DEBUG
                                      mon("L7DDA_   LDD   $5000                   ; 7DDA: FC 50 00      \n");
                                      #endif // Math result X
    PC = 0x7dda; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5000); // Care needed with I/O space and word fetches
    B = rd_mem(0x5001); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$0100                  ; 7DDD: 10 83 01 00   \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x0100; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         LBLE  $7E5D                   ; 7DE1: 10 2F 00 78   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (((SINT8)(N^V) <  0) || (!Z)) {
    clockticks = clockticks + 1; 
    goto L7E5D_;
    }
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$0FFF                  ; 7DE5: 10 83 0F FF   \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x0fff; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         LBHI  $7E5D                   ; 7DE9: 10 22 00 70   \n");
                                      #endif // Check perspective division value?
    clockticks = clockticks + 5; 
    if (Z && (!C)) {
    clockticks = clockticks + 1; 
    goto L7E5D_;
    }
                                                     #ifdef DEBUG
                                      mon("         STD   $4704                   ; 7DED: FD 47 04      \n");
                                      #endif // Start division
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4704, A); // Care needed with I/O space and word fetches
    wr_mem(0x4705, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5072                   ; 7DF0: FC 50 72      \n");
                                      #endif // Reg39 = Reg01 x Reg01
    clockticks = clockticks + 5; 
    A = rd_mem(0x5072); // Care needed with I/O space and word fetches
    B = rd_mem(0x5073); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5070                   ; 7DF3: B3 50 70      \n");
                                      #endif // Reg38 = Reg00 x Reg00
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5070)<<8; 
    ea = ea | rd_mem(0x5071); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
    N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         LBCC  $7E5D                   ; 7DF6: 10 24 00 63   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (!C) {
    clockticks = clockticks + 1; 
    goto L7E5D_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   $5074                   ; 7DFA: FC 50 74      \n");
                                      #endif // Reg3A = Reg02 x Reg02
    clockticks = clockticks + 5; 
    A = rd_mem(0x5074); // Care needed with I/O space and word fetches
    B = rd_mem(0x5075); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5070                   ; 7DFD: B3 50 70      \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5070)<<8; 
    ea = ea | rd_mem(0x5071); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
    N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         LBCC  $7E5D                   ; 7E00: 10 24 00 59   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (!C) {
    clockticks = clockticks + 1; 
    goto L7E5D_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   $4700                   ; 7E04: FC 47 00      \n");
                                      #endif // Get division result
    clockticks = clockticks + 5; 
    A = rd_mem(0x4700); // Care needed with I/O space and word fetches
    B = rd_mem(0x4701); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5000                   ; 7E07: FD 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5000, A); // Care needed with I/O space and word fetches
    wr_mem(0x5001, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$86                    ; 7E0A: 86 86         \n");
                                      #endif // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
    clockticks = clockticks + 2; 
    val = (UINT8)0x86; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 7E0C: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x0f); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x7e); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x7E0F:
L7E0F_:
                                                     #ifdef DEBUG
                                      mon("L7E0F_   CLRB                          ; 7E0F: 5F            \n");
                                      #endif
    PC = 0x7e0f; 
    INSTRUCTION_START
    B = 0; // SEARCH_ME
 // V = 0; 
 // Z = 0; 
 // N = 0; 
 // C = 0; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDA   $5004                   ; 7E10: B6 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x5004); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 7E13: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 7E14: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
 // C = res & 0x100; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
 // V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 7E15: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 7E16: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
 // C = res & 0x100; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
 // V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDA   $5002                   ; 7E17: B6 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x5002); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 7E1A: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 7E1B: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
 // C = res & 0x100; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
 // V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 7E1C: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 7E1D: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
 // C = res & 0x100; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
 // V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$08                    ; 7E1E: 86 08         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x08; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         MUL                           ; 7E20: 3D            \n");
                                      #endif
    res = (UINT16)(A * B); 
 // Z = res; 
    C = res & 0x80; 
    A = res >> 8; 
    B = (UINT8)res; 
    clockticks = clockticks + 11; 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$4C00                  ; 7E21: 8E 4C 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x4c00; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ABX                           ; 7E24: 3A            \n");
                                      #endif
    X = X + B; 
    clockticks = clockticks + 3; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $02,X                   ; 7E25: EC 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
    ea = (UINT16)(X + 0x0002); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
    Z = A|B; 
    N = A; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BNE   L7E2D                   ; 7E27: 26 04         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    clockticks = clockticks + 3; 
    goto L7E2D_;
    }
                                                     #ifdef DEBUG
                                      mon("         STU   $02,X                   ; 7E29: EF 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = U; 
    Z = val; 
    N = (UINT8)(val >> 8); 
    V = 0; 
    ea = (X + 0x0002); 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         BRA   $7E2F                   ; 7E2B: 20 02         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L7E2F_;
    
    // ---------------------------------------------------------------------------
  case 0x7E2D:
L7E2D_:
                                                     #ifdef DEBUG
                                      mon("L7E2D    STU   [,X]                    ; 7E2D: EF 94         \n");
                                      #endif
    PC = 0x7e2d; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = U; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = ea; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
  case 0x7E2F:
L7E2F_:
                                                     #ifdef DEBUG
                                      mon("L7E2F_   STU   ,X                      ; 7E2F: EF 84         \n");
                                      #endif
    PC = 0x7e2f; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = U; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = X; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 7E31: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,U                      ; 7E34: ED C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = U; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 7E36: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $06,X                   ; 7E39: A3 06         \n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + 0x0006))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + 0x0006))+1)); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7E3B: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,U                   ; 7E3D: ED 44         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(U + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 7E3F: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $04,X                   ; 7E42: A3 04         \n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + 0x0004))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + 0x0004))+1)); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7E44: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPU  $02,X                   ; 7E46: 11 A3 02      \n");
                                      #endif
    clockticks = clockticks + 2; 
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + 0x0002))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + 0x0002))+1)); 
    val = U; 
    arg = ea; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BEQ   $7E4D                   ; 7E49: 27 02         \n");
                                      #endif // These 3 lines surely do nothing as ORA #0 does nothing ?
    // temp hack to force a flush
    if (!Z) {
    clockticks = clockticks + 3; 
    goto L7E4D_;
    }
                                                     #ifdef DEBUG
                                      mon("         ORA   #$00                    ; 7E4B: 8A 00         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0x00; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
  case 0x7E4D:
L7E4D_:
                                                     #ifdef DEBUG
                                      mon("L7E4D_   STD   $02,U                   ; 7E4D: ED 42         \n");
                                      #endif
    PC = 0x7e4d; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(U + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 7E4F: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,X                   ; 7E52: ED 06         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 7E54: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,X                   ; 7E57: ED 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    Z = A | B; 
    N = A; 
    V = 0; 
    ea = (UINT16)(X + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAU  $06,U                   ; 7E59: 33 46         \n");
                                      #endif
    clockticks = clockticks + 5; 
    U = (U + 0x0006); 
                                                     #ifdef DEBUG
                                      mon("         BRA   $7E62                   ; 7E5B: 20 05         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L7E62_;
    
    // ---------------------------------------------------------------------------
  case 0x7E5D:
L7E5D_:
                                                     #ifdef DEBUG
                                      mon("L7E5D_   LDX   <$84                    ; 7E5D: 9E 84         \n");
                                      #endif
    PC = 0x7e5d; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x84)<<8; 
    ea = ea | rd_mem(DP|0x85); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   L7F9A                   ; 7E5F: BD 7F 9A      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x62); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x7e); 
    clockticks = clockticks + 1; 
    goto L7F9A_;
  case 0x7E62:
L7E62_:
                                                     #ifdef DEBUG
                                      mon("L7E62_   INC   <$82                    ; 7E62: 0C 82         \n");
                                      #endif
    PC = 0x7e62; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x82); 
    res = (UINT8)(val + 1); 
 // Z = res; 
 // N = res; 
 // V = res & ~val; 
    wr_mem(DP|0x82, res); 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDX   <$84                    ; 7E64: 9E 84         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x84)<<8; 
    ea = ea | rd_mem(DP|0x85); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LEAX  $08,X                   ; 7E66: 30 08         \n");
                                      #endif
    clockticks = clockticks + 5; 
    X = (X + 0x0008); 
 // Z = X; 
                                                     #ifdef DEBUG
                                      mon("         DEC   <$81                    ; 7E68: 0A 81         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x81); 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    wr_mem(DP|0x81, res); 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LBNE  $7DD3                   ; 7E6A: 10 26 FF 65   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (Z) {
    clockticks = clockticks + 1; 
    goto L7DD3_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$4C00                  ; 7E6E: 8E 4C 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x4c00; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
  case 0x7E71:
L7E71_:
                                                     #ifdef DEBUG
                                      mon("L7E71_   LDU   $02,X                   ; 7E71: EE 02         \n");
                                      #endif
    PC = 0x7e71; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + 0x0002))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + 0x0002))+1)); 
    U = ea; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BEQ   $7E95                   ; 7E73: 27 20         \n");
                                      #endif
    // temp hack to force a flush
    if (!Z) {
    clockticks = clockticks + 3; 
    goto L7E95_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   #$1F98                  ; 7E75: CC 1F 98      \n");
                                      #endif // Starfield Y offset
    clockticks = clockticks + 3; 
    A = 0x1f; 
    B = 0x98; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7E78: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 7E7A: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7E7D: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
  case 0x7E7F:
L7E7F_:
                                                     #ifdef DEBUG
                                      mon("L7E7F_   LDD   $04,U                   ; 7E7F: EC 44         \n");
                                      #endif
    PC = 0x7e7f; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    ea = (UINT16)(U + 0x0004); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7E81: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $02,U                   ; 7E83: EC 42         \n");
                                      #endif
    clockticks = clockticks + 5; 
    ea = (UINT16)(U + 0x0002); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7E85: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $33DC                   ; 7E87: FC 33 DC      \n");
                                      #endif // JSRL to Vector ROM subroutine to draw dot
    clockticks = clockticks + 5; 
    A = rd_mem(0x33dc); // Care needed with I/O space and word fetches
    B = rd_mem(0x33dd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7E8A: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDU   ,U                      ; 7E8C: EE C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(U)<<8; 
    ea = ea | rd_mem((UINT16)((U)+1)); 
    U = ea; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BNE   $7E7F                   ; 7E8E: 26 EF         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    clockticks = clockticks + 3; 
    goto L7E7F_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 7E90: CC 80 40      \n");
                                      #endif // Insert Vector CNTR
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7E93: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
  case 0x7E95:
L7E95_:
                                                     #ifdef DEBUG
                                      mon("L7E95_   LEAX  $08,X                   ; 7E95: 30 08         \n");
                                      #endif
    PC = 0x7e95; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    X = (X + 0x0008); 
 // Z = X; 
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$4C80                  ; 7E97: 8C 4C 80      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0x4c80; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BCS   $7E71                   ; 7E9A: 25 D5         \n");
                                      #endif
    // temp hack to force a flush
    if (C) {
    clockticks = clockticks + 3; 
    goto L7E71_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   $5098                   ; 7E9C: FC 50 98      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x5098); // Care needed with I/O space and word fetches
    B = rd_mem(0x5099); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5040                   ; 7E9F: FD 50 40      \n");
                                      #endif // XT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5040, A); // Care needed with I/O space and word fetches
    wr_mem(0x5041, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $509A                   ; 7EA2: FC 50 9A      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x509a); // Care needed with I/O space and word fetches
    B = rd_mem(0x509b); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5042                   ; 7EA5: FD 50 42      \n");
                                      #endif // YT2
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5042, A); // Care needed with I/O space and word fetches
    wr_mem(0x5043, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $509C                   ; 7EA8: FC 50 9C      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x509c); // Care needed with I/O space and word fetches
    B = rd_mem(0x509d); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5044                   ; 7EAB: FD 50 44      \n");
                                      #endif // ZT2
    clockticks = clockticks + 5; 
    Z = A | B; 
    N = A; 
    V = 0; 
    wr_mem(0x5044, A); // Care needed with I/O space and word fetches
    wr_mem(0x5045, B); 
    // End of function sub_7D9A
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 7EAE: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x7EAF:
L7EAF_:
                                                     #ifdef DEBUG
                                      mon("L7EAF    LDX   #$4C00                  ; 7EAF: 8E 4C 00      \n");
                                      #endif // Towers surface dots
    PC = 0x7eaf; 
    INSTRUCTION_START
    clockticks = clockticks + 3; 
    X = 0x4c00; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 7EB2: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
  case 0x7EB5:
L7EB5_:
                                                     #ifdef DEBUG
                                      mon("L7EB5_   STD   ,X++                    ; 7EB5: ED 81         \n");
                                      #endif
    PC = 0x7eb5; 
    INSTRUCTION_START
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = X; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    X = X + 2; 
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$4C80                  ; 7EB7: 8C 4C 80      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0x4c80; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BCS   $7EB5                   ; 7EBA: 25 F9         \n");
                                      #endif
    // temp hack to force a flush
    if (C) {
    clockticks = clockticks + 3; 
    goto L7EB5_;
    }
                                                     #ifdef DEBUG
                                      mon("         STA   <$88                    ; 7EBC: 97 88         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x88; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$6280                  ; 7EBE: CC 62 80      \n");
                                      #endif // Insert Vector STAT 2, 80 for green dots
    clockticks = clockticks + 3; 
    A = 0x62; 
    B = 0x80; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7EC1: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$018C                  ; 7EC3: CC 01 8C      \n");
                                      #endif // Point BIC to Math RAM $5C60
    clockticks = clockticks + 3; 
    A = 0x01; 
    B = 0x8c; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $4701                   ; 7EC6: FD 47 01      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4701, A); // Care needed with I/O space and word fetches
    wr_mem(0x4702, B); 
                                                     #ifdef DEBUG
                                      mon("         STB   <$82                    ; 7EC9: D7 82         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = B; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x82; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         NEGB                          ; 7ECB: 50            \n");
                                      #endif
    val = (UINT8)B; 
    res = -val; 
 // V = val & (UINT8)res; 
 // C = res & 0x100; 
    val = (UINT8)res; 
    B = val; 
 // Z = val; 
 // N = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ADDB  #$BE                    ; 7ECC: CB BE         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    arg = 0xbe; 
    res = val + (UINT8)arg; 
    C = (res >> 1) & 0x80; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
    H = (UINT8)(val ^ arg ^ res ^ C); 
 // V = (UINT8)(val ^ arg ^ res ^ C); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STB   <$81                    ; 7ECE: D7 81         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = B; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    ea = DP|0x81; 
    wr_mem(ea, val); 
                                                     #ifdef DEBUG
                                      mon("         LDU   #$4C80                  ; 7ED0: CE 4C 80      \n");
                                      #endif
    clockticks = clockticks + 3; 
    U = 0x4c80; 
 // Z = U; 
 // N = (U) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$5C60                  ; 7ED3: 8E 5C 60      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x5c60; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
  case 0x7ED6:
L7ED6_:
                                                     #ifdef DEBUG
                                      mon("L7ED6_   STX   <$84                    ; 7ED6: 9F 84         \n");
                                      #endif
    PC = 0x7ed6; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = X; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = DP|0x84; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$67                    ; 7ED8: 86 67         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x67; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 7EDA: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0xdd); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x7e); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x7EDD:
L7EDD_:
                                                     #ifdef DEBUG
                                      mon("L7EDD_   LDD   $5000                   ; 7EDD: FC 50 00      \n");
                                      #endif // Math result X
    PC = 0x7edd; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5000); // Care needed with I/O space and word fetches
    B = rd_mem(0x5001); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$0100                  ; 7EE0: 10 83 01 00   \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x0100; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         LBLE  L7F58                   ; 7EE4: 10 2F 00 70   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (((SINT8)(N^V) <  0) || (!Z)) {
    clockticks = clockticks + 1; 
    goto L7F58_;
    }
                                                     #ifdef DEBUG
                                      mon("         STD   $4704                   ; 7EE8: FD 47 04      \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x4704, A); // Care needed with I/O space and word fetches
    wr_mem(0x4705, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5072                   ; 7EEB: FC 50 72      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x5072); // Care needed with I/O space and word fetches
    B = rd_mem(0x5073); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5070                   ; 7EEE: B3 50 70      \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5070)<<8; 
    ea = ea | rd_mem(0x5071); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
    N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         LBCC  L7F58                   ; 7EF1: 10 24 00 63   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (!C) {
    clockticks = clockticks + 1; 
    goto L7F58_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   $5074                   ; 7EF5: FC 50 74      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x5074); // Care needed with I/O space and word fetches
    B = rd_mem(0x5075); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $5070                   ; 7EF8: B3 50 70      \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5070)<<8; 
    ea = ea | rd_mem(0x5071); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
    N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         LBCC  L7F58                   ; 7EFB: 10 24 00 59   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (!C) {
    clockticks = clockticks + 1; 
    goto L7F58_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   $4700                   ; 7EFF: FC 47 00      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x4700); // Care needed with I/O space and word fetches
    B = rd_mem(0x4701); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $5000                   ; 7F02: FD 50 00      \n");
                                      #endif // Math result X
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    wr_mem(0x5000, A); // Care needed with I/O space and word fetches
    wr_mem(0x5001, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$86                    ; 7F05: 86 86         \n");
                                      #endif // Reg01 = Reg01 x Reg00, Reg02 = Reg02 x Reg00
    clockticks = clockticks + 2; 
    val = (UINT8)0x86; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 7F07: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x0a); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x7f); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x7F0A:
L7F0A_:
                                                     #ifdef DEBUG
                                      mon("L7F0A_   CLRB                          ; 7F0A: 5F            \n");
                                      #endif
    PC = 0x7f0a; 
    INSTRUCTION_START
    B = 0; // SEARCH_ME
 // V = 0; 
 // Z = 0; 
 // N = 0; 
 // C = 0; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDA   $5004                   ; 7F0B: B6 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x5004); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 7F0E: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 7F0F: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
 // C = res & 0x100; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
 // V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 7F10: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 7F11: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
 // C = res & 0x100; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
 // V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDA   $5002                   ; 7F12: B6 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x5002); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 7F15: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 7F16: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
 // C = res & 0x100; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
 // V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 7F17: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
 // N = 0; 
 // Z = val; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 7F18: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
 // C = res & 0x100; 
 // Z = (UINT8)res; 
 // N = (UINT8)res; 
 // V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$08                    ; 7F19: 86 08         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x08; 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         MUL                           ; 7F1B: 3D            \n");
                                      #endif
    res = (UINT16)(A * B); 
 // Z = res; 
    C = res & 0x80; 
    A = res >> 8; 
    B = (UINT8)res; 
    clockticks = clockticks + 11; 
                                                     #ifdef DEBUG
                                      mon("         LDX   #$4C00                  ; 7F1C: 8E 4C 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x4c00; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ABX                           ; 7F1F: 3A            \n");
                                      #endif
    X = X + B; 
    clockticks = clockticks + 3; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $02,X                   ; 7F20: EC 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
    ea = (UINT16)(X + 0x0002); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
    Z = A|B; 
    N = A; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BNE   $7F28                   ; 7F22: 26 04         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    clockticks = clockticks + 3; 
    goto L7F28_;
    }
                                                     #ifdef DEBUG
                                      mon("         STU   $02,X                   ; 7F24: EF 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = U; 
    Z = val; 
    N = (UINT8)(val >> 8); 
    V = 0; 
    ea = (X + 0x0002); 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         BRA   $7F2A                   ; 7F26: 20 02         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L7F2A_;
    
    // ---------------------------------------------------------------------------
  case 0x7F28:
L7F28_:
                                                     #ifdef DEBUG
                                      mon("L7F28_   STU   [,X]                    ; 7F28: EF 94         \n");
                                      #endif
    PC = 0x7f28; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = U; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = ea; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
  case 0x7F2A:
L7F2A_:
                                                     #ifdef DEBUG
                                      mon("L7F2A_   STU   ,X                      ; 7F2A: EF 84         \n");
                                      #endif
    PC = 0x7f2a; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = U; 
 // Z = val; 
 // N = (UINT8)(val >> 8); 
 // V = 0; 
    ea = X; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 7F2C: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,U                      ; 7F2F: ED C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = U; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 7F31: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $06,X                   ; 7F34: A3 06         \n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + 0x0006))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + 0x0006))+1)); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7F36: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,U                   ; 7F38: ED 44         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(U + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 7F3A: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         SUBD  $04,X                   ; 7F3D: A3 04         \n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + 0x0004))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + 0x0004))+1)); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
 // C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
 // N = res >> 8; 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7F3F: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         CMPU  $02,X                   ; 7F41: 11 A3 02      \n");
                                      #endif
    clockticks = clockticks + 2; 
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + 0x0002))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + 0x0002))+1)); 
    val = U; 
    arg = ea; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BEQ   $7F48                   ; 7F44: 27 02         \n");
                                      #endif
    // temp hack to force a flush
    if (!Z) {
    clockticks = clockticks + 3; 
    goto L7F48_;
    }
                                                     #ifdef DEBUG
                                      mon("         ORA   #$00                    ; 7F46: 8A 00         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0x00; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
  case 0x7F48:
L7F48_:
                                                     #ifdef DEBUG
                                      mon("L7F48_   STD   $02,U                   ; 7F48: ED 42         \n");
                                      #endif
    PC = 0x7f48; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(U + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 7F4A: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $06,X                   ; 7F4D: ED 06         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + 0x0006); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 7F4F: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,X                   ; 7F52: ED 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    Z = A | B; 
    N = A; 
    V = 0; 
    ea = (UINT16)(X + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LEAU  $06,U                   ; 7F54: 33 46         \n");
                                      #endif
    clockticks = clockticks + 5; 
    U = (U + 0x0006); 
                                                     #ifdef DEBUG
                                      mon("         BRA   $7F5D                   ; 7F56: 20 05         \n");
                                      #endif
    clockticks = clockticks + 3; 
    goto L7F5D_;
    
    // ---------------------------------------------------------------------------
  case 0x7F58:
L7F58_:
                                                     #ifdef DEBUG
                                      mon("L7F58    LDX   <$84                    ; 7F58: 9E 84         \n");
                                      #endif // Towers surface dots
    PC = 0x7f58; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x84)<<8; 
    ea = ea | rd_mem(DP|0x85); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   L7FDB                   ; 7F5A: BD 7F DB      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x5d); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x7f); 
    clockticks = clockticks + 1; 
    goto L7FDB_;
  case 0x7F5D:
L7F5D_:
                                                     #ifdef DEBUG
                                      mon("L7F5D_   INC   <$82                    ; 7F5D: 0C 82         \n");
                                      #endif
    PC = 0x7f5d; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x82); 
    res = (UINT8)(val + 1); 
 // Z = res; 
 // N = res; 
 // V = res & ~val; 
    wr_mem(DP|0x82, res); 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDX   <$84                    ; 7F5F: 9E 84         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x84)<<8; 
    ea = ea | rd_mem(DP|0x85); 
    X = ea; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LEAX  $08,X                   ; 7F61: 30 08         \n");
                                      #endif
    clockticks = clockticks + 5; 
    X = (X + 0x0008); 
 // Z = X; 
                                                     #ifdef DEBUG
                                      mon("         DEC   <$81                    ; 7F63: 0A 81         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x81); 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    wr_mem(DP|0x81, res); 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         LBNE  $7ED6                   ; 7F65: 10 26 FF 6D   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (Z) {
    clockticks = clockticks + 1; 
    goto L7ED6_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDX   #$4C00                  ; 7F69: 8E 4C 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x4c00; 
 // Z = X; 
 // N = (X) >> 8; 
 // V = 0; 
  case 0x7F6C:
L7F6C_:
                                                     #ifdef DEBUG
                                      mon("L7F6C_   LDU   $02,X                   ; 7F6C: EE 02         \n");
                                      #endif
    PC = 0x7f6c; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem((UINT16)(X + 0x0002))<<8; 
    ea = ea | rd_mem((UINT16)(((UINT16)(X + 0x0002))+1)); 
    U = ea; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BEQ   $7F92                   ; 7F6E: 27 22         \n");
                                      #endif
    // temp hack to force a flush
    if (!Z) {
    clockticks = clockticks + 3; 
    goto L7F92_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   #$FF98                  ; 7F70: CC FF 98      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0xff; 
    B = 0x98; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7F73: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7F75: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 7F77: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7F7A: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
  case 0x7F7C:
L7F7C_:
                                                     #ifdef DEBUG
                                      mon("L7F7C_   LDD   $04,U                   ; 7F7C: EC 44         \n");
                                      #endif
    PC = 0x7f7c; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    ea = (UINT16)(U + 0x0004); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7F7E: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $02,U                   ; 7F80: EC 42         \n");
                                      #endif
    clockticks = clockticks + 5; 
    ea = (UINT16)(U + 0x0002); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7F82: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDD   $33DC                   ; 7F84: FC 33 DC      \n");
                                      #endif // JSRL to Vector ROM subroutine to draw dot
    clockticks = clockticks + 5; 
    A = rd_mem(0x33dc); // Care needed with I/O space and word fetches
    B = rd_mem(0x33dd); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7F87: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
                                                     #ifdef DEBUG
                                      mon("         LDU   ,U                      ; 7F89: EE C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(U)<<8; 
    ea = ea | rd_mem((UINT16)((U)+1)); 
    U = ea; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         BNE   $7F7C                   ; 7F8B: 26 EF         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    clockticks = clockticks + 3; 
    goto L7F7C_;
    }
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8040                  ; 7F8D: CC 80 40      \n");
                                      #endif // Insert Vector CNTR
    clockticks = clockticks + 3; 
    A = 0x80; 
    B = 0x40; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,Y++                    ; 7F90: ED A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = Y; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    Y = Y + 2; 
  case 0x7F92:
L7F92_:
                                                     #ifdef DEBUG
                                      mon("L7F92_   LEAX  $08,X                   ; 7F92: 30 08         \n");
                                      #endif
    PC = 0x7f92; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    X = (X + 0x0008); 
 // Z = X; 
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$4C80                  ; 7F94: 8C 4C 80      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
    arg = 0x4c80; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
                                                     #ifdef DEBUG
                                      mon("         BCS   $7F6C                   ; 7F97: 25 D3         \n");
                                      #endif
    // temp hack to force a flush
    if (C) {
    clockticks = clockticks + 3; 
    goto L7F6C_;
    }
    // End of function sub_7EAF
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 7F99: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x7F9A:
L7F9A_:
                                                     #ifdef DEBUG
                                      mon("L7F9A    JSR   $CE45                   ; 7F9A: BD CE 45      \n");
                                      #endif
    PC = 0x7f9a; 
    INSTRUCTION_START
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x9d); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x7f); 
    clockticks = clockticks + 1; 
    PC = 0xce45; 
    JUMP;
  case 0x7F9D:
L7F9D_:
                                                     #ifdef DEBUG
                                      mon("L7F9D_   LDA   <$53                    ; 7F9D: 96 53         \n");
                                      #endif
    PC = 0x7f9d; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x53); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7F9F: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$54                    ; 7FA1: D6 54         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x54); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,X                      ; 7FA3: ED 84         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = X; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         MUL                           ; 7FA5: 3D            \n");
                                      #endif
    res = (UINT16)(A * B); 
 // Z = res; 
    C = res & 0x80; 
    A = res >> 8; 
    B = (UINT8)res; 
    clockticks = clockticks + 11; 
                                                     #ifdef DEBUG
                                      mon("         TST   $5002                   ; 7FA6: 7D 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x5002); 
    Z = val; 
    N = val; 
    V = 0; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BMI   $7FAC                   ; 7FA9: 2B 01         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)N <  0) {
    clockticks = clockticks + 3; 
    goto L7FAC_;
    }
                                                     #ifdef DEBUG
                                      mon("         NEGA                          ; 7FAB: 40            \n");
                                      #endif
    val = (UINT8)A; 
    res = -val; 
 // V = val & (UINT8)res; 
 // C = res & 0x100; 
    val = (UINT8)res; 
    A = val; 
 // Z = val; 
 // N = val; 
    clockticks = clockticks + 2; 
  case 0x7FAC:
L7FAC_:
                                                     #ifdef DEBUG
                                      mon("L7FAC_   STD   $02,X                   ; 7FAC: ED 02         \n");
                                      #endif
    PC = 0x7fac; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   <$53                    ; 7FAE: 96 53         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x53); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$1F                    ; 7FB0: 84 1F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x1f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$55                    ; 7FB2: D6 55         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x55); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         MUL                           ; 7FB4: 3D            \n");
                                      #endif
    res = (UINT16)(A * B); 
 // Z = res; 
    C = res & 0x80; 
    A = res >> 8; 
    B = (UINT8)res; 
    clockticks = clockticks + 11; 
                                                     #ifdef DEBUG
                                      mon("         TST   $5004                   ; 7FB5: 7D 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x5004); 
    Z = val; 
    N = val; 
    V = 0; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BMI   $7FBB                   ; 7FB8: 2B 01         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)N <  0) {
    clockticks = clockticks + 3; 
    goto L7FBB_;
    }
                                                     #ifdef DEBUG
                                      mon("         NEGA                          ; 7FBA: 40            \n");
                                      #endif
    val = (UINT8)A; 
    res = -val; 
 // V = val & (UINT8)res; 
    C = res & 0x100; 
    val = (UINT8)res; 
    A = val; 
 // Z = val; 
 // N = val; 
    clockticks = clockticks + 2; 
  case 0x7FBB:
L7FBB_:
                                                     #ifdef DEBUG
                                      mon("L7FBB_   STD   $04,X                   ; 7FBB: ED 04         \n");
                                      #endif
    PC = 0x7fbb; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   <$82                    ; 7FBD: 96 82         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x82); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STA   $4702                   ; 7FBF: B7 47 02      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = A; 
 // Z = val; 
 // N = val; 
 // V = 0; 
    wr_mem(0x4702, val); 
                                                     #ifdef DEBUG
                                      mon("         LDA   #$60                    ; 7FC2: 86 60         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x60; 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
                                                     #ifdef DEBUG
                                      mon("         JSR   $CDBA                   ; 7FC4: BD CD BA      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0xc7); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x7f); 
    clockticks = clockticks + 1; 
    PC = 0xcdba; 
    JUMP;
  case 0x7FC7:
L7FC7_:
                                                     #ifdef DEBUG
                                      mon("L7FC7_   LDD   $5000                   ; 7FC7: FC 50 00      \n");
                                      #endif // Math result X
    PC = 0x7fc7; 
    INSTRUCTION_START
    clockticks = clockticks + 5; 
    A = rd_mem(0x5000); // Care needed with I/O space and word fetches
    B = rd_mem(0x5001); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,X                      ; 7FCA: ED 84         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = X; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5002                   ; 7FCC: FC 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    A = rd_mem(0x5002); // Care needed with I/O space and word fetches
    B = rd_mem(0x5003); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,X                   ; 7FCF: ED 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   $5004                   ; 7FD1: FC 50 04      \n");
                                      #endif // Math result Z
    clockticks = clockticks + 5; 
    A = rd_mem(0x5004); // Care needed with I/O space and word fetches
    B = rd_mem(0x5005); 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$FF                    ; 7FD4: 84 FF         \n");
                                      #endif // These 2 lines do nothing ?
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0xff; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$FF                    ; 7FD6: C4 FF         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    res = val & (SINT8)0xff; 
    B = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,X                   ; 7FD8: ED 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    Z = A | B; 
    N = A; 
    V = 0; 
    ea = (UINT16)(X + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    // End of function sub_7F9A
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 7FDA: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
    // =============== S U B R O U T I N E =======================================
  case 0x7FDB:
L7FDB_:
                                                     #ifdef DEBUG
                                      mon("L7FDB    JSR   $CE45                   ; 7FDB: BD CE 45      \n");
                                      #endif // Random number update
    PC = 0x7fdb; 
    INSTRUCTION_START
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0xde); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x7f); 
    clockticks = clockticks + 1; 
    PC = 0xce45; 
    JUMP;
  case 0x7FDE:
L7FDE_:
                                                     #ifdef DEBUG
                                      mon("L7FDE_   LDA   <$53                    ; 7FDE: 96 53         \n");
                                      #endif
    PC = 0x7fde; 
    INSTRUCTION_START
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x53); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$7F                    ; 7FE0: 84 7F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x7f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ORA   #$70                    ; 7FE2: 8A 70         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0x70; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         LDB   <$54                    ; 7FE4: D6 54         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x54); 
    B = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ADDD  $5040                   ; 7FE6: F3 50 40      \n");
                                      #endif // XT2
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5040)<<8; 
    ea = ea | rd_mem(0x5041); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg + val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   ,X                      ; 7FE9: ED 84         \n");
                                      #endif
    clockticks = clockticks + 4; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = X; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDA   <$55                    ; 7FEB: 96 55         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x55); 
    A = val; 
 // N = val; 
 // Z = val; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$7F                    ; 7FED: 84 7F         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x7f; 
    A = res; 
 // Z = res; 
 // N = res; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         TST   $5002                   ; 7FEF: 7D 50 02      \n");
                                      #endif // Math result Y
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x5002); 
    Z = val; 
    N = val; 
    V = 0; 
    clockticks = clockticks + 2; 
                                                     #ifdef DEBUG
                                      mon("         BMI   $7FF5                   ; 7FF2: 2B 01         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)N <  0) {
    clockticks = clockticks + 3; 
    goto L7FF5_;
    }
                                                     #ifdef DEBUG
                                      mon("         NEGA                          ; 7FF4: 40            \n");
                                      #endif
    val = (UINT8)A; 
    res = -val; 
 // V = val & (UINT8)res; 
 // C = res & 0x100; 
    val = (UINT8)res; 
    A = val; 
 // Z = val; 
 // N = val; 
    clockticks = clockticks + 2; 
  case 0x7FF5:
L7FF5_:
                                                     #ifdef DEBUG
                                      mon("L7FF5_   ADDD  $5042                   ; 7FF5: F3 50 42      \n");
                                      #endif // YT2
    PC = 0x7ff5; 
    INSTRUCTION_START
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x5042)<<8; 
    ea = ea | rd_mem(0x5043); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg + val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
 // Z = res; 
 // V = ((arg ^ res) & (val ^ res)) >> 8; 
 // N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
                                                     #ifdef DEBUG
                                      mon("         STD   $02,X                   ; 7FF8: ED 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
 // Z = A | B; 
 // N = A; 
 // V = 0; 
    ea = (UINT16)(X + 0x0002); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 7FFA: CC 00 00      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x00; 
    B = 0x00; 
 // Z = A|B; 
 // N = A; 
 // V = 0; 
                                                     #ifdef DEBUG
                                      mon("         STD   $04,X                   ; 7FFD: ED 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    Z = A | B; 
    N = A; 
    V = 0; 
    ea = (UINT16)(X + 0x0004); 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    // End of function sub_7FDB
    // end of 'ROM'
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 7FFF: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
