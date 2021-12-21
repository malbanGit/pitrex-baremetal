/*
Small and stupid wireframe-with-hidden-line-removal Doom level renderer
*/
/*
 *  Copyright (C) 2015 Jeroen Domburg <jeroen at spritesmods.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */



typedef struct __attribute__((packed)) {
	int16_t x;
	int16_t y;
} DoomVertex;

typedef struct __attribute__((packed)) {
	int16_t vFrom;	//start vertex
	int16_t vTo;	//end vertex
	int16_t flags;
	int16_t types;
	int16_t tag;
	int16_t sdRight;	//right side sidedef index
	int16_t sdLeft;		//left side sidedef index
} DoomLine;

typedef struct __attribute__((packed)) {
	int16_t xOff;
	int16_t yOff;
	char texUp[8];
	char texDwn[8];
	char texMid[8];
	int16_t sector;
} DoomSide;

typedef struct __attribute__((packed)) {
	int16_t vStart;	//vertex start
	int16_t vEnd;	//vertex end
	int16_t angle;
	int16_t linedef; //linedef index
	int16_t dir;
	int16_t offset;
} DoomSeg;

typedef struct __attribute__((packed)) {
	int16_t sCount; //segment count
	int16_t sStart; //start segment index
} DoomSsec;

typedef struct __attribute__((packed)) {
	int16_t zFloor;
	int16_t zCeil;
	char fFloor[8];
	char fCeil[8];
	int16_t lightLevel;
	int16_t flags;
	int16_t tag;
} DoomSec;

typedef struct __attribute__((packed)) {
	int16_t yhi;
	int16_t ylo;
	int16_t xlo;
	int16_t xhi;
} DoomBbox;

typedef struct __attribute__((packed)) {
	DoomVertex lineStart;
	DoomVertex lineD;
	DoomBbox bbRight;
	DoomBbox bbLeft;
	int16_t rChild;
	int16_t lChild;
} DoomNode;

static DoomVertex *dmvert;
static DoomLine *dmline;
static DoomSec *dmsec;
static DoomSsec *dmssec;
static DoomSeg *dmseg;
static DoomNode *dmnode;
static DoomSide *dmside;
static int dmlinecnt;
static int dmnodecnt;

static float myx=1087;
static float myy=-3504;
static float myz=-34;
static float tgtz=-34;
static float zvel=0;
static float myangle=0;
static float sc=(1.0/4.0);

//Projection and view matrices
static float proj[4][4];
static float view[4][4];

//Zmap bitbuffer
unsigned int zbm[SIZEX*SIZEY/32];
//Zmap full xbuffer
char zbmx[SIZEX];


//External line drawing routines
extern void veLineAdd(int x1, int y1, int x2, int y2);


//Multiply 2 4x4 matrices
static void m4m4Mult(float m1[4][4], float m2[4][4], float res[4][4]) {
	int x, y, p;
	float c;
	for (x=0; x<4; x++) {
		for (y=0; y<4; y++) {
			c=0;
			for (p=0; p<4; p++) {
				c+=m1[p][x]*m2[y][p];
			}
			res[y][x]=c;
		}
	}
}

//Multiply a vector with a 4x4 matrix
static void vm4Mult(float vc[4], float m[4][4], float res[4]) {
	int u, v;
	float c;
	for (u=0; u<4; u++) {
		c=0;
		for (v=0; v<4; v++) {
			c+=m[u][v]*vc[v];
		}
		res[u]=c;
	}
}

//Cross-product the first 3 dimensions of a and b. Res[4]=1.
static void crossProd(float a[4], float b[4], float res[4]) {
	res[0]=a[1]*b[2]-a[2]*b[1];
	res[1]=a[2]*b[0]-a[0]*b[2];
	res[2]=a[0]*b[1]-a[1]*b[0];
	res[3]=1;
}

//Generate the relevant bits of a projection matrix
// http://stackoverflow.com/questions/18404890/how-to-build-perspective-projection-matrix-no-api
// http://rodrigo-silveira.com/3d-programming-transformation-matrix-tutorial/
static void genProjMat(float fov, float aspect, float near, float far) {
	int i;
	for (i=0; i<16; i++) proj[i/4][i%4]=0;
	float depth=far-near;
	float cotA2=1.0/tan(0.5f*fov);
	proj[0][0]=cotA2*aspect;
	proj[1][1]=cotA2*(1.0/aspect);
	proj[2][2]=(-near-far)/depth;
	proj[2][3]=2*far*near/depth;
	proj[3][2]=1;
}

