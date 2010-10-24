#include "image.hpp"

using namespace std;

int& image::operator()(int i,int j){
  return buffer[i*largeur+j];
}

int image::operator()(int i,int j)const{
  return buffer[i*largeur+j];
}

image::image(int largeur, int hauteur,int valmax){
  
}

image::image(char* nomFichier){
	/* Ouverture */
	FILE* ifp = fopen(nomFichier,"r");
	
	/* Lecture du Magic number */
	int ich1 = getc( ifp );
	if ( ich1 == EOF )
		pm_erreur( "EOF / read error reading magic number" );
	int ich2 = getc( ifp );
	if ( ich2 == EOF )
		pm_erreur( "EOF / read error reading magic number" );
	if(ich2 != '2'){
		pm_erreur(" wrong ifp format ");
		exit(-1);
	}
	
		
	/*Lecture des dimensions*/
	largeur = pm_getint( ifp );
	hauteur = pm_getint( ifp );
	valmax = pm_getint( ifp );
	
	buffer = new int[hauteur*largeur];
	
	/*Lecture*/
	for(int i=0; i < hauteur; i++){
		for(int j=0; j < largeur ; j++){
			(*this)(i,j)=pm_getint(ifp);
			//cerr<<buffer[i*largeur+j];
			//exit(-1);
		}
	}
	/* fermeture */
		fclose(ifp);
}
  
int image::EcrireImagePGM(char* nomFichier)const{
  /* Ouverture */
  filebuf fb;
  fb.open(nomFichier,ios::out);
  ostream os(&fb);

  /* Ecriture */
os<<"P2"<<endl;
os<<largeur<<" "<<hauteur<<endl;
os<<valmax<<endl;

for(int i=0; i < hauteur; i++){
  for(int j=0; j < largeur ; j++){
    cerr<<(i,j)<<" ";
   // exit(-1);
    os<<(*this)(i,j);
    os<<endl;
  }
}
  fb.close();
return 1;

}
