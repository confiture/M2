//////////////////////////////////////////////////////////////////////////////
// LIBRAIRIE XFIG 
// Implementation                              
//////////////////////////////////////////////////////////////////////////////

#include"xfig.hpp"

/* variables de la librairie */
int XFIG_resolution = 1200; // pt / inch
double XFIG_scale = 1.0; // échelle 
FILE *XFIG_f;
int XFIG_line_style = XFIG_LINESTYLE_SOLID;
int XFIG_pen_color = XFIG_COLOR_BLACK;
int XFIG_fill_color = XFIG_COLOR_WHITE;
int XFIG_area_fill=XFIG_AREA_FILL_NONE;
int XFIG_thickness=1;
int XFIG_depth=50;
int XFIG_join_style=XFIG_JOINSTYLE_MITER;
int XFIG_cap_style=XFIG_CAPSTYLE_BUTT;
int XFIG_radius=7;
float XFIG_style_val=0.0;
XFIG_BOOL XFIG_with_forward_arrow=XFIG_FALSE;
XFIG_BOOL XFIG_with_backward_arrow=XFIG_FALSE;
int XFIG_forward_arrow_type=1;
int XFIG_forward_arrow_style=1;
int XFIG_backward_arrow_type=1;
int XFIG_backward_arrow_style=1;
float XFIG_forward_arrow_thickness=1.0;
float XFIG_forward_arrow_width=4.0;
float XFIG_forward_arrow_height=8.0;
float XFIG_backward_arrow_thickness=1.0;
float XFIG_backward_arrow_width=4.0;
float XFIG_backward_arrow_height=8.0;
XFIG_BOOL XFIG_upward_y = XFIG_FALSE;
int XFIG_text_justify = XFIG_JUSTIFY_LEFT;
float XFIG_text_angle = 0.0;
float XFIG_font_size = 12.0;
int XFIG_font = XFIG_POSTSCRIPT_TIMES_ROMAN;
int XFIG_font_flags = XFIG_FONTFLAG_PS;

/* code correspondant à une flèche à l'extrémité initiale */ 
void XFIG_forward_arrow()
{
	if (XFIG_with_forward_arrow)
	fprintf(XFIG_f,"%d %d %f %f %f\n",
	  XFIG_forward_arrow_type,
	  XFIG_forward_arrow_style,
	  XFIG_forward_arrow_thickness*XFIG_scale,
	  XFIG_forward_arrow_width*XFIG_resolution*XFIG_scale/80.0,
	  XFIG_forward_arrow_height*XFIG_resolution*XFIG_scale/80.0
	);	
}

/* code correspondant à une flèche à l'extrémité finale */ 
void XFIG_backward_arrow()
{
	if (XFIG_with_backward_arrow)
	fprintf(XFIG_f,"%d %d %f %f %f\n",
	  XFIG_backward_arrow_type,
	  XFIG_backward_arrow_style,
	  XFIG_backward_arrow_thickness*XFIG_scale,
	  XFIG_backward_arrow_width*XFIG_resolution*XFIG_scale/80.0,
	  XFIG_backward_arrow_height*XFIG_resolution*XFIG_scale/80.0
	);	
}

/* ouverture d'un fichier XFIG */
void XFIG_open(const char *nom)
{
	XFIG_f = fopen(nom, "w");
	fprintf(XFIG_f, "#FIG 3.2  Produced by xfig version 3.2.5\n");
	fprintf(XFIG_f, "Portrait\n");
	fprintf(XFIG_f, "Center\n");
	fprintf(XFIG_f, "Inches\n");
	fprintf(XFIG_f, "A4   \n");   
	fprintf(XFIG_f, "100.00\n");
	fprintf(XFIG_f, "Single\n");
	fprintf(XFIG_f, "-2\n");
	fprintf(XFIG_f, "%d 2\n", XFIG_resolution);
}

/* fermeture d'un fichier XFIG */
void XFIG_close()
{
	fclose(XFIG_f);
}

/* tracé d'un arc de cercle de centre (xc,yc) et rayon r
 * compris entre les angles start_angle et end_angle
 * angles exprimés en degré : 
 *  si start_angle>end_angle, sens clockwise
 *  si start_angle<end_angle, sens counter_clockwise
 */
