
// for now INI setting just stupidly overwrite other saved settings!
int iniHandler(void* user, const char* section, const char* name, const char* value)
{
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    #define MATCH_NAME(n) strcmp(name, n) == 0
  
    if (MATCH_NAME("YSH_A_DELAY")) YSH_A_DELAY = atoi(value); else 
    if (MATCH_NAME("YSH_B_DELAY")) YSH_B_DELAY = atoi(value); else 
    if (MATCH_NAME("XSH_A_DELAY")) XSH_A_DELAY = atoi(value); else 
    if (MATCH_NAME("XSH_B_DELAY")) XSH_B_DELAY = atoi(value); else 

    if (MATCH_NAME("DELAY_AFTER_XSH_VALUE")) DELAY_AFTER_XSH_VALUE = atoi(value); else 
    if (MATCH_NAME("DELAY_AFTER_YSH_VALUE")) DELAY_AFTER_YSH_VALUE = atoi(value); else 
    if (MATCH_NAME("DELAY_PORT_B_BEFORE_PORT_A_VALUE")) DELAY_PORT_B_BEFORE_PORT_A_VALUE = atoi(value); else 
    if (MATCH_NAME("CRANKY_DELAY_Y_TO_NULL_VALUE")) CRANKY_DELAY_Y_TO_NULL_VALUE = atoi(value); else 
    if (MATCH_NAME("DELAY_ZERO_VALUE")) DELAY_ZERO_VALUE = atoi(value); else 
    if (MATCH_NAME("DELAY_AFTER_T1_END_VALUE")) DELAY_AFTER_T1_END_VALUE = atoi(value); else 
    if (MATCH_NAME("SCALE_STRENGTH_DIF")) SCALE_STRENGTH_DIF = atoi(value); else 
    if (MATCH_NAME("MAX_USED_STRENGTH")) {MAX_USED_STRENGTH = atoi(value);} else 
    if (MATCH_NAME("RESET_TO_ZERO_DIF_MAX")) resetToZeroDifMax = atoi(value); else 
    if (MATCH_NAME("CRANKY_FLAG")) crankyFlag = atoi(value); else 
    if (MATCH_NAME("CYCLE_EQUIVALENT")) {setCycleEquivalent(atoi(value));printf("Cycles set to %i, %i", atoi(value), getCycleEquivalent());} else 
    if (MATCH_NAME("ORIENTATION")) orientation = atoi(value); else 
            
    if (MATCH_NAME("XCAL_VALUE")) softwareXCalibration = atof(value); else 
    if (MATCH_NAME("YCAL_VALUE")) softwareYCalibration = atof(value); else 
    if (MATCH_NAME("CALIBRATION_VALUE5")) calibrationValue_5 = atoi(value); else 
    if (MATCH_NAME("CALIBRATION_VALUE10")) calibrationValue_10 = atoi(value); else 
    if (MATCH_NAME("CALIBRATION_VALUE15")) calibrationValue_15 = atoi(value); else 
    if (MATCH_NAME("CALIBRATION_VALUE20")) calibrationValue_20 = atoi(value); else 
    if (MATCH_NAME("CALIBRATION_VALUE50")) calibrationValue_50 = atoi(value); else 
    if (MATCH_NAME("CALIBRATION_VALUE100")) calibrationValue_100 = atoi(value); else 
    if (MATCH_NAME("USE_PIPELINE")) usePipeline = atoi(value); else 
    if (MATCH_NAME("BUFFER_TYPE")) bufferType = atoi(value); else 
    if (MATCH_NAME("BEAM_OFF_BETWEEN_CONSECUTIVE_DRAWS")) beamOffBetweenConsecutiveDraws = atoi(value); else 
    if (MATCH_NAME("REFRESH_HZ")) v_setRefresh(atoi(value)); else  // refreh in Hz
    if (MATCH_NAME("SIZE_X")) sizeX = atof(value); else  // 1.0 is "normal", this is a float!
    if (MATCH_NAME("SIZE_Y")) sizeY = atof(value); else  // 1.0 is "normal", this is a float!
    if (MATCH_NAME("OFFSET_X")) offsetX = atoi(value); else  //positive -> right
    if (MATCH_NAME("OFFSET_Y")) offsetY = atoi(value); else  // positive up
    if (MATCH_NAME("LOG_OUTPUT")) logOutput = atoi(value); else  // if != 0, than write all "printf" also to filesystem!
    if (MATCH_NAME("UART_OUTPUT")) uartOutput = atoi(value); else  // if == 0, than no "printf" to serial
    if (MATCH_NAME("MINSCALE")) MINSCALE = atoi(value); else  // if != 0, than write all "printf" also to filesystem!
    if (MATCH_NAME("MAXSCALE")) MAXSCALE = atoi(value); else  // if != 0, than write all "printf" also to filesystem!

    if (MATCH_NAME("DOUBLE_TIMER")) useDoubleTimer = atoi(value); else  // default 0
    if (MATCH_NAME("KEEP_DOTS_ORDERED")) keepDotsTogether = atoi(value); else  // default 0
    if (MATCH_NAME("OPTIMIZATION")) optimizationON = atoi(value); else  

    if (MATCH_NAME("SWAP_JOYSTICK_PORTS")) swapPorts = atoi(value); else  

      
    if (MATCH_NAME("BRIGHTNESS_MUL")) intensityMul = atof(value); else  

      if (MATCH_NAME("BRIGHTNESS_MIN")) intensityMin = atoi(value); else  
    if (MATCH_NAME("BRIGHTNESS_MAX")) intensityMax = atoi(value); else  

    if (MATCH_NAME("ANGLE_OPTIMIZATION")) doAngleOptimization = atoi(value); else 
    if (MATCH_NAME("ANGLE_MIN_VALUE")) angleValue = atoi(value); else 

    if (MATCH_NAME("OVERLAPPING_OPTIMIZATION")) doOverlappingOptimization = atoi(value); else 
    if (MATCH_NAME("INVERT_Y_AXIS")) invertYAxis = atoi(value); else 
    if (MATCH_NAME("INI_OVERWRITE_ALLOWED")) iniOverwrite = atoi(value); else 
    if (MATCH_NAME("IN_GAME_SETTINGS")) inGameSettingsAllowed = atoi(value); else  
    {
        // since we use this with "cascades"
        // do not notify if a setting was not found!
        // printf("Ini-Handler, unkown entry: %s = %s", name, value);
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

typedef enum {
  CAL_TYPE_NONE = 0,
  CAL_TYPE_UCHAR = 1,
  CAL_TYPE_BOOL = 2,
  CAL_TYPE_UINT = 3,
  CAL_TYPE_SINT = 4,
  CAL_TYPE_FLOAT = 5
} CalType;

typedef struct {
  uint8_t *ucvalue;
  uint8_t *scvalue;
  unsigned int *uivalue;
  int16_t *sivalue;
  float *fvalue;
  int min;
  int max;
  int defaultValue;
  char *name;
  CalType type;
  char **help;
} CalibrationItems;

// pos in 8 bit (7f)
// width in 8 bit
// scale is changed within component
// text is 0 terminated
// reacts on current josytick input
int v_immediateWidgetSlider(CalibrationItems *item, int x, int y, int active)
{
  char buf[80];
  int width = 80;

  int min = item->min;
  int max = item->max;
  int resetValue = item->defaultValue;
  char *text = item->name;
  
  float pos;

  if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
    pos = ((float)width)/((float)(max-min)) * ((float)((*item->ucvalue)-min));
  else if(item->type == CAL_TYPE_UINT)
    pos = ((float)width)/((float)(max-min)) * ((float)((*item->uivalue)-min) );
  else if(item->type == CAL_TYPE_SINT)
    pos = ((float)width)/((float) (max-min) ) * ((float)((*item->sivalue) - min));
  else if(item->type == CAL_TYPE_FLOAT)
    pos = ((float)width)/((float) (max-min) ) * ((float)((*item->fvalue) - min));

  if (active)
  {
    v_setBrightness(0x5f);
    sprintf(buf, "%i", min);
    v_printStringRaster(x-10, y-10, buf, 50, -4, 0);
    sprintf(buf, "%i", max);
    v_printStringRaster(x+width-10, y-10, buf, 50, -4, 0);

    if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
      sprintf(buf, "%i", *item->ucvalue);
    else if(item->type == CAL_TYPE_UINT)
      sprintf(buf, "%i", *item->uivalue);
    else if(item->type == CAL_TYPE_SINT)
      sprintf(buf, "%i", *item->sivalue);
    else if(item->type == CAL_TYPE_FLOAT)
      sprintf(buf, "%1.5f", *item->fvalue);
    
    v_printStringRaster((int)(x+pos)-5, y+15, buf, 35, -2, 0);

    // attention - float!
    if ((internalButtonState&0x0f) == (0x06)) // button 2+3 
    {
      if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL)) *item->ucvalue=resetValue;
      else if(item->type == CAL_TYPE_UINT)  *item->uivalue=resetValue;
      else if(item->type == CAL_TYPE_SINT)  *item->sivalue=resetValue;
      else if(item->type == CAL_TYPE_FLOAT)  *item->fvalue=resetValue;
    }
    
    if ((internalButtonState &0xf) == 0)
    {
      if (valueChangeDelay>0) 
        valueChangeDelay--;
      if ((internalJoy1X==0) && (internalJoy1Y==0)) valueChangeDelay =0;
      if (valueChangeDelay<=0)
      {
        if (internalJoy1X>80)
        {
          if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
          {
            if (*item->ucvalue<max) *item->ucvalue=*item->ucvalue + 1;
          }
          else if (item->type == CAL_TYPE_UINT) 
          {
            if (*item->uivalue<max) *item->uivalue=*item->uivalue + 1;
          }
          else if(item->type == CAL_TYPE_SINT)  
          {
            if (*item->sivalue<max) *item->sivalue=*item->sivalue + 1;
          }
          else if(item->type == CAL_TYPE_FLOAT)  
          {
            if (*item->fvalue<max) *item->fvalue=*item->fvalue + 0.0001;
          }

          valueChangeDelay = 64-(internalJoy1X>>1);
        }
        if (internalJoy1X<-80)
        {
          if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
          {
            if (*item->ucvalue>min) *item->ucvalue=*item->ucvalue - 1;
          }
          else if (item->type == CAL_TYPE_UINT) 
          {
            if (*item->uivalue>min) *item->uivalue=*item->uivalue - 1;
          }
          else if(item->type == CAL_TYPE_SINT)  
          {
            if (*item->sivalue>min) *item->sivalue=*item->sivalue - 1;
          }
          else if(item->type == CAL_TYPE_FLOAT)  
          {
            if (*item->fvalue>min) *item->fvalue=*item->fvalue - 0.0001;
          }
          valueChangeDelay = 64+(internalJoy1X>>1);
        }
      }
    }
    else if ((internalButtonState &0xf) == 2)
    {
      if (valueChangeDelay>0) 
        valueChangeDelay--;
      if ((internalJoy1X==0) && (internalJoy1Y==0)) valueChangeDelay =0;
      if (valueChangeDelay<=0)
      {
        if (internalJoy1X>80)
        {
          if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
          {
            if (*item->ucvalue<max- 10) *item->ucvalue=*item->ucvalue + 10;
          }
          else if (item->type == CAL_TYPE_UINT) 
          {
            if (*item->uivalue<max- 10) *item->uivalue=*item->uivalue + 10;
          }
          else if(item->type == CAL_TYPE_SINT)  
          {
            if (*item->sivalue<max- 10) *item->sivalue=*item->sivalue + 10;
          }
          else if(item->type == CAL_TYPE_FLOAT)  
          {
            if (*item->fvalue<max- 0.1) *item->fvalue=*item->fvalue + 0.1;
          }

          valueChangeDelay = 64-(internalJoy1X>>1);
        }
        if (internalJoy1X<-80)
        {
          if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
          {
            if (*item->ucvalue>min+ 10) *item->ucvalue=*item->ucvalue - 10;
          }
          else if (item->type == CAL_TYPE_UINT) 
          {
            if (*item->uivalue>min+ 10) *item->uivalue=*item->uivalue - 10;
          }
          else if(item->type == CAL_TYPE_SINT)  
          {
            if (*item->sivalue>min+ 10) *item->sivalue=*item->sivalue - 10;
          }
          else if(item->type == CAL_TYPE_FLOAT)  
          {
            if (*item->fvalue>min+0.1) *item->fvalue=*item->fvalue - 0.1;
          }
          valueChangeDelay = 64+(internalJoy1X>>1);
        }
      }
    }
  }
  else
  {
    v_setBrightness(0x20);
  }
  
  v_printStringRaster(x-20, y+25, text, 40, -2, 0);
//  ZERO_AND_WAIT();
//  UNZERO();
//  v_setScaleForced(0x7f);
//  v_moveToImmediate8(x, y);
//  v_drawToImmediate8(width, 0);

v_directDraw32(x*128,y*128,(x+width)*128,y*128,0x3f);
  
  
  
//  ZERO_AND_WAIT();
//  UNZERO();
  
//  v_moveToImmediate8((int)(x+pos), y+5);
//  v_drawToImmediate8(0, -10);
v_directDraw32((x+pos)*128,(y+5)*128,(x+pos)*128,(y-10)*128,0x3f);
//  ZERO_AND_WAIT();
}




