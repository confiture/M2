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
  
  // les dérivées au bord des courbes
  DC1_0 = [ 10.0;  0.0; 3*%pi]; // dérivée de C1 en u=0
  DC1_1 = [ 10.0;  0.0;-3*%pi]; // dérivée de C1 en u=1
  DC2_0 = [  0.0; 10.0;  -%pi]; // dérivée de C2 en v=0
  DC2_1 = [  0.0; 10.0;   %pi]; // dérivée de C2 en v=1
  DC3_0 = [ 10.0;  0.0; 2*%pi]; // dérivée de C3 en u=0
  DC3_1 = [ 10.0;  0.0; 2*%pi]; // dérivée de C3 en u=1
  DC4_0 = [  0.0; 10.0; 3*%pi]; // dérivée de C4 en v=0
  DC4_1 = [  0.0; 10.0;-3*%pi]; // dérivée de C4 en v=1

    case 2
  // définition des courbes frontières 
  C1 = [10.0*u;      -10.0*ones(u);  cos(u*%pi/2)         ];
  C2 = [zeros(v);     10.0*(v-1);    (v-1).*((-%pi+1)*v-1)];
  C3 = [10.0*u;       zeros(u);    3*sin(u*%pi)           ];
  C4 = [10.0*ones(v); 10.0*(v-1);    -3*sin(v*%pi)        ];
  
  // les dérivées au bord des courbes
  DC1_0 = [ 10.0;  0.0;   0    ]; // dérivée de C1 en u=0
  DC1_1 = [ 10.0;  0.0;  -%pi/2]; // dérivée de C1 en u=1
  DC2_0 = [  0.0; 10.0;   %pi-2]; // dérivée de C2 en v=0
  DC2_1 = [  0.0; 10.0;  -%pi  ]; // dérivée de C2 en v=1
  DC3_0 = [ 10.0;  0.0; 3*%pi  ]; // dérivée de C3 en u=0
  DC3_1 = [ 10.0;  0.0;-3*%pi  ]; // dérivée de C3 en u=1
  DC4_0 = [  0.0; 10.0;-3*%pi  ]; // dérivée de C4 en v=0
  DC4_1 = [  0.0; 10.0; 3*%pi  ]; // dérivée de C4 en v=1

    else
      error('num_test invalide');
  end
  
  // les 4 coins du patch
  P00 = C1(:,1); // = C2(:,1)
  P01 = C3(:,1); // = C2(:,$)
  P10 = C1(:,$); // = C4(:,1)
  P11 = C3(:,$); // = C4(:,$)

  // définition des twists
  W00 = [0;0;0]; // en u=0, v=0
  W01 = [0;0;0]; // en u=0, v=1
  W10 = [0;0;0]; // en u=1, v=0
  W11 = [0;0;0]; // en u=1, v=1

  // calcul des dérivées transverses T1, T2, T3, T4

  /////////////////////////////
  // A COMPLETER 
  /////////////////////////////

  // calcul de la surface
  
  /////////////////////////////
  // A COMPLETER 
  /////////////////////////////


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
  
  // les 4 champs de dérivées transverses
  h = 0.5;
  fid = mopen("T1.vect", "w");
  write_VECTS(fid,C1(:,1:2:n),T1(:,1:2:n),[0.5,0.5,0.5],h,1);
  mclose(fid);
  fid = mopen("T2.vect", "w");
  write_VECTS(fid,C2(:,1:2:n),T2(:,1:2:n),[0.5,1.0,0.5],h,1);
  mclose(fid);
  fid = mopen("T3.vect", "w");
  write_VECTS(fid,C3(:,1:2:n),T3(:,1:2:n),[1.0,0.5,0.5],h,1);
  mclose(fid);
  fid = mopen("T4.vect", "w");
  write_VECTS(fid,C4(:,1:2:n),T4(:,1:2:n),[0.5,0.5,1.0],h,1);
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

  
// tracé d'une surface [x,y,z] en filaire couleur dans une fenetre avec un titre
function trace_surface(x,y,z,titre,couleur)
  
// tracé de la surface
fB = scf();
fB.axes_size = [500,500];
fB.figure_name = titre;
aB = fB.children;
aB.view="3d";
//aB.data_bounds = [0,0,0;1,1,1];
aB.margins=[0,0,0,0];
aB.tight_limits="on";

// la surface 
mesh(x,y,z,"EdgeColor",couleur);
h2=get("hdl");
h2.color_mode=0;

aB.isoview = "on";
aB.auto_scale="off";
aB.cube_scaling="off";
aB.axes_visible=["on","on","on"];
aB.box="off";
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

// objet VECT : 1 poyligne avec n sommets et 1 couleur
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


