#include <FL/Fl.H>
#include <fl/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include "RayCastViewPort.h"
#include "psTranFunGroup.h"
#include "psVolumeMap.h"

int main(int argc, char **argv) {

	Fl_Double_Window *win = new Fl_Double_Window(1200, 600);
	psVolumeMap *vm = new psVolumeMap("..\\data\\skull.raw",256,256,256);
	//psVolumeMap *vm = new psVolumeMap(256,256,256);
	//vm->loadfile("..\\data\\aneurism.raw");
	//vm->setmap();

	RayCastViewPort* vp = new RayCastViewPort(10,10,500,500,vm);
	psTranFunGroup* temp =new psTranFunGroup(520,10,600,300,vp,vm);

	win->end();
	win->show();

	return Fl::run();
}