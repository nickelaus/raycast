#include "psTranFunActiveX.h"
#include <iostream>
#include <fstream>
psTranFunActiveX::psTranFunActiveX(int x, int y, int w, int h,int &f ,psVolumeMap* vm ,const char *label)
:Fl_Gl_Window(x,y,w,h,label),_firstDraw(true),node(vm->node),nodecount(vm->nodecount),focusnode(f),VolumeMap(vm)
{
	mode(FL_DOUBLE );
	range_Min=vm->getMinValue();
	range_Max=vm->getMaxValue();

    count = vm->GetDimensionX()*vm->GetDimensionY()*vm->GetDimensionZ();

	data = (unsigned char*)vm->data;

	step = 1;
	view_Max = range_Max;
	view_Min = range_Min;


	hist();

}
psTranFunActiveX::~psTranFunActiveX(void)
{
	gluDeleteQuadric(qobj);// new add [8/10/2006 tangwei]

}

void psTranFunActiveX::draw() {
	
	if( _firstDraw )
	{
		_firstDraw = false;
	
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glShadeModel (GL_SMOOTH);

		GLfloat a=0.5,b=1,c=0.7;
		GLfloat light_position[] = { 5.0,5.0, 10.0, 0.0 };
		GLfloat light_ambient[] = { a, a, a, 1.0};
		GLfloat light_specular[] = { b, b, b, 1.0 };
		GLfloat light_diffuse[] = { c, c, c, 1.0};

		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
		glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);


		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
	

		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj,GLU_FILL);
		gluQuadricNormals(qobj,GLU_SMOOTH);
		
	}

	if (!valid()) 
	{
		valid(1);
		glLoadIdentity();
		glViewport(0,0,w(),h());

	}

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(view_Min,view_Max,0,(view_Max-view_Min)*h()/w(),-100,100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	for (int i=0;i<hcount;i++)
	{
		glBegin(GL_LINES);
			glVertex3f(i*step,(GLfloat)histogram[i]/(GLfloat)hmax*(view_Max-view_Min)*h()/w(),0);
			glVertex3f(i*step,0,0);
		glEnd();
	}

	for(int i=0;i<nodecount-1;i++)
	{
		glPushMatrix();
		glTranslatef(node[i].gray,node[i].opac*(view_Max-view_Min)*h()/w(),0);

		GLfloat mat_ambient[] = {node[i].R, node[i].G, node[i].B, 0.0 };
		GLfloat mat_specular[] = {node[i].R, node[i].G, node[i].B, 0.0 };
		GLfloat mat_shininess[] = { 50.0 };
		glMaterialfv(GL_FRONT, GL_SPECULAR,mat_ambient);
		glMaterialfv(GL_FRONT, GL_AMBIENT,mat_ambient);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glutSolidSphere((view_Max-view_Min)*0.01*(focusnode==i?1.5:1.0),80,60);
		glPushMatrix();
		glRotatef(90,0.0,1.0,0.0);
		float angle = atan((node[i+1].opac-node[i].opac)*(view_Max-view_Min)*h()/w()/(node[i+1].gray-node[i].gray))*180/3.14159*(-1);
		float length = sqrtf(pow((node[i+1].opac-node[i].opac)*(view_Max-view_Min)*h()/w(),2)+pow((node[i+1].gray-node[i].gray),2));
		glRotatef(angle,1.0,0.0,0.0);
		GLfloat line_color[] = { 0.0, 0.0, 1.0, 0.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT,line_color);
		gluCylinder(qobj,(view_Max-view_Min)*0.002,(view_Max-view_Min)*0.002,length,80,60);
		glPopMatrix();
		glPopMatrix();
	}
	glTranslatef(node[nodecount-1].gray,node[nodecount-1].opac*(view_Max-view_Min)*h()/w(),0);

	GLfloat mat_ambient[] = {node[nodecount-1].R, node[nodecount-1].G, node[nodecount-1].B, 0.0 };
	GLfloat mat_specular[] = {node[nodecount-1].R, node[nodecount-1].G, node[nodecount-1].B, 0.0 };
	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR,mat_ambient);
	glMaterialfv(GL_FRONT, GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glutSolidSphere((view_Max-view_Min)*0.01*(focusnode==nodecount-1?1.5:1.0),80,60);
}

void psTranFunActiveX::draw_overlay() 
{
	if (!valid()) 
	{
		valid(1);
		glLoadIdentity();
		glViewport(0,0,w(),h());		
	}
}

int psTranFunActiveX :: handle( int event )
{
	switch( event )
	{
	case FL_PUSH:
		for(int i = 0; i < nodecount ;i++)
		{
			if( Fl::event_inside( worldpos2viewpos(node[i].gray,node[i].opac).x-10, worldpos2viewpos(node[i].gray,node[i].opac).y-10, 20, 20) )
			{
				focusnode=i;				
				redraw();
				do_callback();
				return 1;
			}
		}
		focusnode= -1;
		redraw();
		do_callback();
		return 1;
		break;

	case FL_DRAG:
		{
			if (focusnode!=-1)
			{
				node[focusnode].gray=viewpos2worldpos(Fl::event_x(),Fl::event_y()).x;
				node[focusnode].opac=viewpos2worldpos(Fl::event_x(),Fl::event_y()).y;
				if (node[focusnode].gray<=node[focusnode-1].gray)
				{
					node[focusnode].gray=node[focusnode-1].gray+1;
				}
				if (node[focusnode].gray>=node[focusnode+1].gray)
				{
					node[focusnode].gray=node[focusnode+1].gray-1;
				}
				if (node[focusnode].opac<=0)
				{
					node[focusnode].opac=0;
				}
				if (node[focusnode].opac>=1)
				{
					node[focusnode].opac=1;
				}
				if(focusnode == 0)
					node[focusnode].gray=range_Min;
				if(focusnode == 6)
					node[focusnode].gray=range_Max;

				redraw();
				do_callback();// for alpha [8/9/2006 tangwei]
				return 1;
			}
			
		}
		break;

	}
	return Fl_Gl_Window::handle(event);
}

void psTranFunActiveX::hist()  
{
	hcount=static_cast<int>((range_Max-range_Min)/step)+1;
	histogram = new int[hcount];

	for (int i= 0 ; i<hcount; i++)
	{
		histogram[i]=0;
	}

	for (int i = 0 ; i<count; i++)
	{
		histogram[static_cast<int>((static_cast<GLfloat>(data[i])-range_Min)/step)]++;
	}
	hmax=0;
	for (int i = 1 ; i<hcount -1 ; i++)  // 避免两端超大值影响 [8/6/2006 tangwei]
	{
		if (histogram[i]>hmax)
		{
			hmax = histogram[i];
		}
	}
	
		
}

