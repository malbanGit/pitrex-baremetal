/** \file
   Objects
*/
#include "objects.h"

const  objects_char_t gobjects[] = {
  {
    7, /* 0 Ship */
    -4, -5, 0, -3, 4, -5, 0, 5, -4, -5, 0, 2, 4, -5,
  },
  {
    2, /* 1 "punkt" */
    1, 0, 0, 0,
  },
  {
    2, /* 2 Linie */
    -10, 0, 10, 0,
  },
  {
    9,          //Jet 3
    -2, 3,
    0, 8,
    2, 3,
    -127, -127,
    0, -3,
    -10, -7,
    0, 13,
    10, -7,
    0, -3,
  },
  
  {
    5,          //Engine 4
    -2, -3,
    0, -3,
    2, -4,
    0, -10,
    -2, -3,
  },
  
  { 
    5,          //Kreuz 5
    -4, 0,
    4, 0,
    -127, -127,
    0, 4,
    0, -4,
  },


  { 5,          //Bullet 6
    -2, -3,
    2, -3,
    1, 3,
    -1, 3,
    -2, -3,
  },


  { 1,          //Platzhalter 7
    0, 0,
  },
  { 1,          //Platzhalter 8
    0, 0,
  },
  { 1,          //Platzhalter 9
    0, 0,
  },
  { 1,          //Platzhalter 10
    0, 0,
  },
  { 1,          //Platzhalter 11
    0, 0,
  },
  { 1,          //Platzhalter 12
    0, 0,
  },



  { 19,          //Meteor1 13
    -5, 0,
    -6, -2,
    -5, -6,
    0, -5,
    7, -6,
    5, 0,
    9, 3,
    4, 8,
    -1, 9,
    -6, 6,
    -5, 0,
    -127, -127,
    0, 4,
    -1, 6,
    -2, 4,
    -127, -127,
    0, -3,
    4, -3,
    2, 0,
  },


  { 19,          //Meteor2 14
    -5, -1,
    -6, -3,
    -3, -6,
    0, -6,
    2, -5,
    6, -2,
    7, 1,
    4, 5,
    -1, 7,
    -5, 4,
    -5, -1,
    -127, -127,
    2, 2,
    0, 5,
    0, 2,
    -127, -127,
    -3, -4,
    -1, -4,
    -2, -1,
  },

  { 1,          //Platzhalter 15
    0, 0,
  },
  { 1,          //Platzhalter 16
    0, 0,
  },
  { 1,          //Platzhalter 17
    0, 0,
  },

{17,          //Enemy01 18
4,-4,
3,-1,
4,3,
3,4,
3,3,
0,2,
-3,3,
-3,4,
-4,3,
-3,-1,
-4,-4,
0,-3,
4,-4,
-127,-127,
1,-1,
0,1,
-1,-1,
},


{  23,          //Enemy02 19
4,5,
2,6,
4,2,
0,3,
-4,2,
-3,4,
-2,6,
-4,5,
-6,1,
-2,-5,
1,-5,
6,1,
4,5,
-127,-127,
0,1,
1,1,
1,0,
1,-1,
0,-1,
-1,-1,
-1,0,
-1,1,
0,1,
},

{57,          //Enemy03 20
-3,1,
-6,-1,
-127,-127,
-1,1,
-1,0,
-2,0,
-3,-2,
-4,-3,
-7,-3,
-7,0,
-4,1,
-3,2,
-1,1,
-127,-127,
3,1,
6,-1,
-127,-127,
2,3,
1,3,
3,7,
4,5,
2,3,
2,3,
-127,-127,
-2,3,
-1,3,
-3,7,
-4,5,
-2,3,
-2,3,
-127,-127,
1,1,
1,0,
2,0,
3,-2,
4,-3,
7,-3,
7,0,
4,1,
3,2,
1,1,
-127,-127,
1,3,
2,2,
1,0,
2,-1,
3,-3,
2,-5,
0,-6,
-2,-5,
-3,-3,
-2,-1,
-1,0,
-2,2,
-1,3,
0,4,
1,3,
},

{53,          //Enemy 04  21
15,9,
17,14,
22,14,
-127,-127,
-5,-16,
-3,-11,
1,-11,
-127,-127,
-25,10,
-22,15,
-17,15,
-127,-127,
7,-7,
5,-5,
12,2,
15,0,
7,-7,
-127,-127,
-5,-7,
-2,-5,
-10,2,
-12,0,
-5,-7,
-127,-127,
25,17,
30,12,
30,5,
25,0,
17,0,
12,5,
12,12,
17,17,
25,17,
-127,-127,
-2,-25,
-7,-20,
-7,-12,
-2,-7,
5,-7,
10,-12,
10,-20,
5,-25,
-2,-25,
-127,-127,
-15,17,
-10,12,
-10,5,
-15,0,
-22,0,
-27,5,
-27,12,
-22,17,
-15,17,
},



};
