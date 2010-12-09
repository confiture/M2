int * readPixmap(char* filename,
		 int * type,
		 int * nbColumns,
		 int * nbRows);

int writePixmap(int * pixmap,
		int cols, int rows,
		int type,
		char * filename);

void Bresenham(int Ax, 
	       int Ay, 
	       int Bx, 
	       int By, 
	       int Color, 
	       int width, 
	       int *FrameBuffer);
