#include "hud.h"
#include "game.h"
#include "enemies.h"

#define HUD_BOTTOM .6

hud::hud(void)
{
}

hud::~hud(void)
{
}

void hud::run()
{
	mRadarHeading -= .06;
	if (mRadarHeading < 0)
	{
		mRadarHeading += 2*PI;
	}
}

void hud::draw()
{
	// Blank out the area at the top of the screen

	drawStatusPanel(TRUE);
	drawStatusPanel(FALSE);

	drawRadarPanel(TRUE);
	drawRadarPanel(FALSE);

	drawLivesPanel(TRUE);
	drawLivesPanel(FALSE);

	bool targeted = game::mPlayers.mPlayer1.isTargetingEnemy();
	drawReticule(TRUE, targeted);
	drawReticule(FALSE, targeted);
}

void hud::drawStatusPanel(bool glowPass)
{
  if (glowPass) return; // malban
	vector::pen pen = vector::penRed();
	pen.a *= .9;
	pen.lineRadius = 2;

	{
		static int blinkTimer = 0;
		static BOOL blinkState = FALSE;
		--blinkTimer;
		if (blinkTimer <= 0)
		{
			blinkState = !blinkState;
			blinkTimer = blinkState ? 30 : 10;
		}

		entity* e = game::mEnemies.getActiveEnemy();
		if (e && blinkState)
		{
			font::AlphanumericsPrint(TRUE, font::ALIGN_LEFT, .013, -.92, .9, &pen, (char *)"enemy in range");
			font::AlphanumericsPrint(FALSE, font::ALIGN_LEFT, .013, -.92, .9, &pen, (char *)"enemy in range");
		}

		int targetDir = game::mPlayers.mPlayer1.getTargetDirection();
		if (targetDir != 0)
		{
			if (blinkState)
			{
				font::AlphanumericsPrint(TRUE, font::ALIGN_LEFT, .013, -.92, .76, &pen, targetDir > 0 ? (char *)"enemy to left" : (char *)"enemy to right");
				font::AlphanumericsPrint(FALSE, font::ALIGN_LEFT, .013, -.92, .76, &pen, targetDir > 0 ? (char *)"enemy to left" : (char *)"enemy to right");
			}
		}
	}

	if (game::mPlayers.mPlayer1.mMotionBlocked)
	{
		static int blinkTimer = 0;
		static bool blinkState = TRUE;
		++blinkTimer;
		if (blinkTimer > 15)
		{
			blinkTimer = 0;
			blinkState = !blinkState;
		}
		if (blinkState)
		{
			font::AlphanumericsPrint(TRUE, font::ALIGN_LEFT, .013, -.92, .83, &pen, (char *)"motion blocked by object");
			font::AlphanumericsPrint(FALSE, font::ALIGN_LEFT, .013, -.92, .83, &pen, (char *)"motion blocked by object");
		}
	}
}

