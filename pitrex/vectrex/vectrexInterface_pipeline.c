// if zeroing is enabled -we can not moveor draw
// ensure it is off!


// if zeroing is enabled -we can not moveor draw
// ensure it is off!

// moving from the current position, without bothering about the beam illumination state
// zeroing off is assumed

/*
a) only large positioning moves
b) only where scale > sms
c) sss must be with small vectors/moves
*/
// change _y to ((_y)+(ABS(_y))*softwareYCalibration)
// change _x to ((_x)+(ABS(_x))*softwareXCalibration)

#define PL_CONTINUE_TO(_x, _y) \
do{ \
  if (cpb->force & PL_BASE_FORCE_USE_FIX_SIZE) \
  { \
    timingNow = cpb->timingForced; \
    if ((ABS((((_y)+(ABS(_y))*softwareYCalibration))/(timingNow+SCALE_STRENGTH_DIF))>127) || (ABS((_x)/(timingNow+SCALE_STRENGTH_DIF))>127)) \
    { /* we must use another scaling!!! -> OR build two vectors! */ \
      SET_OPTIMAL_SCALE_SMS(((_x)+(ABS(_x))*softwareXCalibration), ((_y)+(ABS(_y))*softwareYCalibration),cpb->sms); \
      timingNow = currentScale; \
    } \
  } \
  else \
  { \
    SET_OPTIMAL_SCALE_SMS(((_x)+(ABS(_x))*softwareXCalibration), ((_y)+(ABS(_y))*softwareYCalibration),cpb->sms); \
    timingNow = currentScale; \
  } \
  rampingNow = 1; \
  pl[fpc].yStart = (cpb->yStart); /* before offset and user scaling */\
  pl[fpc].xStart = (cpb->xStart); \
  pl[fpc].yEnd = (cpb->yEnd); /* before offset and user scaling */\
  pl[fpc].xEnd = (cpb->xEnd); \
  pl[fpc].y = ((((_y)+(ABS(_y))*softwareYCalibration))/(timingNow+SCALE_STRENGTH_DIF)); \
  pl[fpc].x = ((((_x)+(ABS(_x))*softwareXCalibration))/(timingNow+SCALE_STRENGTH_DIF)); \
  pl[fpc].y0 = (cpb->y0); \
  pl[fpc].x0 = (cpb->x0); \
  pl[fpc].yRest = ((_y)+(ABS(_y))*softwareYCalibration)-(pl[fpc].y*(timingNow+SCALE_STRENGTH_DIF)); \
  pl[fpc].xRest = ((_x)+(ABS(_x))*softwareXCalibration)-(pl[fpc].x*(timingNow+SCALE_STRENGTH_DIF)); \
  if (currentY == pl[fpc].y) \
  { \
  /* not need to set Y at all */  \
  } \
  else \
  { \
    pl[fpc].flags = pl[fpc].flags | PL_Y_MUST_BE_SET; \
    if (currentMUX != MUX_Y) \
      pl[fpc].flags = pl[fpc].flags | PL_MUX_Y_MUST_BE_SET; \
    currentMUX = MUX_Y; \
    /* we must set Y */ \
    if ((pl[fpc].y==0) && (currentY!=0)) \
    {  \
        if ((crankyFlag&CRANKY_NULLING_CALIBRATE) && (!calibDone)) \
        { \
            if (calibrationValue == 0) \
            { \
              pl[fpc].flags = pl[fpc].flags | PL_CALIBRATE_0 | PL_MUX_Y_MUST_BE_SET; /* because integrator reset, when y = 0 */  \
              currentA =0x0; \
              calibDone = 1;\
            } \
            else \
            { /* costs much time - perhaps we can leave that out? */ \
              if (pl[fpc].type != PL_MOVE) \
              {\
                pl[fpc].flags = pl[fpc].flags | PL_CALIBRATE | PL_MUX_Y_MUST_BE_SET;  /* because integrator reset, when y = 0 */  \
                currentA =0x100;\
                calibDone = 1;\
              } \
            } \
        } \
        else if (crankyFlag&CRANKY_NULLING_WAIT) \
        { \
            pl[fpc].flags = pl[fpc].flags | PL_Y_DELAY_TO_NULL; /* because integrator reset, when y = 0 */  \
        } \
    } \
    if (currentA == pl[fpc].y) \
    { \
      ;/* no need to load A */ \
    } \
    else \
    { \
      /* we must set VIA_a */ \
      currentA = pl[fpc].y; \
      pl[fpc].flags = pl[fpc].flags | PL_Y_A_MUST_BE_SET; \
    } \
    currentY = currentA; /* store A to Y */ \
  } \
  if (currentA != pl[fpc].x) \
  { \
    pl[fpc].flags = pl[fpc].flags | PL_X_A_MUST_BE_SET; \
  } \
  currentA = pl[fpc].x; \
  if (currentMUX != MUX_X) \
    pl[fpc].flags = pl[fpc].flags | PL_MUX_X_MUST_BE_SET; \
  currentMUX = MUX_X; \
  if ((t1_timingSet&0xff) == (timingNow&0xff)) \
    pl[fpc].flags = pl[fpc].flags | PL_T1_LO_EQUALS; \
  t1_timingSet=timingNow; \
} while (0)


#define INIT_NEXT_PIPELINE_ITEM \
  pl[fpc].this_timing = timingNow; \
  pl[fpc].last_timing = timingLast; \
  fpc++; \
  /*pl[fpc].base=0;*/ \
  pl[fpc].debug[0]=0; \
  pl[fpc].flags = 0; \
  if (lastMustFinish) \
    pl[fpc].flags = pl[fpc].flags | PL_LAST_MUST_FINISH; \
  if (rampingNow) \
    pl[fpc].flags = pl[fpc].flags | PL_LAST_IS_RAMPING;  \
  timingLast = timingNow; \
  timingNow = 0; \
  rampingLast = rampingNow;  \
  rampingNow = 0;



#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

#define ADD_CLIPPED_VECTOR(_x0,_y0,_x1,_y1, baseVector) \
do { \
  cpb->x0 = _x0; \
  cpb->y0 = _y0; \
  cpb->x1 = _x1; \
  cpb->y1 = _y1; \
  cpb->intensity = baseVector->intensity; \
  cpb->force = baseVector->force | PL_BASE_FORCE_NOT_CLIPPED; \
  cpb->force &= ~PL_BASE_FORCE_EMPTY; \
  cpb->sms = MAX_USED_STRENGTH; \
  cpb = &pb[++pipelineCounter]; \
  cpb->debug[0] = 0; \
  cpb->pattern = 0; \
  cpb->force = 0; \
  } while (0)

