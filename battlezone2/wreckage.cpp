#include "wreckage.h"

#include "game.h"
#include "objloader.h"

wreckage::wreckage()
{
	int i=0;

	for (i=0; i<MAX_WRECKAGE; i++)
	{
		mWreckage[i] = NULL;
	}

	// TANK PARTS

	i=0;

	vector::pen tankPen = vector::penGreen();
	tankPen.a *= 1.2;

	{
		entityWreckage* e = static_cast<entityWreckage*>(entity::createEntity(entity::ENTITY_TYPE_WRECKAGE));
		e->setModel(objloader::loadFile((char *)"models/tank_wreckage_turret.obj"));
		e->setScale(5);
		e->getModel()->Scale(5);
		e->setPen(tankPen);
		mWreckage[i] = e;
	}

	++i;

	{
		entityWreckage* e = static_cast<entityWreckage*>(entity::createEntity(entity::ENTITY_TYPE_WRECKAGE));
		e->setModel(objloader::loadFile((char *)"models/tank_radar.obj"));
		e->setScale(5);
		e->getModel()->Scale(5);
		e->setPen(tankPen);
		mWreckage[i] = e;
	}

	// GENERIC PARTS

	++i;

	{
		entityWreckage* e = static_cast<entityWreckage*>(entity::createEntity(entity::ENTITY_TYPE_WRECKAGE));
		e->setModel(objloader::loadFile((char *)"models/tank_wreckage_chunk1.obj"));
		e->setScale(2);
		e->getModel()->Scale(2);
		e->setPen(tankPen);
		mWreckage[i] = e;
	}

	++i;

	{
		entityWreckage* e = static_cast<entityWreckage*>(entity::createEntity(entity::ENTITY_TYPE_WRECKAGE));
		e->setModel(objloader::loadFile((char *)"models/tank_wreckage_chunk1.obj"));
		e->setScale(2);
		e->getModel()->Scale(2);
		e->setPen(tankPen);
		mWreckage[i] = e;
	}

	++i;

	{
		entityWreckage* e = static_cast<entityWreckage*>(entity::createEntity(entity::ENTITY_TYPE_WRECKAGE));
		e->setModel(objloader::loadFile((char *)"models/tank_wreckage_chunk2.obj"));
		e->setScale(1.4);
		e->getModel()->Scale(1.4);
		e->setPen(tankPen);
		mWreckage[i] = e;
	}

	++i;

	{
		entityWreckage* e = static_cast<entityWreckage*>(entity::createEntity(entity::ENTITY_TYPE_WRECKAGE));
		e->setModel(objloader::loadFile((char *)"models/tank_wreckage_chunk2.obj"));
		e->setScale(1.4);
		e->getModel()->Scale(1.4);
		e->setPen(tankPen);
		mWreckage[i] = e;
	}

	++i;

	// MISSILE PARTS

	{
		entityWreckage* e = static_cast<entityWreckage*>(entity::createEntity(entity::ENTITY_TYPE_WRECKAGE));
		e->setModel(objloader::loadFile((char *)"models/missile_wreckage.obj"));
		e->setScale(2);
		e->getModel()->Scale(2);
		e->setPen(tankPen);
		mWreckage[i] = e;
	}

	++i;

	{
		entityWreckage* e = static_cast<entityWreckage*>(entity::createEntity(entity::ENTITY_TYPE_WRECKAGE));
		e->setModel(objloader::loadFile((char *)"models/missile_wreckage.obj"));
		e->setScale(2);
		e->getModel()->Scale(2);
		e->setPen(tankPen);
		mWreckage[i] = e;
	}

	++i;

}

wreckage::~wreckage()
{
}

void wreckage::run()
{
	for (int i=0; i<MAX_WRECKAGE; i++)
	{
		if (mWreckage[i] && mWreckage[i]->getEnabled())
		{
			--mWreckage[i]->mFadeTimer;
			if (mWreckage[i]->mFadeTimer <= 0)
			{
				mWreckage[i]->setState(entity::ENTITY_STATE_INACTIVE);
			}
			else
			{
				Point3d speed = mWreckage[i]->getSpeed();
				Point3d pos = mWreckage[i]->getPos();
				Point3d rot = mWreckage[i]->getRotationRate();
				vector::pen pen = mWreckage[i]->getPen();

				pen.a = ((1.0f/200.0f) * mWreckage[i]->mFadeTimer) * .5;


				speed.y += .01; // gravity


				rot.x *= .99;
				rot.y *= .99;
				rot.z *= .99;

				// bounce
				if (pos.y > 0)
				{
					pos.y = 0;
					speed.y = -speed.y * .5;
					speed.x *= .5;
				}

				mWreckage[i]->setSpeed(speed);
				mWreckage[i]->setPos(pos);
				mWreckage[i]->setRotationRate(rot);
				mWreckage[i]->setPen(pen);

				mWreckage[i]->run();
			}
		}
	}
}

void wreckage::draw(const camera& cam)
{
	for (int i=0; i<MAX_WRECKAGE; i++)
	{
		if (mWreckage[i] && mWreckage[i]->getEnabled())
		{
			mWreckage[i]->draw(cam);
		}
	}
}

void wreckage::explodeTank(const entity& e, const Point3d& pos, float heading)
{
	if (e.getType() == entity::ENTITY_TYPE_MISSILE)
	{
		// Missiles
		for (int i=2; i<7; i++)
		{
			Point3d p(pos);
			mWreckage[i]->setPos(p);
			mWreckage[i]->setHeading(heading);

			Point3d speed;
			Point3d rot;

			speed.x = (mathutils::frandFrom0To1()-.5) * .6;
			speed.y = -((mathutils::frandFrom0To1()*.6)+.6); // up
			speed.z = (mathutils::frandFrom0To1()-.5) * .6;

			rot.x = (mathutils::frandFrom0To1()-.5) * .8;
			rot.y = (mathutils::frandFrom0To1()-.5) * .03;
			rot.z = (mathutils::frandFrom0To1()-.5) * .8;

			mWreckage[i]->setSpeed(speed);
			mWreckage[i]->setRotationRate(rot);

			mWreckage[i]->mFadeTimer = 200;

			mWreckage[i]->setState(entity::ENTITY_STATE_RUNNING);
		}
	}
	else
	{
		// Tanks
		for (int i=0; i<5; i++)
		{
			Point3d p(pos);
			mWreckage[i]->setPos(p);
			mWreckage[i]->setHeading(heading);

			Point3d speed;
			Point3d rot;

			speed.x = (mathutils::frandFrom0To1()-.5) * .6;
			speed.y = -((mathutils::frandFrom0To1()*.6)+.6); // up
			speed.z = (mathutils::frandFrom0To1()-.5) * .6;

			rot.x = (mathutils::frandFrom0To1()-.5) * .8;
			rot.y = (mathutils::frandFrom0To1()-.5) * .03;
			rot.z = (mathutils::frandFrom0To1()-.5) * .8;

			mWreckage[i]->setSpeed(speed);
			mWreckage[i]->setRotationRate(rot);

			mWreckage[i]->mFadeTimer = 200;

			mWreckage[i]->setState(entity::ENTITY_STATE_RUNNING);
		}
	}
}

