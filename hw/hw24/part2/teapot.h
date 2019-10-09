#include "svl/svl.h"

class Teapot 
{
public:
	Vec3 pos;
	int score;
	int status;
	int count;
	
	Teapot(){status = 1;count = 20;};
	void show();
};

