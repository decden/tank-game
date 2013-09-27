#include <iostream>
#include <SDL2/SDL.h>

#include "tank_game.h"



int main(int argc, char *argv[])
{
	TankGame game;

	float tick_duration = 1000.0f / SDL_GetPerformanceFrequency();
	float frame_ticks = SDL_GetPerformanceFrequency() * 1.0f / 60.0f;
	auto timer = SDL_GetPerformanceCounter();

	if (game.initialize(false))
	{
		game.loadLevel01();

		for (int i = 0; i < 500; ++i)
		{
			if (game.getPlayerTank())
			{
				game.getPlayerTank()->turnTurret(0.02);
				if (i % 4 == 0) game.getPlayerTank()->fireBullet(5.0f);
			}
			
			game.updateLogic(1.0f/60.0);
			game.render();

			auto now = SDL_GetPerformanceCounter();
			float delay = frame_ticks - (float)(now - timer);
			if (delay > 0)
			{
				SDL_Delay(delay * tick_duration);
			}
			timer = SDL_GetPerformanceCounter();
		}
	}
}