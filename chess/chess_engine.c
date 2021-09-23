#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>


#ifdef FREESTANDING

#include <vectrex/vectrexInterface.h>

#define CLOCKS_PER_SEC 1000000
#define clock_t unsigned int

#define clock __clock

unsigned int __clock()
{
  unsigned int val;
  CCNT0(val);
  return val;
}

#else
#include <time.h>
#endif



#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5
#define EPS_SQUARE 6
#define EMPTY 7
#define WHITE 0
#define BLACK 1
#define false 0

#define VALUE_PAWN 100
#define VALUE_KNIGHT 310
#define VALUE_BISHOP 320
#define VALUE_ROOK 500
#define VALUE_QUEEN 900
#define VALUE_KING 10000

#define MATE 10000  // equal value of King, losing King==mate

#define COL(pos) ((pos)&7)
#define ROW(pos) (((unsigned)pos)>>3)

// For move generation
#define MOVE_TYPE_NONE 0
#define MOVE_TYPE_NORMAL 1
#define MOVE_TYPE_CASTLE 2
#define MOVE_TYPE_PAWN_TWO 3
#define MOVE_TYPE_EPS 4
#define MOVE_TYPE_PROMOTION_TO_QUEEN 5
#define MOVE_TYPE_PROMOTION_TO_ROOK 6
#define MOVE_TYPE_PROMOTION_TO_BISHOP 7
#define MOVE_TYPE_PROMOTION_TO_KNIGHT 8

// Some useful squares
#define A1 56
#define B1 57
#define C1 58
#define D1 59
#define E1 60
#define F1 61
#define G1 62
#define H1 63
#define A8 0
#define B8 1
#define C8 2
#define D8 3
#define E8 4
#define F8 5
#define G8 6
#define H8 7

// Board representation
int piece[64];
int color[64];

int initialise_piece[64] = {
    ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
    PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
    ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK
};

int initialise_color[64] = {
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
};

int computer_move_from_x = 0;
int computer_move_from_y = 0;
int computer_move_to_x = 0;
int computer_move_to_y = 0;

int side;               // Side to move, value = BLACK or WHITE
int computer_side;
int max_depth;          // Max depth to ab_search

typedef struct tag_MOVE {
    int from;
    int dest;
    int type;
} MOVE;

typedef struct tag_HIST {
    MOVE m;
    int castle;
    int cap;
} HIST;

HIST hist[6000];

// For castle rights we use a bitfield, like in TSCP
//
// 0001 -> White can short castle
// 0010 -> White can long castle
// 0100 -> Black can short castle
// 1000 -> Black can long castle
//
// 15 = 1111 = 1*2^3 + 1*2^2 + 1*2^1 + 1*2^0

int castle_rights = 15;        // At start position all castle types ar available

// This mask is applied like this
//
// castle &= castle_mask[from] & castle_mask[dest]
//
// When from and dest are whatever pieces, then nothing happens, otherwise
// the values are chosen in such a way that if vg the white king moves
// to F1 then
//
// castle = castle & (12 & 15)
// 1111 & (1100 & 1111) == 1111 & 1100 == 1100
//
// and white's lost all its castle rights

