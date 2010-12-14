
//-----------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------//
//--------------------------FONCTIONS REPONDANT AUX QUESTIONS------------------------------------//
//-----------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------// 

//-----------------------------------------------------------------------------------------//
// Cette fonction permet de tracer la triangulation à partir de la structure demi-arête
// Entrée : nom_f = le nom du fichier � lire
// Sortie : aretes = structure demi-arete
function [aretes]=question3(nom_f)
  // Lecture du fichier
  [coord,elts] = lireDelaunay(nom_f)
  // On ordonne les éléments
  [elts]=ordElts(elts,coord)
  // On obtient la structure demi-arête
  aretes=cons_aretes(elts,coord)
  // On trace les demi-aretes
  traceAretes(aretes,coord)
endfunction
//-----------------------------------------------------------------------------------------//  
// Cette fonction permet de tracer l'enveloppe convexe à partir de la structure demi-arête
// Entrée : nom_f = le nom du fichier � lire
// Sortie : points = liste des points de l'enveloppe convexe 
function [points]=question41(nom_f)
  // Lecture du fichier
  [coord,elts] = lireDelaunay(nom_f)
  // On ordonne les éléments
  [elts]=ordElts(elts,coord)
  // On obtient la structure demi-arete
  aretes=cons_aretes(elts,coord)
  // On calcule l'enveloppe convexe à partir de la structure demi-arête
  [points]=envPts(aretes,coord)
  // On trace l'enveloppe convexe
  plot(points(1,:),points(2,:), 'b')
  plot(points(1,:), points(2,:), 'bo' ); 
endfunction
//-----------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------//  
// Cette fonction permet de tracer les triangles coloriés, en affectant des couleurs à deux couleurs adjacents
// Entrée : nom_f = le nom du fichier � lire
function question42(nom_f)
  // Lecture du fichier
  [coord,elts] = lireDelaunay(nom_f)
  // On ordonne les éléments
  [elts]=ordElts(elts,coord)
  // On obtient la structure demi-arete
  aretes=cons_aretes(elts,coord)
  // On colorie les faces triangles en affectant des couleurs différentes à deux triangles 
  //  adjacents
  colorFaces(aretes,coord)
endfunction
//-----------------------------------------------------------------------------------------//   
//-----------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------//
//-------------------------------FONCTIONS PRINCIPALES-------------------------------------------//
//-----------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------//
// Cette fonction permet de construire la structure demi-arête.
// Entrée : aretes = structure demi-arete définie de la façon suivante :
// pour la ligne i correspondant à la demi-arete i on a : 
// aretes(i)=[demi-arete suivante, demi-arete precedente, demi-arete opposee, sommet d'origine, face droite]
//////////////////////////////////
function aretes=cons_aretes(elts,coord)
	aretes=[]
	nbelts=size(elts,1)
	for i=1:nbelts
		n=size(aretes,1)
		a=[]
		a=[elts(i,1) elts(i,2)]
		a=[a ; elts(i,2) elts(i,3)]
		a=[a ; elts(i,3) elts(i,1)]
		
		pos=[]			
		pos(1)=status_arete(aretes,a(1,1),a(1,2))
		pos(2)=status_arete(aretes,a(2,1),a(2,2))
		pos(3)=status_arete(aretes,a(3,1),a(3,2))
		
		for j=1:3
		         precedente=n+pmodulo(j+1,3)+1
			suivante=n+pmodulo(j,3)+1
			aretes=[aretes ; suivante precedente pos(j) a(j,:) i]
			if(pos(j)<>0) then
			  aretes(pos(j),3)=n+j
			end
		end
        end
        aretes=cons_aretes_bord(aretes)    
        
        // On supprime la colonne   
        aretes(:,5)=[]       
        endfunction
//-----------------------------------------------------------------------------------------//        
// Cette fonction rajoute les demi-arêtes manquantes après l'exécution de cons_aretes,
// c'est à dire les demi-arêtes sur le bord. Une demi-arête est sur le bord quand
// elle n'a pas de demi-arête opposée.
// Entrée : aretes = structure demi-arete intermédiaire définie de la façon suivante :
// pour la ligne i correspondant à la demi-arete i on a : 
// aretes(i)=[demi-arete suivante, demi-arete precedente, demi-arete opposee, sommet d'origine, sommet d'arrivée, face droite]
// Sortie : aretes = tableau aretes précédent avec ajouts des demi-aretes manquantes
function aretes=cons_aretes_bord(aretes)
  n=size(aretes,1)
  initSize=n //La taille initiale du tableau des arêtes
  i=1
  while(aretes(i,3)<>0) do
    i=i+1
  end
  init=i //la première demi-arête qui n'a pas d'opposé et qui est donc sur le bord
  
  while(1==1) do
     aretes=[aretes ;n+2 n i aretes(i,5) aretes(i,4) 0] //on ajoute l'arête et son opposé
     n=n+1
     aretes(i,3)=n
     
     i=n
     while (aretes(i,3)<>0) do //on cherche la demi-arête dont l'opposée n'existe pas
       i=aretes(aretes(i,3),2) 
       if(i==init) then //on s'arête si on retombe sur la première demi-arête traitée qui n'avait
                        //pas de demi-arête opposée
         break
       end
     end 
     
     if(i==init) then
       aretes(initSize+1,2)=n //on met a jour la demi-arête précédente
                              //de la première demi-arête traitée qui n'avait
                              //de demi-arête opposée puis on arête la boucle
       aretes(n,1)=initSize+1
       break
     end
   end
