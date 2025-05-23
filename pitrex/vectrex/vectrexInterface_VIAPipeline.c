
#define MAX_VIA_ITEMS 20000
typedef struct
{
  int reg;
  int value;
  int leastWaitTime; 
  int waitFor;
} VIA_Item;

VIA_Item LAST_DUMMY={-1,0,0,0};

VIA_Item viaPipeline1[MAX_VIA_ITEMS];
VIA_Item viaPipeline2[MAX_VIA_ITEMS];
VIA_Item viaPipeline3[MAX_VIA_ITEMS];

VIA_Item *viaPipeline[3] = {viaPipeline1, viaPipeline2, viaPipeline2};

VIA_Item *viaP_last; // current via pipeline 
VIA_Item *viaP; // current via pipeline 

#define LAST_PIPELINE_ITEM 16
void init_viaPipeline(VIA_Item p[])
{
   viaP = p;
   for (int i=0; i<MAX_VIA_ITEMS-1;i++)
   {
      p->reg=0;
      p->value=0;
      p->leastWaitTime=0;
      p->waitFor=0;
      p++;
   }
    p->reg=LAST_PIPELINE_ITEM;
    p->value=0;
    p->leastWaitTime=0;
    p->waitFor=0;

    viaP_last = &LAST_DUMMY;
}

typedef enum {
  VW_B = 1,
  VW_A = 2,
  VW_DDRB = 4,
  VW_DDRA = 8,
  VW_T1_lo = 16,
  VW_T1_hi = 32,
  VW_T2_lo = 256,
  VW_T2_hi = 512,
  VW_SHIFT = 1024,
  VW_AUX = 2048,
  VW_CNTR = 4096,
  VW_IRQ = 8192,
  VW_IRQ_enable = 16384,
} VIA_WAIT_DEFS;

#define VIA_WAIT_NONE           0
#define VIA_WAIT_ALL            (VW_B+VW_A+VW_DDRB+VW_DDRA+VW_T1_lo+VW_T1_hi+VW_T2_lo+VW_T2_hi+VW_SHIFT+VW_AUX+VW_CNTR+VW_IRQ+VW_IRQ_enable)
#define VIA_WAIT_AFTER_T1_hi    (VW_B+VW_A+VW_T1_hi+VW_AUX+VW_CNTR)

int iZSH;
int iXSH;
int iYSH;

int iVIA_B;
int iVIA_A;
int iVIA_DDRB;
int iVIA_DDRA;
int iVIA_T1_lo;
int iVIA_T1_hi;
int iVIA_T2_lo;
int iVIA_T2_hi;
int iVIA_SHIFT;
int iVIA_AUX;
int iVIA_CNTR;
int iVIA_IRQ;
int iVIA_IRQ_enable;


#define CYCLES_AFTER_VIA_B 4
#define CYCLES_AFTER_VIA_A 4
#define CYCLES_AFTER_VIA_DDRB 4
#define CYCLES_AFTER_VIA_DDRA 4
#define CYCLES_AFTER_VIA_T1_lo 4
#define CYCLES_AFTER_VIA_T1_hi 4
#define CYCLES_AFTER_VIA_T2_lo 4
#define CYCLES_AFTER_VIA_T2_hi 4
#define CYCLES_AFTER_VIA_SHIFT 4
#define CYCLES_AFTER_VIA_AUX 4
#define CYCLES_AFTER_VIA_CNTR 4
#define CYCLES_AFTER_VIA_IRQ 4
#define CYCLES_AFTER_VIA_IRQ_enable 4

#define VIA_FORCEFULL_WAIT 0xffff


#define NEXT_VIA_PIPEPLINE_ITEM \
  do \
  { \
    viaP_last = viaP; \
    viaP++; \
    if (viaP->reg == LAST_PIPELINE_ITEM) viaP--; \
  } \
  while(0)

