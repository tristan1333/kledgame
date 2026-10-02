#include <algorithm>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "characters.h"
#include "load_textures.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include "config.h"
#include "camera.h"
#include "renderer.h"
#include "animation_sets.h"
#include "Multi_Piece.h"
#include "AI_Handler.h"
#include "projectile_manager.h"
#include "terrain.h"
#include "spawner.h"
#include "map_handler.h"
#include "background.h"
#include "collectibles.h"
#include "fps_handler.h"
#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4
#define PLAY_1_SPIN 0
#define PLAY_2_SPIN 1
#define F_STATE 0
#define G_STATE 1
#define UP_STATE 2
#define DOWN_STATE 3
#define MENU 1
#define GAME 3
#define START 1
#define QUIT 3
#define CHARACTERS 2
#define GAME_START 3
#define PLAYER_MODEL 0
#define FLOOR 1 //Declaration of multiple parts. States for ease of access, also includes external files which need to be accessed by the program.

bool track = true;
int GAME_STATE = MENU; //GAME_STATE tracks how the game is currently running.
bool follow_char = true; //Whether the camera is following the character or not. Test variable.

//bool update_map = true; //What
int MENU_STATE = 0; //The state of a menu. Used with multiple menus.
int timer = 0; //Used for timing key presses.
//int camera_timer = 0; //Might of been a good concept. No clue what it does. Irrelevant anyway.

int cam_timer = 6000;

std::vector<terrain_piece> map; //This is the temporary map. It's is unloaded/loaded again in map_handler.
/*
-------------------------------------------------
-------------------------------------------------
-------------------------------------------------
!!!!!!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!!!!!!!
			CURRENTLY EVERYTHING RELIES ON MAP OBJECT
			MOVE EVERYTHING FROM RELYING ON MAP OBJECT!!!!!!!!!
-------------------------------------------------
-------------------------------------------------
-------------------------------------------------
*/

bool quit = false; //Bleh

Mix_Chunk *gstab = NULL; //Test for sound mixing. Keeping here for future reference.
SDL_Window* mainwin; //Pointer to the mainwindow.
SDL_Surface* mainsurface; //Pointer to the mainsurface.
TTF_Font* mainfont; //

SDL_Texture * text;

SDL_Texture* loadText(std::string Text,SDL_Color color) //Loads text to a texture.
{
	SDL_Texture* tex;
	SDL_Surface* text_surf = TTF_RenderText_Solid(mainfont,Text.c_str(),color);// tristan is a gay cunt
	tex = SDL_CreateTextureFromSurface( mainren, text_surf);
	SDL_FreeSurface(text_surf);
	return tex;
}


SDL_Texture* loadTexture(const std::string &path) //Prepares a texture. Takes an outside image file path.
{	
	SDL_Texture* newtex = NULL;
	
	SDL_Surface* loadedimg = IMG_Load(path.c_str());
	if(loadedimg != NULL)
	{
		newtex = SDL_CreateTextureFromSurface(mainren, loadedimg);
		SDL_FreeSurface(loadedimg);
		if(newtex == NULL)
		{
			printf("Failed to create texture from surface");
		}
	}
	else
	{
		printf("Failed to loadBMP");
	}
	return newtex;
}





