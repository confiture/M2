//////////////////////////////////////////////////////////////////
// interpolation produit tensoriel
//////////////////////////////////////////////////////////////////

function main(num_test)
  
  if argn(2)==0
    num_test=1;
  end
  
  select num_test
    case 1
  // test1 - surface avec X=u et Y=v
  u = 1:3;
  v = 1:4;
  [X,Y] = meshgrid(u,v);
  Z = rand(X);
  Su = linspace(u(1),u($),49);
  Sv = linspace(v(1),v($),49);
  methode_u = 'natural';
  [SX,SY,SZ,X1,Y1,Z1] = interp_prod_tensoriel(X,Y,Z,u,v,Su,Sv,methode_u);
  [SX2,SY2,SZ2,X12,Y12,Z12] = interp_prod_tensoriel2(X,Y,Z,u,v,Su,Sv,methode_u);
  
    case 2
  // test2 - surface p�riodique en u
  u = 1:5;
  v = 1:3;
  X = [2 3 4 3 2; 0 3 6 3 0; 1 3 5 3 1]; 
  Y = [3 2 3 4 3; 3 0 3 6 3; 3 1 3 5 3]; 
  Z = [0 0 0 0 0; 2 2 2 2 2; 3 3 3 3 3]; 
  Su = linspace(u(1),u($),101);
  Sv = linspace(v(1),v($),49);
  methode_u = 'fast_periodic';
  methode_v = 'natural';
  [SX,SY,SZ,X1,Y1,Z1] = interp_prod_tensoriel2(X,Y,Z,u,v,Su,Sv,methode_u,methode_v);
  //[SX2,SY2,SZ2,X12,Y12,Z12] = interp_prod_tensoriel2(X,Y,Z,u,v,Su,Sv,methode_u);
  
    else
      error('numero de test invalide');

  end // select
  
  // sorties graphiques au format Geomview
  f = mopen('points.sphere', 'w');
  mfprintf(f, "{LIST\n");
  for i=1:size(X,1)
  for j=1:size(X,2)
    write_SPHERE(f,X(i,j),Y(i,j),Z(i,j),0.05);
  end
  end
  mfprintf(f, "}\n");
  mclose(f);
  
  f = mopen('donnees.mesh', 'w');
  mfprintf(f,"{\n appearance {-face +edge linewidth 2}\n");
  write_MESH(f,X,Y,Z,[0 0 0]);
  mfprintf(f,"}\n");
  mclose(f);
  
  f = mopen('surface_interm.mesh', 'w');
  mfprintf(f,"{\n appearance {-face +edge}\n");
  write_MESH(f,X1,Y1,Z1,[1 0 0]);
  mfprintf(f,"}\n");
  mclose(f);
  
  f = mopen('surface_finale.mesh', 'w');
  write_MESH(f,SX,SY,SZ,[0 0.5 0]);
  mclose(f);
  
//  f = mopen('surface_interm2.mesh', 'w');
//  mfprintf(f,"{\n appearance {-face +edge}\n");
//  write_MESH(f,X12,Y12,Z12,[1 0 0]);
//  mfprintf(f,"}\n");
//  mclose(f);
//  
//  f = mopen('surface_finale2.mesh', 'w');
//  write_MESH(f,SX2,SY2,SZ2,[0 0.5 0]);
//  mclose(f);
endfunction

//----------------------------------------------
// interpolation spline 1D des donn�es (x,y) suivant les valeurs xp
// et la methode spline_type
function yp = splin1(x,y,xp,spline_type)
  d = splin(x,y,spline_type);
  yp = interp(xp,x,y,d);
endfunction

