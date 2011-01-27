//////////////////////////////////////////////////////////////////////////////
// LIBRAIRIE XFIG 
// Interface                              
//////////////////////////////////////////////////////////////////////////////

#ifndef _XFIG_HPP
#define _XFIG_HPP

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>

/* défintion des constantes */

/* valeurs booleennes */
#define XFIG_FALSE 0
#define XFIG_TRUE  1

/* type d'objet */
#define XFIG_OBJECT_COLOR	 0
#define XFIG_OBJECT_ELLIPSE  1
#define XFIG_OBJECT_POLYLINE 2
#define XFIG_OBJECT_SPLINE   3
#define XFIG_OBJECT_TEXT	 4
#define XFIG_OBJECT_ARC 	 5
#define XFIG_OBJECT_COMPOUND 6

/* couleurs prédéfinies */
#define XFIG_COLOR_DEFAULT   -1
#define XFIG_COLOR_BLACK	  0
#define XFIG_COLOR_BLUE 	  1
#define XFIG_COLOR_GREEN	  2
#define XFIG_COLOR_CYAN 	  3
#define XFIG_COLOR_RED  	  4
#define XFIG_COLOR_MAGENTA    5
#define XFIG_COLOR_YELLOW	  6
#define XFIG_COLOR_WHITE	  7
#define XFIG_COLOR_BLUE1	  8
#define XFIG_COLOR_BLUE2	  9
#define XFIG_COLOR_BLUE3	 10
#define XFIG_COLOR_BLUE4	 11
#define XFIG_COLOR_GREEN1	 12
#define XFIG_COLOR_GREEN2	 13
#define XFIG_COLOR_GREEN3	 14
#define XFIG_COLOR_CYAN1	 15
#define XFIG_COLOR_CYAN2	 16
#define XFIG_COLOR_CYAN3	 17
#define XFIG_COLOR_RED1 	 18
#define XFIG_COLOR_RED2 	 19
#define XFIG_COLOR_RED3 	 10
#define XFIG_COLOR_MAGENTA1  21
#define XFIG_COLOR_MAGENTA2  22
#define XFIG_COLOR_MAGENTA3  23
#define XFIG_COLOR_BROWN1	 24
#define XFIG_COLOR_BROWN2	 25
#define XFIG_COLOR_BROWN3	 26
#define XFIG_COLOR_PINK1	 27
#define XFIG_COLOR_PINK2	 28
#define XFIG_COLOR_PINK3	 29
#define XFIG_COLOR_PINK4	 30
#define XFIG_COLOR_GOLD 	 31

/* déclaration des styles de lignes */
#define XFIG_LINESTYLE_DEFAULT            -1
#define XFIG_LINESTYLE_SOLID               0
#define XFIG_LINESTYLE_DASHED              1
#define XFIG_LINESTYLE_DOTTED              2
#define XFIG_LINESTYLE_DASH_DOTTED         3
#define XFIG_LINESTYLE_DASH_DOUBLE_DOTTED  4
#define XFIG_LINESTYLE_DASH_TRIPLE_DOTTED  5

/* déclaration des styles de remplissage */
#define XFIG_AREA_FILL_NONE -1
#define XFIG_AREA_FILL_BLACK     0
#define XFIG_AREA_FILL_BLACK100  0
#define XFIG_AREA_FILL_BLACK95   1
#define XFIG_AREA_FILL_BLACK90   2
#define XFIG_AREA_FILL_BLACK85   3
#define XFIG_AREA_FILL_BLACK80   4
#define XFIG_AREA_FILL_BLACK75   5
#define XFIG_AREA_FILL_BLACK70   6
#define XFIG_AREA_FILL_BLACK65   7
#define XFIG_AREA_FILL_BLACK60   8
#define XFIG_AREA_FILL_BLACK55   9
#define XFIG_AREA_FILL_BLACK50  10
#define XFIG_AREA_FILL_BLACK45  11
#define XFIG_AREA_FILL_BLACK40  12
#define XFIG_AREA_FILL_BLACK35  13
#define XFIG_AREA_FILL_BLACK30  14
#define XFIG_AREA_FILL_BLACK25  15
#define XFIG_AREA_FILL_BLACK20  16
#define XFIG_AREA_FILL_BLACK15  17
#define XFIG_AREA_FILL_BLACK10  18
#define XFIG_AREA_FILL_BLACK05  19
#define XFIG_AREA_FILL_BLACK5   19
#define XFIG_AREA_FILL_BLACK00  20
#define XFIG_AREA_FILL_BLACK0   20
#define XFIG_AREA_FILL_FULL     20
#define XFIG_AREA_FILL_WHITE00  20
#define XFIG_AREA_FILL_WHITE0   20
#define XFIG_AREA_FILL_WHITE05  21
#define XFIG_AREA_FILL_WHITE5   21
#define XFIG_AREA_FILL_WHITE10  22
#define XFIG_AREA_FILL_WHITE15  23
#define XFIG_AREA_FILL_WHITE20  24
#define XFIG_AREA_FILL_WHITE25  25
#define XFIG_AREA_FILL_WHITE30  26
#define XFIG_AREA_FILL_WHITE35  27
#define XFIG_AREA_FILL_WHITE40  28
#define XFIG_AREA_FILL_WHITE45  29
#define XFIG_AREA_FILL_WHITE50  30
#define XFIG_AREA_FILL_WHITE55  31
#define XFIG_AREA_FILL_WHITE60  32
#define XFIG_AREA_FILL_WHITE65  33
#define XFIG_AREA_FILL_WHITE70  34
#define XFIG_AREA_FILL_WHITE75  35
#define XFIG_AREA_FILL_WHITE80  36
#define XFIG_AREA_FILL_WHITE85  37
#define XFIG_AREA_FILL_WHITE90  38
#define XFIG_AREA_FILL_WHITE95  39
#define XFIG_AREA_FILL_WHITE100 40
#define XFIG_AREA_FILL_WHITE    40

