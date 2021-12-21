/*------------------------------------------------------------------
  mat4x4.h:

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
#ifndef MAT4x4_DEF
#define MAT4x4_DEF

#include "vec4x1.h"

typedef float MAT [4][4];

extern void Matrix_id        ( MAT );
extern void Matrix_zero      ( MAT );
extern void Matrix_copy      ( MAT, MAT );
extern void Matrix_print     ( MAT );
extern void Matrix_mult      ( MAT, MAT, MAT );
extern void Matrix_transpose ( MAT, MAT );
extern void Matrix_vec_mult  ( MAT, VEC, VEC );
extern void Matrix_vec_multn ( MAT, VEC [], VEC [], int );

extern void Matrix_x_rot     ( MAT, float );
extern void Matrix_y_rot     ( MAT, float );
extern void Matrix_z_rot     ( MAT, float );

extern void Matrix_set_xrow  ( MAT, VEC );
extern void Matrix_set_yrow  ( MAT, VEC );
extern void Matrix_set_zrow  ( MAT, VEC );

extern void Matrix_set_xcol  ( MAT, VEC );
extern void Matrix_set_ycol  ( MAT, VEC );
extern void Matrix_set_zcol  ( MAT, VEC );

extern void Matrix_set_trans ( MAT, VEC );
#endif
