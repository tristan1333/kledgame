#ifndef AI_DEF_VECT
#define AI_DEF_VECT
#include "SDL.h"
#include "characters.h"
#include <vector>
#include "camera.h"
//#include "terrain.h"
#include "projectile_manager.h"
#include "collectibles.h"
#include "Multi_Piece.h"
#include "config.h"
int ACTIVE_BOSS = 0;
int beat = 0;
bool ziggs_fight = false;
ziggs zigg(100, 100, -1);
void Begin_Battle();

bool battle;

std::vector <simple_ai> AI_List; 

void handle_AI(std::vector <terrain_piece> map, int rel_x, int rel_y, bool display_only = true) //Goes through all of the AI and makes sure they are rendered and tick through the world. 
{
	if(!display_only)
	{
		for(int i = 0;i < AI_List.size();i++)	
		{
		int renderx,rendery;
		if(AI_List[i].x  > cam.x)
			{
				renderx = SCREEN_WIDTH/2 + (AI_List[i].x - cam.x); 
			}
			else
			{
				renderx = SCREEN_WIDTH/2 - (cam.x - AI_List[i].x);
			}
			if(AI_List[i].y  > cam.y)
			{
				rendery = SCREEN_HEIGHT/2 + (AI_List[i].y  - cam.y);
			}
			else
			{
				rendery = SCREEN_HEIGHT/2 - (cam.y - AI_List[i].y);
			}	
		AI_List[i].handle_render(renderx,rendery);
		}
				return;
	}
	if(AI_List.size() > bodies)
	{
		for(int i = 0; i < AI_List.size();i++)
		{
			if(AI_List[i].hp > 0)
			{
				continue;
			}
			else
			{
				AI_List.erase(AI_List.begin() + i);
				i--;
				continue;
			}
		}
	} 
	for(int i = 0;i < AI_List.size();i++)
	{
		int renderx,rendery = 0;
		if(AI_List[i].x  > cam.x)
			{
				renderx = SCREEN_WIDTH/2 + (AI_List[i].x - cam.x); 
			}
			else
			{
				renderx = SCREEN_WIDTH/2 - (cam.x - AI_List[i].x);
			}
			if(AI_List[i].y  > cam.y)
			{
				rendery = SCREEN_HEIGHT/2 + (AI_List[i].y  - cam.y);
			}
			else
			{
				rendery = SCREEN_HEIGHT/2 - (cam.y - AI_List[i].y);
			}
		AI_List[i].handle_ai(map,rel_x,rel_y);	
		AI_List[i].handle_render(renderx,rendery);
		if(AI_List[i].AI_ID == AI_BOMBER && AI_List[i].hp > 0)
		{
			if(AI_List[i].fire)
			{
				AI_List[i].fire = false;
				double diff = mainchar.x - AI_List[i].x;
				coll_List.push_back(collectibles(32,32,AI_List[i].x, AI_List[i].y, mainchar.x, mainchar.y, AI_List[i].x + (diff/2),-100, textures[175]));
			}
		}
		
		if(AI_List[i].y > 1000)
		{
			AI_List.erase(AI_List.begin() + i);
			i--;
			continue;
		}
		if(AI_List[i].del)
		{
			AI_List.erase(AI_List.begin() + i);
			i--;
			continue;
		}
	}
}