/* type d'arc */
#define XFIG_SUB_TYPE_OPEN  	1
#define XFIG_SUB_TYPE_PIE_WEDGE 2

/* déclaration des styles de jonction */
#define XFIG_JOINSTYLE_MITER 0
#define XFIG_JOINSTYLE_ROUND 1
#define XFIG_JOINSTYLE_BEVEL 2

/* déclaration des "cap styles" */
#define XFIG_CAPSTYLE_BUTT       0
#define XFIG_CAPSTYLE_ROUND      1
#define XFIG_CAPSTYLE_PROJECTING 2

/* déclaration des styles de flèches */
#define XFIG_ARROWSTYLE_NONE 0

/* définition des justification de texte */
#define XFIG_JUSTIFY_LEFT   0
#define XFIG_JUSTIFY_CENTER 1
#define XFIG_JUSTIFY_RIGHT  2

/* déclaration des font-flags */
#define XFIG_FONTFLAG_RIGID  0
#define XFIG_FONTFLAG_LATEX  1
#define XFIG_FONTFLAG_PS     2
#define XFIG_FONTFLAG_HIDDEN 3

/* déclaration des fonts LaTeX */
#define XFIG_FONTLATEX_DEFAULT    0
#define XFIG_FONTLATEX_RM		  1
#define XFIG_FONTLATEX_BF		  2
#define XFIG_FONTLATEX_IT		  3
#define XFIG_FONTLATEX_SF		  4
#define XFIG_FONTLATEX_TT		  5
#define XFIG_FONTLATEX_ROMAN	  1
#define XFIG_FONTLATEX_BOLDFACE   2
#define XFIG_FONTLATEX_ITALIC	  3
#define XFIG_FONTLATEX_SANSSERIF  4
#define XFIG_FONTLATEX_TYPEWRITER 5

