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


inline void matMVec22(double ** mat,double * vec,double* vecS){
	vecS[0] = mat[0][0]*vec[0] + mat[0][1]*vec[1];
	vecS[1] = mat[1][0]*vec[0] + mat[1][1]*vec[1];
}

