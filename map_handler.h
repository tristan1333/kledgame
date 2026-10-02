#ifndef _MAP_HANDLER_T
#define _MAP_HANDLER_T
#define FOREST_LOC 24
#define CAVE_LOC 46
#include "collectibles.h"
//TURN MAP INTO A VECTOR OF POINTERS POINTING TO THE MAP OVER TO HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DEFINE THE CURRENT MAP VECTOR ACCORDING TO WHERE WE ARE RIGHT NOW!!!! ANYTHING OUTSIDE THE ACTIVE MAP IS NOT WORTH 
//MENTIONING ANYWAY, EFFECTIVELY ANYTHING OUTSIDE OF THE MAP SHOULD BE DELETED BUT THAT REQUIRES A BIT OF WORK IN OTHER AREAS!!!! GET THIS WORKING BEFORE CLEANING UP STUFF!!!!!!!!!!!!!!!!!!!!
class terrain_set //A set of terrain_pieces and aesthetic stuff.
{
	public:
		std::vector<terrain_piece> pieces;
		int loc = 0;
		void generate_terrain(int x)
		{
//			int target = rand() % 2 + 1;
//			if(target == 1)
	//		{
		//		int offset = rand()%600 + 1;
				pieces.push_back(terrain_piece(x,165,SCREEN_WIDTH,10));
			//}
		/*	if(target == 2)
			{
				int size_of_gap = rand() % 150 + 80;
				int start_of_gap = rand() % (SCREEN_WIDTH - 150) + 50;
				pieces.push_back(terrain_piece(x,220, start_of_gap, 10));
				pieces.push_back(terrain_piece(x + start_of_gap + size_of_gap,220,SCREEN_WIDTH - start_of_gap - size_of_gap,10));
			}*/
		}
		terrain_set(int x, int area)
		{
			loc = area;
			generate_terrain(x);
		}
};

class maps //The world map thatr is used. Contains all information about the pieces and the sets. 
{
	public:
		int active = -1;
		int max = 0;
		bool firstgen = false;
		std::vector <terrain_set> chunks;
		int get_area(int x)
   		{
    		return x/SCREEN_WIDTH; //Thus 630/SCREEN_WIDTH = 0. So, we are at 0 in the map. 1000/640 is 1, thus we are at 1 in the map. Moving to a file system, even this will work.
		}
		
		bool check_map(int x)
		{
			if(get_area(x) > max)
			{
			//	printf("Generating New Terrain Set");
				int area = 0;
				int collect = rand()%100 + 1;
				if(collect < 20)
				{
					coll_List.push_back(collectibles(x + (rand()%SCREEN_WIDTH + SCREEN_WIDTH * 2), 150, textures[159], MINE, 32, 16));
					printf("MINESPLACED");
				}
				if(collect > 85)
				{
					if(ACTIVE_BOSS == 0)//Ziggs
					{
						coll_List.push_back(collectibles(x + (rand()%SCREEN_WIDTH + SCREEN_WIDTH * 2),70,textures[164], FLAG, 50,90, false));
					}
					else if(ACTIVE_BOSS == 1)
					{
						coll_List.push_back(collectibles(x + (rand()%SCREEN_WIDTH + SCREEN_WIDTH * 2),70,textures[252], KFLAG, 50,90, false));
					}
				}
				if(chunks[get_area(x)].loc == CAVE_LOC)
				{
					area = rand()%6+1;
					if(area == 1)
					{
						area = FOREST_LOC;
					}
					else
					{
						area = CAVE_LOC;
					}
					printf("CAVE");
				}
				else
				{
					printf("FOREST");
					area = rand()%8+1;
					if(area == 1)
					{
						area = CAVE_LOC;
					}
					else
					{
						area = FOREST_LOC;
					}
				}
				chunks.push_back(terrain_set((max + 2) * 640, area));
				max++;
			}
			if(get_area(x) != active)
			{
				active = get_area(x);
				return false;
			}
			return true;
		}
		
		map()
		{
		}	
};

maps g_map; //G_map.


#endif
