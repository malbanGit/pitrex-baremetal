#ifndef GAMEDIPS_H
#define GAMEDIPS_H

#include "globals.h"
#include "dips.h"



DIP_START(gui_dips)
DIP_END




DIP_START(no_dips)
 DIP_SET("NO DIPS","NO","YES","","","","","","",1,0x7777,0,0,0,0,1,0,0,0,0,0,0)
DIP_END


DIP_START(starwars_dips)
   { "PLAY MODE ", {"1 COIN 1 PLAY","2 COINS 1 PLAY","FREE PLAY","1 COIN 2 PLAYS"},
   3, 2, 0xfc, 0, 0, {0, 1, 2, 3 }},	 
   { "BONUS AT", {"10,000", "20,000", "30,000", "NONE "},
   3, 1, 0xc0, 1, 1,{0, 0x40  ,0x80 ,0xc0}},
   { "DIFFICULTY", {"EASY", "HARD", "", ""},
   1, 1, 0xcf, 0, 0,{0x10,  0 , 0, 0}},
   { "SHIPS", {"3", "4", "5", "6"},
   3, 1, 0xf3, 0, 0,{0, 0x04, 0x08, 0x0c }},
DIP_END

DIP_START(mhavoc_dips)
 
   { "DEMO SOUNDS", {"SILENT", "SOUND", "", ""},
   1, 1, 0xfd, 1, 0,{0x02, 0x00, 0x00, 0 }},
   { "ADAPTIVE ", {"OFF", "ON", "", ""},
   1, 1, 0xfe, 1, 0,{0x01, 0x00, 0x00, 0 }},
   { "BONUS LIFE", {"50000", "100000", "200000", ""},
   2, 1, 0xf3, 1, 0,{0x0c, 0x04, 0x08, 0 }},
   { "DIFFICULTY", {"EASY", "MEDIUM", "HARD", "DEMO"},
   3, 1, 0xcf, 1, 0,{0x10,  0 , 0x30, 0x20}},
   { "LIVES", {"3", "4", "5", "6"},
   3, 1, 0x3f, 1, 0,{0x00, 0xc0, 0x80, 0x40 }},
   { "COINAGE ", {"2 COINS 1 PLAY","1 COIN 1 PLAY","1 COIN 2 PLAYS","FREE PLAY"},
   3, 2, 0xfc, 1, 0, {0x00, 0x03, 0x00, 0x01 }},
   { "RIGHT COIN ", {"x1","x2","x3","x4"},
   3, 2, 0xf3, 0, 0, {0x0c, 0x08, 0x04, 0x00 }},
   { "LEFT COIN ", {"x1","x2","",""},
   1, 2, 0xef, 0, 0, {0x10, 0x00, 0, 0 }},
   { "BONUS CREDITS", {"2 EACH 4","1 EACH 3","1 EACH 4","1 EACH 5","NONE"},
   1, 2, 0x1f, 0, 0, {0x80, 0x40, 0xa0, 0x60,0xe0 }},
   { "2 CREDIT MIN", {"NO", "YES", " ", " "},
   1, 0x8888, 0, 0, 0,{0, 1, 0, 0, }},
DIP_END



DIP_START(omega_dips)
//DIP1
   { "1ST BONUS SHIP", {"40K", "50K","70K","100K"},
   3, 1, 0xfc, 1, 1, {0, 1, 2, 3 }},
   { "2ND & 3RD SHIP", {"150K/250K", "250K/500K","500/750K","750K/1500K"},
   3, 1, 0xf3, 0, 0, {0, 4, 8, 0x0c }},
   {"SHIPS/CREDIT", {"1C2S/2C4S", "1C2S/2C5S","1C3S/2C6S","1C3S/2C7S"},
   3, 1, 0xcf, 2, 2, {0, 0x10, 0x20, 0x30 }},
//DIP2
   { "COIN SW1", {"1 C/2 CREDITS", "1C/3 CREDITS","1C/5 CREDITS","4 C/5 CREDITS","3 C/4 CREDITS","2 C/3 CREDITS","2 C/3 CREDITS","1 C/1 CREDITS"},
   7, 2, 0xF8, 7, 7, {0, 1, 2, 3, 4, 5, 6, 7 }},	 
   { "COIN SW2", {"1 C/2 CREDITS", "1C/3 CREDITS","1C/5 CREDITS","4 C/5 CREDITS","3 C/4 CREDITS","2 C/3 CREDITS","2 C/3 CREDITS","1 C/1 CREDITS"},
   7, 2, 0xC7, 7, 7,  {0, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38 }},		 
    { "PLAY MODE ", {"COIN PLAY", "FREE PLAY"},
   1, 2, 0x3f, 0, 0, {0, 0x40, 0, 0 }},	                    
  
   { "COCKTAIL", {"NO", "YES", " ", " "},
   1, 2, 0x7f, 0, 0,{0, 0x80, 0, 0, }},