const signed char vectrexExactExample[]=
{   (signed char) 0xFF, +0x00, +0x0C,  // pattern, y, x
    (signed char) 0x00, +0x00, +0x10,  // pattern, y, x
    (signed char) 0xFF, +0x00, +0x10,  // pattern, y, x
    (signed char) 0x00, +0x18, +0x18,  // pattern, y, x
    (signed char) 0xFF, -0x40, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x10, +0x24,  // pattern, y, x
    (signed char) 0xFF, +0x44, -0x02,  // pattern, y, x
    (signed char) 0xFF, +0x44, -0x02,  // pattern, y, x
    (signed char) 0xFF, +0x42, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x42, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x0C, -0x08,  // pattern, y, x
    (signed char) 0xFF, +0x08, -0x24,  // pattern, y, x
    (signed char) 0xFF, -0x08, -0x4C,  // pattern, y, x
    (signed char) 0xFF, -0x08, -0x4C,  // pattern, y, x
    (signed char) 0xFF, -0x10, -0x08,  // pattern, y, x
    (signed char) 0xFF, -0x18, +0x00,  // pattern, y, x
    (signed char) 0x00, -0x10, +0x0C,  // pattern, y, x
    (signed char) 0xFF, -0x18, +0x00,  // pattern, y, x
    (signed char) 0x00, +0x00, -0x0C,  // pattern, y, x
    (signed char) 0xFF, +0x18, -0x0C,  // pattern, y, x
    (signed char) 0x00, -0x10, -0x14,  // pattern, y, x
    (signed char) 0xFF, -0x14, +0x14,  // pattern, y, x
    (signed char) 0x00, -0x0C, -0x08,  // pattern, y, x
    (signed char) 0xFF, +0x10, -0x18,  // pattern, y, x
    (signed char) 0x00, +0x48, +0x30,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x4E,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x4E,  // pattern, y, x
    (signed char) 0x00, -0x04, +0x04,  // pattern, y, x
    (signed char) 0xFF, -0x36, +0x02,  // pattern, y, x
    (signed char) 0xFF, -0x36, +0x02,  // pattern, y, x
    (signed char) 0xFF, -0x36, +0x02,  // pattern, y, x
    (signed char) 0xFF, -0x36, +0x02,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x20,  // pattern, y, x
    (signed char) 0x00, -0x18, +0x00,  // pattern, y, x
    (signed char) 0xFF, -0x10, +0x00,  // pattern, y, x
    (signed char) 0xFF, -0x0C, -0x18,  // pattern, y, x
    (signed char) 0xFF, +0x14, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x18,  // pattern, y, x
    (signed char) 0x00, -0x34, -0x24,  // pattern, y, x
    (signed char) 0xFF, +0x00, -0x08,  // pattern, y, x
    (signed char) 0xFF, +0x44, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x02, -0x4E,  // pattern, y, x
    (signed char) 0xFF, +0x02, -0x4E,  // pattern, y, x
    (signed char) 0x00, +0x08, +0x00,  // pattern, y, x
    (signed char) 0xFF, -0x40, +0x00,  // pattern, y, x
    (signed char) 0x00, +0x00, +0x08,  // pattern, y, x
    (signed char) 0xFF, +0x04, +0x1C,  // pattern, y, x
    (signed char) 0x00, -0x04, -0x0C,  // pattern, y, x
    (signed char) 0xFF, -0x06, +0x42,  // pattern, y, x
    (signed char) 0xFF, -0x06, +0x42,  // pattern, y, x
    (signed char) 0x00, +0x08, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x04, -0x3A,  // pattern, y, x
    (signed char) 0xFF, +0x04, -0x3A,  // pattern, y, x
    (signed char) 0xFF, +0x30, -0x04,  // pattern, y, x
    (signed char) 0x00, +0x24, +0x18,  // pattern, y, x
    (signed char) 0xFF, +0x00, +0x4C,  // pattern, y, x
    (signed char) 0xFF, +0x48, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x48, +0x00,  // pattern, y, x
    (signed char) 0xFF, -0x04, -0x2C,  // pattern, y, x
    (signed char) 0xFF, -0x04, -0x2C,  // pattern, y, x
    (signed char) 0x00, +0x04, -0x14,  // pattern, y, x
    (signed char) 0xFF, +0x04, -0x1C,  // pattern, y, x
    (signed char) 0xFF, -0x08, -0x24,  // pattern, y, x
    (signed char) 0xFF, -0x1C, -0x1C,  // pattern, y, x
    (signed char) 0xFF, -0x30, -0x10,  // pattern, y, x
    (signed char) 0xFF, -0x24, +0x08,  // pattern, y, x
    (signed char) 0xFF, -0x18, +0x18,  // pattern, y, x
    (signed char) 0xFF, -0x18, +0x28,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x30,  // pattern, y, x
    (signed char) 0xFF, +0x1C, +0x20,  // pattern, y, x
    (signed char) 0xFF, +0x24, +0x14,  // pattern, y, x
    (signed char) 0xFF, +0x30, -0x08,  // pattern, y, x
    (signed char) 0xFF, +0x1C, -0x18,  // pattern, y, x
    (signed char) 0xFF, +0x10, -0x20,  // pattern, y, x
    (signed char) 0x00, -0x0C, +0x04,  // pattern, y, x
    (signed char) 0xFF, -0x18, -0x10,  // pattern, y, x
    (signed char) 0x00, -0x20, -0x08,  // pattern, y, x
    (signed char) 0xFF, -0x24, +0x08,  // pattern, y, x
    (signed char) 0xFF, -0x0C, +0x08,  // pattern, y, x
    (signed char) 0xFF, -0x10, -0x10,  // pattern, y, x
    (signed char) 0xFF, +0x10, -0x10,  // pattern, y, x
    (signed char) 0xFF, +0x0C, +0x08,  // pattern, y, x
    (signed char) 0xFF, +0x24, +0x08,  // pattern, y, x
    (signed char) 0x00, +0x28, +0x28,  // pattern, y, x
    (signed char) 0xFF, -0x14, -0x14,  // pattern, y, x
    (signed char) 0x00, -0x0C, +0x08,  // pattern, y, x
    (signed char) 0xFF, +0x10, +0x1C,  // pattern, y, x
    (signed char) 0x00, +0x34, +0x2E,  // pattern, y, x
    (signed char) 0x00, +0x34, +0x2E,  // pattern, y, x
    (signed char) 0xFF, -0x10, +0x04,  // pattern, y, x
    (signed char) 0xFF, -0x04, +0x20,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};

