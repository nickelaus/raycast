#include "psTranFunGroup.h"
#include <iostream>
keynode* psTranFunGroup::node = new keynode[7];
psTranFunGroup::psTranFunGroup(int x, int y, int w, int h, RayCastViewPort* v,psVolumeMap* vm, const char *label):Fl_Group(x,y,w,h,label),range_Min(vm->getMinValue()),range_Max(vm->getMaxValue()),vp(v),VolumeMap(vm)
{

	node = VolumeMap->node;
	focusnode = -1;
	

	activeX = new psTranFunActiveX(x+10,y+10,500,100,focusnode,vm);
	activeX->callback(activeX_cb,this);
	/*dslider = new Flu_Dual_Slider(x+10,y+120,500,20);
	dslider->type( FL_HOR_NICE_SLIDER );
	dslider->range(range_Min,range_Max);
	dslider->high_value(range_Max);
	dslider->low_value(range_Min);
	dslider->callback(slider_cb,this);*/
	dslider = new Flmm_Scalebar(x+10,y+120,500,20);
	dslider->type(FL_HORIZONTAL);
	dslider->value((int)range_Min,(int)range_Max,(int)range_Min,(int)range_Max);
	dslider->callback(slider_cb,this);

	//colorpanel = new Fl_Color_Chooser(x+10,y+150,195,115);
	colorpanel = new Flmm_ColorA_Chooser(x+10,y+150,245,145);
	colorpanel->deactivate();
	colorpanel->callback(color_cb, this);
	setbt = new Fl_Button(x+285,y+150,100,30,"reset");

	setbt->callback(setbt_cb,this);
	end();
	
}

psTranFunGroup::~psTranFunGroup(void)
{
	delete setbt;
	delete colorpanel;
	delete dslider;
	delete activeX;

}

void psTranFunGroup::slider_cb( Fl_Widget *w, void* g)
{
	//Flu_Dual_Slider *d = (Flu_Dual_Slider*)w;
	Flmm_Scalebar *d = (Flmm_Scalebar*)w;
	psTranFunActiveX *activeX = (psTranFunActiveX *)(static_cast<psTranFunGroup*>(g)->activeX);
	//activeX->view_Min = d->low_value();
	//activeX->view_Max = d->high_value();
	activeX->view_Min = d->value();
	activeX->view_Max = d->value()+d->slider_size_i();
	activeX->redraw();
}
void psTranFunGroup::activeX_cb( Fl_Widget *w, void* g)
{
	psTranFunGroup *d = (psTranFunGroup*)g;
	//Fl_Color_Chooser *colorpanel = (Fl_Color_Chooser *)(static_cast<psTranFunGroup*>(g)->colorpanel);
	Flmm_ColorA_Chooser *colorpanel = (Flmm_ColorA_Chooser *)(static_cast<psTranFunGroup*>(g)->colorpanel);
	keynode* n = static_cast<keynode*>(d->node);
	int f = d->focusnode;
	if(f==-1)
	{
		colorpanel->deactivate();
	}
	else
	{
		
		//colorpanel->rgb(n[f].R,n[f].G,n[f].B);
		colorpanel->rgb(n[f].R,n[f].G,n[f].B,n[f].opac);
		colorpanel->activate();
		colorpanel->redraw();// for alpha [8/9/2006 tangwei]
	}
	
}
void psTranFunGroup::color_cb( Fl_Widget *w, void* g)
{
	//Fl_Color_Chooser *c = (Fl_Color_Chooser*)w;
	Flmm_ColorA_Chooser *c = (Flmm_ColorA_Chooser*)w;
	psTranFunGroup *d = (psTranFunGroup*)g;
	psTranFunActiveX *activeX = d->activeX;

	keynode* n = static_cast<keynode*>(d->node);
	int f = d->focusnode;
	if(f != -1)
	{
		n[f].R=c->r();
		n[f].G=c->g();
		n[f].B=c->b();
		n[f].opac=c->a();// for alpha [8/9/2006 tangwei]
		activeX->redraw();
	}
}

void psTranFunGroup::setbt_cb( Fl_Widget *w, void* g)
{
	psTranFunGroup *d = (psTranFunGroup*)g;
	RayCastViewPort *vp = d->vp;
	psVolumeMap *vm = d->VolumeMap;
	vm->setmap();
	vp->resetTexture(node, 7);
	vp->redraw();
}