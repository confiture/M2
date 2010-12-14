exec('util.sce')
exec('nurbs.sce')
/////////////////////////////////////////////////////////
// conversion d'une surface NURBS en patch de Bezier
/////////////////////////////////////////////////////////
function prog41()
  
//// 1 - d�finition de la NURBS
cas = 1; // NURBS lue dans un fichier
//cas = 0; // NURBS definie dans le programme
select cas
case 1 // lecture dans un fichier
  nom_f = uigetfile('*.txt','.','Selection d''un fichier NURBS');
  [T,u,v,du,dv]=lire_fichier_NURBS(nom_f);
  X = T(:,:,1);
  Y = T(:,:,2);
  Z = T(:,:,3);
  omega = T(:,:,4);
else // d�finition de la NURBS dans le programme    
  // tableau de la coordonn�e 1 des points de controle
  X =  [
      0   1   2   3   4;...
      0   1   2   3   4;...
      0   1   2   3   4;...
      0   1   2   3   4;...
      0   1   2   3   4;...
      0   1   2   3   4;...
      0   1   2   3   4];  
  // tableau de la coordonn�e 2 des points de controle
  Y =  [
      0   0   0   0   0;...
      1   1   1   1   1;...
      2   2   2   2   2;...
      3   3   3   3   3;...
      4   4   4   4   4;...
      5   5   5   5   5;...
      6   6   6   6   6];  
  // tableau de la coordonn�e 3 des points de controle
  Z =  [
      0   0   0   0   0;...
      0   0   0   0   0;...
      0   0  -3   0   0;...
      0   0   0   0   0;...
      0   0   3   0   0;...
      0   0   0   0   0;...
      0   0   0   0   0]; 
  // tableau des poids
  omega = [
      1   1   1   1   1;...
      1   1   1   1   1;...
      1   1   2   1   1;...
      1   1   1   1   1;...
      1   1 0.5   1   1;...
      1   1   1   1   1;...
      1   1   1   1   1];
      
  // noeuds, degr� suivant la dimension 1
  u = [0 1 2 3 4]; du = 3; 

  // noeuds, degr� suivant la dimension 2
  v = [0 1 2 3];   dv = 2; nv = length(v)-1;

end // select cas

// ordre et nombre d'intervalles suivant la dimension 1
ordre_u = du+1; nu = length(u)-1;
// ordre et nombre d'intervalles suivant la dimension 2
ordre_v = dv+1; nv = length(v)-1;

//// 2 - conversion de la NURBS en B�zier

// ************************************ 
// ******** PARTIE A COMPLETER ******** 
// ************************************ 


//// 3 - �criture des fichiers

// le polyedre de controle de la NURBS
f=mopen("nurbs.mesh","w");
write_MESH(f, X, Y, Z);
mclose(f)

// DECOMMENTER LES LIGNES SUIVANTES AVANT ENDFUNCTION 

//// la liste de patch de Bezier
f=mopen("nurbs_bezier.list","w");
mfprintf(f, "{\n LIST\n");
//  
//  // r�cup�ration des points de controle en coordonnees homog�nes
//  // de la B�zier (i,j) : 4 matrices B1,B2,B3,B4 
//  // de dimensions ordre_u x ordre_v
X=X.*omega
Y=Y.*omega
Z=Z.*omega

disp(X)
disp("size X")
disp(size(X,1))
disp("=========")
//on augmente le degré en u
for i=1:size(X,2)
  //[B1temp(:,i)' ; B2temp(:,i)' ; B3temp(:,i)' ; B4temp(:,i)'] = NURBS1DToBezier([X(:,i)' ; Y(:,i)' ; Z(:,i)' ; omega(:,i)'],u,ordre_u)
  Btemp= NURBS1DToBezier([X(:,i)' ; Y(:,i)' ; Z(:,i)' ; omega(:,i)'],u,ordre_u)
  Btemp1(:,i)=Btemp(1,:)'
  Btemp2(:,i)=Btemp(2,:)'
  Btemp3(:,i)=Btemp(3,:)'
  Btemp4(:,i)=Btemp(4,:)'
end

disp(Btemp1)
disp("=========")

