/////////////////////////////////////////////////////////////////////////////
// définition de 4 courbes frontières 
// puis calcul et affichage d'une facette de Coons obtenue à l'aide
// du procédé d'interpolation linéaire
function main(num_test)
  
  if argn(2)==0
    num_test=1;
  end;
  
  // le paramétrage
  n=101;
  nIso=11;
  t = linspace(0,1,n);
  u = t; 
  v = t;

  select num_test
    
    case 1
  // définition des courbes frontières 
  C1 = [10.0*u;       zeros(u);    3*sin(u*%pi)  ];
  C2 = [zeros(v);     10.0*v;       -sin(v*%pi)  ];
  C3 = [10.0*u;       10.0*ones(u);  sin(2*u*%pi)];
  C4 = [10.0*ones(v); 10.0*v;        sin(3*v*%pi)];
  
    case 2
  // définition des courbes frontières 
  C1 = [10.0*u;      -10.0*ones(u);  cos(u*%pi/2)         ];
  C2 = [zeros(v);     10.0*(v-1);    (v-1).*((-%pi+1)*v-1)];
  C3 = [10.0*u;       zeros(u);    3*sin(u*%pi)           ];
  C4 = [10.0*ones(v); 10.0*(v-1);    -3*sin(v*%pi)        ];
  
    case 3
  // définition des courbes frontières 
  C1 = [u;zeros(u);0.6*u.*(1-u)];
  C3 = [u.^3;ones(u)+0.1*u.*(1-u);u.*(u-1)];
  C2 = [zeros(v);v;v.*v.*(1-v)];
  C4 = [ones(v)-v.*v.*(1-v);v.*v;zeros(v)];

    else
      error('num_test invalide');
  end
  
  // les 4 coins du patch
  P00 = C1(:,1); // = C2(:,1)
  P01 = C3(:,1); // = C2(:,$)
  P10 = C1(:,$); // = C4(:,1)
  P11 = C3(:,$); // = C4(:,$)

  // les fonctions de base de l'interpolation linéaire
  L0u = 1-u;
  L1u = u;
  L0v = 1-v;
  L1v = v;
  
  //// procédé de Coons
  
  // interpolation de C1 et C3
  X13 = [C1(1,:)' C3(1,:)']* ...
                            [L0v;...
                             L1v];
  Y13 = [C1(2,:)' C3(2,:)']* ...
                            [L0v;...
                             L1v];
  Z13 = [C1(3,:)' C3(3,:)']* ...
                            [L0v;...
                             L1v];
  
  // interpolation de C2 et C4
  X24 = [L0u' L1u']* ...
                    [C2(1,:); ...
                     C4(1,:)];
  Y24 = [L0u' L1u']* ...
                    [C2(2,:); ...
                     C4(2,:)];
  Z24 = [L0u' L1u']* ...
                    [C2(3,:); ...
                     C4(3,:)];
  
  // interpolation produit tensoriel des 4 coins
  XPT = [L0u' L1u']* ...
                    [P00(1) P01(1); ...
                     P10(1) P11(1)]* ...
                                    [L0v;...
                                     L1v];
  YPT = [L0u' L1u']* ...
                    [P00(2) P01(2); ...
                     P10(2) P11(2)]* ...
                                    [L0v;...
                                     L1v];
  ZPT = [L0u' L1u']* ...
                    [P00(3) P01(3); ...
                     P10(3) P11(3)]* ...
                                    [L0v;...
                                     L1v];
  // la somme "booléenne" pour obtenir la surface finale
  XS = X13 + X24 - XPT;
  YS = Y13 + Y24 - YPT;
  ZS = Z13 + Z24 - ZPT;

  // sorties graphiques au format Geomview
  
  // les 4 courbes de bord
  fid = mopen("C1.vect", "w");
  write_VECT(fid,C1(1,:),C1(2,:),C1(3,:),[0,0,0],3);
  mclose(fid);
  fid = mopen("C2.vect", "w");
  write_VECT(fid,C2(1,:),C2(2,:),C2(3,:),[0,1,0],3);
  mclose(fid);
  fid = mopen("C3.vect", "w");
  write_VECT(fid,C3(1,:),C3(2,:),C3(3,:),[1,0,0],3);
  mclose(fid);
  fid = mopen("C4.vect", "w");
  write_VECT(fid,C4(1,:),C4(2,:),C4(3,:),[0,0,1],3);
  mclose(fid);
  
  // la surface interpolant C1 et C3
  fid = mopen("surface_coons_13.mesh", "w"); 
  write_MESH(fid,X13,Y13,Z13,[0.6 1.0 0.8]);
  mclose(fid);
  trace_isoparametrique("surface_coons_13_iso.vect", X13,Y13,Z13, nIso, 2, [0 0 0] );
  
  // la surface interpolant C2 et C4
  fid = mopen("surface_coons_24.mesh", "w"); 
  write_MESH(fid,X24,Y24,Z24,[0.8 0.6 1.0]);
  mclose(fid);
  trace_isoparametrique("surface_coons_24_iso.vect", X24,Y24,Z24, nIso, 1, [0 0 0] );
  
  // la surface interpolant les 4 coins
  fid = mopen("surface_coons_PT.mesh", "w"); 
  write_MESH(fid,XPT,YPT,ZPT,[0.6 0.6 0.6]);
  mclose(fid);
  trace_isoparametrique("surface_coons_PT_iso.vect", XPT,YPT,ZPT, nIso, 3, [0 0 0] );
  
  // la surface finale
  fid = mopen("surface_coons.mesh", "w"); 
  write_MESH(fid,XS,YS,ZS,[1.0 0.8 0.6]);
  mclose(fid);
  trace_isoparametrique("surface_coons_iso.vect", XS,YS,ZS, nIso, 3, [0 0 0] );


