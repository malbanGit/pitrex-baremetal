/*------------------------------------------------------------------
  game.c:

    XINVADERS 3D - 3d Shoot'em up
    Copyright (C) 2000 Don Llopis

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

------------------------------------------------------------------*/
#include "game.h"

#define SW_UPDATE  gv->sw_t += gv->msec
#define SW_PAUSE   gv->sw_save = gv->sw_t
#define SW_UNPAUSE gv->sw_t = gv->sw_save
#define SW_RESET   gv->sw_t = gv->sw_save = 0

static void  Add_obj_2_world ( OBJECT * );
static void  Clear_obj_list ( void );
static void  Sort_obj_list ( void );
static void  Draw_obj_list ( MAT );
static int   Z_compare ( const void *obj0, const void *obj1 );
static float Dist_pt_2_line ( VEC p, VEC l0, VEC l1 );

static void  Draw_vector_font ( int *s[], int x, int y, unsigned int color );

/*------------------------------------------------------------------
 * Variables
 *  
 *
------------------------------------------------------------------*/

GAMEVARS gvars, *gv;
TIMER    gtimer, *gt;

static long 
score_table[ZONE_HEIGHT_MAX] = { 10L, 50L, 100L, 150L, 200L };

/* use a null terminator, so code doesn't have to
	be changed in many places. Don't use \n; assume
	a line break suitable to the particular platform
	should be generated after each line. TBB */
char *game_about_info[] =
{
   "Thanks for playing XInvaders 3D.",
   "XInvaders 3D v1.31, Copyright 2000 Don Llopis",
   "XInvaders 3D comes with ABSOLUTELY NO WARRANTY.",
   "This is free software, and you are welcome to",
   "redistribute it under certain conditions. See",
   "file named COPYING for more information.",
   "Latest version of the game can be found at:",
   "http://www.fiu.edu/~dllopi01/xinv3d.htm",
   "Author can be reached at: dllopi01@fiu.edu",
   NULL
};

char *game_rules_info[] =
{
   "Keys:",
   "SPACE: Fire Missile",
   "Left Arrow: move left",
   "Right Arrow: move right",
   "Up Arrow: move up",
   "Down Arrow: move down",
   "f: Show frame-rate",
   "p: Pause game",
   "q: reset game",
   "ESC: Quit program",
   NULL
};


/* 
 * Not so nice vector-fonts:
 * perhaps one day, when the universe is kinder to me,
 * I'll implement a nice vector-font system. But
 * until the universe lets me go on my trip,
 * the stuff below stays!
 */
static int A[17] = 
{ 
   4,
   0, 0, 24, 0,
   0, 12, 24, 12,
   0, 0, 0, 24,
   24, 0, 24, 24
};

static int C[13] =
{
   3,
   0, 0, 24, 0,
   0, 0, 0, 24,
   0, 24, 24, 24
};

static int D[13] = 
{
   3,
   0, 0, 0, 24,
   0, 0, 24, 12,
   0, 24, 24, 12
};

static int E[17] =
{
   4,
   0, 0, 24, 0,
   0, 12, 24, 12,
   0, 24, 24, 24,
   0, 0, 0, 24

};

static int EXCLAMATION[33] =
{
   8,
   0, 0, 24, 0,
   5, 19, 15, 19,
   0, 0, 5, 19,
   24, 0, 15, 19,
   5, 25, 15, 25,
   5, 30, 15, 30,
   5, 25, 5, 30,
   15, 25, 15, 30
};

static int F[13] =
{
   3,
   0, 0, 0, 24,
   0, 0, 24, 0,
   0, 12, 24, 12
};

static int G[17] =
{
   4, 
   24, 0, 0, 12,
   0, 12, 24, 24,
   24, 24, 24, 12,
   24, 12, 12, 12
};

static int H[13] =
{
   3,
   0, 0, 0, 24,
   0, 12, 24, 12,
   24, 0, 24, 24
};

static int I[13] =
{
   3,
   0, 0, 24, 0,
   0, 24, 24, 24,
   12, 0, 12, 24
};

static int M[17] =
{
   4,
   0, 0, 0, 24,
   0, 0, 12, 12,
   12, 12, 24, 0,
   24, 0, 24, 24
};

