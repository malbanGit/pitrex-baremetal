static void loadHighScore();


unsigned short PCSTART=0x7CF3;
unsigned short NMISTART=0x7B65;
void game ( unsigned short opadd )
{


    switch(opadd)
    {

        default:
            printf("CASE %04X not defined\n",opadd);
            exit(0);


//L_6803:
case 0x6803: //20 FA 6E
clockticks+=6; //SUM
loadHighScore(); 
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x06;
    showme(0x6803,0x20);
    PCSTART=0x6EFA; //jsr
    /**/return;
    //goto L_6EFA;


//L_6806:
case 0x6806: //20 D8 6E
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x09;
    showme(0x6806,0x20);
    PCSTART=0x6ED8; //jsr
    /**/return;
    //goto L_6ED8;


//L_6809:
case 0x6809: //20 68 71
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x0C;
    showme(0x6809,0x20);
    PCSTART=0x7168; //jsr
    /**/return;
    //goto L_7168;


//L_680C:
case 0x680C: //AD 07 20
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2007); 
  ZN=A;


//L_680F:
case 0x680F: //30 FE 46
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x680F,0x30);
        PCSTART=0x680F;
        /**/return;
        //goto L_680F;
    }
//case 0x6811: //46 5B 90
    //clockticks+=5;
    value=ReadMemory(0x5B);
    C = value;
    value = value >> 1;
    WriteMemory(0x5B,value);
  ZN=value;


//L_6813:
case 0x6813: //90 F7 AD
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6813,0x90);
        PCSTART=0x680C;
        /**/return;
        //goto L_680C;
    } 


//L_6815:
case 0x6815: //AD 02 20
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2002); 
  ZN=A;


//L_6818:
case 0x6818: //30 FB AD
clockticks+=26; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6818,0x30);
        PCSTART=0x6815;
        /**/return;
        //goto L_6815;
    }
//case 0x681A: //AD 01 40
    //clockticks+=4;
    A=ReadMemory(0x4001); 
//  ZN=A;
//case 0x681D: //49 02 8D
    //clockticks+=3;
    A ^= 0x02;
//  ZN=A;
//case 0x681F: //8D 01 40
    //clockticks+=4;
    WriteMemory(0x4001,A);
//case 0x6822: //8D 00 30
    //clockticks+=4;
   if (dvg_draw_screen()) break;
//case 0x6825: //8D 00 34
    //clockticks+=4;
    WriteMemory(0x3400,A);
//case 0x6828: //E6 5C D0
    //clockticks+=5;
    value=ReadMemory(0x5C);
    value++;
    WriteMemory(0x5C,value);
  ZN=value;


//L_682A:
case 0x682A: //D0 02 E6
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x682A,0xD0);
        PCSTART=0x682E;
        /**/return;
        //goto L_682E;
    }
//case 0x682C: //E6 5D A2
    //clockticks+=5;
    value=ReadMemory(0x5D);
    value++;
    WriteMemory(0x5D,value);
  ZN=value;


//L_682E:
case 0x682E: //A2 40 29
clockticks+=6; //SUM
    //clockticks+=3;
    X=0x40;
//  ZN=X;
//case 0x6830: //29 02 D0
    //clockticks+=3;
    A &= 0x02;
  ZN=A;


//L_6832:
case 0x6832: //D0 02 A2
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6832,0xD0);
        PCSTART=0x6836;
        /**/return;
        //goto L_6836;
    }
//case 0x6834: //A2 44 A9
    //clockticks+=3;
    X=0x44;
  ZN=X;


//L_6836:
case 0x6836: //A9 02 85
clockticks+=7; //SUM
    //clockticks+=3;
    A=0x02;
  ZN=A;
//case 0x6838: //85 02 86
    //clockticks+=2;
    WriteMemory(0x02,A);
//case 0x683A: //86 03 20
    //clockticks+=2;
    WriteMemory(0x03,X);


//L_683C:
case 0x683C: //20 85 68
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x3F;
    showme(0x683C,0x20);
    PCSTART=0x6885; //jsr
    /**/return;
    //goto L_6885;


//L_683F:
case 0x683F: //B0 C2 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x683F,0xB0);
        PCSTART=0x6803;
        /**/return;
        //goto L_6803;
    } 


//L_6841:
case 0x6841: //20 5C 76
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x44;
    showme(0x6841,0x20);
    PCSTART=0x765C; //jsr
    /**/return;
    //goto L_765C;


//L_6844:
case 0x6844: //20 90 6D
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x47;
    showme(0x6844,0x20);
    PCSTART=0x6D90; //jsr
    /**/return;
    //goto L_6D90;


//L_6847:
case 0x6847: //10 1B 20
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6847,0x10);
        PCSTART=0x6864;
        /**/return;
        //goto L_6864;
    }


//L_6849:
case 0x6849: //20 C4 73
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x4C;
    showme(0x6849,0x20);
    PCSTART=0x73C4; //jsr
    /**/return;
    //goto L_73C4;


//L_684C:
case 0x684C: //B0 16 A5
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x684C,0xB0);
        PCSTART=0x6864;
        /**/return;
        //goto L_6864;
    } 
//case 0x684E: //A5 5A D0
    //clockticks+=3;
    A=ReadMemory(0x5A); 
  ZN=A;


//L_6850:
case 0x6850: //D0 0C 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6850,0xD0);
        PCSTART=0x685E;
        /**/return;
        //goto L_685E;
    }


//L_6852:
case 0x6852: //20 D7 6C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x55;
    showme(0x6852,0x20);
    PCSTART=0x6CD7; //jsr
    /**/return;
    //goto L_6CD7;


//L_6855:
case 0x6855: //20 74 6E
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x58;
    showme(0x6855,0x20);
    PCSTART=0x6E74; //jsr
    /**/return;
    //goto L_6E74;


//L_6858:
case 0x6858: //20 3F 70
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x5B;
    showme(0x6858,0x20);
    PCSTART=0x703F; //jsr
    /**/return;
    //goto L_703F;


//L_685B:
case 0x685B: //20 93 6B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x5E;
    showme(0x685B,0x20);
    PCSTART=0x6B93; //jsr
    /**/return;
    //goto L_6B93;


//L_685E:
case 0x685E: //20 57 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x61;
    showme(0x685E,0x20);
    PCSTART=0x6F57; //jsr
    /**/return;
    //goto L_6F57;


//L_6861:
case 0x6861: //20 F0 69
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x64;
    showme(0x6861,0x20);
    PCSTART=0x69F0; //jsr
    /**/return;
    //goto L_69F0;


//L_6864:
case 0x6864: //20 4F 72
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x67;
    showme(0x6864,0x20);
    PCSTART=0x724F; //jsr
    /**/return;
    //goto L_724F;


//L_6867:
case 0x6867: //20 55 75
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x6A;
    showme(0x6867,0x20);
    PCSTART=0x7555; //jsr
    /**/return;
    //goto L_7555;


//L_686A:
case 0x686A: //A9 7F AA
clockticks+=5; //SUM
    //clockticks+=3;
    A=0x7F;
//  ZN=A;
//case 0x686C: //AA 20 03
    //clockticks+=2;
    X=A;
  ZN=X;


//L_686D:
case 0x686D: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x70;
    showme(0x686D,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_6870:
case 0x6870: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x73;
    showme(0x6870,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_6873:
case 0x6873: //20 C0 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x76;
    showme(0x6873,0x20);
    PCSTART=0x7BC0; //jsr
    /**/return;
    //goto L_7BC0;


//L_6876:
case 0x6876: //AD FB 02
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x02FB); 
  ZN=A;


//L_6879:
case 0x6879: //F0 03 CE
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6879,0xF0);
        PCSTART=0x687E;
        /**/return;
        //goto L_687E;
    } 
//case 0x687B: //CE FB 02
    //clockticks+=6;
    value=ReadMemory(0x02FB);
    value--;
    WriteMemory(0x02FB,value);
  ZN=value;


//L_687E:
case 0x687E: //0D F6 02
clockticks+=4; //SUM
    //clockticks+=4;
    A |= ReadMemory(0x02F6);
  ZN=A;


//L_6881:
case 0x6881: //D0 89 F0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6881,0xD0);
        PCSTART=0x680C;
        /**/return;
        //goto L_680C;
    }


//L_6883:
case 0x6883: //F0 84 A5
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6883,0xF0);
        PCSTART=0x6809;
        /**/return;
        //goto L_6809;
    } 


//L_6885:
case 0x6885: //A5 1C F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_6887:
case 0x6887: //F0 14 A5
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6887,0xF0);
        PCSTART=0x689D;
        /**/return;
        //goto L_689D;
    } 
//case 0x6889: //A5 5A D0
    //clockticks+=3;
    A=ReadMemory(0x5A); 
  ZN=A;


//L_688B:
case 0x688B: //D0 03 4C
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x688B,0xD0);
        PCSTART=0x6890;
        /**/return;
        //goto L_6890;
    }


//L_688D:
case 0x688D: //4C 60 69
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x688D,0x4C);
    PCSTART=0x6960;
    /**/return;
    //goto L_6960;


//L_6890:
case 0x6890: //C6 5A 20
clockticks+=5; //SUM
    //clockticks+=5;
    value=ReadMemory(0x5A);
    value--;
    WriteMemory(0x5A,value);
  ZN=value;


//L_6892:
case 0x6892: //20 E2 69
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0x95;
    showme(0x6892,0x20);
    PCSTART=0x69E2; //jsr
    /**/return;
    //goto L_69E2;


//L_6895:
case 0x6895: //18 60 A9
clockticks+=2; //SUM
    //clockticks+=2;
    C = 0;


//L_6896:
case 0x6896: //60 A9 02
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6896,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6897:
case 0x6897: //A9 02 85
clockticks+=5; //SUM
    //clockticks+=3;
    A=0x02;
  ZN=A;
//case 0x6899: //85 70 D0
    //clockticks+=2;
    WriteMemory(0x70,A);


//L_689B:
case 0x689B: //D0 13 A5
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x689B,0xD0);
        PCSTART=0x68B0;
        /**/return;
        //goto L_68B0;
    }


//L_689D:
case 0x689D: //A5 71 29
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x71); 
//  ZN=A;
//case 0x689F: //29 03 F0
    //clockticks+=3;
    A &= 0x03;
  ZN=A;


//L_68A1:
case 0x68A1: //F0 F4 18
clockticks+=15; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x68A1,0xF0);
        PCSTART=0x6897;
        /**/return;
        //goto L_6897;
    } 
//case 0x68A3: //18 69 07
    //clockticks+=2;
    C = 0;
//case 0x68A4: //69 07 A8
    //clockticks+=3;
    ADC(0x07);
//case 0x68A6: //A8 A5 32
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x68A7: //A5 32 25
    //clockticks+=3;
    A=ReadMemory(0x32); 
//  ZN=A;
//case 0x68A9: //25 33 10
    //clockticks+=3;
    A &= ReadMemory(0x33);
  ZN=A;


//L_68AB:
case 0x68AB: //10 03 20
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x68AB,0x10);
        PCSTART=0x68B0;
        /**/return;
        //goto L_68B0;
    }


//L_68AD:
case 0x68AD: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0xB0;
    showme(0x68AD,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_68B0:
case 0x68B0: //A4 70 F0
clockticks+=3; //SUM
    //clockticks+=3;
    Y=ReadMemory(0x70);
  ZN=Y;


//L_68B2:
case 0x68B2: //F0 E1 A2
clockticks+=9; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x68B2,0xF0);
        PCSTART=0x6895;
        /**/return;
        //goto L_6895;
    } 
//case 0x68B4: //A2 01 AD
    //clockticks+=3;
    X=0x01;
//  ZN=X;
//case 0x68B6: //AD 03 24
    //clockticks+=4;
    A=ReadMemory(0x2403); 
  ZN=A;


//L_68B9:
case 0x68B9: //30 23 C0
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x68B9,0x30);
        PCSTART=0x68DE;
        /**/return;
        //goto L_68DE;
    }
//case 0x68BB: //C0 02 90
    //clockticks+=3;
    temp=Y; temp+=0x100; temp-=0x02;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_68BD:
case 0x68BD: //90 7C AD
clockticks+=6; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x68BD,0x90);
        PCSTART=0x693B;
        /**/return;
        //goto L_693B;
    } 
//case 0x68BF: //AD 04 24
    //clockticks+=4;
    A=ReadMemory(0x2404); 
  ZN=A;


//L_68C2:
case 0x68C2: //10 77 A5
clockticks+=14; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x68C2,0x10);
        PCSTART=0x693B;
        /**/return;
        //goto L_693B;
    }
//case 0x68C4: //A5 6F 09
    //clockticks+=3;
    A=ReadMemory(0x6F); 
//  ZN=A;
//case 0x68C6: //09 04 85
    //clockticks+=3;
    A |= 0x04;
  ZN=A;
//case 0x68C8: //85 6F 8D
    //clockticks+=2;
    WriteMemory(0x6F,A);
//case 0x68CA: //8D 00 32
    //clockticks+=4;
    WriteMemory3200(A);


//L_68CD:
case 0x68CD: //20 D8 6E
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0xD0;
    showme(0x68CD,0x20);
    PCSTART=0x6ED8; //jsr
    /**/return;
    //goto L_6ED8;


//L_68D0:
case 0x68D0: //20 68 71
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0xD3;
    showme(0x68D0,0x20);
    PCSTART=0x7168; //jsr
    /**/return;
    //goto L_7168;


//L_68D3:
case 0x68D3: //20 E8 71
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0xD6;
    showme(0x68D3,0x20);
    PCSTART=0x71E8; //jsr
    /**/return;
    //goto L_71E8;


//L_68D6:
case 0x68D6: //A5 56 85
clockticks+=13; //SUM
    //clockticks+=3;
    A=ReadMemory(0x56); 
//  ZN=A;
//case 0x68D8: //85 58 A2
    //clockticks+=2;
    WriteMemory(0x58,A);
//case 0x68DA: //A2 02 C6
    //clockticks+=3;
    X=0x02;
//  ZN=X;
//case 0x68DC: //C6 70 86
    //clockticks+=5;
    value=ReadMemory(0x70);
    value--;
    WriteMemory(0x70,value);
  ZN=value;


//L_68DE:
case 0x68DE: //86 1C C6
clockticks+=22; //SUM
    //clockticks+=2;
    WriteMemory(0x1C,X);
//case 0x68E0: //C6 70 A5
    //clockticks+=5;
    value=ReadMemory(0x70);
    value--;
    WriteMemory(0x70,value);
//  ZN=value;
//case 0x68E2: //A5 6F 29
    //clockticks+=3;
    A=ReadMemory(0x6F); 
//  ZN=A;
//case 0x68E4: //29 F8 45
    //clockticks+=3;
    A &= 0xF8;
//  ZN=A;
//case 0x68E6: //45 1C 85
    //clockticks+=3;
    A ^= ReadMemory(0x1C);
  ZN=A;
//case 0x68E8: //85 6F 8D
    //clockticks+=2;
    WriteMemory(0x6F,A);
//case 0x68EA: //8D 00 32
    //clockticks+=4;
    WriteMemory3200(A);


//L_68ED:
case 0x68ED: //20 E8 71
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x68;
    stack[S--]=0xF0;
    showme(0x68ED,0x20);
    PCSTART=0x71E8; //jsr
    /**/return;
    //goto L_71E8;


//L_68F0:
case 0x68F0: //A9 01 8D
clockticks+=97; //SUM
    //clockticks+=3;
    A=0x01;
//  ZN=A;
//case 0x68F2: //8D FA 02
    //clockticks+=4;
    WriteMemory(0x02FA,A);
//case 0x68F5: //8D FA 03
    //clockticks+=4;
    WriteMemory(0x03FA,A);
//case 0x68F8: //A9 92 8D
    //clockticks+=3;
    A=0x92;
//  ZN=A;
//case 0x68FA: //8D F8 02
    //clockticks+=4;
    WriteMemory(0x02F8,A);
//case 0x68FD: //8D F8 03
    //clockticks+=4;
    WriteMemory(0x03F8,A);
//case 0x6900: //8D F7 03
    //clockticks+=4;
    WriteMemory(0x03F7,A);
//case 0x6903: //8D F7 02
    //clockticks+=4;
    WriteMemory(0x02F7,A);
//case 0x6906: //A9 7F 8D
    //clockticks+=3;
    A=0x7F;
//  ZN=A;
//case 0x6908: //8D FB 02
    //clockticks+=4;
    WriteMemory(0x02FB,A);
//case 0x690B: //8D FB 03
    //clockticks+=4;
    WriteMemory(0x03FB,A);
//case 0x690E: //A9 05 8D
    //clockticks+=3;
    A=0x05;
//  ZN=A;
//case 0x6910: //8D FD 02
    //clockticks+=4;
    WriteMemory(0x02FD,A);
//case 0x6913: //8D FD 03
    //clockticks+=4;
    WriteMemory(0x03FD,A);
//case 0x6916: //A9 FF 85
    //clockticks+=3;
    A=0xFF;
//  ZN=A;
//case 0x6918: //85 32 85
    //clockticks+=2;
    WriteMemory(0x32,A);
//case 0x691A: //85 33 A9
    //clockticks+=2;
    WriteMemory(0x33,A);
//case 0x691C: //A9 80 85
    //clockticks+=3;
    A=0x80;
//  ZN=A;
//case 0x691E: //85 5A 0A
    //clockticks+=2;
    WriteMemory(0x5A,A);
//case 0x6920: //0A 85 18
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x6921: //85 18 85
    //clockticks+=2;
    WriteMemory(0x18,A);
//case 0x6923: //85 19 A5
    //clockticks+=2;
    WriteMemory(0x19,A);
//case 0x6925: //A5 56 85
    //clockticks+=3;
    A=ReadMemory(0x56); 
//  ZN=A;
//case 0x6927: //85 57 A9
    //clockticks+=2;
    WriteMemory(0x57,A);
//case 0x6929: //A9 04 85
    //clockticks+=3;
    A=0x04;
//  ZN=A;
//case 0x692B: //85 6C 85
    //clockticks+=2;
    WriteMemory(0x6C,A);
//case 0x692D: //85 6E A9
    //clockticks+=2;
    WriteMemory(0x6E,A);
//case 0x692F: //A9 30 8D
    //clockticks+=3;
    A=0x30;
  ZN=A;
//case 0x6931: //8D FC 02
    //clockticks+=4;
    WriteMemory(0x02FC,A);
//case 0x6934: //8D FC 03
    //clockticks+=4;
    WriteMemory(0x03FC,A);
//case 0x6937: //8D 00 3E
    //clockticks+=4;
    WriteMemory(0x3E00,A);


//L_693A:
case 0x693A: //60 A5 32
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x693A,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_693B:
case 0x693B: //A5 32 25
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x32); 
//  ZN=A;
//case 0x693D: //25 32 10
    //clockticks+=3;
    A &= ReadMemory(0x32);
  ZN=A;


//L_693F:
case 0x693F: //10 0B A5
clockticks+=8; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x693F,0x10);
        PCSTART=0x694C;
        /**/return;
        //goto L_694C;
    }
//case 0x6941: //A5 5C 29
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x6943: //29 20 D0
    //clockticks+=3;
    A &= 0x20;
  ZN=A;


//L_6945:
case 0x6945: //D0 05 A0
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6945,0xD0);
        PCSTART=0x694C;
        /**/return;
        //goto L_694C;
    }
//case 0x6947: //A0 06 20
    //clockticks+=3;
    Y = 0x06; 
  ZN=Y;


//L_6949:
case 0x6949: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x69;
    stack[S--]=0x4C;
    showme(0x6949,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_694C:
case 0x694C: //A5 5C 29
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x694E: //29 0F D0
    //clockticks+=3;
    A &= 0x0F;
  ZN=A;


//L_6950:
case 0x6950: //D0 0C A9
clockticks+=19; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6950,0xD0);
        PCSTART=0x695E;
        /**/return;
        //goto L_695E;
    }
//case 0x6952: //A9 01 C5
    //clockticks+=3;
    A=0x01;
//  ZN=A;
//case 0x6954: //C5 70 69
    //clockticks+=3;
    value=ReadMemory(0x70);
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
//  ZN=value;
//case 0x6956: //69 01 49
    //clockticks+=3;
    ADC(0x01);
//case 0x6958: //49 01 45
    //clockticks+=3;
    A ^= 0x01;
//  ZN=A;
//case 0x695A: //45 6F 85
    //clockticks+=3;
    A ^= ReadMemory(0x6F);
  ZN=A;
//case 0x695C: //85 6F 18
    //clockticks+=2;
    WriteMemory(0x6F,A);


//L_695E:
case 0x695E: //18 60 A5
clockticks+=2; //SUM
    //clockticks+=2;
    C = 0;


//L_695F:
case 0x695F: //60 A5 5C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x695F,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6960:
case 0x6960: //A5 5C 29
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x6962: //29 3F D0
    //clockticks+=3;
    A &= 0x3F;
  ZN=A;


//L_6964:
case 0x6964: //D0 0A AD
clockticks+=9; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6964,0xD0);
        PCSTART=0x6970;
        /**/return;
        //goto L_6970;
    }
//case 0x6966: //AD FC 02
    //clockticks+=4;
    A=ReadMemory(0x02FC); 
//  ZN=A;
//case 0x6969: //C9 08 F0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x08;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_696B:
case 0x696B: //F0 03 CE
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x696B,0xF0);
        PCSTART=0x6970;
        /**/return;
        //goto L_6970;
    } 
//case 0x696D: //CE FC 02
    //clockticks+=6;
    value=ReadMemory(0x02FC);
    value--;
    WriteMemory(0x02FC,value);
  ZN=value;


//L_6970:
case 0x6970: //A6 18 B5
clockticks+=7; //SUM
    //clockticks+=3;
    X=ReadMemory(0x18); 
//  ZN=X;
//case 0x6972: //B5 57 D0
    //clockticks+=4;
    temp=0x0057; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_6974:
case 0x6974: //D0 1C AD
clockticks+=18; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6974,0xD0);
        PCSTART=0x6992;
        /**/return;
        //goto L_6992;
    }
//case 0x6976: //AD 1F 02
    //clockticks+=4;
    A=ReadMemory(0x021F); 
//  ZN=A;
//case 0x6979: //0D 20 02
    //clockticks+=4;
    A |= ReadMemory(0x0220);
//  ZN=A;
//case 0x697C: //0D 21 02
    //clockticks+=4;
    A |= ReadMemory(0x0221);
//  ZN=A;
//case 0x697F: //0D 22 02
    //clockticks+=4;
    A |= ReadMemory(0x0222);
  ZN=A;


//L_6982:
case 0x6982: //D0 0E A0
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6982,0xD0);
        PCSTART=0x6992;
        /**/return;
        //goto L_6992;
    }
//case 0x6984: //A0 07 20
    //clockticks+=3;
    Y = 0x07; 
  ZN=Y;


//L_6986:
case 0x6986: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x69;
    stack[S--]=0x89;
    showme(0x6986,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_6989:
case 0x6989: //A5 1C C9
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
//  ZN=A;
//case 0x698B: //C9 02 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x02;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_698D:
case 0x698D: //90 03 20
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x698D,0x90);
        PCSTART=0x6992;
        /**/return;
        //goto L_6992;
    } 


//L_698F:
case 0x698F: //20 E2 69
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x69;
    stack[S--]=0x92;
    showme(0x698F,0x20);
    PCSTART=0x69E2; //jsr
    /**/return;
    //goto L_69E2;


//L_6992:
case 0x6992: //AD 1B 02
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x021B); 
  ZN=A;


//L_6995:
case 0x6995: //D0 36 AD
clockticks+=9; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6995,0xD0);
        PCSTART=0x69CD;
        /**/return;
        //goto L_69CD;
    }
//case 0x6997: //AD FA 02
    //clockticks+=4;
    A=ReadMemory(0x02FA); 
//  ZN=A;
//case 0x699A: //C9 80 D0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_699C:
case 0x699C: //D0 2F A9
clockticks+=18; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x699C,0xD0);
        PCSTART=0x69CD;
        /**/return;
        //goto L_69CD;
    }
//case 0x699E: //A9 10 8D
    //clockticks+=3;
    A=0x10;
//  ZN=A;
//case 0x69A0: //8D FA 02
    //clockticks+=4;
    WriteMemory(0x02FA,A);
//case 0x69A3: //A6 1C A5
    //clockticks+=3;
    X=ReadMemory(0x1C); 
//  ZN=X;
//case 0x69A5: //A5 57 05
    //clockticks+=3;
    A=ReadMemory(0x57); 
//  ZN=A;
//case 0x69A7: //05 58 F0
    //clockticks+=3;
    A |= ReadMemory(0x58);
  ZN=A;


//L_69A9:
case 0x69A9: //F0 24 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x69A9,0xF0);
        PCSTART=0x69CF;
        /**/return;
        //goto L_69CF;
    } 


//L_69AB:
case 0x69AB: //20 2D 70
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x69;
    stack[S--]=0xAE;
    showme(0x69AB,0x20);
    PCSTART=0x702D; //jsr
    /**/return;
    //goto L_702D;


//L_69AE:
case 0x69AE: //CA F0 1C
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_69AF:
case 0x69AF: //F0 1C A9
clockticks+=19; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x69AF,0xF0);
        PCSTART=0x69CD;
        /**/return;
        //goto L_69CD;
    } 
//case 0x69B1: //A9 80 85
    //clockticks+=3;
    A=0x80;
//  ZN=A;
//case 0x69B3: //85 5A A5
    //clockticks+=2;
    WriteMemory(0x5A,A);
//case 0x69B5: //A5 18 49
    //clockticks+=3;
    A=ReadMemory(0x18); 
//  ZN=A;
//case 0x69B7: //49 01 AA
    //clockticks+=3;
    A ^= 0x01;
//  ZN=A;
//case 0x69B9: //AA B5 57
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x69BA: //B5 57 F0
    //clockticks+=4;
    temp=0x0057; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_69BC:
case 0x69BC: //F0 0F 86
clockticks+=22; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x69BC,0xF0);
        PCSTART=0x69CD;
        /**/return;
        //goto L_69CD;
    } 
//case 0x69BE: //86 18 A9
    //clockticks+=2;
    WriteMemory(0x18,X);
//case 0x69C0: //A9 04 45
    //clockticks+=3;
    A=0x04;
//  ZN=A;
//case 0x69C2: //45 6F 85
    //clockticks+=3;
    A ^= ReadMemory(0x6F);
//  ZN=A;
//case 0x69C4: //85 6F 8D
    //clockticks+=2;
    WriteMemory(0x6F,A);
//case 0x69C6: //8D 00 32
    //clockticks+=4;
    WriteMemory3200(A);
//case 0x69C9: //8A 0A 85
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x69CA: //0A 85 19
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
  ZN=A;
//case 0x69CB: //85 19 18
    //clockticks+=2;
    WriteMemory(0x19,A);


//L_69CD:
case 0x69CD: //18 60 86
clockticks+=2; //SUM
    //clockticks+=2;
    C = 0;


//L_69CE:
case 0x69CE: //60 86 1A
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x69CE,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_69CF:
case 0x69CF: //86 1A A9
clockticks+=7; //SUM
    //clockticks+=2;
    WriteMemory(0x1A,X);
//case 0x69D1: //A9 FF 85
    //clockticks+=3;
    A=0xFF;
  ZN=A;
//case 0x69D3: //85 1C 20
    //clockticks+=2;
    WriteMemory(0x1C,A);


//L_69D5:
case 0x69D5: //20 FA 6E
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x69;
    stack[S--]=0xD8;
    showme(0x69D5,0x20);
    PCSTART=0x6EFA; //jsr
    /**/return;
    //goto L_6EFA;


//L_69D8:
case 0x69D8: //A5 6F 29
clockticks+=13; //SUM
    //clockticks+=3;
    A=ReadMemory(0x6F); 
//  ZN=A;
//case 0x69DA: //29 F8 09
    //clockticks+=3;
    A &= 0xF8;
//  ZN=A;
//case 0x69DC: //09 03 85
    //clockticks+=3;
    A |= 0x03;
  ZN=A;
//case 0x69DE: //85 6F 18
    //clockticks+=2;
    WriteMemory(0x6F,A);
//case 0x69E0: //18 60 A0
    //clockticks+=2;
    C = 0;


//L_69E1:
case 0x69E1: //60 A0 01
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x69E1,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_69E2:
case 0x69E2: //A0 01 20
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x01; 
  ZN=Y;


//L_69E4:
case 0x69E4: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x69;
    stack[S--]=0xE7;
    showme(0x69E4,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_69E7:
case 0x69E7: //A4 18 C8
clockticks+=7; //SUM
    //clockticks+=3;
    Y=ReadMemory(0x18);
//  ZN=Y;
//case 0x69E9: //C8 98 20
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x69EA: //98 20 D1
    //clockticks+=2;
    A = Y;
  ZN=A;


//L_69EB:
case 0x69EB: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x69;
    stack[S--]=0xEE;
    showme(0x69EB,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_69EE:
case 0x69EE: //60 71 A2
clockticks+=0; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x69EE,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_69F0:
case 0x69F0: //A2 07 BD
clockticks+=3; //SUM
    //clockticks+=3;
    X=0x07;
  ZN=X;


//L_69F2:
case 0x69F2: //BD 1B 02
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x021B; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_69F5:
case 0x69F5: //F0 02 10
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x69F5,0xF0);
        PCSTART=0x69F9;
        /**/return;
        //goto L_69F9;
    } 


//L_69F7:
case 0x69F7: //10 04 CA
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x69F7,0x10);
        PCSTART=0x69FD;
        /**/return;
        //goto L_69FD;
    }


//L_69F9:
case 0x69F9: //CA 10 F6
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_69FA:
case 0x69FA: //10 F6 60
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x69FA,0x10);
        PCSTART=0x69F2;
        /**/return;
        //goto L_69F2;
    }


//L_69FC:
case 0x69FC: //60 A0 1C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x69FC,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_69FD:
case 0x69FD: //A0 1C E0
clockticks+=6; //SUM
    //clockticks+=3;
    Y = 0x1C; 
//  ZN=Y;
//case 0x69FF: //E0 04 B0
    //clockticks+=3;
    value = 0x04;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6A01:
case 0x6A01: //B0 07 88
clockticks+=6; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6A01,0xB0);
        PCSTART=0x6A0A;
        /**/return;
        //goto L_6A0A;
    } 
//case 0x6A03: //88 8A D0
    //clockticks+=2;
    Y--;
//  ZN=Y;
//case 0x6A04: //8A D0 03
    //clockticks+=2;
    A = X; 
  ZN=A;


//L_6A05:
case 0x6A05: //D0 03 88
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6A05,0xD0);
        PCSTART=0x6A0A;
        /**/return;
        //goto L_6A0A;
    }


//L_6A07:
case 0x6A07: //88 30 EF
clockticks+=2; //SUM
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_6A08:
case 0x6A08: //30 EF B9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6A08,0x30);
        PCSTART=0x69F9;
        /**/return;
        //goto L_69F9;
    }


//L_6A0A:
case 0x6A0A: //B9 00 02
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0200; temp+=Y;
    A=ReadMemory(temp);
  ZN=A;


//L_6A0D:
case 0x6A0D: //F0 F8 30
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6A0D,0xF0);
        PCSTART=0x6A07;
        /**/return;
        //goto L_6A07;
    } 


//L_6A0F:
case 0x6A0F: //30 F6 85
clockticks+=33; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6A0F,0x30);
        PCSTART=0x6A07;
        /**/return;
        //goto L_6A07;
    }
//case 0x6A11: //85 0B B9
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x6A13: //B9 AF 02
    //clockticks+=4;
    temp=0x02AF; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6A16: //38 FD CA
    //clockticks+=2;
    C = 0x01;
//case 0x6A17: //FD CA 02
    //clockticks+=4;
    temp=0x02CA; temp+=X;
    SBC(ReadMemory(temp));
//case 0x6A1A: //85 08 B9
    //clockticks+=2;
    WriteMemory(0x08,A);
//case 0x6A1C: //B9 69 02
    //clockticks+=4;
    temp=0x0269; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6A1F: //FD 84 02
    //clockticks+=4;
    temp=0x0284; temp+=X;
    SBC(ReadMemory(temp));
//case 0x6A22: //4A 66 08
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6A23: //66 08 0A
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x08);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x08,value);
//    ZN=value;
//case 0x6A25: //0A F0 0C
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
  ZN=A;


//L_6A26:
case 0x6A26: //F0 0C 10
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6A26,0xF0);
        PCSTART=0x6A34;
        /**/return;
        //goto L_6A34;
    } 


//L_6A28:
case 0x6A28: //10 6D 49
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6A28,0x10);
        PCSTART=0x6A97;
        /**/return;
        //goto L_6A97;
    }
//case 0x6A2A: //49 FE D0
    //clockticks+=3;
    A ^= 0xFE;
  ZN=A;


//L_6A2C:
case 0x6A2C: //D0 69 A5
clockticks+=10; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6A2C,0xD0);
        PCSTART=0x6A97;
        /**/return;
        //goto L_6A97;
    }
//case 0x6A2E: //A5 08 49
    //clockticks+=3;
    A=ReadMemory(0x08); 
//  ZN=A;
//case 0x6A30: //49 FF 85
    //clockticks+=3;
    A ^= 0xFF;
  ZN=A;
//case 0x6A32: //85 08 B9
    //clockticks+=2;
    WriteMemory(0x08,A);


//L_6A34:
case 0x6A34: //B9 D2 02
clockticks+=29; //SUM
    //clockticks+=4;
    temp=0x02D2; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6A37: //38 FD ED
    //clockticks+=2;
    C = 0x01;
//case 0x6A38: //FD ED 02
    //clockticks+=4;
    temp=0x02ED; temp+=X;
    SBC(ReadMemory(temp));
//case 0x6A3B: //85 09 B9
    //clockticks+=2;
    WriteMemory(0x09,A);
//case 0x6A3D: //B9 8C 02
    //clockticks+=4;
    temp=0x028C; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6A40: //FD A7 02
    //clockticks+=4;
    temp=0x02A7; temp+=X;
    SBC(ReadMemory(temp));
//case 0x6A43: //4A 66 09
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6A44: //66 09 0A
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x09);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x09,value);
//    ZN=value;
//case 0x6A46: //0A F0 0C
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
  ZN=A;


//L_6A47:
case 0x6A47: //F0 0C 10
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6A47,0xF0);
        PCSTART=0x6A55;
        /**/return;
        //goto L_6A55;
    } 


//L_6A49:
case 0x6A49: //10 4C 49
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6A49,0x10);
        PCSTART=0x6A97;
        /**/return;
        //goto L_6A97;
    }
//case 0x6A4B: //49 FE D0
    //clockticks+=3;
    A ^= 0xFE;
  ZN=A;


//L_6A4D:
case 0x6A4D: //D0 48 A5
clockticks+=10; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6A4D,0xD0);
        PCSTART=0x6A97;
        /**/return;
        //goto L_6A97;
    }
//case 0x6A4F: //A5 09 49
    //clockticks+=3;
    A=ReadMemory(0x09); 
//  ZN=A;
//case 0x6A51: //49 FF 85
    //clockticks+=3;
    A ^= 0xFF;
  ZN=A;
//case 0x6A53: //85 09 A9
    //clockticks+=2;
    WriteMemory(0x09,A);


//L_6A55:
case 0x6A55: //A9 2A 46
clockticks+=8; //SUM
    //clockticks+=3;
    A=0x2A;
//  ZN=A;
//case 0x6A57: //46 0B B0
    //clockticks+=5;
    value=ReadMemory(0x0B);
    C = value;
    value = value >> 1;
    WriteMemory(0x0B,value);
  ZN=value;


//L_6A59:
case 0x6A59: //B0 08 A9
clockticks+=10; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6A59,0xB0);
        PCSTART=0x6A63;
        /**/return;
        //goto L_6A63;
    } 
//case 0x6A5B: //A9 48 46
    //clockticks+=3;
    A=0x48;
//  ZN=A;
//case 0x6A5D: //46 0B B0
    //clockticks+=5;
    value=ReadMemory(0x0B);
    C = value;
    value = value >> 1;
    WriteMemory(0x0B,value);
  ZN=value;


//L_6A5F:
case 0x6A5F: //B0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6A5F,0xB0);
        PCSTART=0x6A63;
        /**/return;
        //goto L_6A63;
    } 
//case 0x6A61: //A9 84 E0
    //clockticks+=3;
    A=0x84;
  ZN=A;


//L_6A63:
case 0x6A63: //E0 01 B0
clockticks+=3; //SUM
    //clockticks+=3;
    value = 0x01;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6A65:
case 0x6A65: //B0 02 69
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6A65,0xB0);
        PCSTART=0x6A69;
        /**/return;
        //goto L_6A69;
    } 
//case 0x6A67: //69 1C D0
    //clockticks+=3;
    ADC(0x1C);


//L_6A69:
case 0x6A69: //D0 0C 69
clockticks+=11; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6A69,0xD0);
        PCSTART=0x6A77;
        /**/return;
        //goto L_6A77;
    }
//case 0x6A6B: //69 12 AE
    //clockticks+=3;
    ADC(0x12);
//case 0x6A6D: //AE 1C 02
    //clockticks+=4;
    X=ReadMemory(0x021C);
//  ZN=X;
//case 0x6A70: //CA F0 02
    //clockticks+=2;
    X--; 
  ZN=X;


//L_6A71:
case 0x6A71: //F0 02 69
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6A71,0xF0);
        PCSTART=0x6A75;
        /**/return;
        //goto L_6A75;
    } 
//case 0x6A73: //69 12 A2
    //clockticks+=3;
    ADC(0x12);


//L_6A75:
case 0x6A75: //A2 01 C5
clockticks+=3; //SUM
    //clockticks+=3;
    X=0x01;
  ZN=X;


//L_6A77:
case 0x6A77: //C5 08 90
clockticks+=3; //SUM
    //clockticks+=3;
    value=ReadMemory(0x08);
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6A79:
case 0x6A79: //90 1C C5
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6A79,0x90);
        PCSTART=0x6A97;
        /**/return;
        //goto L_6A97;
    } 
//case 0x6A7B: //C5 09 90
    //clockticks+=3;
    value=ReadMemory(0x09);
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6A7D:
case 0x6A7D: //90 18 85
clockticks+=19; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6A7D,0x90);
        PCSTART=0x6A97;
        /**/return;
        //goto L_6A97;
    } 
//case 0x6A7F: //85 0B 4A
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x6A81: //4A 18 65
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6A82: //18 65 0B
    //clockticks+=2;
    C = 0;
//case 0x6A83: //65 0B 85
    //clockticks+=3;
    ADC(ReadMemory(0x0B));
//case 0x6A85: //85 0B A5
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x6A87: //A5 09 65
    //clockticks+=3;
    A=ReadMemory(0x09); 
//  ZN=A;
//case 0x6A89: //65 08 B0
    //clockticks+=3;
    ADC(ReadMemory(0x08));


//L_6A8B:
case 0x6A8B: //B0 0A C5
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6A8B,0xB0);
        PCSTART=0x6A97;
        /**/return;
        //goto L_6A97;
    } 
//case 0x6A8D: //C5 0B B0
    //clockticks+=3;
    value=ReadMemory(0x0B);
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6A8F:
case 0x6A8F: //B0 06 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6A8F,0xB0);
        PCSTART=0x6A97;
        /**/return;
        //goto L_6A97;
    } 


//L_6A91:
case 0x6A91: //20 0F 6B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6A;
    stack[S--]=0x94;
    showme(0x6A91,0x20);
    PCSTART=0x6B0F; //jsr
    /**/return;
    //goto L_6B0F;


//L_6A94:
case 0x6A94: //4C F9 69
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6A94,0x4C);
    PCSTART=0x69F9;
    /**/return;
    //goto L_69F9;


//L_6A97:
case 0x6A97: //88 30 FA
clockticks+=2; //SUM
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_6A98:
case 0x6A98: //30 FA 4C
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6A98,0x30);
        PCSTART=0x6A94;
        /**/return;
        //goto L_6A94;
    }


//L_6A9A:
case 0x6A9A: //4C 0A 6A
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6A9A,0x4C);
    PCSTART=0x6A0A;
    /**/return;
    //goto L_6A0A;


//L_6A9D:
case 0x6A9D: //B9 00 02
clockticks+=9; //SUM
    //clockticks+=4;
    temp=0x0200; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6AA0: //29 07 85
    //clockticks+=3;
    A &= 0x07;
  ZN=A;
//case 0x6AA2: //85 08 20
    //clockticks+=2;
    WriteMemory(0x08,A);


//L_6AA4:
case 0x6AA4: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6A;
    stack[S--]=0xA7;
    showme(0x6AA4,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_6AA7:
case 0x6AA7: //29 18 05
clockticks+=65; //SUM
    //clockticks+=3;
    A &= 0x18;
//  ZN=A;
//case 0x6AA9: //05 08 9D
    //clockticks+=3;
    A |= ReadMemory(0x08);
//  ZN=A;
//case 0x6AAB: //9D 00 02
    //clockticks+=5;
    temp=0x0200; temp+=X;
    WriteMemory(temp,A);
//case 0x6AAE: //B9 AF 02
    //clockticks+=4;
    temp=0x02AF; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6AB1: //9D AF 02
    //clockticks+=5;
    temp=0x02AF; temp+=X;
    WriteMemory(temp,A);
//case 0x6AB4: //B9 69 02
    //clockticks+=4;
    temp=0x0269; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6AB7: //9D 69 02
    //clockticks+=5;
    temp=0x0269; temp+=X;
    WriteMemory(temp,A);
//case 0x6ABA: //B9 D2 02
    //clockticks+=4;
    temp=0x02D2; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6ABD: //9D D2 02
    //clockticks+=5;
    temp=0x02D2; temp+=X;
    WriteMemory(temp,A);
//case 0x6AC0: //B9 8C 02
    //clockticks+=4;
    temp=0x028C; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6AC3: //9D 8C 02
    //clockticks+=5;
    temp=0x028C; temp+=X;
    WriteMemory(temp,A);
//case 0x6AC6: //B9 23 02
    //clockticks+=4;
    temp=0x0223; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6AC9: //9D 23 02
    //clockticks+=5;
    temp=0x0223; temp+=X;
    WriteMemory(temp,A);
//case 0x6ACC: //B9 46 02
    //clockticks+=4;
    temp=0x0246; temp+=Y;
    A=ReadMemory(temp);
  ZN=A;
//case 0x6ACF: //9D 46 02
    //clockticks+=5;
    temp=0x0246; temp+=X;
    WriteMemory(temp,A);


//L_6AD2:
case 0x6AD2: //60 85 0B
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6AD2,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6AD3:
case 0x6AD3: //85 0B 86
clockticks+=4; //SUM
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x6AD5: //86 0C A0
    //clockticks+=2;
    WriteMemory(0x0C,X);


//L_6AD7:
case 0x6AD7: //A0 00 C8
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x00; 
  ZN=Y;


//L_6AD9:
case 0x6AD9: //C8 B1 0B
clockticks+=21; //SUM
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x6ADA: //B1 0B 45
    //clockticks+=5;
    temp=ReadMemory(0x0C);
    temp<<=8;
    temp|=ReadMemory(0x0B);
    temp=temp+Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6ADC: //45 09 91
    //clockticks+=3;
    A ^= ReadMemory(0x09);
//  ZN=A;
//case 0x6ADE: //91 02 88
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x6AE0: //88 C9 F0
    //clockticks+=2;
    Y--;
//  ZN=Y;
//case 0x6AE1: //C9 F0 B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xF0;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6AE3:
case 0x6AE3: //B0 1E C9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6AE3,0xB0);
        PCSTART=0x6B03;
        /**/return;
        //goto L_6B03;
    } 
//case 0x6AE5: //C9 A0 B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xA0;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6AE7:
case 0x6AE7: //B0 16 B1
clockticks+=47; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6AE7,0xB0);
        PCSTART=0x6AFF;
        /**/return;
        //goto L_6AFF;
    } 
//case 0x6AE9: //B1 0B 91
    //clockticks+=5;
    temp=ReadMemory(0x0C);
    temp<<=8;
    temp|=ReadMemory(0x0B);
    temp=temp+Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6AEB: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x6AED: //C8 C8 B1
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x6AEE: //C8 B1 0B
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x6AEF: //B1 0B 91
    //clockticks+=5;
    temp=ReadMemory(0x0C);
    temp<<=8;
    temp|=ReadMemory(0x0B);
    temp=temp+Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6AF1: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x6AF3: //C8 B1 0B
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x6AF4: //B1 0B 45
    //clockticks+=5;
    temp=ReadMemory(0x0C);
    temp<<=8;
    temp|=ReadMemory(0x0B);
    temp=temp+Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6AF6: //45 08 65
    //clockticks+=3;
    A ^= ReadMemory(0x08);
//  ZN=A;
//case 0x6AF8: //65 17 91
    //clockticks+=3;
    ADC(ReadMemory(0x17));
//case 0x6AFA: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);


//L_6AFC:
case 0x6AFC: //C8 D0 DA
clockticks+=2; //SUM
    //clockticks+=2;
    Y++; 
  ZN=Y;


//L_6AFD:
case 0x6AFD: //D0 DA 88
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6AFD,0xD0);
        PCSTART=0x6AD9;
        /**/return;
        //goto L_6AD9;
    }


//L_6AFF:
case 0x6AFF: //88 4C 39
clockticks+=2; //SUM
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_6B00:
case 0x6B00: //4C 39 7C
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6B00,0x4C);
    PCSTART=0x7C39;
    /**/return;
    //goto L_7C39;


//L_6B03:
case 0x6B03: //B1 0B 45
clockticks+=21; //SUM
    //clockticks+=5;
    temp=ReadMemory(0x0C);
    temp<<=8;
    temp|=ReadMemory(0x0B);
    temp=temp+Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6B05: //45 08 18
    //clockticks+=3;
    A ^= ReadMemory(0x08);
//  ZN=A;
//case 0x6B07: //18 65 17
    //clockticks+=2;
    C = 0;
//case 0x6B08: //65 17 91
    //clockticks+=3;
    ADC(ReadMemory(0x17));
//case 0x6B0A: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x6B0C: //C8 D0 ED
    //clockticks+=2;
    Y++; 
  ZN=Y;


//L_6B0D:
case 0x6B0D: //D0 ED E0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6B0D,0xD0);
        PCSTART=0x6AFC;
        /**/return;
        //goto L_6AFC;
    }


//L_6B0F:
case 0x6B0F: //E0 01 D0
clockticks+=3; //SUM
    //clockticks+=3;
    value = 0x01;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6B11:
case 0x6B11: //D0 08 C0
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6B11,0xD0);
        PCSTART=0x6B1B;
        /**/return;
        //goto L_6B1B;
    }
//case 0x6B13: //C0 1B D0
    //clockticks+=3;
    temp=Y; temp+=0x100; temp-=0x1B;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6B15:
case 0x6B15: //D0 12 A2
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6B15,0xD0);
        PCSTART=0x6B29;
        /**/return;
        //goto L_6B29;
    }
//case 0x6B17: //A2 00 A0
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x6B19: //A0 1C 8A
    //clockticks+=3;
    Y = 0x1C; 
  ZN=Y;


//L_6B1B:
case 0x6B1B: //8A D0 1E
clockticks+=2; //SUM
    //clockticks+=2;
    A = X; 
  ZN=A;


//L_6B1C:
case 0x6B1C: //D0 1E A9
clockticks+=21; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6B1C,0xD0);
        PCSTART=0x6B3C;
        /**/return;
        //goto L_6B3C;
    }
//case 0x6B1E: //A9 81 8D
    //clockticks+=3;
    A=0x81;
//  ZN=A;
//case 0x6B20: //8D FA 02
    //clockticks+=4;
    WriteMemory(0x02FA,A);
//case 0x6B23: //A6 18 D6
    //clockticks+=3;
    X=ReadMemory(0x18); 
//  ZN=X;
//case 0x6B25: //D6 57 A2
    //clockticks+=6;
    temp=0x0057; temp+=X; temp&=0xFF;
    value=ReadMemory(temp);
    value--;
    WriteMemory(temp,value);
//  ZN=value;
//case 0x6B27: //A2 00 A9
    //clockticks+=3;
    X=0x00;
  ZN=X;


//L_6B29:
case 0x6B29: //A9 A0 9D
clockticks+=24; //SUM
    //clockticks+=3;
    A=0xA0;
//  ZN=A;
//case 0x6B2B: //9D 1B 02
    //clockticks+=5;
    temp=0x021B; temp+=X;
    WriteMemory(temp,A);
//case 0x6B2E: //A9 00 9D
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x6B30: //9D 3E 02
    //clockticks+=5;
    temp=0x023E; temp+=X;
    WriteMemory(temp,A);
//case 0x6B33: //9D 61 02
    //clockticks+=5;
    temp=0x0261; temp+=X;
    WriteMemory(temp,A);
//case 0x6B36: //C0 1B 90
    //clockticks+=3;
    temp=Y; temp+=0x100; temp-=0x1B;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6B38:
case 0x6B38: //90 0D B0
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6B38,0x90);
        PCSTART=0x6B47;
        /**/return;
        //goto L_6B47;
    } 


//L_6B3A:
case 0x6B3A: //B0 37 A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6B3A,0xB0);
        PCSTART=0x6B73;
        /**/return;
        //goto L_6B73;
    } 


//L_6B3C:
case 0x6B3C: //A9 00 9D
clockticks+=11; //SUM
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x6B3E: //9D 1B 02
    //clockticks+=5;
    temp=0x021B; temp+=X;
    WriteMemory(temp,A);
//case 0x6B41: //C0 1B F0
    //clockticks+=3;
    temp=Y; temp+=0x100; temp-=0x1B;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6B43:
case 0x6B43: //F0 21 B0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6B43,0xF0);
        PCSTART=0x6B66;
        /**/return;
        //goto L_6B66;
    } 


//L_6B45:
case 0x6B45: //B0 2C 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6B45,0xB0);
        PCSTART=0x6B73;
        /**/return;
        //goto L_6B73;
    } 


//L_6B47:
case 0x6B47: //20 EC 75
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6B;
    stack[S--]=0x4A;
    showme(0x6B47,0x20);
    PCSTART=0x75EC; //jsr
    /**/return;
    //goto L_75EC;


//L_6B4A:
case 0x6B4A: //B9 00 02
clockticks+=42; //SUM
    //clockticks+=4;
    temp=0x0200; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6B4D: //29 03 49
    //clockticks+=3;
    A &= 0x03;
//  ZN=A;
//case 0x6B4F: //49 02 4A
    //clockticks+=3;
    A ^= 0x02;
//  ZN=A;
//case 0x6B51: //4A 6A 6A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6B52: //6A 6A 09
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x6B53: //6A 09 3F
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x6B54: //09 3F 85
    //clockticks+=3;
    A |= 0x3F;
//  ZN=A;
//case 0x6B56: //85 69 A9
    //clockticks+=2;
    WriteMemory(0x69,A);
//case 0x6B58: //A9 A0 99
    //clockticks+=3;
    A=0xA0;
//  ZN=A;
//case 0x6B5A: //99 00 02
    //clockticks+=5;
    temp=0x0200; temp+=Y;
    WriteMemory(temp,A);
//case 0x6B5D: //A9 00 99
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x6B5F: //99 23 02
    //clockticks+=5;
    temp=0x0223; temp+=Y;
    WriteMemory(temp,A);
//case 0x6B62: //99 46 02
    //clockticks+=5;
    temp=0x0246; temp+=Y;
    WriteMemory(temp,A);


//L_6B65:
case 0x6B65: //60 8A A6
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6B65,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6B66:
case 0x6B66: //8A A6 18
clockticks+=20; //SUM
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x6B67: //A6 18 D6
    //clockticks+=3;
    X=ReadMemory(0x18); 
//  ZN=X;
//case 0x6B69: //D6 57 AA
    //clockticks+=6;
    temp=0x0057; temp+=X; temp&=0xFF;
    value=ReadMemory(temp);
    value--;
    WriteMemory(temp,value);
//  ZN=value;
//case 0x6B6B: //AA A9 81
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x6B6C: //A9 81 8D
    //clockticks+=3;
    A=0x81;
  ZN=A;
//case 0x6B6E: //8D FA 02
    //clockticks+=4;
    WriteMemory(0x02FA,A);


//L_6B71:
case 0x6B71: //D0 D7 AD
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6B71,0xD0);
        PCSTART=0x6B4A;
        /**/return;
        //goto L_6B4A;
    }


//L_6B73:
case 0x6B73: //AD F8 02
clockticks+=11; //SUM
    //clockticks+=4;
    A=ReadMemory(0x02F8); 
//  ZN=A;
//case 0x6B76: //8D F7 02
    //clockticks+=4;
    WriteMemory(0x02F7,A);
//case 0x6B79: //A5 1C F0
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_6B7B:
case 0x6B7B: //F0 CD 86
clockticks+=16; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6B7B,0xF0);
        PCSTART=0x6B4A;
        /**/return;
        //goto L_6B4A;
    } 
//case 0x6B7D: //86 0D A6
    //clockticks+=2;
    WriteMemory(0x0D,X);
//case 0x6B7F: //A6 19 AD
    //clockticks+=3;
    X=ReadMemory(0x19); 
//  ZN=X;
//case 0x6B81: //AD 1C 02
    //clockticks+=4;
    A=ReadMemory(0x021C); 
//  ZN=A;
//case 0x6B84: //4A A9 99
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6B85: //A9 99 B0
    //clockticks+=3;
    A=0x99;
  ZN=A;


//L_6B87:
case 0x6B87: //B0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6B87,0xB0);
        PCSTART=0x6B8B;
        /**/return;
        //goto L_6B8B;
    } 
//case 0x6B89: //A9 20 20
    //clockticks+=3;
    A=0x20;
  ZN=A;


//L_6B8B:
case 0x6B8B: //20 97 73
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6B;
    stack[S--]=0x8E;
    showme(0x6B8B,0x20);
    PCSTART=0x7397; //jsr
    /**/return;
    //goto L_7397;


//L_6B8E:
case 0x6B8E: //A6 0D 4C
clockticks+=3; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0D); 
  ZN=X;


//L_6B90:
case 0x6B90: //4C 4A 6B
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6B90,0x4C);
    PCSTART=0x6B4A;
    /**/return;
    //goto L_6B4A;


//L_6B93:
case 0x6B93: //A5 5C 29
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x6B95: //29 03 F0
    //clockticks+=3;
    A &= 0x03;
  ZN=A;


//L_6B97:
case 0x6B97: //F0 01 60
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6B97,0xF0);
        PCSTART=0x6B9A;
        /**/return;
        //goto L_6B9A;
    } 


//L_6B99:
case 0x6B99: //60 AD 1C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6B99,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6B9A:
case 0x6B9A: //AD 1C 02
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x021C); 
  ZN=A;


//L_6B9D:
case 0x6B9D: //30 FA F0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6B9D,0x30);
        PCSTART=0x6B99;
        /**/return;
        //goto L_6B99;
    }


//L_6B9F:
case 0x6B9F: //F0 03 4C
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6B9F,0xF0);
        PCSTART=0x6BA4;
        /**/return;
        //goto L_6BA4;
    } 


//L_6BA1:
case 0x6BA1: //4C 34 6C
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6BA1,0x4C);
    PCSTART=0x6C34;
    /**/return;
    //goto L_6C34;


//L_6BA4:
case 0x6BA4: //A5 1C F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_6BA6:
case 0x6BA6: //F0 07 AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6BA6,0xF0);
        PCSTART=0x6BAF;
        /**/return;
        //goto L_6BAF;
    } 
//case 0x6BA8: //AD 1B 02
    //clockticks+=4;
    A=ReadMemory(0x021B); 
  ZN=A;


//L_6BAB:
case 0x6BAB: //F0 EC 30
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6BAB,0xF0);
        PCSTART=0x6B99;
        /**/return;
        //goto L_6B99;
    } 


//L_6BAD:
case 0x6BAD: //30 EA AD
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6BAD,0x30);
        PCSTART=0x6B99;
        /**/return;
        //goto L_6B99;
    }


//L_6BAF:
case 0x6BAF: //AD F9 02
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x02F9); 
  ZN=A;


//L_6BB2:
case 0x6BB2: //F0 03 CE
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6BB2,0xF0);
        PCSTART=0x6BB7;
        /**/return;
        //goto L_6BB7;
    } 
//case 0x6BB4: //CE F9 02
    //clockticks+=6;
    value=ReadMemory(0x02F9);
    value--;
    WriteMemory(0x02F9,value);
  ZN=value;


//L_6BB7:
case 0x6BB7: //CE F7 02
clockticks+=6; //SUM
    //clockticks+=6;
    value=ReadMemory(0x02F7);
    value--;
    WriteMemory(0x02F7,value);
  ZN=value;


//L_6BBA:
case 0x6BBA: //D0 DD A9
clockticks+=13; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6BBA,0xD0);
        PCSTART=0x6B99;
        /**/return;
        //goto L_6B99;
    }
//case 0x6BBC: //A9 12 8D
    //clockticks+=3;
    A=0x12;
//  ZN=A;
//case 0x6BBE: //8D F7 02
    //clockticks+=4;
    WriteMemory(0x02F7,A);
//case 0x6BC1: //AD F9 02
    //clockticks+=4;
    A=ReadMemory(0x02F9); 
  ZN=A;


//L_6BC4:
case 0x6BC4: //F0 0A AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6BC4,0xF0);
        PCSTART=0x6BD0;
        /**/return;
        //goto L_6BD0;
    } 
//case 0x6BC6: //AD F6 02
    //clockticks+=4;
    A=ReadMemory(0x02F6); 
  ZN=A;


//L_6BC9:
case 0x6BC9: //F0 CE CD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6BC9,0xF0);
        PCSTART=0x6B99;
        /**/return;
        //goto L_6B99;
    } 
//case 0x6BCB: //CD FD 02
    //clockticks+=4;
    value = ReadMemory(0x02FD); 
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6BCE:
case 0x6BCE: //B0 C9 AD
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6BCE,0xB0);
        PCSTART=0x6B99;
        /**/return;
        //goto L_6B99;
    } 


//L_6BD0:
case 0x6BD0: //AD F8 02
clockticks+=12; //SUM
    //clockticks+=4;
    A=ReadMemory(0x02F8); 
//  ZN=A;
//case 0x6BD3: //38 E9 06
    //clockticks+=2;
    C = 0x01;
//case 0x6BD4: //E9 06 C9
    //clockticks+=3;
    SBC(0x06);
//case 0x6BD6: //C9 20 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x20;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6BD8:
case 0x6BD8: //90 03 8D
clockticks+=6; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6BD8,0x90);
        PCSTART=0x6BDD;
        /**/return;
        //goto L_6BDD;
    } 
//case 0x6BDA: //8D F8 02
    //clockticks+=4;
    WriteMemory(0x02F8,A);


//L_6BDD:
case 0x6BDD: //A9 00 8D
clockticks+=11; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x6BDF: //8D CB 02
    //clockticks+=4;
    WriteMemory(0x02CB,A);
//case 0x6BE2: //8D 85 02
    //clockticks+=4;
    WriteMemory(0x0285,A);


//L_6BE5:
case 0x6BE5: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6B;
    stack[S--]=0xE8;
    showme(0x6BE5,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_6BE8:
case 0x6BE8: //4A 6E EE
clockticks+=27; //SUM
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6BE9: //6E EE 02
    //clockticks+=6;
    saveflags = C;
    value=ReadMemory(0x02EE);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x02EE,value);
//  ZN=value;
//case 0x6BEC: //4A 6E EE
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6BED: //6E EE 02
    //clockticks+=6;
    saveflags = C;
    value=ReadMemory(0x02EE);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x02EE,value);
//  ZN=value;
//case 0x6BF0: //4A 6E EE
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6BF1: //6E EE 02
    //clockticks+=6;
    saveflags = C;
    value=ReadMemory(0x02EE);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x02EE,value);
//  ZN=value;
//case 0x6BF4: //C9 18 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x18;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6BF6:
case 0x6BF6: //90 02 29
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6BF6,0x90);
        PCSTART=0x6BFA;
        /**/return;
        //goto L_6BFA;
    } 
//case 0x6BF8: //29 17 8D
    //clockticks+=3;
    A &= 0x17;
  ZN=A;


//L_6BFA:
case 0x6BFA: //8D A8 02
clockticks+=10; //SUM
    //clockticks+=4;
    WriteMemory(0x02A8,A);
//case 0x6BFD: //A2 10 24
    //clockticks+=3;
    X=0x10;
//  ZN=X;
//case 0x6BFF: //24 60 70
    //clockticks+=3;
    value=ReadMemory(0x60);
    V = value;
  ZN = value & 0x80;
  if((value & A)==0) ZN |= 0x02;


//L_6C01:
case 0x6C01: //70 0C A9
clockticks+=19; //SUM
    //clockticks+=2;
    if(V & 0x40)
    {
        clockticks++;
        showme(0x6C01,0x70);
        PCSTART=0x6C0F;
        /**/return;
        //goto L_6C0F;
    }
//case 0x6C03: //A9 1F 8D
    //clockticks+=3;
    A=0x1F;
//  ZN=A;
//case 0x6C05: //8D 85 02
    //clockticks+=4;
    WriteMemory(0x0285,A);
//case 0x6C08: //A9 FF 8D
    //clockticks+=3;
    A=0xFF;
//  ZN=A;
//case 0x6C0A: //8D CB 02
    //clockticks+=4;
    WriteMemory(0x02CB,A);
//case 0x6C0D: //A2 F0 8E
    //clockticks+=3;
    X=0xF0;
  ZN=X;


//L_6C0F:
case 0x6C0F: //8E 3F 02
clockticks+=11; //SUM
    //clockticks+=4;
    WriteMemory(0x023F,X);
//case 0x6C12: //A2 02 AD
    //clockticks+=3;
    X=0x02;
//  ZN=X;
//case 0x6C14: //AD F8 02
    //clockticks+=4;
    A=ReadMemory(0x02F8); 
  ZN=A;


//L_6C17:
case 0x6C17: //30 17 A4
clockticks+=12; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6C17,0x30);
        PCSTART=0x6C30;
        /**/return;
        //goto L_6C30;
    }
//case 0x6C19: //A4 19 B9
    //clockticks+=3;
    Y=ReadMemory(0x19);
//  ZN=Y;
//case 0x6C1B: //B9 53 00
    //clockticks+=4;
    temp=0x0053; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6C1E: //C9 30 B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x30;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6C20:
case 0x6C20: //B0 0D 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6C20,0xB0);
        PCSTART=0x6C2F;
        /**/return;
        //goto L_6C2F;
    } 


//L_6C22:
case 0x6C22: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6C;
    stack[S--]=0x25;
    showme(0x6C22,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_6C25:
case 0x6C25: //85 08 AD
clockticks+=11; //SUM
    //clockticks+=2;
    WriteMemory(0x08,A);
//case 0x6C27: //AD F8 02
    //clockticks+=4;
    A=ReadMemory(0x02F8); 
//  ZN=A;
//case 0x6C2A: //4A C5 08
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6C2B: //C5 08 B0
    //clockticks+=3;
    value=ReadMemory(0x08);
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6C2D:
case 0x6C2D: //B0 01 CA
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6C2D,0xB0);
        PCSTART=0x6C30;
        /**/return;
        //goto L_6C30;
    } 


//L_6C2F:
case 0x6C2F: //CA 8E 1C
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_6C30:
case 0x6C30: //8E 1C 02
clockticks+=4; //SUM
    //clockticks+=4;
    WriteMemory(0x021C,X);


//L_6C33:
case 0x6C33: //60 A5 5C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6C33,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6C34:
case 0x6C34: //A5 5C 0A
clockticks+=5; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x6C36: //0A D0 0C
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
  ZN=A;


//L_6C37:
case 0x6C37: //D0 0C 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6C37,0xD0);
        PCSTART=0x6C45;
        /**/return;
        //goto L_6C45;
    }


//L_6C39:
case 0x6C39: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6C;
    stack[S--]=0x3C;
    showme(0x6C39,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_6C3C:
case 0x6C3C: //29 03 AA
clockticks+=13; //SUM
    //clockticks+=3;
    A &= 0x03;
//  ZN=A;
//case 0x6C3E: //AA BD D3
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x6C3F: //BD D3 6C
    //clockticks+=4;
    temp=0x6CD3; temp+=X;
    A=ReadMemory(temp);
  ZN=A;
//case 0x6C42: //8D 62 02
    //clockticks+=4;
    WriteMemory(0x0262,A);


//L_6C45:
case 0x6C45: //A5 1C F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_6C47:
case 0x6C47: //F0 05 AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6C47,0xF0);
        PCSTART=0x6C4E;
        /**/return;
        //goto L_6C4E;
    } 
//case 0x6C49: //AD FA 02
    //clockticks+=4;
    A=ReadMemory(0x02FA); 
  ZN=A;


//L_6C4C:
case 0x6C4C: //D0 05 CE
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6C4C,0xD0);
        PCSTART=0x6C53;
        /**/return;
        //goto L_6C53;
    }


//L_6C4E:
case 0x6C4E: //CE F7 02
clockticks+=6; //SUM
    //clockticks+=6;
    value=ReadMemory(0x02F7);
    value--;
    WriteMemory(0x02F7,value);
  ZN=value;


//L_6C51:
case 0x6C51: //F0 01 60
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6C51,0xF0);
        PCSTART=0x6C54;
        /**/return;
        //goto L_6C54;
    } 


//L_6C53:
case 0x6C53: //60 A9 0A
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6C53,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6C54:
case 0x6C54: //A9 0A 8D
clockticks+=13; //SUM
    //clockticks+=3;
    A=0x0A;
//  ZN=A;
//case 0x6C56: //8D F7 02
    //clockticks+=4;
    WriteMemory(0x02F7,A);
//case 0x6C59: //AD 1C 02
    //clockticks+=4;
    A=ReadMemory(0x021C); 
//  ZN=A;
//case 0x6C5C: //4A F0 06
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_6C5D:
case 0x6C5D: //F0 06 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6C5D,0xF0);
        PCSTART=0x6C65;
        /**/return;
        //goto L_6C65;
    } 


//L_6C5F:
case 0x6C5F: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6C;
    stack[S--]=0x62;
    showme(0x6C5F,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_6C62:
case 0x6C62: //4C C4 6C
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6C62,0x4C);
    PCSTART=0x6CC4;
    /**/return;
    //goto L_6CC4;


//L_6C65:
case 0x6C65: //AD 3F 02
clockticks+=104; //SUM
    //clockticks+=4;
    A=ReadMemory(0x023F); 
//  ZN=A;
//case 0x6C68: //C9 80 6A
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
//  ZN=value;
//case 0x6C6A: //6A 85 0C
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x6C6B: //85 0C AD
    //clockticks+=2;
    WriteMemory(0x0C,A);
//case 0x6C6D: //AD CA 02
    //clockticks+=4;
    A=ReadMemory(0x02CA); 
//  ZN=A;
//case 0x6C70: //38 ED CB
    //clockticks+=2;
    C = 0x01;
//case 0x6C71: //ED CB 02
    //clockticks+=4;
    SBC(ReadMemory(0x02CB));
//case 0x6C74: //85 0B AD
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x6C76: //AD 84 02
    //clockticks+=4;
    A=ReadMemory(0x0284); 
//  ZN=A;
//case 0x6C79: //ED 85 02
    //clockticks+=4;
    SBC(ReadMemory(0x0285));
//case 0x6C7C: //06 0B 2A
    //clockticks+=5;
    value = ReadMemory(0x0B);
    C = value >> 7;
    value = value << 1;
    WriteMemory(0x0B,value);
//  ZN=value;
//case 0x6C7E: //2A 06 0B
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x6C7F: //06 0B 2A
    //clockticks+=5;
    value = ReadMemory(0x0B);
    C = value >> 7;
    value = value << 1;
    WriteMemory(0x0B,value);
//  ZN=value;
//case 0x6C81: //2A 38 E5
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x6C82: //38 E5 0C
    //clockticks+=2;
    C = 0x01;
//case 0x6C83: //E5 0C AA
    //clockticks+=3;
    SBC(ReadMemory(0x0C));
//case 0x6C85: //AA AD 62
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x6C86: //AD 62 02
    //clockticks+=4;
    A=ReadMemory(0x0262); 
//  ZN=A;
//case 0x6C89: //C9 80 6A
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
//  ZN=value;
//case 0x6C8B: //6A 85 0C
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x6C8C: //85 0C AD
    //clockticks+=2;
    WriteMemory(0x0C,A);
//case 0x6C8E: //AD ED 02
    //clockticks+=4;
    A=ReadMemory(0x02ED); 
//  ZN=A;
//case 0x6C91: //38 ED EE
    //clockticks+=2;
    C = 0x01;
//case 0x6C92: //ED EE 02
    //clockticks+=4;
    SBC(ReadMemory(0x02EE));
//case 0x6C95: //85 0B AD
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x6C97: //AD A7 02
    //clockticks+=4;
    A=ReadMemory(0x02A7); 
//  ZN=A;
//case 0x6C9A: //ED A8 02
    //clockticks+=4;
    SBC(ReadMemory(0x02A8));
//case 0x6C9D: //06 0B 2A
    //clockticks+=5;
    value = ReadMemory(0x0B);
    C = value >> 7;
    value = value << 1;
    WriteMemory(0x0B,value);
//  ZN=value;
//case 0x6C9F: //2A 06 0B
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x6CA0: //06 0B 2A
    //clockticks+=5;
    value = ReadMemory(0x0B);
    C = value >> 7;
    value = value << 1;
    WriteMemory(0x0B,value);
//  ZN=value;
//case 0x6CA2: //2A 38 E5
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x6CA3: //38 E5 0C
    //clockticks+=2;
    C = 0x01;
//case 0x6CA4: //E5 0C A8
    //clockticks+=3;
    SBC(ReadMemory(0x0C));
//case 0x6CA6: //A8 20 F0
    //clockticks+=2;
    Y=A;
  ZN=Y;


//L_6CA7:
case 0x6CA7: //20 F0 76
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6C;
    stack[S--]=0xAA;
    showme(0x6CA7,0x20);
    PCSTART=0x76F0; //jsr
    /**/return;
    //goto L_76F0;


//L_6CAA:
case 0x6CAA: //85 62 20
clockticks+=2; //SUM
    //clockticks+=2;
    WriteMemory(0x62,A);


//L_6CAC:
case 0x6CAC: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6C;
    stack[S--]=0xAF;
    showme(0x6CAC,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_6CAF:
case 0x6CAF: //A6 19 B4
clockticks+=13; //SUM
    //clockticks+=3;
    X=ReadMemory(0x19); 
//  ZN=X;
//case 0x6CB1: //B4 53 C0
    //clockticks+=4;
    temp=0x53; temp+=X; temp&=0xFF;
    Y=ReadMemory(temp);
//  ZN=Y;
//case 0x6CB3: //C0 35 A2
    //clockticks+=3;
    temp=Y; temp+=0x100; temp-=0x35;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
//  ZN=value;
//case 0x6CB5: //A2 00 90
    //clockticks+=3;
    X=0x00;
  ZN=X;


//L_6CB7:
case 0x6CB7: //90 01 E8
clockticks+=4; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6CB7,0x90);
        PCSTART=0x6CBA;
        /**/return;
        //goto L_6CBA;
    } 
//case 0x6CB9: //E8 3D CF
    //clockticks+=2;
    X++; 
  ZN=X;


//L_6CBA:
case 0x6CBA: //3D CF 6C
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x6CCF; temp+=X;
    A &= ReadMemory(temp);
  ZN=A;


//L_6CBD:
case 0x6CBD: //10 03 1D
clockticks+=6; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6CBD,0x10);
        PCSTART=0x6CC2;
        /**/return;
        //goto L_6CC2;
    }
//case 0x6CBF: //1D D1 6C
    //clockticks+=4;
    temp=0x6CD1; temp+=X;
    A |= ReadMemory(temp);
  ZN=A;


//L_6CC2:
case 0x6CC2: //65 62 85
clockticks+=3; //SUM
    //clockticks+=3;
    ADC(ReadMemory(0x62));


//L_6CC4:
case 0x6CC4: //85 62 A0
clockticks+=10; //SUM
    //clockticks+=2;
    WriteMemory(0x62,A);
//case 0x6CC6: //A0 03 A2
    //clockticks+=3;
    Y = 0x03; 
//  ZN=Y;
//case 0x6CC8: //A2 01 86
    //clockticks+=3;
    X=0x01;
  ZN=X;
//case 0x6CCA: //86 0E 4C
    //clockticks+=2;
    WriteMemory(0x0E,X);


//L_6CCC:
case 0x6CCC: //4C F2 6C
clockticks+=0; //SUM
    //clockticks+=3;
    showme(0x6CCC,0x4C);
    PCSTART=0x6CF2;
    /**/return;
    //goto L_6CF2;


//L_6CD7:
case 0x6CD7: //A5 1C F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_6CD9:
case 0x6CD9: //F0 21 0E
clockticks+=16; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6CD9,0xF0);
        PCSTART=0x6CFC;
        /**/return;
        //goto L_6CFC;
    } 
//case 0x6CDB: //0E 04 20
    //clockticks+=6;
    value = ReadMemory(0x2004);
    C = value >> 7;
    value = value << 1;
    WriteMemory(0x2004,value);
//  ZN=value;
//case 0x6CDE: //66 63 24
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x63);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x63,value);
//    ZN=value;
//case 0x6CE0: //24 63 10
    //clockticks+=3;
    value=ReadMemory(0x63);
    V = value;
  ZN = value & 0x80;
  if((value & A)==0) ZN |= 0x02;


//L_6CE2:
case 0x6CE2: //10 18 70
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6CE2,0x10);
        PCSTART=0x6CFC;
        /**/return;
        //goto L_6CFC;
    }


//L_6CE4:
case 0x6CE4: //70 16 AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(V & 0x40)
    {
        clockticks++;
        showme(0x6CE4,0x70);
        PCSTART=0x6CFC;
        /**/return;
        //goto L_6CFC;
    }
//case 0x6CE6: //AD FA 02
    //clockticks+=4;
    A=ReadMemory(0x02FA); 
  ZN=A;


//L_6CE9:
case 0x6CE9: //D0 11 AA
clockticks+=12; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6CE9,0xD0);
        PCSTART=0x6CFC;
        /**/return;
        //goto L_6CFC;
    }
//case 0x6CEB: //AA A9 03
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x6CEC: //A9 03 85
    //clockticks+=3;
    A=0x03;
//  ZN=A;
//case 0x6CEE: //85 0E A0
    //clockticks+=2;
    WriteMemory(0x0E,A);
//case 0x6CF0: //A0 07 B9
    //clockticks+=3;
    Y = 0x07; 
  ZN=Y;


//L_6CF2:
case 0x6CF2: //B9 1B 02
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x021B; temp+=Y;
    A=ReadMemory(temp);
  ZN=A;


//L_6CF5:
case 0x6CF5: //F0 06 88
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6CF5,0xF0);
        PCSTART=0x6CFD;
        /**/return;
        //goto L_6CFD;
    } 
//case 0x6CF7: //88 C4 0E
    //clockticks+=2;
    Y--;
//  ZN=Y;
//case 0x6CF8: //C4 0E D0
    //clockticks+=3;
    value=ReadMemory(0x0E);
    temp=Y; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6CFA:
case 0x6CFA: //D0 F6 60
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6CFA,0xD0);
        PCSTART=0x6CF2;
        /**/return;
        //goto L_6CF2;
    }


//L_6CFC:
case 0x6CFC: //60 86 0D
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6CFC,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6CFD:
case 0x6CFD: //86 0D A9
clockticks+=14; //SUM
    //clockticks+=2;
    WriteMemory(0x0D,X);
//case 0x6CFF: //A9 12 99
    //clockticks+=3;
    A=0x12;
//  ZN=A;
//case 0x6D01: //99 1B 02
    //clockticks+=5;
    temp=0x021B; temp+=Y;
    WriteMemory(temp,A);
//case 0x6D04: //B5 61 20
    //clockticks+=4;
    temp=0x0061; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_6D06:
case 0x6D06: //20 D2 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0x09;
    showme(0x6D06,0x20);
    PCSTART=0x77D2; //jsr
    /**/return;
    //goto L_77D2;


//L_6D09:
case 0x6D09: //A6 0D C9
clockticks+=16; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0D); 
//  ZN=X;
//case 0x6D0B: //C9 80 6A
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
//  ZN=value;
//case 0x6D0D: //6A 85 09
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x6D0E: //85 09 18
    //clockticks+=2;
    WriteMemory(0x09,A);
//case 0x6D10: //18 7D 3E
    //clockticks+=2;
    C = 0;
//case 0x6D11: //7D 3E 02
    //clockticks+=4;
    temp=0x023E; temp+=X;
    ADC(ReadMemory(temp));


//L_6D14:
case 0x6D14: //30 08 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6D14,0x30);
        PCSTART=0x6D1E;
        /**/return;
        //goto L_6D1E;
    }
//case 0x6D16: //C9 70 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x70;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6D18:
case 0x6D18: //90 0A A9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6D18,0x90);
        PCSTART=0x6D24;
        /**/return;
        //goto L_6D24;
    } 
//case 0x6D1A: //A9 6F D0
    //clockticks+=3;
    A=0x6F;
  ZN=A;


//L_6D1C:
case 0x6D1C: //D0 06 C9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6D1C,0xD0);
        PCSTART=0x6D24;
        /**/return;
        //goto L_6D24;
    }


//L_6D1E:
case 0x6D1E: //C9 91 B0
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x91;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6D20:
case 0x6D20: //B0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6D20,0xB0);
        PCSTART=0x6D24;
        /**/return;
        //goto L_6D24;
    } 
//case 0x6D22: //A9 91 99
    //clockticks+=3;
    A=0x91;
  ZN=A;


//L_6D24:
case 0x6D24: //99 3E 02
clockticks+=9; //SUM
    //clockticks+=5;
    temp=0x023E; temp+=Y;
    WriteMemory(temp,A);
//case 0x6D27: //B5 61 20
    //clockticks+=4;
    temp=0x0061; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_6D29:
case 0x6D29: //20 D5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0x2C;
    showme(0x6D29,0x20);
    PCSTART=0x77D5; //jsr
    /**/return;
    //goto L_77D5;


//L_6D2C:
case 0x6D2C: //A6 0D C9
clockticks+=16; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0D); 
//  ZN=X;
//case 0x6D2E: //C9 80 6A
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
//  ZN=value;
//case 0x6D30: //6A 85 0C
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x6D31: //85 0C 18
    //clockticks+=2;
    WriteMemory(0x0C,A);
//case 0x6D33: //18 7D 61
    //clockticks+=2;
    C = 0;
//case 0x6D34: //7D 61 02
    //clockticks+=4;
    temp=0x0261; temp+=X;
    ADC(ReadMemory(temp));


//L_6D37:
case 0x6D37: //30 08 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6D37,0x30);
        PCSTART=0x6D41;
        /**/return;
        //goto L_6D41;
    }
//case 0x6D39: //C9 70 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x70;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6D3B:
case 0x6D3B: //90 0A A9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6D3B,0x90);
        PCSTART=0x6D47;
        /**/return;
        //goto L_6D47;
    } 
//case 0x6D3D: //A9 6F D0
    //clockticks+=3;
    A=0x6F;
  ZN=A;


//L_6D3F:
case 0x6D3F: //D0 06 C9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6D3F,0xD0);
        PCSTART=0x6D47;
        /**/return;
        //goto L_6D47;
    }


//L_6D41:
case 0x6D41: //C9 91 B0
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x91;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6D43:
case 0x6D43: //B0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6D43,0xB0);
        PCSTART=0x6D47;
        /**/return;
        //goto L_6D47;
    } 
//case 0x6D45: //A9 91 99
    //clockticks+=3;
    A=0x91;
  ZN=A;


//L_6D47:
case 0x6D47: //99 61 02
clockticks+=11; //SUM
    //clockticks+=5;
    temp=0x0261; temp+=Y;
    WriteMemory(temp,A);
//case 0x6D4A: //A2 00 A5
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x6D4C: //A5 09 10
    //clockticks+=3;
    A=ReadMemory(0x09); 
  ZN=A;


//L_6D4E:
case 0x6D4E: //10 01 CA
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6D4E,0x10);
        PCSTART=0x6D51;
        /**/return;
        //goto L_6D51;
    }
//case 0x6D50: //CA 86 08
    //clockticks+=2;
    X--; 
  ZN=X;


//L_6D51:
case 0x6D51: //86 08 A6
clockticks+=44; //SUM
    //clockticks+=2;
    WriteMemory(0x08,X);
//case 0x6D53: //A6 0D C9
    //clockticks+=3;
    X=ReadMemory(0x0D); 
//  ZN=X;
//case 0x6D55: //C9 80 6A
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
//  ZN=value;
//case 0x6D57: //6A 18 65
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x6D58: //18 65 09
    //clockticks+=2;
    C = 0;
//case 0x6D59: //65 09 18
    //clockticks+=3;
    ADC(ReadMemory(0x09));
//case 0x6D5B: //18 7D CA
    //clockticks+=2;
    C = 0;
//case 0x6D5C: //7D CA 02
    //clockticks+=4;
    temp=0x02CA; temp+=X;
    ADC(ReadMemory(temp));
//case 0x6D5F: //99 CA 02
    //clockticks+=5;
    temp=0x02CA; temp+=Y;
    WriteMemory(temp,A);
//case 0x6D62: //A5 08 7D
    //clockticks+=3;
    A=ReadMemory(0x08); 
//  ZN=A;
//case 0x6D64: //7D 84 02
    //clockticks+=4;
    temp=0x0284; temp+=X;
    ADC(ReadMemory(temp));
//case 0x6D67: //99 84 02
    //clockticks+=5;
    temp=0x0284; temp+=Y;
    WriteMemory(temp,A);
//case 0x6D6A: //A2 00 A5
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x6D6C: //A5 0C 10
    //clockticks+=3;
    A=ReadMemory(0x0C); 
  ZN=A;


//L_6D6E:
case 0x6D6E: //10 01 CA
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6D6E,0x10);
        PCSTART=0x6D71;
        /**/return;
        //goto L_6D71;
    }
//case 0x6D70: //CA 86 0B
    //clockticks+=2;
    X--; 
  ZN=X;


//L_6D71:
case 0x6D71: //86 0B A6
clockticks+=45; //SUM
    //clockticks+=2;
    WriteMemory(0x0B,X);
//case 0x6D73: //A6 0D C9
    //clockticks+=3;
    X=ReadMemory(0x0D); 
//  ZN=X;
//case 0x6D75: //C9 80 6A
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
//  ZN=value;
//case 0x6D77: //6A 18 65
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x6D78: //18 65 0C
    //clockticks+=2;
    C = 0;
//case 0x6D79: //65 0C 18
    //clockticks+=3;
    ADC(ReadMemory(0x0C));
//case 0x6D7B: //18 7D ED
    //clockticks+=2;
    C = 0;
//case 0x6D7C: //7D ED 02
    //clockticks+=4;
    temp=0x02ED; temp+=X;
    ADC(ReadMemory(temp));
//case 0x6D7F: //99 ED 02
    //clockticks+=5;
    temp=0x02ED; temp+=Y;
    WriteMemory(temp,A);
//case 0x6D82: //A5 0B 7D
    //clockticks+=3;
    A=ReadMemory(0x0B); 
//  ZN=A;
//case 0x6D84: //7D A7 02
    //clockticks+=4;
    temp=0x02A7; temp+=X;
    ADC(ReadMemory(temp));
//case 0x6D87: //99 A7 02
    //clockticks+=5;
    temp=0x02A7; temp+=Y;
    WriteMemory(temp,A);
//case 0x6D8A: //A9 80 95
    //clockticks+=3;
    A=0x80;
  ZN=A;
//case 0x6D8C: //95 66 60
    //clockticks+=4;
    temp=0x66; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_6D8E:
case 0x6D8E: //60 D8 A5
clockticks+=0; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6D8E,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6D90:
case 0x6D90: //A5 32 25
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x32); 
//  ZN=A;
//case 0x6D92: //25 33 10
    //clockticks+=3;
    A &= ReadMemory(0x33);
  ZN=A;


//L_6D94:
case 0x6D94: //10 01 60
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6D94,0x10);
        PCSTART=0x6D97;
        /**/return;
        //goto L_6D97;
    }


//L_6D96:
case 0x6D96: //60 A5 1A
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6D96,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6D97:
case 0x6D97: //A5 1A 4A
clockticks+=5; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1A); 
//  ZN=A;
//case 0x6D99: //4A F0 18
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_6D9A:
case 0x6D9A: //F0 18 A0
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6D9A,0xF0);
        PCSTART=0x6DB4;
        /**/return;
        //goto L_6DB4;
    } 
//case 0x6D9C: //A0 01 20
    //clockticks+=3;
    Y = 0x01; 
  ZN=Y;


//L_6D9E:
case 0x6D9E: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xA1;
    showme(0x6D9E,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_6DA1:
case 0x6DA1: //A0 02 A6
clockticks+=6; //SUM
    //clockticks+=3;
    Y = 0x02; 
//  ZN=Y;
//case 0x6DA3: //A6 33 10
    //clockticks+=3;
    X=ReadMemory(0x33); 
  ZN=X;


//L_6DA5:
case 0x6DA5: //10 01 88
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6DA5,0x10);
        PCSTART=0x6DA8;
        /**/return;
        //goto L_6DA8;
    }
//case 0x6DA7: //88 84 18
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_6DA8:
case 0x6DA8: //84 18 A5
clockticks+=8; //SUM
    //clockticks+=2;
    WriteMemory(0x18,Y);
//case 0x6DAA: //A5 5C 29
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x6DAC: //29 10 D0
    //clockticks+=3;
    A &= 0x10;
  ZN=A;


//L_6DAE:
case 0x6DAE: //D0 04 98
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6DAE,0xD0);
        PCSTART=0x6DB4;
        /**/return;
        //goto L_6DB4;
    }
//case 0x6DB0: //98 20 D1
    //clockticks+=2;
    A = Y;
  ZN=A;


//L_6DB1:
case 0x6DB1: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xB4;
    showme(0x6DB1,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_6DB4:
case 0x6DB4: //46 18 20
clockticks+=5; //SUM
    //clockticks+=5;
    value=ReadMemory(0x18);
    C = value;
    value = value >> 1;
    WriteMemory(0x18,value);
  ZN=value;


//L_6DB6:
case 0x6DB6: //20 B2 73
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xB9;
    showme(0x6DB6,0x20);
    PCSTART=0x73B2; //jsr
    /**/return;
    //goto L_73B2;


//L_6DB9:
case 0x6DB9: //A0 02 20
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x02; 
  ZN=Y;


//L_6DBB:
case 0x6DBB: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xBE;
    showme(0x6DBB,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_6DBE:
case 0x6DBE: //A0 03 20
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x03; 
  ZN=Y;


//L_6DC0:
case 0x6DC0: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xC3;
    showme(0x6DC0,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_6DC3:
case 0x6DC3: //A0 04 20
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x04; 
  ZN=Y;


//L_6DC5:
case 0x6DC5: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xC8;
    showme(0x6DC5,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_6DC8:
case 0x6DC8: //A0 05 20
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x05; 
  ZN=Y;


//L_6DCA:
case 0x6DCA: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xCD;
    showme(0x6DCA,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_6DCD:
case 0x6DCD: //A9 20 85
clockticks+=11; //SUM
    //clockticks+=3;
    A=0x20;
//  ZN=A;
//case 0x6DCF: //85 00 A9
    //clockticks+=2;
    WriteMemory(0x00,A);
//case 0x6DD1: //A9 64 A2
    //clockticks+=3;
    A=0x64;
//  ZN=A;
//case 0x6DD3: //A2 39 20
    //clockticks+=3;
    X=0x39;
  ZN=X;


//L_6DD5:
case 0x6DD5: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xD8;
    showme(0x6DD5,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_6DD8:
case 0x6DD8: //A9 70 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x70;
  ZN=A;


//L_6DDA:
case 0x6DDA: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xDD;
    showme(0x6DDA,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_6DDD:
case 0x6DDD: //A6 18 B4
clockticks+=18; //SUM
    //clockticks+=3;
    X=ReadMemory(0x18); 
//  ZN=X;
//case 0x6DDF: //B4 32 84
    //clockticks+=4;
    temp=0x32; temp+=X; temp&=0xFF;
    Y=ReadMemory(temp);
//  ZN=Y;
//case 0x6DE1: //84 0B 98
    //clockticks+=2;
    WriteMemory(0x0B,Y);
//case 0x6DE3: //98 18 65
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x6DE4: //18 65 31
    //clockticks+=2;
    C = 0;
//case 0x6DE5: //65 31 85
    //clockticks+=3;
    ADC(ReadMemory(0x31));
//case 0x6DE7: //85 0C 20
    //clockticks+=2;
    WriteMemory(0x0C,A);


//L_6DE9:
case 0x6DE9: //20 1A 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xEC;
    showme(0x6DE9,0x20);
    PCSTART=0x6F1A; //jsr
    /**/return;
    //goto L_6F1A;


//L_6DEC:
case 0x6DEC: //A4 0B C8
clockticks+=5; //SUM
    //clockticks+=3;
    Y=ReadMemory(0x0B);
//  ZN=Y;
//case 0x6DEE: //C8 20 1A
    //clockticks+=2;
    Y++; 
  ZN=Y;


//L_6DEF:
case 0x6DEF: //20 1A 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xF2;
    showme(0x6DEF,0x20);
    PCSTART=0x6F1A; //jsr
    /**/return;
    //goto L_6F1A;


//L_6DF2:
case 0x6DF2: //A4 0B C8
clockticks+=7; //SUM
    //clockticks+=3;
    Y=ReadMemory(0x0B);
//  ZN=Y;
//case 0x6DF4: //C8 C8 20
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x6DF5: //C8 20 1A
    //clockticks+=2;
    Y++; 
  ZN=Y;


//L_6DF6:
case 0x6DF6: //20 1A 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6D;
    stack[S--]=0xF9;
    showme(0x6DF6,0x20);
    PCSTART=0x6F1A; //jsr
    /**/return;
    //goto L_6F1A;


//L_6DF9:
case 0x6DF9: //AD 03 20
clockticks+=20; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2003); 
//  ZN=A;
//case 0x6DFC: //2A 26 63
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x6DFD: //26 63 A5
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x63);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x63,value);
//  ZN=value;
//case 0x6DFF: //A5 63 29
    //clockticks+=3;
    A=ReadMemory(0x63); 
//  ZN=A;
//case 0x6E01: //29 1F C9
    //clockticks+=3;
    A &= 0x1F;
//  ZN=A;
//case 0x6E03: //C9 07 D0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x07;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6E05:
case 0x6E05: //D0 27 E6
clockticks+=13; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6E05,0xD0);
        PCSTART=0x6E2E;
        /**/return;
        //goto L_6E2E;
    }
//case 0x6E07: //E6 31 A5
    //clockticks+=5;
    value=ReadMemory(0x31);
    value++;
    WriteMemory(0x31,value);
//  ZN=value;
//case 0x6E09: //A5 31 C9
    //clockticks+=3;
    A=ReadMemory(0x31); 
//  ZN=A;
//case 0x6E0B: //C9 03 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x03;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6E0D:
case 0x6E0D: //90 13 A6
clockticks+=12; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6E0D,0x90);
        PCSTART=0x6E22;
        /**/return;
        //goto L_6E22;
    } 
//case 0x6E0F: //A6 18 A9
    //clockticks+=3;
    X=ReadMemory(0x18); 
//  ZN=X;
//case 0x6E11: //A9 FF 95
    //clockticks+=3;
    A=0xFF;
  ZN=A;
//case 0x6E13: //95 32 A2
    //clockticks+=4;
    temp=0x32; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_6E15:
case 0x6E15: //A2 00 86
clockticks+=12; //SUM
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x6E17: //86 18 86
    //clockticks+=2;
    WriteMemory(0x18,X);
//case 0x6E19: //86 31 A2
    //clockticks+=2;
    WriteMemory(0x31,X);
//case 0x6E1B: //A2 F0 86
    //clockticks+=3;
    X=0xF0;
  ZN=X;
//case 0x6E1D: //86 5D 4C
    //clockticks+=2;
    WriteMemory(0x5D,X);


//L_6E1F:
case 0x6E1F: //4C B2 73
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6E1F,0x4C);
    PCSTART=0x73B2;
    /**/return;
    //goto L_73B2;


//L_6E22:
case 0x6E22: //E6 0C A6
clockticks+=20; //SUM
    //clockticks+=5;
    value=ReadMemory(0x0C);
    value++;
    WriteMemory(0x0C,value);
//  ZN=value;
//case 0x6E24: //A6 0C A9
    //clockticks+=3;
    X=ReadMemory(0x0C); 
//  ZN=X;
//case 0x6E26: //A9 F4 85
    //clockticks+=3;
    A=0xF4;
//  ZN=A;
//case 0x6E28: //85 5D A9
    //clockticks+=2;
    WriteMemory(0x5D,A);
//case 0x6E2A: //A9 0B 95
    //clockticks+=3;
    A=0x0B;
  ZN=A;
//case 0x6E2C: //95 34 A5
    //clockticks+=4;
    temp=0x34; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_6E2E:
case 0x6E2E: //A5 5D D0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5D); 
  ZN=A;


//L_6E30:
case 0x6E30: //D0 08 A9
clockticks+=9; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6E30,0xD0);
        PCSTART=0x6E3A;
        /**/return;
        //goto L_6E3A;
    }
//case 0x6E32: //A9 FF 85
    //clockticks+=3;
    A=0xFF;
  ZN=A;
//case 0x6E34: //85 32 85
    //clockticks+=2;
    WriteMemory(0x32,A);
//case 0x6E36: //85 33 30
    //clockticks+=2;
    WriteMemory(0x33,A);


//L_6E38:
case 0x6E38: //30 DB A5
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6E38,0x30);
        PCSTART=0x6E15;
        /**/return;
        //goto L_6E15;
    }


//L_6E3A:
case 0x6E3A: //A5 5C 29
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x6E3C: //29 07 D0
    //clockticks+=3;
    A &= 0x07;
  ZN=A;


//L_6E3E:
case 0x6E3E: //D0 31 AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6E3E,0xD0);
        PCSTART=0x6E71;
        /**/return;
        //goto L_6E71;
    }
//case 0x6E40: //AD 07 24
    //clockticks+=4;
    A=ReadMemory(0x2407); 
  ZN=A;


//L_6E43:
case 0x6E43: //10 04 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6E43,0x10);
        PCSTART=0x6E49;
        /**/return;
        //goto L_6E49;
    }
//case 0x6E45: //A9 01 D0
    //clockticks+=3;
    A=0x01;
  ZN=A;


//L_6E47:
case 0x6E47: //D0 07 AD
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6E47,0xD0);
        PCSTART=0x6E50;
        /**/return;
        //goto L_6E50;
    }


//L_6E49:
case 0x6E49: //AD 06 24
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2406); 
  ZN=A;


//L_6E4C:
case 0x6E4C: //10 23 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6E4C,0x10);
        PCSTART=0x6E71;
        /**/return;
        //goto L_6E71;
    }
//case 0x6E4E: //A9 FF A6
    //clockticks+=3;
    A=0xFF;
  ZN=A;


//L_6E50:
case 0x6E50: //A6 0C 18
clockticks+=9; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0C); 
//  ZN=X;
//case 0x6E52: //18 75 34
    //clockticks+=2;
    C = 0;
//case 0x6E53: //75 34 30
    //clockticks+=4;
    temp=0x34; temp+=X; temp&=0xFF;
    ADC(ReadMemory(temp));


//L_6E55:
case 0x6E55: //30 10 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6E55,0x30);
        PCSTART=0x6E67;
        /**/return;
        //goto L_6E67;
    }
//case 0x6E57: //C9 0B B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x0B;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6E59:
case 0x6E59: //B0 0E C9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6E59,0xB0);
        PCSTART=0x6E69;
        /**/return;
        //goto L_6E69;
    } 
//case 0x6E5B: //C9 01 F0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x01;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6E5D:
case 0x6E5D: //F0 04 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6E5D,0xF0);
        PCSTART=0x6E63;
        /**/return;
        //goto L_6E63;
    } 
//case 0x6E5F: //A9 00 F0
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_6E61:
case 0x6E61: //F0 0C A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6E61,0xF0);
        PCSTART=0x6E6F;
        /**/return;
        //goto L_6E6F;
    } 


//L_6E63:
case 0x6E63: //A9 0B D0
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x0B;
  ZN=A;


//L_6E65:
case 0x6E65: //D0 08 A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6E65,0xD0);
        PCSTART=0x6E6F;
        /**/return;
        //goto L_6E6F;
    }


//L_6E67:
case 0x6E67: //A9 24 C9
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x24;
  ZN=A;


//L_6E69:
case 0x6E69: //C9 25 90
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x25;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6E6B:
case 0x6E6B: //90 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6E6B,0x90);
        PCSTART=0x6E6F;
        /**/return;
        //goto L_6E6F;
    } 
//case 0x6E6D: //A9 00 95
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_6E6F:
case 0x6E6F: //95 34 A9
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x34; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_6E71:
case 0x6E71: //A9 00 60
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_6E73:
case 0x6E73: //60 A5 1C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6E73,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6E74:
case 0x6E74: //A5 1C F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_6E76:
case 0x6E76: //F0 5F AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6E76,0xF0);
        PCSTART=0x6ED7;
        /**/return;
        //goto L_6ED7;
    } 
//case 0x6E78: //AD 1B 02
    //clockticks+=4;
    A=ReadMemory(0x021B); 
  ZN=A;


//L_6E7B:
case 0x6E7B: //30 5A AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6E7B,0x30);
        PCSTART=0x6ED7;
        /**/return;
        //goto L_6ED7;
    }
//case 0x6E7D: //AD FA 02
    //clockticks+=4;
    A=ReadMemory(0x02FA); 
  ZN=A;


//L_6E80:
case 0x6E80: //D0 55 AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6E80,0xD0);
        PCSTART=0x6ED7;
        /**/return;
        //goto L_6ED7;
    }
//case 0x6E82: //AD 03 20
    //clockticks+=4;
    A=ReadMemory(0x2003); 
  ZN=A;


//L_6E85:
case 0x6E85: //10 50 A9
clockticks+=24; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6E85,0x10);
        PCSTART=0x6ED7;
        /**/return;
        //goto L_6ED7;
    }
//case 0x6E87: //A9 00 8D
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x6E89: //8D 1B 02
    //clockticks+=4;
    WriteMemory(0x021B,A);
//case 0x6E8C: //8D 3E 02
    //clockticks+=4;
    WriteMemory(0x023E,A);
//case 0x6E8F: //8D 61 02
    //clockticks+=4;
    WriteMemory(0x0261,A);
//case 0x6E92: //A9 30 8D
    //clockticks+=3;
    A=0x30;
  ZN=A;
//case 0x6E94: //8D FA 02
    //clockticks+=4;
    WriteMemory(0x02FA,A);


//L_6E97:
case 0x6E97: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6E;
    stack[S--]=0x9A;
    showme(0x6E97,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_6E9A:
case 0x6E9A: //29 1F C9
clockticks+=6; //SUM
    //clockticks+=3;
    A &= 0x1F;
//  ZN=A;
//case 0x6E9C: //C9 1D 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x1D;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6E9E:
case 0x6E9E: //90 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6E9E,0x90);
        PCSTART=0x6EA2;
        /**/return;
        //goto L_6EA2;
    } 
//case 0x6EA0: //A9 1C C9
    //clockticks+=3;
    A=0x1C;
  ZN=A;


//L_6EA2:
case 0x6EA2: //C9 03 B0
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x03;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6EA4:
case 0x6EA4: //B0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6EA4,0xB0);
        PCSTART=0x6EA8;
        /**/return;
        //goto L_6EA8;
    } 
//case 0x6EA6: //A9 03 8D
    //clockticks+=3;
    A=0x03;
  ZN=A;


//L_6EA8:
case 0x6EA8: //8D 84 02
clockticks+=7; //SUM
    //clockticks+=4;
    WriteMemory(0x0284,A);
//case 0x6EAB: //A2 05 20
    //clockticks+=3;
    X=0x05;
  ZN=X;


//L_6EAD:
case 0x6EAD: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6E;
    stack[S--]=0xB0;
    showme(0x6EAD,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_6EB0:
case 0x6EB0: //CA D0 FA
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_6EB1:
case 0x6EB1: //D0 FA 29
clockticks+=10; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6EB1,0xD0);
        PCSTART=0x6EAD;
        /**/return;
        //goto L_6EAD;
    }
//case 0x6EB3: //29 1F E8
    //clockticks+=3;
    A &= 0x1F;
//  ZN=A;
//case 0x6EB5: //E8 C9 18
    //clockticks+=2;
    X++; 
//  ZN=X;
//case 0x6EB6: //C9 18 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x18;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6EB8:
case 0x6EB8: //90 0C 29
clockticks+=14; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6EB8,0x90);
        PCSTART=0x6EC6;
        /**/return;
        //goto L_6EC6;
    } 
//case 0x6EBA: //29 07 0A
    //clockticks+=3;
    A &= 0x07;
//  ZN=A;
//case 0x6EBC: //0A 69 04
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x6EBD: //69 04 CD
    //clockticks+=3;
    ADC(0x04);
//case 0x6EBF: //CD F6 02
    //clockticks+=4;
    value = ReadMemory(0x02F6); 
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6EC2:
case 0x6EC2: //90 02 A2
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6EC2,0x90);
        PCSTART=0x6EC6;
        /**/return;
        //goto L_6EC6;
    } 
//case 0x6EC4: //A2 80 C9
    //clockticks+=3;
    X=0x80;
  ZN=X;


//L_6EC6:
case 0x6EC6: //C9 15 90
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x15;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6EC8:
case 0x6EC8: //90 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6EC8,0x90);
        PCSTART=0x6ECC;
        /**/return;
        //goto L_6ECC;
    } 
//case 0x6ECA: //A9 14 C9
    //clockticks+=3;
    A=0x14;
  ZN=A;


//L_6ECC:
case 0x6ECC: //C9 03 B0
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x03;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6ECE:
case 0x6ECE: //B0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6ECE,0xB0);
        PCSTART=0x6ED2;
        /**/return;
        //goto L_6ED2;
    } 
//case 0x6ED0: //A9 03 8D
    //clockticks+=3;
    A=0x03;
  ZN=A;


//L_6ED2:
case 0x6ED2: //8D A7 02
clockticks+=6; //SUM
    //clockticks+=4;
    WriteMemory(0x02A7,A);
//case 0x6ED5: //86 59 60
    //clockticks+=2;
    WriteMemory(0x59,X);


//L_6ED7:
case 0x6ED7: //60 A9 02
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6ED7,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6ED8:
case 0x6ED8: //A9 02 8D
clockticks+=16; //SUM
    //clockticks+=3;
    A=0x02;
//  ZN=A;
//case 0x6EDA: //8D F5 02
    //clockticks+=4;
    WriteMemory(0x02F5,A);
//case 0x6EDD: //A2 03 4E
    //clockticks+=3;
    X=0x03;
//  ZN=X;
//case 0x6EDF: //4E 02 28
    //clockticks+=6;
    value=ReadMemory(0x2802);
    C = value;
    value = value >> 1;
    WriteMemory(0x2802,value);
  ZN=value;


//L_6EE2:
case 0x6EE2: //B0 01 E8
clockticks+=4; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6EE2,0xB0);
        PCSTART=0x6EE5;
        /**/return;
        //goto L_6EE5;
    } 
//case 0x6EE4: //E8 86 56
    //clockticks+=2;
    X++; 
  ZN=X;


//L_6EE5:
case 0x6EE5: //86 56 A9
clockticks+=8; //SUM
    //clockticks+=2;
    WriteMemory(0x56,X);
//case 0x6EE7: //A9 00 A2
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x6EE9: //A2 04 9D
    //clockticks+=3;
    X=0x04;
  ZN=X;


//L_6EEB:
case 0x6EEB: //9D 1B 02
clockticks+=16; //SUM
    //clockticks+=5;
    temp=0x021B; temp+=X;
    WriteMemory(temp,A);
//case 0x6EEE: //9D 1F 02
    //clockticks+=5;
    temp=0x021F; temp+=X;
    WriteMemory(temp,A);
//case 0x6EF1: //95 51 CA
    //clockticks+=4;
    temp=0x51; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x6EF3: //CA 10 F5
    //clockticks+=2;
    X--; 
  ZN=X;


//L_6EF4:
case 0x6EF4: //10 F5 8D
clockticks+=6; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6EF4,0x10);
        PCSTART=0x6EEB;
        /**/return;
        //goto L_6EEB;
    }
//case 0x6EF6: //8D F6 02
    //clockticks+=4;
    WriteMemory(0x02F6,A);


//L_6EF9:
case 0x6EF9: //60 A9 00
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6EF9,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6EFA:
case 0x6EFA: //A9 00 8D
clockticks+=39; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x6EFC: //8D 00 36
    //clockticks+=4;
    WriteMemory(0x3600,A);
//case 0x6EFF: //8D 00 3A
    //clockticks+=4;
    WriteMemory(0x3A00,A);
//case 0x6F02: //8D 00 3C
    //clockticks+=4;
    WriteMemory(0x3C00,A);
//case 0x6F05: //8D 01 3C
    //clockticks+=4;
    WriteMemory(0x3C01,A);
//case 0x6F08: //8D 03 3C
    //clockticks+=4;
    WriteMemory(0x3C03,A);
//case 0x6F0B: //8D 04 3C
    //clockticks+=4;
    WriteMemory(0x3C04,A);
//case 0x6F0E: //8D 05 3C
    //clockticks+=4;
    WriteMemory(0x3C05,A);
//case 0x6F11: //85 69 85
    //clockticks+=2;
    WriteMemory(0x69,A);
//case 0x6F13: //85 66 85
    //clockticks+=2;
    WriteMemory(0x66,A);
//case 0x6F15: //85 67 85
    //clockticks+=2;
    WriteMemory(0x67,A);
//case 0x6F17: //85 68 60
    //clockticks+=2;
    WriteMemory(0x68,A);


//L_6F19:
case 0x6F19: //60 B9 34
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6F19,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6F1A:
case 0x6F1A: //B9 34 00
clockticks+=8; //SUM
    //clockticks+=4;
    temp=0x0034; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6F1D: //0A A8 D0
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x6F1E: //A8 D0 14
    //clockticks+=2;
    Y=A;
  ZN=Y;


//L_6F1F:
case 0x6F1F: //D0 14 A5
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6F1F,0xD0);
        PCSTART=0x6F35;
        /**/return;
        //goto L_6F35;
    }
//case 0x6F21: //A5 32 25
    //clockticks+=3;
    A=ReadMemory(0x32); 
//  ZN=A;
//case 0x6F23: //25 33 30
    //clockticks+=3;
    A &= ReadMemory(0x33);
  ZN=A;


//L_6F25:
case 0x6F25: //30 0E A9
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6F25,0x30);
        PCSTART=0x6F35;
        /**/return;
        //goto L_6F35;
    }
//case 0x6F27: //A9 72 A2
    //clockticks+=3;
    A=0x72;
//  ZN=A;
//case 0x6F29: //A2 F8 20
    //clockticks+=3;
    X=0xF8;
  ZN=X;


//L_6F2B:
case 0x6F2B: //20 45 7D
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6F;
    stack[S--]=0x2E;
    showme(0x6F2B,0x20);
    PCSTART=0x7D45; //jsr
    /**/return;
    //goto L_7D45;


//L_6F2E:
case 0x6F2E: //A9 01 A2
clockticks+=6; //SUM
    //clockticks+=3;
    A=0x01;
//  ZN=A;
//case 0x6F30: //A2 F8 4C
    //clockticks+=3;
    X=0xF8;
  ZN=X;


//L_6F32:
case 0x6F32: //4C 45 7D
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6F32,0x4C);
    PCSTART=0x7D45;
    /**/return;
    //goto L_7D45;


//L_6F35:
case 0x6F35: //BE D5 56
clockticks+=8; //SUM
    //clockticks+=4;
    temp=0x56D5; temp+=Y;
    X=ReadMemory(temp);
//  ZN=X;
//case 0x6F38: //B9 D4 56
    //clockticks+=4;
    temp=0x56D4; temp+=Y;
    A=ReadMemory(temp);
  ZN=A;


//L_6F3B:
case 0x6F3B: //4C 45 7D
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6F3B,0x4C);
    PCSTART=0x7D45;
    /**/return;
    //goto L_7D45;


//L_6F3E:
case 0x6F3E: //F0 16 84
clockticks+=12; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6F3E,0xF0);
        PCSTART=0x6F56;
        /**/return;
        //goto L_6F56;
    } 
//case 0x6F40: //84 08 A2
    //clockticks+=2;
    WriteMemory(0x08,Y);
//case 0x6F42: //A2 D5 A0
    //clockticks+=3;
    X=0xD5;
//  ZN=X;
//case 0x6F44: //A0 E0 84
    //clockticks+=3;
    Y = 0xE0; 
  ZN=Y;
//case 0x6F46: //84 00 20
    //clockticks+=2;
    WriteMemory(0x00,Y);


//L_6F48:
case 0x6F48: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6F;
    stack[S--]=0x4B;
    showme(0x6F48,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_6F4B:
case 0x6F4B: //A2 DA A9
clockticks+=6; //SUM
    //clockticks+=3;
    X=0xDA;
//  ZN=X;
//case 0x6F4D: //A9 54 20
    //clockticks+=3;
    A=0x54;
  ZN=A;


//L_6F4F:
case 0x6F4F: //20 FC 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6F;
    stack[S--]=0x52;
    showme(0x6F4F,0x20);
    PCSTART=0x7BFC; //jsr
    /**/return;
    //goto L_7BFC;


//L_6F52:
case 0x6F52: //C6 08 D0
clockticks+=5; //SUM
    //clockticks+=5;
    value=ReadMemory(0x08);
    value--;
    WriteMemory(0x08,value);
  ZN=value;


//L_6F54:
case 0x6F54: //D0 F5 60
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6F54,0xD0);
        PCSTART=0x6F4B;
        /**/return;
        //goto L_6F4B;
    }


//L_6F56:
case 0x6F56: //60 A2 22
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6F56,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6F57:
case 0x6F57: //A2 22 BD
clockticks+=3; //SUM
    //clockticks+=3;
    X=0x22;
  ZN=X;


//L_6F59:
case 0x6F59: //BD 00 02
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0200; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_6F5C:
case 0x6F5C: //D0 04 CA
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6F5C,0xD0);
        PCSTART=0x6F62;
        /**/return;
        //goto L_6F62;
    }


//L_6F5E:
case 0x6F5E: //CA 10 F8
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_6F5F:
case 0x6F5F: //10 F8 60
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6F5F,0x10);
        PCSTART=0x6F59;
        /**/return;
        //goto L_6F59;
    }


//L_6F61:
case 0x6F61: //60 10 63
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x6F61,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_6F62:
case 0x6F62: //10 63 20
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6F62,0x10);
        PCSTART=0x6FC7;
        /**/return;
        //goto L_6FC7;
    }


//L_6F64:
case 0x6F64: //20 08 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6F;
    stack[S--]=0x67;
    showme(0x6F64,0x20);
    PCSTART=0x7708; //jsr
    /**/return;
    //goto L_7708;


//L_6F67:
case 0x6F67: //4A 4A 4A
clockticks+=11; //SUM
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6F68: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6F69: //4A 4A E0
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6F6A: //4A E0 1B
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x6F6B: //E0 1B D0
    //clockticks+=3;
    value = 0x1B;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6F6D:
case 0x6F6D: //D0 07 A5
clockticks+=10; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6F6D,0xD0);
        PCSTART=0x6F76;
        /**/return;
        //goto L_6F76;
    }
//case 0x6F6F: //A5 5C 29
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x6F71: //29 01 4A
    //clockticks+=3;
    A &= 0x01;
//  ZN=A;
//case 0x6F73: //4A F0 01
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_6F74:
case 0x6F74: //F0 01 38
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6F74,0xF0);
        PCSTART=0x6F77;
        /**/return;
        //goto L_6F77;
    } 


//L_6F76:
case 0x6F76: //38 7D 00
clockticks+=2; //SUM
    //clockticks+=2;
    C = 0x01;


//L_6F77:
case 0x6F77: //7D 00 02
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0200; temp+=X;
    ADC(ReadMemory(temp));


//L_6F7A:
case 0x6F7A: //30 25 E0
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x6F7A,0x30);
        PCSTART=0x6FA1;
        /**/return;
        //goto L_6FA1;
    }
//case 0x6F7C: //E0 1B F0
    //clockticks+=3;
    value = 0x1B;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6F7E:
case 0x6F7E: //F0 13 B0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6F7E,0xF0);
        PCSTART=0x6F93;
        /**/return;
        //goto L_6F93;
    } 


//L_6F80:
case 0x6F80: //B0 17 CE
clockticks+=8; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x6F80,0xB0);
        PCSTART=0x6F99;
        /**/return;
        //goto L_6F99;
    } 
//case 0x6F82: //CE F6 02
    //clockticks+=6;
    value=ReadMemory(0x02F6);
    value--;
    WriteMemory(0x02F6,value);
  ZN=value;


//L_6F85:
case 0x6F85: //D0 05 A0
clockticks+=9; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6F85,0xD0);
        PCSTART=0x6F8C;
        /**/return;
        //goto L_6F8C;
    }
//case 0x6F87: //A0 7F 8C
    //clockticks+=3;
    Y = 0x7F; 
  ZN=Y;
//case 0x6F89: //8C FB 02
    //clockticks+=4;
    WriteMemory(0x02FB,Y);


//L_6F8C:
case 0x6F8C: //A9 00 9D
clockticks+=8; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x6F8E: //9D 00 02
    //clockticks+=5;
    temp=0x0200; temp+=X;
    WriteMemory(temp,A);


//L_6F91:
case 0x6F91: //F0 CB 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x6F91,0xF0);
        PCSTART=0x6F5E;
        /**/return;
        //goto L_6F5E;
    } 


//L_6F93:
case 0x6F93: //20 E8 71
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6F;
    stack[S--]=0x96;
    showme(0x6F93,0x20);
    PCSTART=0x71E8; //jsr
    /**/return;
    //goto L_71E8;


//L_6F96:
case 0x6F96: //4C 8C 6F
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6F96,0x4C);
    PCSTART=0x6F8C;
    /**/return;
    //goto L_6F8C;


//L_6F99:
case 0x6F99: //AD F8 02
clockticks+=8; //SUM
    //clockticks+=4;
    A=ReadMemory(0x02F8); 
  ZN=A;
//case 0x6F9C: //8D F7 02
    //clockticks+=4;
    WriteMemory(0x02F7,A);


//L_6F9F:
case 0x6F9F: //D0 EB 9D
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6F9F,0xD0);
        PCSTART=0x6F8C;
        /**/return;
        //goto L_6F8C;
    }


//L_6FA1:
case 0x6FA1: //9D 00 02
clockticks+=16; //SUM
    //clockticks+=5;
    temp=0x0200; temp+=X;
    WriteMemory(temp,A);
//case 0x6FA4: //29 F0 18
    //clockticks+=3;
    A &= 0xF0;
//  ZN=A;
//case 0x6FA6: //18 69 10
    //clockticks+=2;
    C = 0;
//case 0x6FA7: //69 10 E0
    //clockticks+=3;
    ADC(0x10);
//case 0x6FA9: //E0 1B D0
    //clockticks+=3;
    value = 0x1B;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6FAB:
case 0x6FAB: //D0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6FAB,0xD0);
        PCSTART=0x6FAF;
        /**/return;
        //goto L_6FAF;
    }
//case 0x6FAD: //A9 00 A8
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_6FAF:
case 0x6FAF: //A8 BD AF
clockticks+=26; //SUM
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x6FB0: //BD AF 02
    //clockticks+=4;
    temp=0x02AF; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6FB3: //85 04 BD
    //clockticks+=2;
    WriteMemory(0x04,A);
//case 0x6FB5: //BD 69 02
    //clockticks+=4;
    temp=0x0269; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6FB8: //85 05 BD
    //clockticks+=2;
    WriteMemory(0x05,A);
//case 0x6FBA: //BD D2 02
    //clockticks+=4;
    temp=0x02D2; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x6FBD: //85 06 BD
    //clockticks+=2;
    WriteMemory(0x06,A);
//case 0x6FBF: //BD 8C 02
    //clockticks+=4;
    temp=0x028C; temp+=X;
    A=ReadMemory(temp);
  ZN=A;
//case 0x6FC2: //85 07 4C
    //clockticks+=2;
    WriteMemory(0x07,A);


//L_6FC4:
case 0x6FC4: //4C 27 70
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6FC4,0x4C);
    PCSTART=0x7027;
    /**/return;
    //goto L_7027;


//L_6FC7:
case 0x6FC7: //18 A0 00
clockticks+=9; //SUM
    //clockticks+=2;
    C = 0;
//case 0x6FC8: //A0 00 BD
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x6FCA: //BD 23 02
    //clockticks+=4;
    temp=0x0223; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_6FCD:
case 0x6FCD: //10 01 88
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6FCD,0x10);
        PCSTART=0x6FD0;
        /**/return;
        //goto L_6FD0;
    }
//case 0x6FCF: //88 7D AF
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_6FD0:
case 0x6FD0: //7D AF 02
clockticks+=20; //SUM
    //clockticks+=4;
    temp=0x02AF; temp+=X;
    ADC(ReadMemory(temp));
//case 0x6FD3: //9D AF 02
    //clockticks+=5;
    temp=0x02AF; temp+=X;
    WriteMemory(temp,A);
//case 0x6FD6: //85 04 98
    //clockticks+=2;
    WriteMemory(0x04,A);
//case 0x6FD8: //98 7D 69
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x6FD9: //7D 69 02
    //clockticks+=4;
    temp=0x0269; temp+=X;
    ADC(ReadMemory(temp));
//case 0x6FDC: //C9 20 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x20;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_6FDE:
case 0x6FDE: //90 0C 29
clockticks+=8; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x6FDE,0x90);
        PCSTART=0x6FEC;
        /**/return;
        //goto L_6FEC;
    } 
//case 0x6FE0: //29 1F E0
    //clockticks+=3;
    A &= 0x1F;
//  ZN=A;
//case 0x6FE2: //E0 1C D0
    //clockticks+=3;
    value = 0x1C;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_6FE4:
case 0x6FE4: //D0 06 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x6FE4,0xD0);
        PCSTART=0x6FEC;
        /**/return;
        //goto L_6FEC;
    }


//L_6FE6:
case 0x6FE6: //20 2D 70
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x6F;
    stack[S--]=0xE9;
    showme(0x6FE6,0x20);
    PCSTART=0x702D; //jsr
    /**/return;
    //goto L_702D;


//L_6FE9:
case 0x6FE9: //4C 5E 6F
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x6FE9,0x4C);
    PCSTART=0x6F5E;
    /**/return;
    //goto L_6F5E;


//L_6FEC:
case 0x6FEC: //9D 69 02
clockticks+=16; //SUM
    //clockticks+=5;
    temp=0x0269; temp+=X;
    WriteMemory(temp,A);
//case 0x6FEF: //85 05 18
    //clockticks+=2;
    WriteMemory(0x05,A);
//case 0x6FF1: //18 A0 00
    //clockticks+=2;
    C = 0;
//case 0x6FF2: //A0 00 BD
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x6FF4: //BD 46 02
    //clockticks+=4;
    temp=0x0246; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_6FF7:
case 0x6FF7: //10 02 A0
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x6FF7,0x10);
        PCSTART=0x6FFB;
        /**/return;
        //goto L_6FFB;
    }
//case 0x6FF9: //A0 FF 7D
    //clockticks+=3;
    Y = 0xFF; 
  ZN=Y;


//L_6FFB:
case 0x6FFB: //7D D2 02
clockticks+=20; //SUM
    //clockticks+=4;
    temp=0x02D2; temp+=X;
    ADC(ReadMemory(temp));
//case 0x6FFE: //9D D2 02
    //clockticks+=5;
    temp=0x02D2; temp+=X;
    WriteMemory(temp,A);
//case 0x7001: //85 06 98
    //clockticks+=2;
    WriteMemory(0x06,A);
//case 0x7003: //98 7D 8C
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x7004: //7D 8C 02
    //clockticks+=4;
    temp=0x028C; temp+=X;
    ADC(ReadMemory(temp));
//case 0x7007: //C9 18 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x18;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7009:
case 0x7009: //90 08 F0
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7009,0x90);
        PCSTART=0x7013;
        /**/return;
        //goto L_7013;
    } 


//L_700B:
case 0x700B: //F0 04 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x700B,0xF0);
        PCSTART=0x7011;
        /**/return;
        //goto L_7011;
    } 
//case 0x700D: //A9 17 D0
    //clockticks+=3;
    A=0x17;
  ZN=A;


//L_700F:
case 0x700F: //D0 02 A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x700F,0xD0);
        PCSTART=0x7013;
        /**/return;
        //goto L_7013;
    }


//L_7011:
case 0x7011: //A9 00 9D
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_7013:
case 0x7013: //9D 8C 02
clockticks+=16; //SUM
    //clockticks+=5;
    temp=0x028C; temp+=X;
    WriteMemory(temp,A);
//case 0x7016: //85 07 BD
    //clockticks+=2;
    WriteMemory(0x07,A);
//case 0x7018: //BD 00 02
    //clockticks+=4;
    temp=0x0200; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x701B: //A0 E0 4A
    //clockticks+=3;
    Y = 0xE0; 
//  ZN=Y;
//case 0x701D: //4A B0 07
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_701E:
case 0x701E: //B0 07 A0
clockticks+=7; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x701E,0xB0);
        PCSTART=0x7027;
        /**/return;
        //goto L_7027;
    } 
//case 0x7020: //A0 F0 4A
    //clockticks+=3;
    Y = 0xF0; 
//  ZN=Y;
//case 0x7022: //4A B0 02
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_7023:
case 0x7023: //B0 02 A0
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7023,0xB0);
        PCSTART=0x7027;
        /**/return;
        //goto L_7027;
    } 
//case 0x7025: //A0 00 20
    //clockticks+=3;
    Y = 0x00; 
  ZN=Y;


//L_7027:
case 0x7027: //20 FE 72
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x70;
    stack[S--]=0x2A;
    showme(0x7027,0x20);
    PCSTART=0x72FE; //jsr
    /**/return;
    //goto L_72FE;


//L_702A:
case 0x702A: //4C 5E 6F
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x702A,0x4C);
    PCSTART=0x6F5E;
    /**/return;
    //goto L_6F5E;


//L_702D:
case 0x702D: //AD F8 02
clockticks+=23; //SUM
    //clockticks+=4;
    A=ReadMemory(0x02F8); 
//  ZN=A;
//case 0x7030: //8D F7 02
    //clockticks+=4;
    WriteMemory(0x02F7,A);
//case 0x7033: //A9 00 8D
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x7035: //8D 1C 02
    //clockticks+=4;
    WriteMemory(0x021C,A);
//case 0x7038: //8D 3F 02
    //clockticks+=4;
    WriteMemory(0x023F,A);
//case 0x703B: //8D 62 02
    //clockticks+=4;
    WriteMemory(0x0262,A);


//L_703E:
case 0x703E: //60 A5 1C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x703E,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_703F:
case 0x703F: //A5 1C F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_7041:
case 0x7041: //F0 42 AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7041,0xF0);
        PCSTART=0x7085;
        /**/return;
        //goto L_7085;
    } 
//case 0x7043: //AD 1B 02
    //clockticks+=4;
    A=ReadMemory(0x021B); 
  ZN=A;


//L_7046:
case 0x7046: //30 3D AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7046,0x30);
        PCSTART=0x7085;
        /**/return;
        //goto L_7085;
    }
//case 0x7048: //AD FA 02
    //clockticks+=4;
    A=ReadMemory(0x02FA); 
  ZN=A;


//L_704B:
case 0x704B: //F0 39 CE
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x704B,0xF0);
        PCSTART=0x7086;
        /**/return;
        //goto L_7086;
    } 
//case 0x704D: //CE FA 02
    //clockticks+=6;
    value=ReadMemory(0x02FA);
    value--;
    WriteMemory(0x02FA,value);
  ZN=value;


//L_7050:
case 0x7050: //D0 33 A4
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7050,0xD0);
        PCSTART=0x7085;
        /**/return;
        //goto L_7085;
    }
//case 0x7052: //A4 59 30
    //clockticks+=3;
    Y=ReadMemory(0x59);
  ZN=Y;


//L_7054:
case 0x7054: //30 19 D0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7054,0x30);
        PCSTART=0x706F;
        /**/return;
        //goto L_706F;
    }


//L_7056:
case 0x7056: //D0 10 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7056,0xD0);
        PCSTART=0x7068;
        /**/return;
        //goto L_7068;
    }


//L_7058:
case 0x7058: //20 39 71
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x70;
    stack[S--]=0x5B;
    showme(0x7058,0x20);
    PCSTART=0x7139; //jsr
    /**/return;
    //goto L_7139;


//L_705B:
case 0x705B: //D0 24 AC
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x705B,0xD0);
        PCSTART=0x7081;
        /**/return;
        //goto L_7081;
    }
//case 0x705D: //AC 1C 02
    //clockticks+=4;
    Y=ReadMemory(0x021C);
  ZN=Y;


//L_7060:
case 0x7060: //F0 06 A0
clockticks+=9; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7060,0xF0);
        PCSTART=0x7068;
        /**/return;
        //goto L_7068;
    } 
//case 0x7062: //A0 02 8C
    //clockticks+=3;
    Y = 0x02; 
  ZN=Y;
//case 0x7064: //8C FA 02
    //clockticks+=4;
    WriteMemory(0x02FA,Y);


//L_7067:
case 0x7067: //60 A9 01
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7067,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7068:
case 0x7068: //A9 01 8D
clockticks+=7; //SUM
    //clockticks+=3;
    A=0x01;
  ZN=A;
//case 0x706A: //8D 1B 02
    //clockticks+=4;
    WriteMemory(0x021B,A);


//L_706D:
case 0x706D: //D0 12 A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x706D,0xD0);
        PCSTART=0x7081;
        /**/return;
        //goto L_7081;
    }


//L_706F:
case 0x706F: //A9 A0 8D
clockticks+=28; //SUM
    //clockticks+=3;
    A=0xA0;
//  ZN=A;
//case 0x7071: //8D 1B 02
    //clockticks+=4;
    WriteMemory(0x021B,A);
//case 0x7074: //A2 3E 86
    //clockticks+=3;
    X=0x3E;
//  ZN=X;
//case 0x7076: //86 69 A6
    //clockticks+=2;
    WriteMemory(0x69,X);
//case 0x7078: //A6 18 D6
    //clockticks+=3;
    X=ReadMemory(0x18); 
//  ZN=X;
//case 0x707A: //D6 57 A9
    //clockticks+=6;
    temp=0x0057; temp+=X; temp&=0xFF;
    value=ReadMemory(temp);
    value--;
    WriteMemory(temp,value);
//  ZN=value;
//case 0x707C: //A9 81 8D
    //clockticks+=3;
    A=0x81;
  ZN=A;
//case 0x707E: //8D FA 02
    //clockticks+=4;
    WriteMemory(0x02FA,A);


//L_7081:
case 0x7081: //A9 00 85
clockticks+=5; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x7083: //85 59 60
    //clockticks+=2;
    WriteMemory(0x59,A);


//L_7085:
case 0x7085: //60 AD 07
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7085,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7086:
case 0x7086: //AD 07 24
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2407); 
  ZN=A;


//L_7089:
case 0x7089: //10 04 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7089,0x10);
        PCSTART=0x708F;
        /**/return;
        //goto L_708F;
    }
//case 0x708B: //A9 03 D0
    //clockticks+=3;
    A=0x03;
  ZN=A;


//L_708D:
case 0x708D: //D0 07 AD
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x708D,0xD0);
        PCSTART=0x7096;
        /**/return;
        //goto L_7096;
    }


//L_708F:
case 0x708F: //AD 06 24
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2406); 
  ZN=A;


//L_7092:
case 0x7092: //10 07 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7092,0x10);
        PCSTART=0x709B;
        /**/return;
        //goto L_709B;
    }
//case 0x7094: //A9 FD 18
    //clockticks+=3;
    A=0xFD;
  ZN=A;


//L_7096:
case 0x7096: //18 65 61
clockticks+=7; //SUM
    //clockticks+=2;
    C = 0;
//case 0x7097: //65 61 85
    //clockticks+=3;
    ADC(ReadMemory(0x61));
//case 0x7099: //85 61 A5
    //clockticks+=2;
    WriteMemory(0x61,A);


//L_709B:
case 0x709B: //A5 5C 4A
clockticks+=5; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x709D: //4A B0 E5
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_709E:
case 0x709E: //B0 E5 AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x709E,0xB0);
        PCSTART=0x7085;
        /**/return;
        //goto L_7085;
    } 
//case 0x70A0: //AD 05 24
    //clockticks+=4;
    A=ReadMemory(0x2405); 
  ZN=A;


//L_70A3:
case 0x70A3: //10 3C A9
clockticks+=15; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x70A3,0x10);
        PCSTART=0x70E1;
        /**/return;
        //goto L_70E1;
    }
//case 0x70A5: //A9 80 8D
    //clockticks+=3;
    A=0x80;
//  ZN=A;
//case 0x70A7: //8D 03 3C
    //clockticks+=4;
    WriteMemory(0x3C03,A);
//case 0x70AA: //A0 00 A5
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x70AC: //A5 61 20
    //clockticks+=3;
    A=ReadMemory(0x61); 
  ZN=A;


//L_70AE:
case 0x70AE: //20 D2 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x70;
    stack[S--]=0xB1;
    showme(0x70AE,0x20);
    PCSTART=0x77D2; //jsr
    /**/return;
    //goto L_77D2;


//L_70B1:
case 0x70B1: //10 01 88
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x70B1,0x10);
        PCSTART=0x70B4;
        /**/return;
        //goto L_70B4;
    }
//case 0x70B3: //88 0A 18
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_70B4:
case 0x70B4: //0A 18 65
clockticks+=15; //SUM
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x70B5: //18 65 64
    //clockticks+=2;
    C = 0;
//case 0x70B6: //65 64 AA
    //clockticks+=3;
    ADC(ReadMemory(0x64));
//case 0x70B8: //AA 98 6D
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x70B9: //98 6D 3E
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x70BA: //6D 3E 02
    //clockticks+=4;
    ADC(ReadMemory(0x023E));


//L_70BD:
case 0x70BD: //20 25 71
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x70;
    stack[S--]=0xC0;
    showme(0x70BD,0x20);
    PCSTART=0x7125; //jsr
    /**/return;
    //goto L_7125;


//L_70C0:
case 0x70C0: //8D 3E 02
clockticks+=12; //SUM
    //clockticks+=4;
    WriteMemory(0x023E,A);
//case 0x70C3: //86 64 A0
    //clockticks+=2;
    WriteMemory(0x64,X);
//case 0x70C5: //A0 00 A5
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x70C7: //A5 61 20
    //clockticks+=3;
    A=ReadMemory(0x61); 
  ZN=A;


//L_70C9:
case 0x70C9: //20 D5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x70;
    stack[S--]=0xCC;
    showme(0x70C9,0x20);
    PCSTART=0x77D5; //jsr
    /**/return;
    //goto L_77D5;


//L_70CC:
case 0x70CC: //10 01 88
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x70CC,0x10);
        PCSTART=0x70CF;
        /**/return;
        //goto L_70CF;
    }
//case 0x70CE: //88 0A 18
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_70CF:
case 0x70CF: //0A 18 65
clockticks+=15; //SUM
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x70D0: //18 65 65
    //clockticks+=2;
    C = 0;
//case 0x70D1: //65 65 AA
    //clockticks+=3;
    ADC(ReadMemory(0x65));
//case 0x70D3: //AA 98 6D
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x70D4: //98 6D 61
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x70D5: //6D 61 02
    //clockticks+=4;
    ADC(ReadMemory(0x0261));


//L_70D8:
case 0x70D8: //20 25 71
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x70;
    stack[S--]=0xDB;
    showme(0x70D8,0x20);
    PCSTART=0x7125; //jsr
    /**/return;
    //goto L_7125;


//L_70DB:
case 0x70DB: //8D 61 02
clockticks+=6; //SUM
    //clockticks+=4;
    WriteMemory(0x0261,A);
//case 0x70DE: //86 65 60
    //clockticks+=2;
    WriteMemory(0x65,X);


//L_70E0:
case 0x70E0: //60 A9 00
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x70E0,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_70E1:
case 0x70E1: //A9 00 8D
clockticks+=14; //SUM
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x70E3: //8D 03 3C
    //clockticks+=4;
    WriteMemory(0x3C03,A);
//case 0x70E6: //AD 3E 02
    //clockticks+=4;
    A=ReadMemory(0x023E); 
//  ZN=A;
//case 0x70E9: //05 64 F0
    //clockticks+=3;
    A |= ReadMemory(0x64);
  ZN=A;


//L_70EB:
case 0x70EB: //F0 18 AD
clockticks+=16; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x70EB,0xF0);
        PCSTART=0x7105;
        /**/return;
        //goto L_7105;
    } 
//case 0x70ED: //AD 3E 02
    //clockticks+=4;
    A=ReadMemory(0x023E); 
//  ZN=A;
//case 0x70F0: //0A A2 FF
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x70F1: //A2 FF 18
    //clockticks+=3;
    X=0xFF;
//  ZN=X;
//case 0x70F3: //18 49 FF
    //clockticks+=2;
    C = 0;
//case 0x70F4: //49 FF 30
    //clockticks+=3;
    A ^= 0xFF;
  ZN=A;


//L_70F6:
case 0x70F6: //30 02 E8
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x70F6,0x30);
        PCSTART=0x70FA;
        /**/return;
        //goto L_70FA;
    }
//case 0x70F8: //E8 38 65
    //clockticks+=2;
    X++; 
  ZN=X;
//case 0x70F9: //38 65 64
    //clockticks+=2;
    C = 0x01;


//L_70FA:
case 0x70FA: //65 64 85
clockticks+=15; //SUM
    //clockticks+=3;
    ADC(ReadMemory(0x64));
//case 0x70FC: //85 64 8A
    //clockticks+=2;
    WriteMemory(0x64,A);
//case 0x70FE: //8A 6D 3E
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x70FF: //6D 3E 02
    //clockticks+=4;
    ADC(ReadMemory(0x023E));
//case 0x7102: //8D 3E 02
    //clockticks+=4;
    WriteMemory(0x023E,A);


//L_7105:
case 0x7105: //A5 65 0D
clockticks+=7; //SUM
    //clockticks+=3;
    A=ReadMemory(0x65); 
//  ZN=A;
//case 0x7107: //0D 61 02
    //clockticks+=4;
    A |= ReadMemory(0x0261);
  ZN=A;


//L_710A:
case 0x710A: //F0 18 AD
clockticks+=16; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x710A,0xF0);
        PCSTART=0x7124;
        /**/return;
        //goto L_7124;
    } 
//case 0x710C: //AD 61 02
    //clockticks+=4;
    A=ReadMemory(0x0261); 
//  ZN=A;
//case 0x710F: //0A A2 FF
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7110: //A2 FF 18
    //clockticks+=3;
    X=0xFF;
//  ZN=X;
//case 0x7112: //18 49 FF
    //clockticks+=2;
    C = 0;
//case 0x7113: //49 FF 30
    //clockticks+=3;
    A ^= 0xFF;
  ZN=A;


//L_7115:
case 0x7115: //30 02 38
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7115,0x30);
        PCSTART=0x7119;
        /**/return;
        //goto L_7119;
    }
//case 0x7117: //38 E8 65
    //clockticks+=2;
    C = 0x01;
//case 0x7118: //E8 65 65
    //clockticks+=2;
    X++; 
  ZN=X;


//L_7119:
case 0x7119: //65 65 85
clockticks+=15; //SUM
    //clockticks+=3;
    ADC(ReadMemory(0x65));
//case 0x711B: //85 65 8A
    //clockticks+=2;
    WriteMemory(0x65,A);
//case 0x711D: //8A 6D 61
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x711E: //6D 61 02
    //clockticks+=4;
    ADC(ReadMemory(0x0261));
//case 0x7121: //8D 61 02
    //clockticks+=4;
    WriteMemory(0x0261,A);


//L_7124:
case 0x7124: //60 30 09
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7124,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7125:
case 0x7125: //30 09 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7125,0x30);
        PCSTART=0x7130;
        /**/return;
        //goto L_7130;
    }
//case 0x7127: //C9 40 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x40;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7129:
case 0x7129: //90 0D A2
clockticks+=8; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7129,0x90);
        PCSTART=0x7138;
        /**/return;
        //goto L_7138;
    } 
//case 0x712B: //A2 FF A9
    //clockticks+=3;
    X=0xFF;
//  ZN=X;
//case 0x712D: //A9 3F 60
    //clockticks+=3;
    A=0x3F;
  ZN=A;


//L_712F:
case 0x712F: //60 C9 C0
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x712F,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7130:
case 0x7130: //C9 C0 B0
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xC0;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7132:
case 0x7132: //B0 04 A2
clockticks+=8; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7132,0xB0);
        PCSTART=0x7138;
        /**/return;
        //goto L_7138;
    } 
//case 0x7134: //A2 01 A9
    //clockticks+=3;
    X=0x01;
//  ZN=X;
//case 0x7136: //A9 C0 60
    //clockticks+=3;
    A=0xC0;
  ZN=A;


//L_7138:
case 0x7138: //60 A2 1C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7138,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7139:
case 0x7139: //A2 1C BD
clockticks+=3; //SUM
    //clockticks+=3;
    X=0x1C;
  ZN=X;


//L_713B:
case 0x713B: //BD 00 02
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0200; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_713E:
case 0x713E: //F0 1E BD
clockticks+=15; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x713E,0xF0);
        PCSTART=0x715E;
        /**/return;
        //goto L_715E;
    } 
//case 0x7140: //BD 69 02
    //clockticks+=4;
    temp=0x0269; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7143: //38 ED 84
    //clockticks+=2;
    C = 0x01;
//case 0x7144: //ED 84 02
    //clockticks+=4;
    SBC(ReadMemory(0x0284));
//case 0x7147: //C9 04 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x04;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7149:
case 0x7149: //90 04 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7149,0x90);
        PCSTART=0x714F;
        /**/return;
        //goto L_714F;
    } 
//case 0x714B: //C9 FC 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xFC;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_714D:
case 0x714D: //90 0F BD
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x714D,0x90);
        PCSTART=0x715E;
        /**/return;
        //goto L_715E;
    } 


//L_714F:
case 0x714F: //BD 8C 02
clockticks+=13; //SUM
    //clockticks+=4;
    temp=0x028C; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7152: //38 ED A7
    //clockticks+=2;
    C = 0x01;
//case 0x7153: //ED A7 02
    //clockticks+=4;
    SBC(ReadMemory(0x02A7));
//case 0x7156: //C9 04 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x04;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7158:
case 0x7158: //90 09 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7158,0x90);
        PCSTART=0x7163;
        /**/return;
        //goto L_7163;
    } 
//case 0x715A: //C9 FC B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xFC;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_715C:
case 0x715C: //B0 05 CA
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x715C,0xB0);
        PCSTART=0x7163;
        /**/return;
        //goto L_7163;
    } 


//L_715E:
case 0x715E: //CA 10 DA
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_715F:
case 0x715F: //10 DA E8
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x715F,0x10);
        PCSTART=0x713B;
        /**/return;
        //goto L_713B;
    }
//case 0x7161: //E8 60 EE
    //clockticks+=2;
    X++; 
  ZN=X;


//L_7162:
case 0x7162: //60 EE FA
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7162,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7163:
case 0x7163: //EE FA 02
clockticks+=6; //SUM
    //clockticks+=6;
    value=ReadMemory(0x02FA);
    value++;
    WriteMemory(0x02FA,value);
  ZN=value;


//L_7166:
case 0x7166: //60 90 A2
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7166,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7168:
case 0x7168: //A2 1A AD
clockticks+=7; //SUM
    //clockticks+=3;
    X=0x1A;
//  ZN=X;
//case 0x716A: //AD FB 02
    //clockticks+=4;
    A=ReadMemory(0x02FB); 
  ZN=A;


//L_716D:
case 0x716D: //D0 70 AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x716D,0xD0);
        PCSTART=0x71DF;
        /**/return;
        //goto L_71DF;
    }
//case 0x716F: //AD 1C 02
    //clockticks+=4;
    A=ReadMemory(0x021C); 
  ZN=A;


//L_7172:
case 0x7172: //D0 73 8D
clockticks+=23; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7172,0xD0);
        PCSTART=0x71E7;
        /**/return;
        //goto L_71E7;
    }
//case 0x7174: //8D 3F 02
    //clockticks+=4;
    WriteMemory(0x023F,A);
//case 0x7177: //8D 62 02
    //clockticks+=4;
    WriteMemory(0x0262,A);
//case 0x717A: //EE FD 02
    //clockticks+=6;
    value=ReadMemory(0x02FD);
    value++;
    WriteMemory(0x02FD,value);
//  ZN=value;
//case 0x717D: //AD FD 02
    //clockticks+=4;
    A=ReadMemory(0x02FD); 
//  ZN=A;
//case 0x7180: //C9 0B 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x0B;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7182:
case 0x7182: //90 03 CE
clockticks+=8; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7182,0x90);
        PCSTART=0x7187;
        /**/return;
        //goto L_7187;
    } 
//case 0x7184: //CE FD 02
    //clockticks+=6;
    value=ReadMemory(0x02FD);
    value--;
    WriteMemory(0x02FD,value);
  ZN=value;


//L_7187:
case 0x7187: //AD F5 02
clockticks+=12; //SUM
    //clockticks+=4;
    A=ReadMemory(0x02F5); 
//  ZN=A;
//case 0x718A: //18 69 02
    //clockticks+=2;
    C = 0;
//case 0x718B: //69 02 C9
    //clockticks+=3;
    ADC(0x02);
//case 0x718D: //C9 0B 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x0B;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_718F:
case 0x718F: //90 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x718F,0x90);
        PCSTART=0x7193;
        /**/return;
        //goto L_7193;
    } 
//case 0x7191: //A9 0B 8D
    //clockticks+=3;
    A=0x0B;
  ZN=A;


//L_7193:
case 0x7193: //8D F6 02
clockticks+=13; //SUM
    //clockticks+=4;
    WriteMemory(0x02F6,A);
//case 0x7196: //8D F5 02
    //clockticks+=4;
    WriteMemory(0x02F5,A);
//case 0x7199: //85 08 A0
    //clockticks+=2;
    WriteMemory(0x08,A);
//case 0x719B: //A0 1C 20
    //clockticks+=3;
    Y = 0x1C; 
  ZN=Y;


//L_719D:
case 0x719D: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x71;
    stack[S--]=0xA0;
    showme(0x719D,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_71A0:
case 0x71A0: //29 18 09
clockticks+=11; //SUM
    //clockticks+=3;
    A &= 0x18;
//  ZN=A;
//case 0x71A2: //09 04 9D
    //clockticks+=3;
    A |= 0x04;
  ZN=A;
//case 0x71A4: //9D 00 02
    //clockticks+=5;
    temp=0x0200; temp+=X;
    WriteMemory(temp,A);


//L_71A7:
case 0x71A7: //20 03 72
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x71;
    stack[S--]=0xAA;
    showme(0x71A7,0x20);
    PCSTART=0x7203; //jsr
    /**/return;
    //goto L_7203;


//L_71AA:
case 0x71AA: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x71;
    stack[S--]=0xAD;
    showme(0x71AA,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_71AD:
case 0x71AD: //4A 29 1F
clockticks+=5; //SUM
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x71AE: //29 1F 90
    //clockticks+=3;
    A &= 0x1F;
  ZN=A;


//L_71B0:
case 0x71B0: //90 13 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x71B0,0x90);
        PCSTART=0x71C5;
        /**/return;
        //goto L_71C5;
    } 
//case 0x71B2: //C9 18 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x18;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_71B4:
case 0x71B4: //90 02 29
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x71B4,0x90);
        PCSTART=0x71B8;
        /**/return;
        //goto L_71B8;
    } 
//case 0x71B6: //29 17 9D
    //clockticks+=3;
    A &= 0x17;
  ZN=A;


//L_71B8:
case 0x71B8: //9D 8C 02
clockticks+=18; //SUM
    //clockticks+=5;
    temp=0x028C; temp+=X;
    WriteMemory(temp,A);
//case 0x71BB: //A9 00 9D
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x71BD: //9D 69 02
    //clockticks+=5;
    temp=0x0269; temp+=X;
    WriteMemory(temp,A);
//case 0x71C0: //9D AF 02
    //clockticks+=5;
    temp=0x02AF; temp+=X;
    WriteMemory(temp,A);


//L_71C3:
case 0x71C3: //F0 0B 9D
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x71C3,0xF0);
        PCSTART=0x71D0;
        /**/return;
        //goto L_71D0;
    } 


//L_71C5:
case 0x71C5: //9D 69 02
clockticks+=18; //SUM
    //clockticks+=5;
    temp=0x0269; temp+=X;
    WriteMemory(temp,A);
//case 0x71C8: //A9 00 9D
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x71CA: //9D 8C 02
    //clockticks+=5;
    temp=0x028C; temp+=X;
    WriteMemory(temp,A);
//case 0x71CD: //9D D2 02
    //clockticks+=5;
    temp=0x02D2; temp+=X;
    WriteMemory(temp,A);


//L_71D0:
case 0x71D0: //CA C6 08
clockticks+=7; //SUM
    //clockticks+=2;
    X--; 
//  ZN=X;
//case 0x71D1: //C6 08 D0
    //clockticks+=5;
    value=ReadMemory(0x08);
    value--;
    WriteMemory(0x08,value);
  ZN=value;


//L_71D3:
case 0x71D3: //D0 C8 A9
clockticks+=16; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x71D3,0xD0);
        PCSTART=0x719D;
        /**/return;
        //goto L_719D;
    }
//case 0x71D5: //A9 7F 8D
    //clockticks+=3;
    A=0x7F;
//  ZN=A;
//case 0x71D7: //8D F7 02
    //clockticks+=4;
    WriteMemory(0x02F7,A);
//case 0x71DA: //A9 30 8D
    //clockticks+=3;
    A=0x30;
  ZN=A;
//case 0x71DC: //8D FC 02
    //clockticks+=4;
    WriteMemory(0x02FC,A);


//L_71DF:
case 0x71DF: //A9 00 9D
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_71E1:
case 0x71E1: //9D 00 02
clockticks+=7; //SUM
    //clockticks+=5;
    temp=0x0200; temp+=X;
    WriteMemory(temp,A);
//case 0x71E4: //CA 10 FA
    //clockticks+=2;
    X--; 
  ZN=X;


//L_71E5:
case 0x71E5: //10 FA 60
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x71E5,0x10);
        PCSTART=0x71E1;
        /**/return;
        //goto L_71E1;
    }


//L_71E7:
case 0x71E7: //60 A9 60
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x71E7,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_71E8:
case 0x71E8: //A9 60 8D
clockticks+=36; //SUM
    //clockticks+=3;
    A=0x60;
//  ZN=A;
//case 0x71EA: //8D CA 02
    //clockticks+=4;
    WriteMemory(0x02CA,A);
//case 0x71ED: //8D ED 02
    //clockticks+=4;
    WriteMemory(0x02ED,A);
//case 0x71F0: //A9 00 8D
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x71F2: //8D 3E 02
    //clockticks+=4;
    WriteMemory(0x023E,A);
//case 0x71F5: //8D 61 02
    //clockticks+=4;
    WriteMemory(0x0261,A);
//case 0x71F8: //A9 10 8D
    //clockticks+=3;
    A=0x10;
//  ZN=A;
//case 0x71FA: //8D 84 02
    //clockticks+=4;
    WriteMemory(0x0284,A);
//case 0x71FD: //A9 0C 8D
    //clockticks+=3;
    A=0x0C;
  ZN=A;
//case 0x71FF: //8D A7 02
    //clockticks+=4;
    WriteMemory(0x02A7,A);


//L_7202:
case 0x7202: //60 20 B5
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7202,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7203:
case 0x7203: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x06;
    showme(0x7203,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_7206:
case 0x7206: //29 8F 10
clockticks+=3; //SUM
    //clockticks+=3;
    A &= 0x8F;
  ZN=A;


//L_7208:
case 0x7208: //10 02 09
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7208,0x10);
        PCSTART=0x720C;
        /**/return;
        //goto L_720C;
    }
//case 0x720A: //09 F0 18
    //clockticks+=3;
    A |= 0xF0;
  ZN=A;


//L_720C:
case 0x720C: //18 79 23
clockticks+=6; //SUM
    //clockticks+=2;
    C = 0;
//case 0x720D: //79 23 02
    //clockticks+=4;
    temp=0x0223; temp+=Y;
    ADC(ReadMemory(temp));


//L_7210:
case 0x7210: //20 33 72
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x13;
    showme(0x7210,0x20);
    PCSTART=0x7233; //jsr
    /**/return;
    //goto L_7233;


//L_7213:
case 0x7213: //9D 23 02
clockticks+=5; //SUM
    //clockticks+=5;
    temp=0x0223; temp+=X;
    WriteMemory(temp,A);


//L_7216:
case 0x7216: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x19;
    showme(0x7216,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_7219:
case 0x7219: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x1C;
    showme(0x7219,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_721C:
case 0x721C: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x1F;
    showme(0x721C,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_721F:
case 0x721F: //20 B5 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x22;
    showme(0x721F,0x20);
    PCSTART=0x77B5; //jsr
    /**/return;
    //goto L_77B5;


//L_7222:
case 0x7222: //29 8F 10
clockticks+=3; //SUM
    //clockticks+=3;
    A &= 0x8F;
  ZN=A;


//L_7224:
case 0x7224: //10 02 09
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7224,0x10);
        PCSTART=0x7228;
        /**/return;
        //goto L_7228;
    }
//case 0x7226: //09 F0 18
    //clockticks+=3;
    A |= 0xF0;
  ZN=A;


//L_7228:
case 0x7228: //18 79 46
clockticks+=6; //SUM
    //clockticks+=2;
    C = 0;
//case 0x7229: //79 46 02
    //clockticks+=4;
    temp=0x0246; temp+=Y;
    ADC(ReadMemory(temp));


//L_722C:
case 0x722C: //20 33 72
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x2F;
    showme(0x722C,0x20);
    PCSTART=0x7233; //jsr
    /**/return;
    //goto L_7233;


//L_722F:
case 0x722F: //9D 46 02
clockticks+=5; //SUM
    //clockticks+=5;
    temp=0x0246; temp+=X;
    WriteMemory(temp,A);


//L_7232:
case 0x7232: //60 10 0D
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7232,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7233:
case 0x7233: //10 0D C9
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7233,0x10);
        PCSTART=0x7242;
        /**/return;
        //goto L_7242;
    }
//case 0x7235: //C9 E1 B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xE1;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7237:
case 0x7237: //B0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7237,0xB0);
        PCSTART=0x723B;
        /**/return;
        //goto L_723B;
    } 
//case 0x7239: //A9 E1 C9
    //clockticks+=3;
    A=0xE1;
  ZN=A;


//L_723B:
case 0x723B: //C9 FB 90
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xFB;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_723D:
case 0x723D: //90 0F A9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x723D,0x90);
        PCSTART=0x724E;
        /**/return;
        //goto L_724E;
    } 
//case 0x723F: //A9 FA 60
    //clockticks+=3;
    A=0xFA;
  ZN=A;


//L_7241:
case 0x7241: //60 C9 06
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7241,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7242:
case 0x7242: //C9 06 B0
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x06;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7244:
case 0x7244: //B0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7244,0xB0);
        PCSTART=0x7248;
        /**/return;
        //goto L_7248;
    } 
//case 0x7246: //A9 06 C9
    //clockticks+=3;
    A=0x06;
  ZN=A;


//L_7248:
case 0x7248: //C9 20 90
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x20;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_724A:
case 0x724A: //90 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x724A,0x90);
        PCSTART=0x724E;
        /**/return;
        //goto L_724E;
    } 
//case 0x724C: //A9 1F 60
    //clockticks+=3;
    A=0x1F;
  ZN=A;


//L_724E:
case 0x724E: //60 A9 10
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x724E,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_724F:
case 0x724F: //A9 10 85
clockticks+=11; //SUM
    //clockticks+=3;
    A=0x10;
//  ZN=A;
//case 0x7251: //85 00 A9
    //clockticks+=2;
    WriteMemory(0x00,A);
//case 0x7253: //A9 50 A2
    //clockticks+=3;
    A=0x50;
//  ZN=A;
//case 0x7255: //A2 A4 20
    //clockticks+=3;
    X=0xA4;
  ZN=X;


//L_7257:
case 0x7257: //20 FC 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x5A;
    showme(0x7257,0x20);
    PCSTART=0x7BFC; //jsr
    /**/return;
    //goto L_7BFC;


//L_725A:
case 0x725A: //A9 19 A2
clockticks+=6; //SUM
    //clockticks+=3;
    A=0x19;
//  ZN=A;
//case 0x725C: //A2 DB 20
    //clockticks+=3;
    X=0xDB;
  ZN=X;


//L_725E:
case 0x725E: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x61;
    showme(0x725E,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_7261:
case 0x7261: //A9 70 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x70;
  ZN=A;


//L_7263:
case 0x7263: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x66;
    showme(0x7263,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_7266:
case 0x7266: //A2 00 A5
clockticks+=9; //SUM
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x7268: //A5 1C C9
    //clockticks+=3;
    A=ReadMemory(0x1C); 
//  ZN=A;
//case 0x726A: //C9 02 D0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x02;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_726C:
case 0x726C: //D0 18 A5
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x726C,0xD0);
        PCSTART=0x7286;
        /**/return;
        //goto L_7286;
    }
//case 0x726E: //A5 18 D0
    //clockticks+=3;
    A=ReadMemory(0x18); 
  ZN=A;


//L_7270:
case 0x7270: //D0 14 A2
clockticks+=12; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7270,0xD0);
        PCSTART=0x7286;
        /**/return;
        //goto L_7286;
    }
//case 0x7272: //A2 20 AD
    //clockticks+=3;
    X=0x20;
//  ZN=X;
//case 0x7274: //AD 1B 02
    //clockticks+=4;
    A=ReadMemory(0x021B); 
//  ZN=A;
//case 0x7277: //05 59 D0
    //clockticks+=3;
    A |= ReadMemory(0x59);
  ZN=A;


//L_7279:
case 0x7279: //D0 0B AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7279,0xD0);
        PCSTART=0x7286;
        /**/return;
        //goto L_7286;
    }
//case 0x727B: //AD FA 02
    //clockticks+=4;
    A=ReadMemory(0x02FA); 
  ZN=A;


//L_727E:
case 0x727E: //30 06 A5
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x727E,0x30);
        PCSTART=0x7286;
        /**/return;
        //goto L_7286;
    }
//case 0x7280: //A5 5C 29
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x7282: //29 10 F0
    //clockticks+=3;
    A &= 0x10;
  ZN=A;


//L_7284:
case 0x7284: //F0 0D A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7284,0xF0);
        PCSTART=0x7293;
        /**/return;
        //goto L_7293;
    } 


//L_7286:
case 0x7286: //A9 52 A0
clockticks+=8; //SUM
    //clockticks+=3;
    A=0x52;
//  ZN=A;
//case 0x7288: //A0 02 38
    //clockticks+=3;
    Y = 0x02; 
  ZN=Y;
//case 0x728A: //38 20 3F
    //clockticks+=2;
    C = 0x01;


//L_728B:
case 0x728B: //20 3F 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x8E;
    showme(0x728B,0x20);
    PCSTART=0x773F; //jsr
    /**/return;
    //goto L_773F;


//L_728E:
case 0x728E: //A9 00 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_7290:
case 0x7290: //20 8B 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x93;
    showme(0x7290,0x20);
    PCSTART=0x778B; //jsr
    /**/return;
    //goto L_778B;


//L_7293:
case 0x7293: //A9 28 A4
clockticks+=6; //SUM
    //clockticks+=3;
    A=0x28;
//  ZN=A;
//case 0x7295: //A4 57 20
    //clockticks+=3;
    Y=ReadMemory(0x57);
  ZN=Y;


//L_7297:
case 0x7297: //20 3E 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0x9A;
    showme(0x7297,0x20);
    PCSTART=0x6F3E; //jsr
    /**/return;
    //goto L_6F3E;


//L_729A:
case 0x729A: //A9 00 85
clockticks+=11; //SUM
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x729C: //85 00 A9
    //clockticks+=2;
    WriteMemory(0x00,A);
//case 0x729E: //A9 78 A2
    //clockticks+=3;
    A=0x78;
//  ZN=A;
//case 0x72A0: //A2 DB 20
    //clockticks+=3;
    X=0xDB;
  ZN=X;


//L_72A2:
case 0x72A2: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0xA5;
    showme(0x72A2,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_72A5:
case 0x72A5: //A9 50 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x50;
  ZN=A;


//L_72A7:
case 0x72A7: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0xAA;
    showme(0x72A7,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_72AA:
case 0x72AA: //A9 1D A0
clockticks+=8; //SUM
    //clockticks+=3;
    A=0x1D;
//  ZN=A;
//case 0x72AC: //A0 02 38
    //clockticks+=3;
    Y = 0x02; 
  ZN=Y;
//case 0x72AE: //38 20 3F
    //clockticks+=2;
    C = 0x01;


//L_72AF:
case 0x72AF: //20 3F 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0xB2;
    showme(0x72AF,0x20);
    PCSTART=0x773F; //jsr
    /**/return;
    //goto L_773F;


//L_72B2:
case 0x72B2: //A9 00 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_72B4:
case 0x72B4: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0xB7;
    showme(0x72B4,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_72B7:
case 0x72B7: //A9 10 85
clockticks+=11; //SUM
    //clockticks+=3;
    A=0x10;
//  ZN=A;
//case 0x72B9: //85 00 A9
    //clockticks+=2;
    WriteMemory(0x00,A);
//case 0x72BB: //A9 C0 A2
    //clockticks+=3;
    A=0xC0;
//  ZN=A;
//case 0x72BD: //A2 DB 20
    //clockticks+=3;
    X=0xDB;
  ZN=X;


//L_72BF:
case 0x72BF: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0xC2;
    showme(0x72BF,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_72C2:
case 0x72C2: //A9 50 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x50;
  ZN=A;


//L_72C4:
case 0x72C4: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0xC7;
    showme(0x72C4,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_72C7:
case 0x72C7: //A2 00 A5
clockticks+=9; //SUM
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x72C9: //A5 1C C9
    //clockticks+=3;
    A=ReadMemory(0x1C); 
//  ZN=A;
//case 0x72CB: //C9 01 F0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x01;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_72CD:
case 0x72CD: //F0 2E 90
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x72CD,0xF0);
        PCSTART=0x72FD;
        /**/return;
        //goto L_72FD;
    } 


//L_72CF:
case 0x72CF: //90 18 A5
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x72CF,0x90);
        PCSTART=0x72E9;
        /**/return;
        //goto L_72E9;
    } 
//case 0x72D1: //A5 18 F0
    //clockticks+=3;
    A=ReadMemory(0x18); 
  ZN=A;


//L_72D3:
case 0x72D3: //F0 14 A2
clockticks+=12; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x72D3,0xF0);
        PCSTART=0x72E9;
        /**/return;
        //goto L_72E9;
    } 
//case 0x72D5: //A2 20 AD
    //clockticks+=3;
    X=0x20;
//  ZN=X;
//case 0x72D7: //AD 1B 02
    //clockticks+=4;
    A=ReadMemory(0x021B); 
//  ZN=A;
//case 0x72DA: //05 59 D0
    //clockticks+=3;
    A |= ReadMemory(0x59);
  ZN=A;


//L_72DC:
case 0x72DC: //D0 0B AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x72DC,0xD0);
        PCSTART=0x72E9;
        /**/return;
        //goto L_72E9;
    }
//case 0x72DE: //AD FA 02
    //clockticks+=4;
    A=ReadMemory(0x02FA); 
  ZN=A;


//L_72E1:
case 0x72E1: //30 06 A5
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x72E1,0x30);
        PCSTART=0x72E9;
        /**/return;
        //goto L_72E9;
    }
//case 0x72E3: //A5 5C 29
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x72E5: //29 10 F0
    //clockticks+=3;
    A &= 0x10;
  ZN=A;


//L_72E7:
case 0x72E7: //F0 0D A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x72E7,0xF0);
        PCSTART=0x72F6;
        /**/return;
        //goto L_72F6;
    } 


//L_72E9:
case 0x72E9: //A9 54 A0
clockticks+=8; //SUM
    //clockticks+=3;
    A=0x54;
//  ZN=A;
//case 0x72EB: //A0 02 38
    //clockticks+=3;
    Y = 0x02; 
  ZN=Y;
//case 0x72ED: //38 20 3F
    //clockticks+=2;
    C = 0x01;


//L_72EE:
case 0x72EE: //20 3F 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0xF1;
    showme(0x72EE,0x20);
    PCSTART=0x773F; //jsr
    /**/return;
    //goto L_773F;


//L_72F1:
case 0x72F1: //A9 00 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_72F3:
case 0x72F3: //20 8B 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x72;
    stack[S--]=0xF6;
    showme(0x72F3,0x20);
    PCSTART=0x778B; //jsr
    /**/return;
    //goto L_778B;


//L_72F6:
case 0x72F6: //A9 CF A4
clockticks+=6; //SUM
    //clockticks+=3;
    A=0xCF;
//  ZN=A;
//case 0x72F8: //A4 58 4C
    //clockticks+=3;
    Y=ReadMemory(0x58);
  ZN=Y;


//L_72FA:
case 0x72FA: //4C 3E 6F
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x72FA,0x4C);
    PCSTART=0x6F3E;
    /**/return;
    //goto L_6F3E;


//L_72FD:
case 0x72FD: //60 84 00
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x72FD,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_72FE:
case 0x72FE: //84 00 86
clockticks+=64; //SUM
    //clockticks+=2;
    WriteMemory(0x00,Y);
//case 0x7300: //86 0D A5
    //clockticks+=2;
    WriteMemory(0x0D,X);
//case 0x7302: //A5 05 4A
    //clockticks+=3;
    A=ReadMemory(0x05); 
//  ZN=A;
//case 0x7304: //4A 66 04
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7305: //66 04 4A
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x04);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x04,value);
//    ZN=value;
//case 0x7307: //4A 66 04
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7308: //66 04 4A
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x04);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x04,value);
//    ZN=value;
//case 0x730A: //4A 66 04
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x730B: //66 04 85
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x04);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x04,value);
//    ZN=value;
//case 0x730D: //85 05 A5
    //clockticks+=2;
    WriteMemory(0x05,A);
//case 0x730F: //A5 07 18
    //clockticks+=3;
    A=ReadMemory(0x07); 
//  ZN=A;
//case 0x7311: //18 69 04
    //clockticks+=2;
    C = 0;
//case 0x7312: //69 04 4A
    //clockticks+=3;
    ADC(0x04);
//case 0x7314: //4A 66 06
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7315: //66 06 4A
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x06);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x06,value);
//    ZN=value;
//case 0x7317: //4A 66 06
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7318: //66 06 4A
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x06);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x06,value);
//    ZN=value;
//case 0x731A: //4A 66 06
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x731B: //66 06 85
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x06);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x06,value);
//    ZN=value;
//case 0x731D: //85 07 A2
    //clockticks+=2;
    WriteMemory(0x07,A);
//case 0x731F: //A2 04 20
    //clockticks+=3;
    X=0x04;
  ZN=X;


//L_7321:
case 0x7321: //20 1C 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0x24;
    showme(0x7321,0x20);
    PCSTART=0x7C1C; //jsr
    /**/return;
    //goto L_7C1C;


//L_7324:
case 0x7324: //A9 70 38
clockticks+=11; //SUM
    //clockticks+=3;
    A=0x70;
//  ZN=A;
//case 0x7326: //38 E5 00
    //clockticks+=2;
    C = 0x01;
//case 0x7327: //E5 00 C9
    //clockticks+=3;
    SBC(ReadMemory(0x00));
//case 0x7329: //C9 A0 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xA0;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_732B:
case 0x732B: //90 0E 48
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x732B,0x90);
        PCSTART=0x733B;
        /**/return;
        //goto L_733B;
    } 


//L_732D:
case 0x732D: //48 A9 90
clockticks+=6; //SUM
    //clockticks+=3;
    stack[S--]=A;
//case 0x732E: //A9 90 20
    //clockticks+=3;
    A=0x90;
  ZN=A;


//L_7330:
case 0x7330: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0x33;
    showme(0x7330,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_7333:
case 0x7333: //68 38 E9
clockticks+=12; //SUM
    //clockticks+=4;
    A=stack[++S];
//  ZN=A;
//case 0x7334: //38 E9 10
    //clockticks+=2;
    C = 0x01;
//case 0x7335: //E9 10 C9
    //clockticks+=3;
    SBC(0x10);
//case 0x7337: //C9 A0 B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xA0;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7339:
case 0x7339: //B0 F2 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7339,0xB0);
        PCSTART=0x732D;
        /**/return;
        //goto L_732D;
    } 


//L_733B:
case 0x733B: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0x3E;
    showme(0x733B,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_733E:
case 0x733E: //A6 0D BD
clockticks+=7; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0D); 
//  ZN=X;
//case 0x7340: //BD 00 02
    //clockticks+=4;
    temp=0x0200; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_7343:
case 0x7343: //10 16 E0
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7343,0x10);
        PCSTART=0x735B;
        /**/return;
        //goto L_735B;
    }
//case 0x7345: //E0 1B F0
    //clockticks+=3;
    value = 0x1B;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7347:
case 0x7347: //F0 0C 29
clockticks+=17; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7347,0xF0);
        PCSTART=0x7355;
        /**/return;
        //goto L_7355;
    } 
//case 0x7349: //29 0C 4A
    //clockticks+=3;
    A &= 0x0C;
//  ZN=A;
//case 0x734B: //4A A8 B9
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x734C: //A8 B9 F8
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x734D: //B9 F8 50
    //clockticks+=4;
    temp=0x50F8; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7350: //BE F9 50
    //clockticks+=4;
    temp=0x50F9; temp+=Y;
    X=ReadMemory(temp);
  ZN=X;


//L_7353:
case 0x7353: //D0 1B 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7353,0xD0);
        PCSTART=0x7370;
        /**/return;
        //goto L_7370;
    }


//L_7355:
case 0x7355: //20 65 74
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0x58;
    showme(0x7355,0x20);
    PCSTART=0x7465; //jsr
    /**/return;
    //goto L_7465;


//L_7358:
case 0x7358: //A6 0D 60
clockticks+=3; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0D); 
  ZN=X;


//L_735A:
case 0x735A: //60 E0 1B
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x735A,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_735B:
case 0x735B: //E0 1B F0
clockticks+=3; //SUM
    //clockticks+=3;
    value = 0x1B;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_735D:
case 0x735D: //F0 17 E0
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x735D,0xF0);
        PCSTART=0x7376;
        /**/return;
        //goto L_7376;
    } 
//case 0x735F: //E0 1C F0
    //clockticks+=3;
    value = 0x1C;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7361:
case 0x7361: //F0 19 B0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7361,0xF0);
        PCSTART=0x737C;
        /**/return;
        //goto L_737C;
    } 


//L_7363:
case 0x7363: //B0 1F 29
clockticks+=19; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7363,0xB0);
        PCSTART=0x7384;
        /**/return;
        //goto L_7384;
    } 
//case 0x7365: //29 18 4A
    //clockticks+=3;
    A &= 0x18;
//  ZN=A;
//case 0x7367: //4A 4A A8
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7368: //4A A8 B9
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7369: //A8 B9 DE
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x736A: //B9 DE 51
    //clockticks+=4;
    temp=0x51DE; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x736D: //BE DF 51
    //clockticks+=4;
    temp=0x51DF; temp+=Y;
    X=ReadMemory(temp);
  ZN=X;


//L_7370:
case 0x7370: //20 45 7D
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0x73;
    showme(0x7370,0x20);
    PCSTART=0x7D45; //jsr
    /**/return;
    //goto L_7D45;


//L_7373:
case 0x7373: //A6 0D 60
clockticks+=3; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0D); 
  ZN=X;


//L_7375:
case 0x7375: //60 20 0B
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7375,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7376:
case 0x7376: //20 0B 75
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0x79;
    showme(0x7376,0x20);
    PCSTART=0x750B; //jsr
    /**/return;
    //goto L_750B;


//L_7379:
case 0x7379: //A6 0D 60
clockticks+=3; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0D); 
  ZN=X;


//L_737B:
case 0x737B: //60 AD 50
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x737B,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_737C:
case 0x737C: //AD 50 52
clockticks+=8; //SUM
    //clockticks+=4;
    A=0x29; 
//  ZN=A;
//case 0x737F: //AE 51 52
    //clockticks+=4;
    X=0xC9;
  ZN=X;


//L_7382:
case 0x7382: //D0 EC A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7382,0xD0);
        PCSTART=0x7370;
        /**/return;
        //goto L_7370;
    }


//L_7384:
case 0x7384: //A9 70 A2
clockticks+=6; //SUM
    //clockticks+=3;
    A=0x70;
//  ZN=A;
//case 0x7386: //A2 F0 20
    //clockticks+=3;
    X=0xF0;
  ZN=X;


//L_7388:
case 0x7388: //20 E0 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0x8B;
    showme(0x7388,0x20);
    PCSTART=0x7CE0; //jsr
    /**/return;
    //goto L_7CE0;


//L_738B:
case 0x738B: //A6 0D A5
clockticks+=9; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0D); 
//  ZN=X;
//case 0x738D: //A5 5C 29
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x738F: //29 03 D0
    //clockticks+=3;
    A &= 0x03;
  ZN=A;


//L_7391:
case 0x7391: //D0 03 DE
clockticks+=9; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7391,0xD0);
        PCSTART=0x7396;
        /**/return;
        //goto L_7396;
    }
//case 0x7393: //DE 00 02
    //clockticks+=7;
    temp=0x0200; temp+=X;
    value=ReadMemory(temp);
    value--;
    WriteMemory(temp,value);
  ZN=value;


//L_7396:
case 0x7396: //60 F8 75
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7396,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7397:
case 0x7397: //F8 75 52
clockticks+=10; //SUM
    //clockticks+=2;
    DEC = 0x08;
//case 0x7398: //75 52 95
    //clockticks+=4;
    temp=0x52; temp+=X; temp&=0xFF;
    ADC(ReadMemory(temp));
//case 0x739A: //95 52 90
    //clockticks+=4;
    temp=0x52; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_739C:
case 0x739C: //90 12 B5
clockticks+=16; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x739C,0x90);
        PCSTART=0x73B0;
        /**/return;
        //goto L_73B0;
    } 
//case 0x739E: //B5 53 69
    //clockticks+=4;
    temp=0x0053; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x73A0: //69 00 95
    //clockticks+=3;
    ADC(0x00);
//case 0x73A2: //95 53 29
    //clockticks+=4;
    temp=0x53; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x73A4: //29 0F D0
    //clockticks+=3;
    A &= 0x0F;
  ZN=A;


//L_73A6:
case 0x73A6: //D0 08 A9
clockticks+=16; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x73A6,0xD0);
        PCSTART=0x73B0;
        /**/return;
        //goto L_73B0;
    }
//case 0x73A8: //A9 B0 85
    //clockticks+=3;
    A=0xB0;
//  ZN=A;
//case 0x73AA: //85 68 A6
    //clockticks+=2;
    WriteMemory(0x68,A);
//case 0x73AC: //A6 18 F6
    //clockticks+=3;
    X=ReadMemory(0x18); 
//  ZN=X;
//case 0x73AE: //F6 57 D8
    //clockticks+=6;
    temp=0x0057; temp+=X; temp&=0xFF;
    value=ReadMemory(temp);
    value++;
    WriteMemory(temp,value);
  ZN=value;


//L_73B0:
case 0x73B0: //D8 60 A5
clockticks+=2; //SUM
    //clockticks+=2;
    DEC = 0;


//L_73B1:
case 0x73B1: //60 A5 18
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x73B1,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_73B2:
case 0x73B2: //A5 18 0A
clockticks+=24; //SUM
    //clockticks+=3;
    A=ReadMemory(0x18); 
//  ZN=A;
//case 0x73B4: //0A 0A 85
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x73B5: //0A 85 08
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x73B6: //85 08 A5
    //clockticks+=2;
    WriteMemory(0x08,A);
//case 0x73B8: //A5 6F 29
    //clockticks+=3;
    A=ReadMemory(0x6F); 
//  ZN=A;
//case 0x73BA: //29 FB 05
    //clockticks+=3;
    A &= 0xFB;
//  ZN=A;
//case 0x73BC: //05 08 85
    //clockticks+=3;
    A |= ReadMemory(0x08);
  ZN=A;
//case 0x73BE: //85 6F 8D
    //clockticks+=2;
    WriteMemory(0x6F,A);
//case 0x73C0: //8D 00 32
    //clockticks+=4;
    WriteMemory3200(A);


//L_73C3:
case 0x73C3: //60 A5 1C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x73C3,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_73C4:
case 0x73C4: //A5 1C F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_73C6:
case 0x73C6: //F0 02 18
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x73C6,0xF0);
        PCSTART=0x73CA;
        /**/return;
        //goto L_73CA;
    } 


//L_73C8:
case 0x73C8: //18 60 A5
clockticks+=2; //SUM
    //clockticks+=2;
    C = 0;


//L_73C9:
case 0x73C9: //60 A5 5D
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x73C9,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_73CA:
case 0x73CA: //A5 5D 29
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5D); 
//  ZN=A;
//case 0x73CC: //29 04 D0
    //clockticks+=3;
    A &= 0x04;
  ZN=A;


//L_73CE:
case 0x73CE: //D0 F8 A5
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x73CE,0xD0);
        PCSTART=0x73C8;
        /**/return;
        //goto L_73C8;
    }
//case 0x73D0: //A5 1D 05
    //clockticks+=3;
    A=ReadMemory(0x1D); 
//  ZN=A;
//case 0x73D2: //05 1E F0
    //clockticks+=3;
    A |= ReadMemory(0x1E);
  ZN=A;


//L_73D4:
case 0x73D4: //F0 F2 A0
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x73D4,0xF0);
        PCSTART=0x73C8;
        /**/return;
        //goto L_73C8;
    } 
//case 0x73D6: //A0 00 20
    //clockticks+=3;
    Y = 0x00; 
  ZN=Y;


//L_73D8:
case 0x73D8: //20 F6 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0xDB;
    showme(0x73D8,0x20);
    PCSTART=0x77F6; //jsr
    /**/return;
    //goto L_77F6;


//L_73DB:
case 0x73DB: //A2 00 86
clockticks+=20; //SUM
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x73DD: //86 10 A9
    //clockticks+=2;
    WriteMemory(0x10,X);
//case 0x73DF: //A9 01 85
    //clockticks+=3;
    A=0x01;
//  ZN=A;
//case 0x73E1: //85 00 A9
    //clockticks+=2;
    WriteMemory(0x00,A);
//case 0x73E3: //A9 A7 85
    //clockticks+=3;
    A=0xA7;
//  ZN=A;
//case 0x73E5: //85 0E A9
    //clockticks+=2;
    WriteMemory(0x0E,A);
//case 0x73E7: //A9 10 85
    //clockticks+=3;
    A=0x10;
  ZN=A;
//case 0x73E9: //85 00 B5
    //clockticks+=2;
    WriteMemory(0x00,A);


//L_73EB:
case 0x73EB: //B5 1D 15
clockticks+=8; //SUM
    //clockticks+=4;
    temp=0x001D; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x73ED: //15 1E F0
    //clockticks+=4;
   temp=0x1E; temp+=X; temp&=0xFF;
   A |= ReadMemory(temp);
  ZN=A;
   //showme(0x73ED,0x15);


//L_73EF:
case 0x73EF: //F0 67 86
clockticks+=10; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x73EF,0xF0);
        PCSTART=0x7458;
        /**/return;
        //goto L_7458;
    } 
//case 0x73F1: //86 0F A9
    //clockticks+=2;
    WriteMemory(0x0F,X);
//case 0x73F3: //A9 5F A6
    //clockticks+=3;
    A=0x5F;
//  ZN=A;
//case 0x73F5: //A6 0E 20
    //clockticks+=3;
    X=ReadMemory(0x0E); 
  ZN=X;


//L_73F7:
case 0x73F7: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0xFA;
    showme(0x73F7,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_73FA:
case 0x73FA: //A9 40 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x40;
  ZN=A;


//L_73FC:
case 0x73FC: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x73;
    stack[S--]=0xFF;
    showme(0x73FC,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_73FF:
case 0x73FF: //A5 0F 4A
clockticks+=25; //SUM
    //clockticks+=3;
    A=ReadMemory(0x0F); 
//  ZN=A;
//case 0x7401: //4A F8 69
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7402: //F8 69 01
    //clockticks+=2;
    DEC = 0x08;
//case 0x7403: //69 01 D8
    //clockticks+=3;
    ADC(0x01);
//case 0x7405: //D8 85 0D
    //clockticks+=2;
    DEC = 0;
//case 0x7406: //85 0D A9
    //clockticks+=2;
    WriteMemory(0x0D,A);
//case 0x7408: //A9 0D 38
    //clockticks+=3;
    A=0x0D;
//  ZN=A;
//case 0x740A: //38 A0 01
    //clockticks+=2;
    C = 0x01;
//case 0x740B: //A0 01 A2
    //clockticks+=3;
    Y = 0x01; 
//  ZN=Y;
//case 0x740D: //A2 00 20
    //clockticks+=3;
    X=0x00;
  ZN=X;


//L_740F:
case 0x740F: //20 3F 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0x12;
    showme(0x740F,0x20);
    PCSTART=0x773F; //jsr
    /**/return;
    //goto L_773F;


//L_7412:
case 0x7412: //A9 40 AA
clockticks+=5; //SUM
    //clockticks+=3;
    A=0x40;
//  ZN=A;
//case 0x7414: //AA 20 E0
    //clockticks+=2;
    X=A;
  ZN=X;


//L_7415:
case 0x7415: //20 E0 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0x18;
    showme(0x7415,0x20);
    PCSTART=0x7CE0; //jsr
    /**/return;
    //goto L_7CE0;


//L_7418:
case 0x7418: //A0 00 20
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x00; 
  ZN=Y;


//L_741A:
case 0x741A: //20 35 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0x1D;
    showme(0x741A,0x20);
    PCSTART=0x6F35; //jsr
    /**/return;
    //goto L_6F35;


//L_741D:
case 0x741D: //A5 0F 18
clockticks+=16; //SUM
    //clockticks+=3;
    A=ReadMemory(0x0F); 
//  ZN=A;
//case 0x741F: //18 69 1D
    //clockticks+=2;
    C = 0;
//case 0x7420: //69 1D A0
    //clockticks+=3;
    ADC(0x1D);
//case 0x7422: //A0 02 38
    //clockticks+=3;
    Y = 0x02; 
//  ZN=Y;
//case 0x7424: //38 A2 00
    //clockticks+=2;
    C = 0x01;
//case 0x7425: //A2 00 20
    //clockticks+=3;
    X=0x00;
  ZN=X;


//L_7427:
case 0x7427: //20 3F 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0x2A;
    showme(0x7427,0x20);
    PCSTART=0x773F; //jsr
    /**/return;
    //goto L_773F;


//L_742A:
case 0x742A: //A9 00 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_742C:
case 0x742C: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0x2F;
    showme(0x742C,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_742F:
case 0x742F: //A0 00 20
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x00; 
  ZN=Y;


//L_7431:
case 0x7431: //20 35 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0x34;
    showme(0x7431,0x20);
    PCSTART=0x6F35; //jsr
    /**/return;
    //goto L_6F35;


//L_7434:
case 0x7434: //A4 10 20
clockticks+=3; //SUM
    //clockticks+=3;
    Y=ReadMemory(0x10);
  ZN=Y;


//L_7436:
case 0x7436: //20 1A 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0x39;
    showme(0x7436,0x20);
    PCSTART=0x6F1A; //jsr
    /**/return;
    //goto L_6F1A;


//L_7439:
case 0x7439: //E6 10 A4
clockticks+=8; //SUM
    //clockticks+=5;
    value=ReadMemory(0x10);
    value++;
    WriteMemory(0x10,value);
//  ZN=value;
//case 0x743B: //A4 10 20
    //clockticks+=3;
    Y=ReadMemory(0x10);
  ZN=Y;


//L_743D:
case 0x743D: //20 1A 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0x40;
    showme(0x743D,0x20);
    PCSTART=0x6F1A; //jsr
    /**/return;
    //goto L_6F1A;


//L_7440:
case 0x7440: //E6 10 A4
clockticks+=8; //SUM
    //clockticks+=5;
    value=ReadMemory(0x10);
    value++;
    WriteMemory(0x10,value);
//  ZN=value;
//case 0x7442: //A4 10 20
    //clockticks+=3;
    Y=ReadMemory(0x10);
  ZN=Y;


//L_7444:
case 0x7444: //20 1A 6F
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0x47;
    showme(0x7444,0x20);
    PCSTART=0x6F1A; //jsr
    /**/return;
    //goto L_6F1A;


//L_7447:
case 0x7447: //E6 10 A5
clockticks+=25; //SUM
    //clockticks+=5;
    value=ReadMemory(0x10);
    value++;
    WriteMemory(0x10,value);
//  ZN=value;
//case 0x7449: //A5 0E 38
    //clockticks+=3;
    A=ReadMemory(0x0E); 
//  ZN=A;
//case 0x744B: //38 E9 08
    //clockticks+=2;
    C = 0x01;
//case 0x744C: //E9 08 85
    //clockticks+=3;
    SBC(0x08);
//case 0x744E: //85 0E A6
    //clockticks+=2;
    WriteMemory(0x0E,A);
//case 0x7450: //A6 0F E8
    //clockticks+=3;
    X=ReadMemory(0x0F); 
//  ZN=X;
//case 0x7452: //E8 E8 E0
    //clockticks+=2;
    X++; 
//  ZN=X;
//case 0x7453: //E8 E0 14
    //clockticks+=2;
    X++; 
//  ZN=X;
//case 0x7454: //E0 14 90
    //clockticks+=3;
    value = 0x14;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7456:
case 0x7456: //90 93 38
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7456,0x90);
        PCSTART=0x73EB;
        /**/return;
        //goto L_73EB;
    } 


//L_7458:
case 0x7458: //38 60 A2
clockticks+=2; //SUM
    //clockticks+=2;
    C = 0x01;


//L_7459:
case 0x7459: //60 A2 1A
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7459,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_745A:
case 0x745A: //A2 1A BD
clockticks+=3; //SUM
    //clockticks+=3;
    X=0x1A;
  ZN=X;


//L_745C:
case 0x745C: //BD 00 02
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0200; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_745F:
case 0x745F: //F0 03 CA
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x745F,0xF0);
        PCSTART=0x7464;
        /**/return;
        //goto L_7464;
    } 
//case 0x7461: //CA 10 F8
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7462:
case 0x7462: //10 F8 60
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7462,0x10);
        PCSTART=0x745C;
        /**/return;
        //goto L_745C;
    }


//L_7464:
case 0x7464: //60 AD 1B
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7464,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7465:
case 0x7465: //AD 1B 02
clockticks+=7; //SUM
    //clockticks+=4;
    A=ReadMemory(0x021B); 
//  ZN=A;
//case 0x7468: //C9 A2 B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0xA2;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_746A:
case 0x746A: //B0 22 A2
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x746A,0xB0);
        PCSTART=0x748E;
        /**/return;
        //goto L_748E;
    } 
//case 0x746C: //A2 0A BD
    //clockticks+=3;
    X=0x0A;
  ZN=X;


//L_746E:
case 0x746E: //BD EC 50
clockticks+=52; //SUM
    //clockticks+=4;
    temp=0x50EC; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7471: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7472: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7473: //4A 4A 18
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7474: //4A 18 69
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7475: //18 69 F8
    //clockticks+=2;
    C = 0;
//case 0x7476: //69 F8 49
    //clockticks+=3;
    ADC(0xF8);
//case 0x7478: //49 F8 95
    //clockticks+=3;
    A ^= 0xF8;
//  ZN=A;
//case 0x747A: //95 7E BD
    //clockticks+=4;
    temp=0x7E; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x747C: //BD ED 50
    //clockticks+=4;
    temp=0x50ED; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x747F: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7480: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7481: //4A 4A 18
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7482: //4A 18 69
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7483: //18 69 F8
    //clockticks+=2;
    C = 0;
//case 0x7484: //69 F8 49
    //clockticks+=3;
    ADC(0xF8);
//case 0x7486: //49 F8 95
    //clockticks+=3;
    A ^= 0xF8;
//  ZN=A;
//case 0x7488: //95 8A CA
    //clockticks+=4;
    temp=0x8A; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x748A: //CA CA 10
    //clockticks+=2;
    X--; 
//  ZN=X;
//case 0x748B: //CA 10 E0
    //clockticks+=2;
    X--; 
  ZN=X;


//L_748C:
case 0x748C: //10 E0 AD
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x748C,0x10);
        PCSTART=0x746E;
        /**/return;
        //goto L_746E;
    }


//L_748E:
case 0x748E: //AD 1B 02
clockticks+=18; //SUM
    //clockticks+=4;
    A=ReadMemory(0x021B); 
//  ZN=A;
//case 0x7491: //49 FF 29
    //clockticks+=3;
    A ^= 0xFF;
//  ZN=A;
//case 0x7493: //29 70 4A
    //clockticks+=3;
    A &= 0x70;
//  ZN=A;
//case 0x7495: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7496: //4A 4A AA
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7497: //4A AA 86
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7498: //AA 86 09
    //clockticks+=2;
    X=A;
  ZN=X;


//L_7499:
case 0x7499: //86 09 A0
clockticks+=9; //SUM
    //clockticks+=2;
    WriteMemory(0x09,X);
//case 0x749B: //A0 00 BD
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x749D: //BD EC 50
    //clockticks+=4;
    temp=0x50EC; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_74A0:
case 0x74A0: //10 01 88
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x74A0,0x10);
        PCSTART=0x74A3;
        /**/return;
        //goto L_74A3;
    }
//case 0x74A2: //88 18 75
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_74A3:
case 0x74A3: //18 75 7D
clockticks+=31; //SUM
    //clockticks+=2;
    C = 0;
//case 0x74A4: //75 7D 95
    //clockticks+=4;
    temp=0x7D; temp+=X; temp&=0xFF;
    ADC(ReadMemory(temp));
//case 0x74A6: //95 7D 98
    //clockticks+=4;
    temp=0x7D; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x74A8: //98 75 7E
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x74A9: //75 7E 95
    //clockticks+=4;
    temp=0x7E; temp+=X; temp&=0xFF;
    ADC(ReadMemory(temp));
//case 0x74AB: //95 7E 85
    //clockticks+=4;
    temp=0x7E; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x74AD: //85 04 84
    //clockticks+=2;
    WriteMemory(0x04,A);
//case 0x74AF: //84 05 A0
    //clockticks+=2;
    WriteMemory(0x05,Y);
//case 0x74B1: //A0 00 BD
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x74B3: //BD ED 50
    //clockticks+=4;
    temp=0x50ED; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_74B6:
case 0x74B6: //10 01 88
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x74B6,0x10);
        PCSTART=0x74B9;
        /**/return;
        //goto L_74B9;
    }
//case 0x74B8: //88 18 75
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_74B9:
case 0x74B9: //18 75 89
clockticks+=34; //SUM
    //clockticks+=2;
    C = 0;
//case 0x74BA: //75 89 95
    //clockticks+=4;
    temp=0x89; temp+=X; temp&=0xFF;
    ADC(ReadMemory(temp));
//case 0x74BC: //95 89 98
    //clockticks+=4;
    temp=0x89; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x74BE: //98 75 8A
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x74BF: //75 8A 95
    //clockticks+=4;
    temp=0x8A; temp+=X; temp&=0xFF;
    ADC(ReadMemory(temp));
//case 0x74C1: //95 8A 85
    //clockticks+=4;
    temp=0x8A; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x74C3: //85 06 84
    //clockticks+=2;
    WriteMemory(0x06,A);
//case 0x74C5: //84 07 A5
    //clockticks+=2;
    WriteMemory(0x07,Y);
//case 0x74C7: //A5 02 85
    //clockticks+=3;
    A=ReadMemory(0x02); 
//  ZN=A;
//case 0x74C9: //85 0B A5
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x74CB: //A5 03 85
    //clockticks+=3;
    A=ReadMemory(0x03); 
  ZN=A;
//case 0x74CD: //85 0C 20
    //clockticks+=2;
    WriteMemory(0x0C,A);


//L_74CF:
case 0x74CF: //20 49 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0xD2;
    showme(0x74CF,0x20);
    PCSTART=0x7C49; //jsr
    /**/return;
    //goto L_7C49;


//L_74D2:
case 0x74D2: //A4 09 B9
clockticks+=11; //SUM
    //clockticks+=3;
    Y=ReadMemory(0x09);
//  ZN=Y;
//case 0x74D4: //B9 E0 50
    //clockticks+=4;
    temp=0x50E0; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x74D7: //BE E1 50
    //clockticks+=4;
    temp=0x50E1; temp+=Y;
    X=ReadMemory(temp);
  ZN=X;


//L_74DA:
case 0x74DA: //20 45 7D
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0xDD;
    showme(0x74DA,0x20);
    PCSTART=0x7D45; //jsr
    /**/return;
    //goto L_7D45;


//L_74DD:
case 0x74DD: //A4 09 B9
clockticks+=22; //SUM
    //clockticks+=3;
    Y=ReadMemory(0x09);
//  ZN=Y;
//case 0x74DF: //B9 E1 50
    //clockticks+=4;
    temp=0x50E1; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x74E2: //49 04 AA
    //clockticks+=3;
    A ^= 0x04;
//  ZN=A;
//case 0x74E4: //AA B9 E0
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x74E5: //B9 E0 50
    //clockticks+=4;
    temp=0x50E0; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x74E8: //29 0F 49
    //clockticks+=3;
    A &= 0x0F;
//  ZN=A;
//case 0x74EA: //49 04 20
    //clockticks+=3;
    A ^= 0x04;
  ZN=A;


//L_74EC:
case 0x74EC: //20 45 7D
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x74;
    stack[S--]=0xEF;
    showme(0x74EC,0x20);
    PCSTART=0x7D45; //jsr
    /**/return;
    //goto L_7D45;


//L_74EF:
case 0x74EF: //A0 FF C8
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0xFF; 
  ZN=Y;


//L_74F1:
case 0x74F1: //C8 B1 0B
clockticks+=32; //SUM
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x74F2: //B1 0B 91
    //clockticks+=5;
    temp=ReadMemory(0x0C);
    temp<<=8;
    temp|=ReadMemory(0x0B);
    temp=temp+Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x74F4: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x74F6: //C8 B1 0B
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x74F7: //B1 0B 49
    //clockticks+=5;
    temp=ReadMemory(0x0C);
    temp<<=8;
    temp|=ReadMemory(0x0B);
    temp=temp+Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x74F9: //49 04 91
    //clockticks+=3;
    A ^= 0x04;
//  ZN=A;
//case 0x74FB: //91 02 C0
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x74FD: //C0 03 90
    //clockticks+=3;
    temp=Y; temp+=0x100; temp-=0x03;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_74FF:
case 0x74FF: //90 F0 20
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x74FF,0x90);
        PCSTART=0x74F1;
        /**/return;
        //goto L_74F1;
    } 


//L_7501:
case 0x7501: //20 39 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x75;
    stack[S--]=0x04;
    showme(0x7501,0x20);
    PCSTART=0x7C39; //jsr
    /**/return;
    //goto L_7C39;


//L_7504:
case 0x7504: //A6 09 CA
clockticks+=7; //SUM
    //clockticks+=3;
    X=ReadMemory(0x09); 
//  ZN=X;
//case 0x7506: //CA CA 10
    //clockticks+=2;
    X--; 
//  ZN=X;
//case 0x7507: //CA 10 8F
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7508:
case 0x7508: //10 8F 60
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7508,0x10);
        PCSTART=0x7499;
        /**/return;
        //goto L_7499;
    }


//L_750A:
case 0x750A: //60 A2 00
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x750A,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_750B:
case 0x750B: //A2 00 86
clockticks+=11; //SUM
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x750D: //86 17 A0
    //clockticks+=2;
    WriteMemory(0x17,X);
//case 0x750F: //A0 00 A5
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7511: //A5 61 10
    //clockticks+=3;
    A=ReadMemory(0x61); 
  ZN=A;


//L_7513:
case 0x7513: //10 06 A0
clockticks+=12; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7513,0x10);
        PCSTART=0x751B;
        /**/return;
        //goto L_751B;
    }
//case 0x7515: //A0 04 8A
    //clockticks+=3;
    Y = 0x04; 
//  ZN=Y;
//case 0x7517: //8A 38 E5
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x7518: //38 E5 61
    //clockticks+=2;
    C = 0x01;
//case 0x7519: //E5 61 85
    //clockticks+=3;
    SBC(ReadMemory(0x61));


//L_751B:
case 0x751B: //85 08 24
clockticks+=5; //SUM
    //clockticks+=2;
    WriteMemory(0x08,A);
//case 0x751D: //24 08 30
    //clockticks+=3;
    value=ReadMemory(0x08);
    V = value;
  ZN = value & 0x80;
  if((value & A)==0) ZN |= 0x02;


//L_751F:
case 0x751F: //30 02 50
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x751F,0x30);
        PCSTART=0x7523;
        /**/return;
        //goto L_7523;
    }


//L_7521:
case 0x7521: //50 07 A2
clockticks+=2; //SUM
    //clockticks+=2;
    if((V & 0x40)==0)
    {
        clockticks++; 
        showme(0x7521,0x50);
        PCSTART=0x752A;
        /**/return;
        //goto L_752A;
    }


//L_7523:
case 0x7523: //A2 04 A9
clockticks+=11; //SUM
    //clockticks+=3;
    X=0x04;
//  ZN=X;
//case 0x7525: //A9 80 38
    //clockticks+=3;
    A=0x80;
//  ZN=A;
//case 0x7527: //38 E5 08
    //clockticks+=2;
    C = 0x01;
//case 0x7528: //E5 08 86
    //clockticks+=3;
    SBC(ReadMemory(0x08));


//L_752A:
case 0x752A: //86 08 84
clockticks+=19; //SUM
    //clockticks+=2;
    WriteMemory(0x08,X);
//case 0x752C: //84 09 4A
    //clockticks+=2;
    WriteMemory(0x09,Y);
//case 0x752E: //4A 29 FE
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x752F: //29 FE A8
    //clockticks+=3;
    A &= 0xFE;
//  ZN=A;
//case 0x7531: //A8 B9 6E
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x7532: //B9 6E 52
    //clockticks+=4;
    temp=0x526E; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7535: //BE 6F 52
    //clockticks+=4;
    temp=0x526F; temp+=Y;
    X=ReadMemory(temp);
  ZN=X;


//L_7538:
case 0x7538: //20 D3 6A
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x75;
    stack[S--]=0x3B;
    showme(0x7538,0x20);
    PCSTART=0x6AD3; //jsr
    /**/return;
    //goto L_6AD3;


//L_753B:
case 0x753B: //AD 05 24
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2405); 
  ZN=A;


//L_753E:
case 0x753E: //10 14 A5
clockticks+=8; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x753E,0x10);
        PCSTART=0x7554;
        /**/return;
        //goto L_7554;
    }
//case 0x7540: //A5 5C 29
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x7542: //29 04 F0
    //clockticks+=3;
    A &= 0x04;
  ZN=A;


//L_7544:
case 0x7544: //F0 0E C8
clockticks+=16; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7544,0xF0);
        PCSTART=0x7554;
        /**/return;
        //goto L_7554;
    } 
//case 0x7546: //C8 C8 38
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7547: //C8 38 A6
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7548: //38 A6 0C
    //clockticks+=2;
    C = 0x01;
//case 0x7549: //A6 0C 98
    //clockticks+=3;
    X=ReadMemory(0x0C); 
//  ZN=X;
//case 0x754B: //98 65 0B
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x754C: //65 0B 90
    //clockticks+=3;
    ADC(ReadMemory(0x0B));


//L_754E:
case 0x754E: //90 01 E8
clockticks+=4; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x754E,0x90);
        PCSTART=0x7551;
        /**/return;
        //goto L_7551;
    } 
//case 0x7550: //E8 20 D3
    //clockticks+=2;
    X++; 
  ZN=X;


//L_7551:
case 0x7551: //20 D3 6A
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x75;
    stack[S--]=0x54;
    showme(0x7551,0x20);
    PCSTART=0x6AD3; //jsr
    /**/return;
    //goto L_6AD3;


//L_7554:
case 0x7554: //60 A5 1C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7554,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7555:
case 0x7555: //A5 1C D0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_7557:
case 0x7557: //D0 01 60
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7557,0xD0);
        PCSTART=0x755A;
        /**/return;
        //goto L_755A;
    }


//L_7559:
case 0x7559: //60 A2 00
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7559,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_755A:
case 0x755A: //A2 00 AD
clockticks+=7; //SUM
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x755C: //AD 1C 02
    //clockticks+=4;
    A=ReadMemory(0x021C); 
  ZN=A;


//L_755F:
case 0x755F: //30 0A F0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x755F,0x30);
        PCSTART=0x756B;
        /**/return;
        //goto L_756B;
    }


//L_7561:
case 0x7561: //F0 08 6A
clockticks+=15; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7561,0xF0);
        PCSTART=0x756B;
        /**/return;
        //goto L_756B;
    } 
//case 0x7563: //6A 6A 6A
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x7564: //6A 6A 8D
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x7565: //6A 8D 02
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x7566: //8D 02 3C
    //clockticks+=4;
    WriteMemory(0x3C02,A);
//case 0x7569: //A2 80 8E
    //clockticks+=3;
    X=0x80;
  ZN=X;


//L_756B:
case 0x756B: //8E 00 3C
clockticks+=7; //SUM
    //clockticks+=4;
    WriteMemory(0x3C00,X);
//case 0x756E: //A2 01 20
    //clockticks+=3;
    X=0x01;
  ZN=X;


//L_7570:
case 0x7570: //20 CD 75
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x75;
    stack[S--]=0x73;
    showme(0x7570,0x20);
    PCSTART=0x75CD; //jsr
    /**/return;
    //goto L_75CD;


//L_7573:
case 0x7573: //8D 01 3C
clockticks+=6; //SUM
    //clockticks+=4;
    WriteMemory(0x3C01,A);
//case 0x7576: //CA 20 CD
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7577:
case 0x7577: //20 CD 75
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x75;
    stack[S--]=0x7A;
    showme(0x7577,0x20);
    PCSTART=0x75CD; //jsr
    /**/return;
    //goto L_75CD;


//L_757A:
case 0x757A: //8D 04 3C
clockticks+=11; //SUM
    //clockticks+=4;
    WriteMemory(0x3C04,A);
//case 0x757D: //AD 1B 02
    //clockticks+=4;
    A=ReadMemory(0x021B); 
//  ZN=A;
//case 0x7580: //C9 01 F0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x01;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7582:
case 0x7582: //F0 04 8A
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7582,0xF0);
        PCSTART=0x7588;
        /**/return;
        //goto L_7588;
    } 
//case 0x7584: //8A 8D 03
    //clockticks+=2;
    A = X; 
  ZN=A;
//case 0x7585: //8D 03 3C
    //clockticks+=4;
    WriteMemory(0x3C03,A);


//L_7588:
case 0x7588: //AD F6 02
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x02F6); 
  ZN=A;


//L_758B:
case 0x758B: //F0 11 AD
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x758B,0xF0);
        PCSTART=0x759E;
        /**/return;
        //goto L_759E;
    } 
//case 0x758D: //AD 1B 02
    //clockticks+=4;
    A=ReadMemory(0x021B); 
  ZN=A;


//L_7590:
case 0x7590: //30 0C 05
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7590,0x30);
        PCSTART=0x759E;
        /**/return;
        //goto L_759E;
    }
//case 0x7592: //05 59 F0
    //clockticks+=3;
    A |= ReadMemory(0x59);
  ZN=A;


//L_7594:
case 0x7594: //F0 08 A5
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7594,0xF0);
        PCSTART=0x759E;
        /**/return;
        //goto L_759E;
    } 
//case 0x7596: //A5 6D F0
    //clockticks+=3;
    A=ReadMemory(0x6D); 
  ZN=A;


//L_7598:
case 0x7598: //F0 14 C6
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7598,0xF0);
        PCSTART=0x75AE;
        /**/return;
        //goto L_75AE;
    } 
//case 0x759A: //C6 6D D0
    //clockticks+=5;
    value=ReadMemory(0x6D);
    value--;
    WriteMemory(0x6D,value);
  ZN=value;


//L_759C:
case 0x759C: //D0 21 A5
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x759C,0xD0);
        PCSTART=0x75BF;
        /**/return;
        //goto L_75BF;
    }


//L_759E:
case 0x759E: //A5 6C 29
clockticks+=18; //SUM
    //clockticks+=3;
    A=ReadMemory(0x6C); 
//  ZN=A;
//case 0x75A0: //29 0F 85
    //clockticks+=3;
    A &= 0x0F;
//  ZN=A;
//case 0x75A2: //85 6C 8D
    //clockticks+=2;
    WriteMemory(0x6C,A);
//case 0x75A4: //8D 00 3A
    //clockticks+=4;
    WriteMemory(0x3A00,A);
//case 0x75A7: //AD FC 02
    //clockticks+=4;
    A=ReadMemory(0x02FC); 
  ZN=A;
//case 0x75AA: //85 6E 10
    //clockticks+=2;
    WriteMemory(0x6E,A);


//L_75AC:
case 0x75AC: //10 11 C6
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x75AC,0x10);
        PCSTART=0x75BF;
        /**/return;
        //goto L_75BF;
    }


//L_75AE:
case 0x75AE: //C6 6E D0
clockticks+=5; //SUM
    //clockticks+=5;
    value=ReadMemory(0x6E);
    value--;
    WriteMemory(0x6E,value);
  ZN=value;


//L_75B0:
case 0x75B0: //D0 0D A9
clockticks+=19; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x75B0,0xD0);
        PCSTART=0x75BF;
        /**/return;
        //goto L_75BF;
    }
//case 0x75B2: //A9 04 85
    //clockticks+=3;
    A=0x04;
//  ZN=A;
//case 0x75B4: //85 6D A5
    //clockticks+=2;
    WriteMemory(0x6D,A);
//case 0x75B6: //A5 6C 49
    //clockticks+=3;
    A=ReadMemory(0x6C); 
//  ZN=A;
//case 0x75B8: //49 14 85
    //clockticks+=3;
    A ^= 0x14;
  ZN=A;
//case 0x75BA: //85 6C 8D
    //clockticks+=2;
    WriteMemory(0x6C,A);
//case 0x75BC: //8D 00 3A
    //clockticks+=4;
    WriteMemory(0x3A00,A);


//L_75BF:
case 0x75BF: //A5 69 AA
clockticks+=8; //SUM
    //clockticks+=3;
    A=ReadMemory(0x69); 
//  ZN=A;
//case 0x75C1: //AA 29 3F
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x75C2: //29 3F F0
    //clockticks+=3;
    A &= 0x3F;
  ZN=A;


//L_75C4:
case 0x75C4: //F0 01 CA
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x75C4,0xF0);
        PCSTART=0x75C7;
        /**/return;
        //goto L_75C7;
    } 
//case 0x75C6: //CA 86 69
    //clockticks+=2;
    X--; 
  ZN=X;


//L_75C7:
case 0x75C7: //86 69 8E
clockticks+=6; //SUM
    //clockticks+=2;
    WriteMemory(0x69,X);
//case 0x75C9: //8E 00 36
    //clockticks+=4;
    WriteMemory(0x3600,X);


//L_75CC:
case 0x75CC: //60 B5 6A
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x75CC,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_75CD:
case 0x75CD: //B5 6A 30
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x006A; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_75CF:
case 0x75CF: //30 0C B5
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x75CF,0x30);
        PCSTART=0x75DD;
        /**/return;
        //goto L_75DD;
    }
//case 0x75D1: //B5 66 10
    //clockticks+=4;
    temp=0x0066; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_75D3:
case 0x75D3: //10 12 A9
clockticks+=9; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x75D3,0x10);
        PCSTART=0x75E7;
        /**/return;
        //goto L_75E7;
    }
//case 0x75D5: //A9 10 95
    //clockticks+=3;
    A=0x10;
  ZN=A;
//case 0x75D7: //95 66 A9
    //clockticks+=4;
    temp=0x66; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_75D9:
case 0x75D9: //A9 80 30
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x80;
  ZN=A;


//L_75DB:
case 0x75DB: //30 0C B5
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x75DB,0x30);
        PCSTART=0x75E9;
        /**/return;
        //goto L_75E9;
    }


//L_75DD:
case 0x75DD: //B5 66 F0
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0066; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_75DF:
case 0x75DF: //F0 06 30
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x75DF,0xF0);
        PCSTART=0x75E7;
        /**/return;
        //goto L_75E7;
    } 


//L_75E1:
case 0x75E1: //30 04 D6
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x75E1,0x30);
        PCSTART=0x75E7;
        /**/return;
        //goto L_75E7;
    }
//case 0x75E3: //D6 66 D0
    //clockticks+=6;
    temp=0x0066; temp+=X; temp&=0xFF;
    value=ReadMemory(temp);
    value--;
    WriteMemory(temp,value);
  ZN=value;


//L_75E5:
case 0x75E5: //D0 F2 A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x75E5,0xD0);
        PCSTART=0x75D9;
        /**/return;
        //goto L_75D9;
    }


//L_75E7:
case 0x75E7: //A9 00 95
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;


//L_75E9:
case 0x75E9: //95 6A 60
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x6A; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_75EB:
case 0x75EB: //60 86 0D
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x75EB,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_75EC:
case 0x75EC: //86 0D A9
clockticks+=29; //SUM
    //clockticks+=2;
    WriteMemory(0x0D,X);
//case 0x75EE: //A9 50 8D
    //clockticks+=3;
    A=0x50;
//  ZN=A;
//case 0x75F0: //8D F9 02
    //clockticks+=4;
    WriteMemory(0x02F9,A);
//case 0x75F3: //B9 00 02
    //clockticks+=4;
    temp=0x0200; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x75F6: //29 78 85
    //clockticks+=3;
    A &= 0x78;
//  ZN=A;
//case 0x75F8: //85 0E B9
    //clockticks+=2;
    WriteMemory(0x0E,A);
//case 0x75FA: //B9 00 02
    //clockticks+=4;
    temp=0x0200; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x75FD: //29 07 4A
    //clockticks+=3;
    A &= 0x07;
//  ZN=A;
//case 0x75FF: //4A AA F0
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7600: //AA F0 02
    //clockticks+=2;
    X=A;
  ZN=X;


//L_7601:
case 0x7601: //F0 02 05
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7601,0xF0);
        PCSTART=0x7605;
        /**/return;
        //goto L_7605;
    } 
//case 0x7603: //05 0E 99
    //clockticks+=3;
    A |= ReadMemory(0x0E);
  ZN=A;


//L_7605:
case 0x7605: //99 00 02
clockticks+=8; //SUM
    //clockticks+=5;
    temp=0x0200; temp+=Y;
    WriteMemory(temp,A);
//case 0x7608: //A5 1C F0
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_760A:
case 0x760A: //F0 11 A5
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x760A,0xF0);
        PCSTART=0x761D;
        /**/return;
        //goto L_761D;
    } 
//case 0x760C: //A5 0D F0
    //clockticks+=3;
    A=ReadMemory(0x0D); 
  ZN=A;


//L_760E:
case 0x760E: //F0 04 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x760E,0xF0);
        PCSTART=0x7614;
        /**/return;
        //goto L_7614;
    } 
//case 0x7610: //C9 04 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x04;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7612:
case 0x7612: //90 09 BD
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7612,0x90);
        PCSTART=0x761D;
        /**/return;
        //goto L_761D;
    } 


//L_7614:
case 0x7614: //BD 59 76
clockticks+=9; //SUM
    //clockticks+=4;
    temp=0x7659; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7617: //A6 19 18
    //clockticks+=3;
    X=ReadMemory(0x19); 
  ZN=X;
//case 0x7619: //18 20 97
    //clockticks+=2;
    C = 0;


//L_761A:
case 0x761A: //20 97 73
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x76;
    stack[S--]=0x1D;
    showme(0x761A,0x20);
    PCSTART=0x7397; //jsr
    /**/return;
    //goto L_7397;


//L_761D:
case 0x761D: //BE 00 02
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0200; temp+=Y;
    X=ReadMemory(temp);
  ZN=X;


//L_7620:
case 0x7620: //F0 34 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7620,0xF0);
        PCSTART=0x7656;
        /**/return;
        //goto L_7656;
    } 


//L_7622:
case 0x7622: //20 5A 74
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x76;
    stack[S--]=0x25;
    showme(0x7622,0x20);
    PCSTART=0x745A; //jsr
    /**/return;
    //goto L_745A;


//L_7625:
case 0x7625: //30 2F EE
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7625,0x30);
        PCSTART=0x7656;
        /**/return;
        //goto L_7656;
    }
//case 0x7627: //EE F6 02
    //clockticks+=6;
    value=ReadMemory(0x02F6);
    value++;
    WriteMemory(0x02F6,value);
  ZN=value;


//L_762A:
case 0x762A: //20 9D 6A
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x76;
    stack[S--]=0x2D;
    showme(0x762A,0x20);
    PCSTART=0x6A9D; //jsr
    /**/return;
    //goto L_6A9D;


//L_762D:
case 0x762D: //20 03 72
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x76;
    stack[S--]=0x30;
    showme(0x762D,0x20);
    PCSTART=0x7203; //jsr
    /**/return;
    //goto L_7203;


//L_7630:
case 0x7630: //BD 23 02
clockticks+=18; //SUM
    //clockticks+=4;
    temp=0x0223; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7633: //29 1F 0A
    //clockticks+=3;
    A &= 0x1F;
//  ZN=A;
//case 0x7635: //0A 5D AF
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7636: //5D AF 02
    //clockticks+=4;
    temp=0x02AF; temp+=X;
    A ^= ReadMemory(temp);
  ZN=A;
//case 0x7639: //9D AF 02
    //clockticks+=5;
    temp=0x02AF; temp+=X;
    WriteMemory(temp,A);


//L_763C:
case 0x763C: //20 5C 74
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x76;
    stack[S--]=0x3F;
    showme(0x763C,0x20);
    PCSTART=0x745C; //jsr
    /**/return;
    //goto L_745C;


//L_763F:
case 0x763F: //30 15 EE
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x763F,0x30);
        PCSTART=0x7656;
        /**/return;
        //goto L_7656;
    }
//case 0x7641: //EE F6 02
    //clockticks+=6;
    value=ReadMemory(0x02F6);
    value++;
    WriteMemory(0x02F6,value);
  ZN=value;


//L_7644:
case 0x7644: //20 9D 6A
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x76;
    stack[S--]=0x47;
    showme(0x7644,0x20);
    PCSTART=0x6A9D; //jsr
    /**/return;
    //goto L_6A9D;


//L_7647:
case 0x7647: //20 03 72
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x76;
    stack[S--]=0x4A;
    showme(0x7647,0x20);
    PCSTART=0x7203; //jsr
    /**/return;
    //goto L_7203;


//L_764A:
case 0x764A: //BD 46 02
clockticks+=18; //SUM
    //clockticks+=4;
    temp=0x0246; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x764D: //29 1F 0A
    //clockticks+=3;
    A &= 0x1F;
//  ZN=A;
//case 0x764F: //0A 5D D2
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7650: //5D D2 02
    //clockticks+=4;
    temp=0x02D2; temp+=X;
    A ^= ReadMemory(temp);
  ZN=A;
//case 0x7653: //9D D2 02
    //clockticks+=5;
    temp=0x02D2; temp+=X;
    WriteMemory(temp,A);


//L_7656:
case 0x7656: //A6 0D 60
clockticks+=3; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0D); 
  ZN=X;


//L_7658:
case 0x7658: //60 10 05
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7658,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_765C:
case 0x765C: //A5 1C 10
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x1C); 
  ZN=A;


//L_765E:
case 0x765E: //10 38 A2
clockticks+=11; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x765E,0x10);
        PCSTART=0x7698;
        /**/return;
        //goto L_7698;
    }
//case 0x7660: //A2 02 85
    //clockticks+=3;
    X=0x02;
  ZN=X;
//case 0x7662: //85 5D 85
    //clockticks+=2;
    WriteMemory(0x5D,A);
//case 0x7664: //85 32 85
    //clockticks+=2;
    WriteMemory(0x32,A);
//case 0x7666: //85 33 A0
    //clockticks+=2;
    WriteMemory(0x33,A);


//L_7668:
case 0x7668: //A0 00 B9
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x00; 
  ZN=Y;


//L_766A:
case 0x766A: //B9 1D 00
clockticks+=16; //SUM
    //clockticks+=4;
    temp=0x001D; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x766D: //D5 52 B9
    //clockticks+=4;
    temp=0x0052; temp+=X; temp&=0xFF;
    value = ReadMemory(temp);
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
//  ZN=value;
//case 0x766F: //B9 1E 00
    //clockticks+=4;
    temp=0x001E; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7672: //F5 53 90
    //clockticks+=4;
    temp=0x0053; temp+=X; temp&=0xFF;
    SBC(ReadMemory(temp));


//L_7674:
case 0x7674: //90 23 C8
clockticks+=9; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7674,0x90);
        PCSTART=0x7699;
        /**/return;
        //goto L_7699;
    } 
//case 0x7676: //C8 C8 C0
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7677: //C8 C0 14
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7678: //C0 14 90
    //clockticks+=3;
    temp=Y; temp+=0x100; temp-=0x14;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_767A:
case 0x767A: //90 EE CA
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x767A,0x90);
        PCSTART=0x766A;
        /**/return;
        //goto L_766A;
    } 


//L_767C:
case 0x767C: //CA CA 10
clockticks+=4; //SUM
    //clockticks+=2;
    X--; 
//  ZN=X;
//case 0x767D: //CA 10 E8
    //clockticks+=2;
    X--; 
  ZN=X;


//L_767E:
case 0x767E: //10 E8 A5
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x767E,0x10);
        PCSTART=0x7668;
        /**/return;
        //goto L_7668;
    }
//case 0x7680: //A5 33 30
    //clockticks+=3;
    A=ReadMemory(0x33); 
  ZN=A;


//L_7682:
case 0x7682: //30 0E C5
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7682,0x30);
        PCSTART=0x7692;
        /**/return;
        //goto L_7692;
    }
//case 0x7684: //C5 32 90
    //clockticks+=3;
    value=ReadMemory(0x32);
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7686:
case 0x7686: //90 0A 69
clockticks+=8; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7686,0x90);
        PCSTART=0x7692;
        /**/return;
        //goto L_7692;
    } 
//case 0x7688: //69 02 C9
    //clockticks+=3;
    ADC(0x02);
//case 0x768A: //C9 1E 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x1E;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_768C:
case 0x768C: //90 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x768C,0x90);
        PCSTART=0x7690;
        /**/return;
        //goto L_7690;
    } 
//case 0x768E: //A9 FF 85
    //clockticks+=3;
    A=0xFF;
  ZN=A;


//L_7690:
case 0x7690: //85 33 A9
clockticks+=2; //SUM
    //clockticks+=2;
    WriteMemory(0x33,A);


//L_7692:
case 0x7692: //A9 00 85
clockticks+=7; //SUM
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x7694: //85 1C 85
    //clockticks+=2;
    WriteMemory(0x1C,A);
//case 0x7696: //85 31 60
    //clockticks+=2;
    WriteMemory(0x31,A);


//L_7698:
case 0x7698: //60 86 0B
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7698,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7699:
case 0x7699: //86 0B 84
clockticks+=29; //SUM
    //clockticks+=2;
    WriteMemory(0x0B,X);
//case 0x769B: //84 0C 8A
    //clockticks+=2;
    WriteMemory(0x0C,Y);
//case 0x769D: //8A 4A AA
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x769E: //4A AA 98
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x769F: //AA 98 4A
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x76A0: //98 4A 65
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x76A1: //4A 65 0C
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x76A2: //65 0C 85
    //clockticks+=3;
    ADC(ReadMemory(0x0C));
//case 0x76A4: //85 0D 95
    //clockticks+=2;
    WriteMemory(0x0D,A);
//case 0x76A6: //95 32 A2
    //clockticks+=4;
    temp=0x32; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x76A8: //A2 1B A0
    //clockticks+=3;
    X=0x1B;
//  ZN=X;
//case 0x76AA: //A0 12 E4
    //clockticks+=3;
    Y = 0x12; 
  ZN=Y;


//L_76AC:
case 0x76AC: //E4 0D F0
clockticks+=3; //SUM
    //clockticks+=3;
    temp=X; temp+=0x100; temp-=ReadMemory(0x0D);
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_76AE:
case 0x76AE: //F0 1F B5
clockticks+=54; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x76AE,0xF0);
        PCSTART=0x76CF;
        /**/return;
        //goto L_76CF;
    } 
//case 0x76B0: //B5 31 95
    //clockticks+=4;
    temp=0x0031; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x76B2: //95 34 B5
    //clockticks+=4;
    temp=0x34; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x76B4: //B5 32 95
    //clockticks+=4;
    temp=0x0032; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x76B6: //95 35 B5
    //clockticks+=4;
    temp=0x35; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x76B8: //B5 33 95
    //clockticks+=4;
    temp=0x0033; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x76BA: //95 36 B9
    //clockticks+=4;
    temp=0x36; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x76BC: //B9 1B 00
    //clockticks+=4;
    temp=0x001B; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x76BF: //99 1D 00
    //clockticks+=5;
    temp=0x001D; temp+=Y;
    WriteMemory(temp,A);
//case 0x76C2: //B9 1C 00
    //clockticks+=4;
    temp=0x001C; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x76C5: //99 1E 00
    //clockticks+=5;
    temp=0x001E; temp+=Y;
    WriteMemory(temp,A);
//case 0x76C8: //88 88 CA
    //clockticks+=2;
    Y--;
//  ZN=Y;
//case 0x76C9: //88 CA CA
    //clockticks+=2;
    Y--;
//  ZN=Y;
//case 0x76CA: //CA CA CA
    //clockticks+=2;
    X--; 
//  ZN=X;
//case 0x76CB: //CA CA D0
    //clockticks+=2;
    X--; 
//  ZN=X;
//case 0x76CC: //CA D0 DD
    //clockticks+=2;
    X--; 
  ZN=X;


//L_76CD:
case 0x76CD: //D0 DD A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x76CD,0xD0);
        PCSTART=0x76AC;
        /**/return;
        //goto L_76AC;
    }


//L_76CF:
case 0x76CF: //A9 0B 95
clockticks+=50; //SUM
    //clockticks+=3;
    A=0x0B;
//  ZN=A;
//case 0x76D1: //95 34 A9
    //clockticks+=4;
    temp=0x34; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x76D3: //A9 00 95
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x76D5: //95 35 95
    //clockticks+=4;
    temp=0x35; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x76D7: //95 36 A9
    //clockticks+=4;
    temp=0x36; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x76D9: //A9 F0 85
    //clockticks+=3;
    A=0xF0;
//  ZN=A;
//case 0x76DB: //85 5D A6
    //clockticks+=2;
    WriteMemory(0x5D,A);
//case 0x76DD: //A6 0B A4
    //clockticks+=3;
    X=ReadMemory(0x0B); 
//  ZN=X;
//case 0x76DF: //A4 0C B5
    //clockticks+=3;
    Y=ReadMemory(0x0C);
//  ZN=Y;
//case 0x76E1: //B5 53 99
    //clockticks+=4;
    temp=0x0053; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x76E3: //99 1E 00
    //clockticks+=5;
    temp=0x001E; temp+=Y;
    WriteMemory(temp,A);
//case 0x76E6: //B5 52 99
    //clockticks+=4;
    temp=0x0052; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x76E8: //99 1D 00
    //clockticks+=5;
    temp=0x001D; temp+=Y;
    WriteMemory(temp,A);
//case 0x76EB: //A0 00 F0
    //clockticks+=3;
    Y = 0x00; 
  ZN=Y;


//L_76ED:
case 0x76ED: //F0 8D DF
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x76ED,0xF0);
        PCSTART=0x767C;
        /**/return;
        //goto L_767C;
    } 
//case 0x76EF: //DF 98 10
    //clockticks+=2;


//L_76F0:
case 0x76F0: //98 10 09
clockticks+=2; //SUM
    //clockticks+=2;
    A = Y;
  ZN=A;


//L_76F1:
case 0x76F1: //10 09 20
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x76F1,0x10);
        PCSTART=0x76FC;
        /**/return;
        //goto L_76FC;
    }


//L_76F3:
case 0x76F3: //20 08 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x76;
    stack[S--]=0xF6;
    showme(0x76F3,0x20);
    PCSTART=0x7708; //jsr
    /**/return;
    //goto L_7708;


//L_76F6:
case 0x76F6: //20 FC 76
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x76;
    stack[S--]=0xF9;
    showme(0x76F6,0x20);
    PCSTART=0x76FC; //jsr
    /**/return;
    //goto L_76FC;


//L_76F9:
case 0x76F9: //4C 08 77
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x76F9,0x4C);
    PCSTART=0x7708;
    /**/return;
    //goto L_7708;


//L_76FC:
case 0x76FC: //A8 8A 10
clockticks+=4; //SUM
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x76FD: //8A 10 0E
    //clockticks+=2;
    A = X; 
  ZN=A;


//L_76FE:
case 0x76FE: //10 0E 20
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x76FE,0x10);
        PCSTART=0x770E;
        /**/return;
        //goto L_770E;
    }


//L_7700:
case 0x7700: //20 08 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x77;
    stack[S--]=0x03;
    showme(0x7700,0x20);
    PCSTART=0x7708; //jsr
    /**/return;
    //goto L_7708;


//L_7703:
case 0x7703: //20 0E 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x77;
    stack[S--]=0x06;
    showme(0x7703,0x20);
    PCSTART=0x770E; //jsr
    /**/return;
    //goto L_770E;


//L_7706:
case 0x7706: //49 80 49
clockticks+=3; //SUM
    //clockticks+=3;
    A ^= 0x80;
  ZN=A;


//L_7708:
case 0x7708: //49 FF 18
clockticks+=8; //SUM
    //clockticks+=3;
    A ^= 0xFF;
//  ZN=A;
//case 0x770A: //18 69 01
    //clockticks+=2;
    C = 0;
//case 0x770B: //69 01 60
    //clockticks+=3;
    ADC(0x01);


//L_770D:
case 0x770D: //60 85 0C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x770D,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_770E:
case 0x770E: //85 0C 98
clockticks+=7; //SUM
    //clockticks+=2;
    WriteMemory(0x0C,A);
//case 0x7710: //98 C5 0C
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x7711: //C5 0C F0
    //clockticks+=3;
    value=ReadMemory(0x0C);
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7713:
case 0x7713: //F0 10 90
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7713,0xF0);
        PCSTART=0x7725;
        /**/return;
        //goto L_7725;
    } 


//L_7715:
case 0x7715: //90 11 A4
clockticks+=9; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7715,0x90);
        PCSTART=0x7728;
        /**/return;
        //goto L_7728;
    } 
//case 0x7717: //A4 0C 85
    //clockticks+=3;
    Y=ReadMemory(0x0C);
//  ZN=Y;
//case 0x7719: //85 0C 98
    //clockticks+=2;
    WriteMemory(0x0C,A);
//case 0x771B: //98 20 28
    //clockticks+=2;
    A = Y;
  ZN=A;


//L_771C:
case 0x771C: //20 28 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x77;
    stack[S--]=0x1F;
    showme(0x771C,0x20);
    PCSTART=0x7728; //jsr
    /**/return;
    //goto L_7728;


//L_771F:
case 0x771F: //38 E9 40
clockticks+=5; //SUM
    //clockticks+=2;
    C = 0x01;
//case 0x7720: //E9 40 4C
    //clockticks+=3;
    SBC(0x40);


//L_7722:
case 0x7722: //4C 08 77
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x7722,0x4C);
    PCSTART=0x7708;
    /**/return;
    //goto L_7708;


//L_7725:
case 0x7725: //A9 20 60
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x20;
  ZN=A;


//L_7727:
case 0x7727: //60 20 6C
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7727,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7728:
case 0x7728: //20 6C 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x77;
    stack[S--]=0x2B;
    showme(0x7728,0x20);
    PCSTART=0x776C; //jsr
    /**/return;
    //goto L_776C;


//L_772B:
case 0x772B: //BD 2F 77
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x772F; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_772E:
case 0x772E: //60 00 02
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x772E,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_773F:
case 0x773F: //08 86 17
clockticks+=22; //SUM
    //clockticks+=3;
    if(ZN) ZN|=0x02;
    stack[S--]=(ZN&0x82)|(DEC&0x08)|(V&0x40)|(C&0x01);
//case 0x7740: //86 17 88
    //clockticks+=2;
    WriteMemory(0x17,X);
//case 0x7742: //88 84 16
    //clockticks+=2;
    Y--;
//  ZN=Y;
//case 0x7743: //84 16 18
    //clockticks+=2;
    WriteMemory(0x16,Y);
//case 0x7745: //18 65 16
    //clockticks+=2;
    C = 0;
//case 0x7746: //65 16 85
    //clockticks+=3;
    ADC(ReadMemory(0x16));
//case 0x7748: //85 15 28
    //clockticks+=2;
    WriteMemory(0x15,A);
//case 0x774A: //28 AA 08
    //clockticks+=4;
    ZN=C=DEC=V=stack[++S]; // | 0x20;
    ZN&=0x82;
//case 0x774B: //AA 08 B5
    //clockticks+=2;
    X=A;
  ZN=X;


//L_774C:
case 0x774C: //08 B5 00
clockticks+=19; //SUM
    //clockticks+=3;
    if(ZN) ZN|=0x02;
    stack[S--]=(ZN&0x82)|(DEC&0x08)|(V&0x40)|(C&0x01);
//case 0x774D: //B5 00 4A
    //clockticks+=4;
    temp=0x0000; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x774F: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7750: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7751: //4A 4A 28
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7752: //4A 28 20
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;
//case 0x7753: //28 20 85
    //clockticks+=4;
    ZN=C=DEC=V=stack[++S]; // | 0x20;
    ZN&=0x82;


//L_7754:
case 0x7754: //20 85 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x77;
    stack[S--]=0x57;
    showme(0x7754,0x20);
    PCSTART=0x7785; //jsr
    /**/return;
    //goto L_7785;


//L_7757:
case 0x7757: //A5 16 D0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x16); 
  ZN=A;


//L_7759:
case 0x7759: //D0 01 18
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7759,0xD0);
        PCSTART=0x775C;
        /**/return;
        //goto L_775C;
    }
//case 0x775B: //18 A6 15
    //clockticks+=2;
    C = 0;


//L_775C:
case 0x775C: //A6 15 B5
clockticks+=7; //SUM
    //clockticks+=3;
    X=ReadMemory(0x15); 
//  ZN=X;
//case 0x775E: //B5 00 20
    //clockticks+=4;
    temp=0x0000; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_7760:
case 0x7760: //20 85 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x77;
    stack[S--]=0x63;
    showme(0x7760,0x20);
    PCSTART=0x7785; //jsr
    /**/return;
    //goto L_7785;


//L_7763:
case 0x7763: //C6 15 A6
clockticks+=13; //SUM
    //clockticks+=5;
    value=ReadMemory(0x15);
    value--;
    WriteMemory(0x15,value);
//  ZN=value;
//case 0x7765: //A6 15 C6
    //clockticks+=3;
    X=ReadMemory(0x15); 
//  ZN=X;
//case 0x7767: //C6 16 10
    //clockticks+=5;
    value=ReadMemory(0x16);
    value--;
    WriteMemory(0x16,value);
  ZN=value;


//L_7769:
case 0x7769: //10 E1 60
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7769,0x10);
        PCSTART=0x774C;
        /**/return;
        //goto L_774C;
    }


//L_776B:
case 0x776B: //60 A0 00
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x776B,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_776C:
case 0x776C: //A0 00 84
clockticks+=8; //SUM
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x776E: //84 0B A0
    //clockticks+=2;
    WriteMemory(0x0B,Y);
//case 0x7770: //A0 04 26
    //clockticks+=3;
    Y = 0x04; 
  ZN=Y;


//L_7772:
case 0x7772: //26 0B 2A
clockticks+=10; //SUM
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x0B);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x0B,value);
//  ZN=value;
//case 0x7774: //2A C5 0C
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7775: //C5 0C 90
    //clockticks+=3;
    value=ReadMemory(0x0C);
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7777:
case 0x7777: //90 02 E5
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7777,0x90);
        PCSTART=0x777B;
        /**/return;
        //goto L_777B;
    } 
//case 0x7779: //E5 0C 88
    //clockticks+=3;
    SBC(ReadMemory(0x0C));


//L_777B:
case 0x777B: //88 D0 F4
clockticks+=2; //SUM
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_777C:
case 0x777C: //D0 F4 A5
clockticks+=12; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x777C,0xD0);
        PCSTART=0x7772;
        /**/return;
        //goto L_7772;
    }
//case 0x777E: //A5 0B 2A
    //clockticks+=3;
    A=ReadMemory(0x0B); 
//  ZN=A;
//case 0x7780: //2A 29 0F
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7781: //29 0F AA
    //clockticks+=3;
    A &= 0x0F;
//  ZN=A;
//case 0x7783: //AA 60 90
    //clockticks+=2;
    X=A;
  ZN=X;


//L_7784:
case 0x7784: //60 90 04
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7784,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7785:
case 0x7785: //90 04 29
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7785,0x90);
        PCSTART=0x778B;
        /**/return;
        //goto L_778B;
    } 
//case 0x7787: //29 0F F0
    //clockticks+=3;
    A &= 0x0F;
  ZN=A;


//L_7789:
case 0x7789: //F0 27 A6
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7789,0xF0);
        PCSTART=0x77B2;
        /**/return;
        //goto L_77B2;
    } 


//L_778B:
case 0x778B: //A6 17 F0
clockticks+=3; //SUM
    //clockticks+=3;
    X=ReadMemory(0x17); 
  ZN=X;


//L_778D:
case 0x778D: //F0 23 29
clockticks+=46; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x778D,0xF0);
        PCSTART=0x77B2;
        /**/return;
        //goto L_77B2;
    } 
//case 0x778F: //29 0F 18
    //clockticks+=3;
    A &= 0x0F;
//  ZN=A;
//case 0x7791: //18 69 01
    //clockticks+=2;
    C = 0;
//case 0x7792: //69 01 08
    //clockticks+=3;
    ADC(0x01);
//case 0x7794: //08 0A A8
    //clockticks+=3;
    if(ZN) ZN|=0x02;
    stack[S--]=(ZN&0x82)|(DEC&0x08)|(V&0x40)|(C&0x01);
//case 0x7795: //0A A8 B9
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7796: //A8 B9 D4
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x7797: //B9 D4 56
    //clockticks+=4;
    temp=0x56D4; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x779A: //0A 85 0B
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x779B: //85 0B B9
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x779D: //B9 D5 56
    //clockticks+=4;
    temp=0x56D5; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x77A0: //2A 29 1F
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x77A1: //29 1F 09
    //clockticks+=3;
    A &= 0x1F;
//  ZN=A;
//case 0x77A3: //09 40 85
    //clockticks+=3;
    A |= 0x40;
//  ZN=A;
//case 0x77A5: //85 0C A9
    //clockticks+=2;
    WriteMemory(0x0C,A);
//case 0x77A7: //A9 00 85
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x77A9: //85 08 85
    //clockticks+=2;
    WriteMemory(0x08,A);
//case 0x77AB: //85 09 20
    //clockticks+=2;
    WriteMemory(0x09,A);


//L_77AD:
case 0x77AD: //20 D7 6A
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x77;
    stack[S--]=0xB0;
    showme(0x77AD,0x20);
    PCSTART=0x6AD7; //jsr
    /**/return;
    //goto L_6AD7;


//L_77B0:
case 0x77B0: //28 60 4C
clockticks+=4; //SUM
    //clockticks+=4;
    ZN=C=DEC=V=stack[++S]; // | 0x20;
    ZN&=0x82;


//L_77B1:
case 0x77B1: //60 4C CB
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x77B1,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_77B2:
case 0x77B2: //4C CB 7B
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x77B2,0x4C);
    PCSTART=0x7BCB;
    /**/return;
    //goto L_7BCB;


//L_77B5:
case 0x77B5: //06 5F 26
clockticks+=10; //SUM
    //clockticks+=5;
    value = ReadMemory(0x5F);
    C = value >> 7;
    value = value << 1;
    WriteMemory(0x5F,value);
//  ZN=value;
//case 0x77B7: //26 60 10
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x60);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x60,value);
  ZN=value;


//L_77B9:
case 0x77B9: //10 02 E6
clockticks+=7; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x77B9,0x10);
        PCSTART=0x77BD;
        /**/return;
        //goto L_77BD;
    }
//case 0x77BB: //E6 5F A5
    //clockticks+=5;
    value=ReadMemory(0x5F);
    value++;
    WriteMemory(0x5F,value);
  ZN=value;


//L_77BD:
case 0x77BD: //A5 5F 2C
clockticks+=7; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5F); 
//  ZN=A;
//case 0x77BF: //2C D1 77
    //clockticks+=4;
    value=0x02;
    V = value;
  ZN = value & 0x80;
  if((value&A)==0) ZN |= 0x02;


//L_77C2:
case 0x77C2: //F0 04 49
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x77C2,0xF0);
        PCSTART=0x77C8;
        /**/return;
        //goto L_77C8;
    } 
//case 0x77C4: //49 01 85
    //clockticks+=3;
    A ^= 0x01;
  ZN=A;
//case 0x77C6: //85 5F 05
    //clockticks+=2;
    WriteMemory(0x5F,A);


//L_77C8:
case 0x77C8: //05 60 D0
clockticks+=3; //SUM
    //clockticks+=3;
    A |= ReadMemory(0x60);
  ZN=A;


//L_77CA:
case 0x77CA: //D0 02 E6
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x77CA,0xD0);
        PCSTART=0x77CE;
        /**/return;
        //goto L_77CE;
    }
//case 0x77CC: //E6 5F A5
    //clockticks+=5;
    value=ReadMemory(0x5F);
    value++;
    WriteMemory(0x5F,value);
  ZN=value;


//L_77CE:
case 0x77CE: //A5 5F 60
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5F); 
  ZN=A;


//L_77D0:
case 0x77D0: //60 02 18
clockticks+=0; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x77D0,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_77D2:
case 0x77D2: //18 69 40
clockticks+=5; //SUM
    //clockticks+=2;
    C = 0;
//case 0x77D3: //69 40 10
    //clockticks+=3;
    ADC(0x40);


//L_77D5:
case 0x77D5: //10 08 29
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x77D5,0x10);
        PCSTART=0x77DF;
        /**/return;
        //goto L_77DF;
    }
//case 0x77D7: //29 7F 20
    //clockticks+=3;
    A &= 0x7F;
  ZN=A;


//L_77D9:
case 0x77D9: //20 DF 77
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x77;
    stack[S--]=0xDC;
    showme(0x77D9,0x20);
    PCSTART=0x77DF; //jsr
    /**/return;
    //goto L_77DF;


//L_77DC:
case 0x77DC: //4C 08 77
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x77DC,0x4C);
    PCSTART=0x7708;
    /**/return;
    //goto L_7708;


//L_77DF:
case 0x77DF: //C9 41 90
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x41;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_77E1:
case 0x77E1: //90 04 49
clockticks+=8; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x77E1,0x90);
        PCSTART=0x77E7;
        /**/return;
        //goto L_77E7;
    } 
//case 0x77E3: //49 7F 69
    //clockticks+=3;
    A ^= 0x7F;
//  ZN=A;
//case 0x77E5: //69 00 AA
    //clockticks+=3;
    ADC(0x00);


//L_77E7:
case 0x77E7: //AA BD B9
clockticks+=6; //SUM
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x77E8: //BD B9 57
    //clockticks+=4;
    temp=0x57B9; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_77EB:
case 0x77EB: //60 00 00
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x77EB,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_77F6:
case 0x77F6: //AD 03 28
clockticks+=35; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2803); 
//  ZN=A;
//case 0x77F9: //29 03 0A
    //clockticks+=3;
    A &= 0x03;
//  ZN=A;
//case 0x77FB: //0A AA A9
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x77FC: //AA A9 10
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x77FD: //A9 10 85
    //clockticks+=3;
    A=0x10;
//  ZN=A;
//case 0x77FF: //85 00 BD
    //clockticks+=2;
    WriteMemory(0x00,A);
//case 0x7801: //BD 88 78
    //clockticks+=4;
    temp=0x7888; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7804: //85 09 BD
    //clockticks+=2;
    WriteMemory(0x09,A);
//case 0x7806: //BD 87 78
    //clockticks+=4;
    temp=0x7887; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7809: //85 08 71
    //clockticks+=2;
    WriteMemory(0x08,A);
//case 0x780B: //71 08 85
    //clockticks+=5;
    temp=ReadMemory(0x09);
    temp<<=8;
    temp|=ReadMemory(0x08);
    temp=temp+Y;
    ADC(ReadMemory(temp));
//case 0x780D: //85 08 90
    //clockticks+=2;
    WriteMemory(0x08,A);


//L_780F:
case 0x780F: //90 02 E6
clockticks+=7; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x780F,0x90);
        PCSTART=0x7813;
        /**/return;
        //goto L_7813;
    } 
//case 0x7811: //E6 09 98
    //clockticks+=5;
    value=ReadMemory(0x09);
    value++;
    WriteMemory(0x09,value);
  ZN=value;


//L_7813:
case 0x7813: //98 0A A8
clockticks+=14; //SUM
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x7814: //0A A8 B9
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7815: //A8 B9 71
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x7816: //B9 71 78
    //clockticks+=4;
    temp=0x7871; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7819: //BE 72 78
    //clockticks+=4;
    temp=0x7872; temp+=Y;
    X=ReadMemory(temp);
  ZN=X;


//L_781C:
case 0x781C: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x78;
    stack[S--]=0x1F;
    showme(0x781C,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_781F:
case 0x781F: //A9 70 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x70;
  ZN=A;


//L_7821:
case 0x7821: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x78;
    stack[S--]=0x24;
    showme(0x7821,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_7824:
case 0x7824: //A0 00 A2
clockticks+=6; //SUM
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7826: //A2 00 A1
    //clockticks+=3;
    X=0x00;
  ZN=X;


//L_7828:
case 0x7828: //A1 08 85
clockticks+=12; //SUM
    //clockticks+=6;
    value=0x08; value+=X;
    temp=ReadMemory(value+1);
    temp<<=8;
    temp|=ReadMemory(value);
    A=ReadMemory(temp);
//  ZN=A;
//case 0x782A: //85 0B 4A
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x782C: //4A 4A 20
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x782D: //4A 20 4D
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_782E:
case 0x782E: //20 4D 78
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x78;
    stack[S--]=0x31;
    showme(0x782E,0x20);
    PCSTART=0x784D; //jsr
    /**/return;
    //goto L_784D;


//L_7831:
case 0x7831: //A1 08 2A
clockticks+=22; //SUM
    //clockticks+=6;
    value=0x08; value+=X;
    temp=ReadMemory(value+1);
    temp<<=8;
    temp|=ReadMemory(value);
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7833: //2A 26 0B
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7834: //26 0B 2A
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x0B);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x0B,value);
//  ZN=value;
//case 0x7836: //2A A5 0B
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7837: //A5 0B 2A
    //clockticks+=3;
    A=ReadMemory(0x0B); 
//  ZN=A;
//case 0x7839: //2A 0A 20
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x783A: //0A 20 53
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
  ZN=A;


//L_783B:
case 0x783B: //20 53 78
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x78;
    stack[S--]=0x3E;
    showme(0x783B,0x20);
    PCSTART=0x7853; //jsr
    /**/return;
    //goto L_7853;


//L_783E:
case 0x783E: //A1 08 85
clockticks+=8; //SUM
    //clockticks+=6;
    value=0x08; value+=X;
    temp=ReadMemory(value+1);
    temp<<=8;
    temp|=ReadMemory(value);
    A=ReadMemory(temp);
  ZN=A;
//case 0x7840: //85 0B 20
    //clockticks+=2;
    WriteMemory(0x0B,A);


//L_7842:
case 0x7842: //20 4D 78
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x78;
    stack[S--]=0x45;
    showme(0x7842,0x20);
    PCSTART=0x784D; //jsr
    /**/return;
    //goto L_784D;


//L_7845:
case 0x7845: //46 0B 90
clockticks+=5; //SUM
    //clockticks+=5;
    value=ReadMemory(0x0B);
    C = value;
    value = value >> 1;
    WriteMemory(0x0B,value);
  ZN=value;


//L_7847:
case 0x7847: //90 DF 88
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7847,0x90);
        PCSTART=0x7828;
        /**/return;
        //goto L_7828;
    } 


//L_7849:
case 0x7849: //88 4C 39
clockticks+=2; //SUM
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_784A:
case 0x784A: //4C 39 7C
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x784A,0x4C);
    PCSTART=0x7C39;
    /**/return;
    //goto L_7C39;


//L_784D:
case 0x784D: //E6 08 D0
clockticks+=5; //SUM
    //clockticks+=5;
    value=ReadMemory(0x08);
    value++;
    WriteMemory(0x08,value);
  ZN=value;


//L_784F:
case 0x784F: //D0 02 E6
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x784F,0xD0);
        PCSTART=0x7853;
        /**/return;
        //goto L_7853;
    }
//case 0x7851: //E6 09 29
    //clockticks+=5;
    value=ReadMemory(0x09);
    value++;
    WriteMemory(0x09,value);
  ZN=value;


//L_7853:
case 0x7853: //29 3E D0
clockticks+=3; //SUM
    //clockticks+=3;
    A &= 0x3E;
  ZN=A;


//L_7855:
case 0x7855: //D0 04 68
clockticks+=10; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7855,0xD0);
        PCSTART=0x785B;
        /**/return;
        //goto L_785B;
    }
//case 0x7857: //68 68 D0
    //clockticks+=4;
    A=stack[++S];
//  ZN=A;
//case 0x7858: //68 D0 EE
    //clockticks+=4;
    A=stack[++S];
  ZN=A;


//L_7859:
case 0x7859: //D0 EE C9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7859,0xD0);
        PCSTART=0x7849;
        /**/return;
        //goto L_7849;
    }


//L_785B:
case 0x785B: //C9 0A 90
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x0A;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_785D:
case 0x785D: //90 02 69
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x785D,0x90);
        PCSTART=0x7861;
        /**/return;
        //goto L_7861;
    } 
//case 0x785F: //69 0D AA
    //clockticks+=3;
    ADC(0x0D);


//L_7861:
case 0x7861: //AA BD D2
clockticks+=29; //SUM
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x7862: //BD D2 56
    //clockticks+=4;
    temp=0x56D2; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7865: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7867: //C8 BD D3
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7868: //BD D3 56
    //clockticks+=4;
    temp=0x56D3; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x786B: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x786D: //C8 A2 00
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x786E: //A2 00 60
    //clockticks+=3;
    X=0x00;
  ZN=X;


//L_7870:
case 0x7870: //60 64 B6
clockticks+=0; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7870,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7A93:
case 0x7A93: //A2 02 BD
clockticks+=3; //SUM
    //clockticks+=3;
    X=0x02;
  ZN=X;


//L_7A95:
case 0x7A95: //BD 00 24
clockticks+=13; //SUM
    //clockticks+=4;
    temp=0x2400; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7A98: //0A B5 7A
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7A99: //B5 7A 29
    //clockticks+=4;
    temp=0x007A; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x7A9B: //29 1F 90
    //clockticks+=3;
    A &= 0x1F;
  ZN=A;


//L_7A9D:
case 0x7A9D: //90 37 F0
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7A9D,0x90);
        PCSTART=0x7AD6;
        /**/return;
        //goto L_7AD6;
    } 


//L_7A9F:
case 0x7A9F: //F0 10 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7A9F,0xF0);
        PCSTART=0x7AB1;
        /**/return;
        //goto L_7AB1;
    } 
//case 0x7AA1: //C9 1B B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x1B;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7AA3:
case 0x7AA3: //B0 0A A8
clockticks+=15; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7AA3,0xB0);
        PCSTART=0x7AAF;
        /**/return;
        //goto L_7AAF;
    } 
//case 0x7AA5: //A8 A5 5E
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x7AA6: //A5 5E 29
    //clockticks+=3;
    A=ReadMemory(0x5E); 
//  ZN=A;
//case 0x7AA8: //29 07 C9
    //clockticks+=3;
    A &= 0x07;
//  ZN=A;
//case 0x7AAA: //C9 07 98
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x07;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
//  ZN=value;
//case 0x7AAC: //98 90 02
    //clockticks+=2;
    A = Y;
  ZN=A;


//L_7AAD:
case 0x7AAD: //90 02 E9
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7AAD,0x90);
        PCSTART=0x7AB1;
        /**/return;
        //goto L_7AB1;
    } 


//L_7AAF:
case 0x7AAF: //E9 01 95
clockticks+=3; //SUM
    //clockticks+=3;
    SBC(0x01);


//L_7AB1:
case 0x7AB1: //95 7A AD
clockticks+=11; //SUM
    //clockticks+=4;
    temp=0x7A; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x7AB3: //AD 06 20
    //clockticks+=4;
    A=ReadMemory(0x2006); 
//  ZN=A;
//case 0x7AB6: //29 80 F0
    //clockticks+=3;
    A &= 0x80;
  ZN=A;


//L_7AB8:
case 0x7AB8: //F0 04 A9
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7AB8,0xF0);
        PCSTART=0x7ABE;
        /**/return;
        //goto L_7ABE;
    } 
//case 0x7ABA: //A9 F0 85
    //clockticks+=3;
    A=0xF0;
  ZN=A;
//case 0x7ABC: //85 72 A5
    //clockticks+=2;
    WriteMemory(0x72,A);


//L_7ABE:
case 0x7ABE: //A5 72 F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x72); 
  ZN=A;


//L_7AC0:
case 0x7AC0: //F0 08 C6
clockticks+=18; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7AC0,0xF0);
        PCSTART=0x7ACA;
        /**/return;
        //goto L_7ACA;
    } 
//case 0x7AC2: //C6 72 A9
    //clockticks+=5;
    value=ReadMemory(0x72);
    value--;
    WriteMemory(0x72,value);
//  ZN=value;
//case 0x7AC4: //A9 00 95
    //clockticks+=3;
    A=0x00;
  ZN=A;
//case 0x7AC6: //95 7A 95
    //clockticks+=4;
    temp=0x7A; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x7AC8: //95 77 18
    //clockticks+=4;
    temp=0x77; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_7ACA:
case 0x7ACA: //18 B5 77
clockticks+=6; //SUM
    //clockticks+=2;
    C = 0;
//case 0x7ACB: //B5 77 F0
    //clockticks+=4;
    temp=0x0077; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_7ACD:
case 0x7ACD: //F0 23 D6
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7ACD,0xF0);
        PCSTART=0x7AF2;
        /**/return;
        //goto L_7AF2;
    } 
//case 0x7ACF: //D6 77 D0
    //clockticks+=6;
    temp=0x0077; temp+=X; temp&=0xFF;
    value=ReadMemory(temp);
    value--;
    WriteMemory(temp,value);
  ZN=value;


//L_7AD1:
case 0x7AD1: //D0 1F 38
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7AD1,0xD0);
        PCSTART=0x7AF2;
        /**/return;
        //goto L_7AF2;
    }
//case 0x7AD3: //38 B0 1C
    //clockticks+=2;
    C = 0x01;


//L_7AD4:
case 0x7AD4: //B0 1C C9
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7AD4,0xB0);
        PCSTART=0x7AF2;
        /**/return;
        //goto L_7AF2;
    } 


//L_7AD6:
case 0x7AD6: //C9 1B B0
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x1B;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7AD8:
case 0x7AD8: //B0 09 B5
clockticks+=9; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7AD8,0xB0);
        PCSTART=0x7AE3;
        /**/return;
        //goto L_7AE3;
    } 
//case 0x7ADA: //B5 7A 69
    //clockticks+=4;
    temp=0x007A; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x7ADC: //69 20 90
    //clockticks+=3;
    ADC(0x20);


//L_7ADE:
case 0x7ADE: //90 D1 F0
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7ADE,0x90);
        PCSTART=0x7AB1;
        /**/return;
        //goto L_7AB1;
    } 


//L_7AE0:
case 0x7AE0: //F0 01 18
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7AE0,0xF0);
        PCSTART=0x7AE3;
        /**/return;
        //goto L_7AE3;
    } 
//case 0x7AE2: //18 A9 1F
    //clockticks+=2;
    C = 0;


//L_7AE3:
case 0x7AE3: //A9 1F B0
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x1F;
  ZN=A;


//L_7AE5:
case 0x7AE5: //B0 CA 95
clockticks+=10; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7AE5,0xB0);
        PCSTART=0x7AB1;
        /**/return;
        //goto L_7AB1;
    } 
//case 0x7AE7: //95 7A B5
    //clockticks+=4;
    temp=0x7A; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x7AE9: //B5 77 F0
    //clockticks+=4;
    temp=0x0077; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_7AEB:
case 0x7AEB: //F0 01 38
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7AEB,0xF0);
        PCSTART=0x7AEE;
        /**/return;
        //goto L_7AEE;
    } 
//case 0x7AED: //38 A9 78
    //clockticks+=2;
    C = 0x01;


//L_7AEE:
case 0x7AEE: //A9 78 95
clockticks+=7; //SUM
    //clockticks+=3;
    A=0x78;
  ZN=A;
//case 0x7AF0: //95 77 90
    //clockticks+=4;
    temp=0x77; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_7AF2:
case 0x7AF2: //90 23 A9
clockticks+=8; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7AF2,0x90);
        PCSTART=0x7B17;
        /**/return;
        //goto L_7B17;
    } 
//case 0x7AF4: //A9 00 E0
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x7AF6: //E0 01 90
    //clockticks+=3;
    value = 0x01;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7AF8:
case 0x7AF8: //90 16 F0
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7AF8,0x90);
        PCSTART=0x7B10;
        /**/return;
        //goto L_7B10;
    } 


//L_7AFA:
case 0x7AFA: //F0 0C A5
clockticks+=12; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7AFA,0xF0);
        PCSTART=0x7B08;
        /**/return;
        //goto L_7B08;
    } 
//case 0x7AFC: //A5 71 29
    //clockticks+=3;
    A=ReadMemory(0x71); 
//  ZN=A;
//case 0x7AFE: //29 0C 4A
    //clockticks+=3;
    A &= 0x0C;
//  ZN=A;
//case 0x7B00: //4A 4A F0
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7B01: //4A F0 0C
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_7B02:
case 0x7B02: //F0 0C 69
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7B02,0xF0);
        PCSTART=0x7B10;
        /**/return;
        //goto L_7B10;
    } 
//case 0x7B04: //69 02 D0
    //clockticks+=3;
    ADC(0x02);


//L_7B06:
case 0x7B06: //D0 08 A5
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7B06,0xD0);
        PCSTART=0x7B10;
        /**/return;
        //goto L_7B10;
    }


//L_7B08:
case 0x7B08: //A5 71 29
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x71); 
//  ZN=A;
//case 0x7B0A: //29 10 F0
    //clockticks+=3;
    A &= 0x10;
  ZN=A;


//L_7B0C:
case 0x7B0C: //F0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7B0C,0xF0);
        PCSTART=0x7B10;
        /**/return;
        //goto L_7B10;
    } 
//case 0x7B0E: //A9 01 38
    //clockticks+=3;
    A=0x01;
  ZN=A;


//L_7B10:
case 0x7B10: //38 65 73
clockticks+=13; //SUM
    //clockticks+=2;
    C = 0x01;
//case 0x7B11: //65 73 85
    //clockticks+=3;
    ADC(ReadMemory(0x73));
//case 0x7B13: //85 73 F6
    //clockticks+=2;
    WriteMemory(0x73,A);
//case 0x7B15: //F6 74 CA
    //clockticks+=6;
    temp=0x0074; temp+=X; temp&=0xFF;
    value=ReadMemory(temp);
    value++;
    WriteMemory(temp,value);
  ZN=value;


//L_7B17:
case 0x7B17: //CA 30 03
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7B18:
case 0x7B18: //30 03 4C
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7B18,0x30);
        PCSTART=0x7B1D;
        /**/return;
        //goto L_7B1D;
    }


//L_7B1A:
case 0x7B1A: //4C 95 7A
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x7B1A,0x4C);
    PCSTART=0x7A95;
    /**/return;
    //goto L_7A95;


//L_7B1D:
case 0x7B1D: //A5 71 29
clockticks+=8; //SUM
    //clockticks+=3;
    A=ReadMemory(0x71); 
//  ZN=A;
//case 0x7B1F: //29 03 A8
    //clockticks+=3;
    A &= 0x03;
//  ZN=A;
//case 0x7B21: //A8 F0 12
    //clockticks+=2;
    Y=A;
  ZN=Y;


//L_7B22:
case 0x7B22: //F0 12 4A
clockticks+=15; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7B22,0xF0);
        PCSTART=0x7B36;
        /**/return;
        //goto L_7B36;
    } 
//case 0x7B24: //4A 69 00
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7B25: //69 00 49
    //clockticks+=3;
    ADC(0x00);
//case 0x7B27: //49 FF 38
    //clockticks+=3;
    A ^= 0xFF;
//  ZN=A;
//case 0x7B29: //38 65 73
    //clockticks+=2;
    C = 0x01;
//case 0x7B2A: //65 73 90
    //clockticks+=3;
    ADC(ReadMemory(0x73));


//L_7B2C:
case 0x7B2C: //90 0A C0
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7B2C,0x90);
        PCSTART=0x7B38;
        /**/return;
        //goto L_7B38;
    } 
//case 0x7B2E: //C0 02 B0
    //clockticks+=3;
    temp=Y; temp+=0x100; temp-=0x02;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7B30:
case 0x7B30: //B0 02 E6
clockticks+=7; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7B30,0xB0);
        PCSTART=0x7B34;
        /**/return;
        //goto L_7B34;
    } 
//case 0x7B32: //E6 70 E6
    //clockticks+=5;
    value=ReadMemory(0x70);
    value++;
    WriteMemory(0x70,value);
  ZN=value;


//L_7B34:
case 0x7B34: //E6 70 85
clockticks+=5; //SUM
    //clockticks+=5;
    value=ReadMemory(0x70);
    value++;
    WriteMemory(0x70,value);
  ZN=value;


//L_7B36:
case 0x7B36: //85 73 A5
clockticks+=2; //SUM
    //clockticks+=2;
    WriteMemory(0x73,A);


//L_7B38:
case 0x7B38: //A5 5E 4A
clockticks+=5; //SUM
    //clockticks+=3;
    A=ReadMemory(0x5E); 
//  ZN=A;
//case 0x7B3A: //4A B0 27
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_7B3B:
case 0x7B3B: //B0 27 A0
clockticks+=8; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7B3B,0xB0);
        PCSTART=0x7B64;
        /**/return;
        //goto L_7B64;
    } 
//case 0x7B3D: //A0 00 A2
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7B3F: //A2 02 B5
    //clockticks+=3;
    X=0x02;
  ZN=X;


//L_7B41:
case 0x7B41: //B5 74 F0
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0074; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_7B43:
case 0x7B43: //F0 09 C9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7B43,0xF0);
        PCSTART=0x7B4E;
        /**/return;
        //goto L_7B4E;
    } 
//case 0x7B45: //C9 10 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x10;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7B47:
case 0x7B47: //90 05 69
clockticks+=11; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7B47,0x90);
        PCSTART=0x7B4E;
        /**/return;
        //goto L_7B4E;
    } 
//case 0x7B49: //69 EF C8
    //clockticks+=3;
    ADC(0xEF);
//case 0x7B4B: //C8 95 74
    //clockticks+=2;
    Y++; 
  ZN=Y;
//case 0x7B4C: //95 74 CA
    //clockticks+=4;
    temp=0x74; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_7B4E:
case 0x7B4E: //CA 10 F0
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7B4F:
case 0x7B4F: //10 F0 98
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7B4F,0x10);
        PCSTART=0x7B41;
        /**/return;
        //goto L_7B41;
    }
//case 0x7B51: //98 D0 10
    //clockticks+=2;
    A = Y;
  ZN=A;


//L_7B52:
case 0x7B52: //D0 10 A2
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7B52,0xD0);
        PCSTART=0x7B64;
        /**/return;
        //goto L_7B64;
    }
//case 0x7B54: //A2 02 B5
    //clockticks+=3;
    X=0x02;
  ZN=X;


//L_7B56:
case 0x7B56: //B5 74 F0
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0074; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_7B58:
case 0x7B58: //F0 07 18
clockticks+=11; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7B58,0xF0);
        PCSTART=0x7B61;
        /**/return;
        //goto L_7B61;
    } 
//case 0x7B5A: //18 69 EF
    //clockticks+=2;
    C = 0;
//case 0x7B5B: //69 EF 95
    //clockticks+=3;
    ADC(0xEF);
//case 0x7B5D: //95 74 30
    //clockticks+=4;
    temp=0x74; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_7B5F:
case 0x7B5F: //30 03 CA
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7B5F,0x30);
        PCSTART=0x7B64;
        /**/return;
        //goto L_7B64;
    }


//L_7B61:
case 0x7B61: //CA 10 F2
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7B62:
case 0x7B62: //10 F2 60
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7B62,0x10);
        PCSTART=0x7B56;
        /**/return;
        //goto L_7B56;
    }


//L_7B64:
case 0x7B64: //60 48 98
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7B64,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7B65:
case 0x7B65: //48 98 48
clockticks+=23; //SUM
    //clockticks+=3;
    stack[S--]=A;
//case 0x7B66: //98 48 8A
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x7B67: //48 8A 48
    //clockticks+=3;
    stack[S--]=A;
//case 0x7B68: //8A 48 D8
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x7B69: //48 D8 AD
    //clockticks+=3;
    stack[S--]=A;
//case 0x7B6A: //D8 AD FF
    //clockticks+=2;
    DEC = 0;
//case 0x7B6B: //AD FF 01
    //clockticks+=4;
    A=ReadMemory(0x01FF); 
//  ZN=A;
//case 0x7B6E: //0D D0 01
    //clockticks+=4;
    A |= ReadMemory(0x01D0);
  ZN=A;


//L_7B71:
case 0x7B71: //D0 FE E6
clockticks+=13; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7B71,0xD0);
        PCSTART=0x7B71;
        /**/return;
        //goto L_7B71;
    }
//case 0x7B73: //E6 5E A5
    //clockticks+=5;
    value=ReadMemory(0x5E);
    value++;
    WriteMemory(0x5E,value);
//  ZN=value;
//case 0x7B75: //A5 5E 29
    //clockticks+=3;
    A=ReadMemory(0x5E); 
//  ZN=A;
//case 0x7B77: //29 03 D0
    //clockticks+=3;
    A &= 0x03;
  ZN=A;


//L_7B79:
case 0x7B79: //D0 08 E6
clockticks+=13; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7B79,0xD0);
        PCSTART=0x7B83;
        /**/return;
        //goto L_7B83;
    }
//case 0x7B7B: //E6 5B A5
    //clockticks+=5;
    value=ReadMemory(0x5B);
    value++;
    WriteMemory(0x5B,value);
//  ZN=value;
//case 0x7B7D: //A5 5B C9
    //clockticks+=3;
    A=ReadMemory(0x5B); 
//  ZN=A;
//case 0x7B7F: //C9 04 B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x04;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7B81:
case 0x7B81: //B0 FE 20
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7B81,0xB0);
        PCSTART=0x7B81;
        /**/return;
        //goto L_7B81;
    } 


//L_7B83:
case 0x7B83: //20 93 7A
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7B;
    stack[S--]=0x86;
    showme(0x7B83,0x20);
    PCSTART=0x7A93; //jsr
    /**/return;
    //goto L_7A93;


//L_7B86:
case 0x7B86: //A5 6F 29
clockticks+=9; //SUM
    //clockticks+=3;
    A=ReadMemory(0x6F); 
//  ZN=A;
//case 0x7B88: //29 C7 24
    //clockticks+=3;
    A &= 0xC7;
//  ZN=A;
//case 0x7B8A: //24 74 10
    //clockticks+=3;
    value=ReadMemory(0x74);
    V = value;
  ZN = value & 0x80;
  if((value & A)==0) ZN |= 0x02;


//L_7B8C:
case 0x7B8C: //10 02 09
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7B8C,0x10);
        PCSTART=0x7B90;
        /**/return;
        //goto L_7B90;
    }
//case 0x7B8E: //09 08 24
    //clockticks+=3;
    A |= 0x08;
  ZN=A;


//L_7B90:
case 0x7B90: //24 75 10
clockticks+=3; //SUM
    //clockticks+=3;
    value=ReadMemory(0x75);
    V = value;
  ZN = value & 0x80;
  if((value & A)==0) ZN |= 0x02;


//L_7B92:
case 0x7B92: //10 02 09
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7B92,0x10);
        PCSTART=0x7B96;
        /**/return;
        //goto L_7B96;
    }
//case 0x7B94: //09 10 24
    //clockticks+=3;
    A |= 0x10;
  ZN=A;


//L_7B96:
case 0x7B96: //24 76 10
clockticks+=3; //SUM
    //clockticks+=3;
    value=ReadMemory(0x76);
    V = value;
  ZN = value & 0x80;
  if((value & A)==0) ZN |= 0x02;


//L_7B98:
case 0x7B98: //10 02 09
clockticks+=5; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7B98,0x10);
        PCSTART=0x7B9C;
        /**/return;
        //goto L_7B9C;
    }
//case 0x7B9A: //09 20 85
    //clockticks+=3;
    A |= 0x20;
  ZN=A;


//L_7B9C:
case 0x7B9C: //85 6F 8D
clockticks+=9; //SUM
    //clockticks+=2;
    WriteMemory(0x6F,A);
//case 0x7B9E: //8D 00 32
    //clockticks+=4;
    WriteMemory3200(A);
//case 0x7BA1: //A5 72 F0
    //clockticks+=3;
    A=ReadMemory(0x72); 
  ZN=A;


//L_7BA3:
case 0x7BA3: //F0 04 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7BA3,0xF0);
        PCSTART=0x7BA9;
        /**/return;
        //goto L_7BA9;
    } 
//case 0x7BA5: //A9 80 D0
    //clockticks+=3;
    A=0x80;
  ZN=A;


//L_7BA7:
case 0x7BA7: //D0 0E A5
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7BA7,0xD0);
        PCSTART=0x7BB7;
        /**/return;
        //goto L_7BB7;
    }


//L_7BA9:
case 0x7BA9: //A5 68 F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x68); 
  ZN=A;


//L_7BAB:
case 0x7BAB: //F0 0A A5
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7BAB,0xF0);
        PCSTART=0x7BB7;
        /**/return;
        //goto L_7BB7;
    } 
//case 0x7BAD: //A5 5C 6A
    //clockticks+=3;
    A=ReadMemory(0x5C); 
//  ZN=A;
//case 0x7BAF: //6A 90 02
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
  ZN=A;


//L_7BB0:
case 0x7BB0: //90 02 C6
clockticks+=7; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7BB0,0x90);
        PCSTART=0x7BB4;
        /**/return;
        //goto L_7BB4;
    } 
//case 0x7BB2: //C6 68 6A
    //clockticks+=5;
    value=ReadMemory(0x68);
    value--;
    WriteMemory(0x68,value);
  ZN=value;


//L_7BB4:
case 0x7BB4: //6A 6A 6A
clockticks+=6; //SUM
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x7BB5: //6A 6A 8D
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x7BB6: //6A 8D 05
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
  ZN=A;


//L_7BB7:
case 0x7BB7: //8D 05 3C
clockticks+=20; //SUM
    //clockticks+=4;
    WriteMemory(0x3C05,A);
//case 0x7BBA: //68 AA 68
    //clockticks+=4;
    A=stack[++S];
//  ZN=A;
//case 0x7BBB: //AA 68 A8
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x7BBC: //68 A8 68
    //clockticks+=4;
    A=stack[++S];
//  ZN=A;
//case 0x7BBD: //A8 68 40
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x7BBE: //68 40 A9
    //clockticks+=4;
    A=stack[++S];
  ZN=A;


//L_7BBF:
case 0x7BBF: //40 A9 B0
clockticks+=6; //SUM
    //clockticks+=6;
    { unsigned short savepcstart;
    ZN=V=C=DEC=stack[++S];
    savepcstart=stack[++S];
    temp=stack[++S]; temp<<=8;
    savepcstart|=temp;
    showme(0x7BBF,0x40);
    PCSTART=savepcstart;//gotta have PCSTART after //showme
    }
    RTI=1;
    return;


//L_7BC0:
case 0x7BC0: //A9 B0 A0
clockticks+=20; //SUM
    //clockticks+=3;
    A=0xB0;
//  ZN=A;
//case 0x7BC2: //A0 00 91
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7BC4: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7BC6: //C8 91 02
    //clockticks+=2;
    Y++; 
  ZN=Y;
//case 0x7BC7: //91 02 D0
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);


//L_7BC9:
case 0x7BC9: //D0 6E 90
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7BC9,0xD0);
        PCSTART=0x7C39;
        /**/return;
        //goto L_7C39;
    }


//L_7BCB:
case 0x7BCB: //90 04 29
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7BCB,0x90);
        PCSTART=0x7BD1;
        /**/return;
        //goto L_7BD1;
    } 
//case 0x7BCD: //29 0F F0
    //clockticks+=3;
    A &= 0x0F;
  ZN=A;


//L_7BCF:
case 0x7BCF: //F0 05 29
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7BCF,0xF0);
        PCSTART=0x7BD6;
        /**/return;
        //goto L_7BD6;
    } 


//L_7BD1:
case 0x7BD1: //29 0F 18
clockticks+=8; //SUM
    //clockticks+=3;
    A &= 0x0F;
//  ZN=A;
//case 0x7BD3: //18 69 01
    //clockticks+=2;
    C = 0;
//case 0x7BD4: //69 01 08
    //clockticks+=3;
    ADC(0x01);


//L_7BD6:
case 0x7BD6: //08 0A A0
clockticks+=32; //SUM
    //clockticks+=3;
    if(ZN) ZN|=0x02;
    stack[S--]=(ZN&0x82)|(DEC&0x08)|(V&0x40)|(C&0x01);
//case 0x7BD7: //0A A0 00
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7BD8: //A0 00 AA
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7BDA: //AA BD D4
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x7BDB: //BD D4 56
    //clockticks+=4;
    temp=0x56D4; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7BDE: //91 02 BD
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7BE0: //BD D5 56
    //clockticks+=4;
    temp=0x56D5; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7BE3: //C8 91 02
    //clockticks+=2;
    Y++; 
  ZN=Y;
//case 0x7BE4: //91 02 20
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);


//L_7BE6:
case 0x7BE6: //20 39 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7B;
    stack[S--]=0xE9;
    showme(0x7BE6,0x20);
    PCSTART=0x7C39; //jsr
    /**/return;
    //goto L_7C39;


//L_7BE9:
case 0x7BE9: //28 60 4A
clockticks+=4; //SUM
    //clockticks+=4;
    ZN=C=DEC=V=stack[++S]; // | 0x20;
    ZN&=0x82;


//L_7BEA:
case 0x7BEA: //60 4A 29
clockticks+=0; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7BEA,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7BF0:
case 0x7BF0: //A0 01 91
clockticks+=23; //SUM
    //clockticks+=3;
    Y = 0x01; 
//  ZN=Y;
//case 0x7BF2: //91 02 88
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7BF4: //88 8A 6A
    //clockticks+=2;
    Y--;
//  ZN=Y;
//case 0x7BF5: //8A 6A 91
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x7BF6: //6A 91 02
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x7BF7: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7BF9: //C8 D0 3D
    //clockticks+=2;
    Y++; 
  ZN=Y;


//L_7BFA:
case 0x7BFA: //D0 3D 4A
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7BFA,0xD0);
        PCSTART=0x7C39;
        /**/return;
        //goto L_7C39;
    }


//L_7BFC:
case 0x7BFC: //4A 29 0F
clockticks+=8; //SUM
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7BFD: //29 0F 09
    //clockticks+=3;
    A &= 0x0F;
//  ZN=A;
//case 0x7BFF: //09 C0 D0
    //clockticks+=3;
    A |= 0xC0;
  ZN=A;


//L_7C01:
case 0x7C01: //D0 ED A0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7C01,0xD0);
        PCSTART=0x7BF0;
        /**/return;
        //goto L_7BF0;
    }


//L_7C03:
case 0x7C03: //A0 00 84
clockticks+=44; //SUM
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7C05: //84 05 84
    //clockticks+=2;
    WriteMemory(0x05,Y);
//case 0x7C07: //84 07 0A
    //clockticks+=2;
    WriteMemory(0x07,Y);
//case 0x7C09: //0A 26 05
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7C0A: //26 05 0A
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x05);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x05,value);
//  ZN=value;
//case 0x7C0C: //0A 26 05
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7C0D: //26 05 85
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x05);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x05,value);
//  ZN=value;
//case 0x7C0F: //85 04 8A
    //clockticks+=2;
    WriteMemory(0x04,A);
//case 0x7C11: //8A 0A 26
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x7C12: //0A 26 07
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7C13: //26 07 0A
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x07);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x07,value);
//  ZN=value;
//case 0x7C15: //0A 26 07
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7C16: //26 07 85
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x07);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x07,value);
//  ZN=value;
//case 0x7C18: //85 06 A2
    //clockticks+=2;
    WriteMemory(0x06,A);
//case 0x7C1A: //A2 04 B5
    //clockticks+=3;
    X=0x04;
  ZN=X;


//L_7C1C:
case 0x7C1C: //B5 02 A0
clockticks+=61; //SUM
    //clockticks+=4;
    temp=0x0002; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x7C1E: //A0 00 91
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7C20: //91 02 B5
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7C22: //B5 03 29
    //clockticks+=4;
    temp=0x0003; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x7C24: //29 0F 09
    //clockticks+=3;
    A &= 0x0F;
//  ZN=A;
//case 0x7C26: //09 A0 C8
    //clockticks+=3;
    A |= 0xA0;
//  ZN=A;
//case 0x7C28: //C8 91 02
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7C29: //91 02 B5
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7C2B: //B5 00 C8
    //clockticks+=4;
    temp=0x0000; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x7C2D: //C8 91 02
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7C2E: //91 02 B5
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7C30: //B5 01 29
    //clockticks+=4;
    temp=0x0001; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
//  ZN=A;
//case 0x7C32: //29 0F 05
    //clockticks+=3;
    A &= 0x0F;
//  ZN=A;
//case 0x7C34: //05 00 C8
    //clockticks+=3;
    A |= ReadMemory(0x00);
//  ZN=A;
//case 0x7C36: //C8 91 02
    //clockticks+=2;
    Y++; 
  ZN=Y;
//case 0x7C37: //91 02 98
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);


//L_7C39:
case 0x7C39: //98 38 65
clockticks+=9; //SUM
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x7C3A: //38 65 02
    //clockticks+=2;
    C = 0x01;
//case 0x7C3B: //65 02 85
    //clockticks+=3;
    ADC(ReadMemory(0x02));
//case 0x7C3D: //85 02 90
    //clockticks+=2;
    WriteMemory(0x02,A);


//L_7C3F:
case 0x7C3F: //90 02 E6
clockticks+=7; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7C3F,0x90);
        PCSTART=0x7C43;
        /**/return;
        //goto L_7C43;
    } 
//case 0x7C41: //E6 03 60
    //clockticks+=5;
    value=ReadMemory(0x03);
    value++;
    WriteMemory(0x03,value);
  ZN=value;


//L_7C43:
case 0x7C43: //60 A9 D0
clockticks+=0; //SUM
    //clockticks+=6;
    { unsigned short SAVEPCSTART;
    SAVEPCSTART=stack[++S];
    temp=stack[++S]; temp<<=8;
    SAVEPCSTART|=temp;
    showme(0x7C43,0x0060);
    PCSTART=SAVEPCSTART;
    }
    return;


//L_7C49:
case 0x7C49: //A5 05 C9
clockticks+=6; //SUM
    //clockticks+=3;
    A=ReadMemory(0x05); 
//  ZN=A;
//case 0x7C4B: //C9 80 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7C4D:
case 0x7C4D: //90 11 49
clockticks+=18; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7C4D,0x90);
        PCSTART=0x7C60;
        /**/return;
        //goto L_7C60;
    } 
//case 0x7C4F: //49 FF 85
    //clockticks+=3;
    A ^= 0xFF;
//  ZN=A;
//case 0x7C51: //85 05 A5
    //clockticks+=2;
    WriteMemory(0x05,A);
//case 0x7C53: //A5 04 49
    //clockticks+=3;
    A=ReadMemory(0x04); 
//  ZN=A;
//case 0x7C55: //49 FF 69
    //clockticks+=3;
    A ^= 0xFF;
//  ZN=A;
//case 0x7C57: //69 00 85
    //clockticks+=3;
    ADC(0x00);
//case 0x7C59: //85 04 90
    //clockticks+=2;
    WriteMemory(0x04,A);


//L_7C5B:
case 0x7C5B: //90 02 E6
clockticks+=7; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7C5B,0x90);
        PCSTART=0x7C5F;
        /**/return;
        //goto L_7C5F;
    } 
//case 0x7C5D: //E6 05 38
    //clockticks+=5;
    value=ReadMemory(0x05);
    value++;
    WriteMemory(0x05,value);
  ZN=value;


//L_7C5F:
case 0x7C5F: //38 26 08
clockticks+=2; //SUM
    //clockticks+=2;
    C = 0x01;


//L_7C60:
case 0x7C60: //26 08 A5
clockticks+=11; //SUM
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x08);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x08,value);
//  ZN=value;
//case 0x7C62: //A5 07 C9
    //clockticks+=3;
    A=ReadMemory(0x07); 
//  ZN=A;
//case 0x7C64: //C9 80 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7C66:
case 0x7C66: //90 11 49
clockticks+=18; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7C66,0x90);
        PCSTART=0x7C79;
        /**/return;
        //goto L_7C79;
    } 
//case 0x7C68: //49 FF 85
    //clockticks+=3;
    A ^= 0xFF;
//  ZN=A;
//case 0x7C6A: //85 07 A5
    //clockticks+=2;
    WriteMemory(0x07,A);
//case 0x7C6C: //A5 06 49
    //clockticks+=3;
    A=ReadMemory(0x06); 
//  ZN=A;
//case 0x7C6E: //49 FF 69
    //clockticks+=3;
    A ^= 0xFF;
//  ZN=A;
//case 0x7C70: //69 00 85
    //clockticks+=3;
    ADC(0x00);
//case 0x7C72: //85 06 90
    //clockticks+=2;
    WriteMemory(0x06,A);


//L_7C74:
case 0x7C74: //90 02 E6
clockticks+=7; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7C74,0x90);
        PCSTART=0x7C78;
        /**/return;
        //goto L_7C78;
    } 
//case 0x7C76: //E6 07 38
    //clockticks+=5;
    value=ReadMemory(0x07);
    value++;
    WriteMemory(0x07,value);
  ZN=value;


//L_7C78:
case 0x7C78: //38 26 08
clockticks+=2; //SUM
    //clockticks+=2;
    C = 0x01;


//L_7C79:
case 0x7C79: //26 08 A5
clockticks+=11; //SUM
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x08);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x08,value);
//  ZN=value;
//case 0x7C7B: //A5 05 05
    //clockticks+=3;
    A=ReadMemory(0x05); 
//  ZN=A;
//case 0x7C7D: //05 07 F0
    //clockticks+=3;
    A |= ReadMemory(0x07);
  ZN=A;


//L_7C7F:
case 0x7C7F: //F0 0A A2
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7C7F,0xF0);
        PCSTART=0x7C8B;
        /**/return;
        //goto L_7C8B;
    } 
//case 0x7C81: //A2 00 C9
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x7C83: //C9 02 B0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x02;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7C85:
case 0x7C85: //B0 24 A0
clockticks+=5; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7C85,0xB0);
        PCSTART=0x7CAB;
        /**/return;
        //goto L_7CAB;
    } 
//case 0x7C87: //A0 01 D0
    //clockticks+=3;
    Y = 0x01; 
  ZN=Y;


//L_7C89:
case 0x7C89: //D0 10 A0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7C89,0xD0);
        PCSTART=0x7C9B;
        /**/return;
        //goto L_7C9B;
    }


//L_7C8B:
case 0x7C8B: //A0 02 A2
clockticks+=12; //SUM
    //clockticks+=3;
    Y = 0x02; 
//  ZN=Y;
//case 0x7C8D: //A2 09 A5
    //clockticks+=3;
    X=0x09;
//  ZN=X;
//case 0x7C8F: //A5 04 05
    //clockticks+=3;
    A=ReadMemory(0x04); 
//  ZN=A;
//case 0x7C91: //05 06 F0
    //clockticks+=3;
    A |= ReadMemory(0x06);
  ZN=A;


//L_7C93:
case 0x7C93: //F0 16 30
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7C93,0xF0);
        PCSTART=0x7CAB;
        /**/return;
        //goto L_7CAB;
    } 


//L_7C95:
case 0x7C95: //30 04 C8
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7C95,0x30);
        PCSTART=0x7C9B;
        /**/return;
        //goto L_7C9B;
    }


//L_7C97:
case 0x7C97: //C8 0A 10
clockticks+=4; //SUM
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7C98: //0A 10 FC
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
  ZN=A;


//L_7C99:
case 0x7C99: //10 FC 98
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7C99,0x10);
        PCSTART=0x7C97;
        /**/return;
        //goto L_7C97;
    }


//L_7C9B:
case 0x7C9B: //98 AA A5
clockticks+=7; //SUM
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x7C9C: //AA A5 05
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x7C9D: //A5 05 06
    //clockticks+=3;
    A=ReadMemory(0x05); 
  ZN=A;


//L_7C9F:
case 0x7C9F: //06 04 2A
clockticks+=19; //SUM
    //clockticks+=5;
    value = ReadMemory(0x04);
    C = value >> 7;
    value = value << 1;
    WriteMemory(0x04,value);
//  ZN=value;
//case 0x7CA1: //2A 06 06
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7CA2: //06 06 26
    //clockticks+=5;
    value = ReadMemory(0x06);
    C = value >> 7;
    value = value << 1;
    WriteMemory(0x06,value);
//  ZN=value;
//case 0x7CA4: //26 07 88
    //clockticks+=5;
    saveflags = C;
    value = ReadMemory(0x07);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x07,value);
//  ZN=value;
//case 0x7CA6: //88 D0 F6
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_7CA7:
case 0x7CA7: //D0 F6 85
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7CA7,0xD0);
        PCSTART=0x7C9F;
        /**/return;
        //goto L_7C9F;
    }
//case 0x7CA9: //85 05 8A
    //clockticks+=2;
    WriteMemory(0x05,A);


//L_7CAB:
case 0x7CAB: //8A 38 E9
clockticks+=92; //SUM
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x7CAC: //38 E9 0A
    //clockticks+=2;
    C = 0x01;
//case 0x7CAD: //E9 0A 49
    //clockticks+=3;
    SBC(0x0A);
//case 0x7CAF: //49 FF 0A
    //clockticks+=3;
    A ^= 0xFF;
//  ZN=A;
//case 0x7CB1: //0A 66 08
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7CB2: //66 08 2A
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x08);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x08,value);
//    ZN=value;
//case 0x7CB4: //2A 66 08
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7CB5: //66 08 2A
    //clockticks+=5;
    saveflags = C;
    value=ReadMemory(0x08);
    C = value;
    value = value >> 1;
    if ( saveflags & 0x01 ) value |= 0x80;
    WriteMemory(0x08,value);
//    ZN=value;
//case 0x7CB7: //2A 0A 85
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7CB8: //0A 85 08
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7CB9: //85 08 A0
    //clockticks+=2;
    WriteMemory(0x08,A);
//case 0x7CBB: //A0 00 A5
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7CBD: //A5 06 91
    //clockticks+=3;
    A=ReadMemory(0x06); 
//  ZN=A;
//case 0x7CBF: //91 02 A5
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7CC1: //A5 08 29
    //clockticks+=3;
    A=ReadMemory(0x08); 
//  ZN=A;
//case 0x7CC3: //29 F4 05
    //clockticks+=3;
    A &= 0xF4;
//  ZN=A;
//case 0x7CC5: //05 07 C8
    //clockticks+=3;
    A |= ReadMemory(0x07);
//  ZN=A;
//case 0x7CC7: //C8 91 02
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7CC8: //91 02 A5
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7CCA: //A5 04 C8
    //clockticks+=3;
    A=ReadMemory(0x04); 
//  ZN=A;
//case 0x7CCC: //C8 91 02
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7CCD: //91 02 A5
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7CCF: //A5 08 29
    //clockticks+=3;
    A=ReadMemory(0x08); 
//  ZN=A;
//case 0x7CD1: //29 02 0A
    //clockticks+=3;
    A &= 0x02;
//  ZN=A;
//case 0x7CD3: //0A 05 01
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7CD4: //05 01 05
    //clockticks+=3;
    A |= ReadMemory(0x01);
//  ZN=A;
//case 0x7CD6: //05 05 C8
    //clockticks+=3;
    A |= ReadMemory(0x05);
//  ZN=A;
//case 0x7CD8: //C8 91 02
    //clockticks+=2;
    Y++; 
  ZN=Y;
//case 0x7CD9: //91 02 4C
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);


//L_7CDB:
case 0x7CDB: //4C 39 7C
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x7CDB,0x4C);
    PCSTART=0x7C39;
    /**/return;
    //goto L_7C39;


//L_7CDE:
case 0x7CDE: //A2 00 A0
clockticks+=3; //SUM
    //clockticks+=3;
    X=0x00;
  ZN=X;


//L_7CE0:
case 0x7CE0: //A0 01 91
clockticks+=39; //SUM
    //clockticks+=3;
    Y = 0x01; 
//  ZN=Y;
//case 0x7CE2: //91 02 88
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7CE4: //88 98 91
    //clockticks+=2;
    Y--;
//  ZN=Y;
//case 0x7CE5: //98 91 02
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x7CE6: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7CE8: //C8 C8 91
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7CE9: //C8 91 02
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7CEA: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7CEC: //C8 8A 91
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7CED: //8A 91 02
    //clockticks+=2;
    A = X; 
  ZN=A;
//case 0x7CEE: //91 02 4C
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);


//L_7CF0:
case 0x7CF0: //4C 39 7C
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x7CF0,0x4C);
    PCSTART=0x7C39;
    /**/return;
    //goto L_7C39;


//L_7CF3:
case 0x7CF3: //A2 FE 9A
clockticks+=12; //SUM
    //clockticks+=3;
    X=0xFE;
//  ZN=X;
//case 0x7CF5: //9A D8 A9
    //clockticks+=2;
    S = X;
//case 0x7CF6: //D8 A9 00
    //clockticks+=2;
    DEC = 0;
//case 0x7CF7: //A9 00 AA
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x7CF9: //AA CA 9D
    //clockticks+=2;
    X=A;
  ZN=X;


//L_7CFA:
case 0x7CFA: //CA 9D 00
clockticks+=21; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;
//case 0x7CFB: //9D 00 03
    //clockticks+=5;
    temp=0x0300; temp+=X;
    WriteMemory(temp,A);
//case 0x7CFE: //9D 00 02
    //clockticks+=5;
    temp=0x0200; temp+=X;
    WriteMemory(temp,A);
//case 0x7D01: //9D 00 01
    //clockticks+=5;
    temp=0x0100; temp+=X;
    WriteMemory(temp,A);
//case 0x7D04: //95 00 D0
    //clockticks+=4;
    temp=0x00; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);


//L_7D06:
case 0x7D06: //D0 F2 AC
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7D06,0xD0);
        PCSTART=0x7CFA;
        /**/return;
        //goto L_7CFA;
    }
//case 0x7D08: //AC 07 20
    //clockticks+=4;
    Y=ReadMemory(0x2007);
  ZN=Y;


//L_7D0B:
case 0x7D0B: //30 43 E8
clockticks+=75; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7D0B,0x30);
        PCSTART=0x7D50;
        /**/return;
        //goto L_7D50;
    }
//case 0x7D0D: //E8 8E 00
    //clockticks+=2;
    X++; 
//  ZN=X;
//case 0x7D0E: //8E 00 40
    //clockticks+=4;
    WriteMemory(0x4000,X);
//case 0x7D11: //A9 E2 8D
    //clockticks+=3;
    A=0xE2;
//  ZN=A;
//case 0x7D13: //8D 01 40
    //clockticks+=4;
    WriteMemory(0x4001,A);
//case 0x7D16: //A9 B0 8D
    //clockticks+=3;
    A=0xB0;
//  ZN=A;
//case 0x7D18: //8D 03 40
    //clockticks+=4;
    WriteMemory(0x4003,A);
//case 0x7D1B: //85 32 85
    //clockticks+=2;
    WriteMemory(0x32,A);
//case 0x7D1D: //85 33 A9
    //clockticks+=2;
    WriteMemory(0x33,A);
//case 0x7D1F: //A9 03 85
    //clockticks+=3;
    A=0x03;
//  ZN=A;
//case 0x7D21: //85 6F 8D
    //clockticks+=2;
    WriteMemory(0x6F,A);
//case 0x7D23: //8D 00 32
    //clockticks+=4;
    WriteMemory3200(A);
//case 0x7D26: //2D 00 28
    //clockticks+=4;
    A &= ReadMemory(0x2800);
//  ZN=A;
//case 0x7D29: //85 71 AD
    //clockticks+=2;
    WriteMemory(0x71,A);
//case 0x7D2B: //AD 01 28
    //clockticks+=4;
    A=ReadMemory(0x2801); 
//  ZN=A;
//case 0x7D2E: //29 03 0A
    //clockticks+=3;
    A &= 0x03;
//  ZN=A;
//case 0x7D30: //0A 0A 05
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7D31: //0A 05 71
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7D32: //05 71 85
    //clockticks+=3;
    A |= ReadMemory(0x71);
//  ZN=A;
//case 0x7D34: //85 71 AD
    //clockticks+=2;
    WriteMemory(0x71,A);
//case 0x7D36: //AD 02 28
    //clockticks+=4;
    A=ReadMemory(0x2802); 
//  ZN=A;
//case 0x7D39: //29 02 0A
    //clockticks+=3;
    A &= 0x02;
//  ZN=A;
//case 0x7D3B: //0A 0A 0A
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7D3C: //0A 0A 05
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7D3D: //0A 05 71
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7D3E: //05 71 85
    //clockticks+=3;
    A |= ReadMemory(0x71);
  ZN=A;
//case 0x7D40: //85 71 4C
    //clockticks+=2;
    WriteMemory(0x71,A);


//L_7D42:
case 0x7D42: //4C 03 68
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x7D42,0x4C);
    PCSTART=0x6803;
    /**/return;
    //goto L_6803;


//L_7D45:
case 0x7D45: //A0 00 91
clockticks+=19; //SUM
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7D47: //91 02 C8
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7D49: //C8 8A 91
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7D4A: //8A 91 02
    //clockticks+=2;
    A = X; 
  ZN=A;
//case 0x7D4B: //91 02 4C
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);


//L_7D4D:
case 0x7D4D: //4C 39 7C
clockticks+=3; //SUM
    //clockticks+=3;
    showme(0x7D4D,0x4C);
    PCSTART=0x7C39;
    /**/return;
    //goto L_7C39;


//L_7D50:
case 0x7D50: //9D 00 40
clockticks+=42; //SUM
    //clockticks+=5;
    temp=0x4000; temp+=X;
    WriteMemory(temp,A);
//case 0x7D53: //9D 00 41
    //clockticks+=5;
    temp=0x4100; temp+=X;
    WriteMemory(temp,A);
//case 0x7D56: //9D 00 42
    //clockticks+=5;
    temp=0x4200; temp+=X;
    WriteMemory(temp,A);
//case 0x7D59: //9D 00 43
    //clockticks+=5;
    temp=0x4300; temp+=X;
    WriteMemory(temp,A);
//case 0x7D5C: //9D 00 44
    //clockticks+=5;
    temp=0x4400; temp+=X;
    WriteMemory(temp,A);
//case 0x7D5F: //9D 00 45
    //clockticks+=5;
    temp=0x4500; temp+=X;
    WriteMemory(temp,A);
//case 0x7D62: //9D 00 46
    //clockticks+=5;
    temp=0x4600; temp+=X;
    WriteMemory(temp,A);
//case 0x7D65: //9D 00 47
    //clockticks+=5;
    temp=0x4700; temp+=X;
    WriteMemory(temp,A);
//case 0x7D68: //E8 D0 E5
    //clockticks+=2;
    X++; 
  ZN=X;


//L_7D69:
case 0x7D69: //D0 E5 8D
clockticks+=9; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7D69,0xD0);
        PCSTART=0x7D50;
        /**/return;
        //goto L_7D50;
    }
//case 0x7D6B: //8D 00 34
    //clockticks+=4;
    WriteMemory(0x3400,A);
//case 0x7D6E: //A2 00 B5
    //clockticks+=3;
    X=0x00;
  ZN=X;


//L_7D70:
case 0x7D70: //B5 00 D0
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x0000; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_7D72:
case 0x7D72: //D0 47 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7D72,0xD0);
        PCSTART=0x7DBB;
        /**/return;
        //goto L_7DBB;
    }
//case 0x7D74: //A9 11 95
    //clockticks+=3;
    A=0x11;
  ZN=A;


//L_7D76:
case 0x7D76: //95 00 A8
clockticks+=10; //SUM
    //clockticks+=4;
    temp=0x00; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x7D78: //A8 55 00
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x7D79: //55 00 D0
    //clockticks+=4;
    temp=0x00; temp+=X; temp&=0xFF;
    A ^= ReadMemory(temp); 
  ZN=A;


//L_7D7B:
case 0x7D7B: //D0 3E 98
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7D7B,0xD0);
        PCSTART=0x7DBB;
        /**/return;
        //goto L_7DBB;
    }
//case 0x7D7D: //98 0A 90
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x7D7E: //0A 90 F5
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
  ZN=A;


//L_7D7F:
case 0x7D7F: //90 F5 E8
clockticks+=4; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7D7F,0x90);
        PCSTART=0x7D76;
        /**/return;
        //goto L_7D76;
    } 
//case 0x7D81: //E8 D0 EC
    //clockticks+=2;
    X++; 
  ZN=X;


//L_7D82:
case 0x7D82: //D0 EC 8D
clockticks+=12; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7D82,0xD0);
        PCSTART=0x7D70;
        /**/return;
        //goto L_7D70;
    }
//case 0x7D84: //8D 00 34
    //clockticks+=4;
    WriteMemory(0x3400,A);
//case 0x7D87: //8A 85 00
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x7D88: //85 00 2A
    //clockticks+=2;
    WriteMemory(0x00,A);
//case 0x7D8A: //2A 85 01
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
  ZN=A;


//L_7D8B:
case 0x7D8B: //85 01 A0
clockticks+=5; //SUM
    //clockticks+=2;
    WriteMemory(0x01,A);
//case 0x7D8D: //A0 00 A2
    //clockticks+=3;
    Y = 0x00; 
  ZN=Y;


//L_7D8F:
case 0x7D8F: //A2 11 B1
clockticks+=8; //SUM
    //clockticks+=3;
    X=0x11;
//  ZN=X;
//case 0x7D91: //B1 00 D0
    //clockticks+=5;
    temp=ReadMemory(0x01);
    temp<<=8;
    temp|=ReadMemory(0x00);
    temp=temp+Y;
    A=ReadMemory(temp);
  ZN=A;


//L_7D93:
case 0x7D93: //D0 2A 8A
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7D93,0xD0);
        PCSTART=0x7DBF;
        /**/return;
        //goto L_7DBF;
    }


//L_7D95:
case 0x7D95: //8A 91 00
clockticks+=13; //SUM
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x7D96: //91 00 51
    //clockticks+=6;
    temp=ReadMemory(0x01);
    temp<<=8;
    temp|=ReadMemory(0x00);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7D98: //51 00 D0
    //clockticks+=5;
    temp=ReadMemory(0x01);
    temp<<=8;
    temp|=ReadMemory(0x00);
    temp+=Y;
    A ^= ReadMemory(temp); 
  ZN=A;


//L_7D9A:
case 0x7D9A: //D0 23 8A
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7D9A,0xD0);
        PCSTART=0x7DBF;
        /**/return;
        //goto L_7DBF;
    }
//case 0x7D9C: //8A 0A AA
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x7D9D: //0A AA 90
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7D9E: //AA 90 F4
    //clockticks+=2;
    X=A;
  ZN=X;


//L_7D9F:
case 0x7D9F: //90 F4 C8
clockticks+=4; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7D9F,0x90);
        PCSTART=0x7D95;
        /**/return;
        //goto L_7D95;
    } 
//case 0x7DA1: //C8 D0 EB
    //clockticks+=2;
    Y++; 
  ZN=Y;


//L_7DA2:
case 0x7DA2: //D0 EB 8D
clockticks+=17; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7DA2,0xD0);
        PCSTART=0x7D8F;
        /**/return;
        //goto L_7D8F;
    }
//case 0x7DA4: //8D 00 34
    //clockticks+=4;
    WriteMemory(0x3400,A);
//case 0x7DA7: //E6 01 A6
    //clockticks+=5;
    value=ReadMemory(0x01);
    value++;
    WriteMemory(0x01,value);
//  ZN=value;
//case 0x7DA9: //A6 01 E0
    //clockticks+=3;
    X=ReadMemory(0x01); 
//  ZN=X;
//case 0x7DAB: //E0 04 90
    //clockticks+=3;
    value = 0x04;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7DAD:
case 0x7DAD: //90 E0 A9
clockticks+=8; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7DAD,0x90);
        PCSTART=0x7D8F;
        /**/return;
        //goto L_7D8F;
    } 
//case 0x7DAF: //A9 40 E0
    //clockticks+=3;
    A=0x40;
//  ZN=A;
//case 0x7DB1: //E0 40 90
    //clockticks+=3;
    value = 0x40;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7DB3:
case 0x7DB3: //90 D6 E0
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7DB3,0x90);
        PCSTART=0x7D8B;
        /**/return;
        //goto L_7D8B;
    } 
//case 0x7DB5: //E0 48 90
    //clockticks+=3;
    value = 0x48;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7DB7:
case 0x7DB7: //90 D6 B0
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7DB7,0x90);
        PCSTART=0x7D8F;
        /**/return;
        //goto L_7D8F;
    } 


//L_7DB9:
case 0x7DB9: //B0 69 A0
clockticks+=2; //SUM
    //clockticks+=2;
    if(C & 0x01)
    { 
        clockticks++;  
        showme(0x7DB9,0xB0);
        PCSTART=0x7E24;
        /**/return;
        //goto L_7E24;
    } 


//L_7DBB:
case 0x7DBB: //A0 00 F0
clockticks+=3; //SUM
    //clockticks+=3;
    Y = 0x00; 
  ZN=Y;


//L_7DBD:
case 0x7DBD: //F0 0E A0
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7DBD,0xF0);
        PCSTART=0x7DCD;
        /**/return;
        //goto L_7DCD;
    } 


//L_7DBF:
case 0x7DBF: //A0 00 A6
clockticks+=9; //SUM
    //clockticks+=3;
    Y = 0x00; 
//  ZN=Y;
//case 0x7DC1: //A6 01 E0
    //clockticks+=3;
    X=ReadMemory(0x01); 
//  ZN=X;
//case 0x7DC3: //E0 04 90
    //clockticks+=3;
    value = 0x04;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7DC5:
case 0x7DC5: //90 06 C8
clockticks+=7; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7DC5,0x90);
        PCSTART=0x7DCD;
        /**/return;
        //goto L_7DCD;
    } 
//case 0x7DC7: //C8 E0 44
    //clockticks+=2;
    Y++; 
//  ZN=Y;
//case 0x7DC8: //E0 44 90
    //clockticks+=3;
    value = 0x44;
    temp=X; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF;
  ZN=value;


//L_7DCA:
case 0x7DCA: //90 01 C8
clockticks+=4; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7DCA,0x90);
        PCSTART=0x7DCD;
        /**/return;
        //goto L_7DCD;
    } 
//case 0x7DCC: //C8 C9 10
    //clockticks+=2;
    Y++; 
  ZN=Y;


//L_7DCD:
case 0x7DCD: //C9 10 2A
clockticks+=16; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x10;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
//  ZN=value;
//case 0x7DCF: //2A 29 1F
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7DD0: //29 1F C9
    //clockticks+=3;
    A &= 0x1F;
//  ZN=A;
//case 0x7DD2: //C9 02 2A
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x02;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
//  ZN=value;
//case 0x7DD4: //2A 29 03
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7DD5: //29 03 88
    //clockticks+=3;
    A &= 0x03;
  ZN=A;


//L_7DD7:
case 0x7DD7: //88 30 04
clockticks+=2; //SUM
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_7DD8:
case 0x7DD8: //30 04 0A
clockticks+=6; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7DD8,0x30);
        PCSTART=0x7DDE;
        /**/return;
        //goto L_7DDE;
    }
//case 0x7DDA: //0A 0A 90
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
//  ZN=A;
//case 0x7DDB: //0A 90 F9
    //clockticks+=2;
    C = (A >> 7);
    A = A << 1;
  ZN=A;


//L_7DDC:
case 0x7DDC: //90 F9 4A
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7DDC,0x90);
        PCSTART=0x7DD7;
        /**/return;
        //goto L_7DD7;
    } 


//L_7DDE:
case 0x7DDE: //4A A2 14
clockticks+=5; //SUM
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7DDF: //A2 14 90
    //clockticks+=3;
    X=0x14;
  ZN=X;


//L_7DE1:
case 0x7DE1: //90 02 A2
clockticks+=5; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7DE1,0x90);
        PCSTART=0x7DE5;
        /**/return;
        //goto L_7DE5;
    } 
//case 0x7DE3: //A2 1D 8E
    //clockticks+=3;
    X=0x1D;
  ZN=X;


//L_7DE5:
case 0x7DE5: //8E 00 3A
clockticks+=10; //SUM
    //clockticks+=4;
    WriteMemory(0x3A00,X);
//case 0x7DE8: //A2 00 A0
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x7DEA: //A0 08 2C
    //clockticks+=3;
    Y = 0x08; 
  ZN=Y;


//L_7DEC:
case 0x7DEC: //2C 01 20
clockticks+=4; //SUM
    //clockticks+=4;
    value=ReadMemory(0x2001);
    V = value;
  ZN = value & 0x80;
  if((value&A)==0) ZN |= 0x02;


//L_7DEF:
case 0x7DEF: //10 FB 2C
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7DEF,0x10);
        PCSTART=0x7DEC;
        /**/return;
        //goto L_7DEC;
    }


//L_7DF1:
case 0x7DF1: //2C 01 20
clockticks+=4; //SUM
    //clockticks+=4;
    value=ReadMemory(0x2001);
    V = value;
  ZN = value & 0x80;
  if((value&A)==0) ZN |= 0x02;


//L_7DF4:
case 0x7DF4: //30 FB CA
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7DF4,0x30);
        PCSTART=0x7DF1;
        /**/return;
        //goto L_7DF1;
    }
//case 0x7DF6: //CA 8D 00
    //clockticks+=2;
    X--; 
  ZN=X;
//case 0x7DF7: //8D 00 34
    //clockticks+=4;
    WriteMemory(0x3400,A);


//L_7DFA:
case 0x7DFA: //D0 F0 88
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7DFA,0xD0);
        PCSTART=0x7DEC;
        /**/return;
        //goto L_7DEC;
    }
//case 0x7DFC: //88 D0 ED
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_7DFD:
case 0x7DFD: //D0 ED 8E
clockticks+=9; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7DFD,0xD0);
        PCSTART=0x7DEC;
        /**/return;
        //goto L_7DEC;
    }
//case 0x7DFF: //8E 00 3A
    //clockticks+=4;
    WriteMemory(0x3A00,X);
//case 0x7E02: //A0 08 2C
    //clockticks+=3;
    Y = 0x08; 
  ZN=Y;


//L_7E04:
case 0x7E04: //2C 01 20
clockticks+=4; //SUM
    //clockticks+=4;
    value=ReadMemory(0x2001);
    V = value;
  ZN = value & 0x80;
  if((value&A)==0) ZN |= 0x02;


//L_7E07:
case 0x7E07: //10 FB 2C
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7E07,0x10);
        PCSTART=0x7E04;
        /**/return;
        //goto L_7E04;
    }


//L_7E09:
case 0x7E09: //2C 01 20
clockticks+=4; //SUM
    //clockticks+=4;
    value=ReadMemory(0x2001);
    V = value;
  ZN = value & 0x80;
  if((value&A)==0) ZN |= 0x02;


//L_7E0C:
case 0x7E0C: //30 FB CA
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7E0C,0x30);
        PCSTART=0x7E09;
        /**/return;
        //goto L_7E09;
    }
//case 0x7E0E: //CA 8D 00
    //clockticks+=2;
    X--; 
  ZN=X;
//case 0x7E0F: //8D 00 34
    //clockticks+=4;
    WriteMemory(0x3400,A);


//L_7E12:
case 0x7E12: //D0 F0 88
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7E12,0xD0);
        PCSTART=0x7E04;
        /**/return;
        //goto L_7E04;
    }
//case 0x7E14: //88 D0 ED
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_7E15:
case 0x7E15: //D0 ED AA
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7E15,0xD0);
        PCSTART=0x7E04;
        /**/return;
        //goto L_7E04;
    }
//case 0x7E17: //AA D0 C4
    //clockticks+=2;
    X=A;
  ZN=X;


//L_7E18:
case 0x7E18: //D0 C4 8D
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7E18,0xD0);
        PCSTART=0x7DDE;
        /**/return;
        //goto L_7DDE;
    }


//L_7E1A:
case 0x7E1A: //8D 00 34
clockticks+=8; //SUM
    //clockticks+=4;
    WriteMemory(0x3400,A);
//case 0x7E1D: //AD 07 20
    //clockticks+=4;
    A=ReadMemory(0x2007); 
  ZN=A;


//L_7E20:
case 0x7E20: //30 F8 10
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7E20,0x30);
        PCSTART=0x7E1A;
        /**/return;
        //goto L_7E1A;
    }


//L_7E22:
case 0x7E22: //10 FE A9
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7E22,0x10);
        PCSTART=0x7E22;
        /**/return;
        //goto L_7E22;
    }


//L_7E24:
case 0x7E24: //A9 00 A8
clockticks+=12; //SUM
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x7E26: //A8 AA 85
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x7E27: //AA 85 08
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x7E28: //85 08 A9
    //clockticks+=2;
    WriteMemory(0x08,A);
//case 0x7E2A: //A9 50 85
    //clockticks+=3;
    A=0x50;
  ZN=A;


//L_7E2C:
case 0x7E2C: //85 09 A9
clockticks+=10; //SUM
    //clockticks+=2;
    WriteMemory(0x09,A);
//case 0x7E2E: //A9 04 85
    //clockticks+=3;
    A=0x04;
//  ZN=A;
//case 0x7E30: //85 0B A9
    //clockticks+=2;
    WriteMemory(0x0B,A);
//case 0x7E32: //A9 FF 51
    //clockticks+=3;
    A=0xFF;
  ZN=A;


//L_7E34:
case 0x7E34: //51 08 C8
clockticks+=7; //SUM
    //clockticks+=5;
    temp=ReadMemory(0x09);
    temp<<=8;
    temp|=ReadMemory(0x08);
    temp+=Y;
    A ^= ReadMemory(temp); 
//  ZN=A;
//case 0x7E36: //C8 D0 FB
    //clockticks+=2;
    Y++; 
  ZN=Y;


//L_7E37:
case 0x7E37: //D0 FB E6
clockticks+=12; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7E37,0xD0);
        PCSTART=0x7E34;
        /**/return;
        //goto L_7E34;
    }
//case 0x7E39: //E6 09 C6
    //clockticks+=5;
    value=ReadMemory(0x09);
    value++;
    WriteMemory(0x09,value);
//  ZN=value;
//case 0x7E3B: //C6 0B D0
    //clockticks+=5;
    value=ReadMemory(0x0B);
    value--;
    WriteMemory(0x0B,value);
  ZN=value;


//L_7E3D:
case 0x7E3D: //D0 F5 95
clockticks+=18; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7E3D,0xD0);
        PCSTART=0x7E34;
        /**/return;
        //goto L_7E34;
    }
//case 0x7E3F: //95 0D E8
    //clockticks+=4;
    temp=0x0D; temp+=X; temp&=0xFF;
    WriteMemory(temp,A);
//case 0x7E41: //E8 8D 00
    //clockticks+=2;
    X++; 
//  ZN=X;
//case 0x7E42: //8D 00 34
    //clockticks+=4;
    WriteMemory(0x3400,A);
//case 0x7E45: //A5 09 C9
    //clockticks+=3;
    A=ReadMemory(0x09); 
//  ZN=A;
//case 0x7E47: //C9 58 90
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x58;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7E49:
case 0x7E49: //90 E1 D0
clockticks+=2; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7E49,0x90);
        PCSTART=0x7E2C;
        /**/return;
        //goto L_7E2C;
    } 


//L_7E4B:
case 0x7E4B: //D0 02 A9
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7E4B,0xD0);
        PCSTART=0x7E4F;
        /**/return;
        //goto L_7E4F;
    }
//case 0x7E4D: //A9 68 C9
    //clockticks+=3;
    A=0x68;
  ZN=A;


//L_7E4F:
case 0x7E4F: //C9 80 90
clockticks+=3; //SUM
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x80;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7E51:
case 0x7E51: //90 D9 8D
clockticks+=22; //SUM
    //clockticks+=2;
    if ((C & 0x01)==0) 
    { 
        clockticks++;  
        showme(0x7E51,0x90);
        PCSTART=0x7E2C;
        /**/return;
        //goto L_7E2C;
    } 
//case 0x7E53: //8D 00 03
    //clockticks+=4;
    WriteMemory(0x0300,A);
//case 0x7E56: //A2 04 8E
    //clockticks+=3;
    X=0x04;
//  ZN=X;
//case 0x7E58: //8E 00 32
    //clockticks+=4;
    WriteMemory3200(X);
//case 0x7E5B: //86 15 A2
    //clockticks+=2;
    WriteMemory(0x15,X);
//case 0x7E5D: //A2 00 CD
    //clockticks+=3;
    X=0x00;
//  ZN=X;
//case 0x7E5F: //CD 00 02
    //clockticks+=4;
    value = ReadMemory(0x0200); 
    temp=A; temp+=0x100; temp-=value;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7E62:
case 0x7E62: //F0 01 E8
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7E62,0xF0);
        PCSTART=0x7E65;
        /**/return;
        //goto L_7E65;
    } 
//case 0x7E64: //E8 AD 00
    //clockticks+=2;
    X++; 
  ZN=X;


//L_7E65:
case 0x7E65: //AD 00 03
clockticks+=7; //SUM
    //clockticks+=4;
    A=ReadMemory(0x0300); 
//  ZN=A;
//case 0x7E68: //C9 88 F0
    //clockticks+=3;
    temp=A; temp+=0x100; temp-=0x88;
    if (temp>0xFF) C = 0x01; else C = 0x00;
    value=temp&0xFF; 
  ZN=value;


//L_7E6A:
case 0x7E6A: //F0 01 E8
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7E6A,0xF0);
        PCSTART=0x7E6D;
        /**/return;
        //goto L_7E6D;
    } 
//case 0x7E6C: //E8 86 16
    //clockticks+=2;
    X++; 
  ZN=X;


//L_7E6D:
case 0x7E6D: //86 16 A9
clockticks+=7; //SUM
    //clockticks+=2;
    WriteMemory(0x16,X);
//case 0x7E6F: //A9 10 85
    //clockticks+=3;
    A=0x10;
  ZN=A;
//case 0x7E71: //85 00 A2
    //clockticks+=2;
    WriteMemory(0x00,A);


//L_7E73:
case 0x7E73: //A2 24 AD
clockticks+=3; //SUM
    //clockticks+=3;
    X=0x24;
  ZN=X;


//L_7E75:
case 0x7E75: //AD 01 20
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2001); 
  ZN=A;


//L_7E78:
case 0x7E78: //10 FB AD
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7E78,0x10);
        PCSTART=0x7E75;
        /**/return;
        //goto L_7E75;
    }


//L_7E7A:
case 0x7E7A: //AD 01 20
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2001); 
  ZN=A;


//L_7E7D:
case 0x7E7D: //30 FB CA
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7E7D,0x30);
        PCSTART=0x7E7A;
        /**/return;
        //goto L_7E7A;
    }
//case 0x7E7F: //CA 10 F3
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7E80:
case 0x7E80: //10 F3 2C
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7E80,0x10);
        PCSTART=0x7E75;
        /**/return;
        //goto L_7E75;
    }


//L_7E82:
case 0x7E82: //2C 02 20
clockticks+=4; //SUM
    //clockticks+=4;
    value=ReadMemory(0x2002);
    V = value;
  ZN = value & 0x80;
  if((value&A)==0) ZN |= 0x02;


//L_7E85:
case 0x7E85: //30 FB 8D
clockticks+=20; //SUM
    //clockticks+=2;
    if(ZN&0x80)
    {
        clockticks++; 
        showme(0x7E85,0x30);
        PCSTART=0x7E82;
        /**/return;
        //goto L_7E82;
    }
//case 0x7E87: //8D 00 34
    //clockticks+=4;
    WriteMemory(0x3400,A);
//case 0x7E8A: //A9 00 85
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x7E8C: //85 02 A9
    //clockticks+=2;
    WriteMemory(0x02,A);
//case 0x7E8E: //A9 40 85
    //clockticks+=3;
    A=0x40;
//  ZN=A;
//case 0x7E90: //85 03 AD
    //clockticks+=2;
    WriteMemory(0x03,A);
//case 0x7E92: //AD 05 20
    //clockticks+=4;
    A=ReadMemory(0x2005); 
  ZN=A;


//L_7E95:
case 0x7E95: //10 5B A6
clockticks+=9; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7E95,0x10);
        PCSTART=0x7EF2;
        /**/return;
        //goto L_7EF2;
    }
//case 0x7E97: //A6 15 AD
    //clockticks+=3;
    X=ReadMemory(0x15); 
//  ZN=X;
//case 0x7E99: //AD 03 20
    //clockticks+=4;
    A=ReadMemory(0x2003); 
  ZN=A;


//L_7E9C:
case 0x7E9C: //10 0A 4D
clockticks+=6; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7E9C,0x10);
        PCSTART=0x7EA8;
        /**/return;
        //goto L_7EA8;
    }
//case 0x7E9E: //4D 09 00
    //clockticks+=4;
    A ^= ReadMemory(0x0009);
  ZN=A;


//L_7EA1:
case 0x7EA1: //10 05 CA
clockticks+=4; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7EA1,0x10);
        PCSTART=0x7EA8;
        /**/return;
        //goto L_7EA8;
    }
//case 0x7EA3: //CA F0 02
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7EA4:
case 0x7EA4: //F0 02 86
clockticks+=4; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7EA4,0xF0);
        PCSTART=0x7EA8;
        /**/return;
        //goto L_7EA8;
    } 
//case 0x7EA6: //86 15 BC
    //clockticks+=2;
    WriteMemory(0x15,X);


//L_7EA8:
case 0x7EA8: //BC BB 7E
clockticks+=17; //SUM
    //clockticks+=4;
    temp=0x7EBB; temp+=X;
    Y=ReadMemory(temp);
//  ZN=Y;
//case 0x7EAB: //A9 B0 91
    //clockticks+=3;
    A=0xB0;
//  ZN=A;
//case 0x7EAD: //91 02 88
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7EAF: //88 88 B9
    //clockticks+=2;
    Y--;
//  ZN=Y;
//case 0x7EB0: //88 B9 C0
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_7EB1:
case 0x7EB1: //B9 C0 7E
clockticks+=12; //SUM
    //clockticks+=4;
    temp=0x7EC0; temp+=Y;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7EB4: //91 02 88
    //clockticks+=6;
    temp=ReadMemory(0x03);
    temp<<=8;
    temp|=ReadMemory(0x02);
    temp+=Y;
    WriteMemory(temp,A);
//case 0x7EB6: //88 10 F8
    //clockticks+=2;
    Y--;
  ZN=Y;


//L_7EB7:
case 0x7EB7: //10 F8 4C
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7EB7,0x10);
        PCSTART=0x7EB1;
        /**/return;
        //goto L_7EB1;
    }


//L_7EB9:
case 0x7EB9: //4C 9D 7F
clockticks+=0; //SUM
    //clockticks+=3;
    showme(0x7EB9,0x4C);
    PCSTART=0x7F9D;
    /**/return;
    //goto L_7F9D;


//L_7EF2:
case 0x7EF2: //A9 50 A2
clockticks+=6; //SUM
    //clockticks+=3;
    A=0x50;
//  ZN=A;
//case 0x7EF4: //A2 00 20
    //clockticks+=3;
    X=0x00;
  ZN=X;


//L_7EF6:
case 0x7EF6: //20 FC 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7E;
    stack[S--]=0xF9;
    showme(0x7EF6,0x20);
    PCSTART=0x7BFC; //jsr
    /**/return;
    //goto L_7BFC;


//L_7EF9:
case 0x7EF9: //A9 69 A2
clockticks+=6; //SUM
    //clockticks+=3;
    A=0x69;
//  ZN=A;
//case 0x7EFB: //A2 93 20
    //clockticks+=3;
    X=0x93;
  ZN=X;


//L_7EFD:
case 0x7EFD: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x00;
    showme(0x7EFD,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_7F00:
case 0x7F00: //A9 30 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x30;
  ZN=A;


//L_7F02:
case 0x7F02: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x05;
    showme(0x7F02,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_7F05:
case 0x7F05: //A2 03 BD
clockticks+=3; //SUM
    //clockticks+=3;
    X=0x03;
  ZN=X;


//L_7F07:
case 0x7F07: //BD 00 28
clockticks+=9; //SUM
    //clockticks+=4;
    temp=0x2800; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7F0A: //29 01 86
    //clockticks+=3;
    A &= 0x01;
  ZN=A;
//case 0x7F0C: //86 0B 20
    //clockticks+=2;
    WriteMemory(0x0B,X);


//L_7F0E:
case 0x7F0E: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x11;
    showme(0x7F0E,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_7F11:
case 0x7F11: //A6 0B BD
clockticks+=12; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0B); 
//  ZN=X;
//case 0x7F13: //BD 00 28
    //clockticks+=4;
    temp=0x2800; temp+=X;
    A=ReadMemory(temp);
//  ZN=A;
//case 0x7F16: //29 02 4A
    //clockticks+=3;
    A &= 0x02;
//  ZN=A;
//case 0x7F18: //4A 20 D1
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_7F19:
case 0x7F19: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x1C;
    showme(0x7F19,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_7F1C:
case 0x7F1C: //A6 0B CA
clockticks+=5; //SUM
    //clockticks+=3;
    X=ReadMemory(0x0B); 
//  ZN=X;
//case 0x7F1E: //CA 10 E6
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7F1F:
case 0x7F1F: //10 E6 A9
clockticks+=8; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7F1F,0x10);
        PCSTART=0x7F07;
        /**/return;
        //goto L_7F07;
    }
//case 0x7F21: //A9 7A A2
    //clockticks+=3;
    A=0x7A;
//  ZN=A;
//case 0x7F23: //A2 9D 20
    //clockticks+=3;
    X=0x9D;
  ZN=X;


//L_7F25:
case 0x7F25: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x28;
    showme(0x7F25,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_7F28:
case 0x7F28: //A9 10 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x10;
  ZN=A;


//L_7F2A:
case 0x7F2A: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x2D;
    showme(0x7F2A,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_7F2D:
case 0x7F2D: //AD 02 28
clockticks+=12; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2802); 
//  ZN=A;
//case 0x7F30: //29 02 4A
    //clockticks+=3;
    A &= 0x02;
//  ZN=A;
//case 0x7F32: //4A 69 01
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7F33: //69 01 20
    //clockticks+=3;
    ADC(0x01);


//L_7F35:
case 0x7F35: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x38;
    showme(0x7F35,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_7F38:
case 0x7F38: //AD 01 28
clockticks+=13; //SUM
    //clockticks+=4;
    A=ReadMemory(0x2801); 
//  ZN=A;
//case 0x7F3B: //29 03 AA
    //clockticks+=3;
    A &= 0x03;
//  ZN=A;
//case 0x7F3D: //AA BD F5
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x7F3E: //BD F5 7F
    //clockticks+=4;
    temp=0x7FF5; temp+=X;
    A=ReadMemory(temp);
  ZN=A;


//L_7F41:
case 0x7F41: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x44;
    showme(0x7F41,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_7F44:
case 0x7F44: //A5 16 F0
clockticks+=3; //SUM
    //clockticks+=3;
    A=ReadMemory(0x16); 
  ZN=A;


//L_7F46:
case 0x7F46: //F0 07 A2
clockticks+=8; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7F46,0xF0);
        PCSTART=0x7F4F;
        /**/return;
        //goto L_7F4F;
    } 
//case 0x7F48: //A2 88 A9
    //clockticks+=3;
    X=0x88;
//  ZN=X;
//case 0x7F4A: //A9 50 20
    //clockticks+=3;
    A=0x50;
  ZN=A;


//L_7F4C:
case 0x7F4C: //20 FC 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x4F;
    showme(0x7F4C,0x20);
    PCSTART=0x7BFC; //jsr
    /**/return;
    //goto L_7BFC;


//L_7F4F:
case 0x7F4F: //A2 96 8E
clockticks+=10; //SUM
    //clockticks+=3;
    X=0x96;
//  ZN=X;
//case 0x7F51: //8E 0C 00
    //clockticks+=4;
    WriteMemory(0x000C,X);
//case 0x7F54: //A2 07 B5
    //clockticks+=3;
    X=0x07;
  ZN=X;


//L_7F56:
case 0x7F56: //B5 0D F0
clockticks+=4; //SUM
    //clockticks+=4;
    temp=0x000D; temp+=X; temp&=0xFF;
    A = ReadMemory(temp); 
  ZN=A;


//L_7F58:
case 0x7F58: //F0 37 48
clockticks+=27; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7F58,0xF0);
        PCSTART=0x7F91;
        /**/return;
        //goto L_7F91;
    } 
//case 0x7F5A: //48 8E 0B
    //clockticks+=3;
    stack[S--]=A;
//case 0x7F5B: //8E 0B 00
    //clockticks+=4;
    WriteMemory(0x000B,X);
//case 0x7F5E: //AE 0C 00
    //clockticks+=4;
    X=ReadMemory(0x000C);
//  ZN=X;
//case 0x7F61: //8A 38 E9
    //clockticks+=2;
    A = X; 
//  ZN=A;
//case 0x7F62: //38 E9 08
    //clockticks+=2;
    C = 0x01;
//case 0x7F63: //E9 08 8D
    //clockticks+=3;
    SBC(0x08);
//case 0x7F65: //8D 0C 00
    //clockticks+=4;
    WriteMemory(0x000C,A);
//case 0x7F68: //A9 20 20
    //clockticks+=3;
    A=0x20;
  ZN=A;


//L_7F6A:
case 0x7F6A: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x6D;
    showme(0x7F6A,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_7F6D:
case 0x7F6D: //A9 70 20
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x70;
  ZN=A;


//L_7F6F:
case 0x7F6F: //20 DE 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x72;
    showme(0x7F6F,0x20);
    PCSTART=0x7CDE; //jsr
    /**/return;
    //goto L_7CDE;


//L_7F72:
case 0x7F72: //AD 0B 00
clockticks+=4; //SUM
    //clockticks+=4;
    A=ReadMemory(0x000B); 
  ZN=A;


//L_7F75:
case 0x7F75: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x78;
    showme(0x7F75,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_7F78:
case 0x7F78: //AD D4 56
clockticks+=8; //SUM
    //clockticks+=4;
    A=0x2C; 
//  ZN=A;
//case 0x7F7B: //AE D5 56
    //clockticks+=4;
    X=0xCB;
  ZN=X;


//L_7F7E:
case 0x7F7E: //20 45 7D
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x81;
    showme(0x7F7E,0x20);
    PCSTART=0x7D45; //jsr
    /**/return;
    //goto L_7D45;


//L_7F81:
case 0x7F81: //68 48 4A
clockticks+=15; //SUM
    //clockticks+=4;
    A=stack[++S];
//  ZN=A;
//case 0x7F82: //48 4A 4A
    //clockticks+=3;
    stack[S--]=A;
//case 0x7F83: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7F84: //4A 4A 4A
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7F85: //4A 4A 20
    //clockticks+=2;
    C = A;
    A = A >> 1; 
//  ZN=A;
//case 0x7F86: //4A 20 D1
    //clockticks+=2;
    C = A;
    A = A >> 1; 
  ZN=A;


//L_7F87:
case 0x7F87: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x8A;
    showme(0x7F87,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_7F8A:
case 0x7F8A: //68 20 D1
clockticks+=4; //SUM
    //clockticks+=4;
    A=stack[++S];
  ZN=A;


//L_7F8B:
case 0x7F8B: //20 D1 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x8E;
    showme(0x7F8B,0x20);
    PCSTART=0x7BD1; //jsr
    /**/return;
    //goto L_7BD1;


//L_7F8E:
case 0x7F8E: //AE 0B 00
clockticks+=4; //SUM
    //clockticks+=4;
    X=ReadMemory(0x000B);
  ZN=X;


//L_7F91:
case 0x7F91: //CA 10 C2
clockticks+=2; //SUM
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7F92:
case 0x7F92: //10 C2 A9
clockticks+=7; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7F92,0x10);
        PCSTART=0x7F56;
        /**/return;
        //goto L_7F56;
    }
//case 0x7F94: //A9 7F AA
    //clockticks+=3;
    A=0x7F;
//  ZN=A;
//case 0x7F96: //AA 20 03
    //clockticks+=2;
    X=A;
  ZN=X;


//L_7F97:
case 0x7F97: //20 03 7C
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x9A;
    showme(0x7F97,0x20);
    PCSTART=0x7C03; //jsr
    /**/return;
    //goto L_7C03;


//L_7F9A:
case 0x7F9A: //20 C0 7B
clockticks+=6; //SUM
    //clockticks+=6;
    stack[S--]=0x7F;
    stack[S--]=0x9D;
    showme(0x7F9A,0x20);
    PCSTART=0x7BC0; //jsr
    /**/return;
    //goto L_7BC0;


//L_7F9D:
case 0x7F9D: //A9 00 A2
clockticks+=6; //SUM
    //clockticks+=3;
    A=0x00;
//  ZN=A;
//case 0x7F9F: //A2 04 3E
    //clockticks+=3;
    X=0x04;
  ZN=X;


//L_7FA1:
case 0x7FA1: //3E 03 20
clockticks+=11; //SUM
    //clockticks+=7;
    saveflags = C;
    temp=0x2003; temp+=X;
    value = ReadMemory(temp);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(temp,value);
//  ZN=value;
//case 0x7FA4: //6A CA 10
    //clockticks+=2;
    saveflags = C; 
    C = A;
    A = A >> 1;
    if ( saveflags & 0x01 ) A |= 0x80;
//  ZN=A;
//case 0x7FA5: //CA 10 F9
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7FA6:
case 0x7FA6: //10 F9 A8
clockticks+=7; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7FA6,0x10);
        PCSTART=0x7FA1;
        /**/return;
        //goto L_7FA1;
    }
//case 0x7FA8: //A8 A2 07
    //clockticks+=2;
    Y=A;
//  ZN=Y;
//case 0x7FA9: //A2 07 3E
    //clockticks+=3;
    X=0x07;
  ZN=X;


//L_7FAB:
case 0x7FAB: //3E 00 24
clockticks+=11; //SUM
    //clockticks+=7;
    saveflags = C;
    temp=0x2400; temp+=X;
    value = ReadMemory(temp);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(temp,value);
//  ZN=value;
//case 0x7FAE: //2A CA 10
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7FAF: //CA 10 F9
    //clockticks+=2;
    X--; 
  ZN=X;


//L_7FB0:
case 0x7FB0: //10 F9 AA
clockticks+=65; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7FB0,0x10);
        PCSTART=0x7FAB;
        /**/return;
        //goto L_7FAB;
    }
//case 0x7FB2: //AA 45 08
    //clockticks+=2;
    X=A;
//  ZN=X;
//case 0x7FB3: //45 08 86
    //clockticks+=3;
    A ^= ReadMemory(0x08);
//  ZN=A;
//case 0x7FB5: //86 08 08
    //clockticks+=2;
    WriteMemory(0x08,X);
//case 0x7FB7: //08 A9 04
    //clockticks+=3;
    if(ZN) ZN|=0x02;
    stack[S--]=(ZN&0x82)|(DEC&0x08)|(V&0x40)|(C&0x01);
//case 0x7FB8: //A9 04 8D
    //clockticks+=3;
    A=0x04;
//  ZN=A;
//case 0x7FBA: //8D 00 32
    //clockticks+=4;
    WriteMemory3200(A);
//case 0x7FBD: //2E 03 20
    //clockticks+=6;
    saveflags = C;
    value = ReadMemory(0x2003);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x2003,value);
//  ZN=value;
//case 0x7FC0: //2A 2E 04
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7FC1: //2E 04 20
    //clockticks+=6;
    saveflags = C;
    value = ReadMemory(0x2004);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x2004,value);
//  ZN=value;
//case 0x7FC4: //2A 2E 07
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7FC5: //2E 07 24
    //clockticks+=6;
    saveflags = C;
    value = ReadMemory(0x2407);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x2407,value);
//  ZN=value;
//case 0x7FC8: //2A 2E 06
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7FC9: //2E 06 24
    //clockticks+=6;
    saveflags = C;
    value = ReadMemory(0x2406);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x2406,value);
//  ZN=value;
//case 0x7FCC: //2A 2E 05
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7FCD: //2E 05 24
    //clockticks+=6;
    saveflags = C;
    value = ReadMemory(0x2405);
    C = value >> 7;
    value = value << 1;
    value |= (saveflags & 0x01);
    WriteMemory(0x2405,value);
//  ZN=value;
//case 0x7FD0: //2A AA 28
    //clockticks+=2;
    saveflags = C;
    C = A >> 7;
    A = A << 1;
    A |= (saveflags & 0x01);
//  ZN=A;
//case 0x7FD1: //AA 28 D0
    //clockticks+=2;
    X=A;
  ZN=X;
//case 0x7FD2: //28 D0 09
    //clockticks+=4;
    ZN=C=DEC=V=stack[++S]; // | 0x20;
    ZN&=0x82;


//L_7FD3:
case 0x7FD3: //D0 09 45
clockticks+=5; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7FD3,0xD0);
        PCSTART=0x7FDE;
        /**/return;
        //goto L_7FDE;
    }
//case 0x7FD5: //45 0A D0
    //clockticks+=3;
    A ^= ReadMemory(0x0A);
  ZN=A;


//L_7FD7:
case 0x7FD7: //D0 05 98
clockticks+=7; //SUM
    //clockticks+=2;
    if(ZN) //if not zero
    {
        clockticks++; 
        showme(0x7FD7,0xD0);
        PCSTART=0x7FDE;
        /**/return;
        //goto L_7FDE;
    }
//case 0x7FD9: //98 45 09
    //clockticks+=2;
    A = Y;
//  ZN=A;
//case 0x7FDA: //45 09 F0
    //clockticks+=3;
    A ^= ReadMemory(0x09);
  ZN=A;


//L_7FDC:
case 0x7FDC: //F0 02 A9
clockticks+=2; //SUM
    //clockticks+=2;
    if(ZN==0)//if zero
    { 
        clockticks++;  
        showme(0x7FDC,0xF0);
        PCSTART=0x7FE0;
        /**/return;
        //goto L_7FE0;
    } 


//L_7FDE:
case 0x7FDE: //A9 80 8D
clockticks+=3; //SUM
    //clockticks+=3;
    A=0x80;
  ZN=A;


//L_7FE0:
case 0x7FE0: //8D 05 3C
clockticks+=20; //SUM
    //clockticks+=4;
    WriteMemory(0x3C05,A);
//case 0x7FE3: //8D 00 32
    //clockticks+=4;
    WriteMemory3200(A);
//case 0x7FE6: //8D 00 30
    //clockticks+=4;
   if (dvg_draw_screen()) break;
//case 0x7FE9: //86 0A 84
    //clockticks+=2;
    WriteMemory(0x0A,X);
//case 0x7FEB: //84 09 AD
    //clockticks+=2;
    WriteMemory(0x09,Y);
//case 0x7FED: //AD 07 20
    //clockticks+=4;
    A=ReadMemory(0x2007); 
  ZN=A;


//L_7FF0:
case 0x7FF0: //10 FE 4C
clockticks+=2; //SUM
    //clockticks+=2;
    if((ZN&0x80)==0)
    {
        clockticks++;
        showme(0x7FF0,0x10);
        PCSTART=0x7FF0;
        /**/return;
        //goto L_7FF0;
    }


//L_7FF2:
case 0x7FF2: //4C 73 7E
clockticks+=0; //SUM
    //clockticks+=3;
    showme(0x7FF2,0x4C);
    PCSTART=0x7E73;
    /**/return;
    //goto L_7E73;
    }
}