DIP_END

DIP_START(tailg_dips)
   { "SHIELD POINTS", {"15","20","30","40","50","60","70","80"}, 
   7, DIPSW1, 0xdc, 7, 0, {0x23, 0x21, 0x22, 0x20, 0x03, 0x01, 0x02, 0x00 }},
DIP_END


DIP_START(asteroid_dips)
 DIP_SET4(0x2800,0xfc,"PLAY MODE",0x00,"FREE PLAY",0x02,"1 COIN/1 PLAY",0x01,"1 COIN/2 PLAYS",0x03,"2 COINS/1 PLAY",1)
 DIP_SET2(0x2802,0xf3,"LIVES",0x0,"3 Ships",0x01,"4 Ships",0)
 DIP_SET4(0x2803,0xfc,"LANGUAGE",0x00,"ENGLISH",0x01,"GERMAN",0x02,"FRENCH",0x03,"SPANISH",0)
 DIP_SET2(COCKTAIL,0,"COCKTAIL",0x00,"NO",0x01,"YES",0)
DIP_END

DIP_START(astdelux_dips)
   DIP_SET4(0x2800,0xfc,"Bonus At",0x00,"10,000",0x01,"12,000",0x02,"15,000",0x03,"NONE",0)
   DIP_SET2(0x2801,0xfc,"Difficulty",0x02,"Easy",0x00,"Hard",0)
   DIP_SET4(0x2802,0xfc,"Lives",0x00,"2-4 Ships",0x01,"3-5 Ships",0x02,"4-6 Ships",0x03,"5-7 Ships",1)
   DIP_SET4(0x2803,0xfc,"Language",0x00,"English",0x01,"German",0x02,"French",0x03,"Spanish",1)
   DIP_SET4(DIPSW1,0xfc,"Play Mode",0x03,"Free Play",0x02,"1 Coin 2 Plays",0x01,"1 Coin 1 Play",0x00,"2 Coins 1 Play",2)
   DIP_SET2(COCKTAIL,0x00,"Cocktail",0x00,"No",0x01,"Yes",0)
   DIP_SET2(CHEAT,0x00,"Color Monitor",0x00,"No",0x01,"Yes",0)
DIP_END

DIP_START(bwidow_dips)
   DIP_SET4(DIPSW1,0xfc,"Play Mode",0x00,"1 Coin 1 Play",0x01,"2 Coins 1 Play",0x02,"Free Play",0x03,"1 Coin 2 Plays",0)
    
   { "BONUS LIFE", {"20,000", "30,000", "40,000", "NONE "},
   3, DIPSW2, 0x3f, 0, 0,{0, 0x40  ,0x80 ,0xc0 }},
   { "DIFFICULTY", {"EASY", "MEDIUM", "HARD", "DEMO"},
   3, DIPSW2, 0xcf, 1, 1,{0,  0x10 , 0x20, 0x30}},
   { "SPIDERS", {"3", "4", "5", "6"},
   3, DIPSW2, 0xf3, 0, 0,{0, 0x04, 0x08, 0x0c }},
   { "MAX START AT", {"LEVEL 13","LEVEL 21","LEVEL 37","LEVEL 53"},
   3, DIPSW2, 0xfc, 1, 1, {0, 1, 2, 3 }},	
DIP_END
                                                                              