void hud::drawRadarPanel(bool glowPass)
{
  if (glowPass) return; // malban
	float radius = .1;

	// draw the sweep

	Point3d radarPos(0, .82, 0);
	Point3d radarSweep(radius, 0, 0);
	radarSweep = mathutils::rotateScreenPoint(radarSweep, mRadarHeading);
	radarSweep += radarPos;

	vector::pen pen = vector::penRed();
	pen.a *= 1.2;

	vector::drawVectorLine(radarPos, radarSweep, pen, pen);

	// Draw the ticks
	for (int i=0; i<360; i+=90)
	{
		Point3d tickInner(radius*.95, 0, 0);
		tickInner = mathutils::rotateScreenPoint(tickInner, mathutils::DegreesToRads(i));
		tickInner += radarPos;

		Point3d tickOuter(radius*1.1, 0, 0);
		tickOuter = mathutils::rotateScreenPoint(tickOuter, mathutils::DegreesToRads(i));
		tickOuter += radarPos;

		vector::drawVectorLine(tickInner, tickOuter, pen, pen);
	}

	// Draw the FOV
	for (int i=-1; i<=1; i+=2)
	{
		Point3d fovPoint(radius, 0, 0);
		fovPoint = mathutils::rotateScreenPoint(fovPoint, mathutils::DegreesToRads(i*35) + mathutils::DegreesToRads(90));
		fovPoint += radarPos;

		vector::drawVectorLine(radarPos, fovPoint, pen, pen);
	}

	// Draw the enemy blips
	{
		entity* e = game::mEnemies.getActiveEnemy();

		if (e && (e->getState() == entity::ENTITY_STATE_RUNNING))
		{
			Point3d enemyPos = e->getPos();
			float playerHeading = game::mPlayers.mPlayer1.getHeading();
			float blipAngle = mathutils::calculate2dAngle(enemyPos, game::mPlayers.mPlayer1.getPos()) - playerHeading;

			float distance = mathutils::calculate2dDistance(enemyPos, game::mPlayers.mPlayer1.getPos()) * .0003;
			if (distance > radius) distance = radius;

			Point3d blipPoint(distance, 0, 0);
			blipPoint = mathutils::rotateScreenPoint(blipPoint, blipAngle);
			blipPoint += radarPos;

			float a1 = fmod((double)mRadarHeading, 2*PI);
			float a2 = fmod((double)blipAngle, 2*PI);

			if (fabs(a1) > fabs(a2))
			{
				if (fabs(a1) - fabs(a2) < .1)
				{
					if (game::mGameMode == game::GAMEMODE_PLAYING)
					{
						if (!game::mSound.isTrackPlaying(SOUNDID_RADAR_BLIP))
							game::mSound.playTrack(SOUNDID_RADAR_BLIP);
					}
					mBlipBrightness = 2;
				}
			}
			else
			{
				if (fabs(a2) - fabs(a1) < .1)
				{
					if (game::mGameMode == game::GAMEMODE_PLAYING)
					{
						if (!game::mSound.isTrackPlaying(SOUNDID_RADAR_BLIP))
							game::mSound.playTrack(SOUNDID_RADAR_BLIP);
					}
					mBlipBrightness = 2;
				}
			}

			pen.a = mBlipBrightness;
			mBlipBrightness *= .97;

//			glColor4f(pen.r * 1.5, pen.g * 1.5, pen.b * 1.5, pen.a);
//			glVertex3f(blipPoint.x, blipPoint.y, 0 );
drawLine(blipPoint.x, blipPoint.y, blipPoint.x, blipPoint.y, (int)(mBlipBrightness*127.0));

		}
	}
}

void hud::drawLivesPanel(bool glowPass)
{
  if (glowPass) return; // malban
	vector::pen pen = vector::penRed();
	pen.a *= .9;
	pen.lineRadius = 2;


	// Num lives
	float lifeData[] = 
	{
		623.714, 495.715, 632.089, 492.465,
		632.089, 492.465, 636.089, 484.59,
		636.089, 484.59, 675.964, 495.84,
		675.964, 495.84, 668.964, 502.965,
		668.964, 502.965, 630.214, 502.965,
		630.214, 502.965, 623.714, 495.715,
		648.589, 487.965, 672.714, 487.965,
		672.714, 487.965, 672.714, 491.59,
		672.714, 491.59, 661.089, 491.59,
		0,0,0,0
	};

	float scale = 600;

	// LINES
	{
		float x = -672 / scale;
		float y = 502 / scale;

		x += .46;
		y += .89;

		for (int j=0; j<game::mPlayers.mPlayer1.mLives; j++)
		{
			int i=0;
			while ((lifeData[i] != 0) && (lifeData[i+1] != 0))
			{
				Point3d fromvertex, tovertex;

				fromvertex.x = lifeData[i++];
				fromvertex.y = -lifeData[i++];
				tovertex.x = lifeData[i++];
				tovertex.y = -lifeData[i++];

				fromvertex.x /= scale;
				fromvertex.y /= scale;
				tovertex.x /= scale;
				tovertex.y /= scale;

				fromvertex.x += x;
				fromvertex.y += y;
				tovertex.x += x;
				tovertex.y += y;

				vector::drawVectorLine(fromvertex, tovertex, pen, pen);
			};
			x+= .1;
		}
	}
/*
	// GLOW
	{
		vector::pen glowPen(pen);
		glowPen.a *= .15;

		float x = -672 / scale;
		float y = 502 / scale;

		x += .46;
		y += .89;

		for (int j=0; j<game::mPlayers.mPlayer1.mLives; j++)
		{
			int i=0;
			while ((lifeData[i] != 0) && (lifeData[i+1] != 0))
			{
				Point3d fromvertex, tovertex;

				fromvertex.x = lifeData[i++];
				fromvertex.y = -lifeData[i++];
				tovertex.x = lifeData[i++];
				tovertex.y = -lifeData[i++];

				fromvertex.x /= scale;
				fromvertex.y /= scale;
				tovertex.x /= scale;
				tovertex.y /= scale;

				fromvertex.x += x;
				fromvertex.y += y;
				tovertex.x += x;
				tovertex.y += y;

				vector::drawVectorGlow(fromvertex, tovertex, glowPen);
			};
			x+= .1;
		}
	}
*/
	// Score

	font::AlphanumericsPrint(TRUE, font::ALIGN_LEFT, .025, .4, .83, &pen, "score  %d", game::mPlayers.mPlayer1.mPoints);
	font::AlphanumericsPrint(FALSE, font::ALIGN_LEFT, .025, .4, .83, &pen, "score  %d", game::mPlayers.mPlayer1.mPoints);


	// High score (TODO)
}

