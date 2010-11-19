function M=chaikinMat(n)
  M=zeros(2*n-2,n)
  masque=[1/4 ; 3/4 ; 3/4 ; 1/4]
  M(1:4,1)=[1 ; 1/2 ; 3/4 ; 1/4]
  M(2,2)=1/2
  
  for j=2:n-2
    M(2*j-1:2*j+2,j)=[1/4 ; 3/4 ; 3/4 ; 1/4]
  end
  
  M(2*n-3,n-2)=0
  M(2*n-3,n-1)=1/2
  M(2*n-3,n)=1/2
  
  M(2*n-2,n-2)=0  
  M(2*n-2,n-1)=0
  M(2*n-2,n)=1
endfunction

  