DIP_START(spacduel_dips)
   { "PLAY MODE ", {"1 COIN 1 PLAY","2 COINS 1 PLAY","FREE PLAY","1 COIN 2 PLAYS"},
   3, DIPSW2, 0xfc, 0, 0, {0, 1, 2, 3 }},	 
   { "SHIPS", {"3", "4", "5", "6"},
   3, DIPSW1, 0xfc, 0, 0,{1, 3, 0, 2 }},
   { "DIFFICULTY", {"EASY", "NORMAL", "MEDIUM", "HARD"},
   3, DIPSW1, 0xf3, 1, 1,{0x04, 0 , 0x0c, 0x08 }},
   { "LANGUAGE", {"ENGLISH", "GERMAN", "SPANISH","FRENCH"},
   3, DIPSW1, 0xcf, 0, 0,{0, 0x10, 0x30 , 0x20 }},
   { "BONUS LIFE", {"8,000", "10,000", "15,000", "NONE "},
   3, DIPSW1, 0x3f, 1, 1,{0xc0,0  ,0x40 ,0x80 }},
   { "1 PLAYER MODE", {"NO", "YES", " ", " "},
   1, 0x8888, 0, 0, 0,{0, 1, 0, 0, }},
   { "COCKTAIL", {"NO", "YES", " ", " "},
   1, 0x7777, 0, 0, 0,{0, 1, 0, 0, }},
DIP_END

DIP_START(gravitar_dips)
   { "PLAY MODE ", {"1 COIN 1 PLAY","2 COINS 1 PLAY","FREE PLAY","1 COIN 2 PLAYS"},
   3, DIPSW2, 0xfc, 0, 0, {0, 1, 2, 3 }},	 
   { "BONUS AT", {"10,000", "20,000", "30,000", "NONE "},
   3, DIPSW1, 0xc0, 1, 1,{0, 0x40  ,0x80 ,0xc0}},
   { "DIFFICULTY", {"EASY", "HARD", "", ""},
   1, DIPSW1, 0xcf, 0, 0,{0x10,  0 , 0, 0}},
   { "SHIPS", {"3", "4", "5", "6"},
   3, DIPSW1, 0xf3, 0, 0,{0, 0x04, 0x08, 0x0c }},
DIP_END


DIP_START(tempest_dips)
   { "PLAY MODE ", {"1 COIN 1 PLAY","2 COINS 1 PLAY","FREE PLAY","1 COIN 2 PLAYS"},
   3, DIPSW1, 0xfc, 0, 0, {0, 1, 2, 3 }},	
    { "DIFFICULTY ", {"EASY","MEDIUM1","MEDIUM2","HARD"},
   3, 0x60d0, 0xfc, 1, 1, {1, 0, 3, 2 }},	
   { "BONUS LIFE", {"10000", "20000", "30000", "40000","50000", "60000", "70000", "NONE"},
   7, DIPSW2, 0xc7, 1, 1,{0x08,0,0x10,0x18,0x20,0x28,0x30,0x38}},
   { "LANGUAGE", {"ENGLISH","FRENCH","GERMAN","SPANISH"},
   3, DIPSW2, 0xf9, 0, 0,{0, 0x02, 0x04, 0x06 }},
   { "MINIMUM", {"1 CREDIT","2 CREDITS"," "," "},
   1, DIPSW2, 0x7f, 0, 0, {0, 1, 2, 3 }},
    { "LIVES", {"2", "3", "4", "5"},
   3, DIPSW2, 0x3f, 1, 1,{0xc0,0,0x40,0x80}},
   { "COCKTAIL", {"NO", "YES", " ", " "},
   1, 0x7777, 0, 0, 0,{0, 1, 0, 0, }},
   { "LEVEL HACK", {"NO", "YES", " ", " "},
   1, 0x8888, 0, 0, 0,{0, 1, 0, 0, }},
DIP_END

static DIPSWITCH zektor_dips[] = {
   
   { "BONUS LIFE", {"10,000", "20,000", "30,000", "NONE"},
   2, 1, 0xfc, 0, 0, {3, 1, 2, 0 }},   
   { "DIFFICULTY", {"EASY", "NORMAL", "HARD", "VERY HARD"},
   3, 1, 0xf3, 1, 1, {0, 8, 4, 0x0c }},
   { "LIVES", {"2", "3", "4", "5"},
   3, 1, 0xcf, 1, 1,{0, 0x20, 0x10, 0x30 }},
   { "DEMO SOUNDS", {"OFF", "ON", " ", " "},
   1, 1, 0xbf, 1, 1,{0x40, 0, 0, 0 }},
   { "CABINET", {"UPRIGHT", "COCKTAIL", " ", " "},
   1, 1, 0x7f, 0, 0,{0x80, 0, 0, 0, }},
   { "NONE", {"NONE", " ", " ", " "},
   0, 0, 0, 0,0, {0, 0, 0, 0 }}
};