endfunction

///////////////////////////////////////////////////////////////////////
// Routines pour les sorties graphiques au format Geoview
///////////////////////////////////////////////////////////////////////

// tracé de courbes isoparametrique à partir de données grille (x,y,z)
// Entree : nom_f = nom du fichier Geomview
//          x,y,z = donnees grille (matrices MxN)
//          nIso = nb d'isoparametriques à tracer suivant une direction
//          direction = direction pour le tracé des isoparamétriques
//            1 : tracé suivant la direction 1
//                une isoparametrique : points (x(:,j),y(:,j),z(:,j))
//            2 : tracé suivant la direction 2
//                une isoparametrique : points (x(i,:),y(i,:),z(i,:))
//            3 : tracé suivant la direction 1 et la direction 2
//          couleur = triplet (r,g,b)
function trace_isoparametrique(nom_f, x,y,z, nIso, direction, couleur)

  [d1,d2] = size(x);
  f = mopen(nom_f, "w");
  mfprintf(f,"{\nLIST\n");
  if direction==1 | direction == 3
    // tracé suivant la direction 1
    dIso = floor((d2-1)/(nIso-1));
    for j=1:dIso:d2
      write_VECT(fid,x(:,j),y(:,j),z(:,j),couleur,1);
    end    
  end
  if direction==2 | direction == 3
    // tracé suivant la direction 2
    dIso = floor((d1-1)/(nIso-1));
    for i=1:dIso:d1
      write_VECT(fid,x(i,:),y(i,:),z(i,:),couleur,1);
    end    
  end
  
  mfprintf(f,"}\n");
  mclose(f);
endfunction

  
//----------------------------------------------
// ecriture d'un objet de type VECT dans le fichier 
// dont l'identificateur est fid
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          x,y,z = les points de la polyligne
//          c = couleur RGB
//          l = épaisseur de la ligne
function write_VECT(fid,x,y,z,c,l);

// début de l'objet composite
mfprintf(fid,"{\n");

// attribut pour l'épaisseur
mfprintf(fid,"  appearance {linewidth %d}\n",l);

// objet VECT : 1 polyligne avec n sommets et 1 couleur
n=length(x);
mfprintf(fid,"{\n");

// l'entete
mfprintf(fid,"VECT\n 1 %d 1\n %d\n 1\n", n, n);
// les n sommets
for i=1:n
	mfprintf(fid,"%15.7e %15.7e %15.7e \n", x(i), y(i), z(i));