Mix_Chunk* loadMusic(const std::string &path) //Loads sound files. Not used.
{
	Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
	return sound;
}
void restart() //Restarts the game world and resets the character back to original state. 
{
	SDL_RenderPresent(mainren);
	SDL_Delay(2000);
	mainchar.x = 0;
	mainchar.y = 0;
	mainchar.mounted = true;
	mainchar.pnts = 0;
	mainchar.scores = 0;
	mainchar.get_off = false;
	mainchar.hp = 4;
	mainchar.x_offset = 1;
	mainchar.hit_area = 60;
	mainchar.attackspeed = 10;
	mainchar.firing = false;
	mainchar.knockback_curr_x = 0;
	mainchar.knockback_curr_y = 0;
	mainchar.knockback_x_lim = 0;
	mainchar.knockback_x_pw = 0;
	mainchar.knockback_y_lim = 0;
	mainchar.knockback_y_pw = 0;
	mainchar.hit_box_x_set = 0;
	mainchar.indash = false;
	mainchar.ducking = false;
	mainchar.immobile = false;
	mainchar.reset_sprites();
	mainchar.model_state.aset[9].render = true;
	mainchar.model_state.aset[0].render = true;
	g_map.chunks.clear();
	g_map.active = -1;
	g_map.max = 0;
	g_map.firstgen = false;
	cam.track = false;
	cam.halt = false;
	battle = false;
	beat = 0;
	ACTIVE_BOSS = ZIGGS;
	zigg.hp = 5;
	ziggs_fight = false;
	zigg.panic = false;
	zigg.satchel = true;
	zigg.attacking = false;
	zigg.launching = false;
	zigg.launch_bombs = false;
	zigg.model_state.target = 1;
	AI_List.clear();
	P_List.clear();
	coll_List.clear();
	map.clear();
	s_burst.clear();
	s_burst = {spawn_burst(5,1,3000, UNIT),spawn_burst(1,2,7000, UNIT)/*, spawn_burst(8,BOMB_MARKER,3000, COLLECTIBLE)*/};	
	printf("Success clear");
}

bool init() //Starts up the SDL library and makes sure everything is working. 
{
	bool success = true;
	apply_config();
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		success = false;
	}
	else
	{
		mainwin = SDL_CreateWindow("Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if(mainwin == NULL)
		{
			success = false;
		}
		else
		{
			mainsurface = SDL_GetWindowSurface(mainwin);
		}
		
		mainren = SDL_CreateRenderer(mainwin, -1, SDL_RENDERER_ACCELERATED);
		int imgflags = IMG_INIT_PNG;
		if(!(IMG_Init(imgflags) & imgflags))
		{
			success = false;
		}
		if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048 ) < 0)
		{
			success = false;
		}
		if(mainren == NULL)
		{
			success = false;
		}
	}
	TTF_Init(); 
	mainfont = TTF_OpenFont("font.ttf", 28);
	if(!mainfont)
	{
		success = false;
	}
	create_texture_list();
	return success;
}

void handle_map() //Loads out the map as necessary. Allows the world to continue on forever. Can take in different sets. Technically allows for varying parts in the map, but currently just flat. 
{
	if(!g_map.firstgen)
	{
		g_map.chunks.push_back(terrain_set(0,24));
		g_map.chunks.push_back(terrain_set(640,24));
		g_map.firstgen = true;
	}
	bg.set();
	bg.render_tiles();
	if(!g_map.check_map(cam.x)) //USE A WHILE LOOP!!!!!! USING AN IF, THIS WILL CONTINUE, AND CAN LEAVE AN EMPTY SPACE!!!! EVEEEL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DSGSDKOGgjnujngb
	{
		map.clear();
	//	printf("Cleared the map!");
		for(int i = 0; i < g_map.chunks[g_map.active].pieces.size();i++)
		{
			map.push_back(g_map.chunks[g_map.active].pieces[i]);
		//	printf("1PLACED1");
		}
	//	printf("Placed active target!");
		if(g_map.active - 1 > -1)
		{
			for(int i = 0; i < g_map.chunks[g_map.active - 1].pieces.size();i++)
			{
				map.push_back(g_map.chunks[g_map.active - 1].pieces[i]);
				//printf("2PLACED2");
			}
		//	printf("Placed active target - 1!");
		}
		/*if(g_map.active - 1 > 0)
		{
			for(int i = 0; i < g_map.chunks[g_map.active - 2].pieces.size();i++)
			{
					map.push_back(g_map.chunks[g_map.active - 2].pieces[i]);	
			}
		}*/

		for(int i = 0; i < g_map.chunks[g_map.active + 1].pieces.size();i++)
		{
			map.push_back(g_map.chunks[g_map.active +1].pieces[i]);
		//	printf("3PLACED3");
		}
	//	printf("Generated all three areas!");
	}
}

