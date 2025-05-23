#define BLOW_UP 15

signed char Folder[]=
{	(signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, +0x00*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, -0x04*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, +0x05*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, +0x00*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};


// ABC_WIDE
 signed char ABC_0[]=
{   (signed char) 0xFF, +0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_1[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x01*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};

 signed char ABC_2[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_3[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_4[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_5[]=
{   (signed char) 0xFF, +0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_6[]=
{   (signed char) 0x00, +0x07*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x01*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x05*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_7[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_8[]=
{   (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_9[]=
{   (signed char) 0x00, +0x02*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_10[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x03*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_11[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_12[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_13[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_14[]=
{   (signed char) 0x00, +0x00*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_15[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x05*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_16[]=
{   (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x06*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x02*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_17[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};

// S
// ------ 1
// I
// I 2
// I
// ------ 3
//      I
//      I 4
//      I
// ------ 5
//   -> 6
signed char ABC_18[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};

// T
signed char ABC_19[]=
{   (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_20[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_21[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_22[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_23[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_24[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x03*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_25[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_26[]=
{   (signed char) 0xFF, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_27[]=
{   (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_28[]=
{   (signed char) 0xFF, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x03*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_29[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_30[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_31[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_32[]=
{   (signed char) 0x00, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x05*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_33[]=
{   (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_34[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x03*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_35[]=
{   (signed char) 0x00, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_36[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x05*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x05*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_37[]=
{   (signed char) 0x00, +0x05*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_38[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_39[]=
{   (signed char) 0x00, +0x02*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x02*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
 signed char ABC_40[]=
{   (signed char) 0x00, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};



/*

const signed char ABC_0[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x04*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x05*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_1[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_2[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_3[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x04*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x04*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_4[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_5[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_6[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x05*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_7[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_8[]=
{   (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_9[]=
{   (signed char) 0xFF, +0x00*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x06*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_10[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x04*BLOW_UP, +0x05*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, -0x05*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x04*BLOW_UP, +0x05*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_11[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_12[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x04*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x04*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_13[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_14[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_15[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_16[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x02*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_17[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x04*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_18[]=
{   (signed char) 0xFF, +0x02*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_19[]=
{   (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_20[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_21[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_22[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x04*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x04*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_23[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_24[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x05*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x05*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_25[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_26[]=
{   (signed char) 0xFF, +0x01*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x01*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_27[]=
{   (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_28[]=
{   (signed char) 0xFF, +0x01*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x07*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x06*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x02*BLOW_UP, +0x05*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_29[]=
{   (signed char) 0x00, +0x06*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_30[]=
{   (signed char) 0x00, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x03*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x05*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_31[]=
{   (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x04*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x04*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_32[]=
{   (signed char) 0x00, +0x00*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x03*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x05*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x07*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x01*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_33[]=
{   (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_34[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_35[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x08*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_36[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_37[]=
{   (signed char) 0x00, +0x04*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_38[]=
{   (signed char) 0xFF, +0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, -0x08*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, +0x00*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_39[]=
{   (signed char) 0x00, +0x02*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x02*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x06*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char ABC_40[]=
{   (signed char) 0x00, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
    (signed char) 0xFF, +0x02*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
    (signed char) 0x00, -0x04*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};
*/
















 int8_t*  ABC[]=
{
    // starting with ' ' SPACE (0x20)
    ABC_27, // ' '
    ABC_28, // !
    ABC_27, // ' ' - "
    ABC_27, // ' ' - #
    Folder, // ABC_27, // ' ' - $
    ABC_27, // ' ' - %
    ABC_27, // ' ' - &
    ABC_27, // ' ' - '
    ABC_27, // ' ' - (
    ABC_27, // ' ' - )
    ABC_27, // ' ' - *
    ABC_27, // ' ' - +
    ABC_27, // ' ' - ,
    ABC_27, // ' ' - -
    ABC_26, // .
    ABC_27, // ' ' - /
        ABC_38, // 0
    ABC_29, // 1
    ABC_30, // 2
    ABC_31, // 3
    ABC_32, // 4
    ABC_33, // 5
    ABC_34, // 6
    ABC_35, // 7
    ABC_36, // 8
    ABC_37, // 9
    ABC_27, // ' ' - :
    ABC_27, // ' ' - ;
    ABC_39, // <- - <
    ABC_27, // ' ' =
    ABC_40, // -> - >
    ABC_27, // ' ' - ?
    ABC_27, // ' ' - @
    ABC_0, // A
    ABC_1, // B
    ABC_2, // C
    ABC_3, // D
    ABC_4, // E
    ABC_5, // F
    ABC_6, // G
    ABC_7, // H
    ABC_8, // I
    ABC_9,  // J
    ABC_10, // K
    ABC_11, // L
    ABC_12, // M
    ABC_13, // N
    ABC_14, // O
    ABC_15, // P
    ABC_16, // Q
    ABC_17, // R
    ABC_18, // S
    ABC_19, // T
    ABC_20, // U
    ABC_21, // V
    ABC_22, // W
    ABC_23, // X
    ABC_24, // Y
    ABC_25, // Z
    ABC_27, // '[' - /
    ABC_27, // '\' - /
    ABC_27, // ']' - /
    ABC_27, // '^' - /
    ABC_27, // '_' - /
    ABC_27, // ''' - /


    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
    ABC_26, // .
};

// using set scale!
void v_Draw_VLp(int8_t *list)
{
    do
    {
        SET_YSH_IMMEDIATE_8(*(list+1));
        SET_XSH_IMMEDIATE_8(*(list+2));
#ifdef BEAM_LIGHT_BY_SHIFT      
        SET_SHIFT_REG(*list);
#endif      
#ifdef BEAM_LIGHT_BY_CNTL   
        if (*list != 0) SWITCH_BEAM_ON();
#endif      
                list = list+3;
        START_T1_TIMER();
        WAIT_T1_END();
        if (((int8_t) ( *list) )>=0)
          ADD_DELAY_CYCLES(5); // last vector should wait longer
        SWITCH_BEAM_OFF();
    }
    while (((int8_t) ( *list) )<=0);
}

// positions using scale $7f
// zeroes before positioning
// leaves zeroed
// string printed with scale factor given
// print strings given in "C" format (zero terminated) 
//
// only large letters supported
// coordinates given in vectrex 8 bit
void v_printStringMax(int8_t x, int8_t y, char* string, uint8_t textSize, uint8_t brightness, int maxchars)
{
 int currentCharCount =0;
      if (usePipeline)
      {

        int i=0;

        #define SCALEFONT (textSize *1.5 )
        SET_OPTIMAL_SCALE(120*SCALEFONT,120*SCALEFONT);
        // currentScale is set by the macro

        int startX = x*128;
        int startY = y*128;


// print "dummy"
// at the correct position
// otherwise a scroll text "jumps"
        v_directDraw32Hinted(startX-1000, startY-1000, startX, startY, 5, (commonHints| PL_BASE_FORCE_USE_FIX_SIZE));




	if (brightness >= 0)
	{
	  brightness = brightness*intensityMul;
	  if (brightness!=0)
	  {
	    if (brightness>intensityMax)brightness=intensityMax;
	    if (brightness<intensityMin)brightness=intensityMin;
	    
	  }
	}



        while (*string != 0)
        {
            int8_t* list   = ABC[toupper(*string)-0x20];
            string++;
            currentCharCount++;
            do
            {
              if (*list != 0)
                v_directDraw32Hinted(startX, startY, (*(list+2))*SCALEFONT+startX, (*(list+1))*SCALEFONT+startY, brightness, (commonHints| PL_BASE_FORCE_USE_FIX_SIZE));
              startX=(*(list+2))*SCALEFONT+startX;
              startY=(*(list+1))*SCALEFONT+startY;
              list = list+3;
              i++;
            }
            while (((int8_t) ( *list) )<=0);
            if (currentCharCount == maxchars) break;
        }
        return;
      }
    v_setBrightness(brightness);


    ZERO_AND_WAIT();
    UNZERO();
    v_setScale(0x7f);
    v_moveToImmediate8(x, y);
        v_setScale(textSize);
    while (*string != 0)
    {
        int8_t* oneLetterList   = ABC[*string-0x20];
        string++;
        v_Draw_VLp(oneLetterList);
    }
    ZERO_AND_WAIT();
}

// if not pipelined
// positions using scale $7f
// zeroes before positioning
// leaves zeroed
// string printed with scale factor given
// print strings given in "C" format (zero terminated) 
//
// only large letters supported
// coordinates given in vectrex 8 bit
void v_printString(int8_t x, int8_t y, char* string, uint8_t textSize, uint8_t brightness)
{
    if (usePipeline)
    {
      int i=0;

      #define SCALEFONT (textSize *1.5 )
      SET_OPTIMAL_SCALE(120*SCALEFONT,120*SCALEFONT);
      // currentScale is set by the macro

      int startX = x*128;
      int startY = y*128;

      if (brightness >= 0)
      {
	brightness = brightness*intensityMul;
	if (brightness!=0)
	{
	  if (brightness>intensityMax)brightness=intensityMax;
	  if (brightness<intensityMin)brightness=intensityMin;
	  
	}
      }
      
      while (*string != 0)
      {
          int8_t* list   = ABC[toupper(*string)-0x20];
          string++;
          do
          {
            if (*list != 0)
              v_directDraw32Hinted(startX, startY, (*(list+2))*SCALEFONT+startX, (*(list+1))*SCALEFONT+startY, brightness, (commonHints| PL_BASE_FORCE_USE_FIX_SIZE));
            startX=(*(list+2))*SCALEFONT+startX;
            startY=(*(list+1))*SCALEFONT+startY;
            list = list+3;
            i++;
          }
          while (((int8_t) ( *list) )<=0);
      }
      return;
    }
    v_setBrightness(brightness);
    ZERO_AND_WAIT();
    UNZERO();
    v_setScale(0x7f);
    v_moveToImmediate8(x, y);
        v_setScale(textSize);
    while (*string != 0)
    {
      int8_t* oneLetterList   = ABC[*string-0x20];
      string++;
      v_Draw_VLp(oneLetterList);
    }
    ZERO_AND_WAIT();
}

void v_printString_scale(int8_t x, int8_t y, char* string, uint8_t scale, uint8_t size, uint8_t brightness)
{
    if (usePipeline)
    {
      int i=0;
      currentScale = scale;
#undef SCALEFONT 
      #define SCALEFONT (size *1.5 )
      
      int startX = x*128;
      int startY = y*128;

      if (brightness >= 0)
      {
        brightness = brightness*intensityMul;
        if (brightness!=0)
        {
          if (brightness>intensityMax)brightness=intensityMax;
          if (brightness<intensityMin)brightness=intensityMin;
          
        }
      }
      
      while (*string != 0)
      {
          int8_t* list   = ABC[toupper(*string)-0x20];
          string++;
          do
          {
            if (*list != 0)
              v_directDraw32Hinted(startX, startY, (*(list+2))*SCALEFONT+startX, (*(list+1))*SCALEFONT+startY, brightness, (commonHints| PL_BASE_FORCE_USE_FIX_SIZE));
            startX=(*(list+2))*SCALEFONT+startX;
            startY=(*(list+1))*SCALEFONT+startY;
            list = list+3;
            i++;
          }
          while (((int8_t) ( *list) )<=0);
      }
      return;
    }
    v_setBrightness(brightness);
    ZERO_AND_WAIT();
    UNZERO();
    v_setScale(0x7f);
    v_moveToImmediate8(x, y);
        v_setScale(scale);
    while (*string != 0)
    {
      int8_t* oneLetterList   = ABC[*string-0x20];
      string++;
      v_Draw_VLp(oneLetterList);
    }
    ZERO_AND_WAIT();
#undef SCALEFONT 
}
