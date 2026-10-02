#ifndef ANIMATION_SETS_A
#define ANIMATION_SETS_A
#include <SDL.h>
#include <vector>

class animation_set //Animation set which makes allows for moving sprites. 
{
	public:
		bool render = false;
		int sprite_timer;
		int timer = 0;
		int length_ov;
		int x_offset = 0;
		int y_offset = 0;
		std::vector <SDL_Texture*> textures;
		int frame = 0;
		bool change_sprites = false;
		void apply_offsets(int x, int y)
		{
			this->x_offset = x;
			this->y_offset = y;
		}
		void change_sprite()
		{
			if(this->timer + this->sprite_timer > SDL_GetTicks())
			{
				return;
			}
			if(this->frame < this->length_ov)
			{
				this->frame++;
			}
			else
			{
				this->frame = 0;
			}
			timer = SDL_GetTicks();
			return;
		};	
		animation_set(int t, int size)
		{
			this->sprite_timer = t;	
			this->length_ov = size;
		};	
};
#endif