static DIPSWITCH spacfury_dips[] = {
   
   { "BONUS LIFE", {"10,000", "20,000", "30,000", "40,000"},
   3, 1, 0xfc, 0, 0, {0, 2, 1, 3 }},   
   { "DIFFICULTY", {"EASY", "NORMAL", "HARD", "VERY HARD"},
   3, 1, 0xf3, 1, 1, {0, 8, 4, 0x0c }},
   { "LIVES", {"2", "3", "4", "5"},
   3, 1, 0xcf, 1, 1,{0, 0x20, 0x10, 0x30 }},
   { "DEMO SOUNDS", {"OFF", "ON", " ", " "},
   1, 1, 0xbf, 1, 1,{0x40, 0, 0, 0 }},
   { "CABINET", {"UPRIGHT", "COCKTAIL", " ", " "},
   1, 1, 0x7f, 0, 0,{0x80, 0, 0, 0, }},
   { "NONE", {"NONE", " ", " ", " "},
   0, 0, 0, 0,0, {0, 0, 0, 0 }}

};

DIP_START(tacscan_dips)
   { "BONUS LIFE", {"10,000", "20,000", "30,000", "NONE"},
   2, 1, 0xfc, 0, 0, {3, 1, 2, 0 }},   
   { "DIFFICULTY", {"EASY", "NORMAL", "HARD", "VERY HARD"},
   3, 1, 0xf3, 1, 1, {0, 8, 4, 0x0c }},
   { "LIVES", {"2", "4", "6", "8"},
   3, 1, 0xcf, 1, 1,{0, 0x20, 0x10, 0x30 }},
   { "DEMO SOUNDS", {"OFF", "ON", " ", " "},
   1, 1, 0xbf, 1, 1,{0x40, 0, 0, 0 }},
   { "CABINET", {"UPRIGHT", "COCKTAIL", " ", " "},
   1, 1, 0x7f, 0, 0,{0x80, 0, 0, 0, }},
DIP_END

static DIPSWITCH startrek_dips[] = {
   
   { "BONUS LIFE", {"10,000", "20,000", "30,000", "40,000"},
   3, 1, 0xfc, 0, 0, {0, 2, 1, 3 }},   
   { "DIFFICULTY", {"EASY", "NORMAL", "HARD", "TOURNAMENT"},
   3, 1, 0xf3, 1, 1, {0, 8, 4, 0x0c }},
   { "TORPEDOS", {"1", "2", "3", "4"},
   3, 1, 0xcf, 2, 2,{0, 0x20, 0x10, 0x30 }},
   { "DEMO SOUNDS", {"OFF", "ON", " ", " "},
   1, 1, 0xbf, 1, 1,{0x40, 0, 0, 0 }},
   { "CABINET", {"UPRIGHT", "COCKTAIL", " ", " "},
   1, 1, 0x7f, 0, 0,{0x80, 0, 0, 0, }},
   { "NONE", {"NONE", " ", " ", " "},
   0, 0, 0, 0,0, {0, 0, 0, 0 }}

};
static DIPSWITCH elim_dips[] = {
   
   { "BONUS LIFE", {"10,000", "20,000", "30,000", "NONE"},
   2, 1, 0xfc, 0, 0, {1, 2, 0, 3 }},   
   { "DIFFICULTY", {"EASY", "NORMAL", "HARD", "VERY HARD"},
   3, 1, 0xf3, 1, 1, {0, 8, 4, 0x0c }},
   { "LIVES", {"3", "4", "5", " "},
   2, 1, 0xcf, 1, 1,{0x20, 0x10, 0 }},
   { "DEMO SOUNDS", {"OFF", "ON", " ", " "},
   1, 1, 0xbf, 1, 1,{0x40, 0, 0, 0 }},
   { "CABINET", {"UPRIGHT", "COCKTAIL", " ", " "},
   1, 1, 0x7f, 0, 0,{0x80, 0, 0, 0, }},
   { "NONE", {"NONE", " ", " ", " "},
   0, 0, 0, 0,0, {0, 0, 0, 0 }}

};

