#ifndef CAMERA_DEFINES_T
#define CAMERA_DEFINES_T

class camera //The camera that decides where things are positioned
{
	public:
		int x = 0;
		int y = 0;
		int catch_offset = 0;
		bool halt = false;
		bool track = false;
		camera(double x_t, double y_t)
		{
			this->x = x_t;
			this->y = y_t;
		}
		void begin_CatchUp(int x_diff) //Beings the process of the camera moving towards the player after being offset.
		{
			catch_offset = x_diff;
			halt = false;
			track = true;
		}
		void chase() //Moves the camera back over the player.
		{
			if(catch_offset > 0)
			{
				catch_offset -= 1;
			}
			else if(catch_offset < 0)
			{
				catch_offset += 1;
			}
			if(catch_offset == 0)
			{
				track = false;
			}
		}
};

camera cam(0,0); //Global camera. 

#endif
