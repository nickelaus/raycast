/********************************************************************
	created:	2006/08/08
	created:	8:8:2006   20:06
	filename: 	\FltkRayCast\src\psVolumeMap.h
	file path:	\FltkRayCast\src
	file base:	psVolumeMap
	file ext:	h
	author:		tangwei
	
	purpose:	construct the map of volume data 
*********************************************************************/
#ifndef _PSVOLUMEMAP_H
#define _PSVOLUMEMAP_H

#include <FL/GL.h>

struct keynode{
	GLfloat R;
	GLfloat G;
	GLfloat B;
	GLfloat gray;
	GLfloat opac;

};

class psVolumeMap
{
public:
	psVolumeMap(int x,int y,int z);
	psVolumeMap(const char* filename,int x,int y,int z);
	void loadfile(const char* filename);
	void setmap();
public:
	~psVolumeMap(void);

	keynode* node;
	int nodecount;

	char* data;
	int _x;
	int _y;
	int _z;

	GLfloat getMinValue()
	{
		return 0;
	}
	GLfloat getMaxValue()
	{
		return 255;
	}

	int GetDimensionX()
	{
		return _x;
	}
	int GetDimensionY()
	{
		return _y;

	}
	int GetDimensionZ()
	{
		return _z;
	}

	GLubyte tex3Array[256][256][256][4];
	GLubyte tex3Array8[32][32][32][4];

};

#endif