#ifndef PROJECTILE_MANG
#define PROJECTILE_MANG
#include "SDL.h"
#include "characters.h"
#include "fps_handler.h"
#include <string>

class projectile //Projectile class. Used for bullets and arrows. 
{
	public:
		double velocity = 0;
		double vel_y = 0;
		double x = 0;
		double y = 0;
		double w = 0;
		double h = 0;
		double travel_rate = .25;
		int last_travel = 0;
		int hitdiv = 1;
		bool enemy = true;
		SDL_RendererFlip flip;
		SDL_Texture* display;
		SDL_Rect display_box;
		SDL_Rect collbox;
		projectile(int x_,int y_, double vel,int wi,int he,SDL_Texture* tex, 	SDL_RendererFlip f, int div,double y_vel, bool player)
		{
			x = x_;
			y = y_;
			velocity = vel;
			vel_y = y_vel;
			w = wi;
			h = he;
			display = tex;
			flip = f;
			display_box.w = wi;
			display_box.h = he;
			hitdiv = div;
			collbox.w = w/div;
			collbox.h = he/div;
			enemy = player; //Weird naming but it's just a check whether it damages the player
		}

		void travel() //Moves the projectile according to it's velocity. 
		{
			last_travel = SDL_GetTicks();
			x += velocity * ticks_per_frame;
			y += vel_y * ticks_per_frame;
			return;
		}
		void calc_w_h_hitbox(double x_, double y_) //Calculates the hitbox location. 
		{
			if(flip == SDL_FLIP_NONE)
			{
				collbox.x = x_ + (w/2);
				collbox.y = y_ + (h );
			}
			else
			{
				collbox.x = x_ + (collbox.w * 2);
				collbox.y = y_ + (collbox.h);
			}
		}
};

std::vector <projectile> P_List;

void handle_projectiles() //Handles all the projectiles in the world, moves them and their hitboxes. Rendersthem where they should be. 

{
	for(int i = 0;i < P_List.size();i++)
	{
		if(P_List[i].x > cam.x + 1000 || P_List[i].x < cam.x - 1000)
		{
			P_List.erase(P_List.begin() + i);
			i--;
			continue;
		}
		P_List[i].travel();
		int renderx,rendery;
		if(P_List[i].x  > cam.x)
		{
			renderx = SCREEN_WIDTH/2 + (P_List[i].x - cam.x); 
		}
		else
		{
			renderx = SCREEN_WIDTH/2 - (cam.x - P_List[i].x);
		}
		if(P_List[i].y  > cam.y)
		{
			rendery = SCREEN_HEIGHT/2 + (P_List[i].y  - cam.y);
		}
		else
		{
			rendery = SCREEN_HEIGHT/2 - (cam.y - P_List[i].y);
		}
		SDL_Rect position_rect;
		P_List[i].collbox.x = renderx;
		P_List[i].collbox.y = rendery;
		P_List[i].display_box.x = renderx;
		P_List[i].display_box.y = rendery;
		P_List[i].calc_w_h_hitbox(renderx, rendery);
		SDL_RenderCopyEx(mainren,P_List[i].display,NULL,&P_List[i].display_box, 0,NULL,P_List[i].flip);
		//renderTexture(P_List[i].display,mainren,renderx,rendery);
	}
}
#endif
