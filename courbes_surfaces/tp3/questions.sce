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
  plot(D(1,:),D(2,:),'Color',[0 1 0])
  
  
  L(3,2)=L(3,2)+L(3,2)/2.0   
  C=courbe_rat(L,t)
  plot(C(1,:),C(2,:),'Color',[1 0 0])
endfunction

function q13()
  Linit=inputpoly_rat()
  t=0:0.01:1 
   
  //la courbe initiale sans les poids normalisés
  C=courbe_rat(Linit,t)
  plot(C(1,:),C(2,:))
  xs2png(0,"q13-init.png")

   
  Ln1=Linit
  Ln1(3,:)=Ln1(3,:)/Ln1(3,1) //première normalisation
  C=courbe_rat(Ln1,t)
  creer_fig_iso()
  plot(C(1,:),C(2,:))
  xs2png(1,"q13-n1.png")
  
  Ln2=Ln1
  
  //deuxième normalisation
  wbn=Ln2(3,$)
  div=ones(1,size(Ln2,2))*wbn
  div=div.^(ones(1,size(Ln2,2))./(1:size(Ln2,2))) //les racines i-èmes des oméga-barre n
  Ln2(3,:)=Ln2(3,:)./div
  
  C=courbe_rat(Ln2,t)
  creer_fig_iso() //création d'une nouvelle figure
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


function q21(k)
  L = inputpoly_rat(); //saisie du polygone de controle
  nbpts = size(L,2);
	
	
  nbNds = nbpts-k+2;
  Ynoeuds = 10;XG = 10;XD = 90;//Coord d''affichage de la barre des noeuds
  tau = inputnoeuds(nbNds,XG,XD,Ynoeuds); //saisie du vecteur des noeuds
  t = sature_noeuds(tau,k)//saturation des noeuds aux extrémités
		
  L(3,:)=norm_poids(L(3,:)) //normalisation des poids de façon a avoir omega_0=omega_n=1
  // Calcul de la courbe
  // par l'algorithme de De Boor-Cox
  C=nurbsB(L,k,t)	
	
  plot(L(1,:),L(2,:),"o",'Color',[0.5 0.5 0.5])
  plot(C(1,:), C(2,:), 'Color', [1 0 0], 'LineWidth', 2);
  xs2png(0,'q21-k4-21213.png')
endfunction


function q22(k)
  L = inputpoly_rat();
  nbpts = size(L,2);
	
	
  nbNds = nbpts-k+2;
  Ynoeuds = 10;XG = 10;XD = 90;//Coord d''affichage de la barre des noeuds
  tau = inputnoeuds(nbNds,XG,XD,Ynoeuds);
  t = sature_noeuds(tau,k)
		
  // 3) Calcul et affichage de différents points de la courbe
  // par l'algorithme de De Boor-Cox
  C=nurbsB(L,k,t)	
  plot(L(1,:),L(2,:),"o",'Color',[0.5 0.5 0.5])
  plot(C(1,:), C(2,:), 'Color', [1 0 0], 'LineWidth', 2)
  
  i=1
  while(1==1)
    mprintf("Entrez le points à modifier")
    i=mscanf("%d")
    if i==0 then; break;end //si le points est le numéro 0, on s'arrête
    
    mprintf("Entrez le nouveau poids")
    L(3,i)=mscanf("%d")
    C=nurbsB(L,k,t)	
    plot(C(1,:), C(2,:), 'Color', [rand() rand() rand()], 'LineWidth', 2)
  end
  
  xs2png(0,'q22.png')
endfunction


function q23()
  k=4
  L = inputpoly_rat();
  nbpts = size(L,2);	
	
  nbNds = nbpts-k+2;
  Ynoeuds = 10;XG = 10;XD = 90;//Coord d''affichage de la barre des noeuds
  tau = inputnoeuds(nbNds,XG,XD,Ynoeuds);
  t = sature_noeuds(tau,k)
    
  C=nurbsB(L,k,t)	
  plot(L(1,:),L(2,:),"o",'Color',[0.5 0.5 0.5])
  plot(C(1,:), C(2,:), 'Color', [1 0 0], 'LineWidth', 2)
  xs2png(0,'q23-init.png')
  
  a=2
  b=3
  lambda=4
  t=t*a+b
  L(3,:)=L(3,:)*lambda
  creer_fig_b()
  
  C=nurbsB(L,k,t)
  plot(L(1,:),L(2,:),"o",'Color',[0.5 0.5 0.5])
  plot(C(1,:), C(2,:), 'Color', [1 0 0], 'LineWidth', 2)
  xs2png(1,'q23-a2b3lambda4.png')
endfunction
  