#include <iostream>
#include <sstream>

#include "game.hpp"
#include "tetromino.hpp"
#include "linesystem.hpp"

Game::Game()
{
	m_grid = Grid();
	m_tetromino = Tetromino(m_grid);
	
	m_windowParameters.title = "Tetris"; 
	m_windowParameters.x = SDL_WINDOWPOS_CENTERED;
	m_windowParameters.y = SDL_WINDOWPOS_CENTERED;
	m_windowParameters.w = columnsNumber * blockSize;
	m_windowParameters.h = linesNumber * blockSize;
	m_windowParameters.flags = SDL_WINDOW_SHOWN;
                                
	m_rendererParameters.index = -1;
	m_rendererParameters.flags = SDL_RENDERER_ACCELERATED;
	
	m_keyboard = SDL_GetKeyboardState(nullptr);
	
	m_gameOver = false;
}

Game::~Game()
{
	SDL_DestroyTexture(m_lineCounterTextTexture);
	m_lineCounterTextTexture = nullptr;
	SDL_Log("Line counter text texture destroyed with success.\n");
	SDL_DestroyTexture(m_levelCounterTextTexture);
	m_levelCounterTextTexture = nullptr;
	SDL_Log("Level counter text texture destroyed with success.\n");
	SDL_DestroyTexture(m_gridBackground);
	m_gridBackground = nullptr;
	SDL_Log("Grid background texture destroyed with success.\n");
	Mix_FreeMusic(m_music);
	m_music = nullptr;
	SDL_Log("Music freed with success.\n");
	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;
	SDL_Log("Renderer destroyed with success.\n");
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	SDL_Log("Window destroyed with success.\n");
	Mix_CloseAudio();
	SDL_Log("Audio channels closed with success.\n");
	Mix_Quit();
	SDL_Log("SDL2 audio library quitted with success.\n");
	TTF_Quit();
	SDL_Log("SDL2 font library quitted with success.\n");
	IMG_Quit();
	SDL_Log("SDL2 image library quitted with success.\n"); 
	SDL_Quit();
	SDL_Log("SDL2 library quitted with success.\n");
}

bool Game::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while initialising SDL 2 library : %s\n", SDL_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("SDL 2 library initialized with success.\n");
	
	if (!IMG_Init(IMG_INIT_PNG))
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while initialising SDL 2 image library : %s\n", IMG_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("SDL 2 image library initialized with success.\n");
	
	if (TTF_Init() < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while initialising SDL 2 font library : % s\n", TTF_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("SDL 2 font library initialized with success.\n");

	m_gameFont = nullptr;

	if (!Mix_Init(MIX_INIT_OGG))
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while initialising SDL 2 sound library : %s\n", Mix_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("SDL 2 sound library initialized with success.\n");
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while opening audio channels : %s\n", Mix_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("Audio channels opened with success.\n");

	m_window = SDL_CreateWindow(m_windowParameters.title.c_str(), m_windowParameters.x, m_windowParameters.y, m_windowParameters.w, m_windowParameters.h, m_windowParameters.flags);
	if (!m_window)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while creating a window : %s\n", SDL_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("Window created with success.\n");
    
	m_renderer = SDL_CreateRenderer(m_window, m_rendererParameters.index, m_rendererParameters.flags);
	if (!m_renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while creating a renderer : %s\n", SDL_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("Renderer created with success.\n");

	SDL_Surface* backgroundSurface = IMG_Load("assets/images/space.png");
	if (!backgroundSurface)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while creating grid background surface : %s\n", SDL_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("Grid background surface created with success\n");

	m_gridBackground = SDL_CreateTextureFromSurface(m_renderer, backgroundSurface);
	if (!m_gridBackground)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while creating grid background texture : %s\n", SDL_GetError());
		return INIT_FAILURE;
	}

	SDL_FreeSurface(backgroundSurface);
	SDL_Log("Grid background texture created with success. Freeing the grid background surface...\n");

	SDL_RWops* musicFile = SDL_RWFromFile("assets/musics/tetris.ogg", "r");
	if (!musicFile)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while accessing the music file : %s\n", SDL_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("Music file opened with success.\n");

	m_music = Mix_LoadMUS_RW(musicFile, 0);
	if (!m_music)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error while loading the music : %s\n", SDL_GetError());
		return INIT_FAILURE;
	}
	SDL_Log("Music loaded with success.\n");
    
	m_running = true;
    
	return INIT_SUCCESS;
}

void Game::run()
{
	Mix_PlayMusic(m_music, -1);
	while (m_running)
	{
		m_beginTick = SDL_GetTicks();
		handleEvents();
		update();
		draw();
		m_endTick = SDL_GetTicks();
		m_deltaTime = m_beginTick - m_endTick;
		SDL_Delay((Uint32)(m_framerate / 1000 - m_deltaTime));
	}
	Mix_HaltMusic();
}