DIP_START(bradley_dips)
  
   { "PLAY MODE ", {"FREE PLAY", "1 COIN/2 PLAYS", "1 COIN/1 PLAY", "2 COINS/1 PLAY"},
   3, 0x0c00, 0x00, 2, 2, {0, 1, 2, 3 }},	 
   { "LIVES", {"2 TANKS", "3 TANKS", "4 TANKS ", "5 TANKS "},
   3, 0x0a00, 0xfc, 1, 1,{0, 1, 2, 3 }},
   { "MISSILE AT", {"5000 POINTS", "10000 POINTS", "20000 POINTS", "30000 POINTS"},
   3, 0x0a00, 0xf3, 1, 1,{0, 0x04, 0x08, 0x0c }},
   { "BONUS TANK", {"15K AND 100K", "25K AND 100K", "50K AND 100K", " "},
   2, 0x0a00, 0xcf, 0, 0,{0, 0x10, 0x20, 0x30 }},
   { "LANGUAGE", {"ENGLISH", "GERMAN", "FRENCH", "SPANISH"},
   3, 0x0a00, 0x3f, 0, 0,{0, 0x40, 0x80, 0xc0 }},
   { "COCKTAIL", {"NO", "YES", " ", " "},
   1, 0x7777, 0, 0, 0,{0, 1, 0, 0, }},
   { "SINGLE STICK", {"NO", "YES", " ", " "},
   1, 0x8888, 0, 0, 0,{0, 1,0,0 }},  
DIP_END

DIP_START(bzone_dips)
  
   { "PLAY MODE ", {"FREE PLAY", "1 COIN/2 PLAYS", "1 COIN/1 PLAY", "2 COINS/1 PLAY"},
   3, 0x0c00, 0x00, 2, 2, {0, 1, 2, 3 }},	 
   { "LIVES", {"2 TANKS", "3 TANKS", "4 TANKS ", "5 TANKS "},
   3, 0x0a00, 0xfc, 1, 1,{0, 1, 2, 3 }},
   { "MISSILE AT", {"5000 POINTS", "10000 POINTS", "20000 POINTS", "30000 POINTS"},
   3, 0x0a00, 0xf3, 1, 1,{0, 0x04, 0x08, 0x0c }},
   { "BONUS TANK", {"15K AND 100K", "25K AND 100K", "50K AND 100K", " "},
   2, 0x0a00, 0xcf, 0, 0,{0, 0x10, 0x20, 0x30 }},
   { "LANGUAGE", {"ENGLISH", "GERMAN", "FRENCH", "SPANISH"},
   3, 0x0a00, 0x3f, 0, 0,{0, 0x40, 0x80, 0xc0 }},
   { "COCKTAIL", {"NO", "YES", " ", " "},
   1, 0x7777, 0, 0, 0,{0, 1, 0, 0, }},
   { "SINGLE STICK", {"NO", "YES", " ", " "},
   1, 0x8888, 0, 0, 0,{0, 1,0,0 }},  
DIP_END

DIP_START(redbaron_dips)
   { "PLAY MODE ", {"FREE PLAY", "1 COIN/2 PLAYS", "1 COIN/1 PLAY", "2 COINS/1 PLAY"},
   3, 0x0a00, 0x00, 2, 2, {0xff, 0xfe, 0xfd, 0xfc }},	 
    { "LANGUAGE", {"GERMAN", "FRENCH", "SPANISH", "ENGLISH"},
   3, 0x0c00, 0xfc, 3, 3,{0, 0x1, 0x2, 0x3 }},
   { "BONUS PLANE", {"2k 10k 30k", "4k 15k 40k", "6k 20k 50k", "NONE "},
   2, 0x0c00, 0xf3, 2, 2,{0xc0, 0x8, 0x4, 0x00 }},
   { "PLANES", {"2", "3", "4 ", "5"},
   3, 0x0c00, 0xcf, 1, 1,{0x30, 0x20, 0x10, 0x0 }},
   { "MINIMUM PLAY", {"OFF", "ON", " ", " "},
   1, 0x0c00, 0xbf, 0, 0,{0x40, 0x00, 0, 0 }},
   { "SELF ADJUST", {"NO", "YES", " ", " "},
   1, 0x0c00, 0x7f, 0, 0,{0x80, 0x00, 0, 0 }},
DIP_END

DIP_START(cinemat_dips)
    {"SORRY!", {"No", "Working","Dips","Yet"},
      3, 1, 0x01, 0, 0, {0, 1, 2, 3 }},
DIP_END



DIP_START(quantum_dips)
   { "PLAY MODE ", {"FREE PLAY", "1 COIN/1 PLAY", "1 COIN/2 PLAYS", "2 COINS/1 PLAY"},
   3, 1, 0xFC, 1, 1, {0, 1, 2, 3 }},	 
DIP_END


