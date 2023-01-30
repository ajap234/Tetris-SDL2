#include "sdlapp.hpp"

SDLApp::SDLApp()
{
	m_windowParameters.title = "";
	m_windowParameters.x = 0;
	m_windowParameters.y = 0;
	m_windowParameters.w = 0;
	m_windowParameters.h = 0;
	m_windowParameters.flags = 0;
	m_window = nullptr;
	
	m_rendererParameters.index = 0;
	m_rendererParameters.flags = 0;
	m_renderer = nullptr;
	
	m_keyboard = nullptr;
	m_event = {};
	m_running = false;
}