#define ADD_PIPELINE(_x0,_y0,_x1,_y1, intens) \
do { \
  cpb->x0 = _x0; \
  cpb->y0 = _y0; \
  cpb->x1 = _x1; \
  cpb->y1 = _y1; \
  cpb->intensity = intens; \
  cpb->force = cpb->force | PL_BASE_FORCE_NOT_CLIPPED | PL_BASE_FORCE_STABLE; \
  cpb->sms = MAX_USED_STRENGTH; \
  cpb = &pb[++pipelineCounter]; \
  cpb->debug[0] = 0; \
  cpb->pattern = 0; \
  cpb->force = 0; \
  } while (0)



enum {TOP = 0x1, BOTTOM = 0x2, RIGHT = 0x4, LEFT = 0x8};
enum {FALSE, TRUE};
typedef unsigned int outcode;
outcode compute_outcode(int x, int y, int xmin, int ymin, int xmax, int ymax)
{
  outcode oc = 0;
  if (y > ymax)
    oc |= TOP;
  else if (y < ymin)
    oc |= BOTTOM;
  if (x > xmax)
    oc |= RIGHT;
  else if (x < xmin)
    oc |= LEFT;
  return oc;
}

// returns x1 = 1000000 on complete outside!
void cohen_sutherlandCustom(int32_t *x1, int32_t *y1,  int32_t *x2, int32_t *y2, int xmin, int ymin, int xmax, int ymax)
{
  int accept;
  int done;
  outcode outcode1, outcode2;
  accept = FALSE;
  done = FALSE;
  outcode1 = compute_outcode(*x1, *y1, xmin, ymin, xmax, ymax);
  outcode2 = compute_outcode(*x2, *y2, xmin, ymin, xmax, ymax);
  do
  {
    if (outcode1 == 0 && outcode2 == 0)
    {
      accept = TRUE;
      done = TRUE;
    }
    else if (outcode1 & outcode2)
    {
      done = TRUE;
    }
    else
    {
      int x, y;
      int outcode_ex = outcode1 ? outcode1 : outcode2;
      if (outcode_ex & TOP)
      {
        x = *x1 + (*x2 - *x1) * (ymax - *y1) / (*y2 - *y1);
        y = ymax;
      }
      else if (outcode_ex & BOTTOM)
      {
        x = *x1 + (*x2 - *x1) * (ymin - *y1) / (*y2 - *y1);
        y = ymin;
      }
      else if (outcode_ex & RIGHT)
      {
        y = *y1 + (*y2 - *y1) * (xmax - *x1) / (*x2 - *x1);
        x = xmax;
      }
      else
      {
        y = *y1 + (*y2 - *y1) * (xmin - *x1) / (*x2 - *x1);
        x = xmin;
      }
      if (outcode_ex == outcode1)
      {
        *x1 = x;
        *y1 = y;
        outcode1 = compute_outcode(*x1, *y1, xmin, ymin, xmax, ymax);
      }
      else
      {
        *x2 = x;
        *y2 = y;
        outcode2 = compute_outcode(*x2, *y2, xmin, ymin, xmax, ymax);
      }
    }
  } while (done == FALSE);
  if (accept == TRUE)
  {
    return;
  }
  *x1 = 1000000;
  return;
}

// reuses current baseline
void cohen_sutherland(VectorPipelineBase *baseVector, int xmin, int ymin, int xmax, int ymax)
{
  int x1 = baseVector->x0;
  int y1 = baseVector->y0;
  int x2 = baseVector->x1;
  int y2 = baseVector->y1;

  int accept;
  int done;
  outcode outcode1, outcode2;
  accept = FALSE;
  done = FALSE;
  outcode1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
  outcode2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);
  do
  {
    if (outcode1 == 0 && outcode2 == 0)
    {
      accept = TRUE;
      done = TRUE;
    }
    else if (outcode1 & outcode2)
    {
      done = TRUE;
    }
    else
    {
      int x, y;
      int outcode_ex = outcode1 ? outcode1 : outcode2;
      if (outcode_ex & TOP)
      {
        x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
        y = ymax;
      }
      else if (outcode_ex & BOTTOM)
      {
        x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
        y = ymin;
      }
      else if (outcode_ex & RIGHT)
      {
        y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
        x = xmax;
      }
      else
      {
        y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
        x = xmin;
      }
      if (outcode_ex == outcode1)
      {
        x1 = x;
        y1 = y;
        outcode1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
      }
      else
      {
        x2 = x;
        y2 = y;
        outcode2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);
      }
    }
  } while (done == FALSE);
  if (accept == TRUE)
  {
    baseVector->x0 =x1;
    baseVector->y0 =y1;
    baseVector->x1 =x2;
    baseVector->y1 =y2;
    return;
  }
  baseVector->force |= PL_BASE_FORCE_EMPTY;
  return;
}

// https://stackoverflow.com/questions/47884592/how-to-reverse-cohen-sutherland-algorithm
// adds new baselines
// invalidates old baseline
void reverse_cohen_sutherland(VectorPipelineBase *baseVector, int xmin, int ymin, int xmax, int ymax)
{
  int x1 = baseVector->x0;
  int y1 = baseVector->y0;
  int x2 = baseVector->x1;
  int y2 = baseVector->y1;
  baseVector->force |= PL_BASE_FORCE_EMPTY;

  int accept;
  int done;
  outcode outcode1, outcode2;
  accept = FALSE;
  done = FALSE;
  outcode1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
  outcode2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);
  do
  {
    if (outcode1 == 0 && outcode2 == 0)
    {
      done = TRUE;
    }
    else if (outcode1 & outcode2)
    {
      accept = TRUE;
      done = TRUE;
    }
    else
    {
      int x, y;
      int outcode_ex = outcode1 ? outcode1 : outcode2;

      if (outcode_ex & TOP)
      {
        x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
        y = ymax;
      }
      else if (outcode_ex & BOTTOM)
      {
        x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
        y = ymin;
      }
      else if (outcode_ex & RIGHT)
      {
        y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
        x = xmax;
      }
      else
      {
        y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
        x = xmin;
      }

      if (outcode_ex == outcode1)
      {
        ADD_CLIPPED_VECTOR(x1,y1,x,y,baseVector);
        x1 = x;
        y1 = y;
        outcode1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
      }
      else
      {
        ADD_CLIPPED_VECTOR(x,y,x2,y2,baseVector);
        x2 = x;
        y2 = y;
        outcode2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);
      }
    }
  } while (done == FALSE);
  if (accept == TRUE)
  {
    ADD_CLIPPED_VECTOR(x1,y1,x2,y2,baseVector);
    return;
  }
  return;
}
/*
VectorPipelineBase *getUnusedVectorPipelineBase()
{
  VectorPipelineBase *newObject = cpb;
  cpb = &pb[++pipelineCounter]; 
  cpb->debug[0] = 0; 
  cpb->pattern = 0; 
  cpb->force = 0; 
  return newObject;
}
*/

