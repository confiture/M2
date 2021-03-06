/////////////////////////////////////////////////////////////
// Evaluation en x d'une B-spline avec l'algo de deBoor-Cox
// Entrée : D = polygone de deBoor
//          t = vecteur des noeuds
//          ordre = ordre de la B-Spline
//          x = parametre où calculer la BSpline
// Sortie : Sx = S(x)
function Sx = DeBoorCox(D,t,ordre,x)

r = ordre;
while t(r+1) < x
    r = r+1;
end
// en sortie r est tel que x \in [t_r, t_{r+1}[

A = D(:,r-ordre+1:r);
for j = 1 : ordre - 1
    for i = r - ordre + 1 + j : r
        ii = i - r + ordre - j;
        denomin = t(i+ordre-j) - t(i);
        cf1 = (t(i+ordre-j) - x) / denomin;
        cf2 = (x - t(i)) / denomin;
        A(:,ii) = cf1 * A(:,ii) + cf2 * A(:,ii+1);
    end
end
Sx = A(:,1);

endfunction


//saturation des noeuds
function t=sature_noeuds(tau,k)
	nbNds=size(tau,2)
	t = tau;
	// saturation des noeuds de bord
	for i = 1:k-1
		t = [tau(1),t,tau(nbNds)];
	end;
endfunction

//////////////////////////////////////////////////////////////////////////////
// Choix à la souris d'une suite de noeuds strictement croissants entre
// XG et XD
// Entrée : XG, XD = bornes pour la suite de noeuds (XG<XD)
//          nbNds = nb de noeuds total, l'utilisateur entre les nbNds-2 
//            noeuds internes tau(k), 1<k<nbNds
//            les deux noeuds extrémaux sont tau(1)=XG et tau(nbNds)=XD
//          YG = ordonnées pour le dessin de la suite de noeuds
// Sortie : tau = le vecteur des noeuds
function tau = inputnoeuds(nbNds,XG,XD,Y)

tau=zeros(1,nbNds);

info = msprintf("Entrer une suite croissante de %d noeuds sur le segment vert en cliquant la souris",nbNds-2);

tau(1) = XG; tau(nbNds) = XD;
disp("XG")
disp(XG)
disp("XD")
disp(XD)

plot(XG,Y,"gs");
plot(XD,Y,"gs");
plot([XG,XD],[Y,Y],"-g");
i = 2;
while i<nbNds
    xinfo(info);
    [but,x,y] = xclick();
    if x > tau(i-1) & x < tau(nbNds)
        tau(i) = x;
        plot( x , Ynoeuds , 'gv' );
        i = i+1;
    end
end
xinfo("");
endfunction

////////////////////////////////////////////////////////////////////
//Retourne la courbe C qui est la nurbs d'ordre k dont les points de contrôle
//sont dans L ayant pour vecteur des noeuds t.
//L : matrice de 3 lignes, sur chaque colonne on a (x,y,poids)
//k : ordre de la nurbs
//t : le vecteur des noeuds de la nurbs saturé aux extrémités.
////////////////////////////////////////////////////////////////////
function [C]=nurbsB(L,k,t)
  Lhomo=coord_homo(L)     //on se ramène a des coordonnées homogènes
  nb_pts_courbe = 1000;
  t_eval = linspace(t(1),t(size(t,2)),nb_pts_courbe);
  nbpts = size(L,2);
  C2 = zeros(3,nb_pts_courbe);
  for i=1:nb_pts_courbe
    t_eval_i = t_eval(i);
    	  
    // calcul du point C(:,i) de la courbe correspondant à S(t_eval_i)
    C2(:,i) = DeBoorCox(Lhomo,t,k,t_eval_i);
  end
  
  C2=proj_plan(C2) //projection sur le plan z=1
  
  C=zeros(2,nb_pts_courbe)
  C(1,:)=C2(1,:)
  C(2,:)=C2(2,:)
  
  C=C2
endfunction


