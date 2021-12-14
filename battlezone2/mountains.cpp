#include "mountains.h"
#include "game.h"

#define VIRTUAL_SCREEN_WIDTH 2.0f
#define VIRTUAL_SCREEN_HEIGHT 2.0f

mountains::mountains(void)
{
	mMountainHeading = 0;
}


mountains::~mountains(void)
{
}

void mountains::run()
{
	// Volcano bits
	if (mathutils::frandFrom0To1() * 10 < 1)
	{
		Point3d pos(5.055,.17,0);
		Point3d angle(0,0,0);
		float speed = 6;
		float spread = .2;
		int num = 1;
		int timeToLive = 100;
		vector::pen pen = vector::penGreen();
		pen.r *= 1.2;
		pen.g *= 1.2;
		pen.b *= 1.2;
		pen.a = .8;
		pen.lineRadius=3;
		mParticles2d.emitter(&pos, &angle, speed, spread, num, &pen, timeToLive, TRUE, .95);
	}

	mParticles2d.run();
}

void mountains::draw(float heading)
{

	float mountainData[] = 
	{
		-498.333, 395.667, -374, 377, 0, 
		-374, 377, -338.333, 396, 0,
		-356, 386.667, -322, 358.667, 0,
		-322, 358.667, -304.333, 377.333, 0,
		-312.333, 382, -268.667, 358, 0,
		-268.667, 358, -234, 377.333, 0,
		-234, 377.333, -252, 358.667, 0,
		-252, 358.667, -268.667, 358.667, 0,
		-268.333, 396.333, -197.667, 358, 0,
		-197.667, 358, -161.667, 377.333, 0,
		-161.667, 377.333, -126.667, 386, 0,
		-126.667, 386, -74.333, 396.333, 0,
		-197.667, 358.667, -163, 396.667, 0,
		-182, 376, -145.333, 396.333, 0,
		-22.333, 395.667, 13, 366.667, 0,
		13, 366.667, 30.333, 396.333, 0,
		13, 366.667, 31.667, 377, 0,
		31.667, 377, 83.333, 395.667, 0,
		31.667, 377, 66.333, 358, 0,
		66.333, 358, 154.333, 396.667, 0,
		154.333, 396.667, 83.667, 377.333, 0,
		83.667, 377.333, 66.333, 358, 0,
		154.333, 396, 243, 377, 0,
		243, 377, 277.333, 377, 0,
		243, 377, 276.667, 396, 0,
		277.333, 377, 330.333, 396, 0,
		277.333, 377, 313, 357.667, 0,
		313, 357.667, 329.333, 357.667, 0,
		329.333, 357.667, 348.333, 377.333, 0,
		348.333, 377.333, 313.333, 358.333, 0,
		330.333, 396, 348.333, 377.333, 0,
		348.333, 377.333, 365, 367, 0,
		365, 367, 382.667, 377.333, 0,
		330.333, 396, 382.667, 377.333, 0,
		382.667, 377.333, 417.667, 358, 0,
		417.667, 358, 453.333, 377.333, 0,
		453.333, 377.333, 488, 391.333, 0,
		401, 367, 417.667, 376.667, 0,
		417.667, 376.667, 434, 367, 0,
		435.334, 395.667, 488, 391.333, 0,
		488, 391.333, 506.334, 376.667, 0,
		506.334, 376.667, 523.667, 376.667, 0,
		506.334, 376.667, 470, 366.667, 0,
		470, 366.667, 453.333, 377.333, 0,
		470, 366.667, 487, 390.667, 0,
		523.667, 376.667, 559, 395.667, 0,
		523.667, 376.667, 593.667, 396, 0,
		682.333, 396, 734.666, 377.667, 0,
		734.666, 377.667, 753, 396.667, 0,
		734.666, 377.667, 752.333, 359, 0,
		752.333, 359, 787.666, 396.333, 0,
		752.666, 359.667, 769.333, 396.667, 0,
		770, 377, 787.666, 359, 0,
		787.666, 359, 805, 377.667, 0,
		805, 377.667, 839.333, 358.333, 0,
		839.333, 358.333, 875.666, 396.333, 0,
		839.333, 358.333, 857.333, 396.333, 0,
		857.666, 377.333, 876, 367.333, 0,
		876, 367.333, 928, 396.333, 0,
		-1256, 395.667, -1167.334, 376.667, 0,
		-1167.334, 376.667, -1132.334, 376.667, 0,
		-1132.334, 376.667, -1009.334, 396, 0,
		-1009.334, 396, -1027.667, 367.333, 0,
		-1027.667, 367.333, -1063.667, 387.333, 0,
		-1009, 397, -955.667, 358, 0,
		-955.667, 358, -921, 382, 0,
		-929, 396.667, -894.333, 339, 0,
		-894.333, 339, -892.5, 344, 0,
		-892.5, 344, -889.833, 339.833, 0,
		-889.833, 339.833, -888.125, 343.625, 0,
		-888.125, 343.625, -885.333, 339.667, 0,
		-885.333, 339.667, -850.667, 396.667, 0,
		-850.667, 396.667, -798, 358.333, 0,
		-798, 358.333, -762.333, 396.333, 0,
		-798, 358.333, -726.667, 396, 0,
		-762, 377, -726.334, 358, 0,
		-726.334, 358, -656.333, 396.333, 0,
		-690.667, 377, -656.667, 367.333, 0,
		-656.667, 367.333, -639.333, 396.667, 0,
		-656.667, 367.333, -603.333, 396.333, 0,
		-1257.5, 395.5, 998, 395.5, 0,

		// Moon
		-50.602, 326.72, -58.312, 328.61, 1,
		-58.312, 328.61, -66.991, 326.45, 1,
		-66.991, 326.45, -74.752, 321.138, 1,
		-48.917, 289.63, -40.952, 298.453, 1,
		-40.952, 298.453, -38.808, 305.384, 1,
		-38.808, 305.384, -39.012, 313.487, 1,
		-39.012, 313.487, -42.688, 320.507, 1,
		-42.688, 320.507, -50.602, 326.72, 1,
		-74.854, 320.868, -66.787, 322.938, 1,
		-66.787, 322.938, -59.537, 320.958, 1,
		-59.537, 320.958, -51.572, 313.487, 1,
		-51.572, 313.487, -45.751, 305.294, 1,
		-45.751, 305.294, -45.751, 297.103, 1,
		-45.751, 297.103, -48.917, 289.63, 1,
		-51.98, 313.756, -51.368, 316.097, 1,
		-51.368, 316.097, -48.406, 314.926, 1,
		-48.406, 314.926, -52.695, 322.039, 1,
		-52.695, 322.039, -56.882, 321.498, 1,
		-56.882, 321.498, -56.78, 318.707, 1,
		-43.403, 295.842, -43.403, 300.974, 1,
		-43.403, 300.974, -41.156, 301.603, 1,
		-41.156, 301.603, -41.871, 303.043, 1,
		-41.871, 303.043, -39.931, 301.962, 1,

		// Moon (light color)
		-77.714, 302.594, -74.344, 295.392, 2,
		-74.344, 295.392, -67.399, 290.35, 2,
		-67.399, 290.35, -57.698, 287.56, 2,
		-57.698, 287.56, -48.917, 289.63, 2,
		-74.752, 321.138, -79.041, 311.326, 2,
		-79.041, 311.326, -77.714, 302.594, 2,

		-1, -1, -1, -1, 0
	};


/*
	// Blank out the area where the mountains are to get rid of any stars that may otherwise show up there
	// Could also do this by checking the y position of the stars as we draw them...

	glDisable(GL_BLEND);
    glColor4f(0, 0, 0, 1);

	glBegin(GL_QUADS);
	glVertex3f(-1, .15, 0 );
	glVertex3f(1, .15, 0 );
	glVertex3f(1, -.05, 0 );
	glVertex3f(-1, -.05, 0 );
	glEnd();

	glEnable(GL_BLEND);
*/

	const double scale = 270.0f;

	///////////////////////////////////////////////
	// Calculate the extent of the mountain data

	double minX = 32000, minY = 32000;
	double maxX = -32000, maxY = -32000;

	int i=0;
	while ((mountainData[i] != -1) && (mountainData[i+1] != -1))
	{
		Point3d fromvertex, tovertex;

		fromvertex.y = mountainData[i++];
		fromvertex.x = mountainData[i++];
		tovertex.y = mountainData[i++];
		tovertex.x = mountainData[i++];

		int color = mountainData[i++];

		fromvertex.x /= scale;
		fromvertex.y /= scale;
		tovertex.x /= scale;
		tovertex.y /= scale;

		if (fromvertex.x > maxX) maxX = fromvertex.x;
		if (fromvertex.x < minX) minX = fromvertex.x;

		if (fromvertex.y > maxY) maxY = fromvertex.y;
		if (fromvertex.y < minY) minY = fromvertex.y;

		if (tovertex.x > maxX) maxX = tovertex.x;
		if (tovertex.x < minX) minX = tovertex.x;

		if (tovertex.y > maxY) maxY = tovertex.y;
		if (tovertex.y < minY) minY = tovertex.y;
	}

	double rangeX = maxX - minX;
	double rangeY = maxY - minY;

	///////////////////////////////////////////////

	if (theScene->mShowingLogo)
	{
	    mMountainHeading = 1;
	}
	else
	{
	    mMountainHeading += heading * 1.5;
	}

	float myHeading = mMountainHeading;

	myHeading = fmod((double)myHeading, rangeY);
	myHeading -= rangeY;


/*
	myHeading = fmod((double)myHeading, rangeY);
	if (myHeading >= (-VIRTUAL_SCREEN_WIDTH/2))
	{
		myHeading -= rangeY;
	}
	if (myHeading < 0 && myHeading > (-VIRTUAL_SCREEN_WIDTH/2))
	{
		myHeading -= rangeY;
	}
*/

	double xoff = myHeading;  
	double zoff = 1.46; // .88


	matrix mat;
	mat.Translate(0, -game::mPlayers.mPlayer1.getRotation().x * 2, 0);
	mat.Rotate(0,0,game::mPlayers.mPlayer1.getRotation().z*1);


	// LINES

	vector::pen pen = vector::penGreen();
	pen.a *= 1;
//    glColor4f(pen.r, pen.g, pen.b, pen.a);

	//while (xoff <= VIRTUAL_SCREEN_WIDTH)
	for (int j=0; j<3; j++)
	{
		Point3d t;
		mat.TransformVertex(Point3d(xoff,zoff,0), &t);

		int i=0;
		while ((mountainData[i] != -1) && (mountainData[i+1] != -1))
		{
			Point3d fromvertex, tovertex;

			fromvertex.x = mountainData[i++];
			fromvertex.y = -mountainData[i++];
			tovertex.x = mountainData[i++];
			tovertex.y = -mountainData[i++];

			int color = mountainData[i++];

			Point3d tfrom, tto;
			mat.TransformVertex(fromvertex, &tfrom);
			mat.TransformVertex(tovertex, &tto);

			tfrom.x /= scale;
			tfrom.y /= scale;
			tto.x /= scale;
			tto.y /= scale;

			tfrom.x += t.x;
			tfrom.y += t.y;
			tto.x += t.x;
			tto.y += t.y;

			vector::pen p(pen);
			if (color == 1)
				p.a *= 2;
			else if (color == 2)
				p.a *= .4;
			else
				p.a *= 1.2;

			vector::drawVectorLine(tfrom, tto, p, p);
		};
		xoff += rangeY;
	};  


	myHeading = fmod((double)myHeading, rangeY);
	mParticles2d.draw(myHeading, mat);

}