void v_setHardClipping(int enabled, int mode, int respectOrientation, int x0, int y0, int x1, int y1)
{
  clipActive = enabled;
  clipMode = mode; // 0 normal clipping, 1 = inverse clipping

  if (respectOrientation)
  {
    x0= x0*sizeX;
    y0= y0*sizeY;
    x1= x1*sizeX;
    y1= y1*sizeY;
    
    if (orientation == 0) ;// normal
    else  if (orientation == 1)
    {
      int32_t x0_t = x0;
      int32_t x1_t = x1;
      x0 = y0;
      y0 = -x0_t;
      x1 = y1;
      y1 = -x1_t;
    }
    else  if (orientation == 2) 
    {
      x0 = -x0;
      y0 = -y0;
      x1 = -x1;
      y1 = -y1;
    }
    else  if (orientation == 3)
    {
      int32_t x0_t = x0;
      int32_t x1_t = x1;
      x0 = -y0;
      y0 = x0_t;
      x1 = -y1;
      y1 = x1_t;
    }     
  }
  if (x0<x1)
  {
    clipminX= x0;
    clipmaxX= x1;
  }
  else
  {
    clipminX= x1;
    clipmaxX= x0;
  }
  if (y0<y1)
  {
    clipminY= y0;
    clipmaxY= y1;
  }
  else
  {
    clipminY= y1;
    clipmaxY= y0;
  }
}
// custom clippings
// are DIRECTLY
// in the coordinates given
// from caller!
// before repositioning, scaling, orientation ....
void v_setCustomClipping(int enabled, int x0, int y0, int x1, int y1)
{
  customClippingEnabled = enabled;

  customClipxMin = x0;
  customClipyMin = y0;
  customClipxMax = x1;
  customClipyMax = y1;

  
  if (x0<x1)
  {
    customClipxMin= x0;
    customClipxMax= x1;
  }
  else
  {
    customClipxMin= x1;
    customClipxMax= x0;
  }
  if (y0<y1)
  {
    customClipyMin= y0;
    customClipyMax= y1;
  }
  else
  {
    customClipyMin= y1;
    customClipyMax= y0;
  } 
}

// Function to compute the angle between two connected vectors
int angle_between_vectors(int Ax, int Ay, int Bx, int By, int Cx, int Cy) 
{
    double v1x = Bx - Ax;
    double v1y = By - Ay;

    double v2x = Cx - Bx;
    double v2y = Cy - By;

    double dot = v1x * v2x + v1y * v2y;
    double len1 = sqrt(v1x * v1x + v1y * v1y);
    double len2 = sqrt(v2x * v2x + v2y * v2y);

    if (len1 == 0 || len2 == 0)
        return -1.0; // invalid input (zero-length vector)

    double cos_angle = dot / (len1 * len2);

    // Clamp to [-1, 1] to avoid domain errors
    if (cos_angle > 1.0) cos_angle = 1.0;
    if (cos_angle < -1.0) cos_angle = -1.0;

    double angle_rad = acos(cos_angle);
    return angle_rad * (180.0 / M_PI); // convert to degrees
}

// from this point in the pipeline remove the "current" item
// if it has less "angle" to the item given before
// it checks whether previous/next elements are 0

// if lines are "connected

// if so, it does nothing
// returns 1 on remove
// 0 on not removed
int angleOptimization(VectorPipelineBase *pip, int minAngle)
{
  VectorPipelineBase *next= pip->next;
  VectorPipelineBase *previous= pip->previous;
  if (next==0) return 0;
  if (previous==0) return 0; // we would need to change the "head" - and we cannot (want not) do this from here!

  if (next->x0 != pip->x1) return 0;
  if (next->y0 != pip->y1) return 0;
  
  int angle = angle_between_vectors(pip->x0, pip->y0, pip->x1, pip->y1, next->x1, next->y1);
  if (angle < minAngle)
  {
    // pip->force = pip->force | PL_BASE_FORCE_EMPTY; // do not forget, but treat as not existant
// printf("Angle: %i\n", angle);
    // remove after all
    // otherwise "checkExternal" have to test for PL_BASE_FORCE_EMPTY-> bothersome
    previous->next = next;
    next->previous = previous;
    
    next->x0 = pip->x0;
    next->y0 = pip->y0;
    return 1;
  }
  return 0;
}

int overlappingOptimization(VectorPipelineBase *base, VectorPipelineBase *pip)
{
  VectorPipelineBase *next= pip->next;
  VectorPipelineBase *previous= pip->previous;
  
  // first and last are not checked
  if (next==0) return 0;
  if (previous==0) return 0; // we would need to change the "head" - and we cannot (want not) do this from here!

  // only not connected vectors
  if ((previous->x1==pip->x0) && (previous->y1==pip->y0)) return 0;
  if ((pip->x1==next->x0) && (pip->y1==next->y0)) return 0;
  
  // only vertical / horizontal testing 
  int vertical = (pip->x0 == pip->x1); 
  int horizontal = (pip->y0 == pip->y1);
  if (vertical == horizontal) return 0; // must be either, not both or none
  
  if (horizontal)
  {
    while (base != (VectorPipelineBase *)0)
    {
      if (base == pip) {base = base->next;continue;}
      if (base->y0 != base->y1) {base = base->next;continue;} // not horizontal
      if (base->y0 != pip->y0) {base = base->next;continue;} // not on the same row

      int remove = 0;
      if ((base->x0<=pip->x0) && (base->x1>=pip->x1)) remove=1;
      if ((base->x0<=pip->x1) && (base->x1>=pip->x0)) remove=1;
      
      if (remove)
      {
        previous->next = next;
        next->previous = previous;
        // printf("Overlapping horizontal removal! \n");        
        return 1;
      }
      base = base->next;
    }
    return 0;
  }
  if (vertical)
  {
    while (base != (VectorPipelineBase *)0)
    {
      if (base == pip) {base = base->next;continue;}
      if (base->x0 != base->x1) {base = base->next;continue;} // not horizontal
      if (base->x0 != pip->x0) {base = base->next;continue;} // not on the same row

      int remove = 0;
      if ((base->y0<=pip->y0) && (base->y1>=pip->y1)) remove=1;
      if ((base->y0<=pip->y1) && (base->y1>=pip->y0)) remove=1;
      
      if (remove)
      {
        previous->next = next;
        next->previous = previous;
        //printf("Overlapping vertical removal! \n");        
        return 1;
      }
      base = base->next;
    }
  }
  return 0;
}

