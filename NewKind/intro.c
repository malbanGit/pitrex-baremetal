/*
 * Elite - The New Kind.
 *
 * Reverse engineered from the BBC disk version of Elite.
 * Additional material by C.J.Pinder.
 *
 * The original Elite code is (C) I.Bell & D.Braben 1984.
 * This version re-engineered in C by C.J.Pinder 1999-2001.
 *
 * email: <christian@newkind.co.uk>
 *
 *
 */
 
 /*
  * intro.c
  *
  * Run the two intro screens.
  * First is a rolling Cobra MkIII.
  * Second is a parade of the various ships.
  *
  */
 
 
#include <stdlib.h>
#include <vectrex/vectrexInterface.h>

#include "config.h"
#include "elite.h"
#include "gfx.h"
#include "vector.h"
#include "shipdata.h"
#include "shipface.h"
#include "threed.h"
#include "space.h"
#include "stars.h"

static int ship_no;
static int show_time;
static int direction;


static int min_dist[NO_OF_SHIPS+1] = {0, 200, 800, 200,   200, 200, 300, 384,   200,
                                  200, 200, 420, 900, 500, 800, 384, 384,
                                  384, 384, 384, 200, 384, 384, 384,   0,
                                  384,   0, 384, 384, 700, 384,   0,   0,
                                  900};


static Matrix intro_ship_matrix;

