exec('util.sce')
exec('courbe_rationnelle.sce')

//question 1
function q11()
  L=inputpoly_rat()
  t=0:0.01:1  
  C=courbe_rat(L,t)
  
  plot(C(1,:),C(2,:))
  lines(0);
  disp(C);
  disp(L);
endfunction

function q22()
  L=inputpoly_rat()
  disp(L)
  t=0:0.01:1  
  C=courbe_rat(L,t)  
  plot(C(1,:),C(2,:))
  
  L(3,2)=L(3,2)+L(3,2)/2.0  
  D=courbe_rat(L,t)
  disp(L)
  plot(D(1,:),D(2,:))
  
  
  L(3,2)=L(3,2)+L(3,2)/2.0   
  C=courbe_rat(L,t)
  plot(C(1,:),C(2,:))
endfunction
  