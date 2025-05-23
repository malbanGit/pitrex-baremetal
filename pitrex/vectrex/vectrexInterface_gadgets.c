
void DisconnectUSBInterrupt()
{
  DisableInterrupts(); // function defined in out baremetal.S
  // reset everything interrupt
  rpiIRQController->FIQ_control = 0; // clear FIQ
  rpiIRQController->Disable_IRQs_1 = (uint32_t) -1; // clear all 1 interrupts
  rpiIRQController->Disable_IRQs_2 = (uint32_t) -1; // clear all 2 interrupts
  rpiIRQController->Disable_Basic_IRQs = (uint32_t) -1; // clear all Basic interrupts
  
  rpiIRQController->IRQ_basic_pending = rpiIRQController->IRQ_basic_pending; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_1 = rpiIRQController->IRQ_pending_1; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_2 = rpiIRQController->IRQ_pending_2; // acknoldedge all interrupts
}


#if RASPPI >= 2

#if RASPPI <= 3
#define ARM_LOCAL_BASE			0x40000000
#else
#define ARM_LOCAL_BASE			0xFF800000
#endif
#endif
    
#define ARM_LOCAL_CONTROL		(ARM_LOCAL_BASE + 0x000)
#define ARM_LOCAL_PRESCALER		(ARM_LOCAL_BASE + 0x008)
#define ARM_LOCAL_GPU_INT_ROUTING	(ARM_LOCAL_BASE + 0x00C)
#define ARM_LOCAL_PM_ROUTING_SET	(ARM_LOCAL_BASE + 0x010)
#define ARM_LOCAL_PM_ROUTING_CLR	(ARM_LOCAL_BASE + 0x014)
#define ARM_LOCAL_TIMER_LS		(ARM_LOCAL_BASE + 0x01C)
#define ARM_LOCAL_TIMER_MS		(ARM_LOCAL_BASE + 0x020)
#define ARM_LOCAL_INT_ROUTING		(ARM_LOCAL_BASE + 0x024)
#define ARM_LOCAL_AXI_COUNT		(ARM_LOCAL_BASE + 0x02C)
#define ARM_LOCAL_AXI_IRQ		(ARM_LOCAL_BASE + 0x030)
#define ARM_LOCAL_TIMER_CONTROL		(ARM_LOCAL_BASE + 0x034)
#define ARM_LOCAL_TIMER_WRITE		(ARM_LOCAL_BASE + 0x038)

#define ARM_LOCAL_TIMER_INT_CONTROL0	(ARM_LOCAL_BASE + 0x040)
#define ARM_LOCAL_TIMER_INT_CONTROL1	(ARM_LOCAL_BASE + 0x044)
#define ARM_LOCAL_TIMER_INT_CONTROL2	(ARM_LOCAL_BASE + 0x048)
#define ARM_LOCAL_TIMER_INT_CONTROL3	(ARM_LOCAL_BASE + 0x04C)