const signed char _audioIcon[]=
{   (signed char) 111, // count of vectors
    (signed char) 0x3C, (signed char) 0x3B, (signed char) 0xD8, (signed char) 0x2C, // y0, x0, y1, x1
    (signed char) 0xD8, (signed char) 0x2C, (signed char) 0xC9, (signed char) 0x20, // y0, x0, y1, x1
    (signed char) 0xC9, (signed char) 0x20, (signed char) 0xD1, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xD1, (signed char) 0x00, (signed char) 0x32, (signed char) 0xEF, // y0, x0, y1, x1
    (signed char) 0x32, (signed char) 0xEF, (signed char) 0x3A, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0x3A, (signed char) 0x02, (signed char) 0x3C, (signed char) 0x3B, // y0, x0, y1, x1
    (signed char) 0x31, (signed char) 0x28, (signed char) 0x2F, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0x2F, (signed char) 0xF3, (signed char) 0xD4, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0xD4, (signed char) 0x02, (signed char) 0xCE, (signed char) 0x1E, // y0, x0, y1, x1
    (signed char) 0xCE, (signed char) 0x1E, (signed char) 0x31, (signed char) 0x28, // y0, x0, y1, x1
    (signed char) 0xF1, (signed char) 0x14, (signed char) 0xF1, (signed char) 0x0B, // y0, x0, y1, x1
    (signed char) 0xF1, (signed char) 0x0B, (signed char) 0xED, (signed char) 0x07, // y0, x0, y1, x1
    (signed char) 0xED, (signed char) 0x07, (signed char) 0xE4, (signed char) 0x05, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x05, (signed char) 0xDE, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0xDE, (signed char) 0x09, (signed char) 0xDC, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0xDC, (signed char) 0x12, (signed char) 0xE3, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0xE3, (signed char) 0x19, (signed char) 0xEA, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0xEA, (signed char) 0x19, (signed char) 0xF1, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0xEA, (signed char) 0x10, (signed char) 0xE7, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0xE7, (signed char) 0x12, (signed char) 0xE4, (signed char) 0x11, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x11, (signed char) 0xE3, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0xE3, (signed char) 0x0E, (signed char) 0xE6, (signed char) 0x0C, // y0, x0, y1, x1
    (signed char) 0xE6, (signed char) 0x0C, (signed char) 0xE9, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xE9, (signed char) 0x0D, (signed char) 0xEA, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0xED, (signed char) 0x0D, (signed char) 0xE8, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0xE8, (signed char) 0x09, (signed char) 0xE2, (signed char) 0x0A, // y0, x0, y1, x1
    (signed char) 0xE2, (signed char) 0x0A, (signed char) 0xE0, (signed char) 0x11, // y0, x0, y1, x1
    (signed char) 0xE0, (signed char) 0x11, (signed char) 0xE4, (signed char) 0x15, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x15, (signed char) 0xEC, (signed char) 0x13, // y0, x0, y1, x1
    (signed char) 0xEC, (signed char) 0x13, (signed char) 0xEE, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0xF6, (signed char) 0x0D, (signed char) 0xF3, (signed char) 0x07, // y0, x0, y1, x1
    (signed char) 0xF3, (signed char) 0x07, (signed char) 0xEC, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0xEC, (signed char) 0x02, (signed char) 0xE1, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0xE1, (signed char) 0x02, (signed char) 0xDC, (signed char) 0x06, // y0, x0, y1, x1
    (signed char) 0xDC, (signed char) 0x06, (signed char) 0xD8, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xD8, (signed char) 0x0D, (signed char) 0xDC, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0xDC, (signed char) 0x19, (signed char) 0xE4, (signed char) 0x1D, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x1D, (signed char) 0xED, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0xED, (signed char) 0x1C, (signed char) 0xF5, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0xF5, (signed char) 0x14, (signed char) 0xF6, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x2D, (signed char) 0x11, (signed char) 0x2A, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x2A, (signed char) 0x18, (signed char) 0x1E, (signed char) 0x22, // y0, x0, y1, x1
    (signed char) 0x1E, (signed char) 0x22, (signed char) 0x11, (signed char) 0x23, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x23, (signed char) 0x04, (signed char) 0x1E, // y0, x0, y1, x1
    (signed char) 0x04, (signed char) 0x1E, (signed char) 0xFF, (signed char) 0x15, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x15, (signed char) 0xFF, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x09, (signed char) 0x04, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x04, (signed char) 0x00, (signed char) 0x11, (signed char) 0xF9, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xF9, (signed char) 0x19, (signed char) 0xF9, // y0, x0, y1, x1
    (signed char) 0x19, (signed char) 0xF9, (signed char) 0x23, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0x23, (signed char) 0xFD, (signed char) 0x2A, (signed char) 0x04, // y0, x0, y1, x1
    (signed char) 0x2A, (signed char) 0x04, (signed char) 0x2D, (signed char) 0x11, // y0, x0, y1, x1
    (signed char) 0x28, (signed char) 0x12, (signed char) 0x27, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x27, (signed char) 0x08, (signed char) 0x20, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x20, (signed char) 0x00, (signed char) 0x16, (signed char) 0xFE, // y0, x0, y1, x1
    (signed char) 0x16, (signed char) 0xFE, (signed char) 0x0E, (signed char) 0xFF, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xFF, (signed char) 0x04, (signed char) 0x06, // y0, x0, y1, x1
    (signed char) 0x04, (signed char) 0x06, (signed char) 0x02, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x12, (signed char) 0x05, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x05, (signed char) 0x18, (signed char) 0x0E, (signed char) 0x1E, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0x1E, (signed char) 0x1D, (signed char) 0x1D, // y0, x0, y1, x1
    (signed char) 0x1D, (signed char) 0x1D, (signed char) 0x25, (signed char) 0x17, // y0, x0, y1, x1
    (signed char) 0x25, (signed char) 0x17, (signed char) 0x28, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x1F, (signed char) 0x0D, (signed char) 0x1C, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x14, (signed char) 0x16, (signed char) 0x17, // y0, x0, y1, x1
    (signed char) 0x16, (signed char) 0x17, (signed char) 0x0F, (signed char) 0x15, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0x15, (signed char) 0x0B, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x0B, (signed char) 0x12, (signed char) 0x0B, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x0B, (signed char) 0x0D, (signed char) 0x0E, (signed char) 0x07, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0x07, (signed char) 0x17, (signed char) 0x05, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x05, (signed char) 0x1D, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x1D, (signed char) 0x08, (signed char) 0x1F, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x1A, (signed char) 0x0D, (signed char) 0x17, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x09, (signed char) 0x13, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x13, (signed char) 0x09, (signed char) 0x10, (signed char) 0x0C, // y0, x0, y1, x1
    (signed char) 0x10, (signed char) 0x0C, (signed char) 0x10, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0x10, (signed char) 0x10, (signed char) 0x13, (signed char) 0x13, // y0, x0, y1, x1
    (signed char) 0x13, (signed char) 0x13, (signed char) 0x18, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0x12, (signed char) 0x1A, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x1E, (signed char) 0xE6, (signed char) 0x1D, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x1D, (signed char) 0xEA, (signed char) 0x17, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0xE6, (signed char) 0x0E, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xE5, (signed char) 0x02, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xEC, (signed char) 0x01, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0xEA, (signed char) 0x04, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0x04, (signed char) 0xE4, (signed char) 0x0D, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0x0D, (signed char) 0xE1, (signed char) 0x18, (signed char) 0xE2, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0xE2, (signed char) 0x1E, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0xD9, (signed char) 0x1C, (signed char) 0xD5, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xD5, (signed char) 0x0F, (signed char) 0xD3, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0xD3, (signed char) 0x00, (signed char) 0xD6, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xD6, (signed char) 0xFB, (signed char) 0xDA, // y0, x0, y1, x1
    (signed char) 0xFB, (signed char) 0xDA, (signed char) 0xFA, (signed char) 0xDE, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xDE, (signed char) 0xFD, (signed char) 0xE0, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xE0, (signed char) 0x05, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x05, (signed char) 0xD9, (signed char) 0x15, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0x15, (signed char) 0xD8, (signed char) 0x20, (signed char) 0xDD, // y0, x0, y1, x1
    (signed char) 0x20, (signed char) 0xDD, (signed char) 0x22, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x29, (signed char) 0xCA, (signed char) 0x22, (signed char) 0xC5, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0xC5, (signed char) 0x10, (signed char) 0xC4, // y0, x0, y1, x1
    (signed char) 0x10, (signed char) 0xC4, (signed char) 0x00, (signed char) 0xC6, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xC6, (signed char) 0xF2, (signed char) 0xCC, // y0, x0, y1, x1
    (signed char) 0xF2, (signed char) 0xCC, (signed char) 0xF2, (signed char) 0xCF, // y0, x0, y1, x1
    (signed char) 0xF2, (signed char) 0xCF, (signed char) 0xF4, (signed char) 0xD1, // y0, x0, y1, x1
    (signed char) 0xF4, (signed char) 0xD1, (signed char) 0xF7, (signed char) 0xD1, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0xD1, (signed char) 0xF9, (signed char) 0xCE, // y0, x0, y1, x1
    (signed char) 0xF9, (signed char) 0xCE, (signed char) 0xFE, (signed char) 0xCB, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0xCB, (signed char) 0x1F, (signed char) 0xCA, // y0, x0, y1, x1
    (signed char) 0x1F, (signed char) 0xCA, (signed char) 0x26, (signed char) 0xCE, // y0, x0, y1, x1
    (signed char) 0x26, (signed char) 0xCE, (signed char) 0x29, (signed char) 0xCD, // y0, x0, y1, x1
    (signed char) 0x29, (signed char) 0xCD, (signed char) 0x29, (signed char) 0xCA, // y0, x0, y1, x1
};

const signed char _nullList[]=
{   (signed char) 0, // count of vectors
    (signed char) 0x0, (signed char) 0x0, (signed char) 0x0, (signed char) 0x0, // y0, x0, y1, x1
  
};
const signed char _tailgunnerList[]=
{   (signed char) 39, // count of vectors
    (signed char) 0x18, (signed char) 0x30, (signed char) 0x18, (signed char) 0xCF, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0xD4, (signed char) 0xEB, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0xD4, (signed char) 0xEB, (signed char) 0x2A, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0x2A, (signed char) 0x12, (signed char) 0x2A, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0x2A, (signed char) 0x12, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xF0, (signed char) 0x0E, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0xEA, (signed char) 0x0E, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xF8, (signed char) 0x01, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0xFC, (signed char) 0x0E, (signed char) 0xF8, // y0, x0, y1, x1
    (signed char) 0x08, (signed char) 0xFB, (signed char) 0x08, (signed char) 0xF6, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0x06, (signed char) 0x01, (signed char) 0x06, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x0F, (signed char) 0x0E, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x18, (signed char) 0x01, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x1C, (signed char) 0xFD, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x25, (signed char) 0xFD, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0x25, (signed char) 0xFD, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x25, (signed char) 0xF7, (signed char) 0x24, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0x1C, (signed char) 0xF7, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x0F, (signed char) 0xF0, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x0F, (signed char) 0xF0, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x18, (signed char) 0xFD, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0x0F, (signed char) 0xF7, (signed char) 0x13, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x0A, (signed char) 0xFD, (signed char) 0x0A, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x01, (signed char) 0xF0, (signed char) 0x0A, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x01, (signed char) 0xFD, (signed char) 0x01, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xFC, (signed char) 0xFD, (signed char) 0xFC, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xF3, (signed char) 0xF0, (signed char) 0xFC, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xF3, (signed char) 0xFD, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xEF, (signed char) 0xFD, (signed char) 0xEF, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xE6, (signed char) 0xF0, (signed char) 0xEF, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xE6, (signed char) 0xF0, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0xF5, (signed char) 0xE0, (signed char) 0xF5, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xE1, (signed char) 0xF5, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xD8, (signed char) 0xF0, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xD8, (signed char) 0xF0, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xE1, (signed char) 0xFD, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0xCF, (signed char) 0xE5, (signed char) 0xCF, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0xCF, (signed char) 0xE5, (signed char) 0x30, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x30, (signed char) 0x18, (signed char) 0x30, // y0, x0, y1, x1
};


