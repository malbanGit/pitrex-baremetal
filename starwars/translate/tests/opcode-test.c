                   // ORG   $0000
  case 0x0000:
                                                     #ifdef DEBUG
                                      mon("L0000    ABX                           ; 0000: 3A            \n");
                                      #endif
    X = X + B; 
    clockticks = clockticks + 3; 
    PC = 0x0001; 
    JUMP; // sstep
  case 0x0001:
                                                     #ifdef DEBUG
                                      mon("         ASLA                          ; 0001: 48            \n");
                                      #endif
    val = (UINT8)A; 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    A = res; 
    clockticks = clockticks + 2; 
    PC = 0x0002; 
    JUMP; // sstep
  case 0x0002:
                                                     #ifdef DEBUG
                                      mon("         ASLB                          ; 0002: 58            \n");
                                      #endif
    val = (UINT8)B; 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x0003; 
    JUMP; // sstep
  case 0x0003:
                                                     #ifdef DEBUG
                                      mon("         ASRA                          ; 0003: 47            \n");
                                      #endif
    val = (UINT8)A; 
/**/C = val & 1; 
    val = (UINT8)((SINT8)val >> 1); 
    A = val; 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x0004; 
    JUMP; // sstep
  case 0x0004:
                                                     #ifdef DEBUG
                                      mon("         ASRB                          ; 0004: 57            \n");
                                      #endif
    val = (UINT8)B; 
/**/C = val & 1; 
    val = (UINT8)((SINT8)val >> 1); 
    B = val; 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x0005; 
    JUMP; // sstep
  case 0x0005:
                                                     #ifdef DEBUG
                                      mon("         CLRA                          ; 0005: 4F            \n");
                                      #endif
    A = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    clockticks = clockticks + 2; 
    PC = 0x0006; 
    JUMP; // sstep
  case 0x0006:
                                                     #ifdef DEBUG
                                      mon("         CLRB                          ; 0006: 5F            \n");
                                      #endif
    B = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    clockticks = clockticks + 2; 
    PC = 0x0007; 
    JUMP; // sstep
  case 0x0007:
                                                     #ifdef DEBUG
                                      mon("         COMA                          ; 0007: 43            \n");
                                      #endif
    val = (UINT8)A ^ 0xff; 
    A = val; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
/**/C = 1; 
    clockticks = clockticks + 2; 
    PC = 0x0008; 
    JUMP; // sstep
  case 0x0008:
                                                     #ifdef DEBUG
                                      mon("         COMB                          ; 0008: 53            \n");
                                      #endif
    val = (UINT8)B ^ 0xff; 
    B = val; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    C = 1; 
    clockticks = clockticks + 2; 
    PC = 0x0009; 
    JUMP; // sstep
  case 0x0009:
                                                     #ifdef DEBUG
                                      mon("         DAA                           ; 0009: 19            \n");
                                      #endif
    res = A; 
    msn = res & 0xf0; 
    lsn = res & 0x0f; 
    res = res + (((lsn > 0x09) || (H & 0x10)) ? 0x06 : 0); 
    res = res + ((((UINT8)msn > 0x80U) && ((UINT8)lsn > 0x09U)) || (((UINT8)msn > 0x90U) || (C != 0)) ? 0x60 : 0); 
/**/C = C | (res & 0x100); 
    res = (UINT8)res; 
    A = res; 
/**/N = res; 
/**/Z = res; 
/**/V = 0; 
    clockticks = clockticks + 2; 
    PC = 0x000A; 
    JUMP; // sstep
  case 0x000A:
                                                     #ifdef DEBUG
                                      mon("         DECA                          ; 000A: 4A            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val - 1); 
/**/N = res; 
/**/Z = res; 
/**/V = val & ~res; 
    A = res; 
    clockticks = clockticks + 2; 
    PC = 0x000B; 
    JUMP; // sstep
  case 0x000B:
                                                     #ifdef DEBUG
                                      mon("         DECB                          ; 000B: 5A            \n");
                                      #endif
    val = (UINT8)B; 
    res = (UINT8)(val - 1); 
/**/N = res; 
/**/Z = res; 
/**/V = val & ~res; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x000C; 
    JUMP; // sstep
  case 0x000C:
                                                     #ifdef DEBUG
                                      mon("         INCA                          ; 000C: 4C            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val + 1); 
/**/Z = res; 
/**/N = res; 
/**/V = res & ~val; 
    A = res; 
    clockticks = clockticks + 2; 
    PC = 0x000D; 
    JUMP; // sstep
  case 0x000D:
                                                     #ifdef DEBUG
                                      mon("         INCB                          ; 000D: 5C            \n");
                                      #endif
    val = (UINT8)B; 
    res = (UINT8)(val + 1); 
/**/Z = res; 
/**/N = res; 
/**/V = res & ~val; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x000E; 
    JUMP; // sstep
  case 0x000E:
                                                     #ifdef DEBUG
                                      mon("         ASLA                          ; 000E: 48            \n");
                                      #endif
    val = (UINT8)A; 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    A = res; 
    clockticks = clockticks + 2; 
    PC = 0x000F; 
    JUMP; // sstep
  case 0x000F:
                                                     #ifdef DEBUG
                                      mon("         ASLB                          ; 000F: 58            \n");
                                      #endif
    val = (UINT8)B; 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x0010; 
    JUMP; // sstep
  case 0x0010:
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 0010: 44            \n");
                                      #endif
    val = (UINT8)A; 
/**/C = val & 1; 
    val = val >> 1; 
    A = val; 
/**/N = 0; 
/**/Z = val; 
    clockticks = clockticks + 2; 
    PC = 0x0011; 
    JUMP; // sstep
  case 0x0011:
                                                     #ifdef DEBUG
                                      mon("         LSRB                          ; 0011: 54            \n");
                                      #endif
    val = (UINT8)B; 
/**/C = val & 1; 
    val = val >> 1; 
    B = val; 
/**/N = 0; 
/**/Z = val; 
    clockticks = clockticks + 2; 
    PC = 0x0012; 
    JUMP; // sstep
  case 0x0012:
                                                     #ifdef DEBUG
                                      mon("L0012    MUL                           ; 0012: 3D            \n");
                                      #endif
    res = (UINT16)(A * B); 
/**/Z = res; 
/**/C = res & 0x80; 
    A = res >> 8; 
    B = (UINT8)res; 
    clockticks = clockticks + 11; 
    PC = 0x0013; 
    JUMP; // sstep
  case 0x0013:
                                                     #ifdef DEBUG
                                      mon("         NEGA                          ; 0013: 40            \n");
                                      #endif
    val = (UINT8)A; 
    res = -val; 
/**/V = val & (UINT8)res; 
/**/C = res & 0x100; 
    val = (UINT8)res; 
    A = val; 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x0014; 
    JUMP; // sstep
  case 0x0014:
                                                     #ifdef DEBUG
                                      mon("         NEGB                          ; 0014: 50            \n");
                                      #endif
    val = (UINT8)B; 
    res = -val; 
/**/V = val & (UINT8)res; 
    C = res & 0x100; 
    val = (UINT8)res; 
    B = val; 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x0015; 
    JUMP; // sstep
  case 0x0015:
                                                     #ifdef DEBUG
                                      mon("         NOP                           ; 0015: 12            \n");
                                      #endif
    clockticks = clockticks + 2; 
    PC = 0x0016; 
    JUMP; // sstep
  case 0x0016:
                                                     #ifdef DEBUG
                                      mon("         ROLA                          ; 0016: 49            \n");
                                      #endif
    val = (UINT8)A; 
    res = (val << 1) + (C != 0 ?1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)res ^ val; 
    A = (UINT8)res; 
    clockticks = clockticks + 2; 
    PC = 0x0017; 
    JUMP; // sstep
  case 0x0017:
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 0017: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
    PC = 0x0018; 
    JUMP; // sstep
  case 0x0018:
                                                     #ifdef DEBUG
                                      mon("         RORA                          ; 0018: 46            \n");
                                      #endif
    res = ((UINT8)A) | ((C != 0) ? 0x100 : 0); 
    C = res & 1; 
    res = (UINT8)(res >> 1); 
/**/Z = res; 
/**/N = res; 
    A = res; 
    clockticks = clockticks + 2; 
    PC = 0x0019; 
    JUMP; // sstep
  case 0x0019:
                                                     #ifdef DEBUG
                                      mon("         RORB                          ; 0019: 56            \n");
                                      #endif
    res = ((UINT8)B) | ((C != 0) ? 0x100 : 0); 
/**/C = res & 1; 
    res = (UINT8)(res >> 1); 
/**/Z = res; 
/**/N = res; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x001A; 
    JUMP; // sstep
  case 0x001A:
                                                     #ifdef DEBUG
                                      mon("         RTI                           ; 001A: 3B            \n");
                                      #endif
    {
    res=E;
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    CC = rd_mem(S); 
    S++;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    if (res) {
    clockticks = clockticks + 9; 
    clockticks = clockticks + 1; 
    A = rd_mem(S); 
    S++;
    clockticks = clockticks + 1; 
    B = rd_mem(S); 
    S++;
    clockticks = clockticks + 1; 
    DP = rd_mem(S); 
    S++;
    DP = DP << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 1; 
    X = rd_mem(S)<<8; 
    S++;
    X = X | rd_mem(S); 
    S++;
    clockticks = clockticks + 1; 
    Y = rd_mem(S)<<8; 
    S++;
    Y = Y | rd_mem(S); 
    S++;
    clockticks = clockticks + 1; 
    U = rd_mem(S)<<8; 
    S++;
    U = U | rd_mem(S); 
    S++;
    }
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    }
    
  case 0x001B:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 001B: 39            \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x001C:
                                                     #ifdef DEBUG
                                      mon("         SEX                           ; 001C: 1D            \n");
                                      #endif
    Z = B; 
    N = B; 
    A = ((SINT8)B < 0 ? 0xff : 0); 
    clockticks = clockticks + 2; 
    PC = 0x001D; 
    JUMP; // sstep
  case 0x001D:
                                                     #ifdef DEBUG
                                      mon("         SWI                           ; 001D: 3F            \n");
                                      #endif
    clockticks = clockticks + 19; 
    --S;
    wr_mem(S, 0x1e); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x00); 
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
  case 0x001E:
                                                     #ifdef DEBUG
                                      mon("L001E    SWI2                          ; 001E: 10 3F         \n");
                                      #endif
    clockticks = clockticks + 20; 
    --S;
    wr_mem(S, 0x20); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x00); 
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
    PC = (rd_mem(0xfff4)<<8)|rd_mem(0xfff5); 
    JUMP;
  case 0x0020:
                                                     #ifdef DEBUG
                                      mon("L0020    SWI3                          ; 0020: 11 3F         \n");
                                      #endif
    clockticks = clockticks + 20; 
    --S;
    wr_mem(S, 0x22); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x00); 
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
    PC = (rd_mem(0xfff2)<<8)|rd_mem(0xfff3); 
    JUMP;
  case 0x0022:
                                                     #ifdef DEBUG
                                      mon("L0022    SYNC                          ; 0022: 13            \n");
                                      #endif
    clockticks = clockticks + 4; 
    // SYNC - not suported yet!
  case 0x0023:
                                                     #ifdef DEBUG
                                      mon("L0023    TSTA                          ; 0023: 4D            \n");
                                      #endif
/**/val = (UINT8)A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    clockticks = clockticks + 2; 
    PC = 0x0024; 
    JUMP; // sstep
  case 0x0024:
                                                     #ifdef DEBUG
                                      mon("         TSTB                          ; 0024: 5D            \n");
                                      #endif
