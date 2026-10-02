#ifndef GAME_WINDOW_RESIZE_T
#define GAME_WINDOW_RESIZE_T
#include <SDL.h>

class g_window;

class g_window
{
	public:
		int ww; //WINDOW WIDTH
		int wh; //WINDOW HEIGHT
		SDL_Window* win;
		void set_window_size()
		{
			SDL_GetWindowSize(wind,&ww,&wh);
		}
		g_window(SDL_Window* wind)
		{
			this->win = wind;
			this->Set_window_size();
		}
};
#endif
