/********************************************************************
	created:	2006/08/08
	created:	8:8:2006   20:08
	filename: 	\FltkRayCast\src\psTranFunGroup.h
	file path:	\FltkRayCast\src
	file base:	psTranFunGroup
	file ext:	h
	author:		tangwei
	
	purpose:	a group of widgets for setting the transfer function of volume data
*********************************************************************/
#ifndef _PSTRANFUNGROUP_H
#define  _PSTRANFUNGROUP_H
#include "RayCastViewPort.h"
#include <FL\Fl_Group.H>
//#include <FL/Fl_Color_Chooser.H>
#include "psTranFunActiveX.h"
//#include <flu/Flu_Dual_Slider.h>
#include <FL/fl_draw.H>
#include <fl/Fl_Button.H>
#include <fl/Flmm_Scalebar.H>
#include <FL/Flmm_ColorA_Chooser.H>


#include "psVolumeMap.h"

class psTranFunGroup :	public Fl_Group
{
public:
	psTranFunGroup(int x, int y, int w, int h,RayCastViewPort* v,psVolumeMap* vm,const char *label = 0);



	
	GLfloat range_Min;
	GLfloat range_Max;
	GLfloat view_Min;
	GLfloat view_Max;

	static keynode* node;
	int focusnode;
private:
	static void slider_cb( Fl_Widget *w, void* g);
	static void activeX_cb( Fl_Widget *w, void* g);
	static void color_cb( Fl_Widget *w, void* g);
	static void setbt_cb(Fl_Widget *w, void* g);

	psTranFunActiveX* activeX;
	//Flu_Dual_Slider* dslider;
	Flmm_Scalebar* dslider;
	//Fl_Color_Chooser* colorpanel;
	Flmm_ColorA_Chooser* colorpanel;
	Fl_Button* setbt;
	RayCastViewPort* vp;
	psVolumeMap* VolumeMap;
public:
	~psTranFunGroup(void);
};

#endif