/* déclaration des fonts Postscript */
#define XFIG_POSTSCRIPT_DEFAULT  						   -1
#define XFIG_POSTSCRIPT_TIMES_ROMAN  					    0
#define XFIG_POSTSCRIPT_TIMES_ITALIC 					    1
#define XFIG_POSTSCRIPT_TIMES_BOLD						    2
#define XFIG_POSTSCRIPT_TIMES_BOLD_ITALIC				    3
#define XFIG_POSTSCRIPT_AVANTGARDE_BOOK  				    4
#define XFIG_POSTSCRIPT_AVANTGARDE_BOOK_OBLIQUE  		    5
#define XFIG_POSTSCRIPT_AVANTGARDE_DEMI  				    6
#define XFIG_POSTSCRIPT_AVANTGARDE_DEMI_OBLIQUE  		    7
#define XFIG_POSTSCRIPT_BOOKMAN_LIGHT					    8
#define XFIG_POSTSCRIPT_BOOKMAN_LIGHT_ITALIC 			    9
#define XFIG_POSTSCRIPT_BOOKMAN_DEMI 					   10
#define XFIG_POSTSCRIPT_BOOKMAN_DEMI_ITALIC  			   11
#define XFIG_POSTSCRIPT_COURIER  						   12
#define XFIG_POSTSCRIPT_COURIER_OBLIQUE  				   13
#define XFIG_POSTSCRIPT_COURIER_BOLD 					   14
#define XFIG_POSTSCRIPT_COURIER_BOLD_OBLIQUE 			   15
#define XFIG_POSTSCRIPT_HELVETICA						   16
#define XFIG_POSTSCRIPT_HELVETICA_OBLIQUE				   17
#define XFIG_POSTSCRIPT_HELVETICA_BOLD					   18
#define XFIG_POSTSCRIPT_HELVETICA_BOLD_OBLIQUE			   19
#define XFIG_POSTSCRIPT_HELVETICA_NARROW 				   10
#define XFIG_POSTSCRIPT_HELVETICA_NARROW_OBLIQUE 		   21
#define XFIG_POSTSCRIPT_HELVETICA_NARROW_BOLD			   22
#define XFIG_POSTSCRIPT_HELVETICA_NARROW_BOLD_OBLIQUE	   23
#define XFIG_POSTSCRIPT_NEW_CENTURY_SCHOOLBOOK_ROMAN 	   24
#define XFIG_POSTSCRIPT_NEW_CENTURY_SCHOOLBOOK_ITALIC	   25
#define XFIG_POSTSCRIPT_NEW_CENTURY_SCHOOLBOOK_BOLD  	   26
#define XFIG_POSTSCRIPT_NEW_CENTURY_SCHOOLBOOK_BOLD_ITALIC 27
#define XFIG_POSTSCRIPT_PALATINO_ROMAN					   28
#define XFIG_POSTSCRIPT_PALATINO_ITALIC  				   29
#define XFIG_POSTSCRIPT_PALATINO_BOLD					   30
#define XFIG_POSTSCRIPT_PALATINO_BOLD_ITALIC 			   31
#define XFIG_POSTSCRIPT_SYMBOL							   32
#define XFIG_POSTSCRIPT_ZAPF_CHANCERY_MEDIUM_ITALIC  	   33
#define XFIG_POSTSCRIPT_DINGBATS 						   34

/* types et variables de la librairie */
typedef int XFIG_BOOL;

extern int XFIG_resolution; // pt / inch
extern double XFIG_scale; // échelle 
extern int XFIG_line_style;
extern int XFIG_pen_color;
extern int XFIG_fill_color;
extern int XFIG_area_fill;
extern int XFIG_thickness;
extern int XFIG_depth;
extern int XFIG_join_style;
extern int XFIG_cap_style;
extern int XFIG_radius;
extern float XFIG_style_val;
extern XFIG_BOOL XFIG_with_forward_arrow;
extern XFIG_BOOL XFIG_with_backward_arrow;
extern int XFIG_forward_arrow_type;
extern int XFIG_forward_arrow_style;
extern int XFIG_backward_arrow_type;
extern int XFIG_backward_arrow_style;
extern float XFIG_forward_arrow_thickness;
extern float XFIG_forward_arrow_width;
extern float XFIG_forward_arrow_height;
extern float XFIG_backward_arrow_thickness;
extern float XFIG_backward_arrow_width;
extern float XFIG_backward_arrow_height;
extern XFIG_BOOL XFIG_upward_y;
extern int XFIG_text_justify;
extern float XFIG_text_angle;
extern float XFIG_font_size;
extern int XFIG_font;
extern int XFIG_font_flags;

/* changement de coordonnées pour prendre en compte 
 * l'échelle et la résolution 
 */
#define XFIG_COORD_R(v) \
 ((v)*XFIG_scale*XFIG_resolution)
#define XFIG_COORD_XR(v) \
 ((v)*XFIG_scale*XFIG_resolution)
#define XFIG_COORD_YR(v) \
 ((v)*XFIG_scale*XFIG_resolution*(XFIG_upward_y ? 1.0 : -1.0))
#define XFIG_COORD(v) \
 ((int)XFIG_COORD_R(v))
#define XFIG_COORD_X(v) \
 ((int)XFIG_COORD_XR(v))
#define XFIG_COORD_Y(v) \
 ((int)XFIG_COORD_YR(v))

/* code correspondant à une flèche à l'extrémité initiale */ 
void XFIG_forward_arrow();

/* code correspondant à une flèche à l'extrémité finale */ 
void XFIG_backward_arrow();

/* ouverture d'un fichier XFIG */
void XFIG_open(const char *nom);

/* fermeture d'un fichier XFIG */
void XFIG_close();

