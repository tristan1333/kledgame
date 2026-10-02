#ifndef CHARACTERS_DEF_T
#define CHARACTERS_DEF_T
#include <vector>
#include <SDL.h>
#include <string>
#include "camera.h"
#include "renderer.h"
#include "terrain.h"
#include "load_textures.h"
#include "animation_sets.h"
#include "Multi_Piece.h"
//#include "background.h"
#include "projectile_manager.h"
#include "fps_handler.h"
#include <cstdlib>
#include <cmath>
#define LEGS 2
#define TORSO 1
#define WEAPON 0
#define AI_SIMPLE 1
#define AI_ARCHER 2
#define AI_BOMBER 3
#define AI_ROGUE 4
#define SKAARL_IDLE 0
#define SKAARL_WALK 1
#define SKAARL_JUMP 2
#define KLED_IDLE 3
#define KLED_WALK 4
#define KLED_ATTACK 5
#define KLED_DUCK 6
#define KLED_JUMP 7
#define KLED_GUN 8
#define KLED_MOUNTED 9
#define SKAARL_ATTACK 12
#define SKAARL_LEGS 11
#define KLED_MOUNT_WALK 10
#define SKAARL_DASH 13

//class spawner;
class character;

class simple_ai;

class character
{
	public:
		int hp, dmg, mob, def, uti,b_dam,w,h;
		double x,y, x_offset,y_offset;
		float m_speed;
		bool kill = true;
		int last_move = 0;
		int last_drop = 0;
		int y_timer = 0;
		int hit_timer = 250;
		int last_hit = 0;
		int attackspeed = 10;
		int corpse_time = 2000;
		int death_time = 0;
	//	int *spawnrefv;
		bool del = false;
		SDL_Rect weapon_rect;
		SDL_Rect pos_rect;
		int leg_pointer = 2;
		int attack_timer = 0;
		int last_rise = 0;
		int air_time = 300;
		int hit_box_x_set = 0;
		int hit_box_h_bon = 0;
		int hit_box_y_set = 0;
		double knockback_x_lim = 0;
		double knockback_y_lim = 0;
		double knockback_x_pw = 0;
		double knockback_y_pw = 0;
		double knockback_curr_x = 0;
		double knockback_curr_y = 0;
		int render_x_ = 0;
		int render_y_ = 0;
		bool facing = true; //False = left, True = right
		bool immobile = false;
		bool custom_weapon_box = true;
		bool multi_model;
		bool stunned = false;
		bool ducking = false;
		bool blocking = false;
		bool rising = false;
		bool attacking = false;
		bool air = true;
		bool alive = true;
		bool made_move = false;
		multi_piece model_state;
		std::string name;
		SDL_Texture * port;
		SDL_Texture * model;
		character()
		{
			return;
		} 
		virtual void start_attack() //Begins the attack and gets things ready.
		{
			return;
		}
		virtual void handle_attack() //Handles the attack
		{
			return;
		}
		virtual void handle_walk_model() //Handles the walking animation.
		{
			return;
		}
		void set_knockback(double x_p,double y_p,double x_lim, double y_lim) //Sets the knockback variables.
		{
			knockback_x_lim = x_lim;
			knockback_y_lim = y_lim;
			knockback_x_pw = x_p;
			knockback_y_pw = y_p;
			knockback_curr_x = 0;
			knockback_curr_y = 0;
		}
		virtual void knock_back(std::vector <terrain_piece> map) //Handles the knockback where they move involuntarily. 
		{
			immobile = true;
			x += knockback_x_pw * ticks_per_frame;
			y += knockback_y_pw * ticks_per_frame;
			knockback_curr_x += knockback_x_pw * ticks_per_frame;
			knockback_curr_y += knockback_y_pw * ticks_per_frame;
			if(std::abs(knockback_curr_x) > std::abs(knockback_x_lim) && std::abs(knockback_curr_y) > std::abs(knockback_y_lim))
			{
				immobile = false;
				knockback_x_pw = 0;
				knockback_y_pw = 0;
			}
		}
		virtual void handle_spec_actions(int diff, std::vector<terrain_piece> map) //Used extensively for the AI to alter how they work. Seperates an archer from a Warrior. 
		{
			return;
		}
		bool can_move()
		{
			if(!immobile)
			{
				return true;
			}
			return false;
		}
		virtual void duck()
		{
			this->ducking = true;
			return;
		}
		void jump() //Jump. 
		{
			if(this->air)
			{
				return;
			}
			else
			{
				this->air = true;
				this->rising = true;
				//wcam.halt = true;
			}
		}
		
		void set_render() //Sets hit boxes according to world position. 
	    {
			int renderx, rendery;
			if(this->x  > cam.x)
			{
				renderx = SCREEN_WIDTH/2 + (this->x - cam.x); 
			}
			else
			{
				renderx = SCREEN_WIDTH/2 - (cam.x - this->x);
			}
			if(this->y  > cam.y)
			{
				rendery = SCREEN_HEIGHT/2 + (this->y - cam.y);
			}
			else
			{
				rendery = SCREEN_HEIGHT/2 - (cam.y - this->y);
			}
			this->pos_rect.x = renderx;
			this->pos_rect.y = rendery + hit_box_y_set;
			this->pos_rect.w = w + hit_box_x_set;
			render_x_ = renderx;
			render_y_ = rendery;
			return;
		}
		
		virtual void move(double v, std::vector<terrain_piece> map, bool fuck = false) //Moves the character. Takes into consideration the terrain and updates the characters walking model. 
		{ 
			this->x += v * ticks_per_frame;
			made_move = true;
			this->handle_walk_model();
			if(!fuck)
			{
			
			for(int i = 0;i <= map.size();i++)
			{
				if(map[i].w == 0 || (map[i].x + map[i].w) < cam.x - SCREEN_WIDTH/2 || map[i].x >= cam.x + SCREEN_WIDTH/2 || map[i].y < cam.y - SCREEN_HEIGHT/2 || map[i].y > cam.y + SCREEN_HEIGHT)
				{
					continue;
				}
				if(this->x + this->w + this->x_offset > map[i].x && this->x < map[i].x + map[i].w && this->y + this->h + this->y_offset > map[i].y && this->y < map[i].y + map[i].h)
				{
					if(this->x < map[i].x + map[i].w && this->x + this->w + this->x_offset > map[i].x + map[i].w)
					{
						this->x -= (this->x + -(map[i].x + map[i].w));
						printf("X COLLISION");
						break;
					}
					else
					{
						//This could be why it gets set equal to 0. Temporary fix. Will have to see how it plays out. 
						if(this->x - (this->x + this->w + this->x_offset - map[i].x) < 100)
						{
							break;
						}
						this->x -=	(this->x + this->w + this->x_offset - map[i].x);
						break;				
					}//
				}
			}
			}
			int renderx, rendery;
			if(this->x  > cam.x)
			{
				renderx = SCREEN_WIDTH/2 + (this->x - cam.x); 
			}
			else
			{
				renderx = SCREEN_WIDTH/2 - (cam.x - this->x);
			}
			if(this->y  > cam.y)
			{
				rendery = SCREEN_HEIGHT/2 + (this->y - cam.y);
			}
			else
			{
				rendery = SCREEN_HEIGHT/2 - (cam.y - this->y);
			}
			this->pos_rect.x = renderx;
			this->pos_rect.y = rendery;
			render_y_ = rendery;
 			render_x_ = renderx;
			this->pos_rect.w = this->w + this->hit_box_x_set;
			this->pos_rect.h = this->h + this->y_offset;
		}
		