const signed char _vecxExactList[]=
{   (signed char) 56, // count of vectors
    (signed char) 0x21, (signed char) 0x16, (signed char) 0x22, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0x0E, (signed char) 0x26, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x10, (signed char) 0xF2, (signed char) 0x0B, (signed char) 0xED, // y0, x0, y1, x1
    (signed char) 0x08, (signed char) 0xEF, (signed char) 0x0C, (signed char) 0xF6, // y0, x0, y1, x1
    (signed char) 0x06, (signed char) 0xE8, (signed char) 0xFD, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xEA, (signed char) 0xFA, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xEC, (signed char) 0xF6, (signed char) 0xE8, // y0, x0, y1, x1
    (signed char) 0xF6, (signed char) 0xE8, (signed char) 0xFA, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xE4, (signed char) 0xFD, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xE6, (signed char) 0x06, (signed char) 0xE8, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xEA, (signed char) 0x14, (signed char) 0xEE, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0xED, (signed char) 0x13, (signed char) 0xF5, // y0, x0, y1, x1
    (signed char) 0x13, (signed char) 0xF5, (signed char) 0x0C, (signed char) 0xFB, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xFB, (signed char) 0x00, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xFD, (signed char) 0xF7, (signed char) 0xF8, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0xF8, (signed char) 0xF0, (signed char) 0xF0, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xF0, (signed char) 0xEE, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0xEE, (signed char) 0xE4, (signed char) 0xF4, (signed char) 0xDA, // y0, x0, y1, x1
    (signed char) 0xF4, (signed char) 0xDA, (signed char) 0xFA, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xD4, (signed char) 0x03, (signed char) 0xD2, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0xD2, (signed char) 0x0F, (signed char) 0xD6, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0xD6, (signed char) 0x16, (signed char) 0xDD, // y0, x0, y1, x1
    (signed char) 0x16, (signed char) 0xDD, (signed char) 0x18, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0xE6, (signed char) 0x17, (signed char) 0xED, // y0, x0, y1, x1
    (signed char) 0x16, (signed char) 0xF2, (signed char) 0x18, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0x08, (signed char) 0xF4, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0xF4, (signed char) 0x08, (signed char) 0xF4, (signed char) 0xF5, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xDA, (signed char) 0x08, (signed char) 0xE0, // y0, x0, y1, x1
    (signed char) 0x0B, (signed char) 0xE2, (signed char) 0x10, (signed char) 0xDD, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xE2, (signed char) 0x0E, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xE8, (signed char) 0x14, (signed char) 0xE8, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0xE5, (signed char) 0x1E, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x1E, (signed char) 0xE5, (signed char) 0x22, (signed char) 0xE7, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0xE7, (signed char) 0x26, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x26, (signed char) 0x0D, (signed char) 0x24, (signed char) 0x16, // y0, x0, y1, x1
    (signed char) 0x24, (signed char) 0x16, (signed char) 0x21, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x21, (signed char) 0x18, (signed char) 0xDE, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0xDE, (signed char) 0x19, (signed char) 0xDA, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0xDA, (signed char) 0x10, (signed char) 0xEA, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x0A, (signed char) 0xE4, (signed char) 0x06, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x02, (signed char) 0xE4, (signed char) 0xFF, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0xEF, (signed char) 0xDF, (signed char) 0xF0, // y0, x0, y1, x1
    (signed char) 0xDF, (signed char) 0xF0, (signed char) 0xDD, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xDB, (signed char) 0x0D, (signed char) 0xDE, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0xDF, (signed char) 0xEF, (signed char) 0xDE, (signed char) 0xE8, // y0, x0, y1, x1
    (signed char) 0xDE, (signed char) 0xE6, (signed char) 0xEE, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0xEC, (signed char) 0xE6, (signed char) 0xEB, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0x0D, (signed char) 0xDA, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xDA, (signed char) 0x0D, (signed char) 0xDA, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0xE7, (signed char) 0x18, (signed char) 0xE3, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0xE3, (signed char) 0x18, (signed char) 0xE0, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0xE0, (signed char) 0x12, (signed char) 0xE5, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x12, (signed char) 0xE7, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0xED, (signed char) 0x18, (signed char) 0xEB, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0x10, (signed char) 0x21, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0x0D, (signed char) 0x1E, (signed char) 0xE6, // y0, x0, y1, x1
};

const signed char _gravitarList[]=
{   (signed char) 112, // count of vectors
    (signed char) 0x01, (signed char) 0xE4, (signed char) 0x03, (signed char) 0xE9, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xEA, (signed char) 0x02, (signed char) 0xEE, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xF2, (signed char) 0x03, (signed char) 0xF7, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xFB, (signed char) 0x02, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x00, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xDE, (signed char) 0xFE, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0xDA, (signed char) 0x11, (signed char) 0xDE, // y0, x0, y1, x1
    (signed char) 0x09, (signed char) 0xD6, (signed char) 0x0F, (signed char) 0xDA, // y0, x0, y1, x1
    (signed char) 0x0A, (signed char) 0xD9, (signed char) 0x09, (signed char) 0xD6, // y0, x0, y1, x1
    (signed char) 0x06, (signed char) 0xD8, (signed char) 0x0A, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x05, (signed char) 0xD2, (signed char) 0x06, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0xD8, (signed char) 0x05, (signed char) 0xD2, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xDF, (signed char) 0x12, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0x19, (signed char) 0xE1, (signed char) 0x14, (signed char) 0xDF, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0xD6, (signed char) 0x19, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xCA, (signed char) 0x17, (signed char) 0xD6, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0xFA, (signed char) 0xCA, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0xE4, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xE6, (signed char) 0x01, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xE4, (signed char) 0x0C, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xE5, (signed char) 0x0C, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xE7, (signed char) 0x11, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xE4, (signed char) 0x11, (signed char) 0xE7, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xDF, (signed char) 0x14, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x00, (signed char) 0xDF, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xDF, (signed char) 0xFF, (signed char) 0xDB, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xCA, (signed char) 0xFE, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0xD9, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x09, (signed char) 0x03, (signed char) 0x04, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0x00, (signed char) 0x0C, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xFD, (signed char) 0x1C, (signed char) 0xF9, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xFC, (signed char) 0x1C, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xFE, (signed char) 0x11, (signed char) 0xFC, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x00, (signed char) 0x0C, (signed char) 0xFE, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xFF, (signed char) 0x11, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x01, (signed char) 0x1C, (signed char) 0xFF, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x03, (signed char) 0x1C, (signed char) 0x01, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xF9, (signed char) 0x03, (signed char) 0xF7, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0xF7, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0xEE, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xEE, (signed char) 0x0E, (signed char) 0xF0, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xF0, (signed char) 0x0E, (signed char) 0xF2, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xF2, (signed char) 0x12, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0xF3, (signed char) 0x12, (signed char) 0xF2, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0xF2, (signed char) 0x15, (signed char) 0xF4, // y0, x0, y1, x1
    (signed char) 0x15, (signed char) 0xF4, (signed char) 0x03, (signed char) 0xF2, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0xF2, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0xFE, (signed char) 0xDB, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0xDB, (signed char) 0x19, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0x19, (signed char) 0xE1, (signed char) 0x1B, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x1B, (signed char) 0xE6, (signed char) 0x14, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xEC, (signed char) 0x0F, (signed char) 0xEB, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0xEB, (signed char) 0x0D, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x0D, (signed char) 0xEA, (signed char) 0x0C, (signed char) 0xEB, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xEB, (signed char) 0x03, (signed char) 0xE9, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0xE9, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x01, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x1C, (signed char) 0x02, (signed char) 0x17, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x1B, (signed char) 0x0C, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0x12, (signed char) 0x17, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0x1B, (signed char) 0x12, (signed char) 0x1C, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x15, (signed char) 0x13, (signed char) 0x1B, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x10, (signed char) 0x15, (signed char) 0x13, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x08, (signed char) 0x03, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0x20, (signed char) 0xFF, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x25, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x12, (signed char) 0x02, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x0D, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x0B, (signed char) 0x02, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x09, (signed char) 0x17, (signed char) 0x0B, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x08, (signed char) 0x17, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x03, (signed char) 0x1C, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0x04, (signed char) 0x1C, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x03, (signed char) 0x04, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xFC, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xF8, (signed char) 0x02, (signed char) 0xFC, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xF9, (signed char) 0x0C, (signed char) 0xF8, // y0, x0, y1, x1
    (signed char) 0x1B, (signed char) 0xF3, (signed char) 0x1C, (signed char) 0xF9, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xED, (signed char) 0x1B, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xEA, (signed char) 0x14, (signed char) 0xED, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x20, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0x1C, (signed char) 0xFF, (signed char) 0x20, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x19, (signed char) 0x14, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x1C, (signed char) 0x11, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x1C, (signed char) 0x11, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x19, (signed char) 0x0C, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0xFE, (signed char) 0x26, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0x26, (signed char) 0x19, (signed char) 0x1E, // y0, x0, y1, x1
    (signed char) 0x19, (signed char) 0x1E, (signed char) 0x18, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0x25, (signed char) 0x11, (signed char) 0x2C, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x2C, (signed char) 0x0C, (signed char) 0x2E, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x2E, (signed char) 0x0C, (signed char) 0x2D, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x2D, (signed char) 0xFA, (signed char) 0x35, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0x35, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0xFD, (signed char) 0x2B, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x2B, (signed char) 0x12, (signed char) 0x24, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0x24, (signed char) 0x0F, (signed char) 0x27, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0x27, (signed char) 0x0E, (signed char) 0x26, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0x26, (signed char) 0x0B, (signed char) 0x27, // y0, x0, y1, x1
    (signed char) 0x0B, (signed char) 0x27, (signed char) 0x09, (signed char) 0x2B, // y0, x0, y1, x1
    (signed char) 0x09, (signed char) 0x2B, (signed char) 0xFC, (signed char) 0x30, // y0, x0, y1, x1
    (signed char) 0xFC, (signed char) 0x30, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0x17, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x17, (signed char) 0x12, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0x14, (signed char) 0x1A, (signed char) 0x1A, // y0, x0, y1, x1
    (signed char) 0x1A, (signed char) 0x1A, (signed char) 0x1A, (signed char) 0x1D, // y0, x0, y1, x1
    (signed char) 0x1A, (signed char) 0x1D, (signed char) 0xFE, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0x26, (signed char) 0xFD, (signed char) 0x2B, // y0, x0, y1, x1
    (signed char) 0xFB, (signed char) 0x30, (signed char) 0xFA, (signed char) 0x35, // y0, x0, y1, x1
};