//Generate the view angle from the players position and angle
static void genViewMat() {
	float tmp[4][4];
	float rot[4][4]={
		{cos(myangle), 0, sin(myangle), 0},
		{0,1,0, 0},
		{-sin(myangle), 0, cos(myangle), 0},
		{0,0,0,1}
	};
	float scale[4][4]={
		{sc,0,0,0},
		{0,sc,0,0},
		{0,0,sc,0},
		{0,0,0,1}
	};
	float trans[4][4]={
		{1,0,0,-myx},
		{0,1,0,-myz},
		{0,0,1,-myy},
		{0,0,0,1}
	};
	m4m4Mult(trans, rot, tmp);
	m4m4Mult(tmp, scale, view);
}

//Run an (x,y,z) vertex through the view matrix
static void projectv(float x, float y, float z, float ret[4]) {
	float in[4]={x,y,z,1};
	vm4Mult(in, view, ret);
}

//Run a view vector through the projection matrix; do a divide-by-w
static void projectvtop(float in[4], float ret[4]) {
	int j;
	vm4Mult(in, proj, ret);
	for (j=0; j<3; j++) {
		ret[j]/=ret[3];
	}
}


#ifdef DBGMAP
//Debug function: use this instead of projectvtop to get a map view
static void corrvformap(float in[4], float ret[4]) {
	ret[0]=in[0]/1000;
	ret[1]=-in[2]/1000;
	ret[2]=in[1]/1000;
	ret[3]=in[3];
}
#endif

#if 0
//Applies the view and projection matrix
static void project(float x, float y, float z, float ret[4]) {
	float tmp[4];
	projectv(x,y,z,tmp);
	projectvtop(tmp,ret);
}
#endif

//Clear the Z buffer
static void zbmClear() {
	int i;
	for (i=0; i<(SIZEX*SIZEY)/32; i++) zbm[i]=0;
	for (i=0; i<SIZEX; i++) zbmx[i]=0;
}

//Draw a vertical line in the Z buffer
static void zbmVLine(int x, int y1, int y2) {
	int y;
	//Return if x is out of bounds
	if (x<0 || x>=SIZEX) return;
	//Re-order y1 and y2 if needed
	if (y1>y2) {
		y=y2; y2=y1; y1=y;
	}
	//Shortcut: mark entire row as 1 if we can
	if (y1==0 && y2==SIZEY) {
		zbmx[x]=1;
		return;
	}
	//Clip values
	if (y1<0) y1=0;
	if (y2<0) y2=0;
	if (y1>=SIZEY) y1=SIZEY-1;
	if (y2>=SIZEY) y2=SIZEY-1;

	for (y=y1; y<=y2; y++) {
		if ((y&31)==0 && (y2-y)>=32) {
			//We're at a 32-bit boundary and need to set more than 32 bytes,
			//meaning we can set an entire 32bit value at once.
			zbm[(x*SIZEY+y)/32]=0xffffffff;
			y+=31; //plus the ++ of the loop, this makes 32
		} else {
			//Just set the bit.
			zbm[(x*SIZEY+y)/32]|=(1<<(y&31));
		}
	}
}

//Get a pixel in the Z-buffer
static int zbmPixSet(int x, int y) {
	if (x<0 || y<0 || x>=SIZEX || y>=SIZEY) return 1;
	if (zbmx[x]) return 1;
	return zbm[(x*SIZEY+y)/32] & (1<<(y&31));
}

//Draw a vertical line, where the Z-buffer allows it
static void drawVertLine(int x, int yl, int yh) {
	int drawingLine=0;
	int ly, y;

	for (y=yl; y<yh; y++) {
		if (zbmPixSet(x, y)) {
			if (drawingLine) {
				drawingLine=0;
				veLineAdd(x, ly, x, y-1);
			}
		} else {
			if (!drawingLine) {
				drawingLine=1;
				ly=y;
			}
		}
	}
	if (drawingLine) {
		veLineAdd(x, ly, x, y-1);
	}
}