		int get_Diff(int x) //Gets difference between target.
		{
			return x - this->x;		
		}
		void assign_weapon_box(int x, int w, int opp_x,int opp_w,int y, int h) //Assigns the weapon hitbox. 
		{
			
			if(facing)
			{
				this->weapon_rect.x = x;		
				this->weapon_rect.w = w;
			}
			else
			{
				this->weapon_rect.x = x;	
				this->weapon_rect.w = opp_w;		
			}
			this->weapon_rect.y = y;
			this->weapon_rect.h = h;
		}
		void handle_render(int x, int y) //Renders to the screen
		{
			SDL_Rect rect;
			rect.x = x;
			rect.y = y;
			rect.w = this->w;
			rect.h = this->h + hit_box_h_bon;
			SDL_RendererFlip flip;
			if(facing)
			{
				flip = SDL_FLIP_NONE;
			//	SDL_RenderCopyEx(mainren,this->model,NULL,&rect, 0,NULL,flip);
			}
			else
			{
				flip = SDL_FLIP_HORIZONTAL;
				//SDL_RenderCopyEx(mainren,this->model,NULL,&rect, 0,NULL,flip);
			}
			this->model_state.render_piece(rect,flip);
		}
		virtual void handle_death()//Animates the death and changes any values that need to be changed.
		{
			return;
		}
	
	
};

class kled:public character //The main character
{
	 
	public: 
	bool mounted = true;
	bool get_off = false;
	int hit_area = 60;
	int pnts = 0;
	int gun_timer = 0;
	int gun_cooldown = 0;
	int ammo = 2;
	int reload_timer = 0;
	int explode_timer = 0;
	double jump_power = 0;
	SDL_Rect Explode_Rect = {0,0,50,20};
	int explode_x = 0;
	int explode_y = 0;
	SDL_RendererFlip flip_gun = SDL_FLIP_NONE;
	bool set_reload = false;;
	bool firing = false;
	int finish_gun = 0;
	int start_dash_x = 0;
	int dash_travel = 0;
	int scores = 0;
	int dashes = 0;
	bool indash = false;
	bool dashing = false;
	kled(int x, int y)
	{
		this->hp = 4;
		this->m_speed = 7;
		this->b_dam = 1;
		this->name = "Kled";
		this->x = x;
		this->y = y;
		this->w = 80;
		this->h = 70;
		this->x_offset = 1;
		this->y_offset = 1;
		this->hit_box_x_set = 0;
		this->multi_model = true;
		this->set_render();
	}
		void reset_sprites() //Resets the sprites. Clears what is active. 
		{
			for(int i = 0; i < model_state.aset.size(); i++)
			{
				model_state.aset[i].render = false;
				model_state.aset[i].change_sprites = false;
				model_state.aset[i].frame = 0;
			}	
		}
		
		void take_damage(int d,double x_p,double y_p, double targx, double targy) //A standardized way of applying damage. 
		{
			if(blocking)
			{
				return;
			}
			else
			{
				set_knockback(x_p,y_p,targx,targy);
				hp -= d;
				last_hit = SDL_GetTicks();
			}
		}
		
		void start_dash() //Begins dashing.
		{
			if(dashing)
			{
				return;	
			}
			else
			{
				start_dash_x = SDL_GetTicks();
				blocking = true;
				dashing = true;
				reset_sprites();
				this->model_state.aset[9].render = true;
				this->model_state.aset[SKAARL_DASH].render = true;
				this->model_state.aset[SKAARL_DASH].change_sprites = true;
				this->model_state.aset[KLED_MOUNTED].y_offset = 10;
				dashes = 3;
			}
		}
		
		void dash() //Handles how dashing works, moves the player. Acts according to how many dashes are active. 
		{
			if(!indash)
			{
			
				reset_sprites();
				this->model_state.aset[9].render = true;
				this->model_state.aset[SKAARL_DASH].render = true;
				this->model_state.aset[SKAARL_DASH].change_sprites = true;
				this->model_state.aset[KLED_MOUNTED].y_offset = 10;
				indash = true;
				blocking = true;
			}
			if(facing)
			{
				x += 0.4 * ticks_per_frame;
				dash_travel += 0.4 * ticks_per_frame;	
			}
			else
			{
				x -= 0.4 * ticks_per_frame;
				dash_travel -= 0.4 * ticks_per_frame;	
			}

			if(dash_travel > 200 || dash_travel < -200)
			{
				dash_travel = 0;
				indash = false;
				blocking = false;
				this->model_state.aset[KLED_MOUNTED].y_offset = 0;
				reset_sprites();
			}
		}
		
		void dismount() //Dismounts the character. 
		{
			reset_sprites();
			model_state.aset[KLED_IDLE].render = true;
			mounted = false;
			hp = 2;
			x_offset = 0;
			hit_area = 20;
			hit_box_x_set = -30;
		}
		void mount() //Puts the character onto their mount and resets hitboxes.
		{
			mounted = true;
			hp = 4;
			x_offset = 1;
			hit_area = 60;
			hit_box_x_set = 0;
			immobile = false;
			reset_sprites();
			model_state.aset[SKAARL_IDLE].render = true;
			model_state.aset[KLED_MOUNTED].render = true;
			printf("HECK");
		}
		
		void duck() //Makes the character duck
		{
			if(!mounted && !ducking)
			{
				this->ducking = true;
				immobile = true;
				reset_sprites();
				this->model_state.aset[KLED_DUCK].render = true;
				this->model_state.aset[KLED_DUCK].change_sprites = true;
				this->model_state.aset[KLED_DUCK].frame = 0;
				model_state.aset[KLED_DUCK].timer = SDL_GetTicks();
			}
		}
		
		void handle_walk_model() //Handles the walk animations
		{
			if(mounted)
			{
				if(this->model_state.aset[5].render ||  this->model_state.aset[7].render || this->model_state.aset[1].render || this->model_state.aset[SKAARL_ATTACK].render ||  air || rising && !model_state.aset[SKAARL_JUMP].render || attacking)
				{
					return;
				}
				reset_sprites(); 
				this->model_state.aset[1].render = true;
				this->model_state.aset[1].frame = 0;
				this->model_state.aset[1].change_sprites = true;
				this->model_state.aset[10].render = true;
				this->model_state.aset[10].frame = 0;
				this->model_state.aset[10].change_sprites = true;
				return;
			}
			else
			{
				if(this->model_state.aset[5].render || this->model_state.aset[2].render || this->model_state.aset[KLED_WALK].render || air || rising && !model_state.aset[KLED_JUMP].render || attacking)
				{
					return;
				}
				reset_sprites();
				this->model_state.aset[KLED_WALK].render = true;
				this->model_state.aset[KLED_WALK].frame = 0;
				this->model_state.aset[KLED_WALK].change_sprites = true; 
			}
		}
		
		void fire() //Fires the gun and knocks the player back.
		{
			ammo--;
			firing = false;
			immobile = false;
			gun_timer = SDL_GetTicks();
			model_state.aset[KLED_GUN].change_sprites = true;
			explode_timer = SDL_GetTicks();
			int x_pow = -1;
			double y_pow = -.3;
			if(!facing)
			{
				x_pow = -x_pow;
			}
			set_knockback(x_pow,y_pow,80,20);
			double shot_y = .5;
			int bull_shots = rand()%15 + 3;
			if(facing)
			{
				for(bull_shots; bull_shots > 0; bull_shots--)
				{
					double x_v = rand()%99 + 30;
					x_v = x_v/100;
					if(rand()%2 + 1 == 1)
					{
						double y_v = rand()%99 + 1;
						y_v = y_v/1000;
						P_List.push_back(projectile(x + x_offset, y + h/2,x_v,10,10,textures[45],SDL_FLIP_NONE,1,y_v,false));	
					}
					else
					{
						double y_v = rand()%99 + 1;
						y_v = -y_v/1000;
						P_List.push_back(projectile(x + x_offset, y + h/2,x_v,10,10,textures[45],SDL_FLIP_NONE,1,y_v,false));	
					}
				}
				explode_x = x + w;
				explode_y = y + h/2;
				flip_gun = SDL_FLIP_NONE;
			}
			else if(!facing)
			{
				for(bull_shots; bull_shots > 0; bull_shots--)
				{
					double x_v = rand()%99 + 30;
					x_v = x_v/100;
					if(rand()%2 + 1 == 1)
					{
						double y_v = rand()%99 + 1;
						y_v = y_v/1000;
						P_List.push_back(projectile(x + x_offset, y + h/2,-x_v,10,10,textures[45],SDL_FLIP_HORIZONTAL,1,y_v,false));	
					}
					else
					{
						double y_v = rand()%99 + 1;
						y_v = -y_v/1000;
						P_List.push_back(projectile(x + x_offset, y + h/2,-x_v,10,10,textures[45],SDL_FLIP_HORIZONTAL,1,y_v,false));	
					}
				}
				explode_x = x;
				explode_y = y + h/2;
				flip_gun = SDL_FLIP_HORIZONTAL;					
			}
		}
		