end
// la couleur
mfprintf(fid,"%15.7e %15.7e %15.7e 1\n", c(1), c(2), c(3));
mfprintf(fid,"}\n");

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet de type VECT dans le fichier 
// dont l'identificateur est fid
// objet formé de n vecteurs [P(:,i),P(:,i)+h*V(:,i)]
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          P = les points de la polyligne
//          V = vecteurs associés
//          c = couleur RGB
//          h = homothetie pour le vecteur
//          l = épaisseur de la ligne
function write_VECTS(fid,P,V,c,h,l);

// début de l'objet composite
mfprintf(fid,"{\n");

// attribut pour l'épaisseur
mfprintf(fid,"  appearance {linewidth %d}\n",l);

// objet VECT : 1 poyligne avec n sommets et 1 couleur
n=size(P,2);
mfprintf(fid,"{\n");

// l'entete
mfprintf(fid,"VECT\n %d %d %d\n", n, 2*n, n);

// nb de points par polyline
for i=1:n
  mfprintf(fid," 2");
end
mfprintf(fid,"\n");

// nb de couleurs par polyline
for i=1:n
  mfprintf(fid," 1");
end
mfprintf(fid,"\n");

// les n vecteurs
for i=1:n
	mfprintf(fid,"%15.7e %15.7e %15.7e \n", ...
	  P(1,i), P(2,i), P(3,i));
	mfprintf(fid,"%15.7e %15.7e %15.7e \n", ...
	  P(1,i)+h*V(1,i), P(2,i)+h*V(2,i), P(3,i)+h*V(3,i));
end

// les n couleurs
for i=1:n
  mfprintf(fid,"%8.5f %8.5f %8.5f 1\n", c(1), c(2), c(3));
end
mfprintf(fid,"}\n");

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet de type VECT 
// représentant un muage de points 
// dans le fichier dont l'identificateur est fid
// Entree : fid = id du fichier (cree prealablement avec mopen)
//          x,y,z = les points 
//          c = couleur RGB
//          l = épaisseur de la ligne
function write_points_VECT(fid,x,y,z,c,l);

// début de l'objet composite
mfprintf(fid,"{\n");

// attribut pour l'épaisseur
mfprintf(fid,"  appearance {linewidth %d}\n",l);

// objet VECT : n poyligne avec chacune 1 sommet et 1 couleur
n=length(x);
mfprintf(fid,"{\n");

// l'entete
mfprintf(fid,"VECT\n %d %d %d\n", n, n, n);

// nombre de points par ligne
for i=1:n
	mfprintf(fid," 1");
end
mfprintf(fid,"\n");

// nombre de couleurs par ligne
for i=1:n
	mfprintf(fid," 1");
end
mfprintf(fid,"\n");

// les n sommets
for i=1:n
	mfprintf(fid,"%15.7e %15.7e %15.7e \n", x(i), y(i), z(i));
end

// les couleur
for i=1:n
  mfprintf(fid,"%15.7e %15.7e %15.7e 1\n", c(1), c(2), c(3));
end
mfprintf(fid,"}\n");

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet MESH
function write_MESH(fid,X,Y,Z,c)

n1 = size(X,1);
n2 = size(X,2);

// début de l'objet composite
mfprintf(fid,"{\n");

mfprintf(fid, 'CMESH\n%d %d\n', n2, n1);
for i=1:n1
for j=1:n2
    mfprintf(fid, '%15.7e %15.7e %15.7e  %8.5f %8.5f %8.5f 1\n', ...
	  X(i,j) , Y(i,j) , Z(i,j), c(1), c(2), c(3));
end
end

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------

//----------------------------------------------
// ecriture d'un objet SPHERE de centre (x,y,z) et rayon r
function write_SPHERE(fid,x,y,z,r)

// début de l'objet composite
mfprintf(fid,"{\n");

mfprintf(fid, 'SPHERE\n%15.7e %15.7e %15.7e %15.7e\n', r, x, y, z);

// fin de l'objet composite
mfprintf(fid,"}\n");

endfunction
//----------------------------------------------