//Draw (mostly) horizontal line, where the Z-buffer allows it
static void drawHorLine(int x1, int y1, int x2, int y2) {
	int x,y;
	int drawingLine=0;
	int lx, ly, ex, ey;
	int dx=x2-x1;
	int dy=y2-y1;
	if (dx==0) return;
	for (x=x1; x<=x2; x++) {
		y=y1+((dy*(x-x1))/dx);
		if (zbmPixSet(x, y)) {
			if (drawingLine) {
				drawingLine=0;
				veLineAdd(lx, ly, ex, ey);
			}
		} else {
			if (!drawingLine) {
				drawingLine=1;
				lx=x; ly=y;
			}
			ex=x; ey=y;
		}
	}
	if (drawingLine) {
		veLineAdd(lx, ly, ex, ey);
	}
}

//Draw a wall
static void drawWall(int x1, int y1, int x2, int y2, int zs, int ze, int flats, int ends) {
	float vba[4], vbb[4], vta[4], vtb[4];
	float ba[4], bb[4], ta[4], tb[4];
	int rx1, rx2, ryh1, ryh2, ryl1, ryl2;
	int x, t;
	int dx, dyh, dyl;
	int cyh, cyl;
	if (zs==ze) return; //no need to draw no-height walls
	if (zs>ze) {
		t=zs; zs=ze; ze=t;
	}

	//Convert linesegs to view first
	projectv(x1, -zs, y1, vba);
	projectv(x2, -zs, y2, vbb);
	projectv(x1, -ze, y1, vta);
	projectv(x2, -ze, y2, vtb);

	//Cut off walls that go behind us.
	if (vba[2]<NEARPT && vbb[2]<NEARPT) return; //can't see line anyway
	//See if line crosses view plane
	if (vba[2]<NEARPT || vbb[2]<NEARPT) {
		//Line does cross view plane. Calculate where so we can cut it off there.
		int cutx=vba[0]+(vbb[0]-vba[0])*((vba[2]-NEARPT)/(vba[2]-vbb[2]));
		if (vba[2]<NEARPT) {
			vba[2]=NEARPT;
			vba[0]=cutx;
			vta[2]=NEARPT;
			vta[0]=cutx;
		} else {
			vbb[2]=NEARPT;
			vbb[0]=cutx;
			vtb[2]=NEARPT;
			vtb[0]=cutx;
		}
	}


#ifndef DBGMAP
	//Project wall to perspective plane
	projectvtop(vba, ba);
	projectvtop(vbb, bb);
	projectvtop(vta, ta);
	projectvtop(vtb, tb);
#else
	corrvformap(vba, ba);
	corrvformap(vbb, bb);
	corrvformap(vta, ta);
	corrvformap(vtb, tb);
#endif

	//Convert to screen size
	rx1=ba[0]*SIZEX+SIZEX/2;
	rx2=bb[0]*SIZEX+SIZEX/2;
	ryl1=ta[1]*SIZEY+SIZEY/2;
	ryl2=tb[1]*SIZEY+SIZEY/2;
	ryh1=ba[1]*SIZEY+SIZEY/2;
	ryh2=bb[1]*SIZEY+SIZEY/2;

	if (rx1==rx2) return; //discard walls we're looking at on a 90 degrees angle

	if (rx1<rx2) {
		if (ends&1) drawVertLine(rx1, ryl1, ryh1);
		if (ends&2) drawVertLine(rx2, ryl2, ryh2);
		drawHorLine(rx1, ryh1, rx2, ryh2);
		drawHorLine(rx1, ryl1, rx2, ryl2);
	} else {
		//We're looking at the _back_ of a segment. Don't draw this as a wall.
		//Do draw the top or bottom lines if we need no fill there!

		//...but first, swap around coords because otherwise we can't draw
		//them at all :X
		t=rx2; rx2=rx1; rx1=t;
		t=ryh2; ryh2=ryh1; ryh1=t;
		t=ryl2; ryl2=ryl1; ryl1=t;

		if (flats==1) drawHorLine(rx1, ryh1, rx2, ryh2);
		if (flats==-1) drawHorLine(rx1, ryl1, rx2, ryl2);
	}

	dx=(rx2-rx1);
	//Flats is 0 if ceiling and floor should be covered, -1 if floor and 1 if ceiling must be covered.
	if (flats>=0) {
		ryl1=0;
		ryl2=0;
	}
	if (flats<=0) {
		ryh1=SIZEY;
		ryh2=SIZEY;
	}

	dyh=(ryh2-ryh1);
	dyl=(ryl2-ryl1);
	for (x=rx1; x<=rx2; x++) {
		cyh=ryh1+((dyh*(x-rx1))/dx);
		cyl=ryl1+((dyl*(x-rx1))/dx);
		if (x==rx1 || x==rx2) {
			//Leave out corners. We want to be able to connect lines exactly there.
			cyh--;
			cyl++;
		}
#ifndef DBGMAP
		zbmVLine(x, cyl, cyh);
#endif
	}
}

