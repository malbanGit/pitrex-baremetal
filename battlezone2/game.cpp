#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern "C" 
{
  #include <vectrex/vectrexInterface.h>
}

#include "game.h"

// Statics
sound game::mSound;
particle game::mParticles;
camera game::mCamera;
controls game::mControls;
mountains game::mMountains;
enemies game::mEnemies;
players game::mPlayers;
obstructions game::mObstructions;
hud game::mHud;
projectiles game::mProjectiles;
spawner game::mSpawner;
highscore game::mHighscore;
entityLogo game::mLogo;


#define NUM_POINT_DISPLAYS 40
#define TIME_POINT_DISPLAY 50


int game::mSkillLevel;
game::GameMode game::mGameMode;
game::PointDisplay* game::mPointDisplays;
BOOL game::mFreeplay = TRUE;
int game::mCredits = 0;
int game::mNumPlayers = 1;

game::game()
{
    //
    // Load our sounds
    //

    mSound.loadTrack((char*)"sounds/player_engine_loop.wav", SOUNDID_PLAYER_ENGINE_LOOP, .15, true);
    mSound.loadTrack((char*)"sounds/player_fire.wav", SOUNDID_PLAYER_FIRE, .5, false);
    mSound.loadTrack((char*)"sounds/player_explode.wav", SOUNDID_PLAYER_EXPLODE, .7, false);
    mSound.loadTrack((char*)"sounds/enemy_fire.wav", SOUNDID_ENEMY_FIRE, .5, false);
    mSound.loadTrack((char*)"sounds/enemy_explode.wav", SOUNDID_ENEMY_EXPLODE, .5, false);
    mSound.loadTrack((char*)"sounds/new_enemy.wav", SOUNDID_NEW_ENEMY, .5, false);
    mSound.loadTrack((char*)"sounds/radar_blip.wav", SOUNDID_RADAR_BLIP, .5, false);
    mSound.loadTrack((char*)"sounds/hit_obstruction.wav", SOUNDID_HIT_OBSTRUCTION, .5, false);
    mSound.loadTrack((char*)"sounds/obstruction_warning.wav", SOUNDID_OBSTRUCTION_WARNING, .5, true);
    mSound.loadTrack((char*)"sounds/enemy_explode.wav", SOUNDID_PROJECTILE_BLOCKED, .3, false);
    mSound.loadTrack((char*)"sounds/saucer_hit.wav", SOUNDID_SAUCER_HIT, .5, false);
    mSound.loadTrack((char*)"sounds/saucer_loop.wav", SOUNDID_SAUCER_LOOP, .5, true);
    mSound.loadTrack((char*)"sounds/missile_loop.wav", SOUNDID_MISSILE_LOOP, .5, true);
    mSound.loadTrack((char*)"sounds/1812.wav", SOUNDID_HIGHSCORE, .5, false);
    mSound.loadTrack((char*)"sounds/life.wav", SOUNDID_EXTRA_LIFE, .5, false);

    mSound.startSound();

    mPointDisplays = new PointDisplay[NUM_POINT_DISPLAYS];
    for (int i=0; i<NUM_POINT_DISPLAYS; i++)
    {
        mPointDisplays[i].enabled = FALSE;
    }

    mGameMode = GAMEMODE_ATTRACT;
}

game::~game()
{
    delete mPointDisplays;
}

