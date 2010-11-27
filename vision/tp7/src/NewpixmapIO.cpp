#include <stdlib.h>
#include <stdio.h>
#include "utils.hpp"
#include "NewpixmapIO.hpp"


//------------------------------------------------------------------------
// FONCTION TO READ AN IMAGE FILE
//------------------------------------------------------------------------

int * readPixmap(char* filename,
			   int * type,
			   int * nbColumns,
			   int * nbRows)
{
  FILE * filePtr;
  int* pixmap;
  int ich1, ich2;
  int rows, cols ;
  int maxVal;
  int i,j,k;

  /* Additional varaibles */
  int read8Bit;
  int singleBit;
  int mask;


  /* File opening */
  filePtr = fopen(filename,"rb");

  /* Magic number reading */
  ich1 = getc( filePtr );
  if ( ich1 == EOF )
    pm_erreur( "EOF / read error reading magic number" );
  ich2 = getc( filePtr );
  if ( ich2 == EOF )
    pm_erreur( "EOF / read error reading magic number" );

  switch(ich2)
    {
    default:
      fclose(filePtr);
      printf("File format not supported\n");
      exit(1);
      break;

    case '1':
      *type = 1;

      /* Reading dimensions */
      cols = pm_getint( filePtr );
      rows = pm_getint( filePtr );
      /* Passing number of columns and number of rows as arguments */
      *nbColumns = cols;
      *nbRows = rows;

      /* Memory allocation */
      pixmap = (int *) malloc(cols * rows * sizeof(int));

      /* Reading data */
      for(i=0; i < rows; i++)
	for(j=0; j < cols ; j++)
	  pixmap[i * cols + j] = pm_getbit(filePtr);
      break;

    case '2':
      *type = 2;
      /* Reading dimensions */
      cols = pm_getint( filePtr );
      rows = pm_getint( filePtr );
      maxVal = pm_getint( filePtr );
      /* Passing number of columns and number of rows as arguments */
      *nbColumns = cols;
      *nbRows = rows;

      /* Memory allocation */
      pixmap = (int *) malloc(cols * rows * sizeof(int));

      /* Reading data */
      for(i=0; i < rows; i++)
	for(j=0; j < cols ; j++)
	  pixmap[i * cols + j] = (int) pm_getint(filePtr);
      break;


    case '3':
      *type = 3;

      /* Reading dimensions */
      cols = pm_getint( filePtr );
      rows = pm_getint( filePtr );
      maxVal = pm_getint( filePtr );
      /* Passing number of columns and number of rows as arguments */
      *nbColumns = cols;
      *nbRows = rows;
      printf("%d %d \n", cols, rows);
      printf("%d \n", maxVal);
      /* Memory allocation */
      pixmap = (int *) malloc(cols * rows * 3 * sizeof(int));

      /* Reading data */

      for(i=0; i < rows; i++)
	for(j=0; j < cols ; j++)
	  for(k=0 ; k < 3 ; k++)
	    pixmap[i * cols * 3 + j * 3 + k] = (int) pm_getint(filePtr);

      break;




    case '4':
      *type = 4;
      printf("Type: 4\n");
      /* Reading dimensions */
      cols = pm_getint( filePtr );
      rows = pm_getint( filePtr );

      /* Passing number of columns and number of rows as arguments */
      *nbColumns = cols;
      *nbRows = rows;
      printf("cols: %d, rows: %d\n", cols, rows);

      /* Memory allocation */
      pixmap = (int *) malloc(rows * cols * sizeof(int));

      /* Reading data  bit by bit... tough !*/
      /* http://orion.math.iastate.edu/burkardt/g_src/pbmpak/pbmpak.c */

      for(j=0; j < rows; j++) {
	for(i=0; i < cols ; i++) {
	  if (i%8 == 0) {
	    read8Bit = fgetc(filePtr);

	    if (read8Bit == EOF)
	      pm_erreur("EOF / read error, file dimension is not a power of 2");

	  }

	  mask = 7 - i%8;
	  singleBit = ( read8Bit >> mask )%2;

	  pixmap[j * cols + i] = (int) singleBit;
	}
      }

      break;

    case '5':
      *type = 5;
      /* Reading dimensions */
      cols = pm_getint( filePtr );
      rows = pm_getint( filePtr );
      maxVal = pm_getint( filePtr );
      /* Passing number of columns and number of rows as arguments */
      *nbColumns = cols;
      *nbRows = rows;

      /* Memory allocation */
      pixmap = (int *) malloc(cols * rows * sizeof(int));

      /* Reading data */
      for(i=0; i < rows; i++)
	for(j=0; j < cols ; j++)
	  pixmap[i * cols + j] = pm_getrawbyte(filePtr);

      break;

    case '6':
      *type = 6;
      /* Reading dimensions */
      cols = pm_getint( filePtr );
      rows = pm_getint( filePtr );
      maxVal = pm_getint( filePtr );
      /* Passing number of columns and number of rows as arguments */
      *nbColumns = cols;
      *nbRows = rows;

      /* Memory allocation */
      pixmap = (int *) malloc(cols * rows * 3 * sizeof(int));

      /* Reading data */
      for(i=0; i < rows; i++)
	for(j=0; j < cols ; j++)
	  for(k=0 ; k < 3 ; k++)
	    pixmap[i * cols * 3 + j * 3 + k] =  pm_getrawbyte(filePtr);
      break;

    }

  fclose(filePtr);
  return pixmap;
}