int isPreviousOverlapping(VectorPipelineBase *base)
{
  VectorPipelineBase *previous= base->previous;
  if (previous==0) return 0; 
  
  // only vertical / horizontal testing 
  int vertical = (base->x0 == base->x1); 
  int horizontal = (base->y0 == base->y1);
  if (vertical == horizontal) return 0; // must be either, not both or none
  
  if (horizontal)
  {
    while (previous != (VectorPipelineBase *)0)
    {
      if (previous->y0 != previous->y1) {previous = previous->previous;continue;} // not horizontal
      if (previous->y0 != base->y0) {previous = previous->previous;continue;} // not on the same row

      int remove = 0;
      if ((previous->x0<=base->x0) && (previous->x1>=base->x1)) remove=1;
      if ((previous->x0<=base->x1) && (previous->x1>=base->x0)) remove=1;
      
      if (remove)
      {
        // printf("Overlapping horizontal removal! \n");        
        return 1;
      }
      previous = previous->previous;
    }
    return 0;
  }
  if (vertical)
  {
    while (previous != (VectorPipelineBase *)0)
    {
      if (previous->x0 != previous->x1) {previous = previous->previous;continue;} // not horizontal
      if (previous->x0 != base->x0) {previous = previous->previous;continue;} // not on the same row

      int remove = 0;
      if ((previous->y0<=base->y0) && (previous->y1>=base->y1)) remove=1;
      if ((previous->y0<=base->y1) && (previous->y1>=base->y0)) remove=1;
      
      if (remove)
      {
        //printf("Overlapping vertical removal! \n");        
        return 1;
      }
      previous = previous->previous;
    }
  }
  return 0;
}

