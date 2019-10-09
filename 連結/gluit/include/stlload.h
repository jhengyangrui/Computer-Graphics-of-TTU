#include <glut.h>
#include <vector>
using namespace std;

typedef struct {
	float normal[3];
	float vertex1[3];
	float vertex2[3];
	float vertex3[3];
	char unuse[2];
} FACET;

typedef vector<FACET> STLmodel;

extern STLmodel stlRead(char*);
extern void stlUnitize (STLmodel &);
extern GLuint stlRender2DL(const STLmodel&);