int handle_menu(int mod, int max) //Changes the menus if they are matched up wrong. 
{
	MENU_STATE += mod;
	if(MENU_STATE > max)
	{
		MENU_STATE = 0;
	}
	else if(MENU_STATE < 0)
	{
		MENU_STATE = max;
	}
	
	timer = SDL_GetTicks();
}

void change_menu() //Changes the menu state. 
{
	if(GAME_STATE == MENU)
	{
		switch(MENU_STATE)
		{
			case 0:
				GAME_STATE = GAME;
				break;
			case 1:
				GAME_STATE = CHARACTERS;
				break;
			case 2:
				quit = true;
				break;
		}
	}
	MENU_STATE = 0;
	return;
}

/*void handle_spawn()
{
	for(int i = 0;i < spawn_list.size();i++)
	{
		spawn_list[i].spawn();
	}
}*/


void menu_game(SDL_Event *e,const Uint8 *key) //Runs the game world. Includes all of the handlers necessary. Whenever it runs through this, the game is advancing one step.
{
	if(e->type == SDL_QUIT)
	{
		quit = true;
	}//
//	global_map.check_map_generation(mainchar.x);
//	map = create_map(mainchar.x);
//	if(global_map.check_map())
//	{
//		map.clear();
//
//	}
	//CHECK IF MAP NEEDS TO BE CLEARED
		//CLEAR THE MAP
		//CREATE THE MAP
//	renSpec(mainren,textures[24],SCREEN_WIDTH,SCREEN_HEIGHT,0,0);
	if(track)
	{

	handle_map();
	Dynamic_spawns();
	handle_AI(map,mainchar.x,mainchar.y);
	handle_bosses(map,mainchar.x,mainchar.y);
	handle_collectibles(map);
	handle_projectiles();
}
	else
	{
		handle_AI(map,mainchar.x,mainchar.y,false);
	}
	int renderx,rendery;
	/*if(test.x  > cam.x)
		{
			renderx = SCREEN_WIDTH/2 + (test.x - cam.x); 
		}
		else
		{
			renderx = SCREEN_WIDTH/2 - (cam.x - test.x);
		}
		if(test.y  > cam.y)
		{
			rendery = SCREEN_HEIGHT/2 + (test.y  - cam.y);
		}
		else
		{
			rendery = SCREEN_HEIGHT/2 - (cam.y - test.y);
		}
	test.handle_ai(map,mainchar.x,mainchar.y);
	test.handle_render(renderx,rendery);*/
//	SDL_Texture * tex = loadTexture("resources/play.png");

	/*if(two_test.x  > cam.x)
		{
			renderx = SCREEN_WIDTH/2 + (two_test.x - cam.x); 
		}
		else
		{
			renderx = SCREEN_WIDTH/2 - (cam.x - two_test.x);
		}
		if(test.y  > cam.y)
		{
			rendery = SCREEN_HEIGHT/2 + (two_test.y  - cam.y);
		}
		else
		{
			rendery = SCREEN_HEIGHT/2 - (cam.y - two_test.y);
		}
	two_test.handle_ai(map,mainchar.x,mainchar.y);
	two_test.handle_render(renderx,rendery);*/
	if(track)
	{
		mainchar.handle_character(key,map);
		if(mainchar.get_off)
		{
			coll_List.push_back(collectibles(mainchar.x,mainchar.y,textures[40],2,80,70,false));
			mainchar.get_off = false;
		}
		if(mainchar.pnts > 19 && !mainchar.mounted && mainchar.hp > 0)
		{
			mainchar.pnts = 0;
			coll_List.push_back(collectibles(SCREEN_WIDTH + cam.x, 100, textures[40],3,80,70));
		}
 	}
//	for(int i = 0;i < map.size();i++)
//	{

	//	SDL_SetTextureColorMod(textures[6],rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
	//	if(map[i])
	/*	if(map[i].w == 0 || (map[i].x + map[i].w) < cam.x  - SCREEN_WIDTH/2 || map[i].x >= cam.x + SCREEN_WIDTH/2 || map[i].y < cam.y - SCREEN_HEIGHT/2 || map[i].y > cam.y + SCREEN_HEIGHT)
		{
			continue;
		}
		if(map[i].x > cam.x)
		{
			renderx = SCREEN_WIDTH/2 + (map[i].x - cam.x); 
		}
		else
		{
			renderx = SCREEN_WIDTH/2 - (cam.x - map[i].x);
		}
		if(map[i].y > cam.y)
		{
			rendery = SCREEN_HEIGHT/2 + (map[i].y - cam.y);
		}
		else
		{
			rendery = SCREEN_HEIGHT/2 - (cam.y - map[i].y);
		}
		int a = 0;
		while(a <= map[i].w)
		{
			renderTexture(textures[6],mainren,renderx + a,rendery);
			a += 10;
		}
		a -= 10;
		if(a > map[i].w)
		{
		}
	}*/
		if(key[SDL_SCANCODE_RIGHT] && timer + 20 < SDL_GetTicks())
		{
			timer = SDL_GetTicks();
			track = false;	
			cam.x += 10;	
		}
		if(key[SDL_SCANCODE_LEFT] && timer + 20 < SDL_GetTicks())
		{
			timer = SDL_GetTicks();
			track = false;
			cam.x -= 10;
		}
		if(key[SDL_SCANCODE_A] || key[SDL_SCANCODE_D] || key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S])
		{
			track = true;
		}
		if(track)
		{
			if(!cam.halt && !cam.track)
			{
				cam.x = mainchar.x;
			}
			if(cam.track)
			{
				cam.chase();
				cam.x = mainchar.x + cam.catch_offset;
			}

			if(cam_timer < SDL_GetTicks())
			{
				if(!battle)
				{
				
			//Begin_Battle();
				cam_timer = 1000000;
				}
			}
		}
		if(mainchar.x >= cam.x)
	{
		renderx = SCREEN_WIDTH/2 + (mainchar.x - cam.x);
	}
	else
	{
		renderx = SCREEN_WIDTH/2 - (cam.x - mainchar.x);
	}
	if(mainchar.y > cam.y)
	{
		rendery = SCREEN_HEIGHT/2 + (mainchar.y - cam.y);
	}
	else
	{
		rendery = SCREEN_HEIGHT/2 - (cam.y - mainchar.y);
	}
	SDL_Rect ree;
	if(track)
	{
	check_AI_Collision();
	}
	mainchar.handle_render(renderx,rendery);
