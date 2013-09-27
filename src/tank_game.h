#ifndef __TANK_GAME_H__
#define __TANK_GAME_H__

#include <vector>
#include <Box2D/Box2D.h>

#include "i_game_object.h"
#include "tank.h"
#include "bullet.h"

struct SDL_Window;
struct SDL_Renderer;
class  TankGameLevel;

class TankGameContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};

class TankGame
{
public:
	TankGame();
	~TankGame();

	bool initialize(bool fullscreen);

	void loadLevel01();
	void unloadLevel();

	void updateLogic(float seconds);
	void render();

	// Getters
	Tank *getPlayerTank();
	b2World *getPhysicsWorld();

	// In Game logic
	void addBullet(std::shared_ptr<Bullet> bullet);

private:
	void cleanup();
	void initializeGL();
	void setupViewport(float width, float height);

	void clearBackground();

	// SDL Objects
	SDL_Window    *SDLWindow;
	SDL_Renderer  *SDLRenderer;

	// Box2d Physics Objects
	b2World       *physics_world;
	TankGameContactListener contact_listener;

	TankGameLevel *level;
	std::shared_ptr<Tank> player_tank; 
	TankList       enemy_tanks;

	int            window_width;
	int            window_height;
};

#endif // __TANK_GAME_H__