void ConnectInterrupt (unsigned nIRQ, TInterruptHandler *pHandler, void *pParam)
{
  useUSB = 1;
  pi_vectors *pv ;
  asm("ldr %[result],= 0\n\t" : [result]"=r" (pv) :: "cc");  // circumvent gcc UDF
  pv->_irq_pointer = (unsigned int)v_USBinterrupt_vector;

  usbIRQHandler = pHandler;
  usbIRQParam = pParam;
  
/*  
typedef struct {
    volatile uint32_t IRQ_basic_pending;
    volatile uint32_t IRQ_pending_1;
    volatile uint32_t IRQ_pending_2;
    volatile uint32_t FIQ_control;
    volatile uint32_t Enable_IRQs_1;
    volatile uint32_t Enable_IRQs_2;
    volatile uint32_t Enable_Basic_IRQs;
    volatile uint32_t Disable_IRQs_1;
    volatile uint32_t Disable_IRQs_2;
    volatile uint32_t Disable_Basic_IRQs;
    } rpi_irq_controller_t;
    
    
    
    
*/

#if RASPPI >= 2
  write32 (ARM_LOCAL_TIMER_INT_CONTROL0, 0);
#endif
  
  // reset everything interrupt
  rpiIRQController->FIQ_control = 0; // clear FIQ
  rpiIRQController->Disable_IRQs_1 = (uint32_t) -1; // clear all 1 interrupts
  rpiIRQController->Disable_IRQs_2 = (uint32_t) -1; // clear all 2 interrupts
  rpiIRQController->Disable_Basic_IRQs = (uint32_t) -1; // clear all Basic interrupts
  
  rpiIRQController->IRQ_basic_pending = rpiIRQController->IRQ_basic_pending; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_1 = rpiIRQController->IRQ_pending_1; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_2 = rpiIRQController->IRQ_pending_2; // acknoldedge all interrupts
  




  // enable USB interrupt
  rpiIRQController->Enable_IRQs_1 = 1 << nIRQ; // USB = 9

  
  // enable interrupts in geenral
  /* Enable interrupts! */
  EnableInterrupts(); // function defined in out baremetal.S
}
 





// in order to connect the usb keyboard
// for a short time we have to enable the
// usb keyboard interrupt (this is done inside the USPiInitialize() )
// for that to work, we have to ensure our own interrupt is disabled beforhand...
// ... later
// we do not want that interrupt in our system - so we remove it immedeatily after the init
//
// if we had an "own" interrupt - we have to reinitialize it...

int v_initKeyboard()
{
  if (usbKeyboardAvailable) return 1;
  int oldIRQMode = isIRQMode;
  if (isIRQMode == 1)
    v_removeIRQHandling();
    
    
  usbKeyboardAvailable = 0;
  useUSB = 0;
  // Initialize USB system we will want keyboard and mouse 

  if (!USPiInitialize())
  {
    printf("USBV failed to initialize!\n\r");
    DisconnectUSBInterrupt();
    if (oldIRQMode) v_setupIRQHandling();
    return 0;
  }

  if (!USPiKeyboardAvailable())
  {
    printf("Keyboard not found\n\r");
    DisconnectUSBInterrupt();
    if (oldIRQMode) v_setupIRQHandling();
    return 0;
  }
//    USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);

  DisconnectUSBInterrupt();
  usbKeyboardAvailable = 1;
  useUSB = 1;
  printf("USB Keyboard found...\n\r");

  if (oldIRQMode) v_setupIRQHandling();
  return 1;
}