// this can be called from normal space!
// return 1 if a new vectorpipeline was created
// returns 0 is not
void handlePipeline()
{
  int interlaced = 0;
  int interlaceCounter = 0;
  static int oddEven = 0;
  oddEven= (oddEven+1)%2;
  
  // build a list of all items
  // and put "dots" last
  // and stables first (hint)
  if ((pipelineCounter > 0) || (clearPipelineIssued))
  {
     clearPipelineIssued = 0;
     pipelineFilled = 1;
  }
  else
  {
    return;
  }
  // the array is converted to a double linked list
  // that way we can later easily manipulate the "array"
  VectorPipelineBase *head=(VectorPipelineBase *)0;
  VectorPipelineBase *element=(VectorPipelineBase *)0;

  // move dots to be done "last" - possibly without ZERO REF
  // in between
  // dots are often fast moving small objects
  // a) they desturb the integrators
  // b) exact position is not so important!
  //
  // last head is for "dots" -> for a short time an own linked list
  // which is later put at the end of the "normal" vectors
  VectorPipelineBase *lastHead=(VectorPipelineBase *)0;
  VectorPipelineBase *lastElement=(VectorPipelineBase *)0;

  VectorPipelineBase *firstHead=(VectorPipelineBase *)0;
  VectorPipelineBase *firstElement=(VectorPipelineBase *)0;

  // clipping is "costly"
  // and is done on the array
  // this way "reverse" clipping can more easily
  // add new vectors to the end, since
  // we still are in "addition" mode of the pipeline
  
// for debugging and testing only
// comment out, when not testing  
/*
  if (customClippingEnabled)
  {
      int xmin = customClipxMin*sizeX+offsetX;
      int ymin = customClipyMin*sizeY+offsetY;
      int xmax = customClipxMax*sizeX+offsetX;
      int ymax = customClipyMax*sizeY+offsetY;
    
      int xStart=xmin;
      int yStart=ymin;
      int xEnd=xmax;
      int yEnd=ymax;

      if (orientation == 0) ;// normal
      else  if (orientation == 1)
      {
        int32_t xStart_t = xmin;
        int32_t xEnd_t = xmax;
        xStart = ymin;
        yStart = -xmin;
        xEnd = ymax;
        yEnd = -xmax;
      }
      else  if (orientation == 2) 
      {
        xStart = -xmin;
        yStart = -ymin;
        xEnd = -xmax;
        yEnd = -ymax;
      }
      else  if (orientation == 3)
      {
        int32_t xStart_t = xmin;
        int32_t xEnd_t = xmax;
        xStart = -ymin;
        yStart = xmin;
        xEnd = -ymax;
        yEnd = xEnd_t;
      }     
      yStart = yStart & (~POSTION_MARGIN_AND);
      xStart = xStart & (~POSTION_MARGIN_AND);
      yEnd = yEnd & (~POSTION_MARGIN_AND);
      xEnd = xEnd & (~POSTION_MARGIN_AND);
    
      ADD_PIPELINE(xStart+1, yStart+1, xStart+1, yEnd-1, 90);
      ADD_PIPELINE(xEnd-1, yStart+1, xEnd-1, yEnd-1, 90);
      ADD_PIPELINE(xStart+1, yStart+1, xEnd-1, yStart+1, 90);
      ADD_PIPELINE(xStart+1, yEnd-1, xEnd-1, yEnd-1, 90);
  }
*/  
  if (clipActive)
  {
    if (isIRQMode)
    {
      // add a "window"
      ADD_PIPELINE(clipminX, clipminY, clipminX, clipmaxY, 90);
      ADD_PIPELINE(clipminX, clipmaxY, clipmaxX, clipmaxY, 90);
      ADD_PIPELINE(clipmaxX, clipmaxY, clipmaxX, clipminY, 90);
      ADD_PIPELINE(clipmaxX, clipminY, clipminX, clipminY, 90);
    }
#if RASPPI != 1 
    else if (isSMPMode) 
    {
      // add a "window"
      ADD_PIPELINE(clipminX, clipminY, clipminX, clipmaxY, 90);
      ADD_PIPELINE(clipminX, clipmaxY, clipmaxX, clipmaxY, 90);
      ADD_PIPELINE(clipmaxX, clipmaxY, clipmaxX, clipminY, 90);
      ADD_PIPELINE(clipmaxX, clipminY, clipminX, clipminY, 90);
    }
#endif  
    else
    {
      //v_directDraw32(clipminX, clipminY, clipminX, clipmaxY, 90);

      v_setBrightness(90);
      ZERO_AND_WAIT();
      UNZERO();
      v_directDeltaMove32start_nosizing(clipminX, clipminY);
      v_directDeltaMoveEnd();
      v_directDeltaDraw32start_nosizing(clipminX-clipminX, clipmaxY-clipminY);

      //v_directDraw32(clipminX, clipmaxY, clipmaxX, clipmaxY, 90);
      v_directDeltaDraw32start_nosizing(clipmaxX-clipminX, clipmaxY-clipmaxY);
      //v_directDraw32(clipmaxX, clipmaxY, clipmaxX, clipminY, 90);
      v_directDeltaDraw32start_nosizing(clipmaxX-clipmaxX, clipminY-clipmaxY);
      //v_directDraw32(clipmaxX, clipminY, clipminX, clipminY, 90);
      v_directDeltaDraw32start_nosizing(clipminX-clipmaxX, clipminY-clipminY);
      ZERO_AND_WAIT();
    }
    
    VectorPipelineBase *c_cpb;
    for (int i=0; i<pipelineCounter; i++)
    {
      c_cpb = &pb[i];
      // we can "hint" not to clip
      // e.g. already inverse clipped and added reverse results!
      if (!(c_cpb->force & PL_BASE_FORCE_NOT_CLIPPED))
      {
        if (clipMode==0)
        {
          cohen_sutherland(c_cpb, clipminX, clipminY, clipmaxX, clipmaxY);
        }
        else if (clipMode==1)
        {
          reverse_cohen_sutherland(c_cpb, clipminX, clipminY, clipmaxX, clipmaxY);
        }
      }
    }
  }



  // now we build the linked list
  //
  // adding "stables" to the front
  // in case "stables" are sorted (which is likely)
  // backwards ensured, that stables are correctly ordered!
  // when we move them to the front!
  for (int i=0; i<pipelineCounter; i++)
  {

    if (interlaced)
    {
      interlaceCounter++;
      if ((interlaceCounter & 1) == oddEven)
        continue;
    }


    cpb = &pb[i];

    // stables are added in front of the list head!
    if (cpb->force & PL_BASE_FORCE_STABLE)
    {
      if (firstHead == (VectorPipelineBase *)0)
      {
        firstHead = cpb;
        firstElement = cpb;
        firstElement->next = (VectorPipelineBase *)0;
        firstElement->previous = (VectorPipelineBase *)0;
      }
      else
      {
        cpb->next = (VectorPipelineBase *)0;
        cpb->previous = firstElement;
        firstElement->next = cpb;
        firstElement = cpb;
      }
      continue;
    }
    // MOVE dots to the back of the list!
    if (!keepDotsTogether)
    {
      if ((cpb->x0 == cpb->x1) && (cpb->y0 == cpb->y1))
      {
        // in some cases it might be ok
        // to not zero "stars"
        // for now - we let the algorythm decide
        // and do not force to NOT ZERO
        //      if (!(cpb->force & PL_BASE_FORCE_ZERO))
        //        cpb->force |= PL_BASE_FORCE_NO_ZERO; // not sure if that is always a good thing!
        if (lastHead == (VectorPipelineBase *)0)
        {
          lastHead = cpb;
          lastElement = cpb;
          lastElement->next = (VectorPipelineBase *)0;
          lastElement->previous = (VectorPipelineBase *)0;
        }
        else
        {
          cpb->previous = lastElement;
          lastElement->next = cpb;
          lastElement = cpb;
          lastElement->next = (VectorPipelineBase *)0;
        }
        continue;
      }
    }

    // everything not dot and not stable
    // is just added to the normal list
    if (head == (VectorPipelineBase *)0)
    {
      head = cpb;
      element = cpb;
      element->next = (VectorPipelineBase *)0;
      element->previous = (VectorPipelineBase *)0;
    }
    else
    {
      cpb->previous = element;
      element->next = cpb;
      element = cpb;
      element->next = (VectorPipelineBase *)0;
    }
  }
  if (lastHead != 0)
  {
    // add dots last...
    if (head == (VectorPipelineBase *)0)
    {
      head = lastHead;
    }
    if (element != (VectorPipelineBase *)0)
    {
      element->next = lastHead;
      lastHead->previous = element;
    }
  }
  if (firstHead != 0)
  {
    if (head != 0)
    {
      firstElement->next = head;
      head->previous = firstElement;
    }
    head = firstHead;
  }

  pipelineCleanup = 1;
  int cleanupCount =0;
  if (pipelineCleanup)
  {
    // remove double vectors
    // remove vectors that have lie fully on top of each other (also larger on smaller, not just an exact fit)
    
    // remove inverse double vectors
    // remove dots that sit on start/end points
    // todo
    
    cpb = head;
    int c_count = 0;
    while (cpb != (VectorPipelineBase *)0)
    {
        /*      
        typedef struct {
          int yStart;
          int xStart;
          int yEnd;
          int xEnd;
          int y0;
          int x0;
          int y1; // int8_t fontWidth;
          int x1; // int8_t fontHeight;
          int intensity;
          int pattern; // halfOffset
          int sms;
          int timingForced;
          int force;
        #ifdef USE_PERSISTENT_VECTORS  
          int id; // 
        #endif  
          
          unsigned char **rlines;
          void *next;
          void *previous;
          char debug[240]; // reused for raster strings!
        } VectorPipelineBase;
        */
      
        // angle
        int success =0;
        if (doAngleOptimization)
          success = angleOptimization(cpb, angleValue);

        if (doOverlappingOptimization)
          if (!success) success += overlappingOptimization(head, cpb);

        cleanupCount +=success;
        c_count++;
        cpb = cpb->next;
      }
#ifdef MY_DEBUG
      if (myDebug)   printf ("CleanupCount: %i (from: %i)\n", cleanupCount, c_count);
#endif  
      
    }
  
  
  cpb = head;

  
  
  
  
  
  
  // todo
  // might sort for brightness
  // supposedly all same brightnesses will build a (or several) "clusters"

  // now we calculate our "optimal"
  // VectorPipeline
  // for that we keep track of a hell of a lot
  // single items
  // like position, and contents of registers...
  // starts with position 0,0 assumed
  int cx = 0;
  int cy = 0;

  int beamState = 0; // off
  int currentY = 0x100;  // value in reg A (illegal)
  int currentA = 0x100;  // value in reg A (illegal)
  int zeroActive = 1;
  int currentBrightness = 0x100; // invalid
  int timingNow = 0;
  int timingLast = 0;
  int calibLast = 0;


  int t1_timingSet = 0xffff;
  int rampingNow = 0;
  int rampingLast;
  int currentMUX = 0x100;
  int lastMustFinish = 0;

  // before pipeline is called
  // WR does a zero calib!
  int calibDone = 0; // -1 = no, 0 = 0 offset, 1 calibration Tuts done
  #define  MUX_BRIGHTNESS 4
  #define  MUX_Y 0
  #define  MUX_X 1
  #define  MUX_UNKOWN 255


  // since we run in "normal" space
  // we MUST NOT change the vectorpipeline
  // that is used to currently output
  // to the vectrex (that is the vectorPipeLineReading one)
  
  
  VectorPipeline *pl;        // vectorpipeline we write to NOW
  pl = _VPL[vectorPipeLineWriting];        // vectorpipeline we write to NOW
  
  // (since we allways reuse the same array elements, we must ensure they are clean from start)
  int fpc = 0; // final pipeline counter
  pl[fpc].flags = 0;

  // not used anymore...
  consecutiveDraws = 0;
  // when clipping thats not true anymore - but who cares
  // clippin is special anyway
  
  
#ifdef MY_DEBUG
  if (myDebug)  printf("Base pipeline has %i items!\r\n", pipelineCounter);
#endif  
  

  
  while (cpb != (VectorPipelineBase *)0)
  {
// if (vectorPipeLineWriting == vectorPipeLineIsRead) printf("DANGER!!!!!!!!!\n");
    int cv = 0;
    
    
    if ((checkExternal != 0) && (!inSettings))
    {
      cv = checkExternal(&cpb, &pl, &fpc, &cx, &cy);
      if (cv != 0)
      {
        if (cv == CV_SPECIAL_SKIP ) continue;
        if (cv == CV_SPECIAL_NEXT )
        {
          cpb->force = 0;
          cpb->pattern = 0;
          cpb->debug[0] = 0;
          cpb = cpb->next;
          continue;
        }
        if (cv == CV_SPECIAL_DEFLOK )
        {
#ifdef MY_DEBUG
          if (myDebug)  printf("Special DEFLOK!\r\n");
#endif    
          pl[fpc].type = PL_DEFLOK;
          calibDone = 0;
	  calibLast = 0;
          timingNow = 0;
          rampingNow = 0;
          lastMustFinish = 0;
          currentMUX = MUX_X;
          currentA = 0x100;
          currentY=0x100;
          if (beamState)
          {
            pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
            beamState = 0;
          }
          zeroActive = 1;
          cx = 0;
          cy = 0;
          consecutiveDraws = 0;
          INIT_NEXT_PIPELINE_ITEM;
        }
        else 
        {}
      }
    }
    
    
    
    
    // invalidated element... do nothing
    if (cpb->force & PL_BASE_FORCE_EMPTY)
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base EMPTY!\r\n");
#endif      
      // cleanup and next!
      cpb->force = 0;
      cpb = cpb->next;
      continue;
    }

    if (currentBrightness != cpb->intensity)
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base BRIGHTNESS!\r\n");
#endif      
      pl[fpc].type = PL_SET_BRIGHTNESS;
      pl[fpc].intensity = cpb->intensity;
      currentBrightness = cpb->intensity;

      if (currentA != currentBrightness)
      {
        pl[fpc].flags = pl[fpc].flags | PL_I_A_MUST_BE_SET;
        currentA = currentBrightness;
      }

      // if we don't switch the beam off
      // while setting the brightness -
      // bright dots might be possible!
      if (beamState)
      {
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }

      currentMUX = MUX_BRIGHTNESS;
      if (crankyFlag & CRANKY_BETWEEN_VIA_B)
      {
        currentA = 0;
        currentMUX = MUX_X;
      }
      
      
      
      timingNow = 0;
      rampingNow = 0;
      lastMustFinish = 0;
      INIT_NEXT_PIPELINE_ITEM;
    }
    
    
    if ((cpb->force & PL_BASE_FORCE_IS_RASTER_DISPLAY)|| (cpb->force & PL_BASE_FORCE_IS_BI_RASTER_DISPLAY))
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base RASTER!\r\n");
#endif      
      pl[fpc].type = PL_RASTER_TEXT;
      pl[fpc].x = cpb->x0; // 8 bit pos
      pl[fpc].y = cpb->y0;
      pl[fpc].y0 = cpb->y1; // height of font
      pl[fpc].x0 = cpb->x1; // width of font
      pl[fpc].pattern = cpb->pattern; // halfOffset
      pl[fpc].rlines = cpb->rlines; // raster font to use
      pl[fpc].yRest = ((uint8_t)cpb->sms); //movementScale;

      int ii=0;
      for (ii=0;ii<239; ii++)
      {
        if (cpb->debug[ii] == 0) break;
        pl[fpc].debug[ii] = cpb->debug[ii];
      }
      pl[fpc].debug[ii] = 0;


      // leaves with a "zero continuing"

      // is not really needed
      // since raster switches off the beam in itself
      // but here (since we are lazy and raster is an element I rather neglect) we
      // must switch it off twice, since
      // otherwise if previously drawn vector will not get its
      // T1off pause
      // lest I program a special if clause in the "cleanup phase of drawing
      // and I don't want todo that for stupid raster
      if (beamState)
      {
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }
      
      
      zeroActive = 1;
      cx = 0;
      cy = 0;
      rampingNow = 0;
      timingNow = DELAY_ZERO_VALUE;
      lastMustFinish = 1;
      
      calibLast = 0;
      calibDone = 0;
      currentMUX = MUX_X;
      currentA = 0x100;
      currentY = 0;
      
      
      INIT_NEXT_PIPELINE_ITEM;
      // reset base -> ready to be reused next "round"
      cpb->force = 0;
      cpb->pattern = 0;
      cpb->debug[0] = 0;
      cpb = cpb->next;
      
      continue;
    }
    
    if (cpb->force & PL_BASE_FORCE_DEFLOK)
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base DEFLOK!\r\n");
#endif      
      pl[fpc].type = PL_DEFLOK;
      timingNow = 0;
      rampingNow = 0;
      lastMustFinish = 0;
      currentMUX = MUX_X;
      currentA = 0x100;
      calibDone = 0;
      currentY = 0;
      if (beamState)
      {
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }
      zeroActive = 1;
      cx = 0;
      cy = 0;
      consecutiveDraws = 0;
      INIT_NEXT_PIPELINE_ITEM;
      
      // reset base -> ready to be reused next "round"
      cpb->force = 0;
      cpb->pattern = 0;
      cpb->debug[0] = 0;
      cpb = cpb->next;
      
      continue;
    }

    // forcing zero prevents algorythm!
    if (((cpb->force & PL_BASE_FORCE_ZERO )== PL_BASE_FORCE_ZERO) && (!zeroActive))
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base ZERO!\r\n");
#endif      
      pl[fpc].type = PL_ZERO;
      
      // todo in relation to current position?
      timingNow = DELAY_ZERO_VALUE;
      
      calibLast = 0;
      calibDone = 0;
      currentMUX = MUX_X;
      currentA = 0x100;
      currentY = 0;

      
      // todo
      // zero timer in relation to current position
      // the less far from zero -> the less time needed!
      rampingNow = 0;
      lastMustFinish = 1;
      if (beamState)
      {
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }
      zeroActive = 1;
      cx = 0;
      cy = 0;
      consecutiveDraws = 0;
      INIT_NEXT_PIPELINE_ITEM;
    }
    else if (!(cpb->force & PL_BASE_FORCE_NO_ZERO )) // do not force the algorythm if we are not prepared to eventually ZERO
    {
      // VERY EASY algorythm
      // like the "old" one

      // how far away is the cursor from the position we want to start drawing?
      int32_t xMoveDif = cpb->x0-cx;
      int32_t yMoveDif = cpb->y0-cy;

      // test if the position of the last end - and the current start differs by more than our set margin
      if ((ABS(xMoveDif) > POSITION_MARGIN) || (ABS(yMoveDif) > POSITION_MARGIN) )
      {
        // not on the same position, so we either do a MOVE
        // or do a Zero

        // the only left over criteria at the moment is - how far away from the last cursor position
        // if too far away, we throw in a zero ref
        // old:
        //    consecutiveDraws > MAX_CONSECUTIVE_DRAWS;
        //    resetPos += ABS((currentScale-GET_OPTIMAL_SCALE(xMoveDif, yMoveDif)) > 20);
        int resetPos = ((ABS(xMoveDif)>resetToZeroDifMax) || (ABS(yMoveDif)>resetToZeroDifMax) );

//MAX_CONSECUTIVE_DRAWS = 0;
//        if (consecutiveDraws > MAX_CONSECUTIVE_DRAWS)
//          resetPos = true;
        
        
        if ((resetPos) && (!zeroActive))
        {
#ifdef MY_DEBUG
          if (myDebug)  printf("Base CALC ZERO!\r\n");
#endif    
          // do a zeroing!
          // copy paste from above
          pl[fpc].type = PL_ZERO;
          timingNow = DELAY_ZERO_VALUE;

          // zero allways recalibs
          calibDone = 0;
	  calibLast = 0;
          currentMUX = MUX_X;
          currentA = 0x100;
          currentY = 0;

          // todo
          // zero timer in relation to current position
          // the less far from zero -> the less time needed!
          rampingNow = 0;
          lastMustFinish = 1;
          if (beamState)
          {
            pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
            beamState = 0;
          }
          zeroActive = 1;
          cx = 0;
          cy = 0;
          consecutiveDraws = 0;
          INIT_NEXT_PIPELINE_ITEM;
        }
      }
    }

    // the result from above is either
    // a) we are not correctly positioned -> do a MOVE
    // b) we ARE correctly positioned -> do a DRAW
    // both are handled below
    /*****************************************************/
    if ((cpb->force & PL_BASE_FORCE_RESET_ZERO_REF ) && (!calibDone))
    {
#ifdef MY_DEBUG
        if (myDebug)  printf("Base PL_CALIBRATE_0!\r\n");
#endif  
        pl[fpc].flags = pl[fpc].flags | PL_CALIBRATE_0;
        currentA = 0x100;  // value in reg A (illegal)
        currentMUX = MUX_X;
        currentY = 0x100;
        calibDone = 0;
        calibLast = 0;
    }
    if ((cpb->force & PL_BASE_FORCE_CALIBRATE_INTEGRATORS ) && (!calibDone))
    {
#ifdef MY_DEBUG
        if (myDebug)  printf("Base PL_CALIBRATE!\r\n");
#endif  
        pl[fpc].flags = pl[fpc].flags | PL_CALIBRATE;
        currentA = 0x100;  // value in reg A (illegal)
        currentMUX = MUX_X;
        currentY = 0x100;
        calibDone = 1;
        
	calibLast = -1;
    }

    
    
    

    
