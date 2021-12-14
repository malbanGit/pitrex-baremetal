#ifndef _SCENE_H
#define _SCENE_H

#include "model.h"


class scene
{
public:

    enum RenderPass
    {
        RENDERPASS_PRIMARY=0,
        RENDERPASS_BLUR
    };

    scene();

    void run();
    void draw(int pass);
    void size(float w, float h) { mWidth = w; mHeight = h; mAspect = w/h; }

    void glEnable2D();
    void glDisable2D();

    void showHighScores();

    static int mPass;


    float mWidth;
    float mHeight;
    float mAspect;

	BOOL mFlicker;
    BOOL mShowingLogo;

private:

    int mFlickerTimer;

    void drawCredits();
    void drawNumLives();
    void drawScores();

    int mAttractModeTimer;
    bool mShowHighScores;
    int mLogoCounter;
};
extern scene* theScene;

#endif
