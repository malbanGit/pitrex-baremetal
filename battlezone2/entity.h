#ifndef ENTITY_H
#define ENTITY_H


#include "defines.h"
#include "point3d.h"
#include "vector.h"
#include "model.h"

#include "point3d.h"

class entity
{
    public:

    typedef enum
    {
        ENTITY_TYPE_UNDEF=0,
	ENTITY_TYPE_OBSTRUCTION_CUBE,
	ENTITY_TYPE_OBSTRUCTION_SHORTCUBE,
	ENTITY_TYPE_OBSTRUCTION_PYRAMID,
	ENTITY_TYPE_TANK,
	ENTITY_TYPE_SUPERTANK,
	ENTITY_TYPE_SAUCER,
	ENTITY_TYPE_MISSILE,
	ENTITY_TYPE_PROJECTILE,
	ENTITY_TYPE_WRECKAGE,
        ENTITY_TYPE_LOGO,
        ENTITY_NUM_TYPES // Must be last
    } EntityType;

    typedef enum
    {
        ENTITY_STATE_INACTIVE=0,
        ENTITY_STATE_SPAWN_TRANSITION,
        ENTITY_STATE_SPAWNING,
        ENTITY_STATE_RUN_TRANSITION,
        ENTITY_STATE_RUNNING,
        ENTITY_STATE_DESTROY_TRANSITION,
        ENTITY_STATE_DESTROYED
    } EntityState;

    entity();
    ~entity();

    static entity* createEntity(EntityType _entity); // Static class factory

    EntityType getType() const { return mType; }

    Point3d getPos() const { return mPos; }
    void setPos(const Point3d& pos) { mPos = pos; }

    Point3d getSpeed() const { return mSpeed; }
    void setSpeed(const Point3d& speed) { mSpeed = speed; }

    float getHeading() const { return mHeading; }
    void setHeading(const float& heading) { mHeading = heading; }

    Point3d getRotation() const { return mRotation; }
    void setRotation(const Point3d& rot) { mRotation = rot; }

    Point3d getRotationRate() const { return mRotationRate; }
    void setRotationRate(const Point3d& rate) { mRotationRate = rate; }

    Point3d getScale() const { return mScale; }
    void setScale(const Point3d& scale) { mScale = scale; }
    void setScale(float scale) { mScale = scale; }

    vector::pen getPen() const { return mPen; }
    void setPen(const vector::pen& pen) { mPen = pen; }

    bool getEnabled() const { return mState != ENTITY_STATE_INACTIVE; }
    void setEnabled(const bool& enabled) { mState = (enabled) ? ENTITY_STATE_SPAWN_TRANSITION : ENTITY_STATE_INACTIVE; }

    EntityState getState() const { return mState; }
    void setState(const EntityState& state) { mState = state; }

    int getStateTimer() const { return mStateTimer; }
    void setStateTimer(int stateTimer) { mStateTimer = stateTimer; }

    model* getModel() { return mModel; }
    void setModel(model* m) { mModel = m; }

    const int getScoreValue() const { return mScoreValue; }

    const float getRadius() const { return mRadius; }

    const float getAggression() const { return mAggression; }

    virtual void runTransition();
    virtual void run();
    virtual void spawnTransition();
    virtual void spawn();
    virtual void destroyTransition();
    virtual void destroy();

    virtual void hit(entity* aEntity);
    virtual entity* hitTest(const Point3d& pos, float radius=0);

    virtual void draw(const camera& cam);

    virtual entity* getParent() { return this; }

protected:

    EntityType mType;
    Point3d mPos;
    Point3d mSpeed;
    float mHeading;
    Point3d mRotation;
    Point3d mTargetRotation;
    Point3d mRotationRate;
    Point3d mScale;
    float mRadius;
    vector::pen mPen;

    float mAggression;

    EntityState mState;

    int mSpawnTime;
    int mDestroyTime;
    float mStateTimer;

    int mScoreValue;

    model* mModel;
};

#endif // ENTITY_H
