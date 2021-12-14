#include "scene.h"
#include "game.h"

game theGame;

scene* theScene;

int scene::mPass;

#define VIRTUAL_SCREEN_WIDTH 1024
#define VIRTUAL_SCREEN_HEIGHT 1024


static vector::pen singlePlayerColor(1, .5, 0, .75, 12);
static vector::pen player1Color(1, .8, .8, .75, 12);
static vector::pen player2Color(.8, .8, 1, .75, 12);


scene::scene()
{
	theScene = this;

	mAttractModeTimer = 0;
	mLogoCounter = 0;
	mShowHighScores = FALSE;
	mShowingLogo = TRUE;
	mAspect = (float)16 / 9;

	mFlicker = FALSE;
	mFlickerTimer = 0;
}

void scene::run()
{
	theGame.run();
}

void scene::draw(int pass)
{
	if (--mFlickerTimer < 0)
	{
		mFlickerTimer = 1;
		mFlicker = !mFlicker;
	}

	mPass = pass;
	// *******************************************
	bool enterHighscore = FALSE;
	if ((game::mGameMode == game::GAMEMODE_HIGHSCORES_CHECK) || (game::mGameMode == game::GAMEMODE_HIGHSCORES))
	{
		enterHighscore = TRUE;
	}
	else if ((game::mGameMode == game::GAMEMODE_ATTRACT) || (game::mGameMode == game::GAMEMODE_CREDITED))
	{
		if (mShowingLogo)
		{
		    game::mEnemies.disableAllEnemies();
		    if (++mAttractModeTimer > 1000)
		    {
			mShowingLogo = FALSE;
		    }
		}
		else
		{
		    if (++mAttractModeTimer > (mShowHighScores ? 500 : 500))
		    {
			mAttractModeTimer = 0;
			mShowHighScores = !mShowHighScores;
			if (!mShowHighScores)
			{
			    game::mEnemies.disableAllEnemies();

			    if (++mLogoCounter > 2)
			    {
				mShowingLogo = TRUE;
				mLogoCounter = 0;
			    }
			}
		    }
		}
	}
	else
	{
	    mShowingLogo = FALSE;
	    mShowHighScores = FALSE;
	}

	if (mShowHighScores)
	{
		mFlicker = false;
		game::mHighscore.drawTable();
		game::mHud.drawLivesPanel(TRUE);
		game::mHud.drawLivesPanel(FALSE);
	}
	else if (enterHighscore)
	{
		mFlicker = false;
		game::mHighscore.drawEnterScore();
		game::mHud.drawLivesPanel(TRUE);
		game::mHud.drawLivesPanel(FALSE);
	}
	else
	{
		theGame.draw(pass);
	}


	if (!mShowHighScores)
	{
		if (game::mGameMode == game::GAMEMODE_ATTRACT || game::mGameMode == game::GAMEMODE_CREDITED || game::mGameMode == game::GAMEMODE_GAMEOVER)
		{
			vector::pen pen = vector::penGreen();
			pen.a *= 2.2;
			pen.lineRadius *= .8;
			font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, 0, .28, &pen, "Game Over");
			font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, 0, .28, &pen, "Game Over");

			font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .03, 0, -.7, &pen, "©   ATARI 1980");
			font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .03, 0, -.7, &pen, "©   ATARI 1980");
		}
	}
}

void scene::drawCredits()
{
    if (game::mFreeplay)
    {
        vector::pen pen;
        pen.r=1;
        pen.g=1;
        pen.b=1;
        pen.a=.75;
        pen.lineRadius = 3;
        font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .01, 0, -.9, &pen, "FREE PLAY");
        font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .01, 0, -.9, &pen, "FREE PLAY");
    }
    else
    {
        vector::pen pen;
        pen.r=1;
        pen.g=1;
        pen.b=1;
        pen.a=.75;
        pen.lineRadius = 3;
        font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, .015, 0, -.9, &pen, "Credits %d", game::mCredits);
        font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, .015, 0, -.9, &pen, "Credits %d", game::mCredits);
    }
}

void scene::drawNumLives()
{
}

void scene::drawScores()
{
}

void scene::showHighScores()
{
	game::mSound.stopAllTracks();
	if (game::mFreeplay || game::mCredits)
	{
		game::mGameMode = game::GAMEMODE_CREDITED;
	}
	else
	{
		game::mGameMode = game::GAMEMODE_ATTRACT;
	}
	mAttractModeTimer = -200;
	mShowHighScores = TRUE;
}