endfunction
//-----------------------------------------------------------------------------------------//     
// Cette fonction permet de colorier les triangles, en affectant des couleurs différentes à deux triangles adjacents.
// Entrée : -aretes = structure demi-arete
//          -coord = liste des coordonnées des points
function colorFaces(aretes,coord)
  n=size(aretes,1)
  
  //------------------------initialisation du tableau colored----------------------------//
  // colored(1)= numero de la couleur => cest le numéro de la couleur de la face droite 1
  // size(colored) = nombre de faces de la structure demi-arete
  // On a le choix entre 4 numéros de couleur : - 0 = pas de couleurs
  //                                            - 1 = couleur noir
  //                                            - 2 = couleur bleu
  //                                            - 3 = couleur verte
  for i=1:n
    if(aretes(i,5)<>0) then
      colored(aretes(i,5))=0
    end
  end
//------------------------------- FOR ---------------------------//  
  for i=1:n    
    // face droite
    faceD_arete = aretes(i,5)
    
    //---------------------------- IF ----------------------------//
    if(faceD_arete<>0 & ~colored(faceD_arete))
      // premiere face gauche
      faceG1_arete = aretes(aretes(i,3),5)
      // deuxieme face gauche
      aretes_suiv1 = aretes(i,1)
      aretes_suiv_opp1 = aretes(aretes_suiv1,3)
      faceG2_arete = aretes(aretes_suiv_opp1,5)
      // troisieme face gauche
      aretes_suiv2 = aretes(aretes_suiv1,1)
      aretes_suiv_opp2 = aretes(aretes_suiv2,3)
      faceG3_arete = aretes(aretes_suiv_opp2,5)
      //----------------------------------------------------------//      
      // Selection de la bonne couleur par rapport aux 3 faces voisines. C'est-à-dire, 
      // on choisit une couleur différente des 3 autres couleurs; //
      //----------------------------------------------------------//         
      neighCol=zeros(4,1)    
      if(faceG1_arete<>0 & colored(faceG1_arete)<>0)
        neighCol(colored(faceG1_arete))=1
      end 
      if(faceG2_arete<>0 & colored(faceG2_arete)<>0)        
        neighCol(colored(faceG2_arete))=1
      end
      if(faceG3_arete<>0 & colored(faceG3_arete)<>0)
        neighCol(colored(faceG3_arete))=1
      end
      
      // On parcourt le tableau pour récupérer le numéro de la couleur       
      jCol=1
      while (neighCol(jCol)<>0) do
        jCol=jCol+1
      end    
      //----------------------------------------------------------// 
      //----------------------------------------------------------// 
      // Mise à jour du tableau des couleurs de triangles
      colored(faceD_arete)=jCol
      // On colorie le triangle correspondant      
      colorFaceInt(aretes,coord,i,jCol) 
    end
    //--------------------------END IF-----------------------------//

  end
//------------------------------- END FOR ---------------------------//    
endfunction

//-----------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------//
//-------------------------------FONCTIONS INTERMEDIAIRES----------------------------------------//
//-----------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------//
// Cette fonction renvoie 1 si angle(Ox,OM)<angle(Ox,OP) ou angle(Ox,OM) = angle(Ox,OP) et norme(OM) <= norme(OP)
//				  renvoie 0 sinon.
function [X]=ordre_pol(O,M,P)
	// Construction de la matrice qui nous permet de déterminer l'ordre polaire
	mat=zeros(3,3);
	mat(1,1)=1
	mat(1,2)=1
	mat(1,3)=1
	mat(2,1)=O(1);
	mat(2,2)=M(1);
	mat(2,3)=P(1);
	mat(3,1)=O(2);
	mat(3,2)=M(2);
	mat(3,3)=P(2);
	determinant=det(mat);	
	if(determinant>0) then; X=1;end;
	if(determinant<0) then; X=0;end;
	if(determinant==0) then; X=(norm(M-O)>norm(P-O));end;
endfunction
//-----------------------------------------------------------------------------------------//
// Cette fonction ordonne les éléments.
// Elle prend en paramètre la liste des elements
function [elts]=ordElts(elts,coord)
	n=size(elts,1)
	for i=1:n
		if(ordre_pol(coord(:,elts(i,1)),coord(:,elts(i,2)),coord(:,elts(i,3))))
			temp=elts(i,2)
			elts(i,2)=elts(i,3)
			elts(i,3)=temp
		end
	end
