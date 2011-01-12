function X=scal(N,M);
  [m,n]=size(N);
  if ~((m==1)|(n==1)|~(size(N)==size(M))) then error('vecteur dim'); end ;
  X=sum(N.*M);
endfunction ;


//methode QR

function [Q,R]=qr_moi(A)
  [m,n]=size(A);
  // vérifications de base
  if ~(m==n) then error('il faut une matrice carré'); end;
  if det(A)==0 then error('matrice de determinant nul'); end;
  Q=A; 
  J=eye(n,n) //pour stocker les opérations élémentaires
    for i=1:n;
      //GS pour colonne i, sauf la premiere colonne
      if ~(i==1) then 
        V=eye(n,n); //création de la matrice élementaire
        for j=1:i-1;
          V(j,i)=-scal(Q(1:n,j),Q(1:n,i));
      end;//on place chaque prod scal suivant l'algo
        Q=Q*V;
        J=J*V;
    end;
    //enfin, on normalise la colonne i...
      U=eye(n,n); 
      U(i,i)=1/((scal(Q(1:n,i),Q(1:n,i)))^(1/2));
      Q=Q*U;
      J=J*U;
    end; 
 Q=Q;
 R=inv(J);
endfunction ;

// methode LU

function [L,U]=LU_moi(A)
  [m,n]=size(A);
  // vérifications de base
  if ~(m==n) then error('il faut une matrice carré'); end;
  if det(A)==0 then error('matrice de determinant nul'); end;
  U=A; 
  B=zeros(A);
  for i=1:n-1 ;
    J=eye(n,n);
    for j=i+1:n ;
      if U(i,i)==0 then error('aie'); end;
      B(j,i)=U(j,i)/U(i,i);      
      J(j,i)=-B(j,i);
    end ;
  U=J*U;
  end;
  U=U;
  L=B+eye(n,n);
endfunction;
