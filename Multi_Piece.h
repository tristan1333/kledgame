#ifndef MULTI_PIECE_DEFINE
#define MULTI_PIECE_DEFINE
#include "SDL.h"
#include <vector>
#include "animation_sets.h"


class multi_piece //Allows for multiple pieces used in one sprite. 
{
	public:
		int length = 0;
		int target = 0;
		bool single = false;
		std::vector<animation_set> aset;
		void render_piece(SDL_Rect rect, SDL_RendererFlip flip)
		{
			if(single)
			{
				if(this->aset[target].change_sprites)
				{
					this->aset[target].change_sprite();	
				}	
				rect.x += this->aset[target].x_offset;
				rect.y += this->aset[target].y_offset;
				SDL_RenderCopyEx(mainren,this->aset[target].textures[this->aset[target].frame],NULL,&rect, 0,NULL,flip);
				rect.x -= this->aset[target].x_offset;
				rect.y -= this->aset[target].y_offset;
		
				return;
			}
			for(int i = 0; i < aset.size(); i++)
			{
				if(!aset[i].render)
				{
					continue;
				}
				if(this->aset[i].change_sprites)
				{
					this->aset[i].change_sprite();
				}
				rect.x += this->aset[i].x_offset;
				rect.y += this->aset[i].y_offset;
				SDL_RenderCopyEx(mainren,this->aset[i].textures[this->aset[i].frame],NULL,&rect, 0,NULL,flip);
				rect.x -= this->aset[i].x_offset;
				rect.y -= this->aset[i].y_offset;
			}
		}
};
#endif //FOR ANIMATIONS WE NEED TO INDIVIDUALLY CHANGE THE PIECES INSIDE OF THE VECTOR, TO ACTUALLY RENDER WE NEED TO CALL RENDER_PIECE. THIS WILL DISPLAY ALL OF THE PIECES. CHANGE STATE OF TEXTURES, THEN RENDER THEM ALL TOGETHER
