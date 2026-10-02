#ifndef _BACKGROUND_
#define _BACKGROUND_
#include "characters.h"
#include "SDL.h"
#include "camera.h"
#include "map_handler.h"

class parallax_piece //A piece in the parallax. Makes the layers work properly. 
{
	public:
		double speed = 0;
		double x_offset = 0;
		double y_offset = 0;
		SDL_Texture* tex;
		double last_comparison = 0;
		void travel(double new_x) //Moves the parallax piece according to the new camera position.
		{
			x_offset += (new_x - last_comparison) * speed;
			last_comparison = new_x;
			if(x_offset > SCREEN_WIDTH)
			{
				x_offset = 0;
			}
			else if(x_offset < 0)
			{
				x_offset = SCREEN_WIDTH - 1;
			}
			renSpec(mainren,tex,SCREEN_WIDTH, SCREEN_HEIGHT, -x_offset,0);
			renSpec(mainren,tex,SCREEN_WIDTH, SCREEN_HEIGHT, 640 - x_offset, 0);
		}
		parallax_piece(double speed, SDL_Texture* tex)
		{
			this->speed = speed;
			this->tex = tex;
		}
};

std::vector<parallax_piece> forest_bg;

class background
{
	public:
	int origin_tile = 0;
	double curr_x = 0;
	int offset = 0;
	int move_points = 0;
	int speed = 1;
	void set() //Used for the old parallax. Now relies on the camera. 
	{
		if(move_points >= speed)
		{
			move_points = 0;
			offset--;
		}
		if(move_points <= -speed)
		{
			move_points = 0;
			offset++;
		}
		if(offset == -640 || offset == 640 || offset == 0)
		{
			offset = 0;
		}
	}	
	void render_tiles() //Renders the parallax pieces. 
	{
	//	if(g_map.active - 1 < 0)
	//	{
	//		renSpec(mainren,textures[24],SCREEN_WIDTH,SCREEN_HEIGHT,  640 + ((g_map.active * 640) - cam.x),0);
	//		renSpec(mainren,textures[24],SCREEN_WIDTH,SCREEN_HEIGHT, 0 - (cam.x - (g_map.active * SCREEN_WIDTH)),0);
	//	}
	//	else
	//	{
			//Implement multiple layers for parallax
	//		renSpec(mainren,textures[g_map.chunks[g_map.active + 1].loc],SCREEN_WIDTH,SCREEN_HEIGHT,  640 + ((g_map.active * 640) - cam.x),0);
		//	renSpec(mainren,textures[g_map.chunks[g_map.active].loc],SCREEN_WIDTH,SCREEN_HEIGHT, 0 - ((cam.x - (g_map.active * SCREEN_WIDTH))*1.5),0);	
	//	}
		forest_bg[0].travel(cam.x);
		forest_bg[1].travel(cam.x);
		forest_bg[2].travel(cam.x);
		forest_bg[3].travel(cam.x);
	}
};
background bg;
#endif