int castle_mask[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

int hdp;                // Current move order
int nodes;              // Count all visited nodes when searching
int ply;                // ply of ab_search
int count_evaluations;
int count_checks;
int count_MakeMove;
int count_quies_calls;
int count_cap_calls;

// The values of the pieces in centipawns
int value_piece[6] = { VALUE_PAWN, VALUE_KNIGHT, VALUE_BISHOP, VALUE_ROOK, VALUE_QUEEN, VALUE_KING };

// Piece Square Tables
// When evaluating the position we'll add a bonus (or malus) to each piece
// depending on the very square where it's placed. Vg, a knight in d4 will
// be given an extra +15, whilst a knight in a1 will be penalized with -40.
// This simple idea allows the engine to make more sensible moves
int pst_pawn[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 15, 15, 0, 0, 0,
    0, 0, 0, 10, 10, 0, 0, 0,
    0, 0, 0, 5, 5, 0, 0, 0,
    0, 0, 0, -25, -25, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

int pst_knight[64] = {
    -40, -25, -25, -25, -25, -25, -25, -40,
    -30, 0, 0, 0, 0, 0, 0, -30,
    -30, 0, 0, 0, 0, 0, 0, -30,
    -30, 0, 0, 15, 15, 0, 0, -30,
    -30, 0, 0, 15, 15, 0, 0, -30,
    -30, 0, 10, 0, 0, 10, 0, -30,
    -30, 0, 0, 5, 5, 0, 0, -30,
    -40, -30, -25, -25, -25, -25, -30, -40
};

int pst_bishop[64] = {
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -10, 0, 5, 0, 0, 5, 0, -10,
    -10, 0, 0, 10, 10, 0, 0, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 0, 5, 0, 0, 5, 0, -10,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -10, -20, -20, -20, -20, -20, -20, -10
};

int pst_rook[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    10, 10, 10, 10, 10, 10, 10, 10,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 5, 5, 0, 0, 0
};

int pst_king[64] = {
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    10, 15, -15, -15, -15, -15, 15, 10
};

// The flip array is used to calculate the piece/square
// values for BLACKS pieces, without needing to write the
// arrays for them (idea taken from TSCP).
// The piece square value of a white pawn is pst_pawn[sq]
// and the value of a black pawn is pst_pawn[flip[sq]]

int flip[64] = {
  56, 57, 58, 59, 60, 61, 62, 63,
  48, 49, 50, 51, 52, 53, 54, 55,
  40, 41, 42, 43, 44, 45, 46, 47,
  32, 33, 34, 35, 36, 37, 38, 39,
  24, 25, 26, 27, 28, 29, 30, 31,
  16, 17, 18, 19, 20, 21, 22, 23,
  8, 9, 10, 11, 12, 13, 14, 15,
  0, 1, 2, 3, 4, 5, 6, 7
};

// MARK: - Move generations

static int is_in_check(int current_side);
static int is_attacked(int current_side, int k);
static int quiescent(int alpha, int beta);

static void generate_push(int from, int dest, int type, MOVE * pBuf, int *pMCount) {
    MOVE move;
    move.from = from;
    move.dest = dest;
    move.type = type;
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;
}

static void generate_push_normal(int from, int dest, MOVE * pBuf, int *pMCount) {
    generate_push(from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
}

// Special cases for Pawn. Pawn can promote
static void generate_push_pawn(int from, int dest, MOVE * pBuf, int *pMCount) {
    // The 7 and 56 are to limit pawns to the 2nd through 7th ranks, which
    // means this isn't a promotion, i.e., a normal pawn move
    if (piece[dest] == EPS_SQUARE) {
        generate_push (from, dest, MOVE_TYPE_EPS, pBuf, pMCount);
    }
    else if (dest > 7 && dest < 56) {
        generate_push (from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
    }
    else {
        // otherwise it's a promotion
        generate_push (from, dest, MOVE_TYPE_PROMOTION_TO_QUEEN, pBuf, pMCount);
        generate_push (from, dest, MOVE_TYPE_PROMOTION_TO_ROOK, pBuf, pMCount);
        generate_push (from, dest, MOVE_TYPE_PROMOTION_TO_BISHOP, pBuf, pMCount);
        generate_push (from, dest, MOVE_TYPE_PROMOTION_TO_KNIGHT, pBuf, pMCount);
    }
}

// When a pawn moves two squares then appears the possibility of the en passanta capture
static void generate_push_pawn_two (int from, int dest, MOVE * pBuf, int *pMCount) {
    generate_push(from, dest, MOVE_TYPE_PAWN_TWO, pBuf, pMCount);
}

// Special cases for King
static void generate_push_king(int from, int dest, MOVE * pBuf, int *pMCount) {
    // Is it a castle?
    if (from == E1 && (dest == G1 || dest == C1)) {
        // this is a white castle
        generate_push (from, dest, MOVE_TYPE_CASTLE, pBuf, pMCount);
    }
    else if (from == E8 && (dest == G8 || dest == C8)) {
        // this is a black castle
        generate_push (from, dest, MOVE_TYPE_CASTLE, pBuf, pMCount);
    }
    else {
        // otherwise it's a normal king's move
        generate_push (from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
    }
}

// Generate all moves of current_side to move and push them to pBuf, and return number of moves
static int generate_moves(int current_side, MOVE * pBuf) {
    int i;
    int k;
    int y;
    int row;
    int col;
    int movecount = 0;

    for (i = 0; i < 64; i++) {
        if (color[i] == current_side) {
            switch (piece[i]) {
                case PAWN:
                    col = COL (i);
                    row = ROW (i);
                   
                    if (current_side == BLACK) {
                        // Pawn advances one square. We use generate_push_pawn because it can be a promotion
                        if (color[i + 8] == EMPTY)
                            generate_push_pawn (i, i + 8, pBuf, &movecount);
                    
                        // Pawn advances two squares
                        if (row == 1 && color[i + 8] == EMPTY && color[i + 16] == EMPTY)
                            generate_push_pawn_two (i, i + 16, pBuf, &movecount);
                        
                        // Pawn captures and it can be a promotion
                        if (col && color[i + 7] == WHITE)
                            generate_push_pawn (i, i + 7, pBuf, &movecount);
                        
                        // Pawn captures and can be a promotion
                        if (col < 7 && color[i + 9] == WHITE)
                            generate_push_pawn (i, i + 9, pBuf, &movecount);
                        
                        // For en passant capture. Pawn captures and it can be a promotion
                        if (col && piece[i + 7] == EPS_SQUARE)
                            generate_push_pawn (i, i + 7, pBuf, &movecount);
                    
                        // Pawn captures and can be a promotion
                        if (col < 7 && piece[i + 9] == EPS_SQUARE)
                            generate_push_pawn (i, i + 9, pBuf, &movecount);
                    }
                    else {
                        if (color[i - 8] == EMPTY)
                            generate_push_pawn (i, i - 8, pBuf, &movecount);
                    
                        // Pawn moves 2 squares
                        if (row == 6 && color[i - 8] == EMPTY && color[i - 16] == EMPTY)
                            generate_push_pawn_two (i, i - 16, pBuf, &movecount);
                    
                        // For captures
                        if (col && color[i - 9] == BLACK)
                            generate_push_pawn (i, i - 9, pBuf, &movecount);
                    
                        if (col < 7 && color[i - 7] == BLACK)
                            generate_push_pawn (i, i - 7, pBuf, &movecount);
                    
                        // For en passant capture
                        if (col && piece[i - 9] == EPS_SQUARE)
                            generate_push_pawn (i, i - 9, pBuf, &movecount);
                        if (col < 7 && piece[i - 7] == EPS_SQUARE)
                            generate_push_pawn (i, i - 7, pBuf, &movecount);
                    }
                    break;
                    
                // == BISHOP+ROOK
                case QUEEN:
                case BISHOP:
                    for (y = i - 9; y >= 0 && COL (y) != 7; y -= 9) {
                        // go left up
                        if (color[y] != current_side)
                            generate_push_normal (i, y, pBuf, &movecount);
                    
                        if (color[y] != EMPTY)
                            break;
                    }
                    
                    for (y = i - 7; y >= 0 && COL (y) != 0; y -= 7) {
                        // go right up
                        if (color[y] != current_side)
                            generate_push_normal (i, y, pBuf, &movecount);
                    
                        if (color[y] != EMPTY)
                            break;
                    }
                    
                    for (y = i + 9; y < 64 && COL (y) != 0; y += 9) {
                        // go right down
                        if (color[y] != current_side)
                            generate_push_normal (i, y, pBuf, &movecount);
                    
                        if (color[y] != EMPTY)
                            break;
                    }
                    
                    for (y = i + 7; y < 64 && COL (y) != 7; y += 7) {
                        // go left down
                        if (color[y] != current_side)
                            generate_push_normal (i, y, pBuf, &movecount);
                    
                        if (color[y] != EMPTY)
                            break;
                    }
                    
                    // In the case of the bishop we're done
                    if (piece[i] == BISHOP)
                        break;

                // FALL THROUGH FOR QUEEN
                case ROOK:
                    col = COL (i);
                    // go left
                    for (k = i - col, y = i - 1; y >= k; y--) {
                        if (color[y] != current_side)
                            generate_push_normal (i, y, pBuf, &movecount);
                    
                        if (color[y] != EMPTY)
                            break;
                    }
                    
                    // go right
                    for (k = i - col + 7, y = i + 1; y <= k; y++) {
                        if (color[y] != current_side)
                            generate_push_normal (i, y, pBuf, &movecount);
                    
                        if (color[y] != EMPTY)
                            break;
                    }
                    
                    // go up
                    for (y = i - 8; y >= 0; y -= 8) {
                        if (color[y] != current_side)
                            generate_push_normal (i, y, pBuf, &movecount);
                    
                        if (color[y] != EMPTY)
                            break;
                    }
                    
                    // go down
                    for (y = i + 8; y < 64; y += 8) {
                        if (color[y] != current_side)
                            generate_push_normal (i, y, pBuf, &movecount);
                    
                        if (color[y] != EMPTY)
                            break;
                    }
                    break;

                case KNIGHT:
                    col = COL (i);
                    y = i - 6;
                    if (y >= 0 && col < 6 && color[y] != current_side)
                        generate_push_normal (i, y, pBuf, &movecount);
                    
                    y = i - 10;
                    if (y >= 0 && col > 1 && color[y] != current_side)
                        generate_push_normal (i, y, pBuf, &movecount);
                    
                    y = i - 15;
                    if (y >= 0 && col < 7 && color[y] != current_side)
                        generate_push_normal (i, y, pBuf, &movecount);
                    
                    y = i - 17;
                    if (y >= 0 && col > 0 && color[y] != current_side)
                        generate_push_normal (i, y, pBuf, &movecount);
                    
                    y = i + 6;
                    if (y < 64 && col > 1 && color[y] != current_side)
                        generate_push_normal (i, y, pBuf, &movecount);
                    
                    y = i + 10;
                    if (y < 64 && col < 6 && color[y] != current_side)
                        generate_push_normal (i, y, pBuf, &movecount);
                    
                    y = i + 15;
                    if (y < 64 && col > 0 && color[y] != current_side)
                        generate_push_normal (i, y, pBuf, &movecount);
                    
                    y = i + 17;
                    if (y < 64 && col < 7 && color[y] != current_side)
                        generate_push_normal (i, y, pBuf, &movecount);
                break;

                case KING:
                    // the column and rank checks are to make sure it is on the board, The 'normal' moves
                    // left
                    col = COL (i);
                    if (col && color[i - 1] != current_side)
                        generate_push_king (i, i - 1, pBuf, &movecount);
                    
                    // right
                    if (col < 7 && color[i + 1] != current_side)
                        generate_push_king (i, i + 1, pBuf, &movecount);
                    
                    // up
                    if (i > 7 && color[i - 8] != current_side)
                        generate_push_king (i, i - 8, pBuf, &movecount);
                    
                    // down
                    if (i < 56 && color[i + 8] != current_side)
                        generate_push_king (i, i + 8, pBuf, &movecount);
                    
                    // left up
                    if (col && i > 7 && color[i - 9] != current_side)
                        generate_push_king (i, i - 9, pBuf, &movecount);
                    
                    // right up
                    if (col < 7 && i > 7 && color[i - 7] != current_side)
                        generate_push_king (i, i - 7, pBuf, &movecount);
                    
                    // left down
                    if (col && i < 56 && color[i + 7] != current_side)
                        generate_push_king (i, i + 7, pBuf, &movecount);
                    
                    // right down
                    if (col < 7 && i < 56 && color[i + 9] != current_side)
                        generate_push_king (i, i + 9, pBuf, &movecount);

                    // The castle moves
                    if (current_side == WHITE) {
                        // Can white short castle?
                        if (castle_rights & 1) {
                            // If white can castle the white king has to be in square 60
                            if (col && color[i + 1] == EMPTY && color[i + 2] == EMPTY && !is_in_check (current_side) && !is_attacked (current_side, i + 1)) {
                                // The king goes 2 sq to the left
                                generate_push_king (i, i + 2, pBuf, &movecount);
                            }
                        }

                        // Can white long castle?
                        if (castle_rights & 2) {
                            if (col && color[i - 1] == EMPTY && color[i - 2] == EMPTY && color[i - 3] == EMPTY && !is_in_check (current_side) && !is_attacked (current_side, i - 1)) {
                                /* The king goes 2 sq to the left */
                                generate_push_king (i, i - 2, pBuf, &movecount);
                            }
                        }
                    }
                    else if (current_side == BLACK) {
                        // Can black short castle?
                        if (castle_rights & 4) {
                            // If white can castle the white king has to be in square 60
                            if (col && color[i + 1] == EMPTY && color[i + 2] == EMPTY && piece[i + 3] == ROOK && !is_in_check (current_side) && !is_attacked (current_side, i + 1)) {
                                // The king goes 2 sq to the left
                                generate_push_king (i, i + 2, pBuf, &movecount);
                            }
                        }
                    
                        // Can black long castle?
                        if (castle_rights & 8) {
                            if (col &&  color[i - 1] == EMPTY &&    color[i - 2] == EMPTY && color[i - 3] == EMPTY && piece[i - 4] == ROOK && !is_in_check (current_side) && !is_attacked (current_side, i - 1)) {
                                // The king goes 2 sq to the left
                                generate_push_king (i, i - 2, pBuf, &movecount);
                            }
                        }
                    }

                    break;
                }
        }
    }

    return movecount;
}

// Gen all captures of current_side to move and push them to pBuf, return number of moves
// It's necesary at least ir order to use quiescent in the ab_search
static int generate_captures(int current_side, MOVE * pBuf) {
    int i;
    int k;
    int y;
    int row;
    int col;
    int capscount;
    int xside;
    xside = (WHITE + BLACK) - current_side;
    capscount = 0;

    for (i = 0; i < 64; i++) {
        if (color[i] == current_side) {
            switch (piece[i])   {
                case PAWN:
                    col = COL (i);
                    row = ROW (i);
                    if (current_side == BLACK) {
                        // This isn't a capture, but it's necesary in order to * not oversee promotions
                        if (row > 7 && color[i + 8] == EMPTY)
                            /* Pawn advances one square.
                             * We use generate_push_pawn because it can be a promotion */
                            generate_push_pawn (i, i + 8, pBuf, &capscount);
                    
                        if (col && color[i + 7] == WHITE)
                            /* Pawn captures and it can be a promotion */
                            generate_push_pawn (i, i + 7, pBuf, &capscount);
                    
                        if (col < 7 && color[i + 9] == WHITE)
                            /* Pawn captures and can be a promotion */
                            generate_push_pawn (i, i + 9, pBuf, &capscount);
                    
                        /* For en passant capture */
                        if (col && piece[i + 7] == EPS_SQUARE)
                            /* Pawn captures and it can be a promotion */
                            generate_push_pawn (i, i + 7, pBuf, &capscount);
                    
                        if (col < 7 && piece[i + 9] == EPS_SQUARE)
                            /* Pawn captures and can be a promotion */
                            generate_push_pawn (i, i + 9, pBuf, &capscount);
                    }
                    else if (current_side == WHITE) {
                        if (row < 2 && color[i - 8] == EMPTY)
                            /* This isn't a capture, but it's necesary in order to * not oversee promotions */
                            generate_push_pawn (i, i - 8, pBuf, &capscount);
                    
                        /* For captures */
                        if (col && color[i - 9] == BLACK)
                            generate_push_pawn (i, i - 9, pBuf, &capscount);
                    
                        if (col < 7 && color[i - 7] == BLACK)
                            generate_push_pawn (i, i - 7, pBuf, &capscount);
                    
                        /* For en passant capture */
                        if (col && piece[i - 9] == EPS_SQUARE)
                            generate_push_pawn (i, i - 9, pBuf, &capscount);
                    
                        if (col < 7 && piece[i - 7] == EPS_SQUARE)
                            generate_push_pawn (i, i - 7, pBuf, &capscount);
                    }
                    break;

                case KNIGHT:
                    col = COL (i);
                    y = i - 6;
                    if (y >= 0 && col < 6 && color[y] == xside)
                    generate_push_normal (i, y, pBuf, &capscount);
                    y = i - 10;
                    if (y >= 0 && col > 1 && color[y] == xside)
                    generate_push_normal (i, y, pBuf, &capscount);
                    y = i - 15;
                    if (y >= 0 && col < 7 && color[y] == xside)
                    generate_push_normal (i, y, pBuf, &capscount);
                    y = i - 17;
                    if (y >= 0 && col > 0 && color[y] == xside)
                    generate_push_normal (i, y, pBuf, &capscount);
                    y = i + 6;
                    if (y < 64 && col > 1 && color[y] == xside)
                    generate_push_normal (i, y, pBuf, &capscount);
                    y = i + 10;
                    if (y < 64 && col < 6 && color[y] == xside)
                    generate_push_normal (i, y, pBuf, &capscount);
                    y = i + 15;
                    if (y < 64 && col > 0 && color[y] == xside)
                    generate_push_normal (i, y, pBuf, &capscount);
                    y = i + 17;
                    if (y < 64 && col < 7 && color[y] == xside)
                    generate_push_normal (i, y, pBuf, &capscount);
                    break;

                case QUEEN:        /* == BISHOP+ROOK */
                case BISHOP:
                    for (y = i - 9; y >= 0 && COL (y) != 7; y -= 9)
                    {            /* go left up */
                    if (color[y] != EMPTY)
                    {
                    if (color[y] != current_side)
                    generate_push_normal (i, y, pBuf, &capscount);
                    break;
                    }
                    }
                    for (y = i - 7; y >= 0 && COL (y) != 0; y -= 7)
                    {            /* go right up */
                    if (color[y] != EMPTY)
                    {
                    if (color[y] != current_side)
                    generate_push_normal (i, y, pBuf, &capscount);
                    break;
                    }
                    }
                    for (y = i + 9; y < 64 && COL (y) != 0; y += 9)
                    {            /* go right down */
                    if (color[y] != EMPTY)
                    {
                    if (color[y] != current_side)
                    generate_push_normal (i, y, pBuf, &capscount);
                    break;
                    }
                    }
                    for (y = i + 7; y < 64 && COL (y) != 7; y += 7)
                    {            /* go left down */
                    if (color[y] != EMPTY)
                    {
                    if (color[y] != current_side)
                    generate_push_normal (i, y, pBuf, &capscount);
                    break;
                    }
                    }
                    if (piece[i] == BISHOP)    /* In the case of the bishop we're done */
                    break;

                /* FALL THROUGH FOR QUEEN {I meant to do that!} ;-) */
                case ROOK:
                    col = COL (i);
                    for (k = i - col, y = i - 1; y >= k; y--)
                    {            /* go left */
                    if (color[y] != EMPTY)
                    {
                    if (color[y] != current_side)
                    generate_push_normal (i, y, pBuf, &capscount);
                    break;
                    }
                    }
                    for (k = i - col + 7, y = i + 1; y <= k; y++)
                    {            /* go right */
                    if (color[y] != EMPTY)
                    {
                    if (color[y] != current_side)
                    generate_push_normal (i, y, pBuf, &capscount);
                    break;
                    }
                    }
                    for (y = i - 8; y >= 0; y -= 8)
                    {            /* go up */
                    if (color[y] != EMPTY)
                    {
                    if (color[y] != current_side)
                    generate_push_normal (i, y, pBuf, &capscount);
                    break;
                    }
                    }
                    for (y = i + 8; y < 64; y += 8)
                    {            /* go down */
                    if (color[y] != EMPTY)
                    {
                    if (color[y] != current_side)
                    generate_push_normal (i, y, pBuf, &capscount);
                    break;
                    }
                    }
                    break;

                case KING:
                    // the column and rank checks are to make sure it is on the board
                    col = COL (i);
                    if (col && color[i - 1] == xside)
                    generate_push_king (i, i - 1, pBuf, &capscount);    /* left */
                    if (col < 7 && color[i + 1] == xside)
                    generate_push_king (i, i + 1, pBuf, &capscount);    /* right */
                    if (i > 7 && color[i - 8] == xside)
                    generate_push_king (i, i - 8, pBuf, &capscount);    /* up */
                    if (i < 56 && color[i + 8] == xside)
                    generate_push_king (i, i + 8, pBuf, &capscount);    /* down */
                    if (col && i > 7 && color[i - 9] == xside)
                    generate_push_king (i, i - 9, pBuf, &capscount);    /* left up */
                    if (col < 7 && i > 7 && color[i - 7] == xside)
                    generate_push_king (i, i - 7, pBuf, &capscount);    /* right up */
                    if (col && i < 56 && color[i + 7] == xside)
                    generate_push_king (i, i + 7, pBuf, &capscount);    /* left down */
                    if (col < 7 && i < 56 && color[i + 9] == xside)
                    generate_push_king (i, i + 9, pBuf, &capscount);    /* right down */
                    break;
                }
        }
    }

    return capscount;
}

// Evaluation for current position
static int eval() {
    count_evaluations++;

    int i;
    int score = 0;

  /* Check all the squares searching for the pieces */
  for (i = 0; i < 64; i++)
    {
      if (color[i] == WHITE)
    {
      /* In the current square, add the material
       * value of the piece */
      score += value_piece[piece[i]];

      /* Now we add to the evaluation the value of the
       * piece square tables */
      switch (piece[i])
        {
        case PAWN:
          score += pst_pawn[i];
          break;
        case KNIGHT:
          score += pst_knight[i];
          break;
        case BISHOP:
          score += pst_bishop[i];
          break;
        case ROOK:
          score += pst_rook[i];
          break;
        case KING:
          score += pst_king[i];
          break;
        }
    }
      /* Now the evaluation for black: note the change of
         the sign in the score */
      else if (color[i] == BLACK)
    {
      score -= value_piece[piece[i]];

      switch (piece[i])
        {
        case PAWN:
          score -= pst_pawn[flip[i]];
          break;
        case KNIGHT:
          score -= pst_knight[flip[i]];
          break;
        case BISHOP:
          score -= pst_bishop[flip[i]];
          break;
        case ROOK:
          score -= pst_rook[flip[i]];
          break;
        case KING:
          score -= pst_king[flip[i]];
          break;
        }
    }
    }

  /* Finally we return the score, taking into account the side to move */
  if (side == WHITE)
    return score;
  return -score;
}

// MARK: - Chess logic

static int is_in_check (int current_side) {
  int k;            /* The square where the king is placed */

  /* Find the King of the side to move */
  for (k = 0; k < 64; k++)
    if ((piece[k] == KING) && color[k] == current_side)
      break;

  /* Use is_attacked in order to know if current_side is under check */
  return is_attacked (current_side, k);
}

// Returns 1 if square k is attacked by current_side, 0 otherwise.
// Necesary, v.g., to check castle rules (if king goes from e1 to g1, f1 can't be attacked by an enemy piece)
int is_attacked(int current_side, int k) {
  int h;
  int y;
  int row;
  int col;
  int xside;
  xside = (WHITE + BLACK) - current_side;

  row = ROW (k);
  col = COL (k);

  /* Check Knight attack */
  if (col > 0 && row > 1 && color[k - 17] == xside && piece[k - 17] == KNIGHT)
    return 1;
  if (col < 7 && row > 1 && color[k - 15] == xside && piece[k - 15] == KNIGHT)
    return 1;
  if (col > 1 && row > 0 && color[k - 10] == xside && piece[k - 10] == KNIGHT)
    return 1;
  if (col < 6 && row > 0 && color[k - 6] == xside && piece[k - 6] == KNIGHT)
    return 1;
  if (col > 1 && row < 7 && color[k + 6] == xside && piece[k + 6] == KNIGHT)
    return 1;
  if (col < 6 && row < 7 && color[k + 10] == xside && piece[k + 10] == KNIGHT)
    return 1;
  if (col > 0 && row < 6 && color[k + 15] == xside && piece[k + 15] == KNIGHT)
    return 1;
  if (col < 7 && row < 6 && color[k + 17] == xside && piece[k + 17] == KNIGHT)
    return 1;

  /* Check horizontal and vertical lines for attacking of Queen, Rook, King go down */
  y = k + 8;
  if (y < 64)
    {
      if (color[y] == xside && (piece[y] == KING || piece[y] == QUEEN
                || piece[y] == ROOK))
    return 1;
      if (piece[y] == EMPTY || piece[y] == EPS_SQUARE)
    for (y += 8; y < 64; y += 8)
      {
        if (color[y] == xside && (piece[y] == QUEEN || piece[y] == ROOK))
          return 1;
        if (piece[y] != EMPTY && piece[y] != EPS_SQUARE)
          break;
      }
    }
  /* go left */
  y = k - 1;
  h = k - col;
  if (y >= h)
    {
      if (color[y] == xside && (piece[y] == KING || piece[y] == QUEEN
                || piece[y] == ROOK))
    return 1;
      if (piece[y] == EMPTY || piece[y] == EPS_SQUARE)
    for (y--; y >= h; y--)
      {
        if (color[y] == xside && (piece[y] == QUEEN || piece[y] == ROOK))
          return 1;
        if (piece[y] != EMPTY && piece[y] != EPS_SQUARE)
          break;
      }
    }
  /* go right */
  y = k + 1;
  h = k - col + 7;
  if (y <= h)
    {
      if (color[y] == xside && (piece[y] == KING || piece[y] == QUEEN
                || piece[y] == ROOK))
    return 1;
      if (piece[y] == EMPTY || piece[y] == EPS_SQUARE)
    for (y++; y <= h; y++)
      {
        if (color[y] == xside && (piece[y] == QUEEN || piece[y] == ROOK))
          return 1;
        if (piece[y] != EMPTY && piece[y] != EPS_SQUARE)
          break;
      }
    }
  /* go up */
  y = k - 8;
  if (y >= 0)
    {
      if (color[y] == xside && (piece[y] == KING || piece[y] == QUEEN
                || piece[y] == ROOK))
    return 1;
      if (piece[y] == EMPTY || piece[y] == EPS_SQUARE)
    for (y -= 8; y >= 0; y -= 8)
      {
        if (color[y] == xside && (piece[y] == QUEEN || piece[y] == ROOK))
          return 1;
        if (piece[y] != EMPTY && piece[y] != EPS_SQUARE)
          break;
      }
    }
  /* Check diagonal lines for attacking of Queen, Bishop, King, Pawn */
  /* go right down */
  y = k + 9;
  if (y < 64 && COL (y) != 0)
    {
      if (color[y] == xside)
    {
      if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
        return 1;
      if (current_side == BLACK && piece[y] == PAWN)
        return 1;
    }
      if (piece[y] == EMPTY || piece[y] == EPS_SQUARE)
    for (y += 9; y < 64 && COL (y) != 0; y += 9)
      {
        if (color[y] == xside && (piece[y] == QUEEN || piece[y]
                      == BISHOP))
          return 1;
        if (piece[y] != EMPTY && piece[y] != EPS_SQUARE)
          break;
      }
    }
  /* go left down */
  y = k + 7;
  if (y < 64 && COL (y) != 7)
    {
      if (color[y] == xside)
    {
      if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
        return 1;
      if (current_side == BLACK && piece[y] == PAWN)
        return 1;
    }
      if (piece[y] == EMPTY || piece[y] == EPS_SQUARE)
    for (y += 7; y < 64 && COL (y) != 7; y += 7)
      {
        if (color[y] == xside && (piece[y] == QUEEN || piece[y]
                      == BISHOP))
          return 1;
        if (piece[y] != EMPTY && piece[y] != EPS_SQUARE)
          break;

      }
    }
  /* go left up */
  y = k - 9;
  if (y >= 0 && COL (y) != 7)
    {
      if (color[y] == xside)
    {
      if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
        return 1;
      if (current_side == WHITE && piece[y] == PAWN)
        return 1;
    }
      if (piece[y] == EMPTY || piece[y] == EPS_SQUARE)
    for (y -= 9; y >= 0 && COL (y) != 7; y -= 9)
      {
        if (color[y] == xside && (piece[y] == QUEEN || piece[y]
                      == BISHOP))
          return 1;
        if (piece[y] != EMPTY && piece[y] != EPS_SQUARE)
          break;

      }
    }
  /* go right up */
  y = k - 7;
  if (y >= 0 && COL (y) != 0)
    {
      if (color[y] == xside)
    {
      if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
        return 1;
      if (current_side == WHITE && piece[y] == PAWN)
        return 1;
    }
      if (piece[y] == EMPTY || piece[y] == EPS_SQUARE)
    for (y -= 7; y >= 0 && COL (y) != 0; y -= 7)
      {
        if (color[y] == xside && (piece[y] == QUEEN || piece[y]
                      == BISHOP))
          return 1;
        if (piece[y] != EMPTY && piece[y] != EPS_SQUARE)
          break;
      }
    }
  return 0;
}

int make_move(MOVE m) {
  int r;
  int i;
  
  count_MakeMove++;

  hist[hdp].m = m;
  hist[hdp].cap = piece[m.dest];
  hist[hdp].castle = castle_rights;

  piece[m.dest] = piece[m.from];
  color[m.dest] = color[m.from];
  piece[m.from] = EMPTY;
  color[m.from] = EMPTY;

  // en pasant capture
  if (m.type == MOVE_TYPE_EPS)
    {
      if (side == WHITE)
    {
      piece[m.dest + 8] = EMPTY;
      color[m.dest + 8] = EMPTY;
    }
      else
    {
      piece[m.dest - 8] = EMPTY;
      color[m.dest - 8] = EMPTY;
    }
    }

  // Remove possible eps piece, remaining from former move */
  if (hist[hdp - 1].m.type == MOVE_TYPE_PAWN_TWO)
    {
      for (i = 16; i <= 23; i++)
    {
      if (piece[i] == EPS_SQUARE)
        {
          piece[i] = EMPTY;
          break;
        }
    }
      for (i = 40; i <= 47; i++)
    {
      if (piece[i] == EPS_SQUARE)
        {
          piece[i] = EMPTY;
          break;
        }
    }
    }

  /* Add the eps square when a pawn moves two squares */
  if (m.type == MOVE_TYPE_PAWN_TWO)
    {
      if (side == BLACK)
    {
      piece[m.from + 8] = EPS_SQUARE;
      color[m.from + 8] = EMPTY;
    }
      else if (side == WHITE)
    {
      piece[m.from - 8] = EPS_SQUARE;
      color[m.from - 8] = EMPTY;
    }
    }

  /* Once the move is done we check either this is a promotion */
  if (m.type >= MOVE_TYPE_PROMOTION_TO_QUEEN)
    {
      switch (m.type)
    {
    case MOVE_TYPE_PROMOTION_TO_QUEEN:
      piece[m.dest] = QUEEN;
      break;

    case MOVE_TYPE_PROMOTION_TO_ROOK:
      piece[m.dest] = ROOK;
      break;

    case MOVE_TYPE_PROMOTION_TO_BISHOP:
      piece[m.dest] = BISHOP;
      break;

    case MOVE_TYPE_PROMOTION_TO_KNIGHT:
      piece[m.dest] = KNIGHT;
      break;

    default:
      puts ("Impossible to get here...");
      assert (false);
    }
    }

  if (m.type == MOVE_TYPE_CASTLE)
    {
      if (m.dest == G1)
    {
      /* h1-h8 becomes empty */
      piece[m.from + 3] = EMPTY;
      color[m.from + 3] = EMPTY;
      /* rook to f1-f8 */
      piece[m.from + 1] = ROOK;
      color[m.from + 1] = WHITE;
    }
      else if (m.dest == C1)
    {
      /* h1-h8 becomes empty */
      piece[m.from - 4] = EMPTY;
      color[m.from - 4] = EMPTY;
      /* rook to f1-f8 */
      piece[m.from - 1] = ROOK;
      color[m.from - 1] = WHITE;
    }
      else if (m.dest == G8)
    {
      /* h1-h8 becomes empty */
      piece[m.from + 3] = EMPTY;
      color[m.from + 3] = EMPTY;
      /* rook to f1-f8 */
      piece[m.from + 1] = ROOK;
      color[m.from + 1] = BLACK;
    }
      else if (m.dest == C8)
    {
      /* h1-h8 becomes empty */
      piece[m.from - 4] = EMPTY;
      color[m.from - 4] = EMPTY;
      /* rook to f1-f8 */
      piece[m.from - 1] = ROOK;
      color[m.from - 1] = BLACK;
    }
    }

  /* Update ply and hdp */
  ply++;
  hdp++;

  /* Update the castle rights */
  castle_rights &= castle_mask[m.from] & castle_mask[m.dest];

  /* Checking if after making the move we're in check */
  r = !is_in_check (side);

  /* After making move, give turn to opponent */
  side = (WHITE + BLACK) - side;

  return r;
}

// Undo what make_move did
void take_back() {
  side = (WHITE + BLACK) - side;
  hdp--;
  ply--;
  piece[hist[hdp].m.from] = piece[hist[hdp].m.dest];
  piece[hist[hdp].m.dest] = hist[hdp].cap;
  color[hist[hdp].m.from] = side;

  /* Update castle rights */
  castle_rights = hist[hdp].castle;

  /* Return the captured material */
  if (hist[hdp].cap != EMPTY && hist[hdp].cap != EPS_SQUARE)
    {
      color[hist[hdp].m.dest] = (WHITE + BLACK) - side;
    }
  else
    {
      color[hist[hdp].m.dest] = EMPTY;
    }

  /* Promotion */
  if (hist[hdp].m.type >= MOVE_TYPE_PROMOTION_TO_QUEEN)
    {
      piece[hist[hdp].m.from] = PAWN;
    }

  /* If pawn moved two squares in the former move, we have to restore
   * the eps square */
  if (hist[hdp - 1].m.type == MOVE_TYPE_PAWN_TWO)
    {
      if (side == WHITE)
    {
      piece[hist[hdp - 1].m.dest - 8] = EPS_SQUARE;
    }
      else if (side == BLACK)
    {
      piece[hist[hdp - 1].m.dest + 8] = EPS_SQUARE;
    }
    }

  /* To remove the eps square after unmaking a pawn
   * moving two squares*/
  if (hist[hdp].m.type == MOVE_TYPE_PAWN_TWO)
    {
      if (side == WHITE)
    {
      piece[hist[hdp].m.from - 8] = EMPTY;
      color[hist[hdp].m.from - 8] = EMPTY;
    }
      else
    {
      piece[hist[hdp].m.from + 8] = EMPTY;
      color[hist[hdp].m.from + 8] = EMPTY;
    }
    }

  /* Unmaking an en pasant capture */
  if (hist[hdp].m.type == MOVE_TYPE_EPS)
    {
      if (side == WHITE)
    {
      /* The pawn */
      piece[hist[hdp].m.dest + 8] = PAWN;
      color[hist[hdp].m.dest + 8] = BLACK;
      /* The eps square */
      piece[hist[hdp].m.dest] = EPS_SQUARE;

    }
      else
    {
      /* The pawn */
      piece[hist[hdp].m.dest - 8] = PAWN;
      color[hist[hdp].m.dest - 8] = WHITE;
      /* The eps square */
      piece[hist[hdp].m.dest] = EPS_SQUARE;
    }
    }

  /* Undo Castle: return rook to its original square */
  if (hist[hdp].m.type == MOVE_TYPE_CASTLE)
    {
      /* Take the tower to its poriginal place */
      if (hist[hdp].m.dest == G1 && side == WHITE)
    {
      piece[H1] = ROOK;
      color[H1] = WHITE;
      piece[F1] = EMPTY;
      color[F1] = EMPTY;
    }
      else if (hist[hdp].m.dest == C1 && side == WHITE)
    {
      piece[A1] = ROOK;
      color[A1] = WHITE;
      piece[D1] = EMPTY;
      color[D1] = EMPTY;
    }
      else if (hist[hdp].m.dest == G8 && side == BLACK)
    {
      piece[H8] = ROOK;
      color[H8] = BLACK;
      piece[F8] = EMPTY;
      color[F8] = EMPTY;
    }
      else if (hist[hdp].m.dest == C8 && side == BLACK)
    {
      piece[A8] = ROOK;
      color[A8] = BLACK;
      piece[D8] = EMPTY;
      color[D8] = EMPTY;
    }
    }
}

int quiescent(int alpha, int beta) {
    int i;
    int capscnt;
    int stand_pat;
    int score;
    MOVE cBuf[200];

    count_quies_calls++;

    stand_pat = eval();

    if (stand_pat >= beta)
        return beta;
    
    if (alpha < stand_pat)
        alpha = stand_pat;

    capscnt = generate_captures(side, cBuf);

    count_cap_calls++;

    for (i = 0; i < capscnt; ++i) {
        if (!make_move (cBuf[i])) {
            take_back();
            continue;
        }
        
        score = -quiescent(-beta, -alpha);
        take_back();
    
        if (score >= beta)
            return beta;
    
        if (score > alpha)
            alpha = score;
    }
    
    return alpha;
}

// Returns the number of posible positions to a given depth.
// Based on the perft function on Danasah
unsigned long long perft(int depth) {
    int i;
    int movecnt;
    unsigned long long nodes = 0;

    if (!depth)
        return 1;

    MOVE moveBuf[200];

    movecnt = generate_moves(side, moveBuf);

    for (i = 0; i < movecnt; ++i) {
        if (!make_move(moveBuf[i])) {
            take_back();
            continue;
        }

        nodes += perft(depth - 1);
        take_back();
    }

    return nodes;
}

// Alphabeta based main ab_search function (todo: move ordering)
int ab_search(int alpha, int beta, int depth, MOVE * pBestMove) {
    int i;
    int value;
    int havemove;
    int movecnt;

    MOVE moveBuf[200];
    MOVE tmpMove;

    havemove = 0;
    pBestMove->type = MOVE_TYPE_NONE;

    movecnt = generate_moves(side, moveBuf);
    assert (movecnt < 201);

    for (i = 0; i < movecnt; ++i) {
        if (!make_move(moveBuf[i])) {
            take_back();
    
            continue;
        }

        havemove = 1;

        if (depth - 1 > 0) {
            value = -ab_search(-beta, -alpha, depth - 1, &tmpMove);
        }
        else {
            value = -quiescent(-beta, -alpha);
        }

        take_back();

        if (value > alpha) {
            if (value >= beta) {
                return beta;
            }
    
            alpha = value;
            *pBestMove = moveBuf[i];
        }
    }

    if (!havemove) {
        if (is_in_check (side))
            return -MATE + ply;
        else
            return 0;
    }

    return alpha;
}

// Returns the move of the computer
MOVE computer_think(int depth) {
    MOVE m;
    int score;
    double knps;

    ply = 0;
    nodes = 0;
    count_evaluations = 0;
    count_MakeMove = 0;
    count_quies_calls = 0;
    count_cap_calls = 0;

    clock_t start;
    clock_t stop;
    double t = 0.0;

    start = clock ();
    assert (start != -1);

    score = ab_search (-MATE, MATE, depth, &m);

    stop = clock ();
    t = (double) (stop - start) / CLOCKS_PER_SEC;
    knps = ((double) count_quies_calls / t) / 1000.;

    //double ratio_Qsearc_Capcalls = (double) count_quies_calls / (double) count_cap_calls;
    double decimal_score = ((double) score) / 100.;
    
    if (side == BLACK) {
        decimal_score = -decimal_score;
    }

    //printf("Search result: move = %c%d%c%d; depth = %d, score = %.2f, time = %.2fs knps = %.2f\n- countCapCalls = %d\n- countQSearch = %d\n- moves made = %d\n- ratio_Qsearc_Capcalls = %.2f\n",
    //       'a' + COL (m.from), 8 - ROW (m.from), 'a' + COL (m.dest), 8 - ROW (m.dest), depth, decimal_score, t, knps, count_cap_calls, count_quies_calls, count_MakeMove, ratio_Qsearc_Capcalls);

    computer_move_from_x = COL(m.from);
    computer_move_from_y = ROW(m.from);
    computer_move_to_x = COL(m.dest);
    computer_move_to_y = ROW(m.dest);

    printf("Search: depth = %d, score = %.2f, time = %.2fs\n", depth, decimal_score, t);
    printf("Moved from %d,%d to %d,%d\n", computer_move_from_x, computer_move_from_y, computer_move_to_x, computer_move_to_y);

    return m;
}

// MARK: - Helper functions

void print_board() {
    char pieceName[] = "PNBRQKpnbrqk";
    int i;

    for (i = 0; i < 64; i++) {
        if ((i & 7) == 0) {
            printf("   +---+---+---+---+---+---+---+---+\n");
            
            if (i <= 56) {
                printf(" %d |", 8 - (((unsigned) i) >> 3));
            }
        }

        if (piece[i] == EMPTY && ((((unsigned) i) >> 3) % 2 == 0 && i % 2 == 0))
            printf("   |");
        else if (piece[i] == EMPTY && ((((unsigned) i) >> 3) % 2 != 0 && i % 2 != 0))
            printf("   |");
        else if (piece[i] == EMPTY)
            printf("   |");
        else if (piece[i] == EPS_SQUARE)
            printf(" * |");
        else {
            if (color[i] == WHITE)
                printf(" %c |", pieceName[piece[i]]);
            else
                printf("<%c>|", pieceName[piece[i] + 6]);
        }
    
        if ((i & 7) == 7)
            printf("\n");
    }
    
    printf("   +---+---+---+---+---+---+---+---+\n     a   b   c   d   e   f   g   h\n");
}

static void start_game() {
    int i;

    for (i = 0; i < 64; ++i) {
        piece[i] = initialise_piece[i];
        color[i] = initialise_color[i];
    }

    side = WHITE;
    computer_side = BLACK;
    hdp = 0;
    castle_rights = 15;
}

// MARK: - Public functions

int chess_user_move(int from, int dest) {
    ply = 0;

    MOVE moveBuf[200];
    int movecnt = generate_moves(side, moveBuf);
    int i = 0;
    
    for (i = 0; i < movecnt; i++) {
        if (moveBuf[i].from == from && moveBuf[i].dest == dest) {
            if (piece[from] == PAWN && (dest < 8 || dest > 55)) {
                puts("promoting to a McGuffin..., I'll give you a queen");
                moveBuf[i].type = MOVE_TYPE_PROMOTION_TO_QUEEN;
            }

            if (!make_move(moveBuf[i])) {
                take_back();
                printf("Illegal move.\n");
                
                return -1;
            }
            
            return 0;
        }
    }
    
    // Move not found
    return -2;
}

void chess_computer_move() {
    if (side == computer_side) {
        MOVE bestMove = computer_think(max_depth);
        make_move(bestMove);

        print_board();
    }
}

int chess_piece_at(int row, int col) {
    int index = row * 8 + col;
    
    int figure = piece[index];
    int figure_color = color[index];
    
    if (figure_color == EMPTY) {
        return 0;
    }
    
    figure++;
    if (figure_color == BLACK) {
        figure += 10;
    }
    
    return figure;
}

void chess_last_move(int* from_col, int* from_row, int* to_col, int* to_row) {
    *from_col = computer_move_from_x;
    *from_row = computer_move_from_y;
    *to_col = computer_move_to_x;
    *to_row = computer_move_to_y;
}

void chess_initialize() {
    start_game();

    side = WHITE;
    computer_side = WHITE;

    max_depth = 3;
    hdp = 0;
    
    print_board();
}
