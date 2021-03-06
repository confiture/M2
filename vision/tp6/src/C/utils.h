/******************************************************************
 *              File: util.h                                      *
 *  some utility function to read/write        *
 ******************************************************************/

/*****************************************************************
 *        To read char, bit, rawbyte and int from a file         *
 *****************************************************************/
char pm_getc(FILE* file);
unsigned char pm_getbit(FILE* file);
unsigned char pm_getrawbyte(FILE* file);
int pm_getint( FILE* file);
int pm_get_signed_int( FILE* file);
double pm_get_signed_double( FILE* file);
void pm_erreur(char *);
int pm_getMax(int * pixmap, int nbPixels);
int pm_getMin(int * pixmap, int nbPixels);
double pm_getDBLMax(double * pixmap, int nbPixels);
double pm_getDBLMin(double * pixmap, int nbPixels);
void pm_setExtension(char * filename, char *ext);
