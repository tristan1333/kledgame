#ifndef _SPAWNERS_FILE_
#define _SPAWNERS_FILE_
#include <vector>
#include "characters.h"
#include "AI_Handler.h"
#include "collectibles.h"
#include "SDL.h"
#include "camera.h"
#include "terrain.h"
#include "map_handler.h"
#define ZIGGS 0
#define KAT 1
//CURRENTLY WORKING ON HOW TO REWORK THIS SYSTEM!!! PLACE THINGS INTO THE AI_LIST ACCORDING TO THE PLAYERS POSITION, SPAWNERS ARE NOT FOR THIS CONTEXT

#define UNIT 0
#define COLLECTIBLE 1
int boss_timer = 0; //BECAUSE AUTO SPAWNING A BOSS IS AIDS
int target_scores = 0;

void start_ziggs_fight()
{
	ziggs_fight = true;
	zigg.x = cam.x + 320;
}

class spawn_burst //Spawn burst makes certain units appear over time. 
{
	public:
		int burst_number = 0;
		int Rel_ID = 0;
		int burst_intervals = 0;
		int last_timer = 0;
		int type_id = 0;
		void spawn() //Spawns what it should according to where the player is. 
		{
			if(type_id == UNIT)
			{
			
			if(last_timer + burst_intervals > SDL_GetTicks())
			{
				return;
			}
			for(int i = 0;i < burst_number; i++)
			{
				int spawnloc_x = 0; 
				spawnloc_x = rand() % 400 + 330;
				if(rand()%2+1 == 1)
				{
					spawnloc_x = -spawnloc_x;
				}
				AI_List.push_back(simple_ai(cam.x + spawnloc_x, 130, Rel_ID));		
			}
				last_timer = SDL_GetTicks();
				return;	
			}
			else if(type_id == COLLECTIBLE)
			{
				if(Rel_ID == BOMB_MARKER)
				{
					if(last_timer + burst_intervals > SDL_GetTicks())
					{
						return;
					}
					last_timer = SDL_GetTicks();
					int spawn_x = rand()%SCREEN_WIDTH/2 + 0;
					for(int i = 0; i < burst_number; i++)
					{
						int spawn_x = rand()%SCREEN_WIDTH + 200 + 1;
					//	if(g_map.chunks[g_map.get_area(mainchar.x)].loc == CAVE_LOC)
					//	{
					//		return;
					//	}
					
						coll_List.push_back(collectibles(cam.x - SCREEN_WIDTH/2 + spawn_x, 100, textures[0],BOMB_MARKER,64,64, false));
					   // coll_List.push_back(collectibles(cam.x + spawn_x, 100, textures[0],BOMB_MARKER,64,64, false));
					//	spawn_x += 50;
					}
				}
			}
		}
		spawn_burst(int bn, int rid, int tim, int typ) //Initalizes the spawn_burst
		{
			burst_number = bn;
			Rel_ID = rid;
			burst_intervals = tim;
			type_id = typ;
			last_timer = SDL_GetTicks();
		}
};

std::vector<spawn_burst> s_burst = {spawn_burst(5,1,3000, UNIT),spawn_burst(1,2,7000, UNIT)/*, spawn_burst(8,BOMB_MARKER,3000, COLLECTIBLE)*/};
std::vector<spawn_burst> battle_bursts;

int spawn_timer = 0;
int BURST_NUMBER = 5; 

void Begin_Battle() //Begins a battle. Pauses the camera and changes the spawners. 
{
	battle_bursts.clear();
	battle_bursts.push_back(spawn_burst(6,1,2000,UNIT));
	battle_bursts.push_back(spawn_burst(10,1,10000,UNIT));
	battle_bursts.push_back(spawn_burst(6,BOMB_MARKER,10000,COLLECTIBLE));
	if(ACTIVE_BOSS == ZIGGS)
	{
		battle_bursts.push_back(spawn_burst(2, 3, 5000, UNIT));		
	}
	if(ACTIVE_BOSS == KAT)
	{
		battle_bursts.push_back(spawn_burst(1,4, 5000, UNIT));
	}
	cam.halt = true;
	battle = true;
	target_scores = mainchar.scores + (rand()%80+140);
//	start_ziggs_fight();
}

void begin_Fight() //WIP for bosses.
{
	battle_bursts.clear();
	battle_bursts.push_back(spawn_burst(10,1,10000,UNIT));
	if(ACTIVE_BOSS == ZIGGS)
	{
		start_ziggs_fight();
	}
}

void Dynamic_spawns() //The spawn handler. Makes sure enemies spawn and handles battles. 
{
	if(!battle)
	{
		for(int i = 0;i < s_burst.size();i++)
		{
			s_burst[i].spawn();	//Introduce thing later, if haven't killed x for y time, decrease archer spawn burst time
		}
	}
	else
	{
		for(int i = 0; i < battle_bursts.size();i++)
		{
			battle_bursts[i].spawn();
		}
		if(mainchar.scores > target_scores)
		{
			beat++;
			if(ACTIVE_BOSS == ZIGGS && beat >= 1)
			{
				if(!ziggs_fight)
				{
					begin_Fight();
					s_burst.push_back(spawn_burst(1, 3, 10000, UNIT));
				}
			}
			else
			{
				battle = false;
				cam.begin_CatchUp(cam.x - mainchar.x);	
			}
		}
	}
}

#endif
