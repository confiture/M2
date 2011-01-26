//////////////////////////////////////////////////////////////////////////////////
// interpolation spline radiale
// données de la forme paramètres (u(i),v(i)) et points (x(i)=u(i),y(i)=v(i),z(i))
function main()

  // les données
  u = [  
  - 0.9487410  
  - 0.2556054  
    1.644218   
    1.2330669  
    1.2410612  
  - 0.9638286  
  - 0.3443651  
  - 0.5600289  
    0.7651152  
    1.0627435  
  - 0.7355708  
    0.1220763  
    0.2860699  
  - 1.8087938  
    1.2994479  
    0.3195372  
  - 0.8832767  
    1.8180444  
    1.6284619  
  - 0.6559403  
  ]';
  v = [  
  - 0.5709401  
    1.0773599  
    0.1910536  
  - 1.6150845  
    1.8244687  
  - 1.1170366  
  - 1.9426963  
    1.2765959  
  - 1.4780029  
    1.8728014  
  - 1.5297548  
    1.7014895  
    0.9054685  
    1.6037992  
  - 0.4204027  
    0.2620719  
    0.8245959  
    0.7151325  
  - 0.3468255  
  - 1.4390837  
  ]';
  x = u;
  y = v;
  z = exp(-u.*u-v.*v)+0.5;
  
  // grille de points où évaluer la spline
  t = linspace(-2,2,81);
  [xS,yS] = meshgrid(t,t);
  
  // calcul de la spline z=f(x,y) pour les parametres (xS,yS)
  zS = spline_radiale(x,y,z,xS,yS);
  
  // sorties graphiques au format Geomview
  fid = mopen("points.sphere", "w");
  mfprintf(fid, "{LIST\n");
  for i=1:length(u)    
    write_SPHERE(fid,x(i),y(i),z(i),0.025);
  end
  mfprintf(fid, "}\n");
  mclose(fid);
  
  fid = mopen("surface.mesh", "w"); 
  write_MESH(fid,xS,yS,zS,[1.0 0.8 0.6]);
  mclose(fid);
  trace_isoparametrique("surface_iso.vect", ...
    xS,yS,zS, 11, 3, [0 0 0])
  
endfunction

// la fonction "spline"
function y = h(x)
  y = x.^3;
endfunction

// calcul et évaluation d'une spline radiale z=S(x,y)
// interpolant les donnees (u(i),v(i),w(i))
// telle que S(u(i),v(i))=w(i)
// Entrée : u,v,w = tableaux de n valeurs
//          x,y = tableau où évaluer la spline
// Sortie : z = S(x,y)
function z = spline_radiale(u,v,w,x,y)

n = length(u);

// construction er résolution du système
M = zeros(n+3,n+3);
b = zeros(n+3,1);
for i=1:n
  for j=i+1:n
    M(i,j) = h(norm([u(i)-u(j),v(i)-v(j)]));
    M(j,i) = M(i,j);
  end
  M(i,n+1) = 1;
  M(i,n+2) = u(i);
  M(i,n+3) = v(i);
  M(n+1,i) = 1;
  M(n+2,i) = u(i);
  M(n+3,i) = v(i);
  b(i) = w(i);
end
c = M\b;

z = zeros(x);
d = zeros(1,n+3);
for i=1:length(x)
  for j=1:n
    d(j) = h(norm([x(i)-u(j),y(i)-v(j)]));
  end
  d(n+1:n+3) = [1 x(i) y(i)];
  z(i) = d*c;
end

endfunction

///////////////////////////////////////////////////////////////////////
// Routines pour les sorties graphiques au format Geoview
///////////////////////////////////////////////////////////////////////

// tracé de courbes isoparametrique à partir de données grille (x,y,z)
// Entree : nom_f = nom du fichier Geomview
//          x,y,z = donnees grille (matrices MxN)
//          nIso = nb d'isoparametriques à tracer suivant une direction
//          direction = direction pour le tracé des isoparamétriques
//            1 : tracé suivant la direction 1
//                une isoparametrique : points (x(:,j),y(:,j),z(:,j))
//            2 : tracé suivant la direction 2
//                une isoparametrique : points (x(i,:),y(i,:),z(i,:))
//            3 : tracé suivant la direction 1 et la direction 2
//          couleur = triplet (r,g,b)
function trace_isoparametrique(nom_f, x,y,z, nIso, direction, couleur)

  [d1,d2] = size(x);
  f = mopen(nom_f, "w");
  mfprintf(f,"{\nLIST\n");
  if direction==1 | direction == 3
    // tracé suivant la direction 1
    dIso = floor((d2-1)/(nIso-1));
    for j=1:dIso:d2
      write_VECT(fid,x(:,j),y(:,j),z(:,j),couleur,1);
    end    
  end
  if direction==2 | direction == 3
    // tracé suivant la direction 2
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
//          l = épaisseur de la ligne
function write_VECT(fid,x,y,z,c,l);

// début de l'objet composite
mfprintf(fid,"{\n");

// attribut pour l'épaisseur
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
// objet formé de n vecteurs [P(:,i),P(:,i)+h*V(:,i)]
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          P = les points de la polyligne
//          V = vecteurs associés
//          c = couleur RGB
//          h = homothetie pour le vecteur
//          l = épaisseur de la ligne
function write_VECTS(fid,P,V,c,h,l);

// début de l'objet composite
mfprintf(fid,"{\n");

// attribut pour l'épaisseur
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

// début de l'objet composite
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

// début de l'objet composite
mfprintf(fid,"{\n");

mfprintf(fid, 'SPHERE\n%15.7e %15.7e %15.7e %15.7e\n', r, x, y, z);

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------