//	renSpec(mainren,mainchar.model,64,64,renderx,rendery);
//	renderTexture(mainchar.port,mainren,0,SCREEN_HEIGHT - 64);
	SDL_Color black {255,255,255,255};
	std::string s = std::to_string(mainchar.pnts);
	text = loadText(s,black);
	renderTexture(text,mainren,50,20);
	SDL_DestroyTexture(text);
	text = nullptr;
	if(battle)
	{
		s = std::to_string(target_scores - mainchar.scores);
		text = loadText(s,black);
		renderTexture(text,mainren,SCREEN_WIDTH/2 - 30, 100);
	}
	SDL_DestroyTexture(text);
	text = nullptr;
	return;
}

void character_menu(SDL_Event *e,const Uint8 *key) //The help menu. Originally was a character menu.
{
	renderTexture(textures[174], mainren, 0, 0);
	if(e->type == SDL_QUIT)
	{
		quit = true;
	}
	if(timer + 100 < SDL_GetTicks() && key[SDL_SCANCODE_ESCAPE])
	{
		GAME_STATE = MENU;
		return;
	}
	SDL_RenderPresent(mainren);
}

void main_menu(SDL_Event *e,const Uint8 *key) //The main menu handler. 
{
	if(e->type == SDL_QUIT)
	{
		quit = true;
	}
	float y_mod;
	if(timer + 100 < SDL_GetTicks() && key[SDL_SCANCODE_UP])
	{
		handle_menu(-1,2);
	}
	if(timer + 100 < SDL_GetTicks() && key[SDL_SCANCODE_DOWN])
	{
		handle_menu(1,2);
	}
	if(timer + 100 < SDL_GetTicks() && key[SDL_SCANCODE_RETURN])
	{
		change_menu();
	}
	renderTexture(textures[1],mainren,SCREEN_WIDTH/10,SCREEN_HEIGHT/9);
	renderTexture(textures[2],mainren,SCREEN_WIDTH/10.2,SCREEN_HEIGHT/4.6);
	renderTexture(textures[3],mainren,SCREEN_WIDTH/10,SCREEN_HEIGHT/4.4);
	switch(MENU_STATE)
	{
		case 0:
			y_mod = 9;
			break;
		case 1:
			y_mod = 6;
			break;
		case 2:
			y_mod = 4.4f;
			break;
	}
	renderTexture(textures[4],mainren,SCREEN_WIDTH/25,SCREEN_HEIGHT/y_mod);
	SDL_RenderPresent(mainren);
	return;
}