Btemp=[]
//on augmente le degré en v
for i=1:size(Btemp1,1)
  Btemp = NURBS1DToBezier([Btemp1(i,:) ; Btemp2(i,:) ; Btemp3(i,:) ; Btemp4(i,:)],v,ordre_v)
  B1(i,:)=Btemp(1,:)
  B2(i,:)=Btemp(2,:)
  B3(i,:)=Btemp(3,:)
  B4(i,:)=Btemp(4,:)
end
disp("ordre_u")
disp(ordre_u)
disp("size")
disp(size(B1,1))
//// la liste de patch de Bezier
f=mopen("nurbs_bezier.list","w");
mfprintf(f, "{\n LIST\n");

for i=1:nu
  for j=1:nv
    write_BEZ4(f, B1((i-1)*ordre_u+1:i*ordre_u,(j-1)*ordre_v+1:j*ordre_v), B2((i-1)*ordre_u+1:i*ordre_u,(j-1)*ordre_v+1:j*ordre_v),B3((i-1)*ordre_u+1:i*ordre_u,(j-1)*ordre_v+1:j*ordre_v), B4((i-1)*ordre_u+1:i*ordre_u,(j-1)*ordre_v+1:j*ordre_v));
    disp(i)
  end
end

mfprintf(f, "}\n");
mclose(f);

endfunction

//////////////////////////////////////////////////////////
//Renvoie le polygone de contrôle de la NURBS approximant
//le cercle unité.
//D : les points du polygone de contrôle, D(1,:) pour les abscisses et D(2,:)
//    pour les ordonnées
//tau : le vecteur des noeuds
function [D,tau]=cercleUniteNurbs()    
    D=[1. 1. 0.5522847 0. -0.5522847 -1. -1. -1. -0.5522847 -2.220D-16 0.5522847 1. 1.;
    0. 0.5522847 1. 1. 1. 0.5522847 1.110D-16  -0.5522847  -1. -1. -1. -0.5522847 -2.776D-16;
    1 1 1 1 1 1 1 1 1 1 1 1 1]
    tau=[0 1 2 3 4 5 6 7 8 9 10]
endfunction
  
  
////////////////////////////////////////////////////////////////////////////  
//Renvoie la surface de révolution avec une nurbs de degré 3.
//X,Y,Z sont les coordonnées du polygone de contrôle de la surface de révolution.  
//W : les poids des points du polygone de contrôle de la surface de révolution.  
//u et v sont les vecteurs des noeuds.
////////////////////////////////////////////////////////////////////////
function [X,Y,Z,W,u,v]=surfaceRevolution()             
  L=inputpoly_rat() //on saisie la courbe génératrice de la surface de révolution
  nbNds=size(L,2)-2
  tau=inputnoeuds(nbNds,10,90,10)
  
  t=sature_noeuds(tau,4)            //on calcule puis on trace la NURBS génératrice
  C=nurbsB(L,4,t)    
  f=scf()
  set(gca(),"data_bounds",[0,0;100,100])
  plot(C(1,:),C(2,:))
  xs2jpg(f,"generatrice.jpg")
  [cercle,tau_cercle]=cercleUniteNurbs() //Le polygone de contrôle de la NURBS approximant
                                         //le cercle unité
  
  X=zeros(size(cercle,2),size(L,2))
  Y=zeros(size(cercle,2),size(L,2))
  Z=zeros(size(cercle,2),size(L,2))
  W=zeros(size(cercle,2),size(L,2))
  for i=1:size(cercle,2)
    for j=1:size(L,2)
      X(i,j)=L(1,j)*cercle(1,i)
      Y(i,j)=L(1,j)*cercle(2,i)
      Z(i,j)=L(2,j)
      W(i,j)=L(3,j)*cercle(3,i)
    end
  end
  
  u=tau_cercle
  v=tau
endfunction

////////////////////////////////////////////////////////////////
//Saisit la courbe génératrice et écrit la surface de révolution 
//lui correspondant.
//La maillage est écrit dans nurbs.msh et les patchs Bézier rationnelle
//sont écrit dans nurbs_bezier.list .
////////////////////////////////////////////////////////////////
function ecrisSurfaceRevol()
  [X,Y,Z,omega,u,v]=surfaceRevolution()//On saisie la surface de révolution   
      
  // noeuds, degr� suivant la dimension 1
  du = 3; nu=length(u)-1
  dv = 3; nv = length(v)-1;

