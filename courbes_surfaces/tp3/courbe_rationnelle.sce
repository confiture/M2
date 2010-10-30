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


//trace la courbe rationnelle à partir de la matrice L
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

  
  
   