		void handle_character(const Uint8 *key,std::vector<terrain_piece> map) //Where the main handling of the character takes place. Takes input and makes them work in the game world.
		{
			if(x < 0)
			{
				x = 0;
			}
			if(dashing && SDL_GetTicks() > 3000 + start_dash_x)
			{
				dashes = 0;
				dashing = false;
			}
			if(explode_timer + 250 > SDL_GetTicks())
			{
				int renderx;
				int rendery;
				if(flip_gun == SDL_FLIP_NONE)
				{
					explode_x = x + w;
				}
				else
				{
					explode_x = x - Explode_Rect.w;
				}
				if(explode_x  > cam.x)
				{
					renderx = SCREEN_WIDTH/2 + (explode_x - cam.x); 
				}
				else
				{
					renderx = SCREEN_WIDTH/2 - (cam.x - explode_x );
				}
				if(explode_y > cam.y)
				{
					rendery = SCREEN_HEIGHT/2 + (explode_y  - cam.y);
				}
				else
				{
					rendery = SCREEN_HEIGHT/2 - (cam.y - explode_y);
				}

			//	Explode_Rect.x = renderx;
			//	Explode_Rect.y = rendery;
			//	SDL_RenderCopyEx(mainren, textures[48],NULL,&Explode_Rect, 0,NULL, flip_gun);	
			}
			if(hp < 1 && !mounted)
			{
				return;
			}
		//	if(pnts > 19 && !mounted)
		//	{
				//mount();
		//	}
			int targ = 4;
			if(!mounted)
			{
				targ = 2;
			}
			for(int i = 0, x_offset = 200; i < targ; i++, x_offset -= 45)
			{
				SDL_Rect pos = {SCREEN_WIDTH - x_offset, 20, 40, 40};
				if(i < hp)
				{
					SDL_RenderCopyEx(mainren, textures[51],NULL,&pos, 0,NULL, SDL_FLIP_NONE);
				}
				else
				{
					SDL_RenderCopyEx(mainren, textures[52],NULL,&pos, 0,NULL, SDL_FLIP_NONE);		
				}
	
			}
			if(hp < 1 && mounted)
			{
				pnts = 0;
				get_off = true;
				dismount();
			}
			if(this->air && !rising && knockback_y_pw == 0)
			{
					this->y += .4 * ticks_per_frame;
					set_render();
					if(model_state.aset[SKAARL_JUMP].frame == 0 && mounted && !attacking)
					{
						reset_sprites();
						model_state.aset[KLED_MOUNTED].render = true;
						model_state.aset[SKAARL_JUMP].render = true;
						model_state.aset[SKAARL_JUMP].frame = 3;
					}
					else if(model_state.aset[KLED_JUMP].frame == 0 && !mounted && !attacking)
					{
						reset_sprites();
					//	model_state.aset[KLED_MOUNTED].render = true;
						model_state.aset[KLED_JUMP].render = true;
						model_state.aset[KLED_JUMP].frame = 3;
					}
					if(model_state.aset[SKAARL_JUMP].frame == 3 && mounted && !attacking)
					{
						model_state.aset[SKAARL_JUMP].change_sprites = false;
					}
					else if(model_state.aset[KLED_JUMP].frame == 3 && !mounted && !attacking)
					{
						model_state.aset[KLED_JUMP].change_sprites = false;
					}
			}
			if(this->rising)
			{
				
				this->y -= .4 * ticks_per_frame;
				jump_power += .4 * ticks_per_frame;
				set_render();
				if(mounted)
				{
					if(!attacking)
					{
						if(model_state.aset[SKAARL_JUMP].frame == 1)
						{
							model_state.aset[SKAARL_JUMP].change_sprites = false;
						}
						else
						{
							model_state.aset[SKAARL_JUMP].change_sprites = true;
						}
					}
				}
				else
				{
					if(!attacking)
					{
					
						if(model_state.aset[KLED_JUMP].frame == 1)
						{
							model_state.aset[KLED_JUMP].change_sprites = false;
						}
						else
						{
							model_state.aset[KLED_JUMP].change_sprites = true;
						}
					}
				}
				if(jump_power > 125)
				{
					rising = false;
					jump_power = 0;
					if(mounted)
					{
						model_state.aset[SKAARL_JUMP].change_sprites = true;
					}
					else
					{
						model_state.aset[KLED_JUMP].change_sprites = true;
					}
				}
			}
			if(indash)
			{
				dash();
			}
			if(knockback_x_pw != 0 && knockback_y_pw != 0)
			{
				knock_back(map);
			}
			for(int i = 0;i <= map.size();i++)
			{
				if(map[i].w == 0 || (map[i].x + map[i].w) < cam.x - SCREEN_WIDTH/2 || map[i].x >= cam.x + SCREEN_WIDTH/2 || map[i].y < cam.y - SCREEN_HEIGHT/2 || map[i].y > cam.y + SCREEN_HEIGHT)
				{
					continue;
				}
				if(this->x + this->w + this->x_offset > map[i].x && this->x < map[i].x + map[i].w && this->y + this->h + this->y_offset >= map[i].y && this->y <= map[i].y + map[i].h)
				{
					
					if(this->y + this->h + this->y_offset > map[i].y+ map[i].h)
					{
						this->y += (this->y + /*this->h*/ + this->y_offset - (map[i].y + map[i].h)) + 2;
						this->air = false;
						this->rising = false;
						break;
					}
					this->y -= (this->y + this->h + this->y_offset - map[i].y);
					this->air = false;
					break;
				}
				else
				{
					this->air = true;
				}
			}
			if(model_state.aset[KLED_GUN].frame == 4)
			{
				reset_sprites();
			}
			if(firing)
			{
				if(model_state.aset[KLED_GUN].frame == 2)
				{
					model_state.aset[KLED_GUN].change_sprites = false;
					finish_gun = SDL_GetTicks() + 200;
				}
			}
			if(key[SDL_SCANCODE_Q] && gun_timer + 500 < SDL_GetTicks() && !mounted && ammo > 0)
			{
				immobile = true;
				gun_timer = SDL_GetTicks();
				reset_sprites();
				model_state.aset[KLED_GUN].render = true;
				model_state.aset[KLED_GUN].change_sprites = true;
				firing = true;
			}
			if(ammo < 2)
			{
				if(set_reload)
				{
					reload_timer = SDL_GetTicks();
					set_reload = false;
				}
				if(reload_timer + 5000 < SDL_GetTicks())
				{
					ammo++;
					reload_timer = SDL_GetTicks();
				}
			}
			if(gun_timer + 250 < SDL_GetTicks() && firing)
			{
				fire();
			}
			if(ammo != 0)
			{
				for(int i = 0; i < ammo; i++)
				{
					renSpec(mainren,textures[45], 15,15, 20 * i + 50, 50);
				}
			}
			if(key[SDL_SCANCODE_E] && !attacking)
			{
				
				if(mounted && !dashing)
				{
					start_dash();
				}
				else if(mounted && !indash && dashes > 0)
				{
					dashes -= 1;
					dash();
				}
				
			}
			if(key[SDL_SCANCODE_S])
			{
				this->duck();
			}
			else
			{
				if(!mounted && ducking)
				{	
					ducking = false;
					immobile = false;
					model_state.aset[KLED_DUCK].render = false;
					model_state.aset[KLED_DUCK].frame = 0;
				}
			}
			if(ducking && model_state.aset[KLED_DUCK].frame == 1)
			{
				model_state.aset[KLED_DUCK].change_sprites = false;
			}
			if(attacking)
			{
				this->handle_attack();
			}
			if(this->can_move())
			{
				bool A = false;
				bool D = false;
				if(key[SDL_SCANCODE_D] && !indash)
				{
					if(x + w > cam.x + SCREEN_WIDTH/2)
					{
						
					}
					else
					{
						this->move(.35,map);
					}
					D = true;
					//bg.move_points += 1;
				}
				if(key[SDL_SCANCODE_A] && !indash)
				{
					if(x < cam.x - SCREEN_WIDTH/2)
					{
						
					}
					else
					{
						this->move(-.35,map);
					}
				//	facing = false;
					//bg.move_points -= 1;
					A = true;
				}
				if(A && D && facing)
				{
					facing = true;
				}
				else if(A && D && !facing)
				{
					facing = false;
				}
				else
				{
					if(A)
					{
						facing = false;	
						this->model_state.aset[10].x_offset = 10;
					}	
					if(D)
					{
						facing = true;
						this->model_state.aset[10].x_offset = -10;
					}
				}				
				if(key[SDL_SCANCODE_W] && !indash)
				{
					this->jump();
					if(!attacking)
					{
					
						reset_sprites();
						if(!model_state.aset[SKAARL_JUMP].render && mounted)
						{
							model_state.aset[SKAARL_JUMP].render = true;
							model_state.aset[KLED_MOUNTED].render = true;
							model_state.aset[SKAARL_JUMP].frame = 0;
						}
						else if(!model_state.aset[KLED_JUMP].render && !mounted)
						{
							model_state.aset[KLED_JUMP].render = true;
							model_state.aset[KLED_JUMP].frame = 0;
						}
					}
				}
				if(key[SDL_SCANCODE_Y])
				{
					this->y = 0;
				}
				if(key[SDL_SCANCODE_X])
				{
					this->x += 100;
				}
				else
				{
					this->ducking = false;
				}
				if(key[SDL_SCANCODE_SPACE] && !attacking && !indash)
				{
					this->start_attack();
				}
			}

			if(!made_move && !model_state.aset[KLED_ATTACK].render && !ducking && !model_state.aset[KLED_GUN].render && !rising && !air && !firing && !model_state.aset[SKAARL_ATTACK].render && !model_state.aset[SKAARL_DASH].render)
			{
				if(mounted)
				{
				
					reset_sprites();
					model_state.aset[0].render = true;
					model_state.aset[KLED_MOUNTED].render = true;
				}
				else
				{
					reset_sprites();
					model_state.aset[4].frame = 0;
					model_state.aset[3].render = true; // hacked by sam kerr xddddddd ;3
				}
			}
			made_move = false;
		}
	void start_attack() //Prepares the player to begin handling their attack. 
	{
		this->attacking = true;
		reset_sprites();
		if(!mounted)
		{
			model_state.aset[KLED_ATTACK].render = true;
			model_state.aset[KLED_WALK].frame = 0;
			model_state.aset[KLED_ATTACK].change_sprites = true;
			model_state.aset[KLED_ATTACK].frame = 0;
		}
		else if(mounted)
		{
			model_state.aset[SKAARL_ATTACK].render = true;
			model_state.aset[SKAARL_LEGS].render = true;
			model_state.aset[SKAARL_ATTACK].frame = 0;
			model_state.aset[SKAARL_LEGS].frame = 0;
			model_state.aset[SKAARL_ATTACK].change_sprites = true;
			model_state.aset[SKAARL_LEGS].change_sprites = true;	
		}
		//this->model_state.aset[WEAPON].frame = 1;
	}
	void knock_back(std::vector <terrain_piece> map) //Was used for old background scrolling. 
	{
		character::knock_back(map);
		//bg.move_points += knockback_x_pw;
	}
	void handle_attack() //Changes the weapon hit boxes and handles animations. 
	{
		if(ducking)
		{
			attacking = false;
			return;	
		}
		if(facing)
		{
			weapon_rect.x = pos_rect.x - 40 + w;
			weapon_rect.w = 10;
			weapon_rect.y = pos_rect.y + h/2;
			weapon_rect.h = 50;
		}
		else
		{
			weapon_rect.x = pos_rect.x + 10;
			weapon_rect.w = 30;
			weapon_rect.y = pos_rect.y + h/2;
			weapon_rect.h = 50;
		}
		if(!model_state.aset[KLED_ATTACK].render && !mounted || model_state.aset[KLED_ATTACK].frame == 6 && !mounted)
		{
			attacking = false;
			model_state.aset[KLED_ATTACK].render = false;
			if(model_state.aset[KLED_GUN].render)
			{
				return;
			}
			reset_sprites();
		}
		if(!model_state.aset[SKAARL_ATTACK].render && mounted || model_state.aset[SKAARL_ATTACK].frame == 10 && mounted)
		{
			attacking = false;
			model_state.aset[SKAARL_ATTACK].render = false;
			reset_sprites();
		}
		printf("Keg");
		return;
	}
	
