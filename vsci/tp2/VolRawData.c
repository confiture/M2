/************************************************************************
 *
 * Visualisation de données volumiques
 * Fichier de données de type RAW :
 *   format 1 : unsigned char
 *   format 2 : unsigned short
 * de dimensions (sx,sy,sz)
 * avec échelle hz suivant z
 *
 * Compilation : gcc VolRawData.c -lglut -lGLU -lGL
 * Exécution   : ./a.out filename sx sy sz hz format
 *
 ************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>    /* OpenGL Utility Toolkit header */

/* booleen pour l'affichage de la vitesse de rafraichissement */
int reportSpeed = 0;

/* booleen pour l'affichage d'info */
int reportInfo = 1;

/* facteur d'échelle global */
double zoom=1.0;

/* booleen indiquant le déplacement en cours de la souris */
int moving;

/* booleen pour le tracé de l'isocourbe */
int drawIsoCurve = 1;

/* position actuelle de la souris (utilisée lors d'un déplacement souris) */
int startx, starty;

/* tableau des données volumiques */
float *buffer_vol;

/* tableau utilisé pour l'image des slices */
unsigned short *TextureBuffer;

/* dimensions du volume */
int sx, sy, sz;

/* dimensions pour les textures */
int sx2, sy2, sz2;

/* échelle en z */
double hz = 1.0;

/* indices des positions des 3 slices */
int Ox, Oy, Oz;

/* isovaleur de la courbe */
float isoVal=0.5;
/* pas pour la modification de l'isovaleur */
float pasIsoVal=0.005;

/* type Point 3D */
typedef GLfloat GLPoint3[3];

// le type Point3
// les 3 coordonnées
typedef struct
{
	double x,y,z; // les 3 coordonnées
} Point3;

Point3 consPt(double x,double y,double z){
	Point3 p;
	p.x=x;
	p.y=y;
	p.z=z;
}

// le type Triangle = tableau de 3 double
// indices des 3 sommets du triangle
typedef struct
{
	Point3 p[3]; // les 3 coordonnées
} Triangle;

GLPoint3 XPlane[4]; /* coordonnées du bord de la coupe x constant */
GLPoint3 YPlane[4]; /* coordonnées du bord de la coupe y constant */
GLPoint3 ZPlane[4]; /* coordonnées du bord de la coupe z constant */

/* numéros des images des trois slices */
enum { OxTexture, OyTexture, OzTexture};


/*****************************************************************************/
/* Partie pour la gestion d'une traqueboule                                  */
/* (rotation de la scene à la souris)                                        */
/*****************************************************************************/

/****************************/
/* inversion p=inverse de m */
/****************************/
void tbInverse( const GLdouble *m, GLdouble *p )
{
	{   /* transposition de la rotation */
		int i,j;
		for(i=0; i<3; ++i )
			for(j=0; j<3; ++j )
				p[i+4*j] = m[j+4*i];
	}

	{   /* calcul de la translation */
		double q[3]={0,0,0};
		int i,j;
		for(i=0; i<3; ++i ){
			for(j=0; j<3; ++j )
				q[i] -= p[i+4*j]*m[12+j];
		}
		p[12]=q[0]; p[13]=q[1]; p[14]=q[2];
	}

	/* derniere ligne */
	p[3] = p[7] = p[11] = 0; p[15] = 1;
}

/* matrices de placement de la scene par rapport a la camera */
GLdouble tb_matrix[16] =   { 1,0,0,0,
	                        0,1,0,0,
							0,0,1,0,
							0,0,0,1  };
GLdouble tb_inverse[16] =  { 1,0,0,0,
	                        0,1,0,0,
							0,0,1,0,
							0,0,0,1  };

/* variables pour la gestion de la souris */
int tb_ancienX, tb_ancienY, tb_tournerXY=0, tb_translaterXY=0, tb_bougerZ=0;


/*********************************************************************/
/* lit dans la matrice courante la position initiale du point de vue */
/*********************************************************************/
void tbInitTransform()
{
	glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );
	tbInverse( tb_matrix, tb_inverse );
}

/**********************************************/
/* applique la transformation de point de vue */
/**********************************************/
void tbVisuTransform()
{
  glMultMatrixd( tb_matrix );
};