void XFIG_arc(double xc, double yc, double r, 
  double start_angle, double end_angle, XFIG_BOOL closed)
{
	double x1,y1,x2,y2,x3,y3;
	
	/* calcul des angles pour que |start_angle-end_angle|<360 */
	while (start_angle>=end_angle+360.0) start_angle -= 360.0;
	while (start_angle<=end_angle-360.0) start_angle += 360.0;
	
	/* calcul des 3 points du cercle */
	x1 = xc+r*cos(start_angle*M_PI/180.0);
	y1 = xc+r*sin(start_angle*M_PI/180.0);
	x2 = xc+r*cos((start_angle+end_angle)*M_PI/90.0);
	y2 = xc+r*sin((start_angle+end_angle)*M_PI/90.0);
	x3 = xc+r*cos(end_angle*M_PI/180.0);
	y3 = xc+r*sin(end_angle*M_PI/180.0);
	
	fprintf(XFIG_f,"5 %d %d %d %d %d %d -1 %d %6.3f %d %d %d %d\n",
	 closed ? 1 : 2,
	 XFIG_line_style, 
	 XFIG_thickness, 
	 XFIG_pen_color, 
	 XFIG_fill_color, 
	 XFIG_depth, 
	 XFIG_area_fill, 
	 XFIG_style_val, 
	 XFIG_cap_style,
	 start_angle>=end_angle,
	 XFIG_with_forward_arrow,
	 XFIG_with_backward_arrow);
	fprintf(XFIG_f," %f %f  %d %d  %d %d  %d %d \n", 
	 XFIG_COORD_XR(xc), XFIG_COORD_YR(yc),
	 XFIG_COORD_X(x1), XFIG_COORD_Y(y1),
	 XFIG_COORD_X(x2), XFIG_COORD_Y(y2),
	 XFIG_COORD_X(x3), XFIG_COORD_Y(y3));
	XFIG_forward_arrow();
	XFIG_backward_arrow();
}

/* tracé d'une ligne entre (x1,y1) et (x2,y2) */
void XFIG_line(double x1, double y1, double x2, double y2)
{
	fprintf(XFIG_f,"2 1 %d %d %d %d %d -1 %d %6.3f %d %d -1 %d %d 2\n",
	 XFIG_line_style, 
	 XFIG_thickness, 
	 XFIG_pen_color, 
	 XFIG_fill_color, 
	 XFIG_depth, 
	 XFIG_area_fill, 
	 XFIG_style_val, 
	 XFIG_join_style, 
	 XFIG_cap_style,
	 XFIG_with_forward_arrow,
	 XFIG_with_backward_arrow);
	XFIG_forward_arrow();
	XFIG_backward_arrow();
	fprintf(XFIG_f," %d %d  %d %d\n", 
	 XFIG_COORD_X(x1), XFIG_COORD_Y(y1),
	 XFIG_COORD_X(x2), XFIG_COORD_Y(y2));
}

/* tracé d'une flèche entre (x1,y1) et (x2,y2), 
 * flèche à l'extrémité finale  
 */
void XFIG_arrow(double x1, double y1, double x2, double y2)
{
	/* sauvegarde des booleens with_arrow */
	XFIG_BOOL wfa = XFIG_with_forward_arrow  ;
	XFIG_BOOL wba = XFIG_with_backward_arrow ;
	
	XFIG_with_forward_arrow  = XFIG_TRUE;
	XFIG_with_backward_arrow = XFIG_FALSE;
	
	XFIG_line(x1, y1, x2, y2);
	 
	/* restauration des booleens with_arrow */
	XFIG_with_forward_arrow  = wfa ;
	XFIG_with_backward_arrow = wba ;
}

/* tracé d'une double flèche entre (x1,y1) et (x2,y2), 
 * flèches aux deux extrèmités 
 */
void XFIG_double_arrow(double x1, double y1, double x2, double y2)
{
	/* sauvegarde des booleens with_arrow */
	XFIG_BOOL wfa = XFIG_with_forward_arrow  ;
	XFIG_BOOL wba = XFIG_with_backward_arrow ;
	
	XFIG_with_forward_arrow  = XFIG_TRUE;
	XFIG_with_backward_arrow = XFIG_TRUE;
	
	XFIG_line(x1, y1, x2, y2);
	 
	/* restauration des booleens with_arrow */
	XFIG_with_forward_arrow  = wfa ;
	XFIG_with_backward_arrow = wba ;
}

