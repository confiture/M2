// surface de Bézier triangulaire
// définition de la Bézier triangulaire par ses points de controle
// P{i,j,k}=(X{i,j,k},Y{i,j,k},Z{i,j,k}) et poids W{i,j,k}
// sous forme de 4 tableaux (vecteurs-ligne) X, Y, Z et W 
// avec (n+1)*(n+2)/2 éléments (n étant le degré de la Bézier)
// les éléménts d'indices {i,j,k} sont stockés ainsi :
//   element{n,0,0} 
//   element{n-1,1,0} 
//   ...
//   element{1,n-1,0}
//   element{0,n,0}
//   element{n-1,0,1}
//   element{n-2,1,1}
//   ...
//   element{1,0,n-1}
//   element{0,1,n-1}
//   element{0,0,n}
function prog31()

//------------ 1 ------------    
// les points de controle et les poids correspondant
// definir le degré n puis 
// 4 tableaux X,Y,Z et W de longueur (n+1)*(n+2)/2

// exemple de surface de degré 3
n=3;// degré
X = [3 3 2 0  3 2 0  2 0  0];
Y = [0 2 3 3  0 2 3  0 2  0];
Z = [0 0 0 0  2 2 2  3 3  3];
W = [1 1 1 1  1 1 1  1 1  1];

//------------ 2 ------------    
// la triangulation du polyèdre de controle
// n*n triangles :
// définir S le tableau des sommets à partir de X,Y,Z
// et T le tableau des triangles
S = [X' Y' Z'];
T = zeros(n*n,3);
k = 1;i1=1;
for i=1:n
  for j=i:n
    T(k,:) = [i1 i1+1 i1+n+2-i];
    i1 = i1+1;
    k = k+1;
  end
  i1 = i1+1;
end
i1=2;
for i=1:n-1
  for j=i:n-1
    T(k,:) = [i1 i1+n+2-i i1+n+1-i];
    i1 = i1+1;
    k = k+1;
  end
  i1 = i1+2;
end

//------------ 3 ------------    
// ecriture du fichier contenant le polyedre de controle
fid = mopen("beztri-polyedre.off","w");
write_OFF(fid,S,T,[1 0.8 0.6]);
mclose(fid);

//------------ 4 ------------    
// passage en Bezier rationnelle rectangulaire
// convertir les vecteurs X,Y,Z et W en
// 4 matrices Pbar1,Pbar2,Pbar3 et Pbar4 de dimensions (n+1,n+1)
// contenant les points de controle en coordonnées homogènes 
// de la Bézier rationnelle rectangulaire équivalente
// Pbar4(k,l) correspond au poids omega(k,l)
// [Pbar1(k,l),Pbar2(k,l),Pbar3(k,l)] au produit 
// du point de controle P(k,l) par le poids omega(k,l) 

// PARTIE A COMPLETER

// ------------ 5 ------------   
// écriture de la Bézier rectangulaire rationnelle
//

// DECOMMENTER LES LIGNES SUIVANTES AVANT ENDFUNCTION 

//fid = mopen("bezrect.bez","w");
//mfprintf(fid, "BEZ%d%d4\n",n,n);
//for i=1:n+1
//  for j=1:n+1
//    mfprintf(fid, "%15.7e %15.7e %15.7e %15.7e \n",...
//      Pbar1(i,j),Pbar2(i,j),Pbar3(i,j),Pbar4(i,j));
//  end
//end
//  
//mclose(fid);

endfunction

/////////////////////////////////////////////////////////
// elevation de degre d'un polygone de controle de degre n-1
// à un polygone de controle de degré n
// Entrée : P = tableau des n points de controle
//          matrice avec n colonnes (1 point par colonne)
// Sortie : P2 = tableau des n+1 points de controle
//          matrice avec n+1 colonnes (1 point par colonne)
function P2 = elevation_degre(P)

n = size(P,2);  //ordre de la courbe P
P2 = zeros(size(P,1),n+1);
P2(:,1)   = P(:,1);
for i=1:n-1
  P2(:,i+1) = (i*P(:,i)+(n-i)*P(:,i+1))/n;
end
P2(:,n+1) = P(:,n);

endfunction


//----------------------------------------------
// ecriture d'un objet OFF triangulation
// Entrée : S, T = tableaux décrivant la triangulation
//          c = une couleur au format RGB : c = (r,g,b)
//              avec r,g et b trois réels entre 0 et 1
function write_OFF(fid,S,T,c)

nS = size(S,1);
nT = size(T,1);

// début de l'objet composite
mfprintf(fid,"{\n");

mfprintf(fid, 'OFF\n%d %d 0\n', nS, nT);
mfprintf(fid, '%15.7e %15.7e %15.7e\n', S);
mfprintf(fid, '3 %d %d %d %15.7e %15.7e %15.7e\n', [T-1 ones(nT,1)*c]);

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------
