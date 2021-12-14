#include "particle.h"
#include "game.h"

#define VIRTUAL_SCREEN_WIDTH 2 
#define VIRTUAL_SCREEN_HEIGHT 2

#define _DISTANCE 320
#define _MIN_DISTANCE .001
#define _MAX_DISTANCE 800


static BOOL renderParticles = TRUE;
static float brightness = 1.0f;
static float particleLineRadius = 3;

particle::particle()
{
    mNumParticles = 1000;

    mParticles = new PARTICLE[mNumParticles];
    if (mParticles)
    {
        for (int i=0; i<mNumParticles; i++)
        {
            mParticles[i].timeToLive = 0;
        }
    }
}

particle::~particle()
{
    if (mParticles)
    {
        for (int i=0; i<mNumParticles; i++)
        {
            mParticles[i].timeToLive = 0;
        }

        delete mParticles;
    }
    mParticles = NULL;
}

void particle::emitter(Point3d* position, Point3d* angle, float speed, float spread, int num, vector::pen* color, int timeToLive, BOOL gravity, float drag, BOOL groundBounce/*=TRUE*/)
{
    // Emit a number of random thrust particles from the nozzle
    for (int p=0; p<num; p++)
    {
        Point3d speedVertex, speedVector;

        //    timeToLive *= .98;

        speedVertex.x = 0;
        speedVertex.y = speed + ((mathutils::frandFrom0To1()*.5)-.25);
        speedVertex.z = 0;

        matrix mat;
        mat.Identity();
        mat.Rotate(angle->x + ((mathutils::frandFrom0To1()*spread)-spread/2),
                   angle->y + ((mathutils::frandFrom0To1()*spread)-spread/2),
                   angle->z + ((mathutils::frandFrom0To1()*spread)-spread/2));

        mat.TransformVertex(speedVertex, &speedVector);

        assignParticle(position,
                       speedVector.x, speedVector.y, speedVector.z,
                       timeToLive, color, gravity, drag, groundBounce);

    }
}


void particle::assignParticle(Point3d* position,
                              float aSpeedX, float aSpeedY, float aSpeedZ,
                              int aTime, vector::pen* aColor, BOOL gravity, float drag, BOOL groundBounce)
{
    int i;

    // Find an unused particle
    PARTICLE* particle = NULL;

    if (mParticles)
    {
        for (i=0; i<mNumParticles; i++)
        {
            if (mParticles[i].timeToLive <= 0)
            {
                // Found one
                particle = &mParticles[i];
                break;
            }
        }
    }

    if (!particle)
    {
        // Out of particles - pick off the particle closest to being done
        int minTime = 320000;
        int minIndex = 0;

        for (i=0; i<mNumParticles; i++)
        {
            if (mParticles[i].timeToLive <= minTime)
            {
                minIndex = i;
                minTime = mParticles[i].timeToLive;
            }
        }

        particle = &mParticles[minIndex];
    }

    if (particle)
    {
        Point3d pos = *position;

        for (int p=0; p<NUM_POS_STREAM_ITEMS; p++)
        {
            particle->posStream[p] = pos;
        }

        particle->speedX = aSpeedX;
        particle->speedY = aSpeedY;
        particle->speedZ = aSpeedZ;
        particle->color = *aColor;
        particle->timeToLive = aTime * mathutils::frandFrom0To1();
        particle->fadeStep = 1.0 / particle->timeToLive;
        particle->gravity = gravity;
		particle->groundBounce = groundBounce;
        particle->drag = drag;
    }
}

