#ifndef _FRAME_MANAGER
#define _FRAME_MANAGER

#include "SDL.h"
#include "string"

const int TARGET_FPS = 60;
const double ticks_per_frame = 1000/TARGET_FPS;

int last_time;
double time_offset;

int get_Ticks() //Gets the ticks passed since the last pass. 
{
	int time;
	time = SDL_GetTicks() - last_time;
	return time;
}

void Calculate_FPS_time(double curr_ticks) //Calculates the FPS_time.
{
	time_offset = curr_ticks - last_time;
	time_offset = time_offset/1000;
}

#endif