//------------------------------------------------------------------------
// FONCTION TO WRITE AN IMAGE FILE
//------------------------------------------------------------------------

int writePixmap(int * pixmap,
		 int cols, int rows,
		 int type,
		 char * filename)
{


  int i, j, k;
  FILE * filePtr;

  /* Additional variables for binary file writing*/
  int bit;
  unsigned char c;
  int blackOrWhite;
  int mask;



  /* Provides the right extension */
  char * extension =  (char *) malloc(5*sizeof(char));
  if ( (type == 1) || (type == 4))
    extension = "pbm";
  else if ( (type == 2) || (type == 5) )
    extension = "pgm";
  else if ( (type == 3) || (type == 6) )
    extension = "ppm";
  else
    extension = 0;
  pm_setExtension(filename, extension);


  filePtr = fopen(filename, "wb");

  switch (type)
    {
    default:
      fclose(filePtr);
      pm_erreur("File type not supported yet...\n");
      break;

    case 1:
      fprintf(filePtr, "P1\n");
      fprintf(filePtr, "%d %d \n", cols, rows);

      for (j = 0; j < rows; j++) {
	for (i = 0; i < cols; i++) {
	  if (pixmap[j*cols + i] > 0)
	    fprintf(filePtr, "1\n");
	  else
	    fprintf(filePtr, "0\n");
	}
      }
      fprintf(filePtr, "\n");

      break;

    case 2:
      fprintf(filePtr, "P2\n");
      fprintf(filePtr, "%d %d \n", cols, rows);
      fprintf(filePtr, "%u\n", (unsigned char) pm_getMax(pixmap, cols * rows));

      for (j = 0; j < rows; j++)
	for (i = 0; i < cols; i++)
	  fprintf(filePtr, "%u\n", (unsigned char) pixmap[j*cols + i]);
      fprintf(filePtr, "\n");

      break;


      case 3:

	fprintf(filePtr, "P3\n");
	fprintf(filePtr, "%d %d \n", cols, rows);

	fprintf(filePtr, "%u\n", (unsigned char) pm_getMax(pixmap, cols * rows * 3));
	for (j = 0; j < rows; j++)
	  for (i = 0; i < cols; i++)
	    for (k = 0 ; k != 3 ; k++)
	      fprintf(filePtr, "%u\n", (unsigned char) pixmap[j*cols * 3+ i*3 + k]);
	fprintf(filePtr, "\n");

      break;



    case 4:
      /* Tough ! see http://orion.math.iastate.edu/burkardt/g_src/pbmpak/pbmpak.c */
      fprintf(filePtr, "P4\n");
      fprintf(filePtr, "%d %d \n", cols, rows);

      c = 0;
      for (j = 0; j < rows; j++) {
	for (i = 0; i < cols; i++) {
	  if (pixmap[j*cols + i] > 0)
	    blackOrWhite = 1;
	  else
	    blackOrWhite = 0;

	  mask = 7 - i%8;

	  bit = blackOrWhite % 2;
	  c = c | (bit << mask);

	  if ( (i+1)%8 == 0 || i == (cols - 1) ) {
	    fputc(c, filePtr);
	    c = 0;
	  }

	}
      }
      fprintf(filePtr, "\n");

      break;

    case 5:
      fprintf(filePtr, "P5\n");
      fprintf(filePtr, "%d %d \n", cols, rows);
      fprintf(filePtr, "%u\n", (unsigned char) pm_getMax(pixmap, cols * rows));

      for (j = 0; j < rows; j++)
	for (i = 0; i < cols; i++)
	  fputc((unsigned char) pixmap[j*cols + i], filePtr);
      fprintf(filePtr, "\n");

      break;


    case 6:
      fprintf(filePtr, "P6\n");
      fprintf(filePtr, "%d %d \n", cols, rows);
      fprintf(filePtr, "%u\n", (unsigned char) pm_getMax(pixmap, cols * rows *3));

      for (j = 0; j < rows; j++)
	for (i = 0; i < cols; i++)
	  for(k=0; k !=3 ; k++)
	    fputc((unsigned char) pixmap[j*cols*3 + i*3 +k], filePtr);
      fprintf(filePtr, "\n");

      break;

    }

  fclose(filePtr);
  return 0;
}





