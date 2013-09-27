#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <memory>
#include <vector>

#include <Box2D/Box2D.h>

#include "i_physics_game_object.h"

class b2Body;
class TankGame;	

class Block : public IPhysicsGameObject
{
public:
	Block(TankGame *game, unsigned int color, std::vector<b2Vec2> vertices);
	~Block();

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


	static std::shared_ptr<Block> createRectangle(TankGame *game, unsigned int color, float x, float y, float w, float h);

private:
	TankGame *game;

	b2Body *body;

	unsigned int color;
	std::vector<b2Vec2> vertices;
};

typedef std::vector<std::shared_ptr<Block>> BlockList;

#endif // __GAME_LEVEL_BLOCK_H__