void Game::handleEvents()
{
	while (SDL_PollEvent(&m_event) > 0)
	{
		if (m_event.type == SDL_QUIT)
		{
			SDL_Log("Quitting the application...\n");
			m_running = false;
		}

		if (m_keyboard[SDL_SCANCODE_LEFT])
		{
			m_tetromino.moveLeft(m_grid);
		}
		
		if (m_keyboard[SDL_SCANCODE_RIGHT])
		{
			m_tetromino.moveRight(m_grid);
		}
        
		if (m_event.type == SDL_KEYDOWN && m_event.key.keysym.scancode == SDL_SCANCODE_Z)
		{                
			m_tetromino.rotateLeft(m_grid);
		}
		
		if (m_event.type == SDL_KEYDOWN && m_event.key.keysym.scancode == SDL_SCANCODE_E)
		{                
			m_tetromino.rotateRight(m_grid);
		}
        
		if (m_keyboard[SDL_SCANCODE_DOWN])
		{
			m_tetromino.fallFaster(m_grid);
		}
		
		if (m_keyboard[SDL_SCANCODE_UP])
		{
			m_tetromino.instantFall(m_grid);
		}

		if (m_keyboard[SDL_SCANCODE_SPACE] && m_gameOver)
		{
			m_playAgain = true;
		}

		if (m_keyboard[SDL_SCANCODE_ESCAPE])
		{
			pauseGame();
		}
	}
}

void Game::update()
{
	if (m_gameOver && m_playAgain)
	{
		m_grid = Grid();
		m_linesCount = 0;
		m_tetromino = Tetromino(m_grid);
		m_tetromino.resetFallingThreshold();
		m_tetromino.setBlockedState(false);
		m_playAgain = false;
		m_gameOver = false;
	}

	m_tetromino.updateTetromino();
	
	m_tetromino.handleUnlatchThreshold(m_grid);
	
	if(!m_isPaused && !m_gameOver && m_tetromino.getBlockedState())
	{
		m_grid.insertTetromino(m_tetromino);
		m_tetromino = Tetromino(m_grid);
		if(m_tetromino.getBlockedState())
		{
			m_gameOver = true;
		}
		
		else
		{
			m_tetromino.setBlockedState(false);
		}
	}
    
	int completed = checkLineCompletion(m_grid);
    
	if (completed != 0)
	{
		vanishLines(m_grid, completed);
		dropLines(m_grid, completed);
    
		int newLines = 0;
    
		for (int i = 0; i < linesNumber; i++)
		{
			if ((completed >> i) & 1)
			{
				newLines++;
			}
		}
    

		if (m_linesCount > 0 && /*(m_linesCount % 2) + newLines >= 2 &&*/ m_tetromino.getFallingThreshold() > 50)
		{
			m_tetromino.reduceUnlatchThreshold();
		}
	
		m_linesCount += newLines;
	}
    
}