	void create_tex() //Prepares the player textures and some offsets. Allows for animations.
	{
		this->port = loadTexture("resources/portrait/fighter_portrait.png");
		this->model = loadTexture("resources/models/kled.png");
		this->model_state.aset.push_back(animation_set(50,1));
		this->model_state.aset[0].textures.resize(1); //SKAARL IDLE STATE
		this->model_state.aset[0].textures[0] = textures[37]; //UPDATE WITH IDLE ANIMATION.
		this->model_state.aset[0].render = true; 	
		this->model_state.aset.push_back(animation_set(100,7)); // SKAARL WALKING ANIMATION CYCLE
		this->model_state.aset[1].textures.resize(8);
		this->model_state.aset[1].textures[0] = textures[38];
		this->model_state.aset[1].textures[1] = textures[40];
		this->model_state.aset[1].textures[2] = textures[41];
		this->model_state.aset[1].textures[3] = textures[39];
		this->model_state.aset[1].textures[4] = textures[53];
		this->model_state.aset[1].textures[5] = textures[54];
		this->model_state.aset[1].textures[6] = textures[55];
		this->model_state.aset[1].textures[7] = textures[56];
		this->model_state.aset[1].length_ov = 6;
		this->model_state.aset.push_back(animation_set(60,5)); //JUMPING ANIMATIONS
		this->model_state.aset[2].textures.resize(5);
		this->model_state.aset[2].textures[0] = textures[87]; //INITIAL
		this->model_state.aset[2].textures[1] = textures[88]; // RISE
		this->model_state.aset[2].textures[2] = textures[89]; // PEAK
		this->model_state.aset[2].textures[3] = textures[90]; //FALL
		this->model_state.aset[2].textures[4] = textures[91]; //LAND
		
		this->model_state.aset.push_back(animation_set(50,1)); //KLED UNMOUNTED
		this->model_state.aset[3].textures.resize(1);
		this->model_state.aset[3].textures[0] = textures[58];
		
		this->model_state.aset.push_back(animation_set(125,7)); //KLED RUN CYCLE
		this->model_state.aset[4].textures.resize(9);
		this->model_state.aset[4].textures[0] = textures[59];
		this->model_state.aset[4].textures[1] = textures[60];
		this->model_state.aset[4].textures[2] = textures[61];
		this->model_state.aset[4].textures[3] = textures[62];
		this->model_state.aset[4].textures[4] = textures[63];
		this->model_state.aset[4].textures[5] = textures[64];
		this->model_state.aset[4].textures[6] = textures[65];
		this->model_state.aset[4].textures[7] = textures[66];
		
		this->model_state.aset.push_back(animation_set(75,7)); //Kled Attack Cycle
		this->model_state.aset[5].textures.resize(8);
		this->model_state.aset[5].textures[0] = textures[67];
		this->model_state.aset[5].textures[1] = textures[68];
		this->model_state.aset[5].textures[2] = textures[69];
		this->model_state.aset[5].textures[3] = textures[70];
		this->model_state.aset[5].textures[4] = textures[71];
		this->model_state.aset[5].textures[5] = textures[72];
		this->model_state.aset[5].textures[6] = textures[73];
		this->model_state.aset[5].textures[7] = textures[74];
		model_state.aset[KLED_ATTACK].length_ov = 7;
		
		this->model_state.aset.push_back(animation_set(75,2)); //DUCK CYCLE
		this->model_state.aset[6].textures.resize(2);
		this->model_state.aset[6].textures[0] = textures[75];
		this->model_state.aset[6].textures[1] = textures[76];
		
		this->model_state.aset.push_back(animation_set(30,5)); //KLED JUMP CYCLE
		this->model_state.aset[7].textures.resize(5);
		this->model_state.aset[7].textures[0] = textures[82];
		this->model_state.aset[7].textures[1] = textures[83];
		this->model_state.aset[7].textures[2] = textures[84];
		this->model_state.aset[7].textures[3] = textures[85];
		this->model_state.aset[7].textures[4] = textures[86];
		
		this->model_state.aset.push_back(animation_set(70,5));
		
		this->model_state.aset[8].textures.resize(6); //KLED GUN CYCLE

		this->model_state.aset[8].textures[0] = textures[77];
		this->model_state.aset[8].textures[1] = textures[78];
		this->model_state.aset[8].textures[2] = textures[79];
		this->model_state.aset[8].textures[3] = textures[80];
		this->model_state.aset[8].textures[4] = textures[81];
		this->model_state.aset[8].length_ov = 4;
		
		this->model_state.aset.push_back(animation_set(30,1)); //MOUNTED KLED
		this->model_state.aset[9].textures.resize(1);
		this->model_state.aset[9].textures[0] = textures[57];
		this->model_state.aset[9].render = true; 
		
		this->model_state.aset.push_back(animation_set(100, 7)); //KLED MOUNTED WALK
		this->model_state.aset[10].textures.resize(8);
		this->model_state.aset[10].textures[0] = textures[92];
		this->model_state.aset[10].textures[1] = textures[93];
		this->model_state.aset[10].textures[2] = textures[94];
		this->model_state.aset[10].textures[3] = textures[95];
		this->model_state.aset[10].textures[4] = textures[96];
		this->model_state.aset[10].textures[5] = textures[97];
		this->model_state.aset[10].textures[6] = textures[98];
		this->model_state.aset[10].textures[7] = textures[99];
		this->model_state.aset[10].y_offset = -10;
		this->model_state.aset[10].x_offset = -10;
		
		this->model_state.aset.push_back(animation_set(55,7)); //SKAARL LEGS
		this->model_state.aset[11].textures.resize(8);
		this->model_state.aset[11].textures[0] = textures[100];
		this->model_state.aset[11].textures[1] = textures[101];
		this->model_state.aset[11].textures[2] = textures[102];
		this->model_state.aset[11].textures[3] = textures[103];
		this->model_state.aset[11].textures[4] = textures[104];
		this->model_state.aset[11].textures[5] = textures[105];
		this->model_state.aset[11].textures[6] = textures[106];
		this->model_state.aset[11].textures[7] = textures[107];
		this->model_state.aset[11].length_ov = 7;
		
		this->model_state.aset.push_back(animation_set(55, 10)); //KLED ATTACK
		this->model_state.aset[12].textures.resize(11);
		this->model_state.aset[12].textures[0] = textures[108];
		this->model_state.aset[12].textures[1] = textures[109];
		this->model_state.aset[12].textures[2] = textures[110];
		this->model_state.aset[12].textures[3] = textures[111];
		this->model_state.aset[12].textures[4] = textures[112];
		this->model_state.aset[12].textures[5] = textures[113];
		this->model_state.aset[12].textures[6] = textures[114];
		this->model_state.aset[12].textures[7] = textures[115];
		this->model_state.aset[12].textures[8] = textures[116];
		this->model_state.aset[12].textures[9] = textures[117];
		this->model_state.aset[12].textures[10] = textures[118]; 
		
		this->model_state.aset.push_back(animation_set(50, 7));
		this->model_state.aset[13].textures.resize(8);
		this->model_state.aset[13].textures[0] = textures[136];
		this->model_state.aset[13].textures[1] = textures[137];
		this->model_state.aset[13].textures[2] = textures[138];
		this->model_state.aset[13].textures[3] = textures[139];
		this->model_state.aset[13].textures[4] = textures[140];
		this->model_state.aset[13].textures[5] = textures[141];
		this->model_state.aset[13].textures[6] = textures[142];
		this->model_state.aset[13].textures[7] = textures[143];
		this->model_state.aset[13].length_ov = 7;
		this->model_state.aset[13].change_sprites = true;

		this->model_state.aset[12].y_offset = -5;
		this->model_state.aset[12].x_offset = 5;
		this->model_state.aset[12].length_ov = 10;
		set_render();
		
	}
};
kled mainchar(0,0);
class simple_ai:public character //The AI class. Was meant to use polymorphism, too late, so now uses ID's to identify type. 
{