inline static void SET_VIA_B(int value)
{
    if (value == iVIA_B) return;
    iVIA_B = value;
    
    if (viaP_last->reg == VIA_port_a)
    {
        viaP_last->leastWaitTime += 0;  // possibly add a wait to the last item
    }
    viaP->reg = VIA_port_b;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_B;
    viaP->waitFor=iVIA_A;
    NEXT_VIA_PIPEPLINE_ITEM;
}


inline static void SET_VIA_A(int value)
{
    if (value == iVIA_A) return;
    iVIA_A = value;
    
    if (viaP_last->reg == VIA_port_b)
    {
        viaP_last->leastWaitTime += 0;  // possibly add a wait to the last item
    }
    viaP->reg = VIA_port_a;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_A;
    viaP->waitFor=iVIA_B;

    NEXT_VIA_PIPEPLINE_ITEM;
}

inline static void SET_VIA_DDRB(int value)
{
    if (value == iVIA_DDRB) return;
    iVIA_DDRB = value;
    
    viaP->reg = VIA_DDR_b;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_DDRB;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}

inline static void SET_VIA_DDRA(int value)
{
    if (value == iVIA_DDRA) return;
    iVIA_DDRA = value;
    
    viaP->reg = VIA_DDR_a;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_DDRA;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}

inline static void SET_VIA_T1_lo(int value)
{
    if (value == iVIA_T1_lo) return;
    iVIA_T1_lo = value;
    
    viaP->reg = VIA_t1_cnt_lo;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_T1_lo;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}

inline static void SET_VIA_T1_hi(int value)
{
    if (value == iVIA_T1_hi) return;
    iVIA_T1_hi = value;
    
    viaP->reg = VIA_t1_cnt_hi;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_T1_hi;
    viaP->waitFor=VIA_WAIT_AFTER_T1_hi;

    NEXT_VIA_PIPEPLINE_ITEM;
}

inline static void SET_VIA_T2_lo(int value)
{
    if (value == iVIA_T2_lo) return;
    iVIA_T2_lo = value;
    
    viaP->reg = VIA_t2_cnt_lo;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_T2_lo;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}

inline static void SET_VIA_T2_hi(int value)
{
    if (value == iVIA_T2_hi) return;
    iVIA_T2_hi = value;
    
    viaP->reg = VIA_t2_cnt_hi;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_T2_hi;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}


inline static void SET_VIA_SHIFT(int value)
{
    iVIA_SHIFT = value;
    
    viaP->reg = VIA_shift_reg;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_SHIFT;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}


inline static void SET_VIA_AUX(int value)
{
    if (value == iVIA_AUX) return;
    iVIA_AUX = value;
    
    viaP->reg = VIA_aux_cntl;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_AUX;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}

inline static void SET_VIA_CNTR(int value)
{
    if (value == iVIA_CNTR) return;
    iVIA_CNTR = value;
    
    viaP->reg = VIA_cntl;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_CNTR;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}


inline static void SET_VIA_IRQ(int value)
{
    iVIA_CNTR = value;
    
    viaP->reg = VIA_int_flags;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_IRQ;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}

inline static void SET_VIA_IRQ_enable(int value)
{
    if (value == iVIA_IRQ_enable) return;
    iVIA_IRQ_enable = value;
    
    viaP->reg = VIA_int_enable;
    viaP->value = value;
    viaP->leastWaitTime=CYCLES_AFTER_VIA_IRQ_enable;
    viaP->waitFor=VIA_WAIT_NONE;

    NEXT_VIA_PIPEPLINE_ITEM;
}

// waits at least for the leastWaitTime
// but adds to that the value given!
inline static void SET_FORCEFULL_WAIT_ADD(int value)
{
    viaP->reg = VIA_FORCEFULL_WAIT;
    viaP->value = 0;
    viaP->leastWaitTime=value;
    viaP->waitFor=VIA_WAIT_ALL;

    NEXT_VIA_PIPEPLINE_ITEM;
}


void addToViaPipeline(int x0,int y0,int x1,int y1, int brightness)
{
  
}