// ordre et nombre d'intervalles suivant la dimension 1
ordre_u = du+1; nu = length(u)-1;
// ordre et nombre d'intervalles suivant la dimension 2
ordre_v = dv+1; nv = length(v)-1;

//// 3 - �criture des fichiers

// le polyedre de controle de la NURBS
f=mopen("nurbs.mesh","w");
write_MESH(f, X, Y, Z);
mclose(f)

//// la liste de patch de Bezier
f=mopen("nurbs_bezier.list","w");
mfprintf(f, "{\n LIST\n");

//  
//  // r�cup�ration des points de controle en coordonnees homog�nes
//  // de la B�zier (i,j) : 4 matrices B1,B2,B3,B4 
//  // de dimensions ordre_u x ordre_v
X=X.*omega
Y=Y.*omega
Z=Z.*omega

//on augmente le degré en u
for i=1:size(X,2)
  //[B1temp(:,i)' ; B2temp(:,i)' ; B3temp(:,i)' ; B4temp(:,i)'] = NURBS1DToBezier([X(:,i)' ; Y(:,i)' ; Z(:,i)' ; omega(:,i)'],u,ordre_u)
  Btemp= NURBS1DToBezier([X(:,i)' ; Y(:,i)' ; Z(:,i)' ; omega(:,i)'],u,ordre_u)
  Btemp1(:,i)=Btemp(1,:)'
  Btemp2(:,i)=Btemp(2,:)'
  Btemp3(:,i)=Btemp(3,:)'
  Btemp4(:,i)=Btemp(4,:)'
end

Btemp=[]
//on augmente le degré en v
for i=1:size(Btemp1,1)
  Btemp = NURBS1DToBezier([Btemp1(i,:) ; Btemp2(i,:) ; Btemp3(i,:) ; Btemp4(i,:)],v,ordre_v)
  B1(i,:)=Btemp(1,:)
  B2(i,:)=Btemp(2,:)
  B3(i,:)=Btemp(3,:)
  B4(i,:)=Btemp(4,:)
end

for i=1:nu
  for j=1:nv
    write_BEZ4(f, B1((i-1)*ordre_u+1:i*ordre_u,(j-1)*ordre_v+1:j*ordre_v), B2((i-1)*ordre_u+1:i*ordre_u,(j-1)*ordre_v+1:j*ordre_v),B3((i-1)*ordre_u+1:i*ordre_u,(j-1)*ordre_v+1:j*ordre_v), B4((i-1)*ordre_u+1:i*ordre_u,(j-1)*ordre_v+1:j*ordre_v));
  end
end

mfprintf(f, "}\n");
mclose(f);

endfunction
//----------------------------------------------

/////////////////////////////////////////////////////////
// ecriture d'un objet MESH - poly�dre grille rectangulaire
// Entr�e : X,Y,Z = tableau des coordonn�es des points
//          fid : identificateur d'un fichier ouvert en �criture avec mopen
function write_MESH(fid,X,Y,Z)

nu = size(X,1);
nv = size(X,2);

// d�but de l'objet composite
mfprintf(fid,"{\n");

mfprintf(fid, 'MESH\n%d %d\n', nv, nu);
for i=1:nu
for j=1:nv
    mfprintf(fid, '%15.7e %15.7e %15.7e\n', ...
	  X(i,j) , Y(i,j) , Z(i,j));
end
end

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------

/////////////////////////////////////////////////////////
// ecriture d'un patch de B�zier rationnel dans un fichier
// au format Geomview
// Entree = P1,P2,P3,P4 : tableaux des coordonnees des points 
//          en coordonn�es homog�nes du patch de B�zier
//            P1,P2,P3,P4 matrices de dimensions Nu par Nv
//            avec Nu et Nv compris entre 2 et 7
//            (degr�s compris entre 1 et 6)
//          fid : identificateur d'un fichier ouvert en �criture avec
//            mopen
function write_BEZ4(fid, P1,P2,P3,P4)

Nu = size(P1,1);
Nv = size(P1,2);

if Nu<2 | Nu>7
  error('le degre en u doit entre 1 et 6');
end
if Nv<2 | Nv>7
  error('le degre en v doit entre 1 et 6');
end

// ecriture de la Bezier
mfprintf(fid, 'BEZ%1d%1d%1d\n', Nu-1, Nv-1, 4);