/************************************************/
/* callback de gestion des boutons de la souris */
/************************************************/
void tbMouseFunc( int button, int state, int x, int y )
{
  /* enfoncer gauche */
  if( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN ){
    tb_tournerXY = 1;
    tb_ancienX = x;
    tb_ancienY = y;
  }
  /* relacher gauche */
  else if( button==GLUT_LEFT_BUTTON && state==GLUT_UP ){
    tb_tournerXY = 0;
  }
  /* enfoncer milieu */
  if( button==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN ){
    tb_bougerZ = 1;
    tb_ancienX = x;
    tb_ancienY = y;
  }
  /* relacher milieu */
  else if( button==GLUT_MIDDLE_BUTTON && state==GLUT_UP ){
    tb_bougerZ = 0;
  }
}

/***************************************************/
/* callback de changement de position de la souris */
/***************************************************/
void tbMotionFunc( int x, int y )
{
	double dx,dy,nrm, tx,ty,tz;

	if( tb_tournerXY || tb_translaterXY || tb_bougerZ )
	{
		/* deplacement */
		dx = x - tb_ancienX;
		dy = tb_ancienY - y; /* axe vertical dans l'autre sens */

		if( tb_tournerXY ){
			tx = tb_matrix[12]; tb_matrix[12]=0;
			ty = tb_matrix[13]; tb_matrix[13]=0;
			tz = tb_matrix[14]; tb_matrix[14]=0;

			nrm = sqrt( dx*dx+dy*dy+dx*dx+dy*dy );
			glLoadIdentity();
			glRotatef( nrm, -dy, dx, 0 );/*axe perpendiculaire au deplacement*/
			glMultMatrixd( tb_matrix );
			glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );

			tb_matrix[12] = tx;
			tb_matrix[13] = ty;
			tb_matrix[14] = tz;
		}
		else if( tb_translaterXY ){
			tb_matrix[12] += dx/100.0;
			tb_matrix[13] += dy/100.0;
		}
		else if( fabs(dx)>fabs(dy) ){ /* rotation z */
			tx = tb_matrix[12]; tb_matrix[12]=0;
			ty = tb_matrix[13]; tb_matrix[13]=0;
			tz = tb_matrix[14]; tb_matrix[14]=0;

			glLoadIdentity();
			glRotatef( dx, 0,0,-1 );/*axe perpendiculaire a l'ecran*/
			glMultMatrixd( tb_matrix );
			glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );

			tb_matrix[12] = tx;
			tb_matrix[13] = ty;
			tb_matrix[14] = tz;
		}
		else if( fabs(dy)>fabs(dx) ){
			tb_matrix[14] -= dy/100.0;
		}
		tb_ancienX = x;
		tb_ancienY = y;
		tbInverse( tb_matrix, tb_inverse );
		glutPostRedisplay();
	}
}

/*****************************************************************************/
/* Fin de la partie pour la gestion d'une traqueboule                        */
/* (rotation de la scene à la souris)                                        */
/*****************************************************************************/

/********************************/
/* affichage d'un erreur OpenGL */
/********************************/
void AfficheCodeErreurGL(int Code)
{
  switch (Code) {
	case GL_NO_ERROR:
		fprintf( stderr, "GL_NO_ERROR\n");
		break;
	case GL_INVALID_ENUM:
		fprintf( stderr, "GL_INVALID_ENUM\n");
		break;
	case GL_INVALID_VALUE:
		fprintf( stderr, "GL_INVALID_VALUE\n");
		break;
	case GL_INVALID_OPERATION:
		fprintf( stderr, "GL_INVALID_OPERATION\n");
		break;
	case GL_STACK_OVERFLOW:
		fprintf( stderr, "GL_STACK_OVERFLOW\n");
		break;
	case GL_STACK_UNDERFLOW:
		fprintf( stderr, "GL_STACK_UNDERFLOW\n");
		break;
	case GL_OUT_OF_MEMORY:
		fprintf( stderr, "GL_OUT_OF_MEMORY\n");
		break;
	others:
		fprintf( stderr, "Code d'erreur GL inconnu\n");
		break;
	}
}


/*********************************************************************/
/* copie de la fenetre OpenGL dans le fichier filename au format PPM */
/*********************************************************************/
void CopieEcran(const char *filename)
{
	GLint viewport[4] ;
	int x, y, l;
	int width, height;
	unsigned int lineSize;
	unsigned char *data;
	FILE *fd;
	char buffer[256] ;

	/* récupération des position et dimensions de la fenetre */
	glGetIntegerv(GL_VIEWPORT, viewport) ;
	x=viewport[0];
	y=viewport[1];
	width=viewport[2];
	height=viewport[3] ;

	/* lecture de l'image dans data */
	lineSize = ((width*3+3)/4)*4 ;
	data = (unsigned char *)malloc(height*lineSize) ;
	glReadBuffer(GL_FRONT) ;
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data) ;


	/* création du fichier ppm correspondant */
	fd = fopen(filename, "w");
	fprintf(fd, "P6\n%d %d\n255\n", width, height) ;
	for (l=height-1; l>=0; --l)
	{
		unsigned char *ptr = data + l*lineSize ;
		fwrite(ptr, lineSize, 1, fd);
	}
	fclose(fd) ;
	free(data);
	fprintf(stderr, "Fichier %s créé\n", filename);
}