static int N[13] =
{
   3,
   0, 0, 0, 24,
   0, 0, 24, 24,
   24, 0, 24, 24
};

static int O[17] =
{
   4,
   0, 0, 24, 0,
   0, 0, 0, 24,
   0, 24, 24, 24,
   24, 0, 24, 24
};


static int P[13] =
{
   3,
   0, 0, 0, 24,
   0, 0, 24, 12,
   24, 12, 0, 12
};

static int R[17] =
{
   4,
   0, 0, 0, 24,
   0, 0, 24, 12,
   0, 12, 24, 12,
   0, 12, 24, 24
};

static int S[13] =
{
   3,
   24, 0, 0, 5,
   0, 5, 24, 15,
   24, 15, 0, 24
};

static int T[9] =
{
   2,
   0, 0, 24, 0,
   12, 0, 12, 24
};

static int U[13] =
{
   3,
   0, 0, 0, 24,
   0, 24, 24, 24,
   24, 24, 24, 0
};

static int V[9] =
{
   2,
   0, 0, 12, 24,
   24, 0, 12, 24
};

static int X[9] =
{
   2,
   0, 0, 24, 24,
   24, 0, 0,24 
};

static int Y[13] =
{
   3,
   0, 0, 12, 12,
   12, 12, 24, 0,
   12, 12, 12, 24
};

static int ZERO[21] =
{
   5,
   0, 0, 24, 24,
   0, 0, 24, 0,
   0, 24, 24, 24,
   0, 0, 0, 24,
   24, 0, 24, 24
};

static int ONE[5] =
{
   1,
   12, 0, 12, 24
};

static int TWO[21] =
{
   5,
   0, 0, 24, 0,
   24, 0, 24, 12,
   24, 12, 0, 12,
   0, 12, 0, 24,
   0, 24, 24, 24
};

static int THREE[17] =
{
   4,
   0, 0, 24, 0, 
   24, 12, 0, 12,
   0, 24, 24, 24,
   24, 0, 24, 24
};
static int FOUR[13] =
{
   3,
   0, 0, 0, 12,
   0, 12, 24, 12,
   24, 0, 24, 24
};

static int FIVE[21] =
{
   5,
   0, 0, 24, 0,
   0, 0, 0, 12,
   0, 12, 24, 12,
   24, 12, 24, 24,
   24, 24, 0, 24
};

static int SIX[17] =
{
   4,
   0, 0, 0, 24,
   0, 24, 24, 24,
   24, 24, 24, 12,
   24, 12, 0, 12 
};

static int SEVEN[9] = 
{
   2, 
   0, 0, 24, 0, 
   24, 0, 0, 24
};
static int EIGHT[21] =
{
   5,
   0, 0, 24, 0,
   0, 12, 24, 12,
   0, 24, 24, 24,
   0, 0, 0, 24,
   24, 0, 24, 24
};

static int NINE[17] =
{
   4,
   0, 0, 24, 0,
   0, 12, 24, 12,
   0, 0, 0, 12,
   24, 0, 24, 24
};

static int *NUMBER[10] =
{
   &ZERO[0],
   &ONE[0],
   &TWO[0],
   &THREE[0],
   &FOUR[0],
   &FIVE[0],
   &SIX[0],
   &SEVEN[0],
   &EIGHT[0],
   &NINE[0]
};

static int *XINVADERS3D_LOGO[] = 
{ 
   &X[0], &I[0], &N[0], &V[0], &A[0], &D[0], &E[0], &R[0], &S[0], 
   &THREE[0], &D[0], NULL
};

static int *GET[] =
{
   &G[0], &E[0], &T[0], NULL
};

static int *READY[] =
{
   &R[0], &E[0], &A[0], &D[0], &Y[0], &EXCLAMATION[0],
   &EXCLAMATION[0], NULL
};

static int *GAME_OVER[] =
{
   &G[0], &A[0], &M[0], &E[0], &O[0], &V[0], &E[0], &R[0], NULL
};

static int *PAUSED[] =
{
   &P[0], &A[0], &U[0], &S[0], &E[0], &D[0], NULL
};

static int *SCORE[] =
{
   &S[0], &C[0], &O[0], &R[0], &E[0], NULL
};

