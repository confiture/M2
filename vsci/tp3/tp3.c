/************************************************************************
**
**	Marching cube / tetrahedra - Visualisation OpenGL
**  PROGRAMME A COMPLETER (routine calculIsoSurface)
**
**  Version avec traqueboule
**
**  Création de l'exécutable :
**    gcc tp3.c -I. -o tp3 /usr/lib/libglut.so.3 /usr/lib/libGL.so.1 /usr/lib/libGLU.so.1
**
**  Exemple d'exécution - cas d'une fonction implicite
**    ./tp3
**
**  Exemple d'exécution - cas de données volumiques lues dans un fichier
**    ./tp3 CThead.bin 256 256 113 2 2
**    ./tp3 lobster.raw 301 324 56 1.4 1
**
/************************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*----------------------------*/
/* définitions des structures */
/*----------------------------*/

/* point de l'espace 3D */
typedef struct _Point3D
{
	float x;
	float y;
	float z;
} Point3D;

float prodScal(Point3D p0,Point3D p1,Point3D q0,Point3D q1){
	return (p1.x-p0.x)*(q1.x-q0.x)+(p1.y-p0.y)*(q1.y-q0.y)+(p1.z-p0.z)*(q1.z-q0.z);
}

/* facette triangulaire 3D */
typedef struct _Triangle3D
{
	Point3D p[3];
}
Triangle3D;

/* Tetraedre */
typedef struct _Tetraedre
{
	Point3D p[4];
}
Tetraedre;

/*--------------------*/
/* variables globales */
/*--------------------*/

/* buffer des données volumiques */
float *buffer_vol;
unsigned long taille_buffer_vol;

/* dimensions de la grille de données */
int sx, sy, sz;

/* boite englobante correspondante */
float xmin, xmax;
float ymin, ymax;
float zmin, zmax;

/* dimensions d'un cube de la grille de données */
float dx;
float dy;
float dz;

/* zoom pour l'affichage de l'isosurface */
float zoom;

/* echelle en x,y,z */
float eX=1.0;
float eY=1.0;
float eZ=1.0;

/* isoValeur */
float isoVal;

/* taille du cube elementaire */
int taille;

/* mode de tracé */
int mode;

/* buffer des triangles à afficher */
Triangle3D *lT;
unsigned long nTmax;
unsigned long nT;

/************************************************************************
 *
 * Module d'eclairage pour OpenGL.
 *
/************************************************************************/


/*--------------------*/
/* variables globales */
/*--------------------*/

/* propriété de la matiere */
GLfloat matSpecular[]={0.0,0.0,0.0};
GLfloat matShininess[]={128.0};

/* propriété de la lumière */
GLfloat lightAmbient[]={0.0,0.0,0.5,0.0};
GLfloat lightDiffuse[]={1.0,1.0,1.0,0.0};
GLfloat lightSpecular[]={1.0,1.0,1.0,1.0};

GLfloat lightPosition[]={100.0,-100.0,1000.0,0.0};
GLfloat lightSpotDirection[]={0.0,0.0,0.0};

GLfloat colorLight[]={0.0,0.0,1.0,1.0};


/****************
 *
 * fonction d'initialisation de la lumière
 *
 ****************/
void initLumiere(void)
{
	/* specifie le rendu des ombres */
	glShadeModel(GL_SMOOTH);

	/* parametrage d'une lumière */
	/* position de la lumière dans la scene */
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

	/* parametrage de la lumière ambiente, diffuse et speculaire */
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);


	/* lumière de couleur colorLight */
	glLightfv(GL_LIGHT0,GL_COLOR,colorLight);

	/* activation de la lumière */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

/****************
 *
 * fonction d'initialisation des materiaux
 *
 ****************/
void initMatiere(void)
{
	/* les faces avants et arrières des polygones
	   sont considérées comme des faces avants */
	glFrontFace(GL_FRONT_AND_BACK);

	/* definit les propriétés du matériaux */
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

	/* permet de faire capter la lumière à toutes les facettes (2 cotés) */
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
}

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
		fwrite(ptr, width*3, 1, fd);
	}
	fclose(fd) ;
	free(data);
	fprintf(stderr, "Fichier %s créé\n", filename);
}



/***************************************************
 *
 * calcul de la normale d'un triangle 3D
 *
 ***************************************************/