void v_doInputMapping(InputMapping inputMapping[])
{
  int i=0;
  while (inputMapping[i].destination!=0)
  {
    // always reset to zero!
    *inputMapping[i].destination = 0;
    // buttons
    if (inputMapping[i].mapping & BUTTON_AND)
    {
      if ((currentButtonState & (inputMapping[i].mapping&0xff)) == (inputMapping[i].mapping&0xff))
      {
        *inputMapping[i].destination = 1;
      }
    }
    else
    {
      // any button matchings
      if (currentButtonState & (inputMapping[i].mapping&0xff))
        *inputMapping[i].destination = 1;
    }
    if (inputMapping[i].mapping&JOYPAD_1_DIGITAL)
    {
      if (inputMapping[i].mapping&JOYPAD_UP)
      {
        if (currentJoy1Y>0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_DOWN)
      {
        if (currentJoy1Y<0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_RIGHT)
      {
        if (currentJoy1X>0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_LEFT)
      {
        if (currentJoy1X<0) *inputMapping[i].destination = 1;
      }
    }
    if (inputMapping[i].mapping&JOYPAD_2_DIGITAL)
    {
      if (inputMapping[i].mapping&JOYPAD_UP)
      {
        if (currentJoy2Y>0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_DOWN)
      {
        if (currentJoy2Y<0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_RIGHT)
      {
        if (currentJoy2X>0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_LEFT)
      {
        if (currentJoy2X<0) *inputMapping[i].destination = 1;
      }
    }
#define MIN_VALUE 5
    
    // all analog values are ABS(x) > MIN_VALUE, so thy do not "overwrite" with small values already gotten values
    if (inputMapping[i].mapping&JOYPAD_1_ANALOG)
    {
      if (inputMapping[i].mapping&JOYPAD_UP)
      {
        //if (currentJoy1Y>30) *inputMapping[i].destination = currentJoy1Y;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy1Y>MIN_VALUE)
            *inputMapping[i].destination = currentJoy1Y;
      }
      if (inputMapping[i].mapping&JOYPAD_DOWN)
      {
        //if (currentJoy1Y<-30) *inputMapping[i].destination = currentJoy1Y;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
        {
          if (currentJoy1Y<-MIN_VALUE)
            *inputMapping[i].destination = currentJoy1Y;
        }
      }
      if (inputMapping[i].mapping&JOYPAD_RIGHT)
      {
        //if (currentJoy1X>30) *inputMapping[i].destination = currentJoy1X;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy1X>MIN_VALUE)
          *inputMapping[i].destination = currentJoy1X;
      }
      if (inputMapping[i].mapping&JOYPAD_LEFT)
      {
        //if (currentJoy1X<-30) *inputMapping[i].destination = currentJoy1X;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy1X<-MIN_VALUE)
          *inputMapping[i].destination = currentJoy1X;
      }
    }
    if (inputMapping[i].mapping&JOYPAD_2_ANALOG)
    {
      if (inputMapping[i].mapping&JOYPAD_UP)
      {
        //if (currentJoy2Y>30) *inputMapping[i].destination = currentJoy2Y;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy2Y>MIN_VALUE)
          *inputMapping[i].destination = currentJoy2Y;
      }
      if (inputMapping[i].mapping&JOYPAD_DOWN)
      {
        //if (currentJoy2Y<-30) *inputMapping[i].destination = currentJoy2Y;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
        {
          if (currentJoy2Y<-MIN_VALUE)
            *inputMapping[i].destination = currentJoy2Y;
        }
      }
      if (inputMapping[i].mapping&JOYPAD_RIGHT)
      {
        //if (currentJoy2X>30) *inputMapping[i].destination = currentJoy2X;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy2X>MIN_VALUE)
            *inputMapping[i].destination = currentJoy2X;
      }
      if (inputMapping[i].mapping&JOYPAD_LEFT)
      {
        //if (currentJoy2X<-30) *inputMapping[i].destination = currentJoy2X;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy2X<-MIN_VALUE)
            *inputMapping[i].destination = currentJoy2X;
      }
    }
    i++;
  }  
}


#include "calculator.c"


// i KNOW that all with are shorter/equal than rep
// otherwise this would be BAD
void replace(char *in, char *rep, char *with)
{
  int lenRep = strlen(rep);
  int lenWith = strlen(with);
  while (1)
  {
    char *where = strstr(in, rep);
    if (where==0) return;
    
    strncpy(where, with, lenWith);
    char *pos = where+lenWith;
    strcpy(pos, where+lenRep);  
  }
  
}

int evaluateInputIniLine(const char *valueString)
{
  char buf[1024];
  strcpy(buf, valueString);
  replace(buf, "JOYPAD_1_DIGITAL", "4096");
  replace(buf, "JOYPAD_2_DIGITAL", "8192");
  replace(buf, "JOYPAD_1_ANALOG", "16384");
  replace(buf, "JOYPAD_2_ANALOG", "32768");
  
  replace(buf, "JOYPAD_UP", "256");
  replace(buf, "JOYPAD_DOWN", "512");
  replace(buf, "JOYPAD_LEFT", "1024");
  replace(buf, "JOYPAD_RIGHT", "2048");

  replace(buf, "BUTTON_AND", "65536");

  replace(buf, "BUTTON_1_1", "1");
  replace(buf, "BUTTON_1_2", "2");
  replace(buf, "BUTTON_1_3", "4");
  replace(buf, "BUTTON_1_4", "8");
  replace(buf, "BUTTON_2_1", "16");
  replace(buf, "BUTTON_2_2", "32");
  replace(buf, "BUTTON_2_3", "64");
  replace(buf, "BUTTON_2_4", "128");

  replace(buf, "|", "+"); // this is not CORRECT, but should suffice for what I use this for!
  
  // now we should have a line, that we can "calculate"
  simple_queue_t output;
  simple_stack_t ops;

  queue_init(&output);
  stack_init(&ops);

  /* Convert to RPN */
  shunting_yard(buf, &ops, &output);

  /* Compute result */
  int result = compute_rpn(&output);
//  printf("%s -> %s -> result: %i\n", valueString, buf, result);
  return result;
}
int evaluateLine(const char *valueString, NameReplacePair switchMap[])
{
  int i=0;
  char buf[1024];
  strcpy(buf, valueString);

  while (switchMap[i].replace != 0)
  {
    replace(buf, switchMap[i].replace, switchMap[i].with);
    i++;
  }
  // now we should have a line, that we can "calculate"
  simple_queue_t output;
  simple_stack_t ops;

  queue_init(&output);
  stack_init(&ops);

  /* Convert to RPN */
  shunting_yard(buf, &ops, &output);

  /* Compute result */
  int result = compute_rpn(&output);
  return result;
}


char *dipHelp[]=
{
  "ITEM SETTINGS", // title
  "UP/DOWN SCROLLS THROUGH",
  "ITEMS.",
  "LEFT/RIGHT CHANGES SETTINGS.",
  "BUTTON 4 TO EXIT.",
  0
};


int getSelectedItem(DipSwitchSetting *dip)
{
  int selectedValue = *dip->destination;
  int i=0;
  while (dip->values[i].name !=0)
  {
    if (dip->values[i].value == selectedValue) return i;  
    i++;
  }
  return 0;
}


void v_getDipSettings(DipSwitchSetting *dips[], char * title)
{
  if (dips[0] ==0) return;
  
  int saveClientHz =   clientHz;
  clientHz = 50;
  
  int item = 0;
  int itemValue;
  int doingSomething = 0;
  v_noSound();
  int starting = 1;
  itemValue = getSelectedItem(dips[item]);

  int tlen = strlen(title);
  while (1) 
  {
    v_WaitRecal();
    v_setBrightness(0x40);
    v_readButtons();
    v_readJoystick1Analog();
    
    v_printString(0-tlen*5, 100, title, 10, 0x50);
    
    if (!(currentButtonState&0x0f)) starting = 0;
    
    if ((currentButtonState&0x0f) == (0x01)) // exactly button 1
    {
      if (!starting) displayHelp(dipHelp,0);
    }
    if ((currentButtonState&0x0f) == (0x08)) // exactly button 4
    {
      if (!starting) break;
    }
    
    if ((ABS(internalJoy1X)<20) && (ABS(internalJoy1Y)<20)) 
      doingSomething =0;

    if (doingSomething == 0)
    {
      if (internalJoy1Y<-80)
      {
        if (dips[item+1] != 0)
        {
          item++;
          itemValue = getSelectedItem(dips[item]);
        }
        doingSomething=1;
      }
      if (internalJoy1Y>80)
      {
        if (item > 0)
        {
          item--;
          itemValue = getSelectedItem(dips[item]);
        }
        doingSomething=1;
      }
    }
   
    if (doingSomething == 0)
    {
      if (internalJoy1X>80)
      {
        if (dips[item]->values[itemValue+1].name != 0)
        {
          itemValue++;
          *dips[item]->destination = dips[item]->values[itemValue].value;
        }
        doingSomething=1;
      }
      if (internalJoy1X<-80)
      {
        if (itemValue > 0)
        {
          itemValue--;
          *dips[item]->destination = dips[item]->values[itemValue].value;
        }
        doingSomething=1;
      }
    }
    
    int y = 60;
    int i = 0;
    while(dips[i] != 0)
    {
      int intensity = 0x20;
      if (i==item) intensity = 0x50;
      v_printString(-60, y, dips[i]->name, 5, intensity);
      
      int currentValue = getSelectedItem(dips[i]);
      v_printString(40, y, dips[i]->values[currentValue].name, 5, intensity);
      y-=20;
      i++;
    }
    
    
  }
  // save?
  clientHz = saveClientHz;
  
}


// -1 on error
// length of sample else
int v_loadRAW(const char *filename, unsigned char *largeEnoughBuffer)
{
  FILE *fileRead;
  fileRead = fopen(filename, "rb");
  if (fileRead == 0)
  {
    printf("File (%s) could not be opened: %s!\r\n",filename, getErrorText(errno));
    return -1;
  }
  int len = __filelength(fileRead);
  int loadLen = fread(largeEnoughBuffer, len, 1, fileRead);
  if (1 != loadLen)
  {
    printf("Read(1) of %s fails (len loaded: %i) (Error: %i)\r\n", filename, len, errno);
    fclose(fileRead);
    return -1;
  }
  fclose(fileRead);
  printf("File loaded successfully (%s, %i)!\r\n",filename,len);
  return len;
}

void v_displayAbsolutList_8(const signed char list[], int brightness)
{
  int count = (unsigned char) (*list++);

  while (count >0)  
  {
    int y0 = *list++;
    int x0 = *list++;
    int y1 = *list++;
    int x1 = *list++;
    v_directDraw32(x0<<7, y0<<7,x1<<7,y1<<7, brightness);
    count --;
  }
}


void v_displayAbsolutList_8o(int x, int y, const signed char list[], int brightness)
{
  int count = (unsigned char) (*list++);

  while (count >0)  
  {
    int y0 = y+*list++;
    int x0 = x+*list++;
    int y1 = y+*list++;
    int x1 = x+*list++;
    v_directDraw32(x0<<7, y0<<7,x1<<7,y1<<7, brightness);
    count --;
  }
}






const signed char arrowDown[]=
{   (signed char) 6, // count of vectors
    (signed char) 0xFF, (signed char) 0xFD, (signed char) 0xFF, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x03, (signed char) 0xFB, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xFB, (signed char) 0x00, (signed char) 0xFF, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xFD, (signed char) 0x02, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x03, (signed char) 0xFF, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x00, (signed char) 0x02, (signed char) 0xFD, // y0, x0, y1, x1
};

const signed char arrowUp[]=
{   (signed char) 6, // count of vectors
    (signed char) 0x01, (signed char) 0xFD, (signed char) 0x01, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x03, (signed char) 0x05, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x05, (signed char) 0x00, (signed char) 0x01, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0xFD, (signed char) 0xFE, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0x03, (signed char) 0x01, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x00, (signed char) 0xFE, (signed char) 0xFD, // y0, x0, y1, x1
};


static char *_toUpper(char *t)
{
  static char _ubuf[256];
  char *p = _ubuf;
  do
  {
    *p++ = toupper(*t++);
  } while (*t != 0);
  *p = 0;
  return _ubuf;
}


DisplayItem it00;
DisplayItem it01;
DisplayItem it02;
DisplayItem it03;
DisplayItem it04;
DisplayItem it05;
DisplayItem it06;
DisplayItem it07;
DisplayItem it08;
DisplayItem it09;
DisplayItem it10;
DisplayItem it11;
DisplayItem it12;
DisplayItem it13;
DisplayItem it14;
DisplayItem it15;
DisplayItem it16;
DisplayItem it17;
DisplayItem it18;
DisplayItem it19;
DisplayItem it20;
DisplayItem it21;
DisplayItem it22;
DisplayItem it23;
DisplayItem it24;
DisplayItem it25;
DisplayItem it26;
DisplayItem it27;
DisplayItem it28;
DisplayItem it29;


DisplayList defaultDisplayList=
{
  0, // x
  0, // y
  2, // lineSpacing
  
  1, // displayArrows
  1, // rollover

  0, // startYOffset
  0, // itemOffset, f
  5, // itemDisplayCount

  0, // itemCount
  0, // currentMainItem

  0, // autoRaster
  0, // displayRectangle
  1, // fade
  30,// fontWidth
  -4,// fontHeight
  alternate_rasterlines,
  8, // rasterlineCount

  {
    &it00, &it01, &it02, &it03,&it04, &it05,&it06, &it07, &it08, &it09,
    &it10, &it11, &it12, &it13,&it14, &it15,&it16, &it17, &it18, &it19,
    &it20, &it21, &it22, &it23,&it24, &it25,&it26, &it27, &it28, &it29
  }
};

void itemListSetPos(int x, int y)
{
  defaultDisplayList.x=x;
  defaultDisplayList.y=y;
}

void itemListClear()
{
  defaultDisplayList.itemCount=0;
  defaultDisplayList.itemOffset=0;
  defaultDisplayList.autoRaster=0;
  defaultDisplayList.displayRectangle=0;
  defaultDisplayList.displayArrows=1;
  defaultDisplayList.fade=1;
  defaultDisplayList.lineSpacing=2;

  defaultDisplayList.startYOffset=0;
  defaultDisplayList.currentMainItem=-1;
  defaultDisplayList.rollover = 1;
}

void itemListAdd(char* text, int brightness)
{
  if (defaultDisplayList.itemCount+1 == ITEM_MAX_ITEMS) return;
  DisplayItem *item = defaultDisplayList.items[defaultDisplayList.itemCount];

  strncpy(item->text,  _toUpper(text), ITEM_MAX_STRINGSIZE-1);
  item->baseBrightness = brightness;

  defaultDisplayList.itemCount++;
}

// always positive
void itemListScrollDown(int step)
{
  defaultDisplayList.startYOffset -= step;
  if (defaultDisplayList.startYOffset< -defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing)
  {
    defaultDisplayList.startYOffset += (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing);

    defaultDisplayList.itemOffset--;

    if (defaultDisplayList.rollover)
    {
      if (defaultDisplayList.itemOffset<0)
        defaultDisplayList.itemOffset += defaultDisplayList.itemCount;
    }
    else
    {
      if (defaultDisplayList.itemOffset<0)
      {
        defaultDisplayList.itemOffset++;
        defaultDisplayList.startYOffset -= (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing);
        defaultDisplayList.startYOffset += step;
      }
    }
  }
}

// always positive
void itemListScrollUp(int step)
{
  defaultDisplayList.startYOffset += step;
  if (defaultDisplayList.startYOffset> defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing)
  {
    defaultDisplayList.startYOffset -= (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing);

    defaultDisplayList.itemOffset++;
    if (defaultDisplayList.rollover)
    {
      if (defaultDisplayList.itemOffset>=defaultDisplayList.itemCount)
        defaultDisplayList.itemOffset -= defaultDisplayList.itemCount;
    }
    else
    {
      if (defaultDisplayList.itemOffset>=defaultDisplayList.itemCount)
      {
        defaultDisplayList.itemOffset--;
        defaultDisplayList.startYOffset += (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing);
        defaultDisplayList.startYOffset -= step;
      }
    }
    
  }
}

static float getPercentFactor(int mid, int val)
{
  if (!defaultDisplayList.fade) return 1;
  if (val >mid) val = mid-val;

  float ret = 1.0 - ((float)val) / ((float)mid);
  if (ret >1) ret = 0;
  return ret;
}

void itemListDisplay()
{
  int moved = 0;
  static int lastDirection = 0;
  
  if (irqJoyDigital) 
  {
    if (currentJoy1Y>0)
    {
      itemListScrollDown(3);
      lastDirection = 1;
      moved= 1;
    }
    if (currentJoy1Y<0)
    {
      itemListScrollUp(3);
      lastDirection = -1;
      moved= -1;
    }
  }
  else if (irqJoyAnalog) 
  {
    // todo analog
    if (currentJoy1Y>0)
    {
      itemListScrollDown(currentJoy1Y/30);
      lastDirection = 1;
      moved= 1;
    }
    if (currentJoy1Y<0)
    {
      itemListScrollUp(-currentJoy1Y/30);
      lastDirection = -1;
      moved= -1;
    }
  }

  if ((defaultDisplayList.autoRaster) && (!moved) && (lastDirection))
  {
    if (defaultDisplayList.startYOffset > (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing)/2)
    {
      defaultDisplayList.itemOffset++;
      if (defaultDisplayList.itemOffset>=defaultDisplayList.itemCount)
        defaultDisplayList.itemOffset -= defaultDisplayList.itemCount;
      defaultDisplayList.startYOffset = 0;
      lastDirection = 0;
    }
    else if (defaultDisplayList.startYOffset < -(defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing)/2)
    {
      defaultDisplayList.itemOffset--;
      if (defaultDisplayList.itemOffset<0)
        defaultDisplayList.itemOffset += defaultDisplayList.itemCount;
      defaultDisplayList.startYOffset = 0;
      lastDirection = 0;
    }
    else 
    {
      defaultDisplayList.startYOffset = 0;
      lastDirection = 0;
    }
  }
  
  if (defaultDisplayList.itemDisplayCount > defaultDisplayList.itemCount) defaultDisplayList.itemDisplayCount = defaultDisplayList.itemCount;
  
  
  int x = defaultDisplayList.x;
  int y = defaultDisplayList.y;
  int yOffset = 0;
  
  int even = defaultDisplayList.itemDisplayCount & 1;
  
  
  float brightnessFactor; // percent, top = 0%, bottom = 0%, center = 100%
  
  int oneDisplayLength = defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing; // assuming 1 above, 1 below, 1 in between and text
  int size = defaultDisplayList.itemDisplayCount*oneDisplayLength;
  int center = size/2;

  y -=center; // start is in the top left corner
  int centerStart = /*center+*/(oneDisplayLength)/2; // + -> start "above"
  
  if (even) centerStart = 0; 
  
  
  // a positive offset srcolls "up"
  // a negative offset srcolls "down"
  centerStart+=defaultDisplayList.startYOffset; // vectrex inverse Y default is start 5 pixel below start of "home" range


  // display center first
  int displayItem;
  if (defaultDisplayList.rollover)
  {
    displayItem = (defaultDisplayList.itemOffset + defaultDisplayList.itemDisplayCount/2)%defaultDisplayList.itemCount;
    if (displayItem>=defaultDisplayList.itemCount) displayItem-=defaultDisplayList.itemCount;
  }
  else
  {
    displayItem = (defaultDisplayList.itemOffset + defaultDisplayList.itemDisplayCount/2);
  }
    
  defaultDisplayList.currentMainItem = displayItem;
  
  brightnessFactor = getPercentFactor(center, abs(centerStart-((oneDisplayLength)/2) ) );
  int b = (int)(defaultDisplayList.items[displayItem]->baseBrightness * brightnessFactor);
  v_setBrightness(b);    // adjust brightness with position offset

  if (displayItem<defaultDisplayList.itemCount) 
    v_printStringRasterFont( x, y+centerStart,defaultDisplayList.items[displayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);

  if (defaultDisplayList.displayRectangle>0)
  {
    int x1 = x-10;
    int x2 = x1+defaultDisplayList.displayRectangle;

    int y1;
    if (!even)
      y1 = y+(oneDisplayLength)-2;
    else
      y1 = y+(oneDisplayLength)/2-2;
    int y2 = y1-(oneDisplayLength);
    
    x1 = x1 << 7;
    x2 = x2 << 7;
    y1 = y1 << 7;
    y2 = y2 << 7;
  
    v_directDraw32 (x2, y1, x1, y1, 0x40);
    v_directDraw32 (x1, y1, x1, y2, 0x40);
    v_directDraw32 (x1, y2, x2, y2, 0x40);
    v_directDraw32 (x2, y2, x2, y1, 0x40);
  }
  
  
  int upper=0;
  // display items "above"
  for (int i=0; i<defaultDisplayList.itemDisplayCount/2; i++)
  {
    int updisplayItem = displayItem-(1+i);
    
    if (defaultDisplayList.rollover)
    {
      if (updisplayItem<0) updisplayItem+=defaultDisplayList.itemCount;

      brightnessFactor = getPercentFactor(center, abs(centerStart+(1+i)*(oneDisplayLength)-((oneDisplayLength)/2)  ));
      b = (int)(defaultDisplayList.items[updisplayItem]->baseBrightness * brightnessFactor);
      v_setBrightness(b);    // adjust brightness with position offset
      v_printStringRasterFont( x, y+centerStart+(1+i)*(oneDisplayLength),defaultDisplayList.items[updisplayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);
    }
    else
    {
      if ((updisplayItem>=0) && (updisplayItem<defaultDisplayList.itemCount))
      {
        brightnessFactor = getPercentFactor(center, abs(centerStart+(1+i)*(oneDisplayLength)-((oneDisplayLength)/2)  ));
        b = (int)(defaultDisplayList.items[updisplayItem]->baseBrightness * brightnessFactor);
        v_setBrightness(b);    // adjust brightness with position offset
        v_printStringRasterFont( x, y+centerStart+(1+i)*(oneDisplayLength),defaultDisplayList.items[updisplayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);
      }
    }
    upper = y+(2+i)*(oneDisplayLength);
  }
  
  int downer=0;
  // display items "below"
  for (int i=0; i<(defaultDisplayList.itemDisplayCount-1)/2; i++)
  {
    int downdisplayItem = displayItem+(1+i);
    if (defaultDisplayList.rollover)
    {
      if (downdisplayItem>=defaultDisplayList.itemCount) downdisplayItem-=defaultDisplayList.itemCount;

      brightnessFactor = getPercentFactor(center, abs(centerStart-(1+i)*(oneDisplayLength)-((oneDisplayLength)/2)  ));
      b = (int)(defaultDisplayList.items[downdisplayItem]->baseBrightness * brightnessFactor);

      v_setBrightness(b);    // adjust brightness with position offset
      v_printStringRasterFont( x, y+centerStart-(1+i)*(oneDisplayLength),defaultDisplayList.items[downdisplayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);
    }
    else
    {
      if ((downdisplayItem>=0) && (downdisplayItem<defaultDisplayList.itemCount))
      {
        brightnessFactor = getPercentFactor(center, abs(centerStart-(1+i)*(oneDisplayLength)-((oneDisplayLength)/2)  ));
        b = (int)(defaultDisplayList.items[downdisplayItem]->baseBrightness * brightnessFactor);

        v_setBrightness(b);    // adjust brightness with position offset
        v_printStringRasterFont( x, y+centerStart-(1+i)*(oneDisplayLength),defaultDisplayList.items[downdisplayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);
      }
    }
    downer= y-(1+2)*(oneDisplayLength);
  }

  if (defaultDisplayList.displayArrows)
  {
    if (!defaultDisplayList.fade)
    {
      v_displayAbsolutList_8o(0, upper+20, arrowUp, (moved==1) ? 0x40:0x20);
      v_displayAbsolutList_8o(0, downer-20,arrowDown, (moved==-1) ? 0x40:0x20);
    }
    else
    {
      v_displayAbsolutList_8o(0, upper+5, arrowUp, (moved==1) ? 0x40:0x20);
      v_displayAbsolutList_8o(0, downer-5,arrowDown, (moved==-1) ? 0x40:0x20);
    }
    v_setBrightness(64);
  }

}

#include "ymStuff.i"


