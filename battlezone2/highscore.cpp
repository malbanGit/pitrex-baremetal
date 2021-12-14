#include "highscore.h"
#include "game.h"

extern scene oglScene;


static char charList[] =
{
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',
	'_',
	(char) 0,
	(char) 0
};


highscore::highscore(void)
{
	char* names[10] = 
	{
		{ (char*)"EDR" },
		{ (char*)"MPH" },
		{ (char*)"JED" },
		{ (char*)"DES" },
		{ (char*)"TKE" },
		{ (char*)"VKB" },
		{ (char*)"EL " },
		{ (char*)"HAD" },
		{ (char*)"ORR" },
		{ (char*)"GJR" }
    };

	int scores[10] =
	{
		5000,
		5000,
		5000,
		5000,
		5000,
		5000,
		5000,
		5000,
		5000,
		5000
	};

	for (int i=0; i<10; i++)
	{
		strcpy(mEntries[i].name, names[i]);
		mEntries[i].score = scores[i];
	}

	mLastLetter = 0;
	for (int i=0; charList[i] != (char) 0; i++)
	{
		++mLastLetter;
	}

	--mLastLetter;

	mEditCurrentLetter[0] = 0;
	mEditCurrentLetter[1] = mLastLetter;
	mEditCurrentLetter[2] = mLastLetter;
}

highscore::~highscore(void)
{
}

bool highscore::isHighScore(int score)
{
	for (int i=0; i<10; i++)
	{
		if (score > mEntries[i].score)
		{
			return true;
		}
	}
	return false;
}

void highscore::init()
{
	mScore = game::mPlayers.mPlayer1.mPoints;
	mEditCurrentPos = 0;
	strcpy(mEditName, (char*)"___");

	mEditCurrentLetter[0] = 0;
	mEditCurrentLetter[1] = mLastLetter;
	mEditCurrentLetter[2] = mLastLetter;
}

void highscore::run()
{
	if (--mDebounceTimer <= 0)
	{
		mDebounceTimer = 20;

		bool trigger = game::mControls.getTriggerButton(0);
		if (trigger)
		{
			++mEditCurrentPos;
			if (mEditCurrentPos >= 3)
			{
				// All done
				mEditName[0] = charList[mEditCurrentLetter[0]];
				mEditName[1] = charList[mEditCurrentLetter[1]];
				mEditName[2] = charList[mEditCurrentLetter[2]];
				mEditName[3] = '\0';
				addHighScore(mEditName, mScore);
				oglScene.showHighScores();
			}
			else
			{
				mEditCurrentLetter[mEditCurrentPos] = 0;
			}
		}

		Point3d rightStick = game::mControls.getRightStick(0);
		float distance = mathutils::calculate2dDistance(Point3d(0,0,0), rightStick);
		if (distance > .1)
		{
			float angle = mathutils::RadsToDegrees(mathutils::calculate2dAngle(Point3d(0,0,0), rightStick));
			if ((angle > 90) && (angle < 270))
			{
				// Up
				--mEditCurrentLetter[mEditCurrentPos];
				if (mEditCurrentLetter[mEditCurrentPos] <= 0)
				{
					mEditCurrentLetter[mEditCurrentPos] = mLastLetter;
				}
			}
			else
			{
				// Down
				++mEditCurrentLetter[mEditCurrentPos];
				if (mEditCurrentLetter[mEditCurrentPos] > mLastLetter)
				{
					mEditCurrentLetter[mEditCurrentPos] = 0;
				}
			}

			mEditName[mEditCurrentPos] = charList[mEditCurrentLetter[mEditCurrentPos]];
		}
	}

}

void highscore::addHighScore(char* name, int score)
{
	for (int i=0; i<10; i++)
	{
		if (score >= mEntries[i].score)
		{
			// Move all the previous entries down and drop the last entry off the list
			for (int j=10-2; j>=i; j--)
			{
				strcpy(mEntries[j+1].name, mEntries[j].name);
				mEntries[j+1].score = mEntries[j].score;
			}

			// Plop the new score in-place
			// Replace underscores with spaces
			for (int c=0; c<4; c++)
			{
				if (name[c] == '_')
				{
					mEntries[i].name[c] = ' ';
				}
				else
				{
					mEntries[i].name[c] = name[c];
				}
			}

//			strcpy(mEntries[i].name, name);
			mEntries[i].score = score;
			break;
		}
	}
}

void highscore::drawEnterScore()
{
    vector::pen pen = vector::penGreen();
    pen.a *= 2;
    pen.lineRadius = 2;

    float bright = pen.a;
    float dim = pen.a * .75;

    font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .028, 0, .3, &pen, (char*)"GREAT SCORE             ");
    font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .028, 0, .3, &pen, (char*)"GREAT SCORE             ");

    font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .016, 0, .29, &pen, (char*)"                  enter your initials\nchange letter with right hand controller\nselect letter with fire button");
    font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .016, 0, .29, &pen, (char*)"                  enter your initials\nchange letter with right hand controller\nselect letter with fire button");

    // Name edit area
    {
	    if (mEditCurrentPos == 0)
	    {
		    pen.a = bright;
		    font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, -.05, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[0]]);
		    font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, -.05, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[0]]);
	    }
	    else
	    {
		    pen.a = dim;
		    font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, -.05, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[0]]);
		    font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, -.05, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[0]]);
	    }

	    if (mEditCurrentPos == 1)
	    {
		    pen.a = bright;
		    font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, 0, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[1]]);
		    font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, 0, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[1]]);
	    }
	    else
	    {
		    pen.a = dim;
		    font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, 0, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[1]]);
		    font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, 0, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[1]]);
	    }

	    if (mEditCurrentPos == 2)
	    {
		    pen.a = bright;
		    font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, .05, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[2]]);
		    font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, .05, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[2]]);
	    }
	    else
	    {
		    pen.a = dim;
		    font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, .05, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[2]]);
		    font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, .05, -.2, &pen, (char*)"%c", charList[mEditCurrentLetter[2]]);
	    }

    }
}

void highscore::drawTable()
{
	vector::pen pen = vector::penGreen();
	pen.a *= 2;
	pen.lineRadius = 2;

	float bright = pen.a;
	float dim = pen.a * .75;

	font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, 0, .4, &pen, (char*)"High Scores");
	font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, 0, .4, &pen, (char*)"High Scores");

	// Score
	float x = .02;
	float y = .3;
	for (int i=0; i<10; i++)
	{
		font::AlphanumericsPrint(TRUE, font::ALIGN_RIGHT, .03, x, y, &pen, (char*)"%d", mEntries[i].score);
		font::AlphanumericsPrint(FALSE, font::ALIGN_RIGHT, .03, x, y, &pen, (char*)"%d", mEntries[i].score);
		y-= .07;
	}

	// Name
	x = .15;
	y = .3;
	for (int i=0; i<10; i++)
	{
		font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, x, y, &pen, (char*)"%s", mEntries[i].name);
		font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, x, y, &pen, (char*)"%s", mEntries[i].name);
		y-= .07;
	}


	font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, 0, -.44, &pen, (char*)"bonus tank at 15000 and 100000");
	font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, 0, -.44, &pen, (char*)"bonus tank at 15000 and 100000");
}