Point3D normalTriangle3D(Triangle3D triangle)
{
	Point3D V10,V20;
	Point3D N;
	float norme;

	/* triangle = [P0,P1,P1] */
	/* --> calcul des vecteurs V10 = P1-P0 et V20 = P2-P0 */
	V10.x=triangle.p[1].x-triangle.p[0].x;
	V10.y=triangle.p[1].y-triangle.p[0].y;
	V10.z=triangle.p[1].z-triangle.p[0].z;

	V20.x=triangle.p[2].x-triangle.p[0].x;
	V20.y=triangle.p[2].y-triangle.p[0].y;
	V20.z=triangle.p[2].z-triangle.p[0].z;


	/* produit vectoriel N = V10 /\ V20 */
	N.x=(V10.y*V20.z-V20.y*V10.z);
	N.y=(V10.z*V20.x-V20.z*V10.x);
	N.z=(V10.x*V20.y-V20.x*V10.y);

	/* normaliser le vecteur */
	norme=sqrt(N.x*N.x+N.y*N.y+N.z*N.z);
	if (norme>1e-6)
	{
		N.x/=norme;
		N.y/=norme;
		N.z/=norme;
	}
	return N;
}


/*****************************
 *
 * affichage d'un triangle
 * avec le mode correspondant
 *
 *******************************/
static void drawTriangle(Triangle3D triangle,int mode)
{
	Point3D N=normalTriangle3D(triangle);
	glBegin(mode);
	{
		glNormal3f(N.x,N.y,N.z);
		glVertex3f(triangle.p[0].x,triangle.p[0].y,triangle.p[0].z);
		glNormal3f(N.x,N.y,N.z);
		glVertex3f(triangle.p[1].x,triangle.p[1].y,triangle.p[1].z);
		glNormal3f(N.x,N.y,N.z);
		glVertex3f(triangle.p[2].x,triangle.p[2].y,triangle.p[2].z);

		if(mode==GL_LINE_STRIP)
		{
			glNormal3f(N.x,N.y,N.z);
			glVertex3f(triangle.p[0].x,triangle.p[0].y,triangle.p[0].z);
		}
	}
	glEnd();
}

/*****************************
 *
 * affichage des triangles issus du 'marching tetrahedra'
 *
 *******************************/
void drawMarchingTetrahedra()
{
	int i;
	for (i=0; i < nT; i++)
	{
		drawTriangle(lT[i],mode);
	}
}



/************************************************************************
 **
 **	Algorithme du marching cube / tetrahedra
 **
/************************************************************************/

/* booleen pour indiquer la memoire trinagles est disponible (non pleine) */
int infoAjout;

void AJOUT_TRIANGLE(Triangle3D t)
{
	if (nT<nTmax) lT[nT++]=t;
	else if (infoAjout==1)
	{
		printf(" !mémoire triangle pleine\n");
		infoAjout=0;
	}
}

/**
 *Retourne l'intersection du segment [p1,p2] avec le plan z=v.
 *
 */
Point3D intersection(Point3D p1,float val1,Point3D p2,float val2,float v){
	if(v==p1.z)return p1;
	if(v==p2.z)return p2;

	double a = (v-val2)/(val1-val2);
	Point3D p;
	p.x=a*p1.x+(1-a)*p2.x;
	p.y=a*p1.y+(1-a)*p2.y;
	p.z=a*p1.z+(1-a)*p2.z;

	return p;
}

/**
 *Retourne l'intersection du  avec le plan z=v.
 *
 */
void intersectionT(Tetraedre te,float* vals,float v,Point3D* tr,int * taille){
	(*taille)=0;
	int i,j;


	for(i=0;i<3;i++){
		for(j=i+1;j<4;j++){
			if((vals[i]-v)*(vals[j]-v)<=0){
				tr[(*taille)]=intersection(te.p[i],vals[i],te.p[j],vals[j],v);
				(*taille)=(*taille)+1;
			}
		}
	}


/* 	if((*taille)==4){ */
/* 		for(i=0;i<3;i++){ */
/* 			for(j=i+1;j<4;j++){ */
/* 				if */


}