//calibration with move?    
    
    /************ HANDLE start position of Vector ************/
    // fill secondary "optimized" pipeline with values
    if ((cx != cpb->x0) || (cy != cpb->y0))
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base MOVE!\r\n");
#endif      
      pl[fpc].type = PL_MOVE;

      // we nust move to start the new vector
      if (zeroActive)
      {
        zeroActive = 0;
        pl[fpc].flags = pl[fpc].flags | PL_DEACTIVATE_ZERO;
      }
      if (beamState)
      {
        beamState = 0;
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
      }

      if ((cx==0)&& (cy==0))
      {
	// in moves from zero -> do not calibrate
//        if ((calibrationValue != 0) && (calibDone == 1))
        if (calibDone == 1)
        {
          pl[fpc].flags  = pl[fpc].flags | PL_CALIBRATE_0;
          currentMUX = MUX_X;
          currentA = 0;
          calibDone = 0;
          currentY = 0;
        }
        PL_CONTINUE_TO((cpb->x0-cx), (cpb->y0-cy));
        cx = cpb->x0;
        cy = cpb->y0;
        lastMustFinish = 1;
        INIT_NEXT_PIPELINE_ITEM;

        if (useDoubleTimer)
        {
            if ((ABS(cpb->x0)>MAX_USED_STRENGTH) || (ABS(cpb->y0)>MAX_USED_STRENGTH))
            {
              pl[fpc].type = PL_MOVE;
              PL_CONTINUE_TO(pl[fpc-1].xRest, pl[fpc-1].yRest);
              lastMustFinish = 1;
              INIT_NEXT_PIPELINE_ITEM;
            }
        }
      }
      else
      {
        PL_CONTINUE_TO((cpb->x0-cx), (cpb->y0-cy));
if (calibLast == -1)
{
  if (timingNow<512) calibLast = calibrationValues[timingNow]; else calibLast = calibrationValues[511];
}
	
	
	cx = cpb->x0;
        cy = cpb->y0;
        lastMustFinish = 1;
        INIT_NEXT_PIPELINE_ITEM;
      }
      consecutiveDraws++;
    }
    /*********************************************************/


    /************ HANDLE draw position of Vector ************/
    if ((cx != cpb->x1) || (cy != cpb->y1))
    {
      if (/*(calibrationValue != 0) &&*/ (calibDone != 1))
      {
        // during calibration, the beam MUST be off!
        if (beamState)
          pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
        pl[fpc].flags  = pl[fpc].flags | PL_CALIBRATE;
        currentMUX = MUX_X;
        currentA = 0x100;
        currentY = 0x100;
        calibDone = 1;
        calibLast = -1;
      }

      pl[fpc].type = PL_DRAW;

      // we must move to start the new vector
      if (zeroActive)
      {
        zeroActive = 0;
        pl[fpc].flags = pl[fpc].flags | PL_DEACTIVATE_ZERO;
      }

      // before timer (switch of from last draw)
      if (beamOffBetweenConsecutiveDraws)
      {
        if (beamState)
          pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }
      // after timer (we want to draw and the beam is off -> so switch it on!
      if (!beamState)
      {
        beamState = 1;
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_ON;
      }
      
      //check last one, if also draw, than try to use the same T1
      PL_CONTINUE_TO(cpb->x1-cpb->x0, cpb->y1-cpb->y0);
      cx = cpb->x1;
      cy = cpb->y1;
      
      
if (calibLast == -1)
{
  if (timingNow<512) calibLast = calibrationValues[timingNow]; else calibLast = calibrationValues[511];
  if (calibLast == 0) 
  {
    calibDone = 0;
    pl[fpc].flags  = pl[fpc].flags & (~(PL_CALIBRATE));
  }
}
      
      

      // remember the "base" for possible debug informations
      // todo:
      // while debugging - prevent baselist switching!
      //pl[fpc].base=cpb;
      
#ifdef MY_DEBUG
      if ((browseMode) || (myDebug))
      {
        for (int ii=0;ii<239; ii++)
        {
          pl[fpc].debug[ii] = cpb->debug[ii];
          if (cpb->debug[ii] ==0) break;
        }
      }
#endif     
      
      lastMustFinish = 1;

      if ((cpb->pattern != 0) && (cpb->pattern != 0xff))
      {
#ifdef MY_DEBUG
        if (myDebug)  printf("Base PATTERN!\r\n");
#endif  
        pl[fpc].type = PL_DRAW_PATTERN;
        pl[fpc].pattern = cpb->pattern;
        lastMustFinish = 0;
        rampingNow = 0;
      }
#ifdef MY_DEBUG
      else
      {
        if (myDebug)  printf("Base DRAW!\r\n");
      }
#endif
      INIT_NEXT_PIPELINE_ITEM;
      consecutiveDraws++;
    }
    else
    {
      // start and end coordinates are the same
      // do a "dot"
#ifdef MY_DEBUG
      if (myDebug)  printf("Base DOT!\r\n");
#endif
      // remember the "base" for possible debug informations
      // todo:
      // while debugging - prevent baselist switching!
#ifdef PITREX_DEBUG
      if ((browseMode) || (myDebug))
      {
        for (int ii=0;ii<239; ii++)
        {
          pl[fpc].debug[ii] = cpb->debug[ii];
          if (cpb->debug[ii] ==0) break;
        }
      }
#endif
      pl[fpc].type = PL_DRAW_DOT;
      if (!beamState)
      {
        beamState = 1;
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_ON;
      }
//pl[fpc].flags = pl[fpc].flags |       PL_SWITCH_BEAM_OFF;
      
      // dot dwell?
      // todo dwell for this_timing?
      lastMustFinish = 1;
      cx = cpb->x1;
      cy = cpb->y1;
      rampingNow = 0;
      if (cpb->force & PL_BASE_FORCE_USE_DOT_DWELL)
      {
        timingNow = cpb->timingForced;
        pl[fpc].flags = pl[fpc].flags /*| PL_DEFLOK_AFTER */;

        pl[fpc].x = cx;
        pl[fpc].y = cy;
        zeroActive = 1;
        cx = 0;
        cy = 0;
        lastMustFinish = 1;
        INIT_NEXT_PIPELINE_ITEM;

/// REVERSE SHOT EFFECT!        
        {
          currentMUX = MUX_X;
          currentA = 0x100;
          calibDone = 1;
          zeroActive = 1;
          consecutiveDraws = 0;

          
          
          pl[fpc].type = PL_MOVE;
          pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
//          pl[fpc].flags = pl[fpc].flags | PL_DEACTIVATE_ZERO;
          
          
          PL_CONTINUE_TO(-cpb->x1, -cpb->y1);
          cx = -cpb->x1;
          cy = -cpb->y1;
          lastMustFinish = 1;
          INIT_NEXT_PIPELINE_ITEM;

          pl[fpc].x = cx;
          pl[fpc].y = cy;
          pl[fpc].type = PL_DRAW_DOT;
          timingNow = cpb->timingForced;
          pl[fpc].flags = pl[fpc].flags /*| PL_DEFLOK_AFTER */;
          pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
          pl[fpc].flags = pl[fpc].flags | PL_DEFLOK_AFTER; // get rid of after DOT "spreading"
          
          lastMustFinish = 1;
          zeroActive = 1;
          cx = 0;
          cy = 0;

          currentMUX = MUX_X;
          currentY = 0x100;
          currentA = 0x100;
          calibDone = 1;
          zeroActive = 1;
          consecutiveDraws = 0;
        }
        
        
        
        
      }
      else
      {
        timingNow = v_dotDwell;
      }
      INIT_NEXT_PIPELINE_ITEM;

    }

    if (checkExternal != 0)
    {
      if (cv > 0x80)
      {
        if (cv == 0x80 + CV_SPECIAL_DEFLOK)
        {
#ifdef MY_DEBUG
          if (myDebug)  printf("Special DEFLOK AFTER!\r\n");
#endif    
          pl[fpc].type = PL_DEFLOK;
          timingNow = 0;
          rampingNow = 0;
          lastMustFinish = 0;
          currentMUX = MUX_X;
          currentA = 0x100;
          currentY=0x100;
          calibDone = 0;
          if (beamState)
          {
            pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
            beamState = 0;
          }
          zeroActive = 1;
          cx = 0;
          cy = 0;
          consecutiveDraws = 0;
          INIT_NEXT_PIPELINE_ITEM;
        }
      }
    }    
    
    /*********************************************************/

    // when calibrate?
    // when calibareted calibration
    // user commands to SYNC etc

    // reset base -> ready to be reused next "round"
    cpb->force = 0;
    cpb->pattern = 0;
    cpb->debug[0] = 0;
    cpb = cpb->next;
  }
  // clean up after last item of the list
  if (beamState)
    pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF_AFTER | PL_SWITCH_ZERO_AFTER | PL_SWITCH_BEAM_OFF;

  pl[fpc].type = PL_END;
  pl[fpc].last_timing = timingLast;
  lastPLSize = pipelineCounter;
  // setup next round
  pipelineCounter = 0;
  basePipeLineWriting = basePipeLineWriting?0:1;
  pb = _BPL[basePipeLineWriting]; // current base pipeline 
  cpb = &pb[0];
  
  // tell hyperspace
  // to next use the newly written pipeline
  // to output data with
  

  if (isIRQMode) DisableInterrupts();
