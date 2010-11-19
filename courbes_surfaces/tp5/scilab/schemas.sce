function D=chaikin(P)
  n=size(P,2)
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

function D=catmullClark(P)
  n=size(P,2)
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



function D=fourPts(P)
  n=size(P,2)
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

function D=cornerCut(P,a,b)
  n=size(P,2)
  D(:,1)=P(:,1)
  D(:,2)=(P(:,1)+P(:,2))/2
  for j=1:n-2
    D(:,2*j-1)=a*P(:,j)+b*P(:,j+1)
    D(:,2*j)  =b*P(:,j)+a*P(:,j+1)
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
