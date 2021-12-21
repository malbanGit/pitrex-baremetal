/*------------------------------------------------------------------
  vec4x1.h:

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
#ifndef VEC4x1_DEF
#define VEC4x1_DEF

typedef float VEC [4];

extern void   Vector_init    ( VEC );
extern void   Vector_set     ( VEC, float, float, float );
extern void   Vector_copy    ( VEC, VEC );
extern void   Vector_print   ( VEC );
extern void   Vector_negate  ( VEC );
extern void   Vector_add     ( VEC, VEC, VEC );
extern void   Vector_addd    ( VEC, VEC );
extern void   Vector_sub     ( VEC, VEC, VEC );
extern void   Vector_subb    ( VEC, VEC );
extern void   Vector_cross   ( VEC, VEC, VEC );
extern float  Vector_mag     ( VEC );
extern float  Vector_mag_squared ( VEC );
extern float  Vector_dist    ( VEC, VEC );
extern float  Vector_dist_squared ( VEC, VEC );
extern float  Vector_dot     ( VEC, VEC );
extern void   Vector_norm    ( VEC );

#endif
