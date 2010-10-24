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

int** composante_connnex(int conn){
	int id_comp=0;
	int** corres=new int*[hauteur];//le tableau des correspondances
	for(int i=0;i<hauteur;i++){
		corres=new int[largeur];
		for(int j=0;j<largeur;j++)corres[i][j]=0;
	}


	for(int i=0;i<hauteur;i++)corres[i]=0;
	id_comp++;

	if(d==4){
		for(int i=0;i<hauteur-1;i++){
			for(int j=0;i<largeur-1;j++){
				//on regarde le voisin à droite et en dessous s'il fait partie d'une composante connexe
				if(corres[i][j+1]!=0){//à droite
					if(corres[i][j]==0){corres[i][j]=corres[i][j+1];}
					else if(corres[i][j]<corres[i][j+1]){corres[i][j]=corres[i][j+1];}
					else{corres[i][j+1]=corres[i][j];}
				}
				if(corres[i+1][j]!=0){//en dessous
					if(corres[i][j]==0){corres[i][j]=corres[i+1][j];}
					else if(corres[i][j]<corres[i+1][j]){corres[i+1][j]=corres[i][j+1];}
					else{corres[i][j+1]=corres[i][j];}
				}
				else{

				}
			}
		}
	}

}



