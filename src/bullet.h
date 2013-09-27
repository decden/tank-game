#ifndef __BULLET_H__
#define __BULLET_H__

#include <vector>
#include <memory>

#include "i_physics_game_object.h"

class Tank;
class b2World;
class b2Body;

class Bullet : public IPhysicsGameObject
{
public:
	Bullet(Tank *owner, float x, float y, float direction, float speed);
	virtual ~Bullet();

	// IGameObject element
	virtual GameObjectType getObjectType() const;
	virtual bool isAlive() const;
	virtual void draw(float z);
	virtual void update(float time);

	// IPhysicsGameObject element
	virtual void registerPhysicsObject(b2World *world);
	virtual void unregisterPhysicsObject(b2World *world);
	virtual void beforePhysicsStep();
	virtual void afterPhysicsStep();
	virtual void handleCollision(IPhysicsGameObject *other);

	bool canDamnageTank(const Tank *tank) const;	
private:
	b2Body *body;

	Tank   *owner;

	bool alive;
	float age;

	float x;
	float y;
	
	float direction;
	float speed;

	int bounces;
};

typedef std::vector<std::shared_ptr<Bullet>> BulletList;

#endif // __BULLET_H__