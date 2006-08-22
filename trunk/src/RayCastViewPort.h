/********************************************************************
	created:	2006/08/08
	created:	8:8:2006   20:04
	filename: 	\FltkRayCast\src\RayCastViewPort.h
	file path:	\FltkRayCast\src
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