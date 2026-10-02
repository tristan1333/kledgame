#ifndef MISC_GAME_OBJECTS_
#define MISC_GAME_OBJECTS_
#include "SDL.h"
#include "characters.h"
#include "load_textures.h"
#include "camera.h"
#include <cmath>
#include <memory>
#define MUSHROOM 1
#define SKAARL 2
#define SKAARL_RETURN 3
#define BOMB_MARKER 4
#define BOMB 5
#define MINE 6
#define FLAG 7
#define BOUNCING_BOMB 8
#define SATCHEL 9
#define LFLAG 10
#define KFLAG 11

struct Vector2 //x and y.
{
	double x;
	double y;
};

class collectibles //Used for everything misc. This includes banners, bombs, bouncing bombs, and mines. Anything with truly unique function. 
{
	public:
		double x = 0;
		double y = 0;
		int w = 0;
		int h = 0;
		int ID = 0;
		bool pickup = true;
		int timer = 0;
		int fall_timer = 0;
		int anim_state = 0;
		double x_vel = 0;
		double y_vel = 0;
		int bounces = 0;
		double travel_time = 0;
		int angle = 0;
		Vector2 point1;
		Vector2 point2;
		Vector2 point3;
		SDL_Rect coll_loc;
		bool travel = false;
		multi_piece multp;
		SDL_Texture* curr_tex; //Might be necessary to work it out similar to how we handle character animations. For now however, can use a single sprite.
		bool activate = false;
		void set_box() //Renders world co-ordinates to the screen. Used for things that don't use specialized rendering. 
		{
			if(ID == SKAARL || ID == SKAARL_RETURN || ID == BOMB_MARKER || ID == FLAG || ID == BOUNCING_BOMB)
			{
				return;
			}
			int renderx,rendery;
			if(x  > cam.x)
			{
				renderx = SCREEN_WIDTH/2 + (x - cam.x); 
			}
			else
			{
				renderx = SCREEN_WIDTH/2 - (cam.x - x);
			}
			if(y  > cam.y)
			{
				rendery = SCREEN_HEIGHT/2 + (y  - cam.y);
			}
			else
			{
				rendery = SCREEN_HEIGHT/2 - (cam.y - y);
			}	
			coll_loc.x = renderx;
			coll_loc.y = rendery;
			renSpec(mainren,curr_tex,w,h,renderx,rendery);
		}
			virtual void collect() //Used for when you pick something up. 
			{
				if(ID == MUSHROOM)
				{
					if(mainchar.attackspeed == 3)
					{
						return;
					}
				}
				if(ID == SKAARL_RETURN)
				{
					mainchar.mount();
				}
				if(ID == MINE)
				{
					activate = true;
					curr_tex = textures[147];
				}
				return;
			}
			bool routine(std::vector<terrain_piece> map) //Routine is something that needs to process in the world. A routine might be a bomb travelling, or the mount running away. It also determines whether or not it needs to be deleted. 
			{
				double runspeed = .4;
				if(ID == BOMB_MARKER)
				{
					if(fall_timer == 0)
					{
						fall_timer = SDL_GetTicks();
					}
					int renderx,rendery;
					if(x  > cam.x)
					{
						renderx = SCREEN_WIDTH/2 + (x - cam.x); 
					}
					else
					{
						renderx = SCREEN_WIDTH/2 - (cam.x - x);
					}
					if(y  > cam.y)
					{
						rendery = SCREEN_HEIGHT/2 + (y  - cam.y);
					}
					else
					{
						rendery = SCREEN_HEIGHT/2 - (cam.y - y);
					}

					if(fall_timer + 800 < SDL_GetTicks())
					{
						activate = true;
					}
					coll_loc.x = renderx;
					coll_loc.y = rendery;
					if(timer + 150 < SDL_GetTicks())
					{
						if(anim_state == 0)
						{
							curr_tex = textures[144];
							anim_state++;
						}
						else
						{
							curr_tex = textures[145];
							anim_state = 0;
						}
						timer = SDL_GetTicks();
					}
					renSpec(mainren,curr_tex,w,h,renderx,rendery);		
				}
				else if(ID == MINE)
				{
					if(activate)
					{
						w = 64;
						h = 64;
						y -= 50;
						activate = false;
					}
					if(timer + 35 < SDL_GetTicks() && !pickup)
					{
						timer = SDL_GetTicks();	
						if(anim_state == 0)
						{
							curr_tex = textures[148];
						}
						else if(anim_state == 1)
						{
							curr_tex = textures[149];						
						}
						else if(anim_state == 2)
						{
							curr_tex = textures[150];						
						}
						else if(anim_state == 3)
						{
							curr_tex = textures[151];						
						}
						else if(anim_state == 4)
						{
							curr_tex = textures[152];						
						}
						else if(anim_state == 5)
						{
							curr_tex = textures[153];						
						}
						else if(anim_state == 6)
						{
							curr_tex = textures[154];						
						}	
						else if(anim_state == 7)
						{
							curr_tex = textures[155];						
						}
						else if(anim_state == 8)
						{
							curr_tex = textures[156];					
						}
						else if(anim_state == 9)
						{
							curr_tex = textures[157];
						}
						else if(anim_state == 10)
						{
							curr_tex = textures[158];
						}
						else
						{
							return true;
						}
						anim_state++;
					}
					set_box();
				}
				else if(ID == SKAARL)
				{
					for(int i = 0;i <= map.size();i++)
					{
						if(map[i].w == 0 || (map[i].x + map[i].w) < cam.x - SCREEN_WIDTH/2 || map[i].x >= cam.x + SCREEN_WIDTH/2 || map[i].y < cam.y - SCREEN_HEIGHT/2 || map[i].y > cam.y + SCREEN_HEIGHT)
						{
							continue;
						}
						if(this->x + this->w > map[i].x && this->x < map[i].x + map[i].w && this->y + this->h >= map[i].y && this->y <= map[i].y + map[i].h)
						{
					
							if(this->y + this->h > map[i].y+ map[i].h)
							{
								this->y += (this->y  - (map[i].y + map[i].h)) + 2;
								break;
							}
						this->y -= (this->y + this->h - map[i].y);
						break;
						}
						else
						{
								y += .1 * ticks_per_frame;
								fall_timer = SDL_GetTicks();
						}
					}
					x +=  runspeed * ticks_per_frame;
					if(timer + 75 < SDL_GetTicks())
					{
						timer = SDL_GetTicks();

						if(anim_state < 7)
						{
							if(anim_state == 0)
							{
								curr_tex = textures[136];
							}
							else if(anim_state == 1)
							{
								curr_tex = textures[137];
							}
							else if(anim_state == 2)
							{
								curr_tex = textures[138];
							}
							else if(anim_state == 3)
							{
								curr_tex = textures[139];
							}
							else if(anim_state == 4)
							{
								curr_tex = textures[140];
							}
							else if(anim_state == 5)
							{
								curr_tex = textures[141];
							}
							else if(anim_state == 6)
							{
								curr_tex = textures[142];
							}
							else if(anim_state == 7)
							{
								curr_tex = textures[143];
							}
							anim_state++;
						}
						else
						{
							curr_tex = textures[136];
							anim_state = 0;
						}
					}
					int renderx,rendery;
					if(x  > cam.x)
					{
						renderx = SCREEN_WIDTH/2 + (x - cam.x); 
					}
					else
					{
						renderx = SCREEN_WIDTH/2 - (cam.x - x);
					}
					if(y  > cam.y)
					{
						rendery = SCREEN_HEIGHT/2 + (y  - cam.y);
					}
					else
					{
						rendery = SCREEN_HEIGHT/2 - (cam.y - y);
					}
					coll_loc.x = renderx;
					coll_loc.y = rendery;
					renSpec(mainren,curr_tex,w,h,renderx,rendery);						
				}
				else if(ID == FLAG || ID == LFLAG || ID == KFLAG)
				{
					int renderx,rendery;
					if(x  > cam.x)
					{
						renderx = SCREEN_WIDTH/2 + (x - cam.x); 
					}
					else
					{
						renderx = SCREEN_WIDTH/2 - (cam.x - x);
					}
					if(y  > cam.y)
					{
						rendery = SCREEN_HEIGHT/2 + (y  - cam.y);
					}
					else
					{
						rendery = SCREEN_HEIGHT/2 - (cam.y - y);
					}
					coll_loc.x = renderx;
					coll_loc.y = rendery;
					set_box();
					renSpec(mainren,curr_tex,w,h,renderx,rendery);	
					if(timer + 200 < SDL_GetTicks())
					{
						if(ID == FLAG)
						{
							timer = SDL_GetTicks();
							if(anim_state == 0)
							{
								curr_tex = textures[164];
							}
							else if(anim_state == 1)
							{
								curr_tex = textures[170];
							}
							else if(anim_state == 2)
							{
								curr_tex = textures[171];
							}
							else if(anim_state == 3)
							{
								curr_tex = textures[172];
							}
							else if(anim_state == 4)
							{
								curr_tex = textures[173];
							}
							else
							{
								curr_tex = textures[164];
								anim_state = 0;
							}
							anim_state++;
						}
						if(ID == LFLAG)
						{
							timer = SDL_GetTicks();
							if(anim_state == 0)
							{
								curr_tex = textures[252];
							}
							else if(anim_state == 1)
							{
								curr_tex = textures[253];
							}
							else if(anim_state == 2)
							{
								curr_tex = textures[254];
								anim_state = -1;
							}
							anim_state++;
						}
						if(ID == KFLAG)
						{
							timer = SDL_GetTicks();
							if(anim_state == 0)
							{
								curr_tex = textures[258];
							}
							else if(anim_state == 1)
							{
								curr_tex = textures[259];
							}
							else if(anim_state == 2)
							{
								curr_tex = textures[260];
							}
							else if(anim_state == 3)
							{
								curr_tex = textures[261];
								anim_state = -1;
							}
							anim_state++;
						}
					}
				}
				else if(ID == SKAARL_RETURN)
				{
					if(timer + 75 < SDL_GetTicks())
					{
						timer = SDL_GetTicks();

						if(anim_state < 7)
						{
							if(anim_state == 0)
							{
								curr_tex = textures[136];
							}
							else if(anim_state == 1)
							{
								curr_tex = textures[137];
							}
							else if(anim_state == 2)
							{
								curr_tex = textures[138];
							}
							else if(anim_state == 3)
							{
								curr_tex = textures[139];
							}
							else if(anim_state == 4)
							{
								curr_tex = textures[140];
							}
							else if(anim_state == 5)
							{
								curr_tex = textures[141];
							}
							else if(anim_state == 6)
							{
								curr_tex = textures[142];
							}
							else if(anim_state == 7)
							{
								curr_tex = textures[143];
							}
							anim_state++;
						}
						else
						{
							curr_tex = textures[136];
							anim_state = 0;
						}
					}
					SDL_RendererFlip flip = SDL_FLIP_NONE;
						if(y > mainchar.y + 5)
						{
							y -= (runspeed) * ticks_per_frame;
						}
						else if(y < mainchar.y - 5)
						{
							y += runspeed * ticks_per_frame;
						}
						if(x > mainchar.x)
						{
							x -= .35 * ticks_per_frame;
						}
						else if(x < mainchar.x)
						{
							x += .35 * ticks_per_frame;
						}

					if(x > mainchar.x)
					{
						flip = SDL_FLIP_HORIZONTAL;	
					}
					double renderx,rendery;
					if(x  > cam.x)
					{
						renderx = SCREEN_WIDTH/2 + (x - cam.x); 
					}
					else
					{
						renderx = SCREEN_WIDTH/2 - (cam.x - x);
					}
					if(y  > cam.y)
					{
						rendery = SCREEN_HEIGHT/2 + (y  - cam.y);
					}
					else
					{
						rendery = SCREEN_HEIGHT/2 - (cam.y - y);
					}
					coll_loc.x = renderx;
					coll_loc.y = rendery;
					SDL_Rect rect = {renderx,rendery,w,h};
					SDL_RenderCopyEx(mainren,curr_tex,NULL,&rect, 0,NULL,flip);
				}
				else if(ID == BOUNCING_BOMB)
				{
					if(bounces == 2)
					{
						activate = true;
						pickup = false;
					}
					if(!activate)
					{
						if(timer + 25 < SDL_GetTicks())
						{
							timer = SDL_GetTicks();
					 		double diff = point3.x - point2.x;
							for(int i = 0;i <= map.size();i++)
							{
								if(map[i].w == 0 || (map[i].x + map[i].w) < cam.x - SCREEN_WIDTH/2 || map[i].x >= cam.x + SCREEN_WIDTH/2 || map[i].y < cam.y - SCREEN_HEIGHT/2 || map[i].y > cam.y + SCREEN_HEIGHT)
								{
									continue;
								}
								if(this->x + this->w > map[i].x && this->x < map[i].x + map[i].w && this->y + this->h >= map[i].y && this->y <= map[i].y + map[i].h)
								{
					
									if(this->y + this->h > map[i].y+ map[i].h)
									{
									//	this->y += (this->y  - (map[i].y + map[i].h)) + 2;
										if(diff >= 0)
										{
											point3.x = point3.x + 100;
										}
										else
										{
											point3.x = point3.x - 100;
										}
										point3.y = y;
										point1.x = x;
										point1.y = y;
										point2.x = x + ((point3.x - point1.x)/2);
										point2.y = point2.y/2;
										travel_time = 0.01;
										bounces++;
										break;
									}
								}
							}
							 diff = point2.x - point1.x;
							double xa = point1.x + (diff * travel_time);
							diff = point2.y - point1.y;
							int ya = point1.y + (diff * travel_time);
							diff = point3.x - point2.x;
							if(diff > 0)
							{
								angle += 1;
							}
							else
							{
								angle -= 1;
							}

							double xb = point2.x + (diff * travel_time);
							diff = point3.y - point2.y;
							double yb = point2.y + (diff * travel_time);
							diff = xb - xa;
							double world_x = xa + (diff * travel_time);
							diff = yb - ya;
							double world_y = ya + (diff * travel_time);
							x = world_x;
							y = world_y;
							travel_time += 0.02;
							coll_loc.w = 5;
							coll_loc.h = 5;

						}
					}
					else
					{
						angle = 0;
						if(timer + 35 < SDL_GetTicks())
						{
							printf("Wot");
							if(anim_state == 0)
							{
								curr_tex = textures[148];
							}
							else if(anim_state == 1)
							{
								curr_tex = textures[149];
							}
							else if(anim_state == 2)
							{
								curr_tex = textures[150];
							}
							else if(anim_state == 3)
							{
								curr_tex = textures[151];
							}
							else if(anim_state == 4)
							{
								curr_tex = textures[152];
							}
							else if(anim_state == 5)
							{
								curr_tex = textures[153];
							}
							else if(anim_state == 6)
							{
								curr_tex = textures[154];
							}
							else if(anim_state == 7)
							{
								curr_tex = textures[155];
							}
							else if(anim_state == 8)
							{
								curr_tex = textures[156];
							}
							else if(anim_state == 9)
							{
								curr_tex = textures[157];
							}
							else if(anim_state == 10)
							{
								curr_tex = textures[158];
							}
							timer = SDL_GetTicks();
							anim_state++;
							if(anim_state > 10)
							{
								return true;
							}	
						}
					}
					double renderx,rendery;
				 	if(x  > cam.x)
			 		{
				 		renderx = SCREEN_WIDTH/2 + (x - cam.x); 
			 	 	}
			 		else
					{
						renderx = SCREEN_WIDTH/2 - (cam.x - x);
					}
					if(y  > cam.y)
					{
						rendery = SCREEN_HEIGHT/2 + (y  - cam.y);
					}
					else
					{
						rendery = SCREEN_HEIGHT/2 - (cam.y - y);
					}
					coll_loc.x = renderx;
					coll_loc.y = rendery;
					coll_loc.w = 16;
					coll_loc.h = 10;
					SDL_Rect rect = {renderx,rendery,w,h};
					SDL_RenderCopyEx(mainren, curr_tex, NULL, &rect, angle, NULL, SDL_FLIP_NONE);	
				}
				else if(ID == SATCHEL)
				{
					
				}
				else if(ID == BOMB)
				{
					set_box();
					x += x_vel;
					y += y_vel;
					if(y > SCREEN_WIDTH * 2)
					{
						return true;
					}
					
					if(activate)
					{
						if(timer + 35 < SDL_GetTicks())
						{
							if(anim_state == 0)
							{
								curr_tex = textures[148];
							}
							else if(anim_state == 1)
							{
								curr_tex = textures[149];
							}
							else if(anim_state == 2)
							{
								curr_tex = textures[150];
							}
							else if(anim_state == 3)
							{
								curr_tex = textures[151];
							}
							else if(anim_state == 4)
							{
								curr_tex = textures[152];
							}
							else if(anim_state == 5)
							{
								curr_tex = textures[153];
							}
							else if(anim_state == 6)
							{
								curr_tex = textures[154];
							}
							else if(anim_state == 7)
							{
								curr_tex = textures[155];
							}
							else if(anim_state == 8)
							{
								curr_tex = textures[156];
							}
							else if(anim_state == 9)
							{
								curr_tex = textures[157];
							}
							else if(anim_state == 10)
							{
								curr_tex = textures[158];
							}
							timer = SDL_GetTicks();
							anim_state++;
							if(anim_state > 10)
							{
								return true;
							}
						}		
					}
					else
					{
						if(timer + 75 < SDL_GetTicks())
						{
							if(anim_state == 0 || anim_state == 3)
							{
								curr_tex = textures[146];
								
							}
							else if(anim_state == 1)
							{
								curr_tex = textures[165];
							}
							else if(anim_state == 2)
							{
								curr_tex = textures[166];
								anim_state = -1;
							}
							timer = SDL_GetTicks();
							anim_state++;
						}
					}
					for(int i = 0;i <= map.size();i++)
					{
						if(this->x + this->w > map[i].x && this->x < map[i].x + map[i].w && this->y + this->h >= map[i].y && this->y <= map[i].y + map[i].h && !activate)
						{
							activate = true;
							x_vel = 0;
							y_vel = 0;
							anim_state = 0;
							timer = SDL_GetTicks();
							curr_tex = textures[147];
							break;
						}
					}
				}
				else
				{
					return false;
				}
				return false;
			}
		
