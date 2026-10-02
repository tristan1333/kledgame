#ifndef TERRAIN_DEFINES_THL
#define TERRAIN_DEFINES_THL
#include <SDL.h>
#include <vector>
#include <ctime>
#include <cstdlib>
//#include "spawner.h"
//#include "AI_Handler.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class terrain_piece //A piece of terrain placed on the map. These are slotted into the map. 
{
	public:
		int x,y,w,h;
		terrain_piece(int x,int y,int w,int h) //With the way this works, technically it still renders pieces off the screen. However, that shouldn't really matter too much.
		{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
			printf("Created Terrain Piece");
		};
}; //Introduce a random generation system here which places terrain_pieces into a vector.


#endif