const signed char EliteTheNewKind[]=
{   (signed char) 171, // count of vectors
    (signed char) 0x64, (signed char) 0xC4, (signed char) 0x67, (signed char) 0xC4, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0xC4, (signed char) 0x67, (signed char) 0xC0, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0xC0, (signed char) 0x64, (signed char) 0xC0, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0xC0, (signed char) 0x64, (signed char) 0xC4, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0xCC, (signed char) 0x67, (signed char) 0xCC, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0xCC, (signed char) 0x67, (signed char) 0xC8, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0xC8, (signed char) 0x64, (signed char) 0xC8, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0xC8, (signed char) 0x64, (signed char) 0xCC, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0xD4, (signed char) 0x64, (signed char) 0xD0, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0xD0, (signed char) 0x67, (signed char) 0xD0, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0xD0, (signed char) 0x67, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0xD4, (signed char) 0x64, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0x59, (signed char) 0xCF, (signed char) 0x59, (signed char) 0xCC, // y0, x0, y1, x1
    (signed char) 0x59, (signed char) 0xCC, (signed char) 0x52, (signed char) 0xCC, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xCC, (signed char) 0x52, (signed char) 0xCA, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xCA, (signed char) 0x59, (signed char) 0xCA, // y0, x0, y1, x1
    (signed char) 0x59, (signed char) 0xCA, (signed char) 0x59, (signed char) 0xC8, // y0, x0, y1, x1
    (signed char) 0x59, (signed char) 0xC8, (signed char) 0x5A, (signed char) 0xC8, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xC8, (signed char) 0x5A, (signed char) 0xCF, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xCF, (signed char) 0x59, (signed char) 0xCF, // y0, x0, y1, x1
    (signed char) 0x57, (signed char) 0xD4, (signed char) 0x57, (signed char) 0xD7, // y0, x0, y1, x1
    (signed char) 0x57, (signed char) 0xD7, (signed char) 0x5A, (signed char) 0xD7, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xD7, (signed char) 0x5A, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xD9, (signed char) 0x52, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xD9, (signed char) 0x52, (signed char) 0xD7, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xD7, (signed char) 0x55, (signed char) 0xD7, // y0, x0, y1, x1
    (signed char) 0x55, (signed char) 0xD7, (signed char) 0x55, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0x55, (signed char) 0xD4, (signed char) 0x52, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xD4, (signed char) 0x52, (signed char) 0xD2, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xD2, (signed char) 0x5A, (signed char) 0xD2, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xD2, (signed char) 0x5A, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xD4, (signed char) 0x57, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0x59, (signed char) 0xE3, (signed char) 0x5A, (signed char) 0xE3, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xE3, (signed char) 0x5A, (signed char) 0xDC, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xDC, (signed char) 0x52, (signed char) 0xDC, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xDC, (signed char) 0x52, (signed char) 0xE3, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xE3, (signed char) 0x53, (signed char) 0xE3, // y0, x0, y1, x1
    (signed char) 0x53, (signed char) 0xE3, (signed char) 0x54, (signed char) 0xDE, // y0, x0, y1, x1
    (signed char) 0x54, (signed char) 0xDE, (signed char) 0x55, (signed char) 0xDE, // y0, x0, y1, x1
    (signed char) 0x55, (signed char) 0xDE, (signed char) 0x56, (signed char) 0xE2, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0xE2, (signed char) 0x56, (signed char) 0xE2, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0xE2, (signed char) 0x57, (signed char) 0xDE, // y0, x0, y1, x1
    (signed char) 0x57, (signed char) 0xDE, (signed char) 0x58, (signed char) 0xDE, // y0, x0, y1, x1
    (signed char) 0x58, (signed char) 0xDE, (signed char) 0x59, (signed char) 0xE3, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xEC, (signed char) 0x5A, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xEA, (signed char) 0x52, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xEA, (signed char) 0x52, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xEC, (signed char) 0x56, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0xEC, (signed char) 0x52, (signed char) 0xEF, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xEF, (signed char) 0x52, (signed char) 0xF1, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xF1, (signed char) 0x5A, (signed char) 0xF1, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xF1, (signed char) 0x5A, (signed char) 0xEF, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xEF, (signed char) 0x55, (signed char) 0xEF, // y0, x0, y1, x1
    (signed char) 0x55, (signed char) 0xEF, (signed char) 0x5A, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0x60, (signed char) 0xED, (signed char) 0x60, (signed char) 0xF8, // y0, x0, y1, x1
    (signed char) 0x60, (signed char) 0xF8, (signed char) 0x5E, (signed char) 0xF8, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0xF8, (signed char) 0x5E, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0xEA, (signed char) 0x6C, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0xEA, (signed char) 0x6C, (signed char) 0xED, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0xED, (signed char) 0x60, (signed char) 0xED, // y0, x0, y1, x1
    (signed char) 0x6A, (signed char) 0xE5, (signed char) 0x6A, (signed char) 0xDB, // y0, x0, y1, x1
    (signed char) 0x6A, (signed char) 0xDB, (signed char) 0x67, (signed char) 0xDB, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0xDB, (signed char) 0x66, (signed char) 0xE2, // y0, x0, y1, x1
    (signed char) 0x66, (signed char) 0xE2, (signed char) 0x64, (signed char) 0xE2, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0xE2, (signed char) 0x64, (signed char) 0xDB, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0xDB, (signed char) 0x60, (signed char) 0xDB, // y0, x0, y1, x1
    (signed char) 0x60, (signed char) 0xDB, (signed char) 0x60, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x60, (signed char) 0xE5, (signed char) 0x5E, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0xE5, (signed char) 0x5E, (signed char) 0xD7, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0xD7, (signed char) 0x6C, (signed char) 0xD7, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0xD7, (signed char) 0x6C, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0xE5, (signed char) 0x6A, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x59, (signed char) 0xFB, (signed char) 0x5A, (signed char) 0xFB, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xFB, (signed char) 0x5A, (signed char) 0xF4, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xF4, (signed char) 0x52, (signed char) 0xF4, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xF4, (signed char) 0x52, (signed char) 0xFB, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xFB, (signed char) 0x53, (signed char) 0xFB, // y0, x0, y1, x1
    (signed char) 0x53, (signed char) 0xFB, (signed char) 0x54, (signed char) 0xF6, // y0, x0, y1, x1
    (signed char) 0x54, (signed char) 0xF6, (signed char) 0x55, (signed char) 0xF6, // y0, x0, y1, x1
    (signed char) 0x55, (signed char) 0xF6, (signed char) 0x56, (signed char) 0xFA, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0xFA, (signed char) 0x56, (signed char) 0xFA, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0xFA, (signed char) 0x57, (signed char) 0xF6, // y0, x0, y1, x1
    (signed char) 0x57, (signed char) 0xF6, (signed char) 0x58, (signed char) 0xF6, // y0, x0, y1, x1
    (signed char) 0x58, (signed char) 0xF6, (signed char) 0x59, (signed char) 0xFB, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xFE, (signed char) 0x5A, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0xFD, (signed char) 0x52, (signed char) 0xFF, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0xFF, (signed char) 0x52, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x00, (signed char) 0x57, (signed char) 0x01, // y0, x0, y1, x1
    (signed char) 0x57, (signed char) 0x01, (signed char) 0x52, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x02, (signed char) 0x52, (signed char) 0x04, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x04, (signed char) 0x5A, (signed char) 0x07, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x07, (signed char) 0x5A, (signed char) 0x05, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x05, (signed char) 0x56, (signed char) 0x04, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0x04, (signed char) 0x5A, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x02, (signed char) 0x5A, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x00, (signed char) 0x56, (signed char) 0xFF, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0xFF, (signed char) 0x5A, (signed char) 0xFE, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0xFE, (signed char) 0x6C, (signed char) 0xFE, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0xFE, (signed char) 0x6C, (signed char) 0xFB, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0xFB, (signed char) 0x5E, (signed char) 0xFB, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0xFB, (signed char) 0x5E, (signed char) 0xFE, // y0, x0, y1, x1
    (signed char) 0x57, (signed char) 0x10, (signed char) 0x5A, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x10, (signed char) 0x5A, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x0E, (signed char) 0x52, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x0E, (signed char) 0x52, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x10, (signed char) 0x55, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0x55, (signed char) 0x10, (signed char) 0x55, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x55, (signed char) 0x12, (signed char) 0x52, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x14, (signed char) 0x52, (signed char) 0x16, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x16, (signed char) 0x56, (signed char) 0x13, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0x13, (signed char) 0x5A, (signed char) 0x16, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x16, (signed char) 0x5A, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x14, (signed char) 0x57, (signed char) 0x11, // y0, x0, y1, x1
    (signed char) 0x57, (signed char) 0x11, (signed char) 0x57, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x1B, (signed char) 0x5A, (signed char) 0x1B, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x1B, (signed char) 0x5A, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x18, (signed char) 0x52, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x18, (signed char) 0x52, (signed char) 0x1B, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x21, (signed char) 0x55, (signed char) 0x24, // y0, x0, y1, x1
    (signed char) 0x55, (signed char) 0x24, (signed char) 0x5A, (signed char) 0x24, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x24, (signed char) 0x5A, (signed char) 0x26, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x26, (signed char) 0x52, (signed char) 0x26, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x26, (signed char) 0x52, (signed char) 0x24, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x24, (signed char) 0x56, (signed char) 0x21, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0x21, (signed char) 0x52, (signed char) 0x21, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x21, (signed char) 0x52, (signed char) 0x1F, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x1F, (signed char) 0x5A, (signed char) 0x1F, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x1F, (signed char) 0x5A, (signed char) 0x21, // y0, x0, y1, x1
    (signed char) 0x59, (signed char) 0x2C, (signed char) 0x58, (signed char) 0x2E, // y0, x0, y1, x1
    (signed char) 0x58, (signed char) 0x2E, (signed char) 0x56, (signed char) 0x30, // y0, x0, y1, x1
    (signed char) 0x56, (signed char) 0x30, (signed char) 0x54, (signed char) 0x2E, // y0, x0, y1, x1
    (signed char) 0x54, (signed char) 0x2E, (signed char) 0x53, (signed char) 0x2C, // y0, x0, y1, x1
    (signed char) 0x53, (signed char) 0x2C, (signed char) 0x59, (signed char) 0x2C, // y0, x0, y1, x1
    (signed char) 0x58, (signed char) 0x31, (signed char) 0x5A, (signed char) 0x2F, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x2F, (signed char) 0x5A, (signed char) 0x2A, // y0, x0, y1, x1
    (signed char) 0x5A, (signed char) 0x2A, (signed char) 0x52, (signed char) 0x2A, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x2A, (signed char) 0x52, (signed char) 0x2F, // y0, x0, y1, x1
    (signed char) 0x52, (signed char) 0x2F, (signed char) 0x54, (signed char) 0x31, // y0, x0, y1, x1
    (signed char) 0x54, (signed char) 0x31, (signed char) 0x58, (signed char) 0x31, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0x3A, (signed char) 0x64, (signed char) 0x36, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0x36, (signed char) 0x67, (signed char) 0x36, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0x36, (signed char) 0x67, (signed char) 0x3A, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0x3A, (signed char) 0x64, (signed char) 0x3A, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0x32, (signed char) 0x64, (signed char) 0x2E, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0x2E, (signed char) 0x67, (signed char) 0x2E, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0x2E, (signed char) 0x67, (signed char) 0x32, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0x32, (signed char) 0x64, (signed char) 0x32, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0x2A, (signed char) 0x64, (signed char) 0x26, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0x26, (signed char) 0x67, (signed char) 0x26, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0x26, (signed char) 0x67, (signed char) 0x2A, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0x2A, (signed char) 0x64, (signed char) 0x2A, // y0, x0, y1, x1
    (signed char) 0x6A, (signed char) 0x22, (signed char) 0x6A, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x6A, (signed char) 0x18, (signed char) 0x67, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x67, (signed char) 0x18, (signed char) 0x66, (signed char) 0x1F, // y0, x0, y1, x1
    (signed char) 0x66, (signed char) 0x1F, (signed char) 0x64, (signed char) 0x1F, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0x1F, (signed char) 0x64, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x64, (signed char) 0x18, (signed char) 0x60, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x60, (signed char) 0x18, (signed char) 0x60, (signed char) 0x22, // y0, x0, y1, x1
    (signed char) 0x60, (signed char) 0x22, (signed char) 0x5E, (signed char) 0x22, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0x22, (signed char) 0x5E, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0x14, (signed char) 0x6C, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0x14, (signed char) 0x6C, (signed char) 0x22, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0x22, (signed char) 0x6A, (signed char) 0x22, // y0, x0, y1, x1
    (signed char) 0x6A, (signed char) 0x10, (signed char) 0x6A, (signed char) 0x0A, // y0, x0, y1, x1
    (signed char) 0x6A, (signed char) 0x0A, (signed char) 0x5E, (signed char) 0x0A, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0x0A, (signed char) 0x5E, (signed char) 0x07, // y0, x0, y1, x1
    (signed char) 0x5E, (signed char) 0x07, (signed char) 0x6A, (signed char) 0x07, // y0, x0, y1, x1
    (signed char) 0x6A, (signed char) 0x07, (signed char) 0x6A, (signed char) 0x01, // y0, x0, y1, x1
    (signed char) 0x6A, (signed char) 0x01, (signed char) 0x6C, (signed char) 0x01, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0x01, (signed char) 0x6C, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0x6C, (signed char) 0x10, (signed char) 0x6A, (signed char) 0x10, // y0, x0, y1, x1
};