//Draw a subsector
static void drawSsec(int ssec) {
	int i;
	DoomVertex *v1, *v2;
	DoomLine *l;
	int secR, secL, t, endl=0, endr=0;
	for (i=dmssec[ssec].sStart; i<dmssec[ssec].sStart+dmssec[ssec].sCount; i++) {
		v1=&dmvert[dmseg[i].vStart];
		v2=&dmvert[dmseg[i].vEnd];
		l=&dmline[dmseg[i].linedef];
		
		if (v1->x==dmvert[l->vFrom].x && v1->y==dmvert[l->vFrom].y) endr=1;
		if (v2->x==dmvert[l->vTo].x && v2->y==dmvert[l->vTo].y) endl=1;
		
		secR=dmside[l->sdRight].sector;
		if (l->sdLeft==-1) {
			drawWall(v1->x, v1->y, v2->x, v2->y, dmsec[secR].zFloor, dmsec[secR].zCeil, 0, (endl<<1)+endr);
		} else {
			secL=dmside[l->sdLeft].sector;
			if (dmseg[i].dir) {
				//Seg runs opposite to line. Switch secL and secR so we're in the direction
				//assumed by the seg.
				t=secL; secL=secR; secR=t;
			}
			if (dmsec[secR].zFloor>dmsec[secL].zFloor) {
				drawWall(v2->x, v2->y, v1->x, v1->y, dmsec[secL].zFloor, dmsec[secR].zFloor, -1, (endr<<1)+endl);
			} else {
				drawWall(v1->x, v1->y, v2->x, v2->y, dmsec[secR].zFloor, dmsec[secL].zFloor, -1, (endl<<1)+endr);
			}
			if (dmsec[secR].zCeil<dmsec[secL].zCeil) {
				drawWall(v2->x, v2->y, v1->x, v1->y, dmsec[secL].zCeil, dmsec[secR].zCeil, 1, (endr<<1)+endl);
			} else {
				drawWall(v1->x, v1->y, v2->x, v2->y, dmsec[secR].zCeil, dmsec[secL].zCeil, 1, (endl<<1)+endr);
			}
		}
	}
	
}

//Check if contents within a bounding box should be visible.
static int bbVisible(DoomBbox *bb) {
	float vp[4][4];
	int j;
	int flag=0;
	//Every time we look one unit further, the fov will deviate dfov units
	//more from the center of view.
	static float dfov=-1;
	if (dfov==-1) dfov=sin(FOV/2.0)/cos(FOV/2.0);

	projectv(bb->xlo, 0, bb->ylo, vp[0]);
	projectv(bb->xhi, 0, bb->ylo, vp[1]);
	projectv(bb->xhi, 0, bb->yhi, vp[2]);
	projectv(bb->xlo, 0, bb->yhi, vp[3]);

	//Discard if entirely behind player
	flag=0;
	for (j=0; j<4; j++) {
		if (vp[j][2]>=NEARPT) flag=1;
	}
	if (!flag) return 0;
	//Discard if entirely too far away
	flag=0;
	for (j=0; j<4; j++) {
		if (vp[j][2]<FARPT) flag=1;
	}
	if (!flag) return 0;

	//Any bboxes left are in the 'butter zone': not too far away, not
	//too near. We still have a huge horizontal range to deal with. We can strip away
	//all bboxes that fall outside the fov too.
	flag=0;
	for (j=0; j<4; j++) {
		if ((vp[j][2]*dfov)>vp[j][0]) flag=1;
	}
	if (!flag) return 0;

	flag=0;
	for (j=0; j<4; j++) {
		if ((vp[j][2]*-dfov)<vp[j][0]) flag=1;
	}
	if (!flag) return 0;

	return 1;
}

