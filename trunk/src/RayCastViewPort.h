/********************************************************************
	created:	2006/08/08
	created:	8:8:2006   20:04
	filename: 	\FltkRayCast\src\RayCastViewPort.h
	file path:	\FltkRayCast\src
	file base:	RayCastViewPort
	file ext:	h
	author:		tangwei
	
	purpose:	realize the viewport class for Raycasting rendering
*********************************************************************/
#ifndef _FAYCASTVIEWPORT_H
#define _FAYCASTVIEWPORT_H 
#include "FLU/Flu_GL_Canvas.h"
#include <GL/glew.h>
#include <GL/wglext.h>
#include <FL/gl.h>
#include <GL/GLU.h>

#include "psVolumeMap.h"
class RayCastViewPort :
	public Flu_GL_Canvas
{
public:
	RayCastViewPort(int x, int y, int w, int h,psVolumeMap* vm, const char *label = 0);
public:
	virtual ~RayCastViewPort(void);

	virtual void init();

	virtual void reshape( int width, int height );


	virtual void render();

	psVolumeMap* VolumeMap;

	void resetTexture(keynode* n , int c);

	char* data;
	int count ;
private:
	void initList();
	GLuint drawSCubeList;

	float camera[3];
	float lookat[3];
	float cam[3];
	// Mouse rotation (currently not trackball)
	int mousepos_x ;
	int mousepos_y ;
	int tmpX;
	int tmpY; 

	virtual void mouse_button_down( int button, int x, int y );
	virtual void mouse_button_up( int button, int x, int y );
	virtual void mouse_drag( int x, int y );
	//virtual void keyboard( int key, int x, int y ) ;
	virtual void mouse_wheel( int dx, int dy, int x, int y ) ;

	bool isRotate;

	char* textFileRead(char *fn);
	void initTexture();
	void printShaderInfoLog(GLuint obj);
	void printProgramInfoLog(GLuint obj);
	void setShaders();

	

	
private:
	GLuint v;
	GLuint f;
	GLuint p;

	GLuint texture[3];
};
#endif