static int *HI_SCORE[] =
{
   &H[0], &I[0], &S[0], &C[0], &O[0], &R[0], &E[0], NULL
};

static int *PRESS[] =
{
   &P[0], &R[0], &E[0], &S[0], &S[0], NULL
};

static int *FIRE[] =
{
   &F[0], &I[0], &R[0], &E[0], NULL
};

static int *TO[] =
{
   &T[0], &O[0], NULL
};

static int *START[] =
{
   &S[0], &T[0], &A[0], &R[0], &T[0], NULL
};

/*================================================================*/

void Game_main ( void )
{
   while ( Handle_events() )
   {
      /* get start-time of current frame */
      gv->msec    = Timer_msec ( gt );
      gv->ftime   = (double)gv->msec/1000L;
      gv->fps     = 1.0 / gv->ftime;
      gv->fadjust = gv->rfps / gv->fps;
      
      (*Game_actionfn)(); 
      
      Update_display (); 
   }
}

/*================================================================*/

void (*Game_actionfn) ();

/*================================================================*/

int Game_init ( float win_width, float win_height )
{
   /* initialize variables and timers */
   gv = &gvars;
   Game_init_vars ( INITGAME );
   Game_init_keys ();
   
   Stars_init ();
   Jumpgate_init ();
   Camera_init ( win_width, win_height, 0.785398163 );

   gt = &gtimer;
   Timer_init( gt );

   /* setup stopwatch timer and run menu */
   SW_RESET;
   Game_actionfn = Game_menu;

   return TRUE;
}

/*================================================================*/

void Game_init_vars ( int init_type )
{
   /* Help me! I'm using the evil goto !!! */
   switch ( init_type )
   {
      case INITGAME:
         goto g_init_game;
      case NEWGAME:
         goto g_new_game;
      case NEWLEVEL:
         goto g_new_level;
      default:
         goto g_init_game;
   }

g_init_game:
   gv->rfps         = REFERENCE_FRAMERATE;
   gv->display_fps  = FALSE;
   gv->hi_score     = HISCORE;

g_new_game:
   gv->paused       = FALSE;
   gv->gameover     = FALSE;
   gv->pscore       = 0;
   gv->pbonus       = 0;
   gv->plives       = PLAYER_LIVES_START;

g_new_level:
   gv->fcount       = 0;
   gv->msec         = 0;
   gv->ftime        = 0.0;
   gv->fps          = 0.0;
   gv->fadjust      = 0.0;
   
   gv->new_level    = FALSE;
}

/*================================================================*/

void Game_init_keys ( void )
{
   gv->key_QUIT  = FALSE;
   gv->key_FIRE  = FALSE;
   gv->key_UP    = FALSE;
   gv->key_DOWN  = FALSE;
   gv->key_LEFT  = FALSE;
   gv->key_RIGHT = FALSE;
}

/*================================================================*/

void Game_newlevel ( void )
{
   srand ( time( NULL ) );
   Game_init_vars ( NEWLEVEL );
   Game_init_keys ();

   Stars_init ();
   Explosions_clear ();
   Jumpgate_init ();
   One_up_init ();

   Player_init ();
   Aliens_init ();

   SW_RESET;
   Game_actionfn = Game_ready;
}

/*================================================================*/

void Game_menu ( void )
{
   static int blink = 0;
   static int color = 0;
   int        i, *tmp_score[20];
   char       buffer[256], tmp_num[2];
   
   VEC up = { 0.0f, 1.0f, 0.0f, 1.0f };
   VEC from = { 0.0f, 0.0f, 100.0f, 1.0f };
   VEC at = { 0.0f, 0.0f, 0.0f, 1.0f };
   MAT cam_mat;

   /* convert hi-score into a vector-font */
   sprintf ( buffer, "%ld", gv->hi_score );
   i = 0;
   tmp_num[0] = '0';
   tmp_num[1] = '\0';
   do
   {
      tmp_num[0] = buffer[i];
      tmp_score[i] = NUMBER[atoi(tmp_num)];
      i++;
   } while ( buffer[i] != '\0' );
   tmp_score[strlen(buffer)] = NULL;
   Draw_vector_font ( HI_SCORE, 120, 15, GREEN );
   Draw_vector_font ( tmp_score, 360, 15, GREEN );

   Camera_transform ( cam_mat, up, from, at );
   Stars_draw ( cam_mat );
   
   blink += gv->msec;
   if ( blink > 250 )
   {
      color++;
      blink -= 250;
      if ( color > 3 ) color = 0;
   }

   i = RED - ( color * 15 );

   Draw_vector_font ( XINVADERS3D_LOGO, 155, 150, GREEN );
   Draw_vector_font ( PRESS, 40, 400, i );
   Draw_vector_font ( FIRE, 220, 400, i );
   Draw_vector_font ( TO, 370, 400, i );
   Draw_vector_font ( START, 460, 400, i );

   if ( gv->key_FIRE )
   {
      Game_actionfn = Game_newlevel;
   }
}

