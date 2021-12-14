                   // ORG   $8100
  case 0x8100:
                                                     #ifdef DEBUG
                                      mon("L8100    BRA   $8105                   ; 8100: 20 03         \n");
                                      #endif
    PC = 0x8105; 
    JUMP;
    
                   // FCB   $01                     ; 8102:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8103:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8104:  ' ' 00 7F          NEG   <$7F ; 
  case 0x8105:
                                                     #ifdef DEBUG
                                      mon("L8105    CLR   $9394                   ; 8105: 7F 93 94      \n");
                                      #endif
    wr_mem(0x9394, 0); 
    V = 0; 
    Z = 0; 
    N = 0; 
    C = 0; 
    PC = 0x8108; 
    JUMP; // sstep
  case 0x8108:
                                                     #ifdef DEBUG
                                      mon("         JSR   $8379                   ; 8108: BD 83 79      \n");
                                      #endif
    --S;
    wr_mem(S, 0x0b); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8379; 
    JUMP;
  case 0x810B:
                                                     #ifdef DEBUG
                                      mon("L810B    JSR   $8E88                   ; 810B: BD 8E 88      \n");
                                      #endif
    --S;
    wr_mem(S, 0x0e); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8e88; 
    JUMP;
  case 0x810E:
                                                     #ifdef DEBUG
                                      mon("L810E    JSR   $8EDC                   ; 810E: BD 8E DC      \n");
                                      #endif
    --S;
    wr_mem(S, 0x11); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8edc; 
    JUMP;
  case 0x8111:
                                                     #ifdef DEBUG
                                      mon("L8111    JSR   $8EFB                   ; 8111: BD 8E FB      \n");
                                      #endif
    --S;
    wr_mem(S, 0x14); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8efb; 
    JUMP;
  case 0x8114:
                                                     #ifdef DEBUG
                                      mon("L8114    JSR   $8F25                   ; 8114: BD 8F 25      \n");
                                      #endif
    --S;
    wr_mem(S, 0x17); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8f25; 
    JUMP;
  case 0x8117:
                                                     #ifdef DEBUG
                                      mon("L8117    JSR   $8F45                   ; 8117: BD 8F 45      \n");
                                      #endif
    --S;
    wr_mem(S, 0x1a); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8f45; 
    JUMP;
  case 0x811A:
                                                     #ifdef DEBUG
                                      mon("L811A    JSR   $8F6F                   ; 811A: BD 8F 6F      \n");
                                      #endif
    --S;
    wr_mem(S, 0x1d); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8f6f; 
    JUMP;
  case 0x811D:
                                                     #ifdef DEBUG
                                      mon("L811D    JSR   $8F9A                   ; 811D: BD 8F 9A      \n");
                                      #endif
    --S;
    wr_mem(S, 0x20); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8f9a; 
    JUMP;
  case 0x8120:
                                                     #ifdef DEBUG
                                      mon("L8120    JSR   $8FC6                   ; 8120: BD 8F C6      \n");
                                      #endif
    --S;
    wr_mem(S, 0x23); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8fc6; 
    JUMP;
  case 0x8123:
                                                     #ifdef DEBUG
                                      mon("L8123    JSR   $8FFC                   ; 8123: BD 8F FC      \n");
                                      #endif
    --S;
    wr_mem(S, 0x26); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8ffc; 
    JUMP;
  case 0x8126:
                                                     #ifdef DEBUG
                                      mon("L8126    JSR   $902F                   ; 8126: BD 90 2F      \n");
                                      #endif
    --S;
    wr_mem(S, 0x29); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x902f; 
    JUMP;
  case 0x8129:
                                                     #ifdef DEBUG
                                      mon("L8129    JSR   $9056                   ; 8129: BD 90 56      \n");
                                      #endif
    --S;
    wr_mem(S, 0x2c); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x9056; 
    JUMP;
  case 0x812C:
                                                     #ifdef DEBUG
                                      mon("L812C    JSR   $9084                   ; 812C: BD 90 84      \n");
                                      #endif
    --S;
    wr_mem(S, 0x2f); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x9084; 
    JUMP;
  case 0x812F:
                                                     #ifdef DEBUG
                                      mon("L812F    JSR   $90EE                   ; 812F: BD 90 EE      \n");
                                      #endif
    --S;
    wr_mem(S, 0x32); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x90ee; 
    JUMP;
  case 0x8132:
                                                     #ifdef DEBUG
                                      mon("L8132    JSR   $9118                   ; 8132: BD 91 18      \n");
                                      #endif
    --S;
    wr_mem(S, 0x35); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x9118; 
    JUMP;
  case 0x8135:
                                                     #ifdef DEBUG
                                      mon("L8135    JSR   $9142                   ; 8135: BD 91 42      \n");
                                      #endif
    --S;
    wr_mem(S, 0x38); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x9142; 
    JUMP;
  case 0x8138:
                                                     #ifdef DEBUG
                                      mon("L8138    JSR   $9173                   ; 8138: BD 91 73      \n");
                                      #endif
    --S;
    wr_mem(S, 0x3b); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x9173; 
    JUMP;
  case 0x813B:
                                                     #ifdef DEBUG
                                      mon("L813B    JSR   $91A4                   ; 813B: BD 91 A4      \n");
                                      #endif
    --S;
    wr_mem(S, 0x3e); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x91a4; 
    JUMP;
  case 0x813E:
                                                     #ifdef DEBUG
                                      mon("L813E    JSR   $91DA                   ; 813E: BD 91 DA      \n");
                                      #endif
    --S;
    wr_mem(S, 0x41); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x91da; 
    JUMP;
  case 0x8141:
                                                     #ifdef DEBUG
                                      mon("L8141    JSR   $920E                   ; 8141: BD 92 0E      \n");
                                      #endif
    --S;
    wr_mem(S, 0x44); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x920e; 
    JUMP;
  case 0x8144:
                                                     #ifdef DEBUG
                                      mon("L8144    JSR   $923C                   ; 8144: BD 92 3C      \n");
                                      #endif
    --S;
    wr_mem(S, 0x47); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x923c; 
    JUMP;
  case 0x8147:
                                                     #ifdef DEBUG
                                      mon("L8147    JSR   $926A                   ; 8147: BD 92 6A      \n");
                                      #endif
    --S;
    wr_mem(S, 0x4a); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x926a; 
    JUMP;
  case 0x814A:
                                                     #ifdef DEBUG
                                      mon("L814A    JSR   $84E6                   ; 814A: BD 84 E6      \n");
                                      #endif
    --S;
    wr_mem(S, 0x4d); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x84e6; 
    JUMP;
  case 0x814D:
                                                     #ifdef DEBUG
                                      mon("L814D    JSR   $8523                   ; 814D: BD 85 23      \n");
                                      #endif
    --S;
    wr_mem(S, 0x50); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8523; 
    JUMP;
  case 0x8150:
                                                     #ifdef DEBUG
                                      mon("L8150    JSR   $8766                   ; 8150: BD 87 66      \n");
                                      #endif
    --S;
    wr_mem(S, 0x53); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8766; 
    JUMP;
  case 0x8153:
                                                     #ifdef DEBUG
                                      mon("L8153    JSR   $879E                   ; 8153: BD 87 9E      \n");
                                      #endif
    --S;
    wr_mem(S, 0x56); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x879e; 
    JUMP;
  case 0x8156:
                                                     #ifdef DEBUG
                                      mon("L8156    JSR   $87D6                   ; 8156: BD 87 D6      \n");
                                      #endif
    --S;
    wr_mem(S, 0x59); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x87d6; 
    JUMP;
  case 0x8159:
                                                     #ifdef DEBUG
                                      mon("L8159    JSR   $8894                   ; 8159: BD 88 94      \n");
                                      #endif
    --S;
    wr_mem(S, 0x5c); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8894; 
    JUMP;
  case 0x815C:
                                                     #ifdef DEBUG
                                      mon("L815C    JSR   $880A                   ; 815C: BD 88 0A      \n");
                                      #endif
    --S;
    wr_mem(S, 0x5f); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x880a; 
    JUMP;
  case 0x815F:
                                                     #ifdef DEBUG
                                      mon("L815F    JSR   $88F4                   ; 815F: BD 88 F4      \n");
                                      #endif
    --S;
    wr_mem(S, 0x62); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x88f4; 
    JUMP;
  case 0x8162:
                                                     #ifdef DEBUG
                                      mon("L8162    JSR   $81AA                   ; 8162: BD 81 AA      \n");
                                      #endif
    --S;
    wr_mem(S, 0x65); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x81aa; 
    JUMP;
  case 0x8165:
                                                     #ifdef DEBUG
                                      mon("L8165    JSR   $8209                   ; 8165: BD 82 09      \n");
                                      #endif
    --S;
    wr_mem(S, 0x68); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8209; 
    JUMP;
  case 0x8168:
                                                     #ifdef DEBUG
                                      mon("L8168    JSR   $89F4                   ; 8168: BD 89 F4      \n");
                                      #endif
    --S;
    wr_mem(S, 0x6b); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x89f4; 
    JUMP;
  case 0x816B:
                                                     #ifdef DEBUG
                                      mon("L816B    JSR   $8BE4                   ; 816B: BD 8B E4      \n");
                                      #endif
    --S;
    wr_mem(S, 0x6e); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8be4; 
    JUMP;
  case 0x816E:
                                                     #ifdef DEBUG
                                      mon("L816E    JSR   $8A80                   ; 816E: BD 8A 80      \n");
                                      #endif
    --S;
    wr_mem(S, 0x71); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8a80; 
    JUMP;
  case 0x8171:
                                                     #ifdef DEBUG
                                      mon("L8171    JSR   $8379                   ; 8171: BD 83 79      \n");
                                      #endif
    --S;
    wr_mem(S, 0x74); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8379; 
    JUMP;
  case 0x8174:
                                                     #ifdef DEBUG
                                      mon("L8174    JSR   $89D4                   ; 8174: BD 89 D4      \n");
                                      #endif
    --S;
    wr_mem(S, 0x77); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x89d4; 
    JUMP;
  case 0x8177:
                                                     #ifdef DEBUG
                                      mon("L8177    JSR   $8CC2                   ; 8177: BD 8C C2      \n");
                                      #endif
    --S;
    wr_mem(S, 0x7a); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8cc2; 
    JUMP;
  case 0x817A:
                                                     #ifdef DEBUG
                                      mon("L817A    JSR   $8245                   ; 817A: BD 82 45      \n");
                                      #endif
    --S;
    wr_mem(S, 0x7d); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8245; 
    JUMP;
  case 0x817D:
                                                     #ifdef DEBUG
                                      mon("L817D    JSR   $833B                   ; 817D: BD 83 3B      \n");
                                      #endif
    --S;
    wr_mem(S, 0x80); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x833b; 
    JUMP;
  case 0x8180:
                                                     #ifdef DEBUG
                                      mon("L8180    JSR   $8564                   ; 8180: BD 85 64      \n");
                                      #endif
    --S;
    wr_mem(S, 0x83); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8564; 
    JUMP;
  case 0x8183:
                                                     #ifdef DEBUG
                                      mon("L8183    JSR   $85C0                   ; 8183: BD 85 C0      \n");
                                      #endif
    --S;
    wr_mem(S, 0x86); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x85c0; 
    JUMP;
  case 0x8186:
                                                     #ifdef DEBUG
                                      mon("L8186    JSR   $8618                   ; 8186: BD 86 18      \n");
                                      #endif
    --S;
    wr_mem(S, 0x89); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8618; 
    JUMP;
  case 0x8189:
                                                     #ifdef DEBUG
                                      mon("L8189    JSR   $86EF                   ; 8189: BD 86 EF      \n");
                                      #endif
    --S;
    wr_mem(S, 0x8c); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x86ef; 
    JUMP;
  case 0x818C:
                                                     #ifdef DEBUG
                                      mon("L818C    JSR   $8674                   ; 818C: BD 86 74      \n");
                                      #endif
    --S;
    wr_mem(S, 0x8f); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8674; 
    JUMP;
  case 0x818F:
                                                     #ifdef DEBUG
                                      mon("L818F    JSR   $8274                   ; 818F: BD 82 74      \n");
                                      #endif
    --S;
    wr_mem(S, 0x92); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8274; 
    JUMP;
  case 0x8192:
                                                     #ifdef DEBUG
                                      mon("L8192    JSR   $8291                   ; 8192: BD 82 91      \n");
                                      #endif
    --S;
    wr_mem(S, 0x95); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8291; 
    JUMP;
  case 0x8195:
                                                     #ifdef DEBUG
                                      mon("L8195    JSR   $82B1                   ; 8195: BD 82 B1      \n");
                                      #endif
    --S;
    wr_mem(S, 0x98); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x82b1; 
    JUMP;
  case 0x8198:
                                                     #ifdef DEBUG
                                      mon("L8198    JSR   $82E7                   ; 8198: BD 82 E7      \n");
                                      #endif
    --S;
    wr_mem(S, 0x9b); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x82e7; 
    JUMP;
  case 0x819B:
                                                     #ifdef DEBUG
                                      mon("L819B    JSR   $8308                   ; 819B: BD 83 08      \n");
                                      #endif
    --S;
    wr_mem(S, 0x9e); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8308; 
    JUMP;
  case 0x819E:
                                                     #ifdef DEBUG
                                      mon("L819E    JSR   $8CE5                   ; 819E: BD 8C E5      \n");
                                      #endif
    --S;
    wr_mem(S, 0xa1); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8ce5; 
    JUMP;
  case 0x81A1:
                                                     #ifdef DEBUG
                                      mon("L81A1    JSR   $8D8B                   ; 81A1: BD 8D 8B      \n");
                                      #endif
    --S;
    wr_mem(S, 0xa4); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x8d8b; 
    JUMP;
  case 0x81A4:
                                                     #ifdef DEBUG
                                      mon("L81A4    JSR   $9386                   ; 81A4: BD 93 86      \n");
                                      #endif
    --S;
    wr_mem(S, 0xa7); 
    --S;
    wr_mem(S, 0x81); 
    PC = 0x9386; 
    JUMP;
  case 0x81A7:
                                                     #ifdef DEBUG
                                      mon("L81A7    JMP   $CD03                   ; 81A7: 7E CD 03      \n");
                                      #endif
    PC = 0xcd03; 
    JUMP;
    
  case 0x81AA:
                                                     #ifdef DEBUG
                                      mon("L81AA    LDU   #$8205                  ; 81AA: CE 82 05      \n");
                                      #endif
    U = 0x8205; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x81AD; 
    JUMP; // sstep
  case 0x81AD:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$81CE                  ; 81AD: 8E 81 CE      \n");
                                      #endif
    X = 0x81ce; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x81B0; 
    JUMP; // sstep
  case 0x81B0:
                                                     #ifdef DEBUG
                                      mon("L81B0    LDD   ,X++                    ; 81B0: EC 81         \n");
                                      #endif
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
    X = X + 2; 
/**/Z = A|B; 
    N = A; 
    V = 0; 
    PC = 0x81B2; 
    JUMP; // sstep
  case 0x81B2:
                                                     #ifdef DEBUG
                                      mon("         MUL                           ; 81B2: 3D            \n");
                                      #endif
    res = (UINT16)(A * B); 
    Z = res; 
    C = res & 0x80; 
    A = res >> 8; 
    B = (UINT8)res; 
    PC = 0x81B3; 
    JUMP; // sstep
  case 0x81B3:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,DP                   ; 81B3: 1F AB         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    DP = (CC) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x81B5; 
    JUMP; // sstep
  case 0x81B5:
                                                     #ifdef DEBUG
                                      mon("         CMPD  ,X++                    ; 81B5: 10 A3 81      \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = ea; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    X = X + 2; 
    PC = 0x81B8; 
    JUMP; // sstep
  case 0x81B8:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 81B8: 10 26 11 B9   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x81BC; 
    JUMP; // sstep
  case 0x81BC:
                                                     #ifdef DEBUG
                                      mon("         TFR   DP,B                    ; 81BC: 1F B9         \n");
                                      #endif
    B = (UINT8)(UINT8)(DP >> 8); 
    PC = 0x81BE; 
    JUMP; // sstep
  case 0x81BE:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$05                    ; 81BE: C4 05         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x05; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x81C0; 
    JUMP; // sstep
  case 0x81C0:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 81C0: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x81C2; 
    JUMP; // sstep
  case 0x81C2:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 81C2: 10 26 11 AF   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x81C6; 
    JUMP; // sstep
  case 0x81C6:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8205                  ; 81C6: 8C 82 05      \n");
                                      #endif
    val = X; 
    arg = 0x8205; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x81C9; 
    JUMP; // sstep
  case 0x81C9:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $81B0                   ; 81C9: 10 26 FF E3   \n");
                                      #endif
    if (Z) {
    PC = 0x81b0; 
    JUMP;
    }
    PC = 0x81CD; 
    JUMP; // sstep
  case 0x81CD:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 81CD: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 81CE:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 81CF:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 81D0:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 81D1:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 81D2:  ' ' 04 80          LSR   <$80 ; 
                   // FCB   $80                     ; 81D3:  ' ' 80 80          SUBA  #$80 ; 
                   // FCB   $80                     ; 81D4:  ' ' 80 40          SUBA  #$40 ; 
                   // FCB   $40                     ; 81D5:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 81D6:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 81D7:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 81D8:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 81D9:  ' ' FF 00 FF       STU   $00FF ; 
                   // FCB   $00                     ; 81DA:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 81DB:  ' ' FF 01 FF       STU   $01FF ; 
                   // FCB   $01                     ; 81DC:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 81DD:  ' ' FF 01 00       STU   $0100 ; 
                   // FCB   $01                     ; 81DE:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 81DF:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 81E0:  ' ' FF 01 7F       STU   $017F ; 
                   // FCB   $01                     ; 81E1:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 81E2:  ' ' 7F 81 3F       CLR   $813F ; 
                   // FCB   $81                     ; 81E3:  ' ' 81 3F          CMPA  #$3F ; 
                   // FCB   $3F                     ; 81E4:  '?' 3F             SWI   ; 
                   // FCB   $FF                     ; 81E5:  ' ' FF 01 81       STU   $0181 ; 
                   // FCB   $01                     ; 81E6:  ' ' 01             Invalid ; 
                   // FCB   $81                     ; 81E7:  ' ' 81 7F          CMPA  #$7F ; 
                   // FCB   $7F                     ; 81E8:  ' ' 7F 3F FF       CLR   $3FFF ; 
                   // FCB   $3F                     ; 81E9:  '?' 3F             SWI   ; 
                   // FCB   $FF                     ; 81EA:  ' ' FF 01 C0       STU   $01C0 ; 
                   // FCB   $01                     ; 81EB:  ' ' 01             Invalid ; 
                   // FCB   $C0                     ; 81EC:  ' ' C0 40          SUBB  #$40 ; 
                   // FCB   $40                     ; 81ED:  '@' 40             NEGA  ; 
                   // FCB   $30                     ; 81EE:  '0' 30 00          LEAX  0,X ; 
                   // FCB   $00                     ; 81EF:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 81F0:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 81F1:  '@' 40             NEGA  ; 
                   // FCB   $C0                     ; 81F2:  ' ' C0 30          SUBB  #$30 ; 
                   // FCB   $30                     ; 81F3:  '0' 30 00          LEAX  0,X ; 
                   // FCB   $00                     ; 81F4:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 81F5:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 81F6:  ' ' FF FF FE       STU   $FFFE ; 
                   // FCB   $FF                     ; 81F7:  ' ' FF FE 01       STU   $FE01 ; 
                   // FCB   $FE                     ; 81F8:  ' ' FE 01 00       LDU   $0100 ; 
                   // FCB   $01                     ; 81F9:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 81FA:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 81FB:  ' ' 7F 7F 3F       CLR   $7F3F ; 
                   // FCB   $7F                     ; 81FC:  ' ' 7F 3F 01       CLR   $3F01 ; 
                   // FCB   $3F                     ; 81FD:  '?' 3F             SWI   ; 
                   // FCB   $01                     ; 81FE:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 81FF:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8200:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8201:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8202:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8203:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8204:  ' ' 00 4D          NEG   <$4D ; 
                   // FCB   $4D                     ; 8205:  'M' 4D             TSTA  ; 
                   // FCB   $55                     ; 8206:  'U' 55             Invalid ; 
                   // FCB   $4C                     ; 8207:  'L' 4C             INCA  ; 
                   // FCB   $04                     ; 8208:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8209:
                                                     #ifdef DEBUG
                                      mon("L8209    LDU   #$8241                  ; 8209: CE 82 41      \n");
                                      #endif
    U = 0x8241; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x820C; 
    JUMP; // sstep
  case 0x820C:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$822D                  ; 820C: 8E 82 2D      \n");
                                      #endif
    X = 0x822d; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x820F; 
    JUMP; // sstep
  case 0x820F:
                                                     #ifdef DEBUG
                                      mon("L820F    LDB   ,X+                     ; 820F: E6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    B = val; 
/**/N = val; 
/**/Z = val; 
    V = 0; 
    X = X + 1; 
    PC = 0x8211; 
    JUMP; // sstep
  case 0x8211:
                                                     #ifdef DEBUG
                                      mon("         SEX                           ; 8211: 1D            \n");
                                      #endif
    Z = B; 
    N = B; 
    A = ((SINT8)B < 0 ? 0xff : 0); 
    PC = 0x8212; 
    JUMP; // sstep
  case 0x8212:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,DP                   ; 8212: 1F AB         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    DP = (CC) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8214; 
    JUMP; // sstep
  case 0x8214:
                                                     #ifdef DEBUG
                                      mon("         CMPD  ,X++                    ; 8214: 10 A3 81      \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = ea; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    X = X + 2; 
    PC = 0x8217; 
    JUMP; // sstep
  case 0x8217:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8217: 10 26 11 5A   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x821B; 
    JUMP; // sstep
  case 0x821B:
                                                     #ifdef DEBUG
                                      mon("         TFR   DP,B                    ; 821B: 1F B9         \n");
                                      #endif
    B = (UINT8)(UINT8)(DP >> 8); 
    PC = 0x821D; 
    JUMP; // sstep
  case 0x821D:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0C                    ; 821D: C4 0C         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0c; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x821F; 
    JUMP; // sstep
  case 0x821F:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 821F: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8221; 
    JUMP; // sstep
  case 0x8221:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8221: 10 26 11 50   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8225; 
    JUMP; // sstep
  case 0x8225:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8241                  ; 8225: 8C 82 41      \n");
                                      #endif
    val = X; 
    arg = 0x8241; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8228; 
    JUMP; // sstep
  case 0x8228:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $820F                   ; 8228: 10 26 FF E3   \n");
                                      #endif
    if (Z) {
    PC = 0x820f; 
    JUMP;
    }
    PC = 0x822C; 
    JUMP; // sstep
  case 0x822C:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 822C: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 822D:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 822E:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 822F:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8230:  ' ' 04 01          LSR   <$01 ; 
                   // FCB   $01                     ; 8231:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8232:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8233:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8234:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8235:  ' ' 7F 00 7F       CLR   $007F ; 
                   // FCB   $00                     ; 8236:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8237:  ' ' 7F 00 80       CLR   $0080 ; 
                   // FCB   $00                     ; 8238:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8239:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 823A:  ' ' FF 80 08       STU   $8008 ; 
                   // FCB   $80                     ; 823B:  ' ' 80 08          SUBA  #$08 ; 
                   // FCB   $08                     ; 823C:  ' ' 08 FF          ASL   <$FF ; 
                   // FCB   $FF                     ; 823D:  ' ' FF FF FF       STU   $FFFF ; 
                   // FCB   $FF                     ; 823E:  ' ' FF FF 08       STU   $FF08 ; 
                   // FCB   $FF                     ; 823F:  ' ' FF 08 53       STU   $0853 ; 
                   // FCB   $08                     ; 8240:  ' ' 08 53          ASL   <$53 ; 
                   // FCB   $53                     ; 8241:  'S' 53             COMB  ; 
                   // FCB   $45                     ; 8242:  'E' 45             Invalid ; 
                   // FCB   $58                     ; 8243:  'X' 58             ASLB  ; 
                   // FCB   $04                     ; 8244:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8245:
                                                     #ifdef DEBUG
                                      mon("L8245    LDU   #$8270                  ; 8245: CE 82 70      \n");
                                      #endif
    U = 0x8270; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8248; 
    JUMP; // sstep
  case 0x8248:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8260                  ; 8248: 8E 82 60      \n");
                                      #endif
    X = 0x8260; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x824B; 
    JUMP; // sstep
  case 0x824B:
                                                     #ifdef DEBUG
                                      mon("L824B    LDA   ,X+                     ; 824B: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x824D; 
    JUMP; // sstep
  case 0x824D:
                                                     #ifdef DEBUG
                                      mon("         TSTA                          ; 824D: 4D            \n");
                                      #endif
    val = (UINT8)A; 
    Z = val; 
    N = val; 
    V = 0; 
    PC = 0x824E; 
    JUMP; // sstep
  case 0x824E:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 824E: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8250; 
    JUMP; // sstep
  case 0x8250:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0E                    ; 8250: C4 0E         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0e; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8252; 
    JUMP; // sstep
  case 0x8252:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8252: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8254; 
    JUMP; // sstep
  case 0x8254:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8254: 10 26 11 1D   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8258; 
    JUMP; // sstep
  case 0x8258:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8270                  ; 8258: 8C 82 70      \n");
                                      #endif
    val = X; 
    arg = 0x8270; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x825B; 
    JUMP; // sstep
  case 0x825B:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $824B                   ; 825B: 10 26 FF EC   \n");
                                      #endif
    if (Z) {
    PC = 0x824b; 
    JUMP;
    }
    PC = 0x825F; 
    JUMP; // sstep
  case 0x825F:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 825F: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8260:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8261:  ' ' 04 80          LSR   <$80 ; 
                   // FCB   $80                     ; 8262:  ' ' 80 08          SUBA  #$08 ; 
                   // FCB   $08                     ; 8263:  ' ' 08 01          ASL   <$01 ; 
                   // FCB   $01                     ; 8264:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8265:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8266:  ' ' FF 08 7F       STU   $087F ; 
                   // FCB   $08                     ; 8267:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 8268:  ' ' 7F 00 81       CLR   $0081 ; 
                   // FCB   $00                     ; 8269:  ' ' 00 81          NEG   <$81 ; 
                   // FCB   $81                     ; 826A:  ' ' 81 08          CMPA  #$08 ; 
                   // FCB   $08                     ; 826B:  ' ' 08 C0          ASL   <$C0 ; 
                   // FCB   $C0                     ; 826C:  ' ' C0 08          SUBB  #$08 ; 
                   // FCB   $08                     ; 826D:  ' ' 08 40          ASL   <$40 ; 
                   // FCB   $40                     ; 826E:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 826F:  ' ' 00 54          NEG   <$54 ; 
                   // FCB   $54                     ; 8270:  'T' 54             LSRB  ; 
                   // FCB   $53                     ; 8271:  'S' 53             COMB  ; 
                   // FCB   $54                     ; 8272:  'T' 54             LSRB  ; 
                   // FCB   $04                     ; 8273:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8274:
                                                     #ifdef DEBUG
                                      mon("L8274    LDU   #$828E                  ; 8274: CE 82 8E      \n");
                                      #endif
    U = 0x828e; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8277; 
    JUMP; // sstep
  case 0x8277:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8260                  ; 8277: 8E 82 60      \n");
                                      #endif
    X = 0x8260; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x827A; 
    JUMP; // sstep
  case 0x827A:
                                                     #ifdef DEBUG
                                      mon("L827A    LDA   ,X+                     ; 827A: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
    X = X + 1; 
    PC = 0x827C; 
    JUMP; // sstep
  case 0x827C:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 827C: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x827E; 
    JUMP; // sstep
  case 0x827E:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0E                    ; 827E: C4 0E         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0e; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8280; 
    JUMP; // sstep
  case 0x8280:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8280: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8282; 
    JUMP; // sstep
  case 0x8282:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8282: 10 26 10 EF   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8286; 
    JUMP; // sstep
  case 0x8286:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8270                  ; 8286: 8C 82 70      \n");
                                      #endif
    val = X; 
    arg = 0x8270; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8289; 
    JUMP; // sstep
  case 0x8289:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $827A                   ; 8289: 10 26 FF ED   \n");
                                      #endif
    if (Z) {
    PC = 0x827a; 
    JUMP;
    }
    PC = 0x828D; 
    JUMP; // sstep
  case 0x828D:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 828D: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $4C                     ; 828E:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 828F:  'D' 44             LSRA  ; 
                   // FCB   $04                     ; 8290:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8291:
                                                     #ifdef DEBUG
                                      mon("L8291    LDU   #$82AE                  ; 8291: CE 82 AE      \n");
                                      #endif
    U = 0x82ae; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8294; 
    JUMP; // sstep
  case 0x8294:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8260                  ; 8294: 8E 82 60      \n");
                                      #endif
    X = 0x8260; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8297; 
    JUMP; // sstep
  case 0x8297:
                                                     #ifdef DEBUG
                                      mon("L8297    LDA   ,X+                     ; 8297: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8299; 
    JUMP; // sstep
  case 0x8299:
                                                     #ifdef DEBUG
                                      mon("         STA   $8000                   ; 8299: B7 80 00      \n");
                                      #endif
    val = A; 
    Z = val; 
    N = val; 
    V = 0; 
    wr_mem(0x8000, val); 
    PC = 0x829C; 
    JUMP; // sstep
  case 0x829C:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 829C: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x829E; 
    JUMP; // sstep
  case 0x829E:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0E                    ; 829E: C4 0E         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0e; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x82A0; 
    JUMP; // sstep
  case 0x82A0:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 82A0: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x82A2; 
    JUMP; // sstep
  case 0x82A2:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 82A2: 10 26 10 CF   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x82A6; 
    JUMP; // sstep
  case 0x82A6:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8270                  ; 82A6: 8C 82 70      \n");
                                      #endif
    val = X; 
    arg = 0x8270; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x82A9; 
    JUMP; // sstep
  case 0x82A9:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $8297                   ; 82A9: 10 26 FF EA   \n");
                                      #endif
    if (Z) {
    PC = 0x8297; 
    JUMP;
    }
    PC = 0x82AD; 
    JUMP; // sstep
  case 0x82AD:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 82AD: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $53                     ; 82AE:  'S' 53             COMB  ; 
                   // FCB   $54                     ; 82AF:  'T' 54             LSRB  ; 
                   // FCB   $04                     ; 82B0:  ' ' 04 CE          LSR   <$CE ; 
  case 0x82B1:
                                                     #ifdef DEBUG
                                      mon("L82B1    LDU   #$82E3                  ; 82B1: CE 82 E3      \n");
                                      #endif
    U = 0x82e3; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x82B4; 
    JUMP; // sstep
  case 0x82B4:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$82CB                  ; 82B4: 8E 82 CB      \n");
                                      #endif
    X = 0x82cb; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x82B7; 
    JUMP; // sstep
  case 0x82B7:
                                                     #ifdef DEBUG
                                      mon("L82B7    LDD   ,X++                    ; 82B7: EC 81         \n");
                                      #endif
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
    X = X + 2; 
    Z = A|B; 
    N = A; 
    V = 0; 
    PC = 0x82B9; 
    JUMP; // sstep
  case 0x82B9:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 82B9: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x82BB; 
    JUMP; // sstep
  case 0x82BB:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0E                    ; 82BB: C4 0E         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0e; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x82BD; 
    JUMP; // sstep
  case 0x82BD:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 82BD: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x82BF; 
    JUMP; // sstep
  case 0x82BF:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 82BF: 10 26 10 B2   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x82C3; 
    JUMP; // sstep
  case 0x82C3:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$82E3                  ; 82C3: 8C 82 E3      \n");
                                      #endif
    val = X; 
    arg = 0x82e3; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x82C6; 
    JUMP; // sstep
  case 0x82C6:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $82B7                   ; 82C6: 10 26 FF ED   \n");
                                      #endif
    if (Z) {
    PC = 0x82b7; 
    JUMP;
    }
    PC = 0x82CA; 
    JUMP; // sstep
  case 0x82CA:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 82CA: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 82CB:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 82CC:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 82CD:  ' ' 04 80          LSR   <$80 ; 
                   // FCB   $80                     ; 82CE:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 82CF:  ' ' 00 08          NEG   <$08 ; 
                   // FCB   $08                     ; 82D0:  ' ' 08 00          ASL   <$00 ; 
                   // FCB   $00                     ; 82D1:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 82D2:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 82D3:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 82D4:  ' ' FF FF 08       STU   $FF08 ; 
                   // FCB   $FF                     ; 82D5:  ' ' FF 08 7F       STU   $087F ; 
                   // FCB   $08                     ; 82D6:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 82D7:  ' ' 7F FF 00       CLR   $FF00 ; 
                   // FCB   $FF                     ; 82D8:  ' ' FF 00 80       STU   $0080 ; 
                   // FCB   $00                     ; 82D9:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 82DA:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 82DB:  ' ' 01             Invalid ; 
                   // FCB   $08                     ; 82DC:  ' ' 08 C0          ASL   <$C0 ; 
                   // FCB   $C0                     ; 82DD:  ' ' C0 00          SUBB  #$00 ; 
                   // FCB   $00                     ; 82DE:  ' ' 00 08          NEG   <$08 ; 
                   // FCB   $08                     ; 82DF:  ' ' 08 40          ASL   <$40 ; 
                   // FCB   $40                     ; 82E0:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 82E1:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 82E2:  ' ' 00 4C          NEG   <$4C ; 
                   // FCB   $4C                     ; 82E3:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 82E4:  'D' 44             LSRA  ; 
                   // FCB   $44                     ; 82E5:  'D' 44             LSRA  ; 
                   // FCB   $04                     ; 82E6:  ' ' 04 CE          LSR   <$CE ; 
  case 0x82E7:
                                                     #ifdef DEBUG
                                      mon("L82E7    LDU   #$8304                  ; 82E7: CE 83 04      \n");
                                      #endif
    U = 0x8304; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x82EA; 
    JUMP; // sstep
  case 0x82EA:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$82CB                  ; 82EA: 8E 82 CB      \n");
                                      #endif
    X = 0x82cb; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x82ED; 
    JUMP; // sstep
  case 0x82ED:
                                                     #ifdef DEBUG
                                      mon("L82ED    LDD   ,X++                    ; 82ED: EC 81         \n");
                                      #endif
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
    X = X + 2; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x82EF; 
    JUMP; // sstep
  case 0x82EF:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 82EF: FD 80 00      \n");
                                      #endif
    Z = A | B; 
    N = A; 
    V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x82F2; 
    JUMP; // sstep
  case 0x82F2:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 82F2: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x82F4; 
    JUMP; // sstep
  case 0x82F4:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0E                    ; 82F4: C4 0E         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0e; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x82F6; 
    JUMP; // sstep
  case 0x82F6:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 82F6: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x82F8; 
    JUMP; // sstep
  case 0x82F8:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 82F8: 10 26 10 79   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x82FC; 
    JUMP; // sstep
  case 0x82FC:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$82E3                  ; 82FC: 8C 82 E3      \n");
                                      #endif
    val = X; 
    arg = 0x82e3; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x82FF; 
    JUMP; // sstep
  case 0x82FF:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $82ED                   ; 82FF: 10 26 FF EA   \n");
                                      #endif
    if (Z) {
    PC = 0x82ed; 
    JUMP;
    }
    PC = 0x8303; 
    JUMP; // sstep
  case 0x8303:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8303: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $53                     ; 8304:  'S' 53             COMB  ; 
                   // FCB   $54                     ; 8305:  'T' 54             LSRB  ; 
                   // FCB   $44                     ; 8306:  'D' 44             LSRA  ; 
                   // FCB   $04                     ; 8307:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8308:
                                                     #ifdef DEBUG
                                      mon("L8308    LDU   #$8337                  ; 8308: CE 83 37      \n");
                                      #endif
    U = 0x8337; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x830B; 
    JUMP; // sstep
  case 0x830B:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8325                  ; 830B: 8E 83 25      \n");
                                      #endif
    X = 0x8325; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x830E; 
    JUMP; // sstep
  case 0x830E:
                                                     #ifdef DEBUG
                                      mon("L830E    LDY   ,X++                    ; 830E: 10 AE 81      \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    Y = ea; 
/**/Z = Y; 
    N = (Y) >> 8; 
    V = 0; 
    X = X + 2; 
    PC = 0x8311; 
    JUMP; // sstep
  case 0x8311:
                                                     #ifdef DEBUG
                                      mon("         LEAY  ,Y                      ; 8311: 31 A4         \n");
                                      #endif
    Y = Y; 
    Z = Y; 
    PC = 0x8313; 
    JUMP; // sstep
  case 0x8313:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 8313: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8315; 
    JUMP; // sstep
  case 0x8315:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$04                    ; 8315: C4 04         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x04; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8317; 
    JUMP; // sstep
  case 0x8317:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8317: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8319; 
    JUMP; // sstep
  case 0x8319:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8319: 10 26 10 58   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x831D; 
    JUMP; // sstep
  case 0x831D:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8337                  ; 831D: 8C 83 37      \n");
                                      #endif
    val = X; 
    arg = 0x8337; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8320; 
    JUMP; // sstep
  case 0x8320:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $830E                   ; 8320: 10 26 FF EA   \n");
                                      #endif
    if (Z) {
    PC = 0x830e; 
    JUMP;
    }
    PC = 0x8324; 
    JUMP; // sstep
  case 0x8324:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8324: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8325:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8326:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8327:  ' ' 04 00          LSR   <$00 ; 
                   // FCB   $00                     ; 8328:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8329:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 832A:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 832B:  ' ' FF FF 00       STU   $FF00 ; 
                   // FCB   $FF                     ; 832C:  ' ' FF 00 7F       STU   $007F ; 
                   // FCB   $00                     ; 832D:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 832E:  ' ' 7F FF 00       CLR   $FF00 ; 
                   // FCB   $FF                     ; 832F:  ' ' FF 00 80       STU   $0080 ; 
                   // FCB   $00                     ; 8330:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8331:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8332:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8333:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8334:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8335:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8336:  ' ' 00 4C          NEG   <$4C ; 
                   // FCB   $4C                     ; 8337:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 8338:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 8339:  'A' 41             Invalid ; 
                   // FCB   $04                     ; 833A:  ' ' 04 CE          LSR   <$CE ; 
  case 0x833B:
                                                     #ifdef DEBUG
                                      mon("L833B    LDU   #$8375                  ; 833B: CE 83 75      \n");
                                      #endif
    U = 0x8375; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x833E; 
    JUMP; // sstep
  case 0x833E:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8357                  ; 833E: 8E 83 57      \n");
                                      #endif
    X = 0x8357; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8341; 
    JUMP; // sstep
  case 0x8341:
                                                     #ifdef DEBUG
                                      mon("L8341    LDA   ,X+                     ; 8341: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8343; 
    JUMP; // sstep
  case 0x8343:
                                                     #ifdef DEBUG
                                      mon("         BITA  ,X+                     ; 8343: A5 80         \n");
                                      #endif
    res = A & rd_mem(X); 
    Z = res; 
    N = res; 
    V = 0; 
    X = X + 1; 
    PC = 0x8345; 
    JUMP; // sstep
  case 0x8345:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 8345: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8347; 
    JUMP; // sstep
  case 0x8347:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0E                    ; 8347: C4 0E         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0e; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8349; 
    JUMP; // sstep
  case 0x8349:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8349: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x834B; 
    JUMP; // sstep
  case 0x834B:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 834B: 10 26 10 26   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x834F; 
    JUMP; // sstep
  case 0x834F:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8375                  ; 834F: 8C 83 75      \n");
                                      #endif
    val = X; 
    arg = 0x8375; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8352; 
    JUMP; // sstep
  case 0x8352:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $8341                   ; 8352: 10 26 FF EB   \n");
                                      #endif
    if (Z) {
    PC = 0x8341; 
    JUMP;
    }
    PC = 0x8356; 
    JUMP; // sstep
  case 0x8356:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8356: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8357:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8358:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8359:  ' ' 04 AA          LSR   <$AA ; 
                   // FCB   $AA                     ; 835A:  ' ' AA 55          ORA   -11,U ; 
                   // FCB   $55                     ; 835B:  'U' 55             Invalid ; 
                   // FCB   $04                     ; 835C:  ' ' 04 AA          LSR   <$AA ; 
                   // FCB   $AA                     ; 835D:  ' ' AA AA          ORA   ??? ; 
                   // FCB   $AA                     ; 835E:  ' ' AA 08          ORA   8,X ; 
                   // FCB   $08                     ; 835F:  ' ' 08 55          ASL   <$55 ; 
                   // FCB   $55                     ; 8360:  'U' 55             Invalid ; 
                   // FCB   $55                     ; 8361:  'U' 55             Invalid ; 
                   // FCB   $00                     ; 8362:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8363:  ' ' FF FF 08       STU   $FF08 ; 
                   // FCB   $FF                     ; 8364:  ' ' FF 08 FF       STU   $08FF ; 
                   // FCB   $08                     ; 8365:  ' ' 08 FF          ASL   <$FF ; 
                   // FCB   $FF                     ; 8366:  ' ' FF 80 08       STU   $8008 ; 
                   // FCB   $80                     ; 8367:  ' ' 80 08          SUBA  #$08 ; 
                   // FCB   $08                     ; 8368:  ' ' 08 81          ASL   <$81 ; 
                   // FCB   $81                     ; 8369:  ' ' 81 80          CMPA  #$80 ; 
                   // FCB   $80                     ; 836A:  ' ' 80 08          SUBA  #$08 ; 
                   // FCB   $08                     ; 836B:  ' ' 08 FF          ASL   <$FF ; 
                   // FCB   $FF                     ; 836C:  ' ' FF 7F 00       STU   $7F00 ; 
                   // FCB   $7F                     ; 836D:  ' ' 7F 00 FF       CLR   $00FF ; 
                   // FCB   $00                     ; 836E:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 836F:  ' ' FF 01 00       STU   $0100 ; 
                   // FCB   $01                     ; 8370:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8371:  ' ' 00 F0          NEG   <$F0 ; 
                   // FCB   $F0                     ; 8372:  ' ' F0 0F 04       SUBB  $0F04 ; 
                   // FCB   $0F                     ; 8373:  ' ' 0F 04          CLR   <$04 ; 
                   // FCB   $04                     ; 8374:  ' ' 04 42          LSR   <$42 ; 
                   // FCB   $42                     ; 8375:  'B' 42             Invalid ; 
                   // FCB   $49                     ; 8376:  'I' 49             ROLA  ; 
                   // FCB   $54                     ; 8377:  'T' 54             LSRB  ; 
                   // FCB   $04                     ; 8378:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8379:
                                                     #ifdef DEBUG
                                      mon("L8379    LDU   #$84E2                  ; 8379: CE 84 E2      \n");
                                      #endif
    U = 0x84e2; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x837C; 
    JUMP; // sstep
  case 0x837C:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$83A2                  ; 837C: 8E 83 A2      \n");
                                      #endif
    X = 0x83a2; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x837F; 
    JUMP; // sstep
  case 0x837F:
                                                     #ifdef DEBUG
                                      mon("L837F    LDA   ,X+                     ; 837F: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
    X = X + 1; 
    PC = 0x8381; 
    JUMP; // sstep
  case 0x8381:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 8381: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8383; 
    JUMP; // sstep
  case 0x8383:
                                                     #ifdef DEBUG
                                      mon("         ANDB  ,X+                     ; 8383: E4 80         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8385; 
    JUMP; // sstep
  case 0x8385:
                                                     #ifdef DEBUG
                                      mon("         ORB   ,X+                     ; 8385: EA 80         \n");
                                      #endif
    res = B | (UINT8)rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8387; 
    JUMP; // sstep
  case 0x8387:
                                                     #ifdef DEBUG
                                      mon("         TFR   B,CC                    ; 8387: 1F 9A         \n");
                                      #endif
    CC = B; 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8389; 
    JUMP; // sstep
  case 0x8389:
                                                     #ifdef DEBUG
                                      mon("         DAA                           ; 8389: 19            \n");
                                      #endif
    res = A; 
    msn = res & 0xf0; 
    lsn = res & 0x0f; 
    res = res + (((lsn > 0x09) || (H & 0x10)) ? 0x06 : 0); 
    res = res + ((((UINT8)msn > 0x80U) && ((UINT8)lsn > 0x09U)) || (((UINT8)msn > 0x90U) || (C != 0)) ? 0x60 : 0); 
    C = C | (res & 0x100); 
    res = (UINT8)res; 
    A = res; 
    N = res; 
    Z = res; 
    V = 0; 
    PC = 0x838A; 
    JUMP; // sstep
  case 0x838A:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 838A: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x838C; 
    JUMP; // sstep
  case 0x838C:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 838C: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x838E; 
    JUMP; // sstep
  case 0x838E:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 838E: 10 26 0F E3   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8392; 
    JUMP; // sstep
  case 0x8392:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0D                    ; 8392: C4 0D         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0d; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8394; 
    JUMP; // sstep
  case 0x8394:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8394: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8396; 
    JUMP; // sstep
  case 0x8396:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8396: 10 26 0F DB   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x839A; 
    JUMP; // sstep
  case 0x839A:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$84E2                  ; 839A: 8C 84 E2      \n");
                                      #endif
    val = X; 
    arg = 0x84e2; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x839D; 
    JUMP; // sstep
  case 0x839D:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $837F                   ; 839D: 10 26 FF DE   \n");
                                      #endif
    if (Z) {
    PC = 0x837f; 
    JUMP;
    }
    PC = 0x83A1; 
    JUMP; // sstep
  case 0x83A1:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 83A1: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 83A2:  ' ' 00 DE          NEG   <$DE ; 
                   // FCB   $DE                     ; 83A3:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83A4:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 83A5:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 83A6:  ' ' 04 01          LSR   <$01 ; 
                   // FCB   $01                     ; 83A7:  ' ' 01             Invalid ; 
                   // FCB   $DE                     ; 83A8:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83A9:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 83AA:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 83AB:  ' ' 00 09          NEG   <$09 ; 
                   // FCB   $09                     ; 83AC:  ' ' 09 DE          ROL   <$DE ; 
                   // FCB   $DE                     ; 83AD:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83AE:  ' ' 00 09          NEG   <$09 ; 
                   // FCB   $09                     ; 83AF:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 83B0:  ' ' 00 0A          NEG   <$0A ; 
                   // FCB   $0A                     ; 83B1:  ' ' 0A DE          DEC   <$DE ; 
                   // FCB   $DE                     ; 83B2:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83B3:  ' ' 00 10          NEG   <$10 ; 
                   // FCB   $10                     ; 83B4:  ' ' 10 00          Invalid ; 
                   // FCB   $00                     ; 83B5:  ' ' 00 0F          NEG   <$0F ; 
                   // FCB   $0F                     ; 83B6:  ' ' 0F DE          CLR   <$DE ; 
                   // FCB   $DE                     ; 83B7:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83B8:  ' ' 00 15          NEG   <$15 ; 
                   // FCB   $15                     ; 83B9:  ' ' 15             Invalid ; 
                   // FCB   $00                     ; 83BA:  ' ' 00 10          NEG   <$10 ; 
                   // FCB   $10                     ; 83BB:  ' ' 10 DE 00       LDS   <$00 ; 
                   // FCB   $DE                     ; 83BC:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83BD:  ' ' 00 10          NEG   <$10 ; 
                   // FCB   $10                     ; 83BE:  ' ' 10 00          Invalid ; 
                   // FCB   $00                     ; 83BF:  ' ' 00 4A          NEG   <$4A ; 
                   // FCB   $4A                     ; 83C0:  'J' 4A             DECA  ; 
                   // FCB   $DE                     ; 83C1:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83C2:  ' ' 00 50          NEG   <$50 ; 
                   // FCB   $50                     ; 83C3:  'P' 50             NEGB  ; 
                   // FCB   $00                     ; 83C4:  ' ' 00 79          NEG   <$79 ; 
                   // FCB   $79                     ; 83C5:  'y' 79 DE 00       ROL   $DE00 ; 
                   // FCB   $DE                     ; 83C6:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83C7:  ' ' 00 79          NEG   <$79 ; 
                   // FCB   $79                     ; 83C8:  'y' 79 00 7A       ROL   $007A ; 
                   // FCB   $00                     ; 83C9:  ' ' 00 7A          NEG   <$7A ; 
                   // FCB   $7A                     ; 83CA:  'z' 7A DE 00       DEC   $DE00 ; 
                   // FCB   $DE                     ; 83CB:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83CC:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 83CD:  ' ' 80 08          SUBA  #$08 ; 
                   // FCB   $08                     ; 83CE:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 83CF:  ' ' 7F DE 00       CLR   $DE00 ; 
                   // FCB   $DE                     ; 83D0:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83D1:  ' ' 00 85          NEG   <$85 ; 
                   // FCB   $85                     ; 83D2:  ' ' 85 08          BITA  #$08 ; 
                   // FCB   $08                     ; 83D3:  ' ' 08 81          ASL   <$81 ; 
                   // FCB   $81                     ; 83D4:  ' ' 81 DE          CMPA  #$DE ; 
                   // FCB   $DE                     ; 83D5:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83D6:  ' ' 00 81          NEG   <$81 ; 
                   // FCB   $81                     ; 83D7:  ' ' 81 08          CMPA  #$08 ; 
                   // FCB   $08                     ; 83D8:  ' ' 08 99          ASL   <$99 ; 
                   // FCB   $99                     ; 83D9:  ' ' 99 DE          ADCA  <$DE ; 
                   // FCB   $DE                     ; 83DA:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83DB:  ' ' 00 99          NEG   <$99 ; 
                   // FCB   $99                     ; 83DC:  ' ' 99 08          ADCA  <$08 ; 
                   // FCB   $08                     ; 83DD:  ' ' 08 A0          ASL   <$A0 ; 
                   // FCB   $A0                     ; 83DE:  ' ' A0 DE          SUBA  ??? ; 
                   // FCB   $DE                     ; 83DF:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83E0:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 83E1:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 83E2:  ' ' 05             Invalid ; 
                   // FCB   $BF                     ; 83E3:  ' ' BF DE 00       STX   $DE00 ; 
                   // FCB   $DE                     ; 83E4:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83E5:  ' ' 00 25          NEG   <$25 ; 
                   // FCB   $25                     ; 83E6:  '%' 25 01          BCS   $83E9 ; 
                   // FCB   $01                     ; 83E7:  ' ' 01             Invalid ; 
                   // FCB   $F0                     ; 83E8:  ' ' F0 DE 00       SUBB  $DE00 ; 
                   // FCB   $DE                     ; 83E9:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83EA:  ' ' 00 50          NEG   <$50 ; 
                   // FCB   $50                     ; 83EB:  'P' 50             NEGB  ; 
                   // FCB   $01                     ; 83EC:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 83ED:  ' ' FF DE 00       STU   $DE00 ; 
                   // FCB   $DE                     ; 83EE:  ' ' DE 00          LDU   <$00 ; 
                   // FCB   $00                     ; 83EF:  ' ' 00 65          NEG   <$65 ; 
                   // FCB   $65                     ; 83F0:  'e' 65             Invalid ; 
                   // FCB   $01                     ; 83F1:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 83F2:  ' ' 00 DF          NEG   <$DF ; 
                   // FCB   $DF                     ; 83F3:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 83F4:  ' ' 01             Invalid ; 
                   // FCB   $60                     ; 83F5:  '`' 60 01          NEG   1,X ; 
                   // FCB   $01                     ; 83F6:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 83F7:  ' ' 01             Invalid ; 
                   // FCB   $DF                     ; 83F8:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 83F9:  ' ' 01             Invalid ; 
                   // FCB   $61                     ; 83FA:  'a' 61             Invalid ; 
                   // FCB   $01                     ; 83FB:  ' ' 01             Invalid ; 
                   // FCB   $09                     ; 83FC:  ' ' 09 DF          ROL   <$DF ; 
                   // FCB   $DF                     ; 83FD:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 83FE:  ' ' 01             Invalid ; 
                   // FCB   $69                     ; 83FF:  'i' 69 01          ROL   1,X ; 
                   // FCB   $01                     ; 8400:  ' ' 01             Invalid ; 
                   // FCB   $0A                     ; 8401:  ' ' 0A DF          DEC   <$DF ; 
                   // FCB   $DF                     ; 8402:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 8403:  ' ' 01             Invalid ; 
                   // FCB   $70                     ; 8404:  'p' 70 01 0F       NEG   $010F ; 
                   // FCB   $01                     ; 8405:  ' ' 01             Invalid ; 
                   // FCB   $0F                     ; 8406:  ' ' 0F DF          CLR   <$DF ; 
                   // FCB   $DF                     ; 8407:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 8408:  ' ' 01             Invalid ; 
                   // FCB   $75                     ; 8409:  'u' 75             Invalid ; 
                   // FCB   $01                     ; 840A:  ' ' 01             Invalid ; 
                   // FCB   $10                     ; 840B:  ' ' 10 DF 01       STS   <$01 ; 
                   // FCB   $DF                     ; 840C:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 840D:  ' ' 01             Invalid ; 
                   // FCB   $70                     ; 840E:  'p' 70 01 4A       NEG   $014A ; 
                   // FCB   $01                     ; 840F:  ' ' 01             Invalid ; 
                   // FCB   $4A                     ; 8410:  'J' 4A             DECA  ; 
                   // FCB   $DF                     ; 8411:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 8412:  ' ' 01             Invalid ; 
                   // FCB   $B0                     ; 8413:  ' ' B0 09 79       SUBA  $0979 ; 
                   // FCB   $09                     ; 8414:  ' ' 09 79          ROL   <$79 ; 
                   // FCB   $79                     ; 8415:  'y' 79 DF 01       ROL   $DF01 ; 
                   // FCB   $DF                     ; 8416:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 8417:  ' ' 01             Invalid ; 
                   // FCB   $D9                     ; 8418:  ' ' D9 09          ADCB  <$09 ; 
                   // FCB   $09                     ; 8419:  ' ' 09 7A          ROL   <$7A ; 
                   // FCB   $7A                     ; 841A:  'z' 7A DF 01       DEC   $DF01 ; 
                   // FCB   $DF                     ; 841B:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 841C:  ' ' 01             Invalid ; 
                   // FCB   $E0                     ; 841D:  ' ' E0 09          SUBB  9,X ; 
                   // FCB   $09                     ; 841E:  ' ' 09 7F          ROL   <$7F ; 
                   // FCB   $7F                     ; 841F:  ' ' 7F DF 01       CLR   $DF01 ; 
                   // FCB   $DF                     ; 8420:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 8421:  ' ' 01             Invalid ; 
                   // FCB   $E5                     ; 8422:  ' ' E5 09          BITB  9,X ; 
                   // FCB   $09                     ; 8423:  ' ' 09 81          ROL   <$81 ; 
                   // FCB   $81                     ; 8424:  ' ' 81 DF          CMPA  #$DF ; 
                   // FCB   $DF                     ; 8425:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 8426:  ' ' 01             Invalid ; 
                   // FCB   $E1                     ; 8427:  ' ' E1 09          CMPB  9,X ; 
                   // FCB   $09                     ; 8428:  ' ' 09 99          ROL   <$99 ; 
                   // FCB   $99                     ; 8429:  ' ' 99 DF          ADCA  <$DF ; 
                   // FCB   $DF                     ; 842A:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 842B:  ' ' 01             Invalid ; 
                   // FCB   $F9                     ; 842C:  ' ' F9 09 A0       ADCB  $09A0 ; 
                   // FCB   $09                     ; 842D:  ' ' 09 A0          ROL   <$A0 ; 
                   // FCB   $A0                     ; 842E:  ' ' A0 DF          SUBA  ??? ; 
                   // FCB   $DF                     ; 842F:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 8430:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8431:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 8432:  ' ' 05             Invalid ; 
                   // FCB   $BF                     ; 8433:  ' ' BF DF 01       STX   $DF01 ; 
                   // FCB   $DF                     ; 8434:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 8435:  ' ' 01             Invalid ; 
                   // FCB   $25                     ; 8436:  '%' 25 01          BCS   $8439 ; 
                   // FCB   $01                     ; 8437:  ' ' 01             Invalid ; 
                   // FCB   $F0                     ; 8438:  ' ' F0 DF 01       SUBB  $DF01 ; 
                   // FCB   $DF                     ; 8439:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 843A:  ' ' 01             Invalid ; 
                   // FCB   $50                     ; 843B:  'P' 50             NEGB  ; 
                   // FCB   $01                     ; 843C:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 843D:  ' ' FF DF 01       STU   $DF01 ; 
                   // FCB   $DF                     ; 843E:  ' ' DF 01          STU   <$01 ; 
                   // FCB   $01                     ; 843F:  ' ' 01             Invalid ; 
                   // FCB   $65                     ; 8440:  'e' 65             Invalid ; 
                   // FCB   $01                     ; 8441:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8442:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8443:  ' ' FE 20 06       LDU   $2006 ; 
                   // FCB   $20                     ; 8444:  ' ' 20 06          BRA   $844C ; 
                   // FCB   $06                     ; 8445:  ' ' 06 00          ROR   <$00 ; 
                   // FCB   $00                     ; 8446:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8447:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 8448:  ' ' FE 20 07       LDU   $2007 ; 
                   // FCB   $20                     ; 8449:  ' ' 20 07          BRA   $8452 ; 
                   // FCB   $07                     ; 844A:  ' ' 07 00          ASR   <$00 ; 
                   // FCB   $00                     ; 844B:  ' ' 00 09          NEG   <$09 ; 
                   // FCB   $09                     ; 844C:  ' ' 09 FE          ROL   <$FE ; 
                   // FCB   $FE                     ; 844D:  ' ' FE 20 0F       LDU   $200F ; 
                   // FCB   $20                     ; 844E:  ' ' 20 0F          BRA   $845F ; 
                   // FCB   $0F                     ; 844F:  ' ' 0F 00          CLR   <$00 ; 
                   // FCB   $00                     ; 8450:  ' ' 00 0A          NEG   <$0A ; 
                   // FCB   $0A                     ; 8451:  ' ' 0A FE          DEC   <$FE ; 
                   // FCB   $FE                     ; 8452:  ' ' FE 20 10       LDU   $2010 ; 
                   // FCB   $20                     ; 8453:  ' ' 20 10          BRA   $8465 ; 
                   // FCB   $10                     ; 8454:  ' ' 10 00          Invalid ; 
                   // FCB   $00                     ; 8455:  ' ' 00 0F          NEG   <$0F ; 
                   // FCB   $0F                     ; 8456:  ' ' 0F FE          CLR   <$FE ; 
                   // FCB   $FE                     ; 8457:  ' ' FE 20 15       LDU   $2015 ; 
                   // FCB   $20                     ; 8458:  ' ' 20 15          BRA   $846F ; 
                   // FCB   $15                     ; 8459:  ' ' 15             Invalid ; 
                   // FCB   $00                     ; 845A:  ' ' 00 10          NEG   <$10 ; 
                   // FCB   $10                     ; 845B:  ' ' 10 FE 20 16    LDS   $2016 ; 
                   // FCB   $FE                     ; 845C:  ' ' FE 20 16       LDU   $2016 ; 
                   // FCB   $20                     ; 845D:  ' ' 20 16          BRA   $8475 ; 
                   // FCB   $16                     ; 845E:  ' ' 16 00 4A       LBRA  $84AB ; 
                   // FCB   $00                     ; 845F:  ' ' 00 4A          NEG   <$4A ; 
                   // FCB   $4A                     ; 8460:  'J' 4A             DECA  ; 
                   // FCB   $FE                     ; 8461:  ' ' FE 20 50       LDU   $2050 ; 
                   // FCB   $20                     ; 8462:  ' ' 20 50          BRA   $84B4 ; 
                   // FCB   $50                     ; 8463:  'P' 50             NEGB  ; 
                   // FCB   $00                     ; 8464:  ' ' 00 79          NEG   <$79 ; 
                   // FCB   $79                     ; 8465:  'y' 79 FE 20       ROL   $FE20 ; 
                   // FCB   $FE                     ; 8466:  ' ' FE 20 7F       LDU   $207F ; 
                   // FCB   $20                     ; 8467:  ' ' 20 7F          BRA   $84E8 ; 
                   // FCB   $7F                     ; 8468:  ' ' 7F 00 7A       CLR   $007A ; 
                   // FCB   $00                     ; 8469:  ' ' 00 7A          NEG   <$7A ; 
                   // FCB   $7A                     ; 846A:  'z' 7A FE 20       DEC   $FE20 ; 
                   // FCB   $FE                     ; 846B:  ' ' FE 20 80       LDU   $2080 ; 
                   // FCB   $20                     ; 846C:  ' ' 20 80          BRA   $83EE ; 
                   // FCB   $80                     ; 846D:  ' ' 80 08          SUBA  #$08 ; 
                   // FCB   $08                     ; 846E:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 846F:  ' ' 7F FE 20       CLR   $FE20 ; 
                   // FCB   $FE                     ; 8470:  ' ' FE 20 85       LDU   $2085 ; 
                   // FCB   $20                     ; 8471:  ' ' 20 85          BRA   $83F8 ; 
                   // FCB   $85                     ; 8472:  ' ' 85 08          BITA  #$08 ; 
                   // FCB   $08                     ; 8473:  ' ' 08 81          ASL   <$81 ; 
                   // FCB   $81                     ; 8474:  ' ' 81 FE          CMPA  #$FE ; 
                   // FCB   $FE                     ; 8475:  ' ' FE 20 87       LDU   $2087 ; 
                   // FCB   $20                     ; 8476:  ' ' 20 87          BRA   $83FF ; 
                   // FCB   $87                     ; 8477:  ' ' 87             Invalid ; 
                   // FCB   $08                     ; 8478:  ' ' 08 99          ASL   <$99 ; 
                   // FCB   $99                     ; 8479:  ' ' 99 FE          ADCA  <$FE ; 
                   // FCB   $FE                     ; 847A:  ' ' FE 20 9F       LDU   $209F ; 
                   // FCB   $20                     ; 847B:  ' ' 20 9F          BRA   $841C ; 
                   // FCB   $9F                     ; 847C:  ' ' 9F 08          STX   <$08 ; 
                   // FCB   $08                     ; 847D:  ' ' 08 A0          ASL   <$A0 ; 
                   // FCB   $A0                     ; 847E:  ' ' A0 FE          SUBA  ??? ; 
                   // FCB   $FE                     ; 847F:  ' ' FE 20 06       LDU   $2006 ; 
                   // FCB   $20                     ; 8480:  ' ' 20 06          BRA   $8488 ; 
                   // FCB   $06                     ; 8481:  ' ' 06 01          ROR   <$01 ; 
                   // FCB   $01                     ; 8482:  ' ' 01             Invalid ; 
                   // FCB   $BF                     ; 8483:  ' ' BF FE 20       STX   $FE20 ; 
                   // FCB   $FE                     ; 8484:  ' ' FE 20 25       LDU   $2025 ; 
                   // FCB   $20                     ; 8485:  ' ' 20 25          BRA   $84AC ; 
                   // FCB   $25                     ; 8486:  '%' 25 01          BCS   $8489 ; 
                   // FCB   $01                     ; 8487:  ' ' 01             Invalid ; 
                   // FCB   $F0                     ; 8488:  ' ' F0 FE 20       SUBB  $FE20 ; 
                   // FCB   $FE                     ; 8489:  ' ' FE 20 56       LDU   $2056 ; 
                   // FCB   $20                     ; 848A:  ' ' 20 56          BRA   $84E2 ; 
                   // FCB   $56                     ; 848B:  'V' 56             RORB  ; 
                   // FCB   $01                     ; 848C:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 848D:  ' ' FF FE 20       STU   $FE20 ; 
                   // FCB   $FE                     ; 848E:  ' ' FE 20 65       LDU   $2065 ; 
                   // FCB   $20                     ; 848F:  ' ' 20 65          BRA   $84F6 ; 
                   // FCB   $65                     ; 8490:  'e' 65             Invalid ; 
                   // FCB   $01                     ; 8491:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8492:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8493:  ' ' FF 21 66       STU   $2166 ; 
                   // FCB   $21                     ; 8494:  '!' 21 66          BRN   $84FC ; 
                   // FCB   $66                     ; 8495:  'f' 66 01          ROR   1,X ; 
                   // FCB   $01                     ; 8496:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8497:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8498:  ' ' FF 21 67       STU   $2167 ; 
                   // FCB   $21                     ; 8499:  '!' 21 67          BRN   $8502 ; 
                   // FCB   $67                     ; 849A:  'g' 67 01          ASR   1,X ; 
                   // FCB   $01                     ; 849B:  ' ' 01             Invalid ; 
                   // FCB   $09                     ; 849C:  ' ' 09 FF          ROL   <$FF ; 
                   // FCB   $FF                     ; 849D:  ' ' FF 21 6F       STU   $216F ; 
                   // FCB   $21                     ; 849E:  '!' 21 6F          BRN   $850F ; 
                   // FCB   $6F                     ; 849F:  'o' 6F 01          CLR   1,X ; 
                   // FCB   $01                     ; 84A0:  ' ' 01             Invalid ; 
                   // FCB   $0A                     ; 84A1:  ' ' 0A FF          DEC   <$FF ; 
                   // FCB   $FF                     ; 84A2:  ' ' FF 21 70       STU   $2170 ; 
                   // FCB   $21                     ; 84A3:  '!' 21 70          BRN   $8515 ; 
                   // FCB   $70                     ; 84A4:  'p' 70 01 0F       NEG   $010F ; 
                   // FCB   $01                     ; 84A5:  ' ' 01             Invalid ; 
                   // FCB   $0F                     ; 84A6:  ' ' 0F FF          CLR   <$FF ; 
                   // FCB   $FF                     ; 84A7:  ' ' FF 21 75       STU   $2175 ; 
                   // FCB   $21                     ; 84A8:  '!' 21 75          BRN   $851F ; 
                   // FCB   $75                     ; 84A9:  'u' 75             Invalid ; 
                   // FCB   $01                     ; 84AA:  ' ' 01             Invalid ; 
                   // FCB   $10                     ; 84AB:  ' ' 10 FF 21 76    STS   $2176 ; 
                   // FCB   $FF                     ; 84AC:  ' ' FF 21 76       STU   $2176 ; 
                   // FCB   $21                     ; 84AD:  '!' 21 76          BRN   $8525 ; 
                   // FCB   $76                     ; 84AE:  'v' 76 01 4A       ROR   $014A ; 
                   // FCB   $01                     ; 84AF:  ' ' 01             Invalid ; 
                   // FCB   $4A                     ; 84B0:  'J' 4A             DECA  ; 
                   // FCB   $FF                     ; 84B1:  ' ' FF 21 B0       STU   $21B0 ; 
                   // FCB   $21                     ; 84B2:  '!' 21 B0          BRN   $8464 ; 
                   // FCB   $B0                     ; 84B3:  ' ' B0 09 79       SUBA  $0979 ; 
                   // FCB   $09                     ; 84B4:  ' ' 09 79          ROL   <$79 ; 
                   // FCB   $79                     ; 84B5:  'y' 79 FF 21       ROL   $FF21 ; 
                   // FCB   $FF                     ; 84B6:  ' ' FF 21 DF       STU   $21DF ; 
                   // FCB   $21                     ; 84B7:  '!' 21 DF          BRN   $8498 ; 
                   // FCB   $DF                     ; 84B8:  ' ' DF 09          STU   <$09 ; 
                   // FCB   $09                     ; 84B9:  ' ' 09 7A          ROL   <$7A ; 
                   // FCB   $7A                     ; 84BA:  'z' 7A FF 21       DEC   $FF21 ; 
                   // FCB   $FF                     ; 84BB:  ' ' FF 21 E0       STU   $21E0 ; 
                   // FCB   $21                     ; 84BC:  '!' 21 E0          BRN   $849E ; 
                   // FCB   $E0                     ; 84BD:  ' ' E0 09          SUBB  9,X ; 
                   // FCB   $09                     ; 84BE:  ' ' 09 7F          ROL   <$7F ; 
                   // FCB   $7F                     ; 84BF:  ' ' 7F FF 21       CLR   $FF21 ; 
                   // FCB   $FF                     ; 84C0:  ' ' FF 21 E5       STU   $21E5 ; 
                   // FCB   $21                     ; 84C1:  '!' 21 E5          BRN   $84A8 ; 
                   // FCB   $E5                     ; 84C2:  ' ' E5 09          BITB  9,X ; 
                   // FCB   $09                     ; 84C3:  ' ' 09 81          ROL   <$81 ; 
                   // FCB   $81                     ; 84C4:  ' ' 81 FF          CMPA  #$FF ; 
                   // FCB   $FF                     ; 84C5:  ' ' FF 21 E7       STU   $21E7 ; 
                   // FCB   $21                     ; 84C6:  '!' 21 E7          BRN   $84AF ; 
                   // FCB   $E7                     ; 84C7:  ' ' E7 09          STB   9,X ; 
                   // FCB   $09                     ; 84C8:  ' ' 09 99          ROL   <$99 ; 
                   // FCB   $99                     ; 84C9:  ' ' 99 FF          ADCA  <$FF ; 
                   // FCB   $FF                     ; 84CA:  ' ' FF 21 FF       STU   $21FF ; 
                   // FCB   $21                     ; 84CB:  '!' 21 FF          BRN   $84CC ; 
                   // FCB   $FF                     ; 84CC:  ' ' FF 09 A0       STU   $09A0 ; 
                   // FCB   $09                     ; 84CD:  ' ' 09 A0          ROL   <$A0 ; 
                   // FCB   $A0                     ; 84CE:  ' ' A0 FF          SUBA  ??? ; 
                   // FCB   $FF                     ; 84CF:  ' ' FF 21 06       STU   $2106 ; 
                   // FCB   $21                     ; 84D0:  '!' 21 06          BRN   $84D8 ; 
                   // FCB   $06                     ; 84D1:  ' ' 06 01          ROR   <$01 ; 
                   // FCB   $01                     ; 84D2:  ' ' 01             Invalid ; 
                   // FCB   $BF                     ; 84D3:  ' ' BF FF 21       STX   $FF21 ; 
                   // FCB   $FF                     ; 84D4:  ' ' FF 21 25       STU   $2125 ; 
                   // FCB   $21                     ; 84D5:  '!' 21 25          BRN   $84FC ; 
                   // FCB   $25                     ; 84D6:  '%' 25 01          BCS   $84D9 ; 
                   // FCB   $01                     ; 84D7:  ' ' 01             Invalid ; 
                   // FCB   $F0                     ; 84D8:  ' ' F0 FF 21       SUBB  $FF21 ; 
                   // FCB   $FF                     ; 84D9:  ' ' FF 21 56       STU   $2156 ; 
                   // FCB   $21                     ; 84DA:  '!' 21 56          BRN   $8532 ; 
                   // FCB   $56                     ; 84DB:  'V' 56             RORB  ; 
                   // FCB   $01                     ; 84DC:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 84DD:  ' ' FF FF 21       STU   $FF21 ; 
                   // FCB   $FF                     ; 84DE:  ' ' FF 21 65       STU   $2165 ; 
                   // FCB   $21                     ; 84DF:  '!' 21 65          BRN   $8546 ; 
                   // FCB   $65                     ; 84E0:  'e' 65             Invalid ; 
                   // FCB   $01                     ; 84E1:  ' ' 01             Invalid ; 
                   // FCB   $44                     ; 84E2:  'D' 44             LSRA  ; 
                   // FCB   $41                     ; 84E3:  'A' 41             Invalid ; 
                   // FCB   $41                     ; 84E4:  'A' 41             Invalid ; 
                   // FCB   $04                     ; 84E5:  ' ' 04 CE          LSR   <$CE ; 
  case 0x84E6:
                                                     #ifdef DEBUG
                                      mon("L84E6    LDU   #$851F                  ; 84E6: CE 85 1F      \n");
                                      #endif
    U = 0x851f; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x84E9; 
    JUMP; // sstep
  case 0x84E9:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8507                  ; 84E9: 8E 85 07      \n");
                                      #endif
    X = 0x8507; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x84EC; 
    JUMP; // sstep
  case 0x84EC:
                                                     #ifdef DEBUG
                                      mon("L84EC    LDA   ,X+                     ; 84EC: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x84EE; 
    JUMP; // sstep
  case 0x84EE:
                                                     #ifdef DEBUG
                                      mon("         NEGA                          ; 84EE: 40            \n");
                                      #endif
    val = (UINT8)A; 
    res = -val; 
    V = val & (UINT8)res; 
    C = res & 0x100; 
    val = (UINT8)res; 
    A = val; 
    Z = val; 
    N = val; 
    PC = 0x84EF; 
    JUMP; // sstep
  case 0x84EF:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 84EF: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x84F1; 
    JUMP; // sstep
  case 0x84F1:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 84F1: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x84F3; 
    JUMP; // sstep
  case 0x84F3:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 84F3: 10 26 0E 7E   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x84F7; 
    JUMP; // sstep
  case 0x84F7:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0F                    ; 84F7: C4 0F         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0f; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x84F9; 
    JUMP; // sstep
  case 0x84F9:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 84F9: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x84FB; 
    JUMP; // sstep
  case 0x84FB:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 84FB: 10 26 0E 76   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x84FF; 
    JUMP; // sstep
  case 0x84FF:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$851F                  ; 84FF: 8C 85 1F      \n");
                                      #endif
    val = X; 
    arg = 0x851f; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8502; 
    JUMP; // sstep
  case 0x8502:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $84EC                   ; 8502: 10 26 FF E6   \n");
                                      #endif
    if (Z) {
    PC = 0x84ec; 
    JUMP;
    }
    PC = 0x8506; 
    JUMP; // sstep
  case 0x8506:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8506: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8507:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8508:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8509:  ' ' 04 80          LSR   <$80 ; 
                   // FCB   $80                     ; 850A:  ' ' 80 80          SUBA  #$80 ; 
                   // FCB   $80                     ; 850B:  ' ' 80 0B          SUBA  #$0B ; 
                   // FCB   $0B                     ; 850C:  ' ' 0B             Invalid ; 
                   // FCB   $01                     ; 850D:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 850E:  ' ' FF 09 FF       STU   $09FF ; 
                   // FCB   $09                     ; 850F:  ' ' 09 FF          ROL   <$FF ; 
                   // FCB   $FF                     ; 8510:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 8511:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8512:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8513:  ' ' 7F 81 09       CLR   $8109 ; 
                   // FCB   $81                     ; 8514:  ' ' 81 09          CMPA  #$09 ; 
                   // FCB   $09                     ; 8515:  ' ' 09 81          ROL   <$81 ; 
                   // FCB   $81                     ; 8516:  ' ' 81 7F          CMPA  #$7F ; 
                   // FCB   $7F                     ; 8517:  ' ' 7F 01 C0       CLR   $01C0 ; 
                   // FCB   $01                     ; 8518:  ' ' 01             Invalid ; 
                   // FCB   $C0                     ; 8519:  ' ' C0 40          SUBB  #$40 ; 
                   // FCB   $40                     ; 851A:  '@' 40             NEGA  ; 
                   // FCB   $01                     ; 851B:  ' ' 01             Invalid ; 
                   // FCB   $40                     ; 851C:  '@' 40             NEGA  ; 
                   // FCB   $C0                     ; 851D:  ' ' C0 09          SUBB  #$09 ; 
                   // FCB   $09                     ; 851E:  ' ' 09 4E          ROL   <$4E ; 
                   // FCB   $4E                     ; 851F:  'N' 4E             Invalid ; 
                   // FCB   $45                     ; 8520:  'E' 45             Invalid ; 
                   // FCB   $47                     ; 8521:  'G' 47             ASRA  ; 
                   // FCB   $04                     ; 8522:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8523:
                                                     #ifdef DEBUG
                                      mon("L8523    LDU   #$8560                  ; 8523: CE 85 60      \n");
                                      #endif
    U = 0x8560; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8526; 
    JUMP; // sstep
  case 0x8526:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8542                  ; 8526: 8E 85 42      \n");
                                      #endif
    X = 0x8542; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8529; 
    JUMP; // sstep
  case 0x8529:
                                                     #ifdef DEBUG
                                      mon("L8529    LDA   ,X+                     ; 8529: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x852B; 
    JUMP; // sstep
  case 0x852B:
                                                     #ifdef DEBUG
                                      mon("         COMA                          ; 852B: 43            \n");
                                      #endif
    val = (UINT8)A ^ 0xff; 
    A = val; 
    Z = val; 
    N = val; 
    V = 0; 
    C = 1; 
    PC = 0x852C; 
    JUMP; // sstep
  case 0x852C:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 852C: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x852E; 
    JUMP; // sstep
  case 0x852E:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 852E: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8530; 
    JUMP; // sstep
  case 0x8530:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8530: 10 26 0E 41   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8534; 
    JUMP; // sstep
  case 0x8534:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0F                    ; 8534: C4 0F         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0f; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8536; 
    JUMP; // sstep
  case 0x8536:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8536: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8538; 
    JUMP; // sstep
  case 0x8538:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8538: 10 26 0E 39   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x853C; 
    JUMP; // sstep
  case 0x853C:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8560                  ; 853C: 8C 85 60      \n");
                                      #endif
    val = X; 
    arg = 0x8560; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x853F; 
    JUMP; // sstep
  case 0x853F:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8529                   ; 853F: 26 E8         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8529; 
    JUMP;
    }
    PC = 0x8541; 
    JUMP; // sstep
  case 0x8541:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8541: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8542:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8543:  ' ' FF 09 FF       STU   $09FF ; 
                   // FCB   $09                     ; 8544:  ' ' 09 FF          ROL   <$FF ; 
                   // FCB   $FF                     ; 8545:  ' ' FF 00 05       STU   $0005 ; 
                   // FCB   $00                     ; 8546:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 8547:  ' ' 05             Invalid ; 
                   // FCB   $F0                     ; 8548:  ' ' F0 0F 01       SUBB  $0F01 ; 
                   // FCB   $0F                     ; 8549:  ' ' 0F 01          CLR   <$01 ; 
                   // FCB   $01                     ; 854A:  ' ' 01             Invalid ; 
                   // FCB   $0F                     ; 854B:  ' ' 0F F0          CLR   <$F0 ; 
                   // FCB   $F0                     ; 854C:  ' ' F0 09 55       SUBB  $0955 ; 
                   // FCB   $09                     ; 854D:  ' ' 09 55          ROL   <$55 ; 
                   // FCB   $55                     ; 854E:  'U' 55             Invalid ; 
                   // FCB   $AA                     ; 854F:  ' ' AA 09          ORA   9,X ; 
                   // FCB   $09                     ; 8550:  ' ' 09 AA          ROL   <$AA ; 
                   // FCB   $AA                     ; 8551:  ' ' AA 55          ORA   -11,U ; 
                   // FCB   $55                     ; 8552:  'U' 55             Invalid ; 
                   // FCB   $01                     ; 8553:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8554:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 8555:  ' ' FE 09 FE       LDU   $09FE ; 
                   // FCB   $09                     ; 8556:  ' ' 09 FE          ROL   <$FE ; 
                   // FCB   $FE                     ; 8557:  ' ' FE 01 01       LDU   $0101 ; 
                   // FCB   $01                     ; 8558:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8559:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 855A:  ' ' 80 7F          SUBA  #$7F ; 
                   // FCB   $7F                     ; 855B:  ' ' 7F 01 7F       CLR   $017F ; 
                   // FCB   $01                     ; 855C:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 855D:  ' ' 7F 80 09       CLR   $8009 ; 
                   // FCB   $80                     ; 855E:  ' ' 80 09          SUBA  #$09 ; 
                   // FCB   $09                     ; 855F:  ' ' 09 43          ROL   <$43 ; 
                   // FCB   $43                     ; 8560:  'C' 43             COMA  ; 
                   // FCB   $4F                     ; 8561:  'O' 4F             CLRA  ; 
                   // FCB   $4D                     ; 8562:  'M' 4D             TSTA  ; 
                   // FCB   $04                     ; 8563:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8564:
                                                     #ifdef DEBUG
                                      mon("L8564    LDU   #$85BC                  ; 8564: CE 85 BC      \n");
                                      #endif
    U = 0x85bc; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8567; 
    JUMP; // sstep
  case 0x8567:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8583                  ; 8567: 8E 85 83      \n");
                                      #endif
    X = 0x8583; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x856A; 
    JUMP; // sstep
  case 0x856A:
                                                     #ifdef DEBUG
                                      mon("L856A    LDA   ,X+                     ; 856A: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
    V = 0; 
    X = X + 1; 
    PC = 0x856C; 
    JUMP; // sstep
  case 0x856C:
                                                     #ifdef DEBUG
                                      mon("         LSRA                          ; 856C: 44            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = val >> 1; 
    A = val; 
    N = 0; 
    Z = val; 
    PC = 0x856D; 
    JUMP; // sstep
  case 0x856D:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 856D: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x856F; 
    JUMP; // sstep
  case 0x856F:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 856F: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8571; 
    JUMP; // sstep
  case 0x8571:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8571: 10 26 0E 00   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8575; 
    JUMP; // sstep
  case 0x8575:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0F                    ; 8575: C4 0F         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0f; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8577; 
    JUMP; // sstep
  case 0x8577:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8577: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8579; 
    JUMP; // sstep
  case 0x8579:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8579: 10 26 0D F8   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x857D; 
    JUMP; // sstep
  case 0x857D:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$85BC                  ; 857D: 8C 85 BC      \n");
                                      #endif
    val = X; 
    arg = 0x85bc; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8580; 
    JUMP; // sstep
  case 0x8580:
                                                     #ifdef DEBUG
                                      mon("         BNE   $856A                   ; 8580: 26 E8         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x856a; 
    JUMP;
    }
    PC = 0x8582; 
    JUMP; // sstep
  case 0x8582:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8582: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8583:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8584:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8585:  ' ' 04 FF          LSR   <$FF ; 
                   // FCB   $FF                     ; 8586:  ' ' FF 7F 01       STU   $7F01 ; 
                   // FCB   $7F                     ; 8587:  ' ' 7F 01 7F       CLR   $017F ; 
                   // FCB   $01                     ; 8588:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8589:  ' ' 7F 3F 01       CLR   $3F01 ; 
                   // FCB   $3F                     ; 858A:  '?' 3F             SWI   ; 
                   // FCB   $01                     ; 858B:  ' ' 01             Invalid ; 
                   // FCB   $3F                     ; 858C:  '?' 3F             SWI   ; 
                   // FCB   $1F                     ; 858D:  ' ' 1F 01          TFR   D,X ; 
                   // FCB   $01                     ; 858E:  ' ' 01             Invalid ; 
                   // FCB   $1F                     ; 858F:  ' ' 1F 0F          TFR   D,?? ; 
                   // FCB   $0F                     ; 8590:  ' ' 0F 01          CLR   <$01 ; 
                   // FCB   $01                     ; 8591:  ' ' 01             Invalid ; 
                   // FCB   $0F                     ; 8592:  ' ' 0F 07          CLR   <$07 ; 
                   // FCB   $07                     ; 8593:  ' ' 07 01          ASR   <$01 ; 
                   // FCB   $01                     ; 8594:  ' ' 01             Invalid ; 
                   // FCB   $07                     ; 8595:  ' ' 07 03          ASR   <$03 ; 
                   // FCB   $03                     ; 8596:  ' ' 03 01          COM   <$01 ; 
                   // FCB   $01                     ; 8597:  ' ' 01             Invalid ; 
                   // FCB   $03                     ; 8598:  ' ' 03 01          COM   <$01 ; 
                   // FCB   $01                     ; 8599:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 859A:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 859B:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 859C:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 859D:  ' ' 05             Invalid ; 
                   // FCB   $55                     ; 859E:  'U' 55             Invalid ; 
                   // FCB   $2A                     ; 859F:  '*' 2A 01          BPL   $85A2 ; 
                   // FCB   $01                     ; 85A0:  ' ' 01             Invalid ; 
                   // FCB   $AA                     ; 85A1:  ' ' AA 55          ORA   -11,U ; 
                   // FCB   $55                     ; 85A2:  'U' 55             Invalid ; 
                   // FCB   $00                     ; 85A3:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 85A4:  ' ' 80 40          SUBA  #$40 ; 
                   // FCB   $40                     ; 85A5:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 85A6:  ' ' 00 10          NEG   <$10 ; 
                   // FCB   $10                     ; 85A7:  ' ' 10 08          Invalid ; 
                   // FCB   $08                     ; 85A8:  ' ' 08 00          ASL   <$00 ; 
                   // FCB   $00                     ; 85A9:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 85AA:  ' ' C0 60          SUBB  #$60 ; 
                   // FCB   $60                     ; 85AB:  '`' 60 00          NEG   0,X ; 
                   // FCB   $00                     ; 85AC:  ' ' 00 E0          NEG   <$E0 ; 
                   // FCB   $E0                     ; 85AD:  ' ' E0 70          SUBB  -16,S ; 
                   // FCB   $70                     ; 85AE:  'p' 70 00 F0       NEG   $00F0 ; 
                   // FCB   $00                     ; 85AF:  ' ' 00 F0          NEG   <$F0 ; 
                   // FCB   $F0                     ; 85B0:  ' ' F0 78 00       SUBB  $7800 ; 
                   // FCB   $78                     ; 85B1:  'x' 78 00 F8       ASL   $00F8 ; 
                   // FCB   $00                     ; 85B2:  ' ' 00 F8          NEG   <$F8 ; 
                   // FCB   $F8                     ; 85B3:  ' ' F8 7C 00       EORB  $7C00 ; 
                   // FCB   $7C                     ; 85B4:  '|' 7C 00 FC       INC   $00FC ; 
                   // FCB   $00                     ; 85B5:  ' ' 00 FC          NEG   <$FC ; 
                   // FCB   $FC                     ; 85B6:  ' ' FC 7E 00       LDD   $7E00 ; 
                   // FCB   $7E                     ; 85B7:  '~' 7E 00 FE       JMP   $00FE ; 
                   // FCB   $00                     ; 85B8:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 85B9:  ' ' FE 7F 00       LDU   $7F00 ; 
                   // FCB   $7F                     ; 85BA:  ' ' 7F 00 4C       CLR   $004C ; 
                   // FCB   $00                     ; 85BB:  ' ' 00 4C          NEG   <$4C ; 
                   // FCB   $4C                     ; 85BC:  'L' 4C             INCA  ; 
                   // FCB   $53                     ; 85BD:  'S' 53             COMB  ; 
                   // FCB   $52                     ; 85BE:  'R' 52             Invalid ; 
                   // FCB   $04                     ; 85BF:  ' ' 04 CE          LSR   <$CE ; 
  case 0x85C0:
                                                     #ifdef DEBUG
                                      mon("L85C0    LDU   #$8614                  ; 85C0: CE 86 14      \n");
                                      #endif
    U = 0x8614; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x85C3; 
    JUMP; // sstep
  case 0x85C3:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$85DB                  ; 85C3: 8E 85 DB      \n");
                                      #endif
    X = 0x85db; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x85C6; 
    JUMP; // sstep
  case 0x85C6:
                                                     #ifdef DEBUG
                                      mon("L85C6    LDA   ,X+                     ; 85C6: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x85C8; 
    JUMP; // sstep
  case 0x85C8:
                                                     #ifdef DEBUG
                                      mon("         ASLA                          ; 85C8: 48            \n");
                                      #endif
    val = (UINT8)A; 
    res = val << 1; 
    C = res & 0x100; 
    res = (UINT8)res; 
    Z = res; 
    N = res; 
    V = val ^ res; 
    A = res; 
    PC = 0x85C9; 
    JUMP; // sstep
  case 0x85C9:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 85C9: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x85CB; 
    JUMP; // sstep
  case 0x85CB:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 85CB: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x85CD; 
    JUMP; // sstep
  case 0x85CD:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 85CD: 10 26 0D A4   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x85D1; 
    JUMP; // sstep
  case 0x85D1:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0F                    ; 85D1: C4 0F         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0f; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x85D3; 
    JUMP; // sstep
  case 0x85D3:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 85D3: E1 80         \n");
                                      #endif
/**/val = (UINT8)B; 
/**/arg = rd_mem(X); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x85D5; 
    JUMP; // sstep
  case 0x85D5:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8614                  ; 85D5: 8C 86 14      \n");
                                      #endif
    val = X; 
    arg = 0x8614; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x85D8; 
    JUMP; // sstep
  case 0x85D8:
                                                     #ifdef DEBUG
                                      mon("         BNE   $85C6                   ; 85D8: 26 EC         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x85c6; 
    JUMP;
    }
    PC = 0x85DA; 
    JUMP; // sstep
  case 0x85DA:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 85DA: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 85DB:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 85DC:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 85DD:  ' ' 04 FF          LSR   <$FF ; 
                   // FCB   $FF                     ; 85DE:  ' ' FF FE 01       STU   $FE01 ; 
                   // FCB   $FE                     ; 85DF:  ' ' FE 01 7F       LDU   $017F ; 
                   // FCB   $01                     ; 85E0:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 85E1:  ' ' 7F FE 01       CLR   $FE01 ; 
                   // FCB   $FE                     ; 85E2:  ' ' FE 01 3F       LDU   $013F ; 
                   // FCB   $01                     ; 85E3:  ' ' 01             Invalid ; 
                   // FCB   $3F                     ; 85E4:  '?' 3F             SWI   ; 
                   // FCB   $7E                     ; 85E5:  '~' 7E 01 1F       JMP   $011F ; 
                   // FCB   $01                     ; 85E6:  ' ' 01             Invalid ; 
                   // FCB   $1F                     ; 85E7:  ' ' 1F 3E          TFR   U,?? ; 
                   // FCB   $3E                     ; 85E8:  '>' 3E             RESET ; 
                   // FCB   $00                     ; 85E9:  ' ' 00 0F          NEG   <$0F ; 
                   // FCB   $0F                     ; 85EA:  ' ' 0F 1E          CLR   <$1E ; 
                   // FCB   $1E                     ; 85EB:  ' ' 1E 00          EXG   D,D ; 
                   // FCB   $00                     ; 85EC:  ' ' 00 07          NEG   <$07 ; 
                   // FCB   $07                     ; 85ED:  ' ' 07 0E          ASR   <$0E ; 
                   // FCB   $0E                     ; 85EE:  ' ' 0E 00          JMP   <$00 ; 
                   // FCB   $00                     ; 85EF:  ' ' 00 03          NEG   <$03 ; 
                   // FCB   $03                     ; 85F0:  ' ' 03 06          COM   <$06 ; 
                   // FCB   $06                     ; 85F1:  ' ' 06 00          ROR   <$00 ; 
                   // FCB   $00                     ; 85F2:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 85F3:  ' ' 01             Invalid ; 
                   // FCB   $02                     ; 85F4:  ' ' 02             Invalid ; 
                   // FCB   $00                     ; 85F5:  ' ' 00 55          NEG   <$55 ; 
                   // FCB   $55                     ; 85F6:  'U' 55             Invalid ; 
                   // FCB   $AA                     ; 85F7:  ' ' AA 00          ORA   0,X ; 
                   // FCB   $00                     ; 85F8:  ' ' 00 AA          NEG   <$AA ; 
                   // FCB   $AA                     ; 85F9:  ' ' AA 54          ORA   -12,U ; 
                   // FCB   $54                     ; 85FA:  'T' 54             LSRB  ; 
                   // FCB   $00                     ; 85FB:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 85FC:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 85FD:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 85FE:  ' ' 01             Invalid ; 
                   // FCB   $10                     ; 85FF:  ' ' 10 20          Invalid ; 
                   // FCB   $20                     ; 8600:  ' ' 20 01          BRA   $8603 ; 
                   // FCB   $01                     ; 8601:  ' ' 01             Invalid ; 
                   // FCB   $C0                     ; 8602:  ' ' C0 80          SUBB  #$80 ; 
                   // FCB   $80                     ; 8603:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8604:  ' ' 01             Invalid ; 
                   // FCB   $E0                     ; 8605:  ' ' E0 C0          SUBB  ,U+ ; 
                   // FCB   $C0                     ; 8606:  ' ' C0 01          SUBB  #$01 ; 
                   // FCB   $01                     ; 8607:  ' ' 01             Invalid ; 
                   // FCB   $F0                     ; 8608:  ' ' F0 E0 01       SUBB  $E001 ; 
                   // FCB   $E0                     ; 8609:  ' ' E0 01          SUBB  1,X ; 
                   // FCB   $01                     ; 860A:  ' ' 01             Invalid ; 
                   // FCB   $F8                     ; 860B:  ' ' F8 F0 01       EORB  $F001 ; 
                   // FCB   $F0                     ; 860C:  ' ' F0 01 FC       SUBB  $01FC ; 
                   // FCB   $01                     ; 860D:  ' ' 01             Invalid ; 
                   // FCB   $FC                     ; 860E:  ' ' FC F8 01       LDD   $F801 ; 
                   // FCB   $F8                     ; 860F:  ' ' F8 01 FE       EORB  $01FE ; 
                   // FCB   $01                     ; 8610:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 8611:  ' ' FE FC 01       LDU   $FC01 ; 
                   // FCB   $FC                     ; 8612:  ' ' FC 01 4C       LDD   $014C ; 
                   // FCB   $01                     ; 8613:  ' ' 01             Invalid ; 
                   // FCB   $4C                     ; 8614:  'L' 4C             INCA  ; 
                   // FCB   $53                     ; 8615:  'S' 53             COMB  ; 
                   // FCB   $4C                     ; 8616:  'L' 4C             INCA  ; 
                   // FCB   $04                     ; 8617:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8618:
                                                     #ifdef DEBUG
                                      mon("L8618    LDU   #$8670                  ; 8618: CE 86 70      \n");
                                      #endif
    U = 0x8670; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x861B; 
    JUMP; // sstep
  case 0x861B:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8637                  ; 861B: 8E 86 37      \n");
                                      #endif
    X = 0x8637; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x861E; 
    JUMP; // sstep
  case 0x861E:
                                                     #ifdef DEBUG
                                      mon("L861E    LDA   ,X+                     ; 861E: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
    V = 0; 
    X = X + 1; 
    PC = 0x8620; 
    JUMP; // sstep
  case 0x8620:
                                                     #ifdef DEBUG
                                      mon("         ASRA                          ; 8620: 47            \n");
                                      #endif
    val = (UINT8)A; 
    C = val & 1; 
    val = (UINT8)((SINT8)val >> 1); 
    A = val; 
    Z = val; 
    N = val; 
    PC = 0x8621; 
    JUMP; // sstep
  case 0x8621:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 8621: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8623; 
    JUMP; // sstep
  case 0x8623:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8623: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8625; 
    JUMP; // sstep
  case 0x8625:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8625: 10 26 0D 4C   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8629; 
    JUMP; // sstep
  case 0x8629:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0D                    ; 8629: C4 0D         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0d; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x862B; 
    JUMP; // sstep
  case 0x862B:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 862B: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x862D; 
    JUMP; // sstep
  case 0x862D:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 862D: 10 26 0D 44   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8631; 
    JUMP; // sstep
  case 0x8631:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8670                  ; 8631: 8C 86 70      \n");
                                      #endif
    val = X; 
    arg = 0x8670; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8634; 
    JUMP; // sstep
  case 0x8634:
                                                     #ifdef DEBUG
                                      mon("         BNE   $861E                   ; 8634: 26 E8         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x861e; 
    JUMP;
    }
    PC = 0x8636; 
    JUMP; // sstep
  case 0x8636:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8636: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8637:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8638:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8639:  ' ' 04 FF          LSR   <$FF ; 
                   // FCB   $FF                     ; 863A:  ' ' FF FF 09       STU   $FF09 ; 
                   // FCB   $FF                     ; 863B:  ' ' FF 09 7F       STU   $097F ; 
                   // FCB   $09                     ; 863C:  ' ' 09 7F          ROL   <$7F ; 
                   // FCB   $7F                     ; 863D:  ' ' 7F 3F 01       CLR   $3F01 ; 
                   // FCB   $3F                     ; 863E:  '?' 3F             SWI   ; 
                   // FCB   $01                     ; 863F:  ' ' 01             Invalid ; 
                   // FCB   $3F                     ; 8640:  '?' 3F             SWI   ; 
                   // FCB   $1F                     ; 8641:  ' ' 1F 01          TFR   D,X ; 
                   // FCB   $01                     ; 8642:  ' ' 01             Invalid ; 
                   // FCB   $1F                     ; 8643:  ' ' 1F 0F          TFR   D,?? ; 
                   // FCB   $0F                     ; 8644:  ' ' 0F 01          CLR   <$01 ; 
                   // FCB   $01                     ; 8645:  ' ' 01             Invalid ; 
                   // FCB   $0F                     ; 8646:  ' ' 0F 07          CLR   <$07 ; 
                   // FCB   $07                     ; 8647:  ' ' 07 01          ASR   <$01 ; 
                   // FCB   $01                     ; 8648:  ' ' 01             Invalid ; 
                   // FCB   $07                     ; 8649:  ' ' 07 03          ASR   <$03 ; 
                   // FCB   $03                     ; 864A:  ' ' 03 01          COM   <$01 ; 
                   // FCB   $01                     ; 864B:  ' ' 01             Invalid ; 
                   // FCB   $03                     ; 864C:  ' ' 03 01          COM   <$01 ; 
                   // FCB   $01                     ; 864D:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 864E:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 864F:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8650:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 8651:  ' ' 05             Invalid ; 
                   // FCB   $55                     ; 8652:  'U' 55             Invalid ; 
                   // FCB   $2A                     ; 8653:  '*' 2A 01          BPL   $8656 ; 
                   // FCB   $01                     ; 8654:  ' ' 01             Invalid ; 
                   // FCB   $AA                     ; 8655:  ' ' AA D5          ORA   [B,U] ; 
                   // FCB   $D5                     ; 8656:  ' ' D5 08          BITB  <$08 ; 
                   // FCB   $08                     ; 8657:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 8658:  ' ' 80 C0          SUBA  #$C0 ; 
                   // FCB   $C0                     ; 8659:  ' ' C0 08          SUBB  #$08 ; 
                   // FCB   $08                     ; 865A:  ' ' 08 10          ASL   <$10 ; 
                   // FCB   $10                     ; 865B:  ' ' 10 08          Invalid ; 
                   // FCB   $08                     ; 865C:  ' ' 08 00          ASL   <$00 ; 
                   // FCB   $00                     ; 865D:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 865E:  ' ' C0 E0          SUBB  #$E0 ; 
                   // FCB   $E0                     ; 865F:  ' ' E0 08          SUBB  8,X ; 
                   // FCB   $08                     ; 8660:  ' ' 08 E0          ASL   <$E0 ; 
                   // FCB   $E0                     ; 8661:  ' ' E0 F0          SUBB  ??? ; 
                   // FCB   $F0                     ; 8662:  ' ' F0 08 F0       SUBB  $08F0 ; 
                   // FCB   $08                     ; 8663:  ' ' 08 F0          ASL   <$F0 ; 
                   // FCB   $F0                     ; 8664:  ' ' F0 F8 08       SUBB  $F808 ; 
                   // FCB   $F8                     ; 8665:  ' ' F8 08 F8       EORB  $08F8 ; 
                   // FCB   $08                     ; 8666:  ' ' 08 F8          ASL   <$F8 ; 
                   // FCB   $F8                     ; 8667:  ' ' F8 FC 08       EORB  $FC08 ; 
                   // FCB   $FC                     ; 8668:  ' ' FC 08 FC       LDD   $08FC ; 
                   // FCB   $08                     ; 8669:  ' ' 08 FC          ASL   <$FC ; 
                   // FCB   $FC                     ; 866A:  ' ' FC FE 08       LDD   $FE08 ; 
                   // FCB   $FE                     ; 866B:  ' ' FE 08 FE       LDU   $08FE ; 
                   // FCB   $08                     ; 866C:  ' ' 08 FE          ASL   <$FE ; 
                   // FCB   $FE                     ; 866D:  ' ' FE FF 08       LDU   $FF08 ; 
                   // FCB   $FF                     ; 866E:  ' ' FF 08 41       STU   $0841 ; 
                   // FCB   $08                     ; 866F:  ' ' 08 41          ASL   <$41 ; 
                   // FCB   $41                     ; 8670:  'A' 41             Invalid ; 
                   // FCB   $53                     ; 8671:  'S' 53             COMB  ; 
                   // FCB   $52                     ; 8672:  'R' 52             Invalid ; 
                   // FCB   $04                     ; 8673:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8674:
                                                     #ifdef DEBUG
                                      mon("L8674    LDU   #$86EB                  ; 8674: CE 86 EB      \n");
                                      #endif
    U = 0x86eb; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8677; 
    JUMP; // sstep
  case 0x8677:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$869B                  ; 8677: 8E 86 9B      \n");
                                      #endif
    X = 0x869b; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x867A; 
    JUMP; // sstep
  case 0x867A:
                                                     #ifdef DEBUG
                                      mon("L867A    LDA   ,X+                     ; 867A: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
    X = X + 1; 
    PC = 0x867C; 
    JUMP; // sstep
  case 0x867C:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 867C: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x867E; 
    JUMP; // sstep
  case 0x867E:
                                                     #ifdef DEBUG
                                      mon("         ANDB  ,X+                     ; 867E: E4 80         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8680; 
    JUMP; // sstep
  case 0x8680:
                                                     #ifdef DEBUG
                                      mon("         ORB   ,X+                     ; 8680: EA 80         \n");
                                      #endif
    res = B | (UINT8)rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8682; 
    JUMP; // sstep
  case 0x8682:
                                                     #ifdef DEBUG
                                      mon("         TFR   B,CC                    ; 8682: 1F 9A         \n");
                                      #endif
    CC = B; 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8684; 
    JUMP; // sstep
  case 0x8684:
                                                     #ifdef DEBUG
                                      mon("         RORA                          ; 8684: 46            \n");
                                      #endif
    res = ((UINT8)A) | ((C != 0) ? 0x100 : 0); 
    C = res & 1; 
    res = (UINT8)(res >> 1); 
    Z = res; 
    N = res; 
    A = res; 
    PC = 0x8685; 
    JUMP; // sstep
  case 0x8685:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 8685: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8687; 
    JUMP; // sstep
  case 0x8687:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8687: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8689; 
    JUMP; // sstep
  case 0x8689:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8689: 10 26 0C E8   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x868D; 
    JUMP; // sstep
  case 0x868D:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0D                    ; 868D: C4 0D         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0d; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x868F; 
    JUMP; // sstep
  case 0x868F:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 868F: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8691; 
    JUMP; // sstep
  case 0x8691:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8691: 10 26 0C E0   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8695; 
    JUMP; // sstep
  case 0x8695:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$86EB                  ; 8695: 8C 86 EB      \n");
                                      #endif
    val = X; 
    arg = 0x86eb; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8698; 
    JUMP; // sstep
  case 0x8698:
                                                     #ifdef DEBUG
                                      mon("         BNE   $867A                   ; 8698: 26 E0         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x867a; 
    JUMP;
    }
    PC = 0x869A; 
    JUMP; // sstep
  case 0x869A:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 869A: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 869B:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 869C:  ' ' FE 00 00       LDU   $0000 ; 
                   // FCB   $00                     ; 869D:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 869E:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 869F:  ' ' 04 01          LSR   <$01 ; 
                   // FCB   $01                     ; 86A0:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 86A1:  ' ' FE 00 00       LDU   $0000 ; 
                   // FCB   $00                     ; 86A2:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 86A3:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 86A4:  ' ' 05             Invalid ; 
                   // FCB   $0F                     ; 86A5:  ' ' 0F FE          CLR   <$FE ; 
                   // FCB   $FE                     ; 86A6:  ' ' FE 00 07       LDU   $0007 ; 
                   // FCB   $00                     ; 86A7:  ' ' 00 07          NEG   <$07 ; 
                   // FCB   $07                     ; 86A8:  ' ' 07 01          ASR   <$01 ; 
                   // FCB   $01                     ; 86A9:  ' ' 01             Invalid ; 
                   // FCB   $10                     ; 86AA:  ' ' 10 FE 00 08    LDS   $0008 ; 
                   // FCB   $FE                     ; 86AB:  ' ' FE 00 08       LDU   $0008 ; 
                   // FCB   $00                     ; 86AC:  ' ' 00 08          NEG   <$08 ; 
                   // FCB   $08                     ; 86AD:  ' ' 08 00          ASL   <$00 ; 
                   // FCB   $00                     ; 86AE:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 86AF:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 86B0:  ' ' FE 00 3F       LDU   $003F ; 
                   // FCB   $00                     ; 86B1:  ' ' 00 3F          NEG   <$3F ; 
                   // FCB   $3F                     ; 86B2:  '?' 3F             SWI   ; 
                   // FCB   $01                     ; 86B3:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 86B4:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 86B5:  ' ' FE 00 40       LDU   $0040 ; 
                   // FCB   $00                     ; 86B6:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 86B7:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 86B8:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 86B9:  ' ' FE FE 00       LDU   $FE00 ; 
                   // FCB   $FE                     ; 86BA:  ' ' FE 00 7F       LDU   $007F ; 
                   // FCB   $00                     ; 86BB:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 86BC:  ' ' 7F 00 FF       CLR   $00FF ; 
                   // FCB   $00                     ; 86BD:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 86BE:  ' ' FF FE 00       STU   $FE00 ; 
                   // FCB   $FE                     ; 86BF:  ' ' FE 00 7F       LDU   $007F ; 
                   // FCB   $00                     ; 86C0:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 86C1:  ' ' 7F 01 00       CLR   $0100 ; 
                   // FCB   $01                     ; 86C2:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 86C3:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 86C4:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 86C5:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 86C6:  ' ' 80 08          SUBA  #$08 ; 
                   // FCB   $08                     ; 86C7:  ' ' 08 01          ASL   <$01 ; 
                   // FCB   $01                     ; 86C8:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 86C9:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 86CA:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 86CB:  ' ' 80 09          SUBA  #$09 ; 
                   // FCB   $09                     ; 86CC:  ' ' 09 0F          ROL   <$0F ; 
                   // FCB   $0F                     ; 86CD:  ' ' 0F FF          CLR   <$FF ; 
                   // FCB   $FF                     ; 86CE:  ' ' FF 01 87       STU   $0187 ; 
                   // FCB   $01                     ; 86CF:  ' ' 01             Invalid ; 
                   // FCB   $87                     ; 86D0:  ' ' 87             Invalid ; 
                   // FCB   $09                     ; 86D1:  ' ' 09 10          ROL   <$10 ; 
                   // FCB   $10                     ; 86D2:  ' ' 10 FF 01 88    STS   $0188 ; 
                   // FCB   $FF                     ; 86D3:  ' ' FF 01 88       STU   $0188 ; 
                   // FCB   $01                     ; 86D4:  ' ' 01             Invalid ; 
                   // FCB   $88                     ; 86D5:  ' ' 88 08          EORA  #$08 ; 
                   // FCB   $08                     ; 86D6:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 86D7:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 86D8:  ' ' FF 01 BF       STU   $01BF ; 
                   // FCB   $01                     ; 86D9:  ' ' 01             Invalid ; 
                   // FCB   $BF                     ; 86DA:  ' ' BF 09 80       STX   $0980 ; 
                   // FCB   $09                     ; 86DB:  ' ' 09 80          ROL   <$80 ; 
                   // FCB   $80                     ; 86DC:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 86DD:  ' ' FF 01 C0       STU   $01C0 ; 
                   // FCB   $01                     ; 86DE:  ' ' 01             Invalid ; 
                   // FCB   $C0                     ; 86DF:  ' ' C0 08          SUBB  #$08 ; 
                   // FCB   $08                     ; 86E0:  ' ' 08 FE          ASL   <$FE ; 
                   // FCB   $FE                     ; 86E1:  ' ' FE FF 01       LDU   $FF01 ; 
                   // FCB   $FF                     ; 86E2:  ' ' FF 01 FF       STU   $01FF ; 
                   // FCB   $01                     ; 86E3:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 86E4:  ' ' FF 08 FF       STU   $08FF ; 
                   // FCB   $08                     ; 86E5:  ' ' 08 FF          ASL   <$FF ; 
                   // FCB   $FF                     ; 86E6:  ' ' FF FF 01       STU   $FF01 ; 
                   // FCB   $FF                     ; 86E7:  ' ' FF 01 FF       STU   $01FF ; 
                   // FCB   $01                     ; 86E8:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 86E9:  ' ' FF 09 52       STU   $0952 ; 
                   // FCB   $09                     ; 86EA:  ' ' 09 52          ROL   <$52 ; 
                   // FCB   $52                     ; 86EB:  'R' 52             Invalid ; 
                   // FCB   $4F                     ; 86EC:  'O' 4F             CLRA  ; 
                   // FCB   $52                     ; 86ED:  'R' 52             Invalid ; 
                   // FCB   $04                     ; 86EE:  ' ' 04 CE          LSR   <$CE ; 
  case 0x86EF:
                                                     #ifdef DEBUG
                                      mon("L86EF    LDU   #$8762                  ; 86EF: CE 87 62      \n");
                                      #endif
    U = 0x8762; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x86F2; 
    JUMP; // sstep
  case 0x86F2:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8712                  ; 86F2: 8E 87 12      \n");
                                      #endif
    X = 0x8712; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x86F5; 
    JUMP; // sstep
  case 0x86F5:
                                                     #ifdef DEBUG
                                      mon("L86F5    LDA   ,X+                     ; 86F5: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
    X = X + 1; 
    PC = 0x86F7; 
    JUMP; // sstep
  case 0x86F7:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 86F7: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x86F9; 
    JUMP; // sstep
  case 0x86F9:
                                                     #ifdef DEBUG
                                      mon("         ANDB  ,X+                     ; 86F9: E4 80         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x86FB; 
    JUMP; // sstep
  case 0x86FB:
                                                     #ifdef DEBUG
                                      mon("         ORB   ,X+                     ; 86FB: EA 80         \n");
                                      #endif
    res = B | (UINT8)rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x86FD; 
    JUMP; // sstep
  case 0x86FD:
                                                     #ifdef DEBUG
                                      mon("         TFR   B,CC                    ; 86FD: 1F 9A         \n");
                                      #endif
    CC = B; 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x86FF; 
    JUMP; // sstep
  case 0x86FF:
                                                     #ifdef DEBUG
                                      mon("         ROLA                          ; 86FF: 49            \n");
                                      #endif
    val = (UINT8)A; 
    res = (val << 1) + (C != 0 ?1:0); 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)res ^ val; 
    A = (UINT8)res; 
    PC = 0x8700; 
    JUMP; // sstep
  case 0x8700:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 8700: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8702; 
    JUMP; // sstep
  case 0x8702:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8702: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8704; 
    JUMP; // sstep
  case 0x8704:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8704: 10 26 0C 6D   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8708; 
    JUMP; // sstep
  case 0x8708:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0D                    ; 8708: C4 0D         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0d; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x870A; 
    JUMP; // sstep
  case 0x870A:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 870A: E1 80         \n");
                                      #endif
/**/val = (UINT8)B; 
/**/arg = rd_mem(X); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x870C; 
    JUMP; // sstep
  case 0x870C:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8762                  ; 870C: 8C 87 62      \n");
                                      #endif
    val = X; 
    arg = 0x8762; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x870F; 
    JUMP; // sstep
  case 0x870F:
                                                     #ifdef DEBUG
                                      mon("         BNE   $86F5                   ; 870F: 26 E4         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x86f5; 
    JUMP;
    }
    PC = 0x8711; 
    JUMP; // sstep
  case 0x8711:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8711: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8712:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8713:  ' ' FE 00 00       LDU   $0000 ; 
                   // FCB   $00                     ; 8714:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8715:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8716:  ' ' 04 01          LSR   <$01 ; 
                   // FCB   $01                     ; 8717:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 8718:  ' ' FE 00 02       LDU   $0002 ; 
                   // FCB   $00                     ; 8719:  ' ' 00 02          NEG   <$02 ; 
                   // FCB   $02                     ; 871A:  ' ' 02             Invalid ; 
                   // FCB   $00                     ; 871B:  ' ' 00 0F          NEG   <$0F ; 
                   // FCB   $0F                     ; 871C:  ' ' 0F FE          CLR   <$FE ; 
                   // FCB   $FE                     ; 871D:  ' ' FE 00 1E       LDU   $001E ; 
                   // FCB   $00                     ; 871E:  ' ' 00 1E          NEG   <$1E ; 
                   // FCB   $1E                     ; 871F:  ' ' 1E 00          EXG   D,D ; 
                   // FCB   $00                     ; 8720:  ' ' 00 08          NEG   <$08 ; 
                   // FCB   $08                     ; 8721:  ' ' 08 FE          ASL   <$FE ; 
                   // FCB   $FE                     ; 8722:  ' ' FE 00 10       LDU   $0010 ; 
                   // FCB   $00                     ; 8723:  ' ' 00 10          NEG   <$10 ; 
                   // FCB   $10                     ; 8724:  ' ' 10 00          Invalid ; 
                   // FCB   $00                     ; 8725:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8726:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8727:  ' ' FE 00 FE       LDU   $00FE ; 
                   // FCB   $00                     ; 8728:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8729:  ' ' FE 08 80       LDU   $0880 ; 
                   // FCB   $08                     ; 872A:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 872B:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 872C:  ' ' FE 00 00       LDU   $0000 ; 
                   // FCB   $00                     ; 872D:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 872E:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 872F:  ' ' 05             Invalid ; 
                   // FCB   $FE                     ; 8730:  ' ' FE FE 00       LDU   $FE00 ; 
                   // FCB   $FE                     ; 8731:  ' ' FE 00 FC       LDU   $00FC ; 
                   // FCB   $00                     ; 8732:  ' ' 00 FC          NEG   <$FC ; 
                   // FCB   $FC                     ; 8733:  ' ' FC 09 FF       LDD   $09FF ; 
                   // FCB   $09                     ; 8734:  ' ' 09 FF          ROL   <$FF ; 
                   // FCB   $FF                     ; 8735:  ' ' FF FE 00       STU   $FE00 ; 
                   // FCB   $FE                     ; 8736:  ' ' FE 00 FE       LDU   $00FE ; 
                   // FCB   $00                     ; 8737:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8738:  ' ' FE 09 00       LDU   $0900 ; 
                   // FCB   $09                     ; 8739:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 873A:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 873B:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 873C:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 873D:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 873E:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 873F:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8740:  ' ' FF 01 03       STU   $0103 ; 
                   // FCB   $01                     ; 8741:  ' ' 01             Invalid ; 
                   // FCB   $03                     ; 8742:  ' ' 03 00          COM   <$00 ; 
                   // FCB   $00                     ; 8743:  ' ' 00 0F          NEG   <$0F ; 
                   // FCB   $0F                     ; 8744:  ' ' 0F FF          CLR   <$FF ; 
                   // FCB   $FF                     ; 8745:  ' ' FF 01 1F       STU   $011F ; 
                   // FCB   $01                     ; 8746:  ' ' 01             Invalid ; 
                   // FCB   $1F                     ; 8747:  ' ' 1F 00          TFR   D,D ; 
                   // FCB   $00                     ; 8748:  ' ' 00 08          NEG   <$08 ; 
                   // FCB   $08                     ; 8749:  ' ' 08 FF          ASL   <$FF ; 
                   // FCB   $FF                     ; 874A:  ' ' FF 01 11       STU   $0111 ; 
                   // FCB   $01                     ; 874B:  ' ' 01             Invalid ; 
                   // FCB   $11                     ; 874C:  ' ' 11 00          Invalid ; 
                   // FCB   $00                     ; 874D:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 874E:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 874F:  ' ' FF 01 FF       STU   $01FF ; 
                   // FCB   $01                     ; 8750:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8751:  ' ' FF 08 80       STU   $0880 ; 
                   // FCB   $08                     ; 8752:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 8753:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 8754:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 8755:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8756:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8757:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 8758:  ' ' FE FF 01       LDU   $FF01 ; 
                   // FCB   $FF                     ; 8759:  ' ' FF 01 FD       STU   $01FD ; 
                   // FCB   $01                     ; 875A:  ' ' 01             Invalid ; 
                   // FCB   $FD                     ; 875B:  ' ' FD 09 FF       STD   $09FF ; 
                   // FCB   $09                     ; 875C:  ' ' 09 FF          ROL   <$FF ; 
                   // FCB   $FF                     ; 875D:  ' ' FF FF 01       STU   $FF01 ; 
                   // FCB   $FF                     ; 875E:  ' ' FF 01 FF       STU   $01FF ; 
                   // FCB   $01                     ; 875F:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8760:  ' ' FF 09 52       STU   $0952 ; 
                   // FCB   $09                     ; 8761:  ' ' 09 52          ROL   <$52 ; 
                   // FCB   $52                     ; 8762:  'R' 52             Invalid ; 
                   // FCB   $4F                     ; 8763:  'O' 4F             CLRA  ; 
                   // FCB   $4C                     ; 8764:  'L' 4C             INCA  ; 
                   // FCB   $04                     ; 8765:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8766:
                                                     #ifdef DEBUG
                                      mon("L8766    LDU   #$879A                  ; 8766: CE 87 9A      \n");
                                      #endif
    U = 0x879a; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8769; 
    JUMP; // sstep
  case 0x8769:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8785                  ; 8769: 8E 87 85      \n");
                                      #endif
    X = 0x8785; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x876C; 
    JUMP; // sstep
  case 0x876C:
                                                     #ifdef DEBUG
                                      mon("L876C    LDA   ,X+                     ; 876C: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x876E; 
    JUMP; // sstep
  case 0x876E:
                                                     #ifdef DEBUG
                                      mon("         DECA                          ; 876E: 4A            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val - 1); 
    N = res; 
    Z = res; 
    V = val & ~res; 
    A = res; 
    PC = 0x876F; 
    JUMP; // sstep
  case 0x876F:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 876F: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8771; 
    JUMP; // sstep
  case 0x8771:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8771: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8773; 
    JUMP; // sstep
  case 0x8773:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8773: 10 26 0B FE   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8777; 
    JUMP; // sstep
  case 0x8777:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0E                    ; 8777: C4 0E         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0e; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8779; 
    JUMP; // sstep
  case 0x8779:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8779: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x877B; 
    JUMP; // sstep
  case 0x877B:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 877B: 10 26 0B F6   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x877F; 
    JUMP; // sstep
  case 0x877F:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$879A                  ; 877F: 8C 87 9A      \n");
                                      #endif
    val = X; 
    arg = 0x879a; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8782; 
    JUMP; // sstep
  case 0x8782:
                                                     #ifdef DEBUG
                                      mon("         BNE   $876C                   ; 8782: 26 E8         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x876c; 
    JUMP;
    }
    PC = 0x8784; 
    JUMP; // sstep
  case 0x8784:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8784: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $01                     ; 8785:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8786:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8787:  ' ' 04 00          LSR   <$00 ; 
                   // FCB   $00                     ; 8788:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8789:  ' ' FF 08 FF       STU   $08FF ; 
                   // FCB   $08                     ; 878A:  ' ' 08 FF          ASL   <$FF ; 
                   // FCB   $FF                     ; 878B:  ' ' FF FE 08       STU   $FE08 ; 
                   // FCB   $FE                     ; 878C:  ' ' FE 08 80       LDU   $0880 ; 
                   // FCB   $08                     ; 878D:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 878E:  ' ' 80 7F          SUBA  #$7F ; 
                   // FCB   $7F                     ; 878F:  ' ' 7F 02 7F       CLR   $027F ; 
                   // FCB   $02                     ; 8790:  ' ' 02             Invalid ; 
                   // FCB   $7F                     ; 8791:  ' ' 7F 7E 00       CLR   $7E00 ; 
                   // FCB   $7E                     ; 8792:  '~' 7E 00 10       JMP   $0010 ; 
                   // FCB   $00                     ; 8793:  ' ' 00 10          NEG   <$10 ; 
                   // FCB   $10                     ; 8794:  ' ' 10 0F          Invalid ; 
                   // FCB   $0F                     ; 8795:  ' ' 0F 00          CLR   <$00 ; 
                   // FCB   $00                     ; 8796:  ' ' 00 0F          NEG   <$0F ; 
                   // FCB   $0F                     ; 8797:  ' ' 0F 0E          CLR   <$0E ; 
                   // FCB   $0E                     ; 8798:  ' ' 0E 00          JMP   <$00 ; 
                   // FCB   $00                     ; 8799:  ' ' 00 44          NEG   <$44 ; 
                   // FCB   $44                     ; 879A:  'D' 44             LSRA  ; 
                   // FCB   $45                     ; 879B:  'E' 45             Invalid ; 
                   // FCB   $43                     ; 879C:  'C' 43             COMA  ; 
                   // FCB   $04                     ; 879D:  ' ' 04 CE          LSR   <$CE ; 
  case 0x879E:
                                                     #ifdef DEBUG
                                      mon("L879E    LDU   #$87D2                  ; 879E: CE 87 D2      \n");
                                      #endif
    U = 0x87d2; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x87A1; 
    JUMP; // sstep
  case 0x87A1:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$87BD                  ; 87A1: 8E 87 BD      \n");
                                      #endif
    X = 0x87bd; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x87A4; 
    JUMP; // sstep
  case 0x87A4:
                                                     #ifdef DEBUG
                                      mon("L87A4    LDA   ,X+                     ; 87A4: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x87A6; 
    JUMP; // sstep
  case 0x87A6:
                                                     #ifdef DEBUG
                                      mon("         INCA                          ; 87A6: 4C            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val + 1); 
    Z = res; 
    N = res; 
    V = res & ~val; 
    A = res; 
    PC = 0x87A7; 
    JUMP; // sstep
  case 0x87A7:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 87A7: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x87A9; 
    JUMP; // sstep
  case 0x87A9:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 87A9: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x87AB; 
    JUMP; // sstep
  case 0x87AB:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 87AB: 10 26 0B C6   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x87AF; 
    JUMP; // sstep
  case 0x87AF:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0E                    ; 87AF: C4 0E         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0e; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x87B1; 
    JUMP; // sstep
  case 0x87B1:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 87B1: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x87B3; 
    JUMP; // sstep
  case 0x87B3:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 87B3: 10 26 0B BE   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x87B7; 
    JUMP; // sstep
  case 0x87B7:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$87D2                  ; 87B7: 8C 87 D2      \n");
                                      #endif
    val = X; 
    arg = 0x87d2; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x87BA; 
    JUMP; // sstep
  case 0x87BA:
                                                     #ifdef DEBUG
                                      mon("         BNE   $87A4                   ; 87BA: 26 E8         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x87a4; 
    JUMP;
    }
    PC = 0x87BC; 
    JUMP; // sstep
  case 0x87BC:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 87BC: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 87BD:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 87BE:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 87BF:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 87C0:  ' ' FE FF 08       LDU   $FF08 ; 
                   // FCB   $FF                     ; 87C1:  ' ' FF 08 FF       STU   $08FF ; 
                   // FCB   $08                     ; 87C2:  ' ' 08 FF          ASL   <$FF ; 
                   // FCB   $FF                     ; 87C3:  ' ' FF 00 04       STU   $0004 ; 
                   // FCB   $00                     ; 87C4:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 87C5:  ' ' 04 7F          LSR   <$7F ; 
                   // FCB   $7F                     ; 87C6:  ' ' 7F 80 0A       CLR   $800A ; 
                   // FCB   $80                     ; 87C7:  ' ' 80 0A          SUBA  #$0A ; 
                   // FCB   $0A                     ; 87C8:  ' ' 0A 80          DEC   <$80 ; 
                   // FCB   $80                     ; 87C9:  ' ' 80 81          SUBA  #$81 ; 
                   // FCB   $81                     ; 87CA:  ' ' 81 08          CMPA  #$08 ; 
                   // FCB   $08                     ; 87CB:  ' ' 08 0F          ASL   <$0F ; 
                   // FCB   $0F                     ; 87CC:  ' ' 0F 10          CLR   <$10 ; 
                   // FCB   $10                     ; 87CD:  ' ' 10 00          Invalid ; 
                   // FCB   $00                     ; 87CE:  ' ' 00 10          NEG   <$10 ; 
                   // FCB   $10                     ; 87CF:  ' ' 10 11          Invalid ; 
                   // FCB   $11                     ; 87D0:  ' ' 11 00          Invalid ; 
                   // FCB   $00                     ; 87D1:  ' ' 00 49          NEG   <$49 ; 
                   // FCB   $49                     ; 87D2:  'I' 49             ROLA  ; 
                   // FCB   $4E                     ; 87D3:  'N' 4E             Invalid ; 
                   // FCB   $43                     ; 87D4:  'C' 43             COMA  ; 
                   // FCB   $04                     ; 87D5:  ' ' 04 CE          LSR   <$CE ; 
  case 0x87D6:
                                                     #ifdef DEBUG
                                      mon("L87D6    LDU   #$8806                  ; 87D6: CE 88 06      \n");
                                      #endif
    U = 0x8806; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x87D9; 
    JUMP; // sstep
  case 0x87D9:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$87F1                  ; 87D9: 8E 87 F1      \n");
                                      #endif
    X = 0x87f1; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x87DC; 
    JUMP; // sstep
  case 0x87DC:
                                                     #ifdef DEBUG
                                      mon("L87DC    LDA   ,X+                     ; 87DC: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x87DE; 
    JUMP; // sstep
  case 0x87DE:
                                                     #ifdef DEBUG
                                      mon("         CLRA                          ; 87DE: 4F            \n");
                                      #endif
    A = 0; // SEARCH_ME
    V = 0; 
    Z = 0; 
    N = 0; 
    C = 0; 
    PC = 0x87DF; 
    JUMP; // sstep
  case 0x87DF:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 87DF: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x87E1; 
    JUMP; // sstep
  case 0x87E1:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 87E1: A1 80         \n");
                                      #endif
/**/val = (UINT8)A; 
/**/arg = rd_mem(X); 
/**/res = val - (UINT8)arg; 
/**/C = res & 0x100; 
/**/Z = (UINT8)res; 
/**/N = (UINT8)res; 
/**/V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x87E3; 
    JUMP; // sstep
  case 0x87E3:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0F                    ; 87E3: C4 0F         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0f; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x87E5; 
    JUMP; // sstep
  case 0x87E5:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 87E5: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x87E7; 
    JUMP; // sstep
  case 0x87E7:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 87E7: 10 26 0B 8A   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x87EB; 
    JUMP; // sstep
  case 0x87EB:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8806                  ; 87EB: 8C 88 06      \n");
                                      #endif
    val = X; 
    arg = 0x8806; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x87EE; 
    JUMP; // sstep
  case 0x87EE:
                                                     #ifdef DEBUG
                                      mon("         BNE   $87DC                   ; 87EE: 26 EC         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x87dc; 
    JUMP;
    }
    PC = 0x87F0; 
    JUMP; // sstep
  case 0x87F0:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 87F0: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $01                     ; 87F1:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 87F2:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 87F3:  ' ' 04 00          LSR   <$00 ; 
                   // FCB   $00                     ; 87F4:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 87F5:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 87F6:  ' ' 04 FF          LSR   <$FF ; 
                   // FCB   $FF                     ; 87F7:  ' ' FF 00 04       STU   $0004 ; 
                   // FCB   $00                     ; 87F8:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 87F9:  ' ' 04 80          LSR   <$80 ; 
                   // FCB   $80                     ; 87FA:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 87FB:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 87FC:  ' ' 04 7F          LSR   <$7F ; 
                   // FCB   $7F                     ; 87FD:  ' ' 7F 00 04       CLR   $0004 ; 
                   // FCB   $00                     ; 87FE:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 87FF:  ' ' 04 10          LSR   <$10 ; 
                   // FCB   $10                     ; 8800:  ' ' 10 00          Invalid ; 
                   // FCB   $00                     ; 8801:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8802:  ' ' 04 0F          LSR   <$0F ; 
                   // FCB   $0F                     ; 8803:  ' ' 0F 00          CLR   <$00 ; 
                   // FCB   $00                     ; 8804:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8805:  ' ' 04 43          LSR   <$43 ; 
                   // FCB   $43                     ; 8806:  'C' 43             COMA  ; 
                   // FCB   $4C                     ; 8807:  'L' 4C             INCA  ; 
                   // FCB   $52                     ; 8808:  'R' 52             Invalid ; 
                   // FCB   $04                     ; 8809:  ' ' 04 CE          LSR   <$CE ; 
  case 0x880A:
                                                     #ifdef DEBUG
                                      mon("L880A    LDU   #$888F                  ; 880A: CE 88 8F      \n");
                                      #endif
    U = 0x888f; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x880D; 
    JUMP; // sstep
  case 0x880D:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$882D                  ; 880D: 8E 88 2D      \n");
                                      #endif
    X = 0x882d; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8810; 
    JUMP; // sstep
  case 0x8810:
                                                     #ifdef DEBUG
                                      mon("L8810    LDD   ,X++                    ; 8810: EC 81         \n");
                                      #endif
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
    X = X + 2; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8812; 
    JUMP; // sstep
  case 0x8812:
                                                     #ifdef DEBUG
                                      mon("         ADDD  ,X++                    ; 8812: E3 81         \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = ea; 
    arg = (A << 8) | B; 
    res = arg + val; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    V = ((arg ^ res) & (val ^ res)) >> 8; 
    N = res >> 8; 
    A = (UINT8)(res >> 8); 
    B = (UINT8)res; 
    X = X + 2; 
    PC = 0x8814; 
    JUMP; // sstep
  case 0x8814:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,DP                   ; 8814: 1F AB         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    DP = (CC) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8816; 
    JUMP; // sstep
  case 0x8816:
                                                     #ifdef DEBUG
                                      mon("         CMPD  ,X++                    ; 8816: 10 A3 81      \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = ea; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    X = X + 2; 
    PC = 0x8819; 
    JUMP; // sstep
  case 0x8819:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8819: 10 26 0B 58   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x881D; 
    JUMP; // sstep
  case 0x881D:
                                                     #ifdef DEBUG
                                      mon("         TFR   DP,A                    ; 881D: 1F B8         \n");
                                      #endif
    A = (UINT8)(UINT8)(DP >> 8); 
    PC = 0x881F; 
    JUMP; // sstep
  case 0x881F:
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$0F                    ; 881F: 84 0F         \n");
                                      #endif
    val = (UINT8)A; 
    res = val & (SINT8)0x0f; 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8821; 
    JUMP; // sstep
  case 0x8821:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8821: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8823; 
    JUMP; // sstep
  case 0x8823:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8823: 10 26 0B 4E   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8827; 
    JUMP; // sstep
  case 0x8827:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$888F                  ; 8827: 8C 88 8F      \n");
                                      #endif
    val = X; 
    arg = 0x888f; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x882A; 
    JUMP; // sstep
  case 0x882A:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8810                   ; 882A: 26 E4         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8810; 
    JUMP;
    }
    PC = 0x882C; 
    JUMP; // sstep
  case 0x882C:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 882C: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 882D:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 882E:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 882F:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8830:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8831:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8832:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8833:  ' ' 04 00          LSR   <$00 ; 
                   // FCB   $00                     ; 8834:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8835:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8836:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8837:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8838:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8839:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 883A:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 883B:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 883C:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 883D:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 883E:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 883F:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8840:  ' ' 00 0A          NEG   <$0A ; 
                   // FCB   $0A                     ; 8841:  ' ' 0A 00          DEC   <$00 ; 
                   // FCB   $00                     ; 8842:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8843:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8844:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8845:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8846:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8847:  ' ' 00 08          NEG   <$08 ; 
                   // FCB   $08                     ; 8848:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 8849:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 884A:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 884B:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 884C:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 884D:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 884E:  ' ' 00 07          NEG   <$07 ; 
                   // FCB   $07                     ; 884F:  ' ' 07 40          ASR   <$40 ; 
                   // FCB   $40                     ; 8850:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 8851:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8852:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8853:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 8854:  ' ' C0 00          SUBB  #$00 ; 
                   // FCB   $00                     ; 8855:  ' ' 00 08          NEG   <$08 ; 
                   // FCB   $08                     ; 8856:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 8857:  ' ' 7F FF 7F       CLR   $FF7F ; 
                   // FCB   $FF                     ; 8858:  ' ' FF 7F FF       STU   $7FFF ; 
                   // FCB   $7F                     ; 8859:  ' ' 7F FF FF       CLR   $FFFF ; 
                   // FCB   $FF                     ; 885A:  ' ' FF FF FE       STU   $FFFE ; 
                   // FCB   $FF                     ; 885B:  ' ' FF FE 0A       STU   $FE0A ; 
                   // FCB   $FE                     ; 885C:  ' ' FE 0A 7F       LDU   $0A7F ; 
                   // FCB   $0A                     ; 885D:  ' ' 0A 7F          DEC   <$7F ; 
                   // FCB   $7F                     ; 885E:  ' ' 7F FF 80       CLR   $FF80 ; 
                   // FCB   $FF                     ; 885F:  ' ' FF 80 00       STU   $8000 ; 
                   // FCB   $80                     ; 8860:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8861:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8862:  ' ' FF FF 08       STU   $FF08 ; 
                   // FCB   $FF                     ; 8863:  ' ' FF 08 7F       STU   $087F ; 
                   // FCB   $08                     ; 8864:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 8865:  ' ' 7F FF 00       CLR   $FF00 ; 
                   // FCB   $FF                     ; 8866:  ' ' FF 00 01       STU   $0001 ; 
                   // FCB   $00                     ; 8867:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8868:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8869:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 886A:  ' ' 00 0A          NEG   <$0A ; 
                   // FCB   $0A                     ; 886B:  ' ' 0A 80          DEC   <$80 ; 
                   // FCB   $80                     ; 886C:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 886D:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 886E:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 886F:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8870:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8871:  ' ' 01             Invalid ; 
                   // FCB   $08                     ; 8872:  ' ' 08 40          ASL   <$40 ; 
                   // FCB   $40                     ; 8873:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 8874:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 8875:  ' ' C0 00          SUBB  #$00 ; 
                   // FCB   $00                     ; 8876:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8877:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8878:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 8879:  ' ' 05             Invalid ; 
                   // FCB   $40                     ; 887A:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 887B:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 887C:  ' ' C0 01          SUBB  #$01 ; 
                   // FCB   $01                     ; 887D:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 887E:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 887F:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8880:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8881:  ' ' FF FF FF       STU   $FFFF ; 
                   // FCB   $FF                     ; 8882:  ' ' FF FF FF       STU   $FFFF ; 
                   // FCB   $FF                     ; 8883:  ' ' FF FF FF       STU   $FFFF ; 
                   // FCB   $FF                     ; 8884:  ' ' FF FF FE       STU   $FFFE ; 
                   // FCB   $FF                     ; 8885:  ' ' FF FE 09       STU   $FE09 ; 
                   // FCB   $FE                     ; 8886:  ' ' FE 09 80       LDU   $0980 ; 
                   // FCB   $09                     ; 8887:  ' ' 09 80          ROL   <$80 ; 
                   // FCB   $80                     ; 8888:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8889:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 888A:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 888B:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 888C:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 888D:  ' ' 01             Invalid ; 
                   // FCB   $03                     ; 888E:  ' ' 03 41          COM   <$41 ; 
                   // FCB   $41                     ; 888F:  'A' 41             Invalid ; 
                   // FCB   $44                     ; 8890:  'D' 44             LSRA  ; 
                   // FCB   $44                     ; 8891:  'D' 44             LSRA  ; 
                   // FCB   $44                     ; 8892:  'D' 44             LSRA  ; 
                   // FCB   $04                     ; 8893:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8894:
                                                     #ifdef DEBUG
                                      mon("L8894    LDU   #$88F0                  ; 8894: CE 88 F0      \n");
                                      #endif
    U = 0x88f0; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8897; 
    JUMP; // sstep
  case 0x8897:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$88B4                  ; 8897: 8E 88 B4      \n");
                                      #endif
    X = 0x88b4; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x889A; 
    JUMP; // sstep
  case 0x889A:
                                                     #ifdef DEBUG
                                      mon("L889A    LDB   ,X+                     ; 889A: E6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x889C; 
    JUMP; // sstep
  case 0x889C:
                                                     #ifdef DEBUG
                                      mon("         ADDB  ,X+                     ; 889C: EB 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val + (UINT8)arg; 
    C = (res >> 1) & 0x80; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    H = (UINT8)(val ^ arg ^ res ^ C); 
    V = (UINT8)(val ^ arg ^ res ^ C); 
    B = (UINT8)res; 
    X = X + 1; 
    PC = 0x889E; 
    JUMP; // sstep
  case 0x889E:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,A                    ; 889E: 1F A8         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    A = (UINT8)CC; 
    PC = 0x88A0; 
    JUMP; // sstep
  case 0x88A0:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 88A0: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x88A2; 
    JUMP; // sstep
  case 0x88A2:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 88A2: 10 26 0A CF   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x88A6; 
    JUMP; // sstep
  case 0x88A6:
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$2F                    ; 88A6: 84 2F         \n");
                                      #endif
    val = (UINT8)A; 
    res = val & (SINT8)0x2f; 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x88A8; 
    JUMP; // sstep
  case 0x88A8:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 88A8: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x88AA; 
    JUMP; // sstep
  case 0x88AA:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 88AA: 10 26 0A C7   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x88AE; 
    JUMP; // sstep
  case 0x88AE:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$88F0                  ; 88AE: 8C 88 F0      \n");
                                      #endif
    val = X; 
    arg = 0x88f0; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x88B1; 
    JUMP; // sstep
  case 0x88B1:
                                                     #ifdef DEBUG
                                      mon("         BNE   $889A                   ; 88B1: 26 E7         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x889a; 
    JUMP;
    }
    PC = 0x88B3; 
    JUMP; // sstep
  case 0x88B3:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 88B3: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 88B4:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 88B5:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 88B6:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 88B7:  ' ' 04 00          LSR   <$00 ; 
                   // FCB   $00                     ; 88B8:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 88B9:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 88BA:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 88BB:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 88BC:  '@' 40             NEGA  ; 
                   // FCB   $40                     ; 88BD:  '@' 40             NEGA  ; 
                   // FCB   $80                     ; 88BE:  ' ' 80 0A          SUBA  #$0A ; 
                   // FCB   $0A                     ; 88BF:  ' ' 0A 00          DEC   <$00 ; 
                   // FCB   $00                     ; 88C0:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 88C1:  ' ' 80 80          SUBA  #$80 ; 
                   // FCB   $80                     ; 88C2:  ' ' 80 08          SUBA  #$08 ; 
                   // FCB   $08                     ; 88C3:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 88C4:  ' ' 80 80          SUBA  #$80 ; 
                   // FCB   $80                     ; 88C5:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 88C6:  ' ' 00 07          NEG   <$07 ; 
                   // FCB   $07                     ; 88C7:  ' ' 07 40          ASR   <$40 ; 
                   // FCB   $40                     ; 88C8:  '@' 40             NEGA  ; 
                   // FCB   $80                     ; 88C9:  ' ' 80 C0          SUBA  #$C0 ; 
                   // FCB   $C0                     ; 88CA:  ' ' C0 08          SUBB  #$08 ; 
                   // FCB   $08                     ; 88CB:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 88CC:  ' ' 7F 7F FE       CLR   $7FFE ; 
                   // FCB   $7F                     ; 88CD:  ' ' 7F FE 2A       CLR   $FE2A ; 
                   // FCB   $FE                     ; 88CE:  ' ' FE 2A 7F       LDU   $2A7F ; 
                   // FCB   $2A                     ; 88CF:  '*' 2A 7F          BPL   $8950 ; 
                   // FCB   $7F                     ; 88D0:  ' ' 7F 80 FF       CLR   $80FF ; 
                   // FCB   $80                     ; 88D1:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 88D2:  ' ' FF 08 80       STU   $0880 ; 
                   // FCB   $08                     ; 88D3:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 88D4:  ' ' 80 7F          SUBA  #$7F ; 
                   // FCB   $7F                     ; 88D5:  ' ' 7F FF 08       CLR   $FF08 ; 
                   // FCB   $FF                     ; 88D6:  ' ' FF 08 7F       STU   $087F ; 
                   // FCB   $08                     ; 88D7:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 88D8:  ' ' 7F 01 80       CLR   $0180 ; 
                   // FCB   $01                     ; 88D9:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 88DA:  ' ' 80 2A          SUBA  #$2A ; 
                   // FCB   $2A                     ; 88DB:  '*' 2A 80          BPL   $885D ; 
                   // FCB   $80                     ; 88DC:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 88DD:  ' ' 01             Invalid ; 
                   // FCB   $81                     ; 88DE:  ' ' 81 08          CMPA  #$08 ; 
                   // FCB   $08                     ; 88DF:  ' ' 08 40          ASL   <$40 ; 
                   // FCB   $40                     ; 88E0:  '@' 40             NEGA  ; 
                   // FCB   $C0                     ; 88E1:  ' ' C0 00          SUBB  #$00 ; 
                   // FCB   $00                     ; 88E2:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 88E3:  ' ' 05             Invalid ; 
                   // FCB   $40                     ; 88E4:  '@' 40             NEGA  ; 
                   // FCB   $C1                     ; 88E5:  ' ' C1 01          CMPB  #$01 ; 
                   // FCB   $01                     ; 88E6:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 88E7:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 88E8:  ' ' FF FF FE       STU   $FFFE ; 
                   // FCB   $FF                     ; 88E9:  ' ' FF FE 29       STU   $FE29 ; 
                   // FCB   $FE                     ; 88EA:  ' ' FE 29 81       LDU   $2981 ; 
                   // FCB   $29                     ; 88EB:  ')' 29 81          BVS   $886E ; 
                   // FCB   $81                     ; 88EC:  ' ' 81 80          CMPA  #$80 ; 
                   // FCB   $80                     ; 88ED:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 88EE:  ' ' 01             Invalid ; 
                   // FCB   $03                     ; 88EF:  ' ' 03 41          COM   <$41 ; 
                   // FCB   $41                     ; 88F0:  'A' 41             Invalid ; 
                   // FCB   $44                     ; 88F1:  'D' 44             LSRA  ; 
                   // FCB   $44                     ; 88F2:  'D' 44             LSRA  ; 
                   // FCB   $04                     ; 88F3:  ' ' 04 CE          LSR   <$CE ; 
  case 0x88F4:
                                                     #ifdef DEBUG
                                      mon("L88F4    LDU   #$89D0                  ; 88F4: CE 89 D0      \n");
                                      #endif
    U = 0x89d0; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x88F7; 
    JUMP; // sstep
  case 0x88F7:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$891C                  ; 88F7: 8E 89 1C      \n");
                                      #endif
    X = 0x891c; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x88FA; 
    JUMP; // sstep
  case 0x88FA:
                                                     #ifdef DEBUG
                                      mon("L88FA    LDA   ,X+                     ; 88FA: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
    X = X + 1; 
    PC = 0x88FC; 
    JUMP; // sstep
  case 0x88FC:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 88FC: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x88FE; 
    JUMP; // sstep
  case 0x88FE:
                                                     #ifdef DEBUG
                                      mon("         ANDB  ,X+                     ; 88FE: E4 80         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8900; 
    JUMP; // sstep
  case 0x8900:
                                                     #ifdef DEBUG
                                      mon("         ORB   ,X+                     ; 8900: EA 80         \n");
                                      #endif
    res = B | (UINT8)rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8902; 
    JUMP; // sstep
  case 0x8902:
                                                     #ifdef DEBUG
                                      mon("         TFR   B,CC                    ; 8902: 1F 9A         \n");
                                      #endif
    CC = B; 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8904; 
    JUMP; // sstep
  case 0x8904:
                                                     #ifdef DEBUG
                                      mon("         ADCA  ,X+                     ; 8904: A9 80         \n");
                                      #endif
    arg = A; 
    val = rd_mem(X); 
    res = arg + val + (C != 0 ? 1:0); 
    C = (res >> 1) & 0x80; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    H = arg ^ val ^ (UINT8)res ^ C; 
    V = arg ^ val ^ (UINT8)res ^ C; 
    A = (UINT8)res; 
    X = X + 1; 
    PC = 0x8906; 
    JUMP; // sstep
  case 0x8906:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 8906: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8908; 
    JUMP; // sstep
  case 0x8908:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8908: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x890A; 
    JUMP; // sstep
  case 0x890A:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 890A: 10 26 0A 67   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x890E; 
    JUMP; // sstep
  case 0x890E:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$2F                    ; 890E: C4 2F         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x2f; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8910; 
    JUMP; // sstep
  case 0x8910:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8910: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8912; 
    JUMP; // sstep
  case 0x8912:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8912: 10 26 0A 5F   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8916; 
    JUMP; // sstep
  case 0x8916:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$89D0                  ; 8916: 8C 89 D0      \n");
                                      #endif
    val = X; 
    arg = 0x89d0; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8919; 
    JUMP; // sstep
  case 0x8919:
                                                     #ifdef DEBUG
                                      mon("         BNE   $88FA                   ; 8919: 26 DF         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x88fa; 
    JUMP;
    }
    PC = 0x891B; 
    JUMP; // sstep
  case 0x891B:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 891B: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 891C:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 891D:  ' ' FE 00 00       LDU   $0000 ; 
                   // FCB   $00                     ; 891E:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 891F:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8920:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8921:  ' ' 04 00          LSR   <$00 ; 
                   // FCB   $00                     ; 8922:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8923:  ' ' FE 00 01       LDU   $0001 ; 
                   // FCB   $00                     ; 8924:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8925:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8926:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8927:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 8928:  '@' 40             NEGA  ; 
                   // FCB   $FE                     ; 8929:  ' ' FE 00 40       LDU   $0040 ; 
                   // FCB   $00                     ; 892A:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 892B:  '@' 40             NEGA  ; 
                   // FCB   $80                     ; 892C:  ' ' 80 0A          SUBA  #$0A ; 
                   // FCB   $0A                     ; 892D:  ' ' 0A 00          DEC   <$00 ; 
                   // FCB   $00                     ; 892E:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 892F:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 8930:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8931:  ' ' 80 80          SUBA  #$80 ; 
                   // FCB   $80                     ; 8932:  ' ' 80 08          SUBA  #$08 ; 
                   // FCB   $08                     ; 8933:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 8934:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 8935:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 8936:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8937:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8938:  ' ' 00 07          NEG   <$07 ; 
                   // FCB   $07                     ; 8939:  ' ' 07 40          ASR   <$40 ; 
                   // FCB   $40                     ; 893A:  '@' 40             NEGA  ; 
                   // FCB   $FE                     ; 893B:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 893C:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 893D:  ' ' 80 C0          SUBA  #$C0 ; 
                   // FCB   $C0                     ; 893E:  ' ' C0 08          SUBB  #$08 ; 
                   // FCB   $08                     ; 893F:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 8940:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8941:  ' ' FE 00 7F       LDU   $007F ; 
                   // FCB   $00                     ; 8942:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8943:  ' ' 7F FE 2A       CLR   $FE2A ; 
                   // FCB   $FE                     ; 8944:  ' ' FE 2A 7F       LDU   $2A7F ; 
                   // FCB   $2A                     ; 8945:  '*' 2A 7F          BPL   $89C6 ; 
                   // FCB   $7F                     ; 8946:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8947:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 8948:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8949:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 894A:  ' ' FF 08 80       STU   $0880 ; 
                   // FCB   $08                     ; 894B:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 894C:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 894D:  ' ' FE 00 7F       LDU   $007F ; 
                   // FCB   $00                     ; 894E:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 894F:  ' ' 7F FF 08       CLR   $FF08 ; 
                   // FCB   $FF                     ; 8950:  ' ' FF 08 7F       STU   $087F ; 
                   // FCB   $08                     ; 8951:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 8952:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8953:  ' ' FE 00 01       LDU   $0001 ; 
                   // FCB   $00                     ; 8954:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8955:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8956:  ' ' 80 2A          SUBA  #$2A ; 
                   // FCB   $2A                     ; 8957:  '*' 2A 80          BPL   $88D9 ; 
                   // FCB   $80                     ; 8958:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 8959:  ' ' FE 00 01       LDU   $0001 ; 
                   // FCB   $00                     ; 895A:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 895B:  ' ' 01             Invalid ; 
                   // FCB   $81                     ; 895C:  ' ' 81 08          CMPA  #$08 ; 
                   // FCB   $08                     ; 895D:  ' ' 08 40          ASL   <$40 ; 
                   // FCB   $40                     ; 895E:  '@' 40             NEGA  ; 
                   // FCB   $FE                     ; 895F:  ' ' FE 00 C0       LDU   $00C0 ; 
                   // FCB   $00                     ; 8960:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 8961:  ' ' C0 00          SUBB  #$00 ; 
                   // FCB   $00                     ; 8962:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 8963:  ' ' 05             Invalid ; 
                   // FCB   $40                     ; 8964:  '@' 40             NEGA  ; 
                   // FCB   $FE                     ; 8965:  ' ' FE 00 C1       LDU   $00C1 ; 
                   // FCB   $00                     ; 8966:  ' ' 00 C1          NEG   <$C1 ; 
                   // FCB   $C1                     ; 8967:  ' ' C1 01          CMPB  #$01 ; 
                   // FCB   $01                     ; 8968:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8969:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 896A:  ' ' FF FE 00       STU   $FE00 ; 
                   // FCB   $FE                     ; 896B:  ' ' FE 00 FF       LDU   $00FF ; 
                   // FCB   $00                     ; 896C:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 896D:  ' ' FF FE 29       STU   $FE29 ; 
                   // FCB   $FE                     ; 896E:  ' ' FE 29 81       LDU   $2981 ; 
                   // FCB   $29                     ; 896F:  ')' 29 81          BVS   $88F2 ; 
                   // FCB   $81                     ; 8970:  ' ' 81 FE          CMPA  #$FE ; 
                   // FCB   $FE                     ; 8971:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 8972:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8973:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8974:  ' ' 01             Invalid ; 
                   // FCB   $03                     ; 8975:  ' ' 03 00          COM   <$00 ; 
                   // FCB   $00                     ; 8976:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8977:  ' ' FF 01 00       STU   $0100 ; 
                   // FCB   $01                     ; 8978:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8979:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 897A:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 897B:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 897C:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 897D:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 897E:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 897F:  ' ' 01             Invalid ; 
                   // FCB   $02                     ; 8980:  ' ' 02             Invalid ; 
                   // FCB   $00                     ; 8981:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 8982:  '@' 40             NEGA  ; 
                   // FCB   $FF                     ; 8983:  ' ' FF 01 40       STU   $0140 ; 
                   // FCB   $01                     ; 8984:  ' ' 01             Invalid ; 
                   // FCB   $40                     ; 8985:  '@' 40             NEGA  ; 
                   // FCB   $81                     ; 8986:  ' ' 81 0A          CMPA  #$0A ; 
                   // FCB   $0A                     ; 8987:  ' ' 0A 00          DEC   <$00 ; 
                   // FCB   $00                     ; 8988:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8989:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 898A:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 898B:  ' ' 80 81          SUBA  #$81 ; 
                   // FCB   $81                     ; 898C:  ' ' 81 08          CMPA  #$08 ; 
                   // FCB   $08                     ; 898D:  ' ' 08 80          ASL   <$80 ; 
                   // FCB   $80                     ; 898E:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 898F:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 8990:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8991:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8992:  ' ' 01             Invalid ; 
                   // FCB   $03                     ; 8993:  ' ' 03 40          COM   <$40 ; 
                   // FCB   $40                     ; 8994:  '@' 40             NEGA  ; 
                   // FCB   $FF                     ; 8995:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 8996:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8997:  ' ' 80 C1          SUBA  #$C1 ; 
                   // FCB   $C1                     ; 8998:  ' ' C1 08          CMPB  #$08 ; 
                   // FCB   $08                     ; 8999:  ' ' 08 7F          ASL   <$7F ; 
                   // FCB   $7F                     ; 899A:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 899B:  ' ' FF 01 7F       STU   $017F ; 
                   // FCB   $01                     ; 899C:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 899D:  ' ' 7F FF 2A       CLR   $FF2A ; 
                   // FCB   $FF                     ; 899E:  ' ' FF 2A 7F       STU   $2A7F ; 
                   // FCB   $2A                     ; 899F:  '*' 2A 7F          BPL   $8A20 ; 
                   // FCB   $7F                     ; 89A0:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 89A1:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 89A2:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 89A3:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 89A4:  ' ' 00 25          NEG   <$25 ; 
                   // FCB   $25                     ; 89A5:  '%' 25 80          BCS   $8927 ; 
                   // FCB   $80                     ; 89A6:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 89A7:  ' ' FF 01 7F       STU   $017F ; 
                   // FCB   $01                     ; 89A8:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 89A9:  ' ' 7F 00 25       CLR   $0025 ; 
                   // FCB   $00                     ; 89AA:  ' ' 00 25          NEG   <$25 ; 
                   // FCB   $25                     ; 89AB:  '%' 25 7F          BCS   $8A2C ; 
                   // FCB   $7F                     ; 89AC:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 89AD:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 89AE:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 89AF:  ' ' 01             Invalid ; 
                   // FCB   $81                     ; 89B0:  ' ' 81 2A          CMPA  #$2A ; 
                   // FCB   $2A                     ; 89B1:  '*' 2A 80          BPL   $8933 ; 
                   // FCB   $80                     ; 89B2:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 89B3:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 89B4:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 89B5:  ' ' 01             Invalid ; 
                   // FCB   $82                     ; 89B6:  ' ' 82 08          SBCA  #$08 ; 
                   // FCB   $08                     ; 89B7:  ' ' 08 40          ASL   <$40 ; 
                   // FCB   $40                     ; 89B8:  '@' 40             NEGA  ; 
                   // FCB   $FF                     ; 89B9:  ' ' FF 01 C0       STU   $01C0 ; 
                   // FCB   $01                     ; 89BA:  ' ' 01             Invalid ; 
                   // FCB   $C0                     ; 89BB:  ' ' C0 01          SUBB  #$01 ; 
                   // FCB   $01                     ; 89BC:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 89BD:  ' ' 01             Invalid ; 
                   // FCB   $40                     ; 89BE:  '@' 40             NEGA  ; 
                   // FCB   $FF                     ; 89BF:  ' ' FF 01 C1       STU   $01C1 ; 
                   // FCB   $01                     ; 89C0:  ' ' 01             Invalid ; 
                   // FCB   $C1                     ; 89C1:  ' ' C1 02          CMPB  #$02 ; 
                   // FCB   $02                     ; 89C2:  ' ' 02             Invalid ; 
                   // FCB   $01                     ; 89C3:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 89C4:  ' ' FF FF 01       STU   $FF01 ; 
                   // FCB   $FF                     ; 89C5:  ' ' FF 01 FF       STU   $01FF ; 
                   // FCB   $01                     ; 89C6:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 89C7:  ' ' FF FF 29       STU   $FF29 ; 
                   // FCB   $FF                     ; 89C8:  ' ' FF 29 81       STU   $2981 ; 
                   // FCB   $29                     ; 89C9:  ')' 29 81          BVS   $894C ; 
                   // FCB   $81                     ; 89CA:  ' ' 81 FF          CMPA  #$FF ; 
                   // FCB   $FF                     ; 89CB:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 89CC:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 89CD:  ' ' 80 02          SUBA  #$02 ; 
                   // FCB   $02                     ; 89CE:  ' ' 02             Invalid ; 
                   // FCB   $03                     ; 89CF:  ' ' 03 41          COM   <$41 ; 
                   // FCB   $41                     ; 89D0:  'A' 41             Invalid ; 
                   // FCB   $44                     ; 89D1:  'D' 44             LSRA  ; 
                   // FCB   $43                     ; 89D2:  'C' 43             COMA  ; 
                   // FCB   $04                     ; 89D3:  ' ' 04 CE          LSR   <$CE ; 
  case 0x89D4:
                                                     #ifdef DEBUG
                                      mon("L89D4    LDU   #$89F0                  ; 89D4: CE 89 F0      \n");
                                      #endif
    U = 0x89f0; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x89D7; 
    JUMP; // sstep
  case 0x89D7:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8A14                  ; 89D7: 8E 8A 14      \n");
                                      #endif
    X = 0x8a14; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x89DA; 
    JUMP; // sstep
  case 0x89DA:
                                                     #ifdef DEBUG
                                      mon("L89DA    LDB   ,X+                     ; 89DA: E6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x89DC; 
    JUMP; // sstep
  case 0x89DC:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 89DC: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x89DE; 
    JUMP; // sstep
  case 0x89DE:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,A                    ; 89DE: 1F A8         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    A = (UINT8)CC; 
    PC = 0x89E0; 
    JUMP; // sstep
  case 0x89E0:
                                                     #ifdef DEBUG
                                      mon("         LEAX  $01,X                   ; 89E0: 30 01         \n");
                                      #endif
    X = (X + 0x0001); 
/**/Z = X; 
    PC = 0x89E2; 
    JUMP; // sstep
  case 0x89E2:
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$0F                    ; 89E2: 84 0F         \n");
                                      #endif
    val = (UINT8)A; 
    res = val & (SINT8)0x0f; 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x89E4; 
    JUMP; // sstep
  case 0x89E4:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 89E4: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x89E6; 
    JUMP; // sstep
  case 0x89E6:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 89E6: 10 26 09 8B   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x89EA; 
    JUMP; // sstep
  case 0x89EA:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8A7C                  ; 89EA: 8C 8A 7C      \n");
                                      #endif
    val = X; 
    arg = 0x8a7c; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x89ED; 
    JUMP; // sstep
  case 0x89ED:
                                                     #ifdef DEBUG
                                      mon("         BNE   $89DA                   ; 89ED: 26 EB         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x89da; 
    JUMP;
    }
    PC = 0x89EF; 
    JUMP; // sstep
  case 0x89EF:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 89EF: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $43                     ; 89F0:  'C' 43             COMA  ; 
                   // FCB   $4D                     ; 89F1:  'M' 4D             TSTA  ; 
                   // FCB   $50                     ; 89F2:  'P' 50             NEGB  ; 
                   // FCB   $04                     ; 89F3:  ' ' 04 CE          LSR   <$CE ; 
  case 0x89F4:
                                                     #ifdef DEBUG
                                      mon("L89F4    LDU   #$8A7C                  ; 89F4: CE 8A 7C      \n");
                                      #endif
    U = 0x8a7c; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x89F7; 
    JUMP; // sstep
  case 0x89F7:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8A14                  ; 89F7: 8E 8A 14      \n");
                                      #endif
    X = 0x8a14; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x89FA; 
    JUMP; // sstep
  case 0x89FA:
                                                     #ifdef DEBUG
                                      mon("L89FA    LDB   ,X+                     ; 89FA: E6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x89FC; 
    JUMP; // sstep
  case 0x89FC:
                                                     #ifdef DEBUG
                                      mon("         SUBB  ,X+                     ; 89FC: E0 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    B = (UINT8)(res); 
    X = X + 1; 
    PC = 0x89FE; 
    JUMP; // sstep
  case 0x89FE:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,A                    ; 89FE: 1F A8         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    A = (UINT8)CC; 
    PC = 0x8A00; 
    JUMP; // sstep
  case 0x8A00:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8A00: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8A02; 
    JUMP; // sstep
  case 0x8A02:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8A02: 10 26 09 6F   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8A06; 
    JUMP; // sstep
  case 0x8A06:
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$0F                    ; 8A06: 84 0F         \n");
                                      #endif
    val = (UINT8)A; 
    res = val & (SINT8)0x0f; 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8A08; 
    JUMP; // sstep
  case 0x8A08:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8A08: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8A0A; 
    JUMP; // sstep
  case 0x8A0A:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8A0A: 10 26 09 67   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8A0E; 
    JUMP; // sstep
  case 0x8A0E:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8A7C                  ; 8A0E: 8C 8A 7C      \n");
                                      #endif
    val = X; 
    arg = 0x8a7c; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8A11; 
    JUMP; // sstep
  case 0x8A11:
                                                     #ifdef DEBUG
                                      mon("         BNE   $89FA                   ; 8A11: 26 E7         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x89fa; 
    JUMP;
    }
    PC = 0x8A13; 
    JUMP; // sstep
  case 0x8A13:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8A13: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8A14:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8A15:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8A16:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8A17:  ' ' 04 00          LSR   <$00 ; 
                   // FCB   $00                     ; 8A18:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8A19:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8A1A:  ' ' FF 09 00       STU   $0900 ; 
                   // FCB   $09                     ; 8A1B:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 8A1C:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8A1D:  ' ' 7F 81 09       CLR   $8109 ; 
                   // FCB   $81                     ; 8A1E:  ' ' 81 09          CMPA  #$09 ; 
                   // FCB   $09                     ; 8A1F:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 8A20:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8A21:  ' ' 80 80          SUBA  #$80 ; 
                   // FCB   $80                     ; 8A22:  ' ' 80 0B          SUBA  #$0B ; 
                   // FCB   $0B                     ; 8A23:  ' ' 0B             Invalid ; 
                   // FCB   $00                     ; 8A24:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8A25:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 8A26:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8A27:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8A28:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8A29:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8A2A:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8A2B:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8A2C:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8A2D:  ' ' 7F 82 09       CLR   $8209 ; 
                   // FCB   $82                     ; 8A2E:  ' ' 82 09          SBCA  #$09 ; 
                   // FCB   $09                     ; 8A2F:  ' ' 09 40          ROL   <$40 ; 
                   // FCB   $40                     ; 8A30:  '@' 40             NEGA  ; 
                   // FCB   $41                     ; 8A31:  'A' 41             Invalid ; 
                   // FCB   $FF                     ; 8A32:  ' ' FF 09 40       STU   $0940 ; 
                   // FCB   $09                     ; 8A33:  ' ' 09 40          ROL   <$40 ; 
                   // FCB   $40                     ; 8A34:  '@' 40             NEGA  ; 
                   // FCB   $80                     ; 8A35:  ' ' 80 C0          SUBA  #$C0 ; 
                   // FCB   $C0                     ; 8A36:  ' ' C0 0B          SUBB  #$0B ; 
                   // FCB   $0B                     ; 8A37:  ' ' 0B             Invalid ; 
                   // FCB   $40                     ; 8A38:  '@' 40             NEGA  ; 
                   // FCB   $C0                     ; 8A39:  ' ' C0 80          SUBB  #$80 ; 
                   // FCB   $80                     ; 8A3A:  ' ' 80 0B          SUBA  #$0B ; 
                   // FCB   $0B                     ; 8A3B:  ' ' 0B             Invalid ; 
                   // FCB   $40                     ; 8A3C:  '@' 40             NEGA  ; 
                   // FCB   $C1                     ; 8A3D:  ' ' C1 7F          CMPB  #$7F ; 
                   // FCB   $7F                     ; 8A3E:  ' ' 7F 01 7F       CLR   $017F ; 
                   // FCB   $01                     ; 8A3F:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8A40:  ' ' 7F 00 7F       CLR   $007F ; 
                   // FCB   $00                     ; 8A41:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8A42:  ' ' 7F 00 7F       CLR   $007F ; 
                   // FCB   $00                     ; 8A43:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8A44:  ' ' 7F 01 7E       CLR   $017E ; 
                   // FCB   $01                     ; 8A45:  ' ' 01             Invalid ; 
                   // FCB   $7E                     ; 8A46:  '~' 7E 00 7F       JMP   $007F ; 
                   // FCB   $00                     ; 8A47:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8A48:  ' ' 7F 7E 01       CLR   $7E01 ; 
                   // FCB   $7E                     ; 8A49:  '~' 7E 01 00       JMP   $0100 ; 
                   // FCB   $01                     ; 8A4A:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8A4B:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8A4C:  ' ' 7F 7F 00       CLR   $7F00 ; 
                   // FCB   $7F                     ; 8A4D:  ' ' 7F 00 04       CLR   $0004 ; 
                   // FCB   $00                     ; 8A4E:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8A4F:  ' ' 04 7F          LSR   <$7F ; 
                   // FCB   $7F                     ; 8A50:  ' ' 7F 80 FF       CLR   $80FF ; 
                   // FCB   $80                     ; 8A51:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 8A52:  ' ' FF 0B 7F       STU   $0B7F ; 
                   // FCB   $0B                     ; 8A53:  ' ' 0B             Invalid ; 
                   // FCB   $7F                     ; 8A54:  ' ' 7F FF 80       CLR   $FF80 ; 
                   // FCB   $FF                     ; 8A55:  ' ' FF 80 0B       STU   $800B ; 
                   // FCB   $80                     ; 8A56:  ' ' 80 0B          SUBA  #$0B ; 
                   // FCB   $0B                     ; 8A57:  ' ' 0B             Invalid ; 
                   // FCB   $80                     ; 8A58:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8A59:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8A5A:  ' ' 7F 02 80       CLR   $0280 ; 
                   // FCB   $02                     ; 8A5B:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8A5C:  ' ' 80 40          SUBA  #$40 ; 
                   // FCB   $40                     ; 8A5D:  '@' 40             NEGA  ; 
                   // FCB   $40                     ; 8A5E:  '@' 40             NEGA  ; 
                   // FCB   $02                     ; 8A5F:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8A60:  ' ' 80 7F          SUBA  #$7F ; 
                   // FCB   $7F                     ; 8A61:  ' ' 7F 01 02       CLR   $0102 ; 
                   // FCB   $01                     ; 8A62:  ' ' 01             Invalid ; 
                   // FCB   $02                     ; 8A63:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8A64:  ' ' 80 80          SUBA  #$80 ; 
                   // FCB   $80                     ; 8A65:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8A66:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8A67:  ' ' 04 80          LSR   <$80 ; 
                   // FCB   $80                     ; 8A68:  ' ' 80 81          SUBA  #$81 ; 
                   // FCB   $81                     ; 8A69:  ' ' 81 FF          CMPA  #$FF ; 
                   // FCB   $FF                     ; 8A6A:  ' ' FF 09 81       STU   $0981 ; 
                   // FCB   $09                     ; 8A6B:  ' ' 09 81          ROL   <$81 ; 
                   // FCB   $81                     ; 8A6C:  ' ' 81 80          CMPA  #$80 ; 
                   // FCB   $80                     ; 8A6D:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8A6E:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8A6F:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8A70:  ' ' FF FF 00       STU   $FF00 ; 
                   // FCB   $FF                     ; 8A71:  ' ' FF 00 04       STU   $0004 ; 
                   // FCB   $00                     ; 8A72:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8A73:  ' ' 04 FF          LSR   <$FF ; 
                   // FCB   $FF                     ; 8A74:  ' ' FF FE 01       STU   $FE01 ; 
                   // FCB   $FE                     ; 8A75:  ' ' FE 01 00       LDU   $0100 ; 
                   // FCB   $01                     ; 8A76:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8A77:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8A78:  ' ' FF 00 FF       STU   $00FF ; 
                   // FCB   $00                     ; 8A79:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8A7A:  ' ' FF 08 53       STU   $0853 ; 
                   // FCB   $08                     ; 8A7B:  ' ' 08 53          ASL   <$53 ; 
                   // FCB   $53                     ; 8A7C:  'S' 53             COMB  ; 
                   // FCB   $55                     ; 8A7D:  'U' 55             Invalid ; 
                   // FCB   $42                     ; 8A7E:  'B' 42             Invalid ; 
                   // FCB   $04                     ; 8A7F:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8A80:
                                                     #ifdef DEBUG
                                      mon("L8A80    LDU   #$8BE0                  ; 8A80: CE 8B E0      \n");
                                      #endif
    U = 0x8be0; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8A83; 
    JUMP; // sstep
  case 0x8A83:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8AA8                  ; 8A83: 8E 8A A8      \n");
                                      #endif
    X = 0x8aa8; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8A86; 
    JUMP; // sstep
  case 0x8A86:
                                                     #ifdef DEBUG
                                      mon("L8A86    LDA   ,X+                     ; 8A86: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
    X = X + 1; 
    PC = 0x8A88; 
    JUMP; // sstep
  case 0x8A88:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 8A88: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8A8A; 
    JUMP; // sstep
  case 0x8A8A:
                                                     #ifdef DEBUG
                                      mon("         ANDB  ,X+                     ; 8A8A: E4 80         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8A8C; 
    JUMP; // sstep
  case 0x8A8C:
                                                     #ifdef DEBUG
                                      mon("         ORB   ,X+                     ; 8A8C: EA 80         \n");
                                      #endif
    res = B | (UINT8)rd_mem(X); 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8A8E; 
    JUMP; // sstep
  case 0x8A8E:
                                                     #ifdef DEBUG
                                      mon("         TFR   B,CC                    ; 8A8E: 1F 9A         \n");
                                      #endif
    CC = B; 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8A90; 
    JUMP; // sstep
  case 0x8A90:
                                                     #ifdef DEBUG
                                      mon("         SBCA  ,X+                     ; 8A90: A2 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg - (C != 0 ? 1:0); 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    A = (UINT8)res; 
    X = X + 1; 
    PC = 0x8A92; 
    JUMP; // sstep
  case 0x8A92:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,B                    ; 8A92: 1F A9         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    B = (UINT8)CC; 
    PC = 0x8A94; 
    JUMP; // sstep
  case 0x8A94:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8A94: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8A96; 
    JUMP; // sstep
  case 0x8A96:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8A96: 10 26 08 DB   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8A9A; 
    JUMP; // sstep
  case 0x8A9A:
                                                     #ifdef DEBUG
                                      mon("         ANDB  #$0F                    ; 8A9A: C4 0F         \n");
                                      #endif
    val = (UINT8)B; 
    res = val & (SINT8)0x0f; 
    B = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8A9C; 
    JUMP; // sstep
  case 0x8A9C:
                                                     #ifdef DEBUG
                                      mon("         CMPB  ,X+                     ; 8A9C: E1 80         \n");
                                      #endif
    val = (UINT8)B; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8A9E; 
    JUMP; // sstep
  case 0x8A9E:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8A9E: 10 26 08 D3   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8AA2; 
    JUMP; // sstep
  case 0x8AA2:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8BE0                  ; 8AA2: 8C 8B E0      \n");
                                      #endif
    val = X; 
    arg = 0x8be0; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8AA5; 
    JUMP; // sstep
  case 0x8AA5:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8A86                   ; 8AA5: 26 DF         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8a86; 
    JUMP;
    }
    PC = 0x8AA7; 
    JUMP; // sstep
  case 0x8AA7:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8AA7: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8AA8:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8AA9:  ' ' FE 00 00       LDU   $0000 ; 
                   // FCB   $00                     ; 8AAA:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8AAB:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8AAC:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8AAD:  ' ' 04 00          LSR   <$00 ; 
                   // FCB   $00                     ; 8AAE:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8AAF:  ' ' FE 00 01       LDU   $0001 ; 
                   // FCB   $00                     ; 8AB0:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8AB1:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8AB2:  ' ' FF 09 00       STU   $0900 ; 
                   // FCB   $09                     ; 8AB3:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 8AB4:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8AB5:  ' ' FE 00 7F       LDU   $007F ; 
                   // FCB   $00                     ; 8AB6:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8AB7:  ' ' 7F 81 09       CLR   $8109 ; 
                   // FCB   $81                     ; 8AB8:  ' ' 81 09          CMPA  #$09 ; 
                   // FCB   $09                     ; 8AB9:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 8ABA:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8ABB:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 8ABC:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8ABD:  ' ' 80 80          SUBA  #$80 ; 
                   // FCB   $80                     ; 8ABE:  ' ' 80 0B          SUBA  #$0B ; 
                   // FCB   $0B                     ; 8ABF:  ' ' 0B             Invalid ; 
                   // FCB   $00                     ; 8AC0:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8AC1:  ' ' FE 00 FF       LDU   $00FF ; 
                   // FCB   $00                     ; 8AC2:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8AC3:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 8AC4:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8AC5:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8AC6:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 8AC7:  ' ' FE 00 00       LDU   $0000 ; 
                   // FCB   $00                     ; 8AC8:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8AC9:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8ACA:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8ACB:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8ACC:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 8ACD:  ' ' FE 00 7F       LDU   $007F ; 
                   // FCB   $00                     ; 8ACE:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8ACF:  ' ' 7F 82 09       CLR   $8209 ; 
                   // FCB   $82                     ; 8AD0:  ' ' 82 09          SBCA  #$09 ; 
                   // FCB   $09                     ; 8AD1:  ' ' 09 40          ROL   <$40 ; 
                   // FCB   $40                     ; 8AD2:  '@' 40             NEGA  ; 
                   // FCB   $FE                     ; 8AD3:  ' ' FE 00 41       LDU   $0041 ; 
                   // FCB   $00                     ; 8AD4:  ' ' 00 41          NEG   <$41 ; 
                   // FCB   $41                     ; 8AD5:  'A' 41             Invalid ; 
                   // FCB   $FF                     ; 8AD6:  ' ' FF 09 40       STU   $0940 ; 
                   // FCB   $09                     ; 8AD7:  ' ' 09 40          ROL   <$40 ; 
                   // FCB   $40                     ; 8AD8:  '@' 40             NEGA  ; 
                   // FCB   $FE                     ; 8AD9:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 8ADA:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8ADB:  ' ' 80 C0          SUBA  #$C0 ; 
                   // FCB   $C0                     ; 8ADC:  ' ' C0 0B          SUBB  #$0B ; 
                   // FCB   $0B                     ; 8ADD:  ' ' 0B             Invalid ; 
                   // FCB   $40                     ; 8ADE:  '@' 40             NEGA  ; 
                   // FCB   $FE                     ; 8ADF:  ' ' FE 00 C0       LDU   $00C0 ; 
                   // FCB   $00                     ; 8AE0:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 8AE1:  ' ' C0 80          SUBB  #$80 ; 
                   // FCB   $80                     ; 8AE2:  ' ' 80 0B          SUBA  #$0B ; 
                   // FCB   $0B                     ; 8AE3:  ' ' 0B             Invalid ; 
                   // FCB   $40                     ; 8AE4:  '@' 40             NEGA  ; 
                   // FCB   $FE                     ; 8AE5:  ' ' FE 00 C1       LDU   $00C1 ; 
                   // FCB   $00                     ; 8AE6:  ' ' 00 C1          NEG   <$C1 ; 
                   // FCB   $C1                     ; 8AE7:  ' ' C1 7F          CMPB  #$7F ; 
                   // FCB   $7F                     ; 8AE8:  ' ' 7F 01 7F       CLR   $017F ; 
                   // FCB   $01                     ; 8AE9:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8AEA:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8AEB:  ' ' FE 00 00       LDU   $0000 ; 
                   // FCB   $00                     ; 8AEC:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8AED:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8AEE:  ' ' 7F 00 7F       CLR   $007F ; 
                   // FCB   $00                     ; 8AEF:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8AF0:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8AF1:  ' ' FE 00 01       LDU   $0001 ; 
                   // FCB   $00                     ; 8AF2:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8AF3:  ' ' 01             Invalid ; 
                   // FCB   $7E                     ; 8AF4:  '~' 7E 00 7F       JMP   $007F ; 
                   // FCB   $00                     ; 8AF5:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8AF6:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8AF7:  ' ' FE 00 7E       LDU   $007E ; 
                   // FCB   $00                     ; 8AF8:  ' ' 00 7E          NEG   <$7E ; 
                   // FCB   $7E                     ; 8AF9:  '~' 7E 01 00       JMP   $0100 ; 
                   // FCB   $01                     ; 8AFA:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8AFB:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8AFC:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8AFD:  ' ' FE 00 7F       LDU   $007F ; 
                   // FCB   $00                     ; 8AFE:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8AFF:  ' ' 7F 00 04       CLR   $0004 ; 
                   // FCB   $00                     ; 8B00:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8B01:  ' ' 04 7F          LSR   <$7F ; 
                   // FCB   $7F                     ; 8B02:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8B03:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 8B04:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8B05:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 8B06:  ' ' FF 0B 7F       STU   $0B7F ; 
                   // FCB   $0B                     ; 8B07:  ' ' 0B             Invalid ; 
                   // FCB   $7F                     ; 8B08:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8B09:  ' ' FE 00 FF       LDU   $00FF ; 
                   // FCB   $00                     ; 8B0A:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B0B:  ' ' FF 80 0B       STU   $800B ; 
                   // FCB   $80                     ; 8B0C:  ' ' 80 0B          SUBA  #$0B ; 
                   // FCB   $0B                     ; 8B0D:  ' ' 0B             Invalid ; 
                   // FCB   $80                     ; 8B0E:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 8B0F:  ' ' FE 00 01       LDU   $0001 ; 
                   // FCB   $00                     ; 8B10:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8B11:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8B12:  ' ' 7F 02 80       CLR   $0280 ; 
                   // FCB   $02                     ; 8B13:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8B14:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 8B15:  ' ' FE 00 40       LDU   $0040 ; 
                   // FCB   $00                     ; 8B16:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 8B17:  '@' 40             NEGA  ; 
                   // FCB   $40                     ; 8B18:  '@' 40             NEGA  ; 
                   // FCB   $02                     ; 8B19:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8B1A:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 8B1B:  ' ' FE 00 7F       LDU   $007F ; 
                   // FCB   $00                     ; 8B1C:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8B1D:  ' ' 7F 01 02       CLR   $0102 ; 
                   // FCB   $01                     ; 8B1E:  ' ' 01             Invalid ; 
                   // FCB   $02                     ; 8B1F:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8B20:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 8B21:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 8B22:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8B23:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8B24:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8B25:  ' ' 04 80          LSR   <$80 ; 
                   // FCB   $80                     ; 8B26:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 8B27:  ' ' FE 00 81       LDU   $0081 ; 
                   // FCB   $00                     ; 8B28:  ' ' 00 81          NEG   <$81 ; 
                   // FCB   $81                     ; 8B29:  ' ' 81 FF          CMPA  #$FF ; 
                   // FCB   $FF                     ; 8B2A:  ' ' FF 09 81       STU   $0981 ; 
                   // FCB   $09                     ; 8B2B:  ' ' 09 81          ROL   <$81 ; 
                   // FCB   $81                     ; 8B2C:  ' ' 81 FE          CMPA  #$FE ; 
                   // FCB   $FE                     ; 8B2D:  ' ' FE 00 80       LDU   $0080 ; 
                   // FCB   $00                     ; 8B2E:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8B2F:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8B30:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8B31:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B32:  ' ' FF FE 00       STU   $FE00 ; 
                   // FCB   $FE                     ; 8B33:  ' ' FE 00 FF       LDU   $00FF ; 
                   // FCB   $00                     ; 8B34:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B35:  ' ' FF 00 04       STU   $0004 ; 
                   // FCB   $00                     ; 8B36:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8B37:  ' ' 04 FF          LSR   <$FF ; 
                   // FCB   $FF                     ; 8B38:  ' ' FF FE 00       STU   $FE00 ; 
                   // FCB   $FE                     ; 8B39:  ' ' FE 00 FE       LDU   $00FE ; 
                   // FCB   $00                     ; 8B3A:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8B3B:  ' ' FE 01 00       LDU   $0100 ; 
                   // FCB   $01                     ; 8B3C:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8B3D:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B3E:  ' ' FF FE 00       STU   $FE00 ; 
                   // FCB   $FE                     ; 8B3F:  ' ' FE 00 00       LDU   $0000 ; 
                   // FCB   $00                     ; 8B40:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8B41:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B42:  ' ' FF 08 00       STU   $0800 ; 
                   // FCB   $08                     ; 8B43:  ' ' 08 00          ASL   <$00 ; 
                   // FCB   $00                     ; 8B44:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B45:  ' ' FF 01 00       STU   $0100 ; 
                   // FCB   $01                     ; 8B46:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8B47:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B48:  ' ' FF 09 00       STU   $0900 ; 
                   // FCB   $09                     ; 8B49:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 8B4A:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B4B:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 8B4C:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8B4D:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 8B4E:  ' ' FE 09 00       LDU   $0900 ; 
                   // FCB   $09                     ; 8B4F:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 8B50:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B51:  ' ' FF 01 7F       STU   $017F ; 
                   // FCB   $01                     ; 8B52:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8B53:  ' ' 7F 80 09       CLR   $8009 ; 
                   // FCB   $80                     ; 8B54:  ' ' 80 09          SUBA  #$09 ; 
                   // FCB   $09                     ; 8B55:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 8B56:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B57:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 8B58:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8B59:  ' ' 80 7F          SUBA  #$7F ; 
                   // FCB   $7F                     ; 8B5A:  ' ' 7F 01 00       CLR   $0100 ; 
                   // FCB   $01                     ; 8B5B:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8B5C:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8B5D:  ' ' FF 01 FF       STU   $01FF ; 
                   // FCB   $01                     ; 8B5E:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8B5F:  ' ' FF 00 05       STU   $0005 ; 
                   // FCB   $00                     ; 8B60:  ' ' 00 05          NEG   <$05 ; 
                   // FCB   $05                     ; 8B61:  ' ' 05             Invalid ; 
                   // FCB   $01                     ; 8B62:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8B63:  ' ' FF 01 00       STU   $0100 ; 
                   // FCB   $01                     ; 8B64:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8B65:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8B66:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8B67:  ' ' 04 01          LSR   <$01 ; 
                   // FCB   $01                     ; 8B68:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8B69:  ' ' FF 01 7F       STU   $017F ; 
                   // FCB   $01                     ; 8B6A:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8B6B:  ' ' 7F 81 09       CLR   $8109 ; 
                   // FCB   $81                     ; 8B6C:  ' ' 81 09          CMPA  #$09 ; 
                   // FCB   $09                     ; 8B6D:  ' ' 09 40          ROL   <$40 ; 
                   // FCB   $40                     ; 8B6E:  '@' 40             NEGA  ; 
                   // FCB   $FF                     ; 8B6F:  ' ' FF 01 41       STU   $0141 ; 
                   // FCB   $01                     ; 8B70:  ' ' 01             Invalid ; 
                   // FCB   $41                     ; 8B71:  'A' 41             Invalid ; 
                   // FCB   $FE                     ; 8B72:  ' ' FE 09 40       LDU   $0940 ; 
                   // FCB   $09                     ; 8B73:  ' ' 09 40          ROL   <$40 ; 
                   // FCB   $40                     ; 8B74:  '@' 40             NEGA  ; 
                   // FCB   $FF                     ; 8B75:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 8B76:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8B77:  ' ' 80 BF          SUBA  #$BF ; 
                   // FCB   $BF                     ; 8B78:  ' ' BF 0B 40       STX   $0B40 ; 
                   // FCB   $0B                     ; 8B79:  ' ' 0B             Invalid ; 
                   // FCB   $40                     ; 8B7A:  '@' 40             NEGA  ; 
                   // FCB   $FF                     ; 8B7B:  ' ' FF 01 C0       STU   $01C0 ; 
                   // FCB   $01                     ; 8B7C:  ' ' 01             Invalid ; 
                   // FCB   $C0                     ; 8B7D:  ' ' C0 7F          SUBB  #$7F ; 
                   // FCB   $7F                     ; 8B7E:  ' ' 7F 01 40       CLR   $0140 ; 
                   // FCB   $01                     ; 8B7F:  ' ' 01             Invalid ; 
                   // FCB   $40                     ; 8B80:  '@' 40             NEGA  ; 
                   // FCB   $FF                     ; 8B81:  ' ' FF 01 C1       STU   $01C1 ; 
                   // FCB   $01                     ; 8B82:  ' ' 01             Invalid ; 
                   // FCB   $C1                     ; 8B83:  ' ' C1 7E          CMPB  #$7E ; 
                   // FCB   $7E                     ; 8B84:  '~' 7E 01 7F       JMP   $017F ; 
                   // FCB   $01                     ; 8B85:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8B86:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 8B87:  ' ' FF 01 00       STU   $0100 ; 
                   // FCB   $01                     ; 8B88:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8B89:  ' ' 00 7E          NEG   <$7E ; 
                   // FCB   $7E                     ; 8B8A:  '~' 7E 00 7F       JMP   $007F ; 
                   // FCB   $00                     ; 8B8B:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8B8C:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 8B8D:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 8B8E:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8B8F:  ' ' 01             Invalid ; 
                   // FCB   $7D                     ; 8B90:  '}' 7D 00 7F       TST   $007F ; 
                   // FCB   $00                     ; 8B91:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8B92:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 8B93:  ' ' FF 01 7E       STU   $017E ; 
                   // FCB   $01                     ; 8B94:  ' ' 01             Invalid ; 
                   // FCB   $7E                     ; 8B95:  '~' 7E 00 04       JMP   $0004 ; 
                   // FCB   $00                     ; 8B96:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8B97:  ' ' 04 7F          LSR   <$7F ; 
                   // FCB   $7F                     ; 8B98:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 8B99:  ' ' FF 01 7F       STU   $017F ; 
                   // FCB   $01                     ; 8B9A:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8B9B:  ' ' 7F FF 09       CLR   $FF09 ; 
                   // FCB   $FF                     ; 8B9C:  ' ' FF 09 7F       STU   $097F ; 
                   // FCB   $09                     ; 8B9D:  ' ' 09 7F          ROL   <$7F ; 
                   // FCB   $7F                     ; 8B9E:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 8B9F:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 8BA0:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8BA1:  ' ' 80 FE          SUBA  #$FE ; 
                   // FCB   $FE                     ; 8BA2:  ' ' FE 0B 7F       LDU   $0B7F ; 
                   // FCB   $0B                     ; 8BA3:  ' ' 0B             Invalid ; 
                   // FCB   $7F                     ; 8BA4:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 8BA5:  ' ' FF 01 FF       STU   $01FF ; 
                   // FCB   $01                     ; 8BA6:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8BA7:  ' ' FF 7F 01       STU   $7F01 ; 
                   // FCB   $7F                     ; 8BA8:  ' ' 7F 01 80       CLR   $0180 ; 
                   // FCB   $01                     ; 8BA9:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8BAA:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 8BAB:  ' ' FF 01 01       STU   $0101 ; 
                   // FCB   $01                     ; 8BAC:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8BAD:  ' ' 01             Invalid ; 
                   // FCB   $7E                     ; 8BAE:  '~' 7E 02 80       JMP   $0280 ; 
                   // FCB   $02                     ; 8BAF:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8BB0:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 8BB1:  ' ' FF 01 40       STU   $0140 ; 
                   // FCB   $01                     ; 8BB2:  ' ' 01             Invalid ; 
                   // FCB   $40                     ; 8BB3:  '@' 40             NEGA  ; 
                   // FCB   $3F                     ; 8BB4:  '?' 3F             SWI   ; 
                   // FCB   $02                     ; 8BB5:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8BB6:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 8BB7:  ' ' FF 01 7F       STU   $017F ; 
                   // FCB   $01                     ; 8BB8:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8BB9:  ' ' 7F 00 06       CLR   $0006 ; 
                   // FCB   $00                     ; 8BBA:  ' ' 00 06          NEG   <$06 ; 
                   // FCB   $06                     ; 8BBB:  ' ' 06 80          ROR   <$80 ; 
                   // FCB   $80                     ; 8BBC:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 8BBD:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 8BBE:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8BBF:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 8BC0:  ' ' FF 09 80       STU   $0980 ; 
                   // FCB   $09                     ; 8BC1:  ' ' 09 80          ROL   <$80 ; 
                   // FCB   $80                     ; 8BC2:  ' ' 80 FF          SUBA  #$FF ; 
                   // FCB   $FF                     ; 8BC3:  ' ' FF 01 81       STU   $0181 ; 
                   // FCB   $01                     ; 8BC4:  ' ' 01             Invalid ; 
                   // FCB   $81                     ; 8BC5:  ' ' 81 FE          CMPA  #$FE ; 
                   // FCB   $FE                     ; 8BC6:  ' ' FE 09 81       LDU   $0981 ; 
                   // FCB   $09                     ; 8BC7:  ' ' 09 81          ROL   <$81 ; 
                   // FCB   $81                     ; 8BC8:  ' ' 81 FF          CMPA  #$FF ; 
                   // FCB   $FF                     ; 8BC9:  ' ' FF 01 80       STU   $0180 ; 
                   // FCB   $01                     ; 8BCA:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8BCB:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8BCC:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8BCD:  ' ' 04 FF          LSR   <$FF ; 
                   // FCB   $FF                     ; 8BCE:  ' ' FF FF 01       STU   $FF01 ; 
                   // FCB   $FF                     ; 8BCF:  ' ' FF 01 FF       STU   $01FF ; 
                   // FCB   $01                     ; 8BD0:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8BD1:  ' ' FF FF 09       STU   $FF09 ; 
                   // FCB   $FF                     ; 8BD2:  ' ' FF 09 FF       STU   $09FF ; 
                   // FCB   $09                     ; 8BD3:  ' ' 09 FF          ROL   <$FF ; 
                   // FCB   $FF                     ; 8BD4:  ' ' FF FF 01       STU   $FF01 ; 
                   // FCB   $FF                     ; 8BD5:  ' ' FF 01 FE       STU   $01FE ; 
                   // FCB   $01                     ; 8BD6:  ' ' 01             Invalid ; 
                   // FCB   $FE                     ; 8BD7:  ' ' FE 00 04       LDU   $0004 ; 
                   // FCB   $00                     ; 8BD8:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8BD9:  ' ' 04 FF          LSR   <$FF ; 
                   // FCB   $FF                     ; 8BDA:  ' ' FF FF 01       STU   $FF01 ; 
                   // FCB   $FF                     ; 8BDB:  ' ' FF 01 00       STU   $0100 ; 
                   // FCB   $01                     ; 8BDC:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8BDD:  ' ' 00 FE          NEG   <$FE ; 
                   // FCB   $FE                     ; 8BDE:  ' ' FE 08 53       LDU   $0853 ; 
                   // FCB   $08                     ; 8BDF:  ' ' 08 53          ASL   <$53 ; 
                   // FCB   $53                     ; 8BE0:  'S' 53             COMB  ; 
                   // FCB   $42                     ; 8BE1:  'B' 42             Invalid ; 
                   // FCB   $43                     ; 8BE2:  'C' 43             COMA  ; 
                   // FCB   $04                     ; 8BE3:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8BE4:
                                                     #ifdef DEBUG
                                      mon("L8BE4    LDU   #$8CBD                  ; 8BE4: CE 8C BD      \n");
                                      #endif
    U = 0x8cbd; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8BE7; 
    JUMP; // sstep
  case 0x8BE7:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8C07                  ; 8BE7: 8E 8C 07      \n");
                                      #endif
    X = 0x8c07; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8BEA; 
    JUMP; // sstep
  case 0x8BEA:
                                                     #ifdef DEBUG
                                      mon("L8BEA    LDD   ,X++                    ; 8BEA: EC 81         \n");
                                      #endif
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
    X = X + 2; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8BEC; 
    JUMP; // sstep
  case 0x8BEC:
                                                     #ifdef DEBUG
                                      mon("         SUBD  ,X++                    ; 8BEC: A3 81         \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
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
    X = X + 2; 
    PC = 0x8BEE; 
    JUMP; // sstep
  case 0x8BEE:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,DP                   ; 8BEE: 1F AB         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    DP = (CC) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8BF0; 
    JUMP; // sstep
  case 0x8BF0:
                                                     #ifdef DEBUG
                                      mon("         CMPD  ,X++                    ; 8BF0: 10 A3 81      \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = ea; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    X = X + 2; 
    PC = 0x8BF3; 
    JUMP; // sstep
  case 0x8BF3:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8BF3: 10 26 07 7E   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8BF7; 
    JUMP; // sstep
  case 0x8BF7:
                                                     #ifdef DEBUG
                                      mon("         TFR   DP,A                    ; 8BF7: 1F B8         \n");
                                      #endif
    A = (UINT8)(UINT8)(DP >> 8); 
    PC = 0x8BF9; 
    JUMP; // sstep
  case 0x8BF9:
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$0F                    ; 8BF9: 84 0F         \n");
                                      #endif
    val = (UINT8)A; 
    res = val & (SINT8)0x0f; 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8BFB; 
    JUMP; // sstep
  case 0x8BFB:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8BFB: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8BFD; 
    JUMP; // sstep
  case 0x8BFD:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8BFD: 10 26 07 74   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8C01; 
    JUMP; // sstep
  case 0x8C01:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8CBD                  ; 8C01: 8C 8C BD      \n");
                                      #endif
    val = X; 
    arg = 0x8cbd; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8C04; 
    JUMP; // sstep
  case 0x8C04:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8BEA                   ; 8C04: 26 E4         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8bea; 
    JUMP;
    }
    PC = 0x8C06; 
    JUMP; // sstep
  case 0x8C06:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8C06: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 8C07:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C08:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C09:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C0A:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C0B:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C0C:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8C0D:  ' ' 04 00          LSR   <$00 ; 
                   // FCB   $00                     ; 8C0E:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C0F:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C10:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8C11:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8C12:  ' ' FF FF 09       STU   $FF09 ; 
                   // FCB   $FF                     ; 8C13:  ' ' FF 09 00       STU   $0900 ; 
                   // FCB   $09                     ; 8C14:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 8C15:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C16:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8C17:  ' ' 7F FF 80       CLR   $FF80 ; 
                   // FCB   $FF                     ; 8C18:  ' ' FF 80 01       STU   $8001 ; 
                   // FCB   $80                     ; 8C19:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8C1A:  ' ' 01             Invalid ; 
                   // FCB   $09                     ; 8C1B:  ' ' 09 00          ROL   <$00 ; 
                   // FCB   $00                     ; 8C1C:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C1D:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8C1E:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C1F:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8C20:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C21:  ' ' 00 0B          NEG   <$0B ; 
                   // FCB   $0B                     ; 8C22:  ' ' 0B             Invalid ; 
                   // FCB   $00                     ; 8C23:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C24:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8C25:  ' ' FF FF 00       STU   $FF00 ; 
                   // FCB   $FF                     ; 8C26:  ' ' FF 00 01       STU   $0001 ; 
                   // FCB   $00                     ; 8C27:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8C28:  ' ' 01             Invalid ; 
                   // FCB   $01                     ; 8C29:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8C2A:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8C2B:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8C2C:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C2D:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C2E:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8C2F:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8C30:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C31:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8C32:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8C33:  ' ' 7F FF 80       CLR   $FF80 ; 
                   // FCB   $FF                     ; 8C34:  ' ' FF 80 02       STU   $8002 ; 
                   // FCB   $80                     ; 8C35:  ' ' 80 02          SUBA  #$02 ; 
                   // FCB   $02                     ; 8C36:  ' ' 02             Invalid ; 
                   // FCB   $09                     ; 8C37:  ' ' 09 40          ROL   <$40 ; 
                   // FCB   $40                     ; 8C38:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 8C39:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 8C3A:  '@' 40             NEGA  ; 
                   // FCB   $01                     ; 8C3B:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8C3C:  ' ' FF FF 09       STU   $FF09 ; 
                   // FCB   $FF                     ; 8C3D:  ' ' FF 09 40       STU   $0940 ; 
                   // FCB   $09                     ; 8C3E:  ' ' 09 40          ROL   <$40 ; 
                   // FCB   $40                     ; 8C3F:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 8C40:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8C41:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C42:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 8C43:  ' ' C0 00          SUBB  #$00 ; 
                   // FCB   $00                     ; 8C44:  ' ' 00 0B          NEG   <$0B ; 
                   // FCB   $0B                     ; 8C45:  ' ' 0B             Invalid ; 
                   // FCB   $40                     ; 8C46:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 8C47:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 8C48:  ' ' C0 00          SUBB  #$00 ; 
                   // FCB   $00                     ; 8C49:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8C4A:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C4B:  ' ' 00 0B          NEG   <$0B ; 
                   // FCB   $0B                     ; 8C4C:  ' ' 0B             Invalid ; 
                   // FCB   $40                     ; 8C4D:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 8C4E:  ' ' 00 C0          NEG   <$C0 ; 
                   // FCB   $C0                     ; 8C4F:  ' ' C0 01          SUBB  #$01 ; 
                   // FCB   $01                     ; 8C50:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8C51:  ' ' 7F FF 01       CLR   $FF01 ; 
                   // FCB   $FF                     ; 8C52:  ' ' FF 01 7F       STU   $017F ; 
                   // FCB   $01                     ; 8C53:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8C54:  ' ' 7F FF 00       CLR   $FF00 ; 
                   // FCB   $FF                     ; 8C55:  ' ' FF 00 00       STU   $0000 ; 
                   // FCB   $00                     ; 8C56:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C57:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8C58:  ' ' 7F FF 00       CLR   $FF00 ; 
                   // FCB   $FF                     ; 8C59:  ' ' FF 00 7F       STU   $007F ; 
                   // FCB   $00                     ; 8C5A:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8C5B:  ' ' 7F FF 00       CLR   $FF00 ; 
                   // FCB   $FF                     ; 8C5C:  ' ' FF 00 01       STU   $0001 ; 
                   // FCB   $00                     ; 8C5D:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8C5E:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8C5F:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8C60:  ' ' FE 00 7F       LDU   $007F ; 
                   // FCB   $00                     ; 8C61:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8C62:  ' ' 7F FF 7F       CLR   $FF7F ; 
                   // FCB   $FF                     ; 8C63:  ' ' FF 7F FE       STU   $7FFE ; 
                   // FCB   $7F                     ; 8C64:  ' ' 7F FE 00       CLR   $FE00 ; 
                   // FCB   $FE                     ; 8C65:  ' ' FE 00 01       LDU   $0001 ; 
                   // FCB   $00                     ; 8C66:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8C67:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8C68:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8C69:  ' ' 7F FF 7F       CLR   $FF7F ; 
                   // FCB   $FF                     ; 8C6A:  ' ' FF 7F FF       STU   $7FFF ; 
                   // FCB   $7F                     ; 8C6B:  ' ' 7F FF 00       CLR   $FF00 ; 
                   // FCB   $FF                     ; 8C6C:  ' ' FF 00 00       STU   $0000 ; 
                   // FCB   $00                     ; 8C6D:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C6E:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8C6F:  ' ' 04 7F          LSR   <$7F ; 
                   // FCB   $7F                     ; 8C70:  ' ' 7F FF 80       CLR   $FF80 ; 
                   // FCB   $FF                     ; 8C71:  ' ' FF 80 00       STU   $8000 ; 
                   // FCB   $80                     ; 8C72:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C73:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8C74:  ' ' FF FF 0B       STU   $FF0B ; 
                   // FCB   $FF                     ; 8C75:  ' ' FF 0B 7F       STU   $0B7F ; 
                   // FCB   $0B                     ; 8C76:  ' ' 0B             Invalid ; 
                   // FCB   $7F                     ; 8C77:  ' ' 7F FF FF       CLR   $FFFF ; 
                   // FCB   $FF                     ; 8C78:  ' ' FF FF FF       STU   $FFFF ; 
                   // FCB   $FF                     ; 8C79:  ' ' FF FF 80       STU   $FF80 ; 
                   // FCB   $FF                     ; 8C7A:  ' ' FF 80 00       STU   $8000 ; 
                   // FCB   $80                     ; 8C7B:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C7C:  ' ' 00 0B          NEG   <$0B ; 
                   // FCB   $0B                     ; 8C7D:  ' ' 0B             Invalid ; 
                   // FCB   $80                     ; 8C7E:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C7F:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C80:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8C81:  ' ' 01             Invalid ; 
                   // FCB   $7F                     ; 8C82:  ' ' 7F FF 02       CLR   $FF02 ; 
                   // FCB   $FF                     ; 8C83:  ' ' FF 02 80       STU   $0280 ; 
                   // FCB   $02                     ; 8C84:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8C85:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C86:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 8C87:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 8C88:  ' ' 00 40          NEG   <$40 ; 
                   // FCB   $40                     ; 8C89:  '@' 40             NEGA  ; 
                   // FCB   $00                     ; 8C8A:  ' ' 00 02          NEG   <$02 ; 
                   // FCB   $02                     ; 8C8B:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8C8C:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C8D:  ' ' 00 7F          NEG   <$7F ; 
                   // FCB   $7F                     ; 8C8E:  ' ' 7F FF 00       CLR   $FF00 ; 
                   // FCB   $FF                     ; 8C8F:  ' ' FF 00 01       STU   $0001 ; 
                   // FCB   $00                     ; 8C90:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8C91:  ' ' 01             Invalid ; 
                   // FCB   $02                     ; 8C92:  ' ' 02             Invalid ; 
                   // FCB   $80                     ; 8C93:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C94:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8C95:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C96:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C97:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8C98:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8C99:  ' ' 04 80          LSR   <$80 ; 
                   // FCB   $80                     ; 8C9A:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8C9B:  ' ' 00 80          NEG   <$80 ; 
                   // FCB   $80                     ; 8C9C:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8C9D:  ' ' 01             Invalid ; 
                   // FCB   $FF                     ; 8C9E:  ' ' FF FF 09       STU   $FF09 ; 
                   // FCB   $FF                     ; 8C9F:  ' ' FF 09 80       STU   $0980 ; 
                   // FCB   $09                     ; 8CA0:  ' ' 09 80          ROL   <$80 ; 
                   // FCB   $80                     ; 8CA1:  ' ' 80 01          SUBA  #$01 ; 
                   // FCB   $01                     ; 8CA2:  ' ' 01             Invalid ; 
                   // FCB   $80                     ; 8CA3:  ' ' 80 00          SUBA  #$00 ; 
                   // FCB   $00                     ; 8CA4:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8CA5:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8CA6:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8CA7:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8CA8:  ' ' FF FF FF       STU   $FFFF ; 
                   // FCB   $FF                     ; 8CA9:  ' ' FF FF FF       STU   $FFFF ; 
                   // FCB   $FF                     ; 8CAA:  ' ' FF FF 00       STU   $FF00 ; 
                   // FCB   $FF                     ; 8CAB:  ' ' FF 00 00       STU   $0000 ; 
                   // FCB   $00                     ; 8CAC:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8CAD:  ' ' 00 04          NEG   <$04 ; 
                   // FCB   $04                     ; 8CAE:  ' ' 04 FF          LSR   <$FF ; 
                   // FCB   $FF                     ; 8CAF:  ' ' FF FF FF       STU   $FFFF ; 
                   // FCB   $FF                     ; 8CB0:  ' ' FF FF FE       STU   $FFFE ; 
                   // FCB   $FF                     ; 8CB1:  ' ' FF FE 00       STU   $FE00 ; 
                   // FCB   $FE                     ; 8CB2:  ' ' FE 00 01       LDU   $0001 ; 
                   // FCB   $00                     ; 8CB3:  ' ' 00 01          NEG   <$01 ; 
                   // FCB   $01                     ; 8CB4:  ' ' 01             Invalid ; 
                   // FCB   $00                     ; 8CB5:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8CB6:  ' ' FF FF 00       STU   $FF00 ; 
                   // FCB   $FF                     ; 8CB7:  ' ' FF 00 00       STU   $0000 ; 
                   // FCB   $00                     ; 8CB8:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 8CB9:  ' ' 00 FF          NEG   <$FF ; 
                   // FCB   $FF                     ; 8CBA:  ' ' FF FF 08       STU   $FF08 ; 
                   // FCB   $FF                     ; 8CBB:  ' ' FF 08 53       STU   $0853 ; 
                   // FCB   $08                     ; 8CBC:  ' ' 08 53          ASL   <$53 ; 
                   // FCB   $53                     ; 8CBD:  'S' 53             COMB  ; 
                   // FCB   $55                     ; 8CBE:  'U' 55             Invalid ; 
                   // FCB   $42                     ; 8CBF:  'B' 42             Invalid ; 
                   // FCB   $44                     ; 8CC0:  'D' 44             LSRA  ; 
                   // FCB   $04                     ; 8CC1:  ' ' 04 CE          LSR   <$CE ; 
  case 0x8CC2:
                                                     #ifdef DEBUG
                                      mon("L8CC2    LDU   #$8CE0                  ; 8CC2: CE 8C E0      \n");
                                      #endif
    U = 0x8ce0; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8CC5; 
    JUMP; // sstep
  case 0x8CC5:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8C07                  ; 8CC5: 8E 8C 07      \n");
                                      #endif
    X = 0x8c07; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8CC8; 
    JUMP; // sstep
  case 0x8CC8:
                                                     #ifdef DEBUG
                                      mon("L8CC8    LDD   ,X++                    ; 8CC8: EC 81         \n");
                                      #endif
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
    X = X + 2; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8CCA; 
    JUMP; // sstep
  case 0x8CCA:
                                                     #ifdef DEBUG
                                      mon("         SUBD  ,X++                    ; 8CCA: A3 81         \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
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
    X = X + 2; 
    PC = 0x8CCC; 
    JUMP; // sstep
  case 0x8CCC:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,DP                   ; 8CCC: 1F AB         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    DP = (CC) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8CCE; 
    JUMP; // sstep
  case 0x8CCE:
                                                     #ifdef DEBUG
                                      mon("         LEAX  $02,X                   ; 8CCE: 30 02         \n");
                                      #endif
    X = (X + 0x0002); 
/**/Z = X; 
    PC = 0x8CD0; 
    JUMP; // sstep
  case 0x8CD0:
                                                     #ifdef DEBUG
                                      mon("         TFR   DP,A                    ; 8CD0: 1F B8         \n");
                                      #endif
    A = (UINT8)(UINT8)(DP >> 8); 
    PC = 0x8CD2; 
    JUMP; // sstep
  case 0x8CD2:
                                                     #ifdef DEBUG
                                      mon("         ANDA  #$0F                    ; 8CD2: 84 0F         \n");
                                      #endif
    val = (UINT8)A; 
    res = val & (SINT8)0x0f; 
    A = res; 
/**/Z = res; 
/**/N = res; 
/**/V = 0; 
    PC = 0x8CD4; 
    JUMP; // sstep
  case 0x8CD4:
                                                     #ifdef DEBUG
                                      mon("         CMPA  ,X+                     ; 8CD4: A1 80         \n");
                                      #endif
    val = (UINT8)A; 
    arg = rd_mem(X); 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    X = X + 1; 
    PC = 0x8CD6; 
    JUMP; // sstep
  case 0x8CD6:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8CD6: 10 26 06 9B   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8CDA; 
    JUMP; // sstep
  case 0x8CDA:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8CBD                  ; 8CDA: 8C 8C BD      \n");
                                      #endif
    val = X; 
    arg = 0x8cbd; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8CDD; 
    JUMP; // sstep
  case 0x8CDD:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8CC8                   ; 8CDD: 26 E9         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8cc8; 
    JUMP;
    }
    PC = 0x8CDF; 
    JUMP; // sstep
  case 0x8CDF:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8CDF: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $43                     ; 8CE0:  'C' 43             COMA  ; 
                   // FCB   $4D                     ; 8CE1:  'M' 4D             TSTA  ; 
                   // FCB   $50                     ; 8CE2:  'P' 50             NEGB  ; 
                   // FCB   $44                     ; 8CE3:  'D' 44             LSRA  ; 
                   // FCB   $04                     ; 8CE4:  ' ' 04 8E          LSR   <$8E ; 
  case 0x8CE5:
                                                     #ifdef DEBUG
                                      mon("L8CE5    LDX   #$8D77                  ; 8CE5: 8E 8D 77      \n");
                                      #endif
    X = 0x8d77; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8CE8; 
    JUMP; // sstep
  case 0x8CE8:
                                                     #ifdef DEBUG
                                      mon("L8CE8    PSHS  X                       ; 8CE8: 34 10         \n");
                                      #endif
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    PC = 0x8CEA; 
    JUMP; // sstep
  case 0x8CEA:
                                                     #ifdef DEBUG
                                      mon("         LDY   ,X                      ; 8CEA: 10 AE 84      \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    Y = ea; 
    Z = Y; 
    N = (Y) >> 8; 
    V = 0; 
    PC = 0x8CED; 
    JUMP; // sstep
  case 0x8CED:
                                                     #ifdef DEBUG
                                      mon("         JSR   ,Y                      ; 8CED: AD A4         \n");
                                      #endif
    --S;
    wr_mem(S, 0xef); 
    --S;
    wr_mem(S, 0x8c); 
    PC = Y; 
    JUMP;
  case 0x8CEF:
                                                     #ifdef DEBUG
                                      mon("L8CEF    PULS  X                       ; 8CEF: 35 10         \n");
                                      #endif
    X = rd_mem(S)<<8; 
    S++;
    X = X | rd_mem(S); 
    S++;
    PC = 0x8CF1; 
    JUMP; // sstep
  case 0x8CF1:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8CFB                   ; 8CF1: 26 08         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8cfb; 
    JUMP;
    }
    PC = 0x8CF3; 
    JUMP; // sstep
  case 0x8CF3:
                                                     #ifdef DEBUG
                                      mon("         LEAX  $02,X                   ; 8CF3: 30 02         \n");
                                      #endif
    X = (X + 0x0002); 
/**/Z = X; 
    PC = 0x8CF5; 
    JUMP; // sstep
  case 0x8CF5:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8D87                  ; 8CF5: 8C 8D 87      \n");
                                      #endif
    val = X; 
    arg = 0x8d87; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8CF8; 
    JUMP; // sstep
  case 0x8CF8:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8CE8                   ; 8CF8: 26 EE         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8ce8; 
    JUMP;
    }
    PC = 0x8CFA; 
    JUMP; // sstep
  case 0x8CFA:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8CFA: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8CFB:
                                                     #ifdef DEBUG
                                      mon("L8CFB    LDU   #$8D87                  ; 8CFB: CE 8D 87      \n");
                                      #endif
    U = 0x8d87; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x8CFE; 
    JUMP; // sstep
  case 0x8CFE:
                                                     #ifdef DEBUG
                                      mon("         LBRA  $9375                   ; 8CFE: 16 06 74      \n");
                                      #endif
    PC = 0x9375; 
    JUMP;
    
  case 0x8D01:
                                                     #ifdef DEBUG
                                      mon("L8D01    CLRA                          ; 8D01: 4F            \n");
                                      #endif
    A = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    PC = 0x8D02; 
    JUMP; // sstep
  case 0x8D02:
                                                     #ifdef DEBUG
                                      mon("         CLRB                          ; 8D02: 5F            \n");
                                      #endif
    B = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    PC = 0x8D03; 
    JUMP; // sstep
  case 0x8D03:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$00                    ; 8D03: 8E 00 00      \n");
                                      #endif
    X = 0x0000; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8D06; 
    JUMP; // sstep
  case 0x8D06:
                                                     #ifdef DEBUG
                                      mon("         LDY   #$00                    ; 8D06: 10 8E 00 00   \n");
                                      #endif
    Y = 0x0000; 
/**/Z = Y; 
/**/N = (Y) >> 8; 
/**/V = 0; 
    PC = 0x8D0A; 
    JUMP; // sstep
  case 0x8D0A:
                                                     #ifdef DEBUG
                                      mon("         LDU   #$00                    ; 8D0A: CE 00 00      \n");
                                      #endif
    U = 0x0000; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8D0D; 
    JUMP; // sstep
  case 0x8D0D:
                                                     #ifdef DEBUG
                                      mon("         TFR   A,DP                    ; 8D0D: 1F 8B         \n");
                                      #endif
    DP = (A) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8D0F; 
    JUMP; // sstep
  case 0x8D0F:
                                                     #ifdef DEBUG
                                      mon("         TFR   A,CC                    ; 8D0F: 1F 8A         \n");
                                      #endif
    CC = A; 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8D11; 
    JUMP; // sstep
  case 0x8D11:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8D11: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8D12:
                                                     #ifdef DEBUG
                                      mon("L8D12    BSR   $8D01                   ; 8D12: 8D ED         \n");
                                      #endif
    --S;
    wr_mem(S, 0x14); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8D14:
                                                     #ifdef DEBUG
                                      mon("L8D14    LDD   #$5678                  ; 8D14: CC 56 78      \n");
                                      #endif
    A = 0x56; 
    B = 0x78; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8D17; 
    JUMP; // sstep
  case 0x8D17:
                                                     #ifdef DEBUG
                                      mon("         TFR   D,X                     ; 8D17: 1F 01         \n");
                                      #endif
    X = ((A << 8) | B); 
    PC = 0x8D19; 
    JUMP; // sstep
  case 0x8D19:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$5678                  ; 8D19: 8C 56 78      \n");
                                      #endif
    val = X; 
    arg = 0x5678; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8D1C; 
    JUMP; // sstep
  case 0x8D1C:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8D1C: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8D1D:
                                                     #ifdef DEBUG
                                      mon("L8D1D    BSR   $8D01                   ; 8D1D: 8D E2         \n");
                                      #endif
    --S;
    wr_mem(S, 0x1f); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8D1F:
                                                     #ifdef DEBUG
                                      mon("L8D1F    LDX   #$1234                  ; 8D1F: 8E 12 34      \n");
                                      #endif
    X = 0x1234; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8D22; 
    JUMP; // sstep
  case 0x8D22:
                                                     #ifdef DEBUG
                                      mon("         TFR   X,Y                     ; 8D22: 1F 12         \n");
                                      #endif
    Y = X; 
    PC = 0x8D24; 
    JUMP; // sstep
  case 0x8D24:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$1234                  ; 8D24: 10 8C 12 34   \n");
                                      #endif
    val = Y; 
    arg = 0x1234; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8D28; 
    JUMP; // sstep
  case 0x8D28:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8D28: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8D29:
                                                     #ifdef DEBUG
                                      mon("L8D29    BSR   $8D01                   ; 8D29: 8D D6         \n");
                                      #endif
    --S;
    wr_mem(S, 0x2b); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8D2B:
                                                     #ifdef DEBUG
                                      mon("L8D2B    LDY   #$5500                  ; 8D2B: 10 8E 55 00   \n");
                                      #endif
    Y = 0x5500; 
/**/Z = Y; 
/**/N = (Y) >> 8; 
/**/V = 0; 
    PC = 0x8D2F; 
    JUMP; // sstep
  case 0x8D2F:
                                                     #ifdef DEBUG
                                      mon("         TFR   Y,U                     ; 8D2F: 1F 23         \n");
                                      #endif
    U = Y; 
    PC = 0x8D31; 
    JUMP; // sstep
  case 0x8D31:
                                                     #ifdef DEBUG
                                      mon("         CMPU  #$5500                  ; 8D31: 11 83 55 00   \n");
                                      #endif
    val = U; 
    arg = 0x5500; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8D35; 
    JUMP; // sstep
  case 0x8D35:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8D35: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8D36:
                                                     #ifdef DEBUG
                                      mon("L8D36    BSR   $8D01                   ; 8D36: 8D C9         \n");
                                      #endif
    --S;
    wr_mem(S, 0x38); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8D38:
                                                     #ifdef DEBUG
                                      mon("L8D38    LDU   #$FEFF                  ; 8D38: CE FE FF      \n");
                                      #endif
    U = 0xfeff; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8D3B; 
    JUMP; // sstep
  case 0x8D3B:
                                                     #ifdef DEBUG
                                      mon("         TFR   U,D                     ; 8D3B: 1F 30         \n");
                                      #endif
    val = U; 
    A = (UINT8)(val >> 8); 
    B = (UINT8)val; 
    PC = 0x8D3D; 
    JUMP; // sstep
  case 0x8D3D:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$FEFF                  ; 8D3D: 10 83 FE FF   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0xfeff; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8D41; 
    JUMP; // sstep
  case 0x8D41:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8D41: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8D42:
                                                     #ifdef DEBUG
                                      mon("L8D42    BSR   $8D01                   ; 8D42: 8D BD         \n");
                                      #endif
    --S;
    wr_mem(S, 0x44); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8D44:
                                                     #ifdef DEBUG
                                      mon("L8D44    LDA   #$55                    ; 8D44: 86 55         \n");
                                      #endif
    val = (UINT8)0x55; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8D46; 
    JUMP; // sstep
  case 0x8D46:
                                                     #ifdef DEBUG
                                      mon("         TFR   A,B                     ; 8D46: 1F 89         \n");
                                      #endif
    B = (UINT8)A; 
    PC = 0x8D48; 
    JUMP; // sstep
  case 0x8D48:
                                                     #ifdef DEBUG
                                      mon("         CMPB  #$55                    ; 8D48: C1 55         \n");
                                      #endif
    val = (UINT8)B; 
    arg = 0x55; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8D4A; 
    JUMP; // sstep
  case 0x8D4A:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8D4A: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8D4B:
                                                     #ifdef DEBUG
                                      mon("L8D4B    BSR   $8D01                   ; 8D4B: 8D B4         \n");
                                      #endif
    --S;
    wr_mem(S, 0x4d); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8D4D:
                                                     #ifdef DEBUG
                                      mon("L8D4D    LDB   #$AA                    ; 8D4D: C6 AA         \n");
                                      #endif
    val = (UINT8)0xaa; 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8D4F; 
    JUMP; // sstep
  case 0x8D4F:
                                                     #ifdef DEBUG
                                      mon("         TFR   B,DP                    ; 8D4F: 1F 9B         \n");
                                      #endif
    DP = (B) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8D51; 
    JUMP; // sstep
  case 0x8D51:
                                                     #ifdef DEBUG
                                      mon("         PSHS  DP                      ; 8D51: 34 08         \n");
                                      #endif
    --S;
    wr_mem(S, (DP >> 8)); 
    PC = 0x8D53; 
    JUMP; // sstep
  case 0x8D53:
                                                     #ifdef DEBUG
                                      mon("         PULS  A                       ; 8D53: 35 02         \n");
                                      #endif
    A = rd_mem(S); 
    S++;
    PC = 0x8D55; 
    JUMP; // sstep
  case 0x8D55:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$AA                    ; 8D55: 81 AA         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0xaa; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8D57; 
    JUMP; // sstep
  case 0x8D57:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8D57: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8D58:
                                                     #ifdef DEBUG
                                      mon("L8D58    BSR   $8D01                   ; 8D58: 8D A7         \n");
                                      #endif
    --S;
    wr_mem(S, 0x5a); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8D5A:
                                                     #ifdef DEBUG
                                      mon("L8D5A    LDA   #$03                    ; 8D5A: 86 03         \n");
                                      #endif
    val = (UINT8)0x03; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8D5C; 
    JUMP; // sstep
  case 0x8D5C:
                                                     #ifdef DEBUG
                                      mon("         PSHS  A                       ; 8D5C: 34 02         \n");
                                      #endif
    --S;
    wr_mem(S, A); 
    PC = 0x8D5E; 
    JUMP; // sstep
  case 0x8D5E:
                                                     #ifdef DEBUG
                                      mon("         CLRA                          ; 8D5E: 4F            \n");
                                      #endif
    A = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    PC = 0x8D5F; 
    JUMP; // sstep
  case 0x8D5F:
                                                     #ifdef DEBUG
                                      mon("         PULS  DP                      ; 8D5F: 35 08         \n");
                                      #endif
    DP = rd_mem(S); 
    S++;
    DP = DP << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8D61; 
    JUMP; // sstep
  case 0x8D61:
                                                     #ifdef DEBUG
                                      mon("         TFR   DP,CC                   ; 8D61: 1F BA         \n");
                                      #endif
    CC = (UINT8)(DP >> 8); 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8D63; 
    JUMP; // sstep
  case 0x8D63:
                                                     #ifdef DEBUG
                                      mon("         PSHS  CC                      ; 8D63: 34 01         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    PC = 0x8D65; 
    JUMP; // sstep
  case 0x8D65:
                                                     #ifdef DEBUG
                                      mon("         PULS  A                       ; 8D65: 35 02         \n");
                                      #endif
    A = rd_mem(S); 
    S++;
    PC = 0x8D67; 
    JUMP; // sstep
  case 0x8D67:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$03                    ; 8D67: 81 03         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x03; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8D69; 
    JUMP; // sstep
  case 0x8D69:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8D69: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8D6A:
                                                     #ifdef DEBUG
                                      mon("L8D6A    BSR   $8D01                   ; 8D6A: 8D 95         \n");
                                      #endif
    --S;
    wr_mem(S, 0x6c); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8D6C:
                                                     #ifdef DEBUG
                                      mon("L8D6C    LDA   #$01                    ; 8D6C: 86 01         \n");
                                      #endif
    val = (UINT8)0x01; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8D6E; 
    JUMP; // sstep
  case 0x8D6E:
                                                     #ifdef DEBUG
                                      mon("         PSHS  A                       ; 8D6E: 34 02         \n");
                                      #endif
    --S;
    wr_mem(S, A); 
    PC = 0x8D70; 
    JUMP; // sstep
  case 0x8D70:
                                                     #ifdef DEBUG
                                      mon("         PULS  CC                      ; 8D70: 35 01         \n");
                                      #endif
    CC = rd_mem(S); 
    S++;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8D72; 
    JUMP; // sstep
  case 0x8D72:
                                                     #ifdef DEBUG
                                      mon("         TFR   CC,A                    ; 8D72: 1F A8         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    A = (UINT8)CC; 
    PC = 0x8D74; 
    JUMP; // sstep
  case 0x8D74:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$01                    ; 8D74: 81 01         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x01; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8D76; 
    JUMP; // sstep
  case 0x8D76:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8D76: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $8D                     ; 8D77:  ' ' 8D 12          BSR   $8D8B ; 
                   // FCB   $12                     ; 8D78:  ' ' 12             NOP   ; 
                   // FCB   $8D                     ; 8D79:  ' ' 8D 1D          BSR   $8D98 ; 
                   // FCB   $1D                     ; 8D7A:  ' ' 1D             SEX   ; 
                   // FCB   $8D                     ; 8D7B:  ' ' 8D 29          BSR   $8DA6 ; 
                   // FCB   $29                     ; 8D7C:  ')' 29 8D          BVS   $8D0B ; 
                   // FCB   $8D                     ; 8D7D:  ' ' 8D 36          BSR   $8DB5 ; 
                   // FCB   $36                     ; 8D7E:  '6' 36 8D          PSHU  PC,DP,B,CC ; 
                   // FCB   $8D                     ; 8D7F:  ' ' 8D 42          BSR   $8DC3 ; 
                   // FCB   $42                     ; 8D80:  'B' 42             Invalid ; 
                   // FCB   $8D                     ; 8D81:  ' ' 8D 4B          BSR   $8DCE ; 
                   // FCB   $4B                     ; 8D82:  'K' 4B             Invalid ; 
                   // FCB   $8D                     ; 8D83:  ' ' 8D 58          BSR   $8DDD ; 
                   // FCB   $58                     ; 8D84:  'X' 58             ASLB  ; 
                   // FCB   $8D                     ; 8D85:  ' ' 8D 6A          BSR   $8DF1 ; 
                   // FCB   $6A                     ; 8D86:  'j' 6A 54          DEC   -12,U ; 
                   // FCB   $54                     ; 8D87:  'T' 54             LSRB  ; 
                   // FCB   $46                     ; 8D88:  'F' 46             RORA  ; 
                   // FCB   $52                     ; 8D89:  'R' 52             Invalid ; 
                   // FCB   $04                     ; 8D8A:  ' ' 04 8E          LSR   <$8E ; 
  case 0x8D8B:
                                                     #ifdef DEBUG
                                      mon("L8D8B    LDX   #$8E67                  ; 8D8B: 8E 8E 67      \n");
                                      #endif
    X = 0x8e67; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8D8E; 
    JUMP; // sstep
  case 0x8D8E:
                                                     #ifdef DEBUG
                                      mon("L8D8E    PSHS  X                       ; 8D8E: 34 10         \n");
                                      #endif
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    PC = 0x8D90; 
    JUMP; // sstep
  case 0x8D90:
                                                     #ifdef DEBUG
                                      mon("         LDY   ,X                      ; 8D90: 10 AE 84      \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    Y = ea; 
    Z = Y; 
    N = (Y) >> 8; 
    V = 0; 
    PC = 0x8D93; 
    JUMP; // sstep
  case 0x8D93:
                                                     #ifdef DEBUG
                                      mon("         JSR   ,Y                      ; 8D93: AD A4         \n");
                                      #endif
    --S;
    wr_mem(S, 0x95); 
    --S;
    wr_mem(S, 0x8d); 
    PC = Y; 
    JUMP;
  case 0x8D95:
                                                     #ifdef DEBUG
                                      mon("L8D95    PULS  X                       ; 8D95: 35 10         \n");
                                      #endif
    X = rd_mem(S)<<8; 
    S++;
    X = X | rd_mem(S); 
    S++;
    PC = 0x8D97; 
    JUMP; // sstep
  case 0x8D97:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8DA1                   ; 8D97: 26 08         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8da1; 
    JUMP;
    }
    PC = 0x8D99; 
    JUMP; // sstep
  case 0x8D99:
                                                     #ifdef DEBUG
                                      mon("         LEAX  $02,X                   ; 8D99: 30 02         \n");
                                      #endif
    X = (X + 0x0002); 
/**/Z = X; 
    PC = 0x8D9B; 
    JUMP; // sstep
  case 0x8D9B:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$8E77                  ; 8D9B: 8C 8E 77      \n");
                                      #endif
    val = X; 
    arg = 0x8e77; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8D9E; 
    JUMP; // sstep
  case 0x8D9E:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8D8E                   ; 8D9E: 26 EE         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8d8e; 
    JUMP;
    }
    PC = 0x8DA0; 
    JUMP; // sstep
  case 0x8DA0:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8DA0: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8DA1:
                                                     #ifdef DEBUG
                                      mon("L8DA1    LDU   #$8E77                  ; 8DA1: CE 8E 77      \n");
                                      #endif
    U = 0x8e77; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x8DA4; 
    JUMP; // sstep
  case 0x8DA4:
                                                     #ifdef DEBUG
                                      mon("         LBRA  $9375                   ; 8DA4: 16 05 CE      \n");
                                      #endif
    PC = 0x9375; 
    JUMP;
    
  case 0x8DA7:
                                                     #ifdef DEBUG
                                      mon("L8DA7    LBSR  $8D01                   ; 8DA7: 17 FF 57      \n");
                                      #endif
    --S;
    wr_mem(S, 0xaa); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8DAA:
                                                     #ifdef DEBUG
                                      mon("L8DAA    LDD   #$5678                  ; 8DAA: CC 56 78      \n");
                                      #endif
    A = 0x56; 
    B = 0x78; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8DAD; 
    JUMP; // sstep
  case 0x8DAD:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$A55A                  ; 8DAD: 8E A5 5A      \n");
                                      #endif
    X = 0xa55a; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8DB0; 
    JUMP; // sstep
  case 0x8DB0:
                                                     #ifdef DEBUG
                                      mon("         EXG   D,X                     ; 8DB0: 1E 01         \n");
                                      #endif
    val = ((A << 8) | B); 
    A = (UINT8)((X) >> 8); 
    B = (UINT8)X; 
    X = val; 
    PC = 0x8DB2; 
    JUMP; // sstep
  case 0x8DB2:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$5678                  ; 8DB2: 8C 56 78      \n");
                                      #endif
    val = X; 
    arg = 0x5678; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8DB5; 
    JUMP; // sstep
  case 0x8DB5:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8DBB                   ; 8DB5: 26 04         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8dbb; 
    JUMP;
    }
    PC = 0x8DB7; 
    JUMP; // sstep
  case 0x8DB7:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$A55A                  ; 8DB7: 10 83 A5 5A   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0xa55a; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8DBB; 
    JUMP; // sstep
  case 0x8DBB:
                                                     #ifdef DEBUG
                                      mon("L8DBB    RTS                           ; 8DBB: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8DBC:
                                                     #ifdef DEBUG
                                      mon("L8DBC    LBSR  $8D01                   ; 8DBC: 17 FF 42      \n");
                                      #endif
    --S;
    wr_mem(S, 0xbf); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8DBF:
                                                     #ifdef DEBUG
                                      mon("L8DBF    LDX   #$1234                  ; 8DBF: 8E 12 34      \n");
                                      #endif
    X = 0x1234; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8DC2; 
    JUMP; // sstep
  case 0x8DC2:
                                                     #ifdef DEBUG
                                      mon("         LDY   #$0815                  ; 8DC2: 10 8E 08 15   \n");
                                      #endif
    Y = 0x0815; 
/**/Z = Y; 
/**/N = (Y) >> 8; 
/**/V = 0; 
    PC = 0x8DC6; 
    JUMP; // sstep
  case 0x8DC6:
                                                     #ifdef DEBUG
                                      mon("         EXG   X,Y                     ; 8DC6: 1E 12         \n");
                                      #endif
    val = X; 
    X = Y; 
    Y = val; 
    PC = 0x8DC8; 
    JUMP; // sstep
  case 0x8DC8:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$1234                  ; 8DC8: 10 8C 12 34   \n");
                                      #endif
    val = Y; 
    arg = 0x1234; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8DCC; 
    JUMP; // sstep
  case 0x8DCC:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8DBB                   ; 8DCC: 26 ED         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8dbb; 
    JUMP;
    }
    PC = 0x8DCE; 
    JUMP; // sstep
  case 0x8DCE:
                                                     #ifdef DEBUG
                                      mon("         CMPX  #$0815                  ; 8DCE: 8C 08 15      \n");
                                      #endif
    val = X; 
    arg = 0x0815; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8DD1; 
    JUMP; // sstep
  case 0x8DD1:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8DD1: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8DD2:
                                                     #ifdef DEBUG
                                      mon("L8DD2    LBSR  $8D01                   ; 8DD2: 17 FF 2C      \n");
                                      #endif
    --S;
    wr_mem(S, 0xd5); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8DD5:
                                                     #ifdef DEBUG
                                      mon("L8DD5    LDY   #$5500                  ; 8DD5: 10 8E 55 00   \n");
                                      #endif
    Y = 0x5500; 
/**/Z = Y; 
/**/N = (Y) >> 8; 
/**/V = 0; 
    PC = 0x8DD9; 
    JUMP; // sstep
  case 0x8DD9:
                                                     #ifdef DEBUG
                                      mon("         LDU   #$4711                  ; 8DD9: CE 47 11      \n");
                                      #endif
    U = 0x4711; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8DDC; 
    JUMP; // sstep
  case 0x8DDC:
                                                     #ifdef DEBUG
                                      mon("         EXG   Y,U                     ; 8DDC: 1E 23         \n");
                                      #endif
    val = Y; 
    Y = U; 
    U = val; 
    PC = 0x8DDE; 
    JUMP; // sstep
  case 0x8DDE:
                                                     #ifdef DEBUG
                                      mon("         CMPU  #$5500                  ; 8DDE: 11 83 55 00   \n");
                                      #endif
    val = U; 
    arg = 0x5500; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8DE2; 
    JUMP; // sstep
  case 0x8DE2:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8DBB                   ; 8DE2: 26 D7         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8dbb; 
    JUMP;
    }
    PC = 0x8DE4; 
    JUMP; // sstep
  case 0x8DE4:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$4711                  ; 8DE4: 10 8C 47 11   \n");
                                      #endif
    val = Y; 
    arg = 0x4711; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8DE8; 
    JUMP; // sstep
  case 0x8DE8:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8DE8: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8DE9:
                                                     #ifdef DEBUG
                                      mon("L8DE9    LBSR  $8D01                   ; 8DE9: 17 FF 15      \n");
                                      #endif
    --S;
    wr_mem(S, 0xec); 
    --S;
    wr_mem(S, 0x8d); 
    PC = 0x8d01; 
    JUMP;
  case 0x8DEC:
                                                     #ifdef DEBUG
                                      mon("L8DEC    LDU   #$FEFF                  ; 8DEC: CE FE FF      \n");
                                      #endif
    U = 0xfeff; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8DEF; 
    JUMP; // sstep
  case 0x8DEF:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$9999                  ; 8DEF: CC 99 99      \n");
                                      #endif
    A = 0x99; 
    B = 0x99; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8DF2; 
    JUMP; // sstep
  case 0x8DF2:
                                                     #ifdef DEBUG
                                      mon("         EXG   U,D                     ; 8DF2: 1E 30         \n");
                                      #endif
    val = U; 
    U = ((A << 8) | B); 
    A = (UINT8)((val) >> 8); 
    B = (UINT8)val; 
    PC = 0x8DF4; 
    JUMP; // sstep
  case 0x8DF4:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$FEFF                  ; 8DF4: 10 83 FE FF   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0xfeff; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8DF8; 
    JUMP; // sstep
  case 0x8DF8:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8DBB                   ; 8DF8: 26 C1         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8dbb; 
    JUMP;
    }
    PC = 0x8DFA; 
    JUMP; // sstep
  case 0x8DFA:
                                                     #ifdef DEBUG
                                      mon("         CMPU  #$9999                  ; 8DFA: 11 83 99 99   \n");
                                      #endif
    val = U; 
    arg = 0x9999; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8DFE; 
    JUMP; // sstep
  case 0x8DFE:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8DFE: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8DFF:
                                                     #ifdef DEBUG
                                      mon("L8DFF    LBSR  $8D01                   ; 8DFF: 17 FE FF      \n");
                                      #endif
    --S;
    wr_mem(S, 0x02); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8d01; 
    JUMP;
  case 0x8E02:
                                                     #ifdef DEBUG
                                      mon("L8E02    LDA   #$55                    ; 8E02: 86 55         \n");
                                      #endif
    val = (UINT8)0x55; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E04; 
    JUMP; // sstep
  case 0x8E04:
                                                     #ifdef DEBUG
                                      mon("         LDB   #$AA                    ; 8E04: C6 AA         \n");
                                      #endif
    val = (UINT8)0xaa; 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E06; 
    JUMP; // sstep
  case 0x8E06:
                                                     #ifdef DEBUG
                                      mon("         EXG   A,B                     ; 8E06: 1E 89         \n");
                                      #endif
    val = A; 
    A = B; 
    B = val; 
    PC = 0x8E08; 
    JUMP; // sstep
  case 0x8E08:
                                                     #ifdef DEBUG
                                      mon("         CMPB  #$55                    ; 8E08: C1 55         \n");
                                      #endif
    val = (UINT8)B; 
    arg = 0x55; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8E0A; 
    JUMP; // sstep
  case 0x8E0A:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8DBB                   ; 8E0A: 26 AF         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8dbb; 
    JUMP;
    }
    PC = 0x8E0C; 
    JUMP; // sstep
  case 0x8E0C:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$AA                    ; 8E0C: 81 AA         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0xaa; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8E0E; 
    JUMP; // sstep
  case 0x8E0E:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8E0E: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8E0F:
                                                     #ifdef DEBUG
                                      mon("L8E0F    LBSR  $8D01                   ; 8E0F: 17 FE EF      \n");
                                      #endif
    --S;
    wr_mem(S, 0x12); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8d01; 
    JUMP;
  case 0x8E12:
                                                     #ifdef DEBUG
                                      mon("L8E12    LDB   #$AA                    ; 8E12: C6 AA         \n");
                                      #endif
    val = (UINT8)0xaa; 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E14; 
    JUMP; // sstep
  case 0x8E14:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$44                    ; 8E14: 86 44         \n");
                                      #endif
    val = (UINT8)0x44; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E16; 
    JUMP; // sstep
  case 0x8E16:
                                                     #ifdef DEBUG
                                      mon("         PSHS  A                       ; 8E16: 34 02         \n");
                                      #endif
    --S;
    wr_mem(S, A); 
    PC = 0x8E18; 
    JUMP; // sstep
  case 0x8E18:
                                                     #ifdef DEBUG
                                      mon("         CLRA                          ; 8E18: 4F            \n");
                                      #endif
    A = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    PC = 0x8E19; 
    JUMP; // sstep
  case 0x8E19:
                                                     #ifdef DEBUG
                                      mon("         PULS  DP                      ; 8E19: 35 08         \n");
                                      #endif
    DP = rd_mem(S); 
    S++;
    DP = DP << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8E1B; 
    JUMP; // sstep
  case 0x8E1B:
                                                     #ifdef DEBUG
                                      mon("         EXG   B,DP                    ; 8E1B: 1E 9B         \n");
                                      #endif
    val = B; 
    B = (UINT8)(DP >> 8); 
    DP = (val) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8E1D; 
    JUMP; // sstep
  case 0x8E1D:
                                                     #ifdef DEBUG
                                      mon("         PSHS  DP                      ; 8E1D: 34 08         \n");
                                      #endif
    --S;
    wr_mem(S, (DP >> 8)); 
    PC = 0x8E1F; 
    JUMP; // sstep
  case 0x8E1F:
                                                     #ifdef DEBUG
                                      mon("         PULS  A                       ; 8E1F: 35 02         \n");
                                      #endif
    A = rd_mem(S); 
    S++;
    PC = 0x8E21; 
    JUMP; // sstep
  case 0x8E21:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$AA                    ; 8E21: 81 AA         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0xaa; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8E23; 
    JUMP; // sstep
  case 0x8E23:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8DBB                   ; 8E23: 26 96         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8dbb; 
    JUMP;
    }
    PC = 0x8E25; 
    JUMP; // sstep
  case 0x8E25:
                                                     #ifdef DEBUG
                                      mon("         CMPB  #$44                    ; 8E25: C1 44         \n");
                                      #endif
    val = (UINT8)B; 
    arg = 0x44; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8E27; 
    JUMP; // sstep
  case 0x8E27:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8E27: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8E28:
                                                     #ifdef DEBUG
                                      mon("L8E28    LBSR  $8D01                   ; 8E28: 17 FE D6      \n");
                                      #endif
    --S;
    wr_mem(S, 0x2b); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8d01; 
    JUMP;
  case 0x8E2B:
                                                     #ifdef DEBUG
                                      mon("L8E2B    LDA   #$03                    ; 8E2B: 86 03         \n");
                                      #endif
    val = (UINT8)0x03; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E2D; 
    JUMP; // sstep
  case 0x8E2D:
                                                     #ifdef DEBUG
                                      mon("         PSHS  A                       ; 8E2D: 34 02         \n");
                                      #endif
    --S;
    wr_mem(S, A); 
    PC = 0x8E2F; 
    JUMP; // sstep
  case 0x8E2F:
                                                     #ifdef DEBUG
                                      mon("         CLRA                          ; 8E2F: 4F            \n");
                                      #endif
    A = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    PC = 0x8E30; 
    JUMP; // sstep
  case 0x8E30:
                                                     #ifdef DEBUG
                                      mon("         PULS  DP                      ; 8E30: 35 08         \n");
                                      #endif
    DP = rd_mem(S); 
    S++;
    DP = DP << 8; checkDP(); 
    // memory_DP = &memory[DP];
    PC = 0x8E32; 
    JUMP; // sstep
  case 0x8E32:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$30                    ; 8E32: 86 30         \n");
                                      #endif
    val = (UINT8)0x30; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E34; 
    JUMP; // sstep
  case 0x8E34:
                                                     #ifdef DEBUG
                                      mon("         PSHS  A                       ; 8E34: 34 02         \n");
                                      #endif
    --S;
    wr_mem(S, A); 
    PC = 0x8E36; 
    JUMP; // sstep
  case 0x8E36:
                                                     #ifdef DEBUG
                                      mon("         CLRA                          ; 8E36: 4F            \n");
                                      #endif
    A = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    PC = 0x8E37; 
    JUMP; // sstep
  case 0x8E37:
                                                     #ifdef DEBUG
                                      mon("         PULS  CC                      ; 8E37: 35 01         \n");
                                      #endif
    CC = rd_mem(S); 
    S++;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8E39; 
    JUMP; // sstep
  case 0x8E39:
                                                     #ifdef DEBUG
                                      mon("         EXG   DP,CC                   ; 8E39: 1E BA         \n");
                                      #endif
    val = (UINT8)(DP >> 8); 
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    DP = (CC) << 8; checkDP(); 
    // memory_DP = &memory[DP];
    CC = val; 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8E3B; 
    JUMP; // sstep
  case 0x8E3B:
                                                     #ifdef DEBUG
                                      mon("         PSHS  CC                      ; 8E3B: 34 01         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    PC = 0x8E3D; 
    JUMP; // sstep
  case 0x8E3D:
                                                     #ifdef DEBUG
                                      mon("         PULS  A                       ; 8E3D: 35 02         \n");
                                      #endif
    A = rd_mem(S); 
    S++;
    PC = 0x8E3F; 
    JUMP; // sstep
  case 0x8E3F:
                                                     #ifdef DEBUG
                                      mon("         PSHS  DP                      ; 8E3F: 34 08         \n");
                                      #endif
    --S;
    wr_mem(S, (DP >> 8)); 
    PC = 0x8E41; 
    JUMP; // sstep
  case 0x8E41:
                                                     #ifdef DEBUG
                                      mon("         PULS  B                       ; 8E41: 35 04         \n");
                                      #endif
    B = rd_mem(S); 
    S++;
    PC = 0x8E43; 
    JUMP; // sstep
  case 0x8E43:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$03                    ; 8E43: 81 03         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x03; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8E45; 
    JUMP; // sstep
  case 0x8E45:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $8DBB                   ; 8E45: 10 26 FF 72   \n");
                                      #endif
    if (Z) {
    PC = 0x8dbb; 
    JUMP;
    }
    PC = 0x8E49; 
    JUMP; // sstep
  case 0x8E49:
                                                     #ifdef DEBUG
                                      mon("         CMPB  #$30                    ; 8E49: C1 30         \n");
                                      #endif
    val = (UINT8)B; 
    arg = 0x30; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8E4B; 
    JUMP; // sstep
  case 0x8E4B:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8E4B: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8E4C:
                                                     #ifdef DEBUG
                                      mon("L8E4C    LBSR  $8D01                   ; 8E4C: 17 FE B2      \n");
                                      #endif
    --S;
    wr_mem(S, 0x4f); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8d01; 
    JUMP;
  case 0x8E4F:
                                                     #ifdef DEBUG
                                      mon("L8E4F    LDA   #$11                    ; 8E4F: 86 11         \n");
                                      #endif
    val = (UINT8)0x11; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E51; 
    JUMP; // sstep
  case 0x8E51:
                                                     #ifdef DEBUG
                                      mon("         LDB   #$99                    ; 8E51: C6 99         \n");
                                      #endif
    val = (UINT8)0x99; 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E53; 
    JUMP; // sstep
  case 0x8E53:
                                                     #ifdef DEBUG
                                      mon("         PSHS  B                       ; 8E53: 34 04         \n");
                                      #endif
    --S;
    wr_mem(S, B); 
    PC = 0x8E55; 
    JUMP; // sstep
  case 0x8E55:
                                                     #ifdef DEBUG
                                      mon("         CLRB                          ; 8E55: 5F            \n");
                                      #endif
    B = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    PC = 0x8E56; 
    JUMP; // sstep
  case 0x8E56:
                                                     #ifdef DEBUG
                                      mon("         PULS  CC                      ; 8E56: 35 01         \n");
                                      #endif
    CC = rd_mem(S); 
    S++;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    PC = 0x8E58; 
    JUMP; // sstep
  case 0x8E58:
                                                     #ifdef DEBUG
                                      mon("         EXG   CC,A                    ; 8E58: 1E A8         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    val = CC; 
    CC = A; 
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; 
    restore_flags();
    A = val; 
    PC = 0x8E5A; 
    JUMP; // sstep
  case 0x8E5A:
                                                     #ifdef DEBUG
                                      mon("         PSHS  CC                      ; 8E5A: 34 01         \n");
                                      #endif
    simplify_flags();
    CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now
    restore_flags();
    --S;
    wr_mem(S, CC); 
    PC = 0x8E5C; 
    JUMP; // sstep
  case 0x8E5C:
                                                     #ifdef DEBUG
                                      mon("         PULS  B                       ; 8E5C: 35 04         \n");
                                      #endif
    B = rd_mem(S); 
    S++;
    PC = 0x8E5E; 
    JUMP; // sstep
  case 0x8E5E:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$99                    ; 8E5E: 81 99         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x99; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8E60; 
    JUMP; // sstep
  case 0x8E60:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $8DBB                   ; 8E60: 10 26 FF 57   \n");
                                      #endif
    if (Z) {
    PC = 0x8dbb; 
    JUMP;
    }
    PC = 0x8E64; 
    JUMP; // sstep
  case 0x8E64:
                                                     #ifdef DEBUG
                                      mon("         CMPB  #$11                    ; 8E64: C1 11         \n");
                                      #endif
    val = (UINT8)B; 
    arg = 0x11; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8E66; 
    JUMP; // sstep
  case 0x8E66:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8E66: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $8D                     ; 8E67:  ' ' 8D A7          BSR   $8E10 ; 
                   // FCB   $A7                     ; 8E68:  ' ' A7 8D BC 8D    STA   $BC8D,PC ; 
                   // FCB   $8D                     ; 8E69:  ' ' 8D BC          BSR   $8E27 ; 
                   // FCB   $BC                     ; 8E6A:  ' ' BC 8D D2       CMPX  $8DD2 ; 
                   // FCB   $8D                     ; 8E6B:  ' ' 8D D2          BSR   $8E3F ; 
                   // FCB   $D2                     ; 8E6C:  ' ' D2 8D          SBCB  <$8D ; 
                   // FCB   $8D                     ; 8E6D:  ' ' 8D E9          BSR   $8E58 ; 
                   // FCB   $E9                     ; 8E6E:  ' ' E9 8D FF 8E    ADCB  $FF8E,PC ; 
                   // FCB   $8D                     ; 8E6F:  ' ' 8D FF          BSR   $8E70 ; 
                   // FCB   $FF                     ; 8E70:  ' ' FF 8E 0F       STU   $8E0F ; 
                   // FCB   $8E                     ; 8E71:  ' ' 8E 0F 8E       LDX   #$0F8E ; 
                   // FCB   $0F                     ; 8E72:  ' ' 0F 8E          CLR   <$8E ; 
                   // FCB   $8E                     ; 8E73:  ' ' 8E 28 8E       LDX   #$288E ; 
                   // FCB   $28                     ; 8E74:  '(' 28 8E          BVC   $8E04 ; 
                   // FCB   $8E                     ; 8E75:  ' ' 8E 4C 45       LDX   #$4C45 ; 
                   // FCB   $4C                     ; 8E76:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 8E77:  'E' 45             Invalid ; 
                   // FCB   $58                     ; 8E78:  'X' 58             ASLB  ; 
                   // FCB   $47                     ; 8E79:  'G' 47             ASRA  ; 
                   // FCB   $04                     ; 8E7A:  ' ' 04 34          LSR   <$34 ; 
  case 0x8E7B:
                                                     #ifdef DEBUG
                                      mon("L8E7B    PSHS  X                       ; 8E7B: 34 10         \n");
                                      #endif
    --S;
    wr_mem(S, X); 
    --S;
    wr_mem(S, X >> 8); 
    PC = 0x8E7D; 
    JUMP; // sstep
  case 0x8E7D:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$7F80                  ; 8E7D: 8E 7F 80      \n");
                                      #endif
    X = 0x7f80; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8E80; 
    JUMP; // sstep
  case 0x8E80:
                                                     #ifdef DEBUG
                                      mon("         CLRA                          ; 8E80: 4F            \n");
                                      #endif
    A = 0; // SEARCH_ME
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
/**/C = 0; 
    PC = 0x8E81; 
    JUMP; // sstep
  case 0x8E81:
                                                     #ifdef DEBUG
                                      mon("L8E81    CLR   ,X+                     ; 8E81: 6F 80         \n");
                                      #endif
    wr_mem(X, 0); 
/**/V = 0; 
/**/Z = 0; 
/**/N = 0; 
    C = 0; 
    X = X + 1; 
    PC = 0x8E83; 
    JUMP; // sstep
  case 0x8E83:
                                                     #ifdef DEBUG
                                      mon("         INCA                          ; 8E83: 4C            \n");
                                      #endif
    val = (UINT8)A; 
    res = (UINT8)(val + 1); 
    Z = res; 
    N = res; 
    V = res & ~val; 
    A = res; 
    PC = 0x8E84; 
    JUMP; // sstep
  case 0x8E84:
                                                     #ifdef DEBUG
                                      mon("         BNE   $8E81                   ; 8E84: 26 FB         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x8e81; 
    JUMP;
    }
    PC = 0x8E86; 
    JUMP; // sstep
  case 0x8E86:
                                                     #ifdef DEBUG
                                      mon("         PULS  X,PC                    ; 8E86: 35 90         \n");
                                      #endif    // (PUL? PC=RTS)
    X = rd_mem(S)<<8; 
    S++;
    X = X | rd_mem(S); 
    S++;
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8E88:
                                                     #ifdef DEBUG
                                      mon("L8E88    LDU   #$9298                  ; 8E88: CE 92 98      \n");
                                      #endif
    U = 0x9298; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8E8B; 
    JUMP; // sstep
  case 0x8E8B:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8000                  ; 8E8B: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x8E8E; 
    JUMP; // sstep
  case 0x8E8E:
                                                     #ifdef DEBUG
                                      mon("         BSR   $8E7B                   ; 8E8E: 8D EB         \n");
                                      #endif
    --S;
    wr_mem(S, 0x90); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8E90:
                                                     #ifdef DEBUG
                                      mon("L8E90    LDA   #$55                    ; 8E90: 86 55         \n");
                                      #endif
    val = (UINT8)0x55; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E92; 
    JUMP; // sstep
  case 0x8E92:
                                                     #ifdef DEBUG
                                      mon("         STA   $8000                   ; 8E92: B7 80 00      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x8000, val); 
    PC = 0x8E95; 
    JUMP; // sstep
  case 0x8E95:
                                                     #ifdef DEBUG
                                      mon("         LDA   ,X                      ; 8E95: A6 84         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8E97; 
    JUMP; // sstep
  case 0x8E97:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$55                    ; 8E97: 81 55         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x55; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8E99; 
    JUMP; // sstep
  case 0x8E99:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8E99: 10 26 04 D8   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8E9D; 
    JUMP; // sstep
  case 0x8E9D:
                                                     #ifdef DEBUG
                                      mon("         BSR   $8E7B                   ; 8E9D: 8D DC         \n");
                                      #endif
    --S;
    wr_mem(S, 0x9f); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8E9F:
                                                     #ifdef DEBUG
                                      mon("L8E9F    LDA   #$12                    ; 8E9F: 86 12         \n");
                                      #endif
    val = (UINT8)0x12; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8EA1; 
    JUMP; // sstep
  case 0x8EA1:
                                                     #ifdef DEBUG
                                      mon("         STA   $8001                   ; 8EA1: B7 80 01      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x8001, val); 
    PC = 0x8EA4; 
    JUMP; // sstep
  case 0x8EA4:
                                                     #ifdef DEBUG
                                      mon("         LDA   $01,X                   ; 8EA4: A6 01         \n");
                                      #endif
    val = (UINT8)rd_mem((UINT16)(X + 0x0001)); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8EA6; 
    JUMP; // sstep
  case 0x8EA6:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$12                    ; 8EA6: 81 12         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x12; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8EA8; 
    JUMP; // sstep
  case 0x8EA8:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8EA8: 10 26 04 C9   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8EAC; 
    JUMP; // sstep
  case 0x8EAC:
                                                     #ifdef DEBUG
                                      mon("         BSR   $8E7B                   ; 8EAC: 8D CD         \n");
                                      #endif
    --S;
    wr_mem(S, 0xae); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8EAE:
                                                     #ifdef DEBUG
                                      mon("L8EAE    LDA   #$44                    ; 8EAE: 86 44         \n");
                                      #endif
    val = (UINT8)0x44; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8EB0; 
    JUMP; // sstep
  case 0x8EB0:
                                                     #ifdef DEBUG
                                      mon("         STA   $807F                   ; 8EB0: B7 80 7F      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x807f, val); 
    PC = 0x8EB3; 
    JUMP; // sstep
  case 0x8EB3:
                                                     #ifdef DEBUG
                                      mon("         LDA   $7F,X                   ; 8EB3: A6 88 7F      \n");
                                      #endif
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)0x7f)); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8EB6; 
    JUMP; // sstep
  case 0x8EB6:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$44                    ; 8EB6: 81 44         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x44; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8EB8; 
    JUMP; // sstep
  case 0x8EB8:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8EB8: 10 26 04 B9   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8EBC; 
    JUMP; // sstep
  case 0x8EBC:
                                                     #ifdef DEBUG
                                      mon("         BSR   $8E7B                   ; 8EBC: 8D BD         \n");
                                      #endif
    --S;
    wr_mem(S, 0xbe); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8EBE:
                                                     #ifdef DEBUG
                                      mon("L8EBE    LDA   #$34                    ; 8EBE: 86 34         \n");
                                      #endif
    val = (UINT8)0x34; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8EC0; 
    JUMP; // sstep
  case 0x8EC0:
                                                     #ifdef DEBUG
                                      mon("         STA   $7FFF                   ; 8EC0: B7 7F FF      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x7fff, val); 
    PC = 0x8EC3; 
    JUMP; // sstep
  case 0x8EC3:
                                                     #ifdef DEBUG
                                      mon("         LDA   -1,X                    ; 8EC3: A6 1F         \n");
                                      #endif
    val = (UINT8)rd_mem((UINT16)(X + 0xffff)); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8EC5; 
    JUMP; // sstep
  case 0x8EC5:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$34                    ; 8EC5: 81 34         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x34; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8EC7; 
    JUMP; // sstep
  case 0x8EC7:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8EC7: 10 26 04 AA   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8ECB; 
    JUMP; // sstep
  case 0x8ECB:
                                                     #ifdef DEBUG
                                      mon("         BSR   $8E7B                   ; 8ECB: 8D AE         \n");
                                      #endif
    --S;
    wr_mem(S, 0xcd); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8ECD:
                                                     #ifdef DEBUG
                                      mon("L8ECD    LDA   #$AA                    ; 8ECD: 86 AA         \n");
                                      #endif
    val = (UINT8)0xaa; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8ECF; 
    JUMP; // sstep
  case 0x8ECF:
                                                     #ifdef DEBUG
                                      mon("         STA   $7F80                   ; 8ECF: B7 7F 80      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x7f80, val); 
    PC = 0x8ED2; 
    JUMP; // sstep
  case 0x8ED2:
                                                     #ifdef DEBUG
                                      mon("         LDA   -$80,X                  ; 8ED2: A6 88 80      \n");
                                      #endif
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)0x80)); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8ED5; 
    JUMP; // sstep
  case 0x8ED5:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$AA                    ; 8ED5: 81 AA         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0xaa; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8ED7; 
    JUMP; // sstep
  case 0x8ED7:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8ED7: 10 26 04 9A   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8EDB; 
    JUMP; // sstep
  case 0x8EDB:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8EDB: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8EDC:
                                                     #ifdef DEBUG
                                      mon("L8EDC    LDX   #$8000                  ; 8EDC: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8EDF; 
    JUMP; // sstep
  case 0x8EDF:
                                                     #ifdef DEBUG
                                      mon("         LDU   #$92A1                  ; 8EDF: CE 92 A1      \n");
                                      #endif
    U = 0x92a1; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x8EE2; 
    JUMP; // sstep
  case 0x8EE2:
                                                     #ifdef DEBUG
                                      mon("         BSR   $8E7B                   ; 8EE2: 8D 97         \n");
                                      #endif
    --S;
    wr_mem(S, 0xe4); 
    --S;
    wr_mem(S, 0x8e); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8EE4:
                                                     #ifdef DEBUG
                                      mon("L8EE4    LDD   #$A55A                  ; 8EE4: CC A5 5A      \n");
                                      #endif
    A = 0xa5; 
    B = 0x5a; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8EE7; 
    JUMP; // sstep
  case 0x8EE7:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 8EE7: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x8EEA; 
    JUMP; // sstep
  case 0x8EEA:
                                                     #ifdef DEBUG
                                      mon("         LDA   ,X+                     ; 8EEA: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8EEC; 
    JUMP; // sstep
  case 0x8EEC:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$A5                    ; 8EEC: 81 A5         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0xa5; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8EEE; 
    JUMP; // sstep
  case 0x8EEE:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8EEE: 10 26 04 83   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8EF2; 
    JUMP; // sstep
  case 0x8EF2:
                                                     #ifdef DEBUG
                                      mon("         LDA   ,X+                     ; 8EF2: A6 80         \n");
                                      #endif
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    X = X + 1; 
    PC = 0x8EF4; 
    JUMP; // sstep
  case 0x8EF4:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$5A                    ; 8EF4: 81 5A         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x5a; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8EF6; 
    JUMP; // sstep
  case 0x8EF6:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8EF6: 10 26 04 7B   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8EFA; 
    JUMP; // sstep
  case 0x8EFA:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8EFA: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8EFB:
                                                     #ifdef DEBUG
                                      mon("L8EFB    LDX   #$8000                  ; 8EFB: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8EFE; 
    JUMP; // sstep
  case 0x8EFE:
                                                     #ifdef DEBUG
                                      mon("         LDU   #$92A9                  ; 8EFE: CE 92 A9      \n");
                                      #endif
    U = 0x92a9; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x8F01; 
    JUMP; // sstep
  case 0x8F01:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 8F01: 17 FF 77      \n");
                                      #endif
    --S;
    wr_mem(S, 0x04); 
    --S;
    wr_mem(S, 0x8f); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8F04:
                                                     #ifdef DEBUG
                                      mon("L8F04    LDD   #$A55A                  ; 8F04: CC A5 5A      \n");
                                      #endif
    A = 0xa5; 
    B = 0x5a; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8F07; 
    JUMP; // sstep
  case 0x8F07:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 8F07: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x8F0A; 
    JUMP; // sstep
  case 0x8F0A:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$789A                  ; 8F0A: CC 78 9A      \n");
                                      #endif
    A = 0x78; 
    B = 0x9a; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8F0D; 
    JUMP; // sstep
  case 0x8F0D:
                                                     #ifdef DEBUG
                                      mon("         STD   $8002                   ; 8F0D: FD 80 02      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8002, A); // Care needed with I/O space and word fetches
    wr_mem(0x8003, B); 
    PC = 0x8F10; 
    JUMP; // sstep
  case 0x8F10:
                                                     #ifdef DEBUG
                                      mon("         LDD   ,X++                    ; 8F10: EC 81         \n");
                                      #endif
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
    X = X + 2; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8F12; 
    JUMP; // sstep
  case 0x8F12:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$A55A                  ; 8F12: 10 83 A5 5A   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0xa55a; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8F16; 
    JUMP; // sstep
  case 0x8F16:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8F16: 10 26 04 5B   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8F1A; 
    JUMP; // sstep
  case 0x8F1A:
                                                     #ifdef DEBUG
                                      mon("         LDD   ,X++                    ; 8F1A: EC 81         \n");
                                      #endif
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
    X = X + 2; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8F1C; 
    JUMP; // sstep
  case 0x8F1C:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$789A                  ; 8F1C: 10 83 78 9A   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x789a; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8F20; 
    JUMP; // sstep
  case 0x8F20:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8F20: 10 26 04 51   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8F24; 
    JUMP; // sstep
  case 0x8F24:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8F24: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8F25:
                                                     #ifdef DEBUG
                                      mon("L8F25    LDX   #$8000                  ; 8F25: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8F28; 
    JUMP; // sstep
  case 0x8F28:
                                                     #ifdef DEBUG
                                      mon("         LDU   #$92B2                  ; 8F28: CE 92 B2      \n");
                                      #endif
    U = 0x92b2; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x8F2B; 
    JUMP; // sstep
  case 0x8F2B:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 8F2B: 17 FF 4D      \n");
                                      #endif
    --S;
    wr_mem(S, 0x2e); 
    --S;
    wr_mem(S, 0x8f); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8F2E:
                                                     #ifdef DEBUG
                                      mon("L8F2E    LDD   #$A55A                  ; 8F2E: CC A5 5A      \n");
                                      #endif
    A = 0xa5; 
    B = 0x5a; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8F31; 
    JUMP; // sstep
  case 0x8F31:
                                                     #ifdef DEBUG
                                      mon("         STD   $7FFE                   ; 8F31: FD 7F FE      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x7ffe, A); // Care needed with I/O space and word fetches
    wr_mem(0x7fff, B); 
    PC = 0x8F34; 
    JUMP; // sstep
  case 0x8F34:
                                                     #ifdef DEBUG
                                      mon("         LDA   ,-X                     ; 8F34: A6 82         \n");
                                      #endif
    X = X - 1; 
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8F36; 
    JUMP; // sstep
  case 0x8F36:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$5A                    ; 8F36: 81 5A         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x5a; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8F38; 
    JUMP; // sstep
  case 0x8F38:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8F38: 10 26 04 39   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8F3C; 
    JUMP; // sstep
  case 0x8F3C:
                                                     #ifdef DEBUG
                                      mon("         LDA   ,-X                     ; 8F3C: A6 82         \n");
                                      #endif
    X = X - 1; 
    val = (UINT8)rd_mem(X); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8F3E; 
    JUMP; // sstep
  case 0x8F3E:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$A5                    ; 8F3E: 81 A5         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0xa5; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8F40; 
    JUMP; // sstep
  case 0x8F40:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8F40: 10 26 04 31   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8F44; 
    JUMP; // sstep
  case 0x8F44:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8F44: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8F45:
                                                     #ifdef DEBUG
                                      mon("L8F45    LDX   #$8000                  ; 8F45: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x8F48; 
    JUMP; // sstep
  case 0x8F48:
                                                     #ifdef DEBUG
                                      mon("         LDU   #$92BA                  ; 8F48: CE 92 BA      \n");
                                      #endif
    U = 0x92ba; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x8F4B; 
    JUMP; // sstep
  case 0x8F4B:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 8F4B: 17 FF 2D      \n");
                                      #endif
    --S;
    wr_mem(S, 0x4e); 
    --S;
    wr_mem(S, 0x8f); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8F4E:
                                                     #ifdef DEBUG
                                      mon("L8F4E    LDD   #$A55A                  ; 8F4E: CC A5 5A      \n");
                                      #endif
    A = 0xa5; 
    B = 0x5a; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8F51; 
    JUMP; // sstep
  case 0x8F51:
                                                     #ifdef DEBUG
                                      mon("         STD   $7FFE                   ; 8F51: FD 7F FE      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x7ffe, A); // Care needed with I/O space and word fetches
    wr_mem(0x7fff, B); 
    PC = 0x8F54; 
    JUMP; // sstep
  case 0x8F54:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$789A                  ; 8F54: CC 78 9A      \n");
                                      #endif
    A = 0x78; 
    B = 0x9a; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8F57; 
    JUMP; // sstep
  case 0x8F57:
                                                     #ifdef DEBUG
                                      mon("         STD   $7FFC                   ; 8F57: FD 7F FC      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x7ffc, A); // Care needed with I/O space and word fetches
    wr_mem(0x7ffd, B); 
    PC = 0x8F5A; 
    JUMP; // sstep
  case 0x8F5A:
                                                     #ifdef DEBUG
                                      mon("         LDD   ,--X                    ; 8F5A: EC 83         \n");
                                      #endif
    X = X - 2; 
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8F5C; 
    JUMP; // sstep
  case 0x8F5C:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$A55A                  ; 8F5C: 10 83 A5 5A   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0xa55a; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8F60; 
    JUMP; // sstep
  case 0x8F60:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8F60: 10 26 04 11   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8F64; 
    JUMP; // sstep
  case 0x8F64:
                                                     #ifdef DEBUG
                                      mon("         LDD   ,--X                    ; 8F64: EC 83         \n");
                                      #endif
    X = X - 2; 
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8F66; 
    JUMP; // sstep
  case 0x8F66:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$789A                  ; 8F66: 10 83 78 9A   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x789a; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8F6A; 
    JUMP; // sstep
  case 0x8F6A:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8F6A: 10 26 04 07   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8F6E; 
    JUMP; // sstep
  case 0x8F6E:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8F6E: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8F6F:
                                                     #ifdef DEBUG
                                      mon("L8F6F    LDU   #$92C3                  ; 8F6F: CE 92 C3      \n");
                                      #endif
    U = 0x92c3; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8F72; 
    JUMP; // sstep
  case 0x8F72:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8000                  ; 8F72: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x8F75; 
    JUMP; // sstep
  case 0x8F75:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 8F75: 17 FF 03      \n");
                                      #endif
    --S;
    wr_mem(S, 0x78); 
    --S;
    wr_mem(S, 0x8f); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8F78:
                                                     #ifdef DEBUG
                                      mon("L8F78    LDA   #$12                    ; 8F78: 86 12         \n");
                                      #endif
    val = (UINT8)0x12; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8F7A; 
    JUMP; // sstep
  case 0x8F7A:
                                                     #ifdef DEBUG
                                      mon("         STA   $8001                   ; 8F7A: B7 80 01      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x8001, val); 
    PC = 0x8F7D; 
    JUMP; // sstep
  case 0x8F7D:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$01                    ; 8F7D: 86 01         \n");
                                      #endif
    val = (UINT8)0x01; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8F7F; 
    JUMP; // sstep
  case 0x8F7F:
                                                     #ifdef DEBUG
                                      mon("         LDA   A,X                     ; 8F7F: A6 86         \n");
                                      #endif
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)A)); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8F81; 
    JUMP; // sstep
  case 0x8F81:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$12                    ; 8F81: 81 12         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x12; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8F83; 
    JUMP; // sstep
  case 0x8F83:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8F83: 10 26 03 EE   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8F87; 
    JUMP; // sstep
  case 0x8F87:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 8F87: 17 FE F1      \n");
                                      #endif
    --S;
    wr_mem(S, 0x8a); 
    --S;
    wr_mem(S, 0x8f); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8F8A:
                                                     #ifdef DEBUG
                                      mon("L8F8A    LDA   #$44                    ; 8F8A: 86 44         \n");
                                      #endif
    val = (UINT8)0x44; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8F8C; 
    JUMP; // sstep
  case 0x8F8C:
                                                     #ifdef DEBUG
                                      mon("         STA   $807F                   ; 8F8C: B7 80 7F      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x807f, val); 
    PC = 0x8F8F; 
    JUMP; // sstep
  case 0x8F8F:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$7F                    ; 8F8F: 86 7F         \n");
                                      #endif
    val = (UINT8)0x7f; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8F91; 
    JUMP; // sstep
  case 0x8F91:
                                                     #ifdef DEBUG
                                      mon("         LDA   A,X                     ; 8F91: A6 86         \n");
                                      #endif
    val = (UINT8)rd_mem((UINT16)(X + (SINT8)A)); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8F93; 
    JUMP; // sstep
  case 0x8F93:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$44                    ; 8F93: 81 44         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x44; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8F95; 
    JUMP; // sstep
  case 0x8F95:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8F95: 10 26 03 DC   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8F99; 
    JUMP; // sstep
  case 0x8F99:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8F99: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8F9A:
                                                     #ifdef DEBUG
                                      mon("L8F9A    LDU   #$92CB                  ; 8F9A: CE 92 CB      \n");
                                      #endif
    U = 0x92cb; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8F9D; 
    JUMP; // sstep
  case 0x8F9D:
                                                     #ifdef DEBUG
                                      mon("         LDY   #$8000                  ; 8F9D: 10 8E 80 00   \n");
                                      #endif
    Y = 0x8000; 
    Z = Y; 
    N = (Y) >> 8; 
    V = 0; 
    PC = 0x8FA1; 
    JUMP; // sstep
  case 0x8FA1:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 8FA1: 17 FE D7      \n");
                                      #endif
    --S;
    wr_mem(S, 0xa4); 
    --S;
    wr_mem(S, 0x8f); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8FA4:
                                                     #ifdef DEBUG
                                      mon("L8FA4    LDA   #$34                    ; 8FA4: 86 34         \n");
                                      #endif
    val = (UINT8)0x34; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8FA6; 
    JUMP; // sstep
  case 0x8FA6:
                                                     #ifdef DEBUG
                                      mon("         STA   $7FFF                   ; 8FA6: B7 7F FF      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x7fff, val); 
    PC = 0x8FA9; 
    JUMP; // sstep
  case 0x8FA9:
                                                     #ifdef DEBUG
                                      mon("         LDB   #$FF                    ; 8FA9: C6 FF         \n");
                                      #endif
    val = (UINT8)0xff; 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8FAB; 
    JUMP; // sstep
  case 0x8FAB:
                                                     #ifdef DEBUG
                                      mon("         LDA   B,Y                     ; 8FAB: A6 A5         \n");
                                      #endif
    val = (UINT8)rd_mem((UINT16)(Y + (SINT8)B)); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8FAD; 
    JUMP; // sstep
  case 0x8FAD:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$34                    ; 8FAD: 81 34         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x34; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8FAF; 
    JUMP; // sstep
  case 0x8FAF:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8FAF: 10 26 03 C2   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8FB3; 
    JUMP; // sstep
  case 0x8FB3:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 8FB3: 17 FE C5      \n");
                                      #endif
    --S;
    wr_mem(S, 0xb6); 
    --S;
    wr_mem(S, 0x8f); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8FB6:
                                                     #ifdef DEBUG
                                      mon("L8FB6    LDA   #$AA                    ; 8FB6: 86 AA         \n");
                                      #endif
    val = (UINT8)0xaa; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8FB8; 
    JUMP; // sstep
  case 0x8FB8:
                                                     #ifdef DEBUG
                                      mon("         STA   $7F80                   ; 8FB8: B7 7F 80      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x7f80, val); 
    PC = 0x8FBB; 
    JUMP; // sstep
  case 0x8FBB:
                                                     #ifdef DEBUG
                                      mon("         LDB   #$80                    ; 8FBB: C6 80         \n");
                                      #endif
    val = (UINT8)0x80; 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8FBD; 
    JUMP; // sstep
  case 0x8FBD:
                                                     #ifdef DEBUG
                                      mon("         LDA   B,Y                     ; 8FBD: A6 A5         \n");
                                      #endif
    val = (UINT8)rd_mem((UINT16)(Y + (SINT8)B)); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x8FBF; 
    JUMP; // sstep
  case 0x8FBF:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$AA                    ; 8FBF: 81 AA         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0xaa; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x8FC1; 
    JUMP; // sstep
  case 0x8FC1:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8FC1: 10 26 03 B0   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8FC5; 
    JUMP; // sstep
  case 0x8FC5:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8FC5: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8FC6:
                                                     #ifdef DEBUG
                                      mon("L8FC6    LDU   #$92D3                  ; 8FC6: CE 92 D3      \n");
                                      #endif
    U = 0x92d3; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8FC9; 
    JUMP; // sstep
  case 0x8FC9:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$00                    ; 8FC9: 8E 00 00      \n");
                                      #endif
    X = 0x0000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x8FCC; 
    JUMP; // sstep
  case 0x8FCC:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 8FCC: 17 FE AC      \n");
                                      #endif
    --S;
    wr_mem(S, 0xcf); 
    --S;
    wr_mem(S, 0x8f); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8FCF:
                                                     #ifdef DEBUG
                                      mon("L8FCF    LDD   #$1289                  ; 8FCF: CC 12 89      \n");
                                      #endif
    A = 0x12; 
    B = 0x89; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8FD2; 
    JUMP; // sstep
  case 0x8FD2:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 8FD2: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x8FD5; 
    JUMP; // sstep
  case 0x8FD5:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8000                  ; 8FD5: CC 80 00      \n");
                                      #endif
    A = 0x80; 
    B = 0x00; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8FD8; 
    JUMP; // sstep
  case 0x8FD8:
                                                     #ifdef DEBUG
                                      mon("         LDD   D,X                     ; 8FD8: EC 8B         \n");
                                      #endif
    ea = (UINT16)(X + (((UINT8)A<<8)|(UINT8)B)); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8FDA; 
    JUMP; // sstep
  case 0x8FDA:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$1289                  ; 8FDA: 10 83 12 89   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x1289; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8FDE; 
    JUMP; // sstep
  case 0x8FDE:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8FDE: 10 26 03 93   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8FE2; 
    JUMP; // sstep
  case 0x8FE2:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8000                  ; 8FE2: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x8FE5; 
    JUMP; // sstep
  case 0x8FE5:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 8FE5: 17 FE 93      \n");
                                      #endif
    --S;
    wr_mem(S, 0xe8); 
    --S;
    wr_mem(S, 0x8f); 
    PC = 0x8e7b; 
    JUMP;
  case 0x8FE8:
                                                     #ifdef DEBUG
                                      mon("L8FE8    LDD   #$2299                  ; 8FE8: CC 22 99      \n");
                                      #endif
    A = 0x22; 
    B = 0x99; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8FEB; 
    JUMP; // sstep
  case 0x8FEB:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 8FEB: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x8FEE; 
    JUMP; // sstep
  case 0x8FEE:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 8FEE: CC 00 00      \n");
                                      #endif
    A = 0x00; 
    B = 0x00; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8FF1; 
    JUMP; // sstep
  case 0x8FF1:
                                                     #ifdef DEBUG
                                      mon("         LDD   D,X                     ; 8FF1: EC 8B         \n");
                                      #endif
    ea = (UINT16)(X + (((UINT8)A<<8)|(UINT8)B)); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x8FF3; 
    JUMP; // sstep
  case 0x8FF3:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$2299                  ; 8FF3: 10 83 22 99   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x2299; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x8FF7; 
    JUMP; // sstep
  case 0x8FF7:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 8FF7: 10 26 03 7A   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x8FFB; 
    JUMP; // sstep
  case 0x8FFB:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 8FFB: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x8FFC:
                                                     #ifdef DEBUG
                                      mon("L8FFC    LDU   #$92DB                  ; 8FFC: CE 92 DB      \n");
                                      #endif
    U = 0x92db; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x8FFF; 
    JUMP; // sstep
  case 0x8FFF:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$00                    ; 8FFF: 8E 00 00      \n");
                                      #endif
    X = 0x0000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x9002; 
    JUMP; // sstep
  case 0x9002:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9002: 17 FE 76      \n");
                                      #endif
    --S;
    wr_mem(S, 0x05); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9005:
                                                     #ifdef DEBUG
                                      mon("L9005    LDD   #$1289                  ; 9005: CC 12 89      \n");
                                      #endif
    A = 0x12; 
    B = 0x89; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9008; 
    JUMP; // sstep
  case 0x9008:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 9008: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x900B; 
    JUMP; // sstep
  case 0x900B:
                                                     #ifdef DEBUG
                                      mon("         LDD   -$8000,X                ; 900B: EC 89 80 00   \n");
                                      #endif
    ea = (UINT16)(X + 0x8000); 
    A = rd_mem(ea); // Care needed with I/O space and word fetches
    B = rd_mem((UINT16)(ea+1)); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x900F; 
    JUMP; // sstep
  case 0x900F:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$1289                  ; 900F: 10 83 12 89   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x1289; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9013; 
    JUMP; // sstep
  case 0x9013:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9013: 10 26 03 5E   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9017; 
    JUMP; // sstep
  case 0x9017:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8000                  ; 9017: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x901A; 
    JUMP; // sstep
  case 0x901A:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 901A: 17 FE 5E      \n");
                                      #endif
    --S;
    wr_mem(S, 0x1d); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x901D:
                                                     #ifdef DEBUG
                                      mon("L901D    LDD   #$2299                  ; 901D: CC 22 99      \n");
                                      #endif
    A = 0x22; 
    B = 0x99; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9020; 
    JUMP; // sstep
  case 0x9020:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 9020: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x9023; 
    JUMP; // sstep
  case 0x9023:
                                                     #ifdef DEBUG
                                      mon("         LDD   ,X                      ; 9023: EC 84         \n");
                                      #endif
    A = rd_mem(X); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(X+1)); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9025; 
    JUMP; // sstep
  case 0x9025:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$2299                  ; 9025: 10 83 22 99   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x2299; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9029; 
    JUMP; // sstep
  case 0x9029:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9029: 10 26 03 48   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x902D; 
    JUMP; // sstep
  case 0x902D:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 902D: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 902E:  ' ' 00 CE          NEG   <$CE ; 
  case 0x902F:
                                                     #ifdef DEBUG
                                      mon("L902F    LDU   #$92E5                  ; 902F: CE 92 E5      \n");
                                      #endif
    U = 0x92e5; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x9032; 
    JUMP; // sstep
  case 0x9032:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9032: 17 FE 46      \n");
                                      #endif
    --S;
    wr_mem(S, 0x35); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9035:
                                                     #ifdef DEBUG
                                      mon("L9035    LDA   #$13                    ; 9035: 86 13         \n");
                                      #endif
    val = (UINT8)0x13; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x9037; 
    JUMP; // sstep
  case 0x9037:
                                                     #ifdef DEBUG
                                      mon("         STA   $902E                   ; 9037: B7 90 2E      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x902e, val); 
    PC = 0x903A; 
    JUMP; // sstep
  case 0x903A:
                                                     #ifdef DEBUG
                                      mon("         LDA   $FFF1,PC                ; 903A: A6 8C F1      \n");
                                      #endif    //  ($902E)
    val = (UINT8)rd_mem(0x902e); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x903D; 
    JUMP; // sstep
  case 0x903D:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$13                    ; 903D: 81 13         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x13; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x903F; 
    JUMP; // sstep
  case 0x903F:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 903F: 10 26 03 32   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9043; 
    JUMP; // sstep
  case 0x9043:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9043: 17 FE 35      \n");
                                      #endif
    --S;
    wr_mem(S, 0x46); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9046:
                                                     #ifdef DEBUG
                                      mon("L9046    LDA   #$23                    ; 9046: 86 23         \n");
                                      #endif
    val = (UINT8)0x23; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x9048; 
    JUMP; // sstep
  case 0x9048:
                                                     #ifdef DEBUG
                                      mon("         STA   $9055                   ; 9048: B7 90 55      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x9055, val); 
    PC = 0x904B; 
    JUMP; // sstep
  case 0x904B:
                                                     #ifdef DEBUG
                                      mon("         LDA   $07,PC                  ; 904B: A6 8C 07      \n");
                                      #endif    //  ($9055)
    val = (UINT8)rd_mem(0x9055); 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x904E; 
    JUMP; // sstep
  case 0x904E:
                                                     #ifdef DEBUG
                                      mon("         CMPA  #$23                    ; 904E: 81 23         \n");
                                      #endif
    val = (UINT8)A; 
    arg = 0x23; 
    res = val - (UINT8)arg; 
    C = res & 0x100; 
    Z = (UINT8)res; 
    N = (UINT8)res; 
    V = (UINT8)((val ^ arg) & (val ^ res)); 
    PC = 0x9050; 
    JUMP; // sstep
  case 0x9050:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9050: 10 26 03 21   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9054; 
    JUMP; // sstep
  case 0x9054:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 9054: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 9055:  ' ' 00 CE          NEG   <$CE ; 
  case 0x9056:
                                                     #ifdef DEBUG
                                      mon("L9056    LDU   #$92EF                  ; 9056: CE 92 EF      \n");
                                      #endif
    U = 0x92ef; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x9059; 
    JUMP; // sstep
  case 0x9059:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9059: 17 FE 1F      \n");
                                      #endif
    --S;
    wr_mem(S, 0x5c); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x905C:
                                                     #ifdef DEBUG
                                      mon("L905C    LDD   #$9876                  ; 905C: CC 98 76      \n");
                                      #endif
    A = 0x98; 
    B = 0x76; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x905F; 
    JUMP; // sstep
  case 0x905F:
                                                     #ifdef DEBUG
                                      mon("         STD   $8103                   ; 905F: FD 81 03      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8103, A); // Care needed with I/O space and word fetches
    wr_mem(0x8104, B); 
    PC = 0x9062; 
    JUMP; // sstep
  case 0x9062:
                                                     #ifdef DEBUG
                                      mon("         LDD   $F09D,PC                ; 9062: EC 8D F0 9D   \n");
                                      #endif    //  ($18103)
    A = rd_mem(0x8103); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(0x8103+1)); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9066; 
    JUMP; // sstep
  case 0x9066:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$9876                  ; 9066: 10 83 98 76   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x9876; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x906A; 
    JUMP; // sstep
  case 0x906A:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 906A: 10 26 03 07   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x906E; 
    JUMP; // sstep
  case 0x906E:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 906E: 17 FE 0A      \n");
                                      #endif
    --S;
    wr_mem(S, 0x71); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9071:
                                                     #ifdef DEBUG
                                      mon("L9071    LDD   #$5432                  ; 9071: CC 54 32      \n");
                                      #endif
    A = 0x54; 
    B = 0x32; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9074; 
    JUMP; // sstep
  case 0x9074:
                                                     #ifdef DEBUG
                                      mon("         STD   $9392                   ; 9074: FD 93 92      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x9392, A); // Care needed with I/O space and word fetches
    wr_mem(0x9393, B); 
    PC = 0x9077; 
    JUMP; // sstep
  case 0x9077:
                                                     #ifdef DEBUG
                                      mon("         LDD   $0317,PC                ; 9077: EC 8D 03 17   \n");
                                      #endif    //  ($9392)
    A = rd_mem(0x9392); // Care needed with I/O space and word fetches. Use 'ea' here?
    B = rd_mem((UINT16)(0x9392+1)); 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x907B; 
    JUMP; // sstep
  case 0x907B:
                                                     #ifdef DEBUG
                                      mon("         CMPD  #$5432                  ; 907B: 10 83 54 32   \n");
                                      #endif
    val = (((UINT8)A<<8)|(UINT8)B); 
    arg = 0x5432; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x907F; 
    JUMP; // sstep
  case 0x907F:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 907F: 10 26 02 F2   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9083; 
    JUMP; // sstep
  case 0x9083:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 9083: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x9084:
                                                     #ifdef DEBUG
                                      mon("L9084    LDU   #$92FA                  ; 9084: CE 92 FA      \n");
                                      #endif
    U = 0x92fa; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x9087; 
    JUMP; // sstep
  case 0x9087:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8000                  ; 9087: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x908A; 
    JUMP; // sstep
  case 0x908A:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 908A: 17 FD EE      \n");
                                      #endif
    --S;
    wr_mem(S, 0x8d); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x908D:
                                                     #ifdef DEBUG
                                      mon("L908D    LDD   #$5522                  ; 908D: CC 55 22      \n");
                                      #endif
    A = 0x55; 
    B = 0x22; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9090; 
    JUMP; // sstep
  case 0x9090:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 9090: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x9093; 
    JUMP; // sstep
  case 0x9093:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [,X]                    ; 9093: 31 94         \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x9095; 
    JUMP; // sstep
  case 0x9095:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$5522                  ; 9095: 10 8C 55 22   \n");
                                      #endif
    val = Y; 
    arg = 0x5522; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9099; 
    JUMP; // sstep
  case 0x9099:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9099: 10 26 02 D8   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x909D; 
    JUMP; // sstep
  case 0x909D:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 909D: 17 FD DB      \n");
                                      #endif
    --S;
    wr_mem(S, 0xa0); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x90A0:
                                                     #ifdef DEBUG
                                      mon("L90A0    LDD   #$1234                  ; 90A0: CC 12 34      \n");
                                      #endif
    A = 0x12; 
    B = 0x34; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x90A3; 
    JUMP; // sstep
  case 0x90A3:
                                                     #ifdef DEBUG
                                      mon("         STD   $8001                   ; 90A3: FD 80 01      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8001, A); // Care needed with I/O space and word fetches
    wr_mem(0x8002, B); 
    PC = 0x90A6; 
    JUMP; // sstep
  case 0x90A6:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [$01,X]                 ; 90A6: 31 98 01      \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (SINT8)(SINT8)0x01))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0x0002)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x90A9; 
    JUMP; // sstep
  case 0x90A9:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$1234                  ; 90A9: 10 8C 12 34   \n");
                                      #endif
    val = Y; 
    arg = 0x1234; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x90AD; 
    JUMP; // sstep
  case 0x90AD:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 90AD: 10 26 02 C4   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x90B1; 
    JUMP; // sstep
  case 0x90B1:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 90B1: 17 FD C7      \n");
                                      #endif
    --S;
    wr_mem(S, 0xb4); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x90B4:
                                                     #ifdef DEBUG
                                      mon("L90B4    LDD   #$4444                  ; 90B4: CC 44 44      \n");
                                      #endif
    A = 0x44; 
    B = 0x44; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x90B7; 
    JUMP; // sstep
  case 0x90B7:
                                                     #ifdef DEBUG
                                      mon("         STD   $807F                   ; 90B7: FD 80 7F      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x807f, A); // Care needed with I/O space and word fetches
    wr_mem(0x8080, B); 
    PC = 0x90BA; 
    JUMP; // sstep
  case 0x90BA:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [$7F,X]                 ; 90BA: 31 98 7F      \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (SINT8)(SINT8)0x7f))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0x0080)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x90BD; 
    JUMP; // sstep
  case 0x90BD:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$4444                  ; 90BD: 10 8C 44 44   \n");
                                      #endif
    val = Y; 
    arg = 0x4444; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x90C1; 
    JUMP; // sstep
  case 0x90C1:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 90C1: 10 26 02 B0   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x90C5; 
    JUMP; // sstep
  case 0x90C5:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 90C5: 17 FD B3      \n");
                                      #endif
    --S;
    wr_mem(S, 0xc8); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x90C8:
                                                     #ifdef DEBUG
                                      mon("L90C8    LDD   #$3456                  ; 90C8: CC 34 56      \n");
                                      #endif
    A = 0x34; 
    B = 0x56; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x90CB; 
    JUMP; // sstep
  case 0x90CB:
                                                     #ifdef DEBUG
                                      mon("         STD   $7FFF                   ; 90CB: FD 7F FF      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x7fff, A); // Care needed with I/O space and word fetches
    wr_mem(0x8000, B); 
    PC = 0x90CE; 
    JUMP; // sstep
  case 0x90CE:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [-$01,X]                ; 90CE: 31 98 FF      \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (SINT8)(SINT8)0xff))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0x0000)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x90D1; 
    JUMP; // sstep
  case 0x90D1:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$3456                  ; 90D1: 10 8C 34 56   \n");
                                      #endif
    val = Y; 
    arg = 0x3456; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x90D5; 
    JUMP; // sstep
  case 0x90D5:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 90D5: 10 26 02 9C   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x90D9; 
    JUMP; // sstep
  case 0x90D9:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 90D9: 17 FD 9F      \n");
                                      #endif
    --S;
    wr_mem(S, 0xdc); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x90DC:
                                                     #ifdef DEBUG
                                      mon("L90DC    LDD   #$AAAA                  ; 90DC: CC AA AA      \n");
                                      #endif
    A = 0xaa; 
    B = 0xaa; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x90DF; 
    JUMP; // sstep
  case 0x90DF:
                                                     #ifdef DEBUG
                                      mon("         STD   $7F80                   ; 90DF: FD 7F 80      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x7f80, A); // Care needed with I/O space and word fetches
    wr_mem(0x7f81, B); 
    PC = 0x90E2; 
    JUMP; // sstep
  case 0x90E2:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [-$80,X]                ; 90E2: 31 98 80      \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (SINT8)(SINT8)0x80))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0xff81)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x90E5; 
    JUMP; // sstep
  case 0x90E5:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$AAAA                  ; 90E5: 10 8C AA AA   \n");
                                      #endif
    val = Y; 
    arg = 0xaaaa; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x90E9; 
    JUMP; // sstep
  case 0x90E9:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 90E9: 10 26 02 88   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x90ED; 
    JUMP; // sstep
  case 0x90ED:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 90ED: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x90EE:
                                                     #ifdef DEBUG
                                      mon("L90EE    LDX   #$8000                  ; 90EE: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x90F1; 
    JUMP; // sstep
  case 0x90F1:
                                                     #ifdef DEBUG
                                      mon("         LDU   #$9306                  ; 90F1: CE 93 06      \n");
                                      #endif
    U = 0x9306; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x90F4; 
    JUMP; // sstep
  case 0x90F4:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 90F4: 17 FD 84      \n");
                                      #endif
    --S;
    wr_mem(S, 0xf7); 
    --S;
    wr_mem(S, 0x90); 
    PC = 0x8e7b; 
    JUMP;
  case 0x90F7:
                                                     #ifdef DEBUG
                                      mon("L90F7    LDD   #$A55A                  ; 90F7: CC A5 5A      \n");
                                      #endif
    A = 0xa5; 
    B = 0x5a; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x90FA; 
    JUMP; // sstep
  case 0x90FA:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 90FA: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x90FD; 
    JUMP; // sstep
  case 0x90FD:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$5AA5                  ; 90FD: CC 5A A5      \n");
                                      #endif
    A = 0x5a; 
    B = 0xa5; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9100; 
    JUMP; // sstep
  case 0x9100:
                                                     #ifdef DEBUG
                                      mon("         STD   $8002                   ; 9100: FD 80 02      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8002, A); // Care needed with I/O space and word fetches
    wr_mem(0x8003, B); 
    PC = 0x9103; 
    JUMP; // sstep
  case 0x9103:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [,X++]                  ; 9103: 31 91         \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    Y = ea; 
/**/Z = Y; 
    X = X + 2; 
    PC = 0x9105; 
    JUMP; // sstep
  case 0x9105:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$A55A                  ; 9105: 10 8C A5 5A   \n");
                                      #endif
    val = Y; 
    arg = 0xa55a; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9109; 
    JUMP; // sstep
  case 0x9109:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9109: 10 26 02 68   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x910D; 
    JUMP; // sstep
  case 0x910D:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [,X++]                  ; 910D: 31 91         \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    Y = ea; 
/**/Z = Y; 
    X = X + 2; 
    PC = 0x910F; 
    JUMP; // sstep
  case 0x910F:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$5AA5                  ; 910F: 10 8C 5A A5   \n");
                                      #endif
    val = Y; 
    arg = 0x5aa5; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9113; 
    JUMP; // sstep
  case 0x9113:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9113: 10 26 02 5E   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9117; 
    JUMP; // sstep
  case 0x9117:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 9117: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x9118:
                                                     #ifdef DEBUG
                                      mon("L9118    LDX   #$8000                  ; 9118: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
/**/Z = X; 
/**/N = (X) >> 8; 
/**/V = 0; 
    PC = 0x911B; 
    JUMP; // sstep
  case 0x911B:
                                                     #ifdef DEBUG
                                      mon("         LDU   #$9312                  ; 911B: CE 93 12      \n");
                                      #endif
    U = 0x9312; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x911E; 
    JUMP; // sstep
  case 0x911E:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 911E: 17 FD 5A      \n");
                                      #endif
    --S;
    wr_mem(S, 0x21); 
    --S;
    wr_mem(S, 0x91); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9121:
                                                     #ifdef DEBUG
                                      mon("L9121    LDD   #$A55A                  ; 9121: CC A5 5A      \n");
                                      #endif
    A = 0xa5; 
    B = 0x5a; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9124; 
    JUMP; // sstep
  case 0x9124:
                                                     #ifdef DEBUG
                                      mon("         STD   $7FFE                   ; 9124: FD 7F FE      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x7ffe, A); // Care needed with I/O space and word fetches
    wr_mem(0x7fff, B); 
    PC = 0x9127; 
    JUMP; // sstep
  case 0x9127:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$789A                  ; 9127: CC 78 9A      \n");
                                      #endif
    A = 0x78; 
    B = 0x9a; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x912A; 
    JUMP; // sstep
  case 0x912A:
                                                     #ifdef DEBUG
                                      mon("         STD   $7FFC                   ; 912A: FD 7F FC      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x7ffc, A); // Care needed with I/O space and word fetches
    wr_mem(0x7ffd, B); 
    PC = 0x912D; 
    JUMP; // sstep
  case 0x912D:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [,--X]                  ; 912D: 31 93         \n");
                                      #endif
    X = X - 2; 
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x912F; 
    JUMP; // sstep
  case 0x912F:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$A55A                  ; 912F: 10 8C A5 5A   \n");
                                      #endif
    val = Y; 
    arg = 0xa55a; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9133; 
    JUMP; // sstep
  case 0x9133:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9133: 10 26 02 3E   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9137; 
    JUMP; // sstep
  case 0x9137:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [,--X]                  ; 9137: 31 93         \n");
                                      #endif
    X = X - 2; 
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x9139; 
    JUMP; // sstep
  case 0x9139:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$789A                  ; 9139: 10 8C 78 9A   \n");
                                      #endif
    val = Y; 
    arg = 0x789a; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x913D; 
    JUMP; // sstep
  case 0x913D:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 913D: 10 26 02 34   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9141; 
    JUMP; // sstep
  case 0x9141:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 9141: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x9142:
                                                     #ifdef DEBUG
                                      mon("L9142    LDU   #$931E                  ; 9142: CE 93 1E      \n");
                                      #endif
    U = 0x931e; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x9145; 
    JUMP; // sstep
  case 0x9145:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8000                  ; 9145: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x9148; 
    JUMP; // sstep
  case 0x9148:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9148: 17 FD 30      \n");
                                      #endif
    --S;
    wr_mem(S, 0x4b); 
    --S;
    wr_mem(S, 0x91); 
    PC = 0x8e7b; 
    JUMP;
  case 0x914B:
                                                     #ifdef DEBUG
                                      mon("L914B    LDD   #$1234                  ; 914B: CC 12 34      \n");
                                      #endif
    A = 0x12; 
    B = 0x34; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x914E; 
    JUMP; // sstep
  case 0x914E:
                                                     #ifdef DEBUG
                                      mon("         STD   $8001                   ; 914E: FD 80 01      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8001, A); // Care needed with I/O space and word fetches
    wr_mem(0x8002, B); 
    PC = 0x9151; 
    JUMP; // sstep
  case 0x9151:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$01                    ; 9151: 86 01         \n");
                                      #endif
    val = (UINT8)0x01; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x9153; 
    JUMP; // sstep
  case 0x9153:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [A,X]                   ; 9153: 31 96         \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (SINT8)A))<<8; 
    ea = ea | rd_mem((UINT16)(X + (SINT8)A + 1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x9155; 
    JUMP; // sstep
  case 0x9155:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$1234                  ; 9155: 10 8C 12 34   \n");
                                      #endif
    val = Y; 
    arg = 0x1234; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9159; 
    JUMP; // sstep
  case 0x9159:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9159: 10 26 02 18   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x915D; 
    JUMP; // sstep
  case 0x915D:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 915D: 17 FD 1B      \n");
                                      #endif
    --S;
    wr_mem(S, 0x60); 
    --S;
    wr_mem(S, 0x91); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9160:
                                                     #ifdef DEBUG
                                      mon("L9160    LDD   #$4444                  ; 9160: CC 44 44      \n");
                                      #endif
    A = 0x44; 
    B = 0x44; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9163; 
    JUMP; // sstep
  case 0x9163:
                                                     #ifdef DEBUG
                                      mon("         STD   $807F                   ; 9163: FD 80 7F      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x807f, A); // Care needed with I/O space and word fetches
    wr_mem(0x8080, B); 
    PC = 0x9166; 
    JUMP; // sstep
  case 0x9166:
                                                     #ifdef DEBUG
                                      mon("         LDA   #$7F                    ; 9166: 86 7F         \n");
                                      #endif
    val = (UINT8)0x7f; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x9168; 
    JUMP; // sstep
  case 0x9168:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [A,X]                   ; 9168: 31 96         \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (SINT8)A))<<8; 
    ea = ea | rd_mem((UINT16)(X + (SINT8)A + 1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x916A; 
    JUMP; // sstep
  case 0x916A:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$4444                  ; 916A: 10 8C 44 44   \n");
                                      #endif
    val = Y; 
    arg = 0x4444; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x916E; 
    JUMP; // sstep
  case 0x916E:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 916E: 10 26 02 03   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9172; 
    JUMP; // sstep
  case 0x9172:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 9172: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x9173:
                                                     #ifdef DEBUG
                                      mon("L9173    LDU   #$9329                  ; 9173: CE 93 29      \n");
                                      #endif
    U = 0x9329; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x9176; 
    JUMP; // sstep
  case 0x9176:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8000                  ; 9176: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x9179; 
    JUMP; // sstep
  case 0x9179:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9179: 17 FC FF      \n");
                                      #endif
    --S;
    wr_mem(S, 0x7c); 
    --S;
    wr_mem(S, 0x91); 
    PC = 0x8e7b; 
    JUMP;
  case 0x917C:
                                                     #ifdef DEBUG
                                      mon("L917C    LDD   #$3456                  ; 917C: CC 34 56      \n");
                                      #endif
    A = 0x34; 
    B = 0x56; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x917F; 
    JUMP; // sstep
  case 0x917F:
                                                     #ifdef DEBUG
                                      mon("         STD   $7FFF                   ; 917F: FD 7F FF      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x7fff, A); // Care needed with I/O space and word fetches
    wr_mem(0x8000, B); 
    PC = 0x9182; 
    JUMP; // sstep
  case 0x9182:
                                                     #ifdef DEBUG
                                      mon("         LDB   #$FF                    ; 9182: C6 FF         \n");
                                      #endif
    val = (UINT8)0xff; 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x9184; 
    JUMP; // sstep
  case 0x9184:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [B,X]                   ; 9184: 31 95         \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (SINT8)B))<<8; 
    ea = ea | rd_mem((UINT16)(X + (SINT8)B + 1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x9186; 
    JUMP; // sstep
  case 0x9186:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$3456                  ; 9186: 10 8C 34 56   \n");
                                      #endif
    val = Y; 
    arg = 0x3456; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x918A; 
    JUMP; // sstep
  case 0x918A:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 918A: 10 26 01 E7   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x918E; 
    JUMP; // sstep
  case 0x918E:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 918E: 17 FC EA      \n");
                                      #endif
    --S;
    wr_mem(S, 0x91); 
    --S;
    wr_mem(S, 0x91); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9191:
                                                     #ifdef DEBUG
                                      mon("L9191    LDD   #$AAAA                  ; 9191: CC AA AA      \n");
                                      #endif
    A = 0xaa; 
    B = 0xaa; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9194; 
    JUMP; // sstep
  case 0x9194:
                                                     #ifdef DEBUG
                                      mon("         STD   $7F80                   ; 9194: FD 7F 80      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x7f80, A); // Care needed with I/O space and word fetches
    wr_mem(0x7f81, B); 
    PC = 0x9197; 
    JUMP; // sstep
  case 0x9197:
                                                     #ifdef DEBUG
                                      mon("         LDB   #$80                    ; 9197: C6 80         \n");
                                      #endif
    val = (UINT8)0x80; 
    B = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x9199; 
    JUMP; // sstep
  case 0x9199:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [B,X]                   ; 9199: 31 95         \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (SINT8)B))<<8; 
    ea = ea | rd_mem((UINT16)(X + (SINT8)B + 1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x919B; 
    JUMP; // sstep
  case 0x919B:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$AAAA                  ; 919B: 10 8C AA AA   \n");
                                      #endif
    val = Y; 
    arg = 0xaaaa; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x919F; 
    JUMP; // sstep
  case 0x919F:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 919F: 10 26 01 D2   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x91A3; 
    JUMP; // sstep
  case 0x91A3:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 91A3: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x91A4:
                                                     #ifdef DEBUG
                                      mon("L91A4    LDU   #$9334                  ; 91A4: CE 93 34      \n");
                                      #endif
    U = 0x9334; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x91A7; 
    JUMP; // sstep
  case 0x91A7:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$00                    ; 91A7: 8E 00 00      \n");
                                      #endif
    X = 0x0000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x91AA; 
    JUMP; // sstep
  case 0x91AA:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 91AA: 17 FC CE      \n");
                                      #endif
    --S;
    wr_mem(S, 0xad); 
    --S;
    wr_mem(S, 0x91); 
    PC = 0x8e7b; 
    JUMP;
  case 0x91AD:
                                                     #ifdef DEBUG
                                      mon("L91AD    LDD   #$1289                  ; 91AD: CC 12 89      \n");
                                      #endif
    A = 0x12; 
    B = 0x89; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x91B0; 
    JUMP; // sstep
  case 0x91B0:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 91B0: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x91B3; 
    JUMP; // sstep
  case 0x91B3:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$8000                  ; 91B3: CC 80 00      \n");
                                      #endif
    A = 0x80; 
    B = 0x00; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x91B6; 
    JUMP; // sstep
  case 0x91B6:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [D,X]                   ; 91B6: 31 9B         \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (((UINT8)A<<8)|(UINT8)B)))<<8; 
    ea = ea | rd_mem((UINT16)(X + (((UINT8)A<<8)|(UINT8)B) + 1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x91B8; 
    JUMP; // sstep
  case 0x91B8:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$1289                  ; 91B8: 10 8C 12 89   \n");
                                      #endif
    val = Y; 
    arg = 0x1289; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x91BC; 
    JUMP; // sstep
  case 0x91BC:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 91BC: 10 26 01 B5   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x91C0; 
    JUMP; // sstep
  case 0x91C0:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8000                  ; 91C0: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x91C3; 
    JUMP; // sstep
  case 0x91C3:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 91C3: 17 FC B5      \n");
                                      #endif
    --S;
    wr_mem(S, 0xc6); 
    --S;
    wr_mem(S, 0x91); 
    PC = 0x8e7b; 
    JUMP;
  case 0x91C6:
                                                     #ifdef DEBUG
                                      mon("L91C6    LDD   #$2299                  ; 91C6: CC 22 99      \n");
                                      #endif
    A = 0x22; 
    B = 0x99; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x91C9; 
    JUMP; // sstep
  case 0x91C9:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 91C9: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x91CC; 
    JUMP; // sstep
  case 0x91CC:
                                                     #ifdef DEBUG
                                      mon("         LDD   #$00                    ; 91CC: CC 00 00      \n");
                                      #endif
    A = 0x00; 
    B = 0x00; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x91CF; 
    JUMP; // sstep
  case 0x91CF:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [D,X]                   ; 91CF: 31 9B         \n");
                                      #endif
    ea = rd_mem((UINT16)(X + (((UINT8)A<<8)|(UINT8)B)))<<8; 
    ea = ea | rd_mem((UINT16)(X + (((UINT8)A<<8)|(UINT8)B) + 1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x91D1; 
    JUMP; // sstep
  case 0x91D1:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$2299                  ; 91D1: 10 8C 22 99   \n");
                                      #endif
    val = Y; 
    arg = 0x2299; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x91D5; 
    JUMP; // sstep
  case 0x91D5:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 91D5: 10 26 01 9C   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x91D9; 
    JUMP; // sstep
  case 0x91D9:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 91D9: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x91DA:
                                                     #ifdef DEBUG
                                      mon("L91DA    LDU   #$933F                  ; 91DA: CE 93 3F      \n");
                                      #endif
    U = 0x933f; 
/**/Z = U; 
/**/N = (U) >> 8; 
/**/V = 0; 
    PC = 0x91DD; 
    JUMP; // sstep
  case 0x91DD:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$00                    ; 91DD: 8E 00 00      \n");
                                      #endif
    X = 0x0000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x91E0; 
    JUMP; // sstep
  case 0x91E0:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 91E0: 17 FC 98      \n");
                                      #endif
    --S;
    wr_mem(S, 0xe3); 
    --S;
    wr_mem(S, 0x91); 
    PC = 0x8e7b; 
    JUMP;
  case 0x91E3:
                                                     #ifdef DEBUG
                                      mon("L91E3    LDD   #$1289                  ; 91E3: CC 12 89      \n");
                                      #endif
    A = 0x12; 
    B = 0x89; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x91E6; 
    JUMP; // sstep
  case 0x91E6:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 91E6: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x91E9; 
    JUMP; // sstep
  case 0x91E9:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [-$8000,X]              ; 91E9: 31 99 80 00   \n");
                                      #endif
    ea = rd_mem((UINT16)(X + 0x8000))<<8; 
    ea = ea | rd_mem((UINT16)(X + 0x8001)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x91ED; 
    JUMP; // sstep
  case 0x91ED:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$1289                  ; 91ED: 10 8C 12 89   \n");
                                      #endif
    val = Y; 
    arg = 0x1289; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x91F1; 
    JUMP; // sstep
  case 0x91F1:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 91F1: 10 26 01 80   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x91F5; 
    JUMP; // sstep
  case 0x91F5:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$8000                  ; 91F5: 8E 80 00      \n");
                                      #endif
    X = 0x8000; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x91F8; 
    JUMP; // sstep
  case 0x91F8:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 91F8: 17 FC 80      \n");
                                      #endif
    --S;
    wr_mem(S, 0xfb); 
    --S;
    wr_mem(S, 0x91); 
    PC = 0x8e7b; 
    JUMP;
  case 0x91FB:
                                                     #ifdef DEBUG
                                      mon("L91FB    LDD   #$2299                  ; 91FB: CC 22 99      \n");
                                      #endif
    A = 0x22; 
    B = 0x99; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x91FE; 
    JUMP; // sstep
  case 0x91FE:
                                                     #ifdef DEBUG
                                      mon("         STD   $8000                   ; 91FE: FD 80 00      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8000, A); // Care needed with I/O space and word fetches
    wr_mem(0x8001, B); 
    PC = 0x9201; 
    JUMP; // sstep
  case 0x9201:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [,X]                    ; 9201: 31 94         \n");
                                      #endif
    ea = rd_mem(X)<<8; 
    ea = ea | rd_mem((UINT16)((X)+1)); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x9203; 
    JUMP; // sstep
  case 0x9203:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$2299                  ; 9203: 10 8C 22 99   \n");
                                      #endif
    val = Y; 
    arg = 0x2299; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9207; 
    JUMP; // sstep
  case 0x9207:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9207: 10 26 01 6A   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x920B; 
    JUMP; // sstep
  case 0x920B:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 920B: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 920C:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 920D:  ' ' 00 CE          NEG   <$CE ; 
  case 0x920E:
                                                     #ifdef DEBUG
                                      mon("L920E    LDU   #$934C                  ; 920E: CE 93 4C      \n");
                                      #endif
    U = 0x934c; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x9211; 
    JUMP; // sstep
  case 0x9211:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9211: 17 FC 67      \n");
                                      #endif
    --S;
    wr_mem(S, 0x14); 
    --S;
    wr_mem(S, 0x92); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9214:
                                                     #ifdef DEBUG
                                      mon("L9214    LDD   #$1357                  ; 9214: CC 13 57      \n");
                                      #endif
    A = 0x13; 
    B = 0x57; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9217; 
    JUMP; // sstep
  case 0x9217:
                                                     #ifdef DEBUG
                                      mon("         STD   $920C                   ; 9217: FD 92 0C      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x920c, A); // Care needed with I/O space and word fetches
    wr_mem(0x920d, B); 
    PC = 0x921A; 
    JUMP; // sstep
  case 0x921A:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [-$11,PC]               ; 921A: 31 9C EF      \n");
                                      #endif
    ea = rd_mem(0x920c)<<8; 
    ea = ea | rd_mem(0x920d); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x921D; 
    JUMP; // sstep
  case 0x921D:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$1357                  ; 921D: 10 8C 13 57   \n");
                                      #endif
    val = Y; 
    arg = 0x1357; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9221; 
    JUMP; // sstep
  case 0x9221:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9221: 10 26 01 50   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9225; 
    JUMP; // sstep
  case 0x9225:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9225: 17 FC 53      \n");
                                      #endif
    --S;
    wr_mem(S, 0x28); 
    --S;
    wr_mem(S, 0x92); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9228:
                                                     #ifdef DEBUG
                                      mon("L9228    LDD   #$2345                  ; 9228: CC 23 45      \n");
                                      #endif
    A = 0x23; 
    B = 0x45; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x922B; 
    JUMP; // sstep
  case 0x922B:
                                                     #ifdef DEBUG
                                      mon("         STD   $923A                   ; 922B: FD 92 3A      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x923a, A); // Care needed with I/O space and word fetches
    wr_mem(0x923b, B); 
    PC = 0x922E; 
    JUMP; // sstep
  case 0x922E:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [$09,PC]                ; 922E: 31 9C 09      \n");
                                      #endif
    ea = rd_mem(0x923a)<<8; 
    ea = ea | rd_mem(0x923b); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x9231; 
    JUMP; // sstep
  case 0x9231:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$2345                  ; 9231: 10 8C 23 45   \n");
                                      #endif
    val = Y; 
    arg = 0x2345; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9235; 
    JUMP; // sstep
  case 0x9235:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9235: 10 26 01 3C   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9239; 
    JUMP; // sstep
  case 0x9239:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 9239: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 923A:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 923B:  ' ' 00 CE          NEG   <$CE ; 
  case 0x923C:
                                                     #ifdef DEBUG
                                      mon("L923C    LDU   #$935A                  ; 923C: CE 93 5A      \n");
                                      #endif
    U = 0x935a; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x923F; 
    JUMP; // sstep
  case 0x923F:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 923F: 17 FC 39      \n");
                                      #endif
    --S;
    wr_mem(S, 0x42); 
    --S;
    wr_mem(S, 0x92); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9242:
                                                     #ifdef DEBUG
                                      mon("L9242    LDD   #$9876                  ; 9242: CC 98 76      \n");
                                      #endif
    A = 0x98; 
    B = 0x76; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9245; 
    JUMP; // sstep
  case 0x9245:
                                                     #ifdef DEBUG
                                      mon("         STD   $8103                   ; 9245: FD 81 03      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8103, A); // Care needed with I/O space and word fetches
    wr_mem(0x8104, B); 
    PC = 0x9248; 
    JUMP; // sstep
  case 0x9248:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [-$1149,PC]             ; 9248: 31 9D EE B7   \n");
                                      #endif
    ea = rd_mem(0x8103)<<8; 
    ea = ea | rd_mem(0x8104); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x924C; 
    JUMP; // sstep
  case 0x924C:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$9876                  ; 924C: 10 8C 98 76   \n");
                                      #endif
    val = Y; 
    arg = 0x9876; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9250; 
    JUMP; // sstep
  case 0x9250:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9250: 10 26 01 21   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9254; 
    JUMP; // sstep
  case 0x9254:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9254: 17 FC 24      \n");
                                      #endif
    --S;
    wr_mem(S, 0x57); 
    --S;
    wr_mem(S, 0x92); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9257:
                                                     #ifdef DEBUG
                                      mon("L9257    LDD   #$5432                  ; 9257: CC 54 32      \n");
                                      #endif
    A = 0x54; 
    B = 0x32; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x925A; 
    JUMP; // sstep
  case 0x925A:
                                                     #ifdef DEBUG
                                      mon("         STD   $9392                   ; 925A: FD 93 92      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x9392, A); // Care needed with I/O space and word fetches
    wr_mem(0x9393, B); 
    PC = 0x925D; 
    JUMP; // sstep
  case 0x925D:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [$0131,PC]              ; 925D: 31 9D 01 31   \n");
                                      #endif
    ea = rd_mem(0x9392)<<8; 
    ea = ea | rd_mem(0x9393); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x9261; 
    JUMP; // sstep
  case 0x9261:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$5432                  ; 9261: 10 8C 54 32   \n");
                                      #endif
    val = Y; 
    arg = 0x5432; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9265; 
    JUMP; // sstep
  case 0x9265:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9265: 10 26 01 0C   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9269; 
    JUMP; // sstep
  case 0x9269:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 9269: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x926A:
                                                     #ifdef DEBUG
                                      mon("L926A    LDU   #$9369                  ; 926A: CE 93 69      \n");
                                      #endif
    U = 0x9369; 
    Z = U; 
    N = (U) >> 8; 
    V = 0; 
    PC = 0x926D; 
    JUMP; // sstep
  case 0x926D:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 926D: 17 FC 0B      \n");
                                      #endif
    --S;
    wr_mem(S, 0x70); 
    --S;
    wr_mem(S, 0x92); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9270:
                                                     #ifdef DEBUG
                                      mon("L9270    LDD   #$9876                  ; 9270: CC 98 76      \n");
                                      #endif
    A = 0x98; 
    B = 0x76; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9273; 
    JUMP; // sstep
  case 0x9273:
                                                     #ifdef DEBUG
                                      mon("         STD   $8103                   ; 9273: FD 81 03      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x8103, A); // Care needed with I/O space and word fetches
    wr_mem(0x8104, B); 
    PC = 0x9276; 
    JUMP; // sstep
  case 0x9276:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [$8103]                 ; 9276: 31 9F 81 03   \n");
                                      #endif
    ea = rd_mem(0x8103)<<8; 
    ea = ea | rd_mem(0x8104); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x927A; 
    JUMP; // sstep
  case 0x927A:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$9876                  ; 927A: 10 8C 98 76   \n");
                                      #endif
    val = Y; 
    arg = 0x9876; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x927E; 
    JUMP; // sstep
  case 0x927E:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 927E: 10 26 00 F3   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9282; 
    JUMP; // sstep
  case 0x9282:
                                                     #ifdef DEBUG
                                      mon("         LBSR  $8E7B                   ; 9282: 17 FB F6      \n");
                                      #endif
    --S;
    wr_mem(S, 0x85); 
    --S;
    wr_mem(S, 0x92); 
    PC = 0x8e7b; 
    JUMP;
  case 0x9285:
                                                     #ifdef DEBUG
                                      mon("L9285    LDD   #$5432                  ; 9285: CC 54 32      \n");
                                      #endif
    A = 0x54; 
    B = 0x32; 
/**/Z = A|B; 
/**/N = A; 
/**/V = 0; 
    PC = 0x9288; 
    JUMP; // sstep
  case 0x9288:
                                                     #ifdef DEBUG
                                      mon("         STD   $9392                   ; 9288: FD 93 92      \n");
                                      #endif
/**/Z = A | B; 
/**/N = A; 
/**/V = 0; 
    wr_mem(0x9392, A); // Care needed with I/O space and word fetches
    wr_mem(0x9393, B); 
    PC = 0x928B; 
    JUMP; // sstep
  case 0x928B:
                                                     #ifdef DEBUG
                                      mon("         LEAY  [$9392]                 ; 928B: 31 9F 93 92   \n");
                                      #endif
    ea = rd_mem(0x9392)<<8; 
    ea = ea | rd_mem(0x9393); 
    Y = ea; 
/**/Z = Y; 
    PC = 0x928F; 
    JUMP; // sstep
  case 0x928F:
                                                     #ifdef DEBUG
                                      mon("         CMPY  #$5432                  ; 928F: 10 8C 54 32   \n");
                                      #endif
    val = Y; 
    arg = 0x5432; 
    res = val - arg; 
    C = res & 0x10000; 
    res = (UINT16)res; 
    Z = res; 
    N = res >> 8; 
    V = ((val ^ arg) & (val ^ res)) >> 8; 
    PC = 0x9293; 
    JUMP; // sstep
  case 0x9293:
                                                     #ifdef DEBUG
                                      mon("         LBNE  $9375                   ; 9293: 10 26 00 DE   \n");
                                      #endif
    if (Z) {
    PC = 0x9375; 
    JUMP;
    }
    PC = 0x9297; 
    JUMP; // sstep
  case 0x9297:
                                                     #ifdef DEBUG
                                      mon("         RTS                           ; 9297: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $4C                     ; 9298:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 9299:  'D' 44             LSRA  ; 
                   // FCB   $41                     ; 929A:  'A' 41             Invalid ; 
                   // FCB   $20                     ; 929B:  ' ' 20 6E          BRA   $930B ; 
                   // FCB   $6E                     ; 929C:  'n' 6E 38          JMP   -8,Y ; 
                   // FCB   $38                     ; 929D:  '8' 38             Invalid ; 
                   // FCB   $2C                     ; 929E:  ',' 2C 58          BGE   $92F8 ; 
                   // FCB   $58                     ; 929F:  'X' 58             ASLB  ; 
                   // FCB   $04                     ; 92A0:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92A1:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92A2:  'D' 44             LSRA  ; 
                   // FCB   $41                     ; 92A3:  'A' 41             Invalid ; 
                   // FCB   $20                     ; 92A4:  ' ' 20 2C          BRA   $92D2 ; 
                   // FCB   $2C                     ; 92A5:  ',' 2C 58          BGE   $92FF ; 
                   // FCB   $58                     ; 92A6:  'X' 58             ASLB  ; 
                   // FCB   $2B                     ; 92A7:  '+' 2B 04          BMI   $92AD ; 
                   // FCB   $04                     ; 92A8:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92A9:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92AA:  'D' 44             LSRA  ; 
                   // FCB   $44                     ; 92AB:  'D' 44             LSRA  ; 
                   // FCB   $20                     ; 92AC:  ' ' 20 2C          BRA   $92DA ; 
                   // FCB   $2C                     ; 92AD:  ',' 2C 58          BGE   $9307 ; 
                   // FCB   $58                     ; 92AE:  'X' 58             ASLB  ; 
                   // FCB   $2B                     ; 92AF:  '+' 2B 2B          BMI   $92DC ; 
                   // FCB   $2B                     ; 92B0:  '+' 2B 04          BMI   $92B6 ; 
                   // FCB   $04                     ; 92B1:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92B2:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92B3:  'D' 44             LSRA  ; 
                   // FCB   $41                     ; 92B4:  'A' 41             Invalid ; 
                   // FCB   $20                     ; 92B5:  ' ' 20 2C          BRA   $92E3 ; 
                   // FCB   $2C                     ; 92B6:  ',' 2C 2D          BGE   $92E5 ; 
                   // FCB   $2D                     ; 92B7:  '-' 2D 58          BLT   $9311 ; 
                   // FCB   $58                     ; 92B8:  'X' 58             ASLB  ; 
                   // FCB   $04                     ; 92B9:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92BA:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92BB:  'D' 44             LSRA  ; 
                   // FCB   $44                     ; 92BC:  'D' 44             LSRA  ; 
                   // FCB   $20                     ; 92BD:  ' ' 20 2C          BRA   $92EB ; 
                   // FCB   $2C                     ; 92BE:  ',' 2C 2D          BGE   $92ED ; 
                   // FCB   $2D                     ; 92BF:  '-' 2D 2D          BLT   $92EE ; 
                   // FCB   $2D                     ; 92C0:  '-' 2D 58          BLT   $931A ; 
                   // FCB   $58                     ; 92C1:  'X' 58             ASLB  ; 
                   // FCB   $04                     ; 92C2:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92C3:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92C4:  'D' 44             LSRA  ; 
                   // FCB   $41                     ; 92C5:  'A' 41             Invalid ; 
                   // FCB   $20                     ; 92C6:  ' ' 20 41          BRA   $9309 ; 
                   // FCB   $41                     ; 92C7:  'A' 41             Invalid ; 
                   // FCB   $2C                     ; 92C8:  ',' 2C 58          BGE   $9322 ; 
                   // FCB   $58                     ; 92C9:  'X' 58             ASLB  ; 
                   // FCB   $04                     ; 92CA:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92CB:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92CC:  'D' 44             LSRA  ; 
                   // FCB   $41                     ; 92CD:  'A' 41             Invalid ; 
                   // FCB   $20                     ; 92CE:  ' ' 20 42          BRA   $9312 ; 
                   // FCB   $42                     ; 92CF:  'B' 42             Invalid ; 
                   // FCB   $2C                     ; 92D0:  ',' 2C 59          BGE   $932B ; 
                   // FCB   $59                     ; 92D1:  'Y' 59             ROLB  ; 
                   // FCB   $04                     ; 92D2:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92D3:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92D4:  'D' 44             LSRA  ; 
                   // FCB   $44                     ; 92D5:  'D' 44             LSRA  ; 
                   // FCB   $20                     ; 92D6:  ' ' 20 44          BRA   $931C ; 
                   // FCB   $44                     ; 92D7:  'D' 44             LSRA  ; 
                   // FCB   $2C                     ; 92D8:  ',' 2C 58          BGE   $9332 ; 
                   // FCB   $58                     ; 92D9:  'X' 58             ASLB  ; 
                   // FCB   $04                     ; 92DA:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92DB:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92DC:  'D' 44             LSRA  ; 
                   // FCB   $44                     ; 92DD:  'D' 44             LSRA  ; 
                   // FCB   $20                     ; 92DE:  ' ' 20 6E          BRA   $934E ; 
                   // FCB   $6E                     ; 92DF:  'n' 6E 31          JMP   -15,Y ; 
                   // FCB   $31                     ; 92E0:  '1' 31 36          LEAY  -10,Y ; 
                   // FCB   $36                     ; 92E1:  '6' 36 2C          PSHU  Y,DP,B ; 
                   // FCB   $2C                     ; 92E2:  ',' 2C 58          BGE   $933C ; 
                   // FCB   $58                     ; 92E3:  'X' 58             ASLB  ; 
                   // FCB   $04                     ; 92E4:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92E5:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92E6:  'D' 44             LSRA  ; 
                   // FCB   $41                     ; 92E7:  'A' 41             Invalid ; 
                   // FCB   $20                     ; 92E8:  ' ' 20 6E          BRA   $9358 ; 
                   // FCB   $6E                     ; 92E9:  'n' 6E 38          JMP   -8,Y ; 
                   // FCB   $38                     ; 92EA:  '8' 38             Invalid ; 
                   // FCB   $2C                     ; 92EB:  ',' 2C 50          BGE   $933D ; 
                   // FCB   $50                     ; 92EC:  'P' 50             NEGB  ; 
                   // FCB   $43                     ; 92ED:  'C' 43             COMA  ; 
                   // FCB   $04                     ; 92EE:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92EF:  'L' 4C             INCA  ; 
                   // FCB   $44                     ; 92F0:  'D' 44             LSRA  ; 
                   // FCB   $44                     ; 92F1:  'D' 44             LSRA  ; 
                   // FCB   $20                     ; 92F2:  ' ' 20 6E          BRA   $9362 ; 
                   // FCB   $6E                     ; 92F3:  'n' 6E 31          JMP   -15,Y ; 
                   // FCB   $31                     ; 92F4:  '1' 31 36          LEAY  -10,Y ; 
                   // FCB   $36                     ; 92F5:  '6' 36 2C          PSHU  Y,DP,B ; 
                   // FCB   $2C                     ; 92F6:  ',' 2C 50          BGE   $9348 ; 
                   // FCB   $50                     ; 92F7:  'P' 50             NEGB  ; 
                   // FCB   $43                     ; 92F8:  'C' 43             COMA  ; 
                   // FCB   $04                     ; 92F9:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 92FA:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 92FB:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 92FC:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 92FD:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 92FE:  ' ' 20 5B          BRA   $935B ; 
                   // FCB   $5B                     ; 92FF:  '[' 5B             Invalid ; 
                   // FCB   $6E                     ; 9300:  'n' 6E 38          JMP   -8,Y ; 
                   // FCB   $38                     ; 9301:  '8' 38             Invalid ; 
                   // FCB   $2C                     ; 9302:  ',' 2C 58          BGE   $935C ; 
                   // FCB   $58                     ; 9303:  'X' 58             ASLB  ; 
                   // FCB   $5D                     ; 9304:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 9305:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 9306:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 9307:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 9308:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 9309:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 930A:  ' ' 20 5B          BRA   $9367 ; 
                   // FCB   $5B                     ; 930B:  '[' 5B             Invalid ; 
                   // FCB   $2C                     ; 930C:  ',' 2C 58          BGE   $9366 ; 
                   // FCB   $58                     ; 930D:  'X' 58             ASLB  ; 
                   // FCB   $2B                     ; 930E:  '+' 2B 2B          BMI   $933B ; 
                   // FCB   $2B                     ; 930F:  '+' 2B 5D          BMI   $936E ; 
                   // FCB   $5D                     ; 9310:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 9311:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 9312:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 9313:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 9314:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 9315:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 9316:  ' ' 20 5B          BRA   $9373 ; 
                   // FCB   $5B                     ; 9317:  '[' 5B             Invalid ; 
                   // FCB   $2C                     ; 9318:  ',' 2C 2D          BGE   $9347 ; 
                   // FCB   $2D                     ; 9319:  '-' 2D 2D          BLT   $9348 ; 
                   // FCB   $2D                     ; 931A:  '-' 2D 58          BLT   $9374 ; 
                   // FCB   $58                     ; 931B:  'X' 58             ASLB  ; 
                   // FCB   $5D                     ; 931C:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 931D:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 931E:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 931F:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 9320:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 9321:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 9322:  ' ' 20 5B          BRA   $937F ; 
                   // FCB   $5B                     ; 9323:  '[' 5B             Invalid ; 
                   // FCB   $41                     ; 9324:  'A' 41             Invalid ; 
                   // FCB   $2C                     ; 9325:  ',' 2C 58          BGE   $937F ; 
                   // FCB   $58                     ; 9326:  'X' 58             ASLB  ; 
                   // FCB   $5D                     ; 9327:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 9328:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 9329:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 932A:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 932B:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 932C:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 932D:  ' ' 20 5B          BRA   $938A ; 
                   // FCB   $5B                     ; 932E:  '[' 5B             Invalid ; 
                   // FCB   $42                     ; 932F:  'B' 42             Invalid ; 
                   // FCB   $2C                     ; 9330:  ',' 2C 59          BGE   $938B ; 
                   // FCB   $59                     ; 9331:  'Y' 59             ROLB  ; 
                   // FCB   $5D                     ; 9332:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 9333:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 9334:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 9335:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 9336:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 9337:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 9338:  ' ' 20 5B          BRA   $9395 ; 
                   // FCB   $5B                     ; 9339:  '[' 5B             Invalid ; 
                   // FCB   $44                     ; 933A:  'D' 44             LSRA  ; 
                   // FCB   $2C                     ; 933B:  ',' 2C 58          BGE   $9395 ; 
                   // FCB   $58                     ; 933C:  'X' 58             ASLB  ; 
                   // FCB   $5D                     ; 933D:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 933E:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 933F:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 9340:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 9341:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 9342:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 9343:  ' ' 20 5B          BRA   $93A0 ; 
                   // FCB   $5B                     ; 9344:  '[' 5B             Invalid ; 
                   // FCB   $6E                     ; 9345:  'n' 6E 31          JMP   -15,Y ; 
                   // FCB   $31                     ; 9346:  '1' 31 36          LEAY  -10,Y ; 
                   // FCB   $36                     ; 9347:  '6' 36 2C          PSHU  Y,DP,B ; 
                   // FCB   $2C                     ; 9348:  ',' 2C 58          BGE   $93A2 ; 
                   // FCB   $58                     ; 9349:  'X' 58             ASLB  ; 
                   // FCB   $5D                     ; 934A:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 934B:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 934C:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 934D:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 934E:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 934F:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 9350:  ' ' 20 5B          BRA   $93AD ; 
                   // FCB   $5B                     ; 9351:  '[' 5B             Invalid ; 
                   // FCB   $6E                     ; 9352:  'n' 6E 38          JMP   -8,Y ; 
                   // FCB   $38                     ; 9353:  '8' 38             Invalid ; 
                   // FCB   $2C                     ; 9354:  ',' 2C 50          BGE   $93A6 ; 
                   // FCB   $50                     ; 9355:  'P' 50             NEGB  ; 
                   // FCB   $43                     ; 9356:  'C' 43             COMA  ; 
                   // FCB   $52                     ; 9357:  'R' 52             Invalid ; 
                   // FCB   $5D                     ; 9358:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 9359:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 935A:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 935B:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 935C:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 935D:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 935E:  ' ' 20 5B          BRA   $93BB ; 
                   // FCB   $5B                     ; 935F:  '[' 5B             Invalid ; 
                   // FCB   $6E                     ; 9360:  'n' 6E 31          JMP   -15,Y ; 
                   // FCB   $31                     ; 9361:  '1' 31 36          LEAY  -10,Y ; 
                   // FCB   $36                     ; 9362:  '6' 36 2C          PSHU  Y,DP,B ; 
                   // FCB   $2C                     ; 9363:  ',' 2C 50          BGE   $93B5 ; 
                   // FCB   $50                     ; 9364:  'P' 50             NEGB  ; 
                   // FCB   $43                     ; 9365:  'C' 43             COMA  ; 
                   // FCB   $52                     ; 9366:  'R' 52             Invalid ; 
                   // FCB   $5D                     ; 9367:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 9368:  ' ' 04 4C          LSR   <$4C ; 
                   // FCB   $4C                     ; 9369:  'L' 4C             INCA  ; 
                   // FCB   $45                     ; 936A:  'E' 45             Invalid ; 
                   // FCB   $41                     ; 936B:  'A' 41             Invalid ; 
                   // FCB   $58                     ; 936C:  'X' 58             ASLB  ; 
                   // FCB   $20                     ; 936D:  ' ' 20 5B          BRA   $93CA ; 
                   // FCB   $5B                     ; 936E:  '[' 5B             Invalid ; 
                   // FCB   $61                     ; 936F:  'a' 61             Invalid ; 
                   // FCB   $64                     ; 9370:  'd' 64 64          LSR   4,S ; 
                   // FCB   $64                     ; 9371:  'd' 64 72          LSR   -14,S ; 
                   // FCB   $72                     ; 9372:  'r' 72             Invalid ; 
                   // FCB   $5D                     ; 9373:  ']' 5D             TSTB  ; 
                   // FCB   $04                     ; 9374:  ' ' 04 86          LSR   <$86 ; 
  case 0x9375:
                                                     #ifdef DEBUG
                                      mon("L9375    LDA   #$01                    ; 9375: 86 01         \n");
                                      #endif
    val = (UINT8)0x01; 
    A = val; 
/**/N = val; 
/**/Z = val; 
/**/V = 0; 
    PC = 0x9377; 
    JUMP; // sstep
  case 0x9377:
                                                     #ifdef DEBUG
                                      mon("         STA   $9394                   ; 9377: B7 93 94      \n");
                                      #endif
    val = A; 
/**/Z = val; 
/**/N = val; 
/**/V = 0; 
    wr_mem(0x9394, val); 
    PC = 0x937A; 
    JUMP; // sstep
  case 0x937A:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$9395                  ; 937A: 8E 93 95      \n");
                                      #endif
    X = 0x9395; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x937D; 
    JUMP; // sstep
  case 0x937D:
                                                     #ifdef DEBUG
                                      mon("         JSR   $CD1E                   ; 937D: BD CD 1E      \n");
                                      #endif
    --S;
    wr_mem(S, 0x80); 
    --S;
    wr_mem(S, 0x93); 
    PC = 0xcd1e; 
    JUMP;
  case 0x9380:
                                                     #ifdef DEBUG
                                      mon("L9380    TFR   U,X                     ; 9380: 1F 31         \n");
                                      #endif
    X = U; 
    PC = 0x9382; 
    JUMP; // sstep
  case 0x9382:
                                                     #ifdef DEBUG
                                      mon("         JSR   $CD1E                   ; 9382: BD CD 1E      \n");
                                      #endif
    --S;
    wr_mem(S, 0x85); 
    --S;
    wr_mem(S, 0x93); 
    PC = 0xcd1e; 
    JUMP;
  case 0x9385:
                                                     #ifdef DEBUG
                                      mon("L9385    RTS                           ; 9385: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
  case 0x9386:
                                                     #ifdef DEBUG
                                      mon("L9386    LDA   $9394                   ; 9386: B6 93 94      \n");
                                      #endif
    val = (UINT8)rd_mem(0x9394); 
    A = val; 
    N = val; 
    Z = val; 
    V = 0; 
    PC = 0x9389; 
    JUMP; // sstep
  case 0x9389:
                                                     #ifdef DEBUG
                                      mon("         BNE   $9391                   ; 9389: 26 06         \n");
                                      #endif
    // temp hack to force a flush
    if (Z) {
    PC = 0x9391; 
    JUMP;
    }
    PC = 0x938B; 
    JUMP; // sstep
  case 0x938B:
                                                     #ifdef DEBUG
                                      mon("         LDX   #$93A3                  ; 938B: 8E 93 A3      \n");
                                      #endif
    X = 0x93a3; 
    Z = X; 
    N = (X) >> 8; 
    V = 0; 
    PC = 0x938E; 
    JUMP; // sstep
  case 0x938E:
                                                     #ifdef DEBUG
                                      mon("         JSR   $CD1E                   ; 938E: BD CD 1E      \n");
                                      #endif
    --S;
    wr_mem(S, 0x91); 
    --S;
    wr_mem(S, 0x93); 
    PC = 0xcd1e; 
    JUMP;
  case 0x9391:
                                                     #ifdef DEBUG
                                      mon("L9391    RTS                           ; 9391: 39            \n");
                                      #endif
    PC = rd_mem(S)<<8; 
    S++;
    PC = PC | rd_mem(S); 
    S++;
    JUMP;
    
                   // FCB   $00                     ; 9392:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 9393:  ' ' 00 00          NEG   <$00 ; 
                   // FCB   $00                     ; 9394:  ' ' 00 46          NEG   <$46 ; 
                   // FCB   $46                     ; 9395:  'F' 46             RORA  ; 
                   // FCB   $61                     ; 9396:  'a' 61             Invalid ; 
                   // FCB   $69                     ; 9397:  'i' 69 6C          ROL   12,S ; 
                   // FCB   $6C                     ; 9398:  'l' 6C 65          INC   5,S ; 
                   // FCB   $65                     ; 9399:  'e' 65             Invalid ; 
                   // FCB   $64                     ; 939A:  'd' 64 20          LSR   0,Y ; 
                   // FCB   $20                     ; 939B:  ' ' 20 54          BRA   $93F1 ; 
                   // FCB   $54                     ; 939C:  'T' 54             LSRB  ; 
                   // FCB   $65                     ; 939D:  'e' 65             Invalid ; 
                   // FCB   $73                     ; 939E:  's' 73 74 3A       COM   $743A ; 
                   // FCB   $74                     ; 939F:  't' 74 3A 20       LSR   $3A20 ; 
                   // FCB   $3A                     ; 93A0:  ':' 3A             ABX   ; 
                   // FCB   $20                     ; 93A1:  ' ' 20 04          BRA   $93A7 ; 
                   // FCB   $04                     ; 93A2:  ' ' 04 41          LSR   <$41 ; 
                   // FCB   $41                     ; 93A3:  'A' 41             Invalid ; 
                   // FCB   $6C                     ; 93A4:  'l' 6C 6C          INC   12,S ; 
                   // FCB   $6C                     ; 93A5:  'l' 6C 20          INC   0,Y ; 
                   // FCB   $20                     ; 93A6:  ' ' 20 54          BRA   $93FC ; 
                   // FCB   $54                     ; 93A7:  'T' 54             LSRB  ; 
                   // FCB   $65                     ; 93A8:  'e' 65             Invalid ; 
                   // FCB   $73                     ; 93A9:  's' 73 74 73       COM   $7473 ; 
                   // FCB   $74                     ; 93AA:  't' 74 73 20       LSR   $7320 ; 
                   // FCB   $73                     ; 93AB:  's' 73 20 73       COM   $2073 ; 
                   // FCB   $20                     ; 93AC:  ' ' 20 73          BRA   $9421 ; 
                   // FCB   $73                     ; 93AD:  's' 73 75 63       COM   $7563 ; 
                   // FCB   $75                     ; 93AE:  'u' 75             Invalid ; 
                   // FCB   $63                     ; 93AF:  'c' 63 63          COM   3,S ; 
                   // FCB   $63                     ; 93B0:  'c' 63 65          COM   5,S ; 
                   // FCB   $65                     ; 93B1:  'e' 65             Invalid ; 
                   // FCB   $64                     ; 93B2:  'd' 64 65          LSR   5,S ; 
                   // FCB   $65                     ; 93B3:  'e' 65             Invalid ; 
                   // FCB   $64                     ; 93B4:  'd' 64 04          LSR   4,X ; 
                   // FCB   $04                     ; 93B5:  ' ' 04 00          LSR   <$00 ; 
