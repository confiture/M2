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
  t=[0 1 2 3]
  x=[0 1 1 0]
  y=[0 0 1 1]
    
  px=polyfit(t,x)  
  py=polyfit(t,y)  
  
  ttrace = linspace(t(1),t($),1000)
  xtrace = polyval(px,ttrace)
  ytrace = polyval(py,ttrace);
  
  scf()
  plot(x,y,'ro')
  plot(x,y,'g--')
  plot(xtrace,ytrace,'k-')
  a=gca()
  a.isoview = "on"
  
  dx = splin(t,x,"clamped",[1 -1])
  dy = splin(t,y,"clamped",[0 0])
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  plot(xtrace,ytrace,'b-')
  xs2png(0,"SM4little.png")
  
  clf
    
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
  plot(xtrace,ytrace,'k-')
  a=gca()
  a.isoview = "on"
  
  dx = splin(t,x,"clamped",[1 -1])
  dy = splin(t,y,"clamped",[0 0])
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  
  plot(xtrace,ytrace,'b-')
  xs2png(1,"SM4max.png")
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
  
  plot(xtrace,ytrace)
  
  dx = splin(t,x,"periodic")
  dy = splin(t,y,"periodic")
  
  xtrace = interp(ttrace,t,x,dx)
  ytrace = interp(ttrace,t,y,dy)
  plot(xtrace,ytrace,'r')
  xs2png(0,"SM5.png")
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
  xs2png(0,"SM62.png")
  
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
  xs2png(0,"SM63.png")
  
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
  xs2png(0,"SM62.png")
  
  


endfunction

  