/****************
 *
 * fonction qui calcule l'iso-surface par le Marching Cube / Tetrahedra
 * chaque cube élémentaire est un cube d'une grille grossière incluse dans
 * la grille initiale
 * la grille initiale correspond aux points P(i,j,k) avec
 * 0 <= i < sx, 0 <= j < sy, 0 <= k < sz
 * la grille grossière correspond aux points
 * P(i1=i*taille,j1=j*taille,k1=k*taille) avec
 * 0 <= i1 < sx-taille, 0 <= j1 < sy-taille, 0 <= k1 < sz-taille
 *
 * le point P(i,j,k) a pour coordonnées :
 *   x = xmin + i*(xmax-xmin)/(sx-1) = xmin +i*dx
 *   y = ymin + j*(ymax-ymin)/(sy-1) = ymin +j*dy
 *   z = zmin + k*(zmax-zmin)/(sz-1) = zmin +k*dz
 *
 * la macro AJOUT_TRIANGLE utilisée pour l'ajout de triangle ne peut etre
 * incluse que dans la routine calculIsoSurface()
 *
 ****************/
void calculIsoSurface()
{
	/* indices de la base du cube élémentaire */
	int i1, j1, k1;
	int iTrs;
	int length;
	Point3D pts[4];
	Tetraedre trs[6];
	float** valT=malloc(sizeof(float*)*6);

	for(iTrs=0;iTrs<6;iTrs++)valT[iTrs]=malloc(sizeof(float)*4);

	fprintf(stdout, "calculIsoSurface : taille = %2d,", taille);
	fprintf(stdout, " isoVal = %5.2f,", isoVal );
	nT=0; /* effacement de la liste de triangles */
	infoAjout=1; /* memoire triangles disponible */

	//printf("ici3\n");

	/* prise en compte des différents cubes élémentaires */
	/* inclus dans la grille de points initiale */
	for(k1=0;k1<sz-taille ;k1+=taille)
	{
		for(j1=0;j1<sy-taille;j1+=taille)
		{
			for(i1=0;i1<sx-taille;i1+=taille)
			{
				int i;
				int im,jm,km;
				Point3D p[8];   /* coordonnées des sommets du cube élémentaire */
				float val[8];  /* valeurs aux sommets du cube élémentaire */

				/* calcul du cube elementaire : 8 sommets (m=0 à 7) */
				/*   p[m]   : coordonnées du sommet m               */
				/*   val[m] : valeur au sommet m                    */
				/* le sommet m du cube élémentaire correspond au    */
				/* point P(im,jm,km) de la grille de données        */
				/* les 6 faces du cube sont :                       */
				/*      7------------4          z                   */
				/*     /|           /|          |                   */
				/*    / |          / |          |                   */
				/*   6------------5  |          |                   */
				/*   |  |         |  |          |                   */
				/*   |  |         |  |          |                   */
				/*   |  |         |  |          |                   */
				/*   |  3---------|--0          .------------y      */
				/*   | /          | /          /                    */
				/*   |/           |/          /                     */
				/*   2------------1          x                      */
				im=i1;
				jm=j1+taille;
				km=k1;
				p[0].x=xmin+(float)(im)*dx;
				p[0].y=ymin+(float)(jm)*dy;
				p[0].z=zmin+(float)(km)*dz; p[0].z *= 1;
				val[0] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1+taille;
				jm=j1+taille;
				km=k1;
				p[1].x=xmin+(float)(im)*dx;
				p[1].y=ymin+(float)(jm)*dy;
				p[1].z=zmin+(float)(km)*dz; p[1].z *= 1;
				val[1] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1+taille;
				jm=j1;
				km=k1;
				p[2].x=xmin+(float)(im)*dx;
				p[2].y=ymin+(float)(jm)*dy;
				p[2].z=zmin+(float)(km)*dz; p[2].z *= 1;
				val[2] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1;
				jm=j1;
				km=k1;
				p[3].x=xmin+(float)(im)*dx;
				p[3].y=ymin+(float)(jm)*dy;
				p[3].z=zmin+(float)(km)*dz; p[3].z *= 1;
				val[3] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1;
				jm=j1+taille;
				km=k1+taille;
				p[4].x=xmin+(float)(im)*dx;
				p[4].y=ymin+(float)(jm)*dy;
				p[4].z=zmin+(float)(km)*dz; p[4].z *= 1;
				val[4] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1+taille;
				jm=j1+taille;
				km=k1+taille;
				p[5].x=xmin+(float)(im)*dx;
				p[5].y=ymin+(float)(jm)*dy;
				p[5].z=zmin+(float)(km)*dz; p[5].z *= 1;
				val[5] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1+taille;
				jm=j1;
				km=k1+taille;
				p[6].x=xmin+(float)(im)*dx;
				p[6].y=ymin+(float)(jm)*dy;
				p[6].z=zmin+(float)(km)*dz; p[6].z *= 1;
				val[6] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1;
				jm=j1;
				km=k1+taille;
				p[7].x=xmin+(float)(im)*dx;
				p[7].y=ymin+(float)(jm)*dy;
				p[7].z=zmin+(float)(km)*dz; p[7].z *= 1;
				val[7] = buffer_vol[im+jm*sx+km*sx*sy];

				/* découpage du cube élémentaire */
				/* calcul des triangles correspondants au cube élémentaire */
				/* DEBUT PARTIE A MODIFIER */

				/* PARTIE A ENLEVER/MODIFIER PAR LA SUITE            */
				/* exemple d'ajout de triangle :                     */
				/* ajout d'1 triangle par cube intersecté            */
				/* par la surface iso-valeur                         */
				/* i.e. cube avec changement de signe en ses sommets */

				{
					Triangle3D t;
					int i; int s;
					s=0;
					for (i=0; i<8; i++)
						s+=val[i]<isoVal;

					if (s>0 && s<8)
					{
						trs[0].p[0]=p[2];valT[0][0]=val[2];
						trs[0].p[1]=p[1];valT[0][1]=val[1];
						trs[0].p[2]=p[3];valT[0][2]=val[3];
						trs[0].p[3]=p[6];valT[0][3]=val[6];

						trs[1].p[0]=p[1];valT[1][0]=val[1];
						trs[1].p[1]=p[3];valT[1][1]=val[3];
						trs[1].p[2]=p[6];valT[1][2]=val[6];
						trs[1].p[3]=p[0];valT[1][3]=val[0];

						trs[2].p[0]=p[7];valT[2][0]=val[7];
						trs[2].p[1]=p[0];valT[2][1]=val[0];
						trs[2].p[2]=p[3];valT[2][2]=val[3];
						trs[2].p[3]=p[6];valT[2][3]=val[6];

						trs[3].p[0]=p[7];valT[3][0]=val[7];
						trs[3].p[1]=p[4];valT[3][1]=val[4];
						trs[3].p[2]=p[5];valT[3][2]=val[5];
						trs[3].p[3]=p[0];valT[3][3]=val[0];

						trs[4].p[0]=p[7];valT[4][0]=val[7];
						trs[4].p[1]=p[5];valT[4][1]=val[5];
						trs[4].p[2]=p[6];valT[4][2]=val[6];
						trs[4].p[3]=p[0];valT[4][3]=val[0];

						trs[5].p[0]=p[5];valT[5][0]=val[5];
						trs[5].p[1]=p[6];valT[5][1]=val[6];
						trs[5].p[2]=p[1];valT[5][2]=val[1];
						trs[5].p[3]=p[0];valT[5][3]=val[0];

						for(iTrs=0;iTrs<6;iTrs++){
							s=0;
							for(i=0;i<4;i++){
								s+=valT[iTrs][i]<isoVal;

								if(s>0 && s<4){//il y a donc intersection avec l'isosurface
									intersectionT(trs[iTrs],valT[iTrs],isoVal,pts,&length);
								}
								/* le cube élémentaire intersecte l'iso-surface   */
								/* -> affichage d'une face du cube, cette face    */
								/* est décomposée en 2 triangles                  */

								/* affichage de la face [0 1 2 3]        */
								/* -> ajout du triangle (p[0],p[1],p[2]) */
								/* -> ajout du triangle (p[0],p[2],p[3]) */

								t.p[0] = pts[0];
								t.p[1] = pts[1];
								t.p[2] = pts[2];

								AJOUT_TRIANGLE(t);

								if(length==4){
									t.p[0] = pts[0];
									t.p[1] = pts[2];
									t.p[2] = pts[3];
									AJOUT_TRIANGLE(t);

									t.p[0] = pts[0];
									t.p[1] = pts[1];
									t.p[2] = pts[3];
									AJOUT_TRIANGLE(t);

									t.p[0] = pts[1];
									t.p[1] = pts[2];
									t.p[2] = pts[3];
									AJOUT_TRIANGLE(t);

								}
							}
						}
					}
				}
				/* FIN PARTIE A MODIFIER */

				//printf("%d\n",i1);
				/* si memoire triangle pleine : sortir de la boucle */
				if (infoAjout==0) goto 	FinIsoSurface;

			} /* for i1 */
		} /* for j1 */
	} /* for k1 */

	//printf("goto\n");
 FinIsoSurface :
	fprintf(stdout, " nT : %7d", nT);
	fflush(stdout);
	fprintf(stdout, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	fprintf(stdout, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}




/****************
 *
 * routine de (ré)initialisation
 *
 ****************/
void initialisation()
{
	glLoadIdentity();
	taille=10;
	isoVal=0.5;
	zoom = 1.0;
	mode=GL_POLYGON;
	tbInitTransform();

	calculIsoSurface();
}

/****************
 *
 * fonction d'affichage du systeme
 *
 ****************/
void display()
{
	/* glClearColor (0.0, 0.0, 0.0, 0.0); */
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* si le type de tracé est ligne on desactive l'éclairage */
    if(mode==GL_LINE_STRIP)
		glDisable(GL_LIGHTING);
	/* sinon on réactive l'eclairage */
	else
		glEnable(GL_LIGHTING);

	glLoadIdentity();  /* repere camera */

	tbVisuTransform(); /* origine et orientation de la scene */

	/* echelle */
	glScalef(zoom*eX,zoom*eY,zoom*eZ);

	/* centrage de la scene                    */
	/* prise en compte de la boite englobante  */
	glTranslatef( -(xmin+xmax)/2.0,
	              -(ymin+ymax)/2.0,
	              -(zmin+zmax)/2.0);

	/* dessin des triangles du MarchingCube */
	drawMarchingTetrahedra();

	/* envoi des commandes GL */
	glFlush();
	glutSwapBuffers();
}


/****************
 *
 * fonction de redimensionnement de la fenetre
 *
 ****************/
void myReshape(GLsizei w, GLsizei h)
{
	glViewport ( 0, 0, w, h);
	/* glMatrixMode(GL_PROJECTION);
	glOrtho(-(xmin+xmax)*0.500, (xmin+xmax)*0.500,
	        -(ymin+ymax)*0.500, (ymin+ymax)*0.500,
			-(zmin+zmax)*0.500, (zmin+zmax)*0.500);

	glMatrixMode(GL_MODELVIEW); */
}


/****************
 *
 *fonction de gestion des touches clavier
 *
 ****************/
void clavier(unsigned char key, int x, int y)
{
switch(key)
	{
	/* fin du programme */
	case 27 :
	case 'q' :
	case 'Q' :
		exit(0);
	break;

	/* reset */
	case 'r':
	case 'R':
		initialisation();
	break;

	/* modification de l'iso-valeur */
	case '-' :
		if (isoVal>=0.02)
		{
			isoVal-=0.02;
			calculIsoSurface();
		}
	break;

	case '+' :
		if (isoVal<=0.98)
		{
			isoVal+=0.02;
			calculIsoSurface();
		}
	break;

	/* modification de la taille du cube */
	case 't' :
	case 'T' :
		if (taille>1)
		{
			taille-=1;
			calculIsoSurface();
		}
	break;

	case 'y' :
	case 'Y' :
		if (taille<40)
		{
			taille+=1;
			calculIsoSurface();
		}
	break;

	case 'Z' :
		if (zoom<10) zoom *= 1.05;
		break;
	case 'z' :
		if (zoom>0.1) zoom /= 1.05;
		break;

	/* copie d'écran */
  	case 'c' :
  	case 'C' :
		CopieEcran("image0.ppm");
		break;

    default :
	break;
	}
}

/*****************************************************************************
 * creation de données à partir d'un fonction implicite F
 *****************************************************************************/

/* la fonction définissant la surface implicite */
double F(double x, double y, double z)
{
	double v;

	v = x*x+y*y+z*z;
	return v;
}

void creer_data_f_implicite()
{
	int i,j,k,l;

	/* dimensions de la grille */
	sx = sy = sz = 100;

	/* boite englobante */
	xmin = -1.0; xmax = 1.0;
	ymin = -1.0; ymax = 1.0;
	zmin = -1.0; zmax = 1.0;

	/* dimension du cube de base */
	dx = (xmax-xmin)/(float)(sx-1);
	dy = (ymax-ymin)/(float)(sy-1);
	dz = (zmax-zmin)/(float)(sz-1);

	taille_buffer_vol =
		(unsigned long)(sx) *
		(unsigned long)(sy) *
		(unsigned long)(sz);

	/* tableau des données volumiques */
	buffer_vol = (float*) malloc(sizeof(float) * taille_buffer_vol );

	l = 0;
	for (k=0; k<sz; k++)
	for (j=0; j<sy; j++)
	for (i=0; i<sx; i++)
	{
		float x,y,z;
		x = xmin + (xmax-xmin)*(float)(i)/(float)(sx-1);
		y = ymin + (ymax-ymin)*(float)(j)/(float)(sy-1);
		z = zmin + (zmax-zmin)*(float)(k)/(float)(sz-1);

		buffer_vol[l++] = F(x,y,z);
	}
}

/*****************************************************************
 * lecture d'un fichier de données volumiques
 *****************************************************************/
void lire_fichier_data(int argc, char **argv)
{
  long i,j,k;
  int TailleMax;
  char *datafile;
  FILE *f;
  int format;

  /**************************************/
  /* test et récupération des arguments */
  /**************************************/

  if (argc <7) {
	fprintf( stderr, "usage:\n");
	fprintf( stderr, "%s filename sx sy sz hz format\n", argv[0]);
	fprintf( stderr, " sx sy sz : dimensions des données (entier)\n");
	fprintf( stderr, " eZ : facteur d'échelle en z (réel)\n");
	fprintf( stderr, " format (entier) :\n");
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
  eZ = atof(argv[5]);
  format = atoi(argv[6]);

  xmin = 0.0; xmax = (float)(sx-1);
  ymin = 0.0; ymax = (float)(sy-1);
  zmin = 0.0; zmax = (float)(sz-1);

  /* dimension du cube de base */
  dx = (xmax-xmin)/(float)(sx-1);
  dy = (ymax-ymin)/(float)(sy-1);
  dz = (zmax-zmin)/(float)(sz-1);

  taille_buffer_vol =
		(unsigned long)(sx) *
		(unsigned long)(sy) *
		(unsigned long)(sz);

  /*****************************************/
  /* lecture et normalisation des données  */
  /* chaque donnée entre 0 et 65535        */
  /*****************************************/

  /* tableau des données volumiques */
  buffer_vol = (float*) malloc(sizeof(float) * taille_buffer_vol );

  /* ouverture du fichier de donnée volumique */
  if ((f=fopen( datafile, "r")) == 0) {
	fprintf( stderr, "Impossible de lire le fichier %s\n", argv[1]);
	exit(-1);
  }

  /* lecture des données binaires : */
  /* si format = 2 alors                                          */
  /*   chaque valeur lue v est codée sur 2 octets                 */
  /*  (unsigned short v entre 0 et 65535)                         */
  /* si format = 1 alors                                          */
  /*   chaque valeur lue v est codée sur 1 octet                  */
  /*  (unsigned char v entre 0 et 255)                            */
  /* buffer_vol[i + sx*j + sx*sy*k] (valeur réelle)               */
  /*   est la donnée correspondant à :                            */
  /*    x = i  avec  0 <= i <= sx-1								  */
  /*    y = j  avec  0 <= j <= sy-1								  */
  /*    z = k  avec  0 <= k <= sz-1								  */

  if (format==2) /* valeur du fichier sur 2 octets */
    {
		for (i=0; i<taille_buffer_vol; i++)
		{
			unsigned short v;
  			fread( &v, sizeof(unsigned short), 1, f);
			buffer_vol[i] = (float)v;
		}
	}
  else if (format==1) /* valeur du fichier sur 1 octet */
    {
		for (i=0; i<taille_buffer_vol; i++)
		{
			unsigned char v;
  			fread( &v, sizeof(unsigned char), 1, f);
			buffer_vol[i] = (float)v;
		}
	}
  else
	{
		fprintf(stderr, "Mauvais format\n");
		return;
	}

  fclose(f);

}

/*****************************************************************************
 * normalise les données pour quelles soient comprises entre 0.0 et 1.0
 *****************************************************************************/
void normalise_data()
{
	float Max, Min;
	unsigned long i;

	/* transformation affine des données pour que */
	/* min(valeurs) = 0.0 et max(valeurs) = 1.0   */
	Min=Max=buffer_vol[0];
	for (i=1;i<taille_buffer_vol;i++)
	{
		if (Min>buffer_vol[i]) Min=buffer_vol[i];
		if (Max<buffer_vol[i]) Max=buffer_vol[i];
	}
	fprintf( stderr, "Min VolData = %f\n", Min);
	fprintf( stderr, "Max VolData = %f\n", Max);
	for (i=0;i<taille_buffer_vol;i++)
		buffer_vol[i] = (buffer_vol[i]-Min)/(Max-Min);

}

/*****************************************************************************
 * allocation de memoire pour le buffer de triangles
 *****************************************************************************/
void initBufferTriangles()
{
	nTmax = 8000000;
	do
	{
		nTmax /= 2;
		lT = (Triangle3D*)malloc(sizeof(Triangle3D)*nTmax);
	} while (lT == (Triangle3D*)NULL);
	printf("Mémoire allouée pour %d triangles\n", nTmax);

}

/*********************
 * infos au démarrage
 *********************/
void usage()
{
	printf("********** Marching tetrahedra **********\n");
	printf(" Rotations avec la souris (bouton gauche ou milieu enfoncé)\n");
	printf(" Touches clavier :\n");
	printf("  + - : modification de l'isovaleur \n");
	printf("        (entre 0.0 et 1.0 - pas de 0.02)\n");
	printf("  t   : diminution de la taille du cube élémentaire\n");
	printf("  y   : augmentation de la taille du cube élémentaire\n");
	printf("        (entre 1 et 40)\n");
	printf("  Z   : zoom avant \n");
	printf("  z   : zoom arrière \n");
	printf("  r   : reset (rotation, zoom, taille cube)\n");
	printf("  c   : copie d'écran dans le fichier 'image0.ppm'\n");
	printf("  q ESC : quitter\n\n");

}

/****************
 *
 * Programme principal
 *
 ****************/
int main(int argc, char **argv)
{
	/* infos des différentes touches */
	usage();

	/* création d'une mémoire pour stocker                  */
	/* les triangles de la surface iso-valeur               */
	/* -> tableau lT de dimension maximale nTmax            */
	/* la variable nT donne le nombre de triangles calculés */
	/* (cf. routine calculIsoSurface)                       */
	initBufferTriangles();

	/* création d'une grille de données 3D de dimensions (sx,sy,sz)  */
	/* -> tableau buffer_vol                                         */
	/* buffer_vol[i+sx*j+sx*sy*k] contient la valeur du sommet de    */
	/* coordonnées (i,j,k), la valeur entre un réel entre 0.0 et 1.0 */
	if (argc==1)
		creer_data_f_implicite();      /* données à partir d'une fonction */
	else
		lire_fichier_data(argc, argv); /* données à partir d'un fichier   */
	normalise_data();

	/*creation de la fenetre*/
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Marching Tetrahedra");

	/* volume englobant pour la vue 3D */
	glMatrixMode(GL_PROJECTION);
 	glOrtho(-(-xmin+xmax)*.7*eX, (-xmin+xmax)*.7*eX,
	        -(-ymin+ymax)*.7*eY, (-ymin+ymax)*.7*eY,
			-(-zmin+zmax)*10*eZ, (-zmin+zmax)*10*eZ);

	glMatrixMode(GL_MODELVIEW);

	/* initialisation des parametres de vue */
	initialisation();

	/* fonction affichage */
	glutDisplayFunc (display);

	/* gestion touche clavier */
	glutKeyboardFunc(clavier);

	/* function de gestion des moments où rien ne se passe */
	glutIdleFunc(display);

	/* fonctions pour la gestion de la traqueboule */
	glutMouseFunc(tbMouseFunc);
	glutMotionFunc(tbMotionFunc);

	 /* initialisation lumière et materiaux */
	initLumiere();
	initMatiere();
	glClearColor( 1., 1., 1., 0.);

	/* cache les elements invisibles */
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	/* boucle d'evenements GLUT */
	glutMainLoop();

	return 0;
}


