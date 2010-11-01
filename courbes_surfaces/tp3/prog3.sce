//////////////////////////////////////////////////////////
// lecture d'une suite de B�zier rationnelles et
// trac� des polygones de controle
function prog3()
  
  // lecture de la suite de B�zier
  nom_f = 'liste_bezier.txt';
  LP = lire_liste_Bezier(nom_f);
  
  // trac� des polygones de controle
  // en diff�rentes couleurs  
  figure('figure_name', 'Polygones de controle', 'background', 8);
  nB = length(LP); // nombre de B�zier
  couleurs = jetcolormap(nB); // couleurs 
  for i=1:nB
    points_poids = LP(i);
    points = points_poids(1:2,:);
    plot(points(1,:),points(2,:),'o', 'Color', couleurs(i,:));
    plot(points(1,:),points(2,:),'-', 'Color', couleurs(i,:));
  end
  set(gca(), 'isoview','on');
endfunction

//////////////////////////////////////////////////////////
// lecture d'un fichier contenant une suite de B�zier
// rationnelles 
// Entr�e : nom_f = le nom du fichier � lire
// Sortie : LP = la liste des B�zier
//           LP(i) est un tableau avec 3 lignes 
//             ligne 1 : abscisses des points de controle
//             ligne 2 : ordonn�es des points de controle
//             ligne 3 : poids associ�s
function LP = lire_liste_Bezier(nom_f)
  
  // ouverture du fichier
  fid = mopen(nom_f, 'r');
  
  // lecture du nombre de B�zier
  nB = mfscanf(fid, '%d');
  disp(nB)
  
  // lecture des B�zier
  LP = list([]); // d�finir LP comme une liste
  for i=1:nB
    // lecture du degr�
    n = mfscanf(fid, '%d');
    disp(n)
    
    // lecture de la B�zier
    points_poids = mfscanf(n+1, fid, "%f %f %f");
    disp(points_poids)
    
    LP(i) = points_poids';
  end
  
  disp(LP)
  
  mclose(fid);
endfunction

////////////////////////////////////////////////
//On ramène la suite des polygones de controles des Bézier
//à des polygones de contrôles de degré 3.
function [L2]=elev_deg3(L)
  nb=length(L)
  L2=L
  for i=1:nb
    deg=size(L2(i),2)
    for j=1:4-deg
      L2(i)=elev_deg_rat(L2(i))
    end
  end
endfunction

/////////////////////////////////////////////////////////////////////////
//On fabrique le vecteur des noeuds t associé à la suite des poygones de 
//contrôle de courbes Bézier L.
function t=fabrique_noeuds(L)
  nb=length(L)
  
  val_noeud=1
  t=[1 1 1]
  for i=1:nb*4
    t=[t val_noeud]
    if pmodulo(i,4)==1 then
      val_noeud=val_noeud+1
    end
  end
  
  t=[t val_noeud val_noeud val_noeud]
endfunction

//////////////////////////////////////////////////////////////////////
//conversion d'une suite de Bézier en une NURBS
//LP est la suite des polygones de contrôle des courbes de Bézier
//L est la matrice de sortie correspondant au polygone de contrôle de 
//la NURBS.
//t est le vecteur des noeuds associé à la NURBS.
function [L,t]=conversion(LP)
  Ltemp=elev_deg3(LP)
  nb=length(LP)
  
  L=[]
  for i=1:nb
    L=[L Ltemp(i)]
  end
  
  t=fabrique_noeuds(LP)
endfunction

//////////////////////////////////////////////////////////////////////
//conversion d'une suite de Bézier en une NURBS et simplification
//LP est la suite des polygones de contrôle des courbes de Bézier
//Lsimp est la matrice de sortie correspondant au polygone de contrôle de 
//la NURBS.
//tsimp est le vecteur des noeuds associé à la NURBS.
function [Lsimp,tsimp]=conv_simplif(L)
  elimine_noeud=[]
  Ltemp=elev_deg3(LP)
  Lsimp=[]
  val_noeud=2
  tsimp=[1 2 2 2]
  nb=length(LP)
  
  Lsimp=[Lsimp Ltemp(1)]
  for i=1:nb-1,
    pt1=Ltemp(i)(:,4)
    pt2=Ltemp(i+1)(:,1)
    if( (pt1(1)==pt2(1)) & (pt1(2)==pt2(2)) ) then
      Ltemp(i+1)(3,:)=Ltemp(i+1)(3,:)/Ltemp(i+1)(3,1)*Ltemp(i)(3,4) //pour avoir des poids égaux
      morceau(1,1:3)=Ltemp(i+1)(1,1:3)
      morceau(2,1:3)=Ltemp(i+1)(2,1:3)
      morceau(3,1:3)=Ltemp(i+1)(3,1:3)
      Lsimp=[Lsimp morceau]
      tsimp=[tsimp val_noeud+1 val_noeud+1 val_noeud+1]
    else
      Lsimp=[Lsimp Ltemp(i+1)]
      tsimp=[tsimp val_noeud val_noeud+1 val_noeud+1 val_noeud+1]
    end
    val_noeud=val_noeud+1
  end
endfunction
  
