#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include <memory>
#include "i_game_object.h"

class TankGame;
class IGamePhysicsObject;

class TankGameLevel
{
public:
	TankGameLevel(TankGame *game, float width, float height);
	~TankGameLevel();

	void render(TankGame *game);

	void appendElement(std::shared_ptr<IGameObject> object);
	void garbageCollect();
public:
	TankGame *game;

	float     width;
	float     height;

	GameObjectList game_objects;
};

#endif // __GAME_LEVEL_H__