const signed char *exampleLists[]=
{
  _tailgunnerList,
  _vecxExactList,
  _gravitarList,
  _audioIcon,
  _nullList,
  0
};

void displayHelp(char *textLines[], int direct)
{
  if (textLines == 0) return;

  int startLine = 0;
  int f1Request = 0;
  int starting = 1;
  
  
  while (1)
  {
    int line = 1;
    int y = 100;
//    if (direct)
//      v_WaitRecal_direct();
//    else
printf("Help Call\n");
    v_WaitRecal();
    
    v_readButtons();
    v_readJoystick1Analog();
    v_setBrightness(0x40);
    v_printStringRasterFont(-(strlen(textLines[0])*5), y, textLines[0], 50, -5, 0, alternate_rasterlines);
    y-=20;
    y-=20;
    
    while (textLines[line+startLine] != 0)
    {
      v_printStringRasterFont(-90, y, textLines[line+startLine], 40, -3, 0, alternate_raster5);
      y-=20;
      line++;
      if (line >6) break;
    }
    
    v_printStringRasterFont(-20, -80, "BUTTON 1", 40, -3, 0, alternate_raster5);
    if ((internalButtonState&0x0f) == (0x01)) // exactly button 1
    {
      if (!starting)
        f1Request =1;
    }
    else
    {
      if (f1Request)
      {
        if ((line >6) && (textLines[line+startLine] != 0))
        {
          startLine+=6;
        }
        else
        {
          break;
        }
      }
      f1Request =0;
      starting = 0;
    }
  }
}
char *calibrationHelp[]=
{
  "CALIBRATION", // title
  "IF VECTORS ARE DISPLAYED SKEWED,",
  "TRY USE CALIBRATION TO IMPROVE",
  "THE LOOK.",
  "BUTTON 3 TO DISPLAY DIFFERENT",
  "VECTORLISTS.",
  "(NON ZERO SLIGHTLY SLOWER)",
  0
};
char *delayXHelp[]=
{
  "DELAY AFTER XSH", // title
  "CRANKY VECTREX MIGHT NEED",
  "SOME TWEAKING HERE.",
  "   ",
  "- BUT PROBABLY NOT -",
  "(THE LOWER THE FASTER)",
  0
};
char *delayYHelp[]=
{
  "DELAY AFTER YSH", // title
  "CRANKY VECTREX NEED TWEAKING",
  "THIS. IF SINGLE VECTORS",
  "IN A CLUSTER 'STICK OUT' THAN",
  "THIS MIGHT HELP.",
  "(THE LOWER THE FASTER)",
  0
};
char *delayBbeforeAHelp[]=
{
  "DELAY B BEFORE A", // title
  "CRANKY VECTREX NEED TWEAKING",
  "THIS. IF SINGLE VECTORS",
  "IN A CLUSTER 'STICK OUT' THAN",
  "THIS MIGHT HELP.",
  "(THE LOWER THE FASTER)",
  0
};
char *delayYto0Help[]=
{
  "DELAY Y TO NULL", // title
  "CRANKY VECTREX NEED TWEAKING",
  "THIS. IF SINGLE VECTORS",
  "IN A CLUSTER 'STICK OUT' THAN",
  "THIS MIGHT HELP.",
  "(THE LOWER THE FASTER)",
  0
};
char *maxStrengthHelp[]=
{
  "MAX STRENGTH", // title
  "CRANKY VECTREX NEED TWEAKING!",
  "CHANGING HIGH VALUES TO ",
  "NEGATIVE HIGH VALUES ",
  "CAN 'CONFUSE' INTEGRATORS.",
  "    ",
  "    ",
  "THIS CAN BE PREVENTED BY NOT",
  "USING HIGH VALUES AT ALL...",
  "(THE HIGHER THE FASTER)",
  0
};

char *delayZEROHelp[]=
{
  "DELAY ZERO", // title
  "A FIXED NUMBER OF",
  "VECTREX CYCLES FOR THE",
  "ZEROING TO SETTLE.",
  "    ",
  "- SUBJECT TO CHANGE -",
  "(THE LOWER THE FASTER)",
  0
};

char *delayAFTERT1Help[]=
{
  "DELAY AFTER T1", // title
  "POSITIONING IS DONE USING",
  "VIA TIMER 1. T1 CONTROLS",
  "THE RAMP SIGNAL. HOWEVER",
  "REACTING IMMEDIATELY TO",
  "AN EXPIRED T1 IS TO",
  "'EARLY'!",
  "THESE ARE THE CYCLES TO",
  "ADDITIONALLY WAIT BEFORE",
  "CONTINUING.",
  "(THE LOWER THE FASTER)",
  0
};

char *scaleStrengthHelp[]=
{
  "SCALE STRENGTH DIF", // title
  "A CONSTANT USED AS A",
  "CORRECTION VALUE TO",
  "CALCULATE POSITIONING WITH",
  "STRENGTH AND SCALE.",
  "(NO SPEED INFLUENCE)",
  0
};
char *MINSCALEHelp[]=
{
  "MINSCALE", // title
  "THE MINIMUM SCALE THAT",
  "MIGHT BE USED INTERNALLY.",
  "(THE LOWER THE FASTER)",
  0
};

char *orientationHelp[]=
{
  "ORIENTATION", // title
  "TRY IT AND SEE FOR YOURSELF!",
  "(NO SPEED INFLUENCE)",
  0
};
char *pipelinTypeHelp[]=
{
  "PIPELINE TYPE", // title
  "DON'T BOTHER.",
  "THIS VALUE MUST BE SET",
  "PER INDIVIDUAL PROGRAM.",
  "(NO SPEED INFLUENCE)",
  0
};

char *doubleTimerHelp[]=
{
  "DOUBLE TIMER", // title
  "POSITIONING IS MORE EXACT.",
  "FOR NON MOVING OBJECTS, THIS",
  "DOES NOT MATTER.",
  "FAST MOVING OBJECT MIGHT NEED",
  "THIS ENABLED.",
  "(THE LOWER THE FASTER)",
  0
};

char *dotsOrderedHelp[]=
{
  "KEEP DOTS ORDERED", // title
  "THE PIPELINE PER DEFAULT",
  "HANDLES ALL DOTS 'LAST'.",
  "SOME PROGRAMS NEED THIS FOR",
  "GOOD DISPLAY, SOME THE OTHER",
  "WAY AROUND.",
  "(NO SPEED INFLUENCE)",
  0
};

char *beamOffInDrawHelp[]=
{
  "BEAM OFF IN DRAWS", // title
  "WHILE DRAWING A CLUSTER OF",
  "VECTORS, THE LIGHT CAN BE LEFT",
  "ON, OR BE SWITCHED OFF.",
  "   ",
  "   ",
  "   ",
  "LEAVING 'ON' RESULTS IN",
  "SLIGHTLY MORE DOTTED LINES.",
  "(THE LOWER THE FASTER)",
  0
};

char *cycleEquiHelp[]=
{
  "CYCLE EQUIVALENZ", // title
  "HOW MANY PI CYCLES REPRESENT",
  "ONE VECTREX CYCLE.",
  "WEIRD THING CAN HAPPEN IF",
  "CHANGED.",
  "(THE LOWER THE FASTER)",
  0
};

char *zeroBorderHelp[]=
{
  "ZERO BORDER", // title
  "AT WHAT DISTANCE BETWEEN",
  "DRAWS THE PIPELINE WILL",
  "AUTOMATICALLY INSERT A ",
  "ZERO REF.",
  "(THE HIGHER CAN BE FASTER)",
  0
};

char *irqModeHelp[]=
{
  "IRQ MODE", // title
  "SWITCH THE OUTPUT ROUTINES TO",
  "BE IRQ DRIVEN, THIS ENABLES",
  "A MULTITASKING OF SORTS. ",
  "ONLY ACTIVATED WHEN LEAVING",
  "THE SETTINGS MENU.",
  0
};
char *vectrexHzHelp[]=
{
  "VECTREX HZ", // title
  "REFRESH FREQUENCY OF",
  "THE VECTREX OUTPUT.",
  "SHOULD BE 50HZ FOR A ",
  "CLEAN DISPLAY.",
  0
};
char *appHzHelp[]=
{
  "APP HZ", // title
  "REFRESH FREQUENCY OF",
  "THE PROGRAM RUNNING.",
  "ONLY ACTIVE WITH IRQ MODE.",
  "VALUE 0 SYNCS WITH VECTREX",
  "DISPLAY FREQUENCY",
  0
};

char *xOffsetHelp[]=
{
  "X OFFSET", // title
  "POSITIOING ON SCREEN.",
  "HORIZONTALLY",
  "(NO SPEED INFLUENCE)",
  0
};

char *yOffsetHelp[]=
{
  "Y OFFSET", // title
  "POSITIOING ON SCREEN.",
  "VERTICALLY",
  "(NO SPEED INFLUENCE)",
  0
};

char *xSizeHelp[]=
{
  "X SIZE", // title
  "HORIZONTAL SIZE FACTOR.",
  "(NO SPEED INFLUENCE)",
  0
};

char *ySizeHelp[]=
{
  "Y SIZE", // title
  "VERTICAL SIZE FACTOR.",
  "(NO SPEED INFLUENCE)",
  0
};

char *frequencyHelp[]=
{
  "TEST FRQ", // title
  "PLAYTHING - SET THE",
  "REFRESH VALUE OF VECTREX.",
  "   ",
  "ATTENTION:",
  "THIS WILL BE SAVED!",
  0
};
char *pipelinUseHelp[]=
{
  "USE PIPELINE", // title
  "ALWAYS!!!",
  "USE THE PIPELINE!",
  0
};

