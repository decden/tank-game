#include "tank_game_level.h"

#include <iostream>

#include "tank_game.h"
#include "graphics_utils.h"
#include "i_game_object.h"
#include "i_physics_game_object.h"

TankGameLevel::TankGameLevel(TankGame *game, float width, float height)
	: game(game), width(width), height(height)
{ }

TankGameLevel::~TankGameLevel()
{
	for (auto &object : game_objects)
	{
		if (object->hasPhysics())
		{
			object->toPhysicsObject()->unregisterPhysicsObject(game->getPhysicsWorld());
		}
	}
}

void TankGameLevel::render(TankGame *game)
{
	// Draw Background elements
	GraphicsUtils::setColor(0x221c24ff);
	GraphicsUtils::fillRectangle(0.0f, 0.0f, this->width, this->height, 0.0f);

	// Draw all of the graphical elements...
	for (auto &object : game_objects)
	{	
		if (object->isAlive())
		{
			object->draw((int)object->getObjectType());
		}
	}
}

void TankGameLevel::appendElement(std::shared_ptr<IGameObject> object)
{
	game_objects.push_back(object);

	// Register object to physics, if it is a physics object
	if (object->hasPhysics())
	{
		object->toPhysicsObject()->registerPhysicsObject(game->getPhysicsWorld());
	}
}

void TankGameLevel::garbageCollect()
{
	GameObjectList alive_objects;
	for (auto &object : game_objects)
	{
		if (object->isAlive())
		{
			alive_objects.push_back(object);
		}
		else 
		{
			if (object->hasPhysics())
			{
				object->toPhysicsObject()->unregisterPhysicsObject(game->getPhysicsWorld());
			}
		}
	}
	game_objects = alive_objects;
}