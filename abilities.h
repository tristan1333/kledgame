#ifndef ABILITIES_DEF_T
#define ABILITIES_DEF_T
#include <SDL.h>
#include "load_textures.h"

class abilities;

class abilities
{
	public:
		std::string name;
		std::string description;
		SDL_Texture* portrait;
		int last_cast = 0;
		abilities(const std::string port,std::string nam, std::string desc)
		{
			this->name = nam;
			this->description = desc;
			this->portrait = loadTexture(nam);
		}
		void cast()
		{
			return;
		}
};

class undercut:public abilities
{
	public:
		abilities::name = "Undercut";
		abilities::description = "Quickly stab the target";
		abilities::portrait = loadTexture("resources/abilities/fighter/undercut.png")
};

class martial:public abilities
{
	public:
		abilities::name = "Martial Prowess";
		abilities::description = "Basic attacks reduce the cooldown of Undercut by 2 seconds. Activating this ability improves your basic attacks for 3 seconds. You may recast to roll forward in this time.";
		abilities::portrait = loadTexture("resources/abilities/fighter/martial.png");
};

class parry:public abilities
{
	public:
		abilities::name = "Parrying Dagger";
		abilities::description = "The next attack in 2 seconds is negated. Your next attack deals bonus damage and Undercut is refreshed.";
		abilities::portrait = loadTexture("resources/abilities/fighter/parry.png");
};

#endif
