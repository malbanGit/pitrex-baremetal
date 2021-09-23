#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <vectrex/vectrexInterface.h>
int v_printStringRasterFont(int8_t x, int8_t y, char* _string, int8_t xSize, int8_t ySize, unsigned char delimiter, unsigned char *rlines[]);

  #ifdef __cplusplus
}
#endif

int vectrexMode = 1;
int T_WIDTH = 40;
int T_HEIGHT = -4;
int MAX_VLINES = 20;
int Y_START = 80;
int MAX_VROWS = 41;

//char linebuffer[MAX_VLINES][MAX_VROWS];
char **linebuffer;
int linePosition=0;

extern unsigned char *alternate_raster5[];
void showVectrexOut()
{
  int y=Y_START;
  for (int l=0;l<MAX_VLINES;l++)
  {
      y-=10;
      if (linebuffer[l][0] == 0) continue;
      v_printStringRasterFont(-120, y, &linebuffer[l][0], T_WIDTH, T_HEIGHT, 0, alternate_raster5);
  }
}

void scrollup()
{
  linebuffer[MAX_VLINES-1][linePosition] = 0;
  for (int l=0;l<MAX_VLINES-1;l++)
  {
    for (int c=0;c<MAX_VROWS;c++)
    {
      linebuffer[l][c] = linebuffer[l+1][c];
      if (linebuffer[l][c] == 0) break;
    }
  }
  
  linebuffer[MAX_VLINES-1][0] = 0;
  linePosition=0;
}
void clearVectrexOut()
{
  for (int l=0;l<MAX_VLINES;l++)
  {
      linebuffer[l][0] = 0;
  }
  linePosition=0;
  showVectrexOut();
  v_WaitRecal();
}

void printToLine(char *text, int wr)
{
  while (*text != 0)
  {
    char c= *text++;
    if (c =='\r') continue;
    if (c =='\n')
    {
      scrollup();
      continue;
    }
    if ((c <' ') || (c>125)) continue;
//    c = toupper(c);
    linebuffer[MAX_VLINES-1][linePosition++] = c;
    if (linePosition == MAX_VROWS-1) 
    {
      scrollup();
      continue;
    }
  }
  linebuffer[MAX_VLINES-1][linePosition] = 0;
  if (wr)
  {
    showVectrexOut();
    v_WaitRecal();
  }
}
void printToLineC(char c, int wr)
{
  if (c ==0) return;
  if (c =='\r') return;
  if (c =='\n')
  {
    scrollup();
    return;
  }
  if ((c <' ') || (c>125)) return;
//  c = toupper(c);
  linebuffer[MAX_VLINES-1][linePosition++] = c;
  if (linePosition == MAX_VROWS-1) 
  {
    scrollup();
  }
  linebuffer[MAX_VLINES-1][linePosition] = 0;
  if (wr)
  {
    showVectrexOut();
    v_WaitRecal();
  }
}

int printBackspace()
{
  if (linePosition==0) return 0;
  linebuffer[MAX_VLINES-1][--linePosition] = 0;
  showVectrexOut();
  v_WaitRecal();
  return 1;
}