//Recursively render the contents of a bsp node.
static void renderBspNode(int node) {
	if (node&0x8000) {
		//Node is a SSECTOR. Draw it.
		node&=0x7fff;
		drawSsec(node);
	} else {
		float cp[4], toMe[4], ld[4];
		//Calculate cross product of node line and vector to player pos to
		//figure out if player is to the left or to the right of node line
		toMe[0]=dmnode[node].lineStart.x-myx;
		toMe[1]=dmnode[node].lineStart.y-myy;
		toMe[2]=0;
		ld[0]=dmnode[node].lineD.x;
		ld[1]=dmnode[node].lineD.y;
		ld[2]=0;
		crossProd(toMe, ld, cp);
		//First draw front, then back node.
		if (cp[2]>0) {
			if (bbVisible(&dmnode[node].bbLeft)) renderBspNode(dmnode[node].lChild);
			if (bbVisible(&dmnode[node].bbRight)) renderBspNode(dmnode[node].rChild);
		} else {
			if (bbVisible(&dmnode[node].bbRight)) renderBspNode(dmnode[node].rChild);
			if (bbVisible(&dmnode[node].bbLeft)) renderBspNode(dmnode[node].lChild);
		}
	}
}

int coordInBb(DoomBbox *bb, int x, int y) {
	return (bb->xlo<=x && bb->xhi>x && bb->ylo<=y && bb->yhi>y);
}

float linesCross(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
	int dx, dy, det;
	//Quick checks to see if the lines can cross.
	if (ax1>bx1 && ax2>bx1 && ax1>bx2 && ax2>bx2) return -1;
	if (ax1<bx1 && ax2<bx1 && ax1<bx2 && ax2<bx2) return -1;
	if (ay1>by1 && ay2>by1 && ay1>by2 && ay2>by2) return -1;
	if (ay1<by1 && ay2<by1 && ay1<by2 && ay2<by2) return -1;


	dx=bx1-ax1;
	dy=by1-ay1;
	det=(bx2-bx1)*(ay2-ay1)-(by2-by1)*(ax2-ax1);

	if (det==0) {
		return -1;	//actually not 100% elegant, because the routine can return
					//-1 when lines are intersecting too... ah well.
	}
	return (float)(dy*(bx2-bx1)-dx*(by2-by1))/(float)det;
}

//Recursively find the first line the line described by the args crosses any
//of the lines in the level. Returns -1 if no line is found.
//ToDo: modify to use blockmap instead of bsp?
int findCrossedLine(int node, int x1, int y1, int x2, int y2, int *dir) {
	int r, i, l;
	float cp;
	if (node&0x8000) {
		node&=0x7fff;
		for (i=dmssec[node].sStart; i<dmssec[node].sStart+dmssec[node].sCount; i++) {
			l=dmseg[i].linedef;
			//See if the line we're passed crosses this linedef
			cp=linesCross(x1, y1, x2, y2, 
					dmvert[dmline[l].vFrom].x, dmvert[dmline[l].vFrom].y,
					dmvert[dmline[l].vTo].x, dmvert[dmline[l].vTo].y);
//			printf("cp %f\n", cp);
			//We actually clip on a seg that's slightly longer than a line, to make sure
			//the player can't go through the cracks in linesegs.
			if (cp>=-0.01 && cp<=1.01) return l;
		}
	} else {
		if (coordInBb(&dmnode[node].bbLeft, x1, y1) || coordInBb(&dmnode[node].bbLeft, x2, y2)) {
			r=findCrossedLine(dmnode[node].lChild, x1, y1, x2, y2, dir);
			if (r!=-1) return r;
		}
		if (coordInBb(&dmnode[node].bbRight, x1, y1) || coordInBb(&dmnode[node].bbRight, x2, y2)) {
			r=findCrossedLine(dmnode[node].rChild, x1, y1, x2, y2, dir);
			return r;
		}
	}
	return -1;
}