			collectibles();
			collectibles(double x_, double y_, SDL_Texture* tex,int I, int w_, int h_, bool pick = true, double targx = -1000, double targy = -1000)
			{
				pickup = pick;
				x = x_;
				y = y_;
				ID = I;
				curr_tex = tex;
				w = w_;
				h = h_;
				coll_loc.w = w;
				coll_loc.h = h;
				timer = SDL_GetTicks();
				printf("Made collectible");
				double distance = 0;
				if(targx > -1000 && targy > -1000)
				{
					distance = sqrt((x - targx) * (x-targx) + (targy-y)*(targy-y));
				}
				else
				{
					return;
				}
				x_vel = ((.4 * ticks_per_frame)*(targx - x)/distance);
				y_vel = ((.4 * ticks_per_frame)*(targy - y)/distance);	
			}
			collectibles(int w, int h, double start_x, double start_y, double end_x, double end_y, double mid_x, double mid_y, SDL_Texture* tex);
};

collectibles::collectibles(int w, int h, double start_x, double start_y, double end_x, double end_y, double mid_x, double mid_y, SDL_Texture* tex) //Different inputs. 
{
	this->w = w;
	this->h = h;
	x = start_x;
	y = start_y;
	point1.x = start_x;
	point1.y = start_y;
	point3.x = end_x;
	point3.y = end_y;
	point2.x = mid_x;
	point2.y = mid_y;
	this->curr_tex = tex;
	timer = SDL_GetTicks();
	ID = BOUNCING_BOMB;
}

