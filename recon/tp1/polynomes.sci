// interpolation polynomiale

// calcule le polynome p(t) de degr� n approchant au sens des moindres carr�s
// les donn�es (t(i),y(i))
// Entr�e : t = vecteur de m valeurs distinctes entre elles
//          y = vecteur de m valeurs
//          n = degr� du polynome (par d�faut n=m-1 => interpolation polynomiale)
// Sortie : p = vecteur de n+1 coeff. du polynome
//            p(t) = sum_{i=1}^{n+1} p(i) t^{n+1-i}
//
//                         n         n-1
//                 = p(1).t  + p(2).t    + ... + p(n).t + p(n+1)
function p = polyfit(t,y,n)
  
  nargin = argn(2);
  if nargin<2 then
    error('il faut deux arguments en entr�e');
  end
  if length(t) ~= length(y) then
    error('les deux arguments doivent etre de meme taille');
  end
  m = length(t);
  if nargin<3 then
    n = m-1;
  end
  
  // test des abscisses t(i)
  tc = -gsort(-t); // tri par ordre croissant
  dtc = diff(tc);
  if min(dtc)==0
    error('les abscisses t doivent etre distinctes entre elles');
  end
    
  // normalisation des t(i) pour que t(i) dans [-1;1]
  a = max(abs(min(t)),abs(max(t)));
  t = t/a;
  
  // matrice de Vandermonde associ�e aux t(i)
  M = zeros(m,n+1);
  for i=1:m
    for j=1:n+1
      M(i,j) = t(i)^(n+1-j);
    end
  end
  
  // r�solution
  p = (M\matrix(y,m,1))';
  
  // normalisation inverse
  p = p./(a.^(n:-1:0));
  
endfunction

// �value un polynome p(t) en des valeurs t(i)
// Entr�e : p = tableau des n+1 coeff. d'un polynome de degr� n
//          t = tableau de m valeurs
// Sortie : r = tableau de m valeurs r(i) = p(t(i))
function r = polyval(p,t)

  nargin = argn(2);
  if nargin<2 then
    error('il faut deux arguments en entr�e');
  end

  n = length(p)-1; // degr� de p
  
  // sch�ma de Horner
  r = zeros(t);
  for i=1:length(t)
    s = p(1);
    ti = t(i);
    for j=1:n
      s = s*ti+p(j+1);
    end
    r(i) = s;
  end
  
endfunction


function tests_polynomes

  ///////////////////////////////////////////////////////////////////////////
  // interpolation de 5 points avec une ordonn�e manquante
  ///////////////////////////////////////////////////////////////////////////
  t = [0 1 1 3 4];
  y = [1 2 3 1];
  p = polyfit(t,y);
  
  ///////////////////////////////////////////////////////////////////////////
  // interpolation de 5 points avec deux abscisses �gales
  ///////////////////////////////////////////////////////////////////////////
  t = [0 1 1 3 4];
  y = [1 2 3 1 2];
  p = polyfit(t,y);
  
  ///////////////////////////////////////////////////////////////////////////
  // interpolation de 5 points
  ///////////////////////////////////////////////////////////////////////////
  t = [0 1 2 3 4];
  y = [1 2 3 1 2];
  tt = linspace(t(1),t($),1000);
  p = polyfit(t,y);
  yy = polyval(p,tt);
  
  scf();
  plot(t,y,'ko');
  plot(tt,yy,'r-');
  a = gca();
  a.data_bounds = [-1,0;5,3.5];
  a.margins = 0.05*[1 1 1 1];
  
  ///////////////////////////////////////////////////////////////////////////
  // interpolation de 10 points sur une sinusoide
  ///////////////////////////////////////////////////////////////////////////
  t = 0:9;
  y = sin(t*%pi/4);
  tt = linspace(t(1),t($),1000);
  p = polyfit(t,y);
  yy = polyval(p,tt);
  
  // trac� de la fonction initiale
  yinit = sin(tt*%pi/4);
  scf();
  plot(t,y,'ko');
  plot(tt,yinit,'b-');
  a = gca();
  a.data_bounds = [-1,-1.5;10,1.5];
  a.margins = 0.05*[1 1 1 1];
  
  // trac� du polynome interpolant
  scf();
  plot(t,y,'ko');
  plot(tt,yy,'r-');
  a = gca();
  a.data_bounds = [-1,-1.5;10,1.5];
  a.margins = 0.05*[1 1 1 1];
  
  ///////////////////////////////////////////////////////////////////////////
  // interpolation de 10 points sur 1/(1+t*t)
  ///////////////////////////////////////////////////////////////////////////
  
  // la fonction
  deff('[y]=f(t)','y=ones(t)./(1+t.*t)');
  
  // les donn�es
  //t = [-9 -5 -3 -2 0 1 2 4 7 9];
  t = linspace(-5,9,10);
  y = f(t);
  
  // calcul du polynome interpolant
  tt = linspace(t(1),t($),1000);
  p = polyfit(t,y);
  yy = polyval(p,tt);
  
  // trac� de la fonction initiale
  ttinit = tt;
  yinit = f(ttinit);
  scf();
  plot(t,y,'ko');
  plot(ttinit,yinit,'b-');
  a = gca();
  a.data_bounds = [-10,-1;10,2];
  a.margins = 0.05*[1 1 1 1];
  
  // trac� du polynome interpolant
  scf();
  plot(t,y,'ko');
  plot(tt,yy,'r-');
  a = gca();
  a.data_bounds = [-10,-1;10,2];
  a.margins = 0.05*[1 1 1 1];
  

endfunction
  
function tests_regression

  ///////////////////////////////////////////////////////////////////////////
  // r�gression polynomiale
  ///////////////////////////////////////////////////////////////////////////
  
  t = rand(1,100)*7-3.5;
  tt = linspace(-3.5,3.5,1000);
  y = cos(t/3+0.2)+rand(1,100)*0.2-0.1;
  scf();
  plot(t,y,'ko');
  a = gca();
  a.data_bounds = [-4,-1;4,2];
  a.margins = 0.05*[1 1 1 1];
  a.children.children.mark_size=3;
  
  // r�gression lin�aire
  p = polyfit(t,y,1);
  yy = polyval(p,tt);
  scf();
  plot(t,y,'ko');
  a = gca();
  a.children.children.mark_size=3;
  plot(tt,yy,'r-');
  a.data_bounds = [-4,-1;4,2];
  a.margins = 0.05*[1 1 1 1];
  
  // r�gression quadrique
  p = polyfit(t,y,2);
  yy = polyval(p,tt);
  scf();
  plot(t,y,'ko');
  a = gca();
  a.children.children.mark_size=3;
  plot(tt,yy,'r-');
  a.data_bounds = [-4,-1;4,2];
  a.margins = 0.05*[1 1 1 1];
  
  
endfunction

function [pol]=polyfitHerm(a,b,ya,yb,da,db)
  h=b-a
  
  H0 = (2/h^3)*[1 -3*a 3*a^2 -a^3] -(3/h^2)*[0 1 -2*a a^2] + [0 0 0 1]
  H1 = (1/h^3)*[1 -3*a 3*a^2 -a^3] -(2/h^2)*[0 1 -2*a a^2] + (1/h)*[0 0 1 -a]
  H2 = (1/h^3)*[1 -3*a 3*a^2 -a^3] -(1/h^2)*[0 1 -2*a a^2]
  H3 = (-2/h^3)*[1 -3*a 3*a^2 -a^3] + (3/h^2)*[0 1 -2*a a^2]
  
  pol = ya*H0 + h*da*H1 + h*db*H2 + +yb*H3
endfunction