#if RASPPI != 1 
  if (isSMPMode)
  {
    getLock();
  }
#endif
  vectorPipeLineNextRead = vectorPipeLineWriting;
  vectorPipeLineWriting = (vectorPipeLineWriting +1)%3;
  if (vectorPipeLineWriting == vectorPipeLineIsRead)
  {
    vectorPipeLineWriting = (vectorPipeLineWriting +1)%3;
  }
#if RASPPI != 1 
  if (isSMPMode) releaseLock();
#endif
  
  //currentWrite = vectorPipeLineWriting;

#ifdef USE_PERSISTENT_VECTORS  

  if (clearNextPipeline == 0)
  {
    VectorPipelineBase *old_pipeline =_BPL[(basePipeLineWriting?0:1)];
    for (int i=0; i<lastPLSize; i++)
    {
      VectorPipelineBase *o_cpb = &old_pipeline[i];
      if ((o_cpb->force & PL_BASE_PERSISTENT_ENTITY) == PL_BASE_PERSISTENT_ENTITY)
      {
        cpb->yStart = o_cpb->yStart;
        cpb->xStart = o_cpb->xStart;
        cpb->yEnd = o_cpb->yEnd;
        cpb->xEnd = o_cpb->xEnd;
        cpb->y0 = o_cpb->y0;
        cpb->x0 = o_cpb->x0;
        cpb->y1 = o_cpb->y1;
        cpb->y1 = o_cpb->x1;
        cpb->intensity = o_cpb->intensity;
        cpb->pattern = o_cpb->pattern;
        cpb->force = o_cpb->force;
        cpb->sms = o_cpb->sms;
        cpb->timingForced = o_cpb->timingForced;
        cpb->id = o_cpb->id;
        cpb->rlines = o_cpb->rlines; 

        int ii=0;
        for (ii=0;ii<239; ii++)
        {
          cpb->debug[ii] = o_cpb->debug[ii];
          if (cpb->debug[ii] == 0) break;
        }
        
        // initnext
        cpb = &pb[++pipelineCounter];
        cpb->pattern = 0;
        cpb->force = 0;
        cpb->debug[0] = 0;
      }
    }
  }
  clearNextPipeline = 0;
#endif  
  if (isIRQMode) EnableInterrupts();
}

void displayPipeline()
{
  
#define __NORMAL_PIPELINE__
      #include "pipeline.i"
#undef __NORMAL_PIPELINE__
}
  