/* tracé d'une polyline (ouverte) formée des n sommets x[i],y[i] */
void XFIG_polyline(double* x, double *y, int n)
{
	int i;
	
	fprintf(XFIG_f,"2 1 %d %d %d %d %d -1 %d %6.3f %d %d -1 %d %d %d\n",
	 XFIG_line_style, 
	 XFIG_thickness, 
	 XFIG_pen_color, 
	 XFIG_fill_color, 
	 XFIG_depth, 
	 XFIG_area_fill, 
	 XFIG_style_val, 
	 XFIG_join_style, 
	 XFIG_cap_style,
	 XFIG_with_forward_arrow,
	 XFIG_with_backward_arrow,
	 n);
	XFIG_forward_arrow();
	XFIG_backward_arrow();
	for (i=0; i<n; i++)
	{
		int Xi = XFIG_COORD_X(x[i]);
		int Yi = XFIG_COORD_Y(y[i]);
		fprintf(XFIG_f," %d %d ", Xi, Yi);
		if ((i%8)==7) fprintf(XFIG_f, "\n");
	}
	fprintf(XFIG_f, "\n");
}

/* tracé d'un polygone (fermé) formé des n sommets x[i],y[i] */
void XFIG_polygon(double* x, double *y, int n)
{
	int i;
	
	fprintf(XFIG_f,"2 3 %d %d %d %d %d -1 %d %6.3f %d %d -1 0 0 %d\n",
	 XFIG_line_style, 
	 XFIG_thickness, 
	 XFIG_pen_color, 
	 XFIG_fill_color, 
	 XFIG_depth, 
	 XFIG_area_fill, 
	 XFIG_style_val, 
	 XFIG_join_style, 
	 XFIG_cap_style,
	 n+1);
	for (i=0; i<=n; i++)
	{
		int Xi = XFIG_COORD_X(x[i%n]);
		int Yi = XFIG_COORD_Y(y[i%n]);
		fprintf(XFIG_f," %d %d ", Xi, Yi);
		if ((i%8)==7) fprintf(XFIG_f, "\n");
	}
	fprintf(XFIG_f, "\n");
}

/* tracé d'un rectangle (fermé) entre les points (x1,y1) et (x2,y2) */
void XFIG_box(double x1, double y1, double x2, double y2)
{
	int X1 = XFIG_COORD_X(x1);
	int X2 = XFIG_COORD_X(x2);
	int Y1 = XFIG_COORD_Y(y1);
	int Y2 = XFIG_COORD_Y(y2);
	fprintf(XFIG_f,"2 2 %d %d %d %d %d -1 %d %6.3f %d %d -1 0 0 5\n",
	 XFIG_line_style, 
	 XFIG_thickness, 
	 XFIG_pen_color, 
	 XFIG_fill_color, 
	 XFIG_depth, 
	 XFIG_area_fill, 
	 XFIG_style_val, 
	 XFIG_join_style, 
	 XFIG_cap_style);
	fprintf(XFIG_f," %d %d  %d %d  %d %d  %d %d  %d %d\n", 
	 X1, Y1, X1, Y2, X2, Y2, X2, Y1, X1, Y1);
}

/* tracé d'un rectangle (fermé) entre les points (x1,y1) et (x2,y2) 
 * et arrondi aux sommets
 */
void XFIG_arc_box(double x1, double y1, double x2, double y2)
{
	int X1 = XFIG_COORD_X(x1);
	int X2 = XFIG_COORD_X(x2);
	int Y1 = XFIG_COORD_Y(y1);
	int Y2 = XFIG_COORD_Y(y2);
	fprintf(XFIG_f,"2 4 %d %d %d %d %d -1 %d %6.3f %d %d %d 0 0 5\n",
	 XFIG_line_style, 
	 XFIG_thickness, 
	 XFIG_pen_color, 
	 XFIG_fill_color, 
	 XFIG_depth, 
	 XFIG_area_fill, 
	 XFIG_style_val, 
	 XFIG_join_style, 
	 XFIG_cap_style,
	 XFIG_radius);
	fprintf(XFIG_f," %d %d  %d %d  %d %d  %d %d  %d %d\n", 
	 X1, Y1, X1, Y2, X2, Y2, X2, Y1, X1, Y1);
}

