#include "RayCastViewPort.h"
#include <fstream>
#include <iostream>

#include <string.h>
#include <stdio.h>

RayCastViewPort::RayCastViewPort(int x, int y, int w, int h,psVolumeMap* vm, const char *label):Flu_GL_Canvas(x,y,w,h,label),VolumeMap(vm)
{
	
}

RayCastViewPort::~RayCastViewPort(void)
{
	glDeleteLists(drawSCubeList,1);// new add [8/10/2006 tangwei]
}

void RayCastViewPort::initList()
{
	drawSCubeList = glGenLists(1);
	glNewList(drawSCubeList,GL_COMPILE);
	glShadeModel(GL_SMOOTH);
	GLfloat a=5.0/2.0;
	GLfloat b=5.0/2.0;
	GLfloat c=5.0/2.0;
	glBegin(GL_QUADS);
	for(GLfloat i=0.0;i<1.001;i+=1.0/128.0)
	{
		glColor3f(1.0,1.0,1.0-i);
		glVertex3f(a,b,c-2*c*i);
		glColor3f(1.0,0.0,1.0-i);
		glVertex3f(a,-b,c-2*c*i);
		glColor3f(0.0,0.0,1.0-i);
		glVertex3f(-a,-b,c-2*c*i);
		glColor3f(0.0,1.0,1.0-i);
		glVertex3f(-a,b,c-2*c*i);


		glColor3f(1.0-i,1.0,1.0);
		glVertex3f(a-2*a*i,b,c);
		glColor3f(1.0-i,0.0,1.0);
		glVertex3f(a-2*a*i,-b,c);
		glColor3f(1.0-i,0.0,0.0);
		glVertex3f(a-2*a*i,-b,-c);
		glColor3f(1.0-i,1.0,0.0);
		glVertex3f(a-2*a*i,b,-c);

		glColor3f(1.0,1.0-i,1.0);
		glVertex3f(a,b-2*b*i,c);
		glColor3f(0.0,1.0-i,1.0);
		glVertex3f(-a,b-2*b*i,c);
		glColor3f(0.0,1.0-i,0.0);
		glVertex3f(-a,b-2*b*i,-c);
		glColor3f(1.0,1.0-i,0.0);
		glVertex3f(a,b-2*b*i,-c);
	}
	glEnd();
	glEndList();


}