void particle::draw(const camera& cam)
{
    if (mParticles)
    {
		// LINES
        for (int i=0; i<mNumParticles; i++)
        {
            if (mParticles[i].timeToLive > 0)
            {
                // This particle is active
                PARTICLE* particle = &mParticles[i];

                float a = particle->color.a;
                float speedNormal = mathutils::calculate2dDistance(Point3d(0,0,0), Point3d(particle->speedX, particle->speedY, particle->speedZ));
                a = a * (speedNormal * .8);
				a *= theScene->mFlicker ? 1 : .5;

//                for (int p=0; p<NUM_POS_STREAM_ITEMS-1; p++)
		int p = 0;
                {
                    Point3d from = particle->posStream[p];
                    Point3d to = particle->posStream[p+1];

		    Point3d sfrom = cam.mMatrix.WorldToScreen(from);
		    Point3d sto = cam.mMatrix.WorldToScreen(to);

		    if ((sfrom.z > .1) && (sto.z > .1) && (sfrom.z < _MAX_DISTANCE) && (sto.z < _MAX_DISTANCE))
		    {
			    a *= 1 - (sfrom.z / 400);
//			    glColor4f(particle->color.r, particle->color.g, particle->color.b, a); // RGBA

			    vector::pen pen = particle->color;
			    pen.lineRadius *= pen.a;

drawLine(sfrom.x, sfrom.y, sfrom.x, sfrom.y, (int) (particle->color.r+particle->color.g+particle->color.b/3.0*127.0));
drawLine(sto.x, sto.y, sto.x, sto.y, (int) (particle->color.r+particle->color.g+particle->color.b/3.0*127.0));
		    }
                }
            }
        }
/*
		// GLOW
        for (int i=0; i<mNumParticles; i++)
        {
            if (mParticles[i].timeToLive > 0)
            {
                // This particle is active
                PARTICLE* particle = &mParticles[i];

                float a = particle->color.a;
                float speedNormal = mathutils::calculate2dDistance(Point3d(0,0,0), Point3d(particle->speedX, particle->speedY, particle->speedZ));
                a = a * (speedNormal * .8);

                glColor4f(particle->color.r, particle->color.g, particle->color.b, a); // RGBA
                glLineWidth(particle->color.lineRadius);

                for (int p=0; p<NUM_POS_STREAM_ITEMS-1; p++)
                {
                    Point3d from = particle->posStream[p];
                    Point3d to = particle->posStream[p+1];

					Point3d sfrom = cam.mMatrix.WorldToScreen(from);
					Point3d sto = cam.mMatrix.WorldToScreen(to);

					if ((sfrom.z > .1) && (sto.z > .1) && (sfrom.z < _MAX_DISTANCE) && (sto.z < _MAX_DISTANCE))
					{
						vector::pen pen = particle->color;
						a *= 1 - (sfrom.z / 400);
						pen.a = a * .01;

						vector::drawVectorGlow(Point3d(sfrom.x,sfrom.y,0), Point3d(sfrom.x,sfrom.y,0), pen);
					}
                }
            }
        }
*/        
	}
}


void particle::run()
{
    int i;
    if (mParticles)
    {
        for (i=0; i<mNumParticles; i++)
        {
            PARTICLE* particle = &mParticles[i];
            if (particle->timeToLive > 0)
            {
                // This particle is active
                --particle->timeToLive;
                if (particle->timeToLive <= 0)
                {
                    // This particle died
                    particle->timeToLive = 0;
                }
                else
                {
                    // Add drag
                    particle->speedX *= particle->drag;
                    particle->speedY *= particle->drag;
                    particle->speedZ *= particle->drag;

/*
                    Point3d speedClamp(particle->speedX, particle->speedY, 0);
                    speedClamp = mathutils::clamp2dVector(speedClamp, 2);
                    particle->speedX = speedClamp.x;
                    particle->speedY = speedClamp.y;
                    particle->speedZ = speedClamp.z;
*/

					if (particle->gravity)
					{
						particle->speedY -= .01;
					}

                    // Move the particle
                    particle->posStream[0].x += particle->speedX * .5;
                    particle->posStream[0].y += particle->speedY * .5;
                    particle->posStream[0].z += particle->speedZ * .5;

					if (particle->groundBounce)
					{
						if (particle->posStream[0].y < 0)
						{
							// Hit the ground - bounce
							particle->posStream[0].y = 0;
							particle->speedY = -particle->speedY * .8;

							particle->speedX *= .8;
						}
					}

                    // Shift the position stream
                    for (int p=NUM_POS_STREAM_ITEMS-2; p >= 0; p--)
                    {
                        particle->posStream[p+1] = particle->posStream[p];
                    }
                }
            }
        }
    }
}

void particle::killAll()
{
    if (mParticles)
    {
        for (int i=0; i<mNumParticles; i++)
        {
            mParticles[i].timeToLive = 0;
        }
    }
}




