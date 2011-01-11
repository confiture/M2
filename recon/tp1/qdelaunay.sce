/////////////////////////////////////////////////////////////////////////////
// interface avec le programmes qdelaunay 
// a) récupérer les programmes sources sur le site http://www.qhull.org
// b) compiler les programmes
//    -> exécutables qhull qvoronoi rbox qconvex qdelaunay qhalf
// Seul le programme exécutable qdelaunay est utilisé dans ce script
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// récupérer la date actuelle sous forme d'un chaine de caractere
// au format AAMMJJhhmmss
function s = datestring()
  dt = getdate();
  s = msprintf("%04d%02d%02d%02d%02d%02d", dt([1 2 6 7 8 9]));
endfunction

/////////////////////////////////////////////////////////////////////////////
// calcul du centre de la sphere circonscrite à une d-cellule de R^d
// Entree : S = d+1 points de dimension d, sommets de la d-cellule
//              tableau de d+1 lignes et d colonnes (1 point par ligne)
// Sortie : C = point de R^d (ligne de d colonnes)
function C = centre_sphere_circonscrite(S)
   
  d = size(S,2);
  M = zeros(d,d);
  b = zeros(d,1);
  for i=1:d
    for j=1:d
      M(i,j) = S(d+1,j)-S(i,j);
      b(i) = b(i)+0.5*(S(d+1,j)^2-S(i,j)^2);
    end
  end
  C = (M\b)';
endfunction

