#include "svl/svl.h"
#include <gl/glew.h>
#include "gluit.h"
#include "gl/glpng.h"


void aim ();
class Duck
{
    public:
		int name;
		int hit;
		Vec pos;

    void render();
	void step();
	void set_name (int set) {name = set;}

	Duck() : hit(0) {};
	Duck(Vec3 p): hit(0) {move = 0; flip = 1; pos = p;}

	private:
		float theta, move;
		int respawn;
		int respawn_timer;
		int flip;
};