/**************************************/
/* coordonnées des bords des 3 slices */
/**************************************/
void BuildSlicePlanes(void)
{
  ZPlane[0][0] = 0.0;
  ZPlane[0][1] = 0.0;
  ZPlane[0][2] = Oz*hz;

  ZPlane[1][0] = sx-1;
  ZPlane[1][1] = 0.0;
  ZPlane[1][2] = Oz*hz;

  ZPlane[2][0] = sx-1;
  ZPlane[2][1] = sy-1;
  ZPlane[2][2] = Oz*hz;

  ZPlane[3][0] = 0.0;
  ZPlane[3][1] = sy-1;
  ZPlane[3][2] = Oz*hz;

/**/

  YPlane[0][0] = 0.0;
  YPlane[0][1] = Oy;
  YPlane[0][2] = 0.0*hz;

  YPlane[1][0] = sx-1;
  YPlane[1][1] = Oy;
  YPlane[1][2] = 0.0*hz;

  YPlane[2][0] = sx-1;
  YPlane[2][1] = Oy;
  YPlane[2][2] = (sz-1)*hz;

  YPlane[3][0] = 0.0;
  YPlane[3][1] = Oy-1;
  YPlane[3][2] = (sz-1)*hz;

/**/

  XPlane[0][0] = Ox;
  XPlane[0][1] = 0.0;
  XPlane[0][2] = 0.0*hz;

  XPlane[1][0] = Ox;
  XPlane[1][1] = sy-1;
  XPlane[1][2] = 0.0*hz;

  XPlane[2][0] = Ox;
  XPlane[2][1] = sy-1;
  XPlane[2][2] = (sz-1)*hz;

  XPlane[3][0] = Ox;
  XPlane[3][1] = 0.0;
  XPlane[3][2] = (sz-1)*hz;
}




/**********************************************/
/* calcul des images des trois slices         */
/* -- slices x=Ox constant : numéro OxTexture */
/* -- slices y=Oy constant : numéro OyTexture */
/* -- slices z=Oz constant : numéro OzTexture */
/**********************************************/
void makeSliceTexture(void)
{
  int i,j, Code;
  unsigned short *p;

  BuildSlicePlanes();

/* Oz */

  p = TextureBuffer;
  for (j=0; j<sy; j++) {
  for (i=0; i<sx; i++) {
      *p = (unsigned short)(65535.0*buffer_vol[i+j*sx+Oz*sx*sy]);
      p++;
    }
  }

  glBindTexture( GL_TEXTURE_2D, OzTexture);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, sx);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE16, sx2, sy2, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, TextureBuffer);
  Code = glGetError();

//  AfficheCodeErreurGL(Code);

/* Oy */

  p = TextureBuffer;
  for (j=0; j<sz; j++) {
  for (i=0; i<sx; i++) {
      *p = (unsigned short)(65535.0*buffer_vol[i+Oy*sx+j*sx*sy]);
      p++;
    }
  }

  glBindTexture( GL_TEXTURE_2D, OyTexture);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pour s'assurer que la carte graphique va chercher les données
                                         // au bon endroit dans le tableau

  glPixelStorei(GL_UNPACK_ROW_LENGTH, sx); // inutile si la largeur de l'image correspond aux lignes du tableau

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, sx2, sz2, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, TextureBuffer);
  Code = glGetError();

// AfficheCodeErreurGL(Code);

/* Ox */

  p = TextureBuffer;
  for (j=0; j<sz; j++) {
  for (i=0; i<sy; i++) {
      *p = (unsigned short)(65535.0*buffer_vol[Ox+i*sx+j*sx*sy]);
      p++;
    }
  }

  glBindTexture( GL_TEXTURE_2D, OxTexture);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, sy); // inutile si la largeur de l'image correspond aux lignes du tableau

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, sy2, sz2, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, TextureBuffer);

  Code = glGetError();

//  AfficheCodeErreurGL(Code);

}