unsigned int vecHz;
unsigned int appHz;
unsigned int cycleEquivalentConfig;
CalibrationItems calItems[] =
{
  {&calibrationValue_5,0,0,0,0                    ,0,255,0,"CALIBRATION 5", CAL_TYPE_UCHAR, calibrationHelp},
  {&calibrationValue_10,0,0,0,0                    ,0,255,0,"CALIBRATION 10", CAL_TYPE_UCHAR, calibrationHelp},
  {&calibrationValue_15,0,0,0,0                    ,0,255,0,"CALIBRATION 15", CAL_TYPE_UCHAR, calibrationHelp},
  {&calibrationValue_20,0,0,0,0                    ,0,255,0,"CALIBRATION 20", CAL_TYPE_UCHAR, calibrationHelp},
  {&calibrationValue_50,0,0,0,0                    ,0,255,0,"CALIBRATION 50", CAL_TYPE_UCHAR, calibrationHelp},
  {&calibrationValue_100,0,0,0,0                    ,0,255,0,"CALIBRATION 100", CAL_TYPE_UCHAR, calibrationHelp},

    {0,0,0,0,&softwareXCalibration                        ,-2,2,0, "XCAL_VALUE",      CAL_TYPE_FLOAT, xSizeHelp },
    {0,0,0,0,&softwareYCalibration                        ,-2,2,0, "YCAL_VALUE",      CAL_TYPE_FLOAT, ySizeHelp },

  // TEST ONLY
// todo correct documentation
  // after y has been set to A reg
  {&YSH_A_DELAY,0,0,0,0               ,0,20,2, "YSH_A_DELAY", CAL_TYPE_UCHAR, delayXHelp },
  // after mux has been set for Y (after y has been set)
  {&YSH_B_DELAY,0,0,0,0               ,0,20,2, "YSH_B_DELAY", CAL_TYPE_UCHAR, delayXHelp },
  {&XSH_A_DELAY,0,0,0,0               ,0,20,2, "XSH_A_DELAY", CAL_TYPE_UCHAR, delayXHelp },
  {&XSH_B_DELAY,0,0,0,0               ,0,20,2, "XSH_B_DELAY", CAL_TYPE_UCHAR, delayXHelp },

//  {&BEAM_ON_DELAY,0,0,0,0               ,0,20,0, "BEAM_ON_DELAY", CAL_TYPE_UCHAR, delayXHelp },
//  {&BEAM_OFF_DELAY,0,0,0,0               ,0,20,0, "BEAM_OFF_DELAY", CAL_TYPE_UCHAR, delayXHelp },


  {&DELAY_AFTER_XSH_VALUE,0,0,0,0               ,0,10,2, "DELAY AFTER XSH", CAL_TYPE_UCHAR, delayXHelp },
  {&DELAY_AFTER_YSH_VALUE,0,0,0,0               ,0,20,4, "DELAY AFTER YSH", CAL_TYPE_UCHAR, delayYHelp },
  {&DELAY_PORT_B_BEFORE_PORT_A_VALUE,0,0,0,0    ,0,20,2, "DELAY B BEFORE A", CAL_TYPE_UCHAR, delayBbeforeAHelp }, // yes the same applies here!
  {&CRANKY_DELAY_Y_TO_NULL_VALUE,0,0,0,0        ,0,20,6, "DELAY Y TO NULL", CAL_TYPE_UCHAR, delayYto0Help }, // and here!
  {&MAX_USED_STRENGTH,0,0,0,0                   ,1,127,90, "MAX STRENGTH", CAL_TYPE_UCHAR, maxStrengthHelp},
//  {&MAX_CONSECUTIVE_DRAWS,0,0,0,0               ,1,255,60, "MAX_CONSECUTIVE_DRAWS", CAL_TYPE_UCHAR},
  {&DELAY_ZERO_VALUE,0,0,0,0                    ,0,100,30, "DELAY ZERO", CAL_TYPE_UCHAR, delayZEROHelp },
  {&DELAY_AFTER_T1_END_VALUE,0,0,0,0            ,0,50,16, "DELAY AFTER T1", CAL_TYPE_UCHAR, delayAFTERT1Help },
  {&SCALE_STRENGTH_DIF,0,0,0,0                  ,0,10,2, "SCALE STRENGTH DIF", CAL_TYPE_UCHAR, scaleStrengthHelp },
  {&MINSCALE,0,0,0,0                            ,1,20,1, "MINSCALE", CAL_TYPE_UCHAR, MINSCALEHelp },
  {0,0,&MAXSCALE,0,0                            ,1,0x500,1, "MAXSCALE", CAL_TYPE_UINT, MINSCALEHelp },

  {&bufferType,0,0,0,0                          ,0,2,2, "PIPELINE TYPE", CAL_TYPE_UCHAR, pipelinTypeHelp },
  {&orientation,0,0,0,0                         ,0,3,0, "ORIENTATION", CAL_TYPE_UCHAR, orientationHelp },
//  {&optimizationON,0,0,0,0                            ,0,1,1, "OPTIMIZATION", CAL_TYPE_BOOL},
  {&usePipeline,0,0,0,0                         ,0,1,1, "USE PIPELINE", CAL_TYPE_BOOL, pipelinUseHelp },
  {&useDoubleTimer,0,0,0,0                      ,0,1,0, "DOUBLE TIMER", CAL_TYPE_BOOL, doubleTimerHelp },
  {&keepDotsTogether,0,0,0,0                    ,0,1,0, "KEEP DOTS ORDERED", CAL_TYPE_BOOL, dotsOrderedHelp },
  {&beamOffBetweenConsecutiveDraws,0,0,0,0      ,0,1,0, "BEAM OFF IN DRAWS", CAL_TYPE_BOOL, beamOffInDrawHelp },

//  {0,0,&swapPorts,0,0                               ,20,100,50, "SWAP JOYSTICK PORTS", CAL_TYPE_UINT, vectrexHzHelp },

  {&wasIRQMode,0,0,0,0                          ,0,1,1, "IRQ MODE", CAL_TYPE_BOOL, irqModeHelp },
  {0,0,&vecHz,0,0                               ,20,100,50, "VECTREX HZ", CAL_TYPE_UINT, vectrexHzHelp },
  {0,0,&appHz,0,0                               ,0,100,0, "APP HZ", CAL_TYPE_UINT, appHzHelp },


  {0,0,&cycleEquivalentConfig,0,0                     ,0,1000,666, "CYCLE EQUIVALENZ", CAL_TYPE_UINT, cycleEquiHelp },
  {0,0,&resetToZeroDifMax,0,0                   ,0,10000,2500, "ZERO BORDER", CAL_TYPE_UINT, zeroBorderHelp },
  {0,0,0,&offsetX,0                             ,-20000,+20000,0, "X OFFSET", CAL_TYPE_SINT, xOffsetHelp },
  {0,0,0,&offsetY,0                             ,-20000,+20000,0, "Y OFFSET", CAL_TYPE_SINT, yOffsetHelp },

  {0,0,0,0,&sizeX                               ,0,2,1, "X SIZE", CAL_TYPE_FLOAT, xSizeHelp },
  {0,0,0,0,&sizeY                               ,0,2,1, "Y SIZE", CAL_TYPE_FLOAT, ySizeHelp },

  {&intensityMax,0,0,0,0                        ,0,127,127, "BRIGHTNESS_MAX", CAL_TYPE_UCHAR, xSizeHelp },
  {&intensityMin,0,0,0,0                        ,0,127,0, "BRIGHTNESS_MIN",   CAL_TYPE_UCHAR, xSizeHelp },
  {0,0,0,0,&intensityMul                        ,0,2,1, "BRIGHNESS_MUL",      CAL_TYPE_FLOAT, xSizeHelp },


  {&doAngleOptimization,0,0,0,0                 ,0,1,0, "ANGLE_OPTIMIZATION", CAL_TYPE_BOOL, dotsOrderedHelp },
  {&angleValue,0,0,0,0                          ,0,90,10, "ANGLE_MIN_VALUE", CAL_TYPE_UCHAR, dotsOrderedHelp },
  {&doOverlappingOptimization,0,0,0,0           ,0,1,0, "OVERLAPPING_OPTIMIZATION", CAL_TYPE_BOOL, beamOffInDrawHelp },
  {&iniOverwrite,0,0,0,0                        ,0,1,0, "INI_OVERWRITE_ALLOWED", CAL_TYPE_BOOL, dotsOrderedHelp },

  
// not included here!  
//  fprintf(iniOut,"INVERT_Y_AXIS = %i\n", invertYAxis); 
  
  
  {0,0,0,0,0,0,0,0,0,CAL_TYPE_NONE}
};

/*
 * 

 
// todo
typedef enum {
  CRANKY_CRANKY_VIA_B_DELAY_B0 = 0,               ///<
  CRANKY_CRANKY_VIA_B_DELAY_B1 = 0,               ///<
  CRANKY_CRANKY_VIA_B_DELAY_B2 = 0,               ///<
  CRANKY_CRANKY_VIA_B_DELAY_B3 = 0,               ///<
  CRANKY_BETWEEN_VIA_B = 16,               ///< uses above delay
  CRANKY_NULLING_WAIT = 32,               ///< uses fixed "6" cycles
  CRANKY_NULLING_CALIBRATE = 64,               ///< // does a calibrate after each "null" of Y
  CRANKY_T1_DELAYED = 128               ///< // not implemented
} CrankyFlags;
 unsigned int Vec_Rfrsh; // 30000 cylces (vectrex) = $7530, little endian = $3075
CrankyFlags crankyFlag; // cranky should be checked during calibration! In "VecFever" terms cranky off = burst modus
*/
void _displayLargeList(signed char lx, signed char ly, const signed char list[])
{
  int count = *list++;

  while (count >0)  
  {
    int y0 = ly + *list++;
    int x0 = lx + *list++;
    int y1 = ly + *list++;
    int x1 = lx + *list++;
    v_directDraw32((x0<<7), (y0<<7),(x1<<7),(y1<<7), 0x5f);
    count --;
  }
}

