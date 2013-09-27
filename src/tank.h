#ifndef __TANK_H__
#define __TANK_H__

#include <memory>
#include <vector>
#include <Box2D/Box2D.h>

#include "i_physics_game_object.h"

class TankGame;

enum TankType
{
	Tank_Player,
	Tank_EnemySimple
};

class Tank : public IPhysicsGameObject
{
public:
	Tank(TankGame *game, TankType type, float x, float y);
	~Tank();

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

	TankType getTankType() const;

	void turnBody(float angle);
	void turnTurret(float angle);

	void fireBullet(float speed);
private:
	TankGame *game;
	b2Body   *body;

	TankType  type;

	bool alive;

	float x, y;
	
	float angle;
	float turret_angle;

	unsigned int body_color;
	unsigned int wheels_color;
	unsigned int turret_color;

	b2Vec2 shape_vertices[8];
};

typedef std::vector<std::shared_ptr<Tank>> TankList;

#endif // __TANK_H__