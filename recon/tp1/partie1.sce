///////////////////////////////////////////////////////////////////////////////
//
// TP 1 - Reconstruction de courbes (1)
//
// scripts des exemples
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// {Donn�es param�tr�es}

//-----------------------------------------------------------------------------
// {Mod�le polynomial}

exec("polynomes.sci");
p = [1,-2,-3,2];
xtrace = linspace(-1.5,3,1000);
ytrace = polyval(p,xtrace);
plot(xtrace,ytrace)

//-----------------------------------------------------------------------------
// {Interpolation}

exec("polynomes.sci");
// les points de donn�es
t = [0,1,3];
y = [2,1,5];

// calcul du polynome interpolant
p = polyfit(t,y);

// evaluation et trac� du polynome
xtrace = linspace(0,3,1000);
ytrace = polyval(p,xtrace); 
scf(); // nouvelle figure
plot(t,y,'ro');           // points de donn�es en rouge
plot(xtrace,ytrace,'k-'); // polynome interpolant en noir
a = gca();
a.data_bounds = [-1,0;4,6]; // modifier les bornes du graphique


//-----------------------------------------------------------------------------
// {Approximation}

exec("polynomes.sci");

// les donn�es
t = [0 1 2 3 4 5 6]
y = [0 1 1 2 2 3 3]

// approximation par un polynome de degr� 1
p = polyfit(t,y,1) 

// trac� 
xtrace = linspace(0,6,1000);
ytrace = polyval(p,xtrace); 
scf();
plot(t,y,'ro');           // points de donn�es en rouge
plot(xtrace,ytrace,'k-'); // polynome interpolant en noir
a = gca();
a.data_bounds = [-1,-1;7,4]; // modifier les bornes du graphique


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// {Mod�le polynomial par morceaux}

//-----------------------------------------------------------------------------
// {Mod�le spline}

// spline interpolant les donn�es (ti, yi = f(ti) = 1/(1+ti*ti)) avec 
// (t_i) = [-9 -5 -3 -2 0 1 2 4 7 9]

// les donnees
deff('[y]=f(t)','y=ones(t)./(1+t.*t)');
t = [-9 -5 -3 -2 0 1 2 4 7 9];
y = f(t);

// calcul de la spline
d = splin(t,y);

// �valuation de la spline pour des abscisses entre tmin et tmax
xtrace = linspace(min(t),max(t),1000);
ytrace = interp(xtrace, t, y, d);

// trac� de la spline et des points de donn�es
scf();
plot(t,y,'ro');           // points de donn�es en rouge
plot(xtrace,ytrace,'k-'); // spline interpolante en noir
 
//-----------------------------------------------------------------------------
// {Spline p�riodique}

// les donn�es avec y(n)=y(1)
t = [0 1 2 3 4 5];
y = [1 3 4 0 1 1];
n = length(t);
T = t(n)-t(1); // la p�riode

// calcul des deux splines p�riodiques
d1 = splin(t,y,"periodic");
d2 = splin(t,y,"fast_periodic");

// trac� des deux splines
xtrace = linspace(t(1),t(n),1000);
ytrace1 = interp(xtrace,t,y,d1); 
ytrace2 = interp(xtrace,t,y,d2); 
scf();
plot(t,y,'ro');            // points de donn�es en rouge
plot(xtrace,ytrace1,'k-'); // spline 1 en noir
plot(xtrace,ytrace2,'m-'); // spline 2 en magenta

// Afin de v�rifier graphiquement la p�riodicit�
// (raccord $C^2$ aux bords), tracer la spline p�riodique sur
// deux p�riodes suppl�mentaires (une � gauche, une � droite) :

plot(xtrace-T,ytrace1,'k-'); // spline 1 en noir � gauche
plot(xtrace+T,ytrace1,'k-'); // spline 1 en noir � droite
plot(xtrace-T,ytrace2,'m-'); // spline 2 en magenta � gauche
plot(xtrace+T,ytrace2,'m-'); // spline 2 en magenta � droite


//-----------------------------------------------------------------------------
// {Approximation par une spline}

// les donn�es
t = 1:100;
y = sin(t/10); // point sur une sinusoide 
z = y+rand(t)*0.2-0.1;  // ajout d'un bruit (erreur)

// calcul de la spline
tau = linspace(t(1),t($),5); // noeuds de la spline : 
                              // 5 valeurs entre tmin et tmax
[ys,ds] = lsq_splin(t,z,tau);
xtrace = linspace(t(1),t($),1000);
ytrace = interp(xtrace,tau,ys,ds); 

// trac�
scf();
plot(t,z,'r+');           // les points de donn�es en rouge
plot(xtrace,ytrace,'k-'); // spline  en noir


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// {Courbes param�tr�es}

//-----------------------------------------------------------------------------
// {Trac� d'une courbe (x(t), y(t))}


