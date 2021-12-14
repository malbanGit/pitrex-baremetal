#ifndef ENEMIES_H
#define ENEMIES_H

#include "defines.h"
#include "point3d.h"
#include "particle.h"
#include "camera.h"
#include "controls.h"
#include "mathutils.h"
#include "enemy.h"
#include "wreckage.h"

#define NUM_ENEMIES 10

class enemies
{
public:
    enemies();
    ~enemies();

    void run();
    void draw(const camera& cam);

    entity* getUnusedEnemyOfType(const entity::EntityType& type);

    entity* getActiveEnemy();

    int getNumActiveEnemiesOfType(const entity::EntityType& type);

    enemy* hitTestEnemiesAtPoint(const Point3d& point, float radius=0);

	void explodeEnemy(enemy* e);

    void disableAllEnemies();

    void initMissilePatterns();

    enemy* mEnemies[NUM_ENEMIES];

private:
	wreckage mWreckage;
};

#endif // ENEMIES_H
