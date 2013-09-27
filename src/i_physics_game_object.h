#ifndef __I_PHYSICS_GAME_OBJECT_H__
#define __I_PHYSICS_GAME_OBJECT_H__

class b2World;

#include "i_game_object.h"

class IPhysicsGameObject : public IGameObject
{
public:
	IPhysicsGameObject() { }
	virtual ~IPhysicsGameObject() { }

	virtual bool hasPhysics() const { return true; };
	virtual IPhysicsGameObject *toPhysicsObject() { return (IPhysicsGameObject*)this; }

	virtual void registerPhysicsObject(b2World *world) = 0;
	virtual void unregisterPhysicsObject(b2World *world) = 0;

	virtual void beforePhysicsStep() = 0;
	virtual void afterPhysicsStep() = 0;

	virtual void handleCollision(IPhysicsGameObject *other) = 0;
};

#endif // __I_PHYSICS_OBJECT_H__