//----------------------------------------------
// interpolation produit tensoriel d'une grille de points 
// P(i,j) = (X(i,j),Y(i,j),Z(i,j)) associ�e  
// � un param�trage grille rectangulaire (u(i),v(j)) 1<=i<=M, 1<=j<=N.
// X,Y,Z matrices avec N lignes et M colonnes
// Les points S(k,l) = (SX(k,l),SY(k,l),SZ(k,l)) de la surface sont �valu�s 
// SX,SY,SZ matrices avec Q lignes et P colonnes
// suivant la grille rectangulaire (Su(k),Sv(l)) 1<=k<=P, 1<=l<=Q.
// Entr�e : X,Y,Z = coordonn�es des points de donn�es 
//           tableaux de dimensions N x M
//          u,v = parametrage correspondant 
//           u : vecteur de M valeurs
//           v : vecteur de N valeurs
//          Su,Sv = parametrage pour l'�valuation 
//           Su : vecteur de P valeurs
//           Sv : vecteur de Q valeurs
//          methode_u = chaine de caract�re pour le proc�d� d'interpolation 1D 
//           � appliquer suivant le param�tre u
//          methode_v = chaine de caract�re pour le proc�d� d'interpolation 1D 
//           � appliquer suivant le param�tre v
// Le proc�d� d'interpolation 1-D est donn�e par les chaines de caract�res
// m�thode_u, methode_v qui peuvent prendre les valeurs suivantes :
//   'not_a_knot','natural','periodic','fast' ou 'fast_periodic'
//   pour les methodes 'periodic' et 'fast_periodic', il faut que
//   P(1,j)=P(N,j) et/ou P(i,1)=P(i,M)
//   si methode_u n'est pas fournie alors methode_u = 'natural' 
//   si methode_v n'est pas fournie alors methode_v = methode_u 
// Les vecteurs u,v, Su,Sv doivent etre compos�s de valeurs 
// strictement croissantes
// Sortie : SX,SY,SZ = coordonn�es de la surface interpolante 
//           (tableaux de dimensions Q x P)
//          X1,Y1,Z1 = coordonn�es de la surface interm�diaire 
//           issue de la premi�re interpolation faite suivant le parametre u
//           (tableaux de dimensions N x P)
function [SX,SY,SZ,X1,Y1,Z1] = ...
  interp_prod_tensoriel(X,Y,Z,u,v,Su,Sv,methode_u,methode_v) 
  
  // v�rification du nombre d'arguments
  nargin = argn(2);
  if nargin<7
    error('syntaxe [SX,SY,SZ] = interp_prod_tensoriel(X,Y,Z,u,v,Su,Sv[,methode_u[,methode_v]])');
  end
  if nargin<8
    methode_u = 'natural';
  end
  if nargin<9
    methode_v = methode_u;
  end

  // v�rification des arguments
  M = length(u);
  N = length(v);
  P = length(Su);
  Q = length(Sv);
  if (M<2 | N<2 | P<2 | Q<2)
    error('les vecteurs u, v, Su, Sv doivent avoir au moins 2 valeurs');
  end
  if (min(diff(u))<=0 | min(diff(v))<=0 | min(diff(Su))<=0 | min(diff(Sv))<=0)
    error('les vecteurs u, v, Su, Sv doivent �tre compos�es de valeurs strictement croissantes');
  end
  
  if (size(X,1)~=N | size(X,2)~=M | ...
      size(Y,1)~=N | size(Y,2)~=M | ...
      size(Z,1)~=N | size(Z,2)~=M)  
    error('les tableaux (X,Y,Z) doivent avoir des dimensions correspondant � (u,v)');
  end

  // mettre u et Su en ligne, v et Sv en colonne
  u  = u(:)' ;
  Su = Su(:)';
  v  = v(:)  ;
  Sv = Sv(:) ;
  
  ////// interpolation 1-D suivant le param�tre u
  // selection de la routine Scilab suivant la methode
  select methode_u
    case 'not_a_knot'
      routine_interp = 'splin1'; methode_interp = '''not_a_knot''';
    case 'natural'
      routine_interp = 'splin1'; methode_interp = '''natural''';
    case 'periodic'
      routine_interp = 'splin1'; methode_interp = '''periodic''';
    case 'fast'
      routine_interp = 'splin1'; methode_interp = '''fast''';
    case 'fast_periodic'
      routine_interp = 'splin1'; methode_interp = '''fast_periodic''';
    else
      routine_interp = 'splin1'; methode_interp = '''natural''';
  end
    
  X1 = zeros(N,P);
  Y1 = zeros(N,P);
  Z1 = zeros(N,P);
  s_interp_X = ['X1(i,:) = '+ routine_interp+ '(u, X(i,:), Su,'+ methode_interp+ ');'];
  s_interp_Y = ['Y1(i,:) = '+ routine_interp+ '(u, Y(i,:), Su,'+ methode_interp+ ');'];
  s_interp_Z = ['Z1(i,:) = '+ routine_interp+ '(u, Z(i,:), Su,'+ methode_interp+ ');'];  
  for i=1:N
    execstr([s_interp_X;s_interp_Y;s_interp_Z]);
  end
  
  ////// interpolation 1-D suivant le param�tre v
  // selection de la routine Scilab suivant la methode
  select methode_v
    case 'not_a_knot'
      routine_interp = 'splin1'; methode_interp = '''not_a_knot''';
    case 'natural'
      routine_interp = 'splin1'; methode_interp = '''natural''';
    case 'periodic'
      routine_interp = 'splin1'; methode_interp = '''periodic''';
    case 'fast'
      routine_interp = 'splin1'; methode_interp = '''fast''';
    case 'fast_periodic'
      routine_interp = 'splin1'; methode_interp = '''fast_periodic''';
    else
      routine_interp = 'splin1'; methode_interp = '''natural''';
  end
    
  SX = zeros(Q,P);
  SY = zeros(Q,P);
  SZ = zeros(Q,P);
  s_interp_X = ['SX(:,i) = '+ routine_interp+ '(v, X1(:,i), Sv,'+ methode_interp+ ');'];
  s_interp_Y = ['SY(:,i) = '+ routine_interp+ '(v, Y1(:,i), Sv,'+ methode_interp+ ');'];
  s_interp_Z = ['SZ(:,i) = '+ routine_interp+ '(v, Z1(:,i), Sv,'+ methode_interp+ ');'];  
  for i=1:P
    execstr([s_interp_X;s_interp_Y;s_interp_Z]);
  end
  