	public:
		int AI_ID = 0;
		bool irregular_move = false;
		int last_shot = 0;
		int tims = 1000;
		int arch_marker = 167; //Reference where the marker is. Very cheaty. 
		int arch_marker_timer = 0;
		bool fire = false;
		simple_ai(int x, int y, int ID) //The creation of the AI. Sets variables according to ID. 
		{
			this->AI_ID = ID;
			this->hp = 2;
			this->m_speed = 8;
			this->b_dam = 1;
			this->w = 50;
			this->h = 60;
			this->name = "Soldier";
			if(AI_ID == AI_SIMPLE)
			{
				this->x_offset = 0;
				this->y_offset = 0;
			}
			if(AI_ID == AI_ARCHER)
			{
				this->x_offset = 0;
				this->y_offset = 0;
			}
			if(AI_ID == AI_BOMBER)
			{
				hp = 1;
			}
			if(AI_ID == AI_ROGUE)
			{
				this->x_offset = 0;
				this->y_offset = 0;
				irregular_move = true;
				w = 40;
				h = 55;
				hp = 10000;
			}

			this->multi_model = false;
			this->x = x;
			this->y = y;
			create_tex();
		//	this->spawnrefv = spawnref;
		}
		void handle_walk_model() //Changes what animation is being played.
		{
			this->model_state.target = 0;
			
		}
		
		void handle_attack() //Handles the attacks. Takes into account their types, so an archer will fire, a Warrior will swing.
		{
			if(AI_ID == AI_SIMPLE)
			{
			
				this->model_state.target = 2;
				this->assign_weapon_box(pos_rect.x,50, pos_rect.x - 15,20,pos_rect.y,40);
			}
			if(AI_ID == AI_ARCHER)
			{
				double shot_vel = .49;
				SDL_RendererFlip f = SDL_FLIP_NONE;
				if(!facing)
				{
					shot_vel = -shot_vel;
					f = SDL_FLIP_HORIZONTAL;
				}
				this->model_state.target = 1;
				if(tims + last_shot < SDL_GetTicks() && this->model_state.aset[1].frame == 3)
				{
					P_List.push_back(projectile(this->x,this->y + 20,shot_vel,33,6,textures[30],f,3,0,true));
					last_shot = SDL_GetTicks();
				}
			}
			if(AI_ID == AI_BOMBER)
			{
					if(2500 + last_shot < SDL_GetTicks() && this->model_state.aset[1].frame == 10)
					{
						fire = true;
						last_shot = SDL_GetTicks();
					}
					this->model_state.target = 1;
					if(this->model_state.aset[1].frame == 1 || this->model_state.aset[1].frame == 2 || this->model_state.aset[1].frame == 8 || this->model_state.aset[1].frame == 12)
					{
						this->model_state.aset[1].sprite_timer = 150;
					}
					else if(this->model_state.aset[1].frame == 3 || this->model_state.aset[1].frame == 11)
					{
						this->model_state.aset[1].sprite_timer = 200;
					}
					else if(this->model_state.aset[1].frame == 4 || this->model_state.aset[1].frame == 7 || this->model_state.aset[1].frame == 13)
					{
						this->model_state.aset[1].sprite_timer = 100;
					}
					else if(this->model_state.aset[1].frame == 5)
					{
						this->model_state.aset[1].sprite_timer = 125;
					}
					else if(this->model_state.aset[1].frame == 6)
					{
						this->model_state.aset[1].sprite_timer = 70;
					}
					else if(this->model_state.aset[1].frame == 9 || this->model_state.aset[1].frame == 10)
					{
						this->model_state.aset[1].sprite_timer = 50;
					}
					else
					{
						this->model_state.aset[1].sprite_timer = 400;
					}
			}
			set_render();
		}
		
