//////////////////////////////////////////////////////////////////////////////
// Courbe de B�zier 2D
// Calcul de points de la courbe par l'algorithme de De Caste 
// Syntaxe de l'appel du programme :
//   prog11
//////////////////////////////////////////////////////////////////////////////
function prog11()

// 1) Acquisition du polygone B�zier � la souris
P = inputpoly();
n = size(P,2)-1;  //degr� de la courbe P
xinfo("Fin de la saisie : cliquer pour continuer");
xclick

// 2) Calcul (et affichage) des points de la courbe B�zier P(t) 
//    par l'algorithme de De Casteljau
t = 0:0.01:1;
nbpts=length(t);
C = zeros(2,nbpts); // tableau pour stocker les differents pts de la courbe
for k=1:nbpts
	tk = t(k);
	
	// calcul du point Ptk=P(tk) � partir de tk et des points P
	// en utilisant l'algorithme de De Casteljau
	sub = P; //la subdivision
	for i=1:n,
	 for j=1:n+1-i,
	   sub(:,j)=(1-tk)*sub(:,j)+tk*sub(:,j+1)
     end
    end
    
    Ptk=sub(:,1)
		
	// sauvegarde du point Ptk dans le tableau C
	C(:,k) = Ptk;
end
plot(C(1,:),C(2,:))
endfunction

//1.2
//Prend le polygone de controle P en entr�e et le nombre de subdivisions 
//a ex�cuter.
//Trace la n-i�me subdivision de du polygone.
function subdivision(P,n)
  if n<>0 then  
    nbp=size(P,2)
    diag1=zeros(2,nbp)
    diag2=zeros(2,nbp)
    sub=P
    
    diag1(:,1)=sub(:,1)
    diag2(:,1)=sub(:,nbp)
    
    for i=2:nbp,
      sub(:,1)=0.5*sub(:,1)+0.5*sub(:,2)
      diag1(:,i)=sub(:,1)
      for j=2:nbp-i+1,
        sub(:,j)=0.5*sub(:,j)+0.5*sub(:,j+1)
      end
      diag2(:,i)=sub(:,nbp-i+1)
    end
  
    subdivision(diag1,n-1)
    diag2=diag2(:,nbp:-1:1)
    subdivision(diag2,n-1)
  else
    plot(P(1,:),P(2,:))
  end
endfunction

//question 1.2
//Saisit les points s�lectionn�s par l'utilisateur � l'aide de la souris et 
//trace les subdivisions du rang 1 au rang 5 de ce polygone.
function trace_subs()
   P = inputpoly();
   xclick()
   clf
   subdivision(P,1)
   xclick()
   clf
   subdivision(P,2)
   xclick()
   clf
   subdivision(P,3)
   xclick()
   clf
   subdivision(P,4)
   xclick()
   clf
   subdivision(P,5)
endfunction

//question 3.1
function elev_deg()
	P=inputpoly()
	subdivision(P,5)
	
	n=size(P,2)
	P2=zeros(2,n+1)
	P2(:,1)=P(:,1)
	P2(:,n+1)=P(:,n)
	vec1=[(1:n-1)./n;(1:n-1)./n]
	vec2=[(n-(1:n-1))./n;(n-(1:n-1))./n]
	
	P2(:,2:n)=vec1.*P(:,1:n-1)+vec2.*P(:,2:n)
	
	plot(P2(1,:),P2(2,:))
	subdivision(P2,5)
endfunction


//question 4.1
//Cette fonction prend en entr�e les points � transformer P, r le rayon de la rotation
//et (Cx,Cy) le vecteur de translation. La fonction retourne les points transform�s.
function [X]=transformation(P,r,theta,Cx,Cy)
	R(1,1)=cos(theta)
	R(1,2)=-sin(theta)
	R(2,1)=sin(theta)
	R(2,2)=cos(theta)
	X=R*P
	X=r*X
	
	n=size(P,2)
	C=zeros(2,n)
	C(1,:)=Cx
	C(2,:)=Cy
	X=X+C
endfunction


function q41(r,theta,Cx,Cy)
	//On transforme le polygone de controle et on trace la courbe de Bézier.
	P=inputpoly()
	xclick()
	clf
	subdivision(P,5)	
	xclick()
	clf
	
	//On transforme le polygone de contrôle
	Ptrans=transformation(P,r,theta,Cx,Cy)
	plot(Ptrans(1,:),Ptrans(2,:))
	xclick()
	clf
	subdivision(Ptrans,5)
endfunction

//Question 4.2
//Cette fonction prend en entr�e le centre de l'arc de cercle (Cx,Cy),
//son rayon r, l'angle a1 et l'angle a2 avec a2-a1<=pi/2.
//La fonction trace l'arc de cercle approch� par une courbe de B�zier.
function q42(Cx,Cy,r,a1,a2)
  alpha=(a2-a1)/2
  P0=[cos(alpha);-sin(alpha)]
  P3=[cos(alpha);sin(alpha)]
  T0=[sin(alpha);cos(alpha)]
  T3=[-sin(alpha);cos(alpha)]
  lambda=4*(1-cos(alpha))/(3*sin(alpha))
  
  P1=P0+lambda*T0
  P2=P3-lambda*T3
  
  Pin=[P0 P1 P2 P3]
  //plot(Pin(1,:),Pin(2,:))
  //subdivision(Pin,5)
  Pout=transformation(Pin,r,a1+alpha,Cx,Cy)
  disp("Pout")
  disp(Pout)
  //plot2d(Pout(1,:),Pout(2,:),frameflag=4)
  //plot(Pout(1,:),Pout(2,:))
  subdivision(Pout,5)
