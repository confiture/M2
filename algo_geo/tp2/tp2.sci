function tab=demiaretes(coord,elts)
	tab=[]
	nbelts=size(elts,2)
	for i=1:nbelts
		arete()
		
	end
	
	
endfunction

function aretes=cons_aretes(elts)
	aretes=[]
	nbelts=size(elts,2)
	for i=1:nbelts
		arete()
		
	end
endfunction

function bool=status_arete(aretes,pt1,pt2)
	n=size(aretes,1)
	i=1
	bool=0
	while(i<=n & (pt1!=arete(i,1) || pt2!=arete(i,2))
		i=i+1
	end
	
	if i==n+1 then
		bool=1
	end
	
	while(i<=n & (pt2!=arete(i,1) || pt1!=arete(i,2))
		i=i+1
	end
	
	if i==n+1 then
		bool=2
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