//////////////////////////////////////////////////////////////////////////////
// entrée d'un ensemble de points à la souris, le nombre de points devant 
// entre supérieur ou égal à nbpts_mini (par défaut 1)
// en sortie, le tableau X avec p points du plan (dimensions 2 x p)
function [L] = inputpoly()
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
  end
  L(1:2,i)=X(:,i)
end;
endfunction


//////////////////////////////////////////////////////////////////////////////
// entr�e d'un 
// en sortie, le tableau X avec p points du plan (dimensions 2 x p)
function [L] = inputpoly_rat()
L=[]
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
  end
  L(1:2,i)=X(:,i)
end;

//les poids en
for i=1:size(L,2)
  mprintf("poids points numéro %i : ",i)
  L(3,i)=mscanf("%f");
end
endfunction


function ecrit_poly_rat(nom_f,L)
  fid = mopen(nom_f, 'w');
  mfprintf(fid,"%d\n",size(L,2))
  
  for i=1:size(L,2)
    mfprintf(fid,"%f %f %f ",L(1,i),L(2,i),L(3,i))
  end
  
  mclose(fid)
endfunction

function creer_fig_b()
  f = figure();
  set(f,"background",8); // fond en blanc
  set(f,"axes_size",[500 500]);
  a=gca();
  a.isoview="on"
  set(a,"auto_clear","off")
  set(a,"data_bounds",[0,0;100,100])      // bornes des axes en x et y
  set(a,"margins",[0.05,0.05,0.05,0.05])  // marges du repère dans la fenetre
  set(a,"axes_visible",["on","on","on"])  // afficher les axes
  set(a,"box","on")                       
  set(a,"auto_scale","off")                    
endfunction

function creer_fig_iso()
  f = figure();
  set(f,"background",8); // fond en blanc
  set(f,"axes_size",[500 500]);
  a=gca();
  a.isoview="on"
  set(a,"margins",[0.05,0.05,0.05,0.05])  // marges du repère dans la fenetre
  set(a,"axes_visible",["on","on","on"])  // afficher les axes
  set(a,"box","on")                       
endfunction  