		void handle_spec_actions(int diff, std::vector<terrain_piece> map) //Handles how the AI moves and whether or not they should attack or not depending on where the player is. 
		{ //ONLY USE FOR INTENTIONAL MOVEMENT! OUTSIDE MOVEMENT MUST BE HANDLED ABOVE!!!!!!!!!!!!!!!!!!!!!!
			double speed = .1;
			if(diff > 0)
			{
				facing = true;
			}
			else
			{
				facing = false;
				speed = -speed;
			}
			if(AI_ID == AI_SIMPLE)
			{
				if(diff <  mainchar.hit_area - 16 && diff > 0 || diff > -mainchar.hit_area + 12&& diff < 0) // hacked by jack gerrard 
				{
					this->handle_attack();
					this->attacking = true;
					return;
				}
				if(!attacking)
				{
					model_state.aset[2].frame = 0;
				}
			}
			else if(AI_ID == AI_ROGUE)
			{
				
			}
			else if(AI_ID == AI_ARCHER)
			{
				if(diff < 330 && diff > 0 || diff > -330 && diff < 0)
				{
					this->handle_attack();
					this->attacking = true;
					return;
				}
			}
			else if(AI_ID == AI_BOMBER)
			{
				if(diff < 260 && diff > 0 || diff > -260 && diff < 0)
				{
					this->handle_attack();
					this->attacking = true;
					return;
				}
			}
			
			this->attacking = false;
			move(speed, map);
		}
		
		void handle_death() //Plays the death animation and makes sure the game knows they have died. 
		{

			if(AI_ID == AI_SIMPLE)
			{
				if(this->alive)
				{
					this->alive = false;
					mainchar.pnts++;
					mainchar.scores++;	
				}
				this->model_state.target = 3;
				this->model_state.aset[3].change_sprites = true;	
				if(this->model_state.aset[3].frame == 3)
				{
					this->h = 50;
				}
				if(this->model_state.aset[3].frame == 4)
				{
					this->h = 45;
				}
				if(this->model_state.aset[3].frame == 5)
				{
					this->h = 35;
				}
				if(this->model_state.aset[3].frame == 6)
				{
					this->h = 30;
				}
				if(this->model_state.aset[3].frame == 7)
				{	

					this->model_state.aset[3].change_sprites = false;
					this->h = 35;
				}
			}
			if(AI_ID == AI_ARCHER)
			{
				if(this->alive)
				{
					this->alive = false;
					this->model_state.target = 2;
					this->model_state.aset[2].change_sprites = true;
					
					mainchar.pnts++;
					mainchar.scores++;
				}
				if(model_state.aset[2].frame == 3)
				{
					h = 30;
				}
				if(model_state.aset[2].frame == 5)
				{
					h = 20;
				}
				if(model_state.aset[2].frame == 6)				
				{
					h = 20;
				}
					if(this->model_state.aset[2].frame == 7)
				{
					this->model_state.aset[2].change_sprites = false;	
				}
			}
			if(AI_ID == AI_BOMBER)
			{
				if(this->alive)
				{
					this->alive = false;
					this->model_state.target = 2;
					this->model_state.aset[2].change_sprites = true;	
					mainchar.pnts++;
					mainchar.scores++;
				}
				if(this->model_state.aset[2].frame == 13)
				{
					this->model_state.aset[2].change_sprites = false;	
				}
				if(this->model_state.aset[2].frame < 4 || this->model_state.aset[2].frame == 6 || this->model_state.aset[2].frame == 6)
				{
					this->model_state.aset[2].sprite_timer = 75;
				}
				if(this->model_state.aset[2].frame == 4 || this->model_state.aset[2].frame == 9)
				{
					this->model_state.aset[2].sprite_timer = 125;
				}
				if(this->model_state.aset[2].frame == 5)
				{
					this->model_state.aset[2].sprite_timer = 500;
				}
				if(this->model_state.aset[2].frame == 6 || this->model_state.aset[2].frame == 8)
				{
					this->model_state.aset[2].sprite_timer = 75;
				}
				if(this->model_state.aset[2].frame == 7)
				{
					this->model_state.aset[2].sprite_timer = 50;
				}
				if(this->model_state.aset[2].frame == 10)
				{
					this->model_state.aset[2].sprite_timer = 110;
				}
				if(this->model_state.aset[2].frame > 10)
				{
					this->model_state.aset[2].sprite_timer = 100;
				}
			}
		}
		
