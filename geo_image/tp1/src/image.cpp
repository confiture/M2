#include "image.hpp"

using namespace std;

int& image::operator()(int i,int j){
	return buffer[i*largeur+j];
}

int image::operator()(int i,int j)const{
	return buffer[i*largeur+j];
}

image::image(int hauteur, int largeur,int valmax){
	this->hauteur=hauteur;
	this->largeur=largeur;
	this->valmax=valmax;
	this->buffer=new int[hauteur*largeur];
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

	if(4==4){
		for(int i=0;i<hauteur-1;i++){
			for(int j=0;j<largeur-1;j++){
				if((*this)(i,j)==255){
					//voisins noirs
					if((*this)(i+1,j)==0 && (*this)(i,j+1)==0){
						if(corres[i][j]==-1){
							corres[i][j]=id;
							equiv.push_back(id);
							id++;
						}
					}
					//un des voisins n'est pas noir et appartient à un groupe
					else if(corres[i+1][j]!=-1 || corres[i][j+1]!=-1){
						if(corres[i+1][j]!=-1){
							if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j]];}
							else{
								updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j]]);
							}
						}
						if(corres[i][j+1]!=-1){
							if(corres[i][j]==-1){corres[i][j]=equiv[corres[i][j+1]];}
							else{
								updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i][j+1]]);
							}
						}
					}
					//aucun n'appartient à un groupe
					else{
						//si le point courant ne fait pas partie d'un groupe on le crée
						if(corres[i][j]==-1){
							corres[i][j]=id;
							equiv.push_back(id);
							id++;
						}

						if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
						if((*this)(i,j+1)!=0){corres[i][j+1]=equiv[corres[i][j]];}
					}
				}
			}
		}


		//on traite la colonne sur la droite
		int j=largeur-1;
		for(int i=0;i<hauteur-1;i++){
			if((*this)(i,j)==255){
				//voisin d'en bas noir
				if((*this)(i+1,j)==0){
					if(corres[i][j]==-1){
						corres[i][j]=id;
						equiv.push_back(id);
						id++;
					}
				}
				//le voisin d'en bas n'est pas noir et appartient à un groupe
				else if(corres[i+1][j]!=-1){
					if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j]];}
					else{
						updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j]]);
					}
				}
				//le voisin d'en bas n'appartient pas à un groupe
				else{
					//si le point courant ne fait pas partie d'un groupe on le crée
					if(corres[i][j]==-1){
						corres[i][j]=id;
						equiv.push_back(id);
						id++;
					}

					if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
				}
			}
		}


		//on traite la dernière ligne
		int i=hauteur-1;
		for(int j=0;j<largeur-1;j++){
			if((*this)(i,j)==255){
				//voisin de droite est noir
				if((*this)(i,j+1)==0){
					if(corres[i][j]==-1){
						corres[i][j]=id;
						equiv.push_back(id);
						id++;
					}
				}
				//le voisin de droite n'est pas noir et appartient à un groupe
				else if(corres[i][j+1]!=-1){
					if(corres[i][j]==-1){corres[i][j]=equiv[corres[i][j+1]];}
					else{
						updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i][j+1]]);
					}
				}
				//le voisin de droite n'appartient pas à un groupe
				else{
					//si le point courant ne fait pas partie d'un groupe on le crée
					if(corres[i][j]==-1){
						corres[i][j]=id;
						equiv.push_back(id);
						id++;
					}

					if((*this)(i,j+1)!=0){corres[i][j+1]=equiv[corres[i][j]];}
				}
			}
		}
	}

	//mise à jour des groupes grâce au tableau des équivalences
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			//cout<<equiv[corres[i][j]]<<endl;
			if(corres[i][j]!=-1){corres[i][j]=equiv[corres[i][j]]+1;} // +1 pour rammener -1 à 0
			else{corres[i][j]=0;}
		}
	}

	return corres;
}


void image::dispCompConn(char* fic)const{
	set<int> groupes;
	image sortie(hauteur,largeur,255);
	image im_s(*this);
	im_s.seuiller(50);

	im_s.EcrireImagePGM("seuillage.pgm");

	int** conn=im_s.composante_connnex(4);

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			/*
			if(conn[i][j]!=0){
				sortie(i,j)=255;
			}
			else{
				sortie(i,j)=0;
			}
			*/

			if(conn[i][j]!=0){sortie(i,j)=100+conn[i][j];groupes.insert(conn[i][j]);}
			else{sortie(i,j)=0;}
			//std::cout<<conn[i][j]<<endl;
		}
	}

	sortie.EcrireImagePGM(fic);
	cout<<"nombre d'objet "<<groupes.size()<<endl;
}



image image::duplique_elemStruc_bord(image elem_struct) const{
    image sortie(hauteur+2*elem_struct.hauteur,largeur+2*elem_struct.largeur,0);
    
    // On se place sur la premiere ligne de l'image courante
    for(int k=0;k<(*this).hauteur;k++){
      // On traite le 1er bord (bord gauche de l'image)
	  for(int i=elem_struct.hauteur;i<sortie.hauteur-elem_struct.hauteur;i++){
		for(int j=0;j<elem_struct.largeur;j++){
		      sortie(i,j) = (*this)(k,0);
		}
	  }
    // On traite le 2�me bord (bord droite de l'image)
	  for(int i=elem_struct.hauteur;i<sortie.hauteur-elem_struct.hauteur;i++){
		for(int j=sortie.largeur-elem_struct.largeur-1;j<sortie.largeur;j++){
		      sortie(i,j) = (*this)(k,hauteur-1);
		}
	  }
    }
 
 // On traite le 3 �me bord (bord du haut de l'image)
    for(int i=0;i<elem_struct.hauteur;i++){
	  for(int j=0;j<sortie.largeur;j++){
		     sortie(i,j) = sortie(elem_struct.hauteur,j);
	  }
    }
    
 // On traite le 4 �me bord (bord du bas de l'image)
    for(int i=sortie.hauteur-elem_struct.hauteur;i<sortie.hauteur;i++){
	  for(int j=0;j<sortie.largeur;j++){
		     sortie(i,j) = sortie(sortie.hauteur-elem_struct.hauteur,j);
	  }
    }
    return sortie;
}


