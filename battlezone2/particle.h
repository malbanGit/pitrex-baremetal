#ifndef PARTICLE_H
#define PARTICLE_H

#include "point3d.h"
#include "vector.h"
#include "mathutils.h"
#include "camera.h"

#define NUM_POS_STREAM_ITEMS 5

class particle
{
public:
    typedef struct
    {
        Point3d posStream[NUM_POS_STREAM_ITEMS];

        float speedX;
        float speedY;
        float speedZ;

        vector::pen color;

        int timeToLive;

        float fadeStep;

        BOOL gravity;
		BOOL groundBounce;
        float drag;

    }PARTICLE;

    particle();
    ~particle();

    virtual void run();
    virtual void draw(const camera& cam);
	virtual void emitter(Point3d* position, Point3d* angle, float speed, float spread, int num, vector::pen* color, int timeToLive, BOOL gravity=TRUE, float drag=.93, BOOL groundBounce=TRUE);

    void killAll();

    PARTICLE* mParticles;
    int mNumParticles;

protected:

    void assignParticle(Point3d* position,
                                  float aSpeedX, float aSpeedY, float aSpeedZ,
                                  int aTime, vector::pen* aColor, BOOL gravity, float drag, BOOL groundBounce);

};

#endif // PARTICLE_H
