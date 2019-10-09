#define CHECK_ERROR(str)                                           \
{                                                                  \
    GLenum error;                                                  \
    if(error = glGetError())                                       \
       printf("GL Error: %s (%s)\n", gluErrorString(error), str);  \
}

#define CHECK_OPENGL_ERROR(cmd) \
    { cmd; \
     { GLenum  error; \
       while ( (error = glGetError()) != GL_NO_ERROR) { \
		printf( "[%s:%d] '%s' failed with error %s\n", \
		__FILE__, __LINE__, #cmd, gluErrorString(error) ); }\
	 }}

#define BEGIN_2D_OVERLAY(ww,hh)	 \
{                \
	glMatrixMode (GL_PROJECTION);        \
	glPushMatrix(); \
	glLoadIdentity(); gluOrtho2D (0,ww,0,hh);\
	glMatrixMode (GL_MODELVIEW);       \
    glPushMatrix(); glLoadIdentity();  \
    glPushAttrib (GL_ALL_ATTRIB_BITS); glDisable (GL_DEPTH_TEST);\
}

#define END_2D_OVERLAY() \
{  \
	glMatrixMode (GL_PROJECTION); glPopMatrix(); \
    glMatrixMode (GL_MODELVIEW); glPopMatrix(); \
	glPopAttrib(); \
}
