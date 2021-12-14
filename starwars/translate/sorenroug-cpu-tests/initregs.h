void setRegs(char a, char b, unsigned x, unsigned y, unsigned u);
void assertWord(unsigned short exp, unsigned short act, char *reg);
void assertByte(unsigned char exp, unsigned char act, char *reg);
void setDP(void);
char getDP(void);
void setCCflag(int exp, int bit);
void assertCC(int exp, int bit);
void assertRegs(char a, char b, unsigned x, unsigned y, unsigned u);
void printRegs(void);
