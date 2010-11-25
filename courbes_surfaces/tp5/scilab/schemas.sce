//////////////////////////////////////////////////////////////////////
//Prend le polygone de contrôle P en entrée.
//P(1,:) contient les abscisses des points de contrôle.
//P(2,:) contient les ordonnées des points de contrôle.
//Sort le polygone subdivisé D qui est le résultat de la subdivision
//de P par la méthode de Chaikin.
//////////////////////////////////////////////////////////////////////
function D=chaikin(P)
  n=size(P,2) //le nombre de points du polygone P
  D(:,1)=P(:,1)
  D(:,2)=(P(:,1)+P(:,2))/2
  for j=1:n-2
    D(:,2*j-1)=0.75*P(:,j)+0.25*P(:,j+1)
    D(:,2*j)=0.25*P(:,j)+0.75*P(:,j+1)
  end
  D(:,2*n-3)=(P(:,n-1)+P(:,n))/2
  D(:,2*n-2)=P(:,n)
endfunction

function D=chaikinIter(P,k)
  D=P
  for i=1:k
    D=chaikin(D)
  end
endfunction

//////////////////////////////////////////////////////////////////////
//Prend le polygone de contrôle P en entrée.
//P(1,:) contient les abscisses des points de contrôle.
//P(2,:) contient les ordonnées des points de contrôle.
//Retourne le polygone D subdivisé une fois qui est le résultat de la subdivision
//de P par la méthode de Catmull-Clark.
//////////////////////////////////////////////////////////////////////
function D=catmullClark(P)
  n=size(P,2) //le nombre de points du polygone P
  D(:,1)=P(:,1)
  D(:,2)=(P(:,1)+P(:,2))/2
  for j=2:n-1
    D(:,2*j-1)=(P(:,j-1)+6*P(:,j)+P(:,j+1))/8
    D(:,2*j)  =(P(:,j)+P(:,j+1))/2
  end
  D(:,2*n-1)=P(:,n)
endfunction

function D=catmullClarkIter(P,k)
  D=P
  for i=1:k
    D=chaikin(D)
  end
endfunction


//////////////////////////////////////////////////////////////////////
//Prend le polygone de contrôle P en entrée.
//P(1,:) contient les abscisses des points de contrôle.
//P(2,:) contient les ordonnées des points de contrôle.
//Sort le polygone subdivisé D qui est le résultat de la subdivision
//de P par la méthode de Four Points scheme.
//////////////////////////////////////////////////////////////////////
function D=fourPts(P)
  n=size(P,2) //le nombre de points du polygone P
  D(:,1)=P(:,1)
  D(:,2)=(3*P(:,1)+6*P(:,2)-P(:,3))/8
  for j=2:n-2
    D(:,2*j-1)=P(:,j)
    D(:,2*j)  =(-P(:,j-1)+9*P(:,j)+9*P(:,j+1)-P(:,j+2))/16
  end
  D(:,2*n-3)=P(:,n-1)
  D(:,2*n-2)=(-P(:,n-2)+6*P(:,n-1)+3*P(:,n))/8
  D(:,2*n-1)=P(:,n)
endfunction

function D=fourPtsIter(P,k)
  D=P
  for i=1:k
    D=fourPts(D)
  end
endfunction

//////////////////////////////////////////////////////////////////////
//Prend le polygone de contrôle P en entrée.
//P(1,:) contient les abscisses des points de contrôle.
//P(2,:) contient les ordonnées des points de contrôle.
//a et b sont des réels, paramètres du schéma Corner-Cutting.
//Sort le polygone subdivisé D qui est le résultat de la subdivision
//de P par la méthode de Corner-Cutting:
//  D(:,2*j-1)=a*P(:,j)+(1-a)*P(:,j+1)
//  D(:,2*j)  =b*P(:,j)+(1-b)*P(:,j+1)
//////////////////////////////////////////////////////////////////////
function D=cornerCut(P,a,b)
  n=size(P,2)
  D(:,1)=P(:,1)
  D(:,2)=(P(:,1)+P(:,2))/2
  for j=1:n-2
    D(:,2*j-1)=a*P(:,j)+(1-a)*P(:,j+1)
    D(:,2*j)  =b*P(:,j)+(1-b)*P(:,j+1)
  end
  D(:,2*n-3)=(P(:,n-1)+P(:,n))/2
  D(:,2*n-2)=P(:,n)
endfunction

function D=cornerCutIter(P,a,b,k)
  D=P
  for i=1:k
    D=cornerCut(D,a,b)
  end
endfunction

//////////////////////////////////////////////////////////////////////
//Prend le polygone de contrôle P en entrée.
//P(1,:) contient les abscisses des points de contrôle.
//P(2,:) contient les ordonnées des points de contrôle.
//Sort le polygone subdivisé D qui est le résultat de la subdivision
//de P par la méthode de Catmull-Clark.
//////////////////////////////////////////////////////////////////////
function D=chouFleur(P)
  n=size(P,2) //le nombre de points du polygone P
  for i=1:n-1
    D(:,2*i-1)=P(:,i)
    N=P(:,i+1)-P(:,i)
    N=[-N(2,1) ; N(1,1)]/3
    D(:,2*i)=(P(:,i)+P(:,i+1))/2+N
  end
  D(:,2*n-1)=D(:,n)
endfunction
    
function D=chouFleurIter(P,k)
  D=P
  for i=1:k
    D=chouFleur(D)
  end
endfunction