t=linspace(0,2*%pi,1000);
x=cos(t); y=sin(t);
scf();
plot(x,y);
a = gca();
a.isoview = "on";

//M�me principe dans l'espace :
// une h�lice
t=linspace(0,2*pi,1000);
x=cos(3*t); y=sin(3*t); z=t/3;
scf();
param3d(x,y,z);

// changement des parametres graphiques de la courbe
h = gce();
h.foreground = 2; // couleur bleue
h.thickness = 2; // �paisseur de la courbe
a = gca();
a.isoview = "on";

//-----------------------------------------------------------------------------
// {Interpolation de points}

t=[0 1 2 3];
x=[0 1 1 0];
y=[0 0 1 1];

// en utilisant l'interpolation polynomiale simple :
exec("polynomes.sci");
px = polyfit(t,x);
py = polyfit(t,y);

ttrace=linspace(t(1),t($),1000);
xtrace=polyval(px,ttrace);
ytrace=polyval(py,ttrace);

scf();
plot(x,y,'ro'); // les points de donn�es
plot(x,y,'g--'); // relier les points entre eux -> polygone vert
plot(xtrace,ytrace,'k-'); // la courbe interpolante
a = gca();
a.data_bounds(1)=-0.2; // modifier la borne min en x du rep�re
a.isoview = "on";

//-----------------------------------------------------------------------------
// {Comparaison de proc�d�s d'interpolation}

dx = splin(t,x,"clamped",[1 -1]); // deriv�es horizontales
dy = splin(t,y,"clamped",[0  0]); // au bord

ttrace=linspace(t(1),t($),1000);
xtrace=interp(ttrace,t,x,dx);
ytrace=interp(ttrace,t,y,dy);

scf();
plot(x,y,'ro'); // les points de donn�es
plot(x,y,'g--'); // relier les points entre eux -> polygone vert
plot(xtrace,ytrace,'k-'); // la courbe interpolante
a = gca();
a.data_bounds(1)=-0.2; // modifier la borne min en x du rep�re
a.isoview = "on";

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// {Mod�le implicite}


//----------------------------------------------------------
// {Interpolation de 3 points par un cercle}

// les donn�es avec tableau sous forme de vecteur-colonne
x = [-1 ; 0 ; 1];
y = [ 0 ; 1 ; 0];

// calcul des coefficients
A = [x , y , ones(3,1)]; b = -(x.^2+y.^2);
c = A\b;

// calcul du centre et du rayon
xC = -c(1)/2; yC = -c(2)/2; 
r = sqrt(xC^2+yC^2-c(3));

// trac� du cercle
scf();
t = linspace(0,2*%pi,1000);
plot(xC+r*cos(t), yC+r*sin(t), 'k-'); // le cercle
plot(x,y,'ro');                       // les points
a = gca();
a.isoview = "on";

// affichage de la fonction F
mprintf('\nF(x,y)=(x^2+y^2) %+f x %+f y %+f\n', c(1), c(2), c(3));
mprintf('Centre = ( %f , %f )\n', xC, yC);
mprintf('Rayon  = %f\n', r);

//-----------------------------------------------------------------------------
// {Cercle aux moindres carr�s}

function [C,r] = cercle_mc(x,y)

n = length(x);

A = [sum(x.^2) , sum(x.*y) , sum(x) ; ...
     sum(x.*y) , sum(y.^2) , sum(y) ; ...
     sum(x)    , sum(y)    , n];
b = -[sum(x.*(x.^2+y.^2)) ; ...
      sum(y.*(x.^2+y.^2)) ; ...
      sum(   (x.^2+y.^2))];
c = A\b;

// calcul du centre et du rayon
xC = -c(1)/2; yC = -c(2)/2; 
r = sqrt(xC^2+yC^2-c(3));
C = [xC;yC];

endfunction

// a) points proches d'un cercle en entier :
t=linspace(0,%pi*2,200);
x=cos(t)+rand(t)*0.4-0.2;
y=sin(t)+rand(t)*0.4-0.2;
[C1,r1] = cercle_mc(x,y);
scf();
plot(x ,y, 'r+'); // les points en rouge
tc = linspace(0,2*%pi,1000); // param�trage du cercle complet
plot(C1(1)+r1*cos(tc), C1(2)+r1*sin(tc), 'k-'); // le cercle en noir
set(gca(),'isoview','on');

// b) points proches d'un demi-cercle :
t=linspace(0,%pi,200);
x=cos(t)+rand(t)*0.4-0.2;
y=sin(t)+rand(t)*0.4-0.2;

// c) points proches d'un quart de cercle :
t=linspace(0,%pi/2,200);
x=cos(t)+rand(t)*0.4-0.2;
y=sin(t)+rand(t)*0.4-0.2;