std::vector<collectibles> coll_List;

void handle_collectibles(std::vector<terrain_piece> map) //Handles all the collectibles in the world. Whether they are picked up etc.
{
	for(int i = 0; i < coll_List.size();i++)
	{
		if(coll_List[i].pickup)
		{

			coll_List[i].set_box();
			if(SDL_HasIntersection(&mainchar.pos_rect,&coll_List[i].coll_loc))
			{
				if(coll_List[i].ID == MINE || coll_List[i].ID == BOUNCING_BOMB)
				{
					coll_List[i].activate = true;
					coll_List[i].pickup = false;
					coll_List[i].timer = SDL_GetTicks();
					coll_List[i].anim_state = 0;
					i--;

					continue;
				}
				coll_List[i].collect();
				coll_List.erase(coll_List.begin() + i);
				i--;
				continue;
			}
		}
		if(coll_List[i].routine(map))
		{
				coll_List.erase(coll_List.begin() + i);
				i--;
				 continue;
		}
		if(coll_List[i].activate && coll_List[i].ID == BOMB_MARKER)
		{
			int x = 0;
			x = rand()% 900 + 1;	
			coll_List.push_back(collectibles(mainchar.x - (SCREEN_WIDTH/2) + x,-50 - (SCREEN_HEIGHT/2 - 50),textures[146],BOMB, 40,40,false,coll_List[i].x, coll_List[i].y)); //Random bombs
			//coll_List.push_back(collectibles(coll_List[i].x,-50 - (SCREEN_HEIGHT/2 - 50),textures[146],BOMB, 40,40,false,coll_List[i].x, coll_List[i].y));
			coll_List.erase(coll_List.begin() + i);
			i--;
			continue;
		}
	//	else if(coll_List[i].active && coll_List[i].ID == BOMB)
	//	{
			
	//	}
	}
}

#endif