/************************************************************/
/* dessin des slices : affichage des images correspondantes */
/************************************************************/
void drawSlices(void)
{
  float xmax, ymax, zmax;

  xmax = ((float)sx)/((float)sx2);
  ymax = ((float)sy)/((float)sy2);
  zmax = ((float)sz)/((float)sz2);

  glEnable(GL_TEXTURE_2D);
  glColor3f(1.,1.,1.);
  glBindTexture( GL_TEXTURE_2D, OzTexture);

  glBegin(GL_QUADS);
    glTexCoord2f(0.0 , 0.0 );
    glVertex3fv(ZPlane[0]);
    glTexCoord2f(xmax, 0.0 );
    glVertex3fv(ZPlane[1]);
    glTexCoord2f(xmax, ymax);
    glVertex3fv(ZPlane[2]);
    glTexCoord2f(0.0 , ymax);
    glVertex3fv(ZPlane[3]);
  glEnd();

  glBindTexture( GL_TEXTURE_2D, OyTexture);

  glBegin(GL_QUADS);
    glTexCoord2f(0.0 , 0.0 );
    glVertex3fv(YPlane[0]);
    glTexCoord2f(xmax, 0.0 );
    glVertex3fv(YPlane[1]);
    glTexCoord2f(xmax, zmax);
    glVertex3fv(YPlane[2]);
    glTexCoord2f(0.0 , zmax);
    glVertex3fv(YPlane[3]);
  glEnd();

  glBindTexture( GL_TEXTURE_2D, OxTexture);

  glBegin(GL_QUADS);
    glTexCoord2f(0.0 , 0.0 );
    glVertex3fv(XPlane[0]);
    glTexCoord2f(ymax, 0.0 );
    glVertex3fv(XPlane[1]);
    glTexCoord2f(ymax, zmax);
    glVertex3fv(XPlane[2]);
    glTexCoord2f(0.0 , zmax);
    glVertex3fv(XPlane[3]);
  glEnd();

  glDisable(GL_TEXTURE_2D);
}

/*********************************/
/* dessin des bords des 3 slices */
/*********************************/
void drawSlicesBoundary(void)
{
  glLineWidth(3.);
  glColor3f(1.,1.,0.);
  glBegin(GL_LINE_STRIP);
    glVertex3fv(ZPlane[0]);
    glVertex3fv(ZPlane[1]);
    glVertex3fv(ZPlane[2]);
    glVertex3fv(ZPlane[3]);
    glVertex3fv(ZPlane[0]);
  glEnd();

  glBegin(GL_LINE_STRIP);
    glVertex3fv(YPlane[0]);
    glVertex3fv(YPlane[1]);
    glVertex3fv(YPlane[2]);
    glVertex3fv(YPlane[3]);
    glVertex3fv(YPlane[0]);
  glEnd();

  glBegin(GL_LINE_STRIP);
    glVertex3fv(XPlane[0]);
    glVertex3fv(XPlane[1]);
    glVertex3fv(XPlane[2]);
    glVertex3fv(XPlane[3]);
    glVertex3fv(XPlane[0]);
  glEnd();

  glBegin(GL_LINES);
    glVertex3f( 0., Oy, Oz*hz);
    glVertex3f( sx, Oy, Oz*hz);
    glVertex3f( Ox, 0., Oz*hz);
    glVertex3f( Ox, sy, Oz*hz);
    glVertex3f( Ox, Oy, 0.*hz);
    glVertex3f( Ox, Oy, sz*hz);
  glEnd();
}

/**
 *Retourne l'intersection du segment [p1,p2] avec le plan z=v.
 *
 */
Point3 intersection(Point3 p1,Point3 p2,float v){
	if(v==p1.z)return p1;
	if(v==p2.z)return p2;

	double a = (v-p2.z)/(p1.z-p2.z);
	Point3 p;
	p.x=a*p1.x+(1-a)*p2.x;
	p.y=a*p1.y+(1-a)*p2.y;
	p.z=a*p1.z+(1-a)*p2.z;

	return p;
}


/******************************************************************/
/* calcul et tracé de l'isocourbe de valeur isoVal dans la coupe z=Oz */
/******************************************************************/
/* void drawIsoCurveZ(void) */
/* { */
/* 	unsigned long i,j; */

/* 	glLineWidth(1.);     /\* épaisseur 1  *\/ */
/* 	glColor3f(0.,1.,1.); /\* couleur cyan *\/ */
/* 	glBegin(GL_LINES); */