	virtual void handle_ai(std::vector<terrain_piece> map, int x, int y) //Similar to handle_character. Makes them collide with terrain and interact with the world. 
		{
			if(this->air && !rising && !irregular_move)
			{
					this->y += .5 * ticks_per_frame;
			}
			if(this->rising && !irregular_move)
			{
				if(this->y_timer + this->air_time > SDL_GetTicks())
				{
					if(this->last_rise + 4 < SDL_GetTicks())
					{
						this->y -= 3;
						last_rise = SDL_GetTicks();
					}
				}
				else
				{
					rising = false;
				}
			}
			for(int i = 0;i <= map.size();i++)
			{
				if(map[i].w == 0 || (map[i].x + map[i].w) < this->x - SCREEN_WIDTH/2 || map[i].x >= this->x + SCREEN_WIDTH/2 || map[i].y < this->y - SCREEN_HEIGHT/2 || map[i].y > this->y + SCREEN_HEIGHT)
				{
					continue;
				}
				if(this->x + this->w + this->x_offset > map[i].x && this->x < map[i].x + map[i].w && this->y + this->h + this->y_offset >= map[i].y && this->y <= map[i].y + map[i].h)
				{
					if(this->y + this->h + this->y_offset > map[i].y+ map[i].h)
					{
						this->y += (this->y + /*this->h*/ + this->y_offset - (map[i].y + map[i].h)) + 2;
						this->air = false;
						this->rising = false;
						break;
					}
					this->y -= (this->y + this->h + this->y_offset - map[i].y);
					this->air = false;
					break;
				}
				else
				{
					this->air = true;
				}	
			}
			if(this->hp < 1 || !this->alive)
			{
				handle_death();
				return;
			}
			if(AI_ID == AI_ARCHER)
			{
				if(this->x < cam.x - SCREEN_WIDTH/2 || this->x > cam.x + SCREEN_WIDTH/2)
				{
					int iw = 64;
					int ih = 64;
					if(arch_marker_timer + 150 < SDL_GetTicks() && arch_marker != 169)
					{
						arch_marker++;
						arch_marker_timer = SDL_GetTicks();
					}
					if(this->x < cam.x)
					{
						 SDL_Rect dst;
						 dst.x = 0;
						 dst.y = 200;
						 dst.w = iw;
						 dst.h = ih;
						 SDL_RenderCopyEx(mainren,textures[arch_marker],NULL,&dst, 0,NULL,SDL_FLIP_HORIZONTAL);
						 dst.w = iw/2;
						 dst.x = dst.x + dst.w - (dst.w/2);
						 dst.y = dst.y + (ih/2);
						 dst.h = ih/4;
						 SDL_RenderCopyEx(mainren,textures[30],NULL,&dst, 0,NULL,SDL_FLIP_NONE);
					}
					else
					{
						 SDL_Rect dst;
						 dst.x = SCREEN_WIDTH - iw;
						 dst.y = 200;
						 dst.w = iw;
						 dst.h = ih;
						 SDL_RenderCopyEx(mainren,textures[arch_marker],NULL,&dst, 0,NULL,SDL_FLIP_NONE);
						 dst.w = iw/2;
						 dst.x = dst.x + dst.w ;
						 dst.y = dst.y + (ih/2);
						 dst.h = ih/4;
						 SDL_RenderCopyEx(mainren,textures[30],NULL,&dst, 0,NULL,SDL_FLIP_NONE);	 
					}
				}
			}
			set_render();
			if(attacking)
			{
				this->handle_attack(); //THIS PREVENTS FURTHER MOVEMENT, MIGHT CHANGE LATER, COMMENT HERE IF IT BECOMES AN ISSUE
			}
			if(knockback_x_pw != 0 && knockback_y_pw != 0)
			{
				knock_back(map);
			}
			if(this->can_move())
			{
				int difference = this->get_Diff(x);
				handle_spec_actions(difference,map);
			}
		}
		void create_tex() //Creates the texture lists according to the AI type. 
		{
			if(AI_ID == AI_ARCHER)
			{
				this->model = textures[25];
				this->model_state.aset.push_back(animation_set(200,1));
				this->model_state.aset[0].textures.resize(1);
				this->model_state.aset[0].textures[0] = textures[25];
				this->model_state.aset[0].length_ov = 0;
				this->model_state.aset.push_back(animation_set(200,1));
				this->model_state.aset[1].textures.resize(6);
				this->model_state.aset[1].textures[0] = textures[26];
				this->model_state.aset[1].textures[1] = textures[27];
				this->model_state.aset[1].textures[2] = textures[28];
				this->model_state.aset[1].textures[3] = textures[29];
				this->model_state.aset[1].textures[4] = textures[128];
				this->model_state.aset[1].textures[5] = textures[129];
				this->model_state.aset[1].length_ov = 5;
				this->model_state.aset.push_back(animation_set(75,1));
				this->model_state.aset[2].textures.resize(8);
				
				this->model_state.aset[2].textures[0] = textures[49];
				this->model_state.aset[2].textures[1] = textures[50];
				this->model_state.aset[2].textures[2] = textures[130];
				this->model_state.aset[2].textures[3] = textures[131];
				this->model_state.aset[2].textures[4] = textures[132];
				this->model_state.aset[2].textures[5] =	textures[133];
				this->model_state.aset[2].textures[6] =	textures[134];
				this->model_state.aset[2].textures[7] = textures[135];
				this->model_state.aset[2].length_ov = 7;
				this->model_state.length = 1;
				this->model_state.single = true;
				this->model_state.target = 0;
				this->model_state.aset[0].change_sprites = false;
				this->model_state.aset[1].change_sprites = true;

			}
			if(AI_ID == AI_ROGUE)
			{
				this->model = textures[255];
				this->model_state.aset.push_back(animation_set(200,1));
				this->model_state.aset[0].textures.resize(3);
				this->model_state.aset[0].textures[0] = textures[255];
				this->model_state.aset[0].textures[1] = textures[256];
				this->model_state.aset[0].textures[2] = textures[257];
				this->model_state.aset.push_back(animation_set(200,1));
				this->model_state.aset[1].textures.resize(6);
				this->model_state.aset[1].textures[0] = textures[262];
				this->model_state.aset[1].textures[1] = textures[263];
				this->model_state.aset[1].textures[2] = textures[264];
				this->model_state.aset[1].textures[3] = textures[265];
				this->model_state.aset[1].textures[4] = textures[266];
				this->model_state.aset[1].textures[5] = textures[267];
				this->model_state.aset.push_back(animation_set(200,1));
				this->model_state.aset[2].textures.resize(5);
				this->model_state.aset[2].textures[0] = textures[268];
				this->model_state.aset[2].textures[1] = textures[269];
				this->model_state.aset[2].textures[2] = textures[270];
				this->model_state.aset[2].textures[3] = textures[271];
				this->model_state.aset[2].textures[4] = textures[272];
				this->model_state.aset[0].length_ov = 2;
				this->model_state.aset[1].length_ov = 5;
				this->model_state.aset[2].length_ov  = 4;
				this->model_state.length = 2;
				this->model_state.single = true;
				this->model_state.target = 1;
				this->model_state.aset[1].change_sprites = true;
				this->model_state.aset[0].change_sprites = false;
				this->model_state.aset[2].change_sprites = true;
			}
			if(AI_ID == AI_SIMPLE)
			{	
				this->model = textures[14];
				this->model_state.aset.push_back(animation_set(200,6));
				this->model_state.aset[0].textures.resize(7);
		//	this->model_state.aset[0].textures[0] =	textures[10];
				this->model_state.aset[0].textures[0] = textures[11];
				this->model_state.aset[0].textures[1] = textures[12]; //WALKING
				this->model_state.aset[0].textures[2] = textures[13];
				this->model_state.aset[0].textures[3] = textures[14];
				this->model_state.aset[0].textures[4] = textures[119];			
				this->model_state.aset[0].textures[5] = textures[120];
				this->model_state.aset[0].textures[6] = textures[121];		
				//this->model_state.aset[0].length_ov = 6;	
				this->model_state.aset.push_back(animation_set(50,1));
				this->model_state.aset[1].textures.resize(1);	
				this->model_state.aset[1].textures[0] = textures[14];
				this->model_state.aset.push_back(animation_set(55,6));
				this->model_state.aset[2].textures.resize(7);
				this->model_state.aset[2].textures[0] = textures[15]; // ATTACKING
				this->model_state.aset[2].textures[1] = textures[16];
				this->model_state.aset[2].textures[2] = textures[17];
				this->model_state.aset[2].textures[3] = textures[18];
				this->model_state.aset[2].textures[4] = textures[19];
				this->model_state.aset[2].textures[5] = textures[20];
				this->model_state.aset[2].textures[6] = textures[21];
				this->model_state.aset[2].length_ov = 6;
				this->model_state.aset.push_back(animation_set(50,1));
				this->model_state.aset[3].textures.resize(8);
				this->model_state.aset[3].textures[0] = textures[22];
				this->model_state.aset[3].textures[1] = textures[23]; //DYING
				this->model_state.aset[3].textures[2] = textures[122];
				this->model_state.aset[3].textures[3] = textures[123];
				this->model_state.aset[3].textures[4] = textures[124];
				this->model_state.aset[3].textures[5] = textures[125];
				this->model_state.aset[3].textures[6] = textures[126];
				this->model_state.aset[3].textures[7] = textures[127];
				this->model_state.aset[3].length_ov = 7;
				this->model_state.aset[2].length_ov = 6;
				this->model_state.length = 3;
				this->model_state.single = true;
				this->model_state.target = 1;
				this->model_state.aset[0].change_sprites = true;
				this->model_state.aset[2].change_sprites = true;
				this->model_state.aset[3].change_sprites = true;
			}
			if(AI_ID == AI_BOMBER)
			{
				this->model = textures[9];
				this->model_state.aset.push_back(animation_set(100,3));
				this->model_state.aset[0].textures.resize(4);
				this->model_state.aset[0].textures[0] = textures[176];
				this->model_state.aset[0].textures[1] = textures[177];
				this->model_state.aset[0].textures[2] = textures[178];
				this->model_state.aset[0].textures[3] = textures[179];
				this->model_state.aset.push_back(animation_set(100,14));
				this->model_state.aset[1].textures.resize(15);
				this->model_state.aset[1].textures[0] = textures[180];
				this->model_state.aset[1].textures[1] = textures[181];
				this->model_state.aset[1].textures[2] = textures[182];
				this->model_state.aset[1].textures[3] = textures[183];
				this->model_state.aset[1].textures[4] = textures[184];
				this->model_state.aset[1].textures[5] = textures[185];
				this->model_state.aset[1].textures[6] = textures[186];
				this->model_state.aset[1].textures[7] = textures[187];
				this->model_state.aset[1].textures[8] = textures[188];
				this->model_state.aset[1].textures[9] = textures[189];
				this->model_state.aset[1].textures[10] = textures[190];
				this->model_state.aset[1].textures[11] = textures[191];
				this->model_state.aset[1].textures[12] = textures[192];
				this->model_state.aset[1].textures[13] = textures[193];
				this->model_state.aset[1].textures[14] = textures[194];
				this->model_state.aset.push_back(animation_set(100, 13));
				this->model_state.aset[2].textures.resize(14);
				this->model_state.aset[2].textures[0] = textures[195];
				this->model_state.aset[2].textures[1] = textures[196];
				this->model_state.aset[2].textures[2] = textures[197];
				this->model_state.aset[2].textures[3] = textures[198];
				this->model_state.aset[2].textures[4] = textures[199];
				this->model_state.aset[2].textures[5] = textures[200];
				this->model_state.aset[2].textures[6] = textures[201];
				this->model_state.aset[2].textures[7] = textures[202];
				this->model_state.aset[2].textures[8] = textures[203];
				this->model_state.aset[2].textures[9] = textures[204];
				this->model_state.aset[2].textures[10] = textures[205];
				this->model_state.aset[2].textures[11] = textures[206];
				this->model_state.aset[2].textures[12] = textures[207];
				this->model_state.aset[2].textures[13] = textures[208];
				this->model_state.length = 3;
				this->model_state.single = true;
				this->model_state.target = 0;
				this->model_state.aset[0].length_ov = 3;
				this->model_state.aset[1].length_ov = 14;
				this->model_state.aset[2].length_ov = 13;
				this->model_state.aset[0].change_sprites = true;
				this->model_state.aset[1].change_sprites = true;
				this->model_state.aset[2].change_sprites = true;
			}
		}
};
class ziggs:public simple_ai //WIP Boss class.
{
	public:
		int attack_timer = 0;
		int attack_intervals = 1000;
		bool launch_bombs = false;
		bool sky_bombs = false;
		bool panic = false;
		bool satchel = true;
		bool launchdirr = true;
		double x_vel = .2;
		//int hit_timer = 3000;
		double y_vel = -.5;
		bool launching = false;
		void create_tex()
		{
				this->model = textures[209];
				this->model_state.aset.push_back(animation_set(100,3));
				this->model_state.aset[0].textures.resize(4);
				this->model_state.aset[0].textures[0] = textures[210]; //ZIGGS WALK
				this->model_state.aset[0].textures[1] = textures[211];
				this->model_state.aset[0].textures[2] = textures[212];
				this->model_state.aset[0].textures[3] = textures[213];		
				this->model_state.aset.push_back(animation_set(100, 1));
				this->model_state.aset[1].textures.resize(1);
				this->model_state.aset[1].textures[0] = textures[209];
				this->model_state.aset.push_back(animation_set(100, 17));
				this->model_state.aset[2].textures.resize(18);
				this->model_state.aset[2].textures[0] = textures[215];
				this->model_state.aset[2].textures[1] = textures[216];
				this->model_state.aset[2].textures[2] = textures[217];
				this->model_state.aset[2].textures[3] = textures[218];
				this->model_state.aset[2].textures[4] = textures[219];
				this->model_state.aset[2].textures[5] = textures[220];
				this->model_state.aset[2].textures[6] = textures[221];
				this->model_state.aset[2].textures[7] = textures[222];
				this->model_state.aset[2].textures[8] = textures[223];
				this->model_state.aset[2].textures[9] = textures[224];
				this->model_state.aset[2].textures[10] = textures[225];
				this->model_state.aset[2].textures[11] = textures[226];
				this->model_state.aset[2].textures[12] = textures[227];
				this->model_state.aset[2].textures[13] = textures[228];
				this->model_state.aset[2].textures[14] = textures[229];
				this->model_state.aset[2].textures[15] = textures[230];
				this->model_state.aset[2].textures[16] = textures[231];
				this->model_state.aset[2].textures[17] = textures[232];
				this->model_state.aset.push_back(animation_set(100, 12));
				this->model_state.aset[3].textures.resize(13);
				this->model_state.aset[3].textures[0] = textures[233];
				this->model_state.aset[3].textures[1] = textures[234];
				this->model_state.aset[3].textures[2] = textures[235];
				this->model_state.aset[3].textures[3] = textures[236];
				this->model_state.aset[3].textures[4] = textures[237];
				this->model_state.aset[3].textures[5] = textures[238];
				this->model_state.aset[3].textures[6] = textures[239];
				this->model_state.aset[3].textures[7] = textures[240];
				this->model_state.aset[3].textures[8] = textures[241];
				this->model_state.aset[3].textures[9] = textures[242];
				this->model_state.aset[3].textures[10] = textures[243];
				this->model_state.aset[3].textures[11] = textures[244];
				this->model_state.aset[3].textures[12] = textures[245];
				this->model_state.aset.push_back(animation_set(100, 4));
				this->model_state.aset[4].textures.resize(5);
				this->model_state.aset[4].textures[0] = textures[247];
				this->model_state.aset[4].textures[1] = textures[248];
				this->model_state.aset[4].textures[2] = textures[249];
				this->model_state.aset[4].textures[3] = textures[250];
				this->model_state.aset[4].textures[4] = textures[251];
				this->model_state.aset[4].length_ov = 4;
				this->model_state.aset[4].change_sprites = false;
				this->model_state.aset[3].length_ov = 12;
				this->model_state.aset[3].change_sprites = true;				
				this->model_state.length = 4;
				this->model_state.target = 0;
				this->model_state.single = true;
				this->model_state.aset[0].change_sprites = true;
				this->model_state.aset[0].length_ov = 3;
				this->model_state.aset[1].length_ov = 0;
				this->model_state.aset[2].length_ov = 17;
				this->model_state.aset[2].change_sprites = true;
		}
		ziggs(int x, int y, int id):simple_ai(x, y, id = -1)
		{
			hp = 4;
			this->w = 80;
			this->h = 70;
			
		}
		