void RayCastViewPort::init()
{
	camera[0] = 0.0f;
	camera[1] = 0.0f;
	camera[2] = 15.0f;

	cam[0] = cam[1] = cam[2] = 0.0f;

	lookat[0] = 0.0f;
	lookat[1] = 0.0f;
	lookat[2] = 1.0f;
	mousepos_x=0;
	mousepos_y=45;

	isRotate = false;

	initList();
	initTexture();

	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}

	setShaders();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

}
void RayCastViewPort::mouse_button_down( int button, int x, int y )
{
	switch(button)
	{
	case FL_LEFT_MOUSE:
	 
//		std::cout<<"x="<<x<<"  y="<<y<<std::endl;
		  tmpX = x;
		  tmpY = y;
		  isRotate = true;
	  
	  break;
	}
}
void RayCastViewPort::mouse_button_up( int button, int x, int y )
{
	switch(button)
	{
	case FL_LEFT_MOUSE:

		isRotate = false;

		break;
	}
}
void RayCastViewPort::mouse_drag( int x, int y )
{
	if(isRotate)
	{
		mousepos_y = (mousepos_y + (y - tmpY))%360;
		mousepos_x = (mousepos_x + (x - tmpX))%360;
		tmpX = x;
		tmpY = y;
		redraw();
	}
}
//void RayCastViewPort::keyboard( int key, int x, int y ) 
//{
//	//std::cout<<"key="<<(char)key<<"   x="<<x<<"  y="<<y<<std::endl;
//	switch(key)
//	{
//	case 'w':
//		std::cout<<"w";
//		camera[2]--;
//		redraw();
//		break;
//	case 's':
//		std::cout<<"s";
//		camera[2]++;
//		redraw();
//		break;
//	}
//}
void RayCastViewPort::mouse_wheel( int dx, int dy, int x, int y ) 
{
	camera[2]+=dy;
	redraw();
}
void RayCastViewPort::render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camera[0], camera[1], camera[2], lookat[0], lookat[1], lookat[2], 0, 1, 0);
	glTranslatef(cam[0],cam[1],cam[2]);

	glRotatef((GLfloat) mousepos_x, 0.0, 1.0, 0.0);// 旋转操作存在问题 [8/4/2006 tangwei]
	glRotatef((GLfloat) mousepos_y, 1.0, 0.0, 0.0);


	glUseProgram(NULL);



	//获取近面颜色纹理
	glEnable(GL_DEPTH_TEST);// for List Rend [8/3/2006 tangwei]
	glDepthFunc(GL_LESS);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCallList(drawSCubeList);
	//drawSCube(5.0,5.0,5.0,GL_LESS);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,0,0,w(),h(),0);

	//获取远面颜色纹理
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCallList(drawSCubeList);
	//drawSCube(5.0,5.0,5.0,GL_GREATER);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,0,0,w(),h(),0);

	/////////////////////////////////////////////////////////////////////////
	
	glUseProgram(p);

	glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w(), 0, -h());

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(w()/2.0,-h()/2.0, 0.0);
	//glRasterPos2f(0, 0);


	//////////////////////////////////////////////////////////////////////////

	//glClearColor(0.5,0.5,0.5,1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);


	glTexCoord2f(0.0,0.0);  glVertex2f(-w()/2.0,-h()/2.0);
	glTexCoord2f(1.0,0.0);  glVertex2f(w()/2.0,-h()/2.0);
	glTexCoord2f(1.0,1.0);  glVertex2f(w()/2.0,h()/2.0);
	glTexCoord2f(0.0,1.0);  glVertex2f(-w()/2.0,h()/2.0);

	glEnd();
	//////////////////////////////////////////////////////////////////////////

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	/////////////////////////////////////////////////////////////////////////
	
	//*********************
	//计算帧数
	/*
	static DWORD lasttime=0;
	static int frametimes=0;
	frametimes++;
	if(10==frametimes)
	{
	char s[200];
	sprintf(s,"Ray-Casting GLSL by 唐炜 ，Fps:%d",static_cast<int>(10.0/(GetTickCount()-lasttime)*1000));
	frametimes=0;
	lasttime=GetTickCount();
//	glutSetWindowTitle(s);
	this->window()->label(s);
	}
	*/
	//*********************
	

	
	swap_buffers();
}

void RayCastViewPort::reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

char* RayCastViewPort::textFileRead(char *fn) {


	char* optr;
	std::ifstream file;
	file.open(fn, std::ios::in | std::ios::binary);
	if(!file) 
	{
		std::cout << "** FATAL ERROR: Can't open shader files\n";
		exit(-1);
	}

	// get file length:
	file.seekg(0,std::ios::end);
	unsigned long L = file.tellg();
	file.seekg(std::ios::beg);


	optr =  new char[L];
	file.read(optr, L);	
	return optr;
}