/*    
press "2" to change example list
press and hold X axis does ten times number change

press and HOLD 3 zo move sample list
press 4 tp exit
joystick up down to cjose settings type
joystick left right to change setting

button 2+3 at the same time resets to default

button 1 for help

on exit ASK for save as "ini"
*/
void v_SettingsGUI(int ownWaitRecal)
{
  static signed char lx = 0,ly = 0; 
  static int sel=0;
  static int lastButton = 0;
  static int doingSomething = 0;
  int oldCommenHints = commonHints;
  commonHints = oldCommenHints;
  char buf[256];
  int listSel = 0;
  
  vecHz = 1500000 / (((Vec_Rfrsh&0xff)*256) + ((Vec_Rfrsh>>8)&0xff));
  appHz = clientHz;
  
  cycleEquivalentConfig = getCycleEquivalent();
  
  ownWaitRecal = 0;
  inSettings = 1;
  if (ownWaitRecal)
  {
    if (wasIRQMode == 255)
    {
      wasIRQMode = isIRQMode;
      lastButton = internalButtonState;
      if (wasIRQMode==1)
        v_removeIRQHandling();  
    }
  }
  else
  {
    clipMode = 1; // 0 normal clipping, 1 = inverse clipping
    clipActive = 1;

    clipminX=-8000;
    clipmaxX= 9000;
    clipminY= 6500;
    clipmaxY= 16000;
    commonHints = PL_BASE_FORCE_NOT_CLIPPED | PL_BASE_FORCE_STABLE;
  }
  v_noSound();
  ownWaitRecal = 1;
  do 
  {
    if (ownWaitRecal)
    {
      inCalibration = 0;
      currentButtonState &= (0xff-0x20);

      v_WaitRecal();
      inCalibration = 1;
    }
    v_setBrightness(0x40);
    v_readButtons();
    v_readJoystick1Analog();
    if (ownWaitRecal) v_printString(-100, 100, "PITREX", 5, 0x3f);
    
    int x = -40;
    int y = 90;
    
    switch (calItems[sel].type)
    {
      case CAL_TYPE_BOOL:
      case CAL_TYPE_UCHAR:
      case CAL_TYPE_UINT:
      case CAL_TYPE_SINT:
      case CAL_TYPE_FLOAT:
      {
        v_immediateWidgetSlider(&calItems[sel], x, y, (sel) == sel);
        break;
      }
      
      default:
          break;
    }
    sprintf(buf,"%i", (roundCycles / 666));
    v_printStringRaster(-10, 70, buf, 30, -2, 0);
    setCycleEquivalent(cycleEquivalentConfig);
    
    if (calItems[sel].uivalue == &vecHz)
    {
      int32_t cycles = 1500000 / vecHz; // Hz to vectrex cycles
      Vec_Rfrsh = ((cycles&0xff)*256) + ((cycles>>8)&0xff); // swap hi / lo bytes for little endian VIA
    }
    
    if (calItems[sel].uivalue == &appHz)
    {
      if (appHz == 0) isSynchronizedWithIRQ = 1; else isSynchronizedWithIRQ = 0;
      clientHz = appHz;
    }
    
    
    if (ownWaitRecal)
    {
      int usedStringScale = 5;
      int size = 5;
      if (sel == 0) {usedStringScale = 5;  size = 5;}
      if (sel == 1) {usedStringScale = 10; size = 5;}
      if (sel == 2) {usedStringScale = 15; size = 5;}
      if (sel == 3) {usedStringScale = 20; size = 5;}
      if (sel == 4) {usedStringScale = 50; size = 5;}
      if (sel == 5) {usedStringScale = 100; size = 5;}

      if (sel<=5)
        commonHints|= PL_BASE_FORCE_USE_FIX_SIZE;
      else
        commonHints&= ~(PL_BASE_FORCE_USE_FIX_SIZE);
      
      
      
      
      v_printString_scale(-120, -80, "SOME STRING LIKE IN ZBLAST OR SO!", usedStringScale, size, 0x30);
      _displayLargeList(lx,ly,exampleLists[listSel]);
      commonHints&= ~(PL_BASE_FORCE_USE_FIX_SIZE);
    }
    
    if ((internalButtonState&0x0f) == (0x04)) // exactly button 3
    {
      if (internalJoy1Y<-80)
      {
	  ly--;
      }
      if (internalJoy1Y>80)
      {
	  ly++;
      }
      if (internalJoy1X<-80)
      {
	  lx--;
      }
      if (internalJoy1X>80)
      {
	  lx++;
      }
    }
    else
    {
      if (lastButton != internalButtonState)
      {
        if ((internalButtonState&0x0f) == (0x08)) // exactly button 4
        {
          while ((v_directReadButtons()&0x0f) == (0x08));

          inCalibration = 0;
          clipActive = 0;
          v_saveIni();
          if ((wasIRQMode==1) && (!ownWaitRecal))
          {
            wasIRQMode = 255;
            v_setupIRQHandling();  
          }
          break;
        }

        if ( (internalButtonState&0x0f) == (0x02)) // exactly button 2
        {
          listSel++;
          if (exampleLists[listSel] == 0) listSel = 0;
        }
        
        if ((internalButtonState&0x0f) == (0x01)) // exactly button 1
        {
// help "in game" currently is buggy, better switch it off for now          
// todo          while ((v_directReadButtons()&0x0f) == (0x01));
//          displayHelp(calItems[sel].help,1);
        }
      }
      lastButton = internalButtonState;
    
      
      if ((ABS(internalJoy1X)<20) && (ABS(internalJoy1Y)<20)) 
        doingSomething =0;

      if (doingSomething == 0)
      {
        if (internalJoy1Y<-80)
        {
          if (calItems[sel+1].type != CAL_TYPE_NONE)
          {
            sel++;
          }
          doingSomething=1;
        }
        if (internalJoy1Y>80)
        {
          if (sel > 0)
            sel--;
          doingSomething=1;
        }
      } 
    }
    
    setCalibrationValues();
    
    if (ownWaitRecal)
    {
      v_directDraw32(-15000, 15000,-15000,14000, 0x5f);
      v_directDraw32(-15000,14000,-14000,14000, 0x5f);
      v_directDraw32(-14000,14000,-14000,15000, 0x5f);

      v_directDraw32(-15000,-15000,-15000,-14000, 0x5f);
      v_directDraw32(-15000,-14000,-14000,-14000, 0x5f);
      v_directDraw32(-14000,-14000,-14000,-15000, 0x5f);

      v_directDraw32(-12000, 14000,-12000,15000, 0x5f);
      v_directDraw32(-12000, 15000,-13000,15000, 0x5f);
      v_directDraw32(-13000, 15000,-13000,14000, 0x5f);
    }
  }while(ownWaitRecal);

  if (ownWaitRecal)
  {
    if (wasIRQMode==1)
      v_setupIRQHandling();  
    wasIRQMode = 255;
  }

    
  inSettings = 0;
  printf("Exit Setting\n");    
    
  commonHints = oldCommenHints;
}