void game::run()
{
    static bool creditButtonLast = FALSE;
    bool creditButton = mControls.getCreditButton();
    if (creditButton && !creditButtonLast)
    {
        ++mCredits;
    }
    creditButtonLast = creditButton;

    if (mFreeplay)
    {
        mCredits = 2;
    }

    mCamera.run();
    mMountains.run();
    mEnemies.run();
    mObstructions.run();
    mPlayers.run();
    mProjectiles.run();
    mHud.run();
    mParticles.run();
    mSpawner.run();
    mLogo.run();

    switch(mGameMode)
    {
        case GAMEMODE_ATTRACT:
            {
                if (mCredits > 0)
                {
                    mGameMode = GAMEMODE_CREDITED;
                }
            }
            break;
        case GAMEMODE_CREDITED:
            {
                if (mControls.getStart1Button())
                {
                    startGame(1);
                }
                else if (mControls.getStart2Button())
                {
                    startGame(2);
                }
            }
            break;
        case GAMEMODE_PLAYING:
            {
                runPointDisplays();
                mGameOverTimer = 0;
            }
            break;
        case GAMEMODE_HIGHSCORES:
	    mHighscore.run();
	    break;
        case GAMEMODE_HIGHSCORES_CHECK:
	    if (mHighscore.isHighScore(mPlayers.mPlayer1.mPoints) == true)
	    {
		    mSound.playTrack(SOUNDID_HIGHSCORE);
		    mGameMode = GAMEMODE_HIGHSCORES;
		    mHighscore.init();
		    break;
	    }
	    mGameMode = GAMEMODE_GAMEOVER;
	    // fall through
        case GAMEMODE_GAMEOVER:
            if (++mGameOverTimer > 200)
            {
                mGameMode = GAMEMODE_ATTRACT;
            }
            break;
    }

    if ((game::mGameMode == game::GAMEMODE_HIGHSCORES_CHECK) || (game::mGameMode == game::GAMEMODE_HIGHSCORES))
    {
    }
    else if (mGameMode != GAMEMODE_PLAYING)
    {
    }
}

void game::draw(int pass)
{
    // Draw stuff player one's perspective
    mMountains.draw(game::mPlayers.mPlayer1.mHeadingDiff);
    mLogo.draw(mPlayers.mPlayer1.mCamera);

    // Particles
    mParticles.draw(mPlayers.mPlayer1.mCamera);

    mObstructions.draw(mPlayers.mPlayer1.mCamera);
    mEnemies.draw(mPlayers.mPlayer1.mCamera);
    mProjectiles.draw(mPlayers.mPlayer1.mCamera);

    mPlayers.mPlayer1.draw();
    mHud.draw(); // Must be last
}

void game::startGame(int numPlayers)
{
    mNumPlayers = numPlayers;
    mPlayers.startGame(numPlayers);
    mParticles.killAll();
    mEnemies.initMissilePatterns();
    mSpawner.reset();

    mBrightness = 1;
    mSkillLevel = 0;

    mGameMode = GAMEMODE_PLAYING;
}

void game::endGame()
{
    // Doesn't actually end the game, just does some work that happens after the last player life is used
}

void game::showMessageAtLocation(char* message, const Point3d& pos, const vector::pen& pen)
{
    // Find an empty diplay
    for (int i=0; i<NUM_POINT_DISPLAYS; i++)
    {
        if (!mPointDisplays[i].enabled)
        {
            mPointDisplays[i].enabled = TRUE;
            mPointDisplays[i].pos = pos;
            mPointDisplays[i].pen = pen;
            sprintf(mPointDisplays[i].message, message);
            mPointDisplays[i].timer = TIME_POINT_DISPLAY;
            break;
        }
    }
}

void game::runPointDisplays()
{
    for (int i=0; i<NUM_POINT_DISPLAYS; i++)
    {
        if (mPointDisplays[i].enabled)
        {
            mPointDisplays[i].timer--;
            if (mPointDisplays[i].timer <= 0)
            {
                mPointDisplays[i].enabled = FALSE;
            }
        }
    }
}

void game::drawPointDisplays()
{
    for (int i=0; i<NUM_POINT_DISPLAYS; i++)
    {
        if (mPointDisplays[i].enabled)
        {
            float percent = (float)mPointDisplays[i].timer / TIME_POINT_DISPLAY;
            percent *= 2;
            if (percent > 1) percent = 1;

            float scale = .9 * percent;
            float alpha = 1 * percent;

            vector::pen pen = mPointDisplays[i].pen;
            pen.a=alpha;
            pen.lineRadius=3;

            font::AlphanumericsPrint(TRUE, font::ALIGN_CENTER, scale, mPointDisplays[i].pos.x, mPointDisplays[i].pos.y, &pen, mPointDisplays[i].message);
            font::AlphanumericsPrint(FALSE, font::ALIGN_CENTER, scale, mPointDisplays[i].pos.x, mPointDisplays[i].pos.y, &pen, mPointDisplays[i].message);
        }
    }
}

