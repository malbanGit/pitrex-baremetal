                   // ORG   $0000
  case 0x0000:
                                                     #ifdef DEBUG
                                      mon("ASRMem   ASR   <$02                    ; 0000: 07 02         \n");
                                      #endif
    val = (UINT8)rd_mem(DP|0x02); 
    C = val & 1; 
    val = (UINT8)((SINT8)val >> 1); 
    wr_mem(DP|0x02, val); 
    Z = val; 
    N = val; 
    PC = 0x0002; 
    JUMP; // sstep
  case 0x0002:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 0002: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x0400:
                                                     #ifdef DEBUG
                                      mon("ENTRYPT  PSHS  U,Y,X,DP,B,A,CC         ; 0400: 34 7F         \n");
                                      #endif
    --S;
    wr_mem(S, U); 
    --S;
    wr_mem(S, U >> 8); 
    --S;
    wr_mem(S, Y); 
    --S;
    wr_mem(S, Y >> 8); 
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    --S;
    wr_mem(S, (DP >> 8)); 
    --S;
    wr_mem(S, B); 
    --S;
    wr_mem(S, A); 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    PC = 0x0402; 
    JUMP; // sstep
  case 0x0402:
                                                     #ifdef DEBUG
                                      mon("         STS   $046E                   ; 0402: 10 FF 04 6E   \n");
                                      #endif
    val = S; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    wr_mem(0x046e, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x046f, (UINT8)val); 
    PC = 0x0406; 
    JUMP; // sstep
  case 0x0406:
                                                     #ifdef DEBUG
                                      mon("         LDS   #$0464                  ; 0406: 10 CE 04 64   \n");
                                      #endif
    S = 0x0464; 
/**/Z = S; 
/**/N = (S) >> 8; 
/**/V = 0; 
    PC = 0x040A; 
    JUMP; // sstep
  case 0x040A:
                                                     #ifdef DEBUG
                                      mon("         PULS  CC,A,B,DP,X,Y,U         ; 040A: 35 7F         \n");
                                      #endif
    CC = rd_mem(S); 
    S++;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    A = rd_mem(S); 
    S++;
    B = rd_mem(S); 
    S++;
    DP = rd_mem(S); 
    S++;
    DP = DP << 8; checkDP(); 
    // memory_DP = &memory[DP];
    X = rd_mem(S)<<8; 
    S++;
    X = X | rd_mem(S); 
    S++;
    Y = rd_mem(S)<<8; 
    S++;
    Y = Y | rd_mem(S); 
    S++;
    U = rd_mem(S)<<8; 
    S++;
    U = U | rd_mem(S); 
    S++;
    PC = 0x040C; 
    JUMP; // sstep
  case 0x040C:
                                                     #ifdef DEBUG
                                      mon("         JSR   ASRMem                  ; 040C: BD 00 00      \n");
                                      #endif
    --S;
    wr_mem(S, 0x0f); 
    --S;
    wr_mem(S, 0x04); 
    PC = 0x0000; 
    JUMP;
  case 0x040F:
                                                     #ifdef DEBUG
                                      mon("L040F    PSHS  U,Y,X,DP,B,A,CC         ; 040F: 34 7F         \n");
                                      #endif
    --S;
    wr_mem(S, U); 
    --S;
    wr_mem(S, U >> 8); 
    --S;
    wr_mem(S, Y); 
    --S;
    wr_mem(S, Y >> 8); 
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    --S;
    wr_mem(S, (DP >> 8)); 
    --S;
    wr_mem(S, B); 
    --S;
    wr_mem(S, A); 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    PC = 0x0411; 
    JUMP; // sstep
  case 0x0411:
                                                     #ifdef DEBUG
                                      mon("         LDS   $046E                   ; 0411: 10 FE 04 6E   \n");
                                      #endif
    ea = rd_mem(0x046e)<<8; 
    ea = ea | rd_mem(0x046f); 
    S = ea; 
/**/Z = S; 
/**/N = (S) >> 8; 
/**/V = 0; 
    PC = 0x0415; 
    JUMP; // sstep
  case 0x0415:
                                                     #ifdef DEBUG
                                      mon("         PULS  CC,A,B,DP,X,Y,U         ; 0415: 35 7F         \n");
                                      #endif
    CC = rd_mem(S); 
    S++;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    A = rd_mem(S); 
    S++;
    B = rd_mem(S); 
    S++;
    DP = rd_mem(S); 
    S++;
    DP = DP << 8; checkDP(); 
    // memory_DP = &memory[DP];
    X = rd_mem(S)<<8; 
    S++;
    X = X | rd_mem(S); 
    S++;
    Y = rd_mem(S)<<8; 
    S++;
    Y = Y | rd_mem(S); 
    S++;
    U = rd_mem(S)<<8; 
    S++;
    U = U | rd_mem(S); 
    S++;
    PC = 0x0417; 
    JUMP; // sstep
  case 0x0417:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 0417: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
