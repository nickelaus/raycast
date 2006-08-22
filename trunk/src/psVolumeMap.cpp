#include "psVolumeMap.h"
#include <fstream>
#include <iostream>
psVolumeMap::psVolumeMap(int x, int y, int z):_x(x),_y(y),_z(z)
{
	nodecount = 7;
	node = new keynode[nodecount];
	data = new char[_x*_y*_z];

	node[0].gray = 0;
	node[0].opac = 0.0;
	node[0].R = 0.0;
	node[0].G = 0.0;
	node[0].B = 0.0;

	node[1].gray = 30.0;
	node[1].opac = 0.0;
	node[1].R = 0.0;
	node[1].G = 0.9;
	node[1].B = 0.3;

	node[2].gray = 80.0;
	node[2].opac = 0.1;
	node[2].R = 0.5;
	node[2].G = 0.0;
	node[2].B = 0.0;
/*
	node[3].gray = 180.0;
	node[3].opac = 0.2;
	node[3].R = 0.0;
	node[3].G = 0.0;
	node[3].B = 1.0;

	node[4].gray = 200.0;
	node[4].opac = 0.4;
	node[4].R = 1.0;
	node[4].G = 1.0;
	node[4].B = 0.0;

	node[5].gray = 250.0;
	node[5].opac = 0.4;
	node[5].R = 1.0;
	node[5].G = 0.0;
	node[5].B = 1.0;*/
	for (int i =3; i<=5 ;i++)
	{
		node[i].gray = 255.0;
		node[i].opac = 1;
		node[i].R = 1.0;
		node[i].G = 1;
		node[i].B =1;

	}

	node[6].gray = 255;
	node[6].opac = 1.0;
	node[6].R = 1.0;
	node[6].G = 1.0;
	node[6].B = 1.0;
}

psVolumeMap::psVolumeMap(const char* filename,int x,int y,int z):_x(x),_y(y),_z(z)
{
	nodecount = 7;
	node = new keynode[nodecount];
	data = new char[_x*_y*_z];

	node[0].gray = 0;
	node[0].opac = 0.0;
	node[0].R = 0.0;
	node[0].G = 0.0;
	node[0].B = 0.0;

	node[1].gray = 30.0;
	node[1].opac = 0.0;
	node[1].R = 0.0;
	node[1].G = 0.9;
	node[1].B = 0.3;

	node[2].gray = 80.0;
	node[2].opac = 0.1;
	node[2].R = 0.5;
	node[2].G = 0.0;
	node[2].B = 0.0;
	for (int i =3; i<=5 ;i++)
	{
		node[i].gray = 255.0;
		node[i].opac = 1;
		node[i].R = 1.0;
		node[i].G = 1;
		node[i].B =1;

	}

	node[6].gray = 255;
	node[6].opac = 1.0;
	node[6].R = 1.0;
	node[6].G = 1.0;
	node[6].B = 1.0;

	loadfile(filename);
	setmap();
}

psVolumeMap::~psVolumeMap(void)
{
	delete[] node;
	delete[] data;
}

void psVolumeMap::loadfile(const char* filename) 
{



	std::ifstream file;
	file.open(filename, std::ios::in | std::ios::binary);
	if(!file) 
	{
		std::cout << "** FATAL ERROR: Can't open volume\n";
		exit(-1);
	}

	// get file length:
	file.seekg(0,std::ios::end);
	unsigned long L = file.tellg();
	file.seekg(std::ios::beg);

	if (_x*_y*_z!=L) 
	{
		std::cout << file << " is not a volume";
		exit(-1);
	}
	
	file.read(data, L);	
	return ;
}

void psVolumeMap::setmap()
{
	for(int z=0;z<_z;z++)
		for(int y=0;y<_y;y++)
			for(int x=0;x<_x;x++)
			{
				unsigned char scale=*(data+256*256*z+256*y+x);


				for(int i = 0 ; i<nodecount-1 ; i++ )
				{
					if(scale>=node[i].gray&&scale<node[i+1].gray)
					{
						tex3Array[z][y][x][0]=static_cast<unsigned char>(((node[i+1].R-node[i].R)/(node[i+1].gray-node[i].gray)*(scale - node[i].gray)+node[i].R)*256);
						tex3Array[z][y][x][1]=static_cast<unsigned char>(((node[i+1].G-node[i].G)/(node[i+1].gray-node[i].gray)*(scale - node[i].gray)+node[i].G)*256);
						tex3Array[z][y][x][2]=static_cast<unsigned char>(((node[i+1].B-node[i].B)/(node[i+1].gray-node[i].gray)*(scale - node[i].gray)+node[i].B)*256);
						tex3Array[z][y][x][3]=static_cast<unsigned char>(((node[i+1].opac-node[i].opac)/(node[i+1].gray-node[i].gray)*(scale - node[i].gray)+node[i].opac)*256);
					}

				}





			}
			for(int z=0;z<_z/8;z++)
				for(int y=0;y<_y/8;y++)
					for(int x=0;x<_x/8;x++)
					{
						GLubyte sum=0;
						for(int c=0;c<8;c++)
							for(int b=0;b<8;b++)
								for(int a=0;a<8;a++)
									if(tex3Array[z*8+c][y*8+b][x*8+a][3]>0)
										sum=128;

						tex3Array8[z][y][x][3]=sum;
					}
}