/*================================================================*/

void Game_ready ( void )
{
   static int blink = 0;
   static int color = 0;
   int i;

   VEC up = { 0.0f, 1.0f, 0.0f, 1.0f };
   VEC from = { 0.0f, 0.0f, 100.0f, 1.0f };
   VEC at = { 0.0f, 0.0f, 0.0f, 1.0f };
   MAT cam_mat;
   
   Camera_transform ( cam_mat, up, from, at );
   Stars_draw ( cam_mat );

   blink += gv->msec;
   if ( blink > 250 )
   {
      blink -= 250;
      color++;
      if ( color == 3 )
         color = 0;
   }

   i = RED - ( color * 15 );
   Draw_vector_font ( GET, 140, 200, i );
   Draw_vector_font ( READY, 260, 200, i );

   Game_overlay ();
   SW_UPDATE;
   
   if ( gv->key_FIRE || 
      ( gv->sw_t > READY_TIME ) )
   {
      SW_RESET;
      Game_actionfn = Game_run;
   }
}

/*================================================================*/

void Game_paused_toggle ( void )
{
   static void (*saved_action_fn)( void ) = NULL;
   
   gv->paused ^= TRUE;
   
   if ( gv->paused )
   {
      SW_PAUSE;
      saved_action_fn = Game_actionfn;
      Game_actionfn = Game_paused;
   }
   else
   {
      SW_UNPAUSE;
      Game_actionfn = saved_action_fn;
   }
}

void Game_paused ( void )
{
   VEC up = { 0.0f, 1.0f, 0.0f, 1.0f };
   VEC from = { 0.0f, 0.0f, 100.0f, 1.0f };
   VEC at = { 0.0f, 0.0f, 0.0f, 1.0f };
   MAT cam_mat;
   
   Camera_transform ( cam_mat, up, from, at );
   Stars_draw ( cam_mat );
   Draw_vector_font ( PAUSED, 230, 200, RED );

   Game_overlay ();
   SW_UPDATE;
}

/*================================================================*/

void Game_reset ( void )
{
   SW_RESET;
   Game_init_vars ( INITGAME );
   Game_init_keys ();
   Game_actionfn = Game_menu;
}

void Game_gameover ( void )
{
   VEC up = { 0.0f, 1.0f, 0.0f, 1.0f };
   VEC from = { 0.0f, 0.0f, 100.0f, 1.0f };
   VEC at = { 0.0f, 0.0f, 0.0f, 1.0f };
   MAT cam_mat;
   
   Camera_transform ( cam_mat, up, from, at );
   Stars_draw ( cam_mat );
   Draw_vector_font ( GAME_OVER, 200, 200, RED );
   Game_overlay ();
   SW_UPDATE;

   if ( gv->key_FIRE || (gv->sw_t > GAMEOVER_TIME) )
   {
      SW_RESET;
      Game_init_vars ( NEWGAME );
      Game_init_keys ();
      Game_actionfn = Game_menu;
   }
}

/*================================================================*/