// Coming back months later, this is absolute s#!$. No return check for the textures. Nuclear bomb as code.
void Game::draw()
{
	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00); // background color
	SDL_RenderClear(m_renderer);
 
	SDL_RenderCopy(m_renderer, m_gridBackground, nullptr, nullptr);
 
	SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x7F);
    
	m_grid.displayGrid(m_renderer, m_tetromino);
	m_tetromino.displayTetromino(m_renderer);
    
	TTF_CloseFont(m_gameFont);
	m_gameFont = TTF_OpenFont("assets/fonts/retro.ttf", blockSize * 2 / 3);
	if (!m_gameFont) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create font : %s\n", SDL_GetError());
    
	if (m_gameOver)
	{
		SDL_SetRenderDrawColor(m_renderer, 0xB0, 0x00, 0x00, 0x7F);
		SDL_Rect gameOverFadeRect = {0, 0, m_windowParameters.w, m_windowParameters.h};
		SDL_RenderFillRect(m_renderer, &gameOverFadeRect);
	}
    
	std::stringstream lines;
	lines << "Lines : " << m_linesCount;
	SDL_Color linesColor = {0xFF, 0xFF, 0xFF, 0xFF};
	SDL_Surface* linesCounter = TTF_RenderText_Solid(m_gameFont, lines.str().c_str(), linesColor);
	if (!linesCounter) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create linesCounter text surface : %s\n", SDL_GetError());
	m_lineCounterTextTexture = SDL_CreateTextureFromSurface(m_renderer, linesCounter);
	if (!m_lineCounterTextTexture) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create linesCounterTextTexture text texture : %s\n", SDL_GetError());
	SDL_FreeSurface(linesCounter);
	SDL_Rect linesRect;
	linesRect.x = blockSize;
	linesRect.y = blockSize;
	SDL_QueryTexture(m_lineCounterTextTexture, nullptr, nullptr, &linesRect.w, &linesRect.h);
	SDL_RenderCopy(m_renderer, m_lineCounterTextTexture, nullptr, &linesRect);
	SDL_DestroyTexture(m_lineCounterTextTexture);

	std::stringstream levels;
	levels << "Level : " << (m_linesCount / 10) + 1;
	SDL_Color levelsColor = {0xFF, 0xFF, 0xFF, 0xFF};
	SDL_Surface* levelsCounter = TTF_RenderText_Solid(m_gameFont, levels.str().c_str(), levelsColor);
	if (!levelsCounter) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create levelCounter text surface : %s\n", SDL_GetError());
	m_levelCounterTextTexture = SDL_CreateTextureFromSurface(m_renderer, levelsCounter);
	if (!m_levelCounterTextTexture) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create levelCounterTextTexture text texture : %s\n", SDL_GetError());
	SDL_FreeSurface(levelsCounter);
	SDL_Rect levelsRect;
	levelsRect.x = blockSize * columnsNumber - blockSize;
	levelsRect.y = blockSize;
	SDL_QueryTexture(m_levelCounterTextTexture, nullptr, nullptr, &levelsRect.w, &levelsRect.h);
	levelsRect.x -= levelsRect.w;
	SDL_RenderCopy(m_renderer, m_levelCounterTextTexture, nullptr, &levelsRect);
	SDL_DestroyTexture(m_levelCounterTextTexture);

	if (m_gameOver)
	{
		std::stringstream gameOver;
		TTF_CloseFont(m_gameFont);
		m_gameFont = TTF_OpenFont("assets/fonts/retro.ttf", 2 * blockSize);
		if (!m_gameFont) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create font : %s\n", SDL_GetError());
		gameOver << "Game Over";
		SDL_Color gameOverFadeColor = {0xFF, 0xFF, 0xFF, 0xFF};
		SDL_Surface* gameOverDisplay = TTF_RenderText_Solid(m_gameFont, gameOver.str().c_str(), gameOverFadeColor);
		if (!gameOverDisplay) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gameOverDisplay surface : %s\n", SDL_GetError());
		m_gameOverTextTexture = SDL_CreateTextureFromSurface(m_renderer, gameOverDisplay);
		if (!m_gameOverTextTexture) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gameOverTextTexture text texture : %s\n", SDL_GetError());
		SDL_FreeSurface(gameOverDisplay);
		SDL_Rect gameOverRect;
		gameOverRect.x = m_windowParameters.w / 2;
		gameOverRect.y = m_windowParameters.h / 2;
		SDL_QueryTexture(m_gameOverTextTexture, nullptr, nullptr, &gameOverRect.w, &gameOverRect.h);
		gameOverRect.x -= gameOverRect.w / 2;
		gameOverRect.y -= gameOverRect.h / 2;
		SDL_RenderCopy(m_renderer, m_gameOverTextTexture, nullptr, &gameOverRect);
		SDL_DestroyTexture(m_gameOverTextTexture);
	
		std::stringstream pressSpace;
		TTF_CloseFont(m_gameFont);
		m_gameFont = TTF_OpenFont("assets/fonts/retro.ttf", 2 * blockSize / 3);
		if (!m_gameFont) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create font : %s\n", SDL_GetError());
		pressSpace << "Press \"Space\" to retry...";
		SDL_Surface* pressSpaceDisplay = TTF_RenderText_Solid(m_gameFont, pressSpace.str().c_str(), gameOverFadeColor);
		if (!pressSpaceDisplay) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create pressSpaceDisplay surface : %s\n", SDL_GetError());
		m_pressSpaceTextTexture = SDL_CreateTextureFromSurface(m_renderer, pressSpaceDisplay);
		if (!m_pressSpaceTextTexture) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create pressSpaceTextTexture text texture : %s\n", SDL_GetError());
		SDL_FreeSurface(pressSpaceDisplay);
		SDL_Rect pressSpaceRect;
		pressSpaceRect.x = m_windowParameters.w / 2;
		pressSpaceRect.y = m_windowParameters.h / 2 + 2 * blockSize;
		SDL_QueryTexture(m_pressSpaceTextTexture, nullptr, nullptr, &pressSpaceRect.w, &pressSpaceRect.h);
		pressSpaceRect.x -= pressSpaceRect.w / 2;
		pressSpaceRect.y -= pressSpaceRect.h / 2;
		SDL_RenderCopy(m_renderer, m_pressSpaceTextTexture, nullptr, &pressSpaceRect);
		SDL_DestroyTexture(m_pressSpaceTextTexture);
	}
	
	SDL_RenderPresent(m_renderer);
}

void Game::pauseGame()
{
	m_isPaused = m_isPaused ? false : true;
	m_tetromino.setBlockedState(m_isPaused);
}

Uint32 Game::m_framerate = 120;
Uint32 Game::m_beginTick = 0;
Uint32 Game::m_endTick = 0;
Uint32 Game::m_deltaTime = 0;