int v_saveIniTo(char *filename)
{
  FILE *iniOut=0;
  iniOut = fopen(filename, "w"); 
  if (iniOut == 0) 
  {
    v_message("FILE ERROR, INI NOT SAVED!");
    return 1;
  }

  fprintf(iniOut,"; Global Vectrex Interface ini file for PiTrex baremetal\n"); 
 
  fprintf(iniOut,"; This file must not be larger then 10kb!\n"); 
  fprintf(iniOut,"[VECTERX_INTERFACE]\n"); 
  fprintf(iniOut,"\n"); 
  fprintf(iniOut,"; DELAY_ZERO_VALUE: the value that is 'waited' before a next move/draw\n"); 
  fprintf(iniOut,"; after a zeroRef was done\n"); 
  fprintf(iniOut,"; if screen looks ok, than the smaller the value - the faster\n"); 
  fprintf(iniOut,"DELAY_ZERO_VALUE = %i;  \n", DELAY_ZERO_VALUE); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; CRANKY_FLAG: \n"); 
  fprintf(iniOut,";typedef enum {\n"); 
  fprintf(iniOut,";  CRANKY_CRANKY_VIA_B_DELAY_B0 = 0,   \n"); 
  fprintf(iniOut,";  CRANKY_CRANKY_VIA_B_DELAY_B1 = 0, \n"); 
  fprintf(iniOut,";  CRANKY_CRANKY_VIA_B_DELAY_B2 = 0,  \n"); 
  fprintf(iniOut,";  CRANKY_CRANKY_VIA_B_DELAY_B3 = 0, \n"); 
  fprintf(iniOut,";  CRANKY_BETWEEN_VIA_B = 16,       < uses above delay\n"); 
  fprintf(iniOut,";  CRANKY_NULLING_WAIT = 32,        < uses fixed '6' cycles\n"); 
  fprintf(iniOut,";  CRANKY_NULLING_CALIBRATE = 64,   < // does a calibrate after each 'null' of Y\n"); 
  fprintf(iniOut,";  CRANKY_T1_DELAYED = 128          < // not implemented\n"); 
  fprintf(iniOut,";} CrankyFlags;\n"); 
  fprintf(iniOut,"\n"); 
  fprintf(iniOut,"; 0x16 = 22 cranky delay of 6 cycles - this is for MY worst vectrex\n"); 
  fprintf(iniOut,"; 0x14 = 20 cranky delay of 4 cycles - \n"); 
  fprintf(iniOut,"; the smaller the faster\n"); 
  fprintf(iniOut,"CRANKY_FLAG = %i \n", crankyFlag); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; DELAY_AFTER_T1_END_VALUE: \n"); 
  fprintf(iniOut,"; changes from vectrex to vectrex, the less, the faster\n"); 
  fprintf(iniOut,"; values go between 17 - 22\n"); 
  fprintf(iniOut,"; STO\n"); 
  fprintf(iniOut,"DELAY_AFTER_T1_END_VALUE = %i \n", DELAY_AFTER_T1_END_VALUE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; SCALE_STRENGTH_DIF: \n"); 
  fprintf(iniOut,"; 'correction' cycles with a conversion from scale to strength\n"); 
  fprintf(iniOut,"; SSS\n"); 
  fprintf(iniOut,"SCALE_STRENGTH_DIF = %i;\n", SCALE_STRENGTH_DIF); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; BUFFER_TYPE\n"); 
  fprintf(iniOut,"; buffer type\n"); 
  fprintf(iniOut,"; 0 = no buffer, 1 = doublebuffer, 2 = autobuffer, 3 = no display\n"); 
  fprintf(iniOut,"BUFFER_TYPE = %i;\n", bufferType); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; MAX_USED_STRENGTH: \n"); 
  fprintf(iniOut,"; maximal strength that can be used without getting 'distortions'\n"); 
  fprintf(iniOut,"; on cranky vectrex this can be as low as 90\n"); 
  fprintf(iniOut,"; 'normal' vectrex can go as high as 127\n"); 
  fprintf(iniOut,"; potentially, the higher the faster\n"); 
  fprintf(iniOut,"MAX_USED_STRENGTH = %i\n", MAX_USED_STRENGTH); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; RESET_TO_ZERO_DIF_MAX: \n"); 
  fprintf(iniOut,"; when is a 'move' too far and a return to zero should be done instead\n"); 
  fprintf(iniOut,"; (plus an additional move)\n"); 
  fprintf(iniOut,"RESET_TO_ZERO_DIF_MAX = %i;\n", resetToZeroDifMax); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; CYCLE_EQUIVALENT: \n"); 
  fprintf(iniOut,"; how many Pi cycles represent one vectrex cycle. 666 nanoseconds should be pretty close\n"); 
  fprintf(iniOut,"CYCLE_EQUIVALENT = %i;\n", getCycleEquivalent());
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; BEAM_OFF_BETWEEN_CONSECUTIVE_DRAWS:\n"); 
  fprintf(iniOut,"; a thing of taste, vectors look more 'dotted' if this is 0\n"); 
  fprintf(iniOut,"; potentially a bit faster if 0 though\n"); 
  fprintf(iniOut,"BEAM_OFF_BETWEEN_CONSECUTIVE_DRAWS = %i;\n", beamOffBetweenConsecutiveDraws); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; ORIENTATION: \n"); 
  fprintf(iniOut,"; orientation rotated clock wise 0=normal, 1 = left, 2=up, 3 = right\n"); 
  fprintf(iniOut,"ORIENTATION = %i\n", orientation); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; CALIBRATION_VALUE: \n"); 
  fprintf(iniOut,"; value for a Tuts calibration, if 0 no calibration!\n"); 
  fprintf(iniOut,"CALIBRATION_VALUE5    = %i;\n", calibrationValue_5); 
  fprintf(iniOut,"CALIBRATION_VALUE10   = %i;\n", calibrationValue_10); 
  fprintf(iniOut,"CALIBRATION_VALUE15   = %i;\n", calibrationValue_15); 
  fprintf(iniOut,"CALIBRATION_VALUE20   = %i;\n", calibrationValue_20); 
  fprintf(iniOut,"CALIBRATION_VALUE50   = %i;\n", calibrationValue_50); 
  fprintf(iniOut,"CALIBRATION_VALUE100  = %i;\n", calibrationValue_100); 
  fprintf(iniOut,"XCAL_VALUE  = %f;\n", softwareXCalibration); 
  fprintf(iniOut,"YCAL_VALUE  = %f;\n", softwareYCalibration); 

  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; SWAP_JOYSTICK_PORTS: \n"); 
  fprintf(iniOut,"; swapPorts 1= yes, 0 = 0\n"); 
  fprintf(iniOut,"SWAP_JOYSTICK_PORTS = %i;\n",swapPorts ); 
  fprintf(iniOut,"\n"); 


  fprintf(iniOut,"; USE_PIPELINE: \n"); 
  fprintf(iniOut,"; use pipeline 1= yes, 0 = 0\n"); 
  fprintf(iniOut,"; should always be yes\n"); 
  fprintf(iniOut,"USE_PIPELINE = %i;\n",usePipeline ); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; KEEP_DOTS_ORDERED: \n"); 
  fprintf(iniOut,"KEEP_DOTS_ORDERED = %i;\n",keepDotsTogether ); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; OFFSET_X: \n"); 
  fprintf(iniOut,"; screen position, - more left, + more right\n"); 
  fprintf(iniOut,"OFFSET_X = %i\n", offsetX); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; OFFSET_Y: \n"); 
  fprintf(iniOut,"; screen position, - more left, + more right\n"); 
  fprintf(iniOut,"OFFSET_Y = %i\n", offsetY); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; SIZE_X:\n"); 
  fprintf(iniOut,"; size correction of display, floating post value 1.0 = 'normal' output\n"); 
  fprintf(iniOut,"; higher values enlarge, smaller shrink\n"); 
  fprintf(iniOut,"SIZE_X = %f\n", sizeX); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; SIZE_Y:\n"); 
  fprintf(iniOut,"; size correction of display, floating post value 1.0 = 'normal' output\n"); 
  fprintf(iniOut,"; higher values enlarge, smaller shrink\n"); 
  fprintf(iniOut,"SIZE_Y = %f\n", sizeY); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; LOG_OUTPUT: \n"); 
  fprintf(iniOut,"; if different than 0 - than log all 'printf' outputs also to SD\n"); 
  fprintf(iniOut,"LOG_OUTPUT = %i\n", logOutput); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; UART_OUTPUT: \n"); 
  fprintf(iniOut,"; if different than 0 - then printf does not output to UART (serial)\n"); 
  fprintf(iniOut,"UART_OUTPUT = %i\n", uartOutput); 
  fprintf(iniOut,"\n"); 

  
  int hz = 1500000 / (((Vec_Rfrsh&0xff)*256) + ((Vec_Rfrsh>>8)&0xff));
  fprintf(iniOut,"; REFRESH_HZ: \n"); 
  fprintf(iniOut,"REFRESH_HZ = %i\n", hz); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; MINSCALE: \n"); 
  fprintf(iniOut,"MINSCALE = %i\n", MINSCALE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; MAXSCALE: \n"); 
  fprintf(iniOut,"MAXSCALE = %i\n", MAXSCALE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; DOUBLE_TIMER: \n"); 
  fprintf(iniOut,"DOUBLE_TIMER = %i\n", useDoubleTimer); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; DELAY_AFTER_XSH_VALUE :\n"); 
  fprintf(iniOut,"DELAY_AFTER_XSH_VALUE = %i\n", DELAY_AFTER_XSH_VALUE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; DELAY_AFTER_YSH_VALUE: \n"); 
  fprintf(iniOut,"DELAY_AFTER_YSH_VALUE = %i \n", DELAY_AFTER_YSH_VALUE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; DELAY_PORT_B_BEFORE_PORT_A_VALUE: \n"); 
  fprintf(iniOut,"DELAY_PORT_B_BEFORE_PORT_A_VALUE = %i \n", DELAY_PORT_B_BEFORE_PORT_A_VALUE); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; CRANKY_DELAY_Y_TO_NULL_VALUE: \n"); 
  fprintf(iniOut,"CRANKY_DELAY_Y_TO_NULL_VALUE = %i\n", CRANKY_DELAY_Y_TO_NULL_VALUE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; IN_GAME_SETTINGS: IS BUTTON 2 JOYPAD 2 configured to call the settings menu\n"); 
  fprintf(iniOut,"IN_GAME_SETTINGS = %i\n", inGameSettingsAllowed); 
  fprintf(iniOut,"\n"); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; INVERT_Y_AXIS: \n"); 
  fprintf(iniOut,"INVERT_Y_AXIS = %i\n", invertYAxis); 
  fprintf(iniOut,"\n"); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"YSH_A_DELAY = %i\n", YSH_A_DELAY); 

  fprintf(iniOut,"YSH_B_DELAY = %i\n", YSH_B_DELAY); 
  fprintf(iniOut,"XSH_A_DELAY = %i\n", XSH_A_DELAY); 
  fprintf(iniOut,"XSH_B_DELAY = %i\n", XSH_B_DELAY); 

  fprintf(iniOut,"BRIGHTNESS_MAX = %i\n", intensityMax); 
  fprintf(iniOut,"BRIGHTNESS_MIN = %i\n", intensityMin); 
  fprintf(iniOut,"BRIGHNESS_MUL = %f\n", intensityMul); 
  
  fprintf(iniOut,"ANGLE_OPTIMIZATION = %i\n", doAngleOptimization); 
  fprintf(iniOut,"ANGLE_MIN_VALUE = %i\n", angleValue); 
  fprintf(iniOut,"OVERLAPPING_OPTIMIZATION = %f\n", doOverlappingOptimization); 
  fprintf(iniOut,"INI_OVERWRITE_ALLOWED = %i\n", iniOverwrite); 
  
  fprintf(iniOut,"\n"); 
  
   // as of now this is non optional!
   // {&wasIRQMode,0,0,0,0                          ,0,1,1, "IRQ MODE", CAL_TYPE_BOOL, irqModeHelp },
   // {0,0,&vecHz,0,0                               ,20,100,50, "VECTREX HZ", CAL_TYPE_UINT, vectrexHzHelp },
   // {0,0,&appHz,0,0                               ,0,100,0, "APP HZ", CAL_TYPE_UINT, appHzHelp },

  fclose(iniOut);
  printf("Settings saved to %s\n", filename); 
  return 0;
}
void v_saveIni()
{
    if (lastAccessedIni[0]!=0)
    {
        if (iniOverwrite==0)
        {
          if (strcmp(lastAccessedIni, "vectrexInterface.ini")!=0)
          {
            // we do not overwrite custom ini files per se
            // there might be options that are lost, which are
            // specific to the driver
            // and we do not know about that!
            int i=0;
            while (lastAccessedIni[i]!=0) i++;
            lastAccessedIni[i-1]='2';
          }          
        }
        if (v_saveIniTo(lastAccessedIni))
          printf("Error saving ini to %s\n", lastAccessedIni);
    }
    else
        printf("Ini not saved, do not know where to!\n");
}

#define SET_CALIB_RANGE(smaller,bigger,smallerPos,biggerPos) \
  {calibrationValues[smallerPos] = smaller; \
  calibrationValues[biggerPos] = bigger; \
  double dif = bigger-smaller; \
  dif = dif/ (biggerPos-smallerPos); \
  double baseValue = smaller; \
  for (int i=smallerPos+1; i<biggerPos; i++)  \
  { \
    baseValue += dif; \
    calibrationValues[i] = (unsigned char) (baseValue+0.5); \
  }} 

void setCalibrationValues()
{
  for (int i=0; i<5; i++) calibrationValues[i] = calibrationValue_5;
  
  SET_CALIB_RANGE(calibrationValue_5,calibrationValue_10,5,10)
  SET_CALIB_RANGE(calibrationValue_10,calibrationValue_15,10,15)
  SET_CALIB_RANGE(calibrationValue_15,calibrationValue_20,15,20)
  SET_CALIB_RANGE(calibrationValue_20,calibrationValue_50,20,50)
  SET_CALIB_RANGE(calibrationValue_50,calibrationValue_100,50,100)
  
  for (int i=100; i<512; i++) calibrationValues[i] = calibrationValue_100;
  
  
}
