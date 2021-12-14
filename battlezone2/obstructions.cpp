#include "obstructions.h"
#include "game.h"

#define NUM_OBSTRUCTIONS 50
#define FIELD_SIZE	1000

obstructions::obstructions()
{
	mObstructions = new entity*[NUM_OBSTRUCTIONS];

	// Create random obstructions
	for (int i=0; i<NUM_OBSTRUCTIONS; i++)
	{
		int type = mathutils::frandFrom0To1() * 3;
		switch(type)
		{
			case 0:
				mObstructions[i] = entity::createEntity(entity::ENTITY_TYPE_OBSTRUCTION_CUBE);
				break;
			case 1:
				mObstructions[i] = entity::createEntity(entity::ENTITY_TYPE_OBSTRUCTION_SHORTCUBE);
				break;
			case 2:
				mObstructions[i] = entity::createEntity(entity::ENTITY_TYPE_OBSTRUCTION_PYRAMID);
				break;
		}
	}

	// Assign them random positions
	for (int i=0; i<NUM_OBSTRUCTIONS; i++)
	{
		float x = (mathutils::frandFrom0To1() * FIELD_SIZE)-(FIELD_SIZE/2);
		float y = (mathutils::frandFrom0To1() * FIELD_SIZE)-(FIELD_SIZE/2);
		mObstructions[i]->setPos(Point3d(x, 0, y));
		mObstructions[i]->setEnabled(TRUE);
	}

	// Keep them apart from each other
	BOOL tooClose;
	do
	{
		tooClose = FALSE;
		for (int i=0; i<NUM_OBSTRUCTIONS && !tooClose; i++)
		{
			for (int j=0; j<NUM_OBSTRUCTIONS && !tooClose; j++)
			{
				if (i==j) continue;
				float distance = mathutils::calculate2dDistance(mObstructions[i]->getPos(), mObstructions[j]->getPos());
				if (distance < 80)
				{
					float x = (mathutils::frandFrom0To1() * FIELD_SIZE)-(FIELD_SIZE/2);
					float y = (mathutils::frandFrom0To1() * FIELD_SIZE)-(FIELD_SIZE/2);
					mObstructions[j]->setPos(Point3d(x, 0, y));

					tooClose = TRUE;
					break;
				}
			}
		}
	} while (tooClose);


	// Place them all at ground level
	{
		for (int i=0; i<NUM_OBSTRUCTIONS; i++)
		{
			Point3d pos = mObstructions[i]->getPos();
			pos.y = mObstructions[i]->getModel()->getBottom() * mObstructions[i]->getScale().y;
			mObstructions[i]->setPos(pos);
		}
	}


}

obstructions::~obstructions()
{
    for (int i=0; i<NUM_OBSTRUCTIONS; i++)
    {
		delete mObstructions[i];
    }

	delete mObstructions;
}

void obstructions::run()
{
	for (int i=0; i<NUM_OBSTRUCTIONS; i++)
	{
		entity* ob = mObstructions[i];
		ob->run();

		// Check for obstructions that are too far away and respawn them closer
		Point3d playerPos = game::mPlayers.mPlayer1.getPos();
		float distance = mathutils::calculate2dDistance(playerPos, ob->getPos());
		if (distance > 800)
		{
			// respawn it somewhere closer to the player

			// Keep them apart from each other
			BOOL tooClose;
			do
			{
				tooClose = FALSE;

				float x = (mathutils::frandFrom0To1() * FIELD_SIZE)-(FIELD_SIZE/2);
				float z = (mathutils::frandFrom0To1() * FIELD_SIZE)-(FIELD_SIZE/2);
				x += playerPos.x;
				z += playerPos.z;
				ob->setPos(Point3d(x, mObstructions[i]->getModel()->getBottom() * ob->getScale().y, z));

				for (int k=0; k<NUM_OBSTRUCTIONS && !tooClose; k++)
				{
					for (int j=0; j<NUM_OBSTRUCTIONS && !tooClose; j++)
					{
						if (k==j) continue;
						float distance = mathutils::calculate2dDistance(mObstructions[k]->getPos(), mObstructions[j]->getPos());
						if (distance < 80)
						{
							tooClose = TRUE;
							break;
						}
					}
				}

			} while (tooClose);

		}
	}
}

void obstructions::draw(const camera& cam)
{
	for (int i=0; i<NUM_OBSTRUCTIONS; i++)
	{
		entity* ob = mObstructions[i];

		// Cull obstructions that are out of the view frustum
//		float angle = mathutils::calculate2dAngle(game::mPlayers.mPlayer1.getPos(), ob->getPos());
//		float diff = mathutils::diffAngles(angle, game::mPlayers.mPlayer1.mHeading + mathutils::DegreesToRads(90));
//		if (fabs((double)diff) < .5)
		{
			ob->draw(cam);
		}
	}
}

entity* obstructions::hitTestObstructionsAtPoint(Point3d point, float radius)
{
	for (int i=0; i<NUM_OBSTRUCTIONS; i++)
	{
		entity* ob = mObstructions[i];
		if (ob->getEnabled())
		{
			float distance = mathutils::calculate2dDistance(ob->getPos(), point);
			if (distance < (ob->getRadius() + radius))
				return ob;
		}
	}

	return NULL;
}
