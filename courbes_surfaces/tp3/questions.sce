exec('util.sce')
exec('courbe_rationnelle.sce')
exec('nurbs.sce')

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

function q12()
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

function q13()
  Linit=inputpoly_rat()
  t=0:0.01:1  
  C=courbe_rat(Linit,t)
  plot(C(1,:),C(2,:))
  xs2png(0,"q13-init.png")

   
  Ln1=Linit
  Ln1(3,:)=Ln1(3,:)/Ln1(3,1)
  C=courbe_rat(Ln1,t)
  creer_fig_iso()
  plot(C(1,:),C(2,:))
  xs2png(1,"q13-n1.png")
  
  Ln2=Ln1
  
  wbn=Ln2(3,$)
  div=ones(1,size(Ln2,2))*wbn
  div=div.^(ones(1,size(Ln2,2))./(1:size(Ln2,2)))
  Ln2(3,:)=Ln2(3,:)./div
  
  C=courbe_rat(Ln2,t)
  creer_fig_iso()
  plot(C(1,:),C(2,:))
  xs2png(2,"q13-n2.png") 
endfunction

function q14()
  Linit=inputpoly_rat()
  t=0:0.01:1  
  C=courbe_rat(Linit,t)
  plot(Linit(1,:),Linit(2,:),"b")
  plot(C(1,:),C(2,:))
  
  L=elev_deg_rat(Linit)
  C=courbe_rat(L,t)
  plot(L(1,:),L(2,:),"r")
  plot(C(1,:),C(2,:))
  
  xs2png(0,"q14.png")
endfunction