DIP_START(llander1_dips)
   { "FUEL UNITS", {"450", "600", "750 ", "900",},
   3, 1, 0xFC, 2, 0, {0xf8, 0xf9, 0xfa, 0xfb,}},   
   { "PLAY MODE", {"COINED PLAY", "FREE PLAY", "NONE1 ", "NONE2"},
   1, 2, 0xFC, 0, 0, {0, 1, 0, 0 }},	 
DIP_END

DIP_START(llander_dips)
   { "FUEL UNITS", {"450", "600", "750 ", "900","1100","1300","1550","1800"},
   7, 1, 0xF8, 2, 0, {0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff }},   
   { "PLAY MODE", {"COINED PLAY", "FREE PLAY", "NONE1 ", "NONE2"},
   1, 2, 0xFC, 0, 0, {0xfd, 0xff, 0, 0 }},//fe free play
DIP_END

DIP_START(mhavocp_dips)
    { "LIVES ?", {"1", "2", "3", "4"},
   3, 1, 0xfc, 1, 0,{0x00, 0x01, 0x02, 0x03 }},
    { "LIVES", {"3", "4", "5", "6"},
   3, 1, 0x3f, 1, 0,{0x00, 0xc0, 0x80, 0x40 }},
   { "BONUS LIFE", {"50000", "100000", "200000", ""},
   2, 1, 0xf3, 1, 0,{0x0c, 0x04, 0x08, 0 }},
   { "DIFFICULTY", {"EASY", "MEDIUM", "HARD", "DEMO"},
   3, 1, 0xcf, 1, 0,{0x10,  0 , 0x30, 0x20}},
   { "COINAGE ", {"2 COINS 1 PLAY","1 COIN 1 PLAY","1 COIN 2 PLAYS","FREE PLAY"},
   3, 2, 0xfc, 1, 0, {0x00, 0x03, 0x00, 0x01 }},
   { "RIGHT COIN ", {"x1","x2","x3","x4"},
   3, 2, 0xf3, 0, 0, {0x0c, 0x08, 0x04, 0x00 }},
   { "LEFT COIN ", {"x1","x2","",""},
   1, 2, 0xef, 0, 0, {0x10, 0x00, 0, 0 }},
   { "BONUS CREDITS", {"2 EACH 4","1 EACH 3","1 EACH 4","1 EACH 5","NONE"},
   1, 2, 0x1f, 0, 0, {0x80, 0x40, 0xa0, 0x60,0xe0 }},
   { "2 CREDIT MIN", {"NO", "YES", " ", " "},
   1, 0x8888, 0, 0, 0,{0, 1, 0, 0, }},
DIP_END


DIP_START(starhawk_dips)
   { "GAME TIME", {"2:00/4:00","1:00/2:00","1:30/3:00","0:45/1:30"}, 
   3, DIPSW1, 0xfc, 0, 0, {0, 1, 2, 3 }},	  
DIP_END

DIP_START(armora_dips)
   { "JEEPS", {"2","3","4","5"}, 
   3, DIPSW1, 0xfc, 2, 2, {0x00, 0x02, 0x03, 0x01 }},
   { "DEMO SOUNDS", {"OFF", "ON", "", ""},
   1,  DIPSW1, 0xef, 0, 0,{0x10,  0 , 0, 0}},
DIP_END

DIP_START(warrior_dips)
 DIP_SET2(DIPSW1,0xf7,"Coins",0x00,"1 Coin",0x08,"2 Coins",0)
 DIP_SET2(DIPSW1,0xfb,"Image Align",0x04,"Off",0x08,"On",0)
 DIP_SET4(DIPSW1,0xfc,"Game Time",0x00,"120 Sec",0x02,"90 Sec",0x01,"60 Sec",0x03,"30 Sec",0) 
DIP_END

DIP_START(ripoff_dips)
 DIP_SET4(DIPSW1,0xfc,"Units",0x00,"8",0x01,"16",0x02,"4",0x03,"12",3)
 DIP_SET4(DIPSW1,0xf3,"Coins",0x08,"2Coins 1Credit",0x00,"4Coins 3Credits",0x0c,"1Coin 1Credit",0x04,"2Coins 3Credits",2)
 DIP_SET2(DIPSW1,0xef,"Attract Sound",0x10,"Off",0x00,"On",0)
 DIP_SET2(DIPSW1,0xdf,"Scores",0x20,"Individual",0x00,"Combined",0)
 DIP_SET2(DIPSW1,0xbf,"Diagnostic",0x00,"Off",0x40,"On",0)
DIP_END


#endif
