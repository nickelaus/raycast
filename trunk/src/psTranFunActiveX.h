/********************************************************************
	created:	2006/08/08
	created:	8:8:2006   20:09
	filename: 	\桌面\FltkRayCast\src\psTranFunActiveX.h
	file path:	\桌面\FltkRayCast\src
	file base:	psTranFunActiveX
	file ext:	h
	author:		tangwei
	
	purpose:	a widget for setting the transfer function by interacting with user 
*********************************************************************/
#ifndef _PSTRANFUNACTIVEX_H
#define  _PSTRANFUNACTIVEX_H
#include <FL/Fl_Gl_Window.h>
#include <fl/Fl.H>
#include <FL/gl.h>
#include <gl/glut.h>
#include <math.h>

#include "psVolumeMap.h"
class psTranFunActiveX :	public Fl_Gl_Window
{
private:
	void draw();
	void draw_overlay();
	int handle( int event );
	bool _firstDraw;
public:
	psTranFunActiveX(int x, int y, int w, int h,int &f,psVolumeMap* vm , const char *label = 0);
	~psTranFunActiveX(void);

	
	GLfloat range_Min;
	GLfloat range_Max;
	GLfloat view_Min;
	GLfloat view_Max;


private:
    //灰度直方图
	void hist() ;
	int* histogram;
	int hcount;
	int hmax;
	float step;



	psVolumeMap* VolumeMap;
	keynode* node;
	int nodecount;
	int &focusnode;
	GLUquadricObj *qobj;

	unsigned char* data;
	int count;


	struct viewpos
	{
		GLfloat x;
		GLfloat y;
	};

	struct worldpos
	{
		GLfloat x;
		GLfloat y;
	};
	const viewpos worldpos2viewpos(GLfloat x,GLfloat y) const
	{
		viewpos pos;
		if (x>=view_Min && x<=view_Max)
		{
			pos.x=w()*(x-view_Min)/(view_Max-view_Min);
			pos.y=h()*(1-y);
		}
		else
		{
			pos.x=-1;
			pos.y=-1;
		}
		return pos;
	}
	const worldpos viewpos2worldpos(GLfloat x,GLfloat y) const
	{
		worldpos pos;
		pos.x=x*(view_Max-view_Min)/w()+view_Min;
		pos.y=1-y/h();
		return pos;
	}
};

#endif