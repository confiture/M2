typedef unsigned char bit;

int pm_getint( FILE* file);
char pm_getc(FILE* file);
void pm_erreur(char *);
unsigned char pm_getrawbyte(FILE* file);

float absf(float);
double absd(double);
float max(float,float);
float mod2PI(float);
double medianTab(double* tab_in,int dim);
double** invMat22(double** mat);
