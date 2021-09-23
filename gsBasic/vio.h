#ifdef __cplusplus
extern "C" {
#endif


void printToLine(char *text, int wr);
void printToLineC(char c, int wr);
void showVectrexOut();
void clearVectrexOut();
int printBackspace();

extern int vectrexMode;
extern int T_WIDTH;
extern int T_HEIGHT;
extern int MAX_VLINES;
extern int Y_START;
extern int MAX_VROWS;


#define VPRINT(...) \
do\
{\
  char buf[512]; \
  sprintf(buf, __VA_ARGS__); \
  if (vectrexMode) printToLine((char *)buf, 1);\
  else printf(buf);\
} while (0)

#define FPUTS(s,f) \
do\
{\
  if ((vectrexMode) && (f==stdout)) printToLine((char *)s, 1);\
  else fputs(s,f);\
} while (0)

#define FPUTC(c,f) \
do\
{\
  if ((vectrexMode) && (f==stdout)) printToLineC(c, 1);\
  else {fputc(c,f);}\
} while (0)

#define VFPRINT(f,...) \
do\
{\
  char buf[512]; \
  sprintf(buf, __VA_ARGS__); \
  if ((vectrexMode) && (f==stdout)) printToLine((char *)buf, 1);\
  else fprintf(f,buf);\
} while (0)





#define VPRINT2(...) \
do\
{\
  char buf[512]; \
  sprintf(buf, __VA_ARGS__); \
  if (vectrexMode) printToLine((char *)buf, 0);\
  else printf(buf);\
} while (0)

#define FPUTS2(s,f) \
do\
{\
  if ((vectrexMode) && (f==stdout)) printToLine((char *)s, 0);\
  else fputs(s,f);\
} while (0)

#define FPUTC2(c,f) \
do\
{\
  if ((vectrexMode) && (f==stdout)) printToLineC(c, 0);\
  else {fputc(c,f);}\
} while (0)

#define VFPRINT2(f,...) \
do\
{\
  char buf[512]; \
  sprintf(buf, __VA_ARGS__); \
  if ((vectrexMode) && (f==stdout)) printToLine((char *)buf, 0);\
  else fprintf(f,buf);\
} while (0)













  #ifdef __cplusplus
}
#endif