void Game_overlay ( void )
{
   char buffer[256], tmp_num[2];
   int  i, x, *tmp_score[20];
   int  life[6] = { 0, 450, 25, 450, 12, 425 }; 

   if ( gv->display_fps )
   {
      if ( gv->ftime > 0.0f )
      {
         sprintf ( buffer, "FPS:%3.0f:(%1.3f):ms(%ld)", gv->fps, 
                            gv->fadjust,gv->msec );
      }
      else
         sprintf ( buffer, "FPS: n/a" );

      Draw_text ( buffer, 0, 475, RED );
   }
  
   /* convert score into a vector-font */
   sprintf ( buffer, "%ld", gv->pscore );
   i = 0;
   tmp_num[0] = '0';
   tmp_num[1] = '\0';
   do
   {
      tmp_num[0] = buffer[i];
      tmp_score[i] = NUMBER[atoi(tmp_num)];
      i++;
   } while ( buffer[i] != '\0' );
   tmp_score[strlen(buffer)] = NULL;
   Draw_vector_font ( SCORE, 150, 14, GREEN );
   Draw_vector_font ( tmp_score, 330, 15, GREEN );

   if ( (gv->plives-1) > 0 )
   {
      for ( i=0, x=15; i<(gv->plives-1); i++ )
      {
         Draw_line ( life[0]+x, life[1], life[2]+x, life[3], GREEN );
         Draw_line ( life[2]+x, life[3], life[4]+x, life[5], GREEN );
         Draw_line ( life[4]+x, life[5], life[0]+x, life[1], GREEN );
         x+= 30;
      }
#ifdef GAME_DEBUG
      sprintf ( buffer, "Lives: %d", gv->plives-1 );
      Draw_text ( buffer, 550, 470, GREEN );
#endif
   }

#ifdef GAME_DEBUG
   sprintf ( buffer, "Time: %10.0fs", (double)gv->sw_t/1000);
   Draw_text ( buffer, 0, 470, RED );

   sprintf ( buffer, "U: %d D: %d L: %d R: %d F: %d",
         gv->key_UP, gv->key_DOWN, gv->key_LEFT, gv->key_RIGHT,
         gv->key_FIRE );
   Draw_text ( buffer, 200, 470, WHITE );
#endif

}

/*================================================================*/