void voomHandleKeys(int keys) {
	float dist=0;
	float newx, newy;
	int i, dir, sd;
	int needstrafe=1;
	int iter=2;
	float cp[4], dd[4], ld[4];
	float mydx, mydy;

	if (keys&VOOM_KEY_UP) dist=20;
	if (keys&VOOM_KEY_DOWN) dist=-20;
	if (keys&VOOM_KEY_LEFT) myangle=myangle+0.06;
	if (keys&VOOM_KEY_RIGHT) myangle=myangle-0.06;
	if (dist==0) return; //No keys pressed, we can bail out early.
	mydx=sin(-myangle);
	mydy=cos(-myangle);
	//Normalize dist on angle
	if (dist<0) {
		dist=-dist;
		mydx=-mydx;
		mydy=-mydy;
	}
	newx=myx+mydx*dist;
	newy=myy+mydy*dist;
	while(needstrafe && iter>0) {
		needstrafe=0;
		//See in which sector this makes us end up by recursively drilling down the BSP and checking
		//the linesegs that are in there.
		i=findCrossedLine(dmnodecnt-1, myx, myy, myx+mydx*(dist*1.1), myy+mydy*(dist*1.1), &dir);


		if (i!=-1) {
			//Check if we went to the left or to the right of the lineseg
			dd[0]=mydx*dist;
			dd[1]=mydy*dist;
			dd[2]=0;
			ld[0]=dmvert[dmline[i].vTo].x-dmvert[dmline[i].vFrom].x;
			ld[1]=dmvert[dmline[i].vTo].y-dmvert[dmline[i].vFrom].y;
			ld[2]=0;
			crossProd(dd, ld, cp);
			//Somehow, this sometimes goes wrong... if strafing into a wall, it will sometimes
			//select the wrong side, making us pass through walls we shouldn't be able to pass through.
			if (cp[2]>0) sd=dmline[i].sdRight; else sd=dmline[i].sdLeft;
			printf("sdright %d sdleft %d\n", dmline[i].sdRight, dmline[i].sdLeft);
			if (sd>=0) {
				printf("Crossed line: r=%i and l=%i, going %s (because cross prod is %f), curr z %f\n", 
							dmsec[dmside[dmline[i].sdRight].sector].zFloor,
							dmsec[dmside[dmline[i].sdLeft].sector].zFloor,
							(cp[2]>0)?"l to r": "r to l",
							cp[2],
							-tgtz-PLHEIGHT);
				if ((dmsec[dmside[sd].sector].zFloor-((-myz)-PLHEIGHT))>PLMAXJUMP) {
					needstrafe=1;
				} else {
					tgtz=-dmsec[dmside[sd].sector].zFloor-PLHEIGHT;
				}
			} else {
				needstrafe=1;
			}
			if (needstrafe) {
				//Do motion clipping: the new motion vector is basically the projection of the earlier
				//vector on the wall we ran into.
				float dp=dd[0]*ld[0]+dd[1]*ld[1];
				float lenLine=sqrt(ld[0]*ld[0]+ld[1]*ld[1]);
				float lenMove=sqrt(dd[0]*dd[0]+dd[1]*dd[1]);
				dp=dp/(lenLine*lenMove);
				if (cp[2]>0) dp=-dp;
//				printf("dp %f\n", dp);
				dist=lenLine;
				mydx=dp*(ld[0]/lenLine);
				mydy=dp*(ld[1]/lenLine);
				newx=myx+(mydx*lenMove);
				newy=myy+(mydy*lenMove);
				iter--;
				printf("Strafed. Iter=%d, dx=%f, dy=%f\n", iter, newx-myx, newy-myy);
			}
		}
	}
	if (iter==0) return;
	myx=newx; myy=newy;
}

void voomInit(char** lvlData, int *chunkSz) {
	genProjMat(FOV, (float)(SIZEY)/(float)(SIZEX), NEARPT, FARPT);
	dmvert=(DoomVertex *)lvlData[0];
	dmline=(DoomLine *)lvlData[1];
	dmsec=(DoomSec *)lvlData[2];
	dmssec=(DoomSsec *)lvlData[3];
	dmseg=(DoomSeg *)lvlData[4];
	dmside=(DoomSide *)lvlData[5];
	dmnode=(DoomNode *)lvlData[6];
	dmlinecnt=chunkSz[1]/sizeof(DoomLine);
	dmnodecnt=chunkSz[6]/sizeof(DoomNode);
}


void voomDraw() {
	if (myz<tgtz) {
		//fall
		zvel+=2;
		myz+=zvel;
	}
	if (myz>tgtz) {//hop on ledge and/or fix rounding errors in fall routine
		myz=tgtz; 
		zvel=0;
	}
	genViewMat();
	zbmClear();
	renderBspNode(dmnodecnt-1);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
