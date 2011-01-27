exec("polynomes.sci");

function SM1()
  t=([1:1:9]-ones(1,9))*%pi/4
  y=sin(t)
  
  p=polyfit(t,y)
  xtrace = linspace(0,8*%pi/4,200)
  ytrace = polyval(p,xtrace)
  plot(t,y,'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q112a.png")
  
  clf
  
  t=linspace(-5,9,10)
  y=ones(1,10)./(ones(1,10)+t.^2)
  
  p=polyfit(t,y)
  xtrace = linspace(-5,9,200)
  ytrace = polyval(p,xtrace)
  plot(t,y,'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q112b.png") 
  
  clf
  
  t=[-9 -5 -3 -2 0 1 2 4 7 9]
  y=ones(1,10)./(ones(1,10)+t.^2)
  
  p=polyfit(t,y)
  xtrace = linspace(-9,9,200)
  ytrace = polyval(p,xtrace)
  plot(t,y,'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q112c.png") 
endfunction


function SM2()
  a=0
  b=3
  ya=2
  yb=1
  da=4
  db=-2
  
  p=polyfitHerm(a,b,ya,yb,da,db)
  xtrace = linspace(a,b,100)
  ytrace = polyval(p,xtrace)
  plot([a b],[ya yb],'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q121a--da4db-2.png") 
  
  clf
  
  a=7
  b=15
  ya=15
  yb=0
  da=-8
  db=8
  
  p=polyfitHerm(a,b,ya,yb,da,db)
  xtrace = linspace(a,b,100)
  ytrace = polyval(p,xtrace)
  plot([a b],[ya yb],'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q121b--da-8db8.png")
endfunction  


function SM3()
  
  deff('[y]=f(t)','y=ones(1,10)./(ones(1,10)+t.^2)')
  t=linspace(-5,9,10)
  y=f(t)
  
  d=splin(t,y)
  xtrace = linspace(min(t),max(t),1000)
  ytrace = interp(xtrace,t,y,d)
  plot(t,y,'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q123--notknot.png") 
  
  clf
  
  d=splin(t,y,"clamped",[-2 3])
  xtrace = linspace(min(t),max(t),1000)
  ytrace = interp(xtrace,t,y,d)
  plot(t,y,'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q123--clamped-da2db3.png") 
  
  clf
  
  d=splin(t,y,"natural")
  xtrace = linspace(min(t),max(t),1000)
  ytrace = interp(xtrace,t,y,d)
  plot(t,y,'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q123--natural.png") 
  
  clf
  
  d=splin(t,y,"monotone")
  xtrace = linspace(min(t),max(t),1000)
  ytrace = interp(xtrace,t,y,d)
  plot(t,y,'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q123--monotone.png") 
  
  clf
  
  d=splin(t,y,"fast")
  xtrace = linspace(min(t),max(t),1000)
  ytrace = interp(xtrace,t,y,d)
  plot(t,y,'ro')
  plot(xtrace,ytrace,'k-')
  xs2png(0,"q123--fast.png") 
  
endfunction
  
  
function ex124()
  t = [0 1 2 3 4 5]
  y = [1 3 4 0 1 1]
  n = length(t)
  T = t(n)-t(1)
  
  d1 = splin(t,y,"periodic")
  d2 = splin(t,y,"fast_periodic")
  
  xtrace = linspace(t(1),t(n),1000)
  ytrace1 = interp(xtrace,t,y,d1)
  ytrace2 = interp(xtrace,t,y,d2)
  scf()
  plot(t,y,'ro')
  plot(xtrace,ytrace1,'k-')
  plot(xtrace,ytrace2,'m-')
  
  plot(xtrace-T,ytrace1,'k-')
  plot(xtrace+T,ytrace1,'k-')
  plot(xtrace-T,ytrace2,'m-')
  plot(xtrace+T,ytrace1,'m-')
  
  xs2png(0,"q125--periodic.png")   
endfunction

function ex125()
  t = 1:100
  y = sin(t/10)
  z = y+rand(t)*0.2-0.1
  
  tau = linspace(t(1),t($),5)
  
  [ys,ds] = lsq_splin(t,z,tau)
  xtrace = linspace(t(1),t($),1000)
  ytrace = interp(xtrace,tau,ys,ds)
  
  scf()
  plot(t,z,'r+')
  plot(xtrace,ytrace,'k-')
  
  xs2png(0,"q125--approx.png")   
endfunction


function SM4()
  t=[0 2 4 5 6 7 9 11 12]
  x=[4 2 0 2 4 4 3 1 0]
  y=[0 0 0 1 2 3 4 4 3]
    
  px=polyfit(t,x)  
  py=polyfit(t,y)  
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = polyval(px,ttrace)
  ytrace = polyval(py,ttrace);
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace,'b-')
  a=gca()
  a.isoview = "on"
  
  xs2png(0,"SM4--simple.png")
    
  clf
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  a=gca()
  a.isoview = "on"
  
  dx = splin(t,x,"not_a_knot")
  dy = splin(t,y,"not_a_knot")
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  plot(xtrace,ytrace,'b-')
  xs2png(1,"SM4--notaknot.png")
  
  
  clf
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  a=gca()
  a.isoview = "on"
  
  dx = splin(t,x,"natural")
  dy = splin(t,y,"natural")
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  plot(xtrace,ytrace,'b-')
  xs2png(2,"SM4--natural.png")
  
  
  clf
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  a=gca()
  a.isoview = "on"
  
  dx = splin(t,x,"monotone")
  dy = splin(t,y,"monotone")
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  plot(xtrace,ytrace,'b-')
  xs2png(3,"SM4--monotone.png")
  
  clf
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  a=gca()
  a.isoview = "on"
  
  dx = splin(t,x,"fast")
  dy = splin(t,y,"fast")
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  plot(xtrace,ytrace,'b-')
  xs2png(4,"SM4--fast.png")
  
  clf
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  a=gca()
  a.isoview = "on"
  
  dx = splin(t,x,"clamped",[1 -1])
  dy = splin(t,y,"clamped",[0  0])
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  plot(xtrace,ytrace,'b-')
  xs2png(5,"SM4--clamped")
endfunction


function SM5()
  t = [0 2 4 6 8]
  x = [1 0 -1 0 1]
  y = [0 1 0 -1 0]
    
  dx = splin(t,x,"natural")
  dy = splin(t,y,"natural")
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  xs2png(0,"SM5--natural-little")
  
  dx = splin(t,x,"periodic")
  dy = splin(t,y,"periodic")
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  xs2png(1,"SM5--periodic-little.png")
  
  a=sqrt(0.5)
  t = 0:1:8
  x = [1 a 0 -a -1 -a 0 a 1]
  y = [0 a 1 a 0 -a -1 -a 0]
    
  dx = splin(t,x,"natural")
  dy = splin(t,y,"natural")
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  xs2png(2,"SM5--natural-more.png")
  
  
  dx = splin(t,x,"periodic")
  dy = splin(t,y,"periodic")
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  xs2png(3,"SM5--periodic-more.png")
endfunction

function SM52()
  a=sqrt(0.5)
  t = 0:1:8
  x = [1 a 0 -a -1 -a 0 a 1]
  y = [0 a 1 a 0 -a -1 -a 0]
    
  dx = splin(t,x,"natural")
  dy = splin(t,y,"natural")
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  
  plot(xtrace,ytrace)
  
  dx = splin(t,x,"periodic")
  dy = splin(t,y,"periodic")
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  plot(xtrace,ytrace,'r')
  xs2png(0,"SM52.png")
endfunction

function SM6()
  x=[4 3.5 0 4 4 4 3 1 0]
  y=[0 0 0 2 2.2 3 4 4 3]
  n=length(x)
  
  /////////////////////////
  t = 0:1:n-1
  dx = splin(t,x,"natural")
  dy = splin(t,y,"natural")
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  
  plot(xtrace,ytrace)
  xs2png(0,"SM61.png")
  
  clf 
  
  /////////////////////////
  t = -1:1:n-2
  dx = splin(t,x,"natural")
  dy = splin(t,y,"natural")
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  
  plot(xtrace,ytrace)
  xs2png(1,"SM62.png")
  
  clf 
  
  /////////////////////////
  t = (-1:1:n-2)/(n-1)
  dx = splin(t,x,"natural")
  dy = splin(t,y,"natural")
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  
  plot(xtrace,ytrace)
  xs2png(2,"SM63.png")
  
  clf 
  
  /////////////////////////
  t = [0:1:n-1].^2
  dx = splin(t,x,"natural")
  dy = splin(t,y,"natural")
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  
  plot(xtrace,ytrace)
  xs2png(3,"SM64.png")
  
  clf 
  
  /////////////////////////
  disp(n)
  t(1)=0
  for i=2:n
    t(i)=norm([x(i);y(i)]-[x(i-1);y(i-1)]) + t(i-1)
  end
  
  dx = splin(t,x,"natural")
  dy = splin(t,y,"natural")
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace)
  a=gca()
  a.isoview = "on"
  
  plot(xtrace,ytrace)
  xs2png(4,"SM65.png")
endfunction



function [B,v,n] = droite_mc(x,y)
  B=[0;0]
  n=length(x)
  
  for i=1:n
    B=B+[x(i);y(i)]
  end
  B=B/n
  
  xb=x-ones(1,n)*B(1)
  yb=y-ones(1,n)*B(2)
  
  A=zeros(2,2)
  A(1,1)=sum(xb.^2)
  A(1,2)=sum(xb.*yb)
  A(2,1)=A(1,2)
  A(2,2)=sum(yb.^2)
  
  [V,D]=spec(A)
  
  if(D(1,1)<=D(2,2)) then
    v=V(:,2)
    n=V(:,1)
  else
    n=V(:,2)
    v=V(:,1)
  end
endfunction

function SM7()
  x = 0:19
  n = length(x)
  y = 2*x+3
  z = (rand(x)-0.5*ones(1,n)) + y
  plot(x,z,'ro')
    
  [B,v,n]=droite_mc(x,z)
  
  a = n(1)
  b = n(2)
  c = -( a*B(1) + b*B(2) )
  
  ycons = (-a*x - c)/b
  
  plot(x,ycons)
  plot(x,y,'g')
  xs2png(0,"SM7--20pts.png")
endfunction


function ex142()
//  x = [-1 ; 0 ; 1]
//  y = [ 0 ; 1 ; 0]
  x = [1 ; 2 ; 3]
  y = [4 ; 0 ; 1]
  
  A = [x ,y,ones(3,1)]
  b = -(x.^2+y.^2)  
  c = A\b
  
  xC = -c(1)/2
  yC = -c(2)/2
  r  = sqrt(xC^2+yC^2-c(3))
  
  scf()
  t = linspace(0,2*%pi,1000)
  plot(xC+r*cos(t),yC+r*sin(t),'k-')
  plot(x,y,'ro')
  a = gca()
  a.isoview="on"
  
  mprintf('\n F(x,y)=(x^2+y^2) %+f x %+f y %+f \n',c(1),c(2),c(3))
  mprintf('Centre = ( %f , %f )\n',xC,yC)
  mprintf('Rayon = %f\n',r)
  
  xs2png(0,"ex142--2.png")
endfunction


function [C,r] = cercle_mc(x,y)
  n=length(x)
  
  A = [sum(x.^2) , sum(x.*y) , sum(x); ...
  sum(x.*y) , sum(y.^2) , sum(y); ...
  sum(x) , sum(y) , n]
  
  b = -[sum(x.*(x.^2+y.^2)); ...
  sum(y.*(x.^2+y.^2)); ...
  sum((x.^2+y.^2)); ...
  ]
  
  c=A\b
  
  xC = -c(1)/2
  yC = -c(2)/2
  r  = sqrt(xC^2+yC^2-c(3))
  C = [xC;yC]  
endfunction

function ex143()
  t=linspace(0,%pi*2,200)
  x=cos(t)+rand(t)*0.4-0.2
  y=sin(t)+rand(t)*0.4-0.2
  [C1,r1] = cercle_mc(x,y)
  scf()
  plot(x,y,'r+')
  tc = linspace(0,2*%pi,200)
  plot(C1(1)+r1*cos(tc), C1(2)+r1*sin(tc),'k-')
  set(gca(),'isoview','on')
  
  xs2png(0,"ex143a.png")
  
  ///////////////////
  
  t=linspace(0,%pi,200)
  x=cos(t)+rand(t)*0.4-0.2
  y=sin(t)+rand(t)*0.4-0.2
  [C1,r1] = cercle_mc(x,y)
  scf()
  plot(x,y,'r+')
  tc = linspace(0,%pi,200)
  plot(C1(1)+r1*cos(tc), C1(2)+r1*sin(tc),'k-')
  set(gca(),'isoview','on')
  
  xs2png(1,"ex143b.png")  
  
  ///////////////////
  
  t=linspace(0,%pi/2,200)
  x=cos(t)+rand(t)*0.4-0.2
  y=sin(t)+rand(t)*0.4-0.2
  [C1,r1] = cercle_mc(x,y)
  scf()
  plot(x,y,'r+')
  tc = linspace(0,%pi/2,200)
  plot(C1(1)+r1*cos(tc), C1(2)+r1*sin(tc),'k-')
  set(gca(),'isoview','on')
  
  xs2png(2,"ex143c.png")  
  
  ///////////////////
  
  
  t=linspace(0,%pi*2,200)
  x = [1 ; 2 ; 3]
  y = [4 ; 0 ; 1]
  [C1,r1] = cercle_mc(x,y)
  scf()
  plot(x,y,'r+')
  tc = linspace(0,2*%pi,200)
  plot(C1(1)+r1*cos(tc), C1(2)+r1*sin(tc),'k-')
  set(gca(),'isoview','on')
  
  xs2png(3,"ex143d.png")
endfunction

function [C,r]=cercle_mc2(x,y)
  n=length(x)
  x2=x.^2
  y2=y.^2
  r2=x2+y2
  xy=x.*y
  
  A=[sum(r2.^2) , sum(r2.*x) , sum(r2.*y) , sum(r2) ;...
  sum(r2.*x) , sum(x2) , sum(xy) , sum(x) ;...
  sum(r2.*y) , sum(xy) , sum(y2) , sum(y) ;...
  sum(r2) , sum(x) , sum(y) , n] 
  
  P=[0 0 0 -2;...
  0 1 0 0;...
  0 0 1 0;...
  -2 0 0 0]
  
  [V,D]=spec(inv(P)*A)
  
  indM=1
  while(real(D(indM,indM)) < 0)
    indM=indM+1
  end
  
  indInit=indM
  for ind=indInit:4
    if(real(D(ind,ind))>0 & real(D(ind,ind))< real(D(indM,indM))) then
      indM=ind
    end
  end
  
  c=V(:,indM)
  C=[-c(2)/(2*c(1)) -c(3)/(2*c(1))]
  r=sqrt(C(1)^2+C(2)^2-c(4)/c(1))
endfunction

function SM8()
  x = [1 ; 2 ; 3]
  y = [4 ; 0 ; 1]
  t=0:0.01:2*%pi
  [C,r]=cercle_mc2(x,y)
  scf()
  plot(x,y,'r+')
  plot(C(1)+r*cos(t),C(2)+r*sin(t),'k-')
  set(gca(),'isoview','on')
  xs2png(0,"sm8--0-2pi-3pts.png")
  
  
  t=0:0.01:2*%pi
  x=cos(t)+rand(t)*0.4-0.2
  y=sin(t)+rand(t)*0.4-0.2
  [C,r]=cercle_mc2(x,y)
  scf()
  plot(x,y,'r+')
  plot(C(1)+r*cos(t),C(2)+r*sin(t),'k-')
  set(gca(),'isoview','on')
  xs2png(1,"sm8--0-2pi.png")
  
  
  t=0:0.01:%pi
  x=cos(t)+rand(t)*0.4-0.2
  y=sin(t)+rand(t)*0.4-0.2
  [C,r]=cercle_mc2(x,y)  
  scf()
  plot(x,y,'r+')
  plot(C(1)+r*cos(t),C(2)+r*sin(t),'k-')  
  set(gca(),'isoview','on')
  xs2png(2,"sm8--0-pi.png")
  
  t=0:0.01:%pi/2
  x=cos(t)+rand(t)*0.4-0.2
  y=sin(t)+rand(t)*0.4-0.2  
  [C,r]=cercle_mc2(x,y)
  scf()
  plot(x,y,'r+')
  plot(C(1)+r*cos(t),C(2)+r*sin(t),'k-')
  set(gca(),'isoview','on')
  xs2png(3,"sm8--0-piSur2.png")
endfunction


function [xyOrig]=approxim_parabole(x,y)
  [B,v,n]=droite_mc(x,y)
  M=[v n]
  XY=(M')*[x-B(1) ; y-B(2)]
  p=polyfit(XY(1,:),XY(2,:),2)
  
  //xyOrig=M*XY+b
  xtrace=linspace(XY(1,1),XY(1,$),1000)
  ytrace=polyval(p,xtrace);
  
  xyOrig=M*[xtrace ; ytrace]+[B(1)*ones(1,1000) ; B(2)*ones(1,1000)]
endfunction 
  
function SM9()
  t=linspace(-%pi/2,%pi/2,20);
  x=cos(t)
  y=sin(t)
  scf()
  plot(x,y,'r+')
  XY=approxim_parabole(x,y)
  plot(XY(1,:),XY(2,:),'b-')
  set(gca(),'isoview','on')
  xs2png(0,"sm9---piS2-piS2.png")
  
  
  t=linspace(0,%pi/2,20);
  x=cos(t)
  y=sin(t)
  scf()
  plot(x,y,'r+')
  XY=approxim_parabole(x,y)
  plot(XY(1,:),XY(2,:),'b-')
  set(gca(),'isoview','on')
  xs2png(1,"sm9--0-piS2.png")
  
  
  t=linspace(0,%pi,20);
  x=cos(t)
  y=sin(t)
  scf()
  plot(x,y,'r+')
  XY=approxim_parabole(x,y)
  plot(XY(1,:),XY(2,:),'b-')
  set(gca(),'isoview','on')
  xs2png(2,"sm9--0-pi.png")
endfunction