void Game_run ( void )
{
   float  dist = 0.0f;
   OBJECT *alien, *abomb;
   VEC    up = { 0.0f, 1.0f, 0.0f, 1.0f };
   VEC    from = { 0.0f, 25.0f, 100.0f, 1.0f };
   VEC    at = { 0.0f, 0.0f, -100.0f, 1.0f };
   MAT    cam_mat;

   /* prepare for new frame */
   Clear_obj_list ();   

   /* update-player */
   if ( player->active )
      player->actionfn ( player );

   /* update camera */
   Vector_addd ( from, player->pos );
   Vector_addd ( at, player->pos );
   Camera_transform ( cam_mat, up, from, at );

   /*
    * Do collisions checking : 
    * player_missile vs aliens
    * player vs alien(s)
    * alien bombs vs player
    */

   if ( pm->active )
   {
      /* is player missile in the formation zone ?? */
      if ( pm->zone == gv->formation_zone ) 
      {
         if ( ( pm->zheight > -1 ) && (pm->zheight < ZONE_HEIGHT_MAX ) )
         {
            /* if so which height-level??             */
            alien = (af_list+pm->zheight)->head;
            while ( alien )
            {
               dist = Dist_pt_2_line ( alien->pos, pm->old_pos, pm->pos ) -
                  pm->radius_squared - 1.0f;

               if ( dist < alien->radius_squared )
               {
                  pm->active = alien->active = FALSE;
                  gv->alien_count--; 
                  Objlist_del ( (af_list+pm->zheight), alien );
                  Explosions_add ( alien );
                  Update_fcolumn ( alien );

                  /* update player score & alien count */
                  gv->pscore += score_table[pm->zheight];
                  gv->pbonus += score_table[pm->zheight];
                  if ( gv->pbonus > PLAYER_LIFE_BONUS-1 )
                  {
                     gv->pbonus -= PLAYER_LIFE_BONUS;
                     if ( gv->plives < PLAYER_LIFE_MAX )
                     {
                        gv->plives++;
                        One_up_add ( alien );
                     }
                  }
                  if ( gv->alien_count == 0 )
                     gv->new_level = TRUE;
                  
                  break;
               }
               alien = alien->next;
            }
         }
      }
      /* is player missile in the UFO zone?? */
      else
      {
         if ( (ufo->active) 
            &&  ( pm->zone == ufo->zone )
            && ( pm->zheight == ufo->zheight ) ) 
         {
            dist = Dist_pt_2_line ( ufo->pos, pm->old_pos, pm->pos ) -
              pm->radius_squared - 1.0f; 
            if ( dist < ufo->radius_squared )
            {
               pm->active = ufo->active = FALSE;
               Explosions_add ( ufo );

               /* update player score */
               gv->pscore += UFO_BONUS;
               gv->pbonus += UFO_BONUS;
               if ( gv->pbonus > PLAYER_LIFE_BONUS-1 )
               {
                  gv->pbonus -= PLAYER_LIFE_BONUS;
                  if ( gv->plives < PLAYER_LIFE_MAX )
                  {
                     gv->plives++;
                     One_up_add ( ufo );
                  }
               }
            }
         }
      }
   }
   
   /* alien bombs vs player */
   if ( player->active && !gv->gameover && !gv->pblinking )
   {
      for (abomb=abombs->head; abomb != NULL; abomb=abomb->next )
      {
         if ( (abomb->zone == player->zone) && 
              (abomb->zheight == player->zheight) )
         {
            dist = Dist_pt_2_line ( player->pos, abomb->old_pos, abomb->pos ) -
               abomb->radius_squared - 1.0f;
            if ( dist < player->radius_squared )
            {
               abomb->active = player->active = FALSE;
               Explosions_add ( player );
               Objlist_del ( abombs, abomb );

               break;
            }
         }
      }
   }
   
   /* 
    * player vs aliens:
    * if the remaining aliens reach the player_zone
    * then the game is over! Reduce the number of
    * remaining lives to zero and blowup the player and
    * then initiate a game-over!
    */

   if ( gv->formation_zone == ZONE_0 )
   {
      gv->plives   = 0;
      if ( player->active ) 
      {
         Explosions_add ( player );
         player->active = FALSE;
      }
   }
   
   /*
    *
    * Move Objects and Update Animations : 
    * formation, alien_bombs, ufo, and player_missile
    *
    */
   Aliens_update ();

   if ( pm->active ) 
      pm->actionfn ( pm );
  
   /*
    *
    *  Draw Objects : 
    *
    *  special effects
    *  alien: formation, bombs, and ufo
    *  player missile
    *  player
    *
    */
   
   Stars_draw ( cam_mat );
   Jumpgate_animate ( cam_mat );
   Explosions_draw ( cam_mat );
   One_up_draw ( cam_mat );

   /* 
    * major hack!! this is NOT the right way to draw in 3d 
    * may the universe forgive my transgression!
    *
    * */
   if ( ufo->active ) 
      Add_obj_2_world ( ufo );
   Add_obj_2_world ( &the_formation );

   for (abomb=abombs->head; abomb != NULL; abomb=abomb->next )
   {
      Add_obj_2_world ( abomb );
   }
   
   if ( pm->active )
      Add_obj_2_world ( pm );
   if ( player->active )
      Add_obj_2_world ( player );

   Sort_obj_list ();
   Draw_obj_list ( cam_mat );

   SW_UPDATE;
   Game_overlay ();

   /*
    *  Before acknowledging flags make sure ALL explosions
    *  are finished:
    *  gameover flag has precedence over newlevel flag
    *  since it is possible for a player to have killed
    *  all the aliens but then get killed by an alien bomb
    *  resulting in both a newlevel and gameover condition!!
    */
   if ( Explosions_count() == 0 )
   {
      if ( player->active == FALSE )
      {
         /* reduce player life and check for gameover */
         gv->plives--;
         if ( gv->plives <= 0 )
            gv->gameover = TRUE;

         Player_init ();
         SW_RESET;
         Game_actionfn = Game_ready;
      }

      if ( gv->gameover )
      {
         if ( gv->pscore > gv->hi_score )
            gv->hi_score = gv->pscore;

         SW_RESET;
         Game_actionfn = Game_gameover;
      }
      else 
      {
         if ( gv->new_level && ( Objlist_count ( abombs ) == 0 ) )
         {
            SW_RESET;
            Game_actionfn = Game_newlevel;
         }
      }
   }
}

/*================================================================*/

void Object_update_zone ( OBJECT *obj )
{
   obj->zone    = (int) floor ( (double)obj->pos[ZPOS]/-ZONE_WIDTH  );
   obj->zheight = (int) floor ( (double)obj->pos[YPOS]/ZONE_HEIGHT );
}