endfunction


//----------------------------------------------
// interpolation produit tensoriel d'une grille de points 
// P(i,j) = (X(i,j),Y(i,j),Z(i,j)) associ�e  
// � un param�trage grille rectangulaire (u(i),v(j)) 1<=i<=M, 1<=j<=N.
// X,Y,Z matrices avec N lignes et M colonnes
// Les points S(k,l) = (SX(k,l),SY(k,l),SZ(k,l)) de la surface sont �valu�s 
// SX,SY,SZ matrices avec Q lignes et P colonnes
// suivant la grille rectangulaire (Su(k),Sv(l)) 1<=k<=P, 1<=l<=Q.
// Entr�e : X,Y,Z = coordonn�es des points de donn�es 
//           tableaux de dimensions N x M
//          u,v = parametrage correspondant 
//           u : vecteur de M valeurs
//           v : vecteur de N valeurs
//          Su,Sv = parametrage pour l'�valuation 
//           Su : vecteur de P valeurs
//           Sv : vecteur de Q valeurs
//          methode_u = chaine de caract�re pour le proc�d� d'interpolation 1D 
//           � appliquer suivant le param�tre u
//          methode_v = chaine de caract�re pour le proc�d� d'interpolation 1D 
//           � appliquer suivant le param�tre v
// Le proc�d� d'interpolation 1-D est donn�e par les chaines de caract�res
// m�thode_u, methode_v qui peuvent prendre les valeurs suivantes :
//   'not_a_knot','natural','periodic','fast' ou 'fast_periodic'
//   pour les methodes 'periodic' et 'fast_periodic', il faut que
//   P(1,j)=P(N,j) et/ou P(i,1)=P(i,M)
//   si methode_u n'est pas fournie alors methode_u = 'natural' 
//   si methode_v n'est pas fournie alors methode_v = methode_u 
// Les vecteurs u,v, Su,Sv doivent etre compos�s de valeurs 
// strictement croissantes
// Sortie : SX,SY,SZ = coordonn�es de la surface interpolante 
//           (tableaux de dimensions Q x P)
//          X1,Y1,Z1 = coordonn�es de la surface interm�diaire 
//           issue de la premi�re interpolation faite suivant le parametre u
//           (tableaux de dimensions N x P)
function [SX,SY,SZ,X1,Y1,Z1] = ...
  interp_prod_tensoriel2(X,Y,Z,u,v,Su,Sv,methode_u,methode_v) 
  
  // v�rification du nombre d'arguments
  nargin = argn(2);
  if nargin<7
    error('syntaxe [SX,SY,SZ] = interp_prod_tensoriel(X,Y,Z,u,v,Su,Sv[,methode_u[,methode_v]])');
  end
  if nargin<8
    methode_u = 'natural';
  end
  if nargin<9
    methode_v = methode_u;
  end

  // v�rification des arguments
  M = length(u);
  N = length(v);
  P = length(Su);
  Q = length(Sv);
  if (M<2 | N<2 | P<2 | Q<2)
    error('les vecteurs u, v, Su, Sv doivent avoir au moins 2 valeurs');
  end
  if (min(diff(u))<=0 | min(diff(v))<=0 | min(diff(Su))<=0 | min(diff(Sv))<=0)
    error('les vecteurs u, v, Su, Sv doivent �tre compos�es de valeurs strictement croissantes');
  end
  
  if (size(X,1)~=N | size(X,2)~=M | ...
      size(Y,1)~=N | size(Y,2)~=M | ...
      size(Z,1)~=N | size(Z,2)~=M)  
    error('les tableaux (X,Y,Z) doivent avoir des dimensions correspondant � (u,v)');
  end
  
  // mettre u et Su en ligne, v et Sv en colonne
  u  = u(:)' ;
  Su = Su(:)';
  v  = v(:)  ;
  Sv = Sv(:) ;
  
  ////// interpolation 1-D suivant le param�tre v
  // selection de la routine Scilab suivant la methode
  select methode_v
    case 'not_a_knot'
      routine_interp = 'splin1'; methode_interp = '''not_a_knot''';
    case 'natural'
      routine_interp = 'splin1'; methode_interp = '''natural''';
    case 'periodic'
      routine_interp = 'splin1'; methode_interp = '''periodic''';
    case 'fast'
      routine_interp = 'splin1'; methode_interp = '''fast''';
    case 'fast_periodic'
      routine_interp = 'splin1'; methode_interp = '''fast_periodic''';
    else
      routine_interp = 'splin1'; methode_interp = '''natural''';
  end
    
  X1 = zeros(Q,M);
  Y1 = zeros(Q,M);
  Z1 = zeros(Q,M);
  s_interp_X = ['X1(:,i) = '+ routine_interp+ '(v, X(:,i), Sv,'+ methode_interp+ ');'];
  s_interp_Y = ['Y1(:,i) = '+ routine_interp+ '(v, Y(:,i), Sv,'+ methode_interp+ ');'];
  s_interp_Z = ['Z1(:,i) = '+ routine_interp+ '(v, Z(:,i), Sv,'+ methode_interp+ ');'];  
  for i=1:M
    execstr([s_interp_X;s_interp_Y;s_interp_Z]);
  end
  
  ////// interpolation 1-D suivant le param�tre u
  // selection de la routine Scilab suivant la methode
  select methode_u
    case 'not_a_knot'
      routine_interp = 'splin1'; methode_interp = '''not_a_knot''';
    case 'natural'
      routine_interp = 'splin1'; methode_interp = '''natural''';
    case 'periodic'
      routine_interp = 'splin1'; methode_interp = '''periodic''';
    case 'fast'
      routine_interp = 'splin1'; methode_interp = '''fast''';
    case 'fast_periodic'
      routine_interp = 'splin1'; methode_interp = '''fast_periodic''';
    else
      routine_interp = 'splin1'; methode_interp = '''natural''';
  end
    
  s_interp_X = ['SX(i,:) = '+ routine_interp+ '(u, X1(i,:), Su,'+ methode_interp+ ');'];
  s_interp_Y = ['SY(i,:) = '+ routine_interp+ '(u, Y1(i,:), Su,'+ methode_interp+ ');'];
  s_interp_Z = ['SZ(i,:) = '+ routine_interp+ '(u, Z1(i,:), Su,'+ methode_interp+ ');'];  
  for i=1:Q
    execstr([s_interp_X;s_interp_Y;s_interp_Z]);
  end
  
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