/* 	/\* début du calcul et tracé de la courbe isovaleur            *\/ */
/* 	/\* les données de la coupe z=Oz sont données par              *\/ */
/* 	/\* buffer_vol[i+sx*j+sx*sy*Oz] avec 0<=i<=sx-1 et 0<=j<=sy-1 *\/ */
/* 	/\*                                                            *\/ */
/* 	/\* Pour tracer un segment [(x1,y1,z1)-(x2,y2,z2)],            *\/ */
/* 	/\* insérer les deux instructions OpenGL suivantes :           *\/ */
/* 	/\*    glVertex3f( x1 , y1 , z1 );                             *\/ */
/* 	/\*    glVertex3f( x2 , y2 , z2 );                             *\/ */
/* 	/\*                                                            *\/ */
/* 	/\* PARTIE A COMPLETER                                         *\/ */
/* 	int ii; */
/* 	int jj; */
/* 	int ar; */

/* 	// allocation mémoire du tableaux T */
/* 	int nT=sx*sy*2; */
/* 	printf("ici1\n"); */
/* 	Triangle* T = malloc(nT * sizeof(Triangle)); */
/* 	printf("ici2\n"); */
/* 	int iT=0; */
/* 	Point3 p1; */
/* 	Point3 p2; */
/* 	Point3 p3; */
/* 	Point3 p4; */
/* 	for(ii=0;ii<sx-1;i++){ */
/* 		for(jj=0;jj<sy-1;jj++){ */
/* 			printf("ici3\n"); */
/* 			p1.x=ii; */
/* 			p1.y=jj; */
/* 			p1.z=buffer_vol[ii+sx*jj+sx*sy*Oz]; */
/* 			//p1=consPt(ii,jj,buffer_vol[ii+sx*jj+sx*sy*Oz]); */
/* 			printf("ici3.1\n"); */
/* 			//Point3 p2=consPt(ii,jj+1,buffer_vol[ii+sx*(jj+1)+sx*sy*Oz]); */
/* 			p2.x=ii; */
/* 			p2.y=jj+1; */
/* 			p2.z=buffer_vol[ii+sx*(jj+1)+sx*sy*Oz]; */

/* 			printf("ici3.2\n"); */
/* 			//Point3 p3=consPt(ii+1,jj+1,buffer_vol[ii+1+sx*jj+1+sx*sy*Oz]); */
/* 			p3.x=ii+1; */
/* 			p3.y=jj+1; */
/* 			p3.z=buffer_vol[ii+1+sx*(jj+1)+sx*sy*Oz]; */

/* 			printf("ici3.3\n"); */
/* 			//printf(" %f ",buffer_vol[ii+1+sx*jj+sx*sy*Oz]); */
/* 			printf("ici3.4\n"); */

/* 			p4.x=ii+1; */
/* 			printf("ici3.5\n"); */
/* 			p4.y=jj; */
/* 			printf("ici3.6\n"); */
/* 			//p4.z=0; */
/* 			p4.z=buffer_vol[ii+1+sx*jj+sx*sy*Oz]; */
/* 			//p4=consPt(ii+1,jj,buffer_vol[ii+1+sx*jj+sx*sy*Oz]); */
/* 			printf("ici4\n"); */

/* 			if(jj%2==0){ */
/* 				T[iT].p[0]=p1; */
/* 				T[iT].p[1]=p2; */
/* 				T[iT].p[2]=p3; */

/* 				T[iT+1].p[0]=p1; */
/* 				T[iT+1].p[1]=p4; */
/* 				T[iT+1].p[2]=p3; */
/* 			} */
/* 			else{ */
/* 				T[iT].p[0]=p1; */
/* 				T[iT].p[1]=p2; */
/* 				T[iT].p[2]=p4; */

/* 				T[iT+1].p[0]=p4; */
/* 				T[iT+1].p[1]=p2; */
/* 				T[iT+1].p[2]=p3; */
/* 			} */
/* 			iT+=2; */
/* 			printf("ici5\n"); */
/* 		} */
/* 	} */

/* 	Point3 E1[nT]; */
/* 	Point3 E2[nT]; */
/* 	int e2=0; */
/* 	int N=0; */

/* 	for(iT=0;iT<nT;iT++){ */
/* 		e2=0; */
/* 		for(ar=0;ar<3;ar++){ //boucle sur les arêtes */
/* 			if(T[iT].p[ar].z==isoVal && T[iT].p[(ar+1)%3].z==isoVal){ // cas où l'arête est sur le plan */
/* 				E1[N]=T[iT].p[ar]; */
/* 				E2[N]=T[iT].p[(ar+1)%3]; */
/* 				N++; */
/* 				break; */
/* 			} */
/* 			else if((T[iT].p[ar].z-isoVal)*(T[iT].p[(ar+1)%3].z-isoVal)<=0){ // cas où le plan intersecte l'arête en un seul point */
/* 				if(e2){ */
/* 					E2[N]=intersection(T[iT].p[ar],T[iT].p[(ar+1)%3],isoVal); */
/* 					N++; */
/* 					break; */
/* 				} */
/* 				else{ */
/* 					E1[N]=intersection(T[iT].p[ar],T[iT].p[(ar+1)%3],isoVal); */
/* 					e2=1; */
/* 				} */
/* 			} */
/* 		} */
/* 	} */

