// interpolation produit tensoriel - proc�d� de Hermite
// on d�finit pour les 4 sommets P00, P01, P10 et P11
//   les coordonn�es Pij
//   les d�riv�es premi�res DPuij et DPvij 
//   les d�riv�es secondes crois�es (twists) DDPuvij  
function main()

// on d�finit pour les 4 sommets P00, P01, P10 et P11
//   les coordonn�es Pij
P00 = [-1;-1;0]; 
P01 = [-1; 1;0];
P10 = [ 1;-1;0];
P11 = [ 1; 1;0];
//   les d�riv�es premi�res DPuij et DPvij 
DPu00 = [2;-1;-1]; DPv00 = [-1;2;-1]; 
DPu01 = [2; 1;-1]; DPv01 = [ 1;2; 1]; 
DPu11 = [2;-1; 1]; DPv11 = [-1;2; 1]; 
DPu10 = [2; 1; 1]; DPv10 = [ 1;2;-1];
//DPu00 = [2;-1;-1]; DPv00 = [-1;2;-1]; 
//DPu01 = [2; 1; 1]; DPv01 = [ 1;2; -1]; 
//DPu11 = [2; 1; 1]; DPv11 = [ 1;2; 1]; 
//DPu10 = [2; 1; 1]; DPv10 = [ 1;2;-1];
//   les d�riv�es secondes crois�es (twists) DDPuvij  
//DDPuv00 = [0;0;-2];
//DDPuv01 = [0;0;1];
//DDPuv10 = [0;0;3];
//DDPuv11 = [0;0;-4];
DDPuv00 = [42;0;2];
DDPuv01 = [23;0;1];
DDPuv10 = [0;17;3];
DDPuv11 = [0;0;4];
  
// la discr�tisation en u et v
nu = 101;
u = linspace(0,1,nu);
nv = 101;
v = linspace(0,1,nv);

// les fonctions de Hermite en u et v
H0u =  2*u.^3 -3*u.^2 +1;
H1u =  u.^3 -2*u.^2 +u;
H2u =  u.^3 -u.^2;
H3u = -2*u.^3 +3*u.^2;
H0v =  2*v.^3 -3*v.^2 +1;
H1v =  v.^3 -2*v.^2 +v;
H2v =  v.^3 -v.^2;
H3v = -2*v.^3 +3*v.^2;

// l'interpolation produit tensoriel coordonn�e par coordonn�e
// -> points de la surface S = (Sx(i,j), Sy(i,j), Sz(i,j)) 
// 1 <= i <= nu et 1 <= j <= nv
Sx = [H0u' H1u' H2u' H3u'] * ...
     [P00(1)     DPv00(1)   DPv01(1)   P01(1)    ; ...
      DPu00(1)   DDPuv00(1) DDPuv01(1) DPu01(1)  ; ...
      DPu10(1)   DDPuv10(1) DDPuv11(1) DPu11(1)  ; ...
      P10(1)     DPv10(1)   DPv11(1)   P11(1)    ]* ....
     [H0v;H1v;H2v;H3v];
Sy = [H0u' H1u' H2u' H3u'] * ...
     [P00(2)     DPv00(2)   DPv01(2)   P01(2)    ; ...
      DPu00(2)   DDPuv00(2) DDPuv01(2) DPu01(2)  ; ...
      DPu10(2)   DDPuv10(2) DDPuv11(2) DPu11(2)  ; ...
      P10(2)     DPv10(2)   DPv11(2)   P11(2)    ]* ....
     [H0v;H1v;H2v;H3v];
Sz = [H0u' H1u' H2u' H3u'] * ...
     [P00(3)     DPv00(3)   DPv01(3)   P01(3)    ; ...
      DPu00(3)   DDPuv00(3) DDPuv01(3) DPu01(3)  ; ...
      DPu10(3)   DDPuv10(3) DDPuv11(3) DPu11(3)  ; ...
      P10(3)     DPv10(3)   DPv11(3)   P11(3)    ]* ....
     [H0v;H1v;H2v;H3v];

// trac� des donn�es
fid = mopen("points.sphere","w");
mfprintf(fid, "{LIST\n");
write_SPHERE(fid, P00(1),P00(2),P00(3),0.05);
write_SPHERE(fid, P01(1),P01(2),P01(3),0.05);
write_SPHERE(fid, P10(1),P10(2),P10(3),0.05);
write_SPHERE(fid, P11(1),P11(2),P11(3),0.05);
mfprintf(fid, "}\n");
mclose(fid);
  
// trac� des d�riv�es premi�res et des twists
fid = mopen("derivees.vect","w");
mfprintf(fid, "{LIST\n");
// d�riv�es premi�res / u en rouge
write_VECTS(fid, P00,DPu00,[1 0 0],1,1);
write_VECTS(fid, P01,DPu01,[1 0 0],1,1);
write_VECTS(fid, P10,DPu10,[1 0 0],1,1);
write_VECTS(fid, P11,DPu11,[1 0 0],1,1);
// d�riv�es premi�res / v en bleu
write_VECTS(fid, P00,DPv00,[0 0 1],1,1);
write_VECTS(fid, P01,DPv01,[0 0 1],1,1);
write_VECTS(fid, P10,DPv10,[0 0 1],1,1);
write_VECTS(fid, P11,DPv11,[0 0 1],1,1);
// twists en vert
write_VECTS(fid, P00,DDPuv00,[0 1 0],1,1);
write_VECTS(fid, P01,DDPuv01,[0 1 0],1,1);
write_VECTS(fid, P10,DDPuv10,[0 1 0],1,1);
write_VECTS(fid, P11,DDPuv11,[0 1 0],1,1);
mfprintf(fid, "}\n");
mclose(fid);

