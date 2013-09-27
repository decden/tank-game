#include "tank_game.h"

#include <memory>
#include <iostream>
#include <GL/gl.h>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>

#include "graphics_utils.h"
#include "block.h"
#include "tank_game_level.h"


void TankGameContactListener::BeginContact(b2Contact* contact) {
	//check if fixture A was a ball
	IPhysicsGameObject* body_a = static_cast<IPhysicsGameObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
	IPhysicsGameObject* body_b = static_cast<IPhysicsGameObject*>(contact->GetFixtureB()->GetBody()->GetUserData());

	if (body_a && body_b)
	{
		body_a->handleCollision(body_b);
		body_b->handleCollision(body_a);
	}
	
}

void TankGameContactListener::EndContact(b2Contact* contact) {
}


TankGame::TankGame()
	: SDLWindow(nullptr), SDLRenderer(nullptr), physics_world(nullptr), level(nullptr)
{
	// The game does not have gravity (but it might be fun just for test)
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);

	// Create the container of all that is physics...
	physics_world = new b2World(gravity);
	physics_world->SetContactListener(&contact_listener);
	//physics_world->SetDebugDraw(b2Draw*);
}

TankGame::~TankGame()
{
	this->cleanup();
}

bool TankGame::initialize(bool fullscreen)
{	
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		// FIXME: log failure
		this->cleanup();
		return false;
	}

	int flags = SDL_WINDOW_OPENGL;
	if (fullscreen)
	{
		// obtain screen size
		SDL_DisplayMode info;
		SDL_GetDesktopDisplayMode(0, &info);
		this->window_width  = info.w;
		this->window_height = info.h;
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	else
	{
		this->window_width  = 1200;
		this->window_height = 600;
		flags |= SDL_WINDOW_SHOWN;
	}

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

	// Create the window
	this->SDLWindow = SDL_CreateWindow("Test Tank Game",
		0, 0, this->window_width, this->window_height, flags);

	std::cout << "Creating window of " << this->window_width << "x" << this->window_height << std::endl;

	// Create the renderer
	this->SDLRenderer = SDL_CreateRenderer(this->SDLWindow, 0, 0);

	// Check that the renderer is actually OK
	SDL_RendererInfo displayRenderInfo;
	SDL_GetRendererInfo(this->SDLRenderer, &displayRenderInfo);
	if ((displayRenderInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
		(displayRenderInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0)
	{
		// FIXME: log failure
		this->cleanup();
		return false;
	}

	// Initialize OpenGL (actual viewport initialization is done while loading
	// level)
	this->initializeGL();

	return true;
}

void TankGame::loadLevel01()
{
	this->unloadLevel();

	// Create the level object, which contains all of the static data for it
	this->level = new TankGameLevel(this, 20, 15);

	// Outer Rim
	level->appendElement(Block::createRectangle(this, 0xac9393ff,  1,  0, 18,  1));
	level->appendElement(Block::createRectangle(this, 0xac9393ff, 19,  0,  1, 15));
	level->appendElement(Block::createRectangle(this, 0xac9393ff,  1, 14, 18,  1));
	level->appendElement(Block::createRectangle(this, 0xac9393ff,  0,  0,  1, 15));

	// Actual Blocks
	level->appendElement(Block::createRectangle(this, 0xac9393ff,  5,  5,  1,  5));
	level->appendElement(Block::createRectangle(this, 0xac9393ff, 13,  5,  1,  5));

	// A game list is mantained, so that util the level is not unloaded, the tanks
	// are in memory. Because of this, the rest of the game can correctly assume
	// that, unlike bullets, tanks are always in memory.

	// Player Tank
	player_tank = std::make_shared<Tank>(this, Tank_Player, 3, 3);
	level->appendElement(player_tank);

	// Enemy Tanks 
	auto enemy = std::make_shared<Tank>(this, Tank_EnemySimple, 17, 12);
	enemy_tanks.push_back(enemy);
	level->appendElement(enemy);

	this->setupViewport(level->width, level->height);
}

void TankGame::unloadLevel()
{
	this->enemy_tanks.clear();

	delete this->level;
	this->level = nullptr;
}

void TankGame::updateLogic(float seconds)
{
	if (level)
	{
		// General update step
		for (auto &object : level->game_objects)
		{
			if (object->isAlive()) object->update(seconds);
		}

		// Before physics step
		for (auto &object : level->game_objects)
		{
			if (object->hasPhysics() && object->isAlive()) object->toPhysicsObject()->beforePhysicsStep();
		}

		// Step physics simulation
		this->physics_world->Step(seconds, 1, 1);

		// After physics step
		for (auto &object : level->game_objects)
		{
			if (object->hasPhysics() && object->isAlive()) object->toPhysicsObject()->afterPhysicsStep();
		}

		// Remove dead objects
		level->garbageCollect();
		
	}
}

void TankGame::render()
{
	glLoadIdentity();

	// Clear background
	this->clearBackground();

	if (level) level->render(this);

	// Push the drawn pixel on the screen
	SDL_RenderPresent(this->SDLRenderer);
}

Tank *TankGame::getPlayerTank() { return this->player_tank.get(); }
b2World *TankGame::getPhysicsWorld() { return this->physics_world; }


void TankGame::addBullet(std::shared_ptr<Bullet> bullet)
{
	if (level)
	{
		level->appendElement(bullet);
	}
}

void TankGame::cleanup()
{
	this->unloadLevel();

	if (this->SDLRenderer) SDL_GL_DeleteContext(SDLRenderer);
	if (this->SDLWindow) SDL_DestroyWindow(SDLWindow);

	delete this->physics_world;
	this->physics_world = nullptr;

	SDL_Quit();
	this->unloadLevel();
}

void TankGame::initializeGL()
{
	glClearDepth(0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TankGame::setupViewport(float width, float height)
{
	width += 2;
	height += 2;

	float levelRatio = (float)width / height;
	float screenRatio = (float)window_width / window_height;

	float viewportWidth = width;
	float viewportHeight = height;

	if (levelRatio < screenRatio)
	{
		viewportWidth = height * screenRatio;
	}
	else
	{
		viewportHeight = width / screenRatio;
	}

	std::cout << "Vieport is [" << viewportWidth << ", " << viewportHeight << "]" << std::endl;
	std::cout << "Level is [" << width << ", " << height << "]" << std::endl;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewportWidth, viewportHeight, 0, 0, -100);

	glTranslatef((viewportWidth - width) / 2.0f + 1,
		(viewportHeight - height) / 2.0f + 1, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TankGame::clearBackground()
{
	GraphicsUtils::setClearColor(0xe3dbdb00);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}