for j=1:Nv
for i=1:Nu
    mfprintf(fid, '%15.7e %15.7e %15.7e %15.7e\n', ...
      P1(i,j),P2(i,j),P3(i,j),P4(i,j)); 
end
end

endfunction
//----------------------------------------------

/////////////////////////////////////////////////////////
// Passage de la forme NURBS � la forme B�zier - Cas d'une courbe ouverte
// Entr�e : D = tableau des points de controle DeBoor (1 point par colonne)
//          tau = vecteur des noeuds 
//          k = l'ordre de la BSpline
// Sortie : PC = tableau des diff�rents polygones de controle
// 
// k : entier > 1, d = k-1 (degr� de la Bspline)
// tau : tableau de n+1 r�els (n nombre d'intervalles)
// D : tableau de p=n+d points (p colonnes)
// PC : tableau de q=k*n points
//   PC(:,1+(i-1)*k:i*k) contient les d+1 points de controle 
//    de la i-eme B�zier (1 <= i <= n)
function PC = NURBS1DToBezier(D,tau,k)

// FORME BEZIER : on sature chaque noeud avec la multiplicit� = k
// afin d''obtenir la forme B�zier composite de la B-spline

// calcul du vecteur nodal - cas courbe ouverte
// saturation des noeuds de bord
vectnodal = tau;
for j = 1 : k - 1
  vectnodal = [tau(1) vectnodal tau($)];
end

// saturation des noeuds internes
tau0=tau(1);
for i = 2 : size(tau,2)-1
    for j = 1 : k - 1
        [D, vectnodal] = InsertionNoeud(D,vectnodal,k,tau(i));
    end
end
PC = D;
endfunction
//----------------------------------------------

/////////////////////////////////////////////////////////
// Modification de la structure NURBS par insertion de noeuds
// Entree : D = tableau des points de controle (1 point par colonne)
//          vectnoeuds = le vecteur nodal (avec noeuds de bord satur�s)
//          ordre = ordre de la Bspline
//          x = noeuds � ins�rer
// Sortie : D2 = tableau des points de controle r�sultant
//          vectnoeuds = le vecteur nodal r�sultant
// ordre : entier > 0, d = ordre-1 : degr� de la BSpline
// vectnoeuds : tableau de q=n+2*d+1 r�els avec 
//   vectnoeuds(1) = ... = vectnoeuds(d+1) <= vectnoeuds(d+2)<= ...
//    ... <= vectnoeuds(n+d+1) = ... = vectnoeuds(n+2*d+1)
// D : tableau de p = n+d points
// x : r�el tel que vectnoeuds(d+1) < x < vectnoeuds(n+d+1)
// D2 : tableau de p+1 points
// vectnoeuds2 : tableau de q+1 r�els  
function [D2,vectnoeuds2] = InsertionNoeud(D,vectnoeuds,ordre,x)

if x<=vectnoeuds(1) 
  error('x doit etre > premier noeud');
end

if x>=vectnoeuds($) 
  error('x doit etre < dernier noeud');
end

r = ordre;
while vectnoeuds(r+1) < x
    r = r+1;
end 
// en sortie r est tel que x \in [t_r, t_{r+1}[

//disp("-----------------");
//disp("size(D)", size(D));
//disp("vectnoeuds",vectnoeuds);
//disp("ordre",ordre);
//disp("x",x);
//disp("r",r);

A = D(:,r-ordre+1:r);
j = 1;
for i = r - ordre + 1 + j : r
    ii = i - r + ordre - j;
    denomin = vectnoeuds(i+ordre-j) - vectnoeuds(i);
    cf1 = (vectnoeuds(i+ordre-j) - x) / denomin;
    cf2 = (x - vectnoeuds(i)) / denomin;
    A(:,ii) = cf1 * A(:,ii) + cf2 * A(:,ii+1);
end

// mise � jour du polygone et du vecteur des noeuds
D2 = [D(:,1:r-ordre+1)  A(:,1:ordre-1)  D(:,r:size(D,2))];
vectnoeuds2 = [vectnoeuds(1:r)  x  vectnoeuds(r+1:size(vectnoeuds,2))];

endfunction
//----------------------------------------------

