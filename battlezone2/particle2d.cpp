#include "particle2d.h"
#include "matrix.h"
#include "scene.h"

#define VIRTUAL_SCREEN_WIDTH 800 // FIX ME
#define VIRTUAL_SCREEN_HEIGHT 600 // FIX ME

#define _DISTANCE 320
#define _MIN_DISTANCE .001
#define _MAX_DISTANCE 800


static BOOL renderParticles = TRUE;
static float brightness = 1.0f;
static float particleLineRadius = 3;

particle2d::particle2d()
	: particle()
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

particle2d::~particle2d()
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

void particle2d::emitter(Point3d* position, Point3d* angle, float speed, float spread, int num, vector::pen* color, int timeToLive, BOOL gravity/*=TRUE*/, float drag/*=.93*/, BOOL groundBounce/*=TRUE*/)
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
        mat.Rotate(0,
                   0,
                   angle->y + ((mathutils::frandFrom0To1()*spread)-spread/2));

        mat.TransformVertex(speedVertex, &speedVector);

        assignParticle(position,
                       speedVector.x, speedVector.y, speedVector.z,
                       timeToLive, color, gravity, drag, groundBounce);

    }
}

void particle2d::run()
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
                
                    Point3d speedClamp(particle->speedX, particle->speedY, 0);
                    speedClamp = mathutils::clamp2dVector(speedClamp, 2);
                    particle->speedX = speedClamp.x;
                    particle->speedY = speedClamp.y;

		    if (particle->gravity)
		    {
			    particle->speedY -= .1;
		    }

                    // Move the particle
                    particle->posStream[0].x += particle->speedX * .005;
                    particle->posStream[0].y += particle->speedY * .005;
                    particle->posStream[0].z = 0;

		    if (particle->posStream[0].y < 0)
		    {
			    // Hit the ground - bounce
			    particle->posStream[0].y = 0;
			    particle->speedY = 0;//-particle->speedY * .8;

			    particle->speedX = 0;//(mathutils::frandFrom0To1()-.5) * .1;
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

void particle2d::draw(float heading, const matrix& mat)
{
    if (mParticles)
    {
		// POINTS

        for (int i=0; i<mNumParticles; i++)
        {
            if (mParticles[i].timeToLive > 0)
            {
                // This particle is active
                PARTICLE* particle = &mParticles[i];
                float a = particle->color.a;
                float speedNormal = mathutils::calculate2dDistance(Point3d(0,0,0), Point3d(particle->speedX, particle->speedY, particle->speedZ));
                a = a * (speedNormal * 8);
				a *= theScene->mFlicker ? 1 : .5;

//                for (int p=0; p<NUM_POS_STREAM_ITEMS-1; p++)
		int p = 0;
                {
                    Point3d vertexFrom = particle->posStream[p];

					vertexFrom.x += heading;

		            vector::pen pen = particle->color;

//					glColor4f(pen.r, pen.g, pen.b, a);

					Point3d tfrom;
					mat.TransformVertex(vertexFrom, &tfrom);

drawLine(tfrom.x, tfrom.y, tfrom.x, tfrom.y, (int) (particle->color.r+particle->color.g+particle->color.b/3.0*127.0));
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

//				glColor4f(particle->color.r, particle->color.g, particle->color.b, a); // RGBA
				glPointSize(particle->color.lineRadius);

                float a = particle->color.a;
                float speedNormal = mathutils::calculate2dDistance(Point3d(0,0,0), Point3d(particle->speedX, particle->speedY, particle->speedZ));
                a = a * (speedNormal * 8);

//                for (int p=0; p<NUM_POS_STREAM_ITEMS-1; p++)
				int p = 0;
                {
                    Point3d vertexFrom = particle->posStream[p];

					vertexFrom.x += heading;

		            vector::pen pen = particle->color;
					pen.a = a * .05;

					Point3d tfrom;
					mat.TransformVertex(vertexFrom, &tfrom);

					vector::drawVectorGlow(tfrom, tfrom, pen);
                }

            }
        }
*/

    }
}