function test_centre_sphere_circonscrite()

  // test 1
  d = 10;
  S = rand(d+1,d);
  C = centre_sphere_circonscrite(S);
  mprintf("C = ");
  mprintf("%15.7e ",C');
  mprintf("\n");
  for i=1:d+1
    mprintf(" ||C-S(%2d)|| = %15.7e\n", i, norm(C-S(i,:)));
  end
  
endfunction

/////////////////////////////////////////////////////////////////////////////
// calcul de la triangulation de Delaunay en dimension d
// Entrée : S = tableau des nS sommets 
//               matrice de nS lignes et d colonnes
//               une ligne = les d coordonnees d'un sommet 
// Sortie : T = tableau des nT cellules de la triangulation
//               matrice de nT lignes et d+1 colonnes
//               une ligne = les indices des d+1 sommets de la cellule
//               l'indiçage fait référence au tableau S et commence à 1
//          C = centre des sphères circonscrites aux cellules
//               matrice de nT lignes et d colonnes 
//               une ligne = les d coordonnees d'un centre
//          r = rayon des sphères circonscrites aux cellules
//               vecteur de nT réels
function [T,C,r] = delaunay(S)

  nargout = argn(1);
  nargin = argn(2);
  if nargin~=1
    error('syntaxe : T = delaunay(S)');
  end
  
  // dimensions du tableau de sommets
  nS = size(S,1);
  d = size(S,2);
  
  // nom des fichiers temporaires pour l'interface avec qhull
  date_s = datestring();
  nom_fichier_sommets = msprintf("qhullS-%s.tmp.o", date_s);
  nom_fichier_triangles = msprintf("qhullT-%s.tmp.o", date_s);
  
  // écriture du tableau de sommets dans le fichier nom_fichier_sommets
  f = mopen(nom_fichier_sommets,"w");
  mfprintf(f, "%d\n%d\n", d, nS);
  for i=1:nS
    for j=1:d
      mfprintf(f, " %15.7e", S(i,j));
    end
    mfprintf(f, "\n");
  end
  mclose(f);
  
  // appel du programme qdelaunay
  if MSDOS
    commande = msprintf("qdelaunay.exe Qt s i < %s > %s", ...
     nom_fichier_sommets, nom_fichier_triangles);
  else
    commande = msprintf("./qdelaunay Qt s i < %s > %s", ...
     nom_fichier_sommets, nom_fichier_triangles);
  end
  host(commande);
  
  // lecture des triangles
  f = mopen(nom_fichier_triangles,"r");
  nT = mfscanf(f, "%d");
  T = zeros(nT,d+1);
  for i=1:nT
    for j=1:d+1
      T(i,j) = mfscanf(f, "%d");
    end
  end
  T = T+1; // indicage a partir de 1
  mclose(f);
  
  // destruction des fichiers temporaires
  mdelete(nom_fichier_sommets);
  mdelete(nom_fichier_triangles);
  
  if nargout>1
    // calcul des centres des spheres circonscrites
    C = zeros(nT,d);
    for i=1:nT
      C(i,:) = centre_sphere_circonscrite(S(T(i,:),:));
    end    
    if nargout>2
      // calcul des rayons
      r = zeros(nT,1);
      for i=1:nT
        r(i) = norm(S(T(i,1),:)-C(i,:));
      end      
    end    
  end
  
endfunction

/////////////////////////////////////////////////////////////////////////////
// dessin d'une triangulation en 2D
function dessin_delaunay(S,T)

  nargin = argn(2);
  if nargin~=2
    error('syntaxe : dessin_delaunay(S,T)');
  end
  if size(S,2)~=2
    error('le tableau de sommets doit avoir 2 colonnes');
  end
  if size(T,2)~=3
    error('le tableau de triangles doit avoir 3 colonnes');
  end
  nS = size(S,1);
  nT = size(T,1);
  
  plot(S(:,1),S(:,2),'ko');
  for i=1:nS
    xstring(S(i,1),S(i,2),msprintf("%d",i));
  end
  
  T2 = [T T(:,1)]';
  Sx = zeros(4,nT);
  Sy = zeros(4,nT);
  for i=1:4
    Sx(i,:) = S(T2(i,:),1)';
    Sy(i,:) = S(T2(i,:),2)';
  end    
  plot(Sx,Sy,'r-');
//  nT = size(T,1);
//  for i=1:nT
//    s1 = T(i,1);
//    s2 = T(i,2);
//    s3 = T(i,3);
//    plot(S([s1 s2 s3 s1],1),S([s1 s2 s3 s1],2),'r-');
//  end
  a=gca();
  xmin = min(S(:,1)); xmax = max(S(:,1)); 
  ymin = min(S(:,2)); ymax = max(S(:,2)); 
  a.data_bounds=[...
   xmin-0.2*(xmax-xmin),ymin-0.2*(ymax-ymin);...
   xmax+0.2*(xmax-xmin),ymax+0.2*(ymax-ymin)];
  a.isoview="on";
  
endfunction

//////////////////////////////////////////////////////////////////////////////
// test de la routine delaunay
//////////////////////////////////////////////////////////////////////////////
function test_delaunay(num_test)
  
  if argn(2)<1    
    num_test=0;
  end
  
  select num_test
    
    case 1
  //// test 1 
  // les données
  S = [
    0 0;
    2 0;
    4 0;
    0 2;
    1 2;
    3 2;
    2 3];
  [T,C,r] = delaunay(S);
  
  // tracé de la triangulation de Delaunay
  scf();
  dessin_delaunay(S,T);
  
  // tracé des centres des cercles circonscrits
  plot(C(:,1),C(:,2),'g+');
  
  // tracé des cercles circonscrits  
  t = linspace(0,2*%pi,1000);
  for i=1:size(C,1)
    plot(C(i,1)+r(i)*cos(t),C(i,2)+r(i)*sin(t),'g-');
  end
  
  
    case 2 then
  //// test 2 - points aléatoires
  S = rand(100,2);
  T = delaunay(S);
  scf();
  dessin_delaunay(S,T);
  
    case 3 then
  //// test 3 - points suivant une grille régulière
  S = zeros(100,2);
  for i=0:9
    for j=1:10
      S(i*10+j,:)=[2*j+i 2*i];
    end
  end
  T = delaunay(S);
  scf();
  dessin_delaunay(S,T);
  
    case 4 then
  
  //// test 4 - points entrés à la souris
  S = inputpoints()';
  [T,C] = delaunay(S);
  
  // tracé de la triangulation de Delaunay
  dessin_delaunay(S,T);
  
  // tracé des centres des cercles circonscrits
  plot(C(:,1),C(:,2),'g+');
    
  end // select

endfunction

//////////////////////////////////////////////////////////////////////////////
// entrée d'un ensemble de points à la souris
// en sortie, le tableau X avec p points du plan (dimensions 2 x p)
function X = inputpoints()

f=scf();  // une nouvelle fenetre
set(gca(),"auto_clear","off")
set(gca(),"data_bounds",[0,0;100,100])      // bornes des axes en x et y
set(gca(),"margins",[0.05,0.05,0.05,0.05])  // marges du repere dans la fenetre
set(gca(),"box","on")                       
set(gca(),"isoview","on")                       
set(gca(),"auto_scale","off")                       

// boucle de saisie des points
but = 3;
i = 0;
while but==3 | but==0 | but==10 | but==20
  xinfo("Point suivant : bouton gauche - Dernier point : bouton droite");
  i = i+1;
  [but,v0,v1] = xclick();
  X(1,i) = v0;
  X(2,i) = v1;
  plot(X(1,i),X(2,i),"ro")
end;

endfunction