endfunction
//-----------------------------------------------------------------------------------------//  
// Cette fonction permet de tracer les demi-arêtes à partir de la structure demi-arête et des coordonnées des points
//Entrée :  aretes = structure demi-arête
//          coord = liste des coordonnées des points 
function traceAretes(aretes,coord)
  n=size(aretes,1)
  for i=1:n
    X1=coord(:,aretes(i,4))
    X2=coord(:,aretes(aretes(i,1),4))
    plot([X1(1) X2(1)],[X1(2) X2(2)], 'b');
    plot([X1(1) X2(1)],[X1(2) X2(2)], 'bo' ); 
  end
endfunction
//-----------------------------------------------------------------------------------------//  
// Cette fonction permet de renvoyer les points de l'enveloppe convexe
// Entrée :  - aretes = structure demi-arête
//           - coord = liste des coordonnées des points 
// Sortie :  - points = liste des points de l'enveloppe convexe
function [points]=envPts(aretes,coord)
  points=[]
  n=size(aretes,1)
  for i=1:n,
    if(aretes(i,5)==0) then
      points=[points coord(:,aretes(i,4)) coord(:,aretes(aretes(i,1),4))]
    end
  end
endfunction
//-----------------------------------------------------------------------------------------//  
// Cette fonction retourne le status de l'arete [pt1 pt2]
//si pos=0 : alors l'arete opposee n'existe pas dans le tableau d'aretes aretes
//si pos<>0 : alors l'oppose existe a la position pos dans aretes
//tableau aretes de la forme pour la ligne i correspondant a la demi-arete i : 
//[demi-arete suivante, demi-arete precedente, demi-arete opposee, sommet d'origine
//	sommet d'arrivee, face droite]
////////////////////////////////////////////////////////////
function pos=status_arete(aretes,pt1,pt2)
	pos=0
	n=size(aretes,1)
	i=1
	
	while(i<=n & (pt2<>aretes(i,4) | pt1<>aretes(i,5)))
		i=i+1
	end
	
	if i<>n+1 then
		pos=i
	end
endfunction
//-----------------------------------------------------------------------------------------//   
// lecture du fichier de données.
// Entr�e : nom_f = le nom du fichier � lire
// Sortie : coord = liste des coordonnées des points 
//          elts  = liste des numéros de sommets composant les triangles. 
//          elts(1,1) = numero_Sommet1  du triangle 1  
//          elts(1,2) = numero_Sommet2  du triangle 1 
//          elts(1,3) = numero_Sommet3  du triangle 1
function [coord,elts] = lireDelaunay(nom_f)
  
  // ouverture du fichier
  fid = mopen(nom_f, 'r');
  
  // lecture du nombre de points
  nB = mfscanf(fid, '%d');
  
  // lecture des coordonnées
  for i=1:nB
    coord(1,i) = mfscanf(fid, '%f');
    coord(2,i) = mfscanf(fid, '%f');
  end
   
   nbelts=mfscanf(fid,'%d')
   
   for i=1:nbelts
	elts(i,:)=[mfscanf(fid, '%d') mfscanf(fid, '%d') mfscanf(fid, '%d')];
  end
  mclose(fid);
endfunction
//-------------------------------------------------------------//
// Cette fonction permet de colorier un triangle.
// Entrée : -aretes = structure demi-arete
//          -coord = liste des coordonnées des points
//          -numAr = numéro de la demi-arete ayant pour face droite le triangle qu'on souhaite colorier
//          -col = couleur
function colorFaceInt(aretes,coord,numAr,col)   
  pts_triangle=[coord(:,aretes(numAr,4)) coord(:,aretes(aretes(numAr,1),4)) coord(:,aretes(aretes(aretes(numAr,1),1),4)) coord(:,aretes(numAr,4))]
  xset("color",col)
  xfpoly(pts_triangle(1,:),pts_triangle(2,:))
endfunction





//////////////////////////////////////////////////////////
// lecture du fichier de données.
// Entr�e : nom_f = le nom du fichier � lire
// Sortie : coord = liste des coordonnées des points 
//          elts  = liste des numéros de sommets composant les triangles. 
//          elts(1,1) = numero_Sommet1  du triangle 1  
//          elts(1,2) = numero_Sommet2  du triangle 1 
//          elts(1,3) = numero_Sommet3  du triangle 1
function [coord,elts] = lire_delaunay(nom_f)
  
  // ouverture du fichier
  fid = mopen(nom_f, 'r');
  
  // lecture du nombre de points
  nB = mfscanf(fid, '%d');
  
  // lecture des coordonnées
  for i=1:nB
     coord(1,i) = mfscanf(fid, '%f');
  end
   for i=1:nB
		coord(2,i) = mfscanf(fid, '%f');
	end
	i=1;
   while(1==1)
	if(meof(fid)) then 
		break;
	end
	elts(i,:)=[mfscanf(fid, '%d') mfscanf(fid, '%d') mfscanf(fid, '%d')];
	i=i+1;  
end
  mclose(fid);
endfunction