/////////////////////////////////////////////////////////
// lecture d'un fichier contenant la d�finition d'une NURBS
// Entr�e : nom_f = le nom du fichier texte � lire
// Sortie : T = hyper-matrice de dimensions M x N x 4
//            le point de controle D(k,l) est donn� par [T(k,l,1),T(k,l,2),T(k,l,3)]
//            le poids correspondant w(k,l) est donn� par T(k,l,4)
//          u,v = vecteurs de noeuds de dimensions respectives nu+1 et nv+1
//          du,dv = degr�s en u et v
//             on aura M = nu+du et N = nv+dv
//
// Le fichier d'entr�e doit avoir la structure suivante
// - le degr� du en u sur une ligne
// - le degr� dv en v sur une ligne
// - le nombre nu+1 de noeuds en u sur une ligne
// - les nu+1 valeurs des noeuds u(i) par ordre croissant sur une seule ligne
// - le nombre nv+1 de noeuds en v sur une ligne
// - les nv+1 valeurs des noeuds v(j) par ordre croissant sur une seule ligne
// - les coordonn�es X des points de controle dans l'ordre suivant avec
//   M lignes de N valeurs :
//     X(1,1) X(1,2) ... X(1,N)
//     X(2,1) X(2,2) ... X(2,N)
//      ...    ...        ...
//     X(M,1) X(M,2) ... X(M,N)
// - les coordonn�es Y des points de controle dans l'ordre suivant avec
//   M lignes de N valeurs :
//     Y(1,1) Y(1,2) ... Y(1,N)
//     Y(2,1) Y(2,2) ... Y(2,N)
//      ...    ...        ...
//     Y(M,1) Y(M,2) ... Y(M,N)
// - les coordonn�es Z des points de controle dans l'ordre suivant avec
//   M lignes de N valeurs :
//     Z(1,1) Z(1,2) ... Z(1,N)
//     Z(2,1) Z(2,2) ... Z(2,N)
//      ...    ...        ...
//     Z(M,1) Z(M,2) ... Z(M,N)
// - les poids w dans l'ordre suivant avec
//   M lignes de N valeurs :
//     w(1,1) w(1,2) ... w(1,N)
//     w(2,1) w(2,2) ... w(2,N)
//      ...    ...        ...
//     w(M,1) w(M,2) ... w(M,N)
// chaque ligne commencant par le caractere # est un commentaire
// et n'est pas pris en compte
function [T,u,v,du,dv]=lire_fichier_NURBS(nom_f)
  
  // construction d'un format de lecture pour lire n r�els
  function fmt=format_reels(n)
    fmt = '%f';
    for i=2:n
      fmt = fmt + ' %f';
    end
  endfunction
  
  // teste de quel type est la chaine de caractere s
  // res = 0 : s correspond � une fin de fichier
  // res = 1 : s est une ligne de commentaire ou ligne vide
  // res = 2 : s est une ligne commen�ant par une valeur num�rique
  // res = 3 : s est une ligne ne commen�ant pas par un caract�re num�rique
  function res=type_ligne(s)
    
    // si s vide : fin de fichier
    if size(s,1)==0
      res=0; return;
    end
    
    // codage ASCII de la chaine s
    code_s = ascii(s);
    
    // ligne vide
    if length(code_s)==0
      res=1; return;
    end
    
    // ligne commencant par le caractere #
    if code_s(1)==35
      res=1; return;
    end
    
    // test si le premier caractere significatif de s est num�rique
    res = 3;
    for i=1:length(code_s)
      c = code_s(i);
      // sauter les espaces ou tabulations
      if c==32 | c==9
        continue
      end
      // caractere num�rique : [09]+-.
      if c==43 | c==45 | c==46 | (c>=48 & c<=57)
        res=2; return;
      else
        res=3; return;
      end
    end
  endfunction
  
  
  // lecture dans le fichier f de la ligne significative suivante
  // ligne non vide, ne commen�ant pas par # et dont le premier
  // caractere significatif est un caractere numerique
  function s=lire_ligne_significative(f)
    res=1;
    while res==1
      s = mgetl(f,1);
      res=type_ligne(s);
      if res==0
        error('fin de fichier inattendu');
      elseif res==3
        error('format de fichier incorrect');
      end
    end
  endfunction
  

  // ouverture du fichier
  f = mopen(nom_f,'r');
  [err,msg] = merror(f);
  if err~=0
    error(msg);
  end
  
  // lecture du degr� en u
  s=lire_ligne_significative(f);
  du = msscanf(1,s,"%d");
  if du<1 | du>6
    error('le degr� en u doit etre entre 1 et 6');
  end
  
  // lecture du nombre de noeuds en u 
  s=lire_ligne_significative(f);
  nu = msscanf(1,s,"%d");
  if nu<2
    error('le nb de noeuds en u doit etre >= 2');
  end
  nu=nu-1;
  
  // lecture des nu+1 noeuds u(i)
  s=lire_ligne_significative(f);
  u = msscanf(1,s,format_reels(nu+1));
  // teste si u est form�e de valeurs croissantes
  if (min(diff(u))<0)
    error('le vecteur de noeuds u(i) n''est pas croissant');
  end
  
  // lecture du degr� en v
  s=lire_ligne_significative(f);
  dv = msscanf(1,s,"%d");
  if dv<1 | dv>6
    error('le degr� en v doit etre entre 1 et 6');
  end
  
  // lecture du nombre de noeuds en v 
  s=lire_ligne_significative(f);
  nv = msscanf(1,s,"%d");
  if nv<2
    error('le nb de noeuds en v doit etre >= 2');
  end
  nv=nv-1;
  
  // lecture des nv+1 noeuds v(j)
  s=lire_ligne_significative(f);
  v = msscanf(1,s,format_reels(nv+1));
  // teste si v est form�e de valeurs croissantes
  if (min(diff(v))<0)
    error('le vecteur de noeuds v(i) n''est pas croissant');
  end
  
  // valeurs de M et N
  M = du+nu;
  N = dv+nv;
  
  // lecture de la matrice X
  X = zeros(M,N);
  for i=1:M
    // lecture de la ligne X(i,:)
    s=lire_ligne_significative(f);
    X(i,:) = msscanf(1,s,format_reels(N));
  end
  
  // lecture de la matrice Y
  Y = zeros(M,N);
  for i=1:M
    // lecture de la ligne Y(i,:)
    s=lire_ligne_significative(f);
    Y(i,:) = msscanf(1,s,format_reels(N));
  end
  
  // lecture de la matrice Z
  Z = zeros(M,N);
  for i=1:M
    // lecture de la ligne Z(i,:)
    s=lire_ligne_significative(f);
    Z(i,:) = msscanf(1,s,format_reels(N));
  end
  
  // lecture de la matrice omega
  omega = zeros(M,N);
  for i=1:M
    // lecture de la ligne omega(i,:)
    s=lire_ligne_significative(f);
    omega(i,:) = msscanf(1,s,format_reels(N));
  end
  
  // l'hypermatrice T
  T = zeros(M,N,4);
  T(:,:,1) = X;  
  T(:,:,2) = Y;  
  T(:,:,3) = Z;  
  T(:,:,4) = omega;  
  
    // fermeture du fichier
  mclose(f);
  