int main(int argc, char** argv) //The main function.
{
			srand(time(NULL));
	if(!init())
	{
		SDL_Quit;
	}
	SDL_Event e;
	//simple_ai two_test(400,0);
	mainchar.create_tex();
	//test.create_tex();
	//two_test.create_tex();
//SDL_Texture* tex;
//	map.resize(1, //	map.resize(10,terrain_piece(0,0,0,0));
	//map[0] = terrain_piece(0,220,1000,10);
//	Mix_Chunk* gstab = loadMusic("resources/stab.wav");	
//	map.clear();
//	map = create_map(0);										//curr_time = SDL_GetTicks();
//	spawn_list.push_back(spawner(100,0));
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	int fps_time = SDL_GetTicks();
	double previous = SDL_GetTicks();
	double lag = 0.0;
	int frameps = 0;
	AI_List.reserve(100);
	forest_bg.push_back(parallax_piece(0,textures[163]));
	forest_bg.push_back(parallax_piece(.75,textures[160]));
	forest_bg.push_back(parallax_piece(1, textures[162]));
	forest_bg.push_back(parallax_piece(1, textures[161]));
//	AI_List.push_back(simple_ai(0,0,AI_BOMBER));
	SDL_RenderSetLogicalSize(mainren,SCREEN_WIDTH, SCREEN_HEIGHT);
	zigg.create_tex();
	//start_ziggs_fight();
	while(!quit)  																				//if(currentKeyStates[SDL_SCANCODE_UP]) function[SDL_DEFINED_KEYCODE]
	{

//		double start = SDL_GetTicks();
		double current = SDL_GetTicks();
		double elapsed = current - previous;
		lag += elapsed;
		switch(GAME_STATE)
		{
			case MENU:
				SDL_RenderClear(mainren);
				main_menu(&e,currentKeyStates);
				previous = SDL_GetTicks();
				break;
			case CHARACTERS:
				SDL_RenderClear(mainren);
				character_menu(&e,currentKeyStates);
				break;
			case GAME:
				double targ = (ticks_per_frame * 100)/10000;
				SDL_RenderClear(mainren);
				while(lag >= targ)
				{
					menu_game(&e,currentKeyStates);
					lag -= ticks_per_frame;
				}
				if(mainchar.hp < 1 && !mainchar.mounted)
				{
					restart();
				}
				break;
		}
		SDL_RenderPresent(mainren);
	//	frameps++;
		SDL_PollEvent(&e);
		int capper = SDL_GetTicks() - get_Ticks();
		if(capper < ticks_per_frame)
		{
			SDL_Delay(ticks_per_frame - capper);
		}
		
		previous = current;
	}
}
