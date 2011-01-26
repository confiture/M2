/////////////////////////////////////////////////////////////////////////////
// interpolation de 2 courbes par le sch�ma de Hermite
// S(u,v) = H0(v) C1(u) + H1(v) T1(u) + H2(v) T2(u) + H3(v) C2(u)
function main()

  // le param�trage
  u=linspace(0,1,101);
  v=linspace(0,1,101);
  
  // les 2 courbes fronti�res
  C1 = [zeros(u);cos((2*u+1)*%pi/4);sin((2*u+1)*%pi/4)];
  C2 = [sin((2*u+1)*%pi/4);cos((2*u+1)*%pi/4);zeros(u)];
  
  // les d�riv�es transverses
  a=0; b=0; c=0; d=0;
  T1 = [ones(u);a*ones(u);b*ones(u)];
  T2 = [c*ones(u);d*ones(u);-ones(u)];
  
  // la surface interpolant les courbes et les d�riv�es transverses
  /////////////////////////////
  // A COMPLETER 
  /////////////////////////////
  
  // sortie graphique au format Geomview
  
  // les deux courbes
  fid = mopen("C1.vect","w");
  write_VECT(fid,C1(1,:),C1(2,:),C1(3,:),[1,0,0],2);
  mclose(fid);
  fid = mopen("C2.vect","w");
  write_VECT(fid,C2(1,:),C2(2,:),C2(3,:),[0,0,1],2);
  mclose(fid);
  
  // la surface interpolante
  fid = mopen("surface.mesh","w");
  write_MESH(fid,XS,YS,ZS,[1,0.8,0.6]);
  mclose(fid);
  trace_isoparametrique("surface_iso.vect", XS,YS,ZS, 11, 3, [0 0 0]);
  
  // les d�riv�es transverses
  ind=1:2:101;
  h=0.2;l=1;
  fid = mopen("T1.vect","w");
  write_VECTS(fid,C1(:,ind),T1(:,ind),[1,0.5,0.5],h,l);
  mclose(fid);
  fid = mopen("T2.vect","w");
  write_VECTS(fid,C2(:,ind),T2(:,ind),[0.5,0.5,1],h,l);
  mclose(fid);
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

// objet VECT : 1 polyligne avec n sommets et 1 couleur
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
// ecriture d'un objet de type VECT 
// repr�sentant un muage de points 
// dans le fichier dont l'identificateur est fid
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          x,y,z = les points 
//          c = couleur RGB
//          l = �paisseur de la ligne
function write_points_VECT(fid,x,y,z,c,l);

// d�but de l'objet composite
mfprintf(fid,"{\n");

// attribut pour l'�paisseur
mfprintf(fid,"  appearance {linewidth %d}\n",l);

// objet VECT : n poyligne avec chacune 1 sommet et 1 couleur
n=length(x);
mfprintf(fid,"{\n");

// l'entete
mfprintf(fid,"VECT\n %d %d %d\n", n, n, n);

// nombre de points par ligne
for i=1:n
	mfprintf(fid," 1");
end
mfprintf(fid,"\n");

// nombre de couleurs par ligne
for i=1:n
	mfprintf(fid," 1");
end
mfprintf(fid,"\n");

// les n sommets
for i=1:n
	mfprintf(fid,"%15.7e %15.7e %15.7e \n", x(i), y(i), z(i));
end

// les couleur
for i=1:n
  mfprintf(fid,"%15.7e %15.7e %15.7e 1\n", c(1), c(2), c(3));
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