/* 	for(ii=0;ii<N;ii++){ */
/* 		glVertex3f(E1[ii].x,E1[ii].y,E1[ii].z*hz); */
/* 		glVertex3f(E2[ii].x,E2[ii].y,E2[ii].z*hz); */
/* 	} */

/* 	glEnd(); */
/* } */

void drawIsoCurveZ(void)
{
	unsigned long i,j;

	glLineWidth(1.);     /* épaisseur 1  */
	glColor3f(0.,1.,1.); /* couleur cyan */
	glBegin(GL_LINES);

	/* début du calcul et tracé de la courbe isovaleur            */
	/* les données de la coupe z=Oz sont données par              */
	/* buffer_vol[i+sx*j+sx*sy*Oz] avec 0<=i<=sx-1 et 0<=j<=sy-1 */
	/*                                                            */
	/* Pour tracer un segment [(x1,y1,z1)-(x2,y2,z2)],            */
	/* insérer les deux instructions OpenGL suivantes :           */
	/*    glVertex3f( x1 , y1 , z1 );                             */
	/*    glVertex3f( x2 , y2 , z2 );                             */
	/*                                                            */
	/* PARTIE A COMPLETER                                         */
	int ii;
	int jj;
	int ar;

	Point3 S1;
	Point3 S2;
	Point3 p1;
	Point3 p2;
	Point3 p3;
	Point3 p4;
	int e2;
	Triangle T[2];

	for(ii=0;ii<sx-1;ii++){
		for(jj=0;jj<sy-1;jj++){
			e2=0;

			p1.x=ii;
			p1.y=jj;
			p1.z=buffer_vol[ii+sx*jj+sx*sy*Oz];

			p2.x=ii;
			p2.y=jj+1;
			p2.z=buffer_vol[ii+sx*(jj+1)+sx*sy*Oz];

			p3.x=ii+1;
			p3.y=jj+1;
			p3.z=buffer_vol[ii+1+sx*(jj+1)+sx*sy*Oz];

			p4.x=ii+1;
			p4.y=jj;
			p4.z=buffer_vol[ii+1+sx*jj+sx*sy*Oz];

			if(jj%2==0){
				T[0].p[0]=p1;
				T[0].p[1]=p2;
				T[0].p[2]=p3;

				T[1].p[0]=p1;
				T[1].p[1]=p4;
				T[1].p[2]=p3;
			}
			else{
				T[0].p[0]=p1;
				T[0].p[1]=p2;
				T[0].p[2]=p4;

				T[1].p[0]=p4;
				T[1].p[1]=p2;
				T[1].p[2]=p3;
			}

			for(int iT=0;iT<2;iT++){
				for(ar=0;ar<3;ar++){ //boucle sur les arêtes
					if(T[iT].p[ar].z==isoVal && T[iT].p[(ar+1)%3].z==isoVal){ // cas où l'arête est sur le plan
						S1=T[iT].p[ar];
						S2=T[iT].p[(ar+1)%3];
						e2=1;
						break;
					}
					else if((T[iT].p[ar].z-isoVal)*(T[iT].p[(ar+1)%3].z-isoVal)<=0){ // cas où le plan intersecte l'arête en un seul point
						if(e2){
							S2=intersection(T[iT].p[ar],T[iT].p[(ar+1)%3],isoVal);
							break;
						}
						else{
							S1=intersection(T[iT].p[ar],T[iT].p[(ar+1)%3],isoVal);
							e2=1;
						}
					}
				}

				if(e2){
					glVertex3f(S1.x,S1.y,Oz*hz);
					glVertex3f(S2.x,S2.y,Oz*hz);
				}
			}
		}
	}

	glEnd();
}


/****************************************/
/* affichage d'info                     */
/****************************************/
void info()
{
  if (reportInfo)
  {
  	printf("Ox = %d, Oy = %d, Oz = %d, isovaleur=%f\n",
	   Ox, Oy, Oz, isoVal);
  }
}