// trac� de la surface interpolante
fid = mopen("surface.mesh","w");
write_MESH(fid, Sx,Sy,Sz,[1.0 0.8 0.6]);
mclose(fid);

// trac� de quelques isoparam�triques en u et v
trace_isoparametrique("surface_isoparam.vect", Sx,Sy,Sz, 11, 3, [0 0 0]);

endfunction


///////////////////////////////////////////////////////////////////////
// Routines pour les sorties graphiques au format Geoview
///////////////////////////////////////////////////////////////////////

// trac� de courbes isoparametrique � partir de donn�es grille (x,y,z)
// Entree : nom_f = nom du fichier Geomview
//          x,y,z = donnees grille (matrices MxN)
//          nIso = nb d'isoparametriques � tracer suivant une direction
//          direction = direction pour le trac� des isoparam�triques
//            1 : trac� suivant la direction 1
//                une isoparametrique : points (x(:,j),y(:,j),z(:,j))
//            2 : trac� suivant la direction 2
//                une isoparametrique : points (x(i,:),y(i,:),z(i,:))
//            3 : trac� suivant la direction 1 et la direction 2
//          couleur = triplet (r,g,b)
function trace_isoparametrique(nom_f, x,y,z, nIso, direction, couleur)

  [d1,d2] = size(x);
  f = mopen(nom_f, "w");
  mfprintf(f,"{\nLIST\n");
  if direction==1 | direction == 3
    // trac� suivant la direction 1
    dIso = floor((d2-1)/(nIso-1));
    for j=1:dIso:d2
      write_VECT(fid,x(:,j),y(:,j),z(:,j),couleur,1);
    end    
  end
  if direction==2 | direction == 3
    // trac� suivant la direction 2
    dIso = floor((d1-1)/(nIso-1));
    for i=1:dIso:d1
      write_VECT(fid,x(i,:),y(i,:),z(i,:),couleur,1);
    end    
  end
  
  mfprintf(f,"}\n");
  mclose(f);
endfunction

  
//----------------------------------------------
// ecriture d'un objet de type VECT dans le fichier 
// dont l'identificateur est fid
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          x,y,z = les points de la polyligne
//          c = couleur RGB
//          l = �paisseur de la ligne
function write_VECT(fid,x,y,z,c,l);

// d�but de l'objet composite
mfprintf(fid,"{\n");

// attribut pour l'�paisseur
mfprintf(fid,"  appearance {linewidth %d}\n",l);

// objet VECT : 1 poyligne avec n sommets et 1 couleur
n=length(x);
mfprintf(fid,"{\n");

// l'entete
mfprintf(fid,"VECT\n 1 %d 1\n %d\n 1\n", n, n);
// les n sommets
for i=1:n
	mfprintf(fid,"%15.7e %15.7e %15.7e \n", x(i), y(i), z(i));
end
// la couleur
mfprintf(fid,"%15.7e %15.7e %15.7e 1\n", c(1), c(2), c(3));
mfprintf(fid,"}\n");

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet de type VECT dans le fichier 
// dont l'identificateur est fid
// objet form� de n vecteurs [P(:,i),P(:,i)+h*V(:,i)]
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          P = les points de la polyligne
//          V = vecteurs associ�s
//          c = couleur RGB
//          h = homothetie pour le vecteur
//          l = �paisseur de la ligne
function write_VECTS(fid,P,V,c,h,l);

// d�but de l'objet composite
mfprintf(fid,"{\n");

// attribut pour l'�paisseur
mfprintf(fid,"  appearance {linewidth %d}\n",l);

// objet VECT : 1 poyligne avec n sommets et 1 couleur
n=size(P,2);
mfprintf(fid,"{\n");

// l'entete
mfprintf(fid,"VECT\n %d %d %d\n", n, 2*n, n);

// nb de points par polyline
for i=1:n
  mfprintf(fid," 2");
end
mfprintf(fid,"\n");

// nb de couleurs par polyline
for i=1:n
  mfprintf(fid," 1");
end
mfprintf(fid,"\n");

// les n vecteurs
for i=1:n
	mfprintf(fid,"%15.7e %15.7e %15.7e \n", ...
	  P(1,i), P(2,i), P(3,i));
	mfprintf(fid,"%15.7e %15.7e %15.7e \n", ...
	  P(1,i)+h*V(1,i), P(2,i)+h*V(2,i), P(3,i)+h*V(3,i));
end

// les n couleurs
for i=1:n
  mfprintf(fid,"%8.5f %8.5f %8.5f 1\n", c(1), c(2), c(3));
end
mfprintf(fid,"}\n");

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet MESH
function write_MESH(fid,X,Y,Z,c)

n1 = size(X,1);
n2 = size(X,2);

// d�but de l'objet composite
mfprintf(fid,"{\n");

mfprintf(fid, 'CMESH\n%d %d\n', n2, n1);
for i=1:n1
for j=1:n2
    mfprintf(fid, '%15.7e %15.7e %15.7e  %8.5f %8.5f %8.5f 1\n', ...
	  X(i,j) , Y(i,j) , Z(i,j), c(1), c(2), c(3));
end
end

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet SPHERE de centre (x,y,z) et rayon r
function write_SPHERE(fid,x,y,z,r)

// d�but de l'objet composite
mfprintf(fid,"{\n");

mfprintf(fid, 'SPHERE\n%15.7e %15.7e %15.7e %15.7e\n', r, x, y, z);

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------


