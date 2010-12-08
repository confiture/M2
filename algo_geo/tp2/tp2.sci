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

function [X]=ordElts(X,coord)
	n=size(X,1)
	for i=1:n
		if(ordre_pol(coord(:,X(i,1)),coord(:,X(i,2)),coord(:,X(i,3))))
			temp=X(i,2)
			X(i,2)=X(i,3)
			X(i,3)=temp
		end
	end
endfunction

//////////////////////////////////
//tableau aretes de la forme pour la ligne i correspondant a la demi-arete i : 
//[demi-arete suivante, demi-arete precedente, demi-arete opposee, sommet d'origine
//	sommet d'arrivee, face droite]
//////////////////////////////////
function aretes=cons_aretes(elts)
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
aretes(:,5)=[]
        
        endfunction
        
///////////////////////////////////////////////////////////////////////
//Construit les arêtes manquantes après l'exécuction de cons_aretes,
//c'est à dire les arêtes sur le bord. Une arête est sur le bord quand
//elle n'a pas de demi-arêtes opposée.
///////////////////////////////////////////////////////////////////////
function aretes=cons_aretes_bord(aretes)
  n=size(aretes,1)
  initSize=n //La taille initiale du tableau des arêtes
  i=1
  while(aretes(i,3)<>0) do
    i=i+1
  end
  init=i //la première demi-arête qui n'a pas d'opposé et qui est donc sur le bord
  
  while(1==1) do
     disp("hehe")
     aretes=[aretes ;n+2 n i aretes(i,5) aretes(i,4) 0] //on ajoute l'arête et son opposé
     n=n+1
     aretes(i,3)=n
     
     i=n
     while (aretes(i,3)<>0) do //on cherche la demi-arête dont l'opposée n'existe pas
       disp("hoho")
       i=aretes(aretes(i,3),2) 
       disp(i)
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
  
  
function traceAretes(aretes,coord)
  n=size(aretes,1)
  for i=1:n
    disp("X1 ind")
    disp(aretes(i,4))
    disp("X2 ind")
    disp(aretes(i,1),4)
    disp(aretes(aretes(i,1),4))
    X1=coord(:,aretes(i,4))
    X2=coord(:,aretes(aretes(i,1),4))
    plot([X1(1) X2(1)],[X1(2) X2(2)]);
  end
endfunction

function [points]=envPts(aretes,coord)
  points=[]
  n=size(aretes,1)
  for i=1:n,
    if(aretes(i,5)==0) then
      points=[points coord(:,aretes(i,4)) coord(:,aretes(aretes(i,1),4))]
    end
  end
endfunction

  
////////////////////////////////////////////////////////////
//Retourne le status de l'arete [pt1 pt2]
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

//////////////////////////////////////////////////////////
// lecture d'un fichier contenant une suite de B�zier
// rationnelles
// Entr�e : nom_f = le nom du fichier � lire
// Sortie : LP = la liste des B�zier
// LP(i) est un tableau avec 3 lignes
// ligne 1 : abscisses des points de controle
// ligne 2 : ordonn�es des points de controle
// ligne 3 : poids associ�s
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

//////////////////////////////////////////////////////////
// lecture d'un fichier contenant une suite de B�zier
// rationnelles
// Entr�e : nom_f = le nom du fichier � lire
// Sortie : LP = la liste des B�zier
// LP(i) est un tableau avec 3 lignes
// ligne 1 : abscisses des points de controle
// ligne 2 : ordonn�es des points de controle
// ligne 3 : poids associ�s
function [coord,elts] = lireDelaunay(nom_f)
  
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