void check_AI_Collision() //Checks every sort of collision. Enemies, projectiles, collectibles. Applies effects as well.
{
		for(int a = 0; a < coll_List.size();a++)
		{
			if(coll_List[a].ID == BOMB || coll_List[a].ID== BOUNCING_BOMB)
			{
				if(coll_List[a].activate && coll_List[a].anim_state == 0)
				{
					if(SDL_HasIntersection(&mainchar.pos_rect,&coll_List[a].coll_loc) && mainchar.last_hit + mainchar.hit_timer < SDL_GetTicks())
					{
						if(coll_List[a].ID == BOMB)
						{
							mainchar.take_damage(4, 0, 0, 0, 0);
						}
						else if(coll_List[a].ID == BOUNCING_BOMB)
						{
							mainchar.take_damage(1,0,0,0,0);
						}
					}
				}
			}
			else if(coll_List[a].ID == MINE)
			{
				if(!coll_List[a].pickup && coll_List[a].anim_state < 2)
				{
					if(SDL_HasIntersection(&mainchar.pos_rect, &coll_List[a].coll_loc) && mainchar.last_hit + mainchar.hit_timer < SDL_GetTicks())
					{
						double x_pw = .5;
						double y_pw = -.2;
						if(coll_List[a].x > mainchar.x + mainchar.w/2)
						{
							x_pw = -x_pw;
						}
						mainchar.take_damage(1,x_pw,y_pw,50,20);
					}
				}
			}
			else if(coll_List[a].ID == FLAG || coll_List[a].ID == LFLAG || coll_List[a].ID == KFLAG)
			{
				if(SDL_HasIntersection(&mainchar.weapon_rect, &coll_List[a].coll_loc) && mainchar.attacking)
				{
					if(!battle && !coll_List[a].activate)
					{
						Begin_Battle();
						coll_List[a].activate = true;
					}

				}
			}
			else
			{
				continue;
			}
		}
	for(int i = 0;i < AI_List.size();i++)
	{
		if(AI_List[i].hp < 1)
		{
			continue;		
		}
		if(SDL_HasIntersection(&mainchar.weapon_rect,&AI_List[i].pos_rect) && mainchar.attacking)
		{
			if(AI_List[i].last_hit + AI_List[i].hit_timer < SDL_GetTicks())
			{
				AI_List[i].hp--;
				if(AI_List[i].hp == 0)
				{
					AI_List[i].death_time = SDL_GetTicks();
				}
				AI_List[i].last_hit = SDL_GetTicks();
				double x_pw = .5;
				double y_pw = -.2;
				if(!mainchar.facing)
				{
					x_pw = -x_pw;
				}
				AI_List[i].set_knockback(x_pw,y_pw,50,20);
			}
		}

		for(int a = 0; a < coll_List.size();a++)
		{
			if(coll_List[a].ID == BOMB)
			{
				if(coll_List[a].activate)
				{
					if(SDL_HasIntersection(&AI_List[i].pos_rect,&coll_List[a].coll_loc))
					{
						AI_List[i].hp -= 4;
					}
				}
			}
			else
			{
				continue;
			}
		}
		bool hit = false;
		for(int a = 0; a < P_List.size();a++)
		{
			if(P_List[a].enemy)
			{
				
			
				if(P_List[a].collbox.x > mainchar.pos_rect.x + 100 || P_List[a].collbox.x < mainchar.pos_rect.x - 100)
				{
					continue;
				}
				if(SDL_HasIntersection(&P_List[a].collbox,&mainchar.pos_rect))
				{
					if(mainchar.last_hit + mainchar.hit_timer > SDL_GetTicks())
					{
						continue;
					}
					if(mainchar.ducking)
					{
						P_List[a].enemy = false;
						continue;
					}
					double x_pw = .5;
					double y_pw = -.2;
					if(P_List[a].velocity < 0)
					{
						x_pw = -x_pw;
					}
					mainchar.take_damage(1, x_pw, y_pw, 50, 20);
					P_List.erase(P_List.begin() + a);
					a--;
					continue;
				}
			}
			if(!P_List[a].enemy)
			{
				if(SDL_HasIntersection(&P_List[a].collbox,&AI_List[i].pos_rect))
				{
					double x_pw = .5;
					double y_pw = -.2;
					if(P_List[a].velocity < 0)
					{
						x_pw = -x_pw;
					}
					AI_List[i].hp--;
					AI_List[i].set_knockback(x_pw,y_pw,50,20);
					P_List.erase(P_List.begin() + a);
					a--;
					printf("Process test");
					continue;
				}
			}
		}
		if(SDL_HasIntersection(&mainchar.pos_rect,&AI_List[i].weapon_rect) && AI_List[i].attacking && AI_List[i].AI_ID == 1 && AI_List[i].model_state.aset[2].frame == 4) 
		{
			//printf("E");
			if(mainchar.last_hit + mainchar.hit_timer > SDL_GetTicks())
			{
				continue;
			}
			mainchar.last_hit = SDL_GetTicks();
			double x_pw = .5;
			double y_pw = -.2;
			if(!AI_List[i].facing)
			{
				x_pw = -x_pw;
			}	
			mainchar.take_damage(1, x_pw, y_pw, 50, 20);		
		}
	}
}

