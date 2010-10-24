//////////////////////////////////////////////////////////
// lecture d'une suite de B�zier rationnelles et
// trac� des polygones de controle
function prog3()
  
  // lecture de la suite de B�zier
  nom_f = 'liste_bezier.txt';
  saisie_poly_rat(nom_f)
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
  
  // lecture des B�zier
  LP = list([]); // d�finir LP comme une liste
  for i=1:nB
    // lecture du degr�
    n = mfscanf(fid, '%d');
    
    // lecture de la B�zier
    points_poids = mfscanf(n+1, fid, "%f %f %f");
    
    LP(i) = points_poids';
  end
  
  mclose(fid);
endfunction

function saisie_poly_rat(nom_f)
  L=inputpoly_rat()
  ecrit_poly_rat(nom_f,L)
endfunction