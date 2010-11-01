//////////////////////////////////////////////////////////////////////////////
// calcul de n!/p!/(n-p)!
function cnp=nchoosek(n,p)
if n<0 then n=0; end
if p>n/2 then p=n-p; end
if p<0 then p=0; end

cnp=1;
for i=1:p
  cnp = cnp * (n+1-i)/i;
end

endfunction

function [P2]=coord_homo(P)
  P2=P
  P2(1,:)=P2(1,:).*P2(3,:)
  P2(2,:)=P2(2,:).*P2(3,:)
endfunction

function [P2]=proj_plan(P)
  P2=P
  P2(1,:)=P2(1,:)./P2(3,:)
  P2(2,:)=P2(2,:)./P2(3,:)
endfunction

//calcule la courbe rationnelle à partir de la matrice L
//L : matrice de 3 lignes, sur chaque colonne on a (x,y,poids)
//t : vecteur allant de 0 à 1
function [C]=courbe_rat(L,t)
  P=L
  n=size(L,2)-1
  BNt=zeros(n+1,length(t));//Bernstein pondere avec les poids 
  for k = 0 : n
    BNt(k+1,:) =  nchoosek(n,k)*t.^k.*(1-t).^(n-k);
    P(1:2,k+1) = P(1:2,k+1).*P(3,k+1)
  end
  
  C=(P*BNt)
  
  C= [C(1,:)./C(3,:) ; C(2,:)./C(3,:)]
endfunction


//elevation du degre du polygone P, les points de P
//doivent être en coordonnées homogènes.
function [P2]=elev_deg(P)
	n=size(P,2)
	P2=zeros(3,n+1)
	P2(:,1)=P(:,1)
	P2(:,n+1)=P(:,n)
	vec1=[(1:n-1)./n;(1:n-1)./n ;(1:n-1)./n]
	vec2=[(n-(1:n-1))./n;(n-(1:n-1))./n;(n-(1:n-1))./n]
	
	P2(:,2:n)=vec1.*P(:,1:n-1)+vec2.*P(:,2:n)
endfunction

//Elevation de degré.
function [P2]=elev_deg_rat(P)
  n=size(P,2)
  P2=zeros(3,n)
  
  //passage en coordonnées homogènes
  P2(1,:)=P(1,:).*P(3,:)
  P2(2,:)=P(2,:).*P(3,:)
  P2(3,:)=P(3,:)

  P3=elev_deg(P2)
  P2=P3
  
  //on revient aux coordonnées normales en projetant sur le plan z=1
  P2(1,:)=P2(1,:)./P2(3,:)
  P2(2,:)=P2(2,:)./P2(3,:)
endfunction

//Normalise les poids de telle sorte à avoir p(1)=p(n)=1
//Retourne les poids normalisés.
function [pnorm]=norm_poids(poids)
  pnorm=poids
  pnorm=pnorm/pnorm(1)
  
  wbn=pnorm($)
  div=ones(1,size(pnorm,2))*wbn
  div=div.^(ones(1,size(pnorm,2))./(1:size(pnorm,2)))
  pnorm=pnorm./div
endfunction