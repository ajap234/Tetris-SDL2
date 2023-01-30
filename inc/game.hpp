#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "sdlapp.hpp"
#include "grid.hpp"
#include "tetromino.hpp"

class Game : public SDLApp
{
	public:
		Game();
		virtual ~Game();
		
	public:
		bool init() override;
		void run() override;
		
	private:
		void handleEvents();
		void update();
		void draw();
		void pauseGame();
		
	private:
		bool m_gameOver = false;
		bool m_playAgain = false;
		bool m_isPaused = false;
		TTF_Font* m_gameFont = nullptr;
		SDL_Texture* m_lineCounterTextTexture = nullptr;
		SDL_Texture* m_levelCounterTextTexture = nullptr;
		SDL_Texture* m_gameOverTextTexture = nullptr;
		SDL_Texture* m_pressSpaceTextTexture = nullptr;
		Grid m_grid = {};
		SDL_Texture* m_gridBackground = nullptr;
		Tetromino m_tetromino = {};
		Mix_Music* m_music = nullptr;
		int m_linesCount = 0;
		static Uint32 m_framerate;
		static Uint32 m_beginTick;
		static Uint32 m_endTick;
		static Uint32 m_deltaTime;
};

#endif // GAME_HPP