/* tracé d'une ellipse de centre (x,y) et rayons (rx,ry) */
void XFIG_ellipse(double x, double y, double rx, double ry)
{
	int xe = XFIG_COORD_X(x);
	int ye = XFIG_COORD_Y(y);
	int rxe = XFIG_COORD(rx);
	int rye = XFIG_COORD(ry);
	
	fprintf(XFIG_f,"1 1 %d %d %d %d %d -1 %d %6.3f 1 0.0000 ",
	 XFIG_line_style, 
	 XFIG_thickness, 
	 XFIG_pen_color, 
	 XFIG_fill_color, 
	 XFIG_depth, 
	 XFIG_area_fill, 
	 XFIG_style_val);
	fprintf(XFIG_f," %d %d %d %d %d %d %d %d\n", 
	 xe, ye, rxe, rye, xe, ye, xe+rxe, ye+rye);
}

/* tracé d'un cercle de centre (x,y) et rayon r */
void XFIG_circle(double x, double y, double r)
{
	int xe = XFIG_COORD_X(x);
	int ye = XFIG_COORD_Y(y);
	int re = XFIG_COORD(r);
	
	fprintf(XFIG_f,"1 3 %d %d %d %d %d -1 %d %6.3f 1 0.0000 ",
	 XFIG_line_style, 
	 XFIG_thickness, 
	 XFIG_pen_color, 
	 XFIG_fill_color, 
	 XFIG_depth, 
	 XFIG_area_fill, 
	 XFIG_style_val);
	fprintf(XFIG_f," %d %d %d %d %d %d %d %d\n", 
	 xe, ye, re, re, xe, ye, xe+re, ye);
}

/* codage d'un caractère */
void XFIG_char_coding(unsigned char c, char *code)
{
	if (c>=32 && c<=127)
		/* laisser le caractère tel quel */
		sprintf(code,"%c",c);
	else
	{
		/* coder le caractère en octal sous la forme \xxx */
		int i;
		code[0] = '\\';
		for (i=0; i<3; i++)
		{
			code[3-i] = 48+c%8;
			c /= 8;
		}	
		code[4] = 0;
	}
}


/* tracé d'un texte en position (x,y) */
void XFIG_text(double x, double y, const char *text)
{
	unsigned int i;
	int xe = XFIG_COORD_X(x);
	int ye = XFIG_COORD_Y(y);
	fprintf(XFIG_f,"4 %d %d %d -1 %d %f %f %d %f %f %d %d ",
	 XFIG_text_justify, 
	 XFIG_pen_color, 
	 XFIG_depth, 
	 XFIG_font,
	 XFIG_font_size*XFIG_scale,
	 XFIG_text_angle,
	 XFIG_font_flags,
	 XFIG_font_size*XFIG_scale/3.0,
	 XFIG_font_size*XFIG_scale*30,
	 xe,
	 ye);
	for (i=0; i<strlen(text); i++)
	{
		char code[5];
		XFIG_char_coding((unsigned char)text[i], code);
		fprintf(XFIG_f,"%s", code);
	}
	fprintf(XFIG_f,"\\001\n");
}
	 

/* export du fichier XFIG nommé name_fig 
 * dans le fichier EPS nommé name_eps  
 * avec le facteur d'échelle magnification (réel > 0.0)
 * La routine renvoie -1 en cas de pb, une autre valeur sinon
 */
int XFIG_export_EPS(const char *name_fig, const char *name_eps, 
  float magnification)
{
	char commande[1000];
	
	if (magnification <=0.0) magnification = 1.0;
	sprintf(commande, "fig2dev -L eps -m %f %s %s", 
	  magnification, name_fig, name_eps);
	return system(commande);
}

/* export du fichier XFIG nommé name_fig 
 * dans le fichier JPG nommé name_jpg  
 * avec le facteur d'échelle magnification (réel > 0.0) 
 * et la qualité q (entier entre 1 et 100)
 * La routine renvoie -1 en cas de pb, une autre valeur sinon
 */