//void RayCastViewPort::initTexture()
//{
//
//	//char* vol= loadVolume("..\\data\\aneurism.raw",256,256,256);
//	char* vol= loadVolume("..\\data\\skull.raw",256,256,256);
//	//char* vol = data ;
//	data = vol;
//	//count = 256*256*256;
//	//灰度直方图
//	//	static int hist[256];
//	//for(int z=0;z<256;z++)
//	//		for(int y=0;y<256;y++)
//	//			for(int x=0;x<256;x++)
//	//				hist[*(vol+256*256*z+256*y+x)]++;
//	int m=30;
//	for(int z=0;z<256;z++)
//		for(int y=0;y<256;y++)
//			for(int x=0;x<256;x++)
//			{
//				unsigned char scale=*(vol+256*256*z+256*y+x);
//				if(scale<40)
//				{
//					tex3Array[z][y][x][0]=0;
//					tex3Array[z][y][x][1]=0;
//					tex3Array[z][y][x][2]=0;
//					tex3Array[z][y][x][3]=0;
//				}
//				else if(scale<10)
//				{
//					tex3Array[z][y][x][0]=(unsigned char)scale*250.0/50.0;
//					tex3Array[z][y][x][1]=(unsigned char)scale*180.0/50.0;
//					tex3Array[z][y][x][2]=(unsigned char)scale*50.0/50.0;
//					tex3Array[z][y][x][3]=(unsigned char)scale*10.0/50.0;
//				}
//				else if(scale<=128)
//				{
//					tex3Array[z][y][x][0]=(unsigned char)scale*255.0/128.0;
//					tex3Array[z][y][x][1]=(unsigned char)scale*255.0/128.0;
//					tex3Array[z][y][x][2]=(unsigned char)scale*255.0/128.0;
//					tex3Array[z][y][x][3]=(unsigned char)scale*255.0/128.0;
//				}
//
//
//			}
//			for(int z=0;z<32;z++)
//				for(int y=0;y<32;y++)
//					for(int x=0;x<32;x++)
//					{
//						GLubyte sum=0;
//						for(int c=0;c<8;c++)
//							for(int b=0;b<8;b++)
//								for(int a=0;a<8;a++)
//									if(tex3Array[z*8+c][y*8+b][x*8+a][3]>0)
//										sum=128;
//
//						tex3Array8[z][y][x][3]=sum;
//						//tex3Array8[z][y][x][0]=tex3Array[z*8][y*8][x*8][0];
//						//tex3Array8[z][y][x][1]=tex3Array[z*8][y*8][x*8][1];
//						//tex3Array8[z][y][x][2]=tex3Array[z*8][y*8][x*8][2];
//
//
//					}
//					glGenTextures(4, texture);
//					PFNGLACTIVETEXTUREARBPROC glActiveTexture;
//					glActiveTexture=(PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTexture");
//
//					glActiveTexture(GL_TEXTURE0);
//					glBindTexture(GL_TEXTURE_2D, texture[0]);
//					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//
//					glActiveTexture(GL_TEXTURE1);
//					glBindTexture(GL_TEXTURE_2D, texture[1]);
//					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//
//					PFNGLTEXIMAGE3DPROC glTexImage3D;
//					glTexImage3D=(PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");
//					glActiveTexture(GL_TEXTURE2);
//					glBindTexture(GL_TEXTURE_3D, texture[2]);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//					glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,256,256,256,0,GL_RGBA,GL_UNSIGNED_BYTE,tex3Array);
//
//					glActiveTexture(GL_TEXTURE3);
//					glBindTexture(GL_TEXTURE_3D, texture[3]);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//					glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,32,32,32,0,GL_RGBA,GL_UNSIGNED_BYTE,tex3Array8);
//
//}

void RayCastViewPort::initTexture()
{

	glGenTextures(4, texture);
	PFNGLACTIVETEXTUREARBPROC glActiveTexture;
	glActiveTexture=(PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTexture");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	PFNGLTEXIMAGE3DPROC glTexImage3D;
	glTexImage3D=(PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, texture[2]);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,256,256,256,0,GL_RGBA,GL_UNSIGNED_BYTE,VolumeMap->tex3Array);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, texture[3]);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,32,32,32,0,GL_RGBA,GL_UNSIGNED_BYTE,VolumeMap->tex3Array8);

}
void RayCastViewPort::resetTexture(keynode* n , int c)
{		
	PFNGLTEXIMAGE3DPROC glTexImage3D;
	glTexImage3D=(PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, texture[2]);
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,256,256,256,0,GL_RGBA,GL_UNSIGNED_BYTE,VolumeMap->tex3Array);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, texture[3]);
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,32,32,32,0,GL_RGBA,GL_UNSIGNED_BYTE,VolumeMap->tex3Array8);

}
void RayCastViewPort::printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

void RayCastViewPort::printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

void RayCastViewPort::setShaders() {

	char *vs = NULL,*fs = NULL,*fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	vs = textFileRead("..\\glsl\\texture.vert");
	fs = textFileRead("..\\glsl\\texture.frag");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);

	printShaderInfoLog(v);
	printShaderInfoLog(f);

	p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);

	glLinkProgram(p);
	printProgramInfoLog(p);

	glUseProgram(p);

	GLint loc;

	loc = glGetUniformLocation(p,"myTexture1");
	glUniform1i(loc,0);
	loc = glGetUniformLocation(p,"myTexture2");
	glUniform1i(loc,1);
	loc = glGetUniformLocation(p,"myTexture3");
	glUniform1i(loc,2);
	loc = glGetUniformLocation(p,"myTexture4");
	glUniform1i(loc,3);



}