void handle_bosses(std::vector <terrain_piece> map, int rel_x, int rel_y)
{
	if(ziggs_fight)
	{
			double diff = mainchar.x - zigg.x;
			if(zigg.panic)
			{
				zigg.launch_bombs = false;
				if(zigg.satchel)
				{
					zigg.model_state.target = 3;
				}
				if(zigg.model_state.aset[3].frame == 12)
				{
					if(zigg.facing)
					{
						coll_List.push_back(collectibles(20,16,zigg.x + 35, zigg.y + 40, mainchar.x , mainchar.y, zigg.x,0, textures[214]));
						zigg.launchdirr = true;	
					}
					else
					{
						coll_List.push_back(collectibles(20,16,zigg.x, zigg.y + 40, mainchar.x , mainchar.y, zigg.x,0, textures[214]));
						zigg.launchdirr = false;	
					}

					coll_List[coll_List.size() - 1].activate = true;
					coll_List[coll_List.size() - 1].pickup = false;
					zigg.satchel = false;
					zigg.launch();
					zigg.model_state.aset[3].frame = 0;
				}
				if(zigg.launching)
				{
					zigg.process_launch();
				}
				
			}
			if(zigg.model_state.aset[2].frame == 9 && zigg.launch_bombs)
			{
				for(int targ = 4 - (zigg.hp/2); targ > 0; targ--)
				{
					int elevation = -(rand()%300 + 75);
					int x_offsetf = rand()%100 + 1;
					int x_offsetb = -(rand()%100 + 1);
					x_offsetf = rand()%100 + 1;
					x_offsetb = -(rand()%100 + 1);
					coll_List.push_back(collectibles(20,16,zigg.x, zigg.y, mainchar.x  + x_offsetb + x_offsetf, mainchar.y, zigg.x + (diff/2),elevation, textures[214]));
					x_offsetf = rand()%100 + 1;
					x_offsetb = -(rand()%100 + 1);
					coll_List.push_back(collectibles(20,16,zigg.x, zigg.y, mainchar.x + 100  + x_offsetb + x_offsetf, mainchar.y, zigg.x + (diff/2),elevation, textures[214]));
				}
				zigg.launch_bombs = false;
				zigg.attack_timer = SDL_GetTicks();
			}
		if(zigg.model_state.aset[2].frame == 16)
		{

			zigg.model_state.aset[2].frame = 0;
			zigg.model_state.target = 1;
			zigg.attacking = false;
		}
		if(zigg.sky_bombs)
		{
			int spawn_x = rand()%SCREEN_WIDTH/2 + 0;
			for(int i = 0; i < 4; i++)
			{
				int spawn_x = rand()%SCREEN_WIDTH  + 1;
				coll_List.push_back(collectibles(cam.x - SCREEN_WIDTH/2 + spawn_x, 100, textures[0],BOMB_MARKER,64,64, false));
			}
			zigg.sky_bombs = false;			
		}
	int renderx,rendery = 0;
		if(zigg.x  > cam.x)
			{
				renderx = SCREEN_WIDTH/2 + (zigg.x - cam.x); 
			}
			else
			{
				renderx = SCREEN_WIDTH/2 - (cam.x - zigg.x);
			}
			if(zigg.y  > cam.y)
			{
				rendery = SCREEN_HEIGHT/2 + (zigg.y  - cam.y);
			}
			else
			{
				rendery = SCREEN_HEIGHT/2 - (cam.y - zigg.y);
			}
		zigg.handle_ai(map,rel_x,rel_y);	
		zigg.handle_render(renderx - 15,rendery - 7);
		std::cout << renderx;
		if(SDL_HasIntersection(&mainchar.weapon_rect,&zigg.pos_rect) && mainchar.attacking)
		{
			if(zigg.last_hit + zigg.hit_timer < SDL_GetTicks() && !zigg.panic)
			{
				zigg.hp--;
				zigg.panic = true;
				if(zigg.hp < 1)
				{
					zigg.death_time = SDL_GetTicks();
					ziggs_fight = false;
					ACTIVE_BOSS++;
					beat = -1;
				}
				zigg.last_hit = SDL_GetTicks();
			}
		}
	}
}

#endif