/* tracé d'un arc de cercle de centre (xc,yc) et rayon r
 * compris entre les angles start_angle et end_angle
 * angles exprimés en degré : 
 *  si start_angle>end_angle, sens clockwise
 *  si start_angle<end_angle, sens counter_clockwise
 */
void XFIG_arc(double xc, double yc, double r, 
  double start_angle, double end_angle, XFIG_BOOL closed);

/* tracé d'une ligne entre (x1,y1) et (x2,y2) */
void XFIG_line(double x1, double y1, double x2, double y2);

/* tracé d'une flèche entre (x1,y1) et (x2,y2), 
 * flèche à l'extrémité finale  
 */
void XFIG_arrow(double x1, double y1, double x2, double y2);

/* tracé d'une double flèche entre (x1,y1) et (x2,y2), 
 * flèches aux deux extrèmités 
 */
void XFIG_double_arrow(double x1, double y1, double x2, double y2);

/* tracé d'une polyline (ouverte) formée des n sommets x[i],y[i] */
void XFIG_polyline(double* x, double *y, int n);

/* tracé d'un polygone (fermé) formé des n sommets x[i],y[i] */
void XFIG_polygon(double* x, double *y, int n);

/* tracé d'un rectangle (fermé) entre les points (x1,y1) et (x2,y2) */
void XFIG_box(double x1, double y1, double x2, double y2);

/* tracé d'un rectangle (fermé) entre les points (x1,y1) et (x2,y2) 
 * et arrondi aux sommets
 */
void XFIG_arc_box(double x1, double y1, double x2, double y2);

/* tracé d'une ellipse de centre (x,y) et rayons (rx,ry) */
void XFIG_ellipse(double x, double y, double rx, double ry);

/* tracé d'un cercle de centre (x,y) et rayon r */
void XFIG_circle(double x, double y, double r);

/* tracé d'un texte en position (x,y) */
void XFIG_text(double x, double y, const char *text);
	 

/* export du fichier XFIG nommé name_fig 
 * dans le fichier EPS nommé name_eps  
 * avec le facteur d'échelle magnification (réel > 0.0)
 * La routine renvoie -1 en cas de pb, une autre valeur sinon
 */
int XFIG_export_EPS(const char *name_fig, const char *name_eps, 
  float magnification);

/* export du fichier XFIG nommé name_fig 
 * dans le fichier JPG nommé name_jpg  
 * avec le facteur d'échelle magnification (réel > 0.0) 
 * et la qualité q (entier entre 1 et 100)
 * La routine renvoie -1 en cas de pb, une autre valeur sinon
 */
int XFIG_export_JPG(const char *name_fig, const char *name_jpg, 
  float magnification, int q);

/* export du fichier XFIG nommé name_fig 
 * dans le fichier GIF nommé name_gif  
 * avec le facteur d'échelle magnification (réel > 0.0) 
 * La routine renvoie -1 en cas de pb, une autre valeur sinon
 */
int XFIG_export_GIF(const char *name_fig, const char *name_gif, 
  float magnification);

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
  double start_angle, double end_angle, XFIG_BOOL closed);

// tracé d'une ligne entre P1 et P2 
void XFIG_line(Point P1, Point P2);

// tracé d'une flèche entre P1 et P2, 
// flèche à l'extrémité finale  
void XFIG_arrow(Point P1, Point P2);

// tracé d'une double flèche entre P1 et P2, 
// flèches aux deux extrèmités 
void XFIG_double_arrow(Point P1, Point P2);

// tracé d'une polyline (ouverte) formée des n sommets P[i] 
void XFIG_polyline(Point* P, int n);

// tracé d'une polyline (ouverte) formée du vecteur de points P 
void XFIG_polyline(vPoint P);

// tracé d'un polygone (fermé) formé des n sommets P[i] 
void XFIG_polygon(Point* P, int n);

// tracé d'un polygone (fermé) formé des n sommets P[i] 
void XFIG_polygon(vPoint P);

// tracé d'un rectangle (fermé) entre les points P1 et P2 
void XFIG_box(Point P1, Point P2);

// tracé d'un rectangle (fermé) entre les points P1 et P2 
// et arrondi aux sommets
void XFIG_arc_box(Point P1, Point P2);

// tracé d'une ellipse de centre C=(x,y) et rayons (rx,ry) 
void XFIG_ellipse(Point C, double rx, double ry);

// tracé d'un cercle de centre C=(x,y) et rayon r 
void XFIG_circle(Point C, double r);

// tracé d'un texte en position P=(x,y) 
void XFIG_text(Point P, const char *text);

#endif