/****************************************/
/* callback de (re)dessin de la fenêtre */
/****************************************/
void redraw(void)
{
  int start, end;

  if (reportSpeed) {
    start = glutGet(GLUT_ELAPSED_TIME);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();  /* repere camera */

	  tbVisuTransform(); /* origine et orientation de la scene */

      glScalef(zoom,zoom,zoom);
      glTranslatef( -sx/2., -sy/2., -sz*hz/2.);

      drawSlices();
      drawSlicesBoundary();
	  drawIsoCurveZ();

  if (reportSpeed) {
    glFinish();
    end = glutGet(GLUT_ELAPSED_TIME);
    printf("Speed %.3g frames/sec (%d ms)\n", 1000.0/(end-start), end-start);
  }

  glutSwapBuffers();
}


/******************************************************************/
/* When not visible, stop animating.  Restart when visible again. */
/******************************************************************/
void visible(int vis)
{
  glutIdleFunc(NULL);
}


/*******************************************/
/* callback associé aux évenements clavier */
/*******************************************/
void key(unsigned char c, int x, int y)
{
  if ( (c == 27) || (c=='q')) {
    exit(0);
  }
  if ((c=='X') && (Ox < sx-1)){
	Ox++;
	info();
	makeSliceTexture();
  }
  if ((c=='x') && (Ox > 0)){
	Ox--;
	info();
	makeSliceTexture();
  }
  if ((c=='Y') && (Oy < sy-1)){
	Oy++;
	info();
	makeSliceTexture();
  }
  if ((c=='y') && (Oy > 0)){
	Oy--;
	info();
	makeSliceTexture();
  }
  if ((c=='Z') && (Oz < sz-1)){
	Oz++;
	info();
	makeSliceTexture();
  }
  if ((c=='z') && (Oz > 0)){
	Oz--;
	info();
	makeSliceTexture();
  }

  /* modification isovaleur */
  if ((c=='-') && (isoVal >= pasIsoVal)){
	isoVal -= pasIsoVal;
	info();
  }
  if ((c=='+') && (isoVal <= 1.0-pasIsoVal)){
	isoVal += pasIsoVal;
	info();
  }

  /* changement facteur de zoom */
  if (c=='e') zoom *= 1.05;
  if (c=='E') zoom /= 1.05;

  /* copie d'écran */
  if ((c=='0')){
	CopieEcran("image0.ppm");
  }
  if ((c=='d')){
	drawIsoCurve = 1-drawIsoCurve;
  }
  if ((c=='i')){
	reportInfo = 1-reportInfo;
  }
  glutPostRedisplay();
}


void special(int k, int x, int y)
{
  glutPostRedisplay();
}


/*****************************************************************************/
/*****************************************************************************/
/* ROUTINE PRINCIPALE                                                        */
/*****************************************************************************/
/*****************************************************************************/
int main(int argc, char **argv)
{

  long i,j,k;
  int TailleMax;
  float Max;
  char *datafile;
  FILE *f;
  int format=2;

  /**************************************/
  /* test et récupération des arguments */
  /**************************************/

  if (argc <7) {
	fprintf( stderr, "usage:\n");
	fprintf( stderr, "%s filename sx sy sz hz format\n", argv[0]);
	fprintf( stderr, " sx sy sz : dimensions des données\n");
	fprintf( stderr, " hz : facteur d'échelle en z\n");
	fprintf( stderr, " format :\n");
	fprintf( stderr, "  1 - valeur sur 1 octet \n");
	fprintf( stderr, "  2 - valeur sur 2 octets\n");
	exit(-1);
  }

  /* sx , sy = dimensions des slices */
  /* sz =      nb de slices */
  datafile = argv[1];
  sx = atoi(argv[2]);
  sy = atoi(argv[3]);
  sz = atoi(argv[4]);
  hz = atof(argv[5]);
  format = atoi(argv[6]);

  /*****************************************/
  /* lecture et normalisation des données  */
  /* chaque donnée entre 0 et 65535        */
  /*****************************************/

  /* tableau des données volumiques */
  buffer_vol = (float*) malloc(sizeof(float) * sx*sy*sz );

  /* ouverture du fichier de donnée volumique */
  if ((f=fopen( datafile, "r")) == 0) {
	fprintf( stderr, "Impossible de lire le fichier %s\n", argv[1]);
	exit(-1);
  }

  /* lecture des données binaires : */
  /* chaque valeur sur 2 bytes (entier court entre 0 et 65535)    */
  /* buffer_vol[i + sx*j + sx*sy*k] est la donnée correspondant à */
  /*    x = i  avec  0 <= i <= sx-1								  */
  /*    y = j  avec  0 <= j <= sy-1								  */
  /*    z = k  avec  0 <= k <= sz-1								  */

  if (format==2) /* valeur du fichier sur 2 octets */
    {
		for (i=0; i<sx*sy*sz; i++)
		{
			unsigned short v;
  			fread( &v, sizeof(unsigned short), 1, f);
			buffer_vol[i] = v;
		}
	}
  else if (format==1) /* valeur du fichier sur 1 octet */
    {
		for (i=0; i<sx*sy*sz; i++)
		{
			unsigned char v;
  			fread( &v, sizeof(unsigned char), 1, f);
			buffer_vol[i] = v;
		}
	}
  else
	{
		fprintf(stderr, "Mauvais format\n");
		return -1;
	}

  fclose(f);

  /* multiplication des données par un facteur pour que */
  /* min(buffer_vol[i])=0.0 et max(buffer_vol[i])=1.0   */
  Max=0.0;
  for (i=0;i<sx*sy*sz;i++) {if (Max<buffer_vol[i]) {Max=buffer_vol[i];j=i;}}
  fprintf( stderr, "Max VolData = %f, indice=%d\n", Max,j);
  Max = 1.0/Max;
  for (i=0;i<sx*sy*sz;i++) {buffer_vol[i] *= Max;}
  Max=0.0;
  for (i=0;i<sx*sy*sz;i++) {if (Max<buffer_vol[i]) {Max=buffer_vol[i];}}
  fprintf( stderr, "Nouveau Max VolData = %f\n", Max);


  /**********************************************/
  /* initialisation de variables du programme   */
  /**********************************************/

  /* sx2 : plus petite puissance de 2 >= sx 	*/
  /* sy2 : plus petite puissance de 2 >= sy 	*/
  /* sz2 : plus petite puissance de 2 >= sz 	*/
  /* utilisés pour la représentation des slices */
  sx2 = 1; while (sx2 < sx) {sx2 *= 2;}
  sy2 = 1; while (sy2 < sy) {sy2 *= 2;}
  sz2 = 1; while (sz2 < sz) {sz2 *= 2;}

//  fprintf( stderr, "sz2 = %d\n", sz2);

  /* position initiale des coupes */
  Ox=sx/2; Oy=sy/2; Oz=sz/2;

  /* calcul d'une taille convenable pour le buffer de texture */
  TailleMax = sx2;
  if (TailleMax < sy2) {TailleMax = sy2;}
  if (TailleMax < sz2) {TailleMax = sz2;}
  TextureBuffer = (unsigned short*)
    malloc(sizeof(unsigned short) * TailleMax * TailleMax);


  /**********************************************/
  /* infos diverses dans la fenetre de commande */
  /**********************************************/

  fprintf( stderr, "Dimensions des données %d %d %d\n", sx, sy, sz);

  fprintf( stderr, "\nMode d'emploi :\n");
  fprintf( stderr, "\n rotation avec la souris (bouton gauche enfoncé)\n");

  fprintf( stderr, "\n touches x,y,z,X,Y,Z: modifie la position des 3 plans\n");
  fprintf( stderr, "       de coupe dans le volume de donnees\n");
  fprintf( stderr, " touche d : switch on/off du tracé de la courbe isovaleur'\n");
  fprintf( stderr, " touche 0 : copie d'écran dans le fichier 'image0.ppm'\n");
  fprintf( stderr, " touches +/- : augmente/diminue l'iso-valeur\n");
  fprintf( stderr, " touche i : info\n");
  fprintf( stderr, " touches q,ESC : FIN\n");

  info();

  /*************************************************/
  /* création de la fenetre avec la librairie GLUT */
  /*************************************************/

  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutCreateWindow("Visu. de données volumiques");

  glMatrixMode(GL_PROJECTION);
  glOrtho(-sx/2.,sx/2.,-sy/2., sy/2., -1000.*hz, +1000.*hz);

  glMatrixMode(GL_MODELVIEW);
  tbInitTransform();     /* initialisation du point de vue */

  /* Register GLUT callbacks. */
  glutDisplayFunc(redraw);
  glutMouseFunc(tbMouseFunc);    /* gestion traqueboule */
  glutMotionFunc(tbMotionFunc);  /* gestion traqueboule */
  glutVisibilityFunc(visible);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);

  glClearColor( 1., 1., 1., 0.);
  glClearDepth( 1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0, 1.0);

  /* calcul des images des 3 coupes */
  makeSliceTexture();

  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
