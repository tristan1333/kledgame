#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H
#include <SDL.h>
#include <fstream>
#include <string>
#include <iostream>

bool color_blind = false;
int bodies = 100;

void apply_config() //Loads out the config and changes values according to it.
{
	std::fstream file;
	file.open("config.cfg");
	if(file.bad())
	{
		printf("Bad file");
		file.close();
		return;
	}
	std::string str;
	char input[10];
	while(file.getline(input,10))
	{
		str = input;
		if(str == "COLOR")
		{
			file.getline(input,10);
			str = input;
			if(int val = std::stoi(str))
			{
				if(val != 1)
				{
					color_blind = false;
				}
				else
				{
					color_blind = true;
				}
			}
		}
		else if(str == "BODIES")
		{
			file.getline(input,10);
			str = input;
			if(int val = std::stoi(str))
			{
				bodies = val;
				std::cout << bodies;
			}
		}
	}
}

#endif // tristan is gay BY LIAM
//exec hack.exe 