endfunction
//----------------------------------------------



///////////////////////////////////////////////////////////////////////////////
//Prend en entrée le polygone de controle D, le vecteur des 
//noeuds t saturé aux extrémités, l'odre k de la courbe spline et 
//le noeuds nod à insérer.
//Retourne le polygone de contrôle X avec le noeud nod inséré et t_ins, le
//nouveau vecteur des noeuds
///////////////////////////////////////////////////////////////////////////////
function [X,t_ins]=insert_node(D,t,k,nod)
	//recherche de l'intervalle [tau_i ; tau_i+1]
	i=1
	while(nod>=t(i))
		i=i+1;
        end
	r=i-1
	
	lambda=(nod-t(r-k+2:r))./(t(r+1:r+k-1)-t(r-k+2:r))
	
	//initialisation de A0
	A=D(:,r+1-k:r)
	
	//calcul A1
	A=[(1-lambda);(1-lambda)].*(A(:,1:k-1))+[lambda;lambda].*(A(:,2:k))
	
	X=[D(:,1:r-k+1) A D(:,r:size(D,2))]
	
	i=1
	t_ins=zeros(1,size(t,2)+1)
	
	while (t(i)<nod)
		t_ins(i)=t(i)
		i=i+1
	end
	t_ins(i)=nod
	
	t_ins(i+1:$)=t(i:$)
endfunction