int XFIG_export_JPG(const char *name_fig, const char *name_jpg, 
  float magnification, int q)
{
	char commande[1000];
	
	if (magnification <=0.0) magnification = 1.0;
	if (q<1) q=1;
	if (q>100) q=100;
	
	sprintf(commande, "fig2dev -L jpeg -m %f -q %d %s %s", 
	  magnification, q, name_fig, name_jpg);
	return system(commande);
}

/* export du fichier XFIG nommé name_fig 
 * dans le fichier GIF nommé name_gif  
 * avec le facteur d'échelle magnification (réel > 0.0) 
 * La routine renvoie -1 en cas de pb, une autre valeur sinon
 */
int XFIG_export_GIF(const char *name_fig, const char *name_gif, 
  float magnification)
{
	char commande[1000];
	
	if (magnification <=0.0) magnification = 1.0;
	
	sprintf(commande, "fig2dev -L gif -m %f %s %s", 
	  magnification, name_fig, name_gif);
	return system(commande);
}

//////////////////////////////////////////////////////////////////////////////
// REDEFINITION DES ROUTINES AVEC LE TYPE POINT
//////////////////////////////////////////////////////////////////////////////
#include"lib_base2d.hpp"

// tracé d'un arc de cercle de centre C et rayon r
// compris entre les angles start_angle et end_angle
// angles exprimés en degré : 
//  si start_angle>end_angle, sens clockwise
//  si start_angle<end_angle, sens counter_clockwise
void XFIG_arc(Point C, double r, 
  double start_angle, double end_angle, XFIG_BOOL closed)
{
	XFIG_arc(C.x, C.y, r, start_angle, end_angle, closed);
}

// tracé d'une ligne entre P1 et P2 
void XFIG_line(Point P1, Point P2)
{
	XFIG_line(P1.x, P1.y, P2.x, P2.y);
}

// tracé d'une flèche entre P1 et P2, 
// flèche à l'extrémité finale  
void XFIG_arrow(Point P1, Point P2)
{
	XFIG_arrow(P1.x, P1.y, P2.x, P2.y);
}

// tracé d'une double flèche entre P1 et P2, 
// flèches aux deux extrèmités 
void XFIG_double_arrow(Point P1, Point P2)
{
	XFIG_double_arrow(P1.x, P1.y, P2.x, P2.y);
}

// tracé d'une polyline (ouverte) formée des n sommets P[i] 
void XFIG_polyline(Point* P, int n)
{
	double x[n], y[n];
	for (int i=0; i<n; i++)
	{
		x[i] = P[i].x;
		y[i] = P[i].y;
	}
	XFIG_polyline(x, y, n);
}

// tracé d'une polyline (ouverte) formée du vecteur de points P 
void XFIG_polyline(vPoint P)
{
	XFIG_polyline(&P[0], (int)P.size());
}

// tracé d'un polygone (fermé) formé des n sommets P[i] 
void XFIG_polygon(Point* P, int n)
{
	double x[n], y[n];
	for (int i=0; i<n; i++)
	{
		x[i] = P[i].x;
		y[i] = P[i].y;
	}
	XFIG_polygon(x, y, n);
}

// tracé d'un polygone (fermé) formé des n sommets P[i] 
void XFIG_polygon(vPoint P)
{
	XFIG_polygon(&P[0], (int)P.size());
}

// tracé d'un rectangle (fermé) entre les points P1 et P2 
void XFIG_box(Point P1, Point P2)
{
	XFIG_box(P1.x, P1.y, P2.x, P2.y);
}

// tracé d'un rectangle (fermé) entre les points P1 et P2 
// et arrondi aux sommets
void XFIG_arc_box(Point P1, Point P2)
{
	XFIG_arc_box(P1.x, P1.y, P2.x, P2.y);
}

// tracé d'une ellipse de centre C=(x,y) et rayons (rx,ry) 
void XFIG_ellipse(Point C, double rx, double ry)
{
	XFIG_ellipse(C.x, C.y, rx, ry);
}

// tracé d'un cercle de centre C=(x,y) et rayon r 
void XFIG_circle(Point C, double r)
{
	XFIG_circle(C.x, C.y, r);
}

// tracé d'un texte en position P=(x,y) 
void XFIG_text(Point P, const char *text)
{
	XFIG_text(P.x, P.y, text);
}