/* Algo de bresenham */
void Bresenham(int Ax, int Ay, int Bx, int By,
	       int Color,
	       int WIDTH, int *FrameBuffer)
{
  int dX, dY, fbXincr, fbYincr, fbXYincr, dPr, dPru, P;

  //------------------------------------------------------------------------
  // STORE THE FRAMEBUFFER ENDPOINT-ADDRESSES (A AND B)
  //------------------------------------------------------------------------
  int * AfbAddr = &FrameBuffer[(Ay*WIDTH+Ax) *3];
  int * BfbAddr = &FrameBuffer[(By*WIDTH+Bx)*3];

  //------------------------------------------------------------------------
  // DETERMINE AMOUNT TO INCREMENT FRAMEBUFFER TO GET TO SUBSEQUENT POINTS
  // (ALSO, STORE THE ABSOLUTE VALUE OF THE CHANGE IN X AND Y FOR THE LINE)
  //------------------------------------------------------------------------
  fbXincr=3;
  if ( (dX=Bx-Ax) >= 0) goto AFTERNEGX;
    dX=-dX;
    fbXincr=-3;
  AFTERNEGX:

  fbYincr=3 * WIDTH;
  if ( (dY=By-Ay) >= 0) goto AFTERNEGY;
    fbYincr=-3 * WIDTH;
    dY=-dY;
  AFTERNEGY:

  fbXYincr = fbXincr+fbYincr;

  //------------------------------------------------------------------------
  // DETERMINE INDEPENDENT VARIABLE (ONE THAT ALWAYS INCREMENTS BY 1 (OR -1) )
  // AND INITIATE APPROPRIATE LINE DRAWING ROUTINE (BASED ON FIRST OCTANT
  // ALWAYS). THE X AND Y'S MAY BE FLIPPED IF Y IS THE INDEPENDENT VARIABLE.
  //------------------------------------------------------------------------
  if (dY > dX) goto YisIndependent;        // CHECK IF X OR Y IS INDEPENDENT VARIABLE

  // XisIndependent:
    dPr = dY+dY;                           // AMOUNT TO INCREMENT DECISION IF RIGHT IS CHOSEN (always)
    P = -dX;                               // DECISION VARIABLE START VALUE
    dPru = P+P;                            // AMOUNT TO INCREMENT DECISION IF UP IS CHOSEN
    dY = dX>>1;                            // COUNTER FOR HALF OF LINE (COMING FROM BOTH ENDS)
    XLOOP:                                 // PROCESS EACH POINT IN THE LINE ONE AT A TIME (use dY as loop counter)
        *AfbAddr=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+2)=Color;                    // PLOT THE PIXEL FROM END A
        *BfbAddr=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+2)=Color;                    // PLOT THE PIXEL FROM END B
        if ((P+=dPr) > 0) goto RightAndUp; // INCREMENT DECISION, CHECK IF THE PIXEL IS GOING RIGHT AND UP
        // Up:
            AfbAddr+=fbXincr;              // ADVANCE TO NEXT POINT FROM END A
            BfbAddr-=fbXincr;              // ADVANCE TO NEXT POINT FROM END B
            if ((dY=dY-1) > 0) goto XLOOP; // DECREMENT LOOP VARIABLE AND LOOP
            *AfbAddr=Color;                // (FIX MIDPOINT PROBLEM) PLOT LAST PT FROM END A
            *(AfbAddr+1)=Color;
            *(AfbAddr+2)=Color;

            if ((dX & 1) == 0) return;     // FINISHED IF INDEPENDENT IS EVEN (ODD # STEPS)
            *BfbAddr=Color;                // PLOT LAST PT FROM END B IF INDEPENDENT
            *(BfbAddr+1)=Color;
            *(BfbAddr+2)=Color;
            return;
        RightAndUp:
            AfbAddr+=fbXYincr;             // ADVANCE TO NEXT POINT FROM END A
            BfbAddr-=fbXYincr;             // ADVANCE TO NEXT POINT FROM END B
            P+=dPru;                       // INCREMENT DECISION (for up)
            if ((dY=dY-1) > 0) goto XLOOP; // DECREMENT LOOP VARIABLE AND LOOP
        *AfbAddr=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+2)=Color;
            if ((dX & 1) == 0) return;     // FINISHED IF INDEPENDENT IS EVEN (ODD # STEPS)
     *BfbAddr=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+2)=Color;
            return;

  YisIndependent:
    dPr = dX+dX;                           // AMOUNT TO INCREMENT DECISION IF RIGHT IS CHOSEN (always)
    P = -dY;                               // DECISION VARIABLE START VALUE
    dPru = P+P;                            // AMOUNT TO INCREMENT DECISION IF UP IS CHOSEN
    dX = dY>>1;                            // COUNTER FOR HALF OF LINE (COMING FROM BOTH ENDS)
    YLOOP:                                 // PROCESS EACH POINT IN THE LINE ONE AT A TIME (use dX as loop counter)
       *AfbAddr=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+2)=Color;
      *BfbAddr=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+2)=Color;
        if ((P+=dPr) > 0) goto RightAndUp2;// INCREMENT DECISION, CHECK IF THE PIXEL IS GOING RIGHT AND UP
        // Up:
            AfbAddr+=fbYincr;              // ADVANCE TO NEXT POINT FROM END A
            BfbAddr-=fbYincr;              // ADVANCE TO NEXT POINT FROM END B
            if ((dX=dX-1) > 0) goto YLOOP; // DECREMENT LOOP VARIABLE AND LOOP
       *AfbAddr=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+2)=Color;
            if ((dY & 1) == 0) return;     // FINISHED IF INDEPENDENT IS EVEN (ODD # STEPS)
      *BfbAddr=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+2)=Color;
            return;
        RightAndUp2:
             AfbAddr+=fbXYincr;            // ADVANCE TO NEXT POINT FROM END A
             BfbAddr-=fbXYincr;            // ADVANCE TO NEXT POINT FROM END B
            P+=dPru;                       // INCREMENT DECISION (for up)
            if ((dX=dX-1) > 0) goto YLOOP; // DECREMENT LOOP VARIABLE AND LOOP
       *AfbAddr=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END A
        *(AfbAddr+2)=Color;
            if ((dY & 1) == 0) return;     // FINISHED IF INDEPENDENT IS EVEN (ODD # STEPS)
      *BfbAddr=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+1)=Color;                    // PLOT THE PIXEL FROM END B
        *(BfbAddr+2)=Color;
            return;
}
