#ifndef GAME_H
#define GAME_H


#include "defines.h"
#include "sound.h"
#include "scene.h"
#include "point3d.h"
#include "particle.h"
#include "camera.h"
#include "controls.h"
#include "mathutils.h"
#include "entity.h"
#include "font.h"
#include "mountains.h"
#include "obstructions.h"
#include "enemies.h"
#include "players.h"
#include "hud.h"
#include "projectiles.h"
#include "spawner.h"
#include "highscore.h"
#include "entityLogo.h"

enum
{
	SOUNDID_PLAYER_ENGINE_LOOP=0,
	SOUNDID_PLAYER_FIRE,
	SOUNDID_PLAYER_EXPLODE,
	SOUNDID_ENEMY_FIRE,
	SOUNDID_ENEMY_EXPLODE,
	SOUNDID_NEW_ENEMY,
	SOUNDID_RADAR_BLIP,
	SOUNDID_HIT_OBSTRUCTION,
	SOUNDID_OBSTRUCTION_WARNING,
	SOUNDID_PROJECTILE_BLOCKED,
	SOUNDID_SAUCER_HIT,
	SOUNDID_SAUCER_LOOP,
	SOUNDID_MISSILE_LOOP,
	SOUNDID_HIGHSCORE,
	SOUNDID_EXTRA_LIFE,
    SOUNDID_LAST
};


class game
{
public:

    typedef enum
    {
        GAMEMODE_ATTRACT=0,
        GAMEMODE_CREDITED,
        GAMEMODE_PLAYING,
	GAMEMODE_HIGHSCORES_CHECK,
        GAMEMODE_HIGHSCORES,
        GAMEMODE_GAMEOVER
    }GameMode;

    game();
    ~game();

    void run();
    void draw(int pass);

    void startGame(int numPlayers);
    void endGame();

    static void showMessageAtLocation(char* message, const Point3d& pos, const vector::pen& pen);

    static sound mSound;
    static particle mParticles;
    static camera mCamera;
    static controls mControls;
    static mountains mMountains;
    static enemies mEnemies;
    static obstructions mObstructions;
    static players mPlayers;
    static hud mHud;
    static projectiles mProjectiles;
    static spawner mSpawner;
    static highscore mHighscore;
    static entityLogo mLogo;

    static int mSkillLevel;

    static GameMode mGameMode;

    static BOOL mFreeplay;

    static int mCredits;
    static int mNumPlayers;

    float mBrightness;

    int mGameOverTimer;

private:
    typedef struct
    {
        Point3d pos;
        vector::pen pen;
        char message[128];
        int timer;
        BOOL enabled;
    }PointDisplay;

    static PointDisplay* mPointDisplays;

    void runPointDisplays();
    void drawPointDisplays();
};

extern game theGame;

#endif // GAME_H
