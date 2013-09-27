#ifndef __I_GAME_OBJECT_H__
#define __I_GAME_OBJECT_H__

#include <memory>
#include <vector>

// This enum gives the available object types,
// as well as the order they should be rendered in
enum GameObjectType
{
	GameObject_Background          = 0,
	GameObject_BackgroundDebugInfo = 1,
	GameObject_Tank                = 2,
	GameObject_Bullet              = 3,
	GameObject_FixedBlock          = 4,
	GameObject_TopDebugInfo        = 5
};

class IPhysicsGameObject;

class IGameObject
{
public:
	IGameObject() { }
	virtual ~IGameObject() { }

	virtual GameObjectType getObjectType() const = 0;
	virtual bool hasPhysics() const = 0;
	virtual IPhysicsGameObject *toPhysicsObject() = 0;

	virtual bool isAlive() const = 0;

	virtual void draw(float z) = 0;
	virtual void update(float time) = 0;
};

typedef std::vector<std::shared_ptr<IGameObject>> GameObjectList;

#endif // __I_GAME_OBJECT_H__