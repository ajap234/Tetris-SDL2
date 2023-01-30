#ifndef SDLAPP_HPP
#define SDLAPP_HPP

#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

enum INIT_CHECK : bool { INIT_FAILURE, INIT_SUCCESS };

class SDLApp
{
	protected:
		SDLApp();
		~SDLApp() = default;
	
	public:
		virtual bool init() = 0;
		virtual void run() = 0;
	
	protected:
		struct { std::string title = ""; int x = 0; int y = 0; int w = 0; int h = 0; Uint32 flags = 0; } m_windowParameters = {};
		SDL_Window* m_window = nullptr;
		struct { int index = 0; Uint32 flags; } m_rendererParameters = {};
		SDL_Renderer* m_renderer = nullptr;
		SDL_Event m_event = {};
		bool m_running = false;
		const Uint8* m_keyboard = nullptr;
};

#endif // SDLAPP_HPP