void hud::drawReticule(bool glowPass, bool isTargeted)
{
  if (glowPass) return; // malban
	static int blinkTimer = 0;
	static BOOL blinkState = FALSE;
	--blinkTimer;
	if (blinkTimer <= 0)
	{
		blinkState = !blinkState;
		blinkTimer = blinkState ? 20 : 30;
	}

	BOOL reloading = game::mPlayers.mPlayer1.mReloading;
	if (reloading && blinkState) return;


	vector::pen pen = vector::penGreen();
	pen.a *= isTargeted ? 3 : 1.5;

	{
		// Top vertical line
		vector::drawVectorLine(Point3d(0,.5,0), Point3d(0,.2,0), pen, pen);

		// Top horizontal line
		vector::drawVectorLine(Point3d(-.2,.2,0), Point3d(.2,.2,0), pen, pen);

		// Bottom vertical line
		vector::drawVectorLine(Point3d(0,-.5,0), Point3d(0,-.2,0), pen, pen);

		// Bottom horizontal line
		vector::drawVectorLine(Point3d(-.2,-.2,0), Point3d(.2,-.2,0), pen, pen);

		if (isTargeted)
		{
			// Top side lines
			vector::drawVectorLine(Point3d(-.2, .2, 0), Point3d(-.12, .12, 0), pen, pen);
			vector::drawVectorLine(Point3d(.2, .2, 0), Point3d(.12, .12, 0), pen, pen);

			// Bottom side lines
			vector::drawVectorLine(Point3d(-.2, -.2, 0), Point3d(-.12, -.12, 0), pen, pen);
			vector::drawVectorLine(Point3d(.2, -.2, 0), Point3d(.12, -.12, 0), pen, pen);

			pen.a *= .4;

			// Top middle line
			vector::drawVectorLine(Point3d(0,.2,0), Point3d(0,.12,0), pen, pen);

			// Bottom middle line
			vector::drawVectorLine(Point3d(0,-.2,0), Point3d(0,-.12,0), pen, pen);
		}
		else
		{
			// Top side lines
			vector::drawVectorLine(Point3d(-.2, .2, 0), Point3d(-.2, .12, 0), pen, pen);
			vector::drawVectorLine(Point3d(.2, .2, 0), Point3d(.2, .12, 0), pen, pen);

			// Bottom side lines
			vector::drawVectorLine(Point3d(-.2, -.2, 0), Point3d(-.2, -.12, 0), pen, pen);
			vector::drawVectorLine(Point3d(.2, -.2, 0), Point3d(.2, -.12, 0), pen, pen);
		}
	}
}