/*================================================================*/

static void Draw_vector_font ( int *s[], int x, int y, unsigned int color )
{
   int i, j, X, segments;
   int *letter;

   i = 0;
   X = x;
   letter = s[0];

   do
   {
      /* first integer of pointer array always is # of line segments */
      segments = *letter;
      letter++;
      for ( j=0; j<segments; j++ )
      {
         Draw_line ( (*(letter+(j*4)))+X, (*(letter+(j*4)+1))+y,
                     (*(letter+(j*4)+2))+X, (*(letter+(j*4)+3))+y, color );
      }

      i++;
      letter = s[i];
      X += 30;
   }
   while ( letter );
}

/*================================================================*/

void Add_obj_2_world ( OBJECT *obj )
{
   int i;

   i = gv->gobjcount;

   if ( i < MAX_OBJECTS-1 )
   {
      gv->gobjs[i] = obj;
      gv->gobjcount++;
   }
}

void Clear_obj_list ( void )
{
   gv->gobjcount = 0;
}

void Sort_obj_list ( void )
{
   qsort ( gv->gobjs, gv->gobjcount, sizeof ( OBJECT * ), Z_compare );
}

int Z_compare ( const void *obj0, const void *obj1 )
{
   OBJECT *o0, *o1;

   o0 = ( OBJECT * ) obj0;
   o1 = ( OBJECT * ) obj1;
   
   if ( o0->pos[ZPOS] < o1->pos[ZPOS] )
      return -1;
   else if ( o0->pos[ZPOS] > o1->pos[ZPOS] )
      return 1;
   else
      return 0;
}

void Draw_obj_list ( MAT r )
{
   int i;
   OBJECT *obj;

   if ( gv->gobjcount > 0 )
   {
      i = 0;
      while ( i < gv->gobjcount )
      {
         obj = gv->gobjs[i];
         obj->drawfn ( obj, r );
         i++;
      }
   }
}
/*================================================================*/

float Dist_pt_2_line ( VEC p, VEC l0, VEC l1 )
{
   VEC l0_p;
   VEC l0_l1;
   VEC q;
   float n, d, w, t, dist;

   /* 
    * calc adjusted position of projectile along the
    * line segment formed by old_pos to new_pos
    * use this for calculating distance between moving
    * objects. This must be done since movement is
    * not in discreet amounts due to varying framerates.
    *
    * routine uses the properties of the unit vector
    * and dot product. see ``Computer Graphics:
    * principles and practice'' appendix for
    * explaination. Then checks to see if generated
    * line falls within the line segment l0l1
    *
    */

   Vector_sub ( p, l0, l0_p );
   Vector_sub ( l1, l0, l0_l1 );

   /* 
    * don't try to take Vector_norm if division by zero
    * will result. TBB
    *
    * check using Vec_mag_squared instead of 
    * xpos == 0 && ypos == 0 && zpos == 0 
    * Don
    */
   
   if ( Vector_mag_squared ( l0_l1 ) < 1.0f )
   {
      q[XPOS] = l0[XPOS];
      q[YPOS] = l0[YPOS];
      q[ZPOS] = l0[ZPOS];

      dist = Vector_dist_squared ( q, p );
   } 
   else 
   {
      d = l1[ZPOS] - l0[ZPOS];

      /* l0 and l1 are very close */
      if ( (d > -0.025 ) && ( d < 0.025 ) )
      {
         q[XPOS] = l0[XPOS];
         q[YPOS] = l0[YPOS];
         q[ZPOS] = l0[ZPOS];

         dist = Vector_dist_squared ( q, p );
      }
      else
      {
         Vector_norm ( l0_l1 );
         w = Vector_dot ( l0_p, l0_l1 );

         q[XPOS] = l0[XPOS] + ( l0_l1[XPOS] * w );
         q[YPOS] = l0[YPOS] + ( l0_l1[YPOS] * w );
         q[ZPOS] = l0[ZPOS] + ( l0_l1[ZPOS] * w );
            
         n = q[ZPOS] - l0[ZPOS];

         t = n / d;
         if ( ( t > -0.025f ) && ( t < 1.5f ) )
            dist = Vector_dist_squared ( q, p );
         else
            dist = 1000000.0f;
      }
   }

   return dist;
}