/**/val = (UINT8)B; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    clockticks = clockticks + 2; 
    PC = 0x0025; 
    JUMP; // sstep
  case 0x0025:
                                                     #ifdef DEBUG
                                      mon("         PSHS  B                       ; 0025: 34 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    PC = 0x0027; 
    JUMP; // sstep
  case 0x0027:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,S+                     ; 0027: AB E0         \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = (UINT8)A; 
    arg = rd_mem(S); 
    res = val + (UINT8)arg; 
    C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
    H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    S = S + 1; 
    PC = 0x0029; 
    JUMP; // sstep
  case 0x0029:
                                                     #ifdef DEBUG
                                      mon("         ASLB                          ; 0029: 58            \n");
                                      #endif
    val = (UINT8)B; 
    res = val << 1; 
    C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x002A; 
    JUMP; // sstep
  case 0x002A:
                                                     #ifdef DEBUG
                                      mon("         ROLA                          ; 002A: 49            \n");
                                      #endif
    val = (UINT8)A; 
    res = (val << 1) + (C != 0 ?1:0); 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)res ^ val; 
    A = (UINT8)res; 
    clockticks = clockticks + 2; 
    PC = 0x002B; 
    JUMP; // sstep
  case 0x002B:
                                                     #ifdef DEBUG
                                      mon("         ASRA                          ; 002B: 47            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = (UINT8)((SINT8)val >> 1); 
    A = val; 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x002C; 
    JUMP; // sstep
  case 0x002C:
                                                     #ifdef DEBUG
                                      mon("         RORB                          ; 002C: 56            \n");
                                      #endif
    res = ((UINT8)B) | ((C != 0) ? 0x100 : 0); 
/**/C = res & 1; 
    res = (UINT8)(res >> 1); 
/**/Z = res; 
/**/N = res; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x002D; 
    JUMP; // sstep
  case 0x002D:
                                                     #ifdef DEBUG
                                      mon("         PSHS  B                       ; 002D: 34 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    PC = 0x002F; 
    JUMP; // sstep
  case 0x002F:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,S+                     ; 002F: A1 E0         \n");
                                      #endif
    clockticks = clockticks + 6; 
/**/val = (UINT8)A; 
/**/arg = rd_mem(S); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    S = S + 1; 
    PC = 0x0031; 
    JUMP; // sstep
  case 0x0031:
                                                     #ifdef DEBUG
                                      mon("         ANDCC #$FE                    ; 0031: 1C FE         \n");
                                      #endif
    C = 0; 
    clockticks = clockticks + 3; 
    PC = 0x0033; 
    JUMP; // sstep
  case 0x0033:
                                                     #ifdef DEBUG
                                      mon("         ANDCC #$BF                    ; 0033: 1C BF         \n");
                                      #endif
    F = 0; 
    clockticks = clockticks + 3; 
    PC = 0x0035; 
    JUMP; // sstep
  case 0x0035:
                                                     #ifdef DEBUG
                                      mon("         ANDCC #$EF                    ; 0035: 1C EF         \n");
                                      #endif
    I = 0; 
    clockticks = clockticks + 3; 
    PC = 0x0037; 
    JUMP; // sstep
  case 0x0037:
                                                     #ifdef DEBUG
                                      mon("         ANDCC #$AF                    ; 0037: 1C AF         \n");
                                      #endif
    F = 0; 
    I = 0; 
    clockticks = clockticks + 3; 
    PC = 0x0039; 
    JUMP; // sstep
  case 0x0039:
                                                     #ifdef DEBUG
                                      mon("         CLRA                          ; 0039: 4F            \n");
                                      #endif
    A = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    clockticks = clockticks + 2; 
    PC = 0x003A; 
    JUMP; // sstep
  case 0x003A:
                                                     #ifdef DEBUG
                                      mon("         CLRB                          ; 003A: 5F            \n");
                                      #endif
    B = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    clockticks = clockticks + 2; 
    PC = 0x003B; 
    JUMP; // sstep
  case 0x003B:
                                                     #ifdef DEBUG
                                      mon("         ANDCC #$FD                    ; 003B: 1C FD         \n");
                                      #endif
    V = 0; 
    clockticks = clockticks + 3; 
    PC = 0x003D; 
    JUMP; // sstep
  case 0x003D:
                                                     #ifdef DEBUG
                                      mon("         COMA                          ; 003D: 43            \n");
                                      #endif
    val = (UINT8)A ^ 0xff; 
    A = val; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
/**/C = 1; 
    clockticks = clockticks + 2; 
    PC = 0x003E; 
    JUMP; // sstep
  case 0x003E:
                                                     #ifdef DEBUG
                                      mon("         COMB                          ; 003E: 53            \n");
                                      #endif
    val = (UINT8)B ^ 0xff; 
    B = val; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
/**/C = 1; 
    clockticks = clockticks + 2; 
    PC = 0x003F; 
    JUMP; // sstep
  case 0x003F:
                                                     #ifdef DEBUG
                                      mon("         LEAS  -1,S                    ; 003F: 32 7F         \n");
                                      #endif
    clockticks = clockticks + 5; 
    S = (S + 0xffff); 
    PC = 0x0041; 
    JUMP; // sstep
  case 0x0041:
                                                     #ifdef DEBUG
                                      mon("         LEAX  -1,X                    ; 0041: 30 1F         \n");
                                      #endif
    clockticks = clockticks + 5; 
    X = (X + 0xffff); 
/**/Z = X; 
    PC = 0x0043; 
    JUMP; // sstep
  case 0x0043:
                                                     #ifdef DEBUG
                                      mon("         LEAY  -1,Y                    ; 0043: 31 3F         \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + 0xffff); 
/**/Z = Y; 
    PC = 0x0045; 
    JUMP; // sstep
  case 0x0045:
                                                     #ifdef DEBUG
                                      mon("         LEAU  -1,U                    ; 0045: 33 5F         \n");
                                      #endif
    clockticks = clockticks + 5; 
    U = (U + 0xffff); 
    PC = 0x0047; 
    JUMP; // sstep
  case 0x0047:
                                                     #ifdef DEBUG
                                      mon("         LEAS  $01,S                   ; 0047: 32 61         \n");
                                      #endif
    clockticks = clockticks + 5; 
    S = (S + 0x0001); 
    PC = 0x0049; 
    JUMP; // sstep
  case 0x0049:
                                                     #ifdef DEBUG
                                      mon("         LEAU  $01,U                   ; 0049: 33 41         \n");
                                      #endif
    clockticks = clockticks + 5; 
    U = (U + 0x0001); 
    PC = 0x004B; 
    JUMP; // sstep
  case 0x004B:
                                                     #ifdef DEBUG
                                      mon("         LEAX  $01,X                   ; 004B: 30 01         \n");
                                      #endif
    clockticks = clockticks + 5; 
    X = (X + 0x0001); 
/**/Z = X; 
    PC = 0x004D; 
    JUMP; // sstep
  case 0x004D:
                                                     #ifdef DEBUG
                                      mon("         LEAY  $01,Y                   ; 004D: 31 21         \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + 0x0001); 
/**/Z = Y; 
    PC = 0x004F; 
    JUMP; // sstep
  case 0x004F:
                                                     #ifdef DEBUG
                                      mon("         ASLB                          ; 004F: 58            \n");
                                      #endif
    val = (UINT8)B; 
    res = val << 1; 
    C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x0050; 
    JUMP; // sstep
  case 0x0050:
                                                     #ifdef DEBUG
                                      mon("         ROLA                          ; 0050: 49            \n");
                                      #endif
    val = (UINT8)A; 
    res = (val << 1) + (C != 0 ?1:0); 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
    V = (UINT8)res ^ val; 
    A = (UINT8)res; 
    clockticks = clockticks + 2; 
    PC = 0x0051; 
    JUMP; // sstep
  case 0x0051:
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 0051: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
/**/N = 0; 
/**/Z = val; 
    clockticks = clockticks + 2; 
    PC = 0x0052; 
    JUMP; // sstep
  case 0x0052:
                                                     #ifdef DEBUG
                                      mon("         RORB                          ; 0052: 56            \n");
                                      #endif
    res = ((UINT8)B) | ((C != 0) ? 0x100 : 0); 
    C = res & 1; 
    res = (UINT8)(res >> 1); 
    Z = res; 
    N = res; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x0053; 
    JUMP; // sstep
  case 0x0053:
                                                     #ifdef DEBUG
                                      mon("         PSHS  A                       ; 0053: 34 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, A); 
    clockticks = clockticks + 1; 
    PC = 0x0055; 
    JUMP; // sstep
  case 0x0055:
                                                     #ifdef DEBUG
                                      mon("         PSHS  B                       ; 0055: 34 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    PC = 0x0057; 
    JUMP; // sstep
  case 0x0057:
                                                     #ifdef DEBUG
                                      mon("         PSHS  CC                      ; 0057: 34 01         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    clockticks = clockticks + 1; 
    PC = 0x0059; 
    JUMP; // sstep
  case 0x0059:
                                                     #ifdef DEBUG
                                      mon("         PSHS  B,A                     ; 0059: 34 06         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, A); 
    clockticks = clockticks + 1; 
    PC = 0x005B; 
    JUMP; // sstep
  case 0x005B:
                                                     #ifdef DEBUG
                                      mon("         PSHS  DP                      ; 005B: 34 08         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, (DP >> 8)); 
    clockticks = clockticks + 1; 
    PC = 0x005D; 
    JUMP; // sstep
  case 0x005D:
                                                     #ifdef DEBUG
                                      mon("         PSHS  PC                      ; 005D: 34 80         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    --S;
    wr_mem(S, 0x5f); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x00); 
    clockticks = clockticks + 1; 
    PC = 0x005F; 
    JUMP; // sstep
  case 0x005F:
                                                     #ifdef DEBUG
                                      mon("         PSHS  X                       ; 005F: 34 10         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    clockticks = clockticks + 1; 
    PC = 0x0061; 
    JUMP; // sstep
  case 0x0061:
                                                     #ifdef DEBUG
                                      mon("         PSHS  Y                       ; 0061: 34 20         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    --S;
    wr_mem(S, Y); 
    --S;
    wr_mem(S, Y >> 8); 
    clockticks = clockticks + 1; 
    PC = 0x0063; 
    JUMP; // sstep
  case 0x0063:
                                                     #ifdef DEBUG
                                      mon("         PULS  A                       ; 0063: 35 02         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    A = rd_mem(S); 
    S++;
    PC = 0x0065; 
    JUMP; // sstep
  case 0x0065:
                                                     #ifdef DEBUG
                                      mon("         PULS  B                       ; 0065: 35 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    B = rd_mem(S); 
    S++;
    PC = 0x0067; 
    JUMP; // sstep
  case 0x0067:
                                                     #ifdef DEBUG
                                      mon("         PULS  CC                      ; 0067: 35 01         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    CC = rd_mem(S); 
    S++;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x0069; 
    JUMP; // sstep
  case 0x0069:
                                                     #ifdef DEBUG
                                      mon("         PULS  A,B                     ; 0069: 35 06         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    A = rd_mem(S); 
    S++;
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    B = rd_mem(S); 
    S++;
    PC = 0x006B; 
    JUMP; // sstep
  case 0x006B:
                                                     #ifdef DEBUG
                                      mon("         PULS  DP                      ; 006B: 35 08         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    DP = rd_mem(S); 
    S++;
    DP = DP << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x006D; 
    JUMP; // sstep
  case 0x006D:
                                                     #ifdef DEBUG
                                      mon("         PULS  PC                      ; 006D: 35 80         \n");
                                      #endif    // (PUL? PC=RTS)
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x006F:
                                                     #ifdef DEBUG
                                      mon("         PULS  X                       ; 006F: 35 10         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    X = rd_mem(S)<<8; 
    S++;
    X = X | rd_mem(S); 
    S++;
    PC = 0x0071; 
    JUMP; // sstep
  case 0x0071:
                                                     #ifdef DEBUG
                                      mon("         PULS  Y                       ; 0071: 35 20         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    Y = rd_mem(S)<<8; 
    S++;
    Y = Y | rd_mem(S); 
    S++;
    PC = 0x0073; 
    JUMP; // sstep
  case 0x0073:
                                                     #ifdef DEBUG
                                      mon("         ROLB                          ; 0073: 59            \n");
                                      #endif
    val = (UINT8)B; 
    res = (val << 1) + (C != 0 ?1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)res ^ val; 
    B = (UINT8)res; 
    clockticks = clockticks + 2; 
    PC = 0x0074; 
    JUMP; // sstep
  case 0x0074:
                                                     #ifdef DEBUG
                                      mon("         ROLA                          ; 0074: 49            \n");
                                      #endif
    val = (UINT8)A; 
    res = (val << 1) + (C != 0 ?1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)res ^ val; 
    A = (UINT8)res; 
    clockticks = clockticks + 2; 
    PC = 0x0075; 
    JUMP; // sstep
  case 0x0075:
                                                     #ifdef DEBUG
                                      mon("         RORA                          ; 0075: 46            \n");
                                      #endif
    res = ((UINT8)A) | ((C != 0) ? 0x100 : 0); 
    C = res & 1; 
    res = (UINT8)(res >> 1); 
/**/Z = res; 
/**/N = res; 
    A = res; 
    clockticks = clockticks + 2; 
    PC = 0x0076; 
    JUMP; // sstep
  case 0x0076:
                                                     #ifdef DEBUG
                                      mon("         RORB                          ; 0076: 56            \n");
                                      #endif
    res = ((UINT8)B) | ((C != 0) ? 0x100 : 0); 
/**/C = res & 1; 
    res = (UINT8)(res >> 1); 
/**/Z = res; 
/**/N = res; 
    B = res; 
    clockticks = clockticks + 2; 
    PC = 0x0077; 
    JUMP; // sstep
  case 0x0077:
                                                     #ifdef DEBUG
                                      mon("         PSHS  B                       ; 0077: 34 04         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    PC = 0x0079; 
    JUMP; // sstep
  case 0x0079:
                                                     #ifdef DEBUG
                                      mon("         SUBA  ,S+                     ; 0079: A0 E0         \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = (UINT8)A; 
    arg = rd_mem(S); 
    res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)(res); 
    S = S + 1; 
    PC = 0x007B; 
    JUMP; // sstep
  case 0x007B:
                                                     #ifdef DEBUG
                                      mon("         ORCC  #$01                    ; 007B: 1A 01         \n");
                                      #endif
    C = 1; 
    clockticks = clockticks + 3; 
    PC = 0x007D; 
    JUMP; // sstep
  case 0x007D:
                                                     #ifdef DEBUG
                                      mon("         ORCC  #$40                    ; 007D: 1A 40         \n");
                                      #endif
    F = 1; 
    clockticks = clockticks + 3; 
    PC = 0x007F; 
    JUMP; // sstep
  case 0x007F:
                                                     #ifdef DEBUG
                                      mon("         ORCC  #$10                    ; 007F: 1A 10         \n");
                                      #endif
    I = 1; 
    clockticks = clockticks + 3; 
    PC = 0x0081; 
    JUMP; // sstep
  case 0x0081:
                                                     #ifdef DEBUG
                                      mon("         ORCC  #$50                    ; 0081: 1A 50         \n");
                                      #endif
    F = 1; 
    I = 1; 
    clockticks = clockticks + 3; 
    PC = 0x0083; 
    JUMP; // sstep
  case 0x0083:
                                                     #ifdef DEBUG
                                      mon("         ORCC  #$02                    ; 0083: 1A 02         \n");
                                      #endif
    V = 0x80; 
    clockticks = clockticks + 3; 
    PC = 0x0085; 
    JUMP; // sstep
  case 0x0085:
                                                     #ifdef DEBUG
                                      mon("         TFR   A,B                     ; 0085: 1F 89         \n");
                                      #endif
    B = (UINT8)A; 
    clockticks = clockticks + 6; 
    PC = 0x0087; 
    JUMP; // sstep
  case 0x0087:
                                                     #ifdef DEBUG
                                      mon("         TSTA                          ; 0087: 4D            \n");
                                      #endif
/**/val = (UINT8)A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    clockticks = clockticks + 2; 
    PC = 0x0088; 
    JUMP; // sstep
  case 0x0088:
                                                     #ifdef DEBUG
                                      mon("         TFR   A,CC                    ; 0088: 1F 8A         \n");
                                      #endif
    CC = A; 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    clockticks = clockticks + 6; 
    PC = 0x008A; 
    JUMP; // sstep
  case 0x008A:
                                                     #ifdef DEBUG
                                      mon("         TFR   B,A                     ; 008A: 1F 98         \n");
                                      #endif
    A = (UINT8)B; 
    clockticks = clockticks + 6; 
    PC = 0x008C; 
    JUMP; // sstep
  case 0x008C:
                                                     #ifdef DEBUG
                                      mon("         TSTA                          ; 008C: 4D            \n");
                                      #endif
    val = (UINT8)A; 
    Z = val; 
    N = val; 
    V = 0; 
    clockticks = clockticks + 2; 
    PC = 0x008D; 
    JUMP; // sstep
  case 0x008D:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,A                    ; 008D: 1F A8         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    A = (UINT8)CC; 
    clockticks = clockticks + 6; 
    PC = 0x008F; 
    JUMP; // sstep
  case 0x008F:
                                                     #ifdef DEBUG
                                      mon("         TFR   S,X                     ; 008F: 1F 41         \n");
                                      #endif
    X = S; 
    clockticks = clockticks + 6; 
    PC = 0x0091; 
    JUMP; // sstep
  case 0x0091:
                                                     #ifdef DEBUG
                                      mon("         TFR   X,S                     ; 0091: 1F 14         \n");
                                      #endif
    S = X; 
    clockticks = clockticks + 6; 
    PC = 0x0093; 
    JUMP; // sstep
  case 0x0093:
                                                     #ifdef DEBUG
                                      mon("         CWAI  #$FF                    ; 0093: 3C FF         \n");
                                      #endif
    // CWAI - not suported yet!
    PC = 0x0095; 
    JUMP; // sstep
  case 0x0095:
                                                     #ifdef DEBUG
                                      mon("L0095    BCC   $95                     ; 0095: 24 FE         \n");
                                      #endif
    // temp hack to force a flush
    if (!C) {
    clockticks = clockticks + 3; 
    PC = 0x0095; 
    JUMP;
    }
    PC = 0x0097; 
    JUMP; // sstep
  case 0x0097:
                                                     #ifdef DEBUG
                                      mon("         BCS   $B9                     ; 0097: 25 20         \n");
                                      #endif
    // temp hack to force a flush
    if (C) {
    clockticks = clockticks + 3; 
    PC = 0x00b9; 
    JUMP;
    }
    PC = 0x0099; 
    JUMP; // sstep
  case 0x0099:
                                                     #ifdef DEBUG
                                      mon("         BEQ   $95                     ; 0099: 27 FA         \n");
                                      #endif
    // temp hack to force a flush
    if (!Z) {
    clockticks = clockticks + 3; 
    PC = 0x0095; 
    JUMP;
    }
    PC = 0x009B; 
    JUMP; // sstep
  case 0x009B:
                                                     #ifdef DEBUG
                                      mon("         BGE   $B9                     ; 009B: 2C 1C         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)(N^V) >= 0) {
    clockticks = clockticks + 3; 
    PC = 0x00b9; 
    JUMP;
    }
    PC = 0x009D; 
    JUMP; // sstep
  case 0x009D:
                                                     #ifdef DEBUG
                                      mon("         BGT   $95                     ; 009D: 2E F6         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 3; 
    PC = 0x0095; 
    JUMP;
    }
    PC = 0x009F; 
    JUMP; // sstep
  case 0x009F:
                                                     #ifdef DEBUG
                                      mon("         BHI   $B9                     ; 009F: 22 18         \n");
                                      #endif
    // temp hack to force a flush
    if (Z && (!C)) {
    clockticks = clockticks + 3; 
    PC = 0x00b9; 
    JUMP;
    }
    PC = 0x00A1; 
    JUMP; // sstep
  case 0x00A1:
                                                     #ifdef DEBUG
                                      mon("         BCC   $95                     ; 00A1: 24 F2         \n");
                                      #endif
    // temp hack to force a flush
    if (!C) {
    clockticks = clockticks + 3; 
    PC = 0x0095; 
    JUMP;
    }
    PC = 0x00A3; 
    JUMP; // sstep
  case 0x00A3:
                                                     #ifdef DEBUG
                                      mon("         BLE   $B9                     ; 00A3: 2F 14         \n");
                                      #endif
    // temp hack to force a flush
    if (((SINT8)(N^V) <  0) || (!Z)) {
    clockticks = clockticks + 3; 
    PC = 0x00b9; 
    JUMP;
    }
    PC = 0x00A5; 
    JUMP; // sstep
  case 0x00A5:
                                                     #ifdef DEBUG
                                      mon("         BCS   $95                     ; 00A5: 25 EE         \n");
                                      #endif
    // temp hack to force a flush
    if (C) {
    clockticks = clockticks + 3; 
    PC = 0x0095; 
    JUMP;
    }
    PC = 0x00A7; 
    JUMP; // sstep
  case 0x00A7:
                                                     #ifdef DEBUG
                                      mon("         BLS   $B9                     ; 00A7: 23 10         \n");
                                      #endif
    // temp hack to force a flush
    if ((!Z) || C) {
    clockticks = clockticks + 3; 
    PC = 0x00b9; 
    JUMP;
    }
    PC = 0x00A9; 
    JUMP; // sstep
  case 0x00A9:
                                                     #ifdef DEBUG
                                      mon("         BLT   $95                     ; 00A9: 2D EA         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)(N^V) <  0) {
    clockticks = clockticks + 3; 
    PC = 0x0095; 
    JUMP;
    }
    PC = 0x00AB; 
    JUMP; // sstep
  case 0x00AB:
                                                     #ifdef DEBUG
                                      mon("         BMI   $B9                     ; 00AB: 2B 0C         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)N <  0) {
    clockticks = clockticks + 3; 
    PC = 0x00b9; 
    JUMP;
    }
    PC = 0x00AD; 
    JUMP; // sstep
  case 0x00AD:
                                                     #ifdef DEBUG
                                      mon("         BNE   $95                     ; 00AD: 26 E6         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    clockticks = clockticks + 3; 
    PC = 0x0095; 
    JUMP;
    }
    PC = 0x00AF; 
    JUMP; // sstep
  case 0x00AF:
                                                     #ifdef DEBUG
                                      mon("         BPL   $B9                     ; 00AF: 2A 08         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)N >= 0) {
    clockticks = clockticks + 3; 
    PC = 0x00b9; 
    JUMP;
    }
    PC = 0x00B1; 
    JUMP; // sstep
  case 0x00B1:
                                                     #ifdef DEBUG
                                      mon("         BRA   $95                     ; 00B1: 20 E2         \n");
                                      #endif
    clockticks = clockticks + 3; 
    PC = 0x0095; 
    JUMP;
    
  case 0x00B3:
                                                     #ifdef DEBUG
                                      mon("         BRN   $B9                     ; 00B3: 21 04         \n");
                                      #endif
    clockticks = clockticks + 3; 
    PC = 0x00B5; 
    JUMP; // sstep
  case 0x00B5:
                                                     #ifdef DEBUG
                                      mon("         BSR   $95                     ; 00B5: 8D DE         \n");
                                      #endif
    --S;
    wr_mem(S, 0xb7); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x00); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 7; 
    PC = 0x0095; 
    JUMP;
  case 0x00B7:
                                                     #ifdef DEBUG
                                      mon("L00B7    BVC   $B9                     ; 00B7: 28 00         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)V >= 0) {
    clockticks = clockticks + 3; 
    PC = 0x00b9; 
    JUMP;
    }
    PC = 0x00B9; 
    JUMP; // sstep
  case 0x00B9:
                                                     #ifdef DEBUG
                                      mon("L00B9    BVS   $95                     ; 00B9: 29 DA         \n");
                                      #endif
    // temp hack to force a flush
    if ((SINT8)V <  0) {
    clockticks = clockticks + 3; 
    PC = 0x0095; 
    JUMP;
    }
    PC = 0x00BB; 
    JUMP; // sstep
  case 0x00BB:
                                                     #ifdef DEBUG
                                      mon("         LBCC  $1234                   ; 00BB: 10 24 11 75   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (!C) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00BF; 
    JUMP; // sstep
  case 0x00BF:
                                                     #ifdef DEBUG
                                      mon("         LBCS  $1234                   ; 00BF: 10 25 11 71   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (C) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00C3; 
    JUMP; // sstep
  case 0x00C3:
                                                     #ifdef DEBUG
                                      mon("         LBEQ  $1234                   ; 00C3: 10 27 11 6D   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (!Z) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00C7; 
    JUMP; // sstep
  case 0x00C7:
                                                     #ifdef DEBUG
                                      mon("         LBGE  $1234                   ; 00C7: 10 2C 11 69   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if ((SINT8)(N^V) >= 0) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00CB; 
    JUMP; // sstep
  case 0x00CB:
                                                     #ifdef DEBUG
                                      mon("         LBGT  $1234                   ; 00CB: 10 2E 11 65   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (((SINT8)(N^V) >= 0) && Z) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00CF; 
    JUMP; // sstep
  case 0x00CF:
                                                     #ifdef DEBUG
                                      mon("         LBHI  $1234                   ; 00CF: 10 22 11 61   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (Z && (!C)) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00D3; 
    JUMP; // sstep
  case 0x00D3:
                                                     #ifdef DEBUG
                                      mon("         LBCC  $1234                   ; 00D3: 10 24 11 5D   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (!C) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00D7; 
    JUMP; // sstep
  case 0x00D7:
                                                     #ifdef DEBUG
                                      mon("         LBLE  $1234                   ; 00D7: 10 2F 11 59   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (((SINT8)(N^V) <  0) || (!Z)) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00DB; 
    JUMP; // sstep
  case 0x00DB:
                                                     #ifdef DEBUG
                                      mon("         LBCS  $1234                   ; 00DB: 10 25 11 55   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (C) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00DF; 
    JUMP; // sstep
  case 0x00DF:
                                                     #ifdef DEBUG
                                      mon("         LBLS  $1234                   ; 00DF: 10 23 11 51   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if ((!Z) || C) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00E3; 
    JUMP; // sstep
  case 0x00E3:
                                                     #ifdef DEBUG
                                      mon("         LBLT  $1234                   ; 00E3: 10 2D 11 4D   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if ((SINT8)(N^V) <  0) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00E7; 
    JUMP; // sstep
  case 0x00E7:
                                                     #ifdef DEBUG
                                      mon("         LBMI  $1234                   ; 00E7: 10 2B 11 49   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if ((SINT8)N <  0) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00EB; 
    JUMP; // sstep
  case 0x00EB:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $1234                   ; 00EB: 10 26 11 45   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if (Z) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00EF; 
    JUMP; // sstep
  case 0x00EF:
                                                     #ifdef DEBUG
                                      mon("         LBPL  $1234                   ; 00EF: 10 2A 11 41   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if ((SINT8)N >= 0) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x00F3; 
    JUMP; // sstep
  case 0x00F3:
                                                     #ifdef DEBUG
                                      mon("         LBRA  $1234                   ; 00F3: 16 11 3E      \n");
                                      #endif
    PC = 0x1234; 
    JUMP;
    clockticks = clockticks + 5; 
    
  case 0x00F6:
                                                     #ifdef DEBUG
                                      mon("         LBRN  $1234                   ; 00F6: 10 21 11 3A   \n");
                                      #endif
    clockticks = clockticks + 5; 
    PC = 0x00FA; 
    JUMP; // sstep
  case 0x00FA:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $1234                   ; 00FA: 17 11 37      \n");
                                      #endif
    --S;
    wr_mem(S, 0xfd); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x00); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 9; 
    PC = 0x1234; 
    JUMP;
  case 0x00FD:
                                                     #ifdef DEBUG
                                      mon("L00FD    LBVC  $1234                   ; 00FD: 10 28 11 33   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if ((SINT8)V >= 0) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x0101; 
    JUMP; // sstep
  case 0x0101:
                                                     #ifdef DEBUG
                                      mon("         LBVS  $1234                   ; 0101: 10 29 11 2F   \n");
                                      #endif
    clockticks = clockticks + 5; 
    if ((SINT8)V <  0) {
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
    }
    PC = 0x0105; 
    JUMP; // sstep
  case 0x0105:
                                                     #ifdef DEBUG
                                      mon("         ADCA  #$12                    ; 0105: 89 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    arg = A; 
    val = (SINT8)0x12; 
    res = arg + val + (C != 0 ? 1:0); 
    C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = arg ^ val ^ (UINT8)res ^ C; 
/**/V = arg ^ val ^ (UINT8)res ^ C; 
    A = (UINT8)res; 
    PC = 0x0107; 
    JUMP; // sstep
  case 0x0107:
                                                     #ifdef DEBUG
                                      mon("         ADCA  <$12                    ; 0107: 99 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    arg = A; 
    val = rd_mem(DP|0x12); 
    res = arg + val + (C != 0 ? 1:0); 
    C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = arg ^ val ^ (UINT8)res ^ C; 
/**/V = arg ^ val ^ (UINT8)res ^ C; 
    A = (UINT8)res; 
    PC = 0x0109; 
    JUMP; // sstep
  case 0x0109:
                                                     #ifdef DEBUG
                                      mon("         ADCA  $1234                   ; 0109: B9 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    arg = A; 
    val = rd_mem(0x1234); 
    res = arg + val + (C != 0 ? 1:0); 
    C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = arg ^ val ^ (UINT8)res ^ C; 
/**/V = arg ^ val ^ (UINT8)res ^ C; 
    A = (UINT8)res; 
    PC = 0x010C; 
    JUMP; // sstep
  case 0x010C:
                                                     #ifdef DEBUG
                                      mon("         ADCA  [$1234]                 ; 010C: A9 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    arg = A; 
    val = rd_mem(ea); 
    res = arg + val + (C != 0 ? 1:0); 
    C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = arg ^ val ^ (UINT8)res ^ C; 
/**/V = arg ^ val ^ (UINT8)res ^ C; 
    A = (UINT8)res; 
    PC = 0x0110; 
    JUMP; // sstep
  case 0x0110:
                                                     #ifdef DEBUG
                                      mon("         ADCB  #$12                    ; 0110: C9 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    arg = B; 
    val = (SINT8)0x12; 
    res = arg + val + (C != 0 ? 1:0); 
    C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = arg ^ val ^ (UINT8)res ^ C; 
/**/V = arg ^ val ^ (UINT8)res ^ C; 
    B = (UINT8)res; 
    PC = 0x0112; 
    JUMP; // sstep
  case 0x0112:
                                                     #ifdef DEBUG
                                      mon("         ADCB  <$12                    ; 0112: D9 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    arg = B; 
    val = rd_mem(DP|0x12); 
    res = arg + val + (C != 0 ? 1:0); 
    C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = arg ^ val ^ (UINT8)res ^ C; 
/**/V = arg ^ val ^ (UINT8)res ^ C; 
    B = (UINT8)res; 
    PC = 0x0114; 
    JUMP; // sstep
  case 0x0114:
                                                     #ifdef DEBUG
                                      mon("         ADCB  $1234                   ; 0114: F9 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    arg = B; 
    val = rd_mem(0x1234); 
    res = arg + val + (C != 0 ? 1:0); 
    C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = arg ^ val ^ (UINT8)res ^ C; 
/**/V = arg ^ val ^ (UINT8)res ^ C; 
    B = (UINT8)res; 
    PC = 0x0117; 
    JUMP; // sstep
  case 0x0117:
                                                     #ifdef DEBUG
                                      mon("         ADCB  [$1234]                 ; 0117: E9 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    arg = B; 
    val = rd_mem(ea); 
    res = arg + val + (C != 0 ? 1:0); 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = arg ^ val ^ (UINT8)res ^ C; 
/**/V = arg ^ val ^ (UINT8)res ^ C; 
    B = (UINT8)res; 
    PC = 0x011B; 
    JUMP; // sstep
  case 0x011B:
                                                     #ifdef DEBUG
                                      mon("         ADDA  #$12                    ; 011B: 8B 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    arg = 0x12; 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x011D; 
    JUMP; // sstep
  case 0x011D:
                                                     #ifdef DEBUG
                                      mon("         ADDA  <$12                    ; 011D: 9B 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)A; 
    arg = rd_mem(DP|0x12); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x011F; 
    JUMP; // sstep
  case 0x011F:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $1234                   ; 011F: BB 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem(0x1234); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0122; 
    JUMP; // sstep
  case 0x0122:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$1234]                 ; 0122: AB 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0126; 
    JUMP; // sstep
  case 0x0126:
                                                     #ifdef DEBUG
                                      mon("         ADDB  #$12                    ; 0126: CB 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    arg = 0x12; 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    B = (UINT8)res; 
    PC = 0x0128; 
    JUMP; // sstep
  case 0x0128:
                                                     #ifdef DEBUG
                                      mon("         ADDB  <$12                    ; 0128: DB 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)B; 
    arg = rd_mem(DP|0x12); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    B = (UINT8)res; 
    PC = 0x012A; 
    JUMP; // sstep
  case 0x012A:
                                                     #ifdef DEBUG
                                      mon("         ADDB  $1234                   ; 012A: FB 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)B; 
    arg = rd_mem(0x1234); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    B = (UINT8)res; 
    PC = 0x012D; 
    JUMP; // sstep
  case 0x012D:
                                                     #ifdef DEBUG
                                      mon("         ADDB  [$1234]                 ; 012D: EB 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)B; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    B = (UINT8)res; 
    PC = 0x0131; 
    JUMP; // sstep
  case 0x0131:
                                                     #ifdef DEBUG
                                      mon("         ADDD  #$1234                  ; 0131: C3 12 34      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0x1234; 
    arg = (A << 8) | B; 
    res = arg + val; 
/**/C = res & 0x10000; 
    res = (UINT16)res; 
/**/Z = res; 
/**/V = ((arg ^ res) & (val ^ res)) >> 8; 
/**/N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
    PC = 0x0134; 
    JUMP; // sstep
  case 0x0134:
                                                     #ifdef DEBUG
                                      mon("         ADDD  <$12                    ; 0134: D3 12         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg + val; 
/**/C = res & 0x10000; 
    res = (UINT16)res; 
/**/Z = res; 
/**/V = ((arg ^ res) & (val ^ res)) >> 8; 
/**/N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
    PC = 0x0136; 
    JUMP; // sstep
  case 0x0136:
                                                     #ifdef DEBUG
                                      mon("         ADDD  $1234                   ; 0136: F3 12 34      \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg + val; 
/**/C = res & 0x10000; 
    res = (UINT16)res; 
/**/Z = res; 
/**/V = ((arg ^ res) & (val ^ res)) >> 8; 
/**/N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
    PC = 0x0139; 
    JUMP; // sstep
  case 0x0139:
                                                     #ifdef DEBUG
                                      mon("         ADDD  [$1234]                 ; 0139: E3 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg + val; 
/**/C = res & 0x10000; 
    res = (UINT16)res; 
/**/Z = res; 
/**/V = ((arg ^ res) & (val ^ res)) >> 8; 
/**/N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
    PC = 0x013D; 
    JUMP; // sstep
  case 0x013D:
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$12                    ; 013D: 84 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    res = val & (SINT8)0x12; 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x013F; 
    JUMP; // sstep
  case 0x013F:
                                                     #ifdef DEBUG
                                      mon("         ANDA  <$12                    ; 013F: 94 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)A; 
    res = val & rd_mem(DP|0x12); 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0141; 
    JUMP; // sstep
  case 0x0141:
                                                     #ifdef DEBUG
                                      mon("         ANDA  $1234                   ; 0141: B4 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    res = val & rd_mem(0x1234); 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0144; 
    JUMP; // sstep
  case 0x0144:
                                                     #ifdef DEBUG
                                      mon("         ANDA  [$1234]                 ; 0144: A4 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)A; 
    res = val & rd_mem(ea); 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0148; 
    JUMP; // sstep
  case 0x0148:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$12                    ; 0148: C4 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    res = val & (SINT8)0x12; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x014A; 
    JUMP; // sstep
  case 0x014A:
                                                     #ifdef DEBUG
                                      mon("         ANDB  <$12                    ; 014A: D4 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)B; 
    res = val & rd_mem(DP|0x12); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x014C; 
    JUMP; // sstep
  case 0x014C:
                                                     #ifdef DEBUG
                                      mon("         ANDB  $1234                   ; 014C: F4 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)B; 
    res = val & rd_mem(0x1234); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x014F; 
    JUMP; // sstep
  case 0x014F:
                                                     #ifdef DEBUG
                                      mon("         ANDB  [$1234]                 ; 014F: E4 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)B; 
    res = val & rd_mem(ea); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0153; 
    JUMP; // sstep
  case 0x0153:
                                                     #ifdef DEBUG
                                      mon("         ANDCC #$12                    ; 0153: 1C 12         \n");
                                      #endif
    E = 0; 
    F = 0; 
    H = 0; 
    N = 0; 
    Z = 1; 
    C = 0; 
    clockticks = clockticks + 3; 
    PC = 0x0155; 
    JUMP; // sstep
  case 0x0155:
                                                     #ifdef DEBUG
                                      mon("         ASL   <$12                    ; 0155: 08 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    wr_mem(DP|0x12, res); 
    clockticks = clockticks + 2; 
    PC = 0x0157; 
    JUMP; // sstep
  case 0x0157:
                                                     #ifdef DEBUG
                                      mon("         ASL   $1234                   ; 0157: 78 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    wr_mem(0x1234, res); 
    clockticks = clockticks + 2; 
    PC = 0x015A; 
    JUMP; // sstep
  case 0x015A:
                                                     #ifdef DEBUG
                                      mon("         ASL   [$1234]                 ; 015A: 68 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    wr_mem(ea, res); 
    clockticks = clockticks + 2; 
    PC = 0x015E; 
    JUMP; // sstep
  case 0x015E:
                                                     #ifdef DEBUG
                                      mon("         ASR   <$12                    ; 015E: 07 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
/**/C = val & 1; 
    val = (UINT8)((SINT8)val >> 1); 
    wr_mem(DP|0x12, val); 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x0160; 
    JUMP; // sstep
  case 0x0160:
                                                     #ifdef DEBUG
                                      mon("         ASR   $1234                   ; 0160: 77 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
/**/C = val & 1; 
    val = (UINT8)((SINT8)val >> 1); 
    wr_mem(0x1234, val); 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x0163; 
    JUMP; // sstep
  case 0x0163:
                                                     #ifdef DEBUG
                                      mon("         ASR   [$1234]                 ; 0163: 67 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
/**/C = val & 1; 
    val = (UINT8)((SINT8)val >> 1); 
    wr_mem(ea, val); 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x0167; 
    JUMP; // sstep
  case 0x0167:
                                                     #ifdef DEBUG
                                      mon("         BITA  #$12                    ; 0167: 85 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
/**/res = A & (SINT8)0x12; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0169; 
    JUMP; // sstep
  case 0x0169:
                                                     #ifdef DEBUG
                                      mon("         BITA  <$12                    ; 0169: 95 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
/**/res = A & rd_mem(DP|0x12); 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x016B; 
    JUMP; // sstep
  case 0x016B:
                                                     #ifdef DEBUG
                                      mon("         BITA  $1234                   ; 016B: B5 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/res = A & rd_mem(0x1234); 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x016E; 
    JUMP; // sstep
  case 0x016E:
                                                     #ifdef DEBUG
                                      mon("         BITA  [$1234]                 ; 016E: A5 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/res = A & rd_mem(ea); 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0172; 
    JUMP; // sstep
  case 0x0172:
                                                     #ifdef DEBUG
                                      mon("         BITB  #$12                    ; 0172: C5 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
/**/res = B & (SINT8)0x12; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0174; 
    JUMP; // sstep
  case 0x0174:
                                                     #ifdef DEBUG
                                      mon("         BITB  <$12                    ; 0174: D5 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
/**/res = B & rd_mem(DP|0x12); 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0176; 
    JUMP; // sstep
  case 0x0176:
                                                     #ifdef DEBUG
                                      mon("         BITB  $1234                   ; 0176: F5 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/res = B & rd_mem(0x1234); 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0179; 
    JUMP; // sstep
  case 0x0179:
                                                     #ifdef DEBUG
                                      mon("         BITB  [$1234]                 ; 0179: E5 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/res = B & rd_mem(ea); 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x017D; 
    JUMP; // sstep
  case 0x017D:
                                                     #ifdef DEBUG
                                      mon("         CLR   <$12                    ; 017D: 0F 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    wr_mem(DP|0x12, 0); 
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    clockticks = clockticks + 2; 
    PC = 0x017F; 
    JUMP; // sstep
  case 0x017F:
                                                     #ifdef DEBUG
                                      mon("         CLR   $1234                   ; 017F: 7F 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    wr_mem(0x1234, 0); 
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    clockticks = clockticks + 2; 
    PC = 0x0182; 
    JUMP; // sstep
  case 0x0182:
                                                     #ifdef DEBUG
                                      mon("         CLR   [$1234]                 ; 0182: 6F 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    wr_mem(ea, 0); 
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    clockticks = clockticks + 2; 
    PC = 0x0186; 
    JUMP; // sstep
  case 0x0186:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$12                    ; 0186: 81 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
/**/val = (UINT8)A; 
/**/arg = 0x12; 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x0188; 
    JUMP; // sstep
  case 0x0188:
                                                     #ifdef DEBUG
                                      mon("         CMPA  <$12                    ; 0188: 91 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
/**/val = (UINT8)A; 
/**/arg = rd_mem(DP|0x12); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x018A; 
    JUMP; // sstep
  case 0x018A:
                                                     #ifdef DEBUG
                                      mon("         CMPA  $1234                   ; 018A: B1 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/val = (UINT8)A; 
/**/arg = rd_mem(0x1234); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x018D; 
    JUMP; // sstep
  case 0x018D:
                                                     #ifdef DEBUG
                                      mon("         CMPA  [$1234]                 ; 018D: A1 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/val = (UINT8)A; 
/**/arg = rd_mem(ea); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x0191; 
    JUMP; // sstep
  case 0x0191:
                                                     #ifdef DEBUG
                                      mon("         CMPB  #$12                    ; 0191: C1 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
/**/val = (UINT8)B; 
/**/arg = 0x12; 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x0193; 
    JUMP; // sstep
  case 0x0193:
                                                     #ifdef DEBUG
                                      mon("         CMPB  <$12                    ; 0193: D1 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
/**/val = (UINT8)B; 
/**/arg = rd_mem(DP|0x12); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x0195; 
    JUMP; // sstep
  case 0x0195:
                                                     #ifdef DEBUG
                                      mon("         CMPB  $1234                   ; 0195: F1 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/val = (UINT8)B; 
/**/arg = rd_mem(0x1234); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x0198; 
    JUMP; // sstep
  case 0x0198:
                                                     #ifdef DEBUG
                                      mon("         CMPB  [$1234]                 ; 0198: E1 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/val = (UINT8)B; 
/**/arg = rd_mem(ea); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x019C; 
    JUMP; // sstep
  case 0x019C:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$1234                  ; 019C: 10 83 12 34   \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/val = (((UINT8)A<<8)|(UINT8)B); 
/**/arg = 0x1234; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01A0; 
    JUMP; // sstep
  case 0x01A0:
                                                     #ifdef DEBUG
                                      mon("         CMPD  <$12                    ; 01A0: 10 93 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
/**/val = (((UINT8)A<<8)|(UINT8)B); 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    clockticks = clockticks + 1; 
    PC = 0x01A3; 
    JUMP; // sstep
  case 0x01A3:
                                                     #ifdef DEBUG
                                      mon("         CMPD  $1234                   ; 01A3: 10 B3 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/val = (((UINT8)A<<8)|(UINT8)B); 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    clockticks = clockticks + 1; 
    PC = 0x01A7; 
    JUMP; // sstep
  case 0x01A7:
                                                     #ifdef DEBUG
                                      mon("         CMPD  [$1234]                 ; 01A7: 10 A3 9F 12 34\n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
/**/val = (((UINT8)A<<8)|(UINT8)B); 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    clockticks = clockticks + 1; 
    PC = 0x01AC; 
    JUMP; // sstep
  case 0x01AC:
                                                     #ifdef DEBUG
                                      mon("         CMPS  #$1234                  ; 01AC: 11 8C 12 34   \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/val = S; 
/**/arg = 0x1234; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01B0; 
    JUMP; // sstep
  case 0x01B0:
                                                     #ifdef DEBUG
                                      mon("         CMPS  <$12                    ; 01B0: 11 9C 12      \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
/**/val = S; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01B3; 
    JUMP; // sstep
  case 0x01B3:
                                                     #ifdef DEBUG
                                      mon("         CMPS  $1234                   ; 01B3: 11 BC 12 34   \n");
                                      #endif
    clockticks = clockticks + 7; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/val = S; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01B7; 
    JUMP; // sstep
  case 0x01B7:
                                                     #ifdef DEBUG
                                      mon("         CMPS  [$1234]                 ; 01B7: 11 AC 9F 12 34\n");
                                      #endif
    clockticks = clockticks + 2; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
/**/val = S; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01BC; 
    JUMP; // sstep
  case 0x01BC:
                                                     #ifdef DEBUG
                                      mon("         CMPU  #$1234                  ; 01BC: 11 83 12 34   \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/val = U; 
/**/arg = 0x1234; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01C0; 
    JUMP; // sstep
  case 0x01C0:
                                                     #ifdef DEBUG
                                      mon("         CMPU  <$12                    ; 01C0: 11 93 12      \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
/**/val = U; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01C3; 
    JUMP; // sstep
  case 0x01C3:
                                                     #ifdef DEBUG
                                      mon("         CMPU  $1234                   ; 01C3: 11 B3 12 34   \n");
                                      #endif
    clockticks = clockticks + 7; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/val = U; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01C7; 
    JUMP; // sstep
  case 0x01C7:
                                                     #ifdef DEBUG
                                      mon("         CMPU  [$1234]                 ; 01C7: 11 A3 9F 12 34\n");
                                      #endif
    clockticks = clockticks + 2; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
/**/val = U; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01CC; 
    JUMP; // sstep
  case 0x01CC:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$1234                  ; 01CC: 8C 12 34      \n");
                                      #endif
    clockticks = clockticks + 4; 
/**/val = X; 
/**/arg = 0x1234; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01CF; 
    JUMP; // sstep
  case 0x01CF:
                                                     #ifdef DEBUG
                                      mon("         CPX   <$12                    ; 01CF: 9C 12         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
/**/val = X; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01D1; 
    JUMP; // sstep
  case 0x01D1:
                                                     #ifdef DEBUG
                                      mon("         CPX   $1234                   ; 01D1: BC 12 34      \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/val = X; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01D4; 
    JUMP; // sstep
  case 0x01D4:
                                                     #ifdef DEBUG
                                      mon("         CPX   [$1234]                 ; 01D4: AC 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
/**/val = X; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01D8; 
    JUMP; // sstep
  case 0x01D8:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$1234                  ; 01D8: 10 8C 12 34   \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/val = Y; 
/**/arg = 0x1234; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x01DC; 
    JUMP; // sstep
  case 0x01DC:
                                                     #ifdef DEBUG
                                      mon("         CMPY  <$12                    ; 01DC: 10 9C 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
/**/val = Y; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    clockticks = clockticks + 1; 
    PC = 0x01DF; 
    JUMP; // sstep
  case 0x01DF:
                                                     #ifdef DEBUG
                                      mon("         CMPY  $1234                   ; 01DF: 10 BC 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/val = Y; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    clockticks = clockticks + 1; 
    PC = 0x01E3; 
    JUMP; // sstep
  case 0x01E3:
                                                     #ifdef DEBUG
                                      mon("         CMPY  [$1234]                 ; 01E3: 10 AC 9F 12 34\n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
/**/val = Y; 
/**/arg = ea; 
/**/res = val - arg; 
/**/C = res & 0x10000; 
/**/res = (UINT16)res; 
/**/Z = res; 
/**/N = res >> 8; 
/**/V = ((val ^ arg) & (val ^ res)) >> 8; 
    clockticks = clockticks + 1; 
    PC = 0x01E8; 
    JUMP; // sstep
  case 0x01E8:
                                                     #ifdef DEBUG
                                      mon("         COM   <$12                    ; 01E8: 03 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12) ^ 0xff; 
    wr_mem(DP|0x12, val); 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
/**/C = 1; 
    clockticks = clockticks + 2; 
    PC = 0x01EA; 
    JUMP; // sstep
  case 0x01EA:
                                                     #ifdef DEBUG
                                      mon("         COM   $1234                   ; 01EA: 73 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234) ^ 0xff; 
    wr_mem(0x1234, val); 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
/**/C = 1; 
    clockticks = clockticks + 2; 
    PC = 0x01ED; 
    JUMP; // sstep
  case 0x01ED:
                                                     #ifdef DEBUG
                                      mon("         COM   [$1234]                 ; 01ED: 63 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea) ^ 0xff; 
    wr_mem(ea, val); 
    Z = val; 
    N = val; 
    V = 0; 
    C = 1; 
    clockticks = clockticks + 2; 
    PC = 0x01F1; 
    JUMP; // sstep
  case 0x01F1:
                                                     #ifdef DEBUG
                                      mon("         CWAI  #$FF                    ; 01F1: 3C FF         \n");
                                      #endif
    // CWAI - not suported yet!
    PC = 0x01F3; 
    JUMP; // sstep
  case 0x01F3:
                                                     #ifdef DEBUG
                                      mon("L01F3    DEC   <$12                    ; 01F3: 0A 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
    res = (UINT8)(val - 1); 
/**/N = res; 
/**/Z = res; 
/**/V = val & ~res; 
    wr_mem(DP|0x12, res); 
    clockticks = clockticks + 2; 
    PC = 0x01F5; 
    JUMP; // sstep
  case 0x01F5:
                                                     #ifdef DEBUG
                                      mon("         DEC   $1234                   ; 01F5: 7A 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
    res = (UINT8)(val - 1); 
/**/N = res; 
/**/Z = res; 
/**/V = val & ~res; 
    wr_mem(0x1234, res); 
    clockticks = clockticks + 2; 
    PC = 0x01F8; 
    JUMP; // sstep
  case 0x01F8:
                                                     #ifdef DEBUG
                                      mon("         DEC   [$1234]                 ; 01F8: 6A 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
    res = (UINT8)(val - 1); 
/**/N = res; 
/**/Z = res; 
/**/V = val & ~res; 
    wr_mem(ea, res); 
    clockticks = clockticks + 2; 
    PC = 0x01FC; 
    JUMP; // sstep
  case 0x01FC:
                                                     #ifdef DEBUG
                                      mon("         EORA  #$12                    ; 01FC: 88 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A ^ (SINT8)0x12; 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x01FE; 
    JUMP; // sstep
  case 0x01FE:
                                                     #ifdef DEBUG
                                      mon("         EORA  <$12                    ; 01FE: 98 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    res = A ^ rd_mem(DP|0x12); 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0200; 
    JUMP; // sstep
  case 0x0200:
                                                     #ifdef DEBUG
                                      mon("         EORA  $1234                   ; 0200: B8 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    res = A ^ rd_mem(0x1234); 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0203; 
    JUMP; // sstep
  case 0x0203:
                                                     #ifdef DEBUG
                                      mon("         EORA  [$1234]                 ; 0203: A8 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    res = A ^ rd_mem(ea); 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0207; 
    JUMP; // sstep
  case 0x0207:
                                                     #ifdef DEBUG
                                      mon("         EORB  #$12                    ; 0207: C8 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = B ^ (SINT8)0x12; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0209; 
    JUMP; // sstep
  case 0x0209:
                                                     #ifdef DEBUG
                                      mon("         EORB  <$12                    ; 0209: D8 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    res = B ^ rd_mem(DP|0x12); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x020B; 
    JUMP; // sstep
  case 0x020B:
                                                     #ifdef DEBUG
                                      mon("         EORB  $1234                   ; 020B: F8 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    res = B ^ rd_mem(0x1234); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x020E; 
    JUMP; // sstep
  case 0x020E:
                                                     #ifdef DEBUG
                                      mon("         EORB  [$1234]                 ; 020E: E8 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    res = B ^ rd_mem(ea); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x0212; 
    JUMP; // sstep
  case 0x0212:
                                                     #ifdef DEBUG
                                      mon("         INC   <$12                    ; 0212: 0C 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
    res = (UINT8)(val + 1); 
/**/Z = res; 
/**/N = res; 
/**/V = res & ~val; 
    wr_mem(DP|0x12, res); 
    clockticks = clockticks + 2; 
    PC = 0x0214; 
    JUMP; // sstep
  case 0x0214:
                                                     #ifdef DEBUG
                                      mon("         INC   $1234                   ; 0214: 7C 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
    res = (UINT8)(val + 1); 
/**/Z = res; 
/**/N = res; 
/**/V = res & ~val; 
    wr_mem(0x1234, res); 
    clockticks = clockticks + 2; 
    PC = 0x0217; 
    JUMP; // sstep
  case 0x0217:
                                                     #ifdef DEBUG
                                      mon("         INC   [$1234]                 ; 0217: 6C 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
    res = (UINT8)(val + 1); 
    Z = res; 
    N = res; 
    V = res & ~val; 
    wr_mem(ea, res); 
    clockticks = clockticks + 2; 
    PC = 0x021B; 
    JUMP; // sstep
  case 0x021B:
                                                     #ifdef DEBUG
                                      mon("         JMP   <$12                    ; 021B: 0E 12         \n");
                                      #endif
    clockticks = clockticks + 3; 
    PC = DP|0x12; 
    JUMP;
    
  case 0x021D:
                                                     #ifdef DEBUG
                                      mon("         JMP   $1234                   ; 021D: 7E 12 34      \n");
                                      #endif
    clockticks = clockticks + 4; 
    PC = 0x1234; 
    JUMP;
    
  case 0x0220:
                                                     #ifdef DEBUG
                                      mon("         JMP   [$1234]                 ; 0220: 6E 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    PC = ea; 
    JUMP;
    
  case 0x0224:
                                                     #ifdef DEBUG
                                      mon("         JSR   <$12                    ; 0224: 9D 12         \n");
                                      #endif
    clockticks = clockticks + 7; 
    --S;
    wr_mem(S, 0x26); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x02); 
    clockticks = clockticks + 1; 
    PC = DP|0x12; 
    JUMP;
  case 0x0226:
                                                     #ifdef DEBUG
                                      mon("L0226    JSR   $1234                   ; 0226: BD 12 34      \n");
                                      #endif
    clockticks = clockticks + 8; 
    --S;
    wr_mem(S, 0x29); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x02); 
    clockticks = clockticks + 1; 
    PC = 0x1234; 
    JUMP;
  case 0x0229:
                                                     #ifdef DEBUG
                                      mon("L0229    JSR   [$1234]                 ; 0229: AD 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 3; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    --S;
    wr_mem(S, 0x2d); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x02); 
    clockticks = clockticks + 1; 
    PC = ea; 
    JUMP;
  case 0x022D:
                                                     #ifdef DEBUG
                                      mon("L022D    LDA   #$12                    ; 022D: 86 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x12; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x022F; 
    JUMP; // sstep
  case 0x022F:
                                                     #ifdef DEBUG
                                      mon("         LDA   <$12                    ; 022F: 96 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0231; 
    JUMP; // sstep
  case 0x0231:
                                                     #ifdef DEBUG
                                      mon("         LDA   $1234                   ; 0231: B6 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0234; 
    JUMP; // sstep
  case 0x0234:
                                                     #ifdef DEBUG
                                      mon("         LDA   [$1234]                 ; 0234: A6 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0238; 
    JUMP; // sstep
  case 0x0238:
                                                     #ifdef DEBUG
                                      mon("         LDB   #$12                    ; 0238: C6 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x12; 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x023A; 
    JUMP; // sstep
  case 0x023A:
                                                     #ifdef DEBUG
                                      mon("         LDB   <$12                    ; 023A: D6 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x023C; 
    JUMP; // sstep
  case 0x023C:
                                                     #ifdef DEBUG
                                      mon("         LDB   $1234                   ; 023C: F6 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x023F; 
    JUMP; // sstep
  case 0x023F:
                                                     #ifdef DEBUG
                                      mon("         LDB   [$1234]                 ; 023F: E6 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0243; 
    JUMP; // sstep
  case 0x0243:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$1234                  ; 0243: CC 12 34      \n");
                                      #endif
    clockticks = clockticks + 3; 
    A = 0x12; 
    B = 0x34; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x0246; 
    JUMP; // sstep
  case 0x0246:
                                                     #ifdef DEBUG
                                      mon("         LDD   <$12                    ; 0246: DC 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    A = rd_mem(DP|0x12); // Care needed with I/O space and word fetches
    B = rd_mem(DP|0x13); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x0248; 
    JUMP; // sstep
  case 0x0248:
                                                     #ifdef DEBUG
                                      mon("         LDD   $1234                   ; 0248: FC 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    A = rd_mem(0x1234); // Care needed with I/O space and word fetches
    B = rd_mem(0x1235); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x024B; 
    JUMP; // sstep
  case 0x024B:
                                                     #ifdef DEBUG
                                      mon("         LDD   [$1234]                 ; 024B: EC 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(ea+1)); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x024F; 
    JUMP; // sstep
  case 0x024F:
                                                     #ifdef DEBUG
                                      mon("         LDS   #$1234                  ; 024F: 10 CE 12 34   \n");
                                      #endif
    clockticks = clockticks + 4; 
    S = 0x1234; 
/**/Z = S; 
/**/N = (S) >> 8; 
/**/V = 0; 
    PC = 0x0253; 
    JUMP; // sstep
  case 0x0253:
                                                     #ifdef DEBUG
                                      mon("         LDS   <$12                    ; 0253: 10 DE 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
    S = ea; 
/**/Z = S; 
/**/N = (S) >> 8; 
/**/V = 0; 
    PC = 0x0256; 
    JUMP; // sstep
  case 0x0256:
                                                     #ifdef DEBUG
                                      mon("         LDS   $1234                   ; 0256: 10 FE 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    S = ea; 
/**/Z = S; 
/**/N = (S) >> 8; 
/**/V = 0; 
    PC = 0x025A; 
    JUMP; // sstep
  case 0x025A:
                                                     #ifdef DEBUG
                                      mon("         LDS   [$1234]                 ; 025A: 10 EE 9F 12 34\n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
    S = ea; 
/**/Z = S; 
/**/N = (S) >> 8; 
/**/V = 0; 
    PC = 0x025F; 
    JUMP; // sstep
  case 0x025F:
                                                     #ifdef DEBUG
                                      mon("         LDU   #$1234                  ; 025F: CE 12 34      \n");
                                      #endif
    clockticks = clockticks + 3; 
    U = 0x1234; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x0262; 
    JUMP; // sstep
  case 0x0262:
                                                     #ifdef DEBUG
                                      mon("         LDU   <$12                    ; 0262: DE 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
    U = ea; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x0264; 
    JUMP; // sstep
  case 0x0264:
                                                     #ifdef DEBUG
                                      mon("         LDU   $1234                   ; 0264: FE 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    U = ea; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x0267; 
    JUMP; // sstep
  case 0x0267:
                                                     #ifdef DEBUG
                                      mon("         LDU   [$1234]                 ; 0267: EE 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
    U = ea; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x026B; 
    JUMP; // sstep
  case 0x026B:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$1234                  ; 026B: 8E 12 34      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x1234; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x026E; 
    JUMP; // sstep
  case 0x026E:
                                                     #ifdef DEBUG
                                      mon("         LDX   <$12                    ; 026E: 9E 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
    X = ea; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x0270; 
    JUMP; // sstep
  case 0x0270:
                                                     #ifdef DEBUG
                                      mon("         LDX   $1234                   ; 0270: BE 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    X = ea; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x0273; 
    JUMP; // sstep
  case 0x0273:
                                                     #ifdef DEBUG
                                      mon("         LDX   [$1234]                 ; 0273: AE 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
    X = ea; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x0277; 
    JUMP; // sstep
  case 0x0277:
                                                     #ifdef DEBUG
                                      mon("         LDY   #$1234                  ; 0277: 10 8E 12 34   \n");
                                      #endif
    clockticks = clockticks + 4; 
    Y = 0x1234; 
/**/Z = Y; 
/**/N = (Y) >> 8; 
/**/V = 0; 
    PC = 0x027B; 
    JUMP; // sstep
  case 0x027B:
                                                     #ifdef DEBUG
                                      mon("         LDY   <$12                    ; 027B: 10 9E 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
    Y = ea; 
/**/Z = Y; 
/**/N = (Y) >> 8; 
/**/V = 0; 
    PC = 0x027E; 
    JUMP; // sstep
  case 0x027E:
                                                     #ifdef DEBUG
                                      mon("         LDY   $1234                   ; 027E: 10 BE 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    Y = ea; 
/**/Z = Y; 
/**/N = (Y) >> 8; 
/**/V = 0; 
    PC = 0x0282; 
    JUMP; // sstep
  case 0x0282:
                                                     #ifdef DEBUG
                                      mon("         LDY   [$1234]                 ; 0282: 10 AE 9F 12 34\n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
    Y = ea; 
/**/Z = Y; 
/**/N = (Y) >> 8; 
/**/V = 0; 
    PC = 0x0287; 
    JUMP; // sstep
  case 0x0287:
                                                     #ifdef DEBUG
                                      mon("         LEAS  ,S                      ; 0287: 32 E4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    S = S; 
    PC = 0x0289; 
    JUMP; // sstep
  case 0x0289:
                                                     #ifdef DEBUG
                                      mon("         LEAU  $01,U                   ; 0289: 33 41         \n");
                                      #endif
    clockticks = clockticks + 5; 
    U = (U + 0x0001); 
    PC = 0x028B; 
    JUMP; // sstep
  case 0x028B:
                                                     #ifdef DEBUG
                                      mon("         LEAX  -1,X                    ; 028B: 30 1F         \n");
                                      #endif
    clockticks = clockticks + 5; 
    X = (X + 0xffff); 
/**/Z = X; 
    PC = 0x028D; 
    JUMP; // sstep
  case 0x028D:
                                                     #ifdef DEBUG
                                      mon("         LEAY  $01,Y                   ; 028D: 31 21         \n");
                                      #endif
    clockticks = clockticks + 5; 
    Y = (Y + 0x0001); 
/**/Z = Y; 
    PC = 0x028F; 
    JUMP; // sstep
  case 0x028F:
                                                     #ifdef DEBUG
                                      mon("         ASL   <$12                    ; 028F: 08 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    wr_mem(DP|0x12, res); 
    clockticks = clockticks + 2; 
    PC = 0x0291; 
    JUMP; // sstep
  case 0x0291:
                                                     #ifdef DEBUG
                                      mon("         ASL   $1234                   ; 0291: 78 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    wr_mem(0x1234, res); 
    clockticks = clockticks + 2; 
    PC = 0x0294; 
    JUMP; // sstep
  case 0x0294:
                                                     #ifdef DEBUG
                                      mon("         ASL   [$1234]                 ; 0294: 68 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    wr_mem(ea, res); 
    clockticks = clockticks + 2; 
    PC = 0x0298; 
    JUMP; // sstep
  case 0x0298:
                                                     #ifdef DEBUG
                                      mon("         LSR   <$12                    ; 0298: 04 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
/**/C = val & 1; 
    val = val >> 1; 
    wr_mem(DP|0x12, val); 
/**/N = 0; 
/**/Z = val; 
    clockticks = clockticks + 2; 
    PC = 0x029A; 
    JUMP; // sstep
  case 0x029A:
                                                     #ifdef DEBUG
                                      mon("         LSR   $1234                   ; 029A: 74 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
/**/C = val & 1; 
    val = val >> 1; 
    wr_mem(0x1234, val); 
/**/N = 0; 
/**/Z = val; 
    clockticks = clockticks + 2; 
    PC = 0x029D; 
    JUMP; // sstep
  case 0x029D:
                                                     #ifdef DEBUG
                                      mon("         LSR   [$1234]                 ; 029D: 64 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
/**/C = val & 1; 
    val = val >> 1; 
    wr_mem(ea, val); 
/**/N = 0; 
/**/Z = val; 
    clockticks = clockticks + 2; 
    PC = 0x02A1; 
    JUMP; // sstep
  case 0x02A1:
                                                     #ifdef DEBUG
                                      mon("         NEG   <$12                    ; 02A1: 00 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
    res = -val; 
/**/V = val & (UINT8)res; 
/**/C = res & 0x100; 
    val = (UINT8)res; 
    wr_mem(DP|0x12, val); 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x02A3; 
    JUMP; // sstep
  case 0x02A3:
                                                     #ifdef DEBUG
                                      mon("         NEG   $1234                   ; 02A3: 70 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
    res = -val; 
/**/V = val & (UINT8)res; 
/**/C = res & 0x100; 
    val = (UINT8)res; 
    wr_mem(0x1234, val); 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x02A6; 
    JUMP; // sstep
  case 0x02A6:
                                                     #ifdef DEBUG
                                      mon("         NEG   [$1234]                 ; 02A6: 60 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
    res = -val; 
/**/V = val & (UINT8)res; 
    C = res & 0x100; 
    val = (UINT8)res; 
    wr_mem(ea, val); 
/**/Z = val; 
/**/N = val; 
    clockticks = clockticks + 2; 
    PC = 0x02AA; 
    JUMP; // sstep
  case 0x02AA:
                                                     #ifdef DEBUG
                                      mon("         ORA   #$12                    ; 02AA: 8A 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = A | (UINT8)0x12; 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x02AC; 
    JUMP; // sstep
  case 0x02AC:
                                                     #ifdef DEBUG
                                      mon("         ORA   <$12                    ; 02AC: 9A 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    res = A | (UINT8)rd_mem(DP|0x12); 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x02AE; 
    JUMP; // sstep
  case 0x02AE:
                                                     #ifdef DEBUG
                                      mon("         ORA   $1234                   ; 02AE: BA 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    res = A | (UINT8)rd_mem(0x1234); 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x02B1; 
    JUMP; // sstep
  case 0x02B1:
                                                     #ifdef DEBUG
                                      mon("         ORA   [$1234]                 ; 02B1: AA 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    res = A | (UINT8)rd_mem(ea); 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x02B5; 
    JUMP; // sstep
  case 0x02B5:
                                                     #ifdef DEBUG
                                      mon("         ORB   #$12                    ; 02B5: CA 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    res = B | (UINT8)0x12; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x02B7; 
    JUMP; // sstep
  case 0x02B7:
                                                     #ifdef DEBUG
                                      mon("         ORB   <$12                    ; 02B7: DA 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    res = B | (UINT8)rd_mem(DP|0x12); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x02B9; 
    JUMP; // sstep
  case 0x02B9:
                                                     #ifdef DEBUG
                                      mon("         ORB   $1234                   ; 02B9: FA 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    res = B | (UINT8)rd_mem(0x1234); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x02BC; 
    JUMP; // sstep
  case 0x02BC:
                                                     #ifdef DEBUG
                                      mon("         ORB   [$1234]                 ; 02BC: EA 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    res = B | (UINT8)rd_mem(ea); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x02C0; 
    JUMP; // sstep
  case 0x02C0:
                                                     #ifdef DEBUG
                                      mon("         ORCC  #$12                    ; 02C0: 1A 12         \n");
                                      #endif
    I = 1; 
    V = 0x80; 
    clockticks = clockticks + 3; 
    PC = 0x02C2; 
    JUMP; // sstep
  case 0x02C2:
                                                     #ifdef DEBUG
                                      mon("         ROL   <$12                    ; 02C2: 09 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
    res = (val << 1) + (C != 0 ?1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)res ^ val; 
    wr_mem(DP|0x12, (UINT8)res); 
    clockticks = clockticks + 2; 
    PC = 0x02C4; 
    JUMP; // sstep
  case 0x02C4:
                                                     #ifdef DEBUG
                                      mon("         ROL   $1234                   ; 02C4: 79 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
    res = (val << 1) + (C != 0 ?1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)res ^ val; 
    wr_mem(0x1234, (UINT8)res); 
    clockticks = clockticks + 2; 
    PC = 0x02C7; 
    JUMP; // sstep
  case 0x02C7:
                                                     #ifdef DEBUG
                                      mon("         ROL   [$1234]                 ; 02C7: 69 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
    res = (val << 1) + (C != 0 ?1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)res ^ val; 
    wr_mem(ea, (UINT8)res); 
    clockticks = clockticks + 2; 
    PC = 0x02CB; 
    JUMP; // sstep
  case 0x02CB:
                                                     #ifdef DEBUG
                                      mon("         ROR   <$12                    ; 02CB: 06 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    res = ((UINT8)rd_mem(DP|0x12)) | ((C != 0) ? 0x100 : 0); 
    C = res & 1; 
    res = (UINT8)(res >> 1); 
/**/Z = res; 
/**/N = res; 
    wr_mem(DP|0x12, res); 
    clockticks = clockticks + 2; 
    PC = 0x02CD; 
    JUMP; // sstep
  case 0x02CD:
                                                     #ifdef DEBUG
                                      mon("         ROR   $1234                   ; 02CD: 76 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    res = ((UINT8)rd_mem(0x1234)) | ((C != 0) ? 0x100 : 0); 
    C = res & 1; 
    res = (UINT8)(res >> 1); 
/**/Z = res; 
/**/N = res; 
    wr_mem(0x1234, res); 
    clockticks = clockticks + 2; 
    PC = 0x02D0; 
    JUMP; // sstep
  case 0x02D0:
                                                     #ifdef DEBUG
                                      mon("         ROR   [$1234]                 ; 02D0: 66 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    res = ((UINT8)rd_mem(ea)) | ((C != 0) ? 0x100 : 0); 
    C = res & 1; 
    res = (UINT8)(res >> 1); 
/**/Z = res; 
/**/N = res; 
    wr_mem(ea, res); 
    clockticks = clockticks + 2; 
    PC = 0x02D4; 
    JUMP; // sstep
  case 0x02D4:
                                                     #ifdef DEBUG
                                      mon("         SBCA  #$12                    ; 02D4: 82 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    arg = 0x12; 
    res = val - (UINT8)arg - (C != 0 ? 1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)res; 
    PC = 0x02D6; 
    JUMP; // sstep
  case 0x02D6:
                                                     #ifdef DEBUG
                                      mon("         SBCA  <$12                    ; 02D6: 92 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)A; 
    arg = rd_mem(DP|0x12); 
    res = val - (UINT8)arg - (C != 0 ? 1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)res; 
    PC = 0x02D8; 
    JUMP; // sstep
  case 0x02D8:
                                                     #ifdef DEBUG
                                      mon("         SBCA  $1234                   ; 02D8: B2 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem(0x1234); 
    res = val - (UINT8)arg - (C != 0 ? 1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)res; 
    PC = 0x02DB; 
    JUMP; // sstep
  case 0x02DB:
                                                     #ifdef DEBUG
                                      mon("         SBCA  [$1234]                 ; 02DB: A2 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val - (UINT8)arg - (C != 0 ? 1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)res; 
    PC = 0x02DF; 
    JUMP; // sstep
  case 0x02DF:
                                                     #ifdef DEBUG
                                      mon("         SBCB  #$12                    ; 02DF: C2 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    arg = 0x12; 
    res = val - (UINT8)arg - (C != 0 ? 1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    B = (UINT8)res; 
    PC = 0x02E1; 
    JUMP; // sstep
  case 0x02E1:
                                                     #ifdef DEBUG
                                      mon("         SBCB  <$12                    ; 02E1: D2 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)B; 
    arg = rd_mem(DP|0x12); 
    res = val - (UINT8)arg - (C != 0 ? 1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    B = (UINT8)res; 
    PC = 0x02E3; 
    JUMP; // sstep
  case 0x02E3:
                                                     #ifdef DEBUG
                                      mon("         SBCB  $1234                   ; 02E3: F2 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)B; 
    arg = rd_mem(0x1234); 
    res = val - (UINT8)arg - (C != 0 ? 1:0); 
    C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    B = (UINT8)res; 
    PC = 0x02E6; 
    JUMP; // sstep
  case 0x02E6:
                                                     #ifdef DEBUG
                                      mon("         SBCB  [$1234]                 ; 02E6: E2 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)B; 
    arg = rd_mem(ea); 
    res = val - (UINT8)arg - (C != 0 ? 1:0); 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    B = (UINT8)res; 
    PC = 0x02EA; 
    JUMP; // sstep
  case 0x02EA:
                                                     #ifdef DEBUG
                                      mon("         STA   <$12                    ; 02EA: 97 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    ea = DP|0x12; 
    wr_mem(ea, val); 
    PC = 0x02EC; 
    JUMP; // sstep
  case 0x02EC:
                                                     #ifdef DEBUG
                                      mon("         STA   $1234                   ; 02EC: B7 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x1234, val); 
    PC = 0x02EF; 
    JUMP; // sstep
  case 0x02EF:
                                                     #ifdef DEBUG
                                      mon("         STA   [$1234]                 ; 02EF: A7 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    ea = ea; 
    wr_mem(ea, val); 
    PC = 0x02F3; 
    JUMP; // sstep
  case 0x02F3:
                                                     #ifdef DEBUG
                                      mon("         STB   <$12                    ; 02F3: D7 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = B; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    ea = DP|0x12; 
    wr_mem(ea, val); 
    PC = 0x02F5; 
    JUMP; // sstep
  case 0x02F5:
                                                     #ifdef DEBUG
                                      mon("         STB   $1234                   ; 02F5: F7 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = B; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x1234, val); 
    PC = 0x02F8; 
    JUMP; // sstep
  case 0x02F8:
                                                     #ifdef DEBUG
                                      mon("         STB   [$1234]                 ; 02F8: E7 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = B; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    ea = ea; 
    wr_mem(ea, val); 
    PC = 0x02FC; 
    JUMP; // sstep
  case 0x02FC:
                                                     #ifdef DEBUG
                                      mon("         STD   <$12                    ; 02FC: DD 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    ea = DP|0x12; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    PC = 0x02FE; 
    JUMP; // sstep
  case 0x02FE:
                                                     #ifdef DEBUG
                                      mon("         STD   $1234                   ; 02FE: FD 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x1234, A); // Care needed with I/O space and word fetches
    wr_mem(0x1235, B); 
    PC = 0x0301; 
    JUMP; // sstep
  case 0x0301:
                                                     #ifdef DEBUG
                                      mon("         STD   [$1234]                 ; 0301: ED 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    ea = ea; 
    wr_mem(ea, A); // Care needed with I/O space and word fetches
    wr_mem(ea + 1, B); 
    PC = 0x0305; 
    JUMP; // sstep
  case 0x0305:
                                                     #ifdef DEBUG
                                      mon("         STS   <$12                    ; 0305: 10 DF 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = S; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    ea = DP|0x12; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
    PC = 0x0308; 
    JUMP; // sstep
  case 0x0308:
                                                     #ifdef DEBUG
                                      mon("         STS   $1234                   ; 0308: 10 FF 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = S; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    wr_mem(0x1234, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x1235, (UINT8)val); 
    PC = 0x030C; 
    JUMP; // sstep
  case 0x030C:
                                                     #ifdef DEBUG
                                      mon("         STS   [$1234]                 ; 030C: 10 EF 9F 12 34\n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = S; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    ea = ea; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
    PC = 0x0311; 
    JUMP; // sstep
  case 0x0311:
                                                     #ifdef DEBUG
                                      mon("         STU   <$12                    ; 0311: DF 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = U; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    ea = DP|0x12; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
    PC = 0x0313; 
    JUMP; // sstep
  case 0x0313:
                                                     #ifdef DEBUG
                                      mon("         STU   $1234                   ; 0313: FF 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = U; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    wr_mem(0x1234, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x1235, (UINT8)val); 
    PC = 0x0316; 
    JUMP; // sstep
  case 0x0316:
                                                     #ifdef DEBUG
                                      mon("         STU   [$1234]                 ; 0316: EF 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = U; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    ea = ea; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
    PC = 0x031A; 
    JUMP; // sstep
  case 0x031A:
                                                     #ifdef DEBUG
                                      mon("         STX   <$12                    ; 031A: 9F 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = X; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    ea = DP|0x12; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
    PC = 0x031C; 
    JUMP; // sstep
  case 0x031C:
                                                     #ifdef DEBUG
                                      mon("         STX   $1234                   ; 031C: BF 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = X; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    wr_mem(0x1234, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x1235, (UINT8)val); 
    PC = 0x031F; 
    JUMP; // sstep
  case 0x031F:
                                                     #ifdef DEBUG
                                      mon("         STX   [$1234]                 ; 031F: AF 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = X; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    ea = ea; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
    PC = 0x0323; 
    JUMP; // sstep
  case 0x0323:
                                                     #ifdef DEBUG
                                      mon("         STY   <$12                    ; 0323: 10 9F 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = Y; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    ea = DP|0x12; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
    PC = 0x0326; 
    JUMP; // sstep
  case 0x0326:
                                                     #ifdef DEBUG
                                      mon("         STY   $1234                   ; 0326: 10 BF 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = Y; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    wr_mem(0x1234, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem(0x1235, (UINT8)val); 
    PC = 0x032A; 
    JUMP; // sstep
  case 0x032A:
                                                     #ifdef DEBUG
                                      mon("         STY   [$1234]                 ; 032A: 10 AF 9F 12 34\n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = Y; 
/**/Z = val; 
/**/N = (UINT8)(val >> 8); 
/**/V = 0; 
    ea = ea; 
    wr_mem(ea, (UINT8)(val >> 8)); // Care needed with I/O space and word fetches
    wr_mem((UINT16)(ea + 1), (UINT8)val); 
    PC = 0x032F; 
    JUMP; // sstep
  case 0x032F:
                                                     #ifdef DEBUG
                                      mon("         SUBA  #$12                    ; 032F: 80 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)A; 
    arg = 0x12; 
    res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)(res); 
    PC = 0x0331; 
    JUMP; // sstep
  case 0x0331:
                                                     #ifdef DEBUG
                                      mon("         SUBA  <$12                    ; 0331: 90 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)A; 
    arg = rd_mem(DP|0x12); 
    res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)(res); 
    PC = 0x0333; 
    JUMP; // sstep
  case 0x0333:
                                                     #ifdef DEBUG
                                      mon("         SUBA  $1234                   ; 0333: B0 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem(0x1234); 
    res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)(res); 
    PC = 0x0336; 
    JUMP; // sstep
  case 0x0336:
                                                     #ifdef DEBUG
                                      mon("         SUBA  [$1234]                 ; 0336: A0 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)(res); 
    PC = 0x033A; 
    JUMP; // sstep
  case 0x033A:
                                                     #ifdef DEBUG
                                      mon("         SUBB  #$12                    ; 033A: C0 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)B; 
    arg = 0x12; 
    res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    B = (UINT8)(res); 
    PC = 0x033C; 
    JUMP; // sstep
  case 0x033C:
                                                     #ifdef DEBUG
                                      mon("         SUBB  <$12                    ; 033C: D0 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)B; 
    arg = rd_mem(DP|0x12); 
    res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    B = (UINT8)(res); 
    PC = 0x033E; 
    JUMP; // sstep
  case 0x033E:
                                                     #ifdef DEBUG
                                      mon("         SUBB  $1234                   ; 033E: F0 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)B; 
    arg = rd_mem(0x1234); 
    res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    B = (UINT8)(res); 
    PC = 0x0341; 
    JUMP; // sstep
  case 0x0341:
                                                     #ifdef DEBUG
                                      mon("         SUBB  [$1234]                 ; 0341: E0 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)B; 
    arg = rd_mem(ea); 
    res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    B = (UINT8)(res); 
    PC = 0x0345; 
    JUMP; // sstep
  case 0x0345:
                                                     #ifdef DEBUG
                                      mon("         SUBD  #$1234                  ; 0345: 83 12 34      \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = 0x1234; 
    arg = (A << 8) | B; 
    res = arg - val; 
/**/C = res & 0x10000; 
    res = (UINT16)res; 
/**/Z = res; 
/**/V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
/**/N = res >> 8; 
    B = (UINT8)res; 
    PC = 0x0348; 
    JUMP; // sstep
  case 0x0348:
                                                     #ifdef DEBUG
                                      mon("         SUBD  <$12                    ; 0348: 93 12         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    ea = rd_mem(DP|0x12)<<8; 
    ea = ea | rd_mem(DP|0x13); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
/**/C = res & 0x10000; 
    res = (UINT16)res; 
/**/Z = res; 
/**/V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
/**/N = res >> 8; 
    B = (UINT8)res; 
    PC = 0x034A; 
    JUMP; // sstep
  case 0x034A:
                                                     #ifdef DEBUG
                                      mon("         SUBD  $1234                   ; 034A: B3 12 34      \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
/**/C = res & 0x10000; 
    res = (UINT16)res; 
/**/Z = res; 
/**/V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
/**/N = res >> 8; 
    B = (UINT8)res; 
    PC = 0x034D; 
    JUMP; // sstep
  case 0x034D:
                                                     #ifdef DEBUG
                                      mon("         SUBD  [$1234]                 ; 034D: A3 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 1; 
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    clockticks = clockticks + 1; 
    ea = rd_mem(ea)<<8; 
    ea = ea | rd_mem((UINT16)((ea)+1)); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg - val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
/**/Z = res; 
/**/V = ((arg ^ val) & (arg ^ res)) >> 8; 
    A = (UINT8)(res >> 8); 
/**/N = res >> 8; 
    B = (UINT8)res; 
    PC = 0x0351; 
    JUMP; // sstep
  case 0x0351:
                                                     #ifdef DEBUG
                                      mon("         TST   <$12                    ; 0351: 0D 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
/**/val = (UINT8)rd_mem(DP|0x12); 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    clockticks = clockticks + 2; 
    PC = 0x0353; 
    JUMP; // sstep
  case 0x0353:
                                                     #ifdef DEBUG
                                      mon("         TST   $1234                   ; 0353: 7D 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
/**/val = (UINT8)rd_mem(0x1234); 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    clockticks = clockticks + 2; 
    PC = 0x0356; 
    JUMP; // sstep
  case 0x0356:
                                                     #ifdef DEBUG
                                      mon("         TST   [$1234]                 ; 0356: 6D 9F 12 34   \n");
                                      #endif
    clockticks = clockticks + 6; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(0x1234)<<8; 
    ea = ea | rd_mem(0x1235); 
    val = (UINT8)rd_mem(ea); 
    Z = val; 
    N = val; 
    V = 0; 
    clockticks = clockticks + 2; 
    PC = 0x035A; 
    JUMP; // sstep
  case 0x035A:
                                                     #ifdef DEBUG
                                      mon("         EXG   A,B                     ; 035A: 1E 89         \n");
                                      #endif
    val = A; 
    A = B; 
    B = val; 
    clockticks = clockticks + 8; 
    PC = 0x035C; 
    JUMP; // sstep
  case 0x035C:
                                                     #ifdef DEBUG
                                      mon("         EXG   X,Y                     ; 035C: 1E 12         \n");
                                      #endif
    val = X; 
    X = Y; 
    Y = val; 
    clockticks = clockticks + 8; 
    PC = 0x035E; 
    JUMP; // sstep
  case 0x035E:
                                                     #ifdef DEBUG
                                      mon("         TFR   A,B                     ; 035E: 1F 89         \n");
                                      #endif
    B = (UINT8)A; 
    clockticks = clockticks + 6; 
    PC = 0x0360; 
    JUMP; // sstep
  case 0x0360:
                                                     #ifdef DEBUG
                                      mon("         TFR   S,U                     ; 0360: 1F 43         \n");
                                      #endif
    U = S; 
    clockticks = clockticks + 6; 
    PC = 0x0362; 
    JUMP; // sstep
  case 0x0362:
                                                     #ifdef DEBUG
                                      mon("         PSHS  PC,U,Y,X,DP,B,A,CC      ; 0362: 34 FF         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    --S;
    wr_mem(S, 0x64); 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, 0x03); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 2; 
    --S;
    wr_mem(S, U); 
    --S;
    wr_mem(S, U >> 8); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 2; 
    --S;
    wr_mem(S, Y); 
    --S;
    wr_mem(S, Y >> 8); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 2; 
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, (DP >> 8)); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, B); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    --S;
    wr_mem(S, A); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    clockticks = clockticks + 1; 
    PC = 0x0364; 
    JUMP; // sstep
  case 0x0364:
                                                     #ifdef DEBUG
                                      mon("         PSHU  PC,S,Y,X,DP,B,A,CC      ; 0364: 36 FF         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    --U;
    wr_mem(U, 0x66); 
    clockticks = clockticks + 1; 
    --U;
    wr_mem(U, 0x03); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 2; 
    --U;
    wr_mem(U, S); 
    --U;
    wr_mem(U, S >> 8); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 2; 
    --U;
    wr_mem(U, Y); 
    --U;
    wr_mem(U, Y >> 8); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 2; 
    --U;
    wr_mem(U, X); 
    --U;
    wr_mem(U, X >> 8); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    --U;
    wr_mem(U, (DP >> 8)); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    --U;
    wr_mem(U, B); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    --U;
    wr_mem(U, A); 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --U;
    wr_mem(U, CC); 
    clockticks = clockticks + 1; 
    PC = 0x0366; 
    JUMP; // sstep
  case 0x0366:
                                                     #ifdef DEBUG
                                      mon("         PULS  CC,A,B,DP,X,Y,U,PC      ; 0366: 35 FF         \n");
                                      #endif    // (PUL? PC=RTS)
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    CC = rd_mem(S); 
    S++;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    A = rd_mem(S); 
    S++;
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    B = rd_mem(S); 
    S++;
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    DP = rd_mem(S); 
    S++;
    DP = DP << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    X = rd_mem(S)<<8; 
    S++;
    X = X | rd_mem(S); 
    S++;
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    Y = rd_mem(S)<<8; 
    S++;
    Y = Y | rd_mem(S); 
    S++;
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    U = rd_mem(S)<<8; 
    S++;
    U = U | rd_mem(S); 
    S++;
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x0368:
                                                     #ifdef DEBUG
                                      mon("         PULU  CC,A,B,DP,X,Y,S,PC      ; 0368: 37 FF         \n");
                                      #endif    // (PUL? PC=RTS)
    clockticks = clockticks + 5; 
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    CC = rd_mem(U); 
    U++;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    A = rd_mem(U); 
    U++;
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    B = rd_mem(U); 
    U++;
    clockticks = clockticks + 1; 
    clockticks = clockticks + 1; 
    DP = rd_mem(U); 
    U++;
    DP = DP << 8; checkDP(); 
    // memory_DP = &memory[DP];
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    X = rd_mem(U)<<8; 
    U++;
    X = X | rd_mem(U); 
    U++;
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    Y = rd_mem(U)<<8; 
    U++;
    Y = Y | rd_mem(U); 
    U++;
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    S = rd_mem(U)<<8; 
    U++;
    S = S | rd_mem(U); 
    U++;
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    PC = rd_mem(U)<<8; 
    U++;
    PC = PC | rd_mem(U); 
    U++;
    JUMP;
    
  case 0x036A:
                                                     #ifdef DEBUG
                                      mon("         LDA   <$12                    ; 036A: 96 12         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x12); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x036C; 
    JUMP; // sstep
  case 0x036C:
                                                     #ifdef DEBUG
                                      mon("         LDA   $1234                   ; 036C: B6 12 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x1234); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x036F; 
    JUMP; // sstep
  case 0x036F:
                                                     #ifdef DEBUG
                                      mon("         LDA   $12                     ; 036F: B6 00 12      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x0012); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0372; 
    JUMP; // sstep
  case 0x0372:
                                                     #ifdef DEBUG
                                      mon("         LDA   <$34                    ; 0372: 96 34         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)rd_mem(DP|0x34); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0374; 
    JUMP; // sstep
  case 0x0374:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,X                      ; 0374: AB 84         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0376; 
    JUMP; // sstep
  case 0x0376:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,X+                     ; 0376: AB 80         \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    X = X + 1; 
    PC = 0x0378; 
    JUMP; // sstep
  case 0x0378:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,X++                    ; 0378: AB 81         \n");
                                      #endif
    clockticks = clockticks + 7; 
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    X = X + 2; 
    PC = 0x037A; 
    JUMP; // sstep
  case 0x037A:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,-X                     ; 037A: AB 82         \n");
                                      #endif
    clockticks = clockticks + 6; 
    X = X - 1; 
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x037C; 
    JUMP; // sstep
  case 0x037C:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,--X                    ; 037C: AB 83         \n");
                                      #endif
    clockticks = clockticks + 7; 
    X = X - 2; 
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x037E; 
    JUMP; // sstep
  case 0x037E:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $09,X                   ; 037E: AB 09         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(X + 0x0009)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0380; 
    JUMP; // sstep
  case 0x0380:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -9,X                    ; 0380: AB 17         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(X + 0xfff7)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0382; 
    JUMP; // sstep
  case 0x0382:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $78,X                   ; 0382: AB 88 78      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(X + (SINT8)0x78)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0385; 
    JUMP; // sstep
  case 0x0385:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -$78,X                  ; 0385: AB 88 88      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(X + (SINT8)0x88)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0388; 
    JUMP; // sstep
  case 0x0388:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $1234,X                 ; 0388: AB 89 12 34   \n");
                                      #endif
    clockticks = clockticks + 8; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(X + 0x1234)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x038C; 
    JUMP; // sstep
  case 0x038C:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -$1234,X                ; 038C: AB 89 ED CC   \n");
                                      #endif
    clockticks = clockticks + 8; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(X + 0xedcc)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0390; 
    JUMP; // sstep
  case 0x0390:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,Y                      ; 0390: AB A4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)A; 
    arg = rd_mem(Y); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0392; 
    JUMP; // sstep
  case 0x0392:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,Y+                     ; 0392: AB A0         \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = (UINT8)A; 
    arg = rd_mem(Y); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    Y = Y + 1; 
    PC = 0x0394; 
    JUMP; // sstep
  case 0x0394:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,Y++                    ; 0394: AB A1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    val = (UINT8)A; 
    arg = rd_mem(Y); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    Y = Y + 2; 
    PC = 0x0396; 
    JUMP; // sstep
  case 0x0396:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,-Y                     ; 0396: AB A2         \n");
                                      #endif
    clockticks = clockticks + 6; 
    Y = Y - 1; 
    val = (UINT8)A; 
    arg = rd_mem(Y); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0398; 
    JUMP; // sstep
  case 0x0398:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,--Y                    ; 0398: AB A3         \n");
                                      #endif
    clockticks = clockticks + 7; 
    Y = Y - 2; 
    val = (UINT8)A; 
    arg = rd_mem(Y); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x039A; 
    JUMP; // sstep
  case 0x039A:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $09,Y                   ; 039A: AB 29         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(Y + 0x0009)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x039C; 
    JUMP; // sstep
  case 0x039C:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -9,Y                    ; 039C: AB 37         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(Y + 0xfff7)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x039E; 
    JUMP; // sstep
  case 0x039E:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $78,Y                   ; 039E: AB A8 78      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(Y + (SINT8)0x78)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03A1; 
    JUMP; // sstep
  case 0x03A1:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -$78,Y                  ; 03A1: AB A8 88      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(Y + (SINT8)0x88)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03A4; 
    JUMP; // sstep
  case 0x03A4:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $1234,Y                 ; 03A4: AB A9 12 34   \n");
                                      #endif
    clockticks = clockticks + 8; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(Y + 0x1234)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03A8; 
    JUMP; // sstep
  case 0x03A8:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -$1234,Y                ; 03A8: AB A9 ED CC   \n");
                                      #endif
    clockticks = clockticks + 8; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(Y + 0xedcc)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03AC; 
    JUMP; // sstep
  case 0x03AC:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,U                      ; 03AC: AB C4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)A; 
    arg = rd_mem(U); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03AE; 
    JUMP; // sstep
  case 0x03AE:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,U+                     ; 03AE: AB C0         \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = (UINT8)A; 
    arg = rd_mem(U); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    U = U + 1; 
    PC = 0x03B0; 
    JUMP; // sstep
  case 0x03B0:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,U++                    ; 03B0: AB C1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    val = (UINT8)A; 
    arg = rd_mem(U); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    U = U + 2; 
    PC = 0x03B2; 
    JUMP; // sstep
  case 0x03B2:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,-U                     ; 03B2: AB C2         \n");
                                      #endif
    clockticks = clockticks + 6; 
    U = U - 1; 
    val = (UINT8)A; 
    arg = rd_mem(U); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03B4; 
    JUMP; // sstep
  case 0x03B4:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,--U                    ; 03B4: AB C3         \n");
                                      #endif
    clockticks = clockticks + 7; 
    U = U - 2; 
    val = (UINT8)A; 
    arg = rd_mem(U); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03B6; 
    JUMP; // sstep
  case 0x03B6:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $09,U                   ; 03B6: AB 49         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(U + 0x0009)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03B8; 
    JUMP; // sstep
  case 0x03B8:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -9,U                    ; 03B8: AB 57         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(U + 0xfff7)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03BA; 
    JUMP; // sstep
  case 0x03BA:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $78,U                   ; 03BA: AB C8 78      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(U + (SINT8)0x78)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03BD; 
    JUMP; // sstep
  case 0x03BD:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -$78,U                  ; 03BD: AB C8 88      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(U + (SINT8)0x88)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03C0; 
    JUMP; // sstep
  case 0x03C0:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $1234,U                 ; 03C0: AB C9 12 34   \n");
                                      #endif
    clockticks = clockticks + 8; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(U + 0x1234)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03C4; 
    JUMP; // sstep
  case 0x03C4:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -$1234,U                ; 03C4: AB C9 ED CC   \n");
                                      #endif
    clockticks = clockticks + 8; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(U + 0xedcc)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03C8; 
    JUMP; // sstep
  case 0x03C8:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,S                      ; 03C8: AB E4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    val = (UINT8)A; 
    arg = rd_mem(S); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03CA; 
    JUMP; // sstep
  case 0x03CA:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,S+                     ; 03CA: AB E0         \n");
                                      #endif
    clockticks = clockticks + 6; 
    val = (UINT8)A; 
    arg = rd_mem(S); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    S = S + 1; 
    PC = 0x03CC; 
    JUMP; // sstep
  case 0x03CC:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,S++                    ; 03CC: AB E1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    val = (UINT8)A; 
    arg = rd_mem(S); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    S = S + 2; 
    PC = 0x03CE; 
    JUMP; // sstep
  case 0x03CE:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,-S                     ; 03CE: AB E2         \n");
                                      #endif
    clockticks = clockticks + 6; 
    S = S - 1; 
    val = (UINT8)A; 
    arg = rd_mem(S); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03D0; 
    JUMP; // sstep
  case 0x03D0:
                                                     #ifdef DEBUG
                                      mon("         ADDA  ,--S                    ; 03D0: AB E3         \n");
                                      #endif
    clockticks = clockticks + 7; 
    S = S - 2; 
    val = (UINT8)A; 
    arg = rd_mem(S); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03D2; 
    JUMP; // sstep
  case 0x03D2:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $09,S                   ; 03D2: AB 69         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(S + 0x0009)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03D4; 
    JUMP; // sstep
  case 0x03D4:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -9,S                    ; 03D4: AB 77         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(S + 0xfff7)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03D6; 
    JUMP; // sstep
  case 0x03D6:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $78,S                   ; 03D6: AB E8 78      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(S + (SINT8)0x78)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03D9; 
    JUMP; // sstep
  case 0x03D9:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -$78,S                  ; 03D9: AB E8 88      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(S + (SINT8)0x88)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03DC; 
    JUMP; // sstep
  case 0x03DC:
                                                     #ifdef DEBUG
                                      mon("         ADDA  $1234,S                 ; 03DC: AB E9 12 34   \n");
                                      #endif
    clockticks = clockticks + 8; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(S + 0x1234)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03E0; 
    JUMP; // sstep
  case 0x03E0:
                                                     #ifdef DEBUG
                                      mon("         ADDA  -$1234,S                ; 03E0: AB E9 ED CC   \n");
                                      #endif
    clockticks = clockticks + 8; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(S + 0xedcc)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03E4; 
    JUMP; // sstep
  case 0x03E4:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,X]                    ; 03E4: AB 94         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03E6; 
    JUMP; // sstep
  case 0x03E6:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,X++]                  ; 03E6: AB 91         \n");
                                      #endif
    clockticks = clockticks + 7; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    X = X + 2; 
    PC = 0x03E8; 
    JUMP; // sstep
  case 0x03E8:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,--X]                  ; 03E8: AB 93         \n");
                                      #endif
    clockticks = clockticks + 7; 
    X = X - 2; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03EA; 
    JUMP; // sstep
  case 0x03EA:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$09,X]                 ; 03EA: AB 98 09      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(X + (SINT8)(SINT8)0x09))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0x000a)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03ED; 
    JUMP; // sstep
  case 0x03ED:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$09,X]                ; 03ED: AB 98 F7      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(X + (SINT8)(SINT8)0xf7))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0xfff8)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03F0; 
    JUMP; // sstep
  case 0x03F0:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$78,X]                 ; 03F0: AB 98 78      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(X + (SINT8)(SINT8)0x78))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0x0079)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03F3; 
    JUMP; // sstep
  case 0x03F3:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$78,X]                ; 03F3: AB 98 88      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(X + (SINT8)(SINT8)0x88))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0xff89)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03F6; 
    JUMP; // sstep
  case 0x03F6:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$1234,X]               ; 03F6: AB 99 12 34   \n");
                                      #endif
    clockticks = clockticks + 8; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(X + 0x1234))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0x1235)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03FA; 
    JUMP; // sstep
  case 0x03FA:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$1234,X]              ; 03FA: AB 99 ED CC   \n");
                                      #endif
    clockticks = clockticks + 8; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(X + 0xedcc))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0xedcd)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x03FE; 
    JUMP; // sstep
  case 0x03FE:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,Y]                    ; 03FE: AB B4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(Y)<<8; 
    ea = ea | rd_mem((UINT16)((Y)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0400; 
    JUMP; // sstep
  case 0x0400:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,Y++]                  ; 0400: AB B1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(Y)<<8; 
    ea = ea | rd_mem((UINT16)((Y)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    Y = Y + 2; 
    PC = 0x0402; 
    JUMP; // sstep
  case 0x0402:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,--Y]                  ; 0402: AB B3         \n");
                                      #endif
    clockticks = clockticks + 7; 
    Y = Y - 2; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(Y)<<8; 
    ea = ea | rd_mem((UINT16)((Y)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0404; 
    JUMP; // sstep
  case 0x0404:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$09,Y]                 ; 0404: AB B8 09      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(Y + (SINT8)(SINT8)0x09))<<8; 
    ea = ea | rd_mem((UINT16)(Y + 0x000a)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0407; 
    JUMP; // sstep
  case 0x0407:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$09,Y]                ; 0407: AB B8 F7      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(Y + (SINT8)(SINT8)0xf7))<<8; 
    ea = ea | rd_mem((UINT16)(Y + 0xfff8)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x040A; 
    JUMP; // sstep
  case 0x040A:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$78,Y]                 ; 040A: AB B8 78      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(Y + (SINT8)(SINT8)0x78))<<8; 
    ea = ea | rd_mem((UINT16)(Y + 0x0079)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x040D; 
    JUMP; // sstep
  case 0x040D:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$78,Y]                ; 040D: AB B8 88      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(Y + (SINT8)(SINT8)0x88))<<8; 
    ea = ea | rd_mem((UINT16)(Y + 0xff89)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0410; 
    JUMP; // sstep
  case 0x0410:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$1234,Y]               ; 0410: AB B9 12 34   \n");
                                      #endif
    clockticks = clockticks + 8; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(Y + 0x1234))<<8; 
    ea = ea | rd_mem((UINT16)(Y + 0x1235)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0414; 
    JUMP; // sstep
  case 0x0414:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$1234,Y]              ; 0414: AB B9 ED CC   \n");
                                      #endif
    clockticks = clockticks + 8; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(Y + 0xedcc))<<8; 
    ea = ea | rd_mem((UINT16)(Y + 0xedcd)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0418; 
    JUMP; // sstep
  case 0x0418:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,U]                    ; 0418: AB D4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(U)<<8; 
    ea = ea | rd_mem((UINT16)((U)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x041A; 
    JUMP; // sstep
  case 0x041A:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,U++]                  ; 041A: AB D1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(U)<<8; 
    ea = ea | rd_mem((UINT16)((U)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    U = U + 2; 
    PC = 0x041C; 
    JUMP; // sstep
  case 0x041C:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,--U]                  ; 041C: AB D3         \n");
                                      #endif
    clockticks = clockticks + 7; 
    U = U - 2; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(U)<<8; 
    ea = ea | rd_mem((UINT16)((U)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x041E; 
    JUMP; // sstep
  case 0x041E:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$09,U]                 ; 041E: AB D8 09      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(U + (SINT8)(SINT8)0x09))<<8; 
    ea = ea | rd_mem((UINT16)(U + 0x000a)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0421; 
    JUMP; // sstep
  case 0x0421:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$09,U]                ; 0421: AB D8 F7      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(U + (SINT8)(SINT8)0xf7))<<8; 
    ea = ea | rd_mem((UINT16)(U + 0xfff8)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0424; 
    JUMP; // sstep
  case 0x0424:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$78,U]                 ; 0424: AB D8 78      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(U + (SINT8)(SINT8)0x78))<<8; 
    ea = ea | rd_mem((UINT16)(U + 0x0079)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0427; 
    JUMP; // sstep
  case 0x0427:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$78,U]                ; 0427: AB D8 88      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(U + (SINT8)(SINT8)0x88))<<8; 
    ea = ea | rd_mem((UINT16)(U + 0xff89)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x042A; 
    JUMP; // sstep
  case 0x042A:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$1234,U]               ; 042A: AB D9 12 34   \n");
                                      #endif
    clockticks = clockticks + 8; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(U + 0x1234))<<8; 
    ea = ea | rd_mem((UINT16)(U + 0x1235)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x042E; 
    JUMP; // sstep
  case 0x042E:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$1234,U]              ; 042E: AB D9 ED CC   \n");
                                      #endif
    clockticks = clockticks + 8; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(U + 0xedcc))<<8; 
    ea = ea | rd_mem((UINT16)(U + 0xedcd)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0432; 
    JUMP; // sstep
  case 0x0432:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,S]                    ; 0432: AB F4         \n");
                                      #endif
    clockticks = clockticks + 4; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(S)<<8; 
    ea = ea | rd_mem((UINT16)((S)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0434; 
    JUMP; // sstep
  case 0x0434:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,S++]                  ; 0434: AB F1         \n");
                                      #endif
    clockticks = clockticks + 7; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(S)<<8; 
    ea = ea | rd_mem((UINT16)((S)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    S = S + 2; 
    PC = 0x0436; 
    JUMP; // sstep
  case 0x0436:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [,--S]                  ; 0436: AB F3         \n");
                                      #endif
    clockticks = clockticks + 7; 
    S = S - 2; 
    clockticks = clockticks + 2; 
    clockticks = clockticks + 1; 
    ea = rd_mem(S)<<8; 
    ea = ea | rd_mem((UINT16)((S)+1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0438; 
    JUMP; // sstep
  case 0x0438:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$09,S]                 ; 0438: AB F8 09      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(S + (SINT8)(SINT8)0x09))<<8; 
    ea = ea | rd_mem((UINT16)(S + 0x000a)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x043B; 
    JUMP; // sstep
  case 0x043B:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$09,S]                ; 043B: AB F8 F7      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(S + (SINT8)(SINT8)0xf7))<<8; 
    ea = ea | rd_mem((UINT16)(S + 0xfff8)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x043E; 
    JUMP; // sstep
  case 0x043E:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$78,S]                 ; 043E: AB F8 78      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(S + (SINT8)(SINT8)0x78))<<8; 
    ea = ea | rd_mem((UINT16)(S + 0x0079)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0441; 
    JUMP; // sstep
  case 0x0441:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$78,S]                ; 0441: AB F8 88      \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(S + (SINT8)(SINT8)0x88))<<8; 
    ea = ea | rd_mem((UINT16)(S + 0xff89)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0444; 
    JUMP; // sstep
  case 0x0444:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [$1234,S]               ; 0444: AB F9 12 34   \n");
                                      #endif
    clockticks = clockticks + 8; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(S + 0x1234))<<8; 
    ea = ea | rd_mem((UINT16)(S + 0x1235)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0448; 
    JUMP; // sstep
  case 0x0448:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [-$1234,S]              ; 0448: AB F9 ED CC   \n");
                                      #endif
    clockticks = clockticks + 8; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(S + 0xedcc))<<8; 
    ea = ea | rd_mem((UINT16)(S + 0xedcd)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x044C; 
    JUMP; // sstep
  case 0x044C:
                                                     #ifdef DEBUG
                                      mon("         ADDA  A,X                     ; 044C: AB 86         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(X + (SINT8)A)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x044E; 
    JUMP; // sstep
  case 0x044E:
                                                     #ifdef DEBUG
                                      mon("         ADDA  B,Y                     ; 044E: AB A5         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(Y + (SINT8)B)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0450; 
    JUMP; // sstep
  case 0x0450:
                                                     #ifdef DEBUG
                                      mon("         ADDA  D,U                     ; 0450: AB CB         \n");
                                      #endif
    clockticks = clockticks + 8; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(U + (((UINT8)A<<8)|(UINT8)B))); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0452; 
    JUMP; // sstep
  case 0x0452:
                                                     #ifdef DEBUG
                                      mon("         ADDA  A,S                     ; 0452: AB E6         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(S + (SINT8)A)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0454; 
    JUMP; // sstep
  case 0x0454:
                                                     #ifdef DEBUG
                                      mon("         ADDA  A,U                     ; 0454: AB C6         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(U + (SINT8)A)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0456; 
    JUMP; // sstep
  case 0x0456:
                                                     #ifdef DEBUG
                                      mon("         ADDA  A,S                     ; 0456: AB E6         \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)A; 
    arg = rd_mem((UINT16)(S + (SINT8)A)); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0458; 
    JUMP; // sstep
  case 0x0458:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [A,X]                   ; 0458: AB 96         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(X + (SINT8)A))<<8; 
    ea = ea | rd_mem((UINT16)(X + (SINT8)A + 1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x045A; 
    JUMP; // sstep
  case 0x045A:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [B,Y]                   ; 045A: AB B5         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(Y + (SINT8)B))<<8; 
    ea = ea | rd_mem((UINT16)(Y + (SINT8)B + 1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x045C; 
    JUMP; // sstep
  case 0x045C:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [D,U]                   ; 045C: AB DB         \n");
                                      #endif
    clockticks = clockticks + 8; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(U + (((UINT8)A<<8)|(UINT8)B)))<<8; 
    ea = ea | rd_mem((UINT16)(U + (((UINT8)A<<8)|(UINT8)B) + 1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x045E; 
    JUMP; // sstep
  case 0x045E:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [A,S]                   ; 045E: AB F6         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(S + (SINT8)A))<<8; 
    ea = ea | rd_mem((UINT16)(S + (SINT8)A + 1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0460; 
    JUMP; // sstep
  case 0x0460:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [A,U]                   ; 0460: AB D6         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(U + (SINT8)A))<<8; 
    ea = ea | rd_mem((UINT16)(U + (SINT8)A + 1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0462; 
    JUMP; // sstep
  case 0x0462:
                                                     #ifdef DEBUG
                                      mon("         ADDA  [A,S]                   ; 0462: AB F6         \n");
                                      #endif
    clockticks = clockticks + 5; 
    clockticks = clockticks + 2; 
    ea = rd_mem((UINT16)(S + (SINT8)A))<<8; 
    ea = ea | rd_mem((UINT16)(S + (SINT8)A + 1)); 
    val = (UINT8)A; 
    arg = rd_mem(ea); 
    res = val + (UINT8)arg; 
/**/C = (res >> 1) & 0x80; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/H = (UINT8)(val ^ arg ^ res ^ C); 
/**/V = (UINT8)(val ^ arg ^ res ^ C); 
    A = (UINT8)res; 
    PC = 0x0464; 
    JUMP; // sstep
  case 0x0464:
                                                     #ifdef DEBUG
                                      mon("         LDA   $1234,PC                ; 0464: A6 8D 12 34   \n");
                                      #endif    //  ($169C)
    clockticks = clockticks + 9; 
    val = (UINT8)rd_mem(0x169c); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0468; 
    JUMP; // sstep
  case 0x0468:
                                                     #ifdef DEBUG
                                      mon("         LDA   $EDCC,PC                ; 0468: A6 8D ED CC   \n");
                                      #endif    //  ($F238)
    clockticks = clockticks + 9; 
    val = (UINT8)rd_mem(0xf238); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x046C; 
    JUMP; // sstep
  case 0x046C:
                                                     #ifdef DEBUG
                                      mon("         LDA   $0DC4,PC                ; 046C: A6 8D 0D C4   \n");
                                      #endif    //  ($1234)
    clockticks = clockticks + 9; 
    val = (UINT8)rd_mem(0x1234); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0470; 
    JUMP; // sstep
  case 0x0470:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$78                    ; 0470: 86 78         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x78; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0472; 
    JUMP; // sstep
  case 0x0472:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$56                    ; 0472: 86 56         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x56; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0474; 
    JUMP; // sstep
  case 0x0474:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$34                    ; 0474: 86 34         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x34; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0476; 
    JUMP; // sstep
  case 0x0476:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$12                    ; 0476: 86 12         \n");
                                      #endif
    clockticks = clockticks + 2; 
    val = (UINT8)0x12; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x0478; 
    JUMP; // sstep
  case 0x0478:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$5678                  ; 0478: 8E 56 78      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x5678; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x047B; 
    JUMP; // sstep
  case 0x047B:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$3456                  ; 047B: 8E 34 56      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x3456; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x047E; 
    JUMP; // sstep
  case 0x047E:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$1234                  ; 047E: 8E 12 34      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x1234; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x0481; 
    JUMP; // sstep
  case 0x0481:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$12                    ; 0481: 8E 00 12      \n");
                                      #endif
    clockticks = clockticks + 3; 
    X = 0x0012; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x0484; 
    JUMP; // sstep
  case 0x0484:
                                                     #ifdef DEBUG
                                      mon("         ASL   $5634                   ; 0484: 78 56 34      \n");
                                      #endif
    clockticks = clockticks + 5; 
    val = (UINT8)rd_mem(0x5634); 
    res = val << 1; 
/**/C = res & 0x100; 
    res = (UINT8)res; 
/**/Z = res; 
/**/N = res; 
/**/V = val ^ res; 
    wr_mem(0x5634, res); 
    clockticks = clockticks + 2; 
    PC = 0x0487; 
    JUMP; // sstep
  case 0x0487:
                                                     #ifdef DEBUG
                                      mon("         NOP                           ; 0487: 12            \n");
                                      #endif
    clockticks = clockticks + 2; 
    PC = 0x0488; 
    JUMP; // sstep