static int iTimer = 0;
static int iText = 0;
static int iBrightness = 0;
static int iDirection = 0;
void initialise_intro1 (void)
{
    clear_universe();
    set_init_matrix (intro_ship_matrix);
    add_new_ship (SHIP_COBRA3, 0, 0, 4500, intro_ship_matrix, -127, -127);

    iTimer = 0;
    iText = 0;
    iBrightness = 0x10;
    iDirection = 0;
  
}


void initialise_intro2 (void)
{
    ship_no = 0;
    show_time = 0;
    direction = 100;

    clear_universe();
    create_new_stars();
    set_init_matrix (intro_ship_matrix);
    add_new_ship (1, 0, 0, 5000, intro_ship_matrix, -127, -127);
}



void update_intro1 (void)
{
    universe[0].location.z -= 100;

    if (universe[0].location.z < 384)
        universe[0].location.z = 384;

    gfx_clear_display();

    flight_roll = 1;
    update_universe();
    
    gfx_draw_sprite(IMG_ELITE_TXT, -1, 10);

    if (iDirection == 0)
    {
      iBrightness+=5;
      if (iBrightness >= 0x6f) iDirection = 1;
    }
    else if (iDirection == 1)
    {
      iBrightness-=5;
      if (iBrightness <= 0x10) 
      {
        iDirection = 0;
        iText++;
        if (iText == 3) iText = 0;
      }
    }
    
    if (iText == 0)
      gfx_display_centre_text_b (380, "Original Game (C) I.Bell & D.Braben.", 120, iBrightness);
    else if (iText == 1)
      gfx_display_centre_text_b (380, "Re-engineered by C.J.Pinder.", 120, iBrightness);
    else if (iText == 2)
      gfx_display_centre_text_b (380, "Load New Commander (Y/N)?", 140, iBrightness);
    
    v_displayAbsolutList_8(EliteTheNewKind, 0x20);
    
}


void update_intro2 (void)
{
    show_time++;

    if ((show_time >= 140) && (direction < 0))
        direction = -direction;

    universe[0].location.z += direction;

    if (universe[0].location.z < min_dist[ship_no])
        universe[0].location.z = min_dist[ship_no];

    if (universe[0].location.z > 4500)
    {
        do
        {
            ship_no++;
            if (ship_no > NO_OF_SHIPS)
                ship_no = 1;
        } while (min_dist[ship_no] == 0);

        show_time = 0;
        direction = -100;

        ship_count[universe[0].type] = 0;
        universe[0].type = 0;       

        add_new_ship (ship_no, 0, 0, 4500, intro_ship_matrix, -127, -127);
    }


    gfx_clear_display();
    update_starfield();
    update_universe();

    gfx_draw_sprite (IMG_ELITE_TXT, -1, 10);

    gfx_display_centre_text (360, "Press Fire or Space.", 140, GFX_COL_GOLD);
    gfx_display_centre_text (330, ship_list[ship_no]->name, 120, GFX_COL_WHITE);

    v_displayAbsolutList_8(EliteTheNewKind,0x20);
}

