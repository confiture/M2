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
		}
	}
	/* fermeture */
	fclose(ifp);
}

image::image(const image & im){
	this->buffer = new int[im.hauteur*im.largeur];
	this->hauteur=im.hauteur;
	this->largeur=im.largeur;
	this->valmax=im.valmax;

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			(*this)(i,j)=im(i,j);
		}
	}
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
			os<<(*this)(i,j);
			os<<endl;
		}
	}
	fb.close();
	return 1;

}

int image::seuiller(int seuil){
	if(valmax>=seuil){valmax=255;}
	else{valmax=0;}

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if((*this)(i,j)<seuil){(*this)(i,j)=0;}
			else{(*this)(i,j)=255;}
		}
	}

	return 1;
}

int image::negatif(){
	int max=-1;
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			(*this)(i,j)=valmax-(*this)(i,j);
			if((*this)(i,j)>max)max=(*this)(i,j);
		}
	}

	valmax=max;
	return 1;
}



int** image::composante_connnex(int conn)const{
	int id=0;
	int** corres=new int*[hauteur];//l'image des étiquettes
	for(int i=0;i<hauteur;i++){
		corres[i]=new int[largeur];
		for(int j=0;j<largeur;j++)corres[i][j]=-1;
	}

	std::vector<int> equiv;


	for(int i=0;i<hauteur;i++)corres[i]=0;

	if(4==4){
		for(int i=0;i<hauteur-1;i++){
			for(int j=0;i<largeur-1;j++){
				if((*this)(i,j)==255){
					//on regarde le voisin à droite et en dessous s'il fait partie d'une composante connexe
					if(corres[i][j+1]!=-1){//à droite
						if(corres[i][j]==-1){corres[i][j]=corres[i][j+1];}
						else{updateEquiv(equiv,id,corres[i][j],corres[i][j+1]);}
					}
					if(corres[i+1][j]!=-1){//en dessous
						if(corres[i][j]==-1){corres[i][j]=corres[i+1][j];}
						else{updateEquiv(equiv,id,corres[i][j],corres[i+1][j]);}
					}
					else{
						equiv.push_back(id);
						corres[i][j]=id;
						id++;
					}
				}
			}
		}

		int j=largeur-1;
		for(int i=0;i<hauteur-1;i++){
			if((*this)(i,j)==255){
				//on regarde le voisin à droite et en dessous s'il fait partie d'une composante connexe
				if(corres[i+1][j]!=-1){//en dessous
					if(corres[i][j]==-1){corres[i][j]=corres[i+1][j];}
					else{updateEquiv(equiv,id,corres[i][j],corres[i+1][j]);}
				}
				else{
					equiv.push_back(id);
					corres[i][j]=id;
					id++;
				}
			}
		}

		int i=hauteur-1;
		for(int j=0;j<largeur-1;i++){
			if((*this)(i,j)==255){
				if(corres[i][j+1]!=-1){//en dessous
						if(corres[i][j]==-1){corres[i][j]=corres[i][j+1];}
						else{updateEquiv(equiv,id,corres[i][j],corres[i][j+1]);}
					}
					else{
						equiv.push_back(id);
						corres[i][j]=id;
						id++;
					} 
			}
		}
		
		if(corres[hauteur-1][largeur-1]==-1)corres[hauteur-1][largeur-1]=id;
	}
	
	//mise à jour des groupes grâce au tableau des équivalences
	for(int i=0;i<hauteur;i++){
	  for(int j=0;j<largeur;j++){
	    corres[i][j]=equiv[corres[i][j]]+1; // +1 pour rammener -1 à 0
	  }
	}
	
	return corres;
}


void image::dispCompConn(char* fic)const{
 image sortie(hauteur,largeur,255);
 image im_s(*this);
 im_s.seuiller(100);
 int** conn=im_s.composante_connnex(4);
 
 for(int i=0;i<hauteur;i++){
   for(int j=0;j<largeur;j++){
      if(conn[i][j]!=0){
	sortie(i,j)=255;
      }
      else{
	sortie(i,j)=0;
      }
   }
 }
}