endfunction


//Question 4.3 : B�ziers cubiques
function bezier3(P)
  n=size(P,2)
  for i=0:(n-1)/3-1,
    subdivision([P(:,i*3+1) P(:,i*3+2) P(:,i*3+3) P(:,i*3+4)],3)
  end
endfunction

////////////////////////////////////////////////////////////
//Trace de la lettre S.
////////////////////////////////////////////////////////////
function q43()
  f=figure()
  a=gca()
  a.data_bounds=[-5 -5; 25 25]
  P1=[20 15 10 5 ;
     20 20 20 20]
  bezier3(P1); 
  q42(5,15,5,%pi/2,%pi)
  P2=[0 0 5 10 15 20 20;15 12.5 11 10 9 7.5 5]
  bezier3(P2);
  q42(15,5,5,-%pi/2,0)
  P3=[15 10 5 0;0 0 0 0]
  bezier3(P3)
endfunction

function lettreS_off()
  f=figure()
  a=gca()
  a.data_bounds=[-5 -5; 25 25]
  P1=[20 15 10 5 ;
     20 20 20 20]
  bezier3(P1); 
  P2=[5 2.2385763 0 0 0 0 5 10 15 20 20;20 20 17.761424 15 15 12.5 11 10 9 7.5 5]
  bezier3(P2);
  P3=[15 17.761424 20 20 15 10 5 0;0 0 2.2385763 5 0 0 0 0]
  bezier3(P3)
endfunction

//////////////////////////////////////////////////////////////////////////////
// routine d'approximation de l'offset d'une B�zier cubique par 
// une B�zier cubique
// Entr�e : P = tableau de 4 points (matrice 2 x 4)
//          d = r�el
// Sortie : Q = tableau de 4 points (matrice 2 x 4)
function Q = approx_B3_offset_B3(P,d)
  Q=zeros(2,4)
  P0P1=(P(:,2)-P(:,1))
  P2P3=(P(:,4)-P(:,3))
    
  pP0=[-P0P1(2);P0P1(1)]/norm(P0P1)
  pP3=[-P2P3(2);P2P3(1)]/norm(P2P3)
  Q(:,1)=P(:,1)+d*pP0
  Q(:,4)=P(:,4)+d*pP3
  
  coef=norm(Q(:,1)-Q(:,4))/norm(P(:,1)-P(:,4))
  
  Q(:,2)=Q(:,1)+coef*P0P1
  Q(:,3)=Q(:,4)-coef*P2P3
endfunction

function trace_offset_v2(d)
  //stacksize("max")
  P=inputpoly()
  n=size(P,2)
  
  subdivision([P(:,1) P(:,2) P(:,3) P(:,4)],3)
  Q1=approx_B3_offset_B3([P(:,1) P(:,2) P(:,3) P(:,4)],d)
  q1=Q1(:,4)
  Q2=approx_B3_offset_B3([P(:,1) P(:,2) P(:,3) P(:,4)],-d)
  q2=Q2(:,4)
  subdivision(Q1,3)
  subdivision(Q2,3)
  for i=1:(n-1)/3-1,
    subdivision([P(:,i*3+1) P(:,i*3+2) P(:,i*3+3) P(:,i*3+4)],3)
    Q1=approx_B3_offset_B3([P(:,i*3+1) P(:,i*3+2) P(:,i*3+3) P(:,i*3+4)],d)
	Q1(:,1)=q1
    Q2=approx_B3_offset_B3([P(:,i*3+1) P(:,i*3+2) P(:,i*3+3) P(:,i*3+4)],-d)
    Q2(:,1)=q2
    subdivision(Q1,3)
    subdivision(Q2,3)
    q1=Q1(:,4)
    q2=Q2(:,4)
  end
endfunction

function trace_offset_v2(d)
  //stacksize("max")
  P=inputpoly()
  n=size(P,2)
  
  for i=0:(n-1)/3-1,
    subdivision([P(:,i*3+1) P(:,i*3+2) P(:,i*3+3) P(:,i*3+4)],3)
    Q1=approx_B3_offset_B3([P(:,i*3+1) P(:,i*3+2) P(:,i*3+3) P(:,i*3+4)],d)
    Q2=approx_B3_offset_B3([P(:,i*3+1) P(:,i*3+2) P(:,i*3+3) P(:,i*3+4)],-d)
    subdivision(Q1,3)
    subdivision(Q2,3)
  end
endfunction

//////////////////////////////////////////////////////////////////////////////
// entr�e d'un 
// en sortie, le tableau X avec p points du plan (dimensions 2 x p)
function [X] = inputpoly()

// cr�ation d'une fenetre pour la saisie des points
f=figure();  // une nouvelle fenetre
f.background=8;
set(gca(),"auto_clear","off")
set(gca(),"data_bounds",[0,0;100,100])      // bornes des axes en x et y
set(gca(),"margins",[0.05,0.05,0.05,0.05])  // marges du repere dans la fenetre
set(gca(),"axes_visible",["on","on","on"])  // afficher les axes
set(gca(),"box","on")                       
set(gca(),"auto_scale","off")                       

// boucle de saisie des points
but = 3;
i = 0;
while but==3 | but==0 | but==10 | but==20
  xinfo("Point suivant : bouton gauche - Dernier point : bouton droite");
  i = i+1;
  [but,v0,v1] = xclick();
  X(1,i) = v0;
  X(2,i) = v1;
  plot(X(1,i),X(2,i),"go")
  if i>=2 then
    plot(X(1,i-1:i),X(2,i-1:i),"g-")
  end;
end;

endfunction