		void process_launch()
		{
			model_state.target = 4;
			if(y_vel > .49)
			{
				model_state.aset[4].frame = 1;
			}
			else if(y_vel > .3)
			{
				model_state.aset[4].frame = 2;
			}
			else if(y_vel > 0)
			{
				model_state.aset[4].frame = 3;
			}
			else
			{
				model_state.aset[4].frame = 4;	
			}
			y += y_vel * ticks_per_frame;
			x += x_vel * ticks_per_frame;
			y_vel += .05;
			if(y > 480)
			{
				y_vel = -.05;
				launching = false;
				panic = false;
				int where = rand() % 2 + 1;
				y = 100;
				if(where == 1)
				{
					x = cam.x - 480;
				}
				else
				{
					x = cam.x + 480 ;
				}
				satchel = true;
				model_state.target = 1;
				attacking = false;
			}
		}
		void launch()
		{
			launching = true;
			if(!launchdirr)
			{
				x_vel = .2;
			}
			else
			{
				x_vel = -.2;
			}
		}
		void handle_ai(std::vector<terrain_piece> map, int tx, int ty)
		{
			int diff = this->get_Diff(tx);
			double speed = .1;
			if(diff > 0)
			{
				facing = true;
			}
			else
			{
				facing = false;
				speed = -speed;
			}
			if(diff < 275 && diff > -275 || panic)
			{
				if(diff < 50 && diff > -50)
				{
					//panic = true;
					attacking = false;
					launch_bombs = false;
					sky_bombs = false;
				}
				if(!attacking && !panic)
				{		
					this->model_state.target = 1;
				
				if(attack_timer + attack_intervals < SDL_GetTicks() && !panic)
				{
						int action = rand() % 3 + 1;
						if(action == 1 || action == 3)
						{
							launch_bombs = true;
							model_state.target = 2;
							attacking = true;
						}
						else if(action == 2)
						{
							sky_bombs = true;
						}
					}
				}
			}
			else if(!attacking)
			{
				model_state.target = 0;
				move(speed, map, true);				
			}
			int renderx, rendery;
			if(this->x  > cam.x)
			{
				renderx = SCREEN_WIDTH/2 + (this->x - cam.x); 
			}
			else
			{
				renderx = SCREEN_WIDTH/2 - (cam.x - this->x);
			}
			if(this->y  > cam.y)
			{
				rendery = SCREEN_HEIGHT/2 + (this->y - cam.y);
			}
			else
			{
				rendery = SCREEN_HEIGHT/2 - (cam.y - this->y);
			}
			this->pos_rect.x = renderx;
			this->pos_rect.y = rendery;
			render_y_ = rendery;
 			render_x_ = renderx;
 			set_render();
			this->pos_rect.w = this->w + this->hit_box_x_set;
			this->pos_rect.h = this->h + this->y_offset;
		}
	//	ziggs()
	//	{
